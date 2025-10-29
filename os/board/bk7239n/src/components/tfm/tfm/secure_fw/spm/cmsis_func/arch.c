/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "arch.h"
#include "compiler_ext_defs.h"
#include "tfm_secure_api.h"

#if defined(__ICCARM__)
#include "cmsis_psa/tfm_svcalls.h"
#pragma required = tfm_core_svc_handler
#endif

nsfptr_t ns_entry;

void jump_to_ns_code(void)
{
    /* Calls the non-secure Reset_Handler to jump to the non-secure binary */
    ns_entry();

    tfm_core_panic();
}

__attribute__((naked))
void tfm_enable_irq(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_ENABLE_IRQ));
}

__attribute__((naked))
void tfm_disable_irq(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_DISABLE_IRQ));
}

__attribute__((naked))
void tfm_sfn_completion(enum tfm_status_e res, uint32_t exc_return, uintptr_t msp)
{
    __ASM volatile("MSR msp, r2\n"
                   "SVC %0\n"
                   "BX LR\n"
                   : : "I" (TFM_SVC_SFN_COMPLETION) : );
}

__attribute__((naked))
static psa_signal_t psa_wait_internal(psa_signal_t signal_mask,
                                      uint32_t timeout)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_PSA_WAIT));
}

psa_signal_t psa_wait(psa_signal_t signal_mask, uint32_t timeout)
{
    /* FIXME: By using the 'WFI' instruction this function blocks until an
     * interrupt happens. It is necessary to do this here as tfm_core_psa_wait
     * runs with the priority of the SVC, so it cannot be interrupted, so
     * waiting in it for the required interrupt to happen is not an option.
     */
    psa_signal_t actual_signal_mask;

    while (1) {
        actual_signal_mask = psa_wait_internal(signal_mask, timeout);
        if ((actual_signal_mask & signal_mask) != 0) {
            return actual_signal_mask;
        }
        __WFI();
    }
}

__attribute__((naked))
void tfm_arch_trigger_exc_return(uint32_t exc_return)
{
    __ASM volatile("BX R0");
}

__attribute__((naked))
void psa_eoi(psa_signal_t irq_signal)
{
    __ASM("SVC %0\n"
          "BX LR\n"
          : : "I" (TFM_SVC_PSA_EOI));
}

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
__section("SFN") __naked
int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
        "PUSH   {r4-r12, lr}                \n"
        "SVC    %[SVC_REQ]                  \n"
        "MOV    r4,  #0                     \n"
        "MOV    r5,  r4                     \n"
        "MOV    r6,  r4                     \n"
        "MOV    r7,  r4                     \n"
        "MOV    r8,  r4                     \n"
        "MOV    r9,  r4                     \n"
        "MOV    r10, r4                     \n"
        "MOV    r11, r4                     \n"
        "BLX    lr                          \n"
        "SVC    %[SVC_RET]                  \n"
        "POP    {r4-r12, pc}                \n"
        : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST),
            [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
        );
}

__section("SFN") __naked
void priv_irq_handler_main(uint32_t partition_id, uint32_t unpriv_handler,
                           uint32_t irq_signal, uint32_t irq_line)
{
    __ASM(
          /* Save the callee saved registers*/
          "PUSH   {r4-r12, lr}              \n"
          /* Request SVC to configure environment for the unpriv IRQ handler */
          "SVC    %[SVC_REQ]                \n"
          /* clear the callee saved registers to prevent information leak */
          "MOV    r4,  #0                   \n"
          "MOV    r5,  r4                   \n"
          "MOV    r6,  r4                   \n"
          "MOV    r7,  r4                   \n"
          "MOV    r8,  r4                   \n"
          "MOV    r9,  r4                   \n"
          "MOV    r10, r4                   \n"
          "MOV    r11, r4                   \n"
          /* Branch to the unprivileged handler */
          "BLX    lr                        \n"
          /* Request SVC to reconfigure the environment of the interrupted
           * partition
           */
          "SVC    %[SVC_RET]                \n"
            /* restore callee saved registers and return */
          "POP    {r4-r12, pc}              \n"
          : : [SVC_REQ] "I" (TFM_SVC_DEPRIV_REQ)
            , [SVC_RET] "I" (TFM_SVC_DEPRIV_RET)
          );
}
#elif defined(__ARM_ARCH_8M_BASE__)
__section("SFN") __naked
int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr)
{
    __ASM volatile(
        "PUSH   {lr}                        \n"
        "PUSH   {r4-r7}                     \n"
        "MOV    r4, r8                      \n"
        "MOV    r5, r9                      \n"
        "MOV    r6, r10                     \n"
        "MOV    r7, r11                     \n"
        "PUSH   {r4-r7}                     \n"
        "MOV    r4, r12                     \n"
        "PUSH   {r4}                        \n"
        "SVC    %[SVC_REQ]                  \n"
        "MOVS   r4, #0                      \n"
        "MOV    r5, r4                      \n"
        "MOV    r6, r4                      \n"
        "MOV    r7, r4                      \n"
        "MOV    r8, r4                      \n"
        "MOV    r9, r4                      \n"
        "MOV    r10, r4                     \n"
        "MOV    r11, r4                     \n"
        "BLX    lr                          \n"
        "SVC    %[SVC_RET]                  \n"
        "POP    {r4}                        \n"
        "MOV    r12, r4                     \n"
        "POP    {r4-r7}                     \n"
        "MOV    r8, r4                      \n"
        "MOV    r9, r5                      \n"
        "MOV    r10, r6                     \n"
        "MOV    r11, r7                     \n"
        "POP    {r4-r7}                     \n"
        "POP    {pc}                        \n"
        : : [SVC_REQ] "I" (TFM_SVC_SFN_REQUEST),
            [SVC_RET] "I" (TFM_SVC_SFN_RETURN)
        );
}

__section("SFN") __naked
void priv_irq_handler_main(uint32_t partition_id, uint32_t unpriv_handler,
                           uint32_t irq_signal, uint32_t irq_line)
{
    __ASM(
          /* Save the callee saved registers*/
          "PUSH   {r4-r7, lr}               \n"
          "MOV    r4, r8                    \n"
          "MOV    r5, r9                    \n"
          "MOV    r6, r10                   \n"
          "MOV    r7, r11                   \n"
          "PUSH   {r4-r7}                   \n"
          "MOV    r4, r12                   \n"
          "PUSH   {r4}                      \n"
          /* Request SVC to configure environment for the unpriv IRQ handler */
          "SVC    %[SVC_REQ]                \n"
          /* clear the callee saved registers to prevent information leak */
          "MOVS   r4, #0                    \n"
          "MOV    r5, r4                    \n"
          "MOV    r6, r4                    \n"
          "MOV    r7, r4                    \n"
          "MOV    r8, r4                    \n"
          "MOV    r9, r4                    \n"
          "MOV    r10, r4                   \n"
          "MOV    r11, r4                   \n"
          /* Branch to the unprivileged handler */
          "BLX    lr                        \n"
          /* Request SVC to reconfigure the environment of the interrupted
           * partition
           */
          "SVC    %[SVC_RET]                \n"
          /* restore callee saved registers and return */
          "POP    {r4}                      \n"
          "MOV    r12, r4                   \n"
          "POP    {r4-r7}                   \n"
          "MOV    r8, r4                    \n"
          "MOV    r9, r5                    \n"
          "MOV    r10, r6                   \n"
          "MOV    r11, r7                   \n"
          "POP   {r4-r7, pc}                \n"
          : : [SVC_REQ] "I" (TFM_SVC_DEPRIV_REQ)
          , [SVC_RET] "I" (TFM_SVC_DEPRIV_RET)
          );
}
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__) || \
    defined(__ARM_ARCH_8M_MAIN__)  || defined(__ARM_ARCH_8M_BASE__)
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

#ifndef __ARM_ARCH_8M_BASE__
    NVIC_SetPriority(MemoryManagement_IRQn, 0);
    NVIC_SetPriority(BusFault_IRQn, 0);
    NVIC_SetPriority(SecureFault_IRQn, 0);
#endif

    /*
     * Function based model needs no PendSV for scheduling,
     * set its priority just higher than thread mode.
     */
    NVIC_SetPriority(SVCall_IRQn, 0);
    NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
}
#else
#error Function based model works on V8M series only.
#endif

void tfm_arch_config_extensions(void)
{
#if defined(__FPU_PRESENT) && (__FPU_PRESENT == 1U)
    /* Configure Secure access to the FPU only if the secure image is being
     * built with the FPU in use. This avoids introducing extra interrupt
     * latency when the FPU is not used by the SPE.
     */
#if defined(__FPU_USED) && (__FPU_USED == 1U)
    /* Enable Secure privileged and unprivilged access to the FP Extension */
    SCB->CPACR |= (3U << 10U*2U)     /* enable CP10 full access */
                  | (3U << 11U*2U);  /* enable CP11 full access */

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
    /* If the SPE will ever use the floating-point registers for sensitive data,
     * then FPCCR.TS, FPCCR.CLRONRET and FPCCR.CLRONRETS must be set at
     * initialisation and not changed again afterwards.
     */
    FPU->FPCCR |= FPU_FPCCR_TS_Msk
                  | FPU_FPCCR_CLRONRET_Msk
                  | FPU_FPCCR_CLRONRETS_Msk;
#endif
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__) || defined(__ARM_ARCH_8M_MAIN__)
    /* Permit Non-secure access to the Floating-point Extension.
     * Note: It is still necessary to set CPACR_NS to enable the FP Extension in
     * the NSPE. This configuration is left to NS privileged software.
     */
    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__)
    SCB->CCR |= SCB_CCR_TRD_Msk;
#endif
#endif
}

#if defined(__ARM_ARCH_8M_BASE__)   || \
    defined(__ARM_ARCH_8_1M_MAIN__) || \
    defined(__ARM_ARCH_8M_MAIN__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                        \n"
#endif
    "MRS     r0, MSP                        \n"
    "MOV     r2, lr                         \n"
    "MOVS    r3, #8                         \n"
    "TST     r2, r3                         \n"
    "BNE     from_thread                    \n"
    /*
     * This branch is taken when the code is being invoked from handler mode.
     * This happens when a de-privileged interrupt handler is to be run. Seal
     * the stack before de-privileging.
     */
    "LDR     r1, =0xFEF5EDA5                \n"
    "MOVS    r3, r1                         \n"
    "PUSH    {r1, r3}                       \n"
    "from_thread:                           \n"
    "MRS     r1, PSP                        \n"
    "BL      tfm_core_svc_handler           \n"
    "MOVS    r1, #8                         \n"
    "TST     r1, r0                         \n"
    "BNE     to_thread                      \n"
    /*
     * This branch is taken when the code is going to return to handler mode.
     * This happens after a de-privileged interrupt handler had been run. Pop
     * the sealing from the stack.
     */
    "POP     {r1, r2}                       \n"
    "to_thread:                             \n"
    "BX      r0                             \n"
    );
}
#elif defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_7M__) || \
      defined(__ARM_ARCH_7EM__)
__attribute__((naked)) void SVC_Handler(void)
{
    __ASM volatile(
    "MRS     r0, MSP                        \n"
    "MRS     r1, PSP                        \n"
    "MOV     r2, lr                         \n"
    "BL      tfm_core_svc_handler           \n"
    "BX      r0                             \n"
    );
}
#endif
