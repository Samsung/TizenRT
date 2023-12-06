/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include <platform_stdlib.h>
#include <stdbool.h>
#include "ameba_soc.h"
#include "hci_dbg.h"
#include "osif.h"

static uint32_t hci_log_level = 0x1FF;

void hci_dbg_set_level(uint32_t level)
{
	/* uint32_t >= HCI_TP_DEBUG_ERROR */
	if(level > HCI_TP_DEBUG_HCI_STACK_DEBUG)
		hci_log_level = 0x1FF;
	else
		hci_log_level = level;
}

uint32_t hci_dbg_get_level(void)
{
	return hci_log_level;
}

/**
 * @brief  shift bit mask.
 * @param  Mask[in]: shift bit.
 * @return value.
 */
static uint32_t cal_bit_shift(uint32_t Mask)
{
	uint32_t i;
	for (i = 0; i < 31; i++) {
		if (((Mask >> i) & 0x1) == 1) {
			break;
		}
	}
	return (i);
}

/**
 * @brief  set reg value.
 * @param  reg_address[in]: base address.
 * @param  Mask[in]: bit.
 * @param  val[in]: value.
 * @return none.
 */
static void set_reg_value(uint32_t reg_address, uint32_t Mask, uint32_t val)
{
	uint32_t shift = 0;
	uint32_t data = 0;
	data = HAL_READ32(reg_address, 0);
	shift = cal_bit_shift(Mask);
	data = ((data & (~Mask)) | (val << shift));
	HAL_WRITE32(reg_address, 0, data);
	data = HAL_READ32(reg_address, 0);
}

void hci_debug_enable(void)
{
	uint32_t data;

	/* keep bt wakeup from dlps/lps */
	set_reg_value(0x42008250, BIT14, 1);		//enable HOST_WAKE_BT No GPIO
	osif_delay(5);
	set_reg_value(0x42008250, BIT13, 1);		//HOST_WAKE_BT
	osif_delay(5);
	while (1) {
		osif_delay(100);
		data = HAL_READ32(0x42008254, 0) & 0x1F; // 0x42008254 [0:4]
		if (data == 4) {
			/* bt active */
			HCI_DBG("bt active");
			break;
		} else {
			HCI_DBG("wait for bt wakeup from sleep");
		}
	}
	HCI_DBG("BT wakeup from sleep");

	set_reg_value(0x42008BF8, BIT0, 0);										//swd close
	osif_delay(5);
	set_reg_value(0x42008A08, BIT0 | BIT1 | BIT2 | BIT3 | BIT4, 24);		//Uart tx PA2
	osif_delay(5);
	set_reg_value(0x42008A0C, BIT0 | BIT1 | BIT2 | BIT3 | BIT4, 24);		//Uart rx PA3
	osif_delay(5);
	set_reg_value(0x42008A10, BIT0 | BIT1 | BIT2 | BIT3 | BIT4, 24);		//RTS PA4
	osif_delay(5);
	set_reg_value(0x42008A14, BIT0 | BIT1 | BIT2 | BIT3 | BIT4, 24);		//CTS PA5
	osif_delay(5);
	set_reg_value(0x42008250, BIT19, 1);									//pinmux
	osif_delay(5);
}
