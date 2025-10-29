/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform_ns_tests.h"
#include "tfm_platform_api.h"
#include "platform_tests_common.h"

static struct test_t platform_interface_tests[] = {
    {&tfm_platform_test_common_001, "TFM_NS_PLATFORM_TEST_1001",
     "Minimal platform service test"},
};

void
register_testsuite_ns_platform_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(platform_interface_tests) /
                 sizeof(platform_interface_tests[0]));

    set_testsuite("Platform Service Non-Secure interface tests"
                  "(TFM_NS_PLATFORM_TEST_1XXX)",
                  platform_interface_tests, list_size, p_test_suite);
}
