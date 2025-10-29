/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/client.h"
#include "tfm_psa_call_pack.h"

psa_status_t psa_call(psa_handle_t handle,
                      int32_t type,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len)
{
    if ((type > INT16_MAX) ||
        (type < INT16_MIN) ||
        (in_len > UINT8_MAX) ||
        (out_len > UINT8_MAX)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_psa_call_pack(handle, PARAM_PACK(type, in_len, out_len),
                             in_vec, out_vec);
}
