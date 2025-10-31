/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fpu_s_tests.h"
#include "../fpu_tests_common.h"

static struct test_t fpu_s_tests[] = {
    {
        &tfm_fpu_test_clear_client_fp_data, "TFM_S_FPU_TEST_1001",
        "Clear FP registers in FPU client partition"
    },
    {
        &tfm_fpu_test_fp_protection_psa_call, "TFM_S_FPU_TEST_1002",
        "Test FP context protection after psa calls"
    },
    {
        &tfm_fpu_test_clear_service_fp_data, "TFM_S_FPU_TEST_1003",
        "Clear FP registers in FPU service partition for next test"
    },
    {
        &tfm_fpu_test_fp_protection_psa_call_loop, "TFM_S_FPU_TEST_1004",
        "Test reliability of FP context protection after psa calls"
    }
};

void register_testsuite_s_fpu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(fpu_s_tests) / sizeof(fpu_s_tests[0]));

    set_testsuite("FPU secure interface test (TFM_S_FPU_TEST_1XXX)",
                  fpu_s_tests, list_size, p_test_suite);
}
