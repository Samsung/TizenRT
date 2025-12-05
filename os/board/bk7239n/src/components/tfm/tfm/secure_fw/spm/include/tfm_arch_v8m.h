/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_ARCH_V8M_H__
#define __TFM_ARCH_V8M_H__

#include <stdint.h>
#include <stdbool.h>

#include "cmsis_compiler.h"
#include "tfm_core_trustzone.h"
#include "utilities.h"

#define EXC_RETURN_INDICATOR                    (0xFFUL << 24)
#define EXC_RETURN_RES1                         (0x1FFFFUL << 7)
#define EXC_RETURN_SECURE_STACK                 (1UL << 6)
#define EXC_RETURN_STACK_RULE                   (1UL << 5)
#define EXC_RETURN_FPU_FRAME_BASIC              (1UL << 4)
#define EXC_RETURN_MODE_THREAD                  (1UL << 3)
#define EXC_RETURN_STACK_PROCESS                (1UL << 2)
#define EXC_RETURN_STACK_MAIN                   (0UL << 2)
#define EXC_RETURN_RES0                         (0UL << 1)
#define EXC_RETURN_EXC_SECURE                   (1UL)

/* Initial EXC_RETURN value in LR when a thread is loaded at the first time */
#define EXC_RETURN_THREAD_S_PSP                                 \
        EXC_RETURN_INDICATOR | EXC_RETURN_RES1 |                \
        EXC_RETURN_SECURE_STACK | EXC_RETURN_STACK_RULE |       \
        EXC_RETURN_FPU_FRAME_BASIC | EXC_RETURN_MODE_THREAD |   \
        EXC_RETURN_STACK_PROCESS | EXC_RETURN_RES0 |            \
        EXC_RETURN_EXC_SECURE

#define EXC_RETURN_THREAD_S_MSP                                 \
        EXC_RETURN_INDICATOR | EXC_RETURN_RES1 |                \
        EXC_RETURN_SECURE_STACK | EXC_RETURN_STACK_RULE |       \
        EXC_RETURN_FPU_FRAME_BASIC | EXC_RETURN_MODE_THREAD |   \
        EXC_RETURN_STACK_MAIN | EXC_RETURN_RES0 |               \
        EXC_RETURN_EXC_SECURE

#define EXC_RETURN_HANDLER_S_MSP                                \
        EXC_RETURN_INDICATOR | EXC_RETURN_RES1 |                \
        EXC_RETURN_SECURE_STACK | EXC_RETURN_STACK_RULE |       \
        EXC_RETURN_FPU_FRAME_BASIC |                            \
        EXC_RETURN_STACK_MAIN | EXC_RETURN_RES0 |               \
        EXC_RETURN_EXC_SECURE

/* Exception numbers */
#define EXC_NUM_THREAD_MODE                     (0)
#define EXC_NUM_SVCALL                          (11)
#define EXC_NUM_PENDSV                          (14)

#define SCB_ICSR_ADDR                    (0xE000ED04)
#define SCB_ICSR_PENDSVSET_BIT           (0x10000000)

/* Disable NS exceptions by setting NS PRIMASK to 1 */
#define TFM_NS_EXC_DISABLE()    __TZ_set_PRIMASK_NS(1)
/* Enable NS exceptions by setting NS PRIMASK to 0 */
#define TFM_NS_EXC_ENABLE()     __TZ_set_PRIMASK_NS(0)

/**
 * \brief Check whether Secure or Non-secure stack is used to restore stack
 *        frame on exception return.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             Secure stack is used to restore stack frame on
 *                          exception return.
 * \retval false            Non-secure stack is used to restore stack frame on
 *                          exception return.
 */
__STATIC_INLINE bool is_return_secure_stack(uint32_t lr)
{
    return (lr & EXC_RETURN_SECURE_STACK);
}

/**
 * \brief Check whether the stack frame for this exception has space allocated
 *        for Floating Point(FP) state information.
 *
 * \param[in] lr            LR register containing the EXC_RETURN value.
 *
 * \retval true             The stack allocates space for FP information
 * \retval false            The stack doesn't allocate space for FP information
 */
__STATIC_INLINE bool is_stack_alloc_fp_space(uint32_t lr)
{
    return (lr & EXC_RETURN_FPU_FRAME_BASIC) ? false : true;
}

/**
 * \brief Set PSPLIM register.
 *
 * \param[in] psplim        Register value to be written into PSPLIM.
 */
__STATIC_INLINE void tfm_arch_set_psplim(uint32_t psplim)
{
    __set_PSPLIM(psplim);
}

/**
 * \brief Set MSP limit value.
 *
 * \param[in] msplim        MSP limit value to be written.
 */
__STATIC_INLINE void tfm_arch_set_msplim(uint32_t msplim)
{
    __set_MSPLIM(msplim);
}

/**
 * \brief Seal the thread stack.
 *
 * This function must be called only when the caller is using MSP.
 *
 * \param[in] stk        Thread stack address.
 *
 * \retval stack         Updated thread stack address.
 */
__STATIC_INLINE uintptr_t arch_seal_thread_stack(uintptr_t stk)
{
    TFM_CORE_ASSERT((stk & 0x7) == 0);
    stk -= TFM_STACK_SEALED_SIZE;

    *((uint32_t *)stk)       = TFM_STACK_SEAL_VALUE;
    *((uint32_t *)(stk + 4)) = TFM_STACK_SEAL_VALUE;

    return stk;
}

/**
 * \brief Set MSPLIM register and seal the MSP.
 *
 * This function assumes that the caller is using PSP when calling this
 * function.
 *
 * \param[in] msplim        Register value to be written into MSPLIM.
 */
__STATIC_INLINE void tfm_arch_init_secure_msp(uint32_t msplim)
{
    uint32_t mstk_adr = __get_MSP();

    /*
     * Seal the main stack and update MSP to point below the stack seal.
     * Set MSPLIM. As the initial 'main()' code is running under privileged PSP
     * manipulating MSP works here.
     */
    TFM_CORE_ASSERT((mstk_adr & 0x7) == 0);
    mstk_adr -= TFM_STACK_SEALED_SIZE;

    *((uint32_t *)mstk_adr)       = TFM_STACK_SEAL_VALUE;
    *((uint32_t *)(mstk_adr + 4)) = TFM_STACK_SEAL_VALUE;

    __set_MSP(mstk_adr);
    __set_MSPLIM(msplim);
}

#endif
