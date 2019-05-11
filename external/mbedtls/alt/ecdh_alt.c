/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/*
 *  Elliptic curve Diffie-Hellman
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * RFC 4492
 */

#include <tinyara/config.h>
#include <tinyara/seclink.h>
#include <tinyara/security_hal.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDH_C)

#include "mbedtls/ecdh.h"
#include <string.h>

#include "mbedtls/alt/common.h"
#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
int mbedtls_ecdh_gen_public(mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	unsigned int ret;
	hal_key_type key_type;
	sl_ctx shnd;
	hal_result_e hres = HAL_FAIL;
	unsigned char key_data[MBEDTLS_MAX_KEY_SIZE_ALT];
	unsigned char key_priv[MBEDTLS_MAX_KEY_SIZE_ALT];
	hal_data key = {key_data, sizeof(key_data), key_priv, sizeof(key_priv)};

	switch (grp->id) {
	case MBEDTLS_ECP_DP_SECP192R1:
		key_type = HAL_KEY_ECC_SEC_P192R1;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		key_type = HAL_KEY_ECC_SEC_P224R1;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		key_type = HAL_KEY_ECC_SEC_P256R1;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		key_type = HAL_KEY_ECC_SEC_P384R1;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		key_type = HAL_KEY_ECC_SEC_P512R1;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		key_type = HAL_KEY_ECC_BRAINPOOL_P256R1;
		break;
	case MBEDTLS_ECP_DP_BP384R1:
		key_type = HAL_KEY_ECC_BRAINPOOL_P384R1;
		break;
	case MBEDTLS_ECP_DP_BP512R1:
		key_type = HAL_KEY_ECC_BRAINPOOL_P512R1;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	ret = sl_generate_key(shnd, key_type, grp->key_index, &hres);
	if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		sl_deinit(shnd);
		goto cleanup;
	}

	/* Get Public value from sss */
	ret = sl_get_key(shnd, key_type, grp->key_index, &key, &hres);
	if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		sl_deinit(shnd);
		goto cleanup;
	}

	/* Copy pub value to Q */
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&Q->X, key.data, key.data_len));
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&Q->Y, key.priv, key.priv_len));
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&Q->Z, 1));

	sl_deinit(shnd);
	ret = 0;

cleanup:
	return ret;
}

#endif							/* MBEDTLS_ECDH_GEN_PUBLIC_ALT */

#if defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
int mbedtls_ecdh_compute_shared(mbedtls_ecp_group *grp, mbedtls_mpi *z, const mbedtls_ecp_point *Q, const mbedtls_mpi *d, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	hal_ecdh_data ecc_pub = {0,};
	unsigned char shared_secret_data[MBEDTLS_MAX_KEY_SIZE_ALT];
	hal_data shared_secret = {shared_secret_data, MBEDTLS_MAX_KEY_SIZE_ALT, NULL, 0};
	sl_ctx shnd;
	hal_result_e hres = HAL_FAIL;

	memset(&ecc_pub, 0, sizeof(hal_ecdh_data));

	ecc_pub.pubkey_x = (hal_data *)malloc(sizeof(hal_data));
	if (!ecc_pub.pubkey_x) {
		return MBEDTLS_ERR_ECP_ALLOC_FAILED;
	}
	ecc_pub.pubkey_y = (hal_data *)malloc(sizeof(hal_data));
	if (!ecc_pub.pubkey_y) {
		free(ecc_pub.pubkey_x);
		return MBEDTLS_ERR_ECP_ALLOC_FAILED;
	}

	ecc_pub.pubkey_x->data_len = mbedtls_mpi_size(&Q->X);
	ecc_pub.pubkey_y->data_len = mbedtls_mpi_size(&Q->Y);

	if (!(ecc_pub.pubkey_x->data = (unsigned char *)malloc(ecc_pub.pubkey_x->data_len))) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	if (!(ecc_pub.pubkey_y->data = (unsigned char *)malloc(ecc_pub.pubkey_y->data_len))) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&Q->X, ecc_pub.pubkey_x->data, ecc_pub.pubkey_x->data_len));
	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&Q->Y, ecc_pub.pubkey_y->data, ecc_pub.pubkey_y->data_len));

	switch (grp->id) {
	case MBEDTLS_ECP_DP_SECP192R1:
		ecc_pub.curve = HAL_ECDSA_SEC_P192R1;
		break;
	case MBEDTLS_ECP_DP_SECP224R1:
		ecc_pub.curve = HAL_ECDSA_SEC_P224R1;
		break;
	case MBEDTLS_ECP_DP_SECP256R1:
		ecc_pub.curve = HAL_ECDSA_SEC_P256R1;
		break;
	case MBEDTLS_ECP_DP_SECP384R1:
		ecc_pub.curve = HAL_ECDSA_SEC_P384R1;
		break;
	case MBEDTLS_ECP_DP_SECP521R1:
		ecc_pub.curve = HAL_ECDSA_SEC_P512R1;
		break;
	case MBEDTLS_ECP_DP_BP256R1:
		ecc_pub.curve = HAL_ECDSA_BRAINPOOL_P256R1;
		break;
	default:
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup;
	}

	/* compute ECC shared secret with stored key (permanent) */
	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	ret = sl_ecdh_compute_shared_secret(shnd, &ecc_pub, grp->key_index, &shared_secret, &hres);

	if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		sl_deinit(shnd);
		goto cleanup;
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(z, shared_secret.data, shared_secret.data_len));

	sl_deinit(shnd);
	ret = 0;

cleanup:

	if (ecc_pub.pubkey_x) {
		if (ecc_pub.pubkey_x->data) {
			free(ecc_pub.pubkey_x->data);
		}
		free(ecc_pub.pubkey_x);
	}

	if (ecc_pub.pubkey_y) {
		if (ecc_pub.pubkey_y->data) {
			free(ecc_pub.pubkey_y->data);
		}
		free(ecc_pub.pubkey_y);
	}

	return ret;
}

#endif							/* MBEDTLS_ECDH_COMPUTE_SHARED_ALT */
#endif							/* MBEDTLS_ECDH_C */
