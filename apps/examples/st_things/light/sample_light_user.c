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

#define TAG	"LIGHT_USER"

static const char *PROPERTY_VALUE_SWITCH = "power";
static const char *PROPERTY_VALUE_DIMMING = "dimmingSetting";
static const char *PROPERTY_VALUE_COLOR_TEMPERATURE = "ct";

static char *power_status[] = { "on", "off" };

static bool g_switch_value = false;
static int64_t g_dimming_setting = 50;
static int64_t g_color_temp = 50;

bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_get_request_on_switch() called..\n", TAG);

	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_SWITCH)) {
		st_things_representation_s *temp_rep = st_things_create_representation_inst();
		if (temp_rep == NULL) {
			printf("Create representation inst Failed\n");
			return false;
		}
		temp_rep->set_str_value(temp_rep, "company", "Samsung");
		temp_rep->set_str_value(temp_rep, "sdk", "ST Things SDK");

		printf("[%s]current switch value: %d\n", TAG, g_switch_value);

		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_switch_value ? power_status[0] : power_status[1]);
		resp_rep->set_object_value(resp_rep, "userInfo", temp_rep);
		st_things_destroy_representation_inst(temp_rep);
	}

	printf("[%s]OUT-handle_get_request_on_switch() called..\n", TAG);
	return true;
}

bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_switch() called..\n", TAG);

	char *power;
	if (req_msg->rep->get_str_value(req_msg->rep, PROPERTY_VALUE_SWITCH, &power)) {
		printf("[%s] power : %s\n", TAG, power);
		if (strcmp(power, "off") == 0) {
			g_switch_value = false;
		} else {
			g_switch_value = true;
		}
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_switch_value ? power_status[0] : power_status[1]);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_switch() called..\n", TAG);
	return true;
}

bool handle_get_request_on_dimming(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_get_request_on_dimming() called..\n", TAG);

	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_DIMMING)) {
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_DIMMING, g_dimming_setting);
	}

	printf("[%s]OUT-handle_get_request_on_dimming() called..\n", TAG);
	return true;
}

bool handle_set_request_on_dimming(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_dimming() called..\n", TAG);

	if (req_msg->rep->get_int_value(req_msg->rep, PROPERTY_VALUE_DIMMING, &g_dimming_setting)) {
		printf("dimming : %lld\n", g_dimming_setting);
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_DIMMING, g_dimming_setting);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_dimming() called..\n", TAG);
	return true;
}

bool handle_get_request_on_ct(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_get_request_on_ct() called..\n", TAG);

	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_COLOR_TEMPERATURE)) {
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_COLOR_TEMPERATURE, g_color_temp);
	}

	printf("[%s]OUT-handle_get_request_on_ct() called..\n", TAG);
	return true;
}

bool handle_set_request_on_ct(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_ct() called..\n", TAG);

	if (req_msg->rep->get_int_value(req_msg->rep, PROPERTY_VALUE_COLOR_TEMPERATURE, &g_color_temp)) {
		printf("color_temp : %lld\n", g_color_temp);
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_COLOR_TEMPERATURE, g_color_temp);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_ct() called..\n", TAG);
	return true;
}
