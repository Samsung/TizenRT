/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file ttrace_tc_main.c
/// @brief Test Case Example for T-trace APIs

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <time.h>
#include <ttrace.h>
#include <semaphore.h>
#include <apps/shell/tash.h>
#include "tc_common.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/
extern sem_t tc_sem;
extern int working_tc;

/**
* @testcase         tc_libc_trace_begin
* @brief            Record Tracepoint's message and begin time
* @scenario         Generates a dummy Tracepoint
* @apicovered       trace_begin
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_trace_begin(void)
{
	int ret;
	char *msg = "libc_ttrace_begin_tc";

	ret = trace_begin(TTRACE_TAG_APPS, msg);
	TC_ASSERT_GEQ("trace_begin", ret, 0)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_trace_begin_u
* @brief            Record Tracepoint's unique id and begin time
* @scenario         Generates a dummy Tracepoint
* @apicovered       trace_begin_u
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_trace_begin_u(void)
{
	int ret;
	const int unique_id = 1;

	ret = trace_begin_u(TTRACE_TAG_APPS, unique_id);
	TC_ASSERT_GEQ("trace_begin_u", ret, 0)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_trace_end
* @brief            Record Tracepoint's end time
* @scenario         Generates a dummy Tracepoint
* @apicovered       trace_end
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_trace_end(void)
{
	int ret;

	ret = trace_end(TTRACE_TAG_APPS);
	TC_ASSERT_GEQ("trace_end", ret, 0)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_trace_end_u
* @brief            Record Tracepoint's end time
* @scenario         Generates a dummy Tracepoint
* @apicovered       trace_end_u
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_trace_end_u(void)
{
	int ret;

	ret = trace_end_u(TTRACE_TAG_APPS);
	TC_ASSERT_GEQ("trace_end_u", ret, 0)
	TC_SUCCESS_RESULT();
}

/**
* @testcase         tc_libc_trace_sched
* @brief            Trace scheduler's task transition
* @scenario         Generates a dummy transition
* @apicovered       trace_sched
* @precondition     NA
* @postcondition    NA
*/
static void tc_libc_trace_sched(void)
{
	int ret;
	struct tcb_s prev_tcb;
	struct tcb_s next_tcb;

	prev_tcb.pid = 0;
	prev_tcb.sched_priority = 0;
	prev_tcb.task_state = 3;
	strcpy(prev_tcb.name, "Idle Task");

	next_tcb.pid = 1;
	next_tcb.sched_priority = 1;
	next_tcb.task_state = 1;
	strcpy(next_tcb.name, "Dummy Task");

	ret = trace_sched(&prev_tcb, &next_tcb);
	TC_ASSERT_GEQ("trace_sched", ret, 0);
	TC_SUCCESS_RESULT();
}

static int ttrace_tc_launcher(int argc, char **args)
{
	total_pass = 0;
	total_fail = 0;
	tc_libc_trace_begin();
	tc_libc_trace_begin_u();
	tc_libc_trace_end();
	tc_libc_trace_end_u();
	tc_libc_trace_sched();
	printf("#########################################\n");
	printf("           FS TC Result               \n");
	printf("           PASS : %d FAIL : %d        \n", total_pass, total_fail);
	printf("#########################################\n");
	return total_pass;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ttrace_tc_main(int argc, char *argv[])
#endif
{
	sem_wait(&tc_sem);
	working_tc++;

#ifdef CONFIG_TASH
	tash_cmd_install("ttrace_tc_main", ttrace_tc_launcher, TASH_EXECMD_SYNC);
#else
	ttrace_tc_launcher(argc, argv);
#endif

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}
