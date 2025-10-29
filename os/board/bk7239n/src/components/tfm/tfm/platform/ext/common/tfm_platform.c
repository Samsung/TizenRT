/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "tfm_hal_platform.h"
#include "sdkconfig.h"

__WEAK void tfm_hal_system_reset(void)
{
    NVIC_SystemReset();
}

__WEAK void tfm_hal_system_halt(void)
{
    /*
     * Disable IRQs to stop all threads, not just the thread that
     * halted the system.
     */
    __disable_irq();

    /*
     * Enter sleep to reduce power consumption and do it in a loop in
     * case a signal wakes up the CPU.
     */
    while (1) {
        #if (0 == CONFIG_ENABLE_DEBUG)
        __WFE();
        #endif
    }
}
