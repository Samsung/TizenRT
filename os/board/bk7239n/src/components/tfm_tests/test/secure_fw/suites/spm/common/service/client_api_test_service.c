/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "client_api_test_service.h"

#include "client_api_test_defs.h"
#include "psa/error.h"
#include "psa/service.h"

psa_status_t client_api_test_rot_srv(const psa_msg_t *msg)
{
    int i;
    uint32_t in_data;
    uint32_t out_data = CLIENT_API_TEST_OUTPUT_DATA;

    for (i = 0; i < PSA_MAX_IOVEC; i++) {
        if (msg->in_size[i] != 0) {
            if (psa_read(msg->handle, i, &in_data, sizeof(in_data))
                    != sizeof(in_data)) {
                return CLIENT_API_TEST_ERROR_FAIL;
            }

            if (in_data != CLIENT_API_TEST_INPUT_DATA) {
                return CLIENT_API_TEST_ERROR_FAIL;
            }
        }

        if (msg->out_size[i] != 0) {
            psa_write(msg->handle, i, &out_data, sizeof(out_data));
        }
    }

    return PSA_SUCCESS;
}
