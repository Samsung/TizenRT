/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "irq_test.h"
#include "test_framework.h"
#include "spm_test_defs.h"
#include "psa/client.h"
#include "psa/error.h"
#include "psa_manifest/sid.h"

#ifdef TEST_NS_SLIH_IRQ

void irq_test_slih_case_1(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;

    status = psa_call(handle, TFM_SLIH_TEST_CASE_1, NULL, 0, NULL, 0);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("TFM_NS_IRQ_TEST_SLIH_HANDLING FAILED\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif /* TEST_NS_SLIH_IRQ */

#ifdef TEST_NS_FLIH_IRQ

void irq_test_flih_case_1(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;

    status = psa_call(handle, TFM_FLIH_TEST_CASE_1, NULL, 0, NULL, 0);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("TFM_NS_IRQ_TEST_FLIH not returning signal, FAILED\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}

void irq_test_flih_case_2(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;

    status = psa_call(handle, TFM_FLIH_TEST_CASE_2, NULL, 0, NULL, 0);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("TFM_NS_IRQ_TEST_FLIH returning signal FAILED\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif /* TEST_NS_FLIH_IRQ */
