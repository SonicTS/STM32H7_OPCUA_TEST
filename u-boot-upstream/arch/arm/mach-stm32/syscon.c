// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * Emcraft Systems, www.emcraft.com, (C) Copyright 2023
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <syscon.h>
#include <asm/arch/stm32.h>

static int stm32_syscon_probe(struct udevice *dev)
{
	struct clk clk_syscfg;
	int ret;

	ret = clk_get_by_name(dev, "syscfg-clk", &clk_syscfg);
	if (ret) {
		pr_err("clk_get_by_name(syscfg-clk) failed: %d", ret);
		return ret;
	}

	ret = clk_enable(&clk_syscfg);
	if (ret < 0) {
		pr_err("clk_enable(clk_syscfg) failed: %d", ret);
	}

	clk_free(&clk_syscfg);

	return ret;
}

static const struct udevice_id stm32_syscon_ids[] = {
	{ .compatible = "st,stm32-syscfg",
	  .data = STM32_SYSCON_SYSCFG },
	{ }
};

U_BOOT_DRIVER(syscon_stm32) = {
	.name = "stm32_syscon",
	.id = UCLASS_SYSCON,
	.of_match = stm32_syscon_ids,
	.bind = dm_scan_fdt_dev,
	.probe = stm32_syscon_probe,
};
