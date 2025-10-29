/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/** \file cc3xx_internal_ecdh.h
 *
 * This file contains the declarations of the internal functions required to
 * perform Elliptic Curve Diffie-Hellman, i.e. ECDH raw key agreement
 *
 */

#ifndef CC3XX_INTERNAL_ECDH_H
#define CC3XX_INTERNAL_ECDH_H

#include "psa/crypto.h"

#include "cc_ecpki_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct cc3xx_ecdh_context_s
 *
 * \brief Context structure required for Weierstrass curves
 *
 */
struct cc3xx_ecdh_context_s {
    CCEcpkiDomainID_t domain_id;
    CCEcpkiUserPrivKey_t priv_key;
    CCEcpkiUserPublKey_t publ_key;
};

/*! \typedef cc3xx_ecdh_context_t
 *
 * \brief A concise way to refer to \ref cc3xx_ecdh_context_s
 *
 */
typedef struct cc3xx_ecdh_context_s cc3xx_ecdh_context_t;

/**
 * @brief   Calculate a shared secret value (Short Weierstrass)
 *
 * @param[in]  ctx           ECDH context
 * @param[out] secret        Shared secret value
 * @param[out] secret_size   Shared secret value size
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecdh_calc_secret_wrst(
        cc3xx_ecdh_context_t *ctx,
        uint8_t *secret, size_t *secret_size);

/**
 * @brief   Calculate a shared secret value (Montgomery)
 *
 * @param[in]  priv_key      Priv. key
 * @param[in]  priv_key_size Priv. key size
 * @param[in]  publ_key      Publ. key
 * @param[in]  publ_key_size Publ. key size
 * @param[out] secret        Shared secret value
 * @param[out] secret_size   Shared secret value size
 *
 * @retval PSA_SUCCESS
 * @retval PSA_ERROR_NOT_SUPPORTED
 * @retval PSA_ERROR_INVALID_ARGUMENT
 */
psa_status_t cc3xx_ecdh_calc_secret_mont(
        const uint8_t *priv_key, size_t priv_key_size,
        const uint8_t *publ_key, size_t publ_key_size,
        uint8_t *secret, size_t *secret_size);

#ifdef __cplusplus
}
#endif
#endif /* CC3XX_INTERNAL_ECDH_H */
