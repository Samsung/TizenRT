/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bl1_1_trng_tests.h"

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "test_framework_helpers.h"
#include "trng.h"

#define TEST_VAL_AM      512
#define REPEAT_THRESHOLD (TEST_VAL_AM / 16)

static void tfm_bl1_trng_test_3001(struct test_result_t *ret)
{
    uint8_t buf[TEST_VAL_AM];
    size_t buf_idx;
    uint16_t table[UINT8_MAX + 1];
    size_t table_idx;
    bool failed = false;
    int rc;

    memset(table, 0, sizeof(table));
    rc = bl1_trng_generate_random(buf, sizeof(buf));
    if (rc) {
        TEST_FAIL("TRNG returned error");
        return;
    }

    /* Count the amount of each byte value that's emitted by the TRNG */
    for (buf_idx = 0; buf_idx < sizeof(buf); buf_idx++) {
        table[buf[buf_idx]] += 1;
    }

    /* If any of the counts is over the repeat threshold (currently P >= 1/16,
     * it is a failure. Note that we can't make this too close to P = 1/256 as
     * this would lead to unpredictable failures even with unbiased TRNGs.
     */
    for (table_idx = 0; table_idx < (sizeof(table) / sizeof(table[0])); table_idx++) {
        if (table[table_idx] >= REPEAT_THRESHOLD) {
            TEST_LOG("Byte 0x%x was repeated %d times\r\n", table_idx,
                                                            table[table_idx]);
            failed = true;
        }
    }

    if (failed) {
        TEST_FAIL("Byte was repeated too many times");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}

static void tfm_bl1_trng_test_3002(struct test_result_t *ret)
{
    int rc;

    rc = bl1_trng_generate_random(NULL, 128);
    if (rc == 0) {
        TEST_FAIL("TRNG returned success");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}


static void tfm_bl1_trng_test_3003(struct test_result_t *ret)
{
    int rc;

    rc = bl1_trng_generate_random(NULL, 0);
    if (rc) {
        TEST_FAIL("TRNG returned failure");
        return;
    }

    ret->val = TEST_PASSED;
    return;
}

static struct test_t trng_tests[] = {
    {&tfm_bl1_trng_test_3001, "TFM_BL1_TRNG_TEST_3001",
     "TRNG repeated bytes test" },
    {&tfm_bl1_trng_test_3002, "TFM_BL1_TRNG_TEST_3002",
     "TRNG output null-pointer test" },
    {&tfm_bl1_trng_test_3003, "TFM_BL1_TRNG_TEST_3003",
     "TRNG output zero-length test" },
};

void register_testsuite_bl1_trng_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size = (sizeof(trng_tests) / sizeof(trng_tests[0]));

    set_testsuite("TRNG interface test (TFM_BL1_TRNG_TEST_3XXX)",
                  trng_tests, list_size, p_test_suite);
}
