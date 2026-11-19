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
#include "sched/sched.h"

#ifdef CONFIG_IRQ_FIN_WAIT

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* This is the list of all tasks that are blocked waiting for a fin
 * (irq finalization) notification.  Like the other global task lists it
 * is zero-initialized in BSS, so no boot-time initialization is needed.
 */

dq_queue_t g_waitingforfin;

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
	irqstate_t saved_state;
	FAR struct tcb_s *tcb;

	saved_state = enter_critical_section();

	tcb = this_task();
	DEBUGASSERT(tcb != NULL);

	if (tcb->fin_data != NO_FIN_DATA) {
		/* If there is already pending irq, return saved irq data. */
		ret = tcb->fin_data;

		/* If there is second pending irq, update the irq data. */
		update_fin_queue(tcb);
		leave_critical_section(saved_state);
		return ret;
	}

	/* Remove the tcb task from the ready-to-run list and move it to the
	 * fin waiting list.  fin_notify() removes the task from the list and
	 * makes it ready-to-run again.
	 */

	nxsched_remove_self(tcb);

	tcb->task_state = TSTATE_WAIT_FIN;
	dq_addlast((FAR dq_entry_t *)tcb, list_waitingforfin());

	/* Now, perform the context switch */

	up_switch_context(this_task(), tcb);

	/* Resumed: fin_notify() delivered the data and unblocked us */

	if (tcb->fin_data != NO_FIN_DATA) {
		ret = tcb->fin_data;
	}
	/* If there is pending irq, update the irq data array. */
	update_fin_queue(tcb);

	leave_critical_section(saved_state);

	return ret;
}

/****************************************************************************
 * Name: fin_block
 *
 * Description:
 *   This function blocks the calling task in TSTATE_WAIT_FIN until a
 *   fin_notify() unblocks it, without touching the fin data mailbox.
 *   It is intended for kernel threads (e.g. the binary manager fault
 *   message receiver) that use the fin notification as a pure wakeup
 *   event and dequeue their own payload.
 *
 * Inputs:
 *   None.
 *
 * Return Value:
 *   OK
 *
 ****************************************************************************/

void fin_block(void)
{
	irqstate_t saved_state;
	FAR struct tcb_s *tcb;

	saved_state = enter_critical_section();

	tcb = this_task();
	DEBUGASSERT(tcb != NULL);

	nxsched_remove_self(tcb);

	tcb->task_state = TSTATE_WAIT_FIN;
	dq_addlast((FAR dq_entry_t *)tcb, list_waitingforfin());

	up_switch_context(this_task(), tcb);

	leave_critical_section(saved_state);
}

#else
/* The interface is disabled (CONFIG_IRQ_FIN_WAIT is not set): the entry
 * points still exist so that callers and the syscall plumbing link in
 * every configuration, but they are inert - fin_wait() returns
 * immediately with NO_FIN_DATA and fin_block() returns without blocking.
 */

int fin_wait(void)
{
	return NO_FIN_DATA;
}

void fin_block(void)
{
}

#endif
