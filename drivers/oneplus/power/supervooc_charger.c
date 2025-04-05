#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/regulator/consumer.h>

#define DRIVER_NAME "supervooc-charger"
#define VOOC_MCU_RESET_GPIO "vooc,mcu_reset_gpio"
#define VOOC_MCU_EN_GPIO "vooc,mcu_en_gpio"
#define VOOC_FW_CHECK_GPIO "vooc,fw_check_gpio"

struct supervooc_data {
    struct device *dev;
    struct power_supply *usb_psy;
    struct power_supply *batt_psy;
    struct power_supply_desc psy_desc;
    struct power_supply *vooc_psy;
    struct delayed_work vooc_monitor_work;
    struct regulator *vdd_reg;
    int mcu_reset_gpio;
    int mcu_en_gpio;
    int fw_check_gpio;
    bool vooc_charging;
    int vooc_current;
    int vooc_voltage;
};

static enum power_supply_property supervooc_props[] = {
    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_CURRENT_NOW,
    POWER_SUPPLY_PROP_VOLTAGE_NOW,
};

static int supervooc_get_property(struct power_supply *psy,
                                enum power_supply_property psp,
                                union power_supply_propval *val)
{
    struct supervooc_data *data = power_supply_get_drvdata(psy);

    switch (psp) {
    case POWER_SUPPLY_PROP_ONLINE:
        val->intval = data->vooc_charging;
        break;
    case POWER_SUPPLY_PROP_CURRENT_NOW:
        val->intval = data->vooc_current;
        break;
    case POWER_SUPPLY_PROP_VOLTAGE_NOW:
        val->intval = data->vooc_voltage;
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

static void supervooc_monitor_work(struct work_struct *work)
{
    struct supervooc_data *data = container_of(work, struct supervooc_data,
                                             vooc_monitor_work.work);
    union power_supply_propval val;
    int ret;

    // Check USB presence
    ret = power_supply_get_property(data->usb_psy,
                                  POWER_SUPPLY_PROP_PRESENT, &val);
    if (ret) {
        dev_err(data->dev, "Failed to get USB present status\n");
        goto reschedule;
    }

    if (!val.intval) {
        data->vooc_charging = false;
        goto reschedule;
    }

    // Check battery voltage and temperature
    ret = power_supply_get_property(data->batt_psy,
                                  POWER_SUPPLY_PROP_VOLTAGE_NOW, &val);
    if (ret) {
        dev_err(data->dev, "Failed to get battery voltage\n");
        goto reschedule;
    }

    data->vooc_voltage = val.intval;

    // VOOC charging logic
    if (data->vooc_voltage < 4200000) {  // 4.2V
        data->vooc_current = 6000000;     // 6A
        data->vooc_charging = true;
        gpio_set_value(data->mcu_en_gpio, 1);
    } else {
        data->vooc_current = 1000000;     // 1A
        data->vooc_charging = false;
        gpio_set_value(data->mcu_en_gpio, 0);
    }

    power_supply_changed(data->vooc_psy);

reschedule:
    schedule_delayed_work(&data->vooc_monitor_work,
                         msecs_to_jiffies(1000));  // 1s interval
}

static int supervooc_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct supervooc_data *data;
    struct power_supply_config psy_cfg = {};
    int ret;

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    data->dev = dev;
    platform_set_drvdata(pdev, data);

    // Get GPIOs
    data->mcu_reset_gpio = of_get_named_gpio(dev->of_node,
                                           VOOC_MCU_RESET_GPIO, 0);
    data->mcu_en_gpio = of_get_named_gpio(dev->of_node,
                                        VOOC_MCU_EN_GPIO, 0);
    data->fw_check_gpio = of_get_named_gpio(dev->of_node,
                                          VOOC_FW_CHECK_GPIO, 0);

    if (!gpio_is_valid(data->mcu_reset_gpio) ||
        !gpio_is_valid(data->mcu_en_gpio) ||
        !gpio_is_valid(data->fw_check_gpio)) {
        dev_err(dev, "Invalid GPIO\n");
        return -EINVAL;
    }

    ret = devm_gpio_request_one(dev, data->mcu_reset_gpio,
                               GPIOF_OUT_INIT_LOW, "vooc_mcu_reset");
    if (ret) {
        dev_err(dev, "Failed to request MCU reset GPIO\n");
        return ret;
    }

    ret = devm_gpio_request_one(dev, data->mcu_en_gpio,
                               GPIOF_OUT_INIT_LOW, "vooc_mcu_en");
    if (ret) {
        dev_err(dev, "Failed to request MCU enable GPIO\n");
        return ret;
    }

    ret = devm_gpio_request_one(dev, data->fw_check_gpio,
                               GPIOF_IN, "vooc_fw_check");
    if (ret) {
        dev_err(dev, "Failed to request FW check GPIO\n");
        return ret;
    }

    // Get power supplies
    data->usb_psy = power_supply_get_by_name("usb");
    if (!data->usb_psy) {
        dev_err(dev, "USB supply not found\n");
        return -EPROBE_DEFER;
    }

    data->batt_psy = power_supply_get_by_name("battery");
    if (!data->batt_psy) {
        dev_err(dev, "Battery supply not found\n");
        return -EPROBE_DEFER;
    }

    // Register VOOC power supply
    data->psy_desc.name = "vooc";
    data->psy_desc.type = POWER_SUPPLY_TYPE_UNKNOWN;
    data->psy_desc.properties = supervooc_props;
    data->psy_desc.num_properties = ARRAY_SIZE(supervooc_props);
    data->psy_desc.get_property = supervooc_get_property;

    psy_cfg.drv_data = data;
    psy_cfg.of_node = dev->of_node;

    data->vooc_psy = devm_power_supply_register(dev, &data->psy_desc, &psy_cfg);
    if (IS_ERR(data->vooc_psy)) {
        dev_err(dev, "Failed to register VOOC power supply\n");
        return PTR_ERR(data->vooc_psy);
    }

    // Initialize work
    INIT_DELAYED_WORK(&data->vooc_monitor_work, supervooc_monitor_work);
    schedule_delayed_work(&data->vooc_monitor_work, 0);

    return 0;
}

static int supervooc_remove(struct platform_device *pdev)
{
    struct supervooc_data *data = platform_get_drvdata(pdev);
    cancel_delayed_work_sync(&data->vooc_monitor_work);
    return 0;
}

static const struct of_device_id supervooc_of_match[] = {
    { .compatible = "oneplus,supervooc-charger", },
    { },
};
MODULE_DEVICE_TABLE(of, supervooc_of_match);

static struct platform_driver supervooc_driver = {
    .probe = supervooc_probe,
    .remove = supervooc_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = supervooc_of_match,
    },
};

module_platform_driver(supervooc_driver);

MODULE_DESCRIPTION("OnePlus SUPERVOOC Fast Charging Driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("OnePlus"); 