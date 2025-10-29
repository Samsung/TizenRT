/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef __FPU_TESTS_COMMON_H__
#define __FPU_TESTS_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "psa/client.h"
#include "test_framework_helpers.h"
#ifdef TFM_PSA_API
#include "psa_manifest/sid.h"
#endif


#define LOOP_ITERATIONS             (300U)
#define LOOPS_S_INT_TEST            (50U)
#define LOOPS_NS_INT_TEST           (5000000U)
#define WAIT_S_INT                  (2000U)

#define NR_FP_REG                   (32U)
#define NR_FP_CALLER_REG            (NR_FP_REG / 2)
#define NR_FP_CALLEE_REG            (NR_FP_REG / 2)
#define FP_BUF_SIZE                 (NR_FP_REG * sizeof(uint32_t))
#define FP_CALLER_BUF_SIZE          (NR_FP_CALLER_REG * sizeof(uint32_t))
#define FP_CALLEE_BUF_SIZE          (NR_FP_CALLEE_REG * sizeof(uint32_t))

#define REL_VALUE_FP_REGS_INVALIDATED        (0xDEADBEEF)

#define S_TIMER_TRIGGERED                    (0x19)
#define S_TIMER_NOT_TRIGGERED                (0x91)

/**
 * Test FP context protection after psa calls.
 */
void tfm_fpu_test_fp_protection_psa_call(struct test_result_t *ret);

/**
 * Clear FP registers in FPU service partition.
 */
void tfm_fpu_test_clear_service_fp_data(struct test_result_t *ret);

/**
 * Test reliability of FP context protection after psa calls by loops.
 */
void tfm_fpu_test_fp_protection_psa_call_loop(struct test_result_t *ret);

/**
 * Clear FP registers in FPU client partition.
 */
void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret);

#ifdef __cplusplus
}
#endif

#endif /* __FPU_TESTS_COMMON__ */
