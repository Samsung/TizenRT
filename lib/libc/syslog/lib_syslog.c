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
/****************************************************************************
 * libc/syslog/lib_syslog.c
 *
 *   Copyright (C) 2007-2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#include <stdio.h>
#include <syslog.h>
#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/streams.h>
#if defined(CONFIG_LOGM) && defined(CONFIG_SYSLOG2LOGM)
#include <tinyara/logm.h>
#endif
#include "syslog/syslog.h"
#if CONFIG_NFILE_STREAMS > 0
#include "lib_internal.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Constant Data
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Constant Data
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vsyslog_internal
 *
 * Description:
 *   This is the internal implementation of vsyslog (see the description of
 *   syslog and vsyslog below)
 *
 ****************************************************************************/

static inline int vsyslog_internal(FAR const char *fmt, va_list ap)
{
#if defined(CONFIG_SYSLOG)
	struct lib_outstream_s stream;
#elif CONFIG_NFILE_DESCRIPTORS > 0
	struct lib_rawoutstream_s stream;
#elif defined(CONFIG_ARCH_LOWPUTC)
	struct lib_outstream_s stream;
#endif

	int ret;

#if defined(CONFIG_SYSLOG_TIMESTAMP)
	struct timespec ts;

	/* Get the current time */

	ret = clock_systimespec(&ts);
#endif

#if defined(CONFIG_SYSLOG)
	/* Wrap the low-level output in a stream object and let lib_vsprintf
	 * do the work.
	 */

	lib_syslogstream((FAR struct lib_outstream_s *)&stream);

#if defined(CONFIG_SYSLOG_TIMESTAMP)
	/* Pre-pend the message with the current time */

	if (ret == OK) {
		(void)lib_sprintf((FAR struct lib_outstream_s *)&stream, "[%6d.%06d]", ts.tv_sec, ts.tv_nsec / 1000);
	}
#endif

	return lib_vsprintf((FAR struct lib_outstream_s *)&stream, fmt, ap);

#elif CONFIG_NFILE_DESCRIPTORS > 0
	/* Wrap the stdout in a stream object and let lib_vsprintf
	 * do the work.
	 */

	lib_rawoutstream(&stream, 1);

#if CONFIG_NFILE_STREAMS > 0 && (defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__))
	if(!up_interrupt_context() && getpid() != 0){
		lib_take_semaphore(stdout);
	}
#elif CONFIG_NFILE_STREAMS > 0
	lib_take_semaphore(stdout);
#endif

#if defined(CONFIG_SYSLOG_TIMESTAMP)
	/* Pre-pend the message with the current time */

	if (ret == OK) {
		(void)lib_sprintf((FAR struct lib_outstream_s *)&stream, "[%6d.%06d]", ts.tv_sec, ts.tv_nsec / 1000);
	}
#endif
	ret = lib_vsprintf(&stream.public,fmt,ap);

#if CONFIG_NFILE_STREAMS > 0 && (defined(CONFIG_BUILD_FLAT) || defined(__KERNEL__))
        if(!up_interrupt_context() && getpid() != 0){
                lib_give_semaphore(stdout);
        }
#elif CONFIG_NFILE_STREAMS > 0
	lib_give_semaphore(stdout);
#endif
	return ret;

#elif defined(CONFIG_ARCH_LOWPUTC)
	/* Wrap the low-level output in a stream object and let lib_vsprintf
	 * do the work.
	 */

	lib_lowoutstream((FAR struct lib_outstream_s *)&stream);

#if defined(CONFIG_SYSLOG_TIMESTAMP)
	/* Pre-pend the message with the current time */

	if (ret == OK) {
		(void)lib_sprintf((FAR struct lib_outstream_s *)&stream, "[%6d.%06d]", ts.tv_sec, ts.tv_nsec / 1000);
	}
#endif

	return lib_vsprintf((FAR struct lib_outstream_s *)&stream, fmt, ap);

#else							/* CONFIG_SYSLOG */
	return 0;
#endif							/* CONFIG_SYSLOG */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vsyslog
 *
 * Description:
 *   The function vsyslog() performs the same task as syslog() with the
 *   difference that it takes a set of arguments which have been obtained
 *   using the stdarg variable argument list macros.
 *
 ****************************************************************************/

int vsyslog(int priority, FAR const char *fmt, va_list ap)
{
	int ret = 0;

	/* Check if this priority is enabled */

	if ((g_syslog_mask & LOG_MASK(priority)) != 0) {
		/* Yes.. let vsylog_internal do the deed */
#if defined(CONFIG_LOGM) && defined(CONFIG_SYSLOG2LOGM)
		ret = logm_internal(LOGM_NORMAL, LOGM_UNKNOWN, priority, fmt, ap);
#else
		ret = vsyslog_internal(fmt, ap);
#endif
	}

	return ret;
}

/****************************************************************************
 * Name: syslog
 *
 * Description:
 *   syslog() generates a log message. The priority argument is formed by
 *   ORing the facility and the level values (see include/syslog.h). The
 *   remaining arguments are a format, as in printf and any arguments to the
 *   format.
 *
 *   The TinyAra implementation does not support any special formatting
 *   characters beyond those supported by printf.
 *
 ****************************************************************************/

int syslog(int priority, FAR const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsyslog(priority, fmt, ap);
	va_end(ap);

	return ret;
}
