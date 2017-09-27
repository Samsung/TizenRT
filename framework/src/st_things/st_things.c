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
#include <stdlib.h>
#include <string.h>

#include <st_things/st_things.h>
#include "st_things_request_handler.h"
#include "st_things_representation.h"
#include "sdkapi_logger.h"

#ifdef __TIZEN__				/* Linux */
#include "things_api.h"
#include "things_resource.h"
#include "ocpayload.h"
#include "octypes.h"
#include "things_def.h"
#else
#include "things_api.h"
#include "things_resource.h"
#include "ocpayload.h"
#include "octypes.h"
#include "things_def.h"
#endif

static st_things_reset_confirm_cb gHandleResetConfirmCB = NULL;
static st_things_reset_result_cb gHandleResetResultCB = NULL;

static st_things_pin_generated_cb gHandlePinGeneratedCB = NULL;
static st_things_pin_display_close_cb gHandlePinDisplayCloseCB = NULL;

static st_things_user_confirm_cb gHandleUserConfirmCB = NULL;

static st_things_status_change_cb gHandleThingsStatusChangeCB = NULL;

/**
* This callback will be invoked by st_things Stack with the result of reset.
* Result will be passed to the application through its registered callback.
*/
void get_reset_result_cb(int result)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	SDKAPI_LOG_V(SDKAPI_DEBUG, "Result of reset is %d", result);
	bool res = (result == 1) ? true : false;
	if (NULL != gHandleResetResultCB) {
		gHandleResetResultCB(res);
	}
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* This callback will be registered with the st_things Stack to get the request for user's confirmation for reset.
* Application's callback will be invoked to get the user's opinion.
*/
int get_reset_confirm_cb(things_reset_result_func_type *func_carrier, es_enrollee_reset_e resetType)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == gHandleResetConfirmCB) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return 0;
	}

	if (RST_NEED_CONFIRM == resetType) {
		// Invoke application's callback to get user's confirmation and pass the result to st_things Stack.
		bool confirm = gHandleResetConfirmCB();
		SDKAPI_LOG_V(SDKAPI_DEBUG, "User's confirmation for reset : %s", confirm ? "true" : "false");
		if (confirm) {
			things_return_user_opinion_for_reset(1);
		} else {
			things_return_user_opinion_for_reset(0);
		}
	} else if (RST_AUTO_RESET == resetType) {
		SDKAPI_LOG(SDKAPI_DEBUG, "Reset will start automatically");
		things_return_user_opinion_for_reset(1);
	} else {
		SDKAPI_LOG_V(SDKAPI_ERROR, "resetType(%d) is invalid", resetType);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return 0;
	}

	// Set the callback through this function's value-result paramter with st_things Stack to get the result of reset.
	*func_carrier = get_reset_result_cb;

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return 1;
}

/**
* This callback will be registered with the st_things Stack to recieve easy-setup events.
* The status will be passed to the application through the callback registered by the application.
*/
void get_things_status_cb(es_enrollee_state_e state)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	SDKAPI_LOG_V(SDKAPI_DEBUG, "Received event for easy-setup state change (%d)", state);
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
		SDKAPI_LOG(SDKAPI_DEBUG, "This state is ignored");
		handled = false;
		break;
	}

	if (handled && NULL != gHandleThingsStatusChangeCB) {
		gHandleThingsStatusChangeCB(status);
	}
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* st_things Stack invokes this callback for providing ownership transfer states.
*/
void ownership_transfer_state_cb(const char *addr, uint16_t port, const char *uuid, int event)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	SDKAPI_LOG_V(SDKAPI_DEBUG, "Received event for ownership-transfer state change (%d)", event);
	(void)addr;
	(void)port;
	(void)uuid;

	if (NULL == gHandleThingsStatusChangeCB) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return;
	}

	switch (event) {
	case 1:					// OTM Started
		gHandleThingsStatusChangeCB(ST_THINGS_STATUS_ES_STARTED);
		break;
	case 2:					// OTM Done
		gHandleThingsStatusChangeCB(ST_THINGS_STATUS_ES_DONE);
		break;
	case 3:					// OTM Error
		gHandleThingsStatusChangeCB(ST_THINGS_STATUS_ES_FAILED_ON_OWNERSHIP_TRANSFER);
		break;
	}
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* Callback for getting users confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership transfer
*/
int get_user_confirm_cb(void)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == gHandleUserConfirmCB) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return 0;
	}

	bool confirm = gHandleUserConfirmCB();
	SDKAPI_LOG_V(SDKAPI_DEBUG, "User's confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership-transfer : %s", confirm ? "true" : "false");

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return confirm ? 1 : 0;
}

/**
* Callback for getting the pin for random pin based ownership transfer.
*/
void generated_pin_cb(char *pinData, size_t pinSize)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL != gHandlePinGeneratedCB) {
		gHandlePinGeneratedCB(pinData, pinSize);
	}
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* Callback for getting the close pin display request after random pin based ownership transfer.
*/
void close_pin_display_cb(void)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL != gHandlePinDisplayCloseCB) {
		gHandlePinDisplayCloseCB();
	}
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

int st_things_initialize(const char *json_path, bool *easysetup_complete)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == json_path) {
		SDKAPI_LOG(SDKAPI_ERROR, "The path of a json file is NULL");
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	int result = 0;
	if (1 != (result = things_initialize_stack(json_path, easysetup_complete))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_initialize_stack failed (result:%d)", result);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_V(SDKAPI_DEBUG, "Is EasySetup completed : %s", (*easysetup_complete) ? "true" : "false");
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_deinitialize(void)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);

	int result = 0;
	if (1 != (result = things_deinitialize_stack())) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_deinitialize_stack failed (result:%d)", result);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_start(void)
{
	SDKAPI_LOG_INIT();
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	int result = 0;

	// Register callback for reset confirmation.
	if (1 != (result = things_register_confirm_reset_start_func(get_reset_confirm_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_confirm_reset_start_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for easy-setup status.
	if (1 != (result = things_register_easysetup_state_func(get_things_status_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_easysetup_state_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving the Security Ownership Transfer state changes.
	if (1 != (result = things_register_otm_event_handler(ownership_transfer_state_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_otm_event_handler failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving request during ownership transfer for getting confirmation from user.
	if (1 != (result = things_register_user_confirm_func(get_user_confirm_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_user_confirm_func failed (result:%d)", result);
		goto error;
	}
	// Register callback for receiving the pin generated for ownership transfer.
	if (1 != (result = things_register_pin_generated_func(generated_pin_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_pin_generated_func failed (result:%d)", result);
		goto error;
	}
	// Register callback to receiving request for closing the PIN display.
	if (1 != (result = things_register_pin_display_close_func(close_pin_display_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_pin_display_close_func failed (result:%d)", result);
		goto error;
	}
	// Register callback to handle GET and POST requests.
	if (1 != (result = things_register_handle_request_func(handle_get_request_cb, handle_set_request_cb))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_register_handle_request_func failed (result:%d)", result);
		goto error;
	}
	// Start st_things Stack.
	if (1 != (result = things_start_stack())) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_start_stack failed (result:%d)", result);
		goto error;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;

error:
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_OPERATION_FAILED;
}

int st_things_stop(void)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	int result = 0;

	// Terminate st_things Stack.
	if (1 != (result = things_stop())) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_stop failed (result:%d)", result);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	SDKAPI_LOG_DEINIT();
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_request_cb(st_things_get_request_cb get_cb, st_things_set_request_cb set_cb)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == get_cb || NULL == set_cb) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return register_request_handler_cb(get_cb, set_cb);
}

int st_things_register_reset_cb(st_things_reset_confirm_cb confirm_cb, st_things_reset_result_cb result_cb)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == confirm_cb || NULL == result_cb) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	gHandleResetConfirmCB = confirm_cb;
	gHandleResetResultCB = result_cb;

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_reset(void)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	int result = 0;
	if (1 != (result = things_reset(NULL, RST_AUTO_RESET))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_reset failed (result:%d)", result);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_pin_handling_cb(st_things_pin_generated_cb generated_cb, st_things_pin_display_close_cb close_cb)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == generated_cb || NULL == close_cb) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	gHandlePinGeneratedCB = generated_cb;
	gHandlePinDisplayCloseCB = close_cb;

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_user_confirm_cb(st_things_user_confirm_cb confirm_cb)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == confirm_cb) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	gHandleUserConfirmCB = confirm_cb;

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_register_things_status_change_cb(st_things_status_change_cb status_cb)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == status_cb) {
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	gHandleThingsStatusChangeCB = status_cb;

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_notify_observers(const char *resource_uri)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == resource_uri || 1 > strlen(resource_uri)) {
		SDKAPI_LOG(SDKAPI_ERROR, "The resource URI is invalid");
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	int result = 0;
	if (1 != (result = things_notify_observers(resource_uri))) {
		SDKAPI_LOG_V(SDKAPI_ERROR, "things_notify_observers failed (result:%d)", result);
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

int st_things_send_push_message(const char *resource_uri, st_things_representation_s *rep)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	if (NULL == resource_uri || 1 > strlen(resource_uri)) {
		SDKAPI_LOG(SDKAPI_ERROR, "The resource URI is invalid");
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}
	if (NULL == rep) {
		SDKAPI_LOG(SDKAPI_ERROR, "rep is NULL");
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_INVALID_PARAMETER;
	}

	if (things_get_easysetup_state()) {
		const char *device_type = things_get_device_type();
		const char *device_name = things_get_device_name();
		//Choosing the first resource type
		int count = 0;
		char **resource_types = NULL;
		if (0 == things_get_resource_type(resource_uri, &count, &resource_types)) {
			SDKAPI_LOG(SDKAPI_ERROR, "things_get_resource_type failed");
			SDKAPI_LOG_EXIT(SDKAPI_INFO);
			return ST_THINGS_ERROR_OPERATION_FAILED;
		}
		if (count < 1) {
			SDKAPI_LOG_V(SDKAPI_ERROR, "things_get_resource_type invalid count = %d", count);
			SDKAPI_LOG_EXIT(SDKAPI_INFO);
			return ST_THINGS_ERROR_OPERATION_FAILED;
		}
		const char *resourceType = resource_types[0];	//Choosing the first one

		OCRepPayload *data = OCRepPayloadCreate();

		OCRepPayloadSetPropString(data, KEY_PUSH_DATA_ET, "device.changed");
		OCRepPayloadSetPropString(data, KEY_PUSH_DATA_DT, device_type);
		OCRepPayloadSetPropString(data, KEY_PUSH_DATA_DNAME, device_name);
		OCRepPayloadSetPropString(data, KEY_PUSH_DATA_RT, resourceType);
		OCRepPayloadSetPropObject(data, KEY_PUSH_DATA_REP, rep->payload);

		const char *peerID = things_get_server_uuid();	//Corresponding IoTivity API - OCGetServerInstanceIDString();
		OCRepPayload *push_message = OCRepPayloadCreate();

		OCRepPayloadSetPropInt(push_message, KEY_PUSH_NS_MESSAGE_ID, 0);	//Mandatory Field; For Push Message messageid = 0
		OCRepPayloadSetPropString(push_message, KEY_PUSH_NS_PROVIDER_ID, peerID);	//Mandatory Field
		OCRepPayloadSetPropObjectAsOwner(push_message, KEY_PUSH_DATA, data);	//data will be transferred to the push_message payload
		int result = 0;
		if (1 != (result = things_send_push_message(URI_PUSHSERVICE, push_message))) {
			SDKAPI_LOG_V(SDKAPI_ERROR, "things_send_push_message failed (result:%d)", result);
			SDKAPI_LOG_EXIT(SDKAPI_INFO);
			return ST_THINGS_ERROR_OPERATION_FAILED;
		}
	} else {
		SDKAPI_LOG(SDKAPI_ERROR, "Fail to send Push Notification message to Cloud (device not connected)");
		SDKAPI_LOG_EXIT(SDKAPI_INFO);
		return ST_THINGS_ERROR_OPERATION_FAILED;
	}

	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return ST_THINGS_ERROR_NONE;
}

st_things_representation_s *st_things_create_representation_inst()
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	st_things_representation_s *rep = create_representation_inst();
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
	return rep;
}

void st_things_destroy_representation_inst(st_things_representation_s *rep)
{
	SDKAPI_LOG_ENTRY(SDKAPI_INFO);
	destroy_representation_inst(rep);
	SDKAPI_LOG_EXIT(SDKAPI_INFO);
}
