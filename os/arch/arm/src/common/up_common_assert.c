/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#include <errno.h>
#include <sys/types.h>
#include <debug.h>
#include <stdlib.h>
#include <sys/boardctl.h>
#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <arch/board/board.h>
#include <tinyara/assertmode.h>

#ifdef CONFIG_SMP
#include "irq/irq.h"
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Global assert action variable - shared across ARM architectures
 * Default initialization based on config:
 * - CONFIG_BOARD_ASSERT_AUTORESET: Auto reset on assert
 * - CONFIG_BOARD_ASSERT_SYSTEM_HALT: System halt on assert
 * - Neither selected: Task exit/terminate on assert
 */

#if defined(CONFIG_BOARD_ASSERT_AUTORESET)
volatile int g_assert_action = ASSERT_ACTION_AUTORESET;
#elif defined(CONFIG_BOARD_ASSERT_SYSTEM_HALT)
volatile int g_assert_action = ASSERT_ACTION_HALT;
#else
volatile int g_assert_action = ASSERT_ACTION_NONE;
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: get_assert_action
 *
 * Description:
 *   Get the current runtime assert action mode.
 *
 * Returned Value:
 *   0 = AUTO RESET, 1 = SYSTEM HALT, 2 = TASK EXIT/TERMINATE
 *
 ****************************************************************************/

int get_assert_action(void)
{
	return g_assert_action;
}

/****************************************************************************
 * Name: set_assert_action
 *
 * Description:
 *   Set the runtime assert action mode.
 *
 * Input Parameters:
 *   action - 0 for AUTO RESET, 1 for SYSTEM HALT, 2 for TASK EXIT/TERMINATE
 *
 * Returned Value:
 *   OK on success, -EINVAL for invalid action value
 *
 ****************************************************************************/

int set_assert_action(int action)
{
	if (action < ASSERT_ACTION_AUTORESET || action > ASSERT_ACTION_NONE) {
		return -EINVAL;
	}
	g_assert_action = action;
	return OK;
}

/* Watchdog kick interval in ms during halt */
#define ASSERT_HALT_WDOG_KICK_INTERVAL 5000

/****************************************************************************
 * Name: halt_on_assert
 *
 * Description:
 *   Halt the system in an infinite loop when an assert occurs.
 *   This is called when ASSERT_ACTION_HALT is selected.
 *   The watchdog is kicked periodically to prevent reset during debugging.
 *
 ****************************************************************************/

void halt_on_assert(void)
{
	/* Disable interrupts on this CPU */
	irqsave();

#ifdef CONFIG_SMP
	/* Try (again) to stop activity on other CPUs */
	spin_trylock(&g_cpu_irqlock);
#endif

	for (;;) {
#ifdef CONFIG_WATCHDOG_FOR_IRQ
		/* Kick hardware watchdog to allow debug time */
		up_wdog_keepalive();
		up_mdelay(ASSERT_HALT_WDOG_KICK_INTERVAL);
#endif
#ifdef CONFIG_ARCH_LEDS
		/* FLASH LEDs a 2Hz */
		board_autoled_on(LED_PANIC);
		up_mdelay(250);
		board_autoled_off(LED_PANIC);
		up_mdelay(250);
#endif
	}
}

/****************************************************************************
 * Name: apply_assert_action
 *
 * Description:
 *   Apply the configured assert action after an assert occurs.
 *   This is the common implementation shared across ARM architectures.
 *
 * Input Parameters:
 *   errorcode     - Exit code to use if task exit is required
 *   is_irq_or_idle - True if in IRQ context or idle task
 *
 ****************************************************************************/

void apply_assert_action(int errorcode, bool is_irq_or_idle)
{
	switch (g_assert_action) {
	case ASSERT_ACTION_AUTORESET:
		boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
		break;

	case ASSERT_ACTION_HALT:
		halt_on_assert();
		break;

	case ASSERT_ACTION_NONE:
	default:
		/* Are we in an interrupt handler or the idle task? */
		if (is_irq_or_idle) {
			halt_on_assert();
		} else {
			exit(errorcode);
		}
		break;
	}
}
