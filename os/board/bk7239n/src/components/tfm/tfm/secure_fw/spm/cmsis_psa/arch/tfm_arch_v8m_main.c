/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <inttypes.h>
#include "compiler_ext_defs.h"
#include "region_defs.h"
#include "spm_ipc.h"
#include "svc_num.h"
#include "tfm_arch.h"
#include "tfm_core_utils.h"
#include "tfm_hal_device_header.h"
#include "tfm_memory_utils.h"
#include "tfm_secure_api.h"
#include "tfm_svcalls.h"
#include "utilities.h"
#if defined(__FPU_USED) && (__FPU_USED == 1U) && (CONFIG_TFM_FP >= 1)
#include "core_ext.h"
#endif

#if !defined(__ARM_ARCH_8M_MAIN__) && !defined(__ARM_ARCH_8_1M_MAIN__)
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
        ".syntax unified                            \n"
#endif
        "   push   {r4-r6, lr}                      \n"
        "   cpsid  i                                \n"
        "   mov    r4, r2                           \n"
        "   mrs    r5, psplim                       \n"
        "   movs   r12, #0                          \n"
        "   cmp    r2, #0                           \n"
        "   itttt  ne                               \n" /* To callee stack */
        "   msrne  psplim, r12                      \n"
        "   movne  r4, sp                           \n"
        "   movne  sp, r2                           \n"
        "   msrne  psplim, r3                       \n"
        "   ldr    r2, =scheduler_lock              \n" /* To lock sched */
        "   movs   r3, #"M2S(SCHEDULER_LOCKED)"     \n"
        "   str    r3, [r2, #0]                     \n"
        "   cpsie  i                                \n"
        "   mov    r6, r1                           \n"
        "   bl     cross_call_entering_c            \n"
        "   cpsid  i                                \n"
        "   mov    r1, r6                           \n"
        "   bl     cross_call_exiting_c             \n"
        "   movs   r12, #0                          \n"
        "   cmp    r4, #0                           \n"
        "   ittt   ne                               \n" /* To caller stack */
        "   msrne  psplim, r12                      \n"
        "   movne  sp, r4                           \n"
        "   msrne  psplim, r5                       \n"
        "   ldr    r4, =scheduler_lock              \n" /* To unlock sched */
        "   movs   r5, #"M2S(SCHEDULER_UNLOCKED)"   \n"
        "   str    r5, [r4, #0]                     \n"
        "   cpsie  i                                \n"
        "   pop    {r4-r6, pc}                      \n"
    );
}

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1*/

__attribute__((naked)) void PendSV_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                \n"
#endif
        "   movs    r0, #"M2S(EXC_RETURN_SECURE_STACK)" \n"
        "   ands    r0, lr                              \n" /* NS interrupted */
        "   beq     v8m_pendsv_exit                     \n" /* No schedule */
        "   push    {r0, lr}                            \n" /* Save R0, LR */
        "   bl      do_schedule                         \n"
        "   pop     {r2, lr}                            \n"
        "   cmp     r0, r1                              \n" /* curr, next ctx */
        "   beq     v8m_pendsv_exit                     \n" /* No schedule */
        "   cpsid   i                                   \n"
        "   mrs     r2, psp                             \n"
        "   mrs     r3, psplim                          \n"
        "   stmdb   r2!, {r4-r11}                       \n" /* Save callee */
        "   stmia   r0, {r2, r3, r4, lr}                \n" /* Save curr ctx */
        "   ldmia   r1, {r2, r3, r4, lr}                \n" /* Load next ctx */
        "   ldmia   r2!, {r4-r11}                       \n" /* Restore callee */
        "   msr     psp, r2                             \n"
        "   msr     psplim, r3                          \n"
        "   cpsie   i                                   \n"
        "v8m_pendsv_exit:                               \n"
        "   bx      lr                                  \n"
    );
}

#if defined(__ICCARM__)
uint32_t tfm_core_svc_handler(uint32_t *msp, uint32_t exc_return,
                              uint32_t *psp);
#pragma required = tfm_core_svc_handler
#endif

__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r0, MSP                        \n"
    "MOV     r1, lr                         \n"
    "MRS     r2, PSP                        \n"
    "MRS     r3, PSPLIM                     \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
    "PUSH    {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "BL      tfm_core_svc_handler           \n"
    "MOV     lr, r0                         \n"
    "POP     {r1, r2}                       \n" /* Orig_exc_return, dummy */
    "POP     {r2, r3}                       \n" /* PSP PSPLIM */
    "AND     r0, #8                         \n" /* Mode bit */
    "AND     r1, #8                         \n"
    "SUBS    r0, r1                         \n" /* Compare EXC_RETURN values */
    "BGT     to_flih_func                   \n"
    "BLT     from_flih_func                 \n"
    "BX      lr                             \n"
    "to_flih_func:                          \n"
    "PUSH    {r2, r3}                       \n" /* PSP PSPLIM */
    "PUSH    {r4-r11}                       \n"
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
    "POP     {r4-r11}                       \n"
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
    /* Set fault priority to less than 0x80 (with AIRCR.PRIS set) to prevent
     * Non-secure from pre-empting faults that may indicate corruption of Secure
     * state.
     */
    NVIC_SetPriority(MemoryManagement_IRQn, 0xF);
    NVIC_SetPriority(BusFault_IRQn, 0x8);
    NVIC_SetPriority(UsageFault_IRQn, 0x1);
    NVIC_SetPriority(SecureFault_IRQn, 0);

    // NVIC_SetPriority(SVCall_IRQn, 0xFF);
    NVIC_SetPriorityGrouping(4);
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
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
    //NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void tfm_arch_config_extensions(void)
{
#if (CONFIG_TFM_FP >= 1)
#ifdef __GNUC__
    /* Enable SPE privileged and unprivileged access to the FP Extension */
    SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                  | (3U << 11U*2U);  /* enable CP11 full access */
#endif

#ifdef CONFIG_TFM_LAZY_STACKING
    /* Enable lazy stacking. */
    FPU->FPCCR |= FPU_FPCCR_LSPEN_Msk;
#else
    /* Disable lazy stacking. */
    FPU->FPCCR &= ~FPU_FPCCR_LSPEN_Msk;
#endif

    /* If the SPE will ever use the floating-point registers for sensitive
     * data, then FPCCR.ASPEN, FPCCR.TS, FPCCR.CLRONRET and FPCCR.CLRONRETS
     * must be set at initialisation and not changed again afterwards.
     * Let SPE decide the S/NS shared setting (LSPEN and CLRONRET) to avoid the
     * possible side-path brought by flexibility.
     */
    FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk
                  | FPU_FPCCR_TS_Msk
                  | FPU_FPCCR_CLRONRET_Msk
                  | FPU_FPCCR_CLRONRETS_Msk
                  | FPU_FPCCR_LSPENS_Msk;

    /* Permit Non-secure access to the Floating-point Extension.
     * Note: It is still necessary to set CPACR_NS to enable the FP Extension
     * in the NSPE. This configuration is left to NS privileged software.
     */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;

    /* Prevent non-secure from modifying FPU’s power setting. */
    SCnSCB->CPPWR |= SCnSCB_CPPWR_SUS11_Msk | SCnSCB_CPPWR_SUS10_Msk;
#endif /* CONFIG_TFM_FP >= 1 */

#if defined(__ARM_ARCH_8_1M_MAIN__)
    SCB->CCR |= SCB_CCR_TRD_Msk;
#endif
}

__attribute__((naked, noinline)) void tfm_arch_clear_fp_status(void)
{
    __ASM volatile(
                   "mrs  r0, control         \n"
                   "bics r0, r0, #4          \n"
                   "msr  control, r0         \n"
                   "isb                      \n"
                   "bx   lr                  \n"
                  );
}

#if (CONFIG_TFM_FP >= 1)
__attribute__((naked, noinline)) void tfm_arch_clear_fp_data(void)
{
    __ASM volatile(
                    "eor  r0, r0, r0         \n"
                    "vmov s0, r0             \n"
                    "vmov s1, r0             \n"
                    "vmov s2, r0             \n"
                    "vmov s3, r0             \n"
                    "vmov s4, r0             \n"
                    "vmov s5, r0             \n"
                    "vmov s6, r0             \n"
                    "vmov s7, r0             \n"
                    "vmov s8, r0             \n"
                    "vmov s9, r0             \n"
                    "vmov s10, r0            \n"
                    "vmov s11, r0            \n"
                    "vmov s12, r0            \n"
                    "vmov s13, r0            \n"
                    "vmov s14, r0            \n"
                    "vmov s15, r0            \n"
                    "vmov s16, r0            \n"
                    "vmov s17, r0            \n"
                    "vmov s18, r0            \n"
                    "vmov s19, r0            \n"
                    "vmov s20, r0            \n"
                    "vmov s21, r0            \n"
                    "vmov s22, r0            \n"
                    "vmov s23, r0            \n"
                    "vmov s24, r0            \n"
                    "vmov s25, r0            \n"
                    "vmov s26, r0            \n"
                    "vmov s27, r0            \n"
                    "vmov s28, r0            \n"
                    "vmov s29, r0            \n"
                    "vmov s30, r0            \n"
                    "vmov s31, r0            \n"
                    "vmsr fpscr, r0          \n"
                    "bx   lr                 \n"
                  );
}
#endif
