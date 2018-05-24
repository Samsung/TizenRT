/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <st_things/st_things.h>
#include <iotbus/iotbus_gpio.h>

#define TAG	                                                     "MSG_HANDLING_THINGS"

#define MAX_URL_LEN                                              64

#define URI_SWITCH                                               "/capability/switch/main/0"
#define URI_SWITCH_LEN                                           sizeof(URI_SWITCH)

#define URI_COLOR_TEMPERATURE                                    "/capability/colorTemperature/main/0"
#define URI_COLOR_TEMPERATURE_LEN                                sizeof(URI_COLOR_TEMPERATURE)

#define URI_DOOR_CONTROL                                         "/capability/doorControl/main/0"
#define URI_DOOR_CONTROL_LEN                                     sizeof(URI_DOOR_CONTROL)

#define URI_AIRQUILITY                                           "/capability/airQualitySensor/main/0"
#define URI_AIRQUILITY_LEN                                       sizeof(URI_AIRQUILITY)

#define URI_TONE                                                 "/capability/tone/main/0"
#define URI_TONE_LEN                                             sizeof(URI_TONE)

#define URI_FILTER                                               "/capability/filterStatus/main/0"
#define URI_FILTER_LEN                                           sizeof(URI_FILTER)

#define URI_TURBO                                                "/capability/robotCleanerTurboMode/main/0"
#define URI_TURBO_LEN                                            sizeof(URI_FILTER)

extern void init_modes(void);

extern bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_wwstcolortemperatureresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_resource_wwstcolortemperatureresuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_doorcontrol(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_resource_doorcontrol(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_wwstairqualitysensorlevelresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_tone(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_resource_tone(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_wwstfilterstateresuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_resource_wwstfilterstateresuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

extern bool handle_get_request_on_resource_wwstrobotcleanerturbomoderesuri(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_resource_wwstrobotcleanerturbomoderesuri(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

static void handle_reset_result(bool result)
{
	printf("[%s]Reset %s.\n", result ? "succeeded" : "failed", TAG);
}

static void handle_things_status_change(st_things_status_e things_status)
{
	printf("[%s]Things status is changed: %d\n", TAG, things_status);
}

static bool handle_reset_request(void)
{
	printf("[%s]Received a request for RESET.\n", TAG);
	return true;
}

static bool handle_ownership_transfer_request(void)
{
	printf("[%s]Received a request for Ownership-transfer. \n", TAG);
	return true;
}

static bool handle_get_request(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("Received a GET request on %s\n", req_msg->resource_uri);

	if (0 == strncmp(req_msg->resource_uri, URI_SWITCH, URI_SWITCH_LEN)) {
		return handle_get_request_on_switch(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_COLOR_TEMPERATURE, URI_COLOR_TEMPERATURE_LEN)) {
		return handle_get_request_on_resource_wwstcolortemperatureresuri(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_DOOR_CONTROL, URI_DOOR_CONTROL_LEN)) {
		return handle_get_request_on_resource_doorcontrol(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_TONE, URI_TONE_LEN)) {
		return handle_get_request_on_resource_tone(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_AIRQUILITY, URI_AIRQUILITY_LEN)) {
		return handle_get_request_on_resource_wwstairqualitysensorlevelresuri(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_FILTER, URI_FILTER_LEN)) {
		return handle_get_request_on_resource_wwstfilterstateresuri(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_TURBO, URI_TURBO_LEN)) {
		return handle_get_request_on_resource_wwstrobotcleanerturbomoderesuri(req_msg, resp_rep);
	} else {
		printf("Not supported uri.\n");
	}

	return false;
}

static bool handle_set_request(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("Received a SET request on %s\n", req_msg->resource_uri);

	if (0 == strncmp(req_msg->resource_uri, URI_SWITCH, URI_SWITCH_LEN)) {
		return handle_set_request_on_switch(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_COLOR_TEMPERATURE, URI_COLOR_TEMPERATURE_LEN)) {
		return handle_set_request_on_resource_wwstcolortemperatureresuri(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_DOOR_CONTROL, URI_DOOR_CONTROL_LEN)) {
		return handle_set_request_on_resource_doorcontrol(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_TONE, URI_TONE_LEN)) {
		return handle_set_request_on_resource_tone(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_FILTER, URI_FILTER_LEN)) {
		return handle_set_request_on_resource_wwstfilterstateresuri(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, URI_TURBO, URI_TURBO_LEN)) {
		return handle_set_request_on_resource_wwstrobotcleanerturbomoderesuri(req_msg, resp_rep);
	}

	return false;
}

int ess_process(void)
{
	bool easysetup_complete = false;
	st_things_initialize("device_def.json", &easysetup_complete);

	st_things_register_request_cb(handle_get_request, handle_set_request);
	st_things_register_reset_cb(handle_reset_request, handle_reset_result);
	st_things_register_user_confirm_cb(handle_ownership_transfer_request);
	st_things_register_things_status_change_cb(handle_things_status_change);

	init_modes();

	if (st_things_start() == ST_THINGS_ERROR_NONE) {
		printf("[%s]=====================================================\n", TAG);
		printf("[%s]                   Stack Started                     \n", TAG);
		printf("[%s]=====================================================\n", TAG);
	}

	return 0;
}
