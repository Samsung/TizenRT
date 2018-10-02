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
 * examples/security_sdk/security_sdk_factory.c
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

#define BUF_NAME_SIZE 20
#define SEE_HASHNUM 4

struct facto_info {
	char object_name[20];
	char key_name[20];
	int existence;
};

static struct facto_info g_cert_existence[ARTIK_CERTS_NUM];
static struct facto_info g_key_existence[1];

void security_sdk_print_existence(void)
{
	int i;

	for (i = 0; i < ARTIK_CERTS_NUM; i++) {
		if (g_cert_existence[i].existence) {
			printf("%s", "[ O ]");
		} else {
			printf("%s", "[ X ]");
		}
		printf(" %s\n", g_cert_existence[i].object_name);
	}
	for (i = 0; i < 1; i++) {
		if (g_key_existence[i].existence) {
			printf("%s", "[ O ]");
		} else {
			printf("%s", "[ X ]");
		}
		printf(" %s\n", g_key_existence[i].object_name);
	}
}

void security_sdk_init_existence(void)
{
	int i;
	char cert_object_name[ARTIK_CERTS_NUM][20] = {
			"ARTIK_DEVICE_CERT",
			"ARTIK_DEVICECA_CERT",
			"ARTIK_ROOTCA_CERT"
	};
	static const char * const cert_index_name[] = {
			ARTIK_DEVICE_CERT_ID,
			ARTIK_DEVICECA_CERT_ID,
			ARTIK_ROOTCA_CERT_ID
	};

	char key_object_name[1][20] = {
			"ARTIK_DEVICE_KEY"
	};
	static const char * const key_index_name[] = {
			ARTIK_DEVICE_KEY_ID
	};

	for (i = 0; i < ARTIK_CERTS_NUM; i++) {
		memset(g_cert_existence[i].object_name, 0, BUF_NAME_SIZE);
		memset(g_cert_existence[i].key_name, 0, BUF_NAME_SIZE);
		g_cert_existence[i].existence = 0;

		memcpy(g_cert_existence[i].object_name, cert_object_name[i], strlen(cert_object_name[i]));
		memcpy(g_cert_existence[i].key_name, cert_index_name[i], strlen(cert_index_name[i]));
	}

	for (i = 0; i < 1; i++) {
		memset(g_key_existence[i].object_name, 0, BUF_NAME_SIZE);
		memset(g_key_existence[i].key_name, 0, BUF_NAME_SIZE);
		g_key_existence[i].existence = 0;

		memcpy(g_key_existence[i].object_name, key_object_name[i], strlen(key_object_name[i]));
		memcpy(g_key_existence[i].key_name, key_index_name[i], strlen(key_index_name[i]));
	}
}

static int security_sdk_print_crt(int debug_mode, unsigned char *buf, unsigned int buf_len)
{
	int ret;
	int pos = 0;
	size_t len = 0;
	unsigned char *p = NULL;
	int buf_format = MBEDTLS_X509_FORMAT_DER;

	if (!buf) {
		ret = -1;
		goto exit;
	}

	mbedtls_x509_crt crt;
	mbedtls_x509_crt *t_crt;

	mbedtls_x509_crt_init(&crt);

	if (strstr((const char *)buf, "-----BEGIN CERTIFICATE-----") != NULL) {
		buf_format = MBEDTLS_X509_FORMAT_PEM;
	}

	if (buf_format == MBEDTLS_X509_FORMAT_DER) {

		p = (unsigned char *)buf;

		while (pos < buf_len) {
			p = (unsigned char *)buf + pos;
			ret = mbedtls_asn1_get_tag(&p, buf + buf_len, &len,
				MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);
			if (ret != 0) {
				goto exit;
			}
			if ((pos + len) < buf_len) {
				ret = mbedtls_x509_crt_parse(&crt, buf + pos, len + 4);
				if (ret != 0) {
					goto exit;
				}
			}
			pos += len + 4;
		}
	} else {
		ret = mbedtls_x509_crt_parse(&crt, buf, buf_len);
		if (ret != 0) {
			goto exit;
		}
	}

	t_crt = &crt;

	while (t_crt != NULL) {
		ret = mbedtls_x509_crt_info((char *)buf, buf_len, "  - ", t_crt);
		if (ret <= 0) {
			goto exit;
		}
		printf("\n%s\n", buf);
		t_crt = t_crt->next;
	}

	mbedtls_x509_crt_free(&crt);
	return 0;

exit:
	mbedtls_x509_crt_free(&crt);
	return ret;
}

static void security_sdk_factory_getcert(int debug_mode)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	int ret = 0, i = 0;
	unsigned char *cert = NULL;
	unsigned int cert_len = 0;
	unsigned char *buf = NULL;
	unsigned int buf_len = 0;

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	cert_len = 1024;
	cert = malloc(cert_len);

	for (i = 0; i < ARTIK_CERTS_NUM; i++) {
		printf("[%d] %s ... ", i, g_cert_existence[i].object_name);

		ret = security->get_certificate(handle, g_cert_existence[i].key_name,
				ARTIK_SECURITY_CERT_TYPE_PEM, (unsigned char **)&buf,
				(unsigned int *)&buf_len);
		if (ret != 0 || !buf) {
			printf("get certificate failed : %d\n", ret);

			if (buf) {
				free(buf);
				buf = NULL;
				buf_len = 0;
			}

			continue;
		}

		cert_len = 1024;
		memset(cert, 0, cert_len);
		memcpy(cert, buf, buf_len);

		if (buf) {
			free(buf);
			buf = NULL;
			buf_len = 0;
		}

		ret = security_sdk_print_crt(debug_mode, cert, cert_len);
		if (ret) {
			printf("parse failed : %x\n", -ret);
			continue;
		}

		g_cert_existence[i].existence = 1;

		printf("success\n");
	}

exit:
	if (cert) {
		free(cert);
		cert = NULL;
	}

	if (buf) {
		free(buf);
		buf = NULL;
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

}

static void security_sdk_factory_ecctest(int debug_mode)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	int i = 0, key_cnt = 0;
	int ret = 0;
	unsigned int hash_len[SEE_HASHNUM] = { 20, 32, 48, 64 };
	unsigned char hash[64];
	unsigned char *out = NULL;
	unsigned int outlen = 0;

	printf("--------------- sign/verify start --------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	while (key_cnt < 1) {
		printf("key_id: %s, objec_name: %s\n", g_key_existence[key_cnt].key_name
				, g_key_existence[key_cnt].object_name);
		while (i < SEE_HASHNUM) {
			printf("[%d] ARTIK ECC key test (hashlen %d) ...\n", i, hash_len[i]);

			ret = security->get_ecdsa_signature(handle, ECC_SEC_P256R1,
					g_key_existence[key_cnt].key_name, hash, hash_len[i], &out, &outlen);
			if (ret != 0) {
				printf("sign failed : %d\n", ret);
				break;
			}
			printf("sign success\n");

			ret += security->verify_ecdsa_signature(handle, ECC_SEC_P256R1,
					g_key_existence[key_cnt].key_name, hash, hash_len[i], out, outlen);
			if (ret != 0) {
				printf("verify failed : %d\n", ret);
				break;
			}
			printf("verify success\n");

			i++;

			if (out) {
				free(out);
				out = NULL;
			}
		}

		if (out) {
			free(out);
			out = NULL;
		}

		while (!ret) {
			printf("[%d] GET ARTIK PUBKEY ... ", i);

			ret += security->get_publickey(handle, ECC_SEC_P256R1,
					g_key_existence[key_cnt].key_name, &out, &outlen);
			if (ret != 0) {
				break;
			}
			printf("success\n");

			if (out) {
				free(out);
				out = NULL;
			}

			break;
		}
		if (ret) {
			printf("failed : %d\n", ret);
		} else {
			g_key_existence[key_cnt].existence = 1;
		}

		key_cnt++;
	}

exit:
	if (out) {
		free(out);
		out = NULL;
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}
}

void security_sdk_factory_key_test(int debug_mode)
{
	if (debug_mode < 0) {
		return;
	}

	printf("\n------------------------------------------\n");
	printf("TESTCASE : GET FACTORY cert\n");
	printf("------------------------------------------\n");

	security_sdk_factory_getcert(debug_mode);

	printf("\n------------------------------------------\n");
	printf("TESTCASE : ARTIK key\n");
	printf("------------------------------------------\n");

	security_sdk_factory_ecctest(debug_mode);
}

