/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "portmacrocommon.h"

/*------------------------------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the given hardware
 * and compiler.
 *
 * These settings should not be altered.
 *------------------------------------------------------------------------------
 */

/**
 * Architecture specifics.
 */
#define portARCH_NAME                       "Cortex-M33"
#define portDONT_DISCARD                    __attribute__( ( used ) )
/*-----------------------------------------------------------*/

/**
 * @brief Critical section management.
 */
static inline void _disable_irq_(void)
{
  __asm volatile ("cpsid i" : : : "memory");
}

static inline void _enable_irq_(void)
{
  __asm volatile ("cpsie i" : : : "memory");
}
#define portDISABLE_INTERRUPTS()                  _disable_irq_()
#define portENABLE_INTERRUPTS()                   _enable_irq_()
/*-----------------------------------------------------------*/



uint32_t platform_is_in_interrupt_context(void);
int port_disable_interrupts_flag(void);
void port_enable_interrupts_flag(int val);
uint32_t port_disable_mie_flag(void);
void port_enable_mie_flag(uint32_t val);
uint32_t platform_local_irq_disabled(void);

    #ifdef __cplusplus
        }
    #endif

#endif /* PORTMACRO_H */
