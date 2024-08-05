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
#include <tinyara/clock.h>

/* This structure is used to send data to pm driver from app side for timedSuspend */
struct pm_suspend_arg_s {
	int domain_id;                    /* domain ID to be suspended */
	unsigned int timer_interval;      /* duration to be suspended */
};

typedef struct pm_suspend_arg_s pm_suspend_arg_t;

struct pm_domain_arg_s {
	int domain_id;               /* the domain ID after registering domain */
	char *domain_name;           /* the name of domain that need to be register */
};

typedef struct pm_domain_arg_s pm_domain_arg_t;


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
#define CONFIG_PM_NDOMAINS 32
#endif

#define PM_IDLE_DOMAIN 0
#define PM_LCD_DOMAIN  1

#if CONFIG_PM_NDOMAINS < 1
#error CONFIG_PM_NDOMAINS invalid
#endif

/* CONFIG_IDLE_CUSTOM. Some architectures support this definition.  This,
 * if defined, will allow you replace the default IDLE loop with your
 * own, custom idle loop to support board-specific IDLE time power management
 */


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

/* This enumeration provides all power management related wakeup source code. */

typedef enum {
	PM_WAKEUP_UNKNOWN,       /* Unknown Wakeup Source*/
	PM_WAKEUP_BLE,           /* BLE Interrupts */
	PM_WAKEUP_WIFI,          /* Wifi Interrupts */
	PM_WAKEUP_UART_CONSOLE,  /* UART Console Interrupts */
	PM_WAKEUP_UART_TTYS2,    /* UART TTYS2 Interrupts */
	PM_WAKEUP_GPIO,          /* GPIO Interrupts */
	PM_WAKEUP_HW_TIMER,      /* Timer Expiration */
	PM_WAKEUP_SRC_COUNT,
} pm_wakeup_reason_code_t;

#ifdef CONFIG_PM_METRICS
static const char *wakeup_src_name[PM_WAKEUP_SRC_COUNT] = {"UNKNOWN", "BLE", "WIFI", "UART CONSOLE", "UART TTYS2", "GPIO", "HW TIMER"};
#endif
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
 * Name: pm_driver_register
 *
 * Description:
 *   Register pm driver path,  "/dev/pm"
 *
 ****************************************************************************/
void pm_driver_register(void);

#ifdef CONFIG_PM

/****************************************************************************
 * Name: pm_start
 *
 * Description:
 *   This function is called by the application thread to start the Power
 *   Management system. This fucntion sets the ready_to_state_change flag which
 *   enables pm to transition between low and high power states.
 *
 * Input parameters:
 *   None.
 *
 * Returned value:
 *    None.
 *
 ****************************************************************************/

void pm_start(void);

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
 * Name: pm_domain_register
 *
 * Description:
 *   This function is called by a device driver in order to register domain inside PM.
 *
 * Input parameters:
 *   domain - the string domain need to be registered in PM.
 * 
 * Returned value:
 *    non-negative integer   : ID of domain
 *    ERROR (-1)             : On Error
 *
 *
 ****************************************************************************/

int pm_domain_register(char *domain);

/****************************************************************************
 * Name: pm_idle
 *
 * Description:
 *   This function is called by IDLE thread to make board sleep. This function
 *   also allow to set wake up timer & handler and do all the PM pre processing
 *   required before going to sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void pm_idle(void);

/****************************************************************************
 * Name: pm_suspend
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   performing meaningful activities (non-idle), needs the power kept at
 *   last the specified level.
 *
 * Input Parameters:
 *   domain_id - The domain ID of the PM activity
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

int pm_suspend(int domain_id);

/****************************************************************************
 * Name: pm_resume
 *
 * Description:
 *   This function is called by a device driver to indicate that it is
 *   idle now, could relax the previous requested power level.
 *
 * Input Parameters:
 *   domain_id - The domain ID of the PM activity
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

int pm_resume(int domain_id);

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
#ifdef CONFIG_PM_TIMEDWAKEUP
int pm_sleep(int milliseconds);
#else
#define pm_sleep(milliseconds) usleep(milliseconds * USEC_PER_MSEC)
#endif

/************************************************************************
 * Name: pm_timedsuspend
 *
 * Description:
 *   This function locks PM transition for a specific duration.  
 * 
 * Parameters:
 *   domain_id - domain ID to be suspended
 *   milliseconds - expected lock duration in millisecond
 *
 * Return Value:
 *   0 - success
 *   -1 - error
 *
 ************************************************************************/

int pm_timedsuspend(int domain_id, unsigned int milliseconds);

/****************************************************************************
 * Name: pm_suspendcount
 *
 * Description:
 *   This function is called to get current stay count.
 *
 * Input Parameters:
 *   domain_id - The domain ID of the PM activity
 *
 * Returned Value:
 *   Current pm stay count
 *
 * Assumptions:
 *   This function may be called from an interrupt handler.
 *
 ****************************************************************************/

uint16_t pm_suspendcount(int domain_id);

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
#else
#define pm_dvfs(div_lvl)	(0)
#endif

#ifdef CONFIG_PM_METRICS
/****************************************************************************
 * Name: pm_metrics
 *
 * Description:
 *   This internal function is called to analyze the PM suspend and sleep behaviour.
 *   It gathers the pm metrics statistics for provided time (in msec) to provide the
 *   domain specific pm suspend information.
 *
 * Input Parameters:
 *   milliseconds - the monitoring duration in milliseconds
 *
 * Returned Value:
 *   OK (0)     - On Success
 *   ERROR (-1) - On Error
 *
 ****************************************************************************/
int pm_metrics(int milliseconds);
#endif

/****************************************************************************
 * Stubs
 ****************************************************************************/

#else							/* CONFIG_PM */

/* Stubbed out versions of all of PM interface functions that may be used to
 * avoid so much conditional compilation in driver code when PM is disabled:
 */

#define pm_start()              (0)
#define pm_initialize()
#define pm_register(cb)         (0)
#define pm_unregister(cb)       (0)
#define pm_domain_register(domain)	(0)
#define pm_idle()
#define pm_suspend(domain_id)   (0)
#define pm_resume(domain_id)    (0)
#define pm_sleep(milliseconds)				usleep(milliseconds * USEC_PER_MSEC)
#define pm_timedsuspend(domain_id, milliseconds)	(0)

#endif							/* CONFIG_PM */

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* __INCLUDE_TINYARA_POWER_PM_H */
