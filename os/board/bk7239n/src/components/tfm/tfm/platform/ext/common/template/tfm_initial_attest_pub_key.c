/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/crypto.h"

/*
 * This file contains the hard coded public key part of the ECDSA P-256 secret
 * key in: platform/ext/common/template/tfm_initial_attestation_key.pem
 *
 * The public key is used to verify the initial attestation token (IAT) for test
 * and debug purpose only. This file is built only when tests are enabled.
 *
 * The key is stored in raw format, without any encoding(ASN.1, COSE).
 *
 * #######  DO NOT USE THIS KEY IN PRODUCTION #######
 */

/* Type of the EC curve which the key belongs to, in PSA curve ID form */
const psa_ecc_family_t initial_attest_curve_type = PSA_ECC_FAMILY_SECP_R1;

/*
 * Initial attestation public key in raw format, without any encoding.
 * It belongs to the ECDSA P-256 curve.
 *
 * The octet string below is the output of psa_export_public_key(), which
 * consists of the following parts:
 * - The byte 0x04;
 * - x_P as a 32 byte string, big-endian;
 * - y_P as a 32 byte string, big-endian;
 *
 * The octet string can be passed to psa_import_key() to create a public key
 * object for IAT verification.
 *
 * See psa_export_public_key() in PSA Crypto API spec for more details of
 * representation of the public key.
 */
const uint8_t initial_attest_pub_key[] =
{
    0x04, 0x79, 0xEB, 0xA9, 0x0E, 0x8B, 0xF4, 0x50,
    0xA6, 0x75, 0x15, 0x76, 0xAD, 0x45, 0x99, 0xB0,
    0x7A, 0xDF, 0x93, 0x8D, 0xA3, 0xBB, 0x0B, 0xD1,
    0x7D, 0x00, 0x36, 0xED, 0x49, 0xA2, 0xD0, 0xFC,
    0x3F, 0xBF, 0xCD, 0xFA, 0x89, 0x56, 0xB5, 0x68,
    0xBF, 0xDB, 0x86, 0x73, 0xE6, 0x48, 0xD8, 0xB5,
    0x8D, 0x92, 0x99, 0x55, 0xB1, 0x4A, 0x26, 0xC3,
    0x08, 0x0F, 0x34, 0x11, 0x7D, 0x97, 0x1D, 0x68,
    0x64,
};

const uint32_t initial_attest_pub_key_size = sizeof(initial_attest_pub_key);
