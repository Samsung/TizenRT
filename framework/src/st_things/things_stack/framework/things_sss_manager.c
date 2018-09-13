/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

///
/// This sample provides the way to create cloud sample
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "things_common.h"
#include "securevirtualresourcetypes.h"
#include "octypes.h"
#include "srmresourcestrings.h"
#include "credresource.h"

#include "mbedtls/ssl_ciphersuites.h"
#include "mbedtls/see_api.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pem.h"

#include "things_sss_manager.h"

#define TAG "THINGS_SSS"

static OicSecKey_t cacert;
static OicSecKey_t subca;
static OicSecKey_t devicecert;

static uint8_t g_certChain;

OCStackResult things_sss_rootca_handler_init(OicUuid_t* subjectUuid)
{
	OCStackResult res = OC_STACK_ERROR;
	uint16_t cred_id = 0;

	res = CredSaveTrustCertChain(subjectUuid, cacert.data, cacert.len, OIC_ENCODING_DER, MF_TRUST_CA, &cred_id);
	if (OC_STACK_OK != res) {
		THINGS_LOG_E(TAG, "SRPCredSaveTrustCertChain #2 error");
	    return res;
	}
	THINGS_LOG_D(TAG, "CA Cert.der for MF_TRUST_CA HW saved w/ cred ID=%d", cred_id);

	return res;
}

//this callback will be invoked to get key context based on key usage
void *OCGetHwKey(const char *service, const char *usage, const char *keytype)
{
	THINGS_LOG_D(TAG, "IN : %s", __func__);

	if (service == NULL || usage == NULL) {
		return NULL;
	}

	mbedtls_pk_context *pkey;

	pkey = (mbedtls_pk_context *)things_malloc(sizeof(mbedtls_pk_context));

	if (pkey == NULL) {
		THINGS_LOG_E(TAG, "pkey context alloc failed");
		return NULL;
	}
	THINGS_LOG_D(TAG, "Out: %s", __func__);
	return (void *)pkey;
}

//this callback will free key context that was retreived from TZ
int OCFreeHwKey(void* keyContext)
{
	THINGS_LOG_D(TAG, "IN : %s", __func__);

	if (keyContext == NULL) {
		return -1;
	}

	things_free(keyContext);

	THINGS_LOG_D(TAG, "Out: %s", __func__);

	return 0;
}

//this callback will be invoked to load own certificate in case of TZ
int OCGetOwnCertFromHw(const void* keyContext, uint8_t** certChain, size_t* certChainLen)
{
	(void *)keyContext;
	
	THINGS_LOG_D(TAG, "IN : %s", __func__);

	if (certChain == NULL || certChainLen == NULL) {
		return -1;
	}

	/* If input buffer's address is same with previous allocated memory,
       it will return the success value and iotivity stack use previous memory. */
	if (*certChain && *certChain == g_certChain) {
		return 0;
	}

	*certChainLen = devicecert.len + subca.len + cacert.len;
	*certChain = (uint8_t *)things_malloc(*certChainLen);

	memcpy(*certChain, devicecert.data, devicecert.len);
	memcpy(*certChain + devicecert.len, subca.data, subca.len);
	memcpy(*certChain + devicecert.len + subca.len, cacert.data, cacert.len);

	g_certChain = *certChain;

	THINGS_LOG_D(TAG, "Out: %s", __func__);
	return 0;
}

//this callback will be invoked to load private key in case of TZ
int OCSetupPkContextFromHw(mbedtls_pk_context* ctx, void* keyContext)
{
	THINGS_LOG_D(TAG, "IN : %s", __func__);

	if (ctx == NULL || keyContext == NULL) {
		return -1;
	}

	mbedtls_pk_context* pkey = keyContext;

	mbedtls_pk_init(pkey);
	const mbedtls_pk_info_t *pk_info;

	if ((pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY)) == NULL) {
		mbedtls_pk_free(pkey);
		things_free(pkey);
		return -1;
	}

	if (mbedtls_pk_setup(pkey, pk_info)) {
		mbedtls_pk_free(pkey);
		things_free(pkey);
		return -1;
	}

	mbedtls_ecdsa_init((mbedtls_ecdsa_context*)pkey->pk_ctx);
	((mbedtls_ecdsa_context *)(pkey->pk_ctx))->key_index = FACTORYKEY_ARTIK_DEVICE;
	((mbedtls_ecdsa_context *)(pkey->pk_ctx))->grp.id = MBEDTLS_ECP_DP_SECP256R1;

	ctx->pk_info = pkey->pk_info;
	ctx->pk_ctx = pkey->pk_ctx;

	THINGS_LOG_D(TAG, "Out: %s", __func__);

	return 0;
}

static int things_set_cert_chains(void)
{
	int ret = -1;
	uint8_t *buf = NULL;
	uint32_t buflen = SEE_BUF_MAX_SIZE;

	buf = (uint8_t *)things_malloc(buflen);
	if (buf == NULL) {
		THINGS_LOG_E(TAG, "things_set_cert_chains() : Memory is full");
		return -1;
	}

	if ((ret = see_get_certificate(buf, &buflen, FACTORYKEY_ARTIK_CERT, 0)) != 0) {
		THINGS_LOG_E(TAG, "things_set_cert_chains() : see_get_certificate fail %d", ret);
		things_free(buf);
		return ret;
	}

	int cnt = 0;
	int i = 0;
	uint8_t *ptr = buf;
	// Artik Certificate Order : rootCA -> subCA -> deviceCA
	for (i = 0; i < buflen - 2; i++) {
		if (buf[i] == 0x30 && buf[i + 1] == 0x82 && buf[i + 2] == 0x02) {
			cnt++;
		
			// For rootCA
			if (cnt == 2) {
				cacert.len = (buf + i) - ptr;
				cacert.data = (uint8_t *)things_malloc(cacert.len);
				if (cacert.data == NULL) {
					THINGS_LOG_E(TAG, "Memory is full");
					things_free(buf);
					return -1;
				}
				cacert.encoding = OIC_ENCODING_DER;
				memcpy(cacert.data, ptr, cacert.len);
				ptr = buf + i;
			}

			// For subCA
			if (cnt == 3) {
				subca.len = (buf + i) - ptr;
				subca.data = (uint8_t *)things_malloc(subca.len);
				if (subca.data == NULL) {
					THINGS_LOG_E(TAG, "Memory is full");
					things_free(buf);
					things_free(cacert.data);
					return -2;
				}
				subca.encoding = OIC_ENCODING_DER;
				memcpy(subca.data, ptr, subca.len);
				ptr = buf + i;
			}
		}
	}

	// For deviceCA
	devicecert.len = (buf + buflen) - ptr;
	devicecert.data = (uint8_t *)things_malloc(devicecert.len);
	if (devicecert.data == NULL) {
		THINGS_LOG_E(TAG, "Memory is full");
		things_free(buf);
		things_free(cacert.data);
		things_free(subca.data);
		return -3;
	}
	devicecert.encoding = OIC_ENCODING_DER;
	memcpy(devicecert.data, ptr, devicecert.len);

	things_free(buf);
	return 0;
}

OCStackResult things_sss_key_handler_init(void)
{
	THINGS_LOG_D(TAG, "%s is called", __func__);

	if (things_set_cert_chains() < 0) {
		THINGS_LOG_E(TAG, "things_set_cert_chains() failed");
		return OC_STACK_ERROR;
	}

	if (SetHwPkixCallbacks(OCGetHwKey, OCFreeHwKey, OCGetOwnCertFromHw, OCSetupPkContextFromHw)) {
		THINGS_LOG_E(TAG, "SetHwPkixCallbacks failed");
		return OC_STACK_ERROR;
	}

	uint16_t cipher = MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256;
	if (CA_STATUS_OK != CASelectCipherSuite(cipher, CA_ADAPTER_TCP)) {
		THINGS_LOG_E(TAG, "CASelectCipherSuite returned an error");
		return OC_STACK_ERROR;
	}

	return OC_STACK_OK;
}

bool things_encrypt_artik_uuid(unsigned char *output)
{
	THINGS_LOG_D(TAG, "In %s", __func__);
	
	unsigned char uuid[((UUID_LENGTH * 2) + 4 + 1)] = { 0, };
	unsigned int uuid_len = 0;

	get_artik_crt_uuid(uuid, &uuid_len);

	/* 
	 * urlsafe((base64(sha256(base64(sha256(CertUUID))))).substring(0,7))
	 */

	// 1. sha256(CertUUID)
	unsigned char uuid_hash[32 + 1] = { 0, };
	memset(uuid_hash, 0, sizeof(uuid_hash));
	mbedtls_sha256((const unsigned char *)uuid, uuid_len, uuid_hash, 0);

	// 2. base64(uuid_hash)
	int ret = 0;
	size_t written_len = 0;
	unsigned char encode_buf[128] = { 0, };
	if ((ret = mbedtls_base64_encode(encode_buf, sizeof(encode_buf),
									&written_len, uuid_hash, strlen((char *)uuid_hash))) != 0) {
		THINGS_LOG_E(TAG, "mbedtls_base64_encode() error [%d], written_len [%d]", ret, written_len);
		return false;
	}

	// 3. sha256(encode_buf)
	mbedtls_sha256(encode_buf, strlen((char *)encode_buf), uuid_hash, 0);

	// 4. base64(uuid_hash)
	unsigned char encode_buf2[128] = { 0, };
	if ((ret = mbedtls_base64_encode(encode_buf2, sizeof(encode_buf2),
									&written_len, uuid_hash, strlen((char *)uuid_hash))) != 0) {
		THINGS_LOG_E(TAG, "mbedtls_base64_encode() error [%d], written_len [%d]", ret, written_len);
		return false;
	}

	// 5. use 8 bytes of encode_buf2
	memcpy(output, encode_buf2, 8);
	THINGS_LOG_D(TAG, "output len [%d][%s]", strlen((char *)output), output);

	// 6. url safe
	int i;
	for (i = 0; i < 8; i++) {
		if (output[i] == '+') {
			output[i] = '-';
		} else if (output[i] == '/') {
			output[i] = '_';
		}
	}
	THINGS_LOG_D(TAG, "urlsafe output len [%d][%s]", strlen((char *)output), output);

	return true;
}