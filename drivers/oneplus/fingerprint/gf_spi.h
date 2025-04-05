#ifndef _GF_SPI_H_
#define _GF_SPI_H_

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

/* GPIO definitions */
#define GF_IRQ_GPIO "fp,gpio_irq"
#define GF_RESET_GPIO "fp,gpio_reset"
#define GF_POWER_GPIO "fp,gpio_power"

/* SPI parameters */
#define GF_SPI_SPEED_HZ 19200000
#define GF_SPI_BITS_PER_WORD 8
#define GF_SPI_MODE SPI_MODE_0

/* Power control */
#define GF_VDD_MIN_UV 2800000
#define GF_VDD_MAX_UV 3300000
#define GF_VIO_MIN_UV 1800000
#define GF_VIO_MAX_UV 1800000

/* Interrupt parameters */
#define GF_INT_DETECT_PERIOD 10
#define GF_INT_DETECT_THRESHOLD 10

/* Input device parameters */
#define GF_INPUT_NAME "gf_input"
#define GF_INPUT_FF_MAX 0x7f
#define GF_INPUT_FF_MIN 0

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
    bool irq_enabled;
    bool power_on;
    struct mutex lock;
    struct mutex buf_lock;
    unsigned users;
};

/* Function prototypes */
int gf_parse_dts(struct gf_dev *gf_dev);
int gf_power_on(struct gf_dev *gf_dev);
int gf_power_off(struct gf_dev *gf_dev);
int gf_hw_reset(struct gf_dev *gf_dev, unsigned int delay_ms);
void gf_enable_irq(struct gf_dev *gf_dev);
void gf_disable_irq(struct gf_dev *gf_dev);
int gf_init_input_device(struct gf_dev *gf_dev);
int gf_init_spi(struct gf_dev *gf_dev);

/* SPI data transfer */
int gf_spi_read_bytes(struct gf_dev *gf_dev, u16 addr, u32 data_len, u8 *rx_buf);
int gf_spi_write_bytes(struct gf_dev *gf_dev, u16 addr, u32 data_len, u8 *tx_buf);

#endif /* _GF_SPI_H_ */ 