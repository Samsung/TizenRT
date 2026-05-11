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

#ifndef __INCLUDE_TINYARA_ASSERTACTION_H
#define __INCLUDE_TINYARA_ASSERTACTION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Driver path */
#define ASSERTMODE_DRVPATH "/dev/assertmode"

/* Assert action codes - defines what happens after an assert occurs */
#define ASSERT_ACTION_AUTORESET    0  /* Auto reset on assert */
#define ASSERT_ACTION_HALT         1  /* System halt on assert */
#define ASSERT_ACTION_NONE         2  /* Task exit/terminate on assert */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Structure used for ASSERTMODEIOC_GET_ACTION and ASSERTMODEIOC_SET_ACTION */
struct assertmode_info_s {
	int action;  /* ASSERT_ACTION_AUTORESET, ASSERT_ACTION_HALT, or ASSERT_ACTION_NONE */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Global assert action variable - defined in up_common_assert.c */
extern volatile int g_assert_action;

/****************************************************************************
 * Public Function Prototypes
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

int get_assert_action(void);

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

int set_assert_action(int action);

/****************************************************************************
 * Name: halt_on_assert
 *
 * Description:
 *   Halt the system in an infinite loop when an assert occurs.
 *   This is called when ASSERT_ACTION_HALT is selected.
 *
 ****************************************************************************/

void halt_on_assert(void);

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

void apply_assert_action(int errorcode, bool is_irq_or_idle);

/****************************************************************************
 * Name: assertmode_register
 *
 * Description:
 *   Register the /dev/assertmode driver.
 *
 ****************************************************************************/

void assertmode_register(void);
#endif /* __INCLUDE_TINYARA_ASSERTACTION_H */