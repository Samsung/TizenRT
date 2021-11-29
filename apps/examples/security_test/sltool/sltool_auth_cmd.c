/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <tinyara/seclink.h>
#include "sltool.h"

#define OUTPUT_CERT_SIZE 8192

static char g_test_crt[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICaDCCAgygAwIBAgIBAjAMBggqhkjOPQQDAgUAMHAxLTArBgNVBAMTJFNhbXN1\r\n"
	"bmcgRWxlY3Ryb25pY3MgT0NGIFJvb3QgQ0EgVEVTVDEUMBIGA1UECxMLT0NGIFJv\r\n"
	"b3QgQ0ExHDAaBgNVBAoTE1NhbXN1bmcgRWxlY3Ryb25pY3MxCzAJBgNVBAYTAktS\r\n"
	"MCAXDTE2MTEyNDAyNDcyN1oYDzIwNjkxMjMxMTQ1OTU5WjBwMS0wKwYDVQQDEyRT\r\n"
	"YW1zdW5nIEVsZWN0cm9uaWNzIE9DRiBSb290IENBIFRFU1QxFDASBgNVBAsTC09D\r\n"
	"RiBSb290IENBMRwwGgYDVQQKExNTYW1zdW5nIEVsZWN0cm9uaWNzMQswCQYDVQQG\r\n"
	"EwJLUjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBzzury7p8HANVn+v4CIa2h/\r\n"
	"R/SAt3VVst+vTv4/kR+lgU1OEiT3t9+mOWE7J+oddpRofFW2DdeJkpfQUVOn4NOj\r\n"
	"gZIwgY8wDgYDVR0PAQH/BAQDAgHGMC4GA1UdHwQnMCUwI6AhoB+GHWh0dHA6Ly9j\r\n"
	"YS5zYW1zdW5naW90cy5jb20vY3JsMA8GA1UdEwEB/wQFMAMBAf8wPAYIKwYBBQUH\r\n"
	"AQEEMDAuMCwGCCsGAQUFBzABhiBodHRwOi8vb2NzcC10ZXN0LnNhbXN1bmdpb3Rz\r\n"
	"LmNvbTAMBggqhkjOPQQDAgUAA0gAMEUCIQCIsi3BcOQMXO/pCiUA+S75bYFWS27E\r\n"
	"GAq9e2E3+hQ2TAIgWrTieFAZ5xRH3BnSHG+XEF2HPD99y/SYSa6T59YW+jE=\r\n"
	"-----END CERTIFICATE-----\r\n\0";

int sltool_handle_remove_auth(sl_options_s *opt)
{
	printf("[sltool] idx %d  %s:%d\n", opt->idx, __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	sl_ctx hnd;
	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}
	if ((res = sl_remove_certificate(hnd, opt->idx)) != SECLINK_OK) {
		printf("[sltool] error %s %s:%d\n", sl_strerror(res), __FUNCTION__, __LINE__);
		fres = -1;
	}
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	return fres;
}

/*  Description: set authenticate */
int sltool_handle_set_auth(sl_options_s *opt)
{
	printf("[sltool] idx %d certificate length(%d) %s:%d\n",
		   opt->idx, sizeof(g_test_crt), __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	hal_data input;
	sl_ctx hnd;
	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}
	input.data = g_test_crt;
	input.data_len = sizeof(g_test_crt);
	input.priv = NULL;
	input.priv_len = 0;
	if ((res = sl_set_certificate(hnd, opt->idx, &input)) != SECLINK_OK) {
		printf("[sltool] error %s %s:%d\n", sl_strerror(res), __FUNCTION__, __LINE__);
		fres = -1;
	}
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	return fres;
}

int sltool_handle_get_auth(sl_options_s *opt)
{
	printf("[sltool] idx %d  %s:%d\n", opt->idx, __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	hal_data output;
	sl_ctx hnd;
	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}
	char *data = (char *)malloc(OUTPUT_CERT_SIZE);
	if (!data) {
		printf("[sltool] fail to alloc memory %s:%d\n", __FUNCTION__, __LINE__);
		fres = -1;
		goto out;
	}
	output.data = data;
	output.data_len = OUTPUT_CERT_SIZE;
	output.priv = NULL;
	output.priv_len = 0;
	if ((res = sl_get_certificate(hnd, opt->idx, &output)) != SECLINK_OK) {
		printf("[sltool] error %s %s:%d\n", sl_strerror(res), __FUNCTION__, __LINE__);
		fres = -1;
		goto out;
	}
	sltool_print_certificate(output.data, output.data_len, "Stored certificate");
out:
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	if (data) {
		free(data);
	}
	return fres;
}
