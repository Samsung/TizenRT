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
 * pm/pm_sleep.c
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

#include <tinyara/pm/pm.h>
#include <tinyara/irq.h>
#include <tinyara/wdog.h>
#include <tinyara/clock.h>
#include <errno.h>

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

static void pm_timer_callback(int argc, uint32_t sem, uint32_t pid)
{
#ifdef CONFIG_PM_METRICS
	pm_metrics_update_sleep((pid_t)pid);
#endif
	/* As the timer is expired, give back the semaphore to unlock the thread */
	sem_post((sem_t*)sem);
}

/************************************************************************
 * Name: pm_sleep
 *
 * Description:
 *   This function allows the board to sleep for given time interval.
 *   When this function is called, it is expected that board will sleep for 
 *   given duration of time. But for some cases board might not go 
 *   to sleep instantly if :
 * 	1. system is in pm lock (pm state transition is locked)
 *      2. Other threads(other than idle) are running
 *      3. NORMAL to SLEEP state threshold time is large
 * 
 * Parameters:
 *   milliseconds - expected board sleep duration
 *
 * Return Value:
 *   OK - success
 *   ERROR - error
 *
 ************************************************************************/

int pm_sleep(int milliseconds)
{
	int ret;
	WDOG_ID wdog;
	sem_t pm_sem;

	/* initialize the timer's semaphore. It will be used to lock the
	 * thread before sleep and unlock after expire */
	sem_init(&pm_sem, 0, 0);

	wdog = wd_create();
	if (!wdog) {
		set_errno(EAGAIN);
		return ERROR;
	}

	if (wd_setwakeupsource(wdog) != OK) {
		wd_delete(wdog);
		return ERROR;
	}

	ret = wd_start(wdog, MSEC2TICK(milliseconds), (wdentry_t)pm_timer_callback, 2, (uint32_t)&pm_sem, (uint32_t)getpid());
	if (ret != OK) {
		pmdbg("pm_sleep: wd_start failed\n");
		wd_delete(wdog);
		return ERROR;
	}

	/* sem_wait untill the timer expires */
	do {
		ret = sem_wait(&pm_sem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);

	/* When the semaphore is freed, make the pm timer free */
	wd_delete(wdog);

    return OK;
}
