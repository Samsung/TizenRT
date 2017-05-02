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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arch/irq.h>
#include <tinyara/logm.h>
#include <tinyara/config.h>
#include "logm.h"

volatile int logm_bufsize = LOGM_BUFFER_SIZE;
volatile int logm_print_interval = LOGM_PRINT_INTERVAL * 1000;
char *volatile g_logm_rsvbuf = NULL;
volatile uint8_t logm_status;

static int logm_change_bufsize(void)
{
	int buflen;

	/* Get new values */
	logm_get(LOGM_NEW_BUFSIZE, &buflen);

	/* Keep using old size because of invalid parameter */
	if (buflen < 0) {
		LOGM_STATUS_CLEAR(LOGM_BUFFER_RESIZE_REQ);
		return 0;
	}

	/* Realloc new buffer with new length */
	g_logm_rsvbuf = (char *)realloc(g_logm_rsvbuf, buflen);
	if (!g_logm_rsvbuf) {
		return -1;
	}
	memset(g_logm_rsvbuf, 0, buflen);
	lmdbg("logm new malloced address is 0x%x\n", g_logm_rsvbuf);

	/* Reinitialize all  */
	g_logm_head = 0;
	g_logm_tail = 0;
	logm_bufsize = buflen;
	g_logm_available = buflen;
	g_logm_enqueued_count = 0;
	g_logm_dropmsg_count = 0;
	g_logm_overflow_offset = -1;

	LOGM_STATUS_CLEAR(LOGM_BUFFER_RESIZE_REQ);	// finish

	return 0;
}

int logm_task(int argc, char *argv[])
{
	int ret = 0;
	irqstate_t flags;

	g_logm_rsvbuf = (char *)malloc(logm_bufsize);
	memset(g_logm_rsvbuf, 0, logm_bufsize);

	/* Now logm is ready */
	LOGM_STATUS_SET(LOGM_READY);
	g_logm_available = logm_bufsize;

#ifdef CONFIG_LOGM_TEST
	logmtest_init();
#endif

	while (1) {
		while (g_logm_enqueued_count > 0) {
			ret = 0;
			while (*(g_logm_rsvbuf + (g_logm_head + ret) % logm_bufsize)) {
				fputc(g_logm_rsvbuf[(g_logm_head + ret++) % logm_bufsize], stdout);
			}
			g_logm_head = (g_logm_head + ret + 1) % logm_bufsize;
			g_logm_available += (ret + 1);

			g_logm_enqueued_count--;

			if (LOGM_STATUS(LOGM_BUFFER_OVERFLOW)) {
				LOGM_STATUS_CLEAR(LOGM_BUFFER_OVERFLOW);
			}

			if (g_logm_overflow_offset >= 0 && g_logm_overflow_offset == g_logm_head) {
				fprintf(stdout, "\n[LOGM BUFFER OVERFLOW] %d messages are dropped\n", g_logm_dropmsg_count);
				g_logm_overflow_offset = -1;
			}
		}

		if (LOGM_STATUS(LOGM_BUFFER_RESIZE_REQ)) {
			flags = irqsave();
			logm_change_bufsize();
			irqrestore(flags);
		}
		usleep(logm_print_interval);
	}
	return 0;					// Just to make compiler happy
}
