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
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include "health_monitor/health_monitor.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: health_monitor_get_next_check_time
 *
 * Description:
 *   Read the published earliest deadline tick via lockless snapshot.
 *
 * Inputs:
 *   check_at - Output pointer for earliest deadline tick. Must not be NULL.
 *
 * Return Value:
 *   1 and write check_at if registered threads exist; 0 if empty; -EAGAIN if a
 *   concurrent update was detected.
 *   Leaves output unchanged on 0/-EAGAIN.
 *
 * Assumptions:
 *   Returned tick is a hint; caller must not assume expiry without inspection.
 *
 ****************************************************************************/

int health_monitor_get_next_check_time(clock_t *check_at)
{
	uint32_t sequence;
	uint32_t count;
	clock_t next;
#ifdef CONFIG_SYSTEM_TIME64
	const volatile uint32_t *addr;
	uint32_t high;
	uint32_t low;
	uint32_t high_check;
#endif

	DEBUGASSERT(check_at != NULL);

	/* Reject snapshot if writer is currently updating (odd sequence). */
	sequence = g_health_sequence;
	if (sequence & 1) {
		return -EAGAIN;
	}
#ifdef CONFIG_SMP
	/* SP_DMB() orders stores only; snapshot loads need a full barrier. */
	SP_DSB();
#endif

	count = g_health_next_count;
	if (count == 0) {
#ifdef CONFIG_SMP
		SP_DSB();
#endif
		if (sequence != g_health_sequence) {
			return -EAGAIN;
		}
		return 0;
	}

#ifdef CONFIG_SYSTEM_TIME64
	/* Sample high/low/high */
	addr = (const volatile uint32_t *)&g_health_next_tick;
	high = addr[1];
	low = addr[0];
	high_check = addr[1];
	if (high != high_check) {
		return -EAGAIN;
	}
	next = ((clock_t)high << 32) | low;
#else
	next = g_health_next_tick;
#endif

	/* Check whether the sequence changed during the read. */
#ifdef CONFIG_SMP
	SP_DSB();
#endif
	if (sequence != g_health_sequence) {
		return -EAGAIN;
	}

	*check_at = next;
	return 1;
}
