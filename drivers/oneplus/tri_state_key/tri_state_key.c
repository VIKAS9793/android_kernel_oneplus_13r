#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

#define DRIVER_NAME "tri-state-key"

struct tristate_data {
    struct device *dev;
    struct input_dev *input;
    struct work_struct work;
    struct workqueue_struct *workqueue;
    int key1_gpio;
    int key2_gpio;
    int key3_gpio;
    int key1_irq;
    int key2_irq;
    int key3_irq;
    int last_position;
};

static void tri_state_key_work(struct work_struct *work)
{
    struct tristate_data *data = container_of(work, struct tristate_data, work);
    int position;
    int key1, key2, key3;

    key1 = gpio_get_value(data->key1_gpio);
    key2 = gpio_get_value(data->key2_gpio);
    key3 = gpio_get_value(data->key3_gpio);

    if (!key1 && !key2 && key3)
        position = 1;      // Silent
    else if (!key1 && key2 && !key3)
        position = 2;      // Vibration
    else if (key1 && !key2 && !key3)
        position = 3;      // Ring
    else
        return;

    if (data->last_position != position) {
        input_report_abs(data->input, ABS_X, position);
        input_sync(data->input);
        data->last_position = position;
    }
}

static irqreturn_t tri_state_key_irq(int irq, void *_dev)
{
    struct tristate_data *data = _dev;
    queue_work(data->workqueue, &data->work);
    return IRQ_HANDLED;
}

static int tri_state_key_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct tristate_data *data;
    struct device_node *np = dev->of_node;
    int error;

    data = devm_kzalloc(dev, sizeof(struct tristate_data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    data->dev = dev;
    platform_set_drvdata(pdev, data);

    // Get GPIOs
    data->key1_gpio = of_get_named_gpio(np, "tristate,gpio_key1", 0);
    data->key2_gpio = of_get_named_gpio(np, "tristate,gpio_key2", 0);
    data->key3_gpio = of_get_named_gpio(np, "tristate,gpio_key3", 0);

    if (!gpio_is_valid(data->key1_gpio) ||
        !gpio_is_valid(data->key2_gpio) ||
        !gpio_is_valid(data->key3_gpio)) {
        dev_err(dev, "Invalid GPIO\n");
        return -EINVAL;
    }

    // Set up input device
    data->input = devm_input_allocate_device(dev);
    if (!data->input)
        return -ENOMEM;

    data->input->name = "OnePlus Alert Slider";
    data->input->dev.parent = dev;
    input_set_abs_params(data->input, ABS_X, 1, 3, 0, 0);
    error = input_register_device(data->input);
    if (error) {
        dev_err(dev, "Failed to register input device\n");
        return error;
    }

    // Set up workqueue
    data->workqueue = create_singlethread_workqueue("tri_state_key_wq");
    if (!data->workqueue) {
        dev_err(dev, "Failed to create workqueue\n");
        return -ENOMEM;
    }
    INIT_WORK(&data->work, tri_state_key_work);

    // Request GPIOs and IRQs
    error = devm_gpio_request_one(dev, data->key1_gpio, GPIOF_IN, "tri_state_key1");
    if (error) {
        dev_err(dev, "Failed to request GPIO1\n");
        goto err_destroy_wq;
    }

    error = devm_gpio_request_one(dev, data->key2_gpio, GPIOF_IN, "tri_state_key2");
    if (error) {
        dev_err(dev, "Failed to request GPIO2\n");
        goto err_destroy_wq;
    }

    error = devm_gpio_request_one(dev, data->key3_gpio, GPIOF_IN, "tri_state_key3");
    if (error) {
        dev_err(dev, "Failed to request GPIO3\n");
        goto err_destroy_wq;
    }

    data->key1_irq = gpio_to_irq(data->key1_gpio);
    data->key2_irq = gpio_to_irq(data->key2_gpio);
    data->key3_irq = gpio_to_irq(data->key3_gpio);

    error = devm_request_irq(dev, data->key1_irq, tri_state_key_irq,
                            IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                            "tri_state_key1", data);
    if (error) {
        dev_err(dev, "Failed to request IRQ1\n");
        goto err_destroy_wq;
    }

    error = devm_request_irq(dev, data->key2_irq, tri_state_key_irq,
                            IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                            "tri_state_key2", data);
    if (error) {
        dev_err(dev, "Failed to request IRQ2\n");
        goto err_destroy_wq;
    }

    error = devm_request_irq(dev, data->key3_irq, tri_state_key_irq,
                            IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
                            "tri_state_key3", data);
    if (error) {
        dev_err(dev, "Failed to request IRQ3\n");
        goto err_destroy_wq;
    }

    // Get initial position
    queue_work(data->workqueue, &data->work);

    return 0;

err_destroy_wq:
    destroy_workqueue(data->workqueue);
    return error;
}

static int tri_state_key_remove(struct platform_device *pdev)
{
    struct tristate_data *data = platform_get_drvdata(pdev);
    destroy_workqueue(data->workqueue);
    return 0;
}

static const struct of_device_id tri_state_key_of_match[] = {
    { .compatible = "oneplus,tri-state-key", },
    { },
};
MODULE_DEVICE_TABLE(of, tri_state_key_of_match);

static struct platform_driver tri_state_key_driver = {
    .probe = tri_state_key_probe,
    .remove = tri_state_key_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = tri_state_key_of_match,
    },
};

module_platform_driver(tri_state_key_driver);

MODULE_DESCRIPTION("OnePlus Alert Slider Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("OnePlus"); 