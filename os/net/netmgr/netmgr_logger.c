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
#include <tinyara/net/netlog_lwnl.h>
#include <tinyara/net/netlogk.h>
#include <tinyara/kmalloc.h>

typedef struct {
	uint8_t level;
	nl_color_e color;
} netlog_config_s;

#ifdef NETLOG_TEST
static netlog_config_s g_mod_config[4] = {
		{NL_LEVEL_VERB, NL_COLOR_DEFAULT},
		{NL_LEVEL_VERB, NL_COLOR_DEFAULT},
		{NL_LEVEL_VERB, NL_COLOR_DEFAULT},
		{NL_LEVEL_VERB, NL_COLOR_DEFAULT},
};
#else
static netlog_config_s g_mod_config[4] = {
		{NL_LEVEL_UNKNOWN, NL_COLOR_DEFAULT},
		{NL_LEVEL_UNKNOWN, NL_COLOR_DEFAULT},
		{NL_LEVEL_UNKNOWN, NL_COLOR_DEFAULT},
		{NL_LEVEL_UNKNOWN, NL_COLOR_DEFAULT},
};
#endif

static nl_options g_nl_timer = NL_OPT_DISABLE;
static nl_options g_nl_function = NL_OPT_DISABLE;
static nl_options g_nl_files = NL_OPT_DISABLE;

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

static uint32_t _get_lwip_level(uint32_t debug)
{
	uint8_t mask = (debug & 0x0f);
	if (mask == 0) {
		return NL_LEVEL_VERB;
	} else if (mask == 0x01 || mask == 0x02) {
		return NL_LEVEL_INFO;
	} else if (mask == 0x03) {
		return NL_LEVEL_ERROR;
	} else {
		assert(0);
	}
	return NL_LEVEL_UNKNOWN;
}

static int _filter_lwip_level(uint32_t mod, uint32_t level)
{
	uint32_t lwip_mod = mod >> 4;
	if (g_lwip_mod_level[lwip_mod] < level || g_lwip_mod_level[lwip_mod] == NL_LEVEL_UNKNOWN) {
		return -1;
	}
	return 0;
}

static const char *get_color_code(nl_color_e color)
{
	switch (color) {
	case NL_COLOR_RED:
		return "\033[0;31m";
	case NL_COLOR_GREEN:
		return "\033[0;32m";
	case NL_COLOR_YELLOW:
		return "\033[0;33m";
	default:
		return "\033[m";
	}
}
static inline int _print_time(void)
{
	time_t time_info = time(NULL);
	struct tm *tm = localtime(&time_info);

	// for korea time zone
	tm->tm_hour = (tm->tm_hour + 9) % 24;

	struct timespec current_time;
	clock_gettime(CLOCK_REALTIME, &current_time);

	unsigned int micro_sec = current_time.tv_nsec / 10000000;
	return printf(" [%02d:%02d:%02d.%02u]",
								tm->tm_hour, tm->tm_min, tm->tm_sec, micro_sec);
}

static int _filter_level(uint32_t mod, uint32_t level)
{
	if (g_mod_config[mod].level < level || g_mod_config[mod].level == NL_LEVEL_UNKNOWN) {
		return -1;
	}
	return 0;
}

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
									uint32_t log_level,
									const char *func,
									const char *file,
									const int line,
									const char *fmt, ...)
{
	uint32_t level = NL_LEVEL_UNKNOWN;
	if (mod == NL_MOD_UNKNOWN) {
		return 0;
	}

	if (mod == NL_MOD_LWIP) {
		level = _get_lwip_level(log_level);
		if (_filter_lwip_level(log_level, level)) {
			return 0;
		}
	} else {
		level = log_level;
		if (_filter_level(mod, log_level)) {
			return 0;
		}
	}

	int len = 0;
	va_list args;
	va_start(args, fmt);
	if (level == NL_LEVEL_ERROR) {
		len += printf("\033[01;45m ");
	} else {
		len += printf("%s", get_color_code(g_mod_config[mod].color));
	}
	if (g_nl_timer == NL_OPT_ENABLE) {
		len += _print_time();
	}
	if (g_nl_function == NL_OPT_ENABLE) {
		len += printf("[%s]", func);
	}
	if (g_nl_files == NL_OPT_ENABLE) {
		len += printf("[%s:%d]", file, line);
	}
	len += vprintf(fmt, args);
	len += printf("\033[m"); // Resets the terminal to default.
	va_end(args);

	return len;
}

int netlogk_set_level(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (msg->mod == NL_MOD_LWIP) {
		return netlogk_set_lwip_level(msg);
	}
	g_mod_config[msg->mod].level = msg->level;
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
	g_mod_config[msg->mod].color = msg->color;
	return 0;
}

int netlogk_set_timer(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	g_nl_timer = msg->op;
	return 0;
}

int netlogk_set_function(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	g_nl_function = msg->op;
	return 0;
}

int netlogk_set_file(netlog_msg_s *msg)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	g_nl_files = msg->op;
	return 0;
}

int netlogk_reset(void)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	uint32_t size = sizeof(g_mod_config) / sizeof(netlog_config_s);
	for (int i = 0; i < size; i++) {
		g_mod_config[i].level = NL_LEVEL_UNKNOWN;
		g_mod_config[i].color = NL_COLOR_DEFAULT;
	}

	size = sizeof(g_lwip_mod_level) / sizeof(uint8_t);
	for (int i = 0; i < size; i++) {
		g_lwip_mod_level[i] = NL_LEVEL_UNKNOWN;
	}
	g_nl_timer = NL_OPT_DISABLE;
	g_nl_function = NL_OPT_DISABLE;
	g_nl_files = NL_OPT_DISABLE;

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
	case NL_OPS_RESET:
		return netlogk_reset();
	default:
		assert(0);
	}
	return -1;
}
