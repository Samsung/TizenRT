/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file see/see_api.c
/// @brief SEE api is supporting security api for using secure storage.

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "tls/pk.h"
#include "tls/pem.h"
#include "tls/x509_crt.h"
#include "tls/pk_internal.h"

#include "tls/see_api.h"

static see_mutex_t m_handler = { PTHREAD_MUTEX_INITIALIZER, 0 };

#define _SEE_MUTEX_LOCK                     \
{                                          \
	if (see_mutex_lock(&m_handler) != 0)    \
		return -1;                         \
}
#define _SEE_MUTEX_UNLOCK                   \
{                                          \
	if (see_mutex_unlock(&m_handler) != 0)  \
		return -1;                         \
}

//#define ISP_CHECKBUSY() while(isp_get_status()){}
#define ISP_CHECKBUSY()

int see_init(void)
{
	SEE_DEBUG("%s called\n", __func__);

	/* */
	if (m_handler.valid) {
		m_handler.valid++;
		return SEE_OK;
	}

	/* Get Mutex */
	if (see_mutex_init(&m_handler) != 0) {
		return SEE_ERROR;
	}

	return SEE_OK;
}

int see_free(void)
{
	SEE_DEBUG("%s called\n", __func__);

	/* */
	if (m_handler.valid > 1) {
		m_handler.valid--;
		return SEE_OK;
	}

	/* Free Mutex */
	if (see_mutex_free(&m_handler) != 0) {
		return SEE_ERROR;
	}

	memset(&m_handler, 0, sizeof(see_mutex_t));

	return SEE_OK;
}

#define SEE_BEGIN_CERT           "-----BEGIN CERTIFICATE-----\n"
#define SEE_END_CERT             "-----END CERTIFICATE-----\n"

#define PEM_BEGIN_CERT           "-----BEGIN CERTIFICATE-----"
#define PEM_END_CERT             "-----END CERTIFICATE-----"

/* Set certificate in Secure storage (support der and pem types) */
int see_set_certificate(unsigned char *cert, unsigned int cert_len, unsigned int cert_index, unsigned int cert_type)
{
	int r;
	unsigned int use_len;
	mbedtls_pem_context pem;
	SEE_DEBUG("%s called\n", __func__);

	if (cert == NULL || cert_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK if (cert_type == CERT_PEM) {
		mbedtls_pem_init(&pem);
		if ((r = mbedtls_pem_read_buffer(&pem, PEM_BEGIN_CERT, PEM_END_CERT, cert, NULL, 0, &use_len)) != 0) {
			SEE_DEBUG("mbedtls_pem_read_buffer fail %x\n", -r);
			_SEE_MUTEX_UNLOCK mbedtls_pem_free(&pem);
			return SEE_INVALID_INPUT_PARAMS;
		}

		ISP_CHECKBUSY();
		if ((r = isp_write_cert(pem.buf, pem.buflen, cert_index)) != 0) {
			SEE_DEBUG("isp_write_cert fail %x\n", r);
			isp_clear(0);
			_SEE_MUTEX_UNLOCK mbedtls_pem_free(&pem);
			return SEE_ERROR;
		}
	} else if (cert_type == CERT_DER) {
		ISP_CHECKBUSY();
		if ((r = isp_write_cert(cert, cert_len, cert_index)) != 0) {
			_SEE_MUTEX_UNLOCK SEE_DEBUG("isp_write_cert fail %x\n", r);
			isp_clear(0);
			return SEE_ERROR;
		}
	} else {
		_SEE_MUTEX_UNLOCK return SEE_INVALID_INPUT_PARAMS;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

#define SEE_SUBJECT_KEY_INDEX   0xFF
#define SEE_ISSUER_KEY_INDEX    0xFF

#define SEE_SUBJECT_NAME        "CN=ARTIK051_dev,O=Samsung DA,C=KR"
#define SEE_ISSUER_NAME         "C=KR,ST=KyeongGi,L=Suwon,O=Samsung,OU=DA,CN=ARTIK051,emailAddress=hh0619.choi@samsung.com"
/*"CN=ARTIK051,O=Samsung,C=KR"*/
#define SEE_NOT_BEFORE          "20160101000000"
#define SEE_NOT_AFTER           "20361231235959"
#define SEE_SERIAL              "1"
#define SEE_IS_CA               0
#define SEE_MAX_PATHLEN         -1
#define SEE_KEY_USAGE           0
#define SEE_NS_CERT_TYPE        0

int see_generate_certificate(cert_opt opt, unsigned char *out_buf, unsigned int *out_buflen)
{
	SEE_DEBUG("%s called\n", __func__);
#if defined(CONFIG_HW_RSA_SIGN)
	int r = 0;
	int cert_len;
	unsigned int subject_key_len = 1024;
	unsigned char subject_key_buf[1024];
	unsigned char cert_buf[2500];

	mbedtls_pk_context subject_key;
	mbedtls_pk_context issuer_key;
	mbedtls_x509write_cert crt;
	mbedtls_mpi serial;

	if (opt.cert_index >= MAX_CERT_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (opt.subject_key_index >= MAX_KEY_INDEX || opt.issuer_key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	mbedtls_pk_init(&subject_key);
	mbedtls_pk_init(&issuer_key);

	mbedtls_x509write_crt_init(&crt);
	mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
	mbedtls_mpi_init(&serial);

	mbedtls_pk_setup(&issuer_key, &mbedtls_rsa_info);

	/* Check mandatory params */
	if (!opt.subject_key_index && !opt.issuer_key_index) {
		opt.subject_key_index = SEE_SUBJECT_KEY_INDEX;
		opt.issuer_key_index = SEE_ISSUER_KEY_INDEX;
	} else {
		//subject_key.key_index = opt.subject_key_index;
		//issuer_key.key_index = opt.issuer_key_index;
	}

	/* Check optional params */
	if (!opt.issuer_name || !opt.subject_name || !opt.not_before || !opt.not_after) {
		opt.issuer_name = SEE_ISSUER_NAME;
		opt.subject_name = SEE_SUBJECT_NAME;
		opt.not_before = SEE_NOT_BEFORE;
		opt.not_after = SEE_NOT_AFTER;
		opt.serial = SEE_SERIAL;
		opt.is_ca = SEE_IS_CA;
		opt.max_pathlen = SEE_MAX_PATHLEN;
		opt.key_usage = SEE_KEY_USAGE;
		opt.ns_cert_type = SEE_NS_CERT_TYPE;
	}

	/*
	 *  0. Parse serial to MPI
	 */
	if ((r = mbedtls_mpi_read_string(&serial, 10, opt.serial)) != 0) {
		SEE_DEBUG("mpi_read_string returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	/*
	 *  1.0 Load subject key
	 */
	if (opt.subject_key_index == 0xFF) {
		if ((r = see_get_publickey(subject_key_buf, &subject_key_len)) != 0) {
			SEE_DEBUG("see_get_publickey returned %x\n\n", r);
			goto see_exit;
		}

		if ((r = mbedtls_pk_parse_public_key(&subject_key, subject_key_buf, subject_key_len)) != 0) {
			SEE_DEBUG("pk_parse_key returned -0x%02x\n\n", -r);
			goto see_exit;
		}

	} else {
		if ((r = mbedtls_pk_parse_public_key(&subject_key, opt.subject_pub_key, opt.subject_pub_keylen)) != 0) {
			SEE_DEBUG("pk_parse_key returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}

	/*
	 *  1.1 Set key
	 */
	if (subject_key.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(subject_key.pk_ctx))->key_index = opt.subject_key_index;
	}
	if (issuer_key.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(issuer_key.pk_ctx))->key_index = opt.issuer_key_index;
	}
	mbedtls_x509write_crt_set_subject_key(&crt, &subject_key);
	crt.issuer_key = &issuer_key;

	/*
	 *  2.0 Setting the certificate value.
	 */
	if ((r = mbedtls_x509write_crt_set_subject_name(&crt, opt.subject_name)) != 0) {
		SEE_DEBUG("x509write_crt_set_subject_name returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	if ((r = mbedtls_x509write_crt_set_issuer_name(&crt, opt.issuer_name)) != 0) {
		SEE_DEBUG("x509write_crt_set_issuer_name returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	if ((r = mbedtls_x509write_crt_set_serial(&crt, &serial)) != 0) {
		SEE_DEBUG("x509write_crt_set_serial returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	if ((r = mbedtls_x509write_crt_set_validity(&crt, opt.not_before, opt.not_after)) != 0) {
		SEE_DEBUG("x509write_crt_set_validity returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	/*
	 *  2.1 Adding the basic constraints extension.
	 */
	if ((r = mbedtls_x509write_crt_set_basic_constraints(&crt, opt.is_ca, opt.max_pathlen)) != 0) {
		SEE_DEBUG("x509write_crt_set_basic_contraints returned -0x%02x\n\n", -r);
		goto see_exit;
	}
#if defined(MBEDTLS_SHA1_C)
	/*
	 *  2.2 Adding the Subject/Authority key identifier.
	 */
	if ((r = mbedtls_x509write_crt_set_subject_key_identifier(&crt)) != 0) {
		SEE_DEBUG("crt_set_subject_key_identifier returned -0x%02x\n\n", -r);
		goto see_exit;
	}

	if ((r = mbedtls_x509write_crt_set_authority_key_identifier(&crt)) != 0) {
		SEE_DEBUG("crt_set_authority_key_identifier returned -0x%02x\n\n", -r);
		goto see_exit;
	}
#endif
	/*
	 *  2.3 Adding the key usage extension.
	 */
	if (opt.key_usage) {
		if ((r = mbedtls_x509write_crt_set_key_usage(&crt, opt.key_usage)) != 0) {
			SEE_DEBUG("crt_set_key_usage returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}
	if (opt.ns_cert_type) {
		if ((r = mbedtls_x509write_crt_set_ns_cert_type(&crt, opt.ns_cert_type)) != 0) {
			SEE_DEBUG("crt_set_ns_cert_type returned -0x%02x\n\n", -r);
			goto see_exit;
		}
	}

	/*
	 *  3.0 Writing the certificate.
	 */
	if ((cert_len = mbedtls_x509write_crt_der(&crt, cert_buf, sizeof(cert_buf), NULL, NULL)) <= 0) {
		SEE_DEBUG("x509write_crt_der returned -0x%02x\n\n", -cert_len);
		r = cert_len;
		goto see_exit;
	}

	if ((r = mbedtls_pem_write_buffer(SEE_BEGIN_CERT, SEE_END_CERT, cert_buf + sizeof(cert_buf) - cert_len, cert_len, out_buf, *out_buflen, out_buflen)) != 0) {
		SEE_DEBUG("mbedt pem write buffer fail %x\n", r);
		goto see_exit;
	}

	/*
	 * 3.1 Write Cert in isp storage
	 */
	/*
	   if(cert_der = malloc(cert_len)) goto see_exit;

	   while(cert_len >= 0) {
	   cert_der[cert_derlen++] = cert_buf[cert_len--];
	   }
	 */

	SEE_DEBUG("size %d\n", *out_buflen);

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_write_cert(cert_buf, cert_len, opt.cert_index)) != 0) {
		SEE_DEBUG("isp_write_cert fail %x\n", r);
		isp_clear(0);
		goto see_exit;
	}
_SEE_MUTEX_UNLOCK see_exit:
	mbedtls_pk_free(&subject_key);
	mbedtls_pk_free(&issuer_key);
	mbedtls_x509write_crt_free(&crt);
	mbedtls_mpi_free(&serial);

	return r;
#else
	return 1;
#endif
}

int see_get_certificate(unsigned char *cert, unsigned int *cert_len, unsigned int cert_index, unsigned int cert_type)
{
	int r;
	unsigned int buf_len, olen;
	unsigned char buf[4096];
	SEE_DEBUG("%s called\n", __func__);

	if (cert == NULL || cert_len == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (cert_index >= MAX_CERT_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (cert_type != CERT_DER && cert_type != CERT_PEM) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_read_cert(buf, &buf_len, cert_index)) != 0) {
		SEE_DEBUG("isp_read_cert fail %x\n", r);
		isp_clear(0);
		return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK if (cert_type == CERT_PEM) {
		if ((r = mbedtls_pem_write_buffer(SEE_BEGIN_CERT, SEE_END_CERT, buf, buf_len, cert, *cert_len, &olen)) != 0) {
			SEE_DEBUG("change der to pem format fail %x\n", r);
			return SEE_ERROR;
		}
		*cert_len = olen;
	} else {
		memcpy(cert, buf, olen);
		*cert_len = olen;
	}

	return SEE_OK;
}

int see_write_secure_storage(unsigned char *data, unsigned int data_len, unsigned int index)
{
	int r;

	SEE_DEBUG("%s called\n", __func__);

	if (data == NULL || !data_len) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (index >= MAX_DATA_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_write_storage(data, data_len, index)) != 0) {
		SEE_DEBUG("isp_write_storage fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_read_secure_storage(unsigned char *data, unsigned int *data_len, unsigned int index)
{
	int r;

	SEE_DEBUG("%s called\n", __func__);

	if (data == NULL || !data_len) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (index >= MAX_DATA_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_read_storage(data, data_len, index)) != 0) {
		SEE_DEBUG("isp_read_storage fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_publickey(unsigned char *key_der, unsigned int *key_len)
{
	int r = 0;

	SEE_DEBUG("%s called\n", __func__);

	if (key_der == NULL || key_len == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_read_publickey(key_der, key_len)) != 0) {
		SEE_DEBUG("isp_read_publickey fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_generate_key(unsigned int key_type, unsigned int key_index, unsigned int key_len, unsigned int pukey_e)
{
	int r = 0;
	unsigned int key = key_type & 0xFF0000;
	unsigned int object_id = key_type & 0xFF;

	SEE_DEBUG("%s called index %d type %x len %d\n", __func__, key_index, key_type, key_len);

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK switch (key) {
	case AES_KEY:
		ISP_CHECKBUSY();
		r = isp_aes_generate_key_securekey(key_len, key_index);
		break;
	case HMAC_KEY:
		ISP_CHECKBUSY();
		r = isp_hmac_generate_key_securekey(key_len, key_index);
		break;
	case RSA_KEY:
		ISP_CHECKBUSY();
		r = isp_rsa_generate_key_securekey(key_index, object_id, pukey_e);
		break;
	case ECC_KEY:
		ISP_CHECKBUSY();
		r = isp_ecdsa_generate_key_securekey(key_index, object_id);
		break;
		/*                case ECC_KEY:
		   ISP_CHECKBUSY();
		   r = isp_ecdsa_generate_publickey_securekey(key_index, object_id);
		   break; */
	default:
		_SEE_MUTEX_UNLOCK return SEE_INVALID_INPUT_PARAMS;
	}
	_SEE_MUTEX_UNLOCK if (r) {
		SEE_DEBUG("isp_generate_key fail %x %x %x\n", r, key, object_id);
		isp_clear(0);
		return SEE_ERROR;
	}

	return SEE_OK;
}

int see_setup_key(unsigned char *key_der, unsigned int key_len, unsigned int key_type, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called index %d type %d len %d\n", __func__, key_index, key_type, key_len);

	if (key_der == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK
#if 1
	ISP_CHECKBUSY();
	if ((r = isp_set_securekey(key_der, key_len, key_type, key_index)) != 0) {
		SEE_DEBUG("isp_set_userkey fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
#else
	ISP_CHECKBUSY();
	if ((r = isp_set_factorykey(key_der)) != 0) {
		SEE_DEBUG("isp_set_userkey fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
#endif
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_remove_key(unsigned int key_index, unsigned int key_type)
{
	int r;
	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_remove_key(key_type, key_index)) != 0) {
		SEE_DEBUG("isp_remove_key fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_aes_encrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
{
	int r;

	SEE_DEBUG("%s called \n", __func__);

	if (aes_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_aes_encrypt_securekey(aes_param, key_index)) != 0) {
		SEE_DEBUG("isp_aes_encrypt fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_aes_decrypt(unsigned int key_index, struct sAES_PARAM *aes_param)
{
	int r;

	SEE_DEBUG("%s called %d mode %d\n", __func__, key_index, aes_param->u32Mode);

	if (aes_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_aes_decrypt_securekey(aes_param, key_index)) != 0) {
		SEE_DEBUG("isp_aes_decrypt fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_hmac(struct sHMAC_MSG *hmac_msg, unsigned char *output, unsigned int object_id, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d %x\n", __func__, key_index, object_id);
	if (!hmac_msg || !output) {
		return SEE_ERROR;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_hmac_securekey(output, hmac_msg, object_id, key_index)) != 0) {
		SEE_DEBUG("isp_hmac fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_hash(struct sHASH_MSG *h_param, unsigned char *hash, unsigned int mode)
{
	int r;

//        SEE_DEBUG("%s called %d %x\n",__func__, h_param->msg_byte_len, mode);

	if (hash == NULL || h_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_hash(hash, h_param, mode)) != 0) {
		SEE_DEBUG("isp_hash fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_generate_random(unsigned int *data, unsigned int len)
{
	int r;

	if (data == NULL || len > SEE_MAX_RANDOM_SIZE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	SEE_DEBUG("%s len : %d\n", __func__, len);

	/* Change length to word number */
	if (len & 0x3) {
		len = len + 4 - (len & 0x3);
	}

	_SEE_MUTEX_LOCK
	ISP_CHECKBUSY();
	if ((r = isp_generate_random(data, len / 4)) != 0) {
		isp_clear(0);
		_SEE_MUTEX_UNLOCK
		SEE_DEBUG("isp_generate_random fail %x\n", r);
		return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK

	return SEE_OK;
}

/* Generate G, P, GX (G^X mod P) */
int see_generate_dhm_params(struct sDH_PARAM *d_param, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);
	if (d_param == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_dh_generate_keypair_userparam_securestorage(d_param, key_index)) != 0) {
		SEE_DEBUG("isp_generate_dh_param fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

/* Compute shared secret key = GXY ((G^Y)^X mod P) */
int see_compute_dhm_param(struct sDH_PARAM *d_param, unsigned int key_index, unsigned char *output, unsigned int *olen)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (d_param == NULL || output == NULL) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_dh_compute_shared_secret_securekey(output, olen, *d_param, key_index)) != 0) {
		SEE_DEBUG("isp_compute_dh_param fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_rsa_decryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (input == NULL || output == NULL || inlen == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_rsa_decrypt_securekey(output, outlen, input, inlen, key_index)) != 0) {
		SEE_DEBUG("isp_rsa_decrypt fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_rsa_encryption(unsigned int key_index, unsigned int pad_type, unsigned char *output, unsigned int *outlen, unsigned char *input, unsigned int inlen)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (input == NULL || output == NULL || inlen == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (pad_type != MBEDTLS_RSA_PKCS_V15) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_rsa_encrypt_securekey(output, outlen, input, inlen, key_index)) != 0) {
		SEE_DEBUG("isp_rsa_encrypt fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %x\n", __func__, key_index);

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_rsa_sign_securekey(rsa_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_rsa_sign fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_verify_rsa_signature(struct sRSA_SIGN *rsa_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d ", __func__, key_index);

	if (rsa_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_rsa_verify_securekey(rsa_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_rsa_verify fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_get_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_ecdsa_sign_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_ecdsa_sign fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_verify_ecdsa_signature(struct sECC_SIGN *ecc_sign, unsigned char *hash, unsigned int hash_len, unsigned int key_index)
{
	int r;

	SEE_DEBUG("%s called %d\n", __func__, key_index);

	if (ecc_sign == NULL || hash == NULL || hash_len == 0) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	if (key_index >= MAX_KEY_INDEX && key_index < 0xFE) {
		return SEE_INVALID_INPUT_PARAMS;
	}

	_SEE_MUTEX_LOCK ISP_CHECKBUSY();
	if ((r = isp_ecdsa_verify_securekey(ecc_sign, hash, hash_len, key_index)) != 0) {
		SEE_DEBUG("isp_ecdsa_verify fail %x\n", r);
		isp_clear(0);
		_SEE_MUTEX_UNLOCK return SEE_ERROR;
	}
	_SEE_MUTEX_UNLOCK return SEE_OK;
}

int see_mutex_init(see_mutex_t *m)
{
	if (m == NULL) {
		return -1;
	}

	if (pthread_mutex_init(&m->mutex, NULL)) {
		m->valid = 0;
		return -1;
	}

	m->valid = 1;
	return 0;
}

int see_mutex_free(see_mutex_t *m)
{
	if (m == NULL) {
		return -1;
	}

	if (pthread_mutex_destroy(&m->mutex) != 0) {
		return -1;
	}

	m->valid = 0;
	return 0;
}

int see_mutex_lock(see_mutex_t *m)
{
	if (m == NULL || !m->valid) {
		return -1;
	}

	if (pthread_mutex_lock(&m->mutex) != 0) {
		return -1;
	}

	return 0;
}

int see_mutex_unlock(see_mutex_t *m)
{
	if (m == NULL || !m->valid) {
		return -1;
	}

	if (pthread_mutex_unlock(&m->mutex) != 0) {
		return -1;
	}

	return 0;
}

#define SEE_MAX_KEY_SLOT  8
#define SEE_MAX_KEY_TYPE  15
#define SEE_KEY_OFFSET    10

typedef struct key_slot {
	char use[SEE_MAX_KEY_SLOT];
} key_manager_s;

key_manager_s key_m[SEE_MAX_KEY_TYPE - SEE_KEY_OFFSET];

unsigned int see_free_keyindex(unsigned int key_type, unsigned int key_index)
{
	_SEE_MUTEX_LOCK(key_m[key_type - SEE_KEY_OFFSET].use)[key_index] = 0;
	_SEE_MUTEX_UNLOCK return 0;;
}

unsigned int see_get_keyindex(unsigned int key_type)
{
	int i, j;
	for (i = 0; i < SEE_MAX_KEY_SLOT; i++) {
		if (!(key_m[key_type - SEE_KEY_OFFSET].use)[i]) {
			_SEE_MUTEX_LOCK(key_m[key_type - SEE_KEY_OFFSET].use)[i] = 1;
			_SEE_MUTEX_UNLOCK return i;
		}
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 8; j++) {
			printf("%x", (key_m[i].use)[j]);
		}
		printf("\n");
	}

	return 0xFF;
}
