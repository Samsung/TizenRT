/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
/************************************************************************
 * pm/pm_timedsuspend.c
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
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>
#include <tinyara/pm/pm.h>
#include <tinyara/wdog.h>
#include <tinyara/sched.h>
#include <errno.h>
#include "pm.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/* This array maps the domain to their respective wdog timer */
static WDOG_ID domain_timer_map[CONFIG_PM_NDOMAINS];

/************************************************************************
 * Private Functions
 ************************************************************************/

static void timer_timeout(int argc, int domain_id)
{
	DEBUGASSERT(domain_timer_map[domain_id] != NULL);
	/* PM transition will be resume here */
	if (pm_resume(domain_id) != OK) {
		pmlldbg("Unable to resume domain: %s\n", pm_domain_map[domain_id]);
	}
	(void)wd_delete(domain_timer_map[domain_id]);
	domain_timer_map[domain_id] = NULL;
}
/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: pm_timedsuspend
 *
 * Description:
 *   This function locks PM state transition for a specific duration.
 *
 * Parameters:
 *   domain_id - ID of domain to be suspended
 *   milliseconds - expected lock duration in millisecond
 *
 * Return Value:
 *   0 - success
 *  -1 - error
 *
 ************************************************************************/

int pm_timedsuspend(int domain_id, unsigned int milliseconds)
{
	irqstate_t flags;
	WDOG_ID wdog;
	int tick_remain;
	int ret = ERROR;
	int delay = MSEC2TICK(milliseconds);
	if ((domain_id < 0) || (domain_id >= CONFIG_PM_NDOMAINS) || (pm_domain_map[domain_id] == NULL)) {
		set_errno(EINVAL);
		pmdbg("Invalid domain_id: %d\n", domain_id);
		return ret;
	}
	flags = enter_critical_section();
	wdog = domain_timer_map[domain_id];
	/* If delay is zero then cancel the timer (PM Policy) */
	if (delay == 0) {
		if (wdog) {
			timer_timeout(NULL, domain_id);
		}
		ret = OK;
		goto exit;
	}
	/* If timer is not there, then create a timer & suspend domain */
	if (!wdog) {
		wdog = wd_create();
		if (wdog == NULL) {
			pmdbg("Unable to create WDog Timer, error = %d\n", get_errno());
			set_errno(EAGAIN);
			goto exit;
		}
		/* Unable to suspend domain, so delete the timer */
		if (pm_suspend(domain_id) != OK) {
			pmdbg("Unable to suspend domain: %s\n", pm_domain_map[domain_id]);
			(void)wd_delete(wdog);
			goto exit;
		}
		domain_timer_map[domain_id] = wdog;
	}
	/* New delay is less than running timer ticks left, no need to do anyting */
	tick_remain = wd_gettime(wdog);
	if (delay <= tick_remain) {
		pmvdbg("Domain: %s is already suspended for %d milliseconds\n", pm_domain_map[domain_id], TICK2MSEC(tick_remain));
		ret = OK;
		goto exit;
	}
	/* Start the timer */
	if (wd_start(wdog, delay, (wdentry_t)timer_timeout, 1, domain_id) != OK) {
		pmdbg("Error starting Wdog timer\n");
		set_errno(EAGAIN);
		timer_timeout(NULL, domain_id);
		goto exit;
	}
	pmvdbg("Domain: %s is suspended for %d milliseconds\n", pm_domain_map[domain_id], milliseconds);
	ret = OK;
exit:
	leave_critical_section(flags);
	return ret;
}
