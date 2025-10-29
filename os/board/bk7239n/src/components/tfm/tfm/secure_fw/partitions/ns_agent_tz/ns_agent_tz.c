/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "compiler_ext_defs.h"
#include "security_defs.h"
#include "tfm_arch.h"
#include "tfm_hal_platform.h"

#if defined(__ICCARM__)
static uint32_t ns_agent_tz_init_c(void);
#pragma required = ns_agent_tz_init_c
#endif

/*
 * Initialization before launching NSPE in C.
 */
__used static uint32_t ns_agent_tz_init_c(void)
{
    /* SCB_NS.VTOR points to the Non-secure vector table base address */
    SCB_NS->VTOR = tfm_hal_get_ns_VTOR();

    /* Setups Main stack pointer of the non-secure code */
    __TZ_set_MSP_NS(tfm_hal_get_ns_MSP());

    return tfm_hal_get_ns_entry_point();
}

/*
 * 'r0' implicitly holds the address of non-secure entry,
 * given during non-secure partition initialization.
 */
__naked void ns_agent_tz_main(void)
{
    __ASM volatile(
#ifndef __ICCARM__
        ".syntax unified                            \n"
#endif
        "   bl       ns_agent_tz_init_c             \n"
        "   ldr      r2, [sp]                       \n"
        "   ldr      r3, ="M2S(STACK_SEAL_PATTERN)" \n" /* SEAL double-check */
        "   cmp      r2, r3                         \n"
        "   bne      ns_agent_nspe_jump_panic       \n"
        "   mov      r4, r0                         \n"
        "   movs     r2, #1                         \n" /* For NS execution */
        "   bics     r4, r2                         \n"
        "   mov      r0, r4                         \n"
        "   mov      r1, r4                         \n"
        "   mov      r2, r4                         \n"
        "   mov      r3, r4                         \n"
        "   mov      r5, r4                         \n"
        "   mov      r6, r4                         \n"
        "   mov      r7, r4                         \n"
        "   mov      r8, r4                         \n"
        "   mov      r9, r4                         \n"
        "   mov      r10, r4                        \n"
        "   mov      r11, r4                        \n"
        "   mov      r12, r4                        \n"
        "   mov      r14, r4                        \n"
        "   bxns     r0                             \n"
        "ns_agent_nspe_jump_panic:                  \n"
        "   b        .                              \n"
    );
}
