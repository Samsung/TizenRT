/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "extra_tests_common.h"

int32_t register_extra_tests(struct extra_tests_t *internal_test_t,
                     const struct extra_tests_t *platform_test_t)
{
    if (internal_test_t == NULL || platform_test_t == NULL ||
        platform_test_t->test_entry == NULL) {
        return EXTRA_TEST_FAILED;
    }

    internal_test_t->test_entry = platform_test_t->test_entry;
    internal_test_t->expected_ret = platform_test_t->expected_ret;

    return EXTRA_TEST_SUCCESS;
}
