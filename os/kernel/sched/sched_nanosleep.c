/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <time.h>
#include <errno.h>

#include <tinyara/clock.h>
#include <arch/irq.h>
#include <tinyara/wdog.h>
#include <tinyara/cancelpt.h>

#include "clock/clock.h"
#include "sched/sched.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Name: sleep_timeout
 *
 * Description:
 *   This is a callback funtion for the watchdog.
 *   It checks if the task is in SLEEP state and wakes it up.
 *
 * Parameters:
 *   argc - Ignored
 *   itcb - Pointer to the task that was put to sleep
 *
 * Returned Value:
 *   void
 ****************************************************************************/

static void sleep_timeout(int argc, uint32_t itcb)
{
	/* On many small machines, pointers are encoded and cannot be simply cast
	 * from uint32_t to struct tcb_s*.  The following union works around this
	 * (see wdogparm_t).  This odd logic could be conditioned on
	 * CONFIG_CAN_CAST_POINTERS, but it is not too bad in any case.
	 */

	union {
		FAR struct tcb_s *wtcb;
		uint32_t itcb;
	} u;

	u.itcb = itcb;
	ASSERT(u.wtcb);

	if (u.wtcb->task_state == TSTATE_SLEEP_NOSIGNAL) {
		up_unblock_task(u.wtcb);
	}
}

/****************************************************************************
 * Name: timedwait
 *
 * Description:
 *   This function suspends the calling process for a duration specified by timeout
 *
 * Parameters:
 *   timeout - The amount of time to suspend the task

 * Returned Value:
 *   If timeout is specified, its return value is zero.
 *   else it returns EINVAL
 *
 ****************************************************************************/

static int timedwait(FAR const struct timespec *timeout)
{
	FAR struct tcb_s *rtcb = this_task();
	int32_t waitticks;
	irqstate_t saved_state;
	int ret = EINVAL;

	DEBUGASSERT(rtcb->waitdog == NULL);

	(void)enter_cancellation_point();

	sched_lock();                           /* Not necessary */

	saved_state = irqsave();

	if (timeout) {
		/* Convert the timespec to system clock ticks, making sure that
		 * the resulting delay is greater than or equal to the requested
		 * time in nanoseconds.
		 */

#ifdef CONFIG_HAVE_LONG_LONG
		uint64_t waitticks64 = ((uint64_t) timeout->tv_sec * NSEC_PER_SEC + (uint64_t) timeout->tv_nsec + NSEC_PER_TICK - 1) / NSEC_PER_TICK;
		DEBUGASSERT(waitticks64 <= UINT32_MAX);
		waitticks = (uint32_t) waitticks64;
#else
		uint32_t waitmsec;

		DEBUGASSERT(timeout->tv_sec < UINT32_MAX / MSEC_PER_SEC);
		waitmsec = timeout->tv_sec * MSEC_PER_SEC + (timeout->tv_nsec + NSEC_PER_MSEC - 1) / NSEC_PER_MSEC;
		waitticks = MSEC2TICK(waitmsec);
#endif

		/* Create a watchdog */

		rtcb->waitdog = wd_create();
		DEBUGASSERT(rtcb->waitdog);

		if (rtcb->waitdog) {
			/* This little bit of nonsense is necessary for some
			 * processors where sizeof(pointer) < sizeof(uint32_t).
			 * see wdog.h.
			 */

			wdparm_t wdparm;
			wdparm.pvarg = (FAR void *)rtcb;

			/* Start the watchdog */

			wd_start(rtcb->waitdog, waitticks, (wdentry_t) sleep_timeout, 1, wdparm.dwarg);

			/* Now wait for the watchdog */

			up_block_task(rtcb, TSTATE_SLEEP_NOSIGNAL);

			/* We no longer need the watchdog */

			wd_delete(rtcb->waitdog);
			rtcb->waitdog = NULL;

			ret = OK;
		}

	}

	irqrestore(saved_state);
	sched_unlock();
	leave_cancellation_point();
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: nanosleep
 *
 * Description:
 *   The nanosleep() function causes the current thread to be suspended from
 *   execution until the time interval specified by the rqtp argument has elapsed
 *   The suspension time may be longer than requested because the
 *   argument value is rounded up to an integer multiple of the sleep
 *   resolution or because of the scheduling of other activity by the system. But,
 *   the suspension time will not be less than the time specified by rqtp, as
 *   measured by the system clock, CLOCK_REALTIME.
 *
 * Parameters:
 *   rqtp - The amount of time to be suspended from execution.
 *   rmtp - Ignored
 *	    This parameter is kept here to maintain compatibility with sig_nanosleep() API.
 *	    Since signals are disabled, the sleep operation cannot be interrupted.
 *	    So, we are sure that we will always sleep for at least the requested period.
 *
 * Returned Value:
 *   If the nanosleep() function returns because the requested time has
 *   elapsed, its return value is zero.
 *
 *   If nanosleep() fails, it returns a value of -1 and sets errno to
 *   indicate the error. The nanosleep() function will fail if:
 *
 *     EINVAL - The rqtp argument specified a nanosecond value less than
 *       zero or greater than or equal to 1000 million.
 *
 ****************************************************************************/

int nanosleep(FAR const struct timespec *rqtp, FAR struct timespec *rmtp)
{
	int errval;
	irqstate_t flags;

	/* nanosleep() is a cancellation point */
	(void)enter_cancellation_point();

	if (!rqtp || rqtp->tv_nsec < 0 || rqtp->tv_nsec >= 1000000000) {
		errval = EINVAL;
		goto errout;
	}

	/* Get the start time of the wait.  Interrupts are disabled to prevent
	 * timer interrupts while we do tick-related calculations before and
	 * after the wait.
	 */

	flags = irqsave();
	errval = timedwait(rqtp);

	if (errval == OK) {
		irqrestore(flags);
		leave_cancellation_point();
		return OK;
	}

	irqrestore(flags);
errout:
	set_errno(errval);
	leave_cancellation_point();
	return ERROR;
}
