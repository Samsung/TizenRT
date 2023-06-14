/*
 * Copyright (c) 2015, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/debug.h>
#include <platform_def.h>
#include <stdint.h>
#include <string.h>
/* SPIC FLASH Headder */
#include "spi-sheipa.h"

static uint32_t fifo_size;
static uint32_t fifo_half_size;
/* This function is used to wait the SSI is not at busy state. */

/* SPIC Driver */
static uint32_t spi_flash_setser(struct sheipa_spi *dev, uint32_t ser_num)
{
	struct spi_flash_portmap *spi_flash_map;

	spi_flash_map = dev->regs;

	/* Disable SPI_FLASH */
	spi_flash_map->ssienr = 0;

	if (ser_num > MAX_SLAVES) {
		return -1;
	} else {
		spi_flash_map->ser = 1 << ser_num;
		return 0;
	}
}

/*
 * This function is used to set the baud rate register.
 */
static uint32_t spi_flash_setbaudr(struct sheipa_spi *dev, uint32_t baudrate)
{
	struct spi_flash_portmap *spi_flash_map;

	spi_flash_map = dev->regs;
	/* Disable SPI_FLASH */
	spi_flash_map->ssienr = 0;

	if ((baudrate & 0xffff0000) || (baudrate == 0)) {
		return -1;
	} else {
		spi_flash_map->baudr = baudrate;
		return 0;
	}
}

/*
 * This function is used to set the fast baud rate register for fast read cmd.
 */
static uint32_t spi_flash_setfbaudr(struct sheipa_spi *dev, uint32_t fbaudrate)
{
	struct spi_flash_portmap *spi_flash_map;

	spi_flash_map = dev->regs;
	/* Disable SPI_FLASH */
	spi_flash_map->ssienr = 0;

	if ((fbaudrate & 0xffff0000) || (fbaudrate == 0)) {
		return -1;
	} else {
		spi_flash_map->fbaudr = fbaudrate;
		return 0;
	}
}

/*
 * This function is used to set the Baudr controller.
 */
static uint32_t spi_flash_set_dummy_cycle(struct sheipa_spi *dev,
					uint32_t dum_cycle)
{
	struct spi_flash_portmap *spi_flash_map;
	uint32_t cycle;

	cycle = 0;
	spi_flash_map = dev->regs;
	/* Disable SPI_FLASH */
	spi_flash_map->ssienr = 0;

	/* if using fast_read baud_rate */
	if (spi_flash_map->ctrlr0 & 0x00100000)
		cycle = spi_flash_map->fbaudr;
	else
		cycle = spi_flash_map->baudr;

	cycle = (cycle*dum_cycle*2) + DEF_RD_TUNING_DUMMY_CYCLE;
	if (cycle > 0x10000)
		return -1;

	DW_BITS_SET_VAL(spi_flash_map->auto_length, BFO_SPI_FLASH_AUTO_LEN_DUM,
		    cycle, BFW_SPI_FLASH_AUTO_LEN_DUM);

	return 0;
}

/* Setting auto mode auto_length reg */
static void set_auto_length(struct sheipa_spi *dev)
{

	struct spi_flash_portmap *spi_flash_map;

	spi_flash_map = (struct spi_flash_portmap *)dev->regs;
	spi_flash_map->ssienr = 0;

	/* setting auto mode CS_H_WR_LEN, address length(3 byte) */
	spi_flash_map->auto_length = ((spi_flash_map->auto_length
						& 0xfffcffff)
						| 0x00030000);
}

/*
 * this function is used to spic and flash initialization.
 */
static void flash_init(struct sheipa_spi *dev,
			struct spi_flash_param *spi_flash_config)
{
	struct spi_flash_portmap *spi_flash_map;
	spi_flash_map = (struct spi_flash_portmap *)dev->regs;

	/* Set baud_rate */
	spi_flash_setbaudr(dev, spi_flash_config->baudr_div);
	/* Set fast baudr rate */
	spi_flash_setfbaudr(dev, spi_flash_config->baudr_div);
	/* Set slave(FLASH) num */
	spi_flash_setser(dev, spi_flash_config->num_slaves);
	/* init addr length 3 byte */
	spi_flash_map->addr_length = 3;
}

/* auto mode initial setting */
static void auto_init(struct sheipa_spi *dev)
{
	struct spi_flash_portmap *spi_flash_map;

	spi_flash_map = (struct spi_flash_portmap *)dev->regs;
	set_auto_length(dev);
	/* Disable SPI_FLASH */
	spi_flash_map->ssienr = 0;
	/* Set valid_cmd_reg: auto_cmd and single channel */
	spi_flash_map->valid_cmd = (spi_flash_map->valid_cmd | 0x600);
}

static void flash_read(struct sheipa_spi *dev)
{

	volatile uint32_t *mem = (volatile uint32_t *)(PLAT_SHEIPA_FIP_BASE);
	volatile uint32_t *addr;
	volatile uint32_t *base;
	volatile uint32_t *end;

	base = (volatile uint32_t *)(dev->auto_regs + FLASH_FIP_OFFSET);
	end = (volatile uint32_t *)(dev->auto_regs + FLASH_FIP_OFFSET + FLASH_SIZE);

	/* set read dummy cycle 0 */
	spi_flash_set_dummy_cycle(dev, DEF_RD_TUNING_DUMMY_CYCLE);

	for (addr = base; addr < end; addr++) {
		*mem = *addr;
		mem++;
	}
}

/*
 * A piece of default spi config info unless the platform
 * supplies it.
 */

static const struct spi_flash_param spi_default_config_info = {
	.num_slaves = 0,
	.fifo_size = FIFO_SIZE,
	.baudr_div = 1,
};

int rtk_flash_read(void)
{
	struct sheipa_spi dws;
	struct spi_flash_param spi_flash_config;

	dws.regs = (struct sheipa_spi *)(SPIC_BASE);
	dws.auto_regs = (struct sheipa_spi *)(FLASH_BASE);

	spi_flash_config = spi_default_config_info;
	fifo_size = spi_flash_config.fifo_size;
	fifo_half_size = fifo_size / 2;
	/* user mode init setting */
	flash_init(&dws, &spi_flash_config);
	/* auto mode init setting */
	auto_init(&dws);

	INFO("Read Secure Boot Image\n");
	flash_read(&dws);
	return 0;
}
