/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "client_api_tests.h"
#include "client_api_test_defs.h"

#include "test_framework.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"

void psa_framework_version_test(struct test_result_t *ret)
{
    uint32_t version;

    version = psa_framework_version();
    if (version != PSA_FRAMEWORK_VERSION) {
        TEST_FAIL("The version of the PSA Framework API is not valid!\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}

void psa_version_test(uint32_t sid, struct test_result_t *ret)
{
    uint32_t version;

    version = psa_version(sid);
    if (version == PSA_VERSION_NONE) {
        TEST_FAIL("RoT Service is not implemented or caller is not authorized" \
                  " to access it!\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}

void request_rot_service_test(psa_handle_t handle,
                              struct test_result_t *ret)
{
    psa_status_t status;
    uint32_t in_data_1 = CLIENT_API_TEST_INPUT_DATA;
    uint32_t in_data_2 = CLIENT_API_TEST_INPUT_DATA;
    uint32_t out_data_1 = 0, out_data_2 = 0;
    struct psa_invec invecs[] =
        {
            {&in_data_1, sizeof(in_data_1)},
            {&in_data_2, sizeof(in_data_2)}
        };
    struct psa_outvec outvecs[] =
        {
            {&out_data_1, sizeof(out_data_1)},
            {&out_data_2, sizeof(out_data_2)}
        };

    status = psa_call(handle, CLIENT_API_TEST_TYPE_REQUEST_SRV,
                      invecs, IOVEC_LEN(invecs), outvecs, IOVEC_LEN(outvecs));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("psa_call failed!\r\n");
        return;
    }

    if (out_data_1 != CLIENT_API_TEST_OUTPUT_DATA
        || out_data_2 != CLIENT_API_TEST_OUTPUT_DATA) {
        TEST_FAIL("Invalid output data!\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}
