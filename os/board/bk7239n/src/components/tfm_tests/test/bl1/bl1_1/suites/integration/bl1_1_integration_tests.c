/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bl1_1_integration_tests.h"

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "region_defs.h"
#include "fih.h"
#include "otp.h"
#include "test_framework_helpers.h"

static void tfm_bl1_integration_test_5001(struct test_result_t *ret)
{
    fih_int fih_rc;
    uint8_t *bad_image = (uint8_t*)BL1_2_CODE_START;

    memset(bad_image, 0, BL1_2_CODE_SIZE);

    FIH_CALL(validate_image_at_addr, fih_rc, bad_image);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("Bad image was successfully validated");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}


static void tfm_bl1_integration_test_5002(struct test_result_t *ret)
{
    fih_int fih_rc;
    uint8_t *bad_image = (uint8_t*)BL1_2_CODE_START;

    FIH_CALL(bl1_read_bl1_2_image, fih_rc, bad_image);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("OTP read failed");
        return;
    }

    bad_image[0] ^= 0xFF;

    FIH_CALL(validate_image_at_addr, fih_rc, bad_image);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("Bad image was successfully validated");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}

static void tfm_bl1_integration_test_5003(struct test_result_t *ret)
{
    fih_int fih_rc;
    uint8_t *bad_image = (uint8_t*)BL1_2_CODE_START;

    FIH_CALL(bl1_read_bl1_2_image, fih_rc, bad_image);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("OTP read failed");
        return;
    }

    bad_image[BL1_2_CODE_SIZE - 1] ^= 0xFF;

    FIH_CALL(validate_image_at_addr, fih_rc, bad_image);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("Bad image was successfully validated");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}

static struct test_t integration_tests[] = {
    {&tfm_bl1_integration_test_5001, "TFM_BL1_1_INTEGRATION_TEST_5001",
     "INTEGRATION zeroed image test" },
    {&tfm_bl1_integration_test_5002, "TFM_BL1_1_INTEGRATION_TEST_5002",
     "INTEGRATION bit-flipped first byte test" },
    {&tfm_bl1_integration_test_5003, "TFM_BL1_1_INTEGRATION_TEST_5003",
     "INTEGRATION bit-flipped last byte test" },
};

void register_testsuite_bl1_1_integration(struct test_suite_t *p_test_suite)
{
    uint32_t list_size = (sizeof(integration_tests) / sizeof(integration_tests[0]));

    set_testsuite("INTEGRATION test (TFM_BL1_1_INTEGRATION_TEST_5XXX)",
                  integration_tests, list_size, p_test_suite);
}

