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

#ifndef __http_query_h__
#define __http_query_h__

#include <stdio.h>

struct http_divided_query_t {
	int slash_count;
	char *paths;
};

struct http_query_handler_t {
	int method;
	struct http_divided_query_t dq;
	http_cb_t func;
};

/* Pre definition */
struct http_server_t;
struct http_client_t;
struct http_keyvalue_list_t;

int  http_divide_query_params(const char *url, char *query, char *params);
int  http_parse_query(const char *query, struct http_divided_query_t *dq);
int  http_parse_params(const char *params, struct http_keyvalue_list_t *params_list);
void http_release_query(struct http_divided_query_t *dq);

int  http_dispatch_url(struct http_client_t *client, struct http_req_message *req);

#endif
