/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fwu_ns_tests.h"
#include "test_framework_helpers.h"
#include "../fwu_tests_common.h"

static struct test_t psa_fwu_ns_tests[] = {
    {&tfm_fwu_test_common_001, "TFM_NS_FWU_TEST_1001",
     "Write, install and abort interface"},
#if defined TFM_FWU_TEST_WRITE_WITH_NULL
    {&tfm_fwu_test_common_002, "TFM_NS_FWU_TEST_1002",
     "Write interface with NULL block pointer"},
#endif
    {&tfm_fwu_test_common_003, "TFM_NS_FWU_TEST_1003",
     "Write interface with image_offset + block_size overflow"},
    {&tfm_fwu_test_common_004, "TFM_NS_FWU_TEST_1004",
     "Install interface with NULL dependency uuid"},
    {&tfm_fwu_test_common_005, "TFM_NS_FWU_TEST_1005",
     "Install interface with NULL dependency version"},
    {&tfm_fwu_test_common_006, "TFM_NS_FWU_TEST_1006",
     "Install before write"},
    {&tfm_fwu_test_common_007, "TFM_NS_FWU_TEST_1007",
     "Install after abort"},
    {&tfm_fwu_test_common_008, "TFM_NS_FWU_TEST_1008",
     "Abort interface with no image is being installing"},
    {&tfm_fwu_test_common_009, "TFM_NS_FWU_TEST_1009",
     "Query interface with the active image"},
#if defined TFM_FWU_TEST_QUERY_WITH_NULL
    {&tfm_fwu_test_common_010, "TFM_NS_FWU_TEST_1010",
     "Query interface with NULL info"},
#endif
    {&tfm_fwu_test_common_011, "TFM_NS_FWU_TEST_1011",
     "Query interface invald image id"},
    {&tfm_fwu_test_common_012, "TFM_NS_FWU_TEST_1012",
     "Accept interface"},
#ifdef TFM_FWU_TEST_REQUEST_REBOOT
    {&tfm_fwu_test_common_013, "TFM_NS_FWU_TEST_1013",
     "Reboot interface"},
#endif
#if (MCUBOOT_IMAGE_NUMBER > 1)
    {&tfm_fwu_test_common_014, "TFM_NS_FWU_TEST_1014",
     "Image update with dependency, no new image is required"},
    {&tfm_fwu_test_common_015, "TFM_NS_FWU_TEST_1015",
     "Image update with dependency, new image is required"},
#endif
#ifdef TFM_PSA_API
    {&tfm_fwu_test_common_016, "TFM_NS_FWU_TEST_1016",
     "psa_fwu_write boundary test"},
#endif
};

void register_testsuite_ns_psa_fwu_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(psa_fwu_ns_tests) / sizeof(psa_fwu_ns_tests[0]));

    set_testsuite("PSA firmware update NS interface tests "
                 "(TFM_NS_FWU_TEST_1xxx)",
                  psa_fwu_ns_tests, list_size, p_test_suite);
}
