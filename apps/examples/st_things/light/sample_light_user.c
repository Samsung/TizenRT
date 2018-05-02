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
static const char *PROPERTY_VALUE_RANGE = "range";
static const char *PROPERTY_VALUE_STEP = "step";
static const char *PROPERTY_VALUE_COLOR_TEMPERATURE = "ct";

static char *power_status[] = { "off", "on" };

static int g_switch_value = 0;
static int64_t g_dimming_setting = 50;
static int64_t g_dimming_step = 5;
static int64_t g_color_temp = 50;
static int64_t g_dimming_range[] = {0, 100};
static int64_t g_color_range[] = {0, 100};

bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_get_request_on_switch() called..\n", TAG);

	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_SWITCH)) {
		printf("[%s]current switch value: %d\n", TAG, g_switch_value);
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, power_status[g_switch_value]);
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
		if (strncmp(power, "off", sizeof("off")) == 0) {
			g_switch_value = 0;
		} else {
			g_switch_value = 1;
		}
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, power_status[g_switch_value]);
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
	} else if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_RANGE)) {
		resp_rep->set_int_array_value(resp_rep, PROPERTY_VALUE_RANGE, g_dimming_range, 2);
	} else if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_STEP)) {
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_STEP, g_dimming_step);
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
	} else if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_RANGE)) {
		resp_rep->set_int_array_value(resp_rep, PROPERTY_VALUE_RANGE, g_color_range, 2);
	}

	printf("[%s]OUT-handle_get_request_on_ct() called..\n", TAG);
	return true;
}

bool handle_set_request_on_ct(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_ct() called..\n", TAG);

	if (req_msg->rep->get_int_value(req_msg->rep, PROPERTY_VALUE_COLOR_TEMPERATURE, &g_color_temp)) {
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_COLOR_TEMPERATURE, g_color_temp);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_ct() called..\n", TAG);
	return true;
}
