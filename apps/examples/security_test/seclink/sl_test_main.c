/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tinyara/seclink.h>
#include <tinyara/seclink_drv.h>
#include "sl_test.h"

#ifdef SL_TEST_POOL
#undef SL_TEST_POOL
#endif
#define SL_TEST_POOL(command, type, func)\
	extern void func(sl_options *arg);
#include "sl_test_table.h"

static char *g_command[] = {
#ifdef SL_TEST_POOL
#undef SL_TEST_POOL
#endif
#define SL_TEST_POOL(command, type, func) command,
#include "sl_test_table.h"
};

static sl_test_func g_func_list[] = {
#ifdef SL_TEST_POOL
#undef SL_TEST_POOL
#endif
#define SL_TEST_POOL(command, type, func) func,
#include "sl_test_table.h"
};

typedef enum {
#ifdef SL_TEST_POOL
#undef SL_TEST_POOL
#endif
#define SL_TEST_POOL(command, type, func) type,
#include "sl_test_table.h"
	SL_TYPE_MAX,
	SL_TYPE_ERR = -1
} sl_type_e;
/****************************************************************************
 * sl_test_main
 ****************************************************************************/

static int _parse_command(sl_options *opt, int argc, char *argv[])
{
	if (argc < 3) {
		return -1;
	}
	if (strncmp(argv[1], "all", strlen("all") + 1) == 0) {
		opt->count = atoi(argv[2]);
		return SL_TYPE_MAX;
	}

	for (int i = 0; i < SL_TYPE_MAX; i++) {
		if (strncmp(argv[1], g_command[i], strlen(g_command[i]) + 1) == 0) {
			return (sl_type_e)i;
		}
	}
	return SL_TYPE_ERR;
}

void _sl_run_all(sl_options *opt)
{
	for (int i = 0; i < SL_TYPE_MAX; i++) {
		g_func_list[i](opt);
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sl_test_main(int argc, char *argv[])
#endif
{
	sl_options opt;
	memset(&opt, 0, sizeof(sl_options));

	sl_type_e type = _parse_command(&opt, argc, argv);
	if (type == SL_TYPE_ERR) {
		return -1;
	} else if (type == SL_TYPE_MAX) {
		/*  run all test */
		opt.run_all = 1;
		_sl_run_all(&opt);
		return 0;
	}

	opt.argc = argc;
	opt.argv = argv;
	g_func_list[type](&opt);

	return 0;
}
