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

#define PUSH_MESG_SOURCE_ATTRIBUTE    "x.org.iotivity.ns.source"
#define PUSH_MESG_TYPE_ATTRIBUTE      "x.org.iotivity.ns.type"
#define PUSH_MESG_CODE_ATTRIBUTE      "x.com.samsung.code"

static const char *PROPERTY_VALUE_SWITCH = "power";
static const char *PROPERTY_VALUE_DIMMING = "dimmingSetting";
static const char *PROPERTY_VALUE_RANGE = "range";
static const char *PROPERTY_VALUE_STEP = "step";
static const char *PROPERTY_VALUE_COLOR_TEMPERATURE = "ct";

static char *g_power_status[] = { "off", "on" };

static const char *g_push_res_uri = "/service/pushservice";

static char *g_power;
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
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_power_status[g_switch_value]);
	}

	printf("[%s]OUT-handle_get_request_on_switch() called..\n", TAG);
	return true;
}

bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("[%s]IN-handle_set_request_on_switch() called..\n", TAG);
	if (req_msg->rep->get_str_value(req_msg->rep, PROPERTY_VALUE_SWITCH, &g_power)) {
		printf("[%s] power : %s\n", TAG, g_power);
		if (strncmp(g_power, "off", sizeof("off")) == 0) {
			g_switch_value = 0;
		} else {
			g_switch_value = 1;
		}

#ifdef CONFIG_DEMO_LED
		// control LEDs
		led_set_switch(g_switch_value);
		led_set_dimming(g_dimming_setting);
		led_set_color_temprature(g_color_temp);
#endif
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_power_status[g_switch_value]);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_switch() called..\n", TAG);
	return true;
}

// Change value of switch using switch(702) at artik board
void change_switch_value(void)
{
	printf("[%s] current power value: %s\n", TAG, g_power);
	if (g_switch_value == 0) {
		g_switch_value = 1;
	} else {
		g_switch_value = 0;
	}

#ifdef CONFIG_DEMO_LED
	// control LEDs
	led_set_switch(g_switch_value);
	led_set_dimming(g_dimming_setting);
	led_set_color_temprature(g_color_temp);
#endif
	g_power = g_power_status[g_switch_value];
	printf("[%s] new power value : %s\n", TAG, g_power);
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
#ifdef CONFIG_DEMO_LED
		led_set_dimming(g_dimming_setting);
#endif
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
		printf("colortemp : %lld\n", g_color_temp);
#ifdef CONFIG_DEMO_LED
		led_set_color_temprature(g_color_temp);
#endif
		resp_rep->set_int_value(resp_rep, PROPERTY_VALUE_COLOR_TEMPERATURE, g_color_temp);
		st_things_notify_observers(req_msg->resource_uri);
	}

	printf("[%s]OUT-handle_set_request_on_ct() called..\n", TAG);
	return true;
}

void set_rep_push_mesg(void)
{
	st_things_representation_s *st_things_rep = st_things_create_representation_inst();
	if (st_things_rep == NULL) {
		printf("[%s]Failed to create st_things representation.\n", TAG);
		return;
	}
	st_things_rep->set_str_value(st_things_rep, PUSH_MESG_SOURCE_ATTRIBUTE, "/alarms/vs/0");
	st_things_rep->set_str_value(st_things_rep, PUSH_MESG_TYPE_ATTRIBUTE, "4");
	st_things_rep->set_str_value(st_things_rep, PUSH_MESG_CODE_ATTRIBUTE, (g_switch_value ? "Light On" : "Light Off"));
	int res = st_things_push_notification_to_cloud(g_push_res_uri, st_things_rep);
	printf("[%s]Send push notifiation to cloud!! %d\n", TAG, res);
}
