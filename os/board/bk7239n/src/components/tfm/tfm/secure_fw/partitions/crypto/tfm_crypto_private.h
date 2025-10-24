/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CRYPTO_PRIVATE_H__
#define __TFM_CRYPTO_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_PSA_API

#include <stdbool.h>

/* \brief Verifies that in_len is in the [in_min, in_max] range
 *        AND out_len is in the [out_min, out_max] range. In
 *        case any of the two in_len or out_len is out of range,
 *        returns false. Returns true in case of success.
 */
static inline bool tfm_crypto_private_check_length(
    size_t in_len, size_t in_min, size_t in_max,
    size_t out_len, size_t out_min, size_t out_max)
{
   if ((in_len >= in_min) && (in_len <= in_max) &&
        (out_len >= out_min) && (out_len <= out_max)) {
        return true;
    }
    return false;
}

/*
 * Validate the IOVEC[] lengths for IPC model. The tfm_crypto_call_srv()
 * reduces the entries in IOVEC[] which are empty from `in_len` and `out_len`.
 * This means that Crypto service APIs need to ensure that the `in_len`
 * and `out_len` are within the expected range.
 *
 * Also tfm_crypto_call_srv() ensures that all entries in IOVEC[] are
 * initialised. Hence all entries in IOVEC[] can be accessed to
 * initialize internal variables even if they are outside `in_len`
 * and `out_len`.
 */
#define CRYPTO_IN_OUT_LEN_VALIDATE(in_len, in_min, in_max, out_len, out_min, out_max)   \
            if (!tfm_crypto_private_check_length(                             \
                    in_len, in_min, in_max, out_len, out_min, out_max)) {    \
                return PSA_ERROR_PROGRAMMER_ERROR;                           \
            }
#else
/*
 * Validate the IOVEC[] lengths for Library model.  Unlike the IPC model, the
 * service APIs expects to receive the exact of `in_len` and `out_len`
 * as expected by the API.
 */
#define CRYPTO_IN_OUT_LEN_VALIDATE(in_len, in_min, in_max, out_len, out_min, out_max)    \
            if (((in_len) != (in_max)) || ((out_len) != (out_max))) {     \
                return PSA_ERROR_PROGRAMMER_ERROR;                        \
            }
#endif


#ifdef __cplusplus
}
#endif

#endif /* __TFM_CRYPTO_PRIVATE_H__ */
