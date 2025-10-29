/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bl1_2_integration_tests.h"

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "image.h"
#include "fih.h"
#include "region_defs.h"
#include "test_framework_helpers.h"

static int setup_test_image(struct bl1_2_image_t **image)
{
    fih_int fih_rc;
    *image = (struct bl1_2_image_t*)BL2_IMAGE_START;

    FIH_CALL(copy_and_decrypt_image, fih_rc, 0);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        FIH_CALL(validate_image_at_addr, fih_rc, *image);
        if (fih_eq(fih_rc, FIH_SUCCESS)) {
            return 0;
        }
    }

    FIH_CALL(copy_and_decrypt_image, fih_rc, 1);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        FIH_CALL(validate_image_at_addr, fih_rc, *image);
        if (fih_eq(fih_rc, FIH_SUCCESS)) {
            return 0;
        }
    }

    return 1;
}

static void tfm_bl1_integration_test_5001(struct test_result_t *ret)
{
    fih_int fih_rc;
    struct bl1_2_image_t *image;

    if(setup_test_image(&image)) {
        TEST_FAIL("Test image setup failed");
        return;
    }

    memset(image, 0, sizeof(struct bl1_2_image_t));

    FIH_CALL(validate_image_at_addr, fih_rc, image);
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
    struct bl1_2_image_t *image;

    if(setup_test_image(&image)) {
        TEST_FAIL("Test image setup failed");
        return;
    }

    image->protected_values.encrypted_data.data[0] ^= 0xFF;

    FIH_CALL(validate_image_at_addr, fih_rc, image);
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
    struct bl1_2_image_t *image;

    if(setup_test_image(&image)) {
        TEST_FAIL("Test image setup failed");
        return;
    }

    image->protected_values.encrypted_data.data[IMAGE_BL2_CODE_SIZE - 1] ^= 0xFF;

    FIH_CALL(validate_image_at_addr, fih_rc, image);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("Bad image was successfully validated");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}


static void tfm_bl1_integration_test_5004(struct test_result_t *ret)
{
    fih_int fih_rc;
    struct bl1_2_image_t *image;

    if(setup_test_image(&image)) {
        TEST_FAIL("Test image setup failed");
        return;
    }

    image->protected_values.security_counter = 0;

    FIH_CALL(validate_image_at_addr, fih_rc, image);
    if (fih_eq(fih_rc, FIH_SUCCESS)) {
        TEST_FAIL("Bad image was successfully validated");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}

static struct test_t integration_tests[] = {
    {&tfm_bl1_integration_test_5001, "TFM_BL1_2_INTEGRATION_TEST_5001",
     "INTEGRATION zeroed image test" },
    {&tfm_bl1_integration_test_5002, "TFM_BL1_2_INTEGRATION_TEST_5002",
     "INTEGRATION bit-flipped first byte test" },
    {&tfm_bl1_integration_test_5003, "TFM_BL1_2_INTEGRATION_TEST_5003",
     "INTEGRATION bit-flipped last byte test" },
    {&tfm_bl1_integration_test_5004, "TFM_BL1_2_INTEGRATION_TEST_5003",
     "INTEGRATION bad security counter test" },
};

void register_testsuite_bl1_2_integration(struct test_suite_t *p_test_suite)
{
    uint32_t list_size = (sizeof(integration_tests) / sizeof(integration_tests[0]));

    set_testsuite("INTEGRATION test (TFM_BL1_2_INTEGRATION_TEST_5XXX)",
                  integration_tests, list_size, p_test_suite);
}

