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

#ifndef _THINGS_REQ_HANDLER_H_
#define _THINGS_REQ_HANDLER_H_

#include "ocstack.h"
#include "things_common.h"
#include "things_resource.h"
#include "things_api.h"
#include "octypes.h"

typedef struct things_request_handler_s {
	request_handler_cb entity_handler;

	void (*init_module)(void);
	void (*deinit_module)(void);
	int (*notify_things_observers)(const char *uri);
} things_request_handler_s;

things_request_handler_s *get_handler_instance();

typedef int (*stop_softap_func_type)(int value);
typedef int (*handle_request_func_type)(struct things_resource_s *res);
typedef char *(*get_notify_obs_uri_cb)(const char *Uri, const char *query);
typedef OCEntityHandlerResult(*handle_request_interface_cb)(things_resource_s **ppst_target_resource);

void release_handler_instance(things_request_handler_s *);
void notify_result_of_reset(things_resource_s *target_resource, bool result);
int register_handle_request_func(handle_request_func_type get_func, handle_request_func_type set_func);

#endif							// _THINGS_REQ_HANDLER_H_
