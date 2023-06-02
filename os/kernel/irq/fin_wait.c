/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <arch/irq.h>
#include <sys/types.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/sched.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void update_fin_queue(struct tcb_s *tcb)
{
	if (tcb->pending_fin_data != NO_FIN_DATA) {
		tcb->fin_data = tcb->pending_fin_data;
		tcb->pending_fin_data = NO_FIN_DATA;
	} else {
		tcb->fin_data = NO_FIN_DATA;
	}	
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fin_wait
 *
 * Description:
 *   This function is for waiting kernel irq event.
 *   If a task calls this function, it will be blocked until receiving irq event.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   On success, received data will be returned.
 *   On failure, NO_FIN_DATA(-999) will be returned.
 *
 ****************************************************************************/

int fin_wait(void)
{
	int ret = NO_FIN_DATA;
	int saved_state;
	struct tcb_s *tcb;

	saved_state = enter_critical_section();

	tcb = sched_self();
	DEBUGASSERT(tcb);

	if (tcb->fin_data != NO_FIN_DATA) {
		/* If there is already pending irq, return saved irq data. */
		ret = tcb->fin_data;

		/* If there is second pending irq, update the irq data. */
		update_fin_queue(tcb);
		leave_critical_section(saved_state);
		return ret;
	}

	up_block_task(tcb, TSTATE_WAIT_FIN);

	if (tcb->fin_data != NO_FIN_DATA) {
		ret = tcb->fin_data;
	}
	/* If there is pending irq, update the irq data array. */
	update_fin_queue(tcb);

	leave_critical_section(saved_state);

	return ret;
}
