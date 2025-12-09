// Copyright 2020-2025 Beken
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

#include <stdarg.h>
#include <common/bk_err.h>
#include <os/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RESET_SOURCE_POWERON                    = 0x0,
    RESET_SOURCE_REBOOT                     = 0x1,
    RESET_SOURCE_WATCHDOG                   = 0x2,

    RESET_SOURCE_DEEPPS_GPIO                = 0x3,
    RESET_SOURCE_DEEPPS_RTC                 = 0x4,
    RESET_SOURCE_DEEPPS_USB                 = 0x5,
    RESET_SOURCE_DEEPPS_TOUCH               = 0x6,

    RESET_SOURCE_CRASH_ILLEGAL_JUMP         = 0x7,
    RESET_SOURCE_CRASH_UNDEFINED            = 0x8,
    RESET_SOURCE_CRASH_PREFETCH_ABORT       = 0x9,
    RESET_SOURCE_CRASH_DATA_ABORT           = 0xa,
    RESET_SOURCE_CRASH_UNUSED               = 0xb,
    RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION  = 0xc,
    RESET_SOURCE_CRASH_MISALIGNED           = 0xd,
    RESET_SOURCE_CRASH_ASSERT               = 0xe,

    RESET_SOURCE_SUPER_DEEP                 = 0xf,

    RESET_SOURCE_NMI_WDT                    = 0x10,
    RESET_SOURCE_HARD_FAULT                 = 0x11,
    RESET_SOURCE_MPU_FAULT                  = 0x12,
    RESET_SOURCE_BUS_FAULT                  = 0x13,
    RESET_SOURCE_USAGE_FAULT                = 0x14,
    RESET_SOURCE_SECURE_FAULT               = 0x15,
    RESET_SOURCE_DEBUG_MONITOR_FAULT        = 0x16,
    RESET_SOURCE_DEFAULT_EXCEPTION          = 0x17,
    RESET_SOURCE_OTA_REBOOT                 = 0x18,
    RESET_SOURCE_BROWN_OUT                  = 0x19,

    RESET_SOURCE_BOOTLOADER_REBOOT          = 0x1a,
    RESET_SOURCE_BOOTLOADER_NMI_WDT         = 0x1b,
    RESET_SOURCE_BOOTLOADER_MPU_FAULT       = 0x1c,
    RESET_SOURCE_BOOTLOADER_BUS_FAULT       = 0x1d,
    RESET_SOURCE_BOOTLOADER_HARD_FAULT      = 0x1e,
    RESET_SOURCE_BOOTLOADER_UNKNOWN         = 0x1f,

    RESET_SOURCE_FASTBOOT_POWER_ON          = 0x20,

    RESET_SOURCE_UNKNOWN = 0xff,
} RESET_SOURCE_STATUS;

typedef enum {
	MAC_TYPE_BASE = 0,
	MAC_TYPE_STA,
	MAC_TYPE_AP,
	MAC_TYPE_BLUETOOTH,
	MAC_TYPE_ETH,
	MAC_MAX,
} mac_type_t;

#define BK_ERR_INVALID_MAC_TYPE   (BK_ERR_MAC_BASE)
#define BK_ERR_ZERO_MAC           (BK_ERR_MAC_BASE - 1)
#define BK_ERR_GROUP_MAC          (BK_ERR_MAC_BASE - 2)
#define BK_ERR_INVALID_MAC        (BK_ERR_MAC_BASE - 3)

#define BK_MAC_ADDR_LEN           6

#define BK_PRINT_UART_ISR_DEFAULT_PRIORITY       7

#define BK_IS_ZERO_MAC(m) (((m)[0] == 0) && ((m)[1] == 0) && ((m)[2] == 0) && ((m)[3] == 0)\
	&& ((m)[4] == 0) && ((m)[5] == 0))
#define BK_IS_GROUP_MAC(m) (((m)[0] & 0x01))

#define rtos_get_ms_per_tick()               bk_get_ms_per_tick()
#define rtos_get_tick_per_second()           (1000/rtos_get_ms_per_tick())

#define TICK_PER_SECOND       rtos_get_tick_per_second()

#define BK_MS_TO_TICKS(x)     ((x) / (rtos_get_ms_per_tick()))
#define BK_TICKS_TO_MS(x)     ((x) * (rtos_get_ms_per_tick()))

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
int bk_get_tick_timer_id(void);
void bk_printf(const char *fmt, ...);
void bk_dump_printf(const char *fmt, ...);
void bk_null_printf(const char *fmt, ...);
int bk_printf_init(void);
int bk_printf_deinit(void);
uint8_t bk_get_printf_enable(void);
void bk_set_printf_enable(uint8_t enable);
void bk_set_printf_sync(uint8_t enable);
int bk_get_printf_sync(void);

/*  ========= !! NOTE !! =========  */
/*          Obsoleted  API          */
/* use bk_printf_ext(...) instead.  */
void bk_buf_printf_sync(char *buf, int buf_len);  /* Obsoleted  API */

/*  ========= !! NOTE !! =========  */
/*          Obsoleted  API          */
/* use bk_printf_ext(...) instead.  */
void bk_printf_ex(int level, char * tag, const char *fmt, ...);  /* Obsoleted  API */

void bk_printf_ext(int level, char * tag, const char *fmt, ...);
void bk_printf_nonblock(int level, char * tag, const char *fmt, ...);
void bk_printf_static_nonblock(int level, char *tag, const char *fmt, ...);
void bk_printf_static_block(int level, char *tag, const char *fmt, ...);
void bk_vprintf_ext(int level, char *tag, const char *fmt, va_list args);
void bk_printf_raw(int level, char * tag, const char *fmt, ...);
void bk_printf_raw_nonblock(int level, char * tag, const char *fmt, ...);
void bk_vprintf_raw(int level, char *tag, const char *fmt, va_list args);

void bk_ble_printf_ext(int level, char *tag, const char *fmt, ...);
void bk_ble_printf_raw(int level, char *tag, const char *fmt, ...);
void bk_wifi_printf_ext(int level, char *tag, const char *fmt, ...);
void bk_wifi_printf_raw(int level, char *tag, const char *fmt, ...);

void bk_set_ble_log_level(uint8_t level);
void bk_set_wifi_log_level(uint8_t level);

void bk_disable_mod_printf(char *mod_name, uint8_t disable);
char * bk_get_disable_mod(int * idx);
void bk_set_printf_port(uint8_t port_num);
int bk_get_printf_port(void);
uint32_t bk_misc_get_reset_reason(void);
void bk_misc_set_reset_reason(uint32_t type);
uint32_t aon_pmu_hal_get_reset_reason(void);

#if (CONFIG_CPU_CNT > 1)
//This callback function can be registered in cpu1, and will be called when cpu0 stops cpu1 
//Note: As the callback is called in mailbox ISR, remember to keep it simplify 
typedef bk_err_t (*stop_cpu1_notification)(void *);
void stop_cpu1_register_notification(stop_cpu1_notification notification, void *param);
void stop_cpu1_unregister_notification(stop_cpu1_notification notification);
void stop_cpu1_handle_notifications();

#if (CONFIG_CPU_CNT > 2)
//which user is working in CPU2,it should vote to start/stop CPU2
typedef enum{
	CPU2_USER_USB = 0,
	CPU2_USER_JPEG_SW_DEC,
	CPU2_USER_SW_ROTATE,
	CPU2_USER_AUD_ASR,
	CPU2_USER_MAX = 31,	//if the user id > 31, please help care about s_mutex_cpu2_users
}cpu2_user_id_t;

/**
 * @brief	  Vote to start CPU2 CORE
 *
 * This API vote to start CPU2 CORE
 *
 * @attention
 *
 * @return
 *	  - == user_id: The value is equal input user_id means user_id is the first owner to start CPU2 core
 *	    -1: The user_id isn't the first owner to start CPU2 core.
 */
int32_t vote_start_cpu2_core(cpu2_user_id_t user_id);

/**
 * @brief	  Vote to stop CPU2 CORE
 *
 * This API vote to stop CPU2 CORE
 *
 * @attention
 *
 * @return
 *	  - user_id: The value is equal input user_id means user_id is the last owner to stop CPU2 core
 *	    -1: The user_id isn't the last owner to stop CPU2 CORE
 */
int32_t vote_stop_cpu2_core(cpu2_user_id_t user_id);
#endif

#endif


#ifdef __cplusplus
}
#endif
