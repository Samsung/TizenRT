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

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_security.h>

#include "command.h"

static int security_cert(int argc, char *argv[]);
static int security_pk(int argc, char *argv[]);
static int security_root(int argc, char *argv[]);
static int security_rand(int argc, char *argv[]);
static int security_serial(int argc, char *argv[]);

const struct command security_commands[] = {
	{ "cert", "Display certificate stored in SE", security_cert },
	{ "pk", "Display public key extracted from the certificate stored in SE", security_pk },
	{ "root", " Display root CA extracted from the certificate stored in SE", security_root },
	{ "rand", "rand <num> - Generate <num> random bytes from the SE", security_rand },
	{ "serial", "Return the Serial Number contained in the certificate stored in SE", security_serial },
	{ "", "", NULL }
};

static int security_cert(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle;
	artik_error err = S_OK;
	char *se_cert = NULL;

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

	err = security->get_certificate(handle, &se_cert);
	if (err != S_OK || !se_cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	fprintf(stdout, se_cert);

	free(se_cert);
	security->release(handle);

exit:
	if (security)
		artik_release_api_module(security);

	return ret;
}

static int security_pk(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle;
	artik_error err = S_OK;
	char *se_cert = NULL;
	char *se_pk = NULL;

	security = (artik_security_module *)artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	err = security->get_certificate(handle, &se_cert);
	if (err != S_OK || !se_cert) {
		fprintf(stderr, "Failed to get certificate (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	err = security->get_key_from_cert(handle, se_cert, &se_pk);
	if (err != S_OK || !se_pk) {
		fprintf(stderr, "Failed to get public key (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	fprintf(stdout, se_pk);

	free(se_cert);
	free(se_pk);
	security->release(handle);

exit:
	if (security)
		artik_release_api_module(security);

	return ret;
}

static int security_root(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle;
	artik_error err = S_OK;
	char *root_ca = NULL;

	security = (artik_security_module *)artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	err = security->get_root_ca(handle, &root_ca);
	if (err != S_OK || !root_ca) {
		fprintf(stderr, "Failed to get root CA (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	fprintf(stdout, root_ca);

	free(root_ca);
	security->release(handle);

exit:
	if (security)
		artik_release_api_module(security);

	return ret;
}

static int security_rand(int argc, char *argv[])
{
	artik_security_module *security = NULL;
	int ret = 0;
	artik_security_handle handle;
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
	rand_bytes = zalloc(num_bytes);
	if (!rand_bytes) {
		fprintf(stderr, "Failed to allocate memory for random bytes\n");
		ret = -1;
		goto exit;
	}

	err = security->get_random_bytes(handle, rand_bytes, num_bytes);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get random bytes (err=%d)\n", err);
		free(rand_bytes);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < num_bytes; i++)
		fprintf(stdout, "%02x ", rand_bytes[i]);

	fprintf(stdout, "\n");

	free(rand_bytes);
	security->release(handle);

exit:
	artik_release_api_module(security);

	return ret;
}


static int security_serial(int argc, char *argv[])
{
	artik_security_handle handle;
	int ret = 0;
	artik_error err = S_OK;
	unsigned char serial_number[ARTIK_CERT_SN_MAXLEN] = "";
	unsigned int len = ARTIK_CERT_SN_MAXLEN;
	int i = 0;
	artik_security_module *security = (artik_security_module *)artik_request_api_module("security");

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

	err = security->get_certificate_sn(handle, serial_number, &len);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get serial number (err=%d)\n", err);
		ret = -1;
		goto exit;
	}

	for (i = 0; i < (len-1); i++)
		fprintf(stdout, "%02x:", serial_number[i]);

	fprintf(stdout, "%02x\n", serial_number[len-1]);

	security->release(handle);

exit:
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
