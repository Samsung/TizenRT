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

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <st_things/st_things.h>
#include <iotbus/iotbus_gpio.h>
#ifdef CONFIG_DEMO_LED
#include "sample_light_LED.h"
#endif

#define TAG		"LIGHT_THINGS"

static const char *g_res_switch = "/capability/switch/main/0";
static const char *g_res_dimming = "/capability/switchLevel/main/0";
static const char *g_res_temp = "/capability/colorTemperature/main/0";

extern void change_switch_value(void);
extern bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_get_request_on_dimming(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_dimming(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_get_request_on_ct(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep);
extern bool handle_set_request_on_ct(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep);

#ifdef CONFIG_RESET_BUTTON
static bool check_reset_button_pin_number(void)
{
	printf("Current pin number : %d\n", CONFIG_RESET_BUTTON_PIN_NUMBER);
	return CONFIG_RESET_BUTTON_PIN_NUMBER >= 0 ? true : false;
}

static void gpio_callback_event(void *user_data)
{
	printf("gpio_callback_event!!\n");
	printf("reset :: %d\n", st_things_reset());
}
#endif

#ifdef CONFIG_LIGHT_SWITCH_BUTTON
static void gpio_callback_event_trigger_light_switch(void *user_data)
{
	printf("gpio_callback_event to notify switch resource's observers manually!!\n");
	change_switch_value();
	printf("Notify to observer :: %d\n", st_things_notify_observers(g_res_switch));
	set_rep_push_mesg();
}
#endif

#ifndef CONFIG_RESET_BUTTON
static void button_scan_thread(void *user_data)
{
	int old_resetbutton = -1;
	int old_switchbutton = -1;
	int val = 0;
	int ret = 0;

	while (1) {
		ret = key_get_status(KEY_RESET_IO_NUM, &val);
		if (OK == ret && old_resetbutton != val) {
			old_resetbutton = val;

			if (old_resetbutton == KEY_RESET_PRESSED) {
				printf("gpio_callback_event RESET pressed!!\n");
				st_things_reset();
			}

		}

		ret = key_get_status(KEY_SWITCH_IO_NUM, &val);
		if (OK == ret  && old_switchbutton != val) {
			old_switchbutton = val;

			if (old_switchbutton == KEY_SWITCH_PRESSED) {
				printf("gpio_callback_event SWITCH to notify switch resource's observers manually!!\n");
				change_switch_value();
				st_things_notify_observers(g_res_switch);
				set_rep_push_mesg();
			}
		}

		usleep(BUTTON_SCAN_PEROID);
	}
}

void start_button_thread(void)
{
	pthread_attr_t attr;
	struct sched_param sparam;
	sparam.sched_priority = 100;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 4096); // 2048 is not enough
	pthread_attr_setschedparam(&attr, &sparam);

	pthread_t thread;
	int status = pthread_create(&thread, &attr, button_scan_thread, NULL);
	if (status != 0) {
		printf("[%s] pthread_create failed, status=%d\n", __FUNCTION__, status);
		return 0;
	}

	pthread_setname_np(thread, "button_scan_thread");
	pthread_detach(thread);
}
#endif


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

	if (0 == strncmp(req_msg->resource_uri, g_res_switch, strlen(g_res_switch))) {
		return handle_get_request_on_switch(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, g_res_dimming, strlen(g_res_dimming))) {
		return handle_get_request_on_dimming(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, g_res_temp, strlen(g_res_temp))) {
		return handle_get_request_on_ct(req_msg, resp_rep);
	} else {
		printf("Not supported uri.\n");
	}
	return false;
}

static bool handle_set_request(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	printf("Received a SET request on %s\n", req_msg->resource_uri);

	if (0 == strncmp(req_msg->resource_uri, g_res_switch, strlen(g_res_switch))) {
		return handle_set_request_on_switch(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, g_res_dimming, strlen(g_res_dimming))) {
		return handle_set_request_on_dimming(req_msg, resp_rep);
	} else if (0 == strncmp(req_msg->resource_uri, g_res_temp, strlen(g_res_temp))) {
		return handle_set_request_on_ct(req_msg, resp_rep);
	}
	return false;
}

int ess_process(void)
{
#ifdef CONFIG_RESET_BUTTON
	if (!check_reset_button_pin_number()) {
		printf("Error : Invalid pin number.\n");
		return 0;
	}
	iotapi_initialize();
	iotbus_gpio_context_h m_gpio = iotbus_gpio_open(CONFIG_RESET_BUTTON_PIN_NUMBER);
	iotbus_gpio_register_cb(m_gpio, IOTBUS_GPIO_EDGE_RISING, gpio_callback_event, (void *)m_gpio);
#endif

#ifdef CONFIG_LIGHT_SWITCH_BUTTON
	iotbus_gpio_context_h e_gpio = iotbus_gpio_open(CONFIG_LIGHT_SWITCH_BUTTON_PIN_NUMBER);
	iotbus_gpio_register_cb(e_gpio, IOTBUS_GPIO_EDGE_RISING, gpio_callback_event_trigger_light_switch, (void *)e_gpio);
#endif

	bool easysetup_complete = false;
	st_things_initialize("device_def.json", &easysetup_complete);
	st_things_register_request_cb(handle_get_request, handle_set_request);
	st_things_register_reset_cb(handle_reset_request, handle_reset_result);
	st_things_register_user_confirm_cb(handle_ownership_transfer_request);
	st_things_register_things_status_change_cb(handle_things_status_change);

	if (st_things_start() == ST_THINGS_ERROR_NONE) {
		printf("[%s]=====================================================\n", TAG);
		printf("[%s]                    Stack Started                    \n", TAG);
		printf("[%s]=====================================================\n", TAG);
	}

#ifdef CONFIG_DEMO_LED
	int ret = led_open();
	printf("led_open: %d\n", ret);
#endif

#ifndef CONFIG_RESET_BUTTON
	// If not config button with iotbus (it may does not work well),
	// we create thread to scan button instead.
	start_button_thread();
#endif

	return 0;
}
