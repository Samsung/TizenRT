/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "spm_test_defs.h"
#include "tfm_mmiovec_test_service.h"
#include "psa/error.h"
#include "psa/service.h"

psa_status_t test_service_mmiovec_invec(const psa_msg_t *msg)
{
    uint32_t *invec_base = NULL;
    int i;
    uint32_t invec_data[] = MMIOVECT_TEST_INVEC;

    for (i = 0; i < MMIOVEC_TEST_VEC_LEN; i++) {
        /* Map the input vector */
        invec_base = (uint32_t *)psa_map_invec(msg->handle, i);
        if (*invec_base != invec_data[i]) {
            return TFM_MMIOVEC_TEST_ERROR;
        }
        /* Unmap the input vector */
        psa_unmap_invec(msg->handle, i);
    }

    return PSA_SUCCESS;
}

psa_status_t test_service_mmiovec_outvec(const psa_msg_t *msg)
{
    uint8_t *outvec_base = NULL;
    int i;

    for (i = 0; i < MMIOVEC_TEST_VEC_LEN; i++) {
        /* Map the output vector */
        outvec_base = (uint8_t *)psa_map_outvec(msg->handle, i);
        *outvec_base = MMIOVEC_OUTPUT_DATA;
        /* Unmap the output vector */
        psa_unmap_outvec(msg->handle, i, sizeof(uint8_t));
    }

    return PSA_SUCCESS;
}

psa_status_t test_service_outvec_not_unmap(const psa_msg_t *msg)
{
    uint8_t *outvec_base = NULL;
    int i;

    for (i = 0; i < MMIOVEC_TEST_VEC_LEN; i++) {
        /* Map the output vector */
        outvec_base = (uint8_t *)psa_map_outvec(msg->handle, i);
    }

    return PSA_SUCCESS;
}
