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

#ifndef EVENT_GROUPS_H
#define EVENT_GROUPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "esp_define.h"

/**
 * An event group is a collection of bits to which an application can assign a
 * meaning.  For example, an application may create an event group to convey
 * the status of various CAN bus related events in which bit 0 might mean "A CAN
 * message has been received and is ready for processing", bit 1 might mean "The
 * application has queued a message that is ready for sending onto the CAN
 * network", and bit 2 might mean "It is time to send a SYNC message onto the
 * CAN network" etc.  A task can then test the bit values to see which events
 * are active, and optionally enter the Blocked state to wait for a specified
 * bit or a group of specified bits to be active.  To continue the CAN bus
 * example, a CAN controlling task can enter the Blocked state (and therefore
 * not consume any processing time) until either bit 0, bit 1 or bit 2 are
 * active, at which time the bit that was actually active would inform the task
 * which action it had to take (process a received message, send a message, or
 * send a SYNC).
 *
 * The event groups implementation contains intelligence to avoid race
 * conditions that would otherwise occur were an application to use a simple
 * variable for the same purpose.  This is particularly important with respect
 * to when a bit within an event group is to be cleared, and when bits have to
 * be set and then tested atomically - as is the case where event groups are
 * used to create a synchronisation point between multiple tasks (a
 * 'rendezvous').
 *
 */

/**
 * event_groups.h
 *
 * Type by which event groups are referenced.  For example, a call to
 * event_group_create() returns an event_group_handle_t variable that can then
 * be used as a parameter to other event group functions.
 *
 */
	typedef void *event_group_handle_t;

/*
 * The type that holds event bits always matches tick_type_t - therefore the
 * number of bits it holds is set by CONFIG_USE_16_BIT_TICKS (16 bits if defined
 * in .config, 32 bits if not.
 *
 */
	typedef tick_type_t event_bits_t;

/**
 * Create a new event group.
 *
 * Internally, event groups use a [small] block of memory, in which the event
 * group's structure is stored.  If an event groups is created using
 * event_group_create() then the required memory is automatically dynamically
 * allocated inside the event_group_create() function.
 *
 * Although event groups are not related to ticks, for internal implementation
 * reasons the number of bits available for use in an event group is dependent
 * on the CONFIG_USE_16_BIT_TICKS setting in .config file.  If
 * CONFIG_USE_16_BIT_TICKS is set then each event group contains 8 usable bits (bit
 * 0 to bit 7).  If CONFIG_USE_16_BIT_TICKS is not set then each event group has
 * 24 usable bits (bit 0 to bit 23).  The event_bits_t type is used to store
 * event bits within an event group.
 *
 * @return If the event group was created then a handle to the event group is
 * returned.  If there was insufficient heap available to create the
 * event group then NULL is returned.
 *
 */

	event_group_handle_t event_group_create(void);

/**
 * [Potentially] block to wait for one or more bits to be set within a
 * previously created event group.
 *
 * This function cannot be called from an interrupt.
 *
 * @param event_group_h The event group in which the bits are being tested.  The
 * event group must have previously been created using a call to
 * event_group_create().
 *
 * @param bits_to_wait_for A bitwise value that indicates the bit or bits to test
 * inside the event group.  For example, to wait for bit 0 and/or bit 2 set
 * uxBitsToWaitFor to 0x05.  To wait for bits 0 and/or bit 1 and/or bit 2 set
 * uxBitsToWaitFor to 0x07.  Etc.
 *
 * @param clear_on_exit If xClearOnExit is set to ture then any bits within
 * uxBitsToWaitFor that are set within the event group will be cleared before
 * event_group_wait_bits() returns if the wait condition was met (if the function
 * returns for a reason other than a timeout).  If clear_on_exit is set to
 * false then the bits set in the event group are not altered when the call to
 * event_group_wait_bits() returns.
 *
 * @param wait_for_all_bits If wait_for_all_bits is set to ture then
 * event_group_wait_bits() will return when either all the bits in bits_to_wait_for
 * are set or the specified block time expires.  If wait_for_all_bits is set to
 * false then event_group_wait_bits() will return when any one of the bits set
 * in bits_to_wait_for is set or the specified block time expires.  The block
 * time is specified by the ticks_to_wait parameter.
 *
 * @param xTicksToWait The maximum amount of time (specified in 'ticks') to wait
 * for one/all (depending on the wait_for_all_bits value) of the bits specified by
 * ticks_to_wait to become set.
 *
 * @return The value of the event group at the time either the bits being waited
 * for became set, or the block time expired.  Test the return value to know
 * which bits were set.  If event_group_wait_bits() returned because its timeout
 * expired then not all the bits being waited for will be set.  If
 * event_group_wait_bits() returned because the bits it was waiting for were set
 * then the returned value is the event group value before any bits were
 * automatically cleared in the case that xClearOnExit parameter was set to
 * ture.
 *
 */
	event_bits_t event_group_wait_bits(event_group_handle_t event_group_h, const event_bits_t bits_to_wait_for, const base_type_t clear_on_exit, const base_type_t wait_for_all_bits, tick_type_t ticks_to_wait);

/**
 * Clear bits within an event group.  This function cannot be called from an
 * interrupt.
 *
 * @param event_group_h The event group in which the bits are to be cleared.
 *
 * @param bits_to_wait_for A bitwise value that indicates the bit or bits to clear
 * in the event group.  For example, to clear bit 3 only, set bits_to_clear to
 * 0x08.  To clear bit 3 and bit 0 set bits_to_clear to 0x09.
 *
 * @return The value of the event group before the specified bits were cleared.
 *
 */
	event_bits_t event_group_clear_bits(event_group_handle_t event_group_h, const event_bits_t bits_to_clear);

/**
 * Set bits within an event group.
 * This function cannot be called from an interrupt.  event_group_set_bitsFromISR()
 * is a version that can be called from an interrupt.
 *
 * Setting bits in an event group will automatically unblock tasks that are
 * blocked waiting for the bits.
 *
 * @param event_group_h The event group in which the bits are to be set.
 *
 * @param bits_to_set A bitwise value that indicates the bit or bits to set.
 * For example, to set bit 3 only, set uxBitsToSet to 0x08.  To set bit 3
 * and bit 0 set bits_to_set to 0x09.
 *
 * @return The value of the event group at the time the call to
 * event_group_set_bits() returns.  There are two reasons why the returned value
 * might have the bits specified by the bits_to_set parameter cleared.  First,
 * if setting a bit results in a task that was waiting for the bit leaving the
 * blocked state then it is possible the bit will be cleared automatically
 * (see the clear_bit_on_exit parameter of event_group_wait_bits()).  Second, any
 * unblocked (or otherwise Ready state) task that has a priority above that of
 * the task that called event_group_set_bits() will execute and may change the
 * event group value before the call to event_group_set_bits() returns.
 *
 */
	event_bits_t event_group_set_bits(event_group_handle_t event_group_h, const event_bits_t bits_to_set);

/**
 *
 * Delete an event group that was previously created by a call to
 * event_group_create().  Tasks that are blocked on the event group will be
 * unblocked and obtain 0 as the event group's value.
 *
 * @param event_group_h The event group being deleted.
 */
	void event_group_delete(event_group_handle_t event_group_h);

/** @endcond */

#ifdef __cplusplus
}
#endif
#endif							/* EVENT_GROUPS_H */
