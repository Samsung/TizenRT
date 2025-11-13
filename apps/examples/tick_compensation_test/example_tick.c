/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>

/****************************************************************************
 * systimer_busy_wait_ms
 ****************************************************************************/

static void systimer_busy_wait_ms(unsigned int milliseconds)
{
	volatile unsigned int i;
	volatile unsigned int j;

	for (i = 0; i < milliseconds; i++) {
		for (j = 0; j < CONFIG_BOARD_LOOPSPERMSEC; j++) {
		}
	}
}

/****************************************************************************
 * tick_compensation_test_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tick_compensation_test_main(int argc, char *argv[])
#endif
{
	printf("=========Tick Compensation Test==========\n");
	if (argc < 2) {
		printf("Usage: %s <delay_ms>\n", argv[0]);
		return -1;
	}

	int delay_ms = atoi(argv[1]);
	if (delay_ms < 0) {
		printf("Invalid delay value. Please provide a positive integer.\n");
		return -1;
	}
	/* Extra tick to adjust for small tolerance in tick compensation */
	int extra_tick = (1 + (delay_ms + 999) / 1000);
	if (delay_ms <= 0) {
		printf("Invalid delay value. Please provide a positive integer.\n");
		return -1;
	}

	printf("Testing tick compensation with %d ms delay\n", delay_ms);
	u32 time_before_enter_critical;
	u32 time_before_delay;
	u32 time_after_delay;
	u32 time_after_leave_critical;
	time_before_enter_critical = TICK2MSEC(clock_systimer());
	irqstate_t flags = enter_critical_section();
	time_before_delay = TICK2MSEC(clock_systimer());
	systimer_busy_wait_ms(delay_ms); // Busy wait for delay_ms milliseconds
	time_after_delay = TICK2MSEC(clock_systimer());
	leave_critical_section(flags);
	time_after_leave_critical = TICK2MSEC(clock_systimer());
	printf("Time just before entering critical section: (%lu msec,)\n", (unsigned long)time_before_enter_critical);
	printf("Time just before delay: (%lu msec,)\n", (unsigned long)time_before_delay);
	printf("Time just after delay: (%lu msec,)\n", (unsigned long)time_after_delay);
	printf("Time just after leaving critical section: (%lu msec,)\n", (unsigned long)time_after_leave_critical);

	// Calculate actual time differences
	u32 actual_tick_diff = time_after_leave_critical - time_before_enter_critical;

	// Calculate expected time difference
	u32 expected_diff = delay_ms;

	// Compare actual vs expected
	printf("Expected delay: %lu ms\n", (unsigned long)expected_diff);
	printf("Actual tick time difference: %lu ms\n", (unsigned long)actual_tick_diff);

	// Check if tick compensation is working correctly
	if (actual_tick_diff == expected_diff || (actual_tick_diff > expected_diff && actual_tick_diff <= (expected_diff + extra_tick))) {
		printf("Tick compensation: PASS\n");
	} else {
		printf("Tick compensation: FAIL\n");
	}

	return 0;
}
