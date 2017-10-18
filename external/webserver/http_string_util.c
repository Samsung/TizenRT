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

#include <tinyara/config.h>
#include <tinyara/progmem.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_server.h>

#include "http.h"
#include "http_string_util.h"
#include "http_log.h"

int http_find_first_crlf(const char *src, int len, int start)
{
	int i = 0;

	for (i = start + 1; i < len; i++) {
		/* Check CR and LF */
		if (src[i] == '\n' && src[i - 1] == '\r') {
			return i - 1;
		}
	}

	return -1;
}

int http_separate_header(const char *src, int *method, char *url, int *httpver)
{
	int i = 0;
	int divide[3] = {0, };
	int divide_pos = 0;

	int line_length = 0;
	int url_start = 0;
	int url_length = 0;
	int httpver_start = 0;

	*method = HTTP_METHOD_UNKNOWN;
	*httpver = HTTP_HTTP_VERSION_UNKNOWN;
	url[0] = '\0';

	line_length = strlen(src);

	/* Get divide positions */
	for (i = 0; i < line_length; i++) {
		if (src[i] == ' ') {
			divide[divide_pos++] = i;

			if (divide_pos >= 3) {
				HTTP_LOGE("Error: Not HTTP Header!!\n");
				return HTTP_ERROR;
			}
		}
	}

	/* Get request method */
	if (strncmp(src, "GET", 3) == 0) {
		*method = HTTP_METHOD_GET;
	} else if (strncmp(src, "PUT", 3) == 0) {
		*method = HTTP_METHOD_PUT;
	} else if (strncmp(src, "POST", 4) == 0) {
		*method = HTTP_METHOD_POST;
	} else if (strncmp(src, "DELETE", 6) == 0) {
		*method = HTTP_METHOD_DELETE;
	} else {
		HTTP_LOGE("Error: Invalid request method!!\n");
		return HTTP_ERROR;
	}

	/* Get url */
	url_start = divide[0] + 1;
	url_length = divide[1] - url_start;

	if (url_length <= 0 || url_length > HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH) {
		HTTP_LOGE("Error: Invalid url length!!\n");
		return HTTP_ERROR;
	}
	strncpy(url, src + url_start, url_length);
	url[url_length] = '\0';

	/* Get http version */
	httpver_start = divide[1] + 1;

	if (strncmp(src + httpver_start, "HTTP/1.1", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_11;
	} else if (strncmp(src + httpver_start, "HTTP/1.0", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_10;
	} else if (strncmp(src + httpver_start, "HTTP/0.9", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_09;
	}

	return HTTP_OK;
}

int http_separate_status_line(const char *src, int *httpver, int *status, char *phrase)
{
	int i = 0;
	int divide[6] = {0, };
	int divide_pos = 0;

	int line_length = 0;
	char status_code[4] = {0, };
	int status_start = 0;
	int phrase_start = 0;
	int phrase_length = 0;

	*httpver = HTTP_HTTP_VERSION_UNKNOWN;
	phrase[0] = '\0';

	line_length = strlen(src);

	/* Get divide positions */
	for (i = 0; i < line_length; i++) {
		if (src[i] == ' ') {
			divide[divide_pos++] = i;

			if (divide_pos >= (sizeof(divide) / sizeof(divide[0]))) {
				HTTP_LOGE("Error: Not HTTP status line!!\n");
				return HTTP_ERROR;
			}
		}
	}

	/* Get http version */
	if (strncmp(src, "HTTP/1.1", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_11;
	} else if (strncmp(src, "HTTP/1.0", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_10;
	} else if (strncmp(src, "HTTP/0.9", 8) == 0) {
		*httpver = HTTP_HTTP_VERSION_09;
	}

	/* Get status code */
	status_start = divide[0] + 1;
	strncpy(status_code, src + status_start, 4);
	*status = atoi(status_code);

	/* Get reason phrase */
	phrase_start = divide[1] + 1;
	phrase_length = line_length - phrase_start + 1;

	strncpy(phrase, src + phrase_start, phrase_length);

	return HTTP_OK;
}

int http_separate_keyvalue(const char *src, char *key, char *value)
{
	int i = 0;
	int value_position = 0;
	int src_len = strlen(src);

	for (i = 0; i < src_len; i++) {
		if (i >= HTTP_CONF_MAX_KEY_LENGTH) {
			HTTP_LOGE("Error: The key length is over the value buffer size.\n");
			return HTTP_ERROR;
		}
		if (src[i] != ':') {
			key[i] = src[i];
		} else {
			value_position = i + 2; /* 2 because at the front of value, there is a space */
			key[i] = '\0';
			break;
		}
	}

	for (i = value_position; i < src_len; i++) {
		if ((i - value_position) >= HTTP_CONF_MAX_VALUE_LENGTH) {
			HTTP_LOGE("Error: The value length is over the value buffer size.\n");
			return HTTP_ERROR;
		}
		value[i - value_position] = src[i];
	}
	value[i - value_position] = '\0';

	return HTTP_OK;
}
