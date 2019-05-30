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
 * apps/examples/messaging_sample/messaging_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "messaging_sample_internal.h"

#define EXEC_NORMAL   0
#define EXEC_INFINITE 1

static volatile bool inf_flag;
static volatile bool is_running;
int fail_cnt = 0;

static void messaging_sample_show_success_ratio(int loop_cnt)
{
	printf("\n=== Messaging Sample is finished. (run %d-times)\n", loop_cnt);
	printf("- success : %d, fail : %d\n", loop_cnt - fail_cnt, fail_cnt);
}

static void messaging_sample_run(int loop_cnt)
{
	printf("\n==Messaging Sample %d-th Iteration!==\n", loop_cnt);

	noreply_nonblock_messaging_sample();

	/* Wait for finishing previous test. */
	sleep(1);

	sync_block_messaging_sample();

	/* Wait for finishing previous test. */
	sleep(1);

	multicast_messaging_sample();

	/* Wait for finishing previous test. */
	sleep(1);
}

static void messaging_sample_execute_infinitely(void)
{
	int repetition_num;

	/* messaging_sample executes infinitely until receiving stop. */
	printf("\n=== Start Messaging Sample : executes infinitely. ===\n");
	repetition_num = 0;
	is_running = true;
	while (inf_flag) {
		repetition_num++;
		messaging_sample_run(repetition_num);
	}
	messaging_sample_show_success_ratio(repetition_num);
	is_running = false;
}

static void messaging_sample_execute_ntimes(int repetition_num)
{
	int loop_idx;
	is_running = true;
	/* messaging_sample executes multiple-times. */
	printf("\n=== Start Messaging Sample : executes %d-times. ===\n", repetition_num);
	for (loop_idx = 1; loop_idx <= repetition_num; loop_idx++) {
		messaging_sample_run(loop_idx);
	}
	messaging_sample_show_success_ratio(repetition_num);
	is_running = false;
}
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int messaging_main(int argc, char *argv[])
#endif
{
	int repetition_num;
	int option;
	char *cmd_arg = NULL;
	char *cnt_arg = NULL;
	int execution_type = EXEC_NORMAL;

	if (argc >= 4 || argc == 2) {
		goto usage;
	}

	while ((option = getopt(argc, argv, "r:n:")) != ERROR) {
		switch (option) {
		case 'r':
			execution_type = EXEC_INFINITE;
			cmd_arg = optarg;
			break;
		case 'n':
			execution_type = EXEC_NORMAL;
			cnt_arg = optarg;
			break;
		case '?':
		default:
			goto usage;
		}
	}

	/* Execute the messaging sample. */
	if (execution_type == EXEC_INFINITE) {
		if (cmd_arg && strncmp(cmd_arg, "start", strlen("start") + 1) == 0) {
			if (is_running) {
				goto already_running;
			}
			inf_flag = true;
			messaging_sample_execute_infinitely();
		} else if (cmd_arg && strncmp(cmd_arg, "stop", strlen("stop") + 1) == 0) {
			if (inf_flag == false) {
				printf("There is no infinite running Messaging sample. Cannot stop the sample.\n");
				return -1;
			}
			inf_flag = false;
		} else {
			goto usage;
		}

	} else {
		if (is_running) {
			goto already_running;
		}

		if (cnt_arg != NULL) {
			repetition_num = atoi(cnt_arg);
			if (repetition_num <= 0) {
				goto usage;
			}
		} else {
			repetition_num = 1;
		}

		messaging_sample_execute_ntimes(repetition_num);
	}
	return 0;

usage:
	printf("\nUsage : messaging [OPTIONS]\n");
	printf("Options:\n");
	printf(" -r start : Execute messaging sample infinitely until stop cmd.\n");
	printf("    stop  : Stop the messaging sample infinite execution.\n");
	printf(" -n COUNT : Execute messaging sample COUNT-iterations.\n");
	return -1;
already_running:
	printf("There is already running Messaging Sample.\n");
	printf("New sample can run after that previous sample is finished.\n");
	return -1;
}
