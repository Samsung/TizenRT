/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/security_sdk/security_sdk_cert.c
 *
 *   Copyright (C) 2018 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: Youngdae Oh <yd.oh@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <security_sdk_test.h>
#include <tls/x509_crt.h>
#include <tls/pk.h>
#include <tls/pem.h>
#include <tls/oid.h>

#define PEM_BEGIN_CRT		"-----BEGIN CERTIFICATE-----\n"

enum CertFormat {
	CERT_FORMAT_DER,
	CERT_FORMAT_PEM
};

static void output_buffer(uint8_t *buffer, int length)
{
	int i = 0;

	printf("\n");

	while (i < length) {
		uint8_t array[16] = {0};
		int j;
		size_t size = 16;

		if (size > length - i) {
			size = length - i;
		}

		memcpy(array, buffer+i, size);
		for (j = 0 ; j < 16 && i+j < length; j++) {
			printf("%02X ", array[j]);
			if (j%4 == 3) {
				printf(" ");
			}
		}
		if (length > 16) {
			while (j < 16) {
				printf("   ");
				if (j%4 == 3) {
					printf(" ");
				}
				j++;
			}
		}
		printf(" ");
		for (j = 0 ; j < 16 && i+j < length; j++) {
			if (isprint(array[j])) {
				printf("%c", array[j]);
			} else {
				printf(".");
			}
		}
		printf("\n");
		i += 16;
	}
}

static int get_ec_pubkey_from_cert(const char *cert, char **key)
{
	int ret = 0;
	mbedtls_x509_crt x509_cert;
	unsigned char buf[512];
	size_t key_len = 0;

	if (!cert || !key || *key) {
		return E_BAD_ARGS;
	}

	mbedtls_x509_crt_init(&x509_cert);

	ret = mbedtls_x509_crt_parse(&x509_cert, (unsigned char *)cert, strlen(cert) + 1);
	if (ret) {
		fprintf(stderr, "Failed to parse certificate (err=%d)", ret);
		mbedtls_x509_crt_free(&x509_cert);
		return E_ACCESS_DENIED;
	}

	memset(&buf, 0, sizeof(buf));

	ret = mbedtls_pk_write_pubkey_pem(&x509_cert.pk, buf, 2048);
	if (ret) {
		fprintf(stderr, "Failed to write pubkey PEM (err=%d)", ret);
		mbedtls_x509_crt_free(&x509_cert);
		return E_ACCESS_DENIED;
	}

	key_len = strlen((char *)buf) + 1;
	if (key_len <= 0) {
		fprintf(stderr, "Wrong size of key");
		mbedtls_x509_crt_free(&x509_cert);
		return E_SECURITY_ERROR;
	}

	*key = malloc(key_len);
	if (!*key) {
		fprintf(stderr, "Not enough memory to allocate key");
		mbedtls_x509_crt_free(&x509_cert);
		return E_NO_MEM;
	}

	memcpy(*key, buf, key_len);

	mbedtls_x509_crt_free(&x509_cert);
	return S_OK;
}

static int get_certificate_sn(unsigned char *pem_cert_chain, unsigned int pem_cert_chain_len, unsigned char *sn, unsigned int *len)
{
	int ret = 0;
	mbedtls_x509_crt cert;

	if (!pem_cert_chain || pem_cert_chain_len == 0) {
		return E_BAD_ARGS;
	}

	if (!sn || !len || (*len == 0)) {
		return E_BAD_ARGS;
	}

	mbedtls_x509_crt_init(&cert);
	ret = mbedtls_x509_crt_parse(&cert, pem_cert_chain, pem_cert_chain_len);
	if (ret) {
		fprintf(stderr, "Failed to parse certificate (err=%d)", ret);
		mbedtls_x509_crt_free(&cert);
		return E_ACCESS_DENIED;
	}

	if (cert.serial.len > *len) {
		fprintf(stderr, "Buffer is too small");
		mbedtls_x509_crt_free(&cert);
		return E_BAD_ARGS;
	}

	memcpy(sn, cert.serial.p, cert.serial.len);
	*len = cert.serial.len;

	mbedtls_x509_crt_free(&cert);

	return S_OK;
}

int security_sdk_cert_serial(void)
{
	artik_security_handle handle = NULL;
	artik_error ret = S_OK;
	int test_result = 0;
	unsigned char *cert = NULL;
	unsigned int certlen = 0;
	unsigned char serial_number[ARTIK_CERT_SN_MAXLEN] = "";
	unsigned int len = ARTIK_CERT_SN_MAXLEN;
	int i = 0;

	fprintf(stderr, "--------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get cert serial number\n");
	fprintf(stderr, "--------------------------------------------\n");

	artik_security_module *security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	ret = security->request(&handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	ret = security->get_certificate(handle, ARTIK_DEVICE_CERT_ID,
			ARTIK_SECURITY_CERT_TYPE_PEM, (unsigned char **)&cert,
			(unsigned int *)&certlen);
	if ((ret != S_OK) || !cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	ret = get_certificate_sn(cert, certlen, serial_number, &len);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get serial number (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	fprintf(stdout, "SerialNumber:  ");
	for (i = 0; i < (len - 1); i++) {
		fprintf(stdout, "%02x:", serial_number[i]);
	}

	fprintf(stdout, "%02x\n", serial_number[len - 1]);

exit:
	if (cert) {
		free(cert);
	}
	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}

int security_sdk_get_certificates(void)
{
	artik_security_module *security = NULL;
	int test_result = 0;
	artik_security_handle handle = NULL;
	artik_error ret = S_OK;
	unsigned int cnt = 0;
	unsigned char *cert = NULL;
	unsigned int certlen = 0;
	char key_name[20] = {0};

	char *test_cert_pem =
			"-----BEGIN CERTIFICATE-----\n"
			"MIICNzCCAd2gAwIBAgIJAKMtfXi0W6ioMAoGCCqGSM49BAMCMH8xCzAJBgNVBAYT\n"
			"AktSMRAwDgYDVQQIDAdFbmdsYW5kMSQwIgYDVQQKDBtTYW1zdW5nIFNlbWljb25k\n"
			"dWN0b3IgQVJUSUsxGzAZBgNVBAsMEkFSVElLIFJvb3QgQ0EgVGVzdDEbMBkGA1UE\n"
			"AwwSQVJUSUsgUm9vdCBDQSBUZXN0MB4XDTE3MDMxNDA3NTQyMloXDTMyMDMxMDA3\n"
			"NTQyMlowfzELMAkGA1UEBhMCS1IxEDAOBgNVBAgMB0VuZ2xhbmQxJDAiBgNVBAoM\n"
			"G1NhbXN1bmcgU2VtaWNvbmR1Y3RvciBBUlRJSzEbMBkGA1UECwwSQVJUSUsgUm9v\n"
			"dCBDQSBUZXN0MRswGQYDVQQDDBJBUlRJSyBSb290IENBIFRlc3QwWTATBgcqhkjO\n"
			"PQIBBggqhkjOPQMBBwNCAAS9eeiVxAIIstHpInEhbIhNGNtqvZ3Qr1h1NXbvJeC3\n"
			"BgCY5LUhOphwb32PjgjvHo+m0BRbdf5Vhq9rMtpP9Uk4o0IwQDAdBgNVHQ4EFgQU\n"
			"B9ReEry47MDB6Ql+N7TJf/JxGhAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n"
			"BAMCAQYwCgYIKoZIzj0EAwIDSAAwRQIgV2iTYlR/hyWmzxcMf/wU4kOhRCRB2LD2\n"
			"ICpARpmuz7ECIQCLiRlB9EWS810OrPmcDXYhf2vRJyesqrMJiPTL2SNl0g==\n"
			"-----END CERTIFICATE-----\n";
	unsigned char test_cert_der[] = {
			0x30, 0x82, 0x02, 0x22, 0x30, 0x82, 0x01, 0xC8, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x09, 0x00,
			0xB7, 0x5A, 0x90, 0xE7, 0x75, 0x1A, 0xD9, 0x37, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,
			0x3D, 0x04, 0x03, 0x02, 0x30, 0x64, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
			0x02, 0x55, 0x53, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0C, 0x02, 0x55, 0x53,
			0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0C, 0x04, 0x54, 0x65, 0x73, 0x74, 0x31,
			0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x04, 0x54, 0x65, 0x73, 0x74, 0x31, 0x0D,
			0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x04, 0x54, 0x65, 0x73, 0x74, 0x31, 0x1B, 0x30,
			0x19, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x12, 0x4B, 0x79, 0x72, 0x69, 0x6F, 0x20, 0x54, 0x65,
			0x73, 0x74, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x20, 0x43, 0x41, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x37,
			0x30, 0x31, 0x31, 0x36, 0x30, 0x38, 0x31, 0x35, 0x30, 0x34, 0x5A, 0x17, 0x0D, 0x33, 0x37, 0x30,
			0x31, 0x31, 0x31, 0x30, 0x38, 0x31, 0x35, 0x30, 0x34, 0x5A, 0x30, 0x64, 0x31, 0x0B, 0x30, 0x09,
			0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55,
			0x04, 0x08, 0x0C, 0x02, 0x55, 0x53, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0C,
			0x04, 0x54, 0x65, 0x73, 0x74, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x04,
			0x54, 0x65, 0x73, 0x74, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x04, 0x54,
			0x65, 0x73, 0x74, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x12, 0x4B, 0x79,
			0x72, 0x69, 0x6F, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x20, 0x43, 0x41,
			0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
			0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xBD, 0x79, 0xE8, 0x95, 0xC4,
			0x02, 0x08, 0xB2, 0xD1, 0xE9, 0x22, 0x71, 0x21, 0x6C, 0x88, 0x4D, 0x18, 0xDB, 0x6A, 0xBD, 0x9D,
			0xD0, 0xAF, 0x58, 0x75, 0x35, 0x76, 0xEF, 0x25, 0xE0, 0xB7, 0x06, 0x00, 0x98, 0xE4, 0xB5, 0x21,
			0x3A, 0x98, 0x70, 0x6F, 0x7D, 0x8F, 0x8E, 0x08, 0xEF, 0x1E, 0x8F, 0xA6, 0xD0, 0x14, 0x5B, 0x75,
			0xFE, 0x55, 0x86, 0xAF, 0x6B, 0x32, 0xDA, 0x4F, 0xF5, 0x49, 0x38, 0xA3, 0x63, 0x30, 0x61, 0x30,
			0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x07, 0xD4, 0x5E, 0x12, 0xBC, 0xB8,
			0xEC, 0xC0, 0xC1, 0xE9, 0x09, 0x7E, 0x37, 0xB4, 0xC9, 0x7F, 0xF2, 0x71, 0x1A, 0x10, 0x30, 0x1F,
			0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x07, 0xD4, 0x5E, 0x12, 0xBC,
			0xB8, 0xEC, 0xC0, 0xC1, 0xE9, 0x09, 0x7E, 0x37, 0xB4, 0xC9, 0x7F, 0xF2, 0x71, 0x1A, 0x10, 0x30,
			0x0F, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF,
			0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86,
			0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30,
			0x45, 0x02, 0x21, 0x00, 0x89, 0x5F, 0xCC, 0xF1, 0x6A, 0xAC, 0xBF, 0x06, 0x63, 0x74, 0xA4, 0xA4,
			0x19, 0x3B, 0x91, 0x75, 0xB2, 0x58, 0xEB, 0xDC, 0x73, 0x0C, 0x48, 0x13, 0x7D, 0x92, 0xCB, 0xE1,
			0x80, 0x29, 0x05, 0x9E, 0x02, 0x20, 0x74, 0x94, 0xE2, 0x97, 0xAF, 0x43, 0x43, 0x15, 0x3B, 0x5B,
			0x1B, 0xBE, 0x24, 0x5D, 0x83, 0x78, 0xA7, 0x61, 0x7D, 0xE9, 0x58, 0xED, 0xBB, 0x9C, 0xD9, 0x35,
			0x82, 0xFF, 0x26, 0x3B, 0xD1, 0xC3
	};

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get/set certificates\n");
	fprintf(stderr, "------------------------------------------------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	ret = security->request(&handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	/*
	 * Inject PEM certificate and get it in DER form.
	 */
	memset(key_name, 0, sizeof(key_name));
	sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, 0);
	ret = security->set_certificate(handle, key_name,
			(unsigned char *)test_cert_pem, strlen(test_cert_pem));
	ret += security->get_certificate(handle, key_name,
			ARTIK_SECURITY_CERT_TYPE_DER, (unsigned char **)&cert,
			(unsigned int *)&certlen);
	if ((ret != S_OK) || !cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	if (cert) {
		output_buffer(cert, certlen);
	}

	see_selfprintf("[%d] ", cnt++);

	if (!cert || ret) {
		test_result++;
		see_selfprintf(" fail\n");
	} else if (cert[0] != 0x30 || cert[1] != 0x82) {
		test_result++;
		see_selfprintf(" fail\n");
	} else {
		see_selfprintf(" success\n");
	}

	if (cert) {
		free(cert);
		cert = NULL;
	}
	certlen = 0;

	/*
	 * Inject DER certificate and get it in PEM form.
	 */
	memset(key_name, 0, sizeof(key_name));
	sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, 1);
	ret = security->set_certificate(handle, key_name,
			test_cert_der, sizeof(test_cert_der));
	ret += security->get_certificate(handle, key_name,
			ARTIK_SECURITY_CERT_TYPE_PEM, (unsigned char **)&cert,
			(unsigned int *)&certlen);
	if ((ret != S_OK) || !cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	if (cert) {
		printf("PEM_CERT :\n%s", cert);
	}

	see_selfprintf("[%d] ", cnt++);

	if (!cert || ret) {
		test_result++;
		see_selfprintf(" fail\n");
	} else if (!strstr((const char *)cert, PEM_BEGIN_CRT)) {
		test_result++;
		see_selfprintf(" fail\n");
	} else {
		see_selfprintf(" success\n");
	}

	if (cert) {
		free(cert);
		cert = NULL;
	}
	certlen = 0;

exit:
	if (cert) {
		free(cert);
	}
	if (handle) {
		security->release(handle);
	}
	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}

int security_sdk_cert_publickey(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle;
	artik_error ret = S_OK;
	int test_result = 0;
	char *se_pk_pem = NULL;
	unsigned char *se_pk_der = NULL;
	unsigned char *cert = NULL;
	unsigned int certlen = 0;
	int length;
	enum CertFormat fmt = CERT_FORMAT_PEM;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get certificate public key\n");
	fprintf(stderr, "------------------------------------------------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		test_result++;
		goto exit;
	}

	ret = security->request(&handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	ret = security->get_certificate(handle, ARTIK_DEVICE_CERT_ID,
			ARTIK_SECURITY_CERT_TYPE_PEM, (unsigned char **)&cert,
			(unsigned int *)&certlen);
	if ((ret != S_OK) || !cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	ret = get_ec_pubkey_from_cert((char *)cert, &se_pk_pem);
	if (ret != S_OK || !se_pk_pem) {
		fprintf(stderr, "Failed to get public key (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	switch (fmt) {
	case CERT_FORMAT_PEM:
		fprintf(stdout, se_pk_pem);
		break;
	case CERT_FORMAT_DER:
		ret = security->convert_pem_to_der((char *)se_pk_pem, &se_pk_der, (size_t *)&length);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to convert PEM public key to DER (err=%d)\n", ret);
			test_result++;
			goto exit;
		}
		output_buffer(se_pk_der, length);
	}

exit:

	if (se_pk_der) {
		free(se_pk_der);
	}

	if (se_pk_pem) {
		free(se_pk_pem);
	}

	if (cert) {
		free(cert);
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}
