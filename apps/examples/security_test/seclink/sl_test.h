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
#define SECLINK_TEST_TRIAL 5
#define SECLINK_TEST_LIMIT 1000000
#define SECLINK_TEST_MEM_SIZE 4096

#define SL_TEST_ERR(msg) printf(msg "\t%s:%d\n", __FUNCTION__, __LINE__)

/*
 * Functions
 */
void sl_test_free_buffer(hal_data *data);
int sl_test_malloc_buffer(hal_data *data, int buf_len);
int sl_test_malloc_buffer_priv(hal_data *data, int buf_len);
void sl_test_init_buffer(hal_data *data);
void sl_test_print_buffer(char *data, int buf_len, const char *message);
