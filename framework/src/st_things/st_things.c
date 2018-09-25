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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __ST_THINGS_RTOS__
#include <st_things/st_things.h>
#else
#include "st_things.h"
#endif							// __ST_THINGS_RTOS__
#include "st_things_request_handler.h"
#include "st_things_representation.h"
#include "logging/things_logger.h"

#include "things_api.h"
#include "things_types.h"
#include "things_resource.h"
#include "ocpayload.h"
#include "octypes.h"

#define TAG "[st_things_sdk]"

#define SEC_ATRRIBUTE_PUSH_MESG_MSGID        "x.org.iotivity.ns.messageid"
#define SEC_ATRRIBUTE_PUSH_MESG_PROVIDERID   "x.org.iotivity.ns.providerid"
#define SEC_ATRRIBUTE_PUSH_MESG_ET           "et"
#define SEC_ATRRIBUTE_PUSH_MESG_DATA         "x.com.samsung.data"

static st_things_reset_confirm_cb g_handle_reset_confirm_cb = NULL;
static st_things_reset_result_cb g_handle_reset_result_cb = NULL;

static st_things_pin_generated_cb g_handle_pin_generated_cb = NULL;
static st_things_pin_display_close_cb g_handle_pin_display_close_cb = NULL;

static st_things_user_confirm_cb g_handle_user_confirm_cb = NULL;

static st_things_status_change_cb g_handle_things_status_change_cb = NULL;

typedef enum {
	STACK_NOT_INITIALIZED = 0,
	STACK_INITIALIZED,
	STACK_STARTED
} stack_status_e;

static stack_status_e g_stack_status = STACK_NOT_INITIALIZED;

/**
 * This callback will be invoked by DA Stack with the result of reset.
 * Result will be passed to the application through its registered callback.
 */
void get_reset_result_cb(int result)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	THINGS_LOG_D(TAG, "Result of reset is %d", result);
	bool res = (result == 1) ? true : false;
	if (NULL != g_handle_reset_result_cb) {
		g_handle_reset_result_cb(res);
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

/**
 * This callback will be registered with the DA Stack to get the request for user's confirmation for reset.
 * Application's callback will be invoked to get the user's opinion.
 */
int get_reset_confirm_cb(things_reset_result_func_type *func_carrier, things_es_enrollee_reset_e reset_type)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == g_handle_reset_confirm_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return 0;
	}

	if (RST_NEED_CONFIRM == reset_type) {
		// Invoke application's callback to get user's confirmation and pass the result to DA Stack.
		bool confirm = g_handle_reset_confirm_cb();
		THINGS_LOG_D(TAG, "User's confirmation for reset : %s", confirm ? "true" : "false");
		if (confirm) {
			things_return_user_opinion_for_reset(1);
		} else {
			things_return_user_opinion_for_reset(0);
		}
	} else if (RST_AUTO_RESET == reset_type) {
		THINGS_LOG_D(TAG, "Reset will start automatically");
		things_return_user_opinion_for_reset(1);
	} else {
		THINGS_LOG_E(TAG, "reset_type(%d) is invalid", reset_type);
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return 0;
	}

	// Set the callback through this function's value-result paramter with DA Stack to get the result of reset.
	*func_carrier = get_reset_result_cb;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return 1;
}

/**
 * This callback will be registered with the DA Stack to recieve easy-setup events.
 * The status will be passed to the application through the callback registered by the application.
 */
void get_things_status_cb(things_es_enrollee_state_e state)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	THINGS_LOG_D(TAG, "Received event for easy-setup state change (%d)", state);
	bool handled = true;
	st_things_status_e status = ST_THINGS_STATUS_INIT;
	switch (state) {
	case ES_STATE_INIT:
		status = ST_THINGS_STATUS_INIT;
		break;
	case ES_STATE_CONNECTING_TO_ENROLLER:
		status = ST_THINGS_STATUS_CONNECTING_TO_AP;
		break;
	case ES_STATE_CONNECTED_TO_ENROLLER:
		status = ST_THINGS_STATUS_CONNECTED_TO_AP;
		break;
	case ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER:
		status = ST_THINGS_STATUS_CONNECTING_TO_AP_FAILED;
		break;
	case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
		status = ST_THINGS_STATUS_REGISTERING_TO_CLOUD;
		break;
	case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD:
		status = ST_THINGS_STATUS_REGISTERING_FAILED_ON_SIGN_IN;
		break;
	case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
		status = ST_THINGS_STATUS_REGISTERING_FAILED_ON_PUB_RES;
		break;
	case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
		status = ST_THINGS_STATUS_REGISTERED_TO_CLOUD;
		break;
	default:
		THINGS_LOG_D(TAG, "This state is ignored");
		handled = false;
		break;
	}

	if (handled && NULL != g_handle_things_status_change_cb) {
		g_handle_things_status_change_cb(status);
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

/**
 * DA Stack invokes this callback for providing ownership transfer states.
 */
void ownership_transfer_state_cb(const char *addr, uint16_t port, const char *uuid, int event)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	THINGS_LOG_D(TAG, "Received event for ownership-transfer state change (%d)", event);
	(void)addr;
	(void)port;
	(void)uuid;

	if (NULL == g_handle_things_status_change_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return;
	}

	switch (event) {
	case 1:					// OTM Started
		g_handle_things_status_change_cb(ST_THINGS_STATUS_ES_STARTED);
		break;
	case 2:					// OTM Done
		g_handle_things_status_change_cb(ST_THINGS_STATUS_ES_DONE);
		break;
	case 3:					// OTM Error
		g_handle_things_status_change_cb(ST_THINGS_STATUS_ES_FAILED_ON_OWNERSHIP_TRANSFER);
		break;
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

/**
 * Callback for getting users confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership transfer
 */
int get_user_confirm_cb(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == g_handle_user_confirm_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return 0;
	}

	bool confirm = g_handle_user_confirm_cb();
	THINGS_LOG_D(TAG, "User's confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership-transfer : %s", confirm ? "true" : "false");

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return confirm ? 1 : 0;
}

/**
 * Callback for getting the pin for random pin based ownership transfer.
 */
void generated_pin_cb(char *pin_data, size_t pin_size)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL != g_handle_pin_generated_cb) {
		g_handle_pin_generated_cb(pin_data, pin_size);
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

/**
 * Callback for getting the close pin display request after random pin based ownership transfer.
 */
void close_pin_display_cb(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL != g_handle_pin_display_close_cb) {
		g_handle_pin_display_close_cb();
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

int st_things_initialize(const char *json_path, bool *easysetup_complete)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (STACK_NOT_INITIALIZED != g_stack_status) {
		int ret_val = ST_THINGS_ERROR_OPERATION_FAILED;
		switch (g_stack_status) {
		case STACK_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack initialized already.");
			ret_val = ST_THINGS_ERROR_STACK_ALREADY_INITIALIZED;
			break;
		case STACK_STARTED:
			THINGS_LOG_E(TAG, "Stack is currently running.");
			ret_val = ST_THINGS_ERROR_STACK_RUNNING;
			break;
		default:
			THINGS_LOG_E(TAG, "Invalid stack state: %d.", g_stack_status);
			break;
		}

		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ret_val;
	}

	if (NULL == json_path) {
		THINGS_LOG_E(TAG, "Json file path is NULL");
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	THINGS_LOG_D(TAG, "JSON file path: %s", json_path);

	int result = 0;
	if (1 != (result = things_initialize_stack(json_path, easysetup_complete))) {
		THINGS_LOG_E(TAG, "things_initialize_stack failed (result:%d)", result);
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	g_stack_status = STACK_INITIALIZED;

	THINGS_LOG_D(TAG, "Is EasySetup completed : %s", (*easysetup_complete) ? "true" : "false");
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_deinitialize(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (STACK_INITIALIZED != g_stack_status) {
		int ret_val = ST_THINGS_ERROR_OPERATION_FAILED;
		switch (g_stack_status) {
		case STACK_NOT_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is not initialized.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_INITIALIZED;
			break;
		case STACK_STARTED:
			THINGS_LOG_E(TAG, "Stack is currently running. Stop the stack before deinitializing it.");
			ret_val = ST_THINGS_ERROR_STACK_RUNNING;
			break;
		default:
			THINGS_LOG_E(TAG, "Invalid stack state: %d.", g_stack_status);
			break;
		}

		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ret_val;
	}

	int result = 0;
	if (1 != (result = things_deinitialize_stack())) {
		THINGS_LOG_E(TAG, "things_deinitialize_stack failed (result:%d)", result);
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	g_stack_status = STACK_NOT_INITIALIZED;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_start(void)
{
	THINGS_LOG_INIT();
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (STACK_INITIALIZED != g_stack_status) {
		int ret_val = ST_THINGS_ERROR_OPERATION_FAILED;
		switch (g_stack_status) {
		case STACK_NOT_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is not initialized.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_INITIALIZED;
			break;
		case STACK_STARTED:
			THINGS_LOG_D(TAG, "Stack started already.");
			ret_val = ST_THINGS_ERROR_NONE;
			break;
		default:
			THINGS_LOG_E(TAG, "Invalid stack state: %d.", g_stack_status);
			break;
		}

		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ret_val;
	}

	int result = 0;

	// Register callback for reset confirmation.
	if (1 != (result = things_register_confirm_reset_start_func(get_reset_confirm_cb))) {
		THINGS_LOG_E(TAG, "things_register_confirm_reset_start_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for easy-setup status.
	if (1 != (result = things_register_easysetup_state_func(get_things_status_cb))) {
		THINGS_LOG_E(TAG, "things_register_easysetup_state_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving the Security Ownership Transfer state changes.
	if (1 != (result = things_register_otm_event_handler(ownership_transfer_state_cb))) {
		THINGS_LOG_E(TAG, "things_register_otm_event_handler failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving request during ownership transfer for getting confirmation from user.
	if (1 != (result = things_register_user_confirm_func(get_user_confirm_cb))) {
		THINGS_LOG_E(TAG, "things_register_user_confirm_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving the pin generated for ownership transfer.
	if (1 != (result = things_register_pin_generated_func(generated_pin_cb))) {
		THINGS_LOG_E(TAG, "things_register_pin_generated_func failed (result:%d)", result);
		goto error;
	}
	// Register callback to receiving request for closing the PIN display.
	if (1 != (result = things_register_pin_display_close_func(close_pin_display_cb))) {
		THINGS_LOG_E(TAG, "things_register_pin_display_close_func failed (result:%d)", result);
		goto error;
	}
	// Register callback to handle GET and POST requests.
	if (1 != (result = things_register_handle_request_func(handle_get_request_cb, handle_set_request_cb))) {
		THINGS_LOG_E(TAG, "things_register_handle_request_func failed (result:%d)", result);
		goto error;
	}
	// Start DA Stack.
	if (1 != (result = things_start_stack())) {
		THINGS_LOG_E(TAG, "things_start_stack failed (result:%d)", result);
		goto error;
	}

	g_stack_status = STACK_STARTED;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;

error:
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_OPERATION_FAILED;
}

int st_things_push_notification_to_cloud(const char *target_uri, st_things_representation_s *resp_rep)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	int push_ret = 0;
	OCRepPayload *payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(TAG, "Failed to allocate Payload");
		return 0;
	} else {
		OCRepPayloadSetUri(resp_rep->payload, target_uri);
		OCRepPayloadSetPropInt(resp_rep->payload, SEC_ATRRIBUTE_PUSH_MESG_MSGID, 0);
		OCRepPayloadSetPropString(resp_rep->payload, SEC_ATRRIBUTE_PUSH_MESG_PROVIDERID, OCGetServerInstanceIDString());
		OCRepPayloadSetPropString(payload, SEC_ATRRIBUTE_PUSH_MESG_ET, "device.changed");
		OCRepPayloadSetPropObject(resp_rep->payload, SEC_ATRRIBUTE_PUSH_MESG_DATA, payload);
		push_ret = push_notification_to_cloud(target_uri, resp_rep->payload);
		THINGS_LOG_V(TAG, "push_ret [%d]", push_ret);
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return push_ret;
}

int st_things_register_request_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == get_cb || NULL == set_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return register_request_handler_cb(get_cb, set_cb);
}

int st_things_register_reset_cb(st_things_reset_confirm_cb confirm_cb, st_things_reset_result_cb result_cb)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == confirm_cb || NULL == result_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	g_handle_reset_confirm_cb = confirm_cb;
	g_handle_reset_result_cb = result_cb;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_reset(void)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (STACK_STARTED != g_stack_status) {
		int ret_val = ST_THINGS_ERROR_OPERATION_FAILED;
		switch (g_stack_status) {
		case STACK_NOT_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is not initialized. Before reset, stack should be initialized and started.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_INITIALIZED;
			break;
		case STACK_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is initialized but not started.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_STARTED;
			break;
		default:
			THINGS_LOG_E(TAG, "Invalid stack state: %d.", g_stack_status);
			break;
		}

		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ret_val;
	}

	int result = 0;
	if (1 != (result = things_reset(NULL, RST_AUTO_RESET))) {
		THINGS_LOG_E(TAG, "things_reset failed (result:%d)", result);
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_pin_handling_cb(st_things_pin_generated_cb generated_cb, st_things_pin_display_close_cb close_cb)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == generated_cb || NULL == close_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	g_handle_pin_generated_cb = generated_cb;
	g_handle_pin_display_close_cb = close_cb;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_user_confirm_cb(st_things_user_confirm_cb confirm_cb)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == confirm_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	g_handle_user_confirm_cb = confirm_cb;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_things_status_change_cb(st_things_status_change_cb status_cb)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	if (NULL == status_cb) {
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	g_handle_things_status_change_cb = status_cb;

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

int st_things_notify_observers(const char *resource_uri)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (STACK_STARTED != g_stack_status) {
		int ret_val = ST_THINGS_ERROR_OPERATION_FAILED;
		switch (g_stack_status) {
		case STACK_NOT_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is not initialized. Before notifying observers, stack should be initialized and started.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_INITIALIZED;
			break;
		case STACK_INITIALIZED:
			THINGS_LOG_E(TAG, "Stack is initialized but not started.");
			ret_val = ST_THINGS_ERROR_STACK_NOT_STARTED;
			break;
		default:
			THINGS_LOG_E(TAG, "Invalid stack state: %d.", g_stack_status);
			break;
		}

		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ret_val;
	}

	if (NULL == resource_uri || 1 > strlen(resource_uri)) {
		THINGS_LOG_E(TAG, "The resource URI is invalid");
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	int result = 0;
	if (1 != (result = things_notify_observers(resource_uri))) {
		THINGS_LOG_E(TAG, "things_notify_observers failed (result:%d)", result);
		THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return ST_THINGS_ERROR_NONE;
}

st_things_representation_s *st_things_create_representation_inst()
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	st_things_representation_s *rep = create_representation_inst();
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return rep;
}

void st_things_destroy_representation_inst(st_things_representation_s *rep)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	destroy_representation_inst(rep);
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}
