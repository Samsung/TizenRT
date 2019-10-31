/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/****************************************************************************
 *
 *   Copyright (C) 2014-2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * The Samsung sample code has a BSD compatible license that requires this
 * copyright notice:
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
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
 */

#include <tinyara/config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <queue.h>
#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/wdog.h>
#include "event_groups.h"

#if defined(CONFIG_USE_16_BIT_TICKS)
#define EVENT_CLEAR_EVENTS_ON_EXIT_BIT	0x0100U
#define EVENT_UNBLOCKED_DUE_TO_BIT_SET	0x0200U
#define EVENT_WAIT_FOR_ALL_BITS			0x0400U
#define EVENT_BITS_CONTROL_BYTES	0xff00U
#else
#define EVENT_CLEAR_EVENTS_ON_EXIT_BIT	0x01000000UL
#define EVENT_UNBLOCKED_DUE_TO_BIT_SET	0x02000000UL
#define EVENT_WAIT_FOR_ALL_BITS			0x04000000UL
#define EVENT_BITS_CONTROL_BYTES	0xff000000UL
#endif

#define ADAPTER_FALSE			((base_type_t)0)
#define ADAPTER_TRUE			((base_type_t)1)
#define port_max_delay 			(0xffffffff)

typedef struct event_group_definition {
	event_bits_t event_bits;
	sq_queue_t tasks_waiting_for_bits;	/*< List of tasks waiting for a bit to be set. */
	pthread_mutex_t event_group_mux;
} event_group_t;

typedef struct event_node {
	sq_entry_t node;
	pid_t pid;
	tick_type_t item_value;
} event_node_t;

static event_node_t *event_data[MAX_PID_MASK];

event_group_handle_t event_group_create(void)
{
	event_group_t *pevent_bits = NULL;

	/* Allocate the event group. */
	pevent_bits = (event_group_t *)malloc(sizeof(event_group_t));
	if (pevent_bits != NULL) {
		pevent_bits->event_bits = 0;
		sq_init(&(pevent_bits->tasks_waiting_for_bits));
		pthread_mutex_init(&pevent_bits->event_group_mux, NULL);
	}
	return (event_group_handle_t)pevent_bits;
}

static void task_add_event_item(event_node_t *pevent_node)
{
	if (pevent_node) {
		event_data[pevent_node->pid] = pevent_node;
	}
}

static tick_type_t task_reset_event_item_value(event_group_t *pevent_group)
{
	tick_type_t ret = 0;
	pid_t pid = getpid();

	pthread_mutex_lock(&pevent_group->event_group_mux);
	event_node_t *pevent_node = event_data[pid];
	if (pevent_node) {
		/* Get the item value*/
		ret = pevent_node->item_value;
		/* Remove the current node from event group queue*/
		sq_rem((sq_entry_t *)pevent_node, &(pevent_group->tasks_waiting_for_bits));
		free(pevent_node);
		event_data[pid] = NULL;
	}
	pthread_mutex_unlock(&pevent_group->event_group_mux);
	return ret;
}

static base_type_t test_wait_condition(const event_bits_t current_event_bits, const event_bits_t bits_to_wait_for, const base_type_t wait_for_all_bits)
{
	base_type_t wait_condition_met = ADAPTER_FALSE;

	if (wait_for_all_bits == ADAPTER_FALSE) {
		/* Task only has to wait for one bit within bits_to_wait_for to be
			set.  Is one already set? */
		if ((current_event_bits & bits_to_wait_for) != (event_bits_t)0) {
			wait_condition_met = ADAPTER_TRUE;
		}
	} else {
		/* Task has to wait for all the bits in bits_to_wait_for to be set.
			Are they set already? */
		if ((current_event_bits & bits_to_wait_for) == bits_to_wait_for) {
			wait_condition_met = ADAPTER_TRUE;
		}
	}
	return wait_condition_met;
}

/* This is a special value of si_signo that means that it was the timeout
 * that awakened the wait... not the receipt of a signal.
 */
#define SIG_WAIT_TIMEOUT 0xff

static void event_sig_timeout(int argc, uint32_t itcb)
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

	/* There may be a race condition -- make sure the task is
	* still waiting for a signal
	*/
	if (u.wtcb->task_state == TSTATE_WAIT_SIG) {
		u.wtcb->sigunbinfo.si_signo = SIG_WAIT_TIMEOUT;
		u.wtcb->sigunbinfo.si_code = SI_TIMER;
		u.wtcb->sigunbinfo.si_value.sival_int = 0;
#ifdef CONFIG_SCHED_HAVE_PARENT
		u.wtcb->sigunbinfo.si_pid = 0;	/* Not applicable */
		u.wtcb->sigunbinfo.si_status = OK;
#endif
		up_unblock_task(u.wtcb);
	}
}

static void task_place_on_unordered_event_list(event_group_t *event_group, const tick_type_t item_value, const tick_type_t tick_to_wait)
{
	if (!event_group) {
		return;
	}

	/* Store the item value in the event list item.  It is safe to access the
	   event list item here as interrupts won't access the event list item of a
	   task that is not in the Blocked state. */
	event_node_t *pevent_node = (event_node_t *)malloc(sizeof(event_node_t));
	if (!pevent_node) {
		return;
	}
	pevent_node->item_value = item_value;
	pevent_node->pid = getpid();

	/*Add current task to event group*/
	pthread_mutex_lock(&event_group->event_group_mux);
	sq_addlast(&pevent_node->node, &event_group->tasks_waiting_for_bits);
	task_add_event_item(pevent_node);
	pthread_mutex_unlock(&event_group->event_group_mux);

	/* Block the task based on wait time*/
	struct tcb_s *ptcp = sched_gettcb(pevent_node->pid);
	if (!ptcp) {
		return;
	}

	if (tick_to_wait == port_max_delay) {
		/* Wait here until the event bit be set*/
		up_block_task(ptcp, TSTATE_TASK_INACTIVE);
	} else {
		/* Create a timer to unblock the task when the time is over */
		ptcp->waitdog = wd_create();
		DEBUGASSERT(ptcp->waitdog);

		if (ptcp->waitdog) {
			/* We must disable interrupts here so that
			* the time stays valid until the wait begins. */
			irqstate_t flags = irqsave();

			/* This little bit of nonsense is necessary for some
			* processors where sizeof(pointer) < sizeof(uint32_t).
			* see wdog.h.
			*/
			wdparm_t wdparm;
			wdparm.pvarg = (FAR void *)ptcp;

			/* Start the watchdog */
			wd_start(ptcp->waitdog, tick_to_wait, (wdentry_t)event_sig_timeout, 1, wdparm.dwarg);

			/* Now wait for either the signal or the watchdog */
			up_block_task(ptcp, TSTATE_WAIT_SIG);

			/* We are running again, restore and enbale interrupts */
			irqrestore(flags);

			/* We no longer need the watchdog */
			wd_delete(ptcp->waitdog);
			ptcp->waitdog = NULL;
		}
	}
	return;
}

static base_type_t task_remove_from_unordered_event_list(event_node_t *event_list_item, sq_queue_t *event_list, const tick_type_t item_value)
{
	base_type_t ret = ADAPTER_FALSE;

	if (!event_list_item || !event_list) {
		return -1;
	}

	/* Store the new item value in the event list. */
	event_list_item->item_value = item_value;

	struct tcb_s *ptcp = sched_gettcb(event_list_item->pid);
    pid_t pid = getpid();
	struct tcb_s *current_tcb = sched_gettcb(pid);

	/* Remove the event list form the event flag.  Interrupts do not access
		event flags. */
	sq_rem((sq_entry_t *)event_list_item, event_list);

	/* Remove the task from the delayed list and add it to the ready list.  The
		scheduler is suspended so interrupts will not be accessing the ready
		lists. */

	if (ptcp) {
		if(ptcp->task_state >= FIRST_BLOCKED_STATE && ptcp->task_state < LAST_BLOCKED_STATE) {
			sched_removeblocked(ptcp);
			sched_addreadytorun(ptcp);
		}

		if (ptcp->sched_priority >= current_tcb->sched_priority) {
			/* Return true if the task removed from the event list has
				a higher priority than the calling task.  This allows
				the calling task to know if it should force a context
				switch now. */
			ret = ADAPTER_TRUE;
		} else {
			ret = ADAPTER_FALSE;
		}
	}
    return ret;
}

event_bits_t event_group_wait_bits(event_group_handle_t event_group, const event_bits_t bits_to_wait_for, const base_type_t clear_on_exit, const base_type_t wait_for_all_bits, tick_type_t tick_to_wait)
{
	event_group_t *event_bits = (event_group_t *)event_group;
	event_bits_t ret, ucontrol_bits = 0;
	base_type_t wait_condition_met;

	/* Disable pre-emption and get the mutex of event group*/
	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);

	const event_bits_t current_event_bits = event_bits->event_bits;

	/* Check to see if the wait condition is already met or not. */
	wait_condition_met = test_wait_condition(current_event_bits, bits_to_wait_for, wait_for_all_bits);
	if (wait_condition_met != ADAPTER_FALSE) {
		/* The wait condition has already been met so there is no need to
			block. */
		ret = current_event_bits;
		tick_to_wait = (tick_type_t)0;

		/* Clear the wait bits if requested to do so. */
		if (clear_on_exit != ADAPTER_FALSE) {
			event_bits->event_bits &= ~bits_to_wait_for;
		}
		pthread_mutex_unlock(&event_bits->event_group_mux);
	} else if (tick_to_wait == (tick_type_t)0) {
		/* The wait condition has not been met, but no block time was
			specified, so just return the current value. */
		ret = current_event_bits;
		pthread_mutex_unlock(&event_bits->event_group_mux);
	} else {
		/* The task is going to block to wait for its required bits to be
			set.  ucontrol_bits are used to remember the specified behaviour of
			this call to event_group_wait_bits() - for use when the event bits
			unblock the task. */
		if (clear_on_exit != ADAPTER_FALSE) {
			ucontrol_bits |= EVENT_CLEAR_EVENTS_ON_EXIT_BIT;
		}

		if (wait_for_all_bits != ADAPTER_FALSE) {
			ucontrol_bits |= EVENT_WAIT_FOR_ALL_BITS;
		}
		pthread_mutex_unlock(&event_bits->event_group_mux);

		/* Store the bits that the calling task is waiting for in the
			task's event list item so the kernel knows when a match is
			found.  Then enter the blocked state. */
		task_place_on_unordered_event_list(event_bits, (bits_to_wait_for | ucontrol_bits), tick_to_wait);

		/* This is obsolete as it will get set after the task unblocks, but
			some compilers mistakenly generate a warning about the variable
			being returned without being set if it is not done. */
		ret = 0;
	}

	/* Recover the pre-emption*/
	sched_unlock();

	/* Check the reason of this weaking*/
	if (tick_to_wait != (tick_type_t)0) {

		/* The task blocked to wait for its required bits to be set - at this
			point either the required bits were set or the block time expired.  If
			the required bits were set they will have been stored in the task's
			event list item, and they should now be retrieved then cleared. */

		ret = task_reset_event_item_value(event_bits);
		if ((ret & EVENT_UNBLOCKED_DUE_TO_BIT_SET) == (event_bits_t)0) {
			pthread_mutex_lock(&event_bits->event_group_mux);

			/* The task timed out, just return the current event bit value. */
			ret = event_bits->event_bits;

			/* It is possible that the event bits were updated between this
				task leaving the Blocked state and running again. */
			if (test_wait_condition(ret, bits_to_wait_for, wait_for_all_bits) != ADAPTER_FALSE) {
				if (clear_on_exit != ADAPTER_FALSE) {
					event_bits->event_bits &= ~bits_to_wait_for;
				}
			}

			pthread_mutex_unlock(&event_bits->event_group_mux);
		} else {
			/* The task unblocked because the bits were set. */
		}

		/* The task blocked so control bits may have been set. */
		ret &= ~EVENT_BITS_CONTROL_BYTES;
	}
	return ret;
}

event_bits_t event_group_clear_bits(event_group_handle_t event_group, const event_bits_t bits_to_clear)
{
	event_group_t *event_bits = (event_group_t *) event_group;
	event_bits_t ret;

	pthread_mutex_lock(&event_bits->event_group_mux);

	/* The value returned is the event group value prior to the bits being
		cleared. */
	ret = event_bits->event_bits;

	/* Clear the bits. */
	event_bits->event_bits &= ~bits_to_clear;
	pthread_mutex_unlock(&event_bits->event_group_mux);
	return ret;
}

event_bits_t event_group_set_bits(event_group_handle_t event_group, const event_bits_t bits_to_set)
{
	event_bits_t bits_to_clear = 0, ubits_waited_for, ucontrol_bits;
	event_group_t *event_bits = (event_group_t *)event_group;
	base_type_t match_found = ADAPTER_FALSE;

	/* Check the user is not attempting to set the bits used by the kernel
		itself. */
	if (!event_bits || (bits_to_set & EVENT_BITS_CONTROL_BYTES) != 0) {
		return -1;
	}
	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);

	/* Set the bits. */
	event_bits->event_bits |= bits_to_set;

	/* Scan the queue*/
	event_node_t *pevent_node = (event_node_t *)event_bits->tasks_waiting_for_bits.head;
	while (pevent_node) {
		//get the value
		ubits_waited_for = pevent_node->item_value;
		match_found = ADAPTER_FALSE;
		/* Split the bits waited for from the control bits. */
		ucontrol_bits = ubits_waited_for & EVENT_BITS_CONTROL_BYTES;
		ubits_waited_for &= ~EVENT_BITS_CONTROL_BYTES;

		if ((ucontrol_bits & EVENT_WAIT_FOR_ALL_BITS) == (event_bits_t)0) {
			/* Just looking for single bit being set. */
			if ((ubits_waited_for & event_bits->event_bits) != (event_bits_t)0) {
				match_found = ADAPTER_TRUE;
			}
		} else if ((ubits_waited_for & event_bits->event_bits) == ubits_waited_for) {
			/* All bits are set. */
			match_found = ADAPTER_TRUE;
		} else {
			/* Need all bits to be set, but not all the bits were set. */
		}

		if (match_found != ADAPTER_FALSE) {
			/* The bits match.  Should the bits be cleared on exit? */
			if ((ucontrol_bits & EVENT_CLEAR_EVENTS_ON_EXIT_BIT) != (event_bits_t)0) {
				bits_to_clear |= ubits_waited_for;
			}

			/* Store the actual event flag value in the task's event list
				item before removing the task from the event list.   The
				EVENT_UNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
				that is was unblocked due to its required bits matching, rather
				than because it timed out. */

			(void)task_remove_from_unordered_event_list(pevent_node, &event_bits->tasks_waiting_for_bits, event_bits->event_bits | EVENT_UNBLOCKED_DUE_TO_BIT_SET);
		}

		pevent_node = (event_node_t *)pevent_node->node.flink;
	}

	/* Clear any bits that matched when the EVENT_CLEAR_EVENTS_ON_EXIT_BIT
		bit was set in the control word. */
	event_bits->event_bits &= ~bits_to_clear;

	pthread_mutex_unlock(&event_bits->event_group_mux);
	sched_unlock();

	return event_bits->event_bits;
}

void event_group_delete(event_group_handle_t event_group)
{
	event_group_t *event_bits = (event_group_t *)event_group;
	sq_queue_t *ptasks_waiting_for_bits = &(event_bits->tasks_waiting_for_bits);
	event_node_t *pevent_node = NULL;
	event_node_t *pevent_node_temp = NULL;

	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);
	pevent_node = (event_node_t *)event_bits->tasks_waiting_for_bits.head;
	while (pevent_node) {
		task_remove_from_unordered_event_list(pevent_node, ptasks_waiting_for_bits, EVENT_UNBLOCKED_DUE_TO_BIT_SET);
		pevent_node_temp = pevent_node;
		pevent_node = (event_node_t *)pevent_node_temp->node.flink;
		event_data[pevent_node_temp->pid] = NULL;
		free(pevent_node_temp);
	}

	pthread_mutex_unlock(&event_bits->event_group_mux);
	free(event_bits);
	sched_unlock();
}
