/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "compiler_ext_defs.h"
#include "spm_ipc.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"
#include "tfm_hal_device_header.h"
#include "tfm_svcalls.h"
#include "svc_num.h"

#if !defined(__ARM_ARCH_6M__) && !defined(__ARM_ARCH_7M__) && \
    !defined(__ARM_ARCH_7EM__)
#error "Unsupported ARM Architecture."
#endif

/* Delcaraction flag to control the scheduling logic in PendSV. */
uint32_t scheduler_lock = SCHEDULER_UNLOCKED;

/* IAR Specific */
#if defined(__ICCARM__)

#pragma required = do_schedule
#pragma required = scheduler_lock
#pragma required = tfm_core_svc_handler

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

#pragma required = cross_call_entering_c
#pragma required = cross_call_exiting_c

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1*/

#endif

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

__naked void arch_non_preempt_call(uintptr_t fn_addr, uintptr_t frame_addr,
                                   uint32_t stk_base, uint32_t stk_limit)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                \n"
#endif
        "   push   {r4, lr}                             \n"
        "   cpsid  i                                    \n"
        "   cmp    r2, #0                               \n"
        "   beq    v6v7_lock_sched                      \n"
        "   mov    r4, sp                               \n"/* switch stack   */
        "   mov    sp, r2                               \n"
        "   mov    r2, r4                               \n"
        "v6v7_lock_sched:                               \n"/* lock pendsv    */
        "   ldr    r3, =scheduler_lock                  \n"/* R2 = caller SP */
        "   movs   r4, #"M2S(SCHEDULER_LOCKED)"         \n"/* Do not touch   */
        "   str    r4, [r3, #0]                         \n"
        "   cpsie  i                                    \n"
        "   push   {r1, r2}                             \n"
        "   bl     cross_call_entering_c                \n"
        "   pop    {r1, r4}                             \n"
        "   cpsid  i                                    \n"
        "   bl     cross_call_exiting_c                 \n"
        "   cmp    r4, #0                               \n"
        "   beq    v6v7_release_sched                   \n"
        "   mov    sp, r4                               \n"/* switch stack   */
        "v6v7_release_sched:                            \n"
        "   ldr    r2, =scheduler_lock                  \n"/* release pendsv */
        "   movs   r3, #"M2S(SCHEDULER_UNLOCKED)"       \n"
        "   str    r3, [r2, #0]                         \n"
        "   cpsie  i                                    \n"
        "   pop    {r4, pc}                             \n"
    );
}

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1*/

__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                    \n"
#endif
        "   push    {r0, lr}                \n"
        "   bl      do_schedule             \n"
        "   pop     {r2, r3}                \n"
        "   mov     lr, r3                  \n"
        "   cmp     r0, r1                  \n" /* ctx of curr and next thrd */
        "   beq     v6v7_pendsv_exit        \n" /* No schedule if curr = next */
        "   cpsid   i                       \n"
        "   mrs     r2, psp                 \n"
        "   subs    r2, #32                 \n" /* Make room for r4-r11 */
        "   stm     r2!, {r4-r7}            \n" /* Save callee registers */
        "   mov     r4, r8                  \n"
        "   mov     r5, r9                  \n"
        "   mov     r6, r10                 \n"
        "   mov     r7, r11                 \n"
        "   stm     r2!, {r4-r7}            \n"
        "   mov     r5, lr                  \n"
        "   subs    r2, #32                 \n" /* reset r2(SP) to top */
        "   stm     r0!, {r2, r3, r4, r5}   \n" /* Save struct context_ctrl_t */
        "   ldm     r1!, {r2, r3, r4, r5}   \n" /* Load ctx of next thread */
        "   mov     lr, r5                  \n"
        "   adds    r2, #16                 \n" /* Start of popping r4-r11 */
        "   ldm     r2!, {r4-r7}            \n"
        "   mov     r8, r4                  \n"
        "   mov     r9, r5                  \n"
        "   mov     r10, r6                 \n"
        "   mov     r11, r7                 \n"
        "   subs    r2, #32                 \n"
        "   ldm     r2!, {r4-r7}            \n"
        "   adds    r2, #16                 \n" /* End of popping r4-r11 */
        "   msr     psp, r2                 \n"
        "   cpsie   i                       \n"
        "v6v7_pendsv_exit:                  \n"
        "   bx      lr                      \n"
    );
}

__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                        \n"
#endif
    "MRS     r0, MSP                        \n"
    "MOV     r1, lr                         \n"
    "MRS     r2, PSP                        \n"
    "PUSH    {r2, r3}                       \n" /* PSP dummy */
    "PUSH    {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "BL      tfm_core_svc_handler           \n"
    "MOV     lr, r0                         \n"
    "POP     {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "MOVS    r2, #8                         \n"
    "ANDS    r0, r2                         \n" /* Mode bit */
    "ANDS    r1, r2                         \n"
    "POP     {r2, r3}                       \n" /* PSP dummy */
    "SUBS    r0, r1                         \n" /* Compare EXC_RETURN values */
    "BGT     to_flih_func                   \n"
    "BLT     from_flih_func                 \n"
    "BX      lr                             \n"
    "to_flih_func:                          \n"
    "PUSH    {r2, r3}                       \n" /* PSP dummy */
    "PUSH    {r4-r7}                        \n"
    "MOV     r4, r8                         \n"
    "MOV     r5, r9                         \n"
    "MOV     r6, r10                        \n"
    "MOV     r7, r11                        \n"
    "PUSH    {r4-r7}                        \n"
    "LDR     r4, =0xFEF5EDA5                \n" /* clear r4-r11 */
    "MOV     r5, r4                         \n"
    "MOV     r6, r4                         \n"
    "MOV     r7, r4                         \n"
    "MOV     r8, r4                         \n"
    "MOV     r9, r4                         \n"
    "MOV     r10, r4                        \n"
    "MOV     r11, r4                        \n"
    "PUSH    {r4, r5}                       \n" /* Seal stack before EXC_RET */
    "BX      lr                             \n"
    "from_flih_func:                        \n"
    "POP     {r4, r5}                       \n" /* Seal stack */
    "POP     {r4-r7}                        \n"
    "MOV     r8, r4                         \n"
    "MOV     r9, r5                         \n"
    "MOV     r10, r6                        \n"
    "MOV     r11, r7                        \n"
    "POP     {r4-r7}                        \n"
    "POP     {r1, r2}                       \n" /* PSP dummy */
    "BX      lr                             \n"
    );
}

void tfm_arch_set_secure_exception_priorities(void)
{
    /* Set fault priority to the highest */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    NVIC_SetPriority(MemoryManagement_IRQn, 0);
    NVIC_SetPriority(BusFault_IRQn, 0);
#endif

    NVIC_SetPriority(SVCall_IRQn, 0);
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
}

void tfm_arch_config_extensions(void)
{
    /* There are no coprocessors in Armv6-M implementations */
#ifndef __ARM_ARCH_6M__
#if defined(__FPU_USED) && (__FPU_USED == 1U)
    /* Enable privileged and unprivilged access to the floating-point
     * coprocessor.
     */
    SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                  | (3U << 11U*2U);  /* enable CP11 full access */
#endif
#endif
}

/* There is no FPCA in v6m */
#ifndef __ARM_ARCH_6M__
__attribute__((naked, noinline)) void tfm_arch_clear_fp_status(void)
{
    __ASM volatile(
                   ".syntax unified          \n"
                   "mrs  r0, control         \n"
                   "bics r0, r0, #4          \n"
                   "msr  control, r0         \n"
                   "isb                      \n"
                   "bx   lr                  \n"
                  );
}
#else
void tfm_arch_clear_fp_status(void)
{
}
#endif
