/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "mmiovec_test.h"
#include "test_framework.h"
#include "spm_test_defs.h"
#include "psa/client.h"
#include "psa/error.h"
#include "psa_manifest/sid.h"

void invec_map_unmap_test(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;
    uint32_t invec_data[] = MMIOVECT_TEST_INVEC;
    psa_invec in_vec[] = { {&invec_data[0], sizeof(invec_data[0])},
                           {&invec_data[1], sizeof(invec_data[1])},
                           {&invec_data[2], sizeof(invec_data[2])},
                           {&invec_data[3], sizeof(invec_data[3])} };

    status = psa_call(handle, INVEC_MAP_AND_UNMAP, in_vec, MMIOVEC_TEST_VEC_LEN,
                      NULL, 0);
    if (status < 0) {
        TEST_FAIL("Input vector mapping test failed.\r\n");
        return;
    }

    ret->val = TEST_PASSED;
}

void outvec_map_unmap_test(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;
    int i;
    uint8_t *out_data;
    uint32_t outvec_data[MMIOVEC_TEST_VEC_LEN] = {0};
    psa_outvec out_vec[] = { {&outvec_data[0], sizeof(outvec_data[0])},
                             {&outvec_data[1], sizeof(outvec_data[1])},
                             {&outvec_data[2], sizeof(outvec_data[2])},
                             {&outvec_data[3], sizeof(outvec_data[3])} };

    status = psa_call(handle, OUTVEC_MAP_AND_UNMAP, NULL, 0, out_vec,
                      MMIOVEC_TEST_VEC_LEN);

    if (status < 0) {
        TEST_FAIL("Output vector mapping test failed.\r\n");
        return;
    }

    for (i = 0; i < MMIOVEC_TEST_VEC_LEN; i++) {
        if (out_vec[i].len != sizeof(uint8_t)) {
            TEST_FAIL("Output vector mapping test failed.\r\n");
            return;
        }
        out_data = (uint8_t *)(&outvec_data[i]);
        if (*out_data != MMIOVEC_OUTPUT_DATA) {
            TEST_FAIL("Output vector mapping test failed.\r\n");
            return;
        }
    }

    ret->val = TEST_PASSED;
}

void outvec_map_only_test(struct test_result_t *ret, psa_handle_t handle)
{
    psa_status_t status;
    int i;
    uint32_t outvec_data[MMIOVEC_TEST_VEC_LEN] = {0};
    psa_outvec out_vec[] = { {&outvec_data[0], sizeof(outvec_data[0])},
                             {&outvec_data[1], sizeof(outvec_data[1])},
                             {&outvec_data[2], sizeof(outvec_data[2])},
                             {&outvec_data[3], sizeof(outvec_data[3])} };

    status = psa_call(handle, OUTVEC_MAP_NOT_UNMAP, NULL, 0, out_vec,
                      MMIOVEC_TEST_VEC_LEN);

    if (status < 0) {
        TEST_FAIL("Output vector mapping test failed.\r\n");
        return;
    }

    for (i = 0; i < MMIOVEC_TEST_VEC_LEN; i++) {
        if (out_vec[i].len != 0) {
            TEST_FAIL("Output vector mapping test failed.\r\n");
            return;
        }
    }

    ret->val = TEST_PASSED;
}
