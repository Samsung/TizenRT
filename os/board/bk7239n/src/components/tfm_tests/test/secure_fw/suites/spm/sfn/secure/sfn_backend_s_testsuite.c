/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "sfn_s_tests.h"
#include "../sfn_backend_tests.h"

#include "psa/framework_feature.h"

static struct test_t sfn_backend_s_tests[] = {
    {&tfm_sfn_test_1001, "TFM_S_SFN_TEST_1001",
     "Get PSA framework version"},
    {&tfm_sfn_test_1002, "TFM_S_SFN_TEST_1002",
     "Get version of an RoT Service"},
    {&tfm_sfn_test_1003, "TFM_S_SFN_TEST_1003",
     "Request a connection-based RoT Service"},
    {&tfm_sfn_test_1004, "TFM_S_SFN_TEST_1004",
     "Request a stateless RoT Service"},
#if PSA_FRAMEWORK_HAS_MM_IOVEC
    {&tfm_sfn_test_1005, "TFM_S_SFN_TEST_1005",
     "Mapping input vectors and unmapping them. "},
    {&tfm_sfn_test_1006, "TFM_S_SFN_TEST_1006",
     "Mapping output vectors and unmapping them. "},
    {&tfm_sfn_test_1007, "TFM_S_SFN_TEST_1007",
     "Mapping output vectors and not unmapping them. "},
#endif
};

void register_testsuite_s_sfn_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(sfn_backend_s_tests) / sizeof(sfn_backend_s_tests[0]));

    set_testsuite("SFN Backend Secure test (TFM_S_SFN_TEST_1XXX)",
                  sfn_backend_s_tests, list_size, p_test_suite);
}
