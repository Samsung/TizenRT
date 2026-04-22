/*
 * Copyright (c) 2025 Realtek Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BT_DEBUG_H_
#define _BT_DEBUG_H_

#include <diag.h>
#include <log.h>
#include <osif.h>
#ifdef CONFIG_PLATFORM_TIZENRT_OS
#include <debug.h>
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

#define BT_LOG_USE_MUTEX 0

#if defined(BT_LOG_USE_MUTEX) && BT_LOG_USE_MUTEX
extern void *bt_log_mtx;
#define BT_LOG_MUTEX_TAKE   {if (bt_log_mtx) {osif_mutex_take(bt_log_mtx, BT_TIMEOUT_FOREVER);}}
#define BT_LOG_MUTEX_GIVE   {if (bt_log_mtx) {osif_mutex_give(bt_log_mtx);}}
#else
#define BT_LOG_MUTEX_TAKE {}
#define BT_LOG_MUTEX_GIVE {}
#endif


#define BT_LOG_LEVEL_OFF      0
#define BT_LOG_LEVEL_ALWAYS   1
#define BT_LOG_LEVEL_ERROR    2
#define BT_LOG_LEVEL_DEBUG    3

#define BT_LOG_DEBUG_LEVEL  BT_LOG_LEVEL_ERROR
#define BT_LOG_PRINT(level,...)     \
    do {\
        if (level <= BT_LOG_DEBUG_LEVEL) {\
            BT_LOG_MUTEX_TAKE \
            RTK_LOGA(NOTAG, __VA_ARGS__); \
            BT_LOG_MUTEX_GIVE \
        }\
    }while(0)

#define BT_LOG_PRINTS(level,...)     \
    do {\
        if (level <= BT_LOG_DEBUG_LEVEL) {\
            BT_LOG_MUTEX_TAKE \
            RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, __VA_ARGS__); \
            BT_LOG_MUTEX_GIVE \
        }\
    }while(0)

#define BT_BUF_PRINT(level, _str, _buf, _len)     \
    do {\
        if (level <= BT_LOG_DEBUG_LEVEL) {\
            BT_LOG_MUTEX_TAKE   \
            RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%s\r\n", _str);\
            rtk_log_memory_dump_byte(_buf, _len);\
            BT_LOG_MUTEX_GIVE   \
        }\
    }while(0)

#ifdef CONFIG_PLATFORM_TIZENRT_OS
void rtk_tizenrt_bt_log_level(uint8_t level);
void rtk_tizenrt_bt_print(uint8_t level, const char *fmt, ...);
#define BT_LOGA(fmt,...)  rtk_tizenrt_bt_print(BT_LOG_LEVEL_ALWAYS, fmt, ##__VA_ARGS__)
#define BT_LOGE(fmt,...)  rtk_tizenrt_bt_print(BT_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define BT_LOGD(fmt,...)  rtk_tizenrt_bt_print(BT_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define BT_LOGA(fmt,...)  BT_LOG_PRINTS(BT_LOG_LEVEL_ALWAYS, fmt, ##__VA_ARGS__)
#define BT_LOGE(fmt,...)  BT_LOG_PRINTS(BT_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define BT_LOGD(fmt,...)  BT_LOG_PRINTS(BT_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

/* [60344458] 01 02 0a 0b ...*/
#define BT_DUMPD(_str, _buf, _len) BT_BUF_PRINT(BT_LOG_LEVEL_DEBUG, _str, _buf, _len)
/* 01 02 0a 0b ...*/
#define BT_DUMPA(_str, _buf, _len) rtk_bt_log_dump(1, _str, _buf, _len)
/* 0201 0b0a ...*/
#define BT_DUMP16A(_str, _buf, _len) rtk_bt_log_dump(2, _str, _buf, _len)
/* 04030201 0d0c0b0a ...*/
#define BT_DUMP32A(_str, _buf, _len) rtk_bt_log_dump(4, _str, _buf, _len)

/* if _inverse is true, dump from high address to low address. otherwise dump from low address to high address */
#define BT_DUMPHEXA(_str, _buf, _len, _inverse) rtk_bt_log_dumphex(_str, _buf, _len, _inverse)


#define CFG_SW_USE_FLASH_PATCH   BIT0
#define CFG_SW_BT_FW_LOG         BIT1
#define CFG_SW_RSVD1             BIT2
#define CFG_SW_RSVD2             BIT3
#define CFG_SW_UPPERSTACK_SWITCH BIT4
#define CFG_SW_BT_TRACE_LOG      BIT5
#define CFG_SW_DRIVER_DEBUG_LOG  BIT6
#define CFG_SW_RSVD3             BIT7

extern uint32_t hci_cfg_sw_val;
#define CHECK_CFG_SW(x)            (hci_cfg_sw_val & x)

void rtk_bt_log_init(void);
void rtk_bt_log_deinit(void);
void rtk_bt_log_dump(uint8_t unit, const char *str, void *buf, uint16_t len);
void rtk_bt_log_dumphex(const char *str, void *buf, uint16_t len, bool reverse);

void rtk_bt_fw_log_open(void);
void rtk_bt_fw_log_close(void);
void rtk_bt_trace_log_open(void);
void rtk_bt_trace_log_close(void);

#endif /* _BT_DEBUG_H_ */
