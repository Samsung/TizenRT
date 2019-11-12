/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
#include <esp_log.h>
#else
#include <tinyara/syslog/syslog.h>
#endif
#include "iot_bsp_debug.h"

#define BUF_SIZE 512

void iot_bsp_debug(iot_debug_level_t level, const char *tag, const char *fmt, ...)
{
	char buf[BUF_SIZE] = {0,};
	va_list va;

	va_start(va, fmt);
	vsnprintf(buf, BUF_SIZE, fmt, va);
	va_end(va);

#if defined(CONFIG_STDK_IOT_CORE_EASYSETUP_HTTP_LOG_SUPPORT)
	iot_debug_save_log(buf);
#endif

#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	if (level == IOT_DEBUG_LEVEL_ERROR) {
		ESP_LOGE(tag, "%s", buf);
	} else if (level == IOT_DEBUG_LEVEL_WARN) {
		ESP_LOGW(tag, "%s", buf);
	} else if (level == IOT_DEBUG_LEVEL_INFO) {
		ESP_LOGI(tag, "%s", buf);
	} else if (level == IOT_DEBUG_LEVEL_DEBUG) {
		ESP_LOGD(tag, "%s", buf);
	} else {
		ESP_LOGD(tag, "%s", buf);
	}
#else
	syslog(level,"%s\n", buf);
#endif
}

static unsigned int _iot_bsp_debug_get_free_heap_size(void)
{
 struct mallinfo mem_info;

#ifdef CONFIG_CAN_PASS_STRUCTS
    mem_info = mallinfo();
#else
    (void)mallinfo(&mem_info);
#endif
    return mem_info.fordblks;
}

static unsigned int _iot_bsp_debug_get_minimum_free_heap_size(void)
{
	//get minimum free heap for debug info
	return 0;
}

static unsigned int _iot_bsp_debug_get_maximum_heap_size(void)
{
	////get maximum free heap for debug info
	return 0;
}

void iot_bsp_debug_check_heap(const char *tag, const char *func, const int line, const char *fmt, ...)
{
#ifdef CONFIG_ARCH_BOARD_ESP32_FAMILY
	static int count = 0;
	char *buf;
	int ret;
	va_list va;

	va_start(va, fmt);
	ret = vasprintf(&buf, fmt, va);
	va_end(va);

	if (count == 0) {
		ESP_LOGW(tag, "%s(%d) > [MEMCHK][%d] Heap total size : %d", func, line, count, _iot_bsp_debug_get_maximum_heap_size());
	}

	ESP_LOGW(tag, "%s(%d) > [MEMCHK][%d][%s] CU:%d, CR:%d, PU:%d, PR:%d", func, line, ++count, buf,
			_iot_bsp_debug_get_maximum_heap_size() - _iot_bsp_debug_get_free_heap_size(),
			_iot_bsp_debug_get_free_heap_size(),
			_iot_bsp_debug_get_maximum_heap_size() - _iot_bsp_debug_get_minimum_free_heap_size(),
			_iot_bsp_debug_get_minimum_free_heap_size());

	if (ret >= 0) {
		free(buf);
	}
#endif
}
