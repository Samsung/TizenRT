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

#ifndef _CLOUD_CONNECTOR_H_
#define _CLOUD_CONNECTOR_H_

#include "ocstack.h"
#include "things_resource.h"
#include "utils/things_wait_handler.h"
#include "easy-setup/es_common.h"

#define DEFAULT_CONTEXT_VALUE   0x99

#define KEY_TOKEN_ACCESS           "accesstoken"    // mandatory
#define KEY_TOKEN_ACCESS_REFRESH   "refreshtoken"   // mandatory
#define KEY_TOKEN_TYPE             "token_type"     // mandatory
#define KEY_EXPIRE_TIME            "expiresin"      // mandatory
#define KEY_ID_USER                "uid"            // mandatory
#define KEY_SERVER_REDIRECT_URI    "redirect_uri"   // option
#define KEY_CERTIFICATE_FILE       "certificate"    // option
#define KEY_SERVER_ID              "sid"            // option
#define KEY_ID_DEVICE              "di"
#define KEY_LOGINOUT               "login"
#define KEY_ICORE_VER              "coreversion"
#define KEY_IOTIVITY_VER           "verticalversion"
#define KEY_TYPE_GRANT             "granttype"
#define CLOUD_ERROR_CODE           "code"
#define VALUE_TYPE_GRANT_TOKEN     "refresh_token"


int ci_connection_pre_check(const char *domain_name, char **p_ip);


OCStackResult things_cloud_signup(const char *host,
								  const char *device_id,
								  const es_cloud_prov_data_s *event_data,
								  OCClientResponseHandler response,
								  things_check_time_out_call_func timeout_handler,
								  things_timeout_s *timeout);

OCStackResult things_cloud_session(const char *host,
								   const char *uId,
								   const char *device_id,
								   const char *accesstoken,
								   bool is_login,
								   OCClientResponseHandler response,
								   things_check_time_out_call_func timeout_handler,
								   things_timeout_s *timeout);

OCStackResult things_cloud_rsc_publish(char *host,
									   things_resource_s **list,
									   int length,
									   OCClientResponseHandler response,
									   things_check_time_out_call_func timeout_handler,
									   things_timeout_s *timeout);

OCStackResult things_cloud_rsc_publish_with_device_id(char *host,
		const char *id,
		things_resource_s **list,
		int length,
		OCClientResponseHandler response,
		things_check_time_out_call_func timeout_handler,
		things_timeout_s *timeout);

OCStackResult things_cloud_dev_profile_publish(char *host,
		OCClientResponseHandler response,
		things_check_time_out_call_func timeout_handler,
		things_timeout_s *timeout);

OCStackResult things_cloud_refresh(const char *host,
								   const char *uId,
								   const char *device_id,
								   const char *refreshtoken,
								   OCClientResponseHandler response);

extern OCStackResult things_cloud_topic_publish_topic(const char *host,
		const char *topic,
		OCRepPayload *payload,
		OCClientResponseHandler response);

#endif
