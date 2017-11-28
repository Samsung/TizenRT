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
 * @file http-api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_http.h>

#include "command.h"

#ifdef CONFIG_EXAMPLES_ARTIK_HTTP
#include "wifi-auto.h"
#endif

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

static int http_get(int argc, char **argv);
static int http_post(int argc, char **argv);
static int http_put(int argc, char **argv);
static int http_delete(int argc, char **argv);
static int http_get_async(int argc, char **argv);
static int http_post_async(int argc, char **argv);
static int http_put_async(int argc, char **argv);
static int http_delete_async(int argc, char **argv);

const struct command http_commands[] = {
	{ "get", "get <url>", http_get },
	{ "post", "post <url> <body>", http_post },
	{ "put", "put <url> <body>", http_put },
	{ "delete", "delete <url>", http_delete },
	{ "get-async", "get-async <url>", http_get_async },
	{ "post-async", "post-async <url> <body>", http_post_async },
	{ "put-async", "put-async <url> <body>", http_put_async },
	{ "delete-async", "delete-async <url>", http_delete_async },
	{ "", "", NULL }
};

static int http_get(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	char *response = NULL;
	int status = 0;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 4) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	printf("uri = %s\n", argv[3]);
	ret = http->get(argv[3], &headers, &response, &status, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	if (response) {
		fprintf(stderr, "HTTP %d - %s\n", status, response);
		free(response);
	}

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_post(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	char *response = NULL;
	int status = 0;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 5) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->post(argv[3], &headers, argv[4], &response, &status, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	if (response) {
		fprintf(stderr, "HTTP %d - %s\n", status, response);
		free(response);
	}

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_put(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	char *response = NULL;
	int status = 0;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 4) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->put(argv[3], &headers, argv[4], &response, &status, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	if (response) {
		fprintf(stderr, "HTTP %d - %s\n", status, response);
		free(response);
	}

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_delete(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	char *response = NULL;
	int status = 0;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 3) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->del(argv[3], &headers, &response, &status, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	if (response) {
		fprintf(stderr, "HTTP %d - %s\n", status, response);
		free(response);
	}

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static void http_response_callback(artik_error result, int status,
								char *response, void *user_data)
{
	if (result != S_OK)
		fprintf(stderr, "error = %s\n", error_msg(result));
	else
		fprintf(stdout, "response = %s\nstatus = %d\n", response,
								status);
}

static int http_get_async(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 4) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	printf("uri = %s\n", argv[3]);
	ret = http->get_async(argv[3], &headers, http_response_callback, NULL, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	fprintf(stdout, "ASYNC GET %s", argv[3]);

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_post_async(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 5) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->post_async(argv[3], &headers, argv[4],
			http_response_callback, NULL, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	fprintf(stdout, "ASYNC POST %s", argv[3]);

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_put_async(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 4) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->put_async(argv[3], &headers, argv[4],
			http_response_callback, NULL, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	fprintf(stdout, "ASYNC PUT %s", argv[3]);
exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

static int http_delete_async(int argc, char **argv)
{
	artik_http_module *http = (artik_http_module *)artik_request_api_module("http");
	artik_error ret = S_OK;
	artik_http_headers headers;
	artik_http_header_field fields[] = {
		{"Connect", "close"},
		{"User-Agent", "Artik browser"},
		{"Accept-Language", "en-US,en;q=0.8"},
	};

	if (argc < 3) {
		fprintf(stderr, "Wrong number of arguments");
		goto exit;
	}

	headers.fields = fields;
	headers.num_fields = ARRAY_SIZE(fields);

	ret = http->del_async(argv[3], &headers,
			http_response_callback, NULL, NULL);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to get %s (err:%s)\n", argv[3], error_msg(ret));
		goto exit;
	}

	fprintf(stdout, "ASYNC DELETE %s", argv[3]);

exit:
	artik_release_api_module(http);
	return (ret == S_OK);
}

int http_main(int argc, char *argv[])
{
	return commands_parser(argc, argv, http_commands);
}

#ifdef CONFIG_EXAMPLES_ARTIK_HTTP
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int artik_http_main(int argc, char *argv[])
#endif
{
	StartWifiConnection();
	return 0;
}
#endif
