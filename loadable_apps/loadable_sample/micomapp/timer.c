/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <stdlib.h>
#include <string.h>

#include "micomapp_internal.h"

int alloc_timer_args(char **timer_args)
{
	int arg_idx;
	for (arg_idx = 0; arg_idx < TIMER_ARG_NUM; arg_idx++) {
		timer_args[arg_idx] = NULL;
	}

	timer_args[0] = strndup("timer", strlen("timer") + 1);
	if (timer_args[0] == NULL) {
		goto timer_errout;
	}

	timer_args[1] = strndup("-n", strlen("-n") + 1);
	if (timer_args[1] == NULL) {
		goto timer_errout;
	}

	timer_args[2] = strndup(CONFIG_EXAMPLES_MICOM_TIMER_REPEAT, strlen(CONFIG_EXAMPLES_MICOM_TIMER_REPEAT) + 1);
	if (timer_args[2] == NULL) {
		goto timer_errout;
	}

	timer_args[3] = strndup("-t", strlen("-t") + 1);
	if (timer_args[3] == NULL) {
		goto timer_errout;
	}

	timer_args[4] = strndup(CONFIG_EXAMPLES_MICOM_TIMER_INTERVAL, strlen(CONFIG_EXAMPLES_MICOM_TIMER_INTERVAL) + 1);
	if (timer_args[4] == NULL) {
		goto timer_errout;
	}

	return OK;

timer_errout:
	for (arg_idx = 0; arg_idx < TIMER_ARG_NUM; arg_idx++) {
		if (timer_args[arg_idx] != NULL) {
			free(timer_args[arg_idx]);
			timer_args[arg_idx] = NULL;
		}
	}
	return ERROR;
}

void free_timer_args(char **timer_args)
{
	int arg_idx;
	for (arg_idx = 0; arg_idx < TIMER_ARG_NUM; arg_idx++) {
		if (timer_args[arg_idx] != NULL) {
			free(timer_args[arg_idx]);
		}
	}
}
