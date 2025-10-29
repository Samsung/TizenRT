/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_hash_util.h
 *
 * This file contains the declarations of the internal utility functions used
 * to convert Hash types between PSA and low-level CC driver format
 *
 */

#ifndef CC3XX_INTERNAL_HASH_UTIL_H
#define CC3XX_INTERNAL_HASH_UTIL_H

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Function to convert from a PSA hash algorithm of type
 *        \ref psa_algorithm_t to a CC hash mode, either RSA or ECC
 *
 * \param[in]  alg            Input hash algorithm
 * \param[in]  performHashing Boolean indicating whether the conversion
 *                            of the original algorithm is happening from
 *                            a scheme which enforces that the input needs
 *                            to be hashed or not. CC types regarding hash_mode
 *                            specifically indicate whether the CC must do
 *                            input hashing or not
 * \param[out] hash_mode      Pointer to the hash_mode to be used in the
 *                            low-level driver. It can be either of type
 *                            \ref CCRsaHashOpMode_t or \ref CCEcpkiHashOpMode_t
 */
psa_status_t cc3xx_psa_hash_mode_to_cc_hash_mode(psa_algorithm_t alg,
                                                 bool performHashing,
                                                 void *hash_mode);
#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_HASH_UTIL_H */
