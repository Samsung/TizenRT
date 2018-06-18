/* ****************************************************************
*
* Copyright 2018 Samsung Electronics All Rights Reserved.
*
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	  http://www.apache.org/licenses/LICENSE-2.0
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

#define TAG	"MSG_HANDLING_USER"

#define MAX_STR_LEN                                 32
#define MAX_RANGES                                  2
#define MAX_FILTER_SUPPORTED_STATES                 2
#define MAX_ROBOTCLEANER_SUPPORTED_MODES            3


#define PROPERTY_VALUE_SWITCH                       "power"

#define PROP_DOORSTATE                              "doorState"
#define PROP_AIRQUALITY                             "airQuality"
#define PROP_CT                                     "ct"
#define PROP_RANGE                                  "range"
#define PROP_TONE                                   "toneValue"
#define PROP_MACHINESTATES                          "machineStates"
#define PROP_CURRENTMACHINESTATE                    "currentMachineState"
#define PROP_MODES                                  "modes"
#define PROP_SUPPORTEDMODES                         "supportedModes"

#define ATTR_MODE_NORMAL                            "normal"
#define ATTR_MODE_REPLACE                           "replace"

#define ATTR_MODE_ON                                "on"
#define ATTR_MODE_OFF                               "off"
#define ATTR_MODE_SILENCE                           "silence"

static char *g_power_status[]                       = {"on", "off"};

static bool g_switch_value                          = false;
static char g_door_state[MAX_STR_LEN]               = "closed";
static bool g_tone                                  = false;

static double g_airqulity                           = 10.0;
static double g_airqulity_range[MAX_STR_LEN]        = {0., 20.};

static int64_t g_color_temp                         = 50;
static int64_t g_color_temp_range[MAX_STR_LEN]      = {0, 100};

static char g_filter_state[MAX_STR_LEN]             = "normal";
static char **g_filter_support_state                = NULL;

static size_t g_mode_cnt                            = 0;
static char **g_modes                               = NULL;
static char **g_robot_cleaner_supported_modes       = NULL;

void init_modes(void)
{
	int idx;

	// Filter state resource's attributes.
	g_filter_support_state = (char **)malloc(sizeof(char*) * MAX_FILTER_SUPPORTED_STATES);
	for (idx = 0; idx < MAX_ROBOTCLEANER_SUPPORTED_MODES; ++idx) {
		g_filter_support_state[idx] = malloc(sizeof(char) * MAX_STR_LEN);
	}

	strncpy(g_filter_support_state[0], ATTR_MODE_NORMAL, MAX_STR_LEN);
	strncpy(g_filter_support_state[1], ATTR_MODE_REPLACE, MAX_STR_LEN);

	// Robot cleaner resource's attributes.
	g_mode_cnt = 1;
	g_modes = (char **)malloc(sizeof(char*) * 1);
	g_modes[0] = malloc(sizeof(char) * MAX_STR_LEN);
	strncpy(g_modes[0], ATTR_MODE_ON, MAX_STR_LEN);

	g_robot_cleaner_supported_modes = (char **)malloc(sizeof(char*) * MAX_ROBOTCLEANER_SUPPORTED_MODES);
	for (idx = 0; idx < MAX_ROBOTCLEANER_SUPPORTED_MODES; ++idx) {
		g_robot_cleaner_supported_modes[idx] = malloc(sizeof(char) * MAX_STR_LEN);
	}

	strncpy(g_robot_cleaner_supported_modes[0], ATTR_MODE_ON, MAX_STR_LEN);
	strncpy(g_robot_cleaner_supported_modes[1], ATTR_MODE_OFF, MAX_STR_LEN);
	strncpy(g_robot_cleaner_supported_modes[2], ATTR_MODE_SILENCE, MAX_STR_LEN);
}

bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROPERTY_VALUE_SWITCH)) {
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_switch_value ? g_power_status[0] : g_power_status[1]);
	}
	return true;
}

bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	char *power;
	if (req_msg->rep->get_str_value(req_msg->rep, PROPERTY_VALUE_SWITCH, &power)) {
		if (strcmp(power, "off") == 0) {
			g_switch_value = false;
		} else {
			g_switch_value = true;
		}
		resp_rep->set_str_value(resp_rep, PROPERTY_VALUE_SWITCH, g_switch_value ? g_power_status[0] : g_power_status[1]);
		st_things_notify_observers(req_msg->resource_uri);
	}

	return true;
}

bool handle_get_request_on_resource_doorcontrol(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_DOORSTATE)) {
		resp_rep->set_str_value(resp_rep, PROP_DOORSTATE, g_door_state); // can be one of ["unknown", "open", "closing", "closed", "opening"]
	}
	return true;
}

bool handle_set_request_on_resource_doorcontrol(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	char *doorstate;
	if (req_msg->rep->get_str_value(req_msg->rep, PROP_DOORSTATE, &doorstate)) {
		printf("[%s] Door State : %s\n", TAG, doorstate);

		strncpy(g_door_state, doorstate, MAX_STR_LEN);

		resp_rep->set_str_value(resp_rep, PROP_DOORSTATE, g_door_state);
		st_things_notify_observers(req_msg->resource_uri);
		free(doorstate);
	}

	return true;
}

bool handle_get_request_on_resource_tone(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_TONE)) {
		resp_rep->set_bool_value(resp_rep, PROP_TONE, g_tone);
	}

	return true;
}

bool handle_set_request_on_resource_tone(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	bool tone;
	if (req_msg->rep->get_bool_value(req_msg->rep, PROP_TONE, &tone)) {
		g_tone = tone;
		resp_rep->set_bool_value(resp_rep, PROP_TONE, tone);
		st_things_notify_observers(req_msg->resource_uri);
	}

	return true;
}

bool handle_get_request_on_resource_wwstairqualitysensorlevelresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_AIRQUALITY)) {
		resp_rep->set_double_value(resp_rep, PROP_AIRQUALITY, g_airqulity);
	}
	if (req_msg->has_property_key(req_msg, PROP_RANGE)) {
		resp_rep->set_double_array_value(resp_rep, PROP_RANGE, g_airqulity_range, MAX_RANGES);
	}
	return true;
}


bool handle_get_request_on_resource_wwstcolortemperatureresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_CT)) {
		resp_rep->set_int_value(resp_rep, PROP_CT, g_color_temp);
	}
	if (req_msg->has_property_key(req_msg, PROP_RANGE)) {
		resp_rep->set_int_array_value(resp_rep, PROP_RANGE, g_color_temp_range, MAX_RANGES);
	}
	return true;
}

bool handle_set_request_on_resource_wwstcolortemperatureresuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	int64_t color_temp = 0;
	if (req_msg->rep->get_int_value(req_msg->rep, PROP_CT, &color_temp)) {
		g_color_temp = color_temp;
		resp_rep->set_int_value(resp_rep, PROP_CT, g_color_temp);
	}
	return true;
}

bool handle_get_request_on_resource_wwstfilterstateresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_MACHINESTATES)) {
		resp_rep->set_str_array_value(resp_rep, PROP_MACHINESTATES, (const char **)g_filter_support_state, MAX_FILTER_SUPPORTED_STATES);
	}
	if (req_msg->has_property_key(req_msg, PROP_CURRENTMACHINESTATE)) {
		resp_rep->set_str_value(resp_rep, PROP_CURRENTMACHINESTATE, g_filter_state);
	}
	return true;
}

bool handle_set_request_on_resource_wwstfilterstateresuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	char *machinestates;
	if (req_msg->rep->get_str_value(req_msg->rep, PROP_MACHINESTATES, &machinestates)) {
		strncpy(g_filter_state, machinestates, MAX_STR_LEN);
		resp_rep->set_str_value(resp_rep, PROP_MACHINESTATES, machinestates);
		st_things_notify_observers(req_msg->resource_uri);
	}

	return true;
}

bool handle_get_request_on_resource_wwstrobotcleanerturbomoderesuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_MODES)) {
		resp_rep->set_str_array_value(resp_rep, PROP_MODES, (const char **)g_modes, g_mode_cnt);
	}
	if (req_msg->has_property_key(req_msg, PROP_SUPPORTEDMODES)) {
		resp_rep->set_str_array_value(resp_rep, PROP_SUPPORTEDMODES, (const char **)g_robot_cleaner_supported_modes, MAX_ROBOTCLEANER_SUPPORTED_MODES);
	}

	return true;
}

bool handle_set_request_on_resource_wwstrobotcleanerturbomoderesuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	char **req_str_array;
	size_t array_size;
	int idx;

	if (req_msg->rep->get_str_array_value(req_msg->rep, PROP_MODES, &req_str_array, &array_size)) {
		for (idx = 0; idx < g_mode_cnt; ++idx) {
			free(g_modes[idx]);
		}
		free(g_modes);

		g_modes = req_str_array;
		g_mode_cnt = array_size;
	}

	return true;
}
