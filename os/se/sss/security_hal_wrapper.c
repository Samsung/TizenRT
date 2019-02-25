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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <security/hal/security_hal.h>
#include "isp_custom.h"

#define ISP_CHECKBUSY() while (isp_get_status()) {}

#define MBEDTLS_MAX_RANDOM_SIZE 256
#define MBEDTLS_MAX_ECP_KEY_SIZE_ALT 68
#define MAX_BUF_SIZE 2048

#define AES_ECB_MODE		(0x0008)
#define AES_CBC_MODE		(0x0108)
#define AES_CTR_MODE		(0x0208)
#define AES_XTS_MODE		(0x0308)
#define AES_CCM_MODE		(0x1008)
#define AES_GCM_MODE		(0x1108)
#define AES_KW_MODE			(0x1208)

/**
 * Common
 */
int hal_init(void)
{
	return HAL_NOT_SUPPORTED;
}

int hal_deinit(void)
{
	return HAL_NOT_SUPPORTED;
}

int hal_free_data(hal_data *data)
{
	if(data) {
		if(data->data) {
			free(data->data);
		}
		if(data->priv) {
			free(data->priv);
		}
	}
	return HAL_SUCCESS;

}

int hal_get_status(void)
{
	return isp_get_status();
}

/**
 * Key Manager
 */
int hal_set_key(hal_key_type mode, uint32_t key_idx, hal_data *key, hal_data *prikey)
{
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
			key_type = SECURE_STORAGE_TYPE_KEY_DH;
			break;
		default:
			return HAL_NOT_SUPPORTED;
	}

	ISP_CHECKBUSY();
	int ret = isp_set_securekey(key->data, key->data_len, key_type, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;;
	}

	return HAL_SUCCESS;
}

int hal_get_key(hal_key_type mode, uint32_t key_idx, hal_data *key)
{
	uint32_t ret;
	if (key_idx == FACTORYKEY_ARTIK_DEVICE) {
		if (mode <= HAL_KEY_ECC_SEC_P512R1 && mode >= HAL_KEY_ECC_BRAINPOOL_P256R1 ) {
			struct sECC_KEY ecc_key;
			unsigned char r[68];
			unsigned char s[68];
			ecc_key.publickey_x = r;
			ecc_key.x_byte_len = sizeof(r);
			ecc_key.publickey_y = s;
			ecc_key.y_byte_len = sizeof(s);
			ISP_CHECKBUSY();
			ret = isp_ecdsa_get_publickey_securekey(&ecc_key, key_idx, OID_ECC_P256);
			if (ret != 0) {
				isp_clear(0);
				printf("ISP failed (%zu)\n", ret);
				return HAL_FAIL;
			}
			key->data = (unsigned char*)malloc(ecc_key.x_byte_len);
			memcpy(key->data, &ecc_key.publickey_x, ecc_key.x_byte_len);
			key->priv = (unsigned char*)malloc(ecc_key.y_byte_len);
			memcpy(key->priv, &ecc_key.publickey_y, ecc_key.y_byte_len);
		} else {
			ISP_CHECKBUSY();
			key->data = (unsigned char *)malloc(MAX_BUF_SIZE);
			key->data_len = MAX_BUF_SIZE;
			ret = isp_get_factorykey_data(key->data, &key->data_len, key_idx);
			if (ret != 0) {
				isp_clear(0);
				printf("ISP failed (%zu)\n", ret);
				return HAL_FAIL;
			}
		}
	} else {
		//ARTIK does not provide a function to get publickey from sss
		return HAL_NOT_SUPPORTED;
	}

	return HAL_SUCCESS;
}

static int hal_get_key_type(hal_key_type mode, unsigned int *key_type)
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
			*key_type = SECURE_STORAGE_TYPE_KEY_DH;
			break;
		default:
			return HAL_INVALID_ARGS;
	}
	return HAL_SUCCESS;
}

int hal_remove_key(hal_key_type mode, uint32_t key_idx)
{
	unsigned int key_type;
	uint32_t ret;

	ret = hal_get_key_type(mode, &key_type);
	if (ret != HAL_SUCCESS) {
		return ret;
	}
	ret = isp_remove_key(key_type, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	return HAL_SUCCESS;
}

int hal_generate_key(hal_key_type mode, uint32_t key_idx)
{
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
		case HAL_KEY_ECC_SEC_P256R1:
			ret = isp_ecdsa_generate_key_securekey(key_idx, 0x23);
			break;
		case HAL_KEY_ECC_SEC_P384R1:
			ret = isp_ecdsa_generate_key_securekey(key_idx, 0x24);
			break;
		case HAL_KEY_ECC_SEC_P512R1:
			ret = isp_ecdsa_generate_key_securekey(key_idx, 0x25);
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
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

/**
 * Authenticate
 */

int hal_generate_random(uint32_t len, hal_data *random)
{
	uint32_t ret;
	unsigned int inbuf[MBEDTLS_MAX_RANDOM_SIZE];

	if (len > MBEDTLS_MAX_RANDOM_SIZE) {
		return HAL_INVALID_ARGS;
	}

	if (len & 0x3) {
		len = len + 4 - (len & 0x3);
	}

	ISP_CHECKBUSY();
	ret = isp_generate_random(inbuf, len / 4);
	if(ret !=0 ) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	random->data = (unsigned char *)malloc(sizeof(inbuf));
	memcpy(random->data, inbuf, sizeof(inbuf));
	random->data_len = sizeof(inbuf);

	return HAL_SUCCESS;
}

int hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash)
{
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

	h_param.addr_low = (unsigned int) input->data;
	h_param.msg_byte_len = input->data_len;

	ret = isp_hash(output, &h_param, object_id);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	hash->data = (unsigned char *)malloc(hash_len);
	memcpy(hash->data, output, hash_len);
	hash->data_len = hash_len;
	return HAL_SUCCESS;
}

int hal_get_hmac(hal_hmac_type mode, hal_data *input, uint32_t key_idx, hal_data *hmac)
{
	/* isp_hmac_securekey returns ERROR_SSTORAGE_SFS_FREAD */
	/*
	   uint32_t ret;
	   struct sHMAC_MSG hmac_msg;
	   unsigned int object_id;
	   unsigned char output[MAX_BUF_SIZE];

	   switch (mode) {
	   case HAL_HMAC_MD5:
	   return HAL_NOT_SUPPORTED;
	   case HAL_HMAC_SHA1:
	   object_id = HMAC_SHA1_160;
	   break;
	   case HAL_HMAC_SHA224:
	   return HAL_NOT_SUPPORTED;
	   case HAL_HMAC_SHA256:
	   object_id = HMAC_SHA2_256;
	   break;
	   case HAL_HMAC_SHA384:
	   object_id = HMAC_SHA2_384;
	   break;
	   case HAL_HMAC_SHA512:
	   object_id = HMAC_SHA2_512;
	   break;
	   default:
	   return HAL_NOT_SUPPORTED;
	   }

	   memset(&hmac_msg, 0, sizeof(struct sHMAC_MSG));

	   hmac_msg.addr_low = (unsigned int) input->data;
	   hmac_msg.msg_byte_len = input->data_len;

	   ret = isp_hmac_securekey(output, &hmac_msg, object_id, key_idx);
	   if (ret != 0) {
	   isp_clear(0);
	   printf("ISP failed (%zu)\n", ret);
	   return HAL_FAIL;
	   }
	   hmac->data = (unsigned char *)malloc(input->data_len);
	   memcpy(hmac->data, output, input->data_len);
	   hmac->data_len = input->data_len;
	   return HAL_SUCCESS;
	   */
	return HAL_NOT_SUPPORTED;
}

int hal_rsa_sign_md(hal_rsa_mode mode, hal_data *hash, uint32_t key_idx, hal_data *sign)
{
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

	ISP_CHECKBUSY();
	ret = isp_rsa_sign_md_securekey(&rsa_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	if (rsa_sign.signature == NULL) {
		return HAL_FAIL;
	}

	sign->data = (unsigned char *)malloc(rsa_sign.signature_byte_len);
	memcpy(sign->data, rsa_sign.signature, rsa_sign.signature_byte_len);
	sign->data_len = rsa_sign.signature_byte_len;
	sign->priv = (unsigned int *)malloc(sizeof(unsigned int));
	memcpy(sign->priv, &rsa_sign.salt_byte_len, sizeof(unsigned int));

	return HAL_SUCCESS;
}

int hal_rsa_verify_md(hal_rsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	uint32_t ret;
	unsigned int padding = 0;
	struct sRSA_SIGN rsa_sign;

	memset(&rsa_sign, 0, sizeof(struct sRSA_SIGN));

	rsa_sign.signature = (unsigned char *)sign->data;
	rsa_sign.signature_byte_len = sign->data_len;
	rsa_sign.salt_byte_len = *((unsigned int *)sign->priv);

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
		printf("ISP failed (%zu)\n", ret);
		printf("################################################\n");
		printf("sig: %x(%d, salt %d), hash: %x(%d), padding %d\n", rsa_sign.signature, rsa_sign.signature_byte_len, rsa_sign.salt_byte_len,
				hash->data, hash->data_len, padding);
		printf("################################################\n");
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

static int hal_asn1_get_len(unsigned char **p, const unsigned char *end, unsigned int *len)
{
	if (( end - *p) < 1) {
		return HAL_INVALID_ARGS;
	}
	if ((**p & 0x80) == 0) {
		*len = *(*p)++;
	} else {
		switch ( **p & 0x7F )
		{
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
				*len = ((size_t)(*p)[1] << 8) | (*p)[2];
				(*p) += 3;
				break;
			case 3:
				if ((end - *p) < 4) {
					return HAL_INVALID_ARGS;
				}
				*len = ((size_t)(*p)[1] << 16) | ((size_t)(*p)[2] << 8) | (*p)[3];
				(*p) += 4;
				break;
			case 4:
				if ((end - *p) < 5) {
					return HAL_INVALID_ARGS;
				}
				*len = ((size_t)(*p)[1] << 24) | ((size_t)(*p)[2] << 16) | ((size_t)(*p)[3] << 8) | (*p)[4];
				(*p) += 5;
				break;
			default:
				return HAL_INVALID_ARGS;
		}
	}
	if (*len > (size_t)(end - *p)) {
		return HAL_INVALID_ARGS;
	}
	return HAL_SUCCESS;
}

static int hal_asn1_get_tag(unsigned char **p, const unsigned char *end, unsigned int *len, int tag)
{
	if ((end - *p) < 1) {
		return HAL_FAIL;
	}
	if (**p != tag) {
		return HAL_FAIL;
	}
	(*p)++;
	return hal_asn1_get_len(p, end, len);
}

int hal_ecdsa_sign_md(hal_data *hash, uint32_t key_idx, hal_ecdsa_mode *mode, hal_data *sign)
{
	uint32_t ret;
	unsigned char s_buf[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];
	unsigned char r_buf[MBEDTLS_MAX_ECP_KEY_SIZE_ALT];

	struct sECC_SIGN ecc_sign;
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	ecc_sign.s = s_buf;
	ecc_sign.s_byte_len = MBEDTLS_MAX_ECP_KEY_SIZE_ALT;
	ecc_sign.r = r_buf;
	ecc_sign.r_byte_len = MBEDTLS_MAX_ECP_KEY_SIZE_ALT;

	switch (mode->hash_t) {
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

	switch (mode->curve) {
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
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	//ARTIK ISP does not directly extract signature value, but obtains 'r' and 's'
	mode->r->data = (unsigned char*)malloc(ecc_sign.r_byte_len);
	memcpy(mode->r->data, ecc_sign.r, ecc_sign.r_byte_len);
	mode->r->data_len = ecc_sign.r_byte_len;
	mode->s->data = (unsigned char*)malloc(ecc_sign.s_byte_len);
	memcpy(mode->s->data, ecc_sign.s, ecc_sign.s_byte_len);
	mode->s->data_len = ecc_sign.s_byte_len;

	return HAL_SUCCESS;
}
int hal_ecdsa_verify_md(hal_ecdsa_mode mode, hal_data *hash, hal_data *sign, uint32_t key_idx)
{
	uint32_t ret;

	struct sECC_SIGN ecc_sign;
	memset(&ecc_sign, 0, sizeof(struct sECC_SIGN));

	ecc_sign.s = mode.s->data;
	ecc_sign.s_byte_len = mode.s->data_len;
	ecc_sign.r = mode.r->data;
	ecc_sign.r_byte_len = mode.r->data_len;

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
	ret = isp_ecdsa_verify_md_securekey(&ecc_sign, hash->data, hash->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int hal_dh_generate_param(uint32_t dh_idx, hal_dh_data *dh_param)
{
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
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	dh_param->pubkey->data = d_param.publickey;
	dh_param->pubkey->data_len = d_param.publickey_byte_len;

	return HAL_SUCCESS;
}

int hal_dh_compute_shared_secret(hal_dh_data *dh_param, uint32_t dh_idx, hal_data *shared_secret)
{
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

	unsigned char output[300];

	ISP_CHECKBUSY();
	ret = isp_dh_compute_shared_secret_securekey(output, &shared_secret->data_len, d_param, dh_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	shared_secret->data = (unsigned char *)malloc(shared_secret->data_len);
	memcpy(shared_secret->data, output, shared_secret->data_len);

	return HAL_SUCCESS;
}

int hal_ecdh_compute_shared_secret(hal_ecdh_data *ecdh_param, uint32_t key_idx, hal_data *shared_secret)
{
	uint32_t ret;
	struct sECC_KEY ecc_pub;

	memset(&ecc_pub, 0, sizeof(struct sECC_KEY));
	switch (ecdh_param->curve) {
		case HAL_ECDSA_BRAINPOOL_P256R1:
			ecc_pub.curve = OID_ECC_BP256;
			break;
		case HAL_ECDSA_BRAINPOOL_P384R1:
		case HAL_ECDSA_BRAINPOOL_P512R1:
			return HAL_INVALID_ARGS;
		case HAL_ECDSA_SEC_P256R1:
			ecc_pub.curve = OID_ECC_P256;
			break;
		case HAL_ECDSA_SEC_P384R1:
			ecc_pub.curve = OID_ECC_P384;
			break;
		case HAL_ECDSA_SEC_P512R1:
			ecc_pub.curve = OID_ECC_P521;
			break;
	}
	ecc_pub.publickey_x = ecdh_param->pubkey_x->data;
	ecc_pub.x_byte_len = ecdh_param->pubkey_x->data_len;
	ecc_pub.publickey_y = ecdh_param->pubkey_y->data;
	ecc_pub.y_byte_len = ecdh_param->pubkey_y->data_len;

	unsigned char output[300];

	ISP_CHECKBUSY();
	ret = isp_compute_ecdh_securekey(output, &shared_secret->data_len, ecc_pub, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	shared_secret->data = (unsigned char *)malloc(shared_secret->data_len);
	memcpy(shared_secret->data, output, shared_secret->data_len);

	return HAL_SUCCESS;
}

int hal_set_certificate(uint32_t cert_idx, hal_data *cert_in)
{
	uint32_t ret;
	unsigned char *cert = cert_in->data;
	unsigned int cert_len = cert_in->data_len;

	ISP_CHECKBUSY();
	ret = isp_write_cert(cert, cert_len, cert_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int hal_get_certificate(uint32_t cert_idx, hal_data *cert_out)
{
	uint32_t ret;
	unsigned char *buf;
	unsigned int buf_len;

	buf = (unsigned char *)malloc(MAX_BUF_SIZE);
	if (buf == NULL) {
		return HAL_NOT_ENOUGH_MEMORY;
	}

	ISP_CHECKBUSY();
	if (cert_idx == FACTORYKEY_ARTIK_CERT) {
		ret = isp_get_factorykey_data(buf, &buf_len, cert_idx);
		if (ret != 0) {
			isp_clear(0);
			printf("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		ret = isp_read_cert(buf, &buf_len, cert_idx);
		if (ret != 0) {
			isp_clear(0);
			printf("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	}
	cert_out->data = buf;
	cert_out->data_len = buf_len;

	return HAL_SUCCESS;
}

int hal_remove_certificate(uint32_t cert_idx)
{
	return HAL_NOT_SUPPORTED;
}

int hal_get_factorykey_data(uint32_t key_idx, hal_data *data)
{
	uint32_t ret;
	if (key_idx == FACTORYKEY_ARTIK_DEVICE) {
		ISP_CHECKBUSY();
		ret = isp_get_factorykey_data(data->data, &data->data_len, key_idx);
		if (ret != 0) {
			isp_clear(0);
			printf("ISP failed (%zu)\n", ret);
			return HAL_FAIL;
		}
	} else {
		return HAL_NOT_SUPPORTED;
	}

	return HAL_SUCCESS;
}

/**
 * Crypto
 */

int hal_aes_encrypt(hal_data *dec_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *enc_data)
{
	uint32_t ret;
	struct sAES_PARAM param;
	unsigned char aes_output[MAX_BUF_SIZE];

	memset(&param, 0, sizeof(struct sAES_PARAM));

	param.pu8Plaintext = dec_data->data;
	param.u32Plaintext_byte_len = dec_data->data_len;
	param.pu8Ciphertext = aes_output;
	param.u32Ciphertext_byte_len = MAX_BUF_SIZE;
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
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	enc_data->data = (unsigned char *)malloc(param.u32Ciphertext_byte_len);
	memcpy(enc_data->data, param.pu8Ciphertext, param.u32Ciphertext_byte_len);
	enc_data->data_len = param.u32Ciphertext_byte_len;

	return HAL_SUCCESS;
}

int hal_aes_decrypt(hal_data *enc_data, hal_aes_param *aes_param, uint32_t key_idx, hal_data *dec_data)
{
	uint32_t ret;
	struct sAES_PARAM param;
	unsigned char aes_output[MAX_BUF_SIZE];

	memset(&param, 0, sizeof(struct sAES_PARAM));

	param.pu8Plaintext = enc_data->data;
	param.u32Plaintext_byte_len = enc_data->data_len;
	param.pu8Ciphertext = aes_output;
	param.u32Ciphertext_byte_len = MAX_BUF_SIZE;
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
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	dec_data->data = (unsigned char *)malloc(param.u32Ciphertext_byte_len);
	memcpy(dec_data->data, aes_output, param.u32Ciphertext_byte_len);
	dec_data->data_len = param.u32Ciphertext_byte_len;

	return HAL_SUCCESS;
}

int hal_rsa_encrypt(hal_data *dec_data, uint32_t key_idx, hal_data *enc_data)
{
	uint32_t ret;
	unsigned char output[MAX_BUF_SIZE];

	ISP_CHECKBUSY();
	ret = isp_rsa_encrypt_securekey(output, &enc_data->data_len, dec_data->data, dec_data->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	enc_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(enc_data->data, output, enc_data->data_len);

	return HAL_SUCCESS;
}

int hal_rsa_decrypt(hal_data *enc_data, uint32_t key_idx, hal_data *dec_data)
{
	uint32_t ret;
	unsigned char output[MAX_BUF_SIZE];
	ISP_CHECKBUSY();
	ret = isp_rsa_decrypt_securekey(output, &dec_data->data_len, enc_data->data, enc_data->data_len, key_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	dec_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(dec_data->data, output, dec_data->data_len);

	return HAL_SUCCESS;
}

/**
 * Secure Storage
 */

int hal_write_storage(uint32_t ss_idx, hal_data *data)
{
	uint32_t ret;

	ISP_CHECKBUSY();
	ret = isp_write_storage(data->data, data->data_len, ss_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}

	return HAL_SUCCESS;
}

int hal_read_storage(uint32_t ss_idx, hal_data *data)
{
	uint32_t ret;
	unsigned char output[MAX_BUF_SIZE];

	ISP_CHECKBUSY();
	ret = isp_read_storage(output, &data->data_len, ss_idx);
	if (ret != 0) {
		isp_clear(0);
		printf("ISP failed (%zu)\n", ret);
		return HAL_FAIL;
	}
	data->data = (unsigned char *)malloc(data->data_len);
	memcpy(data->data, output, data->data_len);

	return HAL_SUCCESS;
}

int hal_delete_storage(uint32_t ss_idx)
{
	return HAL_NOT_SUPPORTED;
}

