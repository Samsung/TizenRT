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

typedef enum {
	NL_COLOR_DEFAULT,
	NL_COLOR_RED,
	NL_COLOR_GREEN,
	NL_COLOR_YELLOW,
} nl_color_e;

typedef enum {
	NL_OPT_ENABLE,
	NL_OPT_DISABLE,
} nl_options;

typedef struct {
	int idx;
	int size;
	char *buf;
} netmgr_logger;

typedef netmgr_logger *netmgr_logger_p;

typedef enum {
	NL_MOD_UNKNOWN,
  NL_MODE_ALL,
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

typedef enum {
	NL_LWIP_ETHARP,
	NL_LWIP_NETIF,
	NL_LWIP_PBUF,
	NL_LWIP_API_LIB,
	NL_LWIP_API_MSG,
	NL_LWIP_SOCKETS,
	NL_LWIP_ICMP,
	NL_LWIP_IGMP,
	NL_LWIP_INET,
	NL_LWIP_IP,
	NL_LWIP_IP_REASS,
	NL_LWIP_RAW,
	NL_LWIP_MEM,
	NL_LWIP_MEMP,
	NL_LWIP_SYS,
	NL_LWIP_TIMERS,
	NL_LWIP_TCP,
	NL_LWIP_TCP_INPUT,
	NL_LWIP_TCP_FR,
	NL_LWIP_TCP_RTO,
	NL_LWIP_TCP_CWND,
	NL_LWIP_TCP_WND,
	NL_LWIP_TCP_OUTPUT,
	NL_LWIP_TCP_RST,
	NL_LWIP_TCP_QLEN,
	NL_LWIP_UDP,
	NL_LWIP_TCPIP,
	NL_LWIP_SLIP,
	NL_LWIP_DHCP,
	NL_LWIP_AUTOIP,
	NL_LWIP_DNS,
	NL_LWIP_IP6,
	NL_LWIP_ND6,
} netlog_lwip_sublevel_e;
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
int netlogger_print(netlog_module_e mod,
					netlog_log_level_e level,
					const char *func,
					const char *file,
					const int line,
					const char *fmt, ...);

/*
 * DESCRIPTION: set level
 * RETURN: if succeeded return 0 else return -1
 */
int netlog_set_level(netlog_module_e module, netlog_log_level_e level);

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

/**************************************************
 * API
 ***************************************************/
/*  network log for kernel space */
#define NET_LOGK(tag, fmt, args...) \
	nwdbg(fmt, ##args)
#define NET_LOGKE(tag, fmt, args...) \
	ndbg("%d " tag "[ERR]\t" fmt, __LINE__, ##args)
#define NET_LOGKI(tag, fmt, args...) \
	nwdbg("%d " tag "[INFO]\t" fmt, __LINE__, ##args)
#define NET_LOGKV(tag, fmt, args...) \
	nvdbg("%d " tag "[VERB]\t" fmt, __LINE__, ##args)

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
