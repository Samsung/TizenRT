/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <st_things/st_things.h>

#include "blink.h"

#define TAG             "BLINK_THINGS"

static const char *g_res_switch = "/capability/switch/main/0";

extern bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

static void handle_things_status_change(st_things_status_e things_status)
{
	printf("[%s]Things status is changed: %d\n", TAG, things_status);
}

static bool handle_ownership_transfer_request(void)
{
	printf("[%s]Received a request for Ownership-transfer. \n", TAG);
	return true;
}

static bool handle_get_request(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("Received a GET request on %s\n", req_msg->resource_uri);

	if (0 == strncmp(req_msg->resource_uri, g_res_switch, strlen(g_res_switch))) {
		return handle_get_request_on_switch(req_msg, resp_rep);
	}

	return false;
}

static bool handle_set_request(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("Received a SET request on %s\n", req_msg->resource_uri);

	if (0 == strncmp(req_msg->resource_uri, g_res_switch, strlen(g_res_switch))) {
		return handle_set_request_on_switch(req_msg, resp_rep);
	}

	return false;
}

int ess_process(void)
{
	bool easysetup_complete = false;
	st_things_initialize("device_def.json", &easysetup_complete);

	blink_init();
	blink_start();
	st_things_register_request_cb(handle_get_request, handle_set_request);
	st_things_register_user_confirm_cb(handle_ownership_transfer_request);
	st_things_register_things_status_change_cb(handle_things_status_change);
	if (st_things_start() == ST_THINGS_ERROR_NONE) {
		printf("[%s]=====================================================\n", TAG);
		printf("[%s]                    Stack Started                    \n", TAG);
		printf("[%s]=====================================================\n", TAG);
	}

	return 0;
}
