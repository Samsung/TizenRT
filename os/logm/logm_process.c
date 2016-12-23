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
#include <unistd.h>
#include <stdio.h>
#include <tinyara/logm.h>
#include "logm.h"

volatile int g_logm_isready;
extern char g_logm_rsvbuf[][LOGM_MAX_MSG_LEN];
extern int g_logm_head;
extern int g_logm_tail;
extern int g_logm_count;

int logm_task(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	g_logm_head = 0;
	g_logm_tail = 0;
	g_logm_count = 0;

	/* Now logm is ready */
	g_logm_isready = 1;

#ifdef CONFIG_LOGM_TEST
	logmtest_init();
#endif

	while (1) {
		while (g_logm_count > 0) {
			fputs(g_logm_rsvbuf[g_logm_head], stdout);
			g_logm_head += 1;
			if (g_logm_head >= LOGM_RSVBUF_COUNT) {
				g_logm_head -= LOGM_RSVBUF_COUNT;
			}
			g_logm_count--;
		}
		sleep(1);
	}
	return 0;					// Just to make compiler happy
}
