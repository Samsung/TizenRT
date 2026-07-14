/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include "trace_uart.h"
#include "bt_debug.h"
#include "ameba_soc.h"
#include "osif.h"

static void *trace_mutex = NULL;
bool bt_trace_init(void)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		osif_mutex_create(&trace_mutex);
		rtk_bt_trace_log_open();
		BT_LOGA("bt_trace_init: TRACE LOG OPEN\r\n");
	}

	return true;
}

bool bt_trace_deinit(void)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		/* Keep loguart channel always on for coex log. */
		// rtk_bt_trace_log_close();
		osif_mutex_delete(trace_mutex);
		trace_mutex = NULL;
	}
	return true;
}

bool trace_print(void *data, uint16_t len)
{
	if (!CHECK_CFG_SW(CFG_SW_BT_TRACE_LOG)) {
		if (trace_mutex) {
			osif_mutex_take(trace_mutex, BT_TIMEOUT_FOREVER);
		}
		LOGUART_BT_SendData(data, len);
		if (trace_mutex) {
			osif_mutex_give(trace_mutex);
		}
	}

	return true;
}
