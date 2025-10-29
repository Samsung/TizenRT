/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "compiler_ext_defs.h"
#include "spm_ipc.h"
#include "svc_num.h"
#include "tfm_hal_device_header.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"
#include "tfm_secure_api.h"
#include "tfm_svcalls.h"
#include "utilities.h"

#if !defined(__ARM_ARCH_8M_BASE__)
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

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1 */

#endif

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

__naked void arch_non_preempt_call(uintptr_t fn_addr, uintptr_t frame_addr,
                                   uint32_t stk_base, uint32_t stk_limit)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                \n"
#endif
        "   push   {r4-r6, lr}                          \n"
        "   cpsid  i                                    \n"
        "   mov    r4, r2                               \n"
        "   cmp    r2, #0                               \n"
        "   beq    v8b_lock_sched                       \n"
        "   mrs    r5, psplim                           \n"/* To caller stack */
        "   movs   r4, #0                               \n"
        "   msr    psplim, r4                           \n"
        "   mov    r4, sp                               \n"
        "   mov    sp, r2                               \n"
        "   msr    psplim, r3                           \n"
        "v8b_lock_sched:                                \n"/* To lock sched */
        "   ldr    r2, =scheduler_lock                  \n"
        "   movs   r3, #"M2S(SCHEDULER_LOCKED)"         \n"
        "   str    r3, [r2, #0]                         \n"
        "   cpsie  i                                    \n"
        "   mov    r6, r1                               \n"
        "   bl     cross_call_entering_c                \n"
        "   cpsid  i                                    \n"
        "   mov    r1, r6                               \n"
        "   bl     cross_call_exiting_c                 \n"
        "   cmp    r4, #0                               \n"
        "   beq    v8b_release_sched                    \n"
        "   movs   r3, #0                               \n"/* To callee stack */
        "   msr    psplim, r3                           \n"
        "   mov    sp, r4                               \n"
        "   msr    psplim, r5                           \n"
        "v8b_release_sched:                             \n"
        "   ldr    r2, =scheduler_lock                  \n"/* To unlock sched */
        "   movs   r3, #"M2S(SCHEDULER_UNLOCKED)"       \n"
        "   str    r3, [r2, #0]                         \n"
        "   cpsie  i                                    \n"
        "   pop    {r4-r6, pc}                          \n"
    );
}

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1 */

__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                \n"
#endif
        "   movs    r0, #"M2S(EXC_RETURN_SECURE_STACK)" \n"
        "   mov     r1, lr                              \n"
        "   tst     r0, r1                              \n" /* NS interrupted */
        "   beq     v8b_pendsv_exit                     \n" /* No schedule */
        "   push    {r0, lr}                            \n" /* Save R0, LR */
        "   bl      do_schedule                         \n"
        "   pop     {r2, r3}                            \n"
        "   mov     lr, r3                              \n"
        "   cmp     r0, r1                              \n" /* curr, next ctx */
        "   beq     v8b_pendsv_exit                     \n" /* No schedule */
        "   cpsid   i                                   \n"
        "   mrs     r2, psp                             \n"
        "   mrs     r3, psplim                          \n"
        "   subs    r2, #32                             \n" /* For r4-r7 */
        "   stm     r2!, {r4-r7}                        \n" /* Save callee */
        "   mov     r4, r8                              \n"
        "   mov     r5, r9                              \n"
        "   mov     r6, r10                             \n"
        "   mov     r7, r11                             \n"
        "   stm     r2!, {r4-r7}                        \n"
        "   mov     r5, lr                              \n"
        "   subs    r2, #32                             \n" /* set SP to top */
        "   stm     r0!, {r2, r3, r4, r5}               \n" /* Save curr ctx */
        "   ldm     r1!, {r2, r3, r4, r5}               \n" /* Load next ctx */
        "   mov     lr, r5                              \n"
        "   adds    r2, #16                             \n" /* Pop r4-r11 */
        "   ldm     r2!, {r4-r7}                        \n"
        "   mov     r8, r4                              \n"
        "   mov     r9, r5                              \n"
        "   mov     r10, r6                             \n"
        "   mov     r11, r7                             \n"
        "   subs    r2, #32                             \n"
        "   ldm     r2!, {r4-r7}                        \n"
        "   adds    r2, #16                             \n" /* Pop r4-r11 end */
        "   msr     psp, r2                             \n"
        "   msr     psplim, r3                          \n"
        "   cpsie   i                                   \n"
        "v8b_pendsv_exit:                               \n"
        "   bx      lr                                  \n"
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
    "MRS     r3, PSPLIM                     \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
    "PUSH    {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "BL      tfm_core_svc_handler           \n"
    "MOV     lr, r0                         \n"
    "POP     {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "MOVS    r2, #8                         \n"
    "ANDS    r0, r2                         \n" /* Mode bit */
    "ANDS    r1, r2                         \n"
    "POP     {r2, r3}                       \n" /* PSP PSPLIM */
    "SUBS    r0, r1                         \n" /* Compare EXC_RETURN values */
    "BGT     to_flih_func                   \n"
    "BLT     from_flih_func                 \n"
    "BX      lr                             \n"
    "to_flih_func:                          \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
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
    "POP     {r1, r2}                       \n" /* PSP PSPLIM */
    "BX      lr                             \n"
    );
}

void tfm_arch_set_secure_exception_priorities(void)
{
    uint32_t VECTKEY;
    SCB_Type *scb = SCB;
    uint32_t AIRCR;

    /* Set PRIS flag in AIRCR */
    AIRCR = scb->AIRCR;
    VECTKEY = (~AIRCR & SCB_AIRCR_VECTKEYSTAT_Msk);
    scb->AIRCR = SCB_AIRCR_PRIS_Msk |
                 VECTKEY |
                 (AIRCR & ~SCB_AIRCR_VECTKEY_Msk);

    NVIC_SetPriority(SVCall_IRQn, 0);
#ifdef TFM_MULTI_CORE_TOPOLOGY
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
#else
    /*
     * Set secure PendSV priority to the lowest in SECURE state.
     *
     * IMPORTANT NOTE:
     *
     * Although the priority of the secure PendSV must be the lowest possible
     * among other interrupts in the Secure state, it must be ensured that
     * PendSV is not preempted nor masked by Non-Secure interrupts to ensure
     * the integrity of the Secure operation.
     * When AIRCR.PRIS is set, the Non-Secure execution can act on
     * FAULTMASK_NS, PRIMASK_NS or BASEPRI_NS register to boost its priority
     * number up to the value 0x80.
     * For this reason, set the priority of the PendSV interrupt to the next
     * priority level configurable on the platform, just below 0x80.
     */
    NVIC_SetPriority(PendSV_IRQn, (1 << (__NVIC_PRIO_BITS - 1)) - 1);
#endif
}

/* There are no coprocessors in Armv8-M Baseline implementations */
void tfm_arch_config_extensions(void)
{
}

/* There is no FPCA in baseline. */
void tfm_arch_clear_fp_status(void)
{
}
