/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <tinyara/kthread.h>
#include <tinyara/logm.h>

#include "logm.h"

/* Global Variables */
static int g_logmtest_handle = 123;

/* LOGM test routine */
static int logmtest_kthread(int argc, char *argv[])
{
	while (1) {
		logm(1, 0, 3, "lom direct call test1 %d\n", g_logmtest_handle);
		logm(1, 0, 3, "lom direct call test2 %d\n", g_logmtest_handle);
		syslog(4, "sylog call test1 %d\n", g_logmtest_handle);
		syslog(4, "syslog call test2 %d\n", g_logmtest_handle);
		printf("Printf testcall1 %d\n", g_logmtest_handle);
		printf("printf testcall2 %d\n", g_logmtest_handle);

		sleep(2);
	}
	return 0;
}

/* Launch logm test thread */

static void logmtest_launch_kthread(void)
{
	if (kernel_thread("logmtest", 100, 1024, logmtest_kthread, (char *const *)NULL) < 0) {
		logm(1, 0, 3, "logmtest launch failed\n");
	}
}

/* init function  */

void logmtest_init(void)
{
	logmtest_launch_kthread();
}
