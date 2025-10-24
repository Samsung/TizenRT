// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RESET_SOURCE_POWERON = 0x0,
    RESET_SOURCE_REBOOT = 0x1,
    RESET_SOURCE_WATCHDOG = 0x2,

    RESET_SOURCE_DEEPPS_GPIO = 0x3,
    RESET_SOURCE_DEEPPS_RTC = 0x4,
    RESET_SOURCE_DEEPPS_USB = 0x5,
    RESET_SOURCE_DEEPPS_TOUCH = 0x6,

    RESET_SOURCE_CRASH_ILLEGAL_JUMP = 0x7,
    RESET_SOURCE_CRASH_UNDEFINED = 0x8,
    RESET_SOURCE_CRASH_PREFETCH_ABORT = 0x9,
    RESET_SOURCE_CRASH_DATA_ABORT = 0xa,
    RESET_SOURCE_CRASH_UNUSED = 0xb,
    RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION  = 0xc,
    RESET_SOURCE_CRASH_MISALIGNED = 0xd,
    RESET_SOURCE_CRASH_ASSERT = 0xe,

    RESET_SOURCE_UNKNOWN = 0xff,
} RESET_SOURCE_STATUS;

typedef enum {
	MAC_TYPE_BASE = 0,
	MAC_TYPE_STA,
	MAC_TYPE_AP,
	MAC_TYPE_BLUETOOTH,
	MAC_MAX,
} mac_type_t;

#define BK_ERR_INVALID_MAC_TYPE   (BK_ERR_MAC_BASE)
#define BK_ERR_ZERO_MAC           (BK_ERR_MAC_BASE - 1)
#define BK_ERR_GROUP_MAC          (BK_ERR_MAC_BASE - 2)
#define BK_ERR_INVALID_MAC        (BK_ERR_MAC_BASE - 3)

#define BK_MAC_ADDR_LEN           6

#define BK_IS_ZERO_MAC(m) (((m)[0] == 0) && ((m)[1] == 0) && ((m)[2] == 0) && ((m)[3] == 0)\
	&& ((m)[4] == 0) && ((m)[5] == 0))
#define BK_IS_GROUP_MAC(m) (((m)[0] & 0x01))

/**
 * @brief  Set the base MAC of the system
 *
 * The base MAC address is used to generate the MAC of other interfaces,
 * such as WiFi STA, WiFi AP, BLE, BT etc.
 *
 * @return
 *  - BK_OK: success
 *  - BK_ERR_NULL_PARAM: mac is NULL
 *  - BK_ERR_GROUP_MAC: mac is group address mac
 *  - others: other failures
 */
bk_err_t bk_set_base_mac(const uint8_t *mac);

/**
 * @brief  Get the MAC
 *
 * @return
 *  - BK_OK: success
 *  - BK_ERR_NULL_PARAM: parameter mac is NULL
 *  - BK_ERR_INVALID_MAC_TYPE: mac is not supported
 */
bk_err_t bk_get_mac(uint8_t *mac, mac_type_t type);


/**
 * @brief  Reboot the system
 *
 * This function reset the system by triggering the interrupt watchdog.
 */
void bk_reboot(void);
void bk_reboot_ex(uint32_t reset_reason);

int bk_tick_init(void);
int bk_tick_reload(uint32_t time_ms);
void bk_tick_handle(uint8_t arg);
int bk_update_tick(uint32_t tick);
uint64_t bk_get_tick(void);
uint32_t bk_get_second(void);
uint32_t bk_get_ms_per_tick(void);
uint32_t bk_get_ticks_per_second(void);
int bk_get_tick_timer_id(void);
void bk_printf(const char *fmt, ...);
void bk_null_printf(const char *fmt, ...);
int bk_printf_init(void);
int bk_printf_deinit(void);
void bk_set_printf_enable(uint8_t enable);
void bk_set_printf_sync(uint8_t enable);
int bk_get_printf_sync(void);
void bk_buf_printf_sync(char *buf, int buf_len);
void bk_printf_ex(int level, char * tag, const char *fmt, ...);
void bk_disable_mod_printf(char *mod_name, uint8_t disable);
char * bk_get_disable_mod(int * idx);
void bk_set_printf_port(uint8_t port_num);
int bk_get_printf_port(void);
uint32_t bk_misc_get_reset_reason(void);
void bk_misc_set_reset_reason(uint32_t type);

#ifdef __cplusplus
}
#endif
