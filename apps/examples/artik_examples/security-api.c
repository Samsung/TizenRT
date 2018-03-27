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
 * @file security-api.c
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <shell/tash.h>
#include <unistd.h>

#include <artik_module.h>
#include <artik_security.h>

#include <tls/x509_crt.h>
#include <tls/pk.h>
#include <tls/pem.h>
#include <tls/oid.h>

#include "command.h"

static int security_cert(int argc, char *argv[]);
static int security_pk(int argc, char *argv[]);
static int security_chain(int argc, char *argv[]);
static int security_rand(int argc, char *argv[]);
static int security_serial(int argc, char *argv[]);

const struct command security_commands[] = {
	{ "cert", "cert <der|pem> - Display certificate stored in SE (default: pem format)", security_cert },
	{ "pk",
	  "pk <der|pem> - Display public key extracted from the certificate stored in SE (default: pem format)",
	  security_pk },
	{ "chain", " Display root CA extracted and intermediate certificates from the certificate stored in SE", security_chain },
	{ "rand", "rand <num> - Generate <num> random bytes from the SE", security_rand },
	{ "serial", "Return the Serial Number contained in the certificate stored in SE", security_serial },
	{ "", "", NULL }
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

static int security_cert(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_error err = S_OK;
	artik_security_handle handle = NULL;
	unsigned char *se_cert = NULL;
	unsigned int se_cert_size = 0;
	char *se_cert_pem = NULL;
	artik_security_cert_type_t fmt = ARTIK_SECURITY_CERT_TYPE_PEM;

	if (argc > 3) {
		if (strcmp(argv[3], "der") == 0) {
			fmt = ARTIK_SECURITY_CERT_TYPE_DER;
		} else if (strcmp(argv[3], "pem") == 0) {
			fmt = ARTIK_SECURITY_CERT_TYPE_PEM;
		} else {
			fprintf(stderr, "Unknown certificate format %s\n", argv[3]);
			return -1;
		}
	}

	security = (artik_security_module *)artik_request_api_module("security");
	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	err = security->get_certificate(handle, ARTIK_DEVICE_CERT_ID, fmt, &se_cert,
			&se_cert_size);
	if (err != S_OK || !se_cert || !se_cert_size) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	switch (fmt) {
	case ARTIK_SECURITY_CERT_TYPE_DER:
		output_buffer(se_cert, se_cert_size);
		break;
	default:
		se_cert_pem = strndup((char *)se_cert, se_cert_size);
		if (!se_cert_pem) {
			fprintf(stderr, "Failed to duplicate certificate\n");
			goto exit;
		}
		fprintf(stdout, se_cert_pem);
		free(se_cert_pem);
		break;
	}

exit:
	if (se_cert)
		free(se_cert);
	if (handle)
		security->release(handle);
	artik_release_api_module(security);

	return ret;
}

static int security_pk(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	unsigned char *se_cert = NULL;
	unsigned int se_cert_size = 0;
	char *se_cert_pem = NULL;
	char *se_pk_pem = NULL;
	unsigned char *se_pk_der = NULL;
	unsigned int se_pk_der_size = 0;
	artik_security_cert_type_t fmt = ARTIK_SECURITY_CERT_TYPE_PEM;

	security = (artik_security_module *)artik_request_api_module("security");
	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	if (argc > 3) {
		if (strcmp(argv[3], "der") == 0) {
			fmt = ARTIK_SECURITY_CERT_TYPE_DER;
		} else if (strcmp(argv[3], "pem") == 0) {
			fmt = ARTIK_SECURITY_CERT_TYPE_PEM;
		} else {
			fprintf(stderr, "Unknown certificate format %s\n", argv[3]);
			return -1;
		}
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	err = security->get_certificate(handle, ARTIK_DEVICE_CERT_ID,
			ARTIK_SECURITY_CERT_TYPE_PEM,  &se_cert, &se_cert_size);
	if (err != S_OK || !se_cert || !se_cert_size) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	se_cert_pem = strndup((char *)se_cert, se_cert_size);
	free(se_cert);
	if (!se_cert_pem) {
		fprintf(stderr, "Failed to duplicate certificate\n");
		goto exit;
	}

	err = security->get_ec_pubkey_from_cert(se_cert_pem, &se_pk_pem);
	if (err != S_OK || !se_pk_pem) {
		fprintf(stderr, "Failed to get public key (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	switch (fmt) {
	case ARTIK_SECURITY_CERT_TYPE_DER:
		err = security->convert_pem_to_der(se_pk_pem, &se_pk_der,
				&se_pk_der_size);
		if ((err != S_OK) || !se_pk_der || !se_pk_der_size) {
			fprintf(stderr, "Failed to convert PEM public key to DER (err=%d)\n", err);
			ret = -1;
			goto exit;
		}
		output_buffer(se_pk_der, se_pk_der_size);
		free(se_pk_der);
		break;
	default:
		fprintf(stdout, se_pk_pem);
		break;
	}

exit:
	if (handle)
		security->release(handle);
	if (se_pk_pem)
		free(se_pk_pem);
	if (se_cert_pem)
		free(se_cert_pem);
	artik_release_api_module(security);

	return ret;
}

static int security_chain(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	artik_list *chain = NULL;
	int i = 0;
	int ret = 0;
	char cert_name[16] = "ARTIK";

	if (argc > 3)
		strncpy(cert_name, argv[3], sizeof(cert_name));

	security = (artik_security_module *)artik_request_api_module("security");
	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	err = security->get_certificate_pem_chain(handle, cert_name, &chain);
	if (err != S_OK || !chain || !artik_list_size(chain)) {
		fprintf(stderr, "Failed to get CA chain (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < artik_list_size(chain); i++)
		fprintf(stdout, "%s\n", (char *)artik_list_get_by_pos(chain, i)->data);

	artik_list_delete_all(&chain);

exit:
	if (handle)
		security->release(handle);
	artik_release_api_module(security);

	return ret;
}

static int security_rand(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	unsigned char *rand_bytes = NULL;
	int num_bytes = 0;
	int i = 0;

	if (argc < 4) {
		fprintf(stderr, "Wrong number of arguments\n");
		usage(argv[1], security_commands);
		return -1;
	}

	security = (artik_security_module *)artik_request_api_module("security");
	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	num_bytes = atoi(argv[3]);
	if (num_bytes > 64) {
		fprintf(stderr, "Num size should be less than or equal to 64\n");
		ret = -1;
		goto exit;
	}

	if (num_bytes < 0) {
		fprintf(stderr, "Uncountable number\n");
		ret = -1;
		goto exit;
	}

	err = security->get_random_bytes(handle, num_bytes, &rand_bytes);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get random bytes (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < num_bytes; i++)
		fprintf(stdout, "%02x ", rand_bytes[i]);

	fprintf(stdout, "\n");

exit:
	if (rand_bytes) {
		free(rand_bytes);
	}

	if (handle)
		security->release(handle);
	artik_release_api_module(security);

	return ret;
}

static int security_serial(int argc, char *argv[])
{
	artik_security_handle handle = NULL;
	int ret = 0;
	artik_error err = S_OK;
	unsigned char *se_cert = NULL;
	unsigned int se_cert_size = 0;
	char *se_cert_pem = NULL;
	unsigned char serial_number[ARTIK_CERT_SN_MAXLEN] = "";
	unsigned int len = ARTIK_CERT_SN_MAXLEN;
	int i = 0;
	artik_security_module *security = NULL;

	security = (artik_security_module *)artik_request_api_module("security");
	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	err = security->get_certificate(handle, ARTIK_DEVICE_CERT_ID,
			ARTIK_SECURITY_CERT_TYPE_PEM,  &se_cert, &se_cert_size);
	if (err != S_OK || !se_cert || !se_cert_size) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	se_cert_pem = strndup((char *)se_cert, se_cert_size);
	free(se_cert);
	err = security->get_certificate_sn(se_cert_pem, serial_number, &len);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get serial number (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < (len-1); i++)
		fprintf(stdout, "%02x:", serial_number[i]);

	fprintf(stdout, "%02x\n", serial_number[len-1]);

exit:
	if (handle)
		security->release(handle);
	if (se_cert_pem)
		free(se_cert_pem);
	artik_release_api_module(security);

	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int see_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, security_commands);
}
