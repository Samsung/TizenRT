/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __ATCMD_BT_UTILS_H__
#define __ATCMD_BT_UTILS_H__

#include <basic_types.h>
#include <stdint.h>
#include <debug.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define debug_print blevdbg

#ifndef HI_WORD
#define HI_WORD(x)      ((uint8_t)((x & 0xFF00) >> 8))
#endif

#ifndef LO_WORD
#define LO_WORD(x)      ((uint8_t)(x))
#endif

#ifndef UUID128_STR
#define UUID128_STR "%08lx-%04x-%04x-%04x-%04x%08lx"
#endif

#ifndef UUID128_VAL
#define UUID128_VAL(uuid128) \
    *(uint32_t*)(uuid128+12), *(uint16_t*)(uuid128+10), \
    *(uint16_t*)(uuid128+8), *(uint16_t*)(uuid128+6), \
    *(uint16_t*)(uuid128+4), *(uint32_t*)uuid128
#endif

#define APP_PRINT_SEPARATOR()                                                                \
    do {                                                                                    \
        printf("############################################################\r\n \r\n");    \
    } while(0)                                                                             \

#define APP_PROMOTE(...)                                                                \
    do {                                                                                \
        printf("\r\n");                                                                 \
        printf("############################################################\r\n");     \
        printf(__VA_ARGS__);                                                            \
        printf("############################################################\r\n");     \
        printf("\r\n");                                                                 \
    } while (0)

void ble_addr_to_str(void *paddr, char *str, uint32_t len);

int str_to_int(char *str);

int hexnum_str_to_int(char *str);

bool hexdata_str_to_bd_addr(char *str, uint8_t *addr_buf, uint8_t buf_len);

bool hexnum_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

bool hexdata_str_to_array(char *str, uint8_t *byte_arr, uint8_t arr_len);

#ifdef __cplusplus
}
#endif

#endif /* __ATCMD_BT_UTILS_H__ */