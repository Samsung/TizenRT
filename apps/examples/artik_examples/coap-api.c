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
 * @file coap_api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_coap.h>

#include "command.h"

#define MAX_SIZE 63
#define ADD_SPACES(x)				\
	do {					\
		fprintf(stdout, "\n");		\
		fprintf(stdout, "%*s", x, "");	\
	} while (0)

static int coap_connect(int argc, char *argv[]);
static int coap_disconnect(int argc, char *argv[]);
static int coap_request(int argc, char *argv[]);
static int coap_server_start(int argc, char *argv[]);
static int coap_server_stop(int argc, char *argv[]);

static artik_coap_handle g_handle;
static artik_coap_handle f_handle;
static artik_coap_module *g_coap;
static artik_coap_config g_config;
static bool started;
static bool connected;

typedef struct {
	char data[256];
	int data_len;
} dataResource;

typedef struct {
	dataResource *dataRes;
	artik_coap_handle handle;
} serverInterface;

static dataResource dataRes = {
	"Hello World",
	12
};

static serverInterface interface;

static const char client_cert[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIB4TCCAYegAwIBAgIJAJvNMfZLercmMAoGCCqGSM49BAMCME0xCzAJBgNVBAYT\r\n"
	"AkZSMQwwCgYDVQQIDANJZEYxDjAMBgNVBAcMBVBhcmlzMRAwDgYDVQQKDAdTYW1z\r\n"
	"dW5nMQ4wDAYDVQQLDAVBcnRpazAeFw0xODA0MjMxNTM0MjVaFw0xOTA0MjMxNTM0\r\n"
	"MjVaME0xCzAJBgNVBAYTAkZSMQwwCgYDVQQIDANJZEYxDjAMBgNVBAcMBVBhcmlz\r\n"
	"MRAwDgYDVQQKDAdTYW1zdW5nMQ4wDAYDVQQLDAVBcnRpazBZMBMGByqGSM49AgEG\r\n"
	"CCqGSM49AwEHA0IABKXl5NswY/mFN+kOslUjIJCMLMleTxu6cOZmphceJhtn+9a7\r\n"
	"0kukS38y3JivmUYQ1sD6lghw5pxUJlL4GIbczZmjUDBOMB0GA1UdDgQWBBTg5euL\r\n"
	"zcnBcY1SHoT+bq9lkOnPqDAfBgNVHSMEGDAWgBTg5euLzcnBcY1SHoT+bq9lkOnP\r\n"
	"qDAMBgNVHRMEBTADAQH/MAoGCCqGSM49BAMCA0gAMEUCIE0Y8P1HpQ05RmnLqW5+\r\n"
	"qVpvTzbQiQZALLvrPQLcwlSDAiEAiLYz3WTQeGjYmT/7F+c3vwWQp5XWjR8JGNjL\r\n"
	"oC39p5Q=\r\n"
	"-----END CERTIFICATE-----\n";

static const char client_key[] =
	"-----BEGIN EC PARAMETERS-----\r\n"
	"BggqhkjOPQMBBw==\r\n"
	"-----END EC PARAMETERS-----\r\n"
	"-----BEGIN EC PRIVATE KEY-----\r\n"
	"MHcCAQEEIIo1HWeuNp2tfYlrvEZo70rXvUJblFhkx0bizV/NLkieoAoGCCqGSM49\r\n"
	"AwEHoUQDQgAEpeXk2zBj+YU36Q6yVSMgkIwsyV5PG7pw5mamFx4mG2f71rvSS6RL\r\n"
	"fzLcmK+ZRhDWwPqWCHDmnFQmUvgYhtzNmQ==\r\n"
	"-----END EC PRIVATE KEY-----\n";

const struct command coap_commands[] = {
	{ "connect", "connect <uri> <psk/ecdsa> <identity> <psk>", coap_connect },
	{ "disconnect", "", coap_disconnect },
	{ "request", "request <GET/POST/PUT/DELETE/OBSERVE/STOP> <resource> <payload>", coap_request },
	{ "start", "start <psk/ecdsa> <identity> <psk>", coap_server_start },
	{ "stop", "stop", coap_server_stop },
	{ "", "", NULL }
};

static int count_digits(unsigned int number)
{
	int count = 0;

	while (number != 0) {
		number /= 10;
		++count;
	}

	return count;
}

static void response_callback(const artik_coap_msg *msg,
			artik_coap_error error,
			void *user_data)
{
	int i;

	if (msg == NULL || error != ARTIK_COAP_ERROR_NONE) {
		fprintf(stderr, "Fail to receive message\n");
		if (error != ARTIK_COAP_ERROR_RST)
			connected = false;
		return;
	}

	fprintf(stdout, "==[ CoAP Response ]============================================\n");
	fprintf(stdout, "MID    : %d\n", msg->msg_id);
	fprintf(stdout, "Token  : ");

	if (msg->token && msg->token_len > 0) {
		for (i = 0; i < msg->token_len; i++)
			fprintf(stdout, "%02x", msg->token[i]);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "Type   : ");

	switch (msg->msg_type) {
	case ARTIK_COAP_MSG_CON:
		fprintf(stdout, "CON\n");
		break;
	case ARTIK_COAP_MSG_NON:
		fprintf(stdout, "NON\n");
		break;
	case ARTIK_COAP_MSG_ACK:
		fprintf(stdout, "ACK\n");
		break;
	case ARTIK_COAP_MSG_RST:
		fprintf(stdout, "RST\n");
		break;
	default:
		break;
	}

	fprintf(stdout, "Status : %u.%02u\n", msg->code >> 5, msg->code & 0x1f);

	if (msg->options && msg->num_options > 0) {
		int c = 10;

		fprintf(stdout, "Options: ");
		fprintf(stdout, "{");

		for (i = 0; i < msg->num_options; i++) {
			switch (msg->options[i].key) {
			case ARTIK_COAP_OPTION_IF_MATCH: {
				int j;

				if (c + 11 + msg->options[i].data_len < MAX_SIZE)
					c += 11 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"If-Match\":");

				for (j = 0; j < msg->options[i].data_len; j++)
					fprintf(stdout, "%02x", msg->options[i].data[j]);

				break;
			}
			case ARTIK_COAP_OPTION_URI_HOST:
				if (c + 11 + msg->options[i].data_len < MAX_SIZE)
					c += 11 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Uri-Host\":%s",
					msg->options[i].data);

				break;
			case ARTIK_COAP_OPTION_ETAG: {
				int j;

				if (c + 7 + msg->options[i].data_len < MAX_SIZE)
					c += 7 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Etag\":");

				for (j = 0; j < msg->options[i].data_len; j++)
					fprintf(stdout, "%02x", msg->options[i].data[j]);

				break;
			}
			case ARTIK_COAP_OPTION_IF_NONE_MATCH:
				if (c + 15 < MAX_SIZE)
					c += 15;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"If-None-Match\"");

				break;
			case ARTIK_COAP_OPTION_OBSERVE: {
				unsigned int obs = 0;

				memcpy(&obs, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 10 + count_digits(obs) < MAX_SIZE)
					c += 10 + count_digits(obs);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Observe\":%d",
					obs);

				break;
			}
			case ARTIK_COAP_OPTION_URI_PORT: {
				unsigned int uri_port = 0;

				memcpy(&uri_port, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 11 + count_digits(uri_port) < MAX_SIZE)
					c += 11 + count_digits(uri_port);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Uri-Port\":%d",
					uri_port);

				break;
			}
			case ARTIK_COAP_OPTION_LOCATION_PATH:
				if (c + 16 + msg->options[i].data_len < MAX_SIZE)
					c += 16 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Location-Path\":%s",
					msg->options[i].data);

				break;
			case ARTIK_COAP_OPTION_URI_PATH:
				if (c + 11 + msg->options[i].data_len < MAX_SIZE)
					c += 11 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Uri-Path\":%s",
					msg->options[i].data);

				break;
			case ARTIK_COAP_OPTION_CONTENT_FORMAT: {
				unsigned int content_format = 0;

				memcpy(&content_format, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 17 + count_digits(content_format) < MAX_SIZE)
					c += 17 + count_digits(content_format);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Content-Format\":%d",
					content_format);

				break;
			}
			case ARTIK_COAP_OPTION_MAXAGE: {
				unsigned int max_age = 0;

				memcpy(&max_age, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 17 + count_digits(max_age) < MAX_SIZE)
					c += 17 + count_digits(max_age);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Max-Age\":%d",
					max_age);

				break;
			}
			case ARTIK_COAP_OPTION_URI_QUERY:
				if (c + 12 + msg->options[i].data_len < MAX_SIZE)
					c += 12 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Uri-Query\":%s",
					msg->options[i].data);
				break;
			case ARTIK_COAP_OPTION_ACCEPT: {
				unsigned int accept = 0;

				memcpy(&accept, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 9 + count_digits(accept) < MAX_SIZE)
					c += 9 + count_digits(accept);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Accept\":%d",
					accept);
				break;
			}
			case ARTIK_COAP_OPTION_LOCATION_QUERY:
				if (c + 17 + msg->options[i].data_len < MAX_SIZE)
					c += 17 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Location-Query\":%s",
					msg->options[i].data);
				break;
			case ARTIK_COAP_OPTION_BLOCK2: {
				uint32_t block2 = 0;
				uint16_t szx;
				uint32_t num;
				uint8_t more;
				uint16_t size;

				memcpy(&block2, msg->options[i].data,
					msg->options[i].data_len);

				szx = block2 & 0x07;
				szx += 4;

				num = block2/16;

				more = block2 & 0x08;
				more >>= 3;

				size = 1 << (uint16_t)szx;

				if (c + 9 + count_digits(num) + 1
						+ count_digits(more) + 1
						+ count_digits(size) < MAX_SIZE)
					c += 9 + count_digits(num) + 1
						+ count_digits(more) + 1
						+ count_digits(size);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Block2\":%d/%d/%d",
					num, more, size);
				break;
			}
			case ARTIK_COAP_OPTION_PROXY_URI:
				if (c + 12 + msg->options[i].data_len < MAX_SIZE)
					c += 12 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Proxy-Uri\":%s",
					msg->options[i].data);
				break;
			case ARTIK_COAP_OPTION_PROXY_SCHEME:
				if (c + 15 + msg->options[i].data_len < MAX_SIZE)
					c += 15 + msg->options[i].data_len;
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Proxy-Scheme\":%s",
					msg->options[i].data);
				break;
			case ARTIK_COAP_OPTION_SIZE1: {
				unsigned int size1 = 0;

				memcpy(&size1, msg->options[i].data,
					msg->options[i].data_len);

				if (c + 8 + count_digits(size1) < MAX_SIZE)
					c += 8 + count_digits(size1);
				else {
					c = 9;
					ADD_SPACES(c);
				}

				fprintf(stdout, "\"Size1\":%d",
					size1);
				break;
			}
			default:
				break;
			}
			if (i + 1 != msg->num_options)
				fprintf(stdout, ", ");
		}
		fprintf(stdout, "}\n");
	}

	fprintf(stdout, "Payload: %d Bytes\n", msg->data_len);

	if (msg->data && msg->data_len > 0) {
		fprintf(stdout, "---------------------------------------------------------------\n");
		fprintf(stdout, "%s\n", msg->data);
	}

	fprintf(stdout, "===============================================================\n");
}

static void get_test_callback(const artik_coap_msg *request,
				artik_coap_msg *response,
				void *user_data)
{
	serverInterface *iface = (serverInterface *)user_data;
	dataResource *data = iface->dataRes;
	unsigned int content_format = ARTIK_OPTION_CONTENT_FORMAT_PLAIN;

	response->code = ARTIK_COAP_RES_CONTENT;

	if (data->data_len > 0) {
		response->data = (unsigned char *)malloc(data->data_len);

		if (!response->data) {
			fprintf(stderr, "Memory problem for data\n");
			return;
		}

		memcpy(response->data, data->data, data->data_len);
		response->data_len = data->data_len - 1;
	}


	response->options = (artik_coap_option *)malloc(2 * sizeof(artik_coap_option));

	if (!response->options) {
		fprintf(stderr, "Memory problem for options\n");
		return;
	}

	response->options[0].key = ARTIK_COAP_OPTION_CONTENT_FORMAT;
	response->options[0].data = (unsigned char *)malloc(1);

	if (!response->options[0].data) {
		fprintf(stderr, "Memory problem data option\n");
		return;
	}

	response->options[0].data_len = 1;

	memcpy(response->options[0].data, &content_format, response->options[0].data_len);

	response->num_options = 1;
}

static void post_test_callback(const artik_coap_msg *request,
				artik_coap_msg *response,
				void *user_data)
{
#define ERROR_MSG "The length must be inferior to 256"

	serverInterface *iface = (serverInterface *)user_data;
	dataResource *data = iface->dataRes;
	artik_coap_handle *handle = iface->handle;

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		goto exit;
	}

	if (request->data_len > 256) {
		response->code = ARTIK_COAP_RES_UNAUTHORIZED;
		response->data_len = strlen(ERROR_MSG);
		response->data = (unsigned char *)malloc(response->data_len);

		if (!response->data) {
			fprintf(stderr, "Memory problem data response\n");
			goto exit;
		}

		memcpy(response->data, ERROR_MSG, response->data_len);
		goto exit;
	}

	if (data->data_len > 0) {
		response->code = ARTIK_COAP_RES_CHANGED;
		memcpy(data->data, request->data, request->data_len);
		data->data_len = request->data_len;
		if (g_coap->notify_resource_changed(handle, "test") != S_OK) {
			fprintf(stderr, "Fail to set the resource 'test' dirty\n");
			goto exit;
		}
	} else {
		response->code = ARTIK_COAP_RES_CREATED;
		memcpy(data->data, request->data, request->data_len);
		data->data_len = request->data_len;
		if (g_coap->notify_resource_changed(handle, "test") != S_OK) {
			fprintf(stderr, "Fail to set the resource 'test' dirty\n");
			goto exit;
		}
	}

exit:
	artik_release_api_module(g_coap);

#undef ERROR_MSG
}

static void put_test_callback(const artik_coap_msg *request,
				artik_coap_msg *response,
				void *user_data)
{
#define ERROR_MSG_LENGTH "The length must be inferior to 256"
#define ERROR_MSG_CREATION "The resource is not created (do POST before)"

	serverInterface *iface = (serverInterface *)user_data;
	dataResource *data = iface->dataRes;
	artik_coap_handle *handle = iface->handle;

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		goto exit;
	}

	if (request->data_len > 256) {
		response->code = ARTIK_COAP_RES_UNAUTHORIZED;
		response->data_len = strlen(ERROR_MSG_LENGTH);
		response->data = (unsigned char *)malloc(response->data_len);

		if (!response->data) {
			fprintf(stderr, "Memory problem data response\n");
			goto exit;
		}

		memcpy(response->data, ERROR_MSG_LENGTH, response->data_len);
		goto exit;
	}

	if (data->data_len > 0) {
		response->code = ARTIK_COAP_RES_CHANGED;
		memcpy(data->data, request->data, request->data_len);
		data->data_len = request->data_len;
		if (g_coap->notify_resource_changed(handle, "test") != S_OK) {
			fprintf(stderr, "Fail to set the resource 'test' dirty\n");
			goto exit;
		}
	} else {

		response->code = ARTIK_COAP_RES_UNAUTHORIZED;
		response->data_len = strlen(ERROR_MSG_CREATION);
		response->data = (unsigned char *)malloc(response->data_len);

		if (!response->data) {
			fprintf(stderr, "Memory problem data response\n");
			goto exit;
		}

		memcpy(response->data, ERROR_MSG_CREATION, response->data_len);
	}

exit:
	artik_release_api_module(g_coap);

#undef ERROR_MSG_LENGTH
#undef ERROR_MSG_CREATION
}

static void delete_test_callback(const artik_coap_msg *request,
					artik_coap_msg *response,
					void *user_data)
{
	serverInterface *iface = (serverInterface *)user_data;
	dataResource *data = iface->dataRes;
	artik_coap_handle *handle = iface->handle;

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		goto exit;
	}

	response->code = ARTIK_COAP_RES_DELETED;

	memset(data, 0, sizeof(dataResource));

	if (g_coap->notify_resource_changed(handle, "test") != S_OK) {
		fprintf(stderr, "Fail to set the resource 'test' dirty\n");
		goto exit;
	}

exit:
	artik_release_api_module(g_coap);
}

static void get_info_callback(const artik_coap_msg *request,
				artik_coap_msg *response,
				void *user_data)
{
#define INFO_MSG "Welcome from Artik CoAP Server Test"

	response->code = ARTIK_COAP_RES_CONTENT;

	response->data_len = strlen(INFO_MSG);
	response->data = (unsigned char *)malloc(response->data_len);

	if (!response->data) {
		fprintf(stderr, "Memory problem data response\n");
		return;
	}

	memcpy(response->data, INFO_MSG, response->data_len);

	response->options = (artik_coap_option *)malloc(1 * sizeof(artik_coap_option));

	unsigned int content_format = ARTIK_OPTION_CONTENT_FORMAT_PLAIN;

	response->options[0].key = ARTIK_COAP_OPTION_CONTENT_FORMAT;
	response->options[0].data = (unsigned char *)malloc(1);
	memcpy(response->options[0].data, &content_format, 1);
	response->options[0].data_len = 1;

	response->num_options = 1;

#undef INFO_MSG
}

static int coap_connect(int argc, char *argv[])
{
	artik_error ret = S_OK;

	if (argc < 4) {
		fprintf(stderr, "Missing uri parameter\n");
		usage(argv[1], coap_commands);
		return -1;
	}

	if (connected) {
		fprintf(stderr, "Client already connected\n");
		return -1;
	}

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		return -1;
	}

	memset(&g_config, 0, sizeof(artik_coap_config));

	g_config.uri = argv[3];

	if (argc > 4 && !strcmp(argv[4], "psk")) {
		g_config.psk = malloc(sizeof(artik_coap_psk_param));

		if (!g_config.psk) {
			fprintf(stderr, "Memory problem\n");
			ret = -1;
			goto exit;
		}

		memset(g_config.psk, 0, sizeof(artik_coap_psk_param));

		if (argc > 5) {
			g_config.psk->identity = strdup(argv[5]);

			if (!g_config.psk->identity) {
				fprintf(stderr, "Memory problem\n");
				ret = -1;
				goto exit;
			}
		}

		if (argc > 6) {
			g_config.psk->psk = strdup(argv[6]);

			if (!g_config.psk->psk) {
				fprintf(stderr, "Memory problem\n");
				ret = -1;
				goto exit;
			}

			g_config.psk->psk_len = strlen(argv[6]);
		}
	} else if (argc > 4 && !strcmp(argv[4], "ecdsa")) {
		g_config.ssl = (artik_ssl_config *)malloc(sizeof(artik_ssl_config));

		if (!g_config.ssl) {
			fprintf(stderr, "Memory problem\n");
			ret = -1;
			goto exit;
		}

		memset(g_config.ssl, 0, sizeof(artik_ssl_config));

		g_config.ssl->client_cert.data = (char *)client_cert;
		g_config.ssl->client_cert.len = strlen(client_cert) + 1;

		g_config.ssl->client_key.data = (char *)client_key;
		g_config.ssl->client_key.len = strlen(client_key) + 1;
	} else if (argc > 4 && strcmp(argv[4], "psk") && strcmp(argv[4], "ecdsa")) {
		fprintf(stderr, "psk or ecdsa parameter only\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->create_client(&g_handle, &g_config);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to create client\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->set_send_callback(g_handle, response_callback, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to set send callback\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->connect(g_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to connect\n");
		g_coap->disconnect(g_handle);
		g_coap->destroy_client(g_handle);
		g_handle = NULL;
		ret = -1;
		goto exit;
	}

	connected = true;

exit:
	artik_release_api_module(g_coap);
	if (g_config.ssl)
		free(g_config.ssl);
	if (g_config.psk && g_config.psk->identity)
		free((void *)g_config.psk->identity);
	if (g_config.psk && g_config.psk->psk)
		free((void *)g_config.psk->psk);
	if (g_config.psk)
		free(g_config.psk);
	return ret;
}

static int coap_disconnect(int argc, char *argv[])
{
	artik_error ret = S_OK;

	if (!g_handle) {
		fprintf(stderr, "Client not connected\n");
		return -1;
	}

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		return -1;
	}

	ret = g_coap->disconnect(g_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to disconnect\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->destroy_client(g_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to destroy client\n");
		ret = -1;
	}

	g_handle = NULL;
	connected = false;

exit:
	artik_release_api_module(g_coap);
	return ret;
}

static int coap_request(int argc, char *argv[])
{
	artik_error ret = S_OK;
	artik_coap_msg msg;
	bool observe = false;
	bool stop = false;

	if (argc < 5) {
		fprintf(stderr, "Missing parameters\n");
		usage(argv[1], coap_commands);
		return -1;
	}

	if (!g_handle) {
		fprintf(stderr, "Client not connected\n");
		return -1;
	}

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		return -1;
	}

	memset(&msg, 0, sizeof(artik_coap_msg));

	if (!strcmp(argv[3], "GET"))
		msg.code = ARTIK_COAP_REQ_GET;
	else if (!strcmp(argv[3], "POST"))
		msg.code = ARTIK_COAP_REQ_POST;
	else if (!strcmp(argv[3], "PUT"))
		msg.code = ARTIK_COAP_REQ_PUT;
	else if (!strcmp(argv[3], "DELETE"))
		msg.code = ARTIK_COAP_REQ_DELETE;
	else if (!strcmp(argv[3], "OBSERVE"))
		observe = true;
	else if (!strcmp(argv[3], "STOP"))
		stop = true;
	else {
		fprintf(stderr, "Unknown method (GET/POST/PUT/DELETE/OBSERVE/STOP accepted)\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->set_send_callback(g_handle, NULL, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to unset send callback\n");
		ret = -1;
		goto exit;
	}

	if (observe) {
		ret = g_coap->set_observe_callback(g_handle, response_callback, NULL);
		if (ret != S_OK) {
			fprintf(stderr, "Fail to set observe callback\n");
			ret = -1;
			goto exit;
		}
	} else {
		ret = g_coap->set_send_callback(g_handle, response_callback, NULL);
		if (ret != S_OK) {
			fprintf(stderr, "Fail to set send callback\n");
			ret = -1;
			goto exit;
		}
	}

	if (argc == 6 && argv[5]) {
		msg.data = (unsigned char *)argv[5];
		msg.data_len = strlen(argv[5]);
	}

	msg.token = (unsigned char *)malloc(2);

	if (!msg.token) {
		fprintf(stderr, "Memory problem\n");
		ret = -1;
		goto exit;
	}

	msg.token[0] = 0xDE;
	msg.token[1] = 0x0D;
	msg.token_len = 2;

	msg.options = malloc(1*sizeof(artik_coap_option));

	if (!msg.options) {
		fprintf(stderr, "Memory problem\n");
		ret = -1;
		goto exit;
	}

	memset(msg.options, 0, 1*sizeof(artik_coap_option));

	msg.options[0].key = ARTIK_COAP_OPTION_CONTENT_FORMAT;
	msg.options[0].data = malloc(1);

	if (!msg.options[0].data) {
		fprintf(stderr, "Memory problem\n");
		ret = -1;
		goto exit;
	}

	msg.options[0].data[0] = 0;
	msg.options[0].data_len = 1;

	msg.num_options = 1;

	if (observe) {
		ret = g_coap->observe(g_handle, argv[4], ARTIK_COAP_MSG_CON,
			msg.options, msg.num_options, msg.token, msg.token_len);
		if (ret != S_OK) {
			fprintf(stderr, "Fail to observe\n");
			ret = -1;
			goto exit;
		}
	} else if (!stop) {
		ret = g_coap->send_message(g_handle, argv[4], &msg);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to send message\n");
			ret = -1;
			goto exit;
		}
	} else {
		ret = g_coap->cancel_observe(g_handle, argv[4],
			msg.token, msg.token_len);
		if (ret != S_OK) {
			fprintf(stderr, "Failed to cancel observe\n");
			ret = -1;
			goto exit;
		}
		ret = g_coap->set_observe_callback(g_handle, NULL, NULL);
		if (ret != S_OK) {
			fprintf(stderr, "Fail to unset observe callback\n");
			ret = -1;
		}
	}

exit:
	artik_release_api_module(g_coap);
	if (msg.token) {
		free(msg.token);
		msg.token = NULL;
	}
	if (msg.options && msg.options[0].data) {
		free(msg.options[0].data);
		msg.options[0].data = NULL;
	}
	if (msg.options) {
		free(msg.options);
		msg.options = NULL;
	}
	return ret;
}

static int verify_psk_callback(const unsigned char *identity, unsigned char **key,
		int key_len, void *user_data)
{
	struct keymap_t {
		unsigned char *id;
		size_t id_length;
		unsigned char *key;
		size_t key_length;
	} psk[3] = {
		{ (unsigned char *)"Client_identity", 15,
		(unsigned char *)"secretPSK", 9 },
		{ (unsigned char *)"default identity", 16,
		(unsigned char *)"\x11\x22\x33", 3 },
		{ (unsigned char *)"\0", 2,
		(unsigned char *)"", 1 }
	};

	if (identity) {
		int i;

		for (i = 0; i < sizeof(psk)/sizeof(struct keymap_t); i++) {
			if (strlen((char *)identity) == psk[i].id_length &&
					memcmp((char *)identity, (char *)psk[i].id,
						strlen((char *)identity)) == 0) {
				if (key_len < psk[i].key_length) {
					fprintf(stderr, "buffer too small for PSK\n");
					return -1;
				}

				memcpy(*key, psk[i].key, psk[i].key_length);
				return psk[i].key_length;
			}
		}
	}

	printf("verify_psk_callback failed\n");

	return -1;
}

static int coap_server_start(int argc, char *argv[])
{
	artik_error ret = S_OK;
	artik_coap_resource resources[2];
	int num_resources;

	if (started) {
		fprintf(stderr, "The server is already started\n");
		return -1;
	}

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		return -1;
	}

	memset(&g_config, 0, sizeof(artik_coap_config));
	memset(resources, 0, 2 * sizeof(artik_coap_resource));

	if (argc > 3 && !strcmp(argv[3], "psk")) {
		g_config.psk = malloc(sizeof(artik_coap_psk_param));

		if (!g_config.psk) {
			fprintf(stderr, "Memory problem\n");
			ret = -1;
			goto exit;
		}

		memset(g_config.psk, 0, sizeof(artik_coap_psk_param));

		if (argc > 4) {
			g_config.psk->identity = strdup(argv[4]);

			if (!g_config.psk->identity) {
				fprintf(stderr, "Memory problem\n");
				ret = -1;
				goto exit;
			}
		} else {
			fprintf(stderr, "Identity required\n");
			ret = -1;
			goto exit;
		}

		if (argc > 5) {
			g_config.psk->psk = strdup(argv[5]);

			if (!g_config.psk->psk) {
				fprintf(stderr, "Memory problem\n");
				ret = -1;
				goto exit;
			}

			g_config.psk->psk_len = strlen(argv[5]);
		} else {
			fprintf(stderr, "PSK required\n");
			ret = -1;
			goto exit;
		}
	} else if (argc > 3 && !strcmp(argv[3], "ecdsa")) {
		g_config.ssl = (artik_ssl_config *)malloc(sizeof(artik_ssl_config));

		if (!g_config.ssl) {
			fprintf(stderr, "Memory problem\n");
			ret = -1;
			goto exit;
		}

		memset(g_config.ssl, 0, sizeof(artik_ssl_config));

		g_config.ssl->client_cert.data = (char *)client_cert;
		g_config.ssl->client_cert.len = strlen(client_cert) + 1;

		g_config.ssl->client_key.data = (char *)client_key;
		g_config.ssl->client_key.len = strlen(client_key) + 1;
	} else if (argc > 3 && strcmp(argv[3], "psk") && strcmp(argv[3], "ecdsa")) {
		fprintf(stderr, "psk or ecdsa parameter only\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->create_server(&f_handle, &g_config);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to create server\n");
		f_handle = NULL;
		ret = -1;
		goto exit;
	}

	interface.dataRes = &dataRes;
	interface.handle = f_handle;

	resources[0].path = "info";
	resources[0].path_len = 4;
	resources[0].default_notification_type = ARTIK_COAP_RESOURCE_NOTIFY_CON;
	resources[0].attributes = (artik_coap_attr *)malloc(2*sizeof(artik_coap_attr));

	if (!resources[0].attributes) {
		fprintf(stderr, "Memory problem\n");
		f_handle = NULL;
		ret = -1;
		goto exit;
	}

	memset(resources[0].attributes, 0, 2*sizeof(artik_coap_attr));

	resources[0].attributes[0].name = (unsigned char *)"ct";
	resources[0].attributes[0].name_len = 2;
	resources[0].attributes[0].val = (unsigned char *)"0";
	resources[0].attributes[0].val_len = 1;

	resources[0].attributes[1].name = (unsigned char *)"title";
	resources[0].attributes[1].name_len = 5;
	resources[0].attributes[1].val = (unsigned char *)"\"General Info\"";
	resources[0].attributes[1].val_len = 14;

	resources[0].resource_cb[0] = get_info_callback;
	resources[0].resource_data[0] = NULL;

	resources[0].num_attributes = 2;

	resources[1].path = "test";
	resources[1].path_len = 4;
	resources[1].default_notification_type = ARTIK_COAP_RESOURCE_NOTIFY_CON;
	resources[1].attributes = (artik_coap_attr *)malloc(4*sizeof(artik_coap_attr));

	if (!resources[1].attributes) {
		fprintf(stderr, "Memory problem\n");
		f_handle = NULL;
		ret = -1;
		goto exit;
	}

	memset(resources[1].attributes, 0, 4*sizeof(artik_coap_attr));

	resources[1].attributes[0].name = (unsigned char *)"ct";
	resources[1].attributes[0].name_len = 2;
	resources[1].attributes[0].val = (unsigned char *)"0";
	resources[1].attributes[0].val_len = 1;

	resources[1].attributes[1].name = (unsigned char *)"title";
	resources[1].attributes[1].name_len = 5;
	resources[1].attributes[1].val = (unsigned char *)"\"Internal Buffer\"";
	resources[1].attributes[1].val_len = 17;

	resources[1].attributes[2].name = (unsigned char *)"rt";
	resources[1].attributes[2].name_len = 2;
	resources[1].attributes[2].val = (unsigned char *)"\"Data\"";
	resources[1].attributes[2].val_len = 6;

	resources[1].attributes[3].name = (unsigned char *)"if";
	resources[1].attributes[3].name_len = 2;
	resources[1].attributes[3].val = (unsigned char *)"\"buffer\"";
	resources[1].attributes[3].val_len = 8;

	resources[1].num_attributes = 4;

	resources[1].resource_cb[0] = get_test_callback;
	resources[1].resource_data[0] = (void *)&interface;

	resources[1].resource_cb[1] = post_test_callback;
	resources[1].resource_data[1] = (void *)&interface;

	resources[1].resource_cb[2] = put_test_callback;
	resources[1].resource_data[2] = (void *)&interface;

	resources[1].resource_cb[3] = delete_test_callback;
	resources[1].resource_data[3] = (void *)&interface;

	resources[1].observable = true;

	num_resources = 2;

	ret = g_coap->init_resources(f_handle, resources, num_resources);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to init resources\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->start_server(f_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to start server\n");
		ret = -1;
	}

	started = 1;

exit:
	artik_release_api_module(g_coap);
	if (resources[0].attributes)
		free(resources[0].attributes);
	if (resources[1].attributes)
		free(resources[1].attributes);
	if (g_config.psk && g_config.psk->identity)
		free((void *)g_config.psk->identity);
	if (g_config.psk && g_config.psk->psk)
		free((void *)g_config.psk->psk);
	if (g_config.psk)
		free(g_config.psk);
	if (g_config.ssl)
		free(g_config.ssl);

	return ret;
}

static int coap_server_stop(int argc, char *argv[])
{
	artik_error ret = S_OK;

	if (!started) {
		fprintf(stderr, "The server is not started\n");
		return -1;
	}

	g_coap = (artik_coap_module *)artik_request_api_module("coap");
	if (!g_coap) {
		fprintf(stderr, "CoAP module is not available\n");
		return -1;
	}

	ret = g_coap->stop_server(f_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to start server\n");
		ret = -1;
		goto exit;
	}

	ret = g_coap->destroy_server(f_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Fail to destroy server\n");
		ret = -1;
	}

	f_handle = NULL;

	started = false;

exit:
	artik_release_api_module(g_coap);
	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int coap_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, coap_commands);
}
