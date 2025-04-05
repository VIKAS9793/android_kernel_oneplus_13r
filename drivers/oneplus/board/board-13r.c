// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2024 OnePlus, Inc.
 * Author: VIKAS9793 <vikassahani17@gmail.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/pinctrl/consumer.h>
#include <linux/regulator/consumer.h>

/* Device specific constants */
#define ONEPLUS_13R_DEVICE_ID "13r"
#define ONEPLUS_13R_VENDOR_ID "oneplus"

/* Board specific data */
struct oneplus_13r_board {
    struct device *dev;
    struct pinctrl *pinctrl;
    struct regulator *vdd_supply;
};

static int oneplus_13r_probe(struct platform_device *pdev)
{
    struct oneplus_13r_board *board;
    int ret;

    board = devm_kzalloc(&pdev->dev, sizeof(*board), GFP_KERNEL);
    if (!board)
        return -ENOMEM;

    board->dev = &pdev->dev;

    /* Get pinctrl handle */
    board->pinctrl = devm_pinctrl_get(board->dev);
    if (IS_ERR(board->pinctrl)) {
        dev_err(board->dev, "Failed to get pinctrl\n");
        return PTR_ERR(board->pinctrl);
    }

    /* Get VDD regulator */
    board->vdd_supply = devm_regulator_get(board->dev, "vdd");
    if (IS_ERR(board->vdd_supply)) {
        dev_err(board->dev, "Failed to get vdd regulator\n");
        return PTR_ERR(board->vdd_supply);
    }

    /* Enable VDD regulator */
    ret = regulator_enable(board->vdd_supply);
    if (ret) {
        dev_err(board->dev, "Failed to enable vdd regulator\n");
        return ret;
    }

    platform_set_drvdata(pdev, board);

    dev_info(board->dev, "OnePlus 13R board initialized\n");

    return 0;
}

static int oneplus_13r_remove(struct platform_device *pdev)
{
    struct oneplus_13r_board *board = platform_get_drvdata(pdev);

    if (board->vdd_supply)
        regulator_disable(board->vdd_supply);

    return 0;
}

static const struct of_device_id oneplus_13r_of_match[] = {
    { .compatible = "oneplus,13r" },
    { }
};
MODULE_DEVICE_TABLE(of, oneplus_13r_of_match);

static struct platform_driver oneplus_13r_driver = {
    .probe = oneplus_13r_probe,
    .remove = oneplus_13r_remove,
    .driver = {
        .name = "oneplus-13r",
        .of_match_table = oneplus_13r_of_match,
    },
};

module_platform_driver(oneplus_13r_driver);

MODULE_AUTHOR("VIKAS9793 <vikassahani17@gmail.com>");
MODULE_DESCRIPTION("OnePlus 13R Board Driver");
MODULE_LICENSE("GPL v2"); 