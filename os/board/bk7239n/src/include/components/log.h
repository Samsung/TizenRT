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

#ifdef __cplusplus
extern "C" {
#endif

#include <os/os.h>
#include <components/system.h>
#ifdef CONFIG_STDIO_PRINTF
#include <stdio.h>
#endif

#define _BK_LOG_PRINTF    bk_printf_ext
#define _BK_RAW_PRINTF    bk_printf_raw


#define BK_LOG_NONE    0 /*!< No log output */
#define BK_LOG_ERROR   1 /*!< Critical errors, software module can not recover on its own */
#define BK_LOG_WARN    2 /*!< Error conditions from which recovery measures have been taken */
#define BK_LOG_INFO    3 /*!< Information messages which describe normal flow of events */
#define BK_LOG_DEBUG   4 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
#define BK_LOG_VERBOSE 5 /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
#define BK_LOG_ALL     6 /*!< ALL LOG */


#ifdef CFG_LOG_LEVEL
#define LOG_LEVEL         CFG_LOG_LEVEL
#else
#define LOG_LEVEL         BK_LOG_INFO
#endif

#if (LOG_LEVEL >= BK_LOG_ERROR)
#define BK_LOGE( tag, format, ... )         _BK_LOG_PRINTF(BK_LOG_ERROR, tag, format,  ##__VA_ARGS__)
#define BK_RAW_LOGE( tag, format, ... )     _BK_RAW_PRINTF(BK_LOG_ERROR, tag, format,  ##__VA_ARGS__)
#else
#define BK_LOGE(tag, format, ...)           (void)(format, ##__VA_ARGS__)
#define BK_RAW_LOGE(tag, format, ...)       (void)(format, ##__VA_ARGS__)
#endif

#if (LOG_LEVEL >= BK_LOG_WARN)
#define BK_LOGW( tag, format, ... )         _BK_LOG_PRINTF(BK_LOG_WARN, tag, format,  ##__VA_ARGS__)
#define BK_RAW_LOGW( tag, format, ... )     _BK_RAW_PRINTF(BK_LOG_WARN, tag, format,  ##__VA_ARGS__)
#else
#define BK_LOGW(tag, format, ...)           do { (void)(tag); (void)(format); } while(0)
#define BK_RAW_LOGW(tag, format, ...)       do { (void)(tag); (void)(format); } while(0)
#endif

#if (LOG_LEVEL >= BK_LOG_INFO)
#define BK_LOGI( tag, format, ... )         _BK_LOG_PRINTF(BK_LOG_INFO, tag, format,  ##__VA_ARGS__)
#define BK_RAW_LOGI( tag, format, ... )     _BK_RAW_PRINTF(BK_LOG_INFO, tag, format,  ##__VA_ARGS__)
#else
#define BK_LOGI(tag, format, ...)           do { (void)(tag); (void)(format); } while(0)
#define BK_RAW_LOGI(tag, format, ...)       do { (void)(tag); (void)(format); } while(0)
#endif

#if (LOG_LEVEL >= BK_LOG_DEBUG)
#define BK_LOGD( tag, format, ... )         _BK_LOG_PRINTF(BK_LOG_DEBUG, tag, format,  ##__VA_ARGS__)
#define BK_RAW_LOGD( tag, format, ... )     _BK_RAW_PRINTF(BK_LOG_DEBUG, tag, format,  ##__VA_ARGS__)
#else
#define BK_LOGD(tag, format, ...)           do { (void)(tag); (void)(format); } while(0)
#define BK_RAW_LOGD(tag, format, ...)       do { (void)(tag); (void)(format); } while(0)
#endif

#if (LOG_LEVEL >= BK_LOG_VERBOSE)
#define BK_LOGV( tag, format, ... )         _BK_LOG_PRINTF(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#define BK_RAW_LOGV( tag, format, ... )     _BK_RAW_PRINTF(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#else
#define BK_LOGV(tag, format, ...)           do { (void)(tag); (void)(format); } while(0)
#define BK_RAW_LOGV(tag, format, ...)       do { (void)(tag); (void)(format); } while(0)
#endif

#define BK_LOG_RAW(format, ...)             _BK_RAW_PRINTF(BK_LOG_NONE, NULL, format, ##__VA_ARGS__)

#define BK_BLE_LOGE( tag, format, ... )      bk_ble_printf_ext(BK_LOG_ERROR, tag, format, ##__VA_ARGS__)
#define BK_BLE_LOGW( tag, format, ... )      bk_ble_printf_ext(BK_LOG_WARN, tag, format, ##__VA_ARGS__)
#define BK_BLE_LOGI( tag, format, ... )      bk_ble_printf_ext(BK_LOG_INFO, tag, format, ##__VA_ARGS__)
#define BK_BLE_RAW_LOGE( tag, format, ... )      bk_ble_printf_raw(BK_LOG_ERROR, tag, format, ##__VA_ARGS__)
#define BK_BLE_RAW_LOGW( tag, format, ... )      bk_ble_printf_raw(BK_LOG_WARN, tag, format, ##__VA_ARGS__)
#define BK_BLE_RAW_LOGI( tag, format, ... )      bk_ble_printf_raw(BK_LOG_INFO, tag, format, ##__VA_ARGS__)
#if defined(x)
#define BK_BLE_LOGD( tag, format, ... )      bk_ble_printf_ext(BK_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define BK_BLE_LOGV( tag, format, ... )      bk_ble_printf_ext(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#define BK_BLE_RAW_LOGD( tag, format, ... )      bk_ble_printf_raw(BK_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define BK_BLE_RAW_LOGV( tag, format, ... )      bk_ble_printf_raw(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#else
#define BK_BLE_LOGD( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_BLE_LOGV( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_BLE_RAW_LOGD( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_BLE_RAW_LOGV( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#endif

#define BK_WIFI_LOGE( tag, format, ... )      bk_wifi_printf_ext(BK_LOG_ERROR, tag, format, ##__VA_ARGS__)
#define BK_WIFI_LOGW( tag, format, ... )      bk_wifi_printf_ext(BK_LOG_WARN, tag, format, ##__VA_ARGS__)
#define BK_WIFI_LOGI( tag, format, ... )      bk_wifi_printf_ext(BK_LOG_INFO, tag, format, ##__VA_ARGS__)
#define BK_WIFI_RAW_LOGE( tag, format, ... )      bk_wifi_printf_raw(BK_LOG_ERROR, tag, format, ##__VA_ARGS__)
#define BK_WIFI_RAW_LOGW( tag, format, ... )      bk_wifi_printf_raw(BK_LOG_WARN, tag, format, ##__VA_ARGS__)
#define BK_WIFI_RAW_LOGI( tag, format, ... )      bk_wifi_printf_raw(BK_LOG_INFO, tag, format, ##__VA_ARGS__)
#if defined(x)
#define BK_WIFI_LOGD( tag, format, ... )      bk_wifi_printf_ext(BK_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define BK_WIFI_LOGV( tag, format, ... )      bk_wifi_printf_ext(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#define BK_WIFI_RAW_LOGD( tag, format, ... )      bk_wifi_printf_raw(BK_LOG_DEBUG, tag, format, ##__VA_ARGS__)
#define BK_WIFI_RAW_LOGV( tag, format, ... )      bk_wifi_printf_raw(BK_LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#else
#define BK_WIFI_LOGD( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_WIFI_LOGV( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_WIFI_RAW_LOGD( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#define BK_WIFI_RAW_LOGV( tag, format, ... )      do { (void)(tag); (void)(format); } while(0)
#endif

#define BK_IP4_FORMAT "%d.%d.%d.%d"
#define BK_IP4_STR(_ip) ((_ip) & 0xFF), (((_ip) >> 8) & 0xFF), (((_ip) >> 16) & 0xFF), (((_ip) >> 24) & 0xFF)
#define BK_MAC_FORMAT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BK_MAC_STR(_m) (_m)[0], (_m)[1], (_m)[2], (_m)[3], (_m)[4], (_m)[5]
#define BK_MAC_STR_INVERT(_m) (_m)[5], (_m)[4], (_m)[3], (_m)[2], (_m)[1], (_m)[0]

#define BK_U64_FORMAT "%x.%x"
#define BK_U64_TO_U32(x) (((uint32_t)(((x) >> 32) & 0xFFFFFFFF)), ((uint32_t)((x) & 0xFFFFFFFF)))

void bk_mem_dump(const char* titile, uint32_t start, uint32_t len);
#define BK_MEM_DUMP(_title, _start, _len) bk_mem_dump((_title), (_start), (_len))

#ifdef CONFIG_SHELL_ASYNCLOG

#include "components/shell_task.h"

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

