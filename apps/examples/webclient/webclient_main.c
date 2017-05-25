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
/****************************************************************************
 * examples/webclient/webclient_main.c
 *
 *   Copyright (C) 2009, 2011, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
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

/**
* @testcase            lwip_http_01 (client)
* @brief               To send HTTP request messages and receive response messages.
*                      supported methods: GET, POST, PUT, DELETE
*                      Entity and encoding are needed only for POST and PUT methods. If an encoding is not for "C", it always uses the content-length.
* @scenario            1. Start webserver at TASH using the command "webserver start". Refer to webserver_main.c to run HTTP server.
*                      2. Start webclient at TASH using the command "webclient GET http://[serverip]/".
* @apicovered
* @precondition                Connect to Wi-Fi. Both ARTIK051 server and ARTIK051 client should be in the same network.
* @postcondition
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <apps/netutils/webclient.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

#define WEBCLIENT_STACK_SIZE   (1024 * 8)
#define WEBCLIENT_SCHED_PRI    100
#define WEBCLIENT_SCHED_POLICY SCHED_RR

#define WEBCLIENT_BUF_SIZE     4600

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct webclient_input {
	int argc;
	char **argv;
};

const char c_ca_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDhzCCAm+gAwIBAgIBADANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDAwWhcNMjEwMjEyMTQ0NDAwWjA7MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwggEiMA0G\r\n"
	"CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDA3zf8F7vglp0/ht6WMn1EpRagzSHx\r\n"
	"mdTs6st8GFgIlKXsm8WL3xoemTiZhx57wI053zhdcHgH057Zk+i5clHFzqMwUqny\r\n"
	"50BwFMtEonILwuVA+T7lpg6z+exKY8C4KQB0nFc7qKUEkHHxvYPZP9al4jwqj+8n\r\n" "YMPGn8u67GB9t+aEMr5P+1gmIgNb1LTV+/Xjli5wwOQuvfwu7uJBVcA0Ln0kcmnL\r\n" "R7EUQIN9Z/SG9jGr8XmksrUuEvmEF/Bibyc+E1ixVA0hmnM3oTDPb5Lc9un8rNsu\r\n" "KNF+AksjoBXyOGVkCeoMbo4bF6BxyLObyavpw/LPh5aPgAIynplYb6LVAgMBAAGj\r\n" "gZUwgZIwDAYDVR0TBAUwAwEB/zAdBgNVHQ4EFgQUtFrkpbPe0lL2udWmlQ/rPrzH\r\n" "/f8wYwYDVR0jBFwwWoAUtFrkpbPe0lL2udWmlQ/rPrzH/f+hP6Q9MDsxCzAJBgNV\r\n" "BAYTAk5MMREwDwYDVQQKEwhQb2xhclNTTDEZMBcGA1UEAxMQUG9sYXJTU0wgVGVz\r\n" "dCBDQYIBADANBgkqhkiG9w0BAQUFAAOCAQEAuP1U2ABUkIslsCfdlc2i94QHHYeJ\r\n" "SsR4EdgHtdciUI5I62J6Mom+Y0dT/7a+8S6MVMCZP6C5NyNyXw1GWY/YR82XTJ8H\r\n" "DBJiCTok5DbZ6SzaONBzdWHXwWwmi5vg1dxn7YxrM9d0IjxM27WNKs4sDQhZBQkF\r\n" "pjmfs2cb4oPl4Y9T9meTx/lvdkRYEug61Jfn6cA+qHpyPYdTH+UshITnmp5/Ztkf\r\n" "m/UTSLBNFNHesiTZeH31NcxYGdHSme9Nc/gfidRa0FLOCfWxRlFqAI47zG9jAQCZ\r\n" "7Z2mCGDNMhjQc+BYcdnl0lPXjdDK6V0qCg1dVewhUBcW5gZKzV7e9+DpVA==\r\n" "-----END CERTIFICATE-----\r\n";

const char c_cli_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDPzCCAiegAwIBAgIBBDANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER\r\n"
	"MA8GA1UEChMIUG9sYXJTU0wxGTAXBgNVBAMTEFBvbGFyU1NMIFRlc3QgQ0EwHhcN\r\n"
	"MTEwMjEyMTQ0NDA3WhcNMjEwMjEyMTQ0NDA3WjA8MQswCQYDVQQGEwJOTDERMA8G\r\n"
	"A1UEChMIUG9sYXJTU0wxGjAYBgNVBAMTEVBvbGFyU1NMIENsaWVudCAyMIIBIjAN\r\n"
	"BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyHTEzLn5tXnpRdkUYLB9u5Pyax6f\r\n" "M60Nj4o8VmXl3ETZzGaFB9X4J7BKNdBjngpuG7fa8H6r7gwQk4ZJGDTzqCrSV/Uu\r\n" "1C93KYRhTYJQj6eVSHD1bk2y1RPD0hrt5kPqQhTrdOrA7R/UV06p86jt0uDBMHEw\r\n" "MjDV0/YI0FZPRo7yX/k9Z5GIMC5Cst99++UMd//sMcB4j7/Cf8qtbCHWjdmLao5v\r\n" "4Jv4EFbMs44TFeY0BGbH7vk2DmqV9gmaBmf0ZXH4yqSxJeD+PIs1BGe64E92hfx/\r\n" "/DZrtenNLQNiTrM9AM+vdqBpVoNq0qjU51Bx5rU2BXcFbXvI5MT9TNUhXwIDAQAB\r\n" "o00wSzAJBgNVHRMEAjAAMB0GA1UdDgQWBBRxoQBzckAvVHZeM/xSj7zx3WtGITAf\r\n" "BgNVHSMEGDAWgBS0WuSls97SUva51aaVD+s+vMf9/zANBgkqhkiG9w0BAQUFAAOC\r\n" "AQEAAn86isAM8X+mVwJqeItt6E9slhEQbAofyk+diH1Lh8Y9iLlWQSKbw/UXYjx5\r\n" "LLPZcniovxIcARC/BjyZR9g3UwTHNGNm+rwrqa15viuNOFBchykX/Orsk02EH7NR\r\n" "Alw5WLPorYjED6cdVQgBl9ot93HdJogRiXCxErM7NC8/eP511mjq+uLDjLKH8ZPQ\r\n" "8I4ekHJnroLsDkIwXKGIsvIBHQy2ac/NwHLCQOK6mfum1pRx52V4Utu5dLLjD5bM\r\n" "xOBC7KU4xZKuMXXZM6/93Yb51K/J4ahf1TxJlTWXtnzDr9saEYdNy2SKY/6ZiDNH\r\n" "D+stpAKiQLAWaAusIWKYEyw9MQ==\r\n" "-----END CERTIFICATE-----\r\n";

const char c_cli_key_rsa[] =
	"-----BEGIN RSA PRIVATE KEY-----\r\n"
	"MIIEpAIBAAKCAQEAyHTEzLn5tXnpRdkUYLB9u5Pyax6fM60Nj4o8VmXl3ETZzGaF\r\n"
	"B9X4J7BKNdBjngpuG7fa8H6r7gwQk4ZJGDTzqCrSV/Uu1C93KYRhTYJQj6eVSHD1\r\n"
	"bk2y1RPD0hrt5kPqQhTrdOrA7R/UV06p86jt0uDBMHEwMjDV0/YI0FZPRo7yX/k9\r\n"
	"Z5GIMC5Cst99++UMd//sMcB4j7/Cf8qtbCHWjdmLao5v4Jv4EFbMs44TFeY0BGbH\r\n"
	"7vk2DmqV9gmaBmf0ZXH4yqSxJeD+PIs1BGe64E92hfx//DZrtenNLQNiTrM9AM+v\r\n"
	"dqBpVoNq0qjU51Bx5rU2BXcFbXvI5MT9TNUhXwIDAQABAoIBAGdNtfYDiap6bzst\r\n"
	"yhCiI8m9TtrhZw4MisaEaN/ll3XSjaOG2dvV6xMZCMV+5TeXDHOAZnY18Yi18vzz\r\n"
	"4Ut2TnNFzizCECYNaA2fST3WgInnxUkV3YXAyP6CNxJaCmv2aA0yFr2kFVSeaKGt\r\n"
	"ymvljNp2NVkvm7Th8fBQBO7I7AXhz43k0mR7XmPgewe8ApZOG3hstkOaMvbWAvWA\r\n"
	"zCZupdDjZYjOJqlA4eEA4H8/w7F83r5CugeBE8LgEREjLPiyejrU5H1fubEY+h0d\r\n"
	"l5HZBJ68ybTXfQ5U9o/QKA3dd0toBEhhdRUDGzWtjvwkEQfqF1reGWj/tod/gCpf\r\n"
	"DFi6X0ECgYEA4wOv/pjSC3ty6TuOvKX2rOUiBrLXXv2JSxZnMoMiWI5ipLQt+RYT\r\n"
	"VPafL/m7Dn6MbwjayOkcZhBwk5CNz5A6Q4lJ64Mq/lqHznRCQQ2Mc1G8eyDF/fYL\r\n" "Ze2pLvwP9VD5jTc2miDfw+MnvJhywRRLcemDFP8k4hQVtm8PMp3ZmNECgYEA4gz7\r\n" "wzObR4gn8ibe617uQPZjWzUj9dUHYd+in1gwBCIrtNnaRn9I9U/Q6tegRYpii4ys\r\n" "c176NmU+umy6XmuSKV5qD9bSpZWG2nLFnslrN15Lm3fhZxoeMNhBaEDTnLT26yoi\r\n" "33gp0mSSWy94ZEqipms+ULF6sY1ZtFW6tpGFoy8CgYAQHhnnvJflIs2ky4q10B60\r\n" "ZcxFp3rtDpkp0JxhFLhiizFrujMtZSjYNm5U7KkgPVHhLELEUvCmOnKTt4ap/vZ0\r\n" "BxJNe1GZH3pW6SAvGDQpl9sG7uu/vTFP+lCxukmzxB0DrrDcvorEkKMom7ZCCRvW\r\n" "KZsZ6YeH2Z81BauRj218kQKBgQCUV/DgKP2985xDTT79N08jUo3hTP5MVYCCuj/+\r\n" "UeEw1TvZcx3LJby7P6Xad6a1/BqveaGyFKIfEFIaBUBItk801sDDpDaYc4gL00Xc\r\n" "7lFuBHOZkxJYlss5QrGpuOEl9ZwUt5IrFLBdYaKqNHzNVC1pCPfb/JyH6Dr2HUxq\r\n" "gxUwAQKBgQCcU6G2L8AG9d9c0UpOyL1tMvFe5Ttw0KjlQVdsh1MP6yigYo9DYuwu\r\n" "bHFVW2r0dBTqegP2/KTOxKzaHfC1qf0RGDsUoJCNJrd1cwoCLG8P2EF4w3OBrKqv\r\n" "8u4ytY0F+Vlanj5lm3TaoHSVF1+NWPyOTiwevIECGKwSxvlki4fDAA==\r\n" "-----END RSA PRIVATE KEY-----\r\n";

struct http_client_ssl_config_t g_config = {
	(char *)c_ca_crt_rsa, (char *)c_cli_crt_rsa, (char *)c_cli_key_rsa,
	sizeof(c_ca_crt_rsa), sizeof(c_cli_crt_rsa), sizeof(c_cli_key_rsa), 2
};

static int g_https;
static int g_async;
static int g_testentity;

static const char headerfield_connect[] = "Connect";
static const char headerfield_close[] = "close";
static const char headerfield_useragent[] = "User-Agent";
static const char headerfield_tinyara[] = "TinyARA";

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: callback
 ****************************************************************************/

static void callback(struct http_client_response_t *response)
{
	printf("----------async response----------\n");
	printf("status %d %s\n", response->status, response->phrase);
	printf("%s\n", response->entity);
	printf("---------------------------------\n");
}

static void dump_webclient_usage(void)
{
	printf("\n  webclient usage:\n");
	printf("   $ webclient <method> <uri> [options...] \n");
	printf("\n");
	printf(" <method>   : %%s (GET, PUT, POST, DELETE)\n");
	printf(" <uri>      : %%s (Host address : should be started with http:// or https://)\n");
	printf("\n [options...] \n");
	printf(" async=1               Enable asynchronous mode (default is synchronous)\n");
	printf(" chunked=1             Enable chunked encoding (default is disabled)\n");
	printf(" entity=DATA           Set entity data (default is NULL)\n");
	printf(" test_entity=SIZE      Test entity size (default is 0)\n");
	printf("\n  example:\n");
	printf("   $ webclient GET https://127.0.0.1/\n");
	printf("   $ webclient GET https://127.0.0.1/ async=1 entity=data\n");
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wget_main
 ****************************************************************************/

int webclient_init_request(void *arg, struct http_client_request_t *request)
{
	int argc, i;
	char **argv;
	char *p, *q;
	struct webclient_input *input;

	input = arg;
	argc = input->argc;
	argv = input->argv;

	g_async = 0;
	g_testentity = 0;
	memset(request, 0, sizeof(struct http_client_request_t));

	if (argc < 3) {
		return -1;
	}

	if (!strcmp(argv[1], "GET")) {
		request->method = WGET_MODE_GET;
	} else if (!strcmp(argv[1], "POST")) {
		request->method = WGET_MODE_POST;
	} else if (!strcmp(argv[1], "PUT")) {
		request->method = WGET_MODE_PUT;
	} else if (!strcmp(argv[1], "DELETE")) {
		request->method = WGET_MODE_DELETE;
	} else {
		dump_webclient_usage();
		return -1;
	}

	/* argument2 is url. */
	request->url = argv[2];
#ifdef CONFIG_NET_SECURITY_TLS
	if (!strncmp(request->url, "https", 5)) {
		g_https = 1;
	} else
#endif
	if (!strncmp(request->url, "http", 4)) {
		g_https = 0;
	} else {
		return -1;
	}

	for (i = 3; i < argc; i++) {
		p = argv[i];
		if ((q = strchr(p, '=')) == NULL) {
			return -1;
		}
		*q++ = '\0';

		if (strcmp(p, "async") == 0) {
			g_async = atoi(q);
		} else if (strcmp(p, "chunked") == 0) {
			request->encoding = atoi(q);
		} else if (strcmp(p, "entity") == 0) {
			request->entity = q;
		} else if (strcmp(p, "test_entity") == 0) {
			int t = atoi(q);
			if (t > 0 && t <= WEBCLIENT_CONF_MAX_ENTITY_SIZE) {
				request->entity = malloc(t);
				if (request->entity == NULL) {
					return -1;
				}
				g_testentity = 1;
				memset(request->entity, '1', t);
			} else {
				printf("entity is too big\n");
				return -1;
			}
		} else {
			return -1;
		}
	}

	request->buflen = WEBCLIENT_BUF_SIZE;
	return 0;
}

pthread_addr_t webclient_cb(void *arg)
{
	struct http_client_request_t request;
	struct http_keyvalue_list_t headers;
	struct http_client_response_t response;
	struct http_client_ssl_config_t *ssl_config = NULL;

	if (webclient_init_request(arg, &request)) {
		dump_webclient_usage();
		if (g_testentity && request.entity) {
			free(request.entity);
		}
		return NULL;
	}

	ssl_config = g_https? &g_config : NULL;

	/* before sending request,
	 * must initialize keyvalue list for request headers
	 */
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, headerfield_connect, headerfield_close);
	http_keyvalue_list_add(&headers, headerfield_useragent, headerfield_tinyara);
	request.headers = &headers;

	/* before sending request by sync function,
	 * must initialize response structure
	 */
	if (g_async) {
		if (http_client_send_request_async(&request, ssl_config, (wget_callback_t)callback)) {
			printf("fail to send request\n");
			goto release_out;
		}
		/* sleep for end request */
		while (request.async_flag > 0) {
			usleep(100000);
		}
		if (request.async_flag < 0) {
			printf("fail to send request\n");
		}
	} else {
		if (http_client_response_init(&response) < 0) {
			printf("fail to response init\n");
			goto release_out;
		}
		if (http_client_send_request(&request, ssl_config, &response)) {
			printf("fail to send request\n");
			goto release_out;
		}
		printf("----------sync response----------\n");
		printf("status %d %s\n", response.status, response.phrase);
		printf("%s\n", response.entity);
		printf("---------------------------------\n");
	}

release_out:
	if (g_testentity) {
		free(request.entity);
		g_testentity = 0;
	}
	/* after sending request by sync function,
	 * must release response structure
	 */
	if (g_async == 0) {
		http_client_response_release(&response);
	}
	/* before finish of app,
	 * must release keyvalue list for request headers
	 */
	http_keyvalue_list_release(&headers);
	printf("end request\n");

	return NULL;
}

int webclient_main(int argc, char *argv[])
{
	pthread_attr_t attr;
	int status;
	struct sched_param sparam;
	pthread_t tid;
	struct webclient_input arg;

	status = pthread_attr_init(&attr);
	if (status != 0) {
		printf("fail to start webclient\n");
		return -1;
	}

	sparam.sched_priority = WEBCLIENT_SCHED_PRI;
	status = pthread_attr_setschedparam(&attr, &sparam);
	status = pthread_attr_setschedpolicy(&attr, WEBCLIENT_SCHED_POLICY);
	status = pthread_attr_setstacksize(&attr, WEBCLIENT_STACK_SIZE);

	arg.argc = argc;
	arg.argv = argv;

	status = pthread_create(&tid, &attr, webclient_cb, &arg);
	if (status < 0) {
		printf("fail to start webclient\n");
		return -1;
	}
	pthread_setname_np(tid, "webclient");

	pthread_join(tid, NULL);

	return 0;
}
