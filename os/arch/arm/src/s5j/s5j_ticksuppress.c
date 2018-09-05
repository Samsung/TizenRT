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
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <debug.h>
#include <queue.h>
#include <tinyara/wdog.h>
#include <tinyara/clock.h>
#include <tinyara/time.h>
#include <tinyara/wdog.h>
#include <arch/board/board.h>

#include "up_arch.h"
#include "chip/s5jt200_rtc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*
 * The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 * The timer counts at the rate SYSCLK_FREQUENCY as defined in the board.h
 * header file.
 */
#define SYSTICK_RELOAD ((SYSCLK_FREQUENCY / CLK_TCK) - 1)

#define MAX_TCNT UINT32_MAX	/* Max 32-bit timer-tick value */

/****************************************************************************
 * Type Definitions
 ****************************************************************************/
enum {
	TICKSUPPRESS_DISABLE,
	TICKSUPPRESS_ENABLE
} timertick_mode;

enum {
	START,
	READ,
	WRITE,
	STOP
} timer_states;

/****************************************************************************
 * Private Data
 ****************************************************************************/
static uint8_t g_tick_mode;	/* timer-tick mode  */

static uint32_t last_event = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t s5j_get_next_event(void)
{
	return (uint32_t)wd_getdelay();
}

static uint32_t s5j_timer_setup(int state, uint32_t value)
{
	switch (state) {
		case START:
			modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICCKSEL0_MASK, RTC_RTCCON_TICKEN0_ENABLE | RTC_RTCCON_TICCKSEL0_32768HZ);
			break;
		case READ:
			return getreg32(S5J_RTC_CURTICCNT0);
		case WRITE:
			putreg32(SYSTICK_RELOAD * value, S5J_RTC_TICCNT0);
			break;
		case STOP:
			/* Clear interrupt pending */

			putreg32(RTC_INTP_TIMETIC0, S5J_RTC_INTP);

			/* Disable timertick */

			modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICKEN0_ENABLE, 0);
			break;
		default:
			/* Shouldn't reach here */
			DEBUGPANIC();
			break;
	}

	return 0;
}

static void s5j_set_next_event(uint32_t next_event)
{
	/* Disable timer tick */

	s5j_timer_setup(STOP, 0);

	/* Configure the RTC timetick to generate interrupt on next event */

	s5j_timer_setup(WRITE, next_event);

	/* Enable timer tick */

	s5j_timer_setup(START, 0);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_stop_ticksuppress
 *
 * Description:
 *   Disables tick suppression (re-enables periodic ticks) when,
 *   1. Scheduled event expires
 *   2. Context switch takes place on an external event (interrupt)
 *
 ****************************************************************************/

void up_stop_ticksuppress(void)
{
	/* If tick suppression is disabled, do nothing! */

	if (g_tick_mode == TICKSUPPRESS_DISABLE) {
		return;
	}

	/* If this function called from interrupt context,
	 * read current value of the counter and update last event value
	 */

	if (up_interrupt_context()) {
		last_event = ((uint32_t)s5j_timer_setup(READ, 0) / SYSTICK_RELOAD);
	}

	/* This function will be called when last scheduled event expires.
	 * so update the global system timer using expired last event value.
	 */

	if (last_event) {
		/* Update global system timer */

		g_system_timer += last_event;

		/* Update wdog delay */

		wd_updatedelay(last_event);

		/* Reset last event */

		last_event = 0;
	}

	/* Change timer tick state as periodic */

	g_tick_mode = TICKSUPPRESS_DISABLE;

	/* Re-enable periodic timer ticks */

	s5j_set_next_event(0x1);
}

/*****************************************************************************
 * Function:  up_start_ticksuppress
 *
 * Description:
 *   Enables tick suppression (disables periodic ticks), when system goes idle
 *
 *****************************************************************************/

void up_start_ticksuppress(void)
{
	uint32_t next_event = 0;

	/* Check whether the threshold value is reached or not
	 * If threshold value is crossed, ticks are suppressed and
	 * next expected event is scheduled.
	 */

	s5j_timer_setup(STOP, 0);

	next_event = s5j_get_next_event();
	if (!next_event) {
		next_event = MAX_TCNT;
	}
	last_event = next_event;

	g_tick_mode = TICKSUPPRESS_ENABLE;

	/* Schedule next expected event */

	s5j_set_next_event(next_event);
}

/****************************************************************************
 * Function:  up_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

int up_timerisr(int irq, FAR void *context, FAR void *arg)
{
	if (g_tick_mode == TICKSUPPRESS_ENABLE) {

		/* Disable tick suppression */

		up_stop_ticksuppress();
	}

	/* Clear interrupt pending */

	putreg32(RTC_INTP_TIMETIC0, S5J_RTC_INTP);

	/* Process timer interrupt */

	sched_process_timer();

	return 0;
}

/****************************************************************************
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
	/* OSC_CON[16] should be set to 1, so that RTC uses XRTCXTO as srcclk */

	modifyreg32(0x800A0554, 0x0, 1 << 16);

	g_tick_mode = TICKSUPPRESS_DISABLE;

	/* Configure the RTC timetick to generate periodic interrupts */

	modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICKEN0_ENABLE, 0);
	putreg32(SYSTICK_RELOAD, S5J_RTC_TICCNT0);
	modifyreg32(S5J_RTC_RTCCON, RTC_RTCCON_TICCKSEL0_MASK, RTC_RTCCON_TICKEN0_ENABLE | RTC_RTCCON_TICCKSEL0_32768HZ);

	/* Attach the timer interrupt vector */

	irq_attach(IRQ_TOP_RTC_TIC, up_timerisr, NULL);

	/* Enable the timer interrupt */

	up_enable_irq(IRQ_TOP_RTC_TIC);
}
