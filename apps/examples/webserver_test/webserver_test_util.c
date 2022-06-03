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

#include "webserver_test_util.h"

char* ws_test_strcpy(char *dest, const char *src, struct http_client_request_t *ws)
{
	int len = strlen(src);

	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

char* ws_test_strlencpy(char *dest, const char *src, int len, struct http_client_request_t *ws)
{
	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

int ws_test_http_client_response_init(struct http_client_response_t *response)
{
	if(response == NULL) {
		PRNT("Error: NULL Response");
		return WS_TEST_ERR;
	}
	response->phrase = malloc(WS_TEST_CONF_MAX_PHRASE_SIZE);
	if (response->phrase == NULL) {
		PRNT("Error: Fail to init");
		return WS_TEST_ERR;
	}
	response->message = malloc(WS_TEST_CONF_MAX_MESSAGE_SIZE);
	if (response->message == NULL) {
		PRNT("Error: Fail to init");
		free(response->phrase);
		return WS_TEST_ERR;
	}
	memset(response->message, 0, WS_TEST_CONF_MAX_MESSAGE_SIZE);

	response->headers = malloc(sizeof(struct http_keyvalue_list_t));
	if (response->headers == NULL || http_keyvalue_list_init(response->headers) < 0) {
		PRNT("Error: Fail to init");
		free(response->phrase);
		free(response->message);
		return WS_TEST_ERR;
	}

	response->entity = NULL;
	return 0;
}

void ws_test_http_client_response_release(struct http_client_response_t *response)
{
	free(response->phrase);
	free(response->message);
	if (http_keyvalue_list_release(response->headers) != 0) {
		PRNT("http_keyvalue_list_release(response->headers) failed");
	}
	free(response->headers);
}

char* ws_test_chunksize(char *dest, int len, struct http_client_request_t *ws)
{
	FUNC_EN;
	char size[10] = {0,};

	if (!dest) {
		return NULL;
	}

	snprintf(size, sizeof(size), "%x", len);
	len = strlen(size);
	if (dest + len - ws->buffer >= ws->buflen) {
		PRNT("Error: buffer is too small");
		return NULL;
	}
	memcpy(dest, size, len);
	FUNC_EX;
	return dest + len;
}

int find_first_crlf(const char *src, int len, int start)
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

int separate_status_line(const char *src, int *status, char *phrase)
{
	int i = 0;
	int divide[6] = {0, };
	int divide_pos = 0;

	int line_length = 0;
	char status_code[4] = {0, };
	int status_start = 0;
	int phrase_start = 0;
	int phrase_length = 0;

	phrase[0] = '\0';

	line_length = strlen(src);

	/* Get divide positions */
	for (i = 0; i < line_length; i++) {
		if (src[i] == ' ') {
			divide[divide_pos++] = i;

			if (divide_pos >= (sizeof(divide) / sizeof(divide[0]))) {
				printf("Error: Not HTTP status line!!\n");
				return WS_TEST_ERR;
			}
		}
	}

	/* Get status code */
	status_start = divide[0] + 1;
	strncpy(status_code, src + status_start, 4);
	*status = atoi(status_code);

	/* Get reason phrase */
	phrase_start = divide[1] + 1;
	phrase_length = line_length - phrase_start + 1;

	strncpy(phrase, src + phrase_start, phrase_length);

	return WS_TEST_OK;
}

int separate_keyvalue(const char *src, char *key, char *value)
{
	int i = 0;
	int value_position = 0;
	int src_len = strlen(src);

	for (i = 0; i < src_len; i++) {
		if (i >= HTTP_CONF_MAX_KEY_LENGTH) {
			printf("Error: The key length is over the value buffer size.\n");
			return WS_TEST_ERR;
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
			printf("Error: The value length is over the value buffer size.\n");
			return WS_TEST_ERR;
		}
		value[i - value_position] = src[i];
	}
	value[i - value_position] = '\0';

	return WS_TEST_OK;
}

int parsehttpurl(const char *url, uint16_t *port, char *hostname, int hostlen) {
	FUNC_EN;
	const char *src = url;
	char *dest;
	int bytesleft;
	int https = 0;

	/* A valid HTTP URL must begin with http:// if it does not, we will assume
	 * that it is a file name only, but still return an error.  wget() depends
	 * on this strange behavior.
	 */
	if (strncmp(src, "https://", 8) == 0) {
		https = 1;
		*port = 443;
	} else if (strncmp(src, "http://", 7) == 0) {
		*port = 80;
	} else {
		return WS_TEST_ERR;
	}

	/* Skip over the http:// */
	if (https) {
		src += 8;
	} else {
		src += 7;
	}

	/* Concatenate the hostname following http:// and up to the termnator */
	dest = hostname;
	bytesleft = hostlen;
	while (*src != '\0' && *src != '/' && *src != ' ' && *src != ':') {
		/* Make sure that there is space for another character in the hostname.
		 * (reserving space for the null terminator)
		 */

		if (bytesleft > 1) {
			*dest++ = *src++;
			bytesleft--;
		} else {
			return WS_TEST_ERR;
		}
	}
	*dest = '\0';

	FUNC_EX;
	return WS_TEST_OK;
}

void parse_header(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 int *read_finish, int *process_finish)
{
	int sentence_end = 0;
	len->sentence_start = 0;
	len->message_len = 0;
	/* Search CR and LF */
	sentence_end = find_first_crlf(buf, buf_len, len->sentence_start);
	if (sentence_end != -1) {
		buf[sentence_end] = '\0';

		if(response) {
			separate_status_line(buf + len->sentence_start, &response->status, response->phrase);
			printf("Response Status : %d\n", response->status);
			printf("Response Phrase : %s\n", response->phrase);
		}
		len->sentence_start = sentence_end + 2;
		*state = HTTP_REQUEST_PARAMETERS;
	} else {
		*read_finish = true;
		*process_finish = true;
	}
}

int parse_parameter(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 int *process_finish)
{
	/* Search CR and LF */
	char key[HTTP_CONF_MAX_KEY_LENGTH] = { 0, };
	char value[HTTP_CONF_MAX_VALUE_LENGTH] = { 0, };
	int sentence_end = http_find_first_crlf(buf, buf_len, len->sentence_start);
	if (sentence_end >= 0) {
		buf[sentence_end] = '\0';
		if (strlen(buf + len->sentence_start) > 0) {
			/* Read parameters */
			int result = http_separate_keyvalue(buf + len->sentence_start, key, value);
			if (result == WS_TEST_ERR) {
				printf("Error: Fail to separate keyvalue\n");
				return WS_TEST_ERR;
			}
			printf("%s : %s\n", key, value);

			if (strncmp(key, "Content-Length", strlen("Content-Length")+1) == 0) {
				len->content_len = atoi(value);
				if (response) {
					response->total_len = len->content_len;
				}

				printf("This request contains contents, length : %d\n", len->content_len);
			}
		} else {
			*state = HTTP_REQUEST_BODY;
		}
		len->sentence_start = sentence_end + 2;
	} else {
		*process_finish = true;
	}
	return WS_TEST_OK;	
}

void parse_body(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 char **body, int *read_finish, int *process_finish)
{
	if (!len->message_len) {
		*body = buf + len->sentence_start;
	}

	len->message_len = buf_len;

	if (buf_len - len->sentence_start == len->content_len) {
		buf[buf_len] = '\0';
		if (response) {
			response->total_len = len->content_len;

				response->entity_len = buf_len - len->sentence_start;
				response->entity = buf + len->sentence_start;
				PRNT("response entity: %s\n",response->entity);
		}

		*read_finish = true;
	} else {
		printf("Not all body readed\n");
	}
	*process_finish = true;
}

int parse_message(char *buf, int buf_len, char *url,
					   char **body, int *state,
					   struct http_message_len_t *len,
					   struct http_client_response_t *response)
{
	int process_finish = false;
	int read_finish = false;

	while (!process_finish) {
		/* At this point, we read a line of http request */
		switch (*state) {
		case HTTP_REQUEST_HEADER:
			parse_header(len, buf, buf_len, response, state, &read_finish, &process_finish);
			break;
		case HTTP_REQUEST_PARAMETERS:
			if (parse_parameter(len, buf, buf_len, response, state, &process_finish) == WS_TEST_ERR) {
				return WS_TEST_ERR;
			}
			break;
		case HTTP_REQUEST_BODY:
			parse_body(len, buf, buf_len, response, state, body, &read_finish, &process_finish);
			break;
		}
	}
	return read_finish;
}
