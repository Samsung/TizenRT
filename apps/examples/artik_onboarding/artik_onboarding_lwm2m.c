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

/**
 * @file artik_onboarding_lwm2m.c
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/boardctl.h>

#include <artik_module.h>
#include <artik_http.h>
#include <artik_lwm2m.h>

#include "artik_onboarding.h"

static const char akc_root_ca[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
	"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
	"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
	"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
	"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
	"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
	"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
	"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
	"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
	"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
	"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
	"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
	"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
	"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
	"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
	"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
	"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
	"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
	"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
	"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
	"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
	"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
	"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
	"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
	"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
	"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
	"-----END CERTIFICATE-----\r\n";


#define OTA_FIRMWARE_HEADER_SIZE        4096
#define OTA_SIGNATURE_SIZE              3192
#define UUID_MAX_LEN                    64
#define LWM2M_RES_DEVICE_REBOOT         "/3/0/4"
#define LWM2M_RETRY_PERIOD              5
#define PKCS7_BEGIN                     "-----BEGIN PKCS7-----\n"
#define PKCS7_END                       "-----END PKCS7-----\n"
#define END_CERT                        "-----END CERTIFICATE-----\n"

enum ota_error {
	OTA_ERROR_SUCCESS = 0,
	OTA_ERROR_VERIFY,
};

struct ota_signature {
	artik_security_handle handle;
	artik_time signing_time;
	char data[OTA_SIGNATURE_SIZE];
	size_t offset;
	bool found;
};

struct ota_header {
	char data[OTA_FIRMWARE_HEADER_SIZE];
	size_t offset;
};

struct ota_info {
	struct ota_header header;
	struct ota_signature sig;
	enum ota_error error;
	int fd;
};

typedef struct {
	artik_list node;
	mbedtls_md_context_t md_ctx;
	mbedtls_asn1_buf data_digest;
	mbedtls_asn1_buf signed_digest;
	mbedtls_asn1_buf message_data;
	mbedtls_md_type_t digest_alg_id;

	mbedtls_x509_crt *chain;
	mbedtls_x509_crt *cert;
} verify_node;

typedef struct {
	mbedtls_x509_time signing_time;
	mbedtls_asn1_buf digest;
	mbedtls_asn1_buf raw;
} authenticated_attributes;

typedef struct {
	mbedtls_x509_name issuer;
	mbedtls_x509_buf serial;
	mbedtls_md_type_t digest_alg_id;
	authenticated_attributes authenticated_attributes;
	mbedtls_asn1_buf encrypted_digest;
} signer_info;

typedef struct {
	artik_list node;
	mbedtls_md_type_t md_alg_id;
} digest_algo_id;

typedef struct {
	mbedtls_x509_crt *chain;
	mbedtls_x509_crt *cert;
	signer_info signer;
} signed_data;

static artik_lwm2m_handle g_lwm2m_handle;
static struct ota_info *g_ota_info;

struct Lwm2mConfig lwm2m_config;

static void restart_lwm2m(void)
{
	sleep(LWM2M_RETRY_PERIOD);
	StartLwm2m(true, NULL);
}

static void on_error(void *data, void *user_data)
{
	fprintf(stderr, "LWM2M error, trying to reconnect in %d seconds\n",
			LWM2M_RETRY_PERIOD);
	StartLwm2m(false, restart_lwm2m);
}

static pthread_addr_t delayed_reboot(pthread_addr_t arg)
{
	sleep(3);
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);

	return NULL;
}

static void reboot(void)
{
	pthread_t tid;

	if (pthread_create(&tid, NULL, delayed_reboot, NULL) != 0) {
		return;
	}
	pthread_detach(tid);

	fprintf(stderr, "Rebooting in 3 seconds\n");
}

static void on_execute_resource(void *data, void *user_data)
{
	char *uri = (char *)data;

	fprintf(stderr, "LWM2M resource execute: %s\n", uri);

	if (!strncmp(uri, LWM2M_RES_DEVICE_REBOOT, strlen(LWM2M_RES_DEVICE_REBOOT))) {
		reboot();
	}

	if (!strncmp(uri, ARTIK_LWM2M_URI_FIRMWARE_UPDATE, ARTIK_LWM2M_URI_LEN)) {
		artik_lwm2m_module *lwm2m = NULL;

		lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
		if (!lwm2m) {
			fprintf(stderr, "Failed to request LWM2M module\n");
			return;
		}
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT));
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_UPDATING,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_UPDATING));

		int fd = open("/dev/mtdblock7", O_RDWR);

		if (fd < 0) {
			fprintf(stderr, "Open failed\n");
			return;
		}

		if (write(fd, g_ota_info->header.data, OTA_FIRMWARE_HEADER_SIZE) < 0) {
			fprintf(stderr, "Write failed\n");
			close(fd);
			return;
		}
		close(fd);

		lwm2m_config.is_ota_update = 1;
		SaveConfiguration();
		reboot();
	}
}

static char *locate_substring(const char *big, const char *token, size_t len)
{
	size_t tokenlen = strlen(token);
	const char *p;

	if (tokenlen == 0) {
		return (char *)big;
	}

	for (p = big; *p && (p + tokenlen <= big + len); p++) {
		if ((*p == *token) && (strncmp(p, token, tokenlen) == 0)) {
			return (char *)p;
		}
	}

	return NULL;
}

/*
 * Compare two X.509 Names (aka rdnSequence).
 *
 * See RFC 5280 section 7.1, though we don't implement the whole algorithm:
 * we sometimes return unequal when the full algorithm would return equal,
 * but never the other way. (In particular, we don't do Unicode normalisation
 * or space folding.)
 *
 * Return 0 if equal, -1 otherwise.
 */
static int x509_name_cmp(const mbedtls_x509_name *a, const mbedtls_x509_name *b)
{
	/* Avoid recursion, it might not be optimised by the compiler */
	while (a != NULL || b != NULL) {
		if (a == NULL || b == NULL)
			return -1;

		/* type */
		if (a->oid.tag != b->oid.tag
			|| a->oid.len != b->oid.len
			|| memcmp(a->oid.p, b->oid.p, b->oid.len) != 0)
			return -1;

		/* value */
		if (a->val.tag != b->val.tag
			|| a->val.len != b->val.len
			|| memcmp(a->val.p, b->val.p, b->val.len) != 0)
			return -1;

		/* structure of the list of sets */
		if (a->next_merged != b->next_merged)
			return -1;

		a = a->next;
		b = b->next;
	}

	/* a == NULL == b */
	return 0;
}

static int check_pkcs7_validity(signed_data *sig_data)
{
	mbedtls_x509_crt *cert = sig_data->chain;

	while (cert != NULL) {
		if (x509_name_cmp(&sig_data->signer.issuer, &cert->issuer) != 0) {
			char info[1024];

			mbedtls_x509_dn_gets(info, 1024, &cert->issuer);
			fprintf(stderr, "Issuer is: %s", info);

			mbedtls_x509_dn_gets(info, 1024, &sig_data->signer.issuer);
			fprintf(stderr, "Expected issuer is: %s", info);
			cert = cert->next;
			continue;
		}

		if (sig_data->signer.serial.len == cert->issuer_id.len
			&& memcmp(sig_data->signer.serial.p,
					  cert->issuer_id.p, cert->issuer_id.len) == 0) {
			fprintf(stderr, "Issuer serial number does not match.");
			cert = cert->next;
			continue;
		}

		if ((cert->ext_types & MBEDTLS_X509_EXT_EXTENDED_KEY_USAGE) == 0) {
			fprintf(stderr, "Extended key usage extension not found.");
			cert = cert->next;
			continue;
		}

		if (mbedtls_x509_crt_check_extended_key_usage(
				cert,
				MBEDTLS_OID_CODE_SIGNING,
				MBEDTLS_OID_SIZE(MBEDTLS_OID_CODE_SIGNING)) != 0) {
			fprintf(stderr, "Signer certificate verification failed: The purpose of the certificate is not digitalSignature.");
			cert = cert->next;
			continue;
		}

		break;
	}

	if (cert == NULL) {
		fprintf(stderr, "Issuer certificate not found.");
		return E_SECURITY_INVALID_PKCS7;
	}

	if (sig_data->signer.digest_alg_id != MBEDTLS_MD_SHA256) {
		fprintf(stderr, "Only verification with SHA256 is supported.");
		return E_NOT_SUPPORTED;
	}

	sig_data->cert = cert;
	return S_OK;
}

static int initialize_md_context(verify_node *node, signed_data *sig_data)
{
	const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(sig_data->signer.digest_alg_id);

	if (!md_info) {
		fprintf(stderr, "SHA256 is not supported by mbedtls.");
		return E_SECURITY_INVALID_PKCS7;
	}

	mbedtls_md_init(&node->md_ctx);
	if (mbedtls_md_setup(&node->md_ctx, md_info, 0) != 0) {
		fprintf(stderr, "Failed to initialize digest context.");
		return E_BAD_ARGS;
	}

	if (mbedtls_md_starts(&node->md_ctx) != 0) {
		fprintf(stderr, "Failed to prepare digest context.");
		mbedtls_md_free(&node->md_ctx);
		return E_BAD_ARGS;
	}

	return S_OK;
}

static int copy_node_data(verify_node *node, signed_data *sig_data)
{
	size_t data_digest_len =  sig_data->signer.authenticated_attributes.digest.len;
	size_t signed_digest_len = sig_data->signer.encrypted_digest.len;
	size_t message_data_len = sig_data->signer.authenticated_attributes.raw.len;

	node->data_digest.p = malloc(sizeof(unsigned char) * data_digest_len);
	node->signed_digest.p = malloc(sizeof(unsigned char) * signed_digest_len);
	node->message_data.p = malloc(sizeof(unsigned char) * message_data_len);

	if (!node->data_digest.p || !node->signed_digest.p || !node->message_data.p) {
		if (node->data_digest.p)
			free(node->data_digest.p);

		if (node->signed_digest.p)
			free(node->signed_digest.p);

		if (node->message_data.p)
			free(node->message_data.p);

		return E_NO_MEM;
	}

	node->data_digest.len = data_digest_len;
	node->signed_digest.len = signed_digest_len;
	node->message_data.len = message_data_len;

	memcpy(node->data_digest.p, sig_data->signer.authenticated_attributes.digest.p, data_digest_len);
	memcpy(node->signed_digest.p, sig_data->signer.encrypted_digest.p, signed_digest_len);
	memcpy(node->message_data.p, sig_data->signer.authenticated_attributes.raw.p, message_data_len);
	*(node->message_data.p) = 0x31;

	return S_OK;
}

static int write_firmware(char *data, size_t len, void *user_data)
{
	int header_size = 0;
	int sig_size = 0;
	char *end = NULL;

	if (g_ota_info->error != OTA_ERROR_SUCCESS)
		return len;

	if (lwm2m_config.ota_signature_verification && !g_ota_info->sig.found) {
		if (g_ota_info->sig.offset == 0 && !locate_substring(data, PKCS7_BEGIN, len)) {
			fprintf(stderr, "Failed to download firmware: Signature not found\n");
			g_ota_info->error = OTA_ERROR_VERIFY;
			return len;
		}

		end = locate_substring(data, PKCS7_END, len);
		if (!end) {
			sig_size = len;
		} else {
			g_ota_info->sig.found = true;
			sig_size = end + strlen(PKCS7_END) - data;
		}

		if (sig_size + g_ota_info->sig.offset + 1 > OTA_SIGNATURE_SIZE) {
			g_ota_info->error = OTA_ERROR_VERIFY;
			fprintf(stderr, "Failed to download firmware: The signature is too long\n");
			return len;
		}

		memcpy(g_ota_info->sig.data + g_ota_info->sig.offset, data, sig_size);
		len -= sig_size;
		g_ota_info->sig.offset += sig_size;

		if (g_ota_info->sig.found) {
			artik_list *ca_chain = NULL;
			artik_time *signing_time_in = &lwm2m_config.signing_time;
			artik_error err;
			artik_security_handle handle;
			artik_security_module *security = artik_request_api_module("security");

			g_ota_info->sig.data[g_ota_info->sig.offset + sig_size] = '\0';
			security->request(&handle);
			security->get_certificate_pem_chain(handle, "ARTIK", &ca_chain);

			if (lwm2m_config.signing_time.month == 0 && lwm2m_config.signing_time.year == 0 &&
				lwm2m_config.signing_time.day == 0 && lwm2m_config.signing_time.minute == 0 &&
				lwm2m_config.signing_time.hour == 0 && lwm2m_config.signing_time.second == 0)
				signing_time_in = NULL;

			err = security->verify_signature_init(&g_ota_info->sig.handle,
					g_ota_info->sig.data,
					(char *)artik_list_get_by_pos(ca_chain, 2)->data,
					signing_time_in, &g_ota_info->sig.signing_time);
			if (err != S_OK) {
				artik_list_delete_all(&ca_chain);
				artik_release_api_module(security);
				fprintf(stderr, "Verify signature failed: %s", error_msg(err));
				g_ota_info->error = OTA_ERROR_VERIFY;
				return len;
			}

			artik_list_delete_all(&ca_chain);
			artik_release_api_module(security);
		}
	}

	if (len > 0 && OTA_FIRMWARE_HEADER_SIZE - g_ota_info->header.offset > 0) {
		size_t remaining_header_size = OTA_FIRMWARE_HEADER_SIZE - g_ota_info->header.offset;

		header_size = len > remaining_header_size ? remaining_header_size : len;
		memcpy(g_ota_info->header.data + g_ota_info->header.offset, data + sig_size, header_size);

		if (lwm2m_config.ota_signature_verification) {
			artik_error err;
			artik_security_module *security = artik_request_api_module("security");

			err = security->verify_signature_update(
				g_ota_info->sig.handle,
				(unsigned char *)(g_ota_info->header.data + g_ota_info->header.offset),
				header_size);
			if (err != S_OK) {
				fprintf(stderr, "Verify signature failed: %s", error_msg(err));
				g_ota_info->error = OTA_ERROR_VERIFY;
				return len;
			}
		}

		len -= header_size;
		g_ota_info->header.offset += header_size;

		printf("Skip OTA header (header_size %d, len %d, remaining_header_size %d)\n",
			   header_size, len, remaining_header_size);
	}

	if (len > 0) {
		if (write(g_ota_info->fd, data + sig_size + header_size, len) < 0) {
			fprintf(stderr, "Write failed\n");
			return -1;
		}

		if (lwm2m_config.ota_signature_verification) {
			artik_error err;
			artik_security_module *security = artik_request_api_module("security");

			err = security->verify_signature_update(
				g_ota_info->sig.handle,
				(unsigned char *)(data + sig_size + header_size),
				len);
			if (err != S_OK) {
				fprintf(stderr, "Verify signature failed: %s", error_msg(err));
				g_ota_info->error = OTA_ERROR_VERIFY;
				return len;
			}
		}

	}

	return len;
}

static int download_firmware(int argc, char *argv[])
{
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_security_module *security;
	artik_ssl_config ssl_conf;
	int status = 0;

	artik_error ret;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{ "User-Agent", "Artik Firmware Updater"}
	};

	g_ota_info = malloc(sizeof(struct ota_info));
	memset(g_ota_info, 0, sizeof(struct ota_info));

	if (!lwm2m) {
		fprintf(stderr, "Failed to request LWM2M module\n");
		return 1;
	}

	if (!http) {
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
		artik_release_api_module(lwm2m);
		free(g_ota_info);
		return 1;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);
	memset(&ssl_conf, 0, sizeof(artik_ssl_config));
	ssl_conf.verify_cert = ARTIK_SSL_VERIFY_NONE;
	g_ota_info->fd = open("/dev/mtdblock7", O_RDWR);
	if (g_ota_info->fd < 0) {
		fprintf(stderr, "Open failed\n");
		return 1;
	}
	if (lseek(g_ota_info->fd, 4096, SEEK_SET) < 0) {
		fprintf(stderr, "Seek failed\n");
		close(g_ota_info->fd);
		free(g_ota_info);
		return 1;
	}
	ret = http->get_stream(argv[1], &headers, &status, write_firmware, NULL, &ssl_conf);
	if (ret != S_OK) {
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
		artik_release_api_module(lwm2m);
		artik_release_api_module(http);
		close(g_ota_info->fd);
		free(g_ota_info);
		return 1;
	}

	if (g_ota_info->error != OTA_ERROR_SUCCESS) {
		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_PKG_ERR,
			strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_PKG_ERR));

		lwm2m->client_write_resource(
			g_lwm2m_handle,
			ARTIK_LWM2M_URI_FIRMWARE_STATE,
			(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
			strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
		artik_release_api_module(lwm2m);
		artik_release_api_module(http);
		close(g_ota_info->fd);
		free(g_ota_info);
		return 1;
	}

	if (lwm2m_config.ota_signature_verification) {
		security = artik_request_api_module("security");
		ret = security->verify_signature_final(g_ota_info->sig.handle);
		if (ret != S_OK) {
			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_CRC_ERR,
				strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_CRC_ERR));

			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
			artik_release_api_module(lwm2m);
			artik_release_api_module(http);
			artik_release_api_module(security);
			close(g_ota_info->fd);
			free(g_ota_info);
			return 1;
		}
		memcpy(&lwm2m_config.signing_time_tmp, &g_ota_info->sig.signing_time, sizeof(artik_time));
		SaveConfiguration();
		printf("Signature OK\n");
	}

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS,
		strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS));

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_STATE,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADED,
		strlen(ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADED));
	artik_release_api_module(lwm2m);
	artik_release_api_module(http);
	close(g_ota_info->fd);
	return 0;
}

static void on_changed_resource(void *data, void *user_data)
{
	artik_lwm2m_resource_t *res = (artik_lwm2m_resource_t *)data;

	fprintf(stderr, "LWM2M resource changed: %s\n", res->uri);
	if (!strncmp(res->uri, ARTIK_LWM2M_URI_FIRMWARE_PACKAGE_URI, ARTIK_LWM2M_URI_LEN)) {
		artik_lwm2m_module *lwm2m = NULL;
		char *firmware_uri;
		char *argv[2] = { NULL };

		lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
		if (!lwm2m) {
			fprintf(stderr, "Failed to request LWM2M module\n");
			return;
		}

		/* Initialize attribute "Update Result" */
		lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT,
				strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT));

		/* Change attribute "State" to "Downloading" */
		lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADING,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADING));

		/* The FW URI is empty, come back to IDLE */
		if (res->length == 0) {
			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
			return;
		}


		/* The FW URI is not valid */
		if (res->length > 255) {
			fprintf(stderr, "ERROR: Unable to retrieve firmware package uri\n");

			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR,
				strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR));

			lwm2m->client_write_resource(
				g_lwm2m_handle,
				ARTIK_LWM2M_URI_FIRMWARE_STATE,
				(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
				strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));

			return;
		}

		firmware_uri = strndup((char *)res->buffer, res->length);
		fprintf(stdout, "Downloading firmware from %s\n", firmware_uri);
		argv[0] = firmware_uri;
		task_create("download-firmware", 80, 16384, download_firmware, argv);
		artik_release_api_module(lwm2m);
	}
}

static void finish_ota(void)
{
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");

	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS,
		strlen(ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS));
	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_FIRMWARE_STATE,
		(unsigned char *)ARTIK_LWM2M_FIRMWARE_STATE_IDLE,
		strlen(ARTIK_LWM2M_FIRMWARE_STATE_IDLE));
	lwm2m->client_write_resource(
		g_lwm2m_handle,
		ARTIK_LWM2M_URI_DEVICE_FW_VERSION,
		(unsigned char *)ONBOARDING_VERSION,
		strlen(ONBOARDING_VERSION));

	lwm2m_config.is_ota_update = 0;
	if (lwm2m_config.ota_signature_verification) {
		memcpy(&lwm2m_config.signing_time, &lwm2m_config.signing_time_tmp, sizeof(artik_time));
	}
	SaveConfiguration();
}

static pthread_addr_t lwm2m_start_cb(void *arg)
{
	artik_error ret = S_OK;
	artik_lwm2m_config config;
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
	artik_ssl_config ssl_config;
	on_lwm2m_start_cb callback = (on_lwm2m_start_cb)arg;
	artik_secure_element_config se_config = {
		ARTIK_DEVICE_CERT_ID,
		ECC_SEC_P256R1
	};

	if (!lwm2m) {
		printf("Failed to request lwm2m module\n");
		ret = E_NOT_SUPPORTED;
		pthread_exit((void *)ret);
		return NULL;
	}

	memset(&config, 0, sizeof(artik_lwm2m_config));
	memset(&ssl_config, 0, sizeof(artik_ssl_config));
	config.server_id = 123;
	config.server_uri = "coaps+tcp://coaps-api.artik.cloud:5689";
	config.lifetime = 30;
	config.connect_timeout = 15;
	config.name = strndup(cloud_config.device_id, UUID_MAX_LEN);
	config.tls_psk_identity = config.name;
	config.tls_psk_key = strndup(cloud_config.device_token, UUID_MAX_LEN);
	config.objects[ARTIK_LWM2M_OBJECT_DEVICE] =
		lwm2m->create_device_object("Samsung", "ARTIK05x", "1234567890", ONBOARDING_VERSION,
			"1.0", "1.0", "A05x", 0, 5000, 1500, 100, 1000000, 200000,
			"Europe/Paris", "+01:00", "U");

	if (CloudIsSecureDeviceType()) {
		artik_security_handle handle;
		artik_security_module *security = NULL;

		ssl_config.se_config = &se_config;
				security = (artik_security_module *)artik_request_api_module("security");
		ret = security->request(&handle);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to load security module (err=%d)\n", ret);
			artik_release_api_module(handle);
			goto exit;
		}

		ret = security->get_certificate(
			handle, se_config.key_id, ARTIK_SECURITY_CERT_TYPE_PEM,
			(unsigned char **)&ssl_config.client_cert.data, &ssl_config.client_cert.len);
		if (ret != S_OK || !ssl_config.client_cert.data || ssl_config.client_cert.len == 0) {
			fprintf(stderr, "Failed to get device certificate (err=%d)\n", ret);
			artik_release_api_module(handle);
			goto exit;
		}
	}

	ssl_config.ca_cert.data = (char *)akc_root_ca;
	ssl_config.ca_cert.len = sizeof(akc_root_ca);
	ssl_config.verify_cert = ARTIK_SSL_VERIFY_REQUIRED;
	config.ssl_config = &ssl_config;
	if (!config.objects[ARTIK_LWM2M_OBJECT_DEVICE]) {
		fprintf(stderr, "Failed to allocate memory for object device.");
		ret = E_NO_MEM;
		goto exit;
	}

	config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE] =
		lwm2m->create_firmware_object(false, "Artik/TizenRT/OnBoarding", ONBOARDING_VERSION);
	if (!config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE]) {
		fprintf(stderr, "Failed to allocate memory for object firmware.");
		ret = E_NO_MEM;
		goto exit;
	}

	ret = lwm2m->client_request(&g_lwm2m_handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request LWM2M client handle (%d)\n", ret);
		goto exit;
	}

	ret = lwm2m->client_connect(g_lwm2m_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to connect to the DM server (%d)\n", ret);
		goto exit;
	}

	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_ERROR,
			on_error, (void *)g_lwm2m_handle);
	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_DISCONNECT,
			on_error, (void *)g_lwm2m_handle);
	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_RESOURCE_EXECUTE,
			on_execute_resource, (void *)g_lwm2m_handle);
	lwm2m->set_callback(g_lwm2m_handle, ARTIK_LWM2M_EVENT_RESOURCE_CHANGED,
			on_changed_resource, (void *)g_lwm2m_handle);

	if (lwm2m_config.is_ota_update) {
		finish_ota();
	}

	if (callback)
		callback();

exit:
	if (config.objects[ARTIK_LWM2M_OBJECT_DEVICE])
		lwm2m->free_object(config.objects[ARTIK_LWM2M_OBJECT_DEVICE]);

	if (config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE])
		lwm2m->free_object(config.objects[ARTIK_LWM2M_OBJECT_FIRMWARE]);

	if (config.name)
		free(config.name);

	if (config.tls_psk_key)
		free(config.tls_psk_key);

	if (g_lwm2m_handle && ret != S_OK) {
		lwm2m->client_release(g_lwm2m_handle);
		g_lwm2m_handle = NULL;
	}

	if (ssl_config.client_cert.data)
		free(ssl_config.client_cert.data);

	pthread_exit((void *)ret);

	return NULL;
}

static pthread_addr_t lwm2m_stop_cb(void *arg)
{
	artik_error ret = S_OK;
	artik_lwm2m_module *lwm2m = (artik_lwm2m_module *)artik_request_api_module("lwm2m");
	on_lwm2m_start_cb callback = (on_lwm2m_start_cb)arg;

	if (!lwm2m) {
		printf("Failed to request lwm2m module\n");
		ret = E_NOT_SUPPORTED;
		pthread_exit((void *)ret);
		return NULL;
	}

	if (!g_lwm2m_handle) {
		return NULL;
	}

	lwm2m->client_disconnect(g_lwm2m_handle);
	lwm2m->client_release(g_lwm2m_handle);
	g_lwm2m_handle = NULL;

	if (callback)
		callback();

	pthread_exit((void *)ret);

	return NULL;
}

artik_error StartLwm2m(bool start, on_lwm2m_start_cb cb)
{
	artik_error ret = S_OK;

	if (start) {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;

		if (g_lwm2m_handle) {
			fprintf(stderr, "LWM2M is already started, stop it first\n");
			ret = E_BUSY;
			goto exit;
		}

		printf("Starting LWM2M connection\n");

retry:
		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 8);
		status = pthread_create(&tid, &attr, lwm2m_start_cb, (void *)cb);
		if (status) {
			printf("Failed to create thread for starting lwm2m\n");
			ret = E_NO_MEM;
			goto exit;
		}

		pthread_setname_np(tid, "onboarding-lwm2m-start");

		if (cb) {
			pthread_detach(tid);
			goto exit;
		}

		pthread_join(tid, (void **)&ret);

		if (ret != S_OK) {
			printf("Failed to connect to lwm2m (%d), retrying in %d seconds\n",
					ret, LWM2M_RETRY_PERIOD);
			sleep(LWM2M_RETRY_PERIOD);
			goto retry;
		}
	} else {
		static pthread_t tid;
		pthread_attr_t attr;
		int status;
		struct sched_param sparam;

		if (!g_lwm2m_handle) {
			fprintf(stderr, "LWM2M is already stopped, start it first\n");
			ret = E_NOT_INITIALIZED;
			goto exit;
		}

		pthread_attr_init(&attr);
		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		status = pthread_attr_setstacksize(&attr, 1024 * 8);
		status = pthread_create(&tid, &attr, lwm2m_stop_cb, (void *)cb);
		pthread_attr_destroy(&attr);
		if (status) {
			printf("Failed to create thread for stopping lwm2m\n");
			ret = E_NO_MEM;
			goto exit;
		}

		pthread_setname_np(tid, "onboarding-lwm2m-stop");

		if (cb) {
			pthread_detach(tid);
			goto exit;
		}

		pthread_join(tid, (void **)&ret);
	}

exit:
	return ret;
}

void Lwm2mResetConfig(bool force)
{
	if (force) {
		lwm2m_config.ota_signature_verification = 1;
	}

	lwm2m_config.is_ota_update = 0;
	memset(&lwm2m_config.signing_time, 0, sizeof(artik_time));
	memset(&lwm2m_config.signing_time_tmp, 0, sizeof(artik_time));
}
