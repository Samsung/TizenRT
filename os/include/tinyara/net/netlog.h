/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#pragma once

#include <stdint.h>
#ifndef __LINUX__
#include <debug.h>
#endif
#include "netlog_util.h"

typedef enum {
	/*  user level module */
	NL_MOD_WIFI_MANAGER,
	NL_MOD_NETLIB,
	/*  kernel level module */
	NL_MOD_LWIP,
	NL_MOD_NET_MANAGER,
	NL_MOD_UNKNOWN,
  NL_MODE_ALL,
} netlog_module_e;

#define NL_LEVEL_ERROR 0x00
#define NL_LEVEL_INFO 0x01
#define NL_LEVEL_VERB 0x02
#define NL_LEVEL_UNKNOWN 0x03

/*
 * DESCRIPTION: print log to stdout
 * RETURN: return log length
 */
int netlogger_print(netlog_module_e mod,
					uint32_t level,
					const char *func,
					const char *file,
					const int line,
					const char *fmt, ...);

/*
 * DESCRIPTION: set level
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_level(netlog_module_e module, uint32_t level);

/*
 * DESCRIPTION: set color
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_color(netlog_module_e module, nl_color_e color);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_timer(nl_options opt);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_function(nl_options opt);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_file(nl_options opt);

/*
 * DESCRIPTION: reset all log configurations
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_reset(void);

/*  To-do: change printf to netlogger_print */
/*  network log for user space */
#ifdef __LINUX__
#define NET_LOG(mod, fmt, args...) \
	netlogger_print(mod, NL_LEVEL_UNKNOWN, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGE(mod, fmt, args...) \
	netlogger_print(mod, NL_LEVEL_ERROR, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGI(mod, fmt, args...) \
	netlogger_print(mod, NL_LEVEL_INFO, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGV(mod, fmt, args...) \
	netlogger_print(mod, NL_LEVEL_VERB, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#else
#define NET_LOG(mod, fmt, args...) \
	printf(mod fmt, ##args);
#define NET_LOGE(mod, fmt, args...) \
	printf(mod"[ERR] " fmt, ##args);
#define NET_LOGI(mod, fmt, args...) \
	printf(mod"[INFO] " fmt, ##args);
#define NET_LOGV(mod, fmt, args...)
#endif
