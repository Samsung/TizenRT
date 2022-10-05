/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
 * arch/arm/src/stm32h745/stm32h745_systemreset.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdint.h>
#include <tinyara/board.h>
#include <tinyara/reboot_reason.h>
#include "up_arch.h"
#include <sys/boardctl.h>

#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_wwdg.h>
#include <stm32h7xx_hal.h>
#include <stm32h745_systemreset.h>

/****************************************************************************
 * Name: up_systemreset
 *
 * Description:
 *   Internal, reset logic.
 *
 ****************************************************************************/
#define CONFIG_STM32_WWDG_SETWINDOW (100)
static void up_systemreset(void)
{
    /* DeInit peripheral before reset */
    HAL_SuspendTick();

    /* Should start wwdg1 before system reset */
    LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_WWDG1);
    LL_WWDG_SetCounter(WWDG1, CONFIG_STM32_WWDG_SETWINDOW);
    LL_WWDG_SetPrescaler(WWDG1, LL_WWDG_PRESCALER_128);
    LL_WWDG_SetWindow(WWDG1, CONFIG_STM32_WWDG_SETWINDOW);
    LL_WWDG_ClearFlag_EWKUP(WWDG1);
    LL_WWDG_Enable(WWDG1);

    while(!LL_WWDG_IsEnabled(WWDG1)){};

	__disable_irq();

    __DSB();
    __ISB();

	for (;;)
	{
        __WFI();
	}
}

#ifdef CONFIG_SYSTEM_REBOOT_REASON
/****************************************************************************
 * Name: stm32h745_reboot_reason_get_hw_value
 *
 * Description:
 *   Internal, reset logic.
 *
 ****************************************************************************/
static reboot_reason_code_t reboot_reason;
static reboot_reason_code_t stm32h745_reboot_reason_get_hw_value(void)
{
    lldbg("RCC RSR=0x%08x\n", RCC->RSR);
    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_IWDG1RST))
    {
        return REBOOT_SYSTEM_WATCHDOG;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_IWDG2RST))
    {
        return REBOOT_SYSTEM_WATCHDOG;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_WWDG2RST))
    {
        return REBOOT_SYSTEM_WATCHDOG;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_WWDG1RST))
    {
        return REBOOT_SYSTEM_WATCHDOG;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_SFTR1ST))
    {
        return REBOOT_SYSTEM_SFTR1ST_RESET;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_SFTR2ST))
    {
        return REBOOT_SYSTEM_SFTR2ST_RESET;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_PINRST))
    {
        return REBOOT_SYSTEM_HW_RESET;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_PORRST))
    {
        return REBOOT_SYSTEM_HW_RESET;
    }

    if(__HAL_RCC_C1_GET_FLAG(RCC_FLAG_BORRST))
    {
        return REBOOT_SYSTEM_BOR_RESET;
    }

    return REBOOT_UNKNOWN;
}
/****************************************************************************
 * Public functions
 ****************************************************************************/
/****************************************************************************
 * Name: up_reboot_reason_init
 ****************************************************************************/
void up_reboot_reason_init(void)
{
    reboot_reason = stm32h745_reboot_reason_get_hw_value();
    __HAL_RCC_C1_CLEAR_RESET_FLAGS();
}

/****************************************************************************
 * Name: up_reboot_reason_read
 ****************************************************************************/
reboot_reason_code_t up_reboot_reason_read(void)
{
    return reboot_reason;
}

/****************************************************************************
 * Name: up_reboot_reason_write
 ****************************************************************************/
void up_reboot_reason_write(reboot_reason_code_t reason)
{
    reboot_reason = reason;
}

/****************************************************************************
 * Name: reboot_reason_write_user_intended
 ****************************************************************************/
void reboot_reason_write_user_intended(void)
{

}

/****************************************************************************
 * Name: up_reboot_reason_clear
 ****************************************************************************/
void up_reboot_reason_clear(void)
{
    __HAL_RCC_C1_CLEAR_RESET_FLAGS();
    up_reboot_reason_write(REBOOT_REASON_INITIALIZED);
}

/****************************************************************************
 * Name: up_reboot_reason_is_written
 ****************************************************************************/
bool up_reboot_reason_is_written(void)
{
    if(reboot_reason == REBOOT_REASON_INITIALIZED)
    {
        return false;
    }

    return true;
}

#endif

/****************************************************************************
 * Name: board_reset
 *
 * Description:
 *   Reset board.  This function may or may not be supported by a
 *   particular board architecture.
 *
 * Input Parameters:
 *   status - Status information provided with the reset event.  This
 *     meaning of this status information is board-specific.  If not used by
 *     a board, the value zero may be provided in calls to board_reset.
 *
 * Returned Value:
 *   If this function returns, then it was not possible to power-off the
 *   board due to some constraints.  The return value int this case is a
 *   board-specific reason for the failure to shutdown.
 *
 ****************************************************************************/
int board_reset(int status)
{
	if (status == M7_M4_RESET) {	/* only if status is M7_M4_RESET */
		stm32h745_system_reset();
	} else {			/* in all other cases reset only M7 */
		up_systemreset();
	}
	return 0;
}

/****************************************************************************
 * Name: stm32h745_system_reset
 *
 * Description:
 *   Reset board.  This function may or may not be supported by a
 *   particular board architecture.
 *
 ****************************************************************************/
void stm32h745_system_reset(void)
{
    __NVIC_SystemReset();
}






