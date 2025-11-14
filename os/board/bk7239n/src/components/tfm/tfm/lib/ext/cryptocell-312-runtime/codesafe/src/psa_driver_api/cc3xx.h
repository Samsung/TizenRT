/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CC3XX_H
#define CC3XX_H

/** \file cc3xx.h
 *
 * This file includes each module of the cc3xx driver that complies with the
 * PSA Cryptoprocessor Driver interface specification. the list of the
 * available modules is:
 *
 *  - Symmetric ciphers:                      cc3xx_psa_cipher.h
 *  - Access to TRNG for entropy extraction:  cc3xx_psa_entropy.h
 *  - Hashing:                                cc3xx_psa_hash.h
 *  - MAC signing/verification:               cc3xx_psa_mac.h
 *  - Authenticated Encryption w Assoc. Data: cc3xx_psa_aead.h
 *  - Raw key agreement:                      cc3xx_psa_key_agreement.h
 *  - Random key generation and key handling: cc3xx_psa_key_generation.h
 *  - Asymmetric signature schemes:           cc3xx_psa_asymmetric_signature.h
 *  - Asymmetric encryption schemes:          cc3xx_psa_asymmetric_encryption.h
 *
 * \note This file must be included by psa_crypto_driver_wrappers.c. The
 * definition of the context types used in the implementation is provided
 * through \ref cc3xx_crypto_primitives_private.h which is included, when
 * \ref PSA_CRYPTO_DRIVER_CC3XX is defined, in turn by psa/crypto.h, then
 * psa/crypto_struct.h and finally by psa/crypto_driver_context_primitives.h
 * and by psa/crypto_driver_context_composites.h from the mbedTLS module.
 * When \ref PSA_CRYPTO_DRIVER_CC3XX is not defined, the implementation
 * must explicitly include \ref cc3xx_crypto_primitives_private.h while the
 * interface can include \ref cc3xx_crypto_primitives.h only
 *
 */

#ifdef __DOXYGEN_ONLY__
/*!
 *  Used to enable the cc3xx driver in the PSA Driver Core layer. When this is
 *  defined, the \ref cc3xx_crypto_primitives_private.h type definitions are
 *  also visible through including psa/crypto.h
 *
 */
#define PSA_CRYPTO_DRIVER_CC3XX

/*!
 *  Used to enable the type conversion from CCError_t to psa_status_t when
 *  translating error codes from CC format to PSA format in ECC and RSA
 *  modules. The translation functions occupy ~1KB of flash hence keep them
 *  disabled by default. By default any CC low-level failure is treated as
 *  PSA_ERROR_HARDWARE_FAILURE
 */
#define CC3XX_CONFIG_ENABLE_CC_TO_PSA_TYPE_CONVERSION

/*!
 *  Used to enable support for SHA-1 algorithms in the interface layer. By
 *  default this is kept disabled as SHA-1 is deemed un-secure
 */
#define CC3XX_CONFIG_SUPPORT_SHA1

/*!
 *  Used to enable support for the GCM AEAD algorithm in the interface layer.
 *  By default this is kept enabled
 */
#define CC3XX_CONFIG_SUPPORT_GCM

/*!
 *  Used to enable support for RSA algorithm in the interface layer.
 *  By default this is kept enabled. At the moment affects only the RSA utils
 */
#define CC3XX_CONFIG_SUPPORT_RSA

/*!
 *  Used to enable support for the Chacha20 algorithm in the interface layer.
 *  By default it's kept enabled
 */
#define CC3XX_CONFIG_SUPPORT_CHACHA20

/*!
 *  Used to enable support for the Poly1305 algorithm in the interface layer.
 *  by default it's kept enabled. Note that there isn't a separate interface
 *  to exercise the Poly1305 algorithm other than through the combination with
 *  Chacha20 in an AEAD scheme.
 */
#define CC3XX_CONFIG_SUPPORT_CHACHA20_POLY1305
#endif /* __DOXYGEN_ONLY__ */

#include "cc3xx_psa_cipher.h"
#include "cc3xx_psa_entropy.h"
#include "cc3xx_psa_hash.h"
#include "cc3xx_psa_mac.h"
#include "cc3xx_psa_aead.h"
#include "cc3xx_psa_key_agreement.h"
#include "cc3xx_psa_key_generation.h"
#include "cc3xx_psa_asymmetric_signature.h"
#include "cc3xx_psa_asymmetric_encryption.h"

#endif /* CC3XX_H */
