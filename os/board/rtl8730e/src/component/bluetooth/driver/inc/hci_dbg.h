/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef _HCI_DBG_H_
#define _HCI_DBG_H_

#include "hci_common.h"

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

enum hci_dbg_sw {
	HCI_TP_DEBUG_ERROR,
	HCI_TP_DEBUG_WARN,
	HCI_TP_DEBUG_INFO,
	HCI_TP_DEBUG_DEBUG,
	HCI_TP_DEBUG_HCI_UART_TX,
	HCI_TP_DEBUG_HCI_UART_RX,
	HCI_TP_DEBUG_HCI_UART_RX_IDX,
	HCI_TP_DEBUG_DOWNLOAD_PATCH,
	HCI_TP_DEBUG_HCI_STACK_DEBUG,
};

void     hci_dbg_set_level(uint32_t level);
uint32_t hci_dbg_get_level(void);

#define H_BIT(x)           (1 << (x))
#define HCI_DEBUG_ALL      (H_BIT(HCI_TP_DEBUG_DEBUG) | \
                            H_BIT(HCI_TP_DEBUG_INFO)  | \
                            H_BIT(HCI_TP_DEBUG_WARN)  | \
                            H_BIT(HCI_TP_DEBUG_ERROR) ) //0xFFFFFFFF
#define CHECK_DBG_SW(x)    (hci_dbg_get_level() & H_BIT(x))

#define HCI_ASSERT(...) \
    do                  \
    {                   \
    } while (0)

#define HCI_PRINT(fmt, ...)         \
    do                              \
    {                               \
        printf(fmt, ##__VA_ARGS__); \
    } while (0)

#define HCI_ERR(fmt, ...)                                                        \
    do                                                                           \
    {                                                                            \
        printf("%s:%d(err) " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define HCI_DBG(fmt, ...)                                                            \
    do                                                                               \
    {                                                                                \
        if (CHECK_DBG_SW(HCI_TP_DEBUG_DEBUG))                                        \
            printf("%s:%d(dbg) " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define HCI_INFO(fmt, ...)                                                            \
    do                                                                                \
    {                                                                                 \
        if (CHECK_DBG_SW(HCI_TP_DEBUG_INFO))                                          \
            printf("%s:%d(info) " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define HCI_WARN(fmt, ...)                                                            \
    do                                                                                \
    {                                                                                 \
        if (CHECK_DBG_SW(HCI_TP_DEBUG_WARN))                                          \
            printf("%s:%d(warn) " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define HCI_STACK_DBG(fmt, ...)                                                            \
    do                                                                                     \
    {                                                                                      \
        if (CHECK_DBG_SW(HCI_TP_DEBUG_HCI_STACK_DEBUG))                                    \
            printf("%s:%d(stack_dbg) " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define HCI_DUMP(hdr, hdr_len, data, data_len)             \
    do                                                     \
    {                                                      \
        int i = 0;                                         \
        printf("%s:%d(dump)\r\n", __FUNCTION__, __LINE__); \
        printf("%02x ", hdr);                              \
        while(i < data_len) {                              \
            printf("%02x ", data[i++]);                    \
            if (i % 16 == 0)                               \
                printf("\r\n");                            \
        }                                                  \
        printf("\r\n");                                    \
    } while (0)

#endif
