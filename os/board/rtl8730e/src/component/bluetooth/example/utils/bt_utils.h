/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __ATCMD_BT_UTILS_H__
#define __ATCMD_BT_UTILS_H__

#include <basic_types.h>
#include <stdint.h>
#include "platform_autoconf.h"

#if defined(CONFIG_ATCMD_HOST_CONTROL) && CONFIG_ATCMD_HOST_CONTROL
#include <atcmd_service.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(CONFIG_ATCMD_HOST_CONTROL) && CONFIG_ATCMD_HOST_CONTROL
#define BT_AT_SYNC      (1)
#endif

#if defined(CONFIG_ATCMD_HOST_CONTROL) && CONFIG_ATCMD_HOST_CONTROL

#define BT_AT_MESH_ROLE_CLIENT              (0)
#define BT_AT_MESH_ROLE_SERVER              (1)

#define BT_AT_PRINT(fmt, args...)             at_printf(fmt, ##args)

#define BT_AT_PRINT_INDICATE(fmt, args...)    at_printf_indicate(fmt, ##args)
/* 0x11223344...\r\n */
#define BT_AT_DUMP_HEXN(buf, len)             bt_at_iouart_dump_hex("0x", buf, len, false, "\r\n")
/* 0x11223344... */
#define BT_AT_DUMP_HEX(buf, len)              bt_at_iouart_dump_hex("0x", buf, len, false, "")
/* 01,02,0a,0b ...*/
#define BT_AT_DUMP(str, buf, len)             bt_at_iouart_dump(1, str, buf, len)
/* 0201,0b0a ...*/
#define BT_AT_DUMP16(str, buf, len)           bt_at_iouart_dump(2, str, buf, len)


#else /* CONFIG_ATCMD_HOST_CONTROL */

#define BT_AT_PRINT(fmt, args...)
#define BT_AT_PRINT_INDICATE(fmt, args...)
#define BT_AT_DUMP_HEXN(buf, len)
#define BT_AT_DUMP_HEX(buf, len)
#define BT_AT_DUMP(str, buf, len)
#define BT_AT_DUMP16(str, buf, len)

#endif /* CONFIG_ATCMD_HOST_CONTROL */

#define BT_AT_PRINTOK()             BT_AT_PRINT("OK\r\n")
#define BT_AT_PRINTERROR(errno)     BT_AT_PRINT("ERROR:%d\r\n", errno)

typedef enum {
	BT_AT_OK                            =   0x00,   /*!< 0, means success */
	BT_AT_FAIL                          =   0x01,   /*!< 1, means common failure */
	BT_AT_ERR_NOT_READY                 =   0x02,   /*!< 2 */
	BT_AT_ERR_NO_MEMORY                 =   0x03,   /*!< 3 */
	BT_AT_ERR_OS_OPERATION              =   0x04,   /*!< 4 */
	BT_AT_ERR_PARAM_INVALID             =   0x05,   /*!< 5 */
	BT_AT_ERR_ADV_LENGTH_INVALID        =   0x06,   /*!< 6 */
	BT_AT_ERR_NO_CONNECTION             =   0x07,   /*!< 7 */
	BT_AT_ERR_IRK_NOT_FOUND             =   0x08,   /*!< 8 */
	BT_AT_ERR_NO_BOND                   =   0x09,   /*!< 9 */
	BT_AT_ERR_LOWER_STACK               =   0x0A,   /*!< 10 */
	BT_AT_ERR_CREATE_CONN_TIMEOUT       =   0x0B,   /*!< 11 */
	BT_AT_ERR_CMD_INVALID               =   0x0C,   /*!< 12 */
	BT_AT_ERR_CMD_TIMEOUT               =   0x0D,   /*!< 13 */
	BT_AT_ERR_NO_SRV_ADD                =   0x0E,   /*!< 14 */
	BT_AT_ERR_CHAR_ADD_EXCEED_MAX_NUM   =   0x0F,   /*!< 15 */
} bt_at_err_t;

void ble_addr_to_str(void *paddr, char *str, uint32_t len);

int str_to_int(char *str);

bool hexdata_str_to_bd_addr(char *str, uint8_t *addr_buf, uint8_t buf_len);

bool hexnum_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

bool hexdata_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

bool array_to_little_endian(uint8_t *data, size_t length);

uint16_t bt_at_rtk_err_to_at_err(uint16_t rtk_err);

#if defined(CONFIG_ATCMD_HOST_CONTROL) && CONFIG_ATCMD_HOST_CONTROL

void bt_at_iouart_dump_hex(const char *start_str, void *buf, uint16_t len, bool reverse, const char *end_str);

void bt_at_iouart_dump(uint8_t unit, const char *str, void *buf, uint16_t len);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __ATCMD_BT_UTILS_H__ */