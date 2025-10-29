/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "extra_s_tests.h"

void extra_s_tests_entry(struct test_result_t *ret)
{
    struct extra_tests_t extra_s_test_t;

    if (extra_tests_init(&extra_s_test_t) != TEST_SUITE_ERR_NO_ERROR) {
        ret->val = TEST_FAILED;
    }

    if (extra_s_test_t.test_entry == NULL) {
        ret->val = TEST_FAILED;
    }

    if (extra_s_test_t.test_entry() == extra_s_test_t.expected_ret) {
        ret->val = TEST_PASSED;
    }else {
        ret->val = TEST_FAILED;
    }
}

static struct test_t extra_s_interface_tests[] = {
    {&extra_s_tests_entry, "TFM_EXTRA_TEST_1001",
     "Extra Secure test"},
};

void register_testsuite_extra_s_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(extra_s_interface_tests) /
                 sizeof(extra_s_interface_tests[0]));

    set_testsuite("Extra Secure interface tests"
                  "(TFM_S_EXTRA_TEST_1XXX)",
                  extra_s_interface_tests, list_size, p_test_suite);
}
