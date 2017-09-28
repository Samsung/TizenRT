/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __ST_THINGS_REQUEST_HANDLER_H__
#define __ST_THINGS_REQUEST_HANDLER_H__

#ifdef __ST_THINGS_RTOS__
#include <st_things/st_things.h>
#else
#include "st_things.h"
#endif							// __ST_THINGS_RTOS__
#include "things_resource.h"
#include "octypes.h"

#define ST_THINGS_RSRVD_INTERFACE_READWRITE "oic.if.rw"
#define ST_THINGS_RSRVD_INTERFACE_SENSOR "oic.if.s"
#define ST_THINGS_RSRVD_INTERFACE_ACTUATOR "oic.if.a"

bool get_query_value_internal(const char *query, const char *key, char **value, bool *found);

bool add_common_props(things_resource_s *rsrc, bool collection, OCRepPayload *resp_payload);

bool handle_get_req_helper(const char *res_uri, const char *query, OCRepPayload *resp_payload);

bool handle_post_req_helper(const char *res_uri, const char *query, OCRepPayload *req_payload, OCRepPayload *resp_payload);

int handle_get_request_cb(struct things_resource_s *resource);

int handle_set_request_cb(struct things_resource_s *resource);

int register_request_handler_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb);

#endif							// __ST_THINGS_REQUEST_HANDLER_H__
