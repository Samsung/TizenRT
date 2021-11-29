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
#include <tinyara/config.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sltool_usage.h"
#include "sltool.h"

/* seclink index range 0 ~ 63 */
#define SL_IDX_MIN 0
#define SL_IDX_MAX 63

typedef struct sl_handler_parser {
	sltool_ops ops;
	sl_handler func;
} sl_handler_parser_s;

typedef struct sl_ops_parser {
	const char *cmd;
	sltool_ops ops;
} sl_ops_parser_s;

#ifdef SLTOOL_MODE_TABLE
#undef SLTOOL_MODE_TABLE
#endif
#define SLTOOL_MODE_TABLE(cmd, parser) \
	static sl_handler parser(sl_options_s *opt);
#include "sltool_mode_table.h"

static sl_parser_func g_parser_table[] = {
#ifdef SLTOOL_MODE_TABLE
#undef SLTOOL_MODE_TABLE
#endif
#define SLTOOL_MODE_TABLE(cmd, parser) parser,
#include "sltool_mode_table.h"
};

static char *g_parser_str[] = {
#ifdef SLTOOL_MODE_TABLE
#undef SLTOOL_MODE_TABLE
#define SLTOOL_MODE_TABLE(cmd, parser) cmd,
#include "sltool_mode_table.h"
#endif
};

static sl_ops_parser_s g_ops_cmd[] = {
#ifdef SLTOOL_OPS_TABLE
#undef SLTOOL_OPS_TABLE
#endif
#define SLTOOL_OPS_TABLE(cmd, ops) \
	{cmd, ops},
#include "sltool_ops_table.h"
};

/*****************/
/* Key handler   */
/*****************/
#ifdef SLTOOL_KEY_TABLE
#undef SLTOOL_KEY_TABLE
#endif
#define SLTOOL_KEY_TABLE(ops, handler) \
	extern int handler(sl_options_s *opt);
#include "sltool_key_table.h"

static sl_handler_parser_s g_key_handler[] = {
#ifdef SLTOOL_KEY_TABLE
#undef SLTOOL_KEY_TABLE
#endif
#define SLTOOL_KEY_TABLE(ops, handler) {ops, handler},
#include "sltool_key_table.h"
};

/*****************/
/* Auth handler  */
/*****************/
#ifdef SLTOOL_AUTH_TABLE
#undef SLTOOL_AUTH_TABLE
#endif
#define SLTOOL_AUTH_TABLE(ops, handler) \
	extern int handler(sl_options_s *opt);
#include "sltool_auth_table.h"

static sl_handler_parser_s g_auth_handler[] = {
#ifdef SLTOOL_AUTH_TABLE
#undef SLTOOL_AUTH_TABLE
#endif
#define SLTOOL_AUTH_TABLE(ops, handler) {ops, handler},
#include "sltool_auth_table.h"
};

/*****************/
/* Auth handler  */
/*****************/
#ifdef SLTOOL_SS_TABLE
#undef SLTOOL_SS_TABLE
#endif
#define SLTOOL_SS_TABLE(ops, handler) \
	extern int handler(sl_options_s *opt);
#include "sltool_ss_table.h"

static sl_handler_parser_s g_ss_handler[] = {
#ifdef SLTOOL_SS_TABLE
#undef SLTOOL_SS_TABLE
#endif
#define SLTOOL_SS_TABLE(ops, handler) {ops, handler},
#include "sltool_ss_table.h"
};

static sltool_ops _parse_ops(char *cmd)
{
	for (int i = 0; i < sizeof(g_ops_cmd) / sizeof(sl_ops_parser_s); i++) {
		if (strncmp(cmd, g_ops_cmd[i].cmd, strlen(g_ops_cmd[i].cmd) + 1) == 0) {
			return g_ops_cmd[i].ops;
		}
	}
	return SLOPS_UNKNOWN;
}

static sl_handler _parse_key_handler(sl_options_s *opt)
{
	for (int i = 0; i < sizeof(g_key_handler) / sizeof(sl_handler_parser_s); i++) {
		if (opt->ops == g_key_handler[i].ops) {
			return g_key_handler[i].func;
		}
	}
	return NULL;
}

static sl_handler _parse_auth_handler(sl_options_s *opt)
{
	for (int i = 0; i < sizeof(g_auth_handler) / sizeof(sl_handler_parser_s); i++) {
		if (opt->ops == g_auth_handler[i].ops) {
			return g_auth_handler[i].func;
		}
	}
	return NULL;
}

static sl_handler _parse_ss_handler(sl_options_s *opt)
{
	for (int i = 0; i < sizeof(g_ss_handler) / sizeof(sl_handler_parser_s); i++) {
		if (opt->ops == g_ss_handler[i].ops) {
			return g_ss_handler[i].func;
		}
	}
	return NULL;
}

static int _parse_cmd(sl_options_s *opt, int argc, char **argv)
{
	for (int i = 0; i < sizeof(g_parser_str) / sizeof(char *); i++) {
		if (strncmp(argv[1], g_parser_str[i], strlen(g_parser_str[i]) + 1) == 0) {
			opt->func = g_parser_table[i];
			return 0;
		}
	}
	return -1;
}

static int _parse_mode(sl_options_s *opt, int argc, char *argv[])
{
	/*  To-Do parse ss write handler */
	if (argc != 4 && argc != 6) {
		return -1;
	}
	opt->ops = _parse_ops(argv[2]);
	if (opt->ops == SLOPS_UNKNOWN) {
		return -2;
	}

	if (argc == 6) {
		/*  To-Do parse ss write handler */
		opt->ss_data = atoi(argv[5]);
		opt->ss_write_size = atoi(argv[4]);
	}
	opt->idx = atoi(argv[3]);
	if (opt->idx < SL_IDX_MIN || opt->idx > SL_IDX_MAX) {
		printf("invalid slot range %d ~ %d\n", SL_IDX_MIN, SL_IDX_MAX);
		return -1;
	}
	sl_handler handler = opt->func(opt);
	if (!handler) {
		printf("invalid operation %d\n", opt->ops);
		return -3;
	}
	handler(opt);

	return 0;
}

void sltool_cmd(int argc, char **argv)
{
	if (argc < 3) {
		printf("%s", SLTOOL_USAGE);
		return;
	}
	sl_options_s opt;
	int res = _parse_cmd(&opt, argc, argv);
	if (res != 0) {
		printf("fail to parse mode %d\n", res);
		return;
	}
	res = _parse_mode(&opt, argc, argv);
	if (res != 0) {
		printf("fail to parse function %d\n", res);
	}
}
