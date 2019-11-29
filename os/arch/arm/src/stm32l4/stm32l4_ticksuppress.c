/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 * Author: Nitish Ambastha <nitish.a@samsung.com>
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
#include <arch/board/stm32l4r9ai-disco-clocking.h>

#include "up_arch.h"
#include "stm32l4_lptim.h"
#include "chip/stm32l4xrxx_rcc.h"
#include "stm32l4xx_hal.h"
#include <tinyara/pm/pm.h>
#include "nvic.h"
#include "clock/clock.h"
#include "up_internal.h"
#include "up_arch.h"

#include "chip.h"
#include "stm32l4.h"
#include "stm32l4xx_hal_interface.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MAX_TCNT UINT32_MAX	/* Max 32-bit timer-tick value */


/****************************************************************************
 * Type Definitions
 ****************************************************************************/
enum {
	TICKSUPPRESS_DISABLE,
	TICKSUPPRESS_ENABLE
} timertick_mode;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint8_t g_tick_mode;		/* timer-tick mode  */
clock_t next_intval;			/* next delay interval  */
static TIM_HandleTypeDef htim2;		/* TIM handle  */


#define STM32L4_TICKLESS_IDLE_ACTIVITY		5
#define TIM2_PRESCALER				2

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  stm32l4_get_next_delay
 *
 * Description:
 *  This function returns delay (in system ticks) from head of wdog active list.
 *  It calls wd_getdelay() provided by RTOS
 *
 ****************************************************************************/
static uint32_t stm32l4_get_next_delay(void)
{
	return (uint32_t)wd_getdelay();
}

/****************************************************************************
 * Function:  stm32l4_tim2_handler
 *
 * Description:
 *   The tim2 ISR to execute watddog expiration and modify ticks based on elapsed gap
 *
 ****************************************************************************/
int stm32l4_tim2_handler(int irq, uint32_t *regs)
{
	irqstate_t flags;
	uint32_t next_event = 0;
	uint32_t elapsed = 0;

	/* Clear interrupt pending */
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);

	pm_activity(0, STM32L4_TICKLESS_IDLE_ACTIVITY);

	elapsed = next_intval;
	next_intval = 0;

	execute_waketime_interrupt(elapsed);

	return 0;
}

/****************************************************************************
 * Function:  tim2_set_trigger
 *
 * Description:
 *   This function is called to initialize and configure timer TIM2
 *   when the kernel next needs to execute
 *
 ****************************************************************************/
void tim2_set_trigger(int next_event)
{
	__HAL_RCC_TIM2_CLK_ENABLE();
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = TIM2_PRESCALER;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.RepetitionCounter = 0;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	htim2.Init.Period = (uint32_t)(next_event * USEC_PER_TICK * (BOARD_TIM2_FREQUENCY/(USEC_PER_SEC * TIM2_PRESCALER)));

	if(HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		printf("HAL TIM Base Init Error\n");
	}

	/* Clear interrupt pending if any */
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
	(void)irq_attach(STM32L4_IRQ_TIM2, stm32l4_tim2_handler, NULL);
	up_enable_irq(STM32L4_IRQ_TIM2);

	if(HAL_TIM_Base_Start_IT(&htim2) != HAL_OK)
	{
		printf("HAL TIM Base Start IT Error\n");
	}
}

/*****************************************************************************
 * Function:  schedule_expected_event
 *
 * Description:
 *   Get next expected event
 *   Enables tick suppression (disables periodic ticks), when system goes idle
 *
 *****************************************************************************/
void schedule_expected_event(void)
{
	irqstate_t flags;
	uint32_t next_delay;

	next_delay = stm32l4_get_next_delay();
	if (!next_delay) {
		next_delay = MAX_TCNT;
	}

	flags = irqsave();

	next_intval = next_delay;

	/* Configure timer when the kernel next needs to execute*/
	tim2_set_trigger(next_delay);

	irqrestore(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  set_waketime_interrupt
 *
 * Description:
 *   This function configures an interrupt to bring MCU out of low
 *   power state after the expected idle time. This timer should be
 *   active even when the MCU is in low power state
 *
 ****************************************************************************/
extern void set_waketime_interrupt(void)
{
	if (g_tick_mode == TICKSUPPRESS_DISABLE) {
		g_tick_mode = TICKSUPPRESS_ENABLE;

		schedule_expected_event();
	}
}

/****************************************************************************
 * Function:  execute_waketime_interrupt
 *
 * Description:
 *   This function is called to disable the timer interrupt configured
 *   to bring MCU out of low power state after expected idle time.
 *   Checks and execute a watchdog function based on time gap
 *
 ****************************************************************************/
extern void execute_waketime_interrupt(int elapsed)
{
	if (g_tick_mode == TICKSUPPRESS_ENABLE)
	{
		g_tick_mode = TICKSUPPRESS_DISABLE;
		/* Check if it is time to execute a watchdog function */
		wd_timer_nohz(elapsed);
		/* Disable TIM2 */
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
		HAL_TIM_Base_DeInit(&htim2);
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
}

/****************************************************************************
 * Function:  supress_ticks
 *
 * Description:
 *   This function is called to supress ticks
 *
 ****************************************************************************/
extern void supress_ticks(void)
{
	supress_hal_tick();

	/* Disable SysTick */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/****************************************************************************
 * Function:  enable_ticks
 *
 * Description:
 *   This function is called to enable ticks
 *
 ****************************************************************************/
extern void enable_ticks(void)
{
	/* Enable SysTick */
	SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;

	/* Enable HALTick */
	enable_hal_tick();
}
