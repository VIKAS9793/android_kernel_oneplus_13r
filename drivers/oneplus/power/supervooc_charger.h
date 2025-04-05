#ifndef _SUPERVOOC_CHARGER_H_
#define _SUPERVOOC_CHARGER_H_

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

/* GPIO definitions */
#define VOOC_MCU_RESET_GPIO "vooc,mcu_reset_gpio"
#define VOOC_MCU_EN_GPIO "vooc,mcu_en_gpio"
#define VOOC_FW_CHECK_GPIO "vooc,fw_check_gpio"

/* VOOC charging parameters */
#define VOOC_MAX_CURRENT_MA    6000
#define VOOC_FAST_CURRENT_MA   4000
#define VOOC_NORMAL_CURRENT_MA 2000
#define VOOC_MIN_CURRENT_MA    1000

#define VOOC_MAX_VOLTAGE_MV    4400
#define VOOC_FAST_VOLTAGE_MV   4200
#define VOOC_NORMAL_VOLTAGE_MV 4000

/* Temperature thresholds in tenths of degrees Celsius */
#define VOOC_MAX_TEMP     450  /* 45.0°C */
#define VOOC_MIN_TEMP     50   /* 5.0°C */
#define VOOC_NORMAL_TEMP  250  /* 25.0°C */

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
    int battery_temp;
    bool temp_normal;
};

/* Function prototypes */
int supervooc_init_gpio(struct supervooc_data *data);
void supervooc_enable_charging(struct supervooc_data *data);
void supervooc_disable_charging(struct supervooc_data *data);
int supervooc_get_charging_status(struct supervooc_data *data);
int supervooc_set_charging_current(struct supervooc_data *data, int current_ma);
int supervooc_get_battery_temperature(struct supervooc_data *data);
bool supervooc_is_temp_normal(struct supervooc_data *data);

#endif /* _SUPERVOOC_CHARGER_H_ */ 