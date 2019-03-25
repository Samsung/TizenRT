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
#include <tinyara/sched.h>
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
	return (event_group_handle_t) pevent_bits;
}

static base_type_t test_wait_condition(const event_bits_t current_event_bits, const event_bits_t bits_to_wait_for, const base_type_t wait_for_all_bits)
{
	base_type_t wait_condition_met = WIFI_ADAPTER_FALSE;

	if (wait_for_all_bits == WIFI_ADAPTER_FALSE) {
		/* Task only has to wait for one bit within bits_to_wait_for to be
		   set.  Is one already set? */
		if ((current_event_bits & bits_to_wait_for) != (event_bits_t) 0) {
			wait_condition_met = WIFI_ADAPTER_TRUE;
		}
	} else {
		/* Task has to wait for all the bits in bits_to_wait_for to be set.
		   Are they set already? */
		if ((current_event_bits & bits_to_wait_for) == bits_to_wait_for) {
			wait_condition_met = WIFI_ADAPTER_TRUE;
		}
	}
	return wait_condition_met;
}

static void task_place_on_unordered_event_list(sq_queue_t *event_lists, const tick_type_t item_value, const tick_type_t tick_to_wait)
{
	if (!event_lists) {
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
	sq_addlast(&pevent_node->node, event_lists);
	struct tcb_s *ptcp = sched_gettcb(pevent_node->pid);
	if (!ptcp) {
		return;
	}
	//block task
	up_block_task(ptcp, TSTATE_TASK_INACTIVE);
	return;
}

static base_type_t task_remove_from_unordered_event_list(event_node_t *event_list_item, sq_queue_t *event_list, const tick_type_t item_value)
{
	base_type_t ret = WIFI_ADAPTER_FALSE;

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

	sq_rem((sq_entry_t *) event_list_item, event_list);

	/* Remove the task from the delayed list and add it to the ready list.  The
	   scheduler is suspended so interrupts will not be accessing the ready
	   lists. */

	if (ptcp) {
		sched_removeblocked(ptcp);
		sched_addreadytorun(ptcp);

		if (ptcp->sched_priority >= current_tcb->sched_priority) {
			/* Return true if the task removed from the event list has
			   a higher priority than the calling task.  This allows
			   the calling task to know if it should force a context
			   switch now. */
			ret = WIFI_ADAPTER_TRUE;
		} else {
			ret = WIFI_ADAPTER_FALSE;
		}
	}
    return ret;
}

static tick_type_t task_reset_event_item_value(event_group_t *event_bits)
{
	tick_type_t ret = -1;

	if (!event_bits) {
		return ret;
	}

	pid_t pid = getpid();
	event_node_t *pevent_node = (event_node_t *) event_bits->tasks_waiting_for_bits.head;
	while (pevent_node) {
		if (pevent_node->pid == pid) {
			ret = pevent_node->item_value;
			// reset the value, but i have no idea about what value to be set.
			pevent_node->item_value = 0;
			break;
		}
		pevent_node = (event_node_t *) pevent_node->node.flink;
	}
	return ret;
}

event_bits_t event_group_wait_bits(event_group_handle_t event_group, const event_bits_t bits_to_wait_for, const base_type_t clear_on_exit, const base_type_t wait_for_all_bits, tick_type_t tick_to_wait)
{
	event_group_t *event_bits = (event_group_t *) event_group;
	event_bits_t ret, ucontrol_bits = 0;
	base_type_t wait_condition_met;

	/* Check the user is not attempting to wait on the bits used by the kernel
	   itself, and that at least one bit is being requested. */

	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);
	{
		const event_bits_t current_event_bits = event_bits->event_bits;

		/* Check to see if the wait condition is already met or not. */
		wait_condition_met = test_wait_condition(current_event_bits, bits_to_wait_for, wait_for_all_bits);
		if (wait_condition_met != WIFI_ADAPTER_FALSE) {
			/* The wait condition has already been met so there is no need to
			   block. */
			ret = current_event_bits;
			tick_to_wait = (tick_type_t) 0;

			/* Clear the wait bits if requested to do so. */
			if (clear_on_exit != WIFI_ADAPTER_FALSE) {
				event_bits->event_bits &= ~bits_to_wait_for;
			}
		} else if (tick_to_wait == (tick_type_t) 0) {
			/* The wait condition has not been met, but no block time was
			   specified, so just return the current value. */
			ret = current_event_bits;
		} else {
			/* The task is going to block to wait for its required bits to be
			   set.  ucontrol_bits are used to remember the specified behaviour of
			   this call to event_group_wait_bits() - for use when the event bits
			   unblock the task. */
			if (clear_on_exit != WIFI_ADAPTER_FALSE) {
				ucontrol_bits |= EVENT_CLEAR_EVENTS_ON_EXIT_BIT;
			}

			if (wait_for_all_bits != WIFI_ADAPTER_FALSE) {
				ucontrol_bits |= EVENT_WAIT_FOR_ALL_BITS;
			}

			/* Store the bits that the calling task is waiting for in the
			   task's event list item so the kernel knows when a match is
			   found.  Then enter the blocked state. */
			//task_place_on_unordered_event_list( &( event_bits->tasks_waiting_for_bits ), ( bits_to_wait_for | ucontrol_bits ), tick_to_wait );

			/* This is obsolete as it will get set after the task unblocks, but
			   some compilers mistakenly generate a warning about the variable
			   being returned without being set if it is not done. */
			ret = 0;
		}
	}

	pthread_mutex_unlock(&event_bits->event_group_mux);
	sched_unlock();

	if (tick_to_wait != (tick_type_t) 0) {
		//task yield
		task_place_on_unordered_event_list(&(event_bits->tasks_waiting_for_bits), (bits_to_wait_for | ucontrol_bits), tick_to_wait);

		/* The task blocked to wait for its required bits to be set - at this
		   point either the required bits were set or the block time expired.  If
		   the required bits were set they will have been stored in the task's
		   event list item, and they should now be retrieved then cleared. */
		ret = task_reset_event_item_value(event_bits);

		if ((ret & EVENT_UNBLOCKED_DUE_TO_BIT_SET) == (event_bits_t) 0) {
			pthread_mutex_lock(&event_bits->event_group_mux);
			/* The task timed out, just return the current event bit value. */
			ret = event_bits->event_bits;

			/* It is possible that the event bits were updated between this
			   task leaving the Blocked state and running again. */
			if (test_wait_condition(ret, bits_to_wait_for, wait_for_all_bits) != WIFI_ADAPTER_FALSE) {
				if (clear_on_exit != WIFI_ADAPTER_FALSE) {
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

	/* Check the user is not attempting to clear the bits used by the kernel
	   itself. */
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
	event_group_t *event_bits = (event_group_t *) event_group;
	base_type_t match_found = WIFI_ADAPTER_FALSE;

	/* Check the user is not attempting to set the bits used by the kernel
	   itself. */
	if (!event_bits || (bits_to_set & EVENT_BITS_CONTROL_BYTES) != 0) {
		return -1;
	}
	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);

	/* Set the bits. */
	event_bits->event_bits |= bits_to_set;

	event_node_t *pevent_node = (event_node_t *) event_bits->tasks_waiting_for_bits.head;
	while (pevent_node) {
		//get the value
		ubits_waited_for = pevent_node->item_value;
		match_found = WIFI_ADAPTER_FALSE;
		/* Split the bits waited for from the control bits. */
		ucontrol_bits = ubits_waited_for & EVENT_BITS_CONTROL_BYTES;
		ubits_waited_for &= ~EVENT_BITS_CONTROL_BYTES;

		if ((ucontrol_bits & EVENT_WAIT_FOR_ALL_BITS) == (event_bits_t) 0) {
			/* Just looking for single bit being set. */
			if ((ubits_waited_for & event_bits->event_bits) != (event_bits_t) 0) {
				match_found = WIFI_ADAPTER_TRUE;
			}
		} else if ((ubits_waited_for & event_bits->event_bits) == ubits_waited_for) {
			/* All bits are set. */
			match_found = WIFI_ADAPTER_TRUE;
		} else {
			/* Need all bits to be set, but not all the bits were set. */
		}

		if (match_found != WIFI_ADAPTER_FALSE) {
			/* The bits match.  Should the bits be cleared on exit? */
			if ((ucontrol_bits & EVENT_CLEAR_EVENTS_ON_EXIT_BIT) != (event_bits_t) 0) {
				bits_to_clear |= ubits_waited_for;
			}

			/* Store the actual event flag value in the task's event list
			   item before removing the task from the event list.   The
			   EVENT_UNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
			   that is was unblocked due to its required bits matching, rather
			   than because it timed out. */

			(void)task_remove_from_unordered_event_list(pevent_node, &event_bits->tasks_waiting_for_bits, event_bits->event_bits | EVENT_UNBLOCKED_DUE_TO_BIT_SET);
		}

		pevent_node = (event_node_t *) pevent_node->node.flink;
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
	event_group_t *event_bits = (event_group_t *) event_group;
	sq_queue_t *ptasks_waiting_for_bits = &(event_bits->tasks_waiting_for_bits);
	event_node_t *pevent_node = NULL;
	event_node_t *pevent_node_temp = NULL;

	sched_lock();
	pthread_mutex_lock(&event_bits->event_group_mux);
	pevent_node = (event_node_t *) event_bits->tasks_waiting_for_bits.head;
	while (pevent_node) {
		task_remove_from_unordered_event_list(pevent_node, ptasks_waiting_for_bits, EVENT_UNBLOCKED_DUE_TO_BIT_SET);
		pevent_node_temp = pevent_node;
		pevent_node = (event_node_t *) pevent_node_temp->node.flink;
		free(pevent_node_temp);
	}
	pthread_mutex_unlock(&event_bits->event_group_mux);	//Exit mux of event group before deleting it
	free(event_bits);
	sched_unlock();
}
