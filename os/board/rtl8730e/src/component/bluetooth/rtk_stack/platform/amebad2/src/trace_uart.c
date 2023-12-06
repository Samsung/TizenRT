/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "trace_uart.h"
#include "hci_dbg.h"
#include "hci_platform.h"

bool bt_trace_init(void)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		printf("bt_trace_init: TRACE LOG OPEN\r\n");
		hci_platform_bt_log_init();
		hci_platform_bt_trace_log_open();
	}

	return true;
}

bool bt_trace_deinit(void)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		hci_platform_bt_trace_log_close();
		hci_platform_bt_log_deinit();
	}

	return true;
}

bool trace_print(void *data, uint16_t len)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		LOGUART_BT_SendData(data, len);
	}

	return true;
}
