/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fpu_ns_tests.h"
#include "../fpu_tests_common.h"
#include "os_wrapper/delay.h"

static void tfm_fpu_test_fp_protection_secure_interrupt(
                                                struct test_result_t *ret);
static void tfm_fpu_test_fp_protection_non_secure_interrupt(
                                                struct test_result_t *ret);
static bool check_fp_caller_restored(void);
static bool check_fp_callee_restored(void);

static struct test_t fpu_ns_tests[] = {
    {
        &tfm_fpu_test_clear_client_fp_data, "TFM_NS_FPU_TEST_1001",
        "Clear FP registers in FPU client partition"
    },
    {
        &tfm_fpu_test_fp_protection_psa_call, "TFM_NS_FPU_TEST_1002",
        "Test FP context protection after psa calls"
    },
    {
        &tfm_fpu_test_fp_protection_secure_interrupt, "TFM_NS_FPU_TEST_1003",
        "Test FP context protection in S interrupt after interrupt return"
    },
    {
        &tfm_fpu_test_fp_protection_non_secure_interrupt,
        "TFM_NS_FPU_TEST_1004",
        "Test FP context protection in S thread after NS interrupt"
    }
};

void register_testsuite_ns_fpu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(fpu_ns_tests) / sizeof(fpu_ns_tests[0]));

    set_testsuite("FPU non-secure interface test (TFM_NS_FPU_TEST_1XXX)",
                  fpu_ns_tests, list_size, p_test_suite);
}

/**
 * Change FP registers.
 */
__attribute__((naked)) void change_fp_in_ns_thread(void)
{
    __asm volatile(
        "mov       r0, #0xC0000000         \n"
        "vmov      s0, r0                  \n"
        "mov       r0, #0xC1000000         \n"
        "vmov      s1, r0                  \n"
        "mov       r0, #0xC2000000         \n"
        "vmov      s2, r0                  \n"
        "mov       r0, #0xC3000000         \n"
        "vmov      s3, r0                  \n"
        "mov       r0, #0xC4000000         \n"
        "vmov      s4, r0                  \n"
        "mov       r0, #0xC5000000         \n"
        "vmov      s5, r0                  \n"
        "mov       r0, #0xC6000000         \n"
        "vmov      s6, r0                  \n"
        "mov       r0, #0xC7000000         \n"
        "vmov      s7, r0                  \n"
        "mov       r0, #0xC8000000         \n"
        "vmov      s8, r0                  \n"
        "mov       r0, #0xC9000000         \n"
        "vmov      s9, r0                  \n"
        "mov       r0, #0xCA000000         \n"
        "vmov      s10, r0                 \n"
        "mov       r0, #0xCB000000         \n"
        "vmov      s11, r0                 \n"
        "mov       r0, #0xCC000000         \n"
        "vmov      s12, r0                 \n"
        "mov       r0, #0xCD000000         \n"
        "vmov      s13, r0                 \n"
        "mov       r0, #0xCE000000         \n"
        "vmov      s14, r0                 \n"
        "mov       r0, #0xCF000000         \n"
        "vmov      s15, r0                 \n"
        "mov       r0, #0xD0000000         \n"
        "vmov      s16, r0                 \n"
        "mov       r0, #0xD1000000         \n"
        "vmov      s17, r0                 \n"
        "mov       r0, #0xD2000000         \n"
        "vmov      s18, r0                 \n"
        "mov       r0, #0xD3000000         \n"
        "vmov      s19, r0                 \n"
        "mov       r0, #0xD4000000         \n"
        "vmov      s20, r0                 \n"
        "mov       r0, #0xD5000000         \n"
        "vmov      s21, r0                 \n"
        "mov       r0, #0xD6000000         \n"
        "vmov      s22, r0                 \n"
        "mov       r0, #0xD7000000         \n"
        "vmov      s23, r0                 \n"
        "mov       r0, #0xD8000000         \n"
        "vmov      s24, r0                 \n"
        "mov       r0, #0xD9000000         \n"
        "vmov      s25, r0                 \n"
        "mov       r0, #0xDA000000         \n"
        "vmov      s26, r0                 \n"
        "mov       r0, #0xDB000000         \n"
        "vmov      s27, r0                 \n"
        "mov       r0, #0xDC000000         \n"
        "vmov      s28, r0                 \n"
        "mov       r0, #0xDD000000         \n"
        "vmov      s29, r0                 \n"
        "mov       r0, #0xDE000000         \n"
        "vmov      s30, r0                 \n"
        "mov       r0, #0xDF000000         \n"
        "vmov      s31, r0                 \n"

        "bx        lr                      \n"
    );
}

/**
 * Check whether FP caller registers are restored correctly.
 * Return:
 *   True - FP caller registers are restored correctly
 *   False - FP caller registers are not restored correctly
 */
static bool check_fp_caller_restored(void)
{
    static uint32_t fp_buffer[NR_FP_CALLER_REG] = {0};
    const uint32_t fp_expect[NR_FP_CALLER_REG] = {
        0xC0000000, 0xC1000000, 0xC2000000, 0xC3000000,
        0xC4000000, 0xC5000000, 0xC6000000, 0xC7000000,
        0xC8000000, 0xC9000000, 0xCA000000, 0xCB000000,
        0xCC000000, 0xCD000000, 0xCE000000, 0xCF000000
    };

    __asm volatile(
        "vstm      %0, {S0-S15}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    if (!memcmp(fp_buffer, fp_expect, FP_CALLER_BUF_SIZE)) {
        return true;
    }

    return false;
}

/**
 * Check whether FP callee registers are restored correctly.
 * Return:
 *   True - FP callee registers are restored correctly
 *   False - FP callee registers are not restored correctly
 */
static bool check_fp_callee_restored(void)
{
    static uint32_t fp_buffer[NR_FP_CALLEE_REG] = {0};
    const uint32_t fp_expect[NR_FP_CALLEE_REG] = {
        0xD0000000, 0xD1000000, 0xD2000000, 0xD3000000,
        0xD4000000, 0xD5000000, 0xD6000000, 0xD7000000,
        0xD8000000, 0xD9000000, 0xDA000000, 0xDB000000,
        0xDC000000, 0xDD000000, 0xDE000000, 0xDF000000
    };

    __asm volatile(
        "vstm      %0, {S16-S31}            \n"
        :
        :"r"(fp_buffer)
        :"memory"
    );

    if (!memcmp(fp_buffer, fp_expect, FP_CALLEE_BUF_SIZE)) {
        return true;
    }

    return false;
}

/**
 * \brief Test FP context protection in S interrupt. Change FP registers in
 * non-secure thread first, then change them in S interrupt. After interrupt
 * return, check FP context protection in non-secure thread.
 * Expectation: FP registers in S interrupt should not be view in non-secure
 * thread.
 */
static void tfm_fpu_test_fp_protection_secure_interrupt(
                                                struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};
    static uint8_t i;

    ret->val = TEST_FAILED;

    /* Change FP regs */
    change_fp_in_ns_thread();

    /* Start the timer */
    handle = psa_connect(TFM_FPU_SERVICE_START_S_TIMER_SID,
                            TFM_FPU_SERVICE_START_S_TIMER_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }
    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, NULL, 0);
    if (status != PSA_SUCCESS) {
        return;
    }
    psa_close(handle);

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_SID,
                            TFM_FPU_SERVICE_CHECK_S_TIMER_TRIGGERED_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }
    /* Spin here */
    while (1) {
        /* Wait S interrupt triggered */
        os_wrapper_delay(WAIT_S_INT);

        status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
        if (status == PSA_SUCCESS) {
            /* S interrupt triggered */
            if (outvec_data[0] == S_TIMER_TRIGGERED) {
                break;
            } else {
                i++;
                if (i > LOOPS_S_INT_TEST) {
                    TEST_FAIL("Time out: NS thread not interrupted!\r\n");
                    psa_close(handle);
                    return;
                }
            }
        } else {
            TEST_FAIL("Check S interrupt triggered failed!\r\n");
            return;
        }
    }

    psa_close(handle);

    /* FP caller registers should be restored correctly after S interrupt */
    if (!check_fp_caller_restored()) {
        return;
    }

    /* FP callee registers should be restored correctly */
    if (check_fp_callee_restored()) {
        ret->val = TEST_PASSED;
    }
}

/**
 * \brief In secure thread, trigger non-secure timer interrupt, check FP
 *  context protection after NS interrupt.
 * Expectation: FP register in secure thread should be restored after NS
 *  interrupt.
 */
static void tfm_fpu_test_fp_protection_non_secure_interrupt(
                                                    struct test_result_t *ret)
{
    psa_handle_t handle;
    psa_status_t status;
    uint8_t *outvec_data[1] = {0};
    struct psa_outvec outvecs[1] = {{outvec_data, sizeof(outvec_data[0])}};

    ret->val = TEST_FAILED;

    handle = psa_connect(TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_SID,
                            TFM_FPU_SERVICE_CHECK_NS_INTERRUPT_S_TEST_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return;
    }

    status = psa_call(handle, PSA_IPC_CALL, NULL, 0, outvecs, 1);
    if (status == PSA_SUCCESS) {
        ret->val = TEST_PASSED;
    }

    psa_close(handle);
}
