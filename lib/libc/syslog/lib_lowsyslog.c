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
 * lib/syslog/lib_lowsyslog.c
 *
 *   Copyright (C) 2007-2009, 2011-2012 Gregory Nutt. All rights reserved.
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

#include <tinyara/streams.h>
#if defined(CONFIG_LOGM) && defined(CONFIG_SYSLOG2LOGM)
#include <tinyara/logm.h>
#endif
#include "syslog/syslog.h"

#if defined(CONFIG_ARCH_LOWPUTC) || defined(CONFIG_SYSLOG)

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
 * Name: lowvsyslog_internal
 *
 * Description:
 *   This is the internal implementation of lowvsyslog (see the description
 *   of lowsyslog and lowvsyslog below)
 *
 ****************************************************************************/

static inline int lowvsyslog_internal(FAR const char *fmt, va_list ap)
{
	struct lib_outstream_s stream;

	/* Wrap the stdout in a stream object and let lib_vsprintf do the work. */
#if defined(CONFIG_SYSLOG)
	lib_syslogstream((FAR struct lib_outstream_s *)&stream);
#else
	lib_lowoutstream((FAR struct lib_outstream_s *)&stream);
#endif
	return lib_vsprintf((FAR struct lib_outstream_s *)&stream, fmt, ap);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lowvsyslog
 *
 * Description:
 *   The function lowvsyslog() performs the same task as lowsyslog() with
 *   the difference that it takes a set of arguments which have been
 *   obtained using the stdarg variable argument list macros.
 *
 ****************************************************************************/

int lowvsyslog(int priority, FAR const char *fmt, va_list ap)
{
	int ret = 0;

	/* Check if this priority is enabled */

	if ((g_syslog_mask & LOG_MASK(priority)) != 0) {
		/* Yes.. let vsylog_internal to the deed */
#if defined(CONFIG_LOGM) && defined(CONFIG_SYSLOG2LOGM)
		ret = logm_internal(LOGM_LOWPUT, LOGM_UNKNOWN, priority, fmt, ap);
#else
#ifdef __KERNEL__
		irqstate_t flags = enter_critical_section();
#endif
		ret = lowvsyslog_internal(fmt, ap);
#ifdef __KERNEL__
		leave_critical_section(flags);
#endif
#endif
	}
	return ret;
}

/****************************************************************************
 * Name: lowsyslog
 *
 * Description:
 *   syslog() generates a log message. The priority argument is formed by
 *   ORing the facility and the level values (see include/syslog.h). The
 *   remaining arguments are a format, as in printf and any arguments to the
 *   format.
 *
 *   This is a non-standard, low-level system logging interface.  The
 *   difference between syslog() and lowsyslog() is that the syslog()
 *   interface writes to the syslog device (usually fd=1, stdout) whereas
 *   lowsyslog() uses a lower level interface that works even from interrupt
 *   handlers.
 *
 ****************************************************************************/

int lowsyslog(int priority, FAR const char *fmt, ...)
{
	va_list ap;
	int ret;
	va_start(ap, fmt);

	/* Let lowvsyslog do the work */
	ret = lowvsyslog(priority, fmt, ap);

	va_end(ap);

	return ret;
}

#endif							/* CONFIG_ARCH_LOWPUTC || CONFIG_SYSLOG */
