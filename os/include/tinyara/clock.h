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
 *
 *   Copyright (C) 2007-2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
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

/**
 * @defgroup CLOCK_KERNEL CLOCK
 * @brief Provides APIs for Clock
 * @ingroup KERNEL
 *
 * @{
 */

///@file tinyara/clock.h
///@brief Clock APIs
#ifndef _INCLUDE_CLOCK_H
#define _INCLUDE_CLOCK_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <time.h>

#include <tinyara/compiler.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* Efficient, direct access to OS global timer variables will be supported
 * if the execution environment has direct access to kernel global data.
 * The code in this execution context can access the kernel global data
 * directly if:
 *
 * 1. We are not running tick-less (in which case there is no global timer
 *    data),
 * 2. This is an un-protected, non-kernel build,
 * 3. This is a protected build, but this code is being built for execution
 *    within the kernel space.
 * 4. It we are building with SYSCALLs enabled, but not in a kernel build,
 *    then we can't know a priori whether the code has access to the
 *    global variables or not.  In that case we have to assume not.
 */

#undef __HAVE_KERNEL_GLOBALS
#if defined(CONFIG_SCHED_TICKLESS)
/* Case 1: There is no global timer data */

#elif defined(CONFIG_BUILD_PROTECTED) && defined(__KERNEL__)
/* Case 3: Kernel mode of protected kernel build */

#define __HAVE_KERNEL_GLOBALS 1

#elif defined(CONFIG_BUILD_KERNEL) && defined(__KERNEL__)
/* Case 3: Kernel only build */

#define __HAVE_KERNEL_GLOBALS 1

#elif defined(CONFIG_LIB_SYSCALL)
/* Case 4: Building with SYSCALLs enabled, but not part of a kernel build */

#else
/* Case 2: Un-protected, non-kernel build */

#define __HAVE_KERNEL_GLOBALS 1
#endif

/* If CONFIG_SYSTEM_TIME64 is selected and the CPU supports long long types,
 * then a 64-bit system time will be used.
 */

#ifndef CONFIG_HAVE_LONG_LONG
#undef CONFIG_SYSTEM_TIME64
#endif

/* Timing constants *********************************************************/

#define NSEC_PER_SEC          1000000000
#define USEC_PER_SEC             1000000
#define MSEC_PER_SEC                1000
#define DSEC_PER_SEC                  10
#define NSEC_PER_DSEC          100000000
#define USEC_PER_DSEC             100000
#define MSEC_PER_DSEC                100
#define NSEC_PER_MSEC            1000000
#define USEC_PER_MSEC               1000
#define NSEC_PER_USEC               1000

/* If CONFIG_SCHED_TICKLESS is not defined, then the interrupt interval of
 * the system timer is given by USEC_PER_TICK.  This is the expected number
 * of microseconds between calls from the processor-specific logic to
 * sched_process_timer().  The default value of USEC_PER_TICK is 10000
 * microseconds (100KHz).  However, this default setting can be overridden
 * by defining the interval in microseconds as CONFIG_USEC_PER_TICK in the
 * TinyAra configuration file.
 *
 * The following calculations are only accurate when (1) there is no
 * truncation involved and (2) the underlying system timer is an even
 * multiple of microseconds.  If (2) is not true, you will probably want
 * to redefine all of the following.
 */

#ifdef CONFIG_USEC_PER_TICK
#define USEC_PER_TICK        (CONFIG_USEC_PER_TICK)
#else
#define USEC_PER_TICK        (10000)
#endif

/* MSEC_PER_TICK can be very inaccurate if CONFIG_USEC_PER_TICK is not an
 * even multiple of milliseconds.  Calculations using USEC_PER_TICK are
 * preferred for that reason (at the risk of overflow)
 */

#define TICK_PER_DSEC         (USEC_PER_DSEC / USEC_PER_TICK)	/* Truncates! */
#define TICK_PER_SEC          (USEC_PER_SEC  / USEC_PER_TICK)	/* Truncates! */
#define TICK_PER_MSEC         (USEC_PER_MSEC / USEC_PER_TICK)	/* Truncates! */
#define MSEC_PER_TICK         (USEC_PER_TICK / USEC_PER_MSEC)	/* Truncates! */
#define NSEC_PER_TICK         (USEC_PER_TICK * NSEC_PER_USEC)	/* Exact */

#define NSEC2TICK(nsec)       (((nsec)+(NSEC_PER_TICK/2))/NSEC_PER_TICK)	/* Rounds */
#define USEC2TICK(usec)       (((usec)+(USEC_PER_TICK/2))/USEC_PER_TICK)	/* Rounds */

#if (MSEC_PER_TICK * USEC_PER_MSEC) == USEC_PER_TICK
#define MSEC2TICK(msec)     (((msec)+(MSEC_PER_TICK/2))/MSEC_PER_TICK)	/* Rounds */
#else
#define MSEC2TICK(msec)     USEC2TICK((msec) * USEC_PER_MSEC)	/* Rounds */
#endif

#define DSEC2TICK(dsec)       MSEC2TICK((dsec) * MSEC_PER_DSEC)	/* Rounds */
#define SEC2TICK(sec)         MSEC2TICK((sec)  * MSEC_PER_SEC)	/* Rounds */

#define TICK2NSEC(tick)       ((tick) * NSEC_PER_TICK)	/* Exact */
#define TICK2USEC(tick)       ((tick) * USEC_PER_TICK)	/* Exact */

#if (MSEC_PER_TICK * USEC_PER_MSEC) == USEC_PER_TICK
#define TICK2MSEC(tick)     ((tick)*MSEC_PER_TICK)	/* Exact */
#else
#define TICK2MSEC(tick)     (((tick)*USEC_PER_TICK)/USEC_PER_MSEC)	/* Rounds */
#endif

#define TICK2DSEC(tick)       (((tick)+(TICK_PER_DSEC/2))/TICK_PER_DSEC)	/* Rounds */
#define TICK2SEC(tick)        (((tick)+(TICK_PER_SEC/2))/TICK_PER_SEC)	/* Rounds */

/****************************************************************************
 * Public Types
 ****************************************************************************/
/* This structure is used to report CPU usage for a particular thread */

#ifdef CONFIG_SCHED_CPULOAD
struct cpuload_s {
	volatile uint32_t total[CONFIG_SMP_NCPUS];   /* Total number of clock ticks per cpu */
	volatile uint32_t active[CONFIG_SMP_NCPUS];  /* Number of ticks while this thread was active on the specific cpu */
};

#ifdef CONFIG_SCHED_MULTI_CPULOAD
#define SCHED_NCPULOAD 3
#else
#define SCHED_NCPULOAD 1
#endif
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Access to raw system clock ***********************************************/
/* Direct access to the system timer/counter is supported only if (1) the
 * system timer counter is available (i.e., we are not configured to use
 * a hardware periodic timer), and (2) the execution environment has direct
 * access to kernel global data
 */

#ifdef __HAVE_KERNEL_GLOBALS
EXTERN volatile clock_t g_system_timer;

#ifndef CONFIG_SYSTEM_TIME64
#define clock_systimer() g_system_timer
#endif
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Function:  clock_synchronize
 *
 * Description:
 *   Synchronize the system timer to a hardware RTC.  This operation is
 *   normally performed automatically by the system during clock
 *   initialization.  However, the user may also need to explicitly re-
 *   synchronize the system timer to the RTC under certain conditions where
 *   the system timer is known to be in error.  For example, in certain low-
 *   power states, the system timer may be stopped but the RTC will continue
 *   keep correct time.  After recovering from such low-power state, this
 *   function should be called to restore the correct system time.
 *
 *   Calling this function could result in system time going "backward" in
 *   time, especially with certain lower resolution RTC implementations.
 *   Time going backward could have bad consequences if there are ongoing
 *   timers and delays.  So use this interface with care.
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_RTC
/**
 * @cond
 * @internal
 */
void clock_synchronize(void);
/**
 * @endcond
 */
#endif

/****************************************************************************
 * Function:  clock_systimer
 *
 * Description:
 *   Return the current value of the 32/64-bit system timer counter.
 *   Indirect access to the system timer counter is required through this
 *   function if the execution environment does not have direct access to
 *   kernel global data.
 *
 *   Use of this function is also required to assue atomic access to the
 *   64-bit system timer.
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

#if !defined(__HAVE_KERNEL_GLOBALS) || defined(CONFIG_SYSTEM_TIME64)
/**
 * @ingroup CLOCK_KERNEL
 * @brief Return the current value of the 32/64-bit system timer counter
 * @details @b #include <tinyara/clock.h> \n
 * SYSTEM CALL API
 *
 * @return The current value of the system timer counter is returned.
 * @since TizenRT v1.0
 */
clock_t clock_systimer(void);
#endif

/****************************************************************************
 * Name: clock_systimespec
 *
 * Description:
 *   Return the current value of the system timer counter as a struct
 *   timespec.
 *
 * Parameters:
 *   ts - Location to return the time
 *
 * Return Value:
 *   Current version always returns OK
 *
 * Assumptions:
 *
 ****************************************************************************/
/**
 * @cond
 * @internal
 */
int clock_systimespec(FAR struct timespec *ts);
/**
 * @endcond
 */

/****************************************************************************
 * Function:  clock_cpuload
 *
 * Description:
 *   Return load measurement data for the select PID.
 *
 * Parameters:
 *   pid - The task ID of the thread of interest.  pid == 0 is the IDLE thread.
 *   cpuload - The location to return the CPU load
 *
 * Return Value:
 *   OK (0) on success; a negated errno value on failure.  The only reason
 *   that this function can fail is if 'pid' no longer refers to a valid
 *   thread.
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_SCHED_CPULOAD
/**
 * @cond
 * @internal
 */
int clock_cpuload(int pid, int index, FAR struct cpuload_s *cpuload);
/**
 * @endcond
 */
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* _INCLUDE_CLOCK_H */

/**
 * @}
 */
