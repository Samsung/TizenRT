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

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
#ifdef __LINUX__
#include "netlog.h"
#else
#include <sys/socket.h>
#include <tinyara/net/netlog.h>
#include <tinyara/net/netlog_lwnl.h>
#include <tinyara/lwnl/lwnl.h>
#endif

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

static inline int _send_msg(netlog_msg_s *req)
{
	int res = 0;
	lwnl_msg msg = {LWNL_INTF_NAME, {LWNL_REQ_COMMON_NETLOG}, sizeof(netlog_msg_s), req, (void *)res};
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		return -1;
	}
	if (write(fd, (void *)&msg, sizeof(msg)) < 0) {
		return -1;
	}
	close(fd);

	return res;
}

static int _is_kernel_module(netlog_module_e mod)
{
	if (mod == NL_MOD_LWIP || mod == NL_MOD_NET_MANAGER) {
		return 1;
	}
	return 0;
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

int _set_klevel(netlog_module_e mod, uint32_t level)
{
	netlog_msg_s req = {NL_OPS_LEVEL, NL_COLOR_DEFAULT, NL_OPT_UNKNOWN, mod, level, 0};
	return _send_msg(&req);
}

int _set_kcolor(netlog_module_e mod, nl_color_e color)
{
	netlog_msg_s req = {NL_OPS_COLOR, color, NL_OPT_UNKNOWN, mod, NL_LEVEL_UNKNOWN, 0};
	return _send_msg(&req);
}

int _set_ktimer(nl_options opt)
{
	netlog_msg_s req = {NL_OPS_TIMER, NL_COLOR_DEFAULT, opt, NL_MOD_UNKNOWN, NL_LEVEL_UNKNOWN, 0};
	return _send_msg(&req);
}

int _set_kfunction(nl_options opt)
{
	netlog_msg_s req = {NL_OPS_FUNCTION, NL_COLOR_DEFAULT, opt, NL_MOD_UNKNOWN, NL_LEVEL_UNKNOWN, 0};
	return _send_msg(&req);
}

int _set_kfile(nl_options opt)
{
	netlog_msg_s req = {NL_OPS_FILE, NL_COLOR_DEFAULT, opt, NL_MOD_UNKNOWN, NL_LEVEL_UNKNOWN, 0};
	return _send_msg(&req);
}

int _kreset(void)
{
	netlog_msg_s req = {NL_OPS_RESET, NL_COLOR_DEFAULT, NL_OPT_UNKNOWN, NL_MOD_UNKNOWN, NL_LEVEL_UNKNOWN, 0};
	return _send_msg(&req);
}

int netlogger_print(netlog_module_e mod,
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
	level = log_level;
	if (_filter_level(mod, log_level)) {
		return 0;
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

int netlog_set_level(netlog_module_e mod, uint32_t level)
{
	if (_is_kernel_module(mod)) {
		return _set_klevel(mod, level);
	}
	g_mod_config[mod].level = level;
	return 0;
}

int netlog_set_lwip_level(uint32_t submod, uint32_t level)
{
	netlog_msg_s req = {NL_OPS_LEVEL, NL_COLOR_DEFAULT, NL_OPT_UNKNOWN, NL_MOD_LWIP, level, submod};
	return _send_msg(&req);
}

int netlog_set_color(netlog_module_e mod, nl_color_e color)
{
	if (_is_kernel_module(mod)) {
		return _set_kcolor(mod, color);
	}
	g_mod_config[mod].color = color;
	return 0;
}

int netlog_set_timer(nl_options opt)
{
	g_nl_timer = opt;
	return _set_ktimer(opt);
}

int netlog_set_function(nl_options opt)
{
	g_nl_function = opt;
	return _set_kfunction(opt);
}

int netlog_set_file(nl_options opt)
{
	g_nl_files = opt;
	return _set_kfile(opt);
}

int netlog_reset(void)
{
	int size = sizeof(g_mod_config) / sizeof(netlog_config_s);
	for (int i = 0; i < size; i++) {
		g_mod_config[i].level = NL_LEVEL_UNKNOWN;
		g_mod_config[i].color = NL_COLOR_DEFAULT;
	}

	g_nl_timer = NL_OPT_DISABLE;
	g_nl_function = NL_OPT_DISABLE;
	g_nl_files = NL_OPT_DISABLE;

	return _kreset();
}
