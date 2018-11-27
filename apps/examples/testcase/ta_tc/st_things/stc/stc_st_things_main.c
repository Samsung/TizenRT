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
#include <iotbus/iotbus_gpio.h>
#include "tc_common.h"

#define TAG		"TESTS_THINGS"

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

static void handle_reset_result(bool result)
{
	printf("[%s]Reset %s.\n", TAG, result ? "succeeded" : "failed");
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

static void st_things_pin_generated_handle(const char *pin_data, const size_t pin_size)
{
	printf("recieved pin %s\n", pin_data);
	return ;
}


void st_things_pin_display_close_handle(void)
{
	printf(" st_things_pin_display_close_cb \n");
	return ;
}

/**
* @testcase         stc_st_things_initialize_p
* @brief            Initialize st_things stack 
* @scenario         Initialize st_things stack 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_p(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize("test_device_def.json", &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_invalid_json_path_n
* @brief            Initialize st_things stack with invalid json file path. 
* @scenario         Initialize st_things stack with invalid json file path. 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_invalid_json_path_n(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize("tttttttt", &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_n
* @brief            Initialize st_things stack when already its initialized. 
* @scenario         Initialize st_things stack when already its initialized. 
* @apicovered       st_things_initialize 
* @precondition     st_things already initialized. 
* @postcondition    none
*/
static void stc_st_things_initialize_n(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize("test_device_def.json", &easysetup_complete);
	res = st_things_initialize("test_device_def.json", &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_STACK_ALREADY_INITIALIZED);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_empty_json_path_n
* @brief            Initialize st_things stack with empty json file path. 
* @scenario         Initialize st_things stack with empty json file path. 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_empty_json_path_n(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize("", &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_null_json_path_n
* @brief            Initialize st_things stack with NULL json file path. 
* @scenario         Initialize st_things stack with NULL json file path. 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_null_json_path_n(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize(NULL, &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_easysetup_complete_true_n 
* @brief            Initialize st_things stack with &easysetup_complete = true. 
* @scenario         Initialize st_things stack with &easysetup_complete = true. 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_easysetup_complete_true_n(void)
{
	int res;
	bool easysetup_complete = true;
	res = st_things_initialize(NULL, &easysetup_complete);
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_initialize_r
* @brief            Initialize st_things stack multiple times. 
* @scenario         Initialize st_things stack multiple times. 
* @apicovered       st_things_initialize 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_initialize_r(void)
{
	int res;
	int i = 0;
	bool easysetup_complete = false;
	for (i = 0; i < 10; i++) {
		res = st_things_initialize("test_device_def.json", &easysetup_complete);
	}
	TC_ASSERT_EQ("st_things_initialize", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_request_cb_p
* @brief            st_things_register_request_cb 
* @scenario         setting get and set callbacks 
* @apicovered       st_things_register_request_cb 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_register_request_cb_p(void)
{
	int res;
	res = st_things_register_request_cb(handle_get_request, handle_set_request);
	TC_ASSERT_EQ("st_things_register_request_cb", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_request_cb_null_getcb_n 
* @brief            st_things_register_request_cb 
* @scenario         setting get and set callbacks 
* @apicovered       st_things_register_request_cb 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_register_request_cb_null_getcb_n(void)
{
	int res;
	res = st_things_register_request_cb(NULL, handle_set_request);
	TC_ASSERT_EQ("st_things_register_request_cb", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_request_cb_null_setcb_n 
* @brief            st_things_register_request_cb 
* @scenario         setting get and set callbacks 
* @apicovered       st_things_register_request_cb 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_register_request_cb_null_setcb_n(void)
{
	int res;
	res = st_things_register_request_cb(NULL, NULL);
	TC_ASSERT_EQ("st_things_register_request_cb", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_request_cb_null_params_n 
* @brief            st_things_register_request_cb 
* @scenario         setting get and set callbacks 
* @apicovered       st_things_register_request_cb 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_register_request_cb_null_params_n(void)
{
	int res;
	res = st_things_register_request_cb(NULL, NULL);
	TC_ASSERT_EQ("st_things_register_request_cb", res, ST_THINGS_ERROR_INVALID_PARAMETER);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_start_p 
* @brief            start st things stack 
* @scenario         start st things stack 
* @apicovered       st_things_start 
* @precondition     st things stack is not yet started 
* @postcondition    none
*/
static void stc_st_things_start_p(void)
{
	int res;
	res = st_things_start();
	TC_ASSERT_EQ("st_things_start", res, ST_THINGS_ERROR_STACK_NOT_INITIALIZED);
	TC_SUCCESS_RESULT();
}


/**
* @testcase         stc_st_things_start_with_init_p 
* @brief            start st thinhs stack 
* @scenario         start st thinhs stack 
* @apicovered       st_things_start 
* @precondition     st thinhs stack is not yet started 
* @postcondition    none
*/
static void stc_st_things_start_with_init_p(void)
{
	int res;
	bool easysetup_complete = false;
	res = st_things_initialize("test_device_def.json", &easysetup_complete);
	st_things_register_request_cb(handle_get_request, handle_set_request);
	st_things_register_reset_cb(handle_reset_request, handle_reset_result);
	st_things_register_user_confirm_cb(handle_ownership_transfer_request);
	st_things_register_things_status_change_cb(handle_things_status_change);
	printf("res = %d.\n", res);
	res = st_things_start();
	printf("res = %d.\n", res);
	TC_ASSERT_EQ("st_things_start", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_start_r
* @brief            start st_things stack multiple times. 
* @scenario         start st_things stack multiple times. 
* @apicovered       st_things_start 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_start_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_start();
	}
	TC_ASSERT_EQ("st_things_start", res, ST_THINGS_ERROR_OPERATION_FAILED);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_stop_r
* @brief            stop st_things stack multiple times. 
* @scenario         stop st_things stack multiple times. 
* @apicovered       st_things_stop 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_stop_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = things_stop_stack();
	}
	TC_ASSERT_EQ("st_things_stop", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_stop_p 
* @brief            stop st thinhs stack 
* @scenario         stop st thinhs stack 
* @apicovered       st_things_stop 
* @precondition     st things stack is already started 
* @postcondition    none
*/
static void stc_st_things_stop_p(void)
{
	int res;
	res = things_stop_stack();
	TC_ASSERT_EQ("st_things_stop", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}


/**
* @testcase         stc_st_things_reset_r
* @brief            reset st_things stack multiple times. 
* @scenario         reset st_things stack multiple times. 
* @apicovered       st_things_reset 
* @precondition     none
* @postcondition    none
*/
static void stc_st_things_reset_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_reset();
	}
	TC_ASSERT_EQ("st_things_reset", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_reset_p 
* @brief            reset st thinhs stack 
* @scenario         reset st thinhs stack 
* @apicovered       st_things_reset 
* @precondition     st things stack is already started 
* @postcondition    none
*/
static void stc_st_things_reset_p(void)
{
	int res;
	res = st_things_reset();
	TC_ASSERT_EQ("st_things_reset", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_things_status_change_cb_p 
* @brief            st_things_register_things_status_change_cb 
* @scenario         st_things_register_things_status_change_cb 
* @apicovered       st_things_register_things_status_change_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_register_things_status_change_cb_p(void)
{
	int res;
	res = st_things_register_things_status_change_cb(handle_things_status_change);
	TC_ASSERT_EQ("st_things_register_things_status_change_cb", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_things_status_change_cb_r 
* @brief            st_things_register_things_status_change_cb 
* @scenario         st_things_register_things_status_change_cb 
* @apicovered       st_things_register_things_status_change_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_register_things_status_change_cb_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_register_things_status_change_cb(handle_things_status_change);
	}
	TC_ASSERT_EQ("st_things_register_things_status_change_cb", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_pin_handling_cb_p 
* @brief            st_things_register_pin_handling_cb 
* @scenario         st_things_register_pin_handling_cb 
* @apicovered       st_things_register_pin_handling_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_register_pin_handling_cb_p(void)
{
	int res;
	res = st_things_register_pin_handling_cb(st_things_pin_generated_handle, st_things_pin_display_close_handle);
	TC_ASSERT_EQ("st_things_register_pin_handling_cb", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_things_status_change_cb_r 
* @brief            st_things_register_things_status_change_cb 
* @scenario         st_things_register_things_status_change_cb 
* @apicovered       st_things_register_things_status_change_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_register_pin_handling_cb_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_register_pin_handling_cb(st_things_pin_generated_handle, st_things_pin_display_close_handle);
	}
	TC_ASSERT_EQ("st_things_register_pin_handling_cb", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_pin_handling_cb_p 
* @brief            st_things_register_pin_handling_cb 
* @scenario         st_things_register_pin_handling_cb 
* @apicovered       st_things_register_pin_handling_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_deinitialize_p(void)
{
	int res;
	res = st_things_deinitialize();
	TC_ASSERT_EQ("st_things_deinitialize", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_things_status_change_cb_r 
* @brief            st_things_register_things_status_change_cb 
* @scenario         st_things_register_things_status_change_cb 
* @apicovered       st_things_register_things_status_change_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_deinitialize_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_deinitialize();
	}
	TC_ASSERT_EQ("res= st_things_deinitialize", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_pin_handling_cb_p 
* @brief            st_things_register_pin_handling_cb 
* @scenario         st_things_register_pin_handling_cb 
* @apicovered       st_things_register_pin_handling_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_notify_observers_p(void)
{
	int res;
	res = st_things_notify_observers(g_res_switch);
	TC_ASSERT_EQ("st_things_notify_observers", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         stc_st_things_register_things_status_change_cb_r 
* @brief            st_things_register_things_status_change_cb 
* @scenario         st_things_register_things_status_change_cb 
* @apicovered       st_things_register_things_status_change_cb 
* @precondition     none 
* @postcondition    none
*/
static void stc_st_things_notify_observers_r(void)
{
	int res;
	int i = 0;
	for (i = 0; i < 10; i++) {
		res = st_things_notify_observers(g_res_switch);
	}
	TC_ASSERT_EQ("res= st_things_notify_observers", res, ST_THINGS_ERROR_NONE);
	TC_SUCCESS_RESULT();
}




#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int stc_st_things_main(int argc, char *argv[])
#endif
{
	if (tc_handler(TC_START, "ST_THINGS STC") == ERROR) {
		return ERROR;
	}

	/* Positive TCs */
	stc_st_things_initialize_p();
	stc_st_things_initialize_invalid_json_path_n();
	stc_st_things_initialize_n();
	stc_st_things_initialize_empty_json_path_n();
	stc_st_things_initialize_null_json_path_n();
	stc_st_things_initialize_easysetup_complete_true_n();
	stc_st_things_initialize_r();
	stc_st_things_register_request_cb_p();
	stc_st_things_register_request_cb_null_getcb_n();
	stc_st_things_register_request_cb_null_setcb_n();
	stc_st_things_register_request_cb_null_params_n();
	stc_st_things_start_p();
	stc_st_things_start_with_init_p();
	stc_st_things_start_r();
	stc_st_things_stop_p();
	stc_st_things_stop_r();
	stc_st_things_reset_p();
	stc_st_things_reset_r();
	stc_st_things_register_things_status_change_cb_p();
	stc_st_things_register_things_status_change_cb_r();
	stc_st_things_register_pin_handling_cb_p();
	stc_st_things_register_pin_handling_cb_r();
	stc_st_things_deinitialize_p();
	stc_st_things_deinitialize_r();
	stc_st_things_notify_observers_p();
	stc_st_things_notify_observers_r();
	(void)tc_handler(TC_END, "ST_THINGS STC");
	return 0;
}

