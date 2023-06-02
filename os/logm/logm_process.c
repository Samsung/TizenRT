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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <arch/irq.h>
#include <tinyara/logm.h>
#include <tinyara/config.h>
#include <tinyara/kmalloc.h>
#include "logm.h"
#ifdef CONFIG_LOGM_TEST
#include "logm_test.h"
#endif

uint8_t logm_status;
int logm_bufsize = LOGM_BUFFER_SIZE;
char * g_logm_rsvbuf = NULL;
volatile int logm_print_interval = LOGM_PRINT_INTERVAL * 1000;

static int logm_change_bufsize(int buflen)
{
	/* Keep using old size if a parameter is invalid */
	if (buflen < 0) {
		LOGM_STATUS_CLEAR(LOGM_BUFFER_RESIZE_REQ);
		return ERROR;
	}

	/* Realloc new buffer with new length */
	char *new_g_logm_rsvbuf = (char *)kmm_realloc(g_logm_rsvbuf, buflen);
	if (new_g_logm_rsvbuf == NULL) {
		wdbg("Realloc Fail\n");
		return ERROR;
	}
	g_logm_rsvbuf = new_g_logm_rsvbuf;
	memset(g_logm_rsvbuf, 0, buflen);

	/* Reinitialize all  */
	g_logm_head = 0;
	g_logm_tail = 0;
	logm_bufsize = buflen;
	g_logm_dropmsg_count = 0;
	g_logm_overflow_offset = -1;

	LOGM_STATUS_CLEAR(LOGM_BUFFER_RESIZE_REQ);

	return OK;
}

int logm_task(int argc, char *argv[])
{
	irqstate_t flags;

	g_logm_rsvbuf = (char *)kmm_malloc(logm_bufsize);
	memset(g_logm_rsvbuf, 0, logm_bufsize);

	/* Now logm is ready */
	LOGM_STATUS_SET(LOGM_READY);

#ifdef CONFIG_LOGM_TEST
	logmtest_init();
#endif

	while (1) {
		while (g_logm_head != g_logm_tail) {
			fputc(g_logm_rsvbuf[g_logm_head], stdout);
			g_logm_head = (g_logm_head + 1) % logm_bufsize;
			if (LOGM_STATUS(LOGM_BUFFER_OVERFLOW)) {
				LOGM_STATUS_CLEAR(LOGM_BUFFER_OVERFLOW);
			}
			if (g_logm_overflow_offset >= 0 && g_logm_overflow_offset == g_logm_head) {
				fprintf(stdout, "\n[LOGM BUFFER OVERFLOW] %d messages are dropped\n", g_logm_dropmsg_count);
				g_logm_overflow_offset = -1;
			}
		}

		if (LOGM_STATUS(LOGM_BUFFER_RESIZE_REQ)) {
			flags = enter_critical_section();
			if (logm_change_bufsize(new_logm_bufsize) != OK) {
				fprintf(stdout, "\n[LOGM] Failed to change buffer size\n");
			}
			leave_critical_section(flags);
		}
		usleep(logm_print_interval);
	}

	kmm_free(g_logm_rsvbuf);
	return 0;					// Just to make compiler happy
}
