/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
n * you may not use this file except in compliance with the License.
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
#include <math.h>
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "sl_test.h"
#include "sl_test_usage.h"

static char *g_command[] = {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) command,
#include "sl_ss_table.h"
};

#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) \
	extern void func(sl_options *opt);
#include "sl_ss_table.h"

static sl_test_func g_func_list[] = {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) func,
#include "sl_ss_table.h"
};

typedef enum {
#ifdef SL_SS_TEST_POOL
#undef SL_SS_TEST_POOL
#endif
#define SL_SS_TEST_POOL(command, type, func) type,
#include "sl_ss_table.h"
	SL_SS_TYPE_MAX,
	SL_SS_TYPE_ERR = -1
} sl_ss_type_e;

static int _parse_command(sl_options *opt)
{
	int argc = opt->argc;
	char **argv = opt->argv;

	if (argc < 4) {
		return -1;
	}
	opt->count = atoi(argv[3]);

	if (strncmp(argv[2], "all", strlen("all") + 1) == 0) {
		return SL_SS_TYPE_MAX;
	}

	for (int i = 0; i < SL_SS_TYPE_MAX; i++) {
		if (strncmp(argv[2], g_command[i], strlen(g_command[i]) + 1) == 0) {
			return (sl_ss_type_e)i;
		}
	}
	return SL_SS_TYPE_ERR;
}

static void _run_all(sl_options *opt)
{
	for (int i = 0; i < SL_SS_TYPE_MAX; i++) {
		g_func_list[i](opt);
	}
}

void sl_handle_ss(sl_options *opt)
{
	if (opt->run_all == 1) {
		/*  run all test count times */
		_run_all(opt);
		return;
	}
	sl_auth_type_e type = _parse_command(opt);
	if (type == SL_AUTH_TYPE_ERR) {
		printf("%s\n", SL_USAGE);
		return;
	} else if (type == SL_AUTH_TYPE_MAX) {
		_run_all(opt);
		return;
	}
	g_func_list[type](opt);
}
