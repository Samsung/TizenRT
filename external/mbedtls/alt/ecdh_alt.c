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
#include <stdio.h>
#include <stdlib.h>
#include <security/security_common.h>
#include <security/security_keymgr.h>
#include <security/security_auth.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ECDH_C)

#include "mbedtls/ecdh.h"
#include <string.h>

#include "mbedtls/alt/common.h"
#include "alt_utils.h"

#if defined(MBEDTLS_ECDH_GEN_PUBLIC_ALT)
int mbedtls_ecdh_gen_public(mbedtls_ecp_group *grp,
							mbedtls_mpi *d, mbedtls_ecp_point *Q,
							int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	unsigned int ret;
	security_key_type key_type;
	security_handle shnd;
	unsigned char key_data[MBEDTLS_MAX_KEY_SIZE_ALT];
	unsigned char key_priv[MBEDTLS_MAX_KEY_SIZE_ALT];
	security_data x_key = {key_data, sizeof(key_data)};
	security_data y_key = {key_priv, sizeof(key_priv)};
	char key_path[7];

	if ((key_type = alt_get_keytype(grp->id)) == KEY_UNKNOWN) {
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
		goto cleanup;
	}

	ret = security_init(&shnd);
	if (ret != SECURITY_OK) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	grp->key_index = ECP_KEY_INDEX;
	if ((grp->key_index = alt_gen_key(shnd, key_type, ECP_KEY_INDEX)) == -1) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		security_deinit(shnd);
		goto cleanup;
	}

	/* Get Public value from sss */
	snprintf(key_path, 7, "ss/%d", grp->key_index);
	ret = keymgr_get_key(shnd, key_type, key_path, &x_key, &y_key);
	if (ret != SECURITY_OK) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		(void)keymgr_remove_key(shnd, key_type, key_path);
		(void)security_deinit(shnd);
		goto cleanup;
	}

	/* Copy pub value to Q */
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&Q->X, x_key.data, x_key.length));
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&Q->Y, y_key.data, y_key.length));
	MBEDTLS_MPI_CHK(mbedtls_mpi_lset(&Q->Z, 1));

	(void)security_deinit(shnd);
	ret = 0;

cleanup:
	return ret;
}

#endif							/* MBEDTLS_ECDH_GEN_PUBLIC_ALT */

#if defined(MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
int mbedtls_ecdh_compute_shared(mbedtls_ecp_group *grp, mbedtls_mpi *z, const mbedtls_ecp_point *Q, const mbedtls_mpi *d, int (*f_rng)(void *, unsigned char *, size_t), void *p_rng)
{
	int ret;
	security_ecdh_param ecc_pub = {0,};
	unsigned char shared_secret_data[MBEDTLS_MAX_KEY_SIZE_ALT];
	security_data shared_secret = {shared_secret_data, MBEDTLS_MAX_KEY_SIZE_ALT};
	security_handle shnd;
	security_key_type key_type = KEY_UNKNOWN;
	char key_path[7];

	/* compute ECC shared secret with stored key (permanent) */
	ret = security_init(&shnd);
	if (ret != SECURITY_OK) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	memset(&ecc_pub, 0, sizeof(security_ecdh_param));

	ecc_pub.pubkey_x = (security_data *)malloc(sizeof(security_data));
	if (!ecc_pub.pubkey_x) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}
	ecc_pub.pubkey_y = (security_data *)malloc(sizeof(security_data));
	if (!ecc_pub.pubkey_y) {
		free(ecc_pub.pubkey_x);
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup;
	}

	ecc_pub.pubkey_x->length = mbedtls_mpi_size(&Q->X);
	ecc_pub.pubkey_y->length = mbedtls_mpi_size(&Q->Y);

	if (!(ecc_pub.pubkey_x->data = (unsigned char *)malloc(ecc_pub.pubkey_x->length))) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup_with_mem;
	}

	if (!(ecc_pub.pubkey_y->data = (unsigned char *)malloc(ecc_pub.pubkey_y->length))) {
		ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
		goto cleanup_with_mem;
	}

	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&Q->X, ecc_pub.pubkey_x->data, ecc_pub.pubkey_x->length));
	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&Q->Y, ecc_pub.pubkey_y->data, ecc_pub.pubkey_y->length));

	if ((ecc_pub.curve = alt_get_curve(grp->id)) == ECDSA_UNKNOWN) {
		ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
		goto cleanup_with_mem;
	}

	snprintf(key_path, 7, "ss/%d", grp->key_index);
	ret = auth_compute_ecdhkey(shnd, key_path, &ecc_pub, &shared_secret);
	if (ret != SECURITY_OK) {
		ret = MBEDTLS_ERR_ECP_HW_ACCEL_FAILED;
		goto cleanup_with_mem;
	}
	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(z, shared_secret.data, shared_secret.length));

	ret = 0;

cleanup_with_mem:
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

cleanup:
	if ((key_type = alt_get_keytype(grp->id)) == KEY_UNKNOWN) {
		ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
	} else {
		keymgr_remove_key(shnd, key_type, key_path);
	}
	security_deinit(shnd);

	return ret;
}

#endif							/* MBEDTLS_ECDH_COMPUTE_SHARED_ALT */
#endif							/* MBEDTLS_ECDH_C */
