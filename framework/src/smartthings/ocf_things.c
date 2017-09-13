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

#include "ocf_things.h"
#include "ocf_things_request_handler.h"
#include "sdkapi_logger.h"

#include "thingsapi.h"
#include "oicresource.h"
#include "octypes.h"
#include "ocpayload.h"

// TODO: [jae] I leave this code for the case of TizenRT.
//#define MAX_FILE_PATH_LENGTH        (250)
//
//static char gDeviceDefJsonFilePath[MAX_FILE_PATH_LENGTH+1] = {0};

static ocf_things_reset_confirm_cb gHandleResetConfirmCB = NULL;
static ocf_things_reset_result_cb  gHandleResetResultCB = NULL;

static ocf_things_pin_generated_cb gHandlePinGeneratedCB = NULL;
static ocf_things_pin_display_close_cb gHandlePinDisplayCloseCB = NULL;

static ocf_things_user_confirm_cb gHandleUserConfirmCB = NULL;

static ocf_things_status_change_cb gHandleThingsStatusChangeCB = NULL;

/**
* This callback will be invoked by DA Stack with the result of reset.
* Result will be passed to the application through its registered callback.
*/
void get_reset_result_cb(int result)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    SDKAPI_LOG_V(SDKAPI_DEBUG, "Result of reset is %d", result);
    bool res = (result==1)?true:false;
    if(NULL != gHandleResetResultCB)
    {
        gHandleResetResultCB(res);
    }
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* This callback will be registered with the DA Stack to get the request for user's confirmation for reset.
* Application's callback will be invoked to get the user's opinion.
*/
int get_reset_confirm_cb(OICResetResultFuncType* funcCarrier, ESEnrolleeReset resetType)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if(NULL == gHandleResetConfirmCB)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return 0;
    }

    if(RST_NEED_CONFIRM == resetType)
    {
        // Invoke application's callback to get user's confirmation and pass the result to DA Stack.
        bool confirm = gHandleResetConfirmCB();
        SDKAPI_LOG_V(SDKAPI_DEBUG, "User's confirmation for reset : %s", confirm ? "true" : "false");
        if(confirm)
        {
            OICReturnUserOpinion4Reset(1);
        }
        else
        {
            OICReturnUserOpinion4Reset(0);
        }
    }
    else if(RST_AUTO_RESET == resetType)
    {
        SDKAPI_LOG(SDKAPI_DEBUG, "Reset will start automatically");
        OICReturnUserOpinion4Reset(1);
    }
    else
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "resetType(%d) is invalid", resetType);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return 0;
    }

    // Set the callback through this function's value-result paramter with DA Stack to get the result of reset.
    *funcCarrier = get_reset_result_cb;

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return 1;
}

/**
* This callback will be registered with the DA Stack to recieve easy-setup events.
* The status will be passed to the application through the callback registered by the application.
*/
void get_things_status_cb(ESEnrolleeState state)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    SDKAPI_LOG_V(SDKAPI_DEBUG, "Received event for easy-setup state change (%d)", state);
    bool handled = true;
    ocf_things_status_e status = OCF_THINGS_STATUS_INIT;
    switch(state)
    {
        case ES_STATE_INIT:
            status = OCF_THINGS_STATUS_INIT;
            break;
        case ES_STATE_CONNECTING_TO_ENROLLER:
            status = OCF_THINGS_STATUS_CONNECTING_TO_AP;
            break;
        case ES_STATE_CONNECTED_TO_ENROLLER:
            status = OCF_THINGS_STATUS_CONNECTED_TO_AP;
            break;
        case ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER:
            status = OCF_THINGS_STATUS_CONNECTING_TO_AP_FAILED;
            break;
        case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
            status = OCF_THINGS_STATUS_REGISTERING_TO_CLOUD;
            break;
        case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD:
            status = OCF_THINGS_STATUS_REGISTERING_FAILED_ON_SIGN_IN;
            break;
        case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
            status = OCF_THINGS_STATUS_REGISTERING_FAILED_ON_PUB_RES;
            break;
        case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
            status = OCF_THINGS_STATUS_REGISTERED_TO_CLOUD;
            break;
        default:
           SDKAPI_LOG(SDKAPI_DEBUG, "This state is ignored");
           handled = false;
           break;
    }

    if(handled && NULL != gHandleThingsStatusChangeCB)
    {
        gHandleThingsStatusChangeCB(status);
    }
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

/**
* DA Stack invokes this callback for providing ownership transfer states.
*/
void ownership_transfer_state_cb(const char* addr, uint16_t port, const char* uuid, int event)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    SDKAPI_LOG_V(SDKAPI_DEBUG, "Received event for ownership-transfer state change (%d)", event);
    (void)addr;
    (void)port;
    (void)uuid;

    if(NULL == gHandleThingsStatusChangeCB)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return;
    }

    switch(event)
    {
        case 1: // OTM Started
            gHandleThingsStatusChangeCB(OCF_THINGS_STATUS_ES_STARTED);
            break;
        case 2: // OTM Done
            gHandleThingsStatusChangeCB(OCF_THINGS_STATUS_ES_DONE);
            break;
        case 3: // OTM Error
            gHandleThingsStatusChangeCB(OCF_THINGS_STATUS_ES_FAILED_ON_OWNERSHIP_TRANSFER);
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
    if(NULL == gHandleUserConfirmCB)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return 0;
    }

    bool confirm = gHandleUserConfirmCB();
    SDKAPI_LOG_V(SDKAPI_DEBUG, "User's confirmation for MUTUAL VERIFICATION BASED JUST WORK Ownership-transfer : %s", confirm ? "true" : "false");
    if(!confirm)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return 0;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return 1;
}

/**
* Callback for getting the pin for random pin based ownership transfer.
*/
void generated_pin_cb(char* pinData, size_t pinSize)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if(NULL != gHandlePinGeneratedCB)
    {
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
    if(NULL != gHandlePinDisplayCloseCB)
    {
        gHandlePinDisplayCloseCB();
    }
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
}

int ocf_things_initialize(const char* json_path, bool* easysetup_complete)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == json_path )
    {
        SDKAPI_LOG(SDKAPI_ERROR, "The path of a json file is NULL");
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

// TODO: [jae] I leave this code for the case of TizenRT.
//
//    // Given path file path should not be empty and should not exceed the length limit set by DA Stack.
//    int fileLen = strlen(json_path);
//    if( 1 > fileLen || MAX_FILE_PATH_LENGTH < fileLen )
//    {
//        SDKAPI_LOG_EXIT(SDKAPI_INFO);
//        return OCF_THINGS_ERROR_INVALID_PARAMETER;
//    }
//
//    // strncat is optimal than strncpy because it avoids unnecessary buffer padding.
//    gDeviceDefJsonFilePath[0] = '\0';
//    strncat(gDeviceDefJsonFilePath, json_path, MAX_FILE_PATH_LENGTH);
//
//    bool result = true;
//
//    /* TODO: Pass the json file path to DA Stack and get the easy-setup completion status.
//                       A new API in DA Stack needs to be added.*/
//
//    // Update the result in 'easysetup_complete'
//    *easysetup_complete = result;

    int result = 0;
    if (1 != (result = OICInitializeStack(json_path, easysetup_complete)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICInitializeStack failed (result:%d)", result);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_deinitialize(void)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);

// TODO: [jae] I leave this code for the case of TizenRT.
//    memset(gDeviceDefJsonFilePath, 0, sizeof(gDeviceDefJsonFilePath));

    int result = 0;
    if (1 != (result = OICDeinitializeStack()))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICDeinitializeStack failed (result:%d)", result);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_start(void)
{
    SDKAPI_LOG_INIT();
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    int result = 0;

    // Register callback for reset confirmation.
    if (1 != (result = OICRegisterConfirmResetStartFunc(get_reset_confirm_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterConfirmResetStartFunc failed (result:%d)", result);
        goto error;
    }

    // Register callback for easy-setup status.
    if (1 != (result = OICRegisterEasySetupStateFunc(get_things_status_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterEasySetupStateFunc failed (result:%d)", result);
        goto error;
    }

    // Register callback for receiving the Security Ownership Transfer state changes.
    if (1 != (result = OICRegisterOTMEventHandler(ownership_transfer_state_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterOTMEventHandler failed (result:%d)", result);
        goto error;
    }

    // Register callback for receiving request during ownership transfer for getting confirmation from user.
    if (1 != (result = OICRegisterUserConfirmFunc(get_user_confirm_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterUserConfirmFunc failed (result:%d)", result);
        goto error;
    }

    // Register callback for receiving the pin generated for ownership transfer.
    if (1 != (result = OICRegisterPINGeneratedFunc(generated_pin_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterPINGeneratedFunc failed (result:%d)", result);
        goto error;
    }

    // Register callback to receiving request for closing the PIN display.
    if (1 != (result = OICRegisterPINDisplayCloseFunc(close_pin_display_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterPINDisplayCloseFunc failed (result:%d)", result);
        goto error;
    }

    // Register callback to handle GET and POST requests.
    if (1 != (result = OICRegisterHandleRequestFunc(handle_get_request_cb, handle_set_request_cb)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICRegisterHandleRequestFunc failed (result:%d)", result);
        goto error;
    }

    // Start DA Stack.
    if (1 != (result = OICStartStack()))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICStartStack failed (result:%d)", result);
        goto error;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;

error:
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_OPERATION_FAILED;
}

int ocf_things_stop(void)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    int result = 0;

    // Terminate DA Stack.
    if( 1 != (result = OICStop()) )
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICStop failed (result:%d)", result);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    SDKAPI_LOG_DEINIT();
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_register_request_cb(ocf_things_get_request_cb get_cb,
                                   ocf_things_set_request_cb set_cb)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == get_cb || NULL == set_cb )
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return register_request_handler_cb(get_cb, set_cb);
}

int ocf_things_register_reset_cb(ocf_things_reset_confirm_cb confirm_cb,
                                 ocf_things_reset_result_cb  result_cb)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == confirm_cb || NULL == result_cb )
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    gHandleResetConfirmCB= confirm_cb;
    gHandleResetResultCB= result_cb;

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_reset(void)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    int result = 0;
    if(1 != (result = OICReset(NULL, RST_AUTO_RESET)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICReset failed (result:%d)", result);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_register_pin_handling_cb(ocf_things_pin_generated_cb generated_cb,
                                        ocf_things_pin_display_close_cb close_cb)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == generated_cb || NULL == close_cb )
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    gHandlePinGeneratedCB = generated_cb;
    gHandlePinDisplayCloseCB = close_cb;

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_register_user_confirm_cb(ocf_things_user_confirm_cb confirm_cb)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == confirm_cb)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    gHandleUserConfirmCB= confirm_cb;

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_register_things_status_change_cb(ocf_things_status_change_cb status_cb)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == status_cb)
    {
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    gHandleThingsStatusChangeCB= status_cb;

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

int ocf_things_notify_observers(const char* resource_uri)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if( NULL == resource_uri || 1 > strlen(resource_uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "The resource URI is invalid");
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    int result = 0;
    if(1 != (result = OICNotifyObservers(resource_uri)))
    {
        SDKAPI_LOG_V(SDKAPI_ERROR, "OICNotifyObservers failed (result:%d)", result);
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

/* Notification Defines */
#define URI_PUSHSERVICE                 "/service/pushservice"

#define KEY_PUSH_DATA                   "x.com.samsung.data"
#define KEY_PUSH_DATA_ET                "et" //Entity Type
#define KEY_PUSH_DATA_DT                "dt" //Device Type
#define KEY_PUSH_DATA_RT                "rt" //Resource Type
#define KEY_PUSH_DATA_DNAME             "dname" //Device Name
#define KEY_PUSH_DATA_REP               "rep" //Representation
#define KEY_PUSH_NS_MESSAGE_ID          "x.org.iotivity.ns.messageid"
#define KEY_PUSH_NS_PROVIDER_ID         "x.org.iotivity.ns.providerid"

int ocf_things_send_push_message(const char* resource_uri, ocf_things_representation_s* rep)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    if(NULL == resource_uri || 1 > strlen(resource_uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "The resource URI is invalid");
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }
    if(NULL == rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "rep is NULL");
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    if(OICGetEasySetupState())
    {
        const char *deviceType = OICGetDeviceType();
        const char *deviceName = OICGetDeviceName();
        //Choosing the first resource type
        int count = 0;
        char **resource_types = NULL;
        if (0 == OICGetResourceType(resource_uri, &count, &resource_types))
        {
            SDKAPI_LOG(SDKAPI_ERROR,"OICGetResourceType failed");
            SDKAPI_LOG_EXIT(SDKAPI_INFO);
            return OCF_THINGS_ERROR_OPERATION_FAILED;
        }
        if(count < 1)
        {
            SDKAPI_LOG_V(SDKAPI_ERROR,"OICGetResourceType invalid count = %d",count);
            SDKAPI_LOG_EXIT(SDKAPI_INFO);
            return OCF_THINGS_ERROR_OPERATION_FAILED;
        }
        const char *resourceType = resource_types[0]; //Choosing the first one

        OCRepPayload* data = OCRepPayloadCreate();

        OCRepPayloadSetPropString(data, KEY_PUSH_DATA_ET, "device.changed");
        OCRepPayloadSetPropString(data, KEY_PUSH_DATA_DT, deviceType);
        OCRepPayloadSetPropString(data, KEY_PUSH_DATA_DNAME, deviceName);
        OCRepPayloadSetPropString(data, KEY_PUSH_DATA_RT, resourceType);
        OCRepPayloadSetPropObject(data, KEY_PUSH_DATA_REP, rep->payload);

        const char *peerID = OICGetServerUUID(); //Corresponding IoTivity API - OCGetServerInstanceIDString();
        OCRepPayload* push_message = OCRepPayloadCreate();

        OCRepPayloadSetPropInt(push_message, KEY_PUSH_NS_MESSAGE_ID, 0); //Mandatory Field; For Push Message messageid = 0
        OCRepPayloadSetPropString(push_message, KEY_PUSH_NS_PROVIDER_ID, peerID); //Mandatory Field
        OCRepPayloadSetPropObjectAsOwner(push_message, KEY_PUSH_DATA, data); //data will be transferred to the push_message payload
        int result = 0;
        if(1 != (result = OICSendPushMessage(URI_PUSHSERVICE, push_message)))
        {
            SDKAPI_LOG_V(SDKAPI_ERROR, "OICSendPushMessage failed (result:%d)", result);
            SDKAPI_LOG_EXIT(SDKAPI_INFO);
            return OCF_THINGS_ERROR_OPERATION_FAILED;
        }
    }
    else
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Fail to send Push Notification message to Cloud (device not connected)");
        SDKAPI_LOG_EXIT(SDKAPI_INFO);
        return OCF_THINGS_ERROR_OPERATION_FAILED;
    }

    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return OCF_THINGS_ERROR_NONE;
}

ocf_things_representation_s* ocf_things_create_representation_inst()
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    ocf_things_representation_s *rep = create_representation_inst();
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
    return rep;
}

void ocf_things_destroy_representation_inst(ocf_things_representation_s* rep)
{
    SDKAPI_LOG_ENTRY(SDKAPI_INFO);
    destroy_representation_inst(rep);
    SDKAPI_LOG_EXIT(SDKAPI_INFO);
}
