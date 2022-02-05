/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include "netlog_lwnl.h"
#include "netlog_util.h"

//#define USE_NDBG

typedef struct {
	int idx;
	int size;
	char *buf;
} netmgr_logger;
typedef netmgr_logger *netmgr_logger_p;

/*
 * lwip log level mapped to netlog like following table
 *   |---------+-----------------|
 *   | ERROR   | severe          |
 *   | INFO    | serious warning |
 *   | VERBOSE | all             |
 *   |---------+-----------------|
 */
#ifdef __LINUX__
/*  To test netlog under linux without lwIP build, it needs lwIP definitions. */
#include "netlog_lwip.h"
#define LWIP_DBG_LEVEL_ALL 0x00
#define LWIP_DBG_LEVEL_WARNING 0x01
#define LWIP_DBG_LEVEL_SERIOUS 0x02
#define LWIP_DBG_LEVEL_SEVERE 0x03

#define LWIP_DBG_LEVEL_ALL 0x00
#define LWIP_DBG_MIN_LEVEL LWIP_DBG_LEVEL_ALL
#define LWIP_DBG_MASK_LEVEL 0x03
#define LWIP_DBG_LEVEL_OFF LWIP_DBG_LEVEL_ALL
#define LWIP_DBG_ON 0x80U
#define LWIP_DBG_OFF 0x00U
#define LWIP_DBG_TYPES_ON LWIP_DBG_ON
#define LWIP_DBG_HALT 0x00

#define LWIP_STRIP_PARENS( ... )   __VA_ARGS__
#define LWIP_DEBUGF(debug, message)                                  \
	netlogger_print(NL_MOD_LWIP, debug, __FUNCTION__, __FILE__, __LINE__, \
									LWIP_STRIP_PARENS message)
#endif /*  __LINUX__  */

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
	NL_LWIP_SUBLEVEL_SIZE,
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
int netlogk_print(netlog_module_e mod,
					uint32_t level,
					const char *func,
					const char *file,
					const int line,
					const char *fmt, ...);

/*
 * DESCRIPTION: set level
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_level(netlog_msg_s *msg);

/*
 * DESCRIPTION: set lwip level
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_lwip_level(netlog_msg_s *msg);

/*
 * DESCRIPTION: set color
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_color(netlog_msg_s *msg);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_timer(netlog_msg_s *msg);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_function(netlog_msg_s *msg);

/*
 * DESCRIPTION: configure timer in log
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_set_file(netlog_msg_s *msg);

/*
 * DESCRIPTION: reset all log configurations
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_reset(void);

/*
 * DESCRIPTION: handle the requests from applications.
 * RETURN: if succeeded return 0 else return -1
 */
int netlogk_handle_msg(netlog_msg_s *msg);
/**************************************************
 * API
 ***************************************************/
/*  network log for kernel space */
#ifdef USE_NDBG
#define NET_LOGK(tag, fmt, args...) \
	nwdbg(fmt, ##args)
#define NET_LOGKE(tag, fmt, args...) \
	ndbg("%d " tag "[ERR]\t" fmt, __LINE__, ##args)
#define NET_LOGKI(tag, fmt, args...) \
	nwdbg("%d " tag "[INFO]\t" fmt, __LINE__, ##args)
#define NET_LOGKV(tag, fmt, args...) \
	nvdbg("%d " tag "[VERB]\t" fmt, __LINE__, ##args)
#else
#define NET_LOGK(mod, fmt, args...) \
  netlogk_print(mod, NL_LEVEL_UNKNOWN, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGKE(mod, fmt, args...) \
  netlogk_print(mod, NL_LEVEL_ERROR, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGKI(mod, fmt, args...) \
  netlogk_print(mod, NL_LEVEL_INFO, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#define NET_LOGKV(mod, fmt, args...) \
  netlogk_print(mod, NL_LEVEL_VERB, __FUNCTION__, __FILE__, __LINE__, fmt, ##args)
#endif
