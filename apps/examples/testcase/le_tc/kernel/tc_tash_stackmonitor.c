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

/// @file tc_tash_stackmonitor.c

/// @brief Test Case Example for stkmon tash cmd

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

#include <apps/shell/tash.h>

#include "tc_internal.h"

#define DEFAULT_STKSIZE 1024
#define TC_STACKMONITOR_TASKNAME "tc_stackmonitor"
#define TC_STACKMONITOR_PRIO 100

static void *stkmon_thread(void *arg)
{
	/* waiting 10s for testing */
	sleep(10);

	return NULL;
}

static int tc_tash_stackmonitor(int argc, char *args[])
{
	int ret = ERROR;
	pthread_t tc_thread;
	pthread_attr_t tc_thread_attr;

	pthread_attr_init(&tc_thread_attr);

	if (argc < 2) {
		printf("No param - Test with default value(stacksize:1024)\n");
		tc_thread_attr.stacksize = DEFAULT_STKSIZE;
	} else {
		tc_thread_attr.stacksize = atoi(args[1]);
		printf("Test with stacksize: %d\n", tc_thread_attr.stacksize);
	}

	printf("Expected Stack Size for tc_stackmonitor thread : %d\n", tc_thread_attr.stacksize - 4);
	tc_thread_attr.priority = TC_STACKMONITOR_PRIO;

	ret = pthread_create(&tc_thread, &tc_thread_attr, stkmon_thread, NULL);
	TC_ASSERT_EQ_RETURN("pthread_create", ret, OK, ERROR);
	pthread_setname_np(tc_thread, TC_STACKMONITOR_TASKNAME);

	pthread_detach(tc_thread);
	TC_SUCCESS_RESULT();
	return OK;
}

int tash_stackmonitor_main(void)
{
	tash_cmd_install("tc-stkmon", tc_tash_stackmonitor, TASH_EXECMD_SYNC);

	return 0;
}
