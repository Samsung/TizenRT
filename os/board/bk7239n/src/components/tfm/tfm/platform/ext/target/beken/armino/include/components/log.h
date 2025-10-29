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
#pragma GCC diagnostic ignored "-Wunused-value"

#ifdef __cplusplus
extern "C" {
#endif

#include <os/os.h>
#include <components/system.h>
#include <driver/efuse.h>
#if CONFIG_STDIO_PRINTF
#include <stdio.h>
#endif
#include "armino_config.h"

#define BK_LOG_NONE    0 /*!< No log output */
#define BK_LOG_ERROR   1 /*!< Critical errors, software module can not recover on its own */
#define BK_LOG_WARN    2 /*!< Error conditions from which recovery measures have been taken */
#define BK_LOG_INFO    3 /*!< Information messages which describe normal flow of events */
#define BK_LOG_DEBUG   4 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
#define BK_LOG_VERBOSE 5 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */

#if CONFIG_STDIO_PRINTF
#define _OS_PRINTF printf
#else
#define _OS_PRINTF os_printf
#endif

#if CFG_LEGACY_LOG

#define BK_LOGE( tag, format, ... ) _OS_PRINTF(format, ##__VA_ARGS__)
#define BK_LOGW( tag, format, ... ) _OS_PRINTF(format, ##__VA_ARGS__)
#define BK_LOGI( tag, format, ... ) _OS_PRINTF(format, ##__VA_ARGS__)
#define BK_LOGD( tag, format, ... ) _OS_PRINTF(format, ##__VA_ARGS__)
#define BK_LOGV( tag, format, ... ) _OS_PRINTF(format, ##__VA_ARGS__)

#else

#if CFG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
//#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
//#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
//#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif //CFG_LOG_COLORS

#ifdef CONFIG_TFM_LOG_LEVEL
#define LOG_LEVEL         CONFIG_TFM_LOG_LEVEL
#else
#define LOG_LEVEL         BK_LOG_INFO
#endif

#ifdef MCUBOOT_LOG_LEVEL
#undef LOG_LEVEL
#define LOG_LEVEL MCUBOOT_LOG_LEVEL
#endif
#define BK_LOGF(tag, format, ...) if (efuse_is_err_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(E, format), 0, tag, ## __VA_ARGS__)
#define BK_LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format  LOG_RESET_COLOR

#if (LOG_LEVEL >= BK_LOG_ERROR)
#define BK_LOGE(tag, format, ...) if (efuse_is_err_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(E, format), 0, tag, ## __VA_ARGS__)
#else
#define BK_LOGE(tag, format, ...) do { (void)sizeof(tag, format, ## __VA_ARGS__); } while (0)
#endif

#if (LOG_LEVEL >= BK_LOG_WARN)
#define BK_LOGW(tag, format, ...) if (efuse_is_err_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(W, format), 0, tag, ## __VA_ARGS__)
#else
#define BK_LOGW(tag, format, ...) do { (void)sizeof(tag, format, ## __VA_ARGS__); } while (0)
#endif

#if (LOG_LEVEL >= BK_LOG_INFO)
#define BK_LOGI(tag, format, ...) if (efuse_is_info_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(I, format), 0, tag, ## __VA_ARGS__)
#else
#define BK_LOGI(tag, format, ...) do { (void)sizeof(tag, format, ## __VA_ARGS__); } while (0)
#endif

#if (LOG_LEVEL >= BK_LOG_DEBUG)
#define BK_LOGD(tag, format, ...) if (efuse_is_info_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(D, format), 0, tag, ## __VA_ARGS__)
#else
#define BK_LOGD(tag, format, ...) do { (void)sizeof(tag, format, ## __VA_ARGS__); } while (0)
#endif

#if (LOG_LEVEL >= BK_LOG_VERBOSE)
#define BK_LOGV(tag, format, ...) if (efuse_is_info_log_enabled()) _OS_PRINTF(BK_LOG_FORMAT(V, format), 0, tag, ## __VA_ARGS__)
#else
#define BK_LOGV(tag, format, ...) do { (void)sizeof(tag, format, ## __VA_ARGS__); } while (0)
#endif

#endif // CFG_LEGACY_LOG

#define BK_LOG_RAW(format, ...) if (efuse_is_info_log_enabled()) _OS_PRINTF(format, ##__VA_ARGS__)

#define BK_IP4_FORMAT "%d.%d.%d.%d"
#define BK_IP4_STR(_ip) ((_ip) & 0xFF), (((_ip) >> 8) & 0xFF), (((_ip) >> 16) & 0xFF), (((_ip) >> 24) & 0xFF)
#define BK_MAC_FORMAT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BK_MAC_STR(_m) (_m)[0], (_m)[1], (_m)[2], (_m)[3], (_m)[4], (_m)[5]
#define BK_MAC_STR_INVERT(_m) (_m)[5], (_m)[4], (_m)[3], (_m)[2], (_m)[1], (_m)[0]

#define BK_U64_FORMAT "%x.%x"
#define BK_U64_TO_U32(x) (((uint32_t)(((x) >> 32) & 0xFFFFFFFF)), ((uint32_t)((x) & 0xFFFFFFFF)))

void bk_mem_dump(const char* titile, uint32_t start, uint32_t len);
#define BK_MEM_DUMP(_title, _start, _len) bk_mem_dump((_title), (_start), (_len))

#if CONFIG_SHELL_ASYNCLOG

#include "components/shell_task.h"

#undef BK_LOG_FORMAT
#undef BK_LOGE
#undef BK_LOGW
#undef BK_LOGI
#undef BK_LOGD
#undef BK_LOGV

#define BK_LOG_FORMAT(letter, format)   #letter "(%d):" format

#define BK_LOGE( tag, format, ... ) bk_printf_ex(BK_LOG_ERROR, tag, tag ":" BK_LOG_FORMAT(E, format), rtos_get_time(),  ##__VA_ARGS__)
#define BK_LOGW( tag, format, ... ) bk_printf_ex(BK_LOG_WARN, tag, tag ":" BK_LOG_FORMAT(W, format), rtos_get_time(),  ##__VA_ARGS__)
#define BK_LOGI( tag, format, ... ) bk_printf_ex(BK_LOG_INFO, tag, tag ":" BK_LOG_FORMAT(I, format), rtos_get_time(),  ##__VA_ARGS__)
#define BK_LOGD( tag, format, ... ) bk_printf_ex(BK_LOG_DEBUG, tag, tag ":" BK_LOG_FORMAT(D, format), rtos_get_time(),  ##__VA_ARGS__)
#define BK_LOGV( tag, format, ... ) bk_printf_ex(BK_LOG_VERBOSE, tag, tag ":" BK_LOG_FORMAT(V, format), rtos_get_time(), ##__VA_ARGS__)

#define BK_SPY( spy_id, byte_array, byte_cnt )		shell_spy_out(spy_id, byte_array, byte_cnt)

#define BK_TRACE(trace_id, ... )	shell_trace_out(trace_id, ##__VA_ARGS__)

#define BK_LOG_FLUSH()				shell_log_flush()

#else

#define BK_SPY( spy_id, byte_array, byte_cnt )
#define BK_TRACE(trace_id, ... )

#define BK_LOG_FLUSH()

#endif

#ifdef __cplusplus
}
#endif

