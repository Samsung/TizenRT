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

#include <tinyara/streams.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#include "logm.h"

char g_logm_rsvbuf[LOGM_RSVBUF_COUNT][LOGM_MAX_MSG_LEN];
int g_logm_head;
int g_logm_tail;
int g_logm_count;

/* logm_internal hook for syslog & printfs */
int logm_internal(int priority, const char *fmt, va_list ap)
{
	irqstate_t flags;
	int ret = 0;

	/* Need logic for set here */

	if (g_logm_isready) {
		flags = irqsave();

		/* choose a available buffer */
		/* This section will be optimized later */
		if (g_logm_count < LOGM_RSVBUF_COUNT) {
			ret = vsnprintf((char *)&g_logm_rsvbuf[g_logm_tail], LOGM_MAX_MSG_LEN - 1, fmt, ap);
			g_logm_tail += 1;
			if (g_logm_tail >= LOGM_RSVBUF_COUNT) {
				g_logm_tail -= LOGM_RSVBUF_COUNT;
			}
			g_logm_count++;
		}
		irqrestore(flags);
	} else {
		/* Low Output: Sytem is not yet completely ready  */
#ifdef CONFIG_ARCH_LOWPUTC
		struct lib_outstream_s strm;
		lib_lowoutstream(&strm);
		ret = lib_vsprintf(&strm, fmt, ap);
#endif
	}

	return ret;
}

/* logm api */
int logm(int flag, int indx, int priority, const char *fmt, ...)
{
	va_list ap;
	int ret;

	/* LOGIC for initial test here */

	va_start(ap, fmt);
	ret = logm_internal(priority, fmt, ap);
	va_end(ap);

	return ret;
}
