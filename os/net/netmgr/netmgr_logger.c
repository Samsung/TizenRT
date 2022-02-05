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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/net/netlogk.h>
#include <tinyara/kmalloc.h>

#define NETLOGGER_BUF_SIZE 256
#ifdef NETLOG_TEST
static uint8_t g_lwip_mod_level[33] = {
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
		NL_LEVEL_VERB,
};
#else
static uint8_t g_lwip_mod_level[33] = {
		NL_LEVEL_UNKNOWN,
};
#endif

/* static uint32_t _get_lwip_level(uint32_t debug) */
/* { */
/* 	uint8_t mask = (debug & 0x0f); */
/* 	if (mask == 0) { */
/* 		return NL_LEVEL_VERB; */
/* 	} else if (mask == 0x01 || mask == 0x02) { */
/* 		return NL_LEVEL_INFO; */
/* 	} else if (mask == 0x03) { */
/* 		return NL_LEVEL_ERROR; */
/* 	} else { */
/* 		assert(0); */
/* 	} */
/* 	return NL_LEVEL_UNKNOWN; */
/* } */

/* static int _filter_lwip_level(uint32_t mod, uint32_t level) */
/* { */
/* 	uint32_t lwip_mod = mod >> 4; */
/* 	if (g_lwip_mod_level[lwip_mod] < level || g_lwip_mod_level[lwip_mod] == NL_LEVEL_UNKNOWN) { */
/* 		return -1; */
/* 	} */
/* 	return 0; */
/* } */

int netlogger_init(netmgr_logger_p *log)
{
	netmgr_logger *tlog = (netmgr_logger *)kmm_malloc(sizeof(netmgr_logger));
	if (!tlog) {
		return -1;
	}
	tlog->idx = 0;
	tlog->buf = NULL;
	tlog->size = 0;

	*log = tlog;
	return 0;
}

void netlogger_deinit(netmgr_logger_p log)
{
	if (log) {
		if (log->buf) {
			kmm_free(log->buf);
		}
		kmm_free(log);
	}
}

int netlogger_debug_msg(netmgr_logger_p log, const char *format, ...)
{
	va_list argp;
	char str[NETLOGGER_BUF_SIZE];
	int ret;

	va_start(argp, format);
	ret = vsnprintf(str, NETLOGGER_BUF_SIZE, format, argp);
	va_end(argp);

	int remain = log->size - log->idx;
	if (ret > remain) {
		log->size =
				((log->idx + ret) / NETLOGGER_BUF_SIZE + 1) * NETLOGGER_BUF_SIZE;
		log->buf = (char *)kmm_realloc(log->buf, log->size);
		if (!log->buf) {
			return -1;
		}
	}
	memcpy(log->buf + log->idx, str, ret);
	log->idx += ret;

	return 0;
}

int netlogger_serialize(netmgr_logger_p log, char **buf)
{
	*buf = (char *)kumm_malloc(log->idx + 1); // pad 1 bypte for null character
	if (!*buf) {
		return -1;
	}
	memcpy(*buf, log->buf, log->idx);
	(*buf)[log->idx] = 0;

	return log->idx;
}

int netlogk_print(netlog_module_e mod,
									uint32_t level,
									const char *func,
									const char *file,
									const int line,
									const char *fmt, ...)
{
	return 0;
}

int netlogk_set_level(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (msg->mod == NL_MOD_LWIP) {
		return netlogk_set_lwip_level(msg);
	}
	return 0;
}

int netlogk_set_lwip_level(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	uint32_t lwip_mode = msg->sub_mod >> 4;
	g_lwip_mod_level[lwip_mode] = msg->level;
	return 0;
}

int netlogk_set_color(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

int netlogk_set_timer(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

int netlogk_set_function(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

int netlogk_set_file(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
}

int netlogk_reset(void)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	uint32_t size = sizeof(g_lwip_mod_level) / sizeof(uint8_t);
	for (int i = 0; i < size; i++) {
		g_lwip_mod_level[i] = NL_LEVEL_UNKNOWN;
	}
	return 0;
}

int netlogk_handle_msg(netlog_msg_s *msg)
{
	switch (msg->ops) {
	case NL_OPS_TIMER:
		return netlogk_set_timer(msg);
	case NL_OPS_FUNCTION:
		return netlogk_set_function(msg);
	case NL_OPS_FILE:
		return netlogk_set_file(msg);
	case NL_OPS_LEVEL:
		return netlogk_set_level(msg);
	case NL_OPS_COLOR:
		return netlogk_set_color(msg);
	default:
		assert(0);
	}
	return -1;
}
