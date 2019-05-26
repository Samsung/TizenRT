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
/**
 * ASN.1
 */

/*
 *  Generic ASN.1 parsing
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
 *  This file is based on part of mbed TLS (https://tls.mbed.org)
 */

#include <tinyara/config.h>
#include <tinyara/seclink_drv.h>
#include <tinyara/security_hal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../arch/arm/src/s5j/sss/isp_custom.h"

#define ISP_CHECKBUSY() while (isp_get_status()) {}

#define HWRAP_TAG "[HAL_WRAPPER]"

#define HWRAP_ENTER                                                         \
	do {                                                                    \
		sedbg(HWRAP_TAG"%s:%d\n", __FILE__, __LINE__);                      \
	} while (0)

#define HAL_MAX_RANDOM_SIZE 256
#define HAL_MAX_ECP_KEY_SIZE_ALT 128
#define HAL_MAX_ECDSA_LEN 256
#define HAL_MAX_BUF_SIZE 2048
#define HAL_MAX_SHARED_SECRET_LEN 256

#define AES_ECB_MODE		(0x0008)
#define AES_CBC_MODE		(0x0108)
#define AES_CTR_MODE		(0x0208)
#define AES_XTS_MODE		(0x0308)
#define AES_CCM_MODE		(0x1008)
#define AES_GCM_MODE		(0x1108)
#define AES_KW_MODE			(0x1208)

#define HAL_COPY_DATA(in, out, len)                     \
	do {                                                \
		if (in->data == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->data, out, len);                     \
		in->data_len = len;                             \
	} while (0)

#define HAL_COPY_PRIV_DATA(in, out, len)                \
	do {                                                \
		if (in->priv == NULL) {                         \
			return HAL_INVALID_ARGS;                    \
		}                                               \
		memcpy(in->priv, out, len);                     \
		in->priv_len = len;                             \
	} while (0)


#define ciL    (sizeof(uint64_t))
#define biL    (ciL << 3)               /* bits  in limb  */
#define biH    (ciL << 2)               /* half limb size */

#define HAL_ASN1_CHK_ADD(g, f)                      \
	do { if ((ret = f) < 0) return HAL_FAIL; else   \
		g += ret;                                   \
	} while (0)

#define GET_BYTE(X, i)                                                \
	(((X)->p[(i) / ciL] >> (((i) % ciL) * 8)) & 0xff)

#define HAL_MPI_CHK(f) do { if ((ret = f) != 0) { ret = HAL_FAIL; goto cleanup; } } while (0)
#define CHARS_TO_LIMBS(i) ((i) / ciL + ((i) % ciL != 0))

typedef struct _hal_mpi {
	int s;
	uint32_t n;
	uint64_t *p;
} hal_mpi;

static void hal_mpi_zeroize(uint64_t *v, uint32_t n)
{
	volatile uint64_t *p = v;
	while (n--) {
		*p++ = 0;
	}
}

static void hal_mpi_init(hal_mpi *X)
{
	if (X == NULL) {
		return;
	}

	X->s = 1;
	X->n = 0;
	X->p = NULL;
}

static void hal_mpi_free(hal_mpi *X)
{
	if (X == NULL) {
		return;
	}

	if (X->p != NULL) {
		hal_mpi_zeroize(X->p, X->n);
		free(X->p);
	}

	X->s = 1;
	X->n = 0;
	X->p = NULL;
}

static int hal_mpi_grow(hal_mpi *X, uint32_t nblimbs)
{
	uint64_t *p;

	if (nblimbs > 10000) {
		return HAL_ALLOC_FAIL;
	}

	if (X->n < nblimbs)	{
		if ((p = (uint64_t *)calloc(nblimbs, ciL)) == NULL) {
			return HAL_ALLOC_FAIL;
		}

		if (X->p != NULL) {
			memcpy(p, X->p, X->n * ciL);
			hal_mpi_zeroize(X->p, X->n);
			free(X->p);
		}

		X->n = nblimbs;
		X->p = p;
	}

	return HAL_SUCCESS;
}

static int hal_mpi_lset(hal_mpi *X, int64_t z)
{
	int ret;

	HAL_MPI_CHK(hal_mpi_grow(X, 1));
	memset(X->p, 0, X->n * ciL);

	X->p[0] = (z < 0) ? -z : z;
	X->s    = (z < 0) ? -1 : 1;

cleanup:
	return ret;
}


static uint32_t hal_clz(const uint64_t x)
{
	uint32_t j;
	uint64_t mask = (uint64_t)1 << (biL - 1);

	for (j = 0; j < biL; j++) {
		if (x & mask) {
			break;
		}
		mask >>= 1;
	}

	return j;
}

static uint32_t hal_mpi_bitlen(const hal_mpi *X)
{
	uint32_t i;
	uint32_t j;

	if (X->n == 0) {
		return 0;
	}

	for (i = X->n - 1; i > 0; i--) {
		if (X->p[i] != 0) {
			break;
		}
	}
	j = biL - hal_clz(X->p[i]);

	return (i * biL) + j;
}

static uint32_t hal_mpi_size(const hal_mpi *X)
{
	return (hal_mpi_bitlen(X) + 7) >> 3;
}

static int hal_asn1_get_len(unsigned char **p, const unsigned char *end, uint32_t *len)
{
	if ((end - *p) < 1) {
		return HAL_INVALID_ARGS;
	}

	if ((**p & 0x80) == 0) {
		*len = *(*p)++;
	} else {
		switch (**p & 0x7F) {
		case 1:
			if ((end - *p) < 2) {
				return HAL_INVALID_ARGS;
			}
			*len = (*p)[1];
			(*p) += 2;
			break;
		case 2:
			if ((end - *p) < 3) {
				return HAL_INVALID_ARGS;
			}
			*len = ((uint32_t)(*p)[1] << 8) | (*p)[2];
			(*p) += 3;
			break;
		case 3:
			if ((end - *p) < 4) {
				return HAL_INVALID_ARGS;
			}
			*len = ((uint32_t)(*p)[1] << 16) | ((uint32_t)(*p)[2] << 8) | (*p)[3];
			(*p) += 4;
			break;
		case 4:
			if ((end - *p) < 5) {
				return HAL_INVALID_ARGS;
			}
			*len = ((uint32_t)(*p)[1] << 24) | ((uint32_t)(*p)[2] << 16) | ((uint32_t)(*p)[3] << 8) | (*p)[4];
			(*p) += 5;
			break;
		default:
			return HAL_INVALID_ARGS;
		}
	}

	if (*len > (uint32_t)(end - *p)) {
		return HAL_INVALID_ARGS;
	}

	return HAL_SUCCESS;
}

static int hal_asn1_get_tag(unsigned char **p, const unsigned char *end, uint32_t *len, int tag)
{
	if ((end - *p) < 1) {
		return HAL_INVALID_ARGS;
	}

	if (**p != tag) {
		return HAL_INVALID_ARGS;
	}

	(*p)++;

	return hal_asn1_get_len(p, end, len);
}

static int hal_mpi_read_binary(hal_mpi *X, const unsigned char *buf, uint32_t buflen)
{
	int ret;
	uint32_t i;
	uint32_t j;
	uint32_t const limbs = CHARS_TO_LIMBS(buflen);

	/* Ensure that target MPI has exactly the necessary number of limbs */
	if (X->n != limbs) {
		hal_mpi_free(X);
		hal_mpi_init(X);
		HAL_MPI_CHK(hal_mpi_grow(X, limbs));
	}

	HAL_MPI_CHK(hal_mpi_lset(X, 0));

	for (i = buflen, j = 0; i > 0; i--, j++) {
		X->p[j / ciL] |= ((uint64_t)buf[i - 1]) << ((j % ciL) << 3);
	}

cleanup:
	return ret;
}

static int hal_mpi_write_binary(const hal_mpi *X, unsigned char *buf, uint32_t buflen)
{
	uint32_t stored_bytes = X->n * ciL;
	uint32_t bytes_to_copy;
	unsigned char *p;
	uint32_t i;

	if (stored_bytes < buflen) {
		bytes_to_copy = stored_bytes;
		p = buf + buflen - stored_bytes;
		memset(buf, 0, buflen - stored_bytes);
	} else {
		bytes_to_copy = buflen;
		p = buf;
		for (i = bytes_to_copy; i < stored_bytes; i++) {
			if (GET_BYTE(X, i) != 0) {
				return HAL_NOT_ENOUGH_MEMORY;
			}
		}
	}

	for (i = 0; i < bytes_to_copy; i++) {
		p[bytes_to_copy - i - 1] = GET_BYTE(X, i);
	}

	return HAL_SUCCESS;
}

static int hal_asn1_write_tag(unsigned char **p, unsigned char *start, unsigned char tag)
{
	if (*p - start < 1) {
		return -HAL_INVALID_ARGS;
	}

	*--(*p) = tag;

	return 1;
}

static int hal_asn1_write_len(unsigned char **p, unsigned char *start, uint32_t len)
{
	if (len < 0x80) {
		if (*p - start < 1) {
			return -HAL_INVALID_ARGS;
		}
		*--(*p) = (unsigned char)len;
		return 1;
	}

	if (len <= 0xFF) {
		if (*p - start < 2) {
			return -HAL_INVALID_ARGS;
		}
		*--(*p) = (unsigned char)len;
		*--(*p) = 0x81;
		return 2 ;
	}

	if (len <= 0xFFFF) {
		if (*p - start < 3) {
			return -HAL_INVALID_ARGS;
		}
		*--(*p) = (len) & 0xFF;
		*--(*p) = (len >>  8) & 0xFF;
		*--(*p) = 0x82;
		return 3;
	}

	if (len <= 0xFFFFFF) {
		if (*p - start < 4) {
			return -HAL_INVALID_ARGS;
		}

		*--(*p) = (len) & 0xFF;
		*--(*p) = (len >> 8) & 0xFF;
		*--(*p) = (len >> 16) & 0xFF;
		*--(*p) = 0x83;
		return 4;
	}

	if (len <= 0xFFFFFFFF) {
		if (*p - start < 5) {
			return -HAL_INVALID_ARGS;
		}

		*--(*p) = (len) & 0xFF;
		*--(*p) = (len >> 8) & 0xFF;
		*--(*p) = (len >> 16) & 0xFF;
		*--(*p) = (len >> 24) & 0xFF;
		*--(*p) = 0x84;
		return 5;
	}

	return -HAL_INVALID_ARGS;
}

static int hal_asn1_write_mpi(unsigned char **p, unsigned char *start, const hal_mpi *X)
{
	int ret;
	uint32_t len = 0;

	len = hal_mpi_size(X);

	if (*p < start || (uint32_t)(*p - start) < len) {
		return -HAL_INVALID_ARGS;
	}

	(*p) -= len;
	HAL_MPI_CHK(hal_mpi_write_binary(X, *p, len));

	if (X->s == 1 && **p & 0x80) {
		if (*p - start < 1) {
			return HAL_NOT_ENOUGH_MEMORY;
		}
		*--(*p) = 0x00;
		len += 1;
	}

	HAL_ASN1_CHK_ADD(len, hal_asn1_write_len(p, start, len));
	HAL_ASN1_CHK_ADD(len, hal_asn1_write_tag(p, start, 0x02));

	ret = (int)len;

cleanup:
	return ret;
}

static int hal_ecdsa_signature_to_asn1(const hal_mpi *r, const hal_mpi *s, unsigned char *sig, uint32_t *slen)
{
	int ret;
	unsigned char buf[HAL_MAX_ECDSA_LEN];
	unsigned char *p = buf + sizeof(buf);
	uint32_t len = 0;

	HAL_ASN1_CHK_ADD(len, hal_asn1_write_mpi(&p, buf, s));
	HAL_ASN1_CHK_ADD(len, hal_asn1_write_mpi(&p, buf, r));
	HAL_ASN1_CHK_ADD(len, hal_asn1_write_len(&p, buf, len));
	HAL_ASN1_CHK_ADD(len, hal_asn1_write_tag(&p, buf, 0x20|0x10));

	memcpy(sig, p, len);
	*slen = len;

	return HAL_SUCCESS;
}

static int hal_asn1_get_mpi(unsigned char **p, const unsigned char *end, hal_mpi *X)
{
	int ret;
	uint32_t len;

	if ((ret = hal_asn1_get_tag(p, end, &len, 0x02)) != 0) {
		return ret;
	}

	ret = hal_mpi_read_binary(X, *p, len);

	*p += len;

	return ret;
}

/**
 * Common
 */
int sss_hal_init(hal_init_param *params)
{
	HWRAP_ENTER;
	return HAL_SUCCESS;
}

int sss_hal_deinit(void)
{
	HWRAP_ENTER;
	return HAL_SUCCESS;
}

int sss_hal_free_data(hal_data *data)
{
	HWRAP_ENTER;
	if (data) {
		if (data->data) {
			free(data->data);
		}
		if (data->priv) {
			free(data->priv);
		}
	}
	return HAL_SUCCESS;

}

int sss_hal_get_status(void)
{
	HWRAP_ENTER;
	return isp_get_status();
}

/**
 * Key Manager
 */
int sss_hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
	HWRAP_ENTER;
	if (prikey != NULL) {
		return HAL_NOT_SUPPORTED;
	}

	unsigned int key_type;
	switch (mode) {
	case HAL_KEY_AES_128:
	case HAL_KEY_AES_192:
	case HAL_KEY_AES_256:
		key_type = SECURE_STORAGE_TYPE_KEY_AES;
		break;
	case HAL_KEY_RSA_1024:
	case HAL_KEY_RSA_2048:
	case HAL_KEY_RSA_3072:
	case HAL_KEY_RSA_4096:
		key_type = SECURE_STORAGE_TYPE_KEY_RSA;
		break;
	case HAL_KEY_ECC_BRAINPOOL_P256R1:
	case HAL_KEY_ECC_BRAINPOOL_P384R1:
	case HAL_KEY_ECC_BRAINPOOL_P512R1:
	case HAL_KEY_ECC_SEC_P192R1:
	case HAL_KEY_ECC_SEC_P224R1:
	case HAL_KEY_ECC_SEC_P256R1:
	case HAL_KEY_ECC_SEC_P384R1:
	case HAL_KEY_ECC_SEC_P512R1:
		key_type = SECURE_STORAGE_TYPE_KEY_ECC;
		break;
	case HAL_KEY_HMAC_MD5:
	case HAL_KEY_HMAC_SHA1:
	case HAL_KEY_HMAC_SHA224:
	case HAL_KEY_HMAC_SHA256:
	case HAL_KEY_HMAC_SHA384:
	case HAL_KEY_HMAC_SHA512:
		key_type = SECURE_STORAGE_TYPE_KEY_HMAC;
		break;
	case HAL_KEY_DH_1024:
	case HAL_KEY_DH_2048:
	case HAL_KEY_DH_4096:
		key_type = SECURE_STORAGE_TYPE_KEY_DH;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	ISP_CHECKBUSY();
	int ret = isp_set_securekey(key->data, key->data_len, key_type, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;;
	}

	return HAL_SUCCESS;
}

int sss_hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
	HWRAP_ENTER;
	uint32_t ret;
	if ((mode <= HAL_KEY_ECC_SEC_P512R1) && (mode >= HAL_KEY_ECC_BRAINPOOL_P256R1)) {
		struct sECC_KEY ecc_key;
		unsigned int object_id = OID_ECC_P256;
		unsigned char r[HAL_MAX_ECP_KEY_SIZE_ALT];
		unsigned char s[HAL_MAX_ECP_KEY_SIZE_ALT];
		ecc_key.publickey_x = r;
		ecc_key.x_byte_len = HAL_MAX_ECP_KEY_SIZE_ALT;
		ecc_key.publickey_y = s;
		ecc_key.y_byte_len = HAL_MAX_ECP_KEY_SIZE_ALT;

		switch (mode) {
		case HAL_KEY_ECC_SEC_P192R1:
			object_id = OID_ECC_P192;
			break;
		case HAL_KEY_ECC_SEC_P224R1:
			object_id = OID_ECC_P224;
			break;
		case HAL_KEY_ECC_SEC_P256R1:
			object_id = OID_ECC_P256;
			break;
		case HAL_KEY_ECC_SEC_P384R1:
			object_id = OID_ECC_P384;
			break;
		case HAL_KEY_ECC_SEC_P512R1:
			object_id = OID_ECC_P521;
			break;
		case HAL_KEY_ECC_BRAINPOOL_P256R1:
			object_id = OID_ECC_BP256;
			break;
		default:
			return HAL_NOT_SUPPORTED;
		}

		ecc_key.curve = object_id;

		ISP_CHECKBUSY();
		ret = isp_ecdsa_get_publickey_securekey(&ecc_key, key_idx, object_id);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}

		//data: publickey_x, priv: publickey_y
		HAL_COPY_DATA(key, ecc_key.publickey_x, ecc_key.x_byte_len);
		HAL_COPY_PRIV_DATA(key, ecc_key.publickey_y, ecc_key.y_byte_len);

	} else if (key_idx == FACTORYKEY_ARTIK_DEVICE) {
		ISP_CHECKBUSY();
		if (!key->data) {
			return HAL_INVALID_ARGS;
		}
		key->data_len = HAL_MAX_BUF_SIZE;
		ret = isp_get_factorykey_data(key->data, &key->data_len, key_idx);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		return HAL_NOT_SUPPORTED;
	}

	return HAL_SUCCESS;
}

static int _hal_get_key_type(hal_key_type mode, unsigned int *key_type)
{
	switch (mode) {
	case HAL_KEY_AES_128:
	case HAL_KEY_AES_192:
	case HAL_KEY_AES_256:
		*key_type = SECURE_STORAGE_TYPE_KEY_AES;
		break;
	case HAL_KEY_RSA_1024:
	case HAL_KEY_RSA_2048:
	case HAL_KEY_RSA_3072:
		*key_type = SECURE_STORAGE_TYPE_KEY_RSA;
		break;
	case HAL_KEY_ECC_BRAINPOOL_P256R1:
	case HAL_KEY_ECC_BRAINPOOL_P384R1:
	case HAL_KEY_ECC_BRAINPOOL_P512R1:
	case HAL_KEY_ECC_SEC_P192R1:
	case HAL_KEY_ECC_SEC_P224R1:
	case HAL_KEY_ECC_SEC_P256R1:
	case HAL_KEY_ECC_SEC_P384R1:
	case HAL_KEY_ECC_SEC_P512R1:
		*key_type = SECURE_STORAGE_TYPE_KEY_ECC;
		break;
	case HAL_KEY_HMAC_MD5:
	case HAL_KEY_HMAC_SHA1:
	case HAL_KEY_HMAC_SHA224:
	case HAL_KEY_HMAC_SHA256:
	case HAL_KEY_HMAC_SHA384:
	case HAL_KEY_HMAC_SHA512:
		*key_type = SECURE_STORAGE_TYPE_KEY_HMAC;
		break;
	case HAL_KEY_DH_1024:
	case HAL_KEY_DH_2048:
	case HAL_KEY_DH_4096:
		*key_type = SECURE_STORAGE_TYPE_KEY_DH;
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	return HAL_SUCCESS;
}

int sss_hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
	HWRAP_ENTER;
	unsigned int key_type;
	uint32_t ret;

	ret = _hal_get_key_type(mode, &key_type);
	if (ret != HAL_SUCCESS) {
		return ret;
	}
	ret = isp_remove_key(key_type, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int sss_hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
	HWRAP_ENTER;
	uint32_t ret;
	ISP_CHECKBUSY();
	switch (mode) {
	case HAL_KEY_AES_128:
		ret = isp_aes_generate_key_securekey(16, key_idx);
		break;
	case HAL_KEY_AES_192:
		ret = isp_aes_generate_key_securekey(24, key_idx);
		break;
	case HAL_KEY_AES_256:
		ret = isp_aes_generate_key_securekey(32, key_idx);
		break;
	case HAL_KEY_RSA_1024:
		ret = isp_rsa_generate_key_securekey(key_idx, 0xB1, 0);
		break;
	case HAL_KEY_RSA_2048:
		ret = isp_rsa_generate_key_securekey(key_idx, 0xB2, 0);
		break;
	case HAL_KEY_RSA_3072:
		ret = isp_rsa_generate_key_securekey(key_idx, 0xB3, 0);
		break;
	case HAL_KEY_RSA_4096:
		return HAL_NOT_SUPPORTED;
	case HAL_KEY_ECC_BRAINPOOL_P256R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x53);
		break;
	case HAL_KEY_ECC_BRAINPOOL_P384R1:
		return HAL_NOT_SUPPORTED;
		break;
	case HAL_KEY_ECC_BRAINPOOL_P512R1:
		return HAL_NOT_SUPPORTED;
		break;
	case HAL_KEY_ECC_SEC_P192R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x11);
		break;
	case HAL_KEY_ECC_SEC_P224R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x12);
		break;
	case HAL_KEY_ECC_SEC_P256R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x13);
		break;
	case HAL_KEY_ECC_SEC_P384R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x14);
		break;
	case HAL_KEY_ECC_SEC_P512R1:
		ret = isp_ecdsa_generate_key_securekey(key_idx, 0x15);
		break;
	case HAL_KEY_HMAC_MD5:
	case HAL_KEY_HMAC_SHA1:
		ret = isp_hmac_generate_key_securekey(64, key_idx);
		break;
	case HAL_KEY_HMAC_SHA224:
		ret = isp_hmac_generate_key_securekey(64, key_idx);
		break;
	case HAL_KEY_HMAC_SHA256:
		ret = isp_hmac_generate_key_securekey(128, key_idx);
		break;
	case HAL_KEY_HMAC_SHA384:
		ret = isp_hmac_generate_key_securekey(128, key_idx);
		break;
	case HAL_KEY_HMAC_SHA512:
		ret = isp_hmac_generate_key_securekey(64, key_idx);
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

/**
 * Authenticate
 */

int sss_hal_generate_random(uint32_t len, hal_data *random)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned int inbuf[HAL_MAX_RANDOM_SIZE];

	if (len > HAL_MAX_RANDOM_SIZE) {
		return HAL_INVALID_ARGS;
	}

	if (len & 0x3) {
		len = len + 4 - (len & 0x3);
	}

	ISP_CHECKBUSY();
	ret = isp_generate_random(inbuf, len / 4);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(random, inbuf, len);

	return HAL_SUCCESS;
}

int sss_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sHASH_MSG h_param;
	unsigned int object_id;
	int hash_len = 64;


	switch (mode) {
	case HAL_HASH_MD5:
		return HAL_NOT_SUPPORTED;
	case HAL_HASH_SHA1:
		object_id = SHA1_160;
		break;
	case HAL_HASH_SHA224:
		return HAL_NOT_SUPPORTED;
	case HAL_HASH_SHA256:
		object_id = SHA2_256;
		break;
	case HAL_HASH_SHA384:
		object_id = SHA2_384;
		hash_len = 128;
		break;
	case HAL_HASH_SHA512:
		object_id = SHA2_512;
		hash_len = 128;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	unsigned char output[hash_len];
	memset(&h_param, 0, sizeof(struct sHASH_MSG));

	h_param.addr_low = (unsigned int)input->data;
	h_param.msg_byte_len = input->data_len;

	ret = isp_hash(output, &h_param, object_id);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(hash, output, hash_len);

	return HAL_SUCCESS;
}

int sss_hal_get_hmac(hal_hmac_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac)
{
	HWRAP_ENTER;

	return HAL_NOT_SUPPORTED;
}

int sss_hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned int padding = 0;
	struct sRSA_SIGN rsa_sign;
	unsigned char sig_out[1024];

	memset(&rsa_sign, 0, sizeof(struct sRSA_SIGN));

	if (mode.rsa_a == HAL_RSASSA_PKCS1_PSS_MGF1) {
		padding = 1;
	}

	switch (mode.hash_t) {
	case HAL_HASH_SHA1:
		rsa_sign.alg_type = SHA1_160 | (padding ^ 0x1);	/* 0x110X */
		break;
	case HAL_HASH_SHA256:
		rsa_sign.alg_type = SHA2_256 | (padding ^ 0x1);	/* 0x230X */
		break;
	case HAL_HASH_SHA384:
		rsa_sign.alg_type = SHA2_384 | (padding ^ 0x1);	/* 0x240X */
		break;
	case HAL_HASH_SHA512:
		rsa_sign.alg_type = SHA2_512 | (padding ^ 0x1);	/* 0x250X */
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	rsa_sign.signature = sig_out;
	rsa_sign.signature_byte_len = 1024;
	rsa_sign.salt_byte_len = mode.salt_byte_len;

	ISP_CHECKBUSY();
	ret = isp_rsa_sign_md_securekey(&rsa_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	if (rsa_sign.signature == NULL) {
		return HAL_FAIL;
	}

	HAL_COPY_DATA(sign, rsa_sign.signature, rsa_sign.signature_byte_len);

	return HAL_SUCCESS;
}

int sss_hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned int padding = 0;
	struct sRSA_SIGN rsa_sign;

	memset(&rsa_sign, 0, sizeof(struct sRSA_SIGN));

	rsa_sign.signature = (unsigned char *)sign->data;
	rsa_sign.signature_byte_len = sign->data_len;
	rsa_sign.salt_byte_len = mode.salt_byte_len;

	if (mode.rsa_a == HAL_RSASSA_PKCS1_PSS_MGF1) {
		padding = 1;
	}

	switch (mode.hash_t) {
	case HAL_HASH_SHA1:
		rsa_sign.alg_type = SHA1_160 | (padding ^ 0x1);	/* 0x110X */
		break;
	case HAL_HASH_SHA256:
		rsa_sign.alg_type = SHA2_256 | (padding ^ 0x1);	/* 0x230X */
		break;
	case HAL_HASH_SHA384:
		rsa_sign.alg_type = SHA2_384 | (padding ^ 0x1);	/* 0x240X */
		break;
	case HAL_HASH_SHA512:
		rsa_sign.alg_type = SHA2_512 | (padding ^ 0x1);	/* 0x250X */
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	ISP_CHECKBUSY();
	ret = isp_rsa_verify_md_securekey(&rsa_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int sss_hal_ecdsa_sign_md(hal_ecdsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char s_buf[HAL_MAX_ECP_KEY_SIZE_ALT];
	unsigned char r_buf[HAL_MAX_ECP_KEY_SIZE_ALT];
	unsigned char sign_buf[HAL_MAX_BUF_SIZE];

	struct sECC_SIGN ecc_sign;
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	ecc_sign.s = s_buf;
	ecc_sign.s_byte_len = HAL_MAX_ECP_KEY_SIZE_ALT;
	ecc_sign.r = r_buf;
	ecc_sign.r_byte_len = HAL_MAX_ECP_KEY_SIZE_ALT;

	switch (mode.hash_t) {
	case HAL_HASH_MD5:
		ecc_sign.sign_type |= OID_SHA1_160;
		break;
	case HAL_HASH_SHA256:
		ecc_sign.sign_type |= OID_SHA2_256;
		break;
	case HAL_HASH_SHA384:
		ecc_sign.sign_type |= OID_SHA2_384;
		break;
	case HAL_HASH_SHA512:
		ecc_sign.sign_type |= OID_SHA2_512;
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	switch (mode.curve) {
	case HAL_ECDSA_SEC_P256R1:
		ecc_sign.sign_type |= OID_ECC_P256;
		break;
	case HAL_ECDSA_SEC_P384R1:
		ecc_sign.sign_type |= OID_ECC_P384;
		break;
	case HAL_ECDSA_SEC_P512R1:
		ecc_sign.sign_type |= OID_ECC_P521;
		break;
	case HAL_ECDSA_BRAINPOOL_P256R1:
		ecc_sign.sign_type |= OID_ECC_BP256;
		break;
	default:
		return HAL_INVALID_ARGS;
	}

	ISP_CHECKBUSY();

	ret = isp_ecdsa_sign_md_securekey(&ecc_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	/* Signature encoding */
	hal_mpi r;
	hal_mpi s;
	hal_mpi_init(&r);
	hal_mpi_init(&s);

	ret = hal_mpi_read_binary(&r, ecc_sign.r, ecc_sign.r_byte_len);
	if (ret != 0) {
		goto cleanup;
	}

	ret = hal_mpi_read_binary(&s, ecc_sign.s, ecc_sign.s_byte_len);
	if (ret != 0) {
		goto cleanup;
	}

	hal_ecdsa_signature_to_asn1(&r, &s, sign_buf, &sign->data_len);

	hal_mpi_free(&r);
	hal_mpi_free(&s);

	HAL_COPY_DATA(sign, sign_buf, sign->data_len);

	return HAL_SUCCESS;

cleanup:
	hal_mpi_free(&r);
	hal_mpi_free(&s);

	return HAL_FAIL;
}

int sss_hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char *p = (unsigned char *)sign->data;
	const unsigned char *end = sign->data + sign->data_len;
	uint32_t len;
	hal_mpi r, s;
	struct sECC_SIGN ecc_sign;
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	hal_mpi_init(&r);
	hal_mpi_init(&s);

	if ((ret = hal_asn1_get_tag(&p, end, &len, 0x20 | 0x10)) != 0) {
		ret = HAL_INVALID_ARGS;
		goto cleanup;
	}

	if (p + len != end) {
		ret = HAL_INVALID_ARGS;
		goto cleanup;
	}

	if ((ret = hal_asn1_get_mpi(&p, end, &r)) != 0 || (ret = hal_asn1_get_mpi(&p, end, &s)) != 0) {
		ret = HAL_INVALID_ARGS;
		goto cleanup;
	}

	ecc_sign.r_byte_len = hal_mpi_size(&r);
	ecc_sign.s_byte_len = hal_mpi_size(&s);

	ecc_sign.r = (unsigned char *)malloc(ecc_sign.r_byte_len);
	if (ecc_sign.r == NULL) {
		ret = HAL_ALLOC_FAIL;
		goto cleanup;
	}
	ecc_sign.s = (unsigned char *)malloc(ecc_sign.s_byte_len);
	if (ecc_sign.s == NULL) {
		ret = HAL_ALLOC_FAIL;
		goto cleanup;
	}

	HAL_MPI_CHK(hal_mpi_write_binary(&r, ecc_sign.r, ecc_sign.r_byte_len));
	HAL_MPI_CHK(hal_mpi_write_binary(&s, ecc_sign.s, ecc_sign.s_byte_len));

	switch (mode.hash_t) {
	case HAL_HASH_MD5:
		ecc_sign.sign_type |= OID_SHA1_160;
		break;
	case HAL_HASH_SHA256:
		ecc_sign.sign_type |= OID_SHA2_256;
		break;
	case HAL_HASH_SHA384:
		ecc_sign.sign_type |= OID_SHA2_384;
		break;
	case HAL_HASH_SHA512:
		ecc_sign.sign_type |= OID_SHA2_512;
		break;
	default:
		ret = HAL_INVALID_ARGS;
		goto cleanup;
	}

	switch (mode.curve) {
	case HAL_ECDSA_SEC_P256R1:
		ecc_sign.sign_type |= OID_ECC_P256;
		break;
	case HAL_ECDSA_SEC_P384R1:
		ecc_sign.sign_type |= OID_ECC_P384;
		break;
	case HAL_ECDSA_SEC_P512R1:
		ecc_sign.sign_type |= OID_ECC_P521;
		break;
	case HAL_ECDSA_BRAINPOOL_P256R1:
		ecc_sign.sign_type |= OID_ECC_BP256;
		break;
	default:
		ret = HAL_INVALID_ARGS;
		goto cleanup;
	}

	ISP_CHECKBUSY();
	ret = isp_ecdsa_verify_md_securekey(&ecc_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		ret = HAL_FAIL;
		goto cleanup;
	}

	ret = HAL_SUCCESS;

cleanup:
	hal_mpi_free(&r);
	hal_mpi_free(&s);

	if (ecc_sign.r) {
		free(ecc_sign.r);
	}
	if (ecc_sign.s) {
		free(ecc_sign.s);
	}

	return ret;
}

int sss_hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sDH_PARAM d_param;
	unsigned int pubkey_len;
	memset(&d_param, 0, sizeof(struct sDH_PARAM));
	switch (dh_param->mode) {
	case HAL_DH_1024:
		d_param.object_id = OID_DH_1024;
		pubkey_len = 128;
		break;
	case HAL_DH_2048:
		d_param.object_id = OID_DH_2048;
		pubkey_len = 256;
		break;
	default:
		return HAL_INVALID_ARGS;
	}
	d_param.modules_p_byte_len = dh_param->P->data_len;
	d_param.modules_p = dh_param->P->data;
	d_param.generator_g_byte_len = dh_param->G->data_len;;
	d_param.generator_g = dh_param->G->data;
	//set output pubkey
	d_param.publickey = (unsigned char *)malloc(pubkey_len);
	if (d_param.publickey == NULL) {
		return HAL_NOT_ENOUGH_MEMORY;
	}
	memset(d_param.publickey, 0, pubkey_len);
	d_param.publickey_byte_len = pubkey_len;

	ISP_CHECKBUSY();
	ret = isp_dh_generate_keypair_userparam_securestorage(&d_param, dh_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	if (!dh_param->pubkey->data) {
		free(d_param.publickey);
		return HAL_INVALID_ARGS;
	}
	memcpy(dh_param->pubkey->data, d_param.publickey, d_param.publickey_byte_len);
	dh_param->pubkey->data_len = d_param.publickey_byte_len;

	free(d_param.publickey);

	return HAL_SUCCESS;
}

int sss_hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sDH_PARAM d_param;
	memset(&d_param, 0, sizeof(struct sDH_PARAM));
	switch (dh_param->mode) {
	case HAL_DH_1024:
		d_param.object_id = OID_DH_1024;
		break;
	case HAL_DH_2048:
		d_param.object_id = OID_DH_2048;
		break;
	default:
		return HAL_INVALID_ARGS;
	}
	d_param.modules_p_byte_len = dh_param->P->data_len;
	d_param.modules_p = dh_param->P->data;
	d_param.generator_g_byte_len = dh_param->G->data_len;;
	d_param.generator_g = dh_param->G->data;
	d_param.publickey = dh_param->pubkey->data;
	d_param.publickey_byte_len = dh_param->pubkey->data_len;

	unsigned char output[HAL_MAX_SHARED_SECRET_LEN];

	ISP_CHECKBUSY();
	ret = isp_dh_compute_shared_secret_securekey(output, &shared_secret->data_len, d_param, dh_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(shared_secret, output, shared_secret->data_len);

	return HAL_SUCCESS;
}

int sss_hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sECC_KEY ecc_pub;
	memset(&ecc_pub, 0, sizeof(struct sECC_KEY));

	switch (ecdh_param->curve) {
	case HAL_ECDSA_BRAINPOOL_P256R1:
		ecc_pub.curve |= OID_ECC_BP256;
		break;
	case HAL_ECDSA_BRAINPOOL_P384R1:
	case HAL_ECDSA_BRAINPOOL_P512R1:
		return HAL_NOT_SUPPORTED;
	case HAL_ECDSA_SEC_P192R1:
		ecc_pub.curve |= OID_ECC_P192;
		break;
	case HAL_ECDSA_SEC_P224R1:
		ecc_pub.curve |= OID_ECC_P224;
		break;
	case HAL_ECDSA_SEC_P256R1:
		ecc_pub.curve |= OID_ECC_P256;
		break;
	case HAL_ECDSA_SEC_P384R1:
		ecc_pub.curve |= OID_ECC_P384;
		break;
	case HAL_ECDSA_SEC_P512R1:
		ecc_pub.curve |= OID_ECC_P521;
		break;
	case HAL_ECDSA_UNKNOWN:
		return HAL_INVALID_ARGS;
	}
	ecc_pub.publickey_x = ecdh_param->pubkey_x->data;
	ecc_pub.x_byte_len = ecdh_param->pubkey_x->data_len;
	ecc_pub.publickey_y = ecdh_param->pubkey_y->data;
	ecc_pub.y_byte_len = ecdh_param->pubkey_y->data_len;

	unsigned char output[HAL_MAX_SHARED_SECRET_LEN];

	ISP_CHECKBUSY();
	ret = isp_compute_ecdh_securekey(output, &shared_secret->data_len, ecc_pub, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(shared_secret, output, shared_secret->data_len);

	return HAL_SUCCESS;
}

int sss_hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char *cert = cert_in->data;
	unsigned int cert_len = cert_in->data_len;

	ISP_CHECKBUSY();
	ret = isp_write_cert(cert, cert_len, cert_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int sss_hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char buf[HAL_MAX_BUF_SIZE];
	unsigned int buf_len;

	ISP_CHECKBUSY();
	if (cert_idx == FACTORYKEY_ARTIK_CERT) {
		ret = isp_get_factorykey_data(buf, &buf_len, cert_idx);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		ret = isp_read_cert(buf, &buf_len, cert_idx);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	}

	HAL_COPY_DATA(cert_out, buf, buf_len);

	return HAL_SUCCESS;
}

int sss_hal_remove_certificate(uint32_t cert_idx)
{
	HWRAP_ENTER;
	return HAL_NOT_SUPPORTED;
}

int sss_hal_get_factory_key(uint32_t key_idx, hal_data *key)
{
	HWRAP_ENTER;
	uint32_t ret;
	if (key_idx == FACTORYKEY_ARTIK_DEVICE) {
		ISP_CHECKBUSY();
		ret = isp_get_factorykey_data(key->data, &key->data_len, key_idx);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		return HAL_NOT_SUPPORTED;
	}

	return HAL_SUCCESS;
}

int sss_hal_get_factory_cert(uint32_t cert_idx, hal_data *cert)
{
	HWRAP_ENTER;
	uint32_t ret;
	if (cert_idx == FACTORYKEY_ARTIK_CERT) {
		ISP_CHECKBUSY();
		ret = isp_get_factorykey_data(cert->data, &cert->data_len, cert_idx);
		if (ret != 0) {
			isp_clear(0);
			sedbg("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		return HAL_NOT_SUPPORTED;
	}

	return HAL_SUCCESS;
}

int sss_hal_get_factory_data(uint32_t data_idx, hal_data *data)
{
	HWRAP_ENTER;
	return HAL_NOT_SUPPORTED;
}


/**
 * Crypto
 */

int sss_hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sAES_PARAM param;
	unsigned char aes_output[HAL_MAX_BUF_SIZE];

	memset(&param, 0, sizeof(struct sAES_PARAM));

	param.pu8Plaintext = dec_data->data;
	param.u32Plaintext_byte_len = dec_data->data_len;
	param.pu8Ciphertext = aes_output;
	param.u32Ciphertext_byte_len = HAL_MAX_BUF_SIZE;
	switch (aes_param->mode) {
	case HAL_AES_ECB_NOPAD:
	case HAL_AES_ECB_ISO9797_M1:
	case HAL_AES_ECB_ISO9797_M2:
	case HAL_AES_ECB_PKCS5:
	case HAL_AES_ECB_PKCS7:
		param.u32Mode = AES_ECB_MODE;
		break;
	case HAL_AES_CBC_NOPAD:
	case HAL_AES_CBC_ISO9797_M1:
	case HAL_AES_CBC_ISO9797_M2:
	case HAL_AES_CBC_PKCS5:
	case HAL_AES_CBC_PKCS7:
		param.u32Mode = AES_CBC_MODE;
		break;
	case HAL_AES_CTR:
		param.u32Mode = AES_CTR_MODE;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	ISP_CHECKBUSY();
	ret = isp_aes_encrypt_securekey(&param, key_idx);;
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(enc_data, aes_output, param.u32Ciphertext_byte_len);

	return HAL_SUCCESS;
}

int sss_hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data)
{
	HWRAP_ENTER;
	uint32_t ret;
	struct sAES_PARAM param;
	unsigned char aes_output[HAL_MAX_BUF_SIZE];

	memset(&param, 0, sizeof(struct sAES_PARAM));

	param.pu8Plaintext = enc_data->data;
	param.u32Plaintext_byte_len = enc_data->data_len;
	param.pu8Ciphertext = aes_output;
	param.u32Ciphertext_byte_len = HAL_MAX_BUF_SIZE;
	switch (aes_param->mode) {
	case HAL_AES_ECB_NOPAD:
	case HAL_AES_ECB_ISO9797_M1:
	case HAL_AES_ECB_ISO9797_M2:
	case HAL_AES_ECB_PKCS5:
	case HAL_AES_ECB_PKCS7:
		param.u32Mode = AES_ECB_MODE;
		break;
	case HAL_AES_CBC_NOPAD:
	case HAL_AES_CBC_ISO9797_M1:
	case HAL_AES_CBC_ISO9797_M2:
	case HAL_AES_CBC_PKCS5:
	case HAL_AES_CBC_PKCS7:
		param.u32Mode = AES_CBC_MODE;
		break;
	case HAL_AES_CTR:
		param.u32Mode = AES_CTR_MODE;
		break;
	default:
		return HAL_NOT_SUPPORTED;
	}

	ISP_CHECKBUSY();
	ret = isp_aes_decrypt_securekey(&param, key_idx);;
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(dec_data, aes_output, param.u32Ciphertext_byte_len);

	return HAL_SUCCESS;
}

int sss_hal_rsa_encrypt(hal_data *dec_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *enc_data)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char output[HAL_MAX_BUF_SIZE];

	ISP_CHECKBUSY();
	ret = isp_rsa_encrypt_securekey(output, &enc_data->data_len, dec_data->data, dec_data->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(enc_data, output, enc_data->data_len);

	return HAL_SUCCESS;
}

int sss_hal_rsa_decrypt(hal_data *enc_data, hal_rsa_mode *rsa_mode, uint32_t key_idx, hal_data *dec_data)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char output[HAL_MAX_BUF_SIZE];
	ISP_CHECKBUSY();
	ret = isp_rsa_decrypt_securekey(output, &dec_data->data_len, enc_data->data, enc_data->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(dec_data, output, dec_data->data_len);

	return HAL_SUCCESS;
}

/**
 * Secure Storage
 */

int sss_hal_write_storage(uint32_t ss_idx, hal_data *data)
{
	HWRAP_ENTER;
	uint32_t ret;

	ISP_CHECKBUSY();
	ret = isp_write_storage(data->data, data->data_len, ss_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int sss_hal_read_storage(uint32_t ss_idx, hal_data *data)
{
	HWRAP_ENTER;
	uint32_t ret;
	unsigned char output[HAL_MAX_BUF_SIZE];

	ISP_CHECKBUSY();
	ret = isp_read_storage(output, &data->data_len, ss_idx);
	if (ret != 0) {
		isp_clear(0);
		sedbg("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	HAL_COPY_DATA(data, output, data->data_len);

	return HAL_SUCCESS;
}

int sss_hal_delete_storage(uint32_t ss_idx)
{
	HWRAP_ENTER;
	return HAL_NOT_SUPPORTED;
}

static struct sec_ops_s g_sss_ops = {
	sss_hal_init,
	sss_hal_deinit,
	sss_hal_free_data,
	sss_hal_get_status,
	sss_hal_set_key,
	sss_hal_get_key,
	sss_hal_remove_key,
	sss_hal_generate_key,
	sss_hal_generate_random,
	sss_hal_get_hash,
	sss_hal_get_hmac,
	sss_hal_rsa_sign_md,
	sss_hal_rsa_verify_md,
	sss_hal_ecdsa_sign_md,
	sss_hal_ecdsa_verify_md,
	sss_hal_dh_generate_param,
	sss_hal_dh_compute_shared_secret,
	sss_hal_ecdh_compute_shared_secret,
	sss_hal_set_certificate,
	sss_hal_get_certificate,
	sss_hal_remove_certificate,
	sss_hal_get_factory_key,
	sss_hal_get_factory_cert,
	sss_hal_get_factory_data,
	sss_hal_aes_encrypt,
	sss_hal_aes_decrypt,
	sss_hal_rsa_encrypt,
	sss_hal_rsa_decrypt,
	sss_hal_write_storage,
	sss_hal_read_storage,
	sss_hal_delete_storage,
};

static struct sec_lowerhalf_s g_sss_lower = {&g_sss_ops, NULL};

#define SECLINK_PATH "/dev/seclink"

int se_initialize(void)
{
	int res = se_register(SECLINK_PATH, &g_sss_lower);
	if (res != 0) {
		return -1;
	}

	return 0;
}

/*
 * It should not be used to general case.
 * It's for testing HAL APIs in user space.
 * Furthermore it wouldn't be used if binary separation is enabled.
 * Because user space couldn't access kernel space variable directly.
 * So it is protected by CONFIG after binary separation is applied.
 */
struct sec_lowerhalf_s *se_get_device(void)
{
	return &g_sss_lower;
}
