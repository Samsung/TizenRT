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

#include <tinyara/config.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/security_hal.h>
#include "sl_test.h"

void sl_test_free_buffer(hal_data *data)
{
	if (data == NULL) {
		return;
	}

	if (data->data) {
		free(data->data);
		data->data = NULL;
	}
	if (data->priv) {
		free(data->priv);
		data->priv = NULL;
	}
	data->data_len = 0;
	data->priv_len = 0;
}

int sl_test_malloc_buffer(hal_data *data, int buf_len)
{
	if (!data) {
		return -1;
	}
	data->data = (unsigned char *)zalloc(buf_len);
	if (!data->data) {
		return -2;
	}
	data->data_len = buf_len;

	return 0;
}

int sl_test_malloc_buffer_priv(hal_data *data, int buf_len)
{
	if (!data) {
		return -1;
	}
	data->priv = (unsigned char *)zalloc(buf_len);
	if (!data->priv) {
		return -2;
	}
	data->priv_len = buf_len;
	return 0;
}

void sl_test_init_buffer(hal_data *data)
{
	if (data) {
		data->data = NULL;
		data->data_len = 0;
		data->priv = NULL;
		data->priv_len = 0;
	}
}

void sl_test_print_buffer(char *data, int buf_len, const char *message)
{
	printf("================================\n");
	printf("%s length(%d)\n", message, buf_len);
	printf("--------------------------------\n");
	for (int i = 0; i < buf_len; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("%02x ", data[i]);
	}
	printf("\n");
	printf("================================\n");
}

void sl_run_all(sl_options *opt, sl_test_func *func, int tmax)
{
	for (int i = 0; i < tmax; i++) {
		func[i](opt);
	}
}

int sl_parse_command(sl_options *opt, char **command, int tmax, int terr)
{
	int argc = opt->argc;
	char **argv = opt->argv;

	if (argc < 4) {
		return -1;
	}
	opt->count = atoi(argv[3]);

	if (strncmp(argv[2], "all", strlen("all") + 1) == 0) {
		return tmax;
	}

	for (int i = 0; i < tmax; i++) {
		if (strncmp(argv[2], command[i], strlen(command[i]) + 1) == 0) {
			return i;
		}
	}
	return terr;
}
