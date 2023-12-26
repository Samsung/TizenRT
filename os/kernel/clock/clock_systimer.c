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
 * kernel/clock/clock_systimer.c
 *
 *   Copyright (C) 2011, 2014 Gregory Nutt. All rights reserved.
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

#include <stdint.h>

#include <tinyara/arch.h>
#include <tinyara/clock.h>

#include "clock/clock.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* See include/tinyara/clock.h */

#undef clock_systimer

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: clock_systimer
 *
 * Description:
 *   Return the current value of the 32/64-bit system timer counter
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   The current value of the system timer counter
 *
 * Assumptions:
 *
 ****************************************************************************/

clock_t clock_systimer(void)
{
#ifdef CONFIG_SCHED_TICKLESS
#ifdef CONFIG_SYSTEM_TIME64

	struct timespec ts;

	/* Get the time from the platform specific hardware */

	(void)up_timer_gettime(&ts);

	/* Convert to a 64-bit value in microseconds, then in clock tick units */

	return USEC2TICK(1000000 * (uint64_t)ts.tv_sec + (uint64_t)ts.tv_nsec / 1000);

#else							/* CONFIG_SYSTEM_TIME64 */

	struct timespec ts;
	uint64_t tmp;

	/* Get the time from the platform specific hardware */

	(void)up_timer_gettime(&ts);

	/* Convert to a 64- then a 32-bit value */

	tmp = MSEC2TICK(1000 * (uint64_t)ts.tv_sec + (uint64_t)ts.tv_nsec / 1000000);
	return (clock_t)(tmp & 0x00000000ffffffff);

#endif							/* CONFIG_SYSTEM_TIME64 */
#else							/* CONFIG_SCHED_TICKLESS */
#ifdef CONFIG_SYSTEM_TIME64

	irqstate_t flags;
	clock_t sample;

	/* 64-bit accesses are not atomic on most architectures. */

	flags  = enter_critical_section();
	sample = g_system_timer;
	leave_critical_section(flags);
	return sample;

#else							/* CONFIG_SYSTEM_TIME64 */

	/* Return the current system time */

	return g_system_timer;

#endif							/* CONFIG_SYSTEM_TIME64 */
#endif							/* CONFIG_SCHED_TICKLESS */
}
