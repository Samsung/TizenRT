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

#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_keyvalue_list.h>

#include "http.h"
#include "http_client.h"
#include "http_query.h"
#include "http_arch.h"
#include "http_log.h"

static int http_compare_dq(struct http_divided_query_t *src, struct http_divided_query_t *dest, struct http_keyvalue_list_t *params_list)
{
	int i = 0;

	if (src->slash_count == dest->slash_count) {
		bool same = true;
		for (i = 0; i < src->slash_count; i++) {
			char *src_path = src->paths + (i * HTTP_CONF_MAX_DIVIDED_PATH_LENGTH);
			char *dest_path = dest->paths + (i * HTTP_CONF_MAX_DIVIDED_PATH_LENGTH);

			if (strcmp(src_path, dest_path) != 0) {
				/*
				 * Consider variable string such as ':id', ':num'
				 */
				if (src_path[1] != ':' && dest_path[1] != ':') {
					same = false;
					break;
				} else {
					if (params_list) {
						if (src_path[1] == ':') {
							HTTP_LOGD("Add Parameter : [%s: %s]\n", src_path + 2, dest_path + 1);
							http_keyvalue_list_add(params_list, src_path + 2, dest_path + 1);
						} else if (dest_path[1] == ':') {
							HTTP_LOGD("Add Parameter : [%s: %s]\n", dest_path + 2, src_path + 1);
							http_keyvalue_list_add(params_list, dest_path + 2, src_path + 1);
						}
					}
				}
			}
		}

		if (same) {
			return HTTP_OK;
		}
	}

	return HTTP_ERROR;
}

int http_dispatch_url(struct http_client_t *client, struct http_req_message *req)
{
	int i = 0;
	char query[HTTP_CONF_MAX_URL_QUERY_LENGTH] = {0, };
	char params[HTTP_CONF_MAX_URL_PARAMS_LENGTH] = {0, };
	struct http_divided_query_t dq;
	struct http_keyvalue_list_t params_list;
	char *origin_url = req->url;

	if (http_divide_query_params(req->url, query, params)) {
		return HTTP_ERROR;
	}
	req->url = query;
	req->query_string = params;

	http_parse_query(query, &dq);
	if (http_keyvalue_list_init(&params_list) == HTTP_ERROR) {
		http_keyvalue_list_release(&params_list);
		http_release_query(&dq);
		return HTTP_ERROR;
	}

	/* Parse url params */
	if (strlen(params) > 0) {
		http_parse_params(params, &params_list);
	}

	for (i = 0; i < HTTP_CONF_MAX_QUERY_HANDLER_COUNT; i++) {
		struct http_query_handler_t *cur = client->server->query_handlers[i];

		if (cur) {
			if (cur->method == req->method && http_compare_dq(&cur->dq, &dq, &params_list) == HTTP_OK) {
				cur->func(client, req);
				req->url = origin_url;
				http_keyvalue_list_release(&params_list);
				http_release_query(&dq);
				return HTTP_OK;
			}
		}
	}

	if (client->server->cb[req->method]) {
		client->server->cb[req->method](client, req);
	}

	req->url = origin_url;
	http_keyvalue_list_release(&params_list);
	http_release_query(&dq);
	return HTTP_OK;
}

int http_server_register_cb(struct http_server_t *server, int method, const char *url_format, http_cb_t func)
{
	int i = 0;
	int empty_slot = 0;
	struct http_query_handler_t *cur = NULL;

	if (server == NULL) {
		HTTP_LOGE("Error: Server is NULL\n");
		return HTTP_ERROR;
	}

	if (method < HTTP_METHOD_GET || method > HTTP_METHOD_DELETE) {
		HTTP_LOGE("Error: Incorrect method value!!\n");
		return HTTP_ERROR;
	}

	if (url_format == NULL) {
		server->cb[method] = func;
		return HTTP_OK;
	}

	/* Find empty slot */
	for (i = 0; i < HTTP_CONF_MAX_QUERY_HANDLER_COUNT; i++) {
		if (server->query_handlers[i] == NULL) {
			break;
		}
	}

	if (i == HTTP_CONF_MAX_QUERY_HANDLER_COUNT) {
		HTTP_LOGE("Error: Not exist empty dq slot!!\n");
		return HTTP_ERROR;
	}

	empty_slot = i;

	server->query_handlers[empty_slot] = (struct http_query_handler_t *)HTTP_MALLOC(sizeof(struct http_query_handler_t));
	if (server->query_handlers[empty_slot] == NULL) {
		HTTP_LOGE("Error : Cannot allocate dq slot!!\n");
		return HTTP_ERROR;
	}

	cur = server->query_handlers[empty_slot];

	HTTP_MEMSET(cur, 0, sizeof(struct http_query_handler_t));

	http_parse_query(url_format, &cur->dq);

	cur->method = method;
	cur->func = func;

	return HTTP_OK;
}

int http_server_deregister_cb(struct http_server_t *server, int method, const char *url_format)
{
	int i = 0;
	struct http_divided_query_t dq;

	if (server == NULL) {
		HTTP_LOGE("Error: Server is NULL\n");
		return HTTP_ERROR;
	}

	if (method < HTTP_METHOD_GET || method > HTTP_METHOD_DELETE) {
		HTTP_LOGE("Error: Incorrect method value!!\n");
		return HTTP_ERROR;
	}

	if (url_format == NULL) {
		server->cb[method] = NULL;
		return HTTP_OK;
	}

	http_parse_query(url_format, &dq);

	for (i = 0; i < HTTP_CONF_MAX_QUERY_HANDLER_COUNT; i++) {
		if (server->query_handlers[i] != NULL) {
			if (server->query_handlers[i]->method == method &&
				http_compare_dq(&server->query_handlers[i]->dq, &dq, NULL) == HTTP_OK) {
				http_release_query(&server->query_handlers[i]->dq);
				HTTP_FREE(server->query_handlers[i]);
				server->query_handlers[i] = NULL;
				http_release_query(&dq);
				return HTTP_OK;
			}
		}
	}

	http_release_query(&dq);
	return HTTP_ERROR;
}

int http_parse_query(const char *query, struct http_divided_query_t *dq)
{
	int i = 0;
	int query_len = (int)strlen(query);
	int slash_position[HTTP_CONF_MAX_SLASH_COUNT + 1] = {0, };

	HTTP_MEMSET(dq, 0, sizeof(struct http_divided_query_t));

	for (i = 0; i < query_len; i++) {
		if (query[i] == '/') {
			slash_position[dq->slash_count++] = i;
		}
	}

	/* Set the last slash position as query length. To calculate devided path length */
	slash_position[dq->slash_count] = query_len;

	dq->paths = (char *)HTTP_MALLOC(dq->slash_count * HTTP_CONF_MAX_DIVIDED_PATH_LENGTH);
	if (!(dq->paths)) {
		return HTTP_ERROR;
	}

	HTTP_MEMSET(dq->paths, 0, dq->slash_count * HTTP_CONF_MAX_DIVIDED_PATH_LENGTH);

	for (i = 0; i < dq->slash_count; i++) {
		HTTP_MEMCPY(dq->paths + (i * HTTP_CONF_MAX_DIVIDED_PATH_LENGTH),
					query + slash_position[i], slash_position[i + 1] - slash_position[i]);
	}

	return HTTP_OK;
}

int http_parse_params(const char *params, struct http_keyvalue_list_t *params_list)
{
	int i = 0;
	int params_len = strlen(params);

	char key[HTTP_CONF_MAX_KEY_LENGTH] = {0, };
	char value[HTTP_CONF_MAX_VALUE_LENGTH] = {0, };

	bool is_reading_key = false;
	int reading_position = 0;

	/* The minimum length of parameter is 3. For example, 'a=1' */
	if (params_len < 3) {
		return HTTP_ERROR;
	}

	is_reading_key = true;
	reading_position = 0;

	for (i = 0; i < params_len + 1; i++) { /* Consider the last '\0' character */
		if (is_reading_key && params[i] == '=') {
			key[reading_position++] = '\0';
			is_reading_key = false;
			reading_position = 0;
		} else if (!is_reading_key && (params[i] == '&' || params[i] == '#' ||
									   params[i] == '\0')) {
			value[reading_position++] = '\0';
			is_reading_key = true;
			reading_position = 0;

			HTTP_LOGD("Add Parameter : [%s: %s]\n", key, value);
			http_keyvalue_list_add(params_list, key, value);
		} else {
			if (is_reading_key) {
				key[reading_position++] = params[i];
			} else {
				value[reading_position++] = params[i];
			}
		}
	}

	return HTTP_OK;
}

void http_release_query(struct http_divided_query_t *dq)
{
	if (dq->paths) {
		HTTP_FREE(dq->paths);
	}
}

int http_divide_query_params(const char *url, char *query, char *params)
{
	int i = 0;
	int question_position = 0;
	int url_len = (int)strlen(url);

	/* Find '?' */
	for (i = 0; i < url_len; i++) {
		if (url[i] == '?') {
			break;
		}
	}
	question_position = i;

	if (question_position >= HTTP_CONF_MAX_URL_QUERY_LENGTH) {
		return HTTP_ERROR;
	}

	HTTP_MEMCPY(query, url, question_position);
	query[question_position] = '\0';

	if (question_position < url_len) {
		HTTP_MEMCPY(params, url + question_position + 1, url_len - question_position - 1);
		params[url_len - question_position - 1] = '\0';
	} else {
		params[0] = '\0';
	}

	/*
	 * If the last character of query is '/', delete it except root.
	 * Example : / --> /, /device/0/ --> /device/0
	 */
	if (strlen(query) > 1 && query[strlen(query) - 1] == '/') {
		query[strlen(query) - 1] = '\0';
	}

	return HTTP_OK;
}
