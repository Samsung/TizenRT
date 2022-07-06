/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include "hci_dbg.h"

static uint32_t hci_log_level = 0x1FF;

void hci_dbg_printf(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
}

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
