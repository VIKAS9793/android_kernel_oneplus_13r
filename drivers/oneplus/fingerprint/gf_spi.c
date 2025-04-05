#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/regulator/consumer.h>

#define GF_DEV_NAME "goodix_fp"
#define GF_IRQ_GPIO "fp,gpio_irq"
#define GF_RESET_GPIO "fp,gpio_reset"
#define GF_POWER_GPIO "fp,gpio_power"

struct gf_dev {
    struct device *dev;
    struct spi_device *spi;
    struct input_dev *input;
    struct regulator *vdd_reg;
    int irq_gpio;
    int reset_gpio;
    int power_gpio;
    int irq;
    bool enabled;
    struct work_struct work;
    struct workqueue_struct *workqueue;
};

static void gf_enable_irq(struct gf_dev *gf_dev)
{
    if (!gf_dev->enabled) {
        enable_irq(gf_dev->irq);
        gf_dev->enabled = true;
    }
}

static void gf_disable_irq(struct gf_dev *gf_dev)
{
    if (gf_dev->enabled) {
        disable_irq(gf_dev->irq);
        gf_dev->enabled = false;
    }
}

static void gf_hw_reset(struct gf_dev *gf_dev)
{
    gpio_set_value(gf_dev->reset_gpio, 0);
    msleep(10);
    gpio_set_value(gf_dev->reset_gpio, 1);
    msleep(50);
}

static void gf_work(struct work_struct *work)
{
    struct gf_dev *gf_dev = container_of(work, struct gf_dev, work);
    char msg = gpio_get_value(gf_dev->irq_gpio) ? '1' : '0';

    input_report_key(gf_dev->input, KEY_F11, msg == '1');
    input_sync(gf_dev->input);
}

static irqreturn_t gf_irq(int irq, void *handle)
{
    struct gf_dev *gf_dev = handle;
    queue_work(gf_dev->workqueue, &gf_dev->work);
    return IRQ_HANDLED;
}

static int gf_probe(struct spi_device *spi)
{
    struct device *dev = &spi->dev;
    struct gf_dev *gf_dev;
    int ret;

    gf_dev = devm_kzalloc(dev, sizeof(*gf_dev), GFP_KERNEL);
    if (!gf_dev)
        return -ENOMEM;

    gf_dev->dev = dev;
    gf_dev->spi = spi;
    spi_set_drvdata(spi, gf_dev);

    // Get GPIOs
    gf_dev->irq_gpio = of_get_named_gpio(dev->of_node, GF_IRQ_GPIO, 0);
    gf_dev->reset_gpio = of_get_named_gpio(dev->of_node, GF_RESET_GPIO, 0);
    gf_dev->power_gpio = of_get_named_gpio(dev->of_node, GF_POWER_GPIO, 0);

    if (!gpio_is_valid(gf_dev->irq_gpio) ||
        !gpio_is_valid(gf_dev->reset_gpio) ||
        !gpio_is_valid(gf_dev->power_gpio)) {
        dev_err(dev, "Invalid GPIO\n");
        return -EINVAL;
    }

    ret = devm_gpio_request_one(dev, gf_dev->irq_gpio,
                               GPIOF_IN, "gf_irq");
    if (ret) {
        dev_err(dev, "Failed to request IRQ GPIO\n");
        return ret;
    }

    ret = devm_gpio_request_one(dev, gf_dev->reset_gpio,
                               GPIOF_OUT_INIT_LOW, "gf_reset");
    if (ret) {
        dev_err(dev, "Failed to request reset GPIO\n");
        return ret;
    }

    ret = devm_gpio_request_one(dev, gf_dev->power_gpio,
                               GPIOF_OUT_INIT_LOW, "gf_power");
    if (ret) {
        dev_err(dev, "Failed to request power GPIO\n");
        return ret;
    }

    // Set up input device
    gf_dev->input = devm_input_allocate_device(dev);
    if (!gf_dev->input) {
        dev_err(dev, "Failed to allocate input device\n");
        return -ENOMEM;
    }

    gf_dev->input->name = "goodix_fp";
    __set_bit(EV_KEY, gf_dev->input->evbit);
    __set_bit(KEY_F11, gf_dev->input->keybit);

    ret = input_register_device(gf_dev->input);
    if (ret) {
        dev_err(dev, "Failed to register input device\n");
        return ret;
    }

    // Set up workqueue
    gf_dev->workqueue = create_singlethread_workqueue("gf_wq");
    if (!gf_dev->workqueue) {
        dev_err(dev, "Failed to create workqueue\n");
        return -ENOMEM;
    }
    INIT_WORK(&gf_dev->work, gf_work);

    // Set up IRQ
    gf_dev->irq = gpio_to_irq(gf_dev->irq_gpio);
    ret = devm_request_irq(dev, gf_dev->irq, gf_irq,
                          IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                          "gf_irq", gf_dev);
    if (ret) {
        dev_err(dev, "Failed to request IRQ\n");
        goto err_destroy_wq;
    }

    // Power up sequence
    gpio_set_value(gf_dev->power_gpio, 1);
    msleep(10);
    gf_hw_reset(gf_dev);
    gf_enable_irq(gf_dev);

    return 0;

err_destroy_wq:
    destroy_workqueue(gf_dev->workqueue);
    return ret;
}

static int gf_remove(struct spi_device *spi)
{
    struct gf_dev *gf_dev = spi_get_drvdata(spi);
    gf_disable_irq(gf_dev);
    destroy_workqueue(gf_dev->workqueue);
    gpio_set_value(gf_dev->power_gpio, 0);
    return 0;
}

static const struct of_device_id gf_of_match[] = {
    { .compatible = "goodix,fingerprint", },
    { },
};
MODULE_DEVICE_TABLE(of, gf_of_match);

static struct spi_driver gf_driver = {
    .driver = {
        .name = GF_DEV_NAME,
        .of_match_table = gf_of_match,
    },
    .probe = gf_probe,
    .remove = gf_remove,
};

module_spi_driver(gf_driver);

MODULE_DESCRIPTION("Goodix Fingerprint Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("OnePlus"); 