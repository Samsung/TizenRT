/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include <atcmd_service.h>
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include <rtk_bt_common.h>
#include <bt_utils.h>
#include <rtk_bt_def.h>


static uint8_t ctoi(char c)
{
	if ((c >= 'A') && (c <= 'F')) {
		return (c - 'A' + 0x0A);
	}

	if ((c >= 'a') && (c <= 'f')) {
		return (c - 'a' + 0x0A);
	}

	if ((c >= '0') && (c <= '9')) {
		return (c - '0' + 0x00);
	}

	BT_LOGE("[%s]Error: Hex char is invalid!!!\r\n", __func__);
	return 0xFF;
}

/* hexnum means string with 0x or 0X */
static int hexnum_str_to_int(char *str)
{
	uint32_t str_len = strlen(str);
	int result = 0;
	uint32_t n = 2;

	if ((str_len < 3) || (str[0] != '0') || ((str[1] != 'x') && (str[1] != 'X'))) {
		BT_LOGE("[%s]Error: Hexnum is not begin with 0x or 0X!!!\r\n", __func__);
		return -1;
	}
	while (n < str_len) {
		result = (result << 4) | (ctoi(str[n++]));
	}
	return result;
}

/* parse string to integer, if string begin with 0x or 0X, treat as hex,
   else treat as decimal */
int str_to_int(char *str)
{
	bool is_hex = FALSE;
	uint32_t str_len = strlen(str);
	uint32_t i;

	if ((str_len > 2) && ('0' == str[0]) && ('x' == str[1] || 'X' == str[1])) {
		is_hex = TRUE;
	}
	if (is_hex) {
		return hexnum_str_to_int(str);
	} else {
		if (!isdigit((int)str[0]) && str[0] != '-') {
			BT_LOGE("[%s]Error: Invalid decimal char\r\n", __func__);
			return -1;
		}
		for (i = 1; i < str_len; i++) {
			if (!isdigit((int)str[i])) {
				BT_LOGE("[%s]Error: Invalid decimal char\r\n", __func__);
				return -1;
			}
		}
		return atoi(str);
	}
}

/* hexdata means string without 0x or 0X, the addr 123456789a transformed to addr array
    is arr[]: {0x9a, 0x78, 0x56, 0x34, 0x12} */
bool hexdata_str_to_bd_addr(char *str, uint8_t *addr_buf, uint8_t buf_len)
{
	uint32_t str_len = strlen(str);
	uint32_t n = 0;
	uint8_t num = 0;

	if (str_len != 2 * 6 || buf_len < 6) {
		BT_LOGE("[%s]Error: Invalid bd addr string\r\n", __func__);
		return FALSE;
	}

	addr_buf += str_len / 2 - 1;

	while (n < str_len) {
		if ((num = ctoi(str[n++])) == 0xFF) {
			return FALSE;
		}
		*addr_buf = num << 4;
		if ((num = ctoi(str[n++])) == 0xFF) {
			return FALSE;
		}
		*addr_buf |= num;
		addr_buf--;
	}
	return TRUE;
}

/* This is little endian transform, The hex string is considered as a long number begin with 0x,
    e.g. string:"0x123456789a" transformed to array is arr[]: {0x9a, 0x78, 0x56, 0x34, 0x12} */
bool hexnum_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len)
{
	char *str_low = NULL;
	uint32_t str_len = strlen(str);
	uint32_t hex_str_len = 0;
	uint8_t byte_high = 0, byte_low = 0;
	uint32_t n = 0;

	if ((str_len < 3) || (str[0] != '0') || ((str[1] != 'x') && (str[1] != 'X'))) {
		BT_LOGE("[%s]Error: Hexnum is not begin with 0x or 0X!!!\r\n", __func__);
		return FALSE;
	}

	str_low = str + str_len - 1;
	hex_str_len = str_len - 2;
	if (arr_len < (hex_str_len + 1) / 2) {
		return FALSE;
	}

	memset(byte_arr, 0, arr_len);
	while (n <= (hex_str_len + 1) / 2 - 1) {
		byte_low = ctoi(*(str_low--));

		/* deal with the highest byte, it may be 1_hex_str(e.g. 0xF), or 2_hex_str(e.g. 0xFF) */
		if ((n == ((hex_str_len + 1) / 2 - 1)) && (hex_str_len % 2)) {
			byte_high = 0;
		} else {
			byte_high = ctoi(*(str_low--));
		}

		if (byte_low == 0xFF || byte_high == 0xFF) {
			return FALSE;
		}
		byte_arr[n] = byte_high << 4 | byte_low;
		n++;
	}

	return TRUE;
}

/* The hex str is not considered as a number, just regared as a list of byte data,
    e.g. string: "112233445566778899aabb" transformed to arrary is as
    arr[]: {0x11, 0x22, 0x33, 0x44, 0x55, ...}  */
bool hexdata_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len)
{
	uint32_t str_len = strlen(str);
	char *str_ptr = str;
	uint32_t n = 0;
	uint8_t byte_high = 0, byte_low = 0;

	if (str_len % 2 || arr_len < str_len / 2) {
		BT_LOGE("[%s]Error: Hexdata is invalid\r\n", __func__);
		return FALSE;
	}

	while (n < str_len) {
		byte_high = ctoi(*(str_ptr++));
		byte_low = ctoi(*(str_ptr++));
		if (byte_high == 0xFF || byte_low == 0xFF) {
			return FALSE;
		}
		byte_arr[n / 2] = byte_high << 4 | byte_low;
		n += 2;
	}

	return TRUE;
}

bool array_to_little_endian(uint8_t *data, size_t length)
{
	if (!data) {
		BT_LOGE("[%s]Error: array is invalid\r\n", __func__);
		return FALSE;
	}

	for (size_t i = 0; i < length / 2; ++i) {
		uint8_t temp = data[i];
		data[i] = data[length - 1 - i];
		data[length - 1 - i] = temp;
	}

	return TRUE;
}

uint16_t bt_at_rtk_err_to_at_err(uint16_t rtk_err)
{
	uint16_t at_sync_err;
	switch (rtk_err) {
	case RTK_BT_OK:
		at_sync_err = BT_AT_OK;
		break;
	case RTK_BT_FAIL:
		at_sync_err = BT_AT_FAIL;
		break;
	case RTK_BT_ERR_NOT_READY:
		at_sync_err = BT_AT_ERR_NOT_READY;
		break;
	case RTK_BT_ERR_NO_MEMORY:
		at_sync_err = BT_AT_ERR_NO_MEMORY;
		break;
	case RTK_BT_ERR_OS_OPERATION:
		at_sync_err = BT_AT_ERR_OS_OPERATION;
		break;
	case RTK_BT_ERR_PARAM_INVALID:
		at_sync_err = BT_AT_ERR_PARAM_INVALID;
		break;
	case RTK_BT_ERR_ADV_LENGTH_INVALID:
		at_sync_err = BT_AT_ERR_ADV_LENGTH_INVALID;
		break;
	case RTK_BT_ERR_NO_CONNECTION:
		at_sync_err = BT_AT_ERR_NO_CONNECTION;
		break;
	case RTK_BT_ERR_IRK_NOT_FOUND:
		at_sync_err = BT_AT_ERR_IRK_NOT_FOUND;
		break;
	case RTK_BT_ERR_NO_BOND:
		at_sync_err = BT_AT_ERR_NO_BOND;
		break;
	case RTK_BT_ERR_LOWER_STACK_API:
	case RTK_BT_ERR_LOWER_STACK_CB:
		at_sync_err = BT_AT_ERR_LOWER_STACK;
		break;
	case RTK_BT_ERR_CREATE_CONN_TIMEOUT:
		at_sync_err = BT_AT_ERR_CREATE_CONN_TIMEOUT;
		break;
	default:
		at_sync_err = BT_AT_FAIL;
		break;
	}

	return at_sync_err;
}

#if defined(CONFIG_ATCMD_HOST_CONTROL) && CONFIG_ATCMD_HOST_CONTROL

void bt_at_iouart_dump_hex(const char *start_str, void *buf, uint16_t len, bool reverse, const char *end_str)
{
	int i = 0;

	if (!buf || !len) {
		// print "\r\n" or ""
		at_printf("%s", end_str);
		return;
	}

	at_printf("%s", start_str);
	for (i = 0; i < len; i++) {
		if (reverse) {
			at_printf("%02x", *((uint8_t *)(buf) + len - 1 - i));
		} else {
			at_printf("%02x", *((uint8_t *)(buf) + i));
		}
	}
	at_printf("%s", end_str);
}

void bt_at_iouart_dump(uint8_t unit, const char *str, void *buf, uint16_t len)
{
	int i = 0;

	if (!buf || !len) {
		return;
	}

	at_printf("%s%s", str, ",");
	for (i = 0; i < len; i++) {
		if (unit == 4) {
			at_printf("%08x", LE_TO_U32((uint8_t *)buf + i * 4)); /* *(buf + i) may crash at AmebaLite when (buf + i) isn't aligned with 4.*/
		} else if (unit == 2) {
			at_printf("%04x", LE_TO_U16((uint8_t *)buf + i * 2));
		} else {
			at_printf("%02x", *((uint8_t *)buf + i));
		}
	}
	at_printf("\r\n");
}
#endif