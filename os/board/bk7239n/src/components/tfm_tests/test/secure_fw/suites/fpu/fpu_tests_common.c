/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "fpu_tests_common.h"
#include "psa_manifest/sid.h"
#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif

/**
 * Change FP registers.
 */
__attribute__((naked)) static void change_fp_in_client(void)
{
    __asm volatile(
        "mov       r0, #0x000000C0         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0x000000C1         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0x000000C2         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0x000000C3         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0x000000C4         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0x000000C5         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0x000000C6         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0x000000C7         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0x000000C8         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0x000000C9         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0x000000CA         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0x000000CB         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0x000000CC         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0x000000CD         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0x000000CE         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0x000000CF         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0x000000D0         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0x000000D1         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0x000000D2         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0x000000D3         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0x000000D4         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0x000000D5         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0x000000D6         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0x000000D7         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0x000000D8         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0x000000D9         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0x000000DA         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0x000000DB         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0x000000DC         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0x000000DD         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0x000000DE         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0x000000DF         \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check whether FP registers are restored correctly.
 * Return:
 *   True - FP registers are restored correctly
 *   False - FP registers are not restored correctly
 */
static bool check_fp_restored_client(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    const uint32_t fp_expect[NR_FP_REG] =
                            {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
                             0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
                             0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
                             0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF};

    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

#if DOMAIN_NS == 1
    if (!memcmp(fp_buffer, fp_expect, FP_BUF_SIZE))
#else
    if (!tfm_memcmp(fp_buffer, fp_expect, FP_BUF_SIZE))
#endif
    {
        return true;
    }

    return false;
}

/**
 * Clear FP registers.
 */
__attribute__((naked)) static void fpu_client_fp_clear_test(void)
{
    __asm volatile(
        "eor       r0, r0, r0              \n"
        "vmov      s0, r0                  \n"
        "vmov      s1, r0                  \n"
        "vmov      s2, r0                  \n"
        "vmov      s3, r0                  \n"
        "vmov      s4, r0                  \n"
        "vmov      s5, r0                  \n"
        "vmov      s6, r0                  \n"
        "vmov      s7, r0                  \n"
        "vmov      s8, r0                  \n"
        "vmov      s9, r0                  \n"
        "vmov      s10, r0                 \n"
        "vmov      s11, r0                 \n"
        "vmov      s12, r0                 \n"
        "vmov      s13, r0                 \n"
        "vmov      s14, r0                 \n"
        "vmov      s15, r0                 \n"
        "vmov      s16, r0                 \n"
        "vmov      s17, r0                 \n"
        "vmov      s18, r0                 \n"
        "vmov      s19, r0                 \n"
        "vmov      s20, r0                 \n"
        "vmov      s21, r0                 \n"
        "vmov      s22, r0                 \n"
        "vmov      s23, r0                 \n"
        "vmov      s24, r0                 \n"
        "vmov      s25, r0                 \n"
        "vmov      s26, r0                 \n"
        "vmov      s27, r0                 \n"
        "vmov      s28, r0                 \n"
        "vmov      s29, r0                 \n"
        "vmov      s30, r0                 \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check invalidation of FP registers.
 * Return:
 *   True - FP registers are invalidated
 *   False - FP registers are not invalidated
 */
static bool check_fp_invalidated(void)
{
    uint32_t fp_buffer[NR_FP_REG] = {0};
    const uint32_t fp_expect[NR_FP_REG] = {0};

    __asm volatile(
        "vstm      %0, {S0-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

#if DOMAIN_NS == 1
    if (!memcmp(fp_buffer, fp_expect, FP_BUF_SIZE))
#else
    if (!tfm_memcmp(fp_buffer, fp_expect, FP_BUF_SIZE))
#endif
    {
        return true;
    }

    return false;
}

/**
 * Description: Clear FP registers to check basic FP register write/read
 * functionality.
 * Expectation: FP registers in FPU client partition should be cleared.
 */
void tfm_fpu_test_clear_client_fp_data(struct test_result_t *ret)
{
    ret->val = TEST_FAILED;

    fpu_client_fp_clear_test();

    if (check_fp_invalidated()) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }
}

/**
 * Description: Test FP context protection after psa calls. Change FP registers
 * in FPU client/service partition separately, then check FP registers after
 * psa calls.
 * Expectation: FP registers in FPU client/service partition should be saved
 * and restored correctly.
 */
void tfm_fpu_test_fp_protection_psa_call(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;

    ret->val = TEST_FAILED;

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SID,
                         TFM_FPU_SERVICE_CHECK_FP_REGISTER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        ret->val = TEST_FAILED;
        return;
    }

    /* Change FP registers */
    change_fp_in_client();

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_SUCCESS) {
        /* FP registers should be restored */
        if (check_fp_restored_client()) {
            ret->val = TEST_PASSED;
        } else {
            ret->val = TEST_FAILED;
        }
    }

    psa_close(handle);
}

/**
 * Description: Clear FP registers in FPU service partition for next test.
 * Expectation: FP registers in FPU service partition should be cleared.
 */
void tfm_fpu_test_clear_service_fp_data(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;

    ret->val = TEST_FAILED;

    handle = psa_connect(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SID,
                         TFM_FPU_SERVICE_CLEAR_FP_REGISTER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        ret->val = TEST_FAILED;
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    } else {
        ret->val = TEST_FAILED;
    }

    psa_close(handle);
}

/**
 * Description: Test reliability of FP context protection after psa calls by
 * loops. Change FP registers in FPU client/service partition separately, then
 * check FP registers after psa calls.
 * Expectation: FP registers in FPU client/service partition should be saved
 * and restored correctly.
 */
void tfm_fpu_test_fp_protection_psa_call_loop(struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint32_t itr;

    ret->val = TEST_FAILED;

    for (itr = 0; itr < LOOP_ITERATIONS; itr++) {
        TEST_LOG("  > Iteration %d of %d\r", itr + 1, LOOP_ITERATIONS);

        handle = psa_connect(TFM_FPU_SERVICE_CHECK_FP_REGISTER_SID,
                             TFM_FPU_SERVICE_CHECK_FP_REGISTER_VERSION);
        if (!PSA_HANDLE_IS_VALID(handle)) {
            ret->val = TEST_FAILED;
            return;
        }

        /* Change FP registers */
        change_fp_in_client();

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
        if (status == PSA_SUCCESS) {
            /* FP registers should be restored */
            if (check_fp_restored_client()) {
                ret->val = TEST_PASSED;
            } else {
                ret->val = TEST_FAILED;
            }
        } else {
            ret->val = TEST_FAILED;
        }

        psa_close(handle);

        handle = psa_connect(TFM_FPU_SERVICE_CLEAR_FP_REGISTER_SID,
                             TFM_FPU_SERVICE_CLEAR_FP_REGISTER_VERSION);
        if (!PSA_HANDLE_IS_VALID(handle)) {
            ret->val = TEST_FAILED;
            return;
        }

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
        if (status == PSA_SUCCESS) {
            ret->val = TEST_PASSED;
        } else {
            ret->val = TEST_FAILED;
        }

        psa_close(handle);
    }
}
