/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <basic_types.h>
#include "ameba.h"
#include <bt_debug.h>

#define _LE_TO_U32(_a)                                  \
        (((uint32_t)(*((uint8_t *)(_a) + 0)) << 0)  |   \
         ((uint32_t)(*((uint8_t *)(_a) + 1)) << 8)  |   \
         ((uint32_t)(*((uint8_t *)(_a) + 2)) << 16) |   \
         ((uint32_t)(*((uint8_t *)(_a) + 3)) << 24))

#define _LE_TO_U16(_a)                                  \
        (((uint16_t)(*((uint8_t *)(_a) + 0)) << 0)  |   \
         ((uint16_t)(*((uint8_t *)(_a) + 1)) << 8))

uint32_t hci_cfg_sw_val = 0xFF;    /* Open BT trace log & FW log use 0xDD
                                      Open BT trace log only use 0xDF
                                      Open BT FW log only use 0xFD */

#if defined(BT_LOG_USE_MUTEX) && BT_LOG_USE_MUTEX
void *bt_log_mtx = NULL;
#endif

void rtk_bt_log_init(void)
{
#if defined(BT_LOG_USE_MUTEX) && BT_LOG_USE_MUTEX
	if (bt_log_mtx == NULL) {
		osif_mutex_create(&bt_log_mtx);
	}
#endif
}

void rtk_bt_log_deinit(void)
{
#if defined(BT_LOG_USE_MUTEX) && BT_LOG_USE_MUTEX
	if (bt_log_mtx) {
		osif_mutex_delete(bt_log_mtx);
		bt_log_mtx = NULL;
	}
#endif
}

void rtk_bt_log_dump(uint8_t unit, const char *str, void *buf, uint16_t len)
{
	int i = 0;
	int num = 16 / unit;

	if (!buf || !len) {
		return;
	}

	BT_LOG_MUTEX_TAKE
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, str);
	for (i = 0; i < len; i++) {
		if (unit == 4) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%08x ", _LE_TO_U32((uint8_t *)buf + i * 4)); /* *(buf + i) may crash at AmebaLite when (buf + i) isn't aligned with 4.*/
		} else if (unit == 2) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%04x ", _LE_TO_U16((uint8_t *)buf + i * 2));
		} else {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%02x ", *((uint8_t *)buf + i));
		}
		if ((i + 1) % num == 0 || (i + 1) == len) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\r\n");
		}
	}
	BT_LOG_MUTEX_GIVE
}

void rtk_bt_log_dumphex(const char *str, void *buf, uint16_t len, bool reverse)
{
	int i = 0;

	if (!buf || !len) {
		return;
	}

	BT_LOG_MUTEX_TAKE
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, str);
	for (i = 0; i < len; i++) {
		if (reverse) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%02x", *((uint8_t *)(buf) + len - 1 - i));
		} else {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%02x", *((uint8_t *)(buf) + i));
		}
	}
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\r\n");
	BT_LOG_MUTEX_GIVE
}

void rtk_bt_fw_log_open(void)
{
#if !(defined(CONFIG_AMEBAPRO3) && CONFIG_AMEBAPRO3) \
    && !(defined(CONFIG_AMEBAD) && CONFIG_AMEBAD)
	LOGUART_Relay_InitTypeDef LOGUART_Relay_InitStruct;

	LOGUART_AGGCmd(LOGUART_DEV, ENABLE);
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_5, ENABLE);

	LOGUART_Relay_StructInit(&LOGUART_Relay_InitStruct);
	LOGUART_Relay_ClearRxFifo(LOGUART_DEV);
	LOGUART_Relay_SetFormat(LOGUART_DEV, &LOGUART_Relay_InitStruct);
	LOGUART_Relay_SetBaud(LOGUART_DEV, 833333);    //If capture FW log before patch download, please change baudrate to 115200.
	LOGUART_Relay_RxCmd(LOGUART_DEV, ENABLE);
#endif
}

void rtk_bt_fw_log_close(void)
{
#if !(defined(CONFIG_AMEBAPRO3) && CONFIG_AMEBAPRO3) \
    && !(defined(CONFIG_AMEBAD) && CONFIG_AMEBAD)
	LOGUART_Relay_RxCmd(LOGUART_DEV, DISABLE);
	LOGUART_WaitTxComplete();
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_5, DISABLE);
#endif
}

void rtk_bt_trace_log_open(void)
{
#if !(defined(CONFIG_AMEBAD) && CONFIG_AMEBAD)
	LOGUART_AGGCmd(LOGUART_DEV, ENABLE);
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_3, ENABLE);
#endif
}

void rtk_bt_trace_log_close(void)
{
#if !(defined(CONFIG_AMEBAD) && CONFIG_AMEBAD)
	LOGUART_WaitTxComplete();
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_3, DISABLE);
#endif
}

#ifdef CONFIG_PLATFORM_TIZENRT_OS
static uint8_t bt_log_level = BT_LOG_LEVEL_ALWAYS;
void rtk_tizenrt_bt_log_level(uint8_t level)
{
	bt_log_level = level;
}

void rtk_tizenrt_bt_print(uint8_t level, const char *fmt, ...)
{
	if (level <= bt_log_level) {
		u8 tmp_buffer[256] = { 0 };
		va_list arglist;

		va_start(arglist, fmt);
		vsnprintf((char *)tmp_buffer, sizeof(tmp_buffer), fmt, arglist);
		va_end(arglist);

		if (osif_task_context_check()) {
			dbg_noarg("%s", tmp_buffer);
		} else {
			lldbg_noarg("%s", tmp_buffer);
		}
	}
}
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS
