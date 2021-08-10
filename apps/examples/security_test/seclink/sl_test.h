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
#pragma once

#include <unistd.h>
#include <tinyara/security_hal.h>

struct _sl_options;
typedef void (*sl_test_func)(struct _sl_options *arg);

struct _sl_options {
	sl_test_func func;
	int argc;
	char **argv;
	int count; // repeat count
	int type;  // type
	char run_all;
};
typedef struct _sl_options sl_options;


/*
 * Definitions
 */
#define SL_TEST_ERR(msg) printf(msg "\t%s:%d\n", __FUNCTION__, __LINE__)

#define SL_PARSE_MESSAGE(opt, cmd, ttype, ttype_func, ttype_max, ttype_err)       \
	do {                                                                          \
		if (opt->run_all == 1) {                                                  \
			sl_run_all(opt, ttype_func, ttype_max);                               \
		} else {                                                                  \
			ttype type = (ttype)sl_parse_command(opt, cmd, ttype_max, ttype_err); \
			if (type == ttype_err) {                                              \
				printf("%s\n", SL_USAGE);                                         \
				return;                                                           \
			} else if (type == ttype_max) {                                      \
				sl_run_all(opt, ttype_func, ttype_max);                           \
			} else {                                                              \
				ttype_func[type](opt);                                           \
			}                                                                     \
		}                                                                         \
	} while (0)

/*
 * Functions
 */
void sl_test_free_buffer(hal_data *data);
int sl_test_malloc_buffer(hal_data *data, int buf_len);
int sl_test_malloc_buffer_priv(hal_data *data, int buf_len);
void sl_test_init_buffer(hal_data *data);
void sl_test_print_buffer(char *data, int buf_len, const char *message);
void sl_run_all(sl_options *opt, sl_test_func *func, int max);
int sl_parse_command(sl_options *opt, char **command, int tmax, int terr);
