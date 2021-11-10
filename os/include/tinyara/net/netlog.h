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

#ifndef __LINUX__
#include <debug.h>
#endif

/*  network log for kernel space */
#define NET_LOGK(tag, fmt, args...) \
	nwdbg(fmt, ##args)
#define NET_LOGKE(tag, fmt, args...) \
	ndbg("%d " tag "[ERR]\t" fmt, __LINE__, ##args)
#define NET_LOGKI(tag, fmt, args...) \
	nwdbg("%d " tag "[INFO]\t" fmt, __LINE__, ##args)
#define NET_LOGKV(tag, fmt, args...) \
	nvdbg("%d " tag "[VERB]\t" fmt, __LINE__, ##args)

/*  To-do: change dbg to user space API */
/*  network log for user space */
#define NET_LOG(tag, fmt, args...) \
	nwdbg(fmt, ##args)
#define NET_LOGE(tag, fmt, args...) \
	ndbg("%d " tag "[ERR]\t" fmt, __LINE__, ##args)
#define NET_LOGI(tag, fmt, args...) \
	nwdbg("%d " tag "[INFO]\t" fmt, __LINE__, ##args)
#define NET_LOGV(tag, fmt, args...) \
	nvdbg("%d " tag "[VERB]\t" fmt, __LINE__, ##args)

typedef struct {
	int idx;
	int size;
	char *buf;
} netmgr_logger;

typedef netmgr_logger *netmgr_logger_p;

typedef enum {
	/*  user level module */
	NL_MOD_WIFI_MANAGER,
	NL_MOD_NETLIB,
	/*  kernel level module */
	NL_MOD_LWIP,
	NL_MOD_NET_MANAGER,
} netlog_module_e;

typedef enum {
	NL_LEVEL_ERROR,
	NL_LEVEL_INFO,
	NL_LEVEL_VERB,
  NL_LEVEL_UNKNOWN,
} netlog_log_level_e;

/*
 * DESCRIPTION: Initialize logger
 * RETURN: return 0 if succeed, else return negative if fail
 *
 * NOTE: netlogger_deinit() have to be called to prevent memory leak
 *       if it was succeeded.
 */
int netlogger_init(netmgr_logger_p *log);

/*
 * DESCRIPTION: Deinitialize logger
 * RETURN: None
 */
void netlogger_deinit(netmgr_logger_p log);

/*
 * DESCRIPTION: add msg to logger.
 * RETURN: return 0 if succeed, else return negative if fail to add logger.
 */
int netlogger_debug_msg(netmgr_logger_p log, const char *format, ...);

/*
 * DESCRIPTION: serialize message in logger to user memory region.
 * RETURN: return size of buffer if success, else return -1
 *
 * NOTE: a caller has responsible to free buf.
 */
int netlogger_serialize(netmgr_logger_p log, char **buf);

/*
 * DESCRIPTION: print log to stdout
 * RETURN: return log length
 */
int netlogger_print(netlog_module_e mod, netlog_log_level_e level, const char *format, ...);

/*
 * DESCRIPTION: set level
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_level(netlog_module_e module, netlog_log_level_e level);

/*
 * DESCRIPTION: reset all log configurations
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_reset(void);
