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
 * TinyAra Power Management Interfaces
 *
 *   Copyright (C) 2011-2012, 2015-2016 Gregory Nutt. All rights reserved.
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
/* Definition of terms.  Various "sleep" and low power consumption states
 * have various names and are sometimes used in conflicting ways.  In the
 * PM logic, we will use the following terminology:
 *
 * NORMAL  - The normal, full power operating mode.
 * IDLE    - This is still basically normal operational mode, the system is,
 *           however, IDLE and some simple simple steps to reduce power
 *           consumption provided that they do not interfere with normal
 *           Operation.  Simply dimming a backlight might be an example
 *           something that would be done when the system is idle.
 * STANDBY - Standby is a lower power consumption mode that may involve more
 *           extensive power management steps such has disabling clocking or
 *           setting the processor into reduced power consumption modes. In
 *           this state, the system should still be able to resume normal
 *           activity almost immediately.
 * SLEEP   - The lowest power consumption mode.  The most drastic power
 *           reduction measures possible should be taken in this state. It
 *           may require some time to get back to normal operation from
 *           SLEEP (some MCUs may even require going through reset).
 *
 * State changes always proceed from higher to lower power usage:
 *
 * NORMAL->IDLE->STANDBY->SLEEP
 *   ^       |      |        |
 *   |       V      V        V
 *   +-------+------+--------+
 */

#ifndef __INCLUDE_TINYARA_POWER_PM_H
#define __INCLUDE_TINYARA_POWER_PM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <queue.h>
#include <semaphore.h>

#if !defined(CONFIG_PM_NDOMAINS)
#define CONFIG_PM_NDOMAINS 31
#endif

/* This enumeration provides all power management domains */
enum pm_domain_e {
	PM_IDLE_DOMAIN 	= 0,
	PM_LCD_DOMAIN 	= 1,
	PM_UART_DOMAIN 	= 2,
	PM_WIFI_DOMAIN 	= 3,
	PM_BLE_DOMAIN 	= 4,
	PM_TASH_DOMAIN 	= 14,
	PM_APP_DOMAIN 	= 15,
	PM_NDOMAINS 	= CONFIG_PM_NDOMAINS,
};

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* CONFIG_PM_NDOMAINS. Defines the number of "domains" that activity may be
 * monitored on.  For example, you may want to separately manage the power
 * from the Network domain, shutting down the network when it is not be used,
 * from the UI domain, shutting down the UI when it is not in use.
 */

#ifndef CONFIG_PM_NDOMAINS
#define CONFIG_PM_NDOMAINS 1
#endif

#if CONFIG_PM_NDOMAINS < 1
#error CONFIG_PM_NDOMAINS invalid
#endif

/* CONFIG_IDLE_CUSTOM. Some architectures support this definition.  This,
 * if defined, will allow you replace the default IDLE loop with your
 * own, custom idle loop to support board-specific IDLE time power management
 */

/* CONFIG_PM_SLICEMS.  The power management module collects activity counts
 * in time slices.  At the end of the time slice, the count accumulated
 * during that interval is applied to an averaging algorithm to determine
 * the activity level.
 *
 * CONFIG_PM_SLICEMS provides the duration of that time slice in
 * milliseconds.  Default: 100 Milliseconds
 */

#ifndef CONFIG_PM_SLICEMS
#define CONFIG_PM_SLICEMS  100	/* Default is 100 msec */
#endif

#if CONFIG_PM_SLICEMS < 1
#error CONFIG_PM_SLICEMS invalid
#endif

/* The averaging algorithm is simply: Y = (An*X + SUM(Ai*Yi))/SUM(Aj), where
 * i = 1..n-1 and j= 1..n, n is the length of the "memory", Ai is the
 * weight applied to each value, and X is the current activity.  These weights
 * may be negative and a limited to the range of int16_t.
 *
 * CONFIG_PM_MEMORY provides the memory for the algorithm.  Default: 2
 * CONFIG_PM_COEFn provides weight for each sample.  Default: 1
 *
 * Setting CONFIG_PM_MEMORY=1 disables all smoothing.
 */

#ifndef CONFIG_PM_MEMORY
#define CONFIG_PM_MEMORY 2
#endif

#if CONFIG_PM_MEMORY < 1
#error "CONFIG_PM_MEMORY must be >= 1"
#endif

#ifndef CONFIG_PM_COEFN
#define CONFIG_PM_COEFN 1
#endif

#if CONFIG_PM_MEMORY > 1 && !defined(CONFIG_PM_COEF1)
#define CONFIG_PM_COEF1 1
#endif

#if CONFIG_PM_MEMORY > 2 && !defined(CONFIG_PM_COEF2)
#define CONFIG_PM_COEF2 1
#endif

#if CONFIG_PM_MEMORY > 3 && !defined(CONFIG_PM_COEF3)
#define CONFIG_PM_COEF3 1
#endif

#if CONFIG_PM_MEMORY > 4 && !defined(CONFIG_PM_COEF4)
#define CONFIG_PM_COEF4 1
#endif

#if CONFIG_PM_MEMORY > 5 && !defined(CONFIG_PM_COEF5)
#define CONFIG_PM_COEF5 1
#endif

#if CONFIG_PM_MEMORY > 6
#warning "This logic needs to be extended"
#endif

/* State changes then occur when the weight activity account crosses
 * threshold values for certain periods of time (time slice count).
 *
 * CONFIG_PM_xxxENTER_THRESH is the threshold value for entering state xxx.
 * CONFIG_PM_xxxENTER_COUNT is the count for entering state xxx.
 *
 * Resuming to normal state, on the other hand, is usually immediate and
 * controlled by wakeup conditions established by the platform.  The PM
 * module only recommends reduced power states.
 */

#ifndef CONFIG_PM_IDLEENTER_THRESH
#define CONFIG_PM_IDLEENTER_THRESH    1	/* <=1: Essentially no activity */
#endif

#ifndef CONFIG_PM_IDLEEXIT_THRESH
#define CONFIG_PM_IDLEEXIT_THRESH     2	/* >=2: Active */
#endif

#if CONFIG_PM_IDLEENTER_THRESH >= CONFIG_PM_IDLEEXIT_THRESH
#error "Must have CONFIG_PM_IDLEENTER_THRESH < CONFIG_PM_IDLEEXIT_THRESH"
#endif

#ifndef CONFIG_PM_IDLEENTER_COUNT
#define CONFIG_PM_IDLEENTER_COUNT     20	/* 20 IDLE slices to enter
											 * IDLE mode from normal
											 */
#endif

#ifndef CONFIG_PM_STANDBYENTER_THRESH
#define CONFIG_PM_STANDBYENTER_THRESH 1	/*  <=1: Essentially no activity */
#endif

#ifndef CONFIG_PM_STANDBYEXIT_THRESH
#define CONFIG_PM_STANDBYEXIT_THRESH  2	/* >=2: Active */
#endif

#if CONFIG_PM_STANDBYENTER_THRESH >= CONFIG_PM_STANDBYEXIT_THRESH
#error "Must have CONFIG_PM_STANDBYENTER_THRESH < CONFIG_PM_STANDBYEXIT_THRESH"
#endif

#ifndef CONFIG_PM_STANDBYENTER_COUNT
#define CONFIG_PM_STANDBYENTER_COUNT  20	/* 20 IDLE slices to enter
											 * STANDBY mode from IDLE
											 */
#endif

#ifndef CONFIG_PM_SLEEPENTER_THRESH
#define CONFIG_PM_SLEEPENTER_THRESH   1	/*  <=1: Essentially no activity */
#endif

#ifndef CONFIG_PM_SLEEPEXIT_THRESH
#define CONFIG_PM_SLEEPEXIT_THRESH    2	/* >=2: Active */
#endif

#if CONFIG_PM_SLEEPENTER_THRESH >= CONFIG_PM_SLEEPEXIT_THRESH
#error "Must have CONFIG_PM_SLEEPENTER_THRESH < CONFIG_PM_SLEEPEXIT_THRESH"
#endif

#ifndef CONFIG_PM_SLEEPENTER_COUNT
#define CONFIG_PM_SLEEPENTER_COUNT    20	/* 20 IDLE slices to enter SLEEP
											 * mode from STANDBY
											 */
#endif

/* Defines max length of device driver name for PM callback. */
#define MAX_PM_CALLBACK_NAME    32

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This enumeration provides all power management states.  Receipt of the
 * state indication is the state transition event.
 */

enum pm_state_e {
	PM_RESTORE = -1, 			/* PM_RESTORE is not a low power state.
								 *
								 * PM_RESTORE is used to notify for restore from low power state.
								 */
	PM_NORMAL = 0,				/* Normal full power operating mode.  If the driver is in
								 * a reduced power usage mode, it should immediately re-
								 * initialize for normal operatin.
								 *
								 * PM_NORMAL may be followed by PM_IDLE.
								 */
	PM_IDLE,					/* Drivers will receive this state change if it is
								 * appropriate to enter a simple IDLE power state.  This
								 * would include simple things such as reducing display back-
								 * lighting.  The driver should be ready to resume normal
								 * activity instantly.
								 *
								 * PM_IDLE may be followed by PM_STANDBY or PM_NORMAL.
								 */
	PM_STANDBY,					/* The system is entering standby mode. Standby is a lower
								 * power consumption mode that may involve more extensive
								 * power management steps such has disabling clocking or
								 * setting the processor into reduced power consumption
								 * modes. In this state, the system should still be able
								 * to resume normal activity almost immediately.
								 *
								 * PM_STANDBY may be followed PM_SLEEP or by PM_NORMAL
								 */
	PM_SLEEP,					/* The system is entering deep sleep mode.  The most drastic
								 * power reduction measures possible should be taken in this
								 * state. It may require some time to get back to normal
								 * operation from SLEEP (some MCUs may even require going
								 * through reset).
								 *
								 * PM_SLEEP may be following by PM_NORMAL
								 */
	PM_COUNT,
};

enum pm_timer_type_e {
	PM_NO_TIMER = 0,
	PM_WAKEUP_TIMER = 1,
	PM_LOCK_TIMER = 2,
	/* Scope for future expansion, up to 8 timer types can be supported */
};

/* This structure is used to send data to pm driver from app side for timedSuspend */
struct pm_suspend_arg_s {
	enum pm_domain_e domain;          /* domain to be suspended */
	unsigned int timer_interval;      /* duration to be suspended */
};

typedef struct pm_suspend_arg_s pm_suspend_arg_t;

struct pm_timer_s {	
	struct pm_timer_s *next;   			/* pointer to next timer in the linked list */
	int pid;                          		/* id of process that created pm timer */
	uint8_t flags;                    		/* See pm timer definations above*/
	int delay;		          		/* refers to time of sleep expected */
	void (*callback)(struct pm_timer_s *timer);     /* function to be executed when timer expires*/
	sem_t pm_sem;             			/* argument for callback function after timer expires*/
};

typedef struct pm_timer_s pm_timer_t;

/* This structure contain pointers callback functions in the driver.  These
 * callback functions can be used to provide power management information
 * to the driver.
 */

struct pm_callback_s {
	struct dq_entry_s entry;   /* Supports a doubly linked list */

	char name[MAX_PM_CALLBACK_NAME];	/* Name of driver which register callback */

	/**************************************************************************
	 * Name: prepare
	 *
	 * Description:
	 *   Request the driver to prepare for a new power state. This is a
	 *   warning that the system is about to enter into a new power state.  The
	 *   driver should begin whatever operations that may be required to enter
	 *   power state.  The driver may abort the state change mode by returning
	 *   a non-zero value from the callback function
	 *
	 * Input Parameters:
	 *   cb      - Returned to the driver.  The driver version of the callback
	 *             structure may include additional, driver-specific state
	 *             data at the end of the structure.
	 *   domain  - Identifies the activity domain of the state change
	 *   pmstate - Identifies the new PM state
	 *
	 * Returned Value:
	 *   0 (OK) means the event was successfully processed and that the driver
	 *   is prepared for the PM state change.  Non-zero means that the driver
	 *   is not prepared to perform the tasks needed achieve this power setting
	 *   and will cause the state change to be aborted.  NOTE:  The prepare
	 *   method will also be recalled when reverting from lower back to higher
	 *   power consumption modes (say because another driver refused a lower
	 *   power state change).  Drivers are not permitted to return non-zero
	 *   values when reverting back to higher power consumption modes!
	 *
	 **************************************************************************/

	int (*prepare)(FAR struct pm_callback_s *cb, enum pm_state_e pmstate);

	/**************************************************************************
	 * Name: notify
	 *
	 * Description:
	 *   Notify the driver of new power state.  This callback is called after
	 *   all drivers have had the opportunity to prepare for the new power
	 *   state.
	 *
	 * Input Parameters:
	 *   cb      - Returned to the driver.  The driver version of the callback
	 *             structure may include additional, driver-specific state
	 *             data at the end of the structure.
	 *   domain  - Identifies the activity domain of the state change
	 *   pmstate - Identifies the new PM state
	 *
	 * Returned Value:
	 *   None.  The driver already agreed to transition to the low power
	 *   consumption state when when it returned OK to the prepare() call.
	 *   At that time it should have made all preprations necessary to enter
	 *   the new state.  Now the driver must make the state transition.
	 *
	 **************************************************************************/

	void (*notify)(FAR struct pm_callback_s *cb, enum pm_state_e pmstate);
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/****************************************************************************
 * Name: pm_initialize
 *
 * Description:
 *   This function is called by MCU-specific logic at power-on reset in
 *   order to provide one-time initialization the power management subsystem.
 *   This function must be called *very* early in the initialization sequence
 *   *before* any other device drivers are initialized (since they may
 *   attempt to register with the power management subsystem).
 *
 * Input parameters:
 *   None.
 *
 * Returned value:
 *    None.
 *
 ****************************************************************************/

void pm_initialize(void);

/****************************************************************************
 * Name: pm_register
 *
 * Description:
 *   This function is called by a device driver in order to register to
 *   receive power management event callbacks.
 *
 * Input parameters:
 *   callbacks - An instance of struct pm_callback_s providing the driver
 *               callback functions.
 *
 * Returned value:
 *    Zero (OK) on success; otherwise a negated errno value is returned.
 *
 ****************************************************************************/

int pm_register(FAR struct pm_callback_s *callbacks);

/****************************************************************************
 * Name: pm_unregister
 *
 * Description:
 *   This function is called by a device driver in order to unregister
 *   previously registered power management event callbacks.
 *
 * Input parameters:
 *   callbacks - An instance of struct pm_callback_s providing the driver
 *               callback functions.
 *
 * Returned value:
 *    Zero (OK) on success; otherwise a negated errno value is returned.
 *
 ****************************************************************************/

int pm_unregister(FAR struct pm_callback_s *callbacks);

/****************************************************************************
 * Name: pm_suspend
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   performing meaningful activities (non-idle), needs the power kept at
 *   last the specified level.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *
 *     As an example, media player might stay in normal state during playback.
 *
 * Returned Value:
 *   0 - On Success
 *  -1 - On Error
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

int pm_suspend(enum pm_domain_e domain);

/****************************************************************************
 * Name: pm_resume
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   idle now, could relax the previous requested power level.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *
 *     As an example, media player might relax power level after playback.
 *
 * Returned Value:
 *   0 - On Success
 *  -1 - On Error
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

int pm_resume(enum pm_domain_e domain);

/****************************************************************************
 * Name: pm_set_wakeup_timer
 *
 * Description:
 *   This function is called just before sleep to start the required PM wake up
 *   timer. It will start the first timer from the g_pm_timer_activelist with the
 *   required delay.(delay should be positive)
 * 
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   0 - system can go to sleep
 *   -1 - system should not go to sleep
 *
 ****************************************************************************/

int pm_set_wakeup_timer(void);

/****************************************************************************
 * Name: pm_timer_update
 *
 * Description:
 *   This function decreases the delay of head pm timer in the 
 *   g_pm_timer_activelist by given ticks. If the delay becomes 0,
 *   It expires the pm timer.
 *
 * Input Parameters:
 *   Ticks to decrease
 *
 * Returned Value:
 *   None.
 * 
 * Assumption: This should be also implemented for CONFIG_SCHED_TICKLESS and
 * CONFIG_SCHED_TICKSUPRESS.
 *
 ****************************************************************************/

void pm_timer_update(int ticks);

/************************************************************************
 * Name: pm_timer_add
 *
 * Description:
 *   This function adds a wakeup timer in the g_pm_timer_activelist. So that it will be
 *   invoked just before sleep when needed. 
 * 
 * Parameters:
 *   struct pm_timer_s pointer
 *
 * Return Value:
 *   None
 *
 ************************************************************************/

void pm_timer_add(pm_timer_t *timer);

/************************************************************************
 * Name: pm_sleep
 *
 * Description:
 *   This function allows the board to sleep for given time interval.
 *   When this function is called, it is expected that board will sleep for 
 *   given duration of time. But for some cases board might not go 
 *   to sleep instantly if :
 * 	 1. system is in pm lock (pm state transition is locked)
 *      2. Other threads(other than idle) are running
 *      3. NORMAL to SLEEP state threshold time is large
 * 
 * Parameters:
 *   milliseconds - expected board sleep duration (in milliseconds)
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_sleep(int milliseconds);

/************************************************************************
 * Name: pm_timedsuspend
 *
 * Description:
 *   This function locks PM transition for a specific duration.  
 * 
 * Parameters:
 *   domain - domain to be suspended
 *   timer_interval - expected lock duration in millisecond
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timedsuspend(enum pm_domain_e domain, unsigned int timer_interval);

/****************************************************************************
 * Name: pm_staycount
 *
 * Description:
 *   This function is called to get current stay count.
 *
 * Input Parameters:
 *   domain - The domain of the PM activity
 *
 * Returned Value:
 *   Current pm stay count
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

uint32_t pm_staycount(enum pm_domain_e domain);

/****************************************************************************
 * Name: pm_checkstate
 *
 * Description:
 *   This function is called from the MCU-specific IDLE loop to monitor the
 *   the power management conditions.  This function returns the "recommended"
 *   power management state based on the PM configuration and activity
 *   reported in the last sampling periods.  The power management state is
 *   not automatically changed, however.  The IDLE loop must call
 *   pm_changestate() in order to make the state change.
 *
 *   These two steps are separated because the platform-specific IDLE loop may
 *   have additional situational information that is not available to the
 *   the PM sub-system.  For example, the IDLE loop may know that the
 *   battery charge level is very low and may force lower power states
 *   even if there is activity.
 *
 *   NOTE: That these two steps are separated in time and, hence, the IDLE
 *   loop could be suspended for a long period of time between calling
 *   pm_checkstate() and pm_changestate().  The IDLE loop may need to make
 *   these calls atomic by either disabling interrupts until the state change
 *   is completed.
 *
 * Input Parameters:
 *   domain - The PM domain to check
 *
 * Returned Value:
 *   The recommended power management state.
 *
 ****************************************************************************/

enum pm_state_e pm_checkstate(void);

/****************************************************************************
 * Name: pm_changestate
 *
 * Description:
 *   This function is used to platform-specific power management logic.  It
 *   will announce the power management power management state change to all
 *   drivers that have registered for power management event callbacks.
 *
 * Input Parameters:
 *   newstate - Identifies the new PM state
 *
 * Returned Value:
 *   0 (OK) means that the callback function for all registered drivers
 *   returned OK (meaning that they accept the state change).  Non-zero
 *   means that one of the drivers refused the state change.  In this case,
 *   the system will revert to the preceding state.
 *
 * Assumptions:
 *   It is assumed that interrupts are disabled when this function is
 *   called.  This function is probably called from the IDLE loop... the
 *   lowest priority task in the system.  Changing driver power management
 *   states may result in renewed system activity and, as a result, can
 *   suspend the IDLE thread before it completes the entire state change
 *   unless interrupts are disabled throughout the state change.
 *
 ****************************************************************************/

int pm_changestate(enum pm_state_e newstate);

/****************************************************************************
 * Name: pm_querystate
 *
 * Description:
 *   This function returns the current power management state.
 *
 * Input Parameters:
 *
 * Returned Value:
 *   The current power management state.
 *
 ****************************************************************************/

enum pm_state_e pm_querystate(void);

#ifdef CONFIG_PM_DVFS
/****************************************************************************
 * Name: pm_dvfs
 *
 * Description:
 *   This internal function is called to reduce the clock frequency
 *   of CPU core. It can be applied on some scenario which when low
 *	 loading activity is expected, we can invoke this API to wind down
 *   CPU cores with high operating frequency, to enhance the effectiveness
 *	 for power saving.
 *
 * Input Parameters:
 *   div_lvl - voltage frequency scaling level
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *   Incorrect frequency scaling level given as input will lead to an assert.
 *
 ****************************************************************************/
void pm_dvfs(int div_lvl);
#endif

void pm_driver_register(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */

/****************************************************************************
 * Stubs
 ****************************************************************************/

#else							/* CONFIG_PM */

/* Stubbed out versions of all of PM interface functions that may be used to
 * avoid so much conditional compilation in driver code when PM is disabled:
 */

#define pm_initialize()
#define pm_register(cb)			(0)
#define pm_unregister(cb)		(0)
#define pm_suspend(domain)
#define pm_resume(domain)
#define pm_checkstate()		(0)
#define pm_changestate(state)	(0)
#define pm_querystate()        (0)

#endif							/* CONFIG_PM */
#endif							/* __INCLUDE_TINYARA_POWER_PM_H */
