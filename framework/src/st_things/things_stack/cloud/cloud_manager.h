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

#ifndef _CLOUD_MANAGER_H_
#define _CLOUD_MANAGER_H_

#include "things_resource.h"
#include "easy-setup/es_common.h"
#include "cloud_connector.h"
#include "framework/things_server_builder.h"
#include <stdint.h>

#define CI_COMMON_REFRESH_TIME      12      // access token Refresh period time without accesstoken-expire. (unit: hour)
#define CI_TOKEN_EXPIRECHECK_TIME   3000    // access token Check period time whether it's expired or available. (unit: second)

#define CLOUD_EXPIRESIN_INVALID     -1

#ifdef __SECURED__
#define DEFAULT_COAP_TCP_HOST       "coaps+tcp://"
#define UNSUPPORT_COAP_TCP_HOST     "coap+tcp://"
#define DEFAULT_COAP_PORT           "443"
#else
#define DEFAULT_COAP_TCP_HOST       "coap+tcp://"
#define UNSUPPORT_COAP_TCP_HOST     "coaps+tcp://"
#define DEFAULT_COAP_PORT           "5683"
#endif

typedef enum rp_target_e {
	RSC_PUB_ALL = 0,
	RSC_PUB_MAIN_ONLY = 1,
	RSC_PUB_SUB_ALL = 2,
	RSC_PUB_NEED_SUB_ONLY = 3,
} rp_target_e;

typedef struct es_cloud_signup_s {
	char *domain;
	char *address;
	char *port;
	char *access_token;
	char *refresh_token;
	char *token_type;
	int64_t expire_time;
	char *uid;
	char *redirect_uri;
	char *certificate;
	char *sid;
} es_cloud_signup_s;

void *es_cloud_init(things_server_builder_s *server_builder);

void es_cloud_terminate(void);

void es_cloud_signup_clear(es_cloud_signup_s *data);

bool es_cloud_signup_init(es_cloud_signup_s **data);

bool es_cloud_session_stop_trigger(things_es_enrollee_state_e es_state);

OCStackResult publish_resource_into_cloud(rp_target_e target, things_timeout_s *timeout);

OCStackResult log_in_out_to_cloud(bool value, things_timeout_s *timeout);

OCStackResult find_cloud_resource(void);

OCStackResult refresh_token_into_cloud(void);

void *cloud_data_cb_esm(es_cloud_prov_data_s *event_data);

int cloud_retry_sign_in(things_timeout_s *timeout);

int ci_retry_stop_by_wifi_cb(bool is_retry);

int ci_retry_stop_by_tcp_cb(const char *addr_ip, const int port);

int push_notification_to_cloud(const char *uri, OCRepPayload *payload);

void ci_stop_cloud_connection(void *cb_func);

#endif							// _CLOUD_MANAGER_H_
