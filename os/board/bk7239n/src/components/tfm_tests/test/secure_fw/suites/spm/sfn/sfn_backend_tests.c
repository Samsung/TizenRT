/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>

#include "sfn_backend_tests.h"
#include "client_api_tests.h"
#if PSA_FRAMEWORK_HAS_MM_IOVEC
#include "mmiovec_test.h"
#endif

#include "psa/client.h"
#include "psa/framework_feature.h"
#include "psa_manifest/sid.h"

void tfm_sfn_test_1001(struct test_result_t *ret)
{
    psa_framework_version_test(ret);
}

void tfm_sfn_test_1002(struct test_result_t *ret)
{
    psa_version_test(SFN_TEST_STATELESS_SID, ret);
}

void tfm_sfn_test_1003(struct test_result_t *ret)
{
    psa_handle_t handle;

    handle = psa_connect(SFN_TEST_CONNECTION_BASED_SID,
                         SFN_TEST_CONNECTION_BASED_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a connection-based service fails.\r\n");
        return;
    }

    request_rot_service_test(handle, ret);

    psa_close(handle);
}

void tfm_sfn_test_1004(struct test_result_t *ret)
{
#if DOMAIN_NS == 1
    psa_handle_t handle;

    /*
     * Connecting to a stateless service should return PROGRAMMER ERROR to NS
     * client and panic for Secure clients.
     * So this test code is only enabled for NS test.
     */
    handle = psa_connect(SFN_TEST_STATELESS_SID, SFN_TEST_STATELESS_VERSION);
    if (PSA_HANDLE_IS_VALID(handle)) {
        TEST_FAIL("Connecting to a stateless service should not succeed.\r\n");
        return;
    }
#endif

    request_rot_service_test(SFN_TEST_STATELESS_HANDLE, ret);
}

#if PSA_FRAMEWORK_HAS_MM_IOVEC
void tfm_sfn_test_1005(struct test_result_t *ret)
{
    invec_map_unmap_test(ret, SFN_TEST_STATELESS_HANDLE);
}

void tfm_sfn_test_1006(struct test_result_t *ret)
{
    outvec_map_unmap_test(ret, SFN_TEST_STATELESS_HANDLE);
}

void tfm_sfn_test_1007(struct test_result_t *ret)
{
    outvec_map_only_test(ret, SFN_TEST_STATELESS_HANDLE);
}

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */
