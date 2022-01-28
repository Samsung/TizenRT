/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <tinyara/net/netlog.h>
#include <tinyara/net/netlog_lwip.h>
#include "ctrlog_utils.h"
#include "ctrlog_usage.h"

#define CMD_LEVEL "level="
#define CMD_TIME "time="
#define CMD_FUNC "func="
#define CMD_FILE "file="
#define CMD_COLOR "color="
#define CMD_MOD "mod="
#define CMD_RESET "reset"

static char *g_lwip_mod_table_str[] = {
		"etharp",
		"netif",
		"pbuf",
		"api_lib",
		"api_msg",
		"sockets",
		"icmp",
		"igmp",
		"inet",
		"ip",
		"ip_reass",
		"raw",
		"mem",
		"memp",
		"sys",
		"timers",
		"tcp",
		"tcp_input",
		"tcp_fr",
		"tcp_rto",
		"tcp_cwnd",
		"tcp_wnd",
		"tcp_output",
		"tcp_rst",
		"tcp_qlen",
		"udp",
		"tcpip",
		"slip",
		"dhcp",
		"autoip",
		"dns",
		"ip6",
		"nd6"};

static uint32_t g_lwip_mod_table[] = {
		ETHARP_DEBUG,
		NETIF_DEBUG,
		PBUF_DEBUG,
		API_LIB_DEBUG,
		API_MSG_DEBUG,
		SOCKETS_DEBUG,
		ICMP_DEBUG,
		IGMP_DEBUG,
		INET_DEBUG,
		IP_DEBUG,
		IP_REASS_DEBUG,
		RAW_DEBUG,
		MEM_DEBUG,
		MEMP_DEBUG,
		SYS_DEBUG,
		TIMERS_DEBUG,
		TCP_DEBUG,
		TCP_INPUT_DEBUG,
		TCP_FR_DEBUG,
		TCP_RTO_DEBUG,
		TCP_CWND_DEBUG,
		TCP_WND_DEBUG,
		TCP_OUTPUT_DEBUG,
		TCP_RST_DEBUG,
		TCP_QLEN_DEBUG,
		UDP_DEBUG,
		TCPIP_DEBUG,
		SLIP_DEBUG,
		DHCP_DEBUG,
		AUTOIP_DEBUG,
		DNS_DEBUG,
		IP6_DEBUG,
		ND6_DEBUG,
};

static char *g_level_table_str[] = {
		"error",
		"info",
		"verb",
};

static uint32_t g_level_table[] = {
		NL_LEVEL_ERROR,
		NL_LEVEL_INFO,
		NL_LEVEL_VERB,
};

static char *g_color_table_str[] = {
		"red", "yellow", "green", "default"};

static nl_color_e g_color_table[] = {
		NL_COLOR_RED, NL_COLOR_YELLOW, NL_COLOR_GREEN, NL_COLOR_GREEN};

static char *g_mod_table_str[] = {
		"wifi", "netlib", "lwip", "netmgr"};

static netlog_module_e g_mod_table[] = {
		NL_MOD_WIFI_MANAGER,
		NL_MOD_NETLIB,
		NL_MOD_LWIP,
		NL_MOD_NET_MANAGER,
};

static char *g_opt_table_str[] = {
		"on",
		"off",
};

static nl_options g_opt_table[] = {
		NL_OPT_ENABLE,
		NL_OPT_DISABLE,
};

static uint32_t _get_lwip_mod(char *mod_str)
{
	printf("[pkbuild] %s \t%s:%d\n", mod_str, __FUNCTION__, __LINE__);
	int size = sizeof(g_lwip_mod_table_str) / sizeof(char *);
	for (int i = 0; i < size; i++) {
		if ((strlen(mod_str) == strlen(g_lwip_mod_table_str[i]))
        && strncmp(mod_str, g_lwip_mod_table_str[i], strlen(mod_str) + 1) == 0) {
			return g_lwip_mod_table[i];
		}
	}
	return UNKNOWN_DEBUG;
}

static uint32_t _get_level(char *level_str)
{
	printf("[pkbuild] level %s \t%s:%d\n", level_str, __FUNCTION__, __LINE__);
	int size = sizeof(g_level_table_str) / sizeof(char *);
	for (int i = 0; i < size; i++) {
		if (strncmp(level_str, g_level_table_str[i], strlen(g_level_table_str[i]) + 1) == 0) {
			printf("[pkbuild] i %d %d %s \t%s:%d\n", i, g_level_table[i], g_level_table_str[i], __FUNCTION__, __LINE__);
			return g_level_table[i];
		}
	}
	return NL_LEVEL_UNKNOWN;
}

static nl_color_e _get_color(char *color_str)
{
	printf("[pkbuild] %s \t%s:%d\n", color_str, __FUNCTION__, __LINE__);
	int size = sizeof(g_color_table_str) / sizeof(char *);
	for (int i = 0; i < size; i++) {
		if (strncmp(color_str, g_color_table_str[i], strlen(g_color_table_str[i]) + 1) == 0) {
			printf("[pkbuild] i %d %d %s \t%s:%d\n", i, g_color_table[i], g_color_table_str[i], __FUNCTION__, __LINE__);
			return g_color_table[i];
		}
	}
	return NL_COLOR_DEFAULT;
}

static netlog_module_e _get_mod(char *mod_str)
{
	printf("[pkbuild] %s \t%s:%d\n", mod_str, __FUNCTION__, __LINE__);
	int size = sizeof(g_mod_table_str) / sizeof(char *);
	for (int i = 0; i < size; i++) {
		/* lwIP has additional submodule. So instead of comparing null
     * chracter at the end of string, it won't compare it */
		if (strncmp(mod_str, g_mod_table_str[i], strlen(g_mod_table_str[i])) == 0) {
			return g_mod_table[i];
		}
	}
	return NL_MOD_UNKNOWN;
}

static nl_options _get_option(char *opt_str)
{
	printf("[pkbuild] %s \t%s:%d\n", opt_str, __FUNCTION__, __LINE__);
	int size = sizeof(g_opt_table_str) / sizeof(char *);
	for (int i = 0; i < size; i++) {
		if (strncmp(opt_str, g_opt_table_str[i], strlen(g_opt_table_str[i]) + 1) == 0) {
			return g_opt_table[i];
		}
	}
	return NL_OPT_UNKNOWN;
}

/*
 * Handler
 */
static int _handle_level(void *arg)
{
	cl_options_s *opt = (cl_options_s *)arg;
	printf("[pkbuild] mod (%d/%02x) level %d \t%s:%d\n", opt->mod, opt->sub_mod, opt->level, __FUNCTION__, __LINE__);
  if (opt->mod == NL_MOD_LWIP) {
    return netlog_set_lwip_level(opt->sub_mod, opt->level);
  }
	return netlog_set_level(opt->mod, opt->level);
}

static int _handle_time(void *arg)
{
	cl_options_s *opt = (cl_options_s *)arg;
	printf("[pkbuild] opt %d \t%s:%d\n", opt->op, __FUNCTION__, __LINE__);
	return netlog_set_timer(opt->op);
}

static int _handle_func(void *arg)
{
	cl_options_s *opt = (cl_options_s *)arg;
	printf("[pkbuild] opt %d \t%s:%d\n", opt->op, __FUNCTION__, __LINE__);
	return netlog_set_function(opt->op);
}

static int _handle_file(void *arg)
{
	cl_options_s *opt = (cl_options_s *)arg;
	printf("[pkbuild] opt %d \t%s:%d\n", opt->op, __FUNCTION__, __LINE__);
	return netlog_set_file(opt->op);
}

static int _handle_color(void *arg)
{
	cl_options_s *opt = (cl_options_s *)arg;
	printf("[pkbuild] color %d mode %d \t%s:%d\n", opt->color, opt->mod, __FUNCTION__, __LINE__);
	return netlog_set_color(opt->mod, opt->color);
}

static int _handle_reset(void *arg)
{
  printf("[pkbuild]  \t%s:%d\n",  __FUNCTION__, __LINE__);
  return netlog_reset();
}
/*
 * Parser
 */
static int _parse_level(void *arg, int argc, char **argv)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (argc != 2) {
		return -1;
	}
	cl_options_s *opt = (cl_options_s *)arg;
	opt->level = _get_level(argv[0] + strlen(CMD_LEVEL));
	opt->mod = _get_mod(argv[1] + strlen(CMD_MOD));
	if (opt->mod == NL_MOD_LWIP) {
		// get lwip sub module
		opt->sub_mod = _get_lwip_mod(argv[1] + strlen(CMD_MOD) + strlen("lwip:"));
	}
  printf("[pkbuild] %d %d %d \t%s:%d\n", opt->level, opt->mod, opt->sub_mod, __FUNCTION__, __LINE__);
	opt->func = _handle_level;

	return 0;
}

static int _parse_time(void *arg, int argc, char **argv)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (argc != 1) {
		return -1;
	}
	cl_options_s *opt = (cl_options_s *)arg;
	opt->op = _get_option(argv[0] + strlen(CMD_TIME));
	if (opt->op == NL_OPT_UNKNOWN) {
		return -1;
	}
	opt->func = _handle_time;
	return 0;
}

static int _parse_func(void *arg, int argc, char **argv)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (argc != 1) {
		return -1;
	}
	cl_options_s *opt = (cl_options_s *)arg;
	opt->op = _get_option(argv[0] + strlen(CMD_FUNC));
	if (opt->op == NL_OPT_UNKNOWN) {
		return -1;
	}
	opt->func = _handle_func;
	return 0;
}

static int _parse_file(void *arg, int argc, char **argv)
{
	printf("[pkbuild]  \t%s:%d\n", __FUNCTION__, __LINE__);
	if (argc != 1) {
		return -1;
	}
	cl_options_s *opt = (cl_options_s *)arg;
	opt->op = _get_option(argv[0] + strlen(CMD_FILE));
	if (opt->op == NL_OPT_UNKNOWN) {
		return -1;
	}
	opt->func = _handle_file;
	return 0;
}

static int _parse_color(void *arg, int argc, char **argv)
{
	printf("[pkbuild] argc %d \t%s:%d\n", argc, __FUNCTION__, __LINE__);
	if (argc != 2) {
		return -1;
	}
	if (strncmp(CMD_MOD, argv[1], strlen(CMD_MOD)) != 0) {
		return -1;
	}
	cl_options_s *opt = (cl_options_s *)arg;
	opt->color = _get_color(argv[0] + strlen(CMD_COLOR));
	opt->mod = _get_mod(argv[1] + strlen(CMD_MOD));
	opt->func = _handle_color;
	printf("[pkbuild] color %d %d\t%s:%d\n", opt->color, opt->mod, __FUNCTION__, __LINE__);

	return 0;
}

static int _parse_reset(void *arg, int argc, char **argv) {
  printf("[pkbuild]  \t%s:%d\n",  __FUNCTION__, __LINE__);
  if (argc != 1) {
    return -1;
  }
  cl_options_s *opt = (cl_options_s *)arg;
  opt->func = _handle_reset;
  return 0;
}
static int _parse_cmd(void *arg, int argc, char **argv)
{
	if (strncmp(argv[0], CMD_LEVEL, strlen(CMD_LEVEL)) == 0) {
		return _parse_level(arg, argc, argv);
	} else if (strncmp(argv[0], CMD_TIME, strlen(CMD_TIME)) == 0) {
		return _parse_time(arg, argc, argv);
	} else if (strncmp(argv[0], CMD_FUNC, strlen(CMD_FUNC)) == 0) {
		return _parse_func(arg, argc, argv);
	} else if (strncmp(argv[0], CMD_FILE, strlen(CMD_FILE)) == 0) {
		return _parse_file(arg, argc, argv);
	} else if (strncmp(argv[0], CMD_COLOR, strlen(CMD_COLOR)) == 0) {
		return _parse_color(arg, argc, argv);
	} else if (strncmp(argv[0], CMD_RESET, strlen(CMD_RESET) + 1) == 0) {
    return _parse_reset(arg, argc, argv);
  } else {
		assert(0);
	}
	return -1;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ctrlog_main(int argc, char *argv[])
#endif
{
	if (argc < 2) {
		printf("%s", CTRLOG_USAGE);
		return -1;
	}
	cl_options_s opt;
  memset(&opt, 0, sizeof(cl_options_s));

	int res = _parse_cmd((void *)&opt, argc - 1, argv + 1);
	if (res != 0) {
    printf("%s", CTRLOG_USAGE);
		return -1;
	}

	opt.func((void *)&opt);

	return 0;
}
