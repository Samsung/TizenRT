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
#include <tinyara/config.h>
#include <stdlib.h>
#include <security/security_api.h>
#include <mbedtls/x509_crt.h>

#include "sss_storage.h"

#define SUBJECT_NAME_LEN	128

static int parse_crt_chain(mbedtls_x509_crt *crt, unsigned char *buf, unsigned int buf_len)
{
	int r = 0;
	int pos = 0;
	size_t len = 0;
	unsigned char *p = NULL;
	int buf_format = MBEDTLS_X509_FORMAT_DER;

	if (!buf) {
		return -1;
	}

	if (strstr((const char *)buf, "-----BEGIN CERTIFICATE-----") != '\0') {
		buf_format = MBEDTLS_X509_FORMAT_PEM;
	}

	if (buf_format == MBEDTLS_X509_FORMAT_DER) {
		p = (unsigned char *)buf;

		while (pos < buf_len) {
			p = (unsigned char *)buf + pos;
			r = mbedtls_asn1_get_tag(&p, buf + buf_len, &len,
				MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);
			if (r != 0) {
				return r;
			}
			if ((pos + len) < buf_len) {
				r = mbedtls_x509_crt_parse(crt, buf + pos, len + 4);
				if (r != 0) {
					return r;
				}
			}
			pos += len + 4;
		}
	} else {
		r = mbedtls_x509_crt_parse(crt, buf, buf_len);
		if (r != 0) {
			return r;
		}
	}

	return r;
}

static int get_crt_subject_uuid(unsigned char *uuid, unsigned int *uuid_len,
				unsigned char *buf, unsigned int buf_len)
{
	int r = 0;
	char subname[SUBJECT_NAME_LEN];
	unsigned int len;
	char *p1 = NULL, *p2 = NULL;
	mbedtls_x509_crt crt;
	mbedtls_x509_crt *t_crt;

	mbedtls_x509_crt_init(&crt);

	r = parse_crt_chain(&crt, buf, buf_len);
	if (r) {
		printf("Fail to parse certificate %d\n", r);
		goto exit;
	}

	t_crt = &crt;
	while (t_crt != NULL) {
		r = mbedtls_x509_dn_gets(subname, sizeof(subname), &t_crt->subject);
		if (r < 0 || r >= sizeof(subname)) {
			printf("Fail to get dn %d\n", r);
			r = MBEDTLS_ERR_X509_BUFFER_TOO_SMALL;
			goto exit;
		}
		p1 = (char *)strchr((const char *)subname, '(');
		if (p1 != NULL) {
			break;
		}
		t_crt = t_crt->next;
	}
	if (p1 == NULL) {
		goto exit;
	}
	p1++;

	p2 = (char *)strchr((const char *)subname, ')');

	len = p2 - p1;

	memcpy(uuid, p1, len);

	uuid[len] = '\0';
	*uuid_len = len;

	r = 0;
exit:
	mbedtls_x509_crt_free(&crt);

	return r;
}

// TBD: this should be fixed to change to support other SE.
#define ST_FACTORYKEY_ARTIK_CERT (0x00010122)

int get_artik_crt_uuid(unsigned char *uuid, unsigned int *uuid_len)
{
	int r;
	unsigned char *buf = NULL;
	unsigned int buf_len = SEE_MAX_BUF_SIZE;

	buf = (unsigned char *)malloc(SEE_MAX_BUF_SIZE);
	if (buf == NULL) {
		return -1;
	}

	security_handle hnd;
	security_error err = security_init(&hnd);
	if (err != SECURITY_OK) {
		printf("Write SS fail %d\n", err);
		free(buf);
		return -1;
	}
	char auth_name[7] = {0,};
	snprintf(auth_name, 7, "ss/%d", ST_FACTORYKEY_ARTIK_CERT);

	security_data data = {buf, buf_len};

	err = auth_get_certificate(hnd, auth_name, &data);
	if (err != SECURITY_OK) {
		free(buf);
		security_deinit(hnd);
		return -1;
	}
	security_deinit(hnd);

	r = get_crt_subject_uuid(uuid, uuid_len, buf, buf_len);
	if (r) {
		printf("Fail to get subject uuid %d\n", r);
		goto exit;
	}

exit:
	if (buf) {
		free(buf);
	}

	return r;
}
