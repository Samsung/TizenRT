/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "cmsis.h"
#include "exception_info.h"

__attribute__((naked)) void HardFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_HARDFAULT);

    /* A HardFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void MemManage_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_MEMFAULT);

    /* A MemManage fault may indicate corruption of secure state, so it is
     * essential that Non-secure code does not regain control after one is
     * raised. Returning from this exception could allow a pending NS exception
     * to be taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void BusFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_BUSFAULT);

    /* A BusFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void SecureFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_SECUREFAULT);

    /* A SecureFault may indicate corruption of secure state, so it is essential
     * that Non-secure code does not regain control after one is raised.
     * Returning from this exception could allow a pending NS exception to be
     * taken, so the current solution is not to return.
     */
    __ASM volatile("b    .");
}

__attribute__((naked)) void UsageFault_Handler(void)
{
    EXCEPTION_INFO(EXCEPTION_TYPE_USAGEFAULT);
    __ASM volatile("b    .");
}
