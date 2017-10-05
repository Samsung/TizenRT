/* ****************************************************************
*
* Copyright 2017 Samsung Electronics All Rights Reserved.
*
*
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <st_things/st_things.h>

#define TAG                     "LIGHT_USER"

static const char *PROPERTY_VALUE = "value";

static bool g_switch_value = true;

bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_get_request_on_switch() called..\n", TAG);

	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE)) {
		printf("[%s]current switch value: %d\n", TAG, g_switch_value);
		resp_rep->set_bool_value(resp_rep, PROPERTY_VALUE, g_switch_value);
	}

	printf("[%s]OUT-handle_get_request_on_switch() called..\n", TAG);
	return true;
}

bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_switch() called..\n", TAG);

	bool is_value_changed = false;

	bool desired_value = false;
	if (req_msg->rep->get_bool_value(req_msg->rep, PROPERTY_VALUE, &desired_value)) {
		printf("[%s]current switch value: %d\n", TAG, g_switch_value);
		printf("[%s]desired switch value: %d\n", TAG, desired_value);

		if (g_switch_value != desired_value) {
			g_switch_value = desired_value;
			printf("[%s]changed switch value: %d\n", TAG, g_switch_value);
			is_value_changed = true;
		}

		resp_rep->set_bool_value(resp_rep, PROPERTY_VALUE, g_switch_value);
	}

	if (is_value_changed) {
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_switch() called..\n", TAG);
	return true;
}
