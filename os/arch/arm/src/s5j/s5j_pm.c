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
 *
 *  arch/arm/src/s5j/s5j_pm.c
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/irq.h>
#include <tinyara/pm/pm.h>

#include "s5j_clock.h"
#include "s5j_pm.h"
#include "up_arch.h"
#include "up_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Switch to the new sleep state suggested by the PM framework */

int up_pm_switch_state(int state_ID)
{
	int ret = OK;

	switch (state_ID) {

	case S5J_NORMAL:
		ret = s5j_pmnormal();
		break;

	case S5J_STOP:
		ret = s5j_pmstop();
		break;

	case S5J_STANDBY:
		ret = s5j_pmstandby();
		break;

	default:
		break;

	}

	return ret;
}
