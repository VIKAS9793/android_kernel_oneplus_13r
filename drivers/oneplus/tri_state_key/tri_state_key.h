#ifndef _TRI_STATE_KEY_H_
#define _TRI_STATE_KEY_H_

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

/* Key positions */
#define POSITION_TOP      0
#define POSITION_MIDDLE   1
#define POSITION_BOTTOM   2

/* GPIO states */
#define GPIO_LOW  0
#define GPIO_HIGH 1

/* Workqueue delay in milliseconds */
#define WORK_DELAY_MS 50

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
    bool enabled;
};

/* Function prototypes */
void tri_state_key_enable_irq(struct tristate_data *data);
void tri_state_key_disable_irq(struct tristate_data *data);
void tri_state_key_hw_reset(struct tristate_data *data);
int tri_state_key_get_position(struct tristate_data *data);

#endif /* _TRI_STATE_KEY_H_ */ 