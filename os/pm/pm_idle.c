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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <assert.h>
#include <tinyara/pm/pm.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include "../kernel/sched/sched.h"

#include "pm.h"
#ifdef CONFIG_SMP
#include "../arch/arm/src/armv7-a/smp.h"
#endif

#ifdef CONFIG_PM

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

static clock_t stime;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: suspend_devices
 *
 * Description:
 *   This function suspend devices. It is called before entering sleep mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK on success, ERROR on failure if state change fails.
 *
 ****************************************************************************/
static int suspend_devices(void)
{
	/* Then force the global state change */
	if (pm_changestate(PM_SLEEP) < 0) {
		/* The new state change failed */
		pmlldbg("State change failed! newstate = %d\n", PM_SLEEP);
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name: resume_devices
 *
 * Description:
 *   This function resume devices. It is called after waking up from sleep mode.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void resume_devices(void)
{
	/* After wakeup change PM State to STANDBY and reset the time slice */
	(void)pm_changestate(PM_NORMAL);
}

#ifdef CONFIG_SMP
/****************************************************************************
 * Name: disable_secondery_cpus
 *
 * Description:
 *   This function disables all secondary CPUs by sending a hotplug signal.
 *   It waits until each secondary CPU enters the CPU_HOTPLUG state.
 *   This is called before entering sleep mode in an SMP system.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK on success.
 *
 ****************************************************************************/
static int disable_secondary_cpus(void)
{
	/* Send signal to shutdown other cores here */
	for (int cpu = 1; cpu < CONFIG_SMP_NCPUS; cpu++) {
		if (up_get_cpu_state(cpu) == CPU_RUNNING) {
			if (up_cpu_hotplug(cpu) != OK) {
				pmllvdbg("CPU%d hotplug failed! Unable to shutdown secondary core for sleep mode\n", cpu);
				return ERROR;
			}
		}
	}

	return OK;
}

/****************************************************************************
 * Name: enable_secondery_cpus
 *
 * Description:
 *   This function enables all secondary CPUs after waking up from sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void enable_secondary_cpus(void)
{
	/* TODO: move start smp code BSP to here */
}

/****************************************************************************
 * Name: check_secondery_cpus_idle
 *
 * Description:
 *   This function checks if all secondary CPUs are idle and ready for sleep.
 *   If any CPU is not idle or has a pause request, it aborts the sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK if all secondary CPUs are idle, ERROR if sleep should be aborted.
 *
 ****************************************************************************/
static int check_secondary_cpus_idle(void)
{
	int cpu;
	FAR struct tcb_s *tcb;

	/* Perform state-dependent logic here */
	/* For SMP case, we need to check secondary core status
		* If secondary core status is not in idle thread, abort
		* the sleep and check again on next cycle
		*/

	for (cpu = 1; cpu < CONFIG_SMP_NCPUS; cpu++) {
		/* If the CPU is just back from sleep, abort the sleep */
		if (up_get_cpu_state(cpu) == CPU_WAKE_FROM_SLEEP) {
			return ERROR;
		} else if (up_get_cpu_state(cpu) == CPU_HOTPLUG) {
			continue;
		}

		tcb = current_task(cpu);
		/* Check if current cpu is in idle thread */
		if (tcb->pid != cpu) {
			pmllvdbg("Sleep abort! CPU%d task: %s!\n", cpu, tcb->name);
			return ERROR;
		}
	}

	return OK;
}
#else
#define disable_secondary_cpus()    (OK)
#define enable_secondary_cpus()
#define check_secondary_cpus_idle() (OK)
#endif

/****************************************************************************
 * Name: disable_systick
 *
 * Description:
 *   This function disables the system tick timer before entering sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK on success.
 *
 ****************************************************************************/
static int disable_systick(void)
{
	/* TODO: move disable systick code BSP to here */
	return OK;
}

/****************************************************************************
 * Name: enable_and_compensate_systick
 *
 * Description:
 *   This function enables the system tick timer after waking up and
 *   compensates for any ticks missed during sleep. It updates the
 *   system time and timer accordingly.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void enable_and_compensate_systick(void)
{
#ifdef CONFIG_PM_TICKSUPPRESS
	struct pm_sleep_ops *sleep_ops = g_pmglobals.sleep_ops;
	clock_t missing_tick;

	if (!sleep_ops->get_missingtick) {
		return;
	}

	missing_tick = sleep_ops->get_missingtick();

	pmllvdbg("missing_tick: %llu\n", missing_tick);
	pm_metrics_update_missing_tick(missing_tick);

	if (missing_tick > 0) {
		/* Correcting for missed system ticks in sleep. */
		clock_timer_nohz(missing_tick);

		/* Compensate wd timer for missing ticks by pm sleep.
		 * But to guarantee fast execution of interrupt service routine after wakeup,
		 * expiration of wd_timer is not done here
		 *
		 *     WAKE UP -> HW IRQ ISR -> THREAD -> TICK ISR
		 *           |              |          |           |
		 *           +--------------+----------+-----------+
		 *     (corrects tick)                       (expire timer)
		 */
		wd_timer_nohz(missing_tick);
	}
#endif

	/* TODO: move enable systick code BSP to here */
}

#ifdef CONFIG_PM_TIMEDWAKEUP
/****************************************************************************
 * Name: get_next_wakeup_time
 *
 * Description:
 *   This function gets the next wakeup time from the watchdog timer.
 *   If the delay is too short (less than SLEEP_ENTRY_WAIT), it returns ERROR
 *   to abort sleep.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   The next wakeup time in ticks, or ERROR if sleep should be aborted.
 *
 ****************************************************************************/
static int get_next_wakeup_time(void)
{
	clock_t delay;

	/* get wakeup timer */
	delay = wd_getwakeupdelay();
	if ((delay > 0) && (delay < MSEC2TICK(CONFIG_PM_SLEEP_ENTRY_WAIT_MS))) {
		pmllvdbg("Wdog Timer Delay: %ldms is less than SLEEP_ENTRY_WAIT: %ldms\n", TICK2MSEC(delay), CONFIG_PM_SLEEP_ENTRY_WAIT_MS);
		return ERROR;
	}

	return delay;
}

/****************************************************************************
 * Name: set_pm_wakeup_timer
 *
 * Description:
 *   This function sets the wakeup timer to wake up the system after
 *   the specified delay. It uses the sleep_ops to set the timer.
 *
 * Input Parameters:
 *   delay - The wakeup delay in ticks.
 *
 * Returned Value:
 *   OK on success, or the error code from sleep_ops->set_timer on failure.
 *
 ****************************************************************************/
static int set_pm_wakeup_timer(int delay) 
{
	if (delay > 0) {
		/* set wakeup timer */
		pmllvdbg("Setting timer and board will wake up after %ld millisecond\n", delay);
		return g_pmglobals.sleep_ops->set_timer(TICK2USEC(delay));
	}

	return OK;
}
#else
#define get_next_wakeup_time()       (ERROR)
#define set_pm_wakeup_timer(delay)  (OK)
#endif

/****************************************************************************
 * Name: update_wakeup_reason
 *
 * Description:
 *   This function updates the wakeup reason by querying the sleep_ops.
 *   It logs the wakeup source and updates PM metrics.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void update_wakeup_reason(void)
{
	pm_wakeup_reason_code_t wakeup_src;
	struct pm_sleep_ops *sleep_ops = g_pmglobals.sleep_ops;

	if (sleep_ops->get_wakeupreason) {
		wakeup_src = sleep_ops->get_wakeupreason();

		pmllvdbg("wakeup source code = %d\n", wakeup_src);
		pm_metrics_update_wakeup_reason(wakeup_src);
	}
}

/****************************************************************************
 * Name: check_pm_state
 *
 * Description:
 *   This function checks if the system can enter sleep mode.
 *   It verifies the PM state and checks if all secondary CPUs are idle.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK if the system can enter sleep, ERROR if sleep should be aborted.
 *
 ****************************************************************************/
static int check_pm_state(void) 
{
	/* Decide, which power saving level can be obtained */
	if (pm_checkstate() != PM_SLEEP) {
		return ERROR;
	}

	if (check_secondary_cpus_idle() != 0) {
		return ERROR;
	}

	return OK;
}

/****************************************************************************
 * Name: enter_sleep
 *
 * Description:
 *   This function handles the entire sleep sequence. It gets the wakeup time,
 *   suspends devices, disables secondary CPUs and systick, sets the wakeup
 *   timer, enters sleep, and then resumes everything on wakeup.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void enter_sleep(void)
{
	int next_wakeup_time;
	struct pm_sleep_ops *sleep_ops = g_pmglobals.sleep_ops;

	DEBUGASSERT(sleep_ops);

	next_wakeup_time = get_next_wakeup_time();
	if (next_wakeup_time < 0) {
		return;
	}

	if (suspend_devices() != OK) {
		goto DEVICES_RESUME;
	}

	if (disable_secondary_cpus() != OK) {
		goto CPUS_ENABLE;
	}

	if (disable_systick() != OK) {
		goto SYSTICK_ENABLE;
	}

	if (set_pm_wakeup_timer(next_wakeup_time) != OK) {
		goto SYSTICK_ENABLE;
	}

	if (sleep_ops->sleep && sleep_ops->sleep() != 0) {
		goto SYSTICK_ENABLE;
	}

	update_wakeup_reason();

SYSTICK_ENABLE:
	enable_and_compensate_systick();

CPUS_ENABLE:
	enable_secondary_cpus();

DEVICES_RESUME:
	resume_devices();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

void pm_idle(void)
{
	irqstate_t flags;
	clock_t now;
	/* State change only if PM is ready to state change */
	if (!g_pmglobals.is_running) {
		return;
	}

	now = clock_systimer();
	/* We need to check and change PM state transition only if one tick time has been passed,
	 * because state transition only happens when CPU receive TICK INTERRUPT. So checking pm state
	 * multiple times within one tick is waste of CPU clocks and we should avoid it.
	 */
	if (now <= stime) {
		return;
	}	
	
	flags = enter_critical_section();
	sched_lock();

	pm_metrics_update_idle();

	if (check_pm_state() == OK) {
		enter_sleep();
	}

	stime = clock_systimer();

	sched_unlock();
	leave_critical_section(flags);
}

#endif /* CONFIG_PM */
