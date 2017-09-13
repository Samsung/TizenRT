//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 * @file
 *
 * This file contains the implementation for EasySetup Enrollee device
 */

#include "octypes.h"
#include "easysetup.h"
#include "thingslogger.h"
#include "resourcehandler.h"
#include "oic_string.h"
#include "cloud_manager.h"
#include "thingsapi.h"

/**
 * @var ES_ENROLLEE_TAG
 * @brief Logging tag for module name.
 */
#define ES_ENROLLEE_TAG "[easysetup]"

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

/**
 * @var gIsSecured
 * @brief Variable to check if secure mode is enabled or not.
 */
static bool gIsSecured = false;

static ESProvisioningCallbacks gESProvisioningCb;
static ESDeviceProperty gESDeviceProperty;
static OICGetEasySetupStateFuncType    gNotifyEasySetupState = NULL;

void ESRegisterNotifyEasySetupState(void* func)
{
    gNotifyEasySetupState = (OICGetEasySetupStateFuncType)func;
}

void ESWiFiRsrcCallback(ESResult esResult, ESWiFiProvData *eventData)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESWiFiRsrcCallback IN");

    if(esResult != ES_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESWiFiRsrcCallback ES_ERROR Occured");
        return;
    }

    // deliver data to ESProvisioningCallbacks
    if(gESProvisioningCb.WiFiProvCb != NULL)
    {
        gESProvisioningCb.WiFiProvCb(eventData);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "WiFiProvCb is NULL");
        return;
    }
}

void ESCloudRsrcCallback(ESResult esResult, ESCloudProvData *eventData)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESCloudRsrcCallback IN");

    if(esResult != ES_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESCloudRsrcCallback ES_ERROR Occured");
        return;
    }

    if(gESProvisioningCb.CloudDataProvCb != NULL)
    {
        gESProvisioningCb.CloudDataProvCb(eventData);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "CloudDataProvCb is NULL");
        return;
    }
}

void ESDevconfRsrcallback(ESResult esResult, ESDevConfProvData *eventData)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESDevconfRsrcallback IN");

    if(esResult != ES_OK)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESDevconfRsrcallback ES_ERROR Occured");
        return;
    }

    if(gESProvisioningCb.DevConfProvCb != NULL)
    {
        gESProvisioningCb.DevConfProvCb(eventData);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "DevConfProvCb is NULL");
        return;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESDevconfRsrcallback OUT");
}

ESResult ESInitEnrollee(bool isSecured, ESResourceMask resourceMask, ESProvisioningCallbacks callbacks)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESInitEnrollee IN");

    gIsSecured = isSecured;
    gESProvisioningCb.WiFiProvCb = NULL;
    gESProvisioningCb.DevConfProvCb = NULL;
    gESProvisioningCb.CloudDataProvCb = NULL;

    if((resourceMask & ES_WIFI_RESOURCE) == ES_WIFI_RESOURCE)
    {
        if(callbacks.WiFiProvCb != NULL)
        {
            gESProvisioningCb.WiFiProvCb = callbacks.WiFiProvCb;
            RegisterWifiRsrcEventCallBack(ESWiFiRsrcCallback);
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "WiFiProvCb NULL");
            return ES_ERROR;
        }
    }
    if((resourceMask & ES_DEVCONF_RESOURCE) == ES_DEVCONF_RESOURCE)
    {
        if(callbacks.DevConfProvCb != NULL)
        {
            gESProvisioningCb.DevConfProvCb = callbacks.DevConfProvCb;
            RegisterDevConfRsrcEventCallBack(ESDevconfRsrcallback);
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "DevConfProvCb NULL");
            return ES_ERROR;
        }
    }
    if((resourceMask & ES_CLOUD_RESOURCE) == ES_CLOUD_RESOURCE)
    {
        if(callbacks.CloudDataProvCb != NULL)
        {
            gESProvisioningCb.CloudDataProvCb = callbacks.CloudDataProvCb;
            RegisterCloudRsrcEventCallBack(ESCloudRsrcCallback);
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "CloudDataProvCb NULL");
            return ES_ERROR;
        }
    }

    if(CreateEasySetupResources(gIsSecured, resourceMask) != OC_STACK_OK)
    {
        UnRegisterResourceEventCallBack();

        if (DeleteEasySetupResources() != OC_STACK_OK)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Deleting prov resource ES_ERROR!!");
        }

        return ES_ERROR;
    }


    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESInitEnrollee OUT");
    return ES_OK;
}

ESEnrolleeState ESGetState(void)
{
	return GetEnrolleeState();
}

bool ESGetCloudLogINState(void)
{
    switch( GetEnrolleeState() )
    {
    case ES_STATE_REGISTERED_TO_CLOUD:
    case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
    case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
    case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
        return true;
    }

    return false;
}

ESResult ESSetDeviceProperty(ESDeviceProperty *deviceProperty)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetDeviceProperty IN");

    if(SetDeviceProperty(deviceProperty) != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESSetDeviceProperty ES_ERROR");
        return ES_ERROR;
    }

    int modeIdx = 0;
    while((deviceProperty->WiFi).mode[modeIdx] != WiFi_EOF)
    {
        (gESDeviceProperty.WiFi).mode[modeIdx] = (deviceProperty->WiFi).mode[modeIdx];
        THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "WiFi Mode : %d", (gESDeviceProperty.WiFi).mode[modeIdx]);
        modeIdx ++;
    }
    (gESDeviceProperty.WiFi).freq = (deviceProperty->WiFi).freq;
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "WiFi Freq : %d", (gESDeviceProperty.WiFi).freq);

    DAStrncpy((gESDeviceProperty.DevConf).deviceName, (deviceProperty->DevConf).deviceName, MAX_DEVICELEN);
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "Device Name : %s", (gESDeviceProperty.DevConf).deviceName);

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetDeviceProperty OUT");
    return ES_OK;
}

ESResult ESSetState(ESEnrolleeState esState)
{
    //THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetState IN");
    OCStackResult res = OC_STACK_ERROR;

    if(esState < ES_STATE_INIT || esState >= ES_STATE_EOF)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Invalid ESEnrolleeState : %d", esState);
        return ES_ERROR;
    }

    if(ESCloud_SessionStopTrigger(esState) == true)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Cloud Connection Log-Out ing...");
        return ES_ERROR;
    }

    if( (res = SetEnrolleeState(esState)) == OC_STACK_ERROR )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESSetState setting is failed.");
        return ES_ERROR;
    }

    if( gNotifyEasySetupState != NULL && res == OC_STACK_OK )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "Notify Easy-Setup State to THINGS_APP. %d", esState);
        gNotifyEasySetupState(esState);
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "Set ESState succesfully : %d", esState);
    //THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetState OUT");
    return ES_OK;
}

static ESErrorCode CIerrcode2ESerrcode(CIErrorCode cmErrCode)
{
    ESErrorCode esErrCode = ES_ERRCODE_NO_ERROR;

    switch(cmErrCode)
    {
    case ERRCI_NO_ERROR:
    case ERRCI_SUCCESS:
        esErrCode = ES_ERRCODE_NO_ERROR;
        break;
    case ERRCI_SAMSUNG_ACCOUNT_AUTHORIZATION_FAILED:
    case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
        esErrCode = ES_ERRCODE_INVALID_AUTHCODE;
        break;
    case ERRCI_TOKEN_VALIDATION_FAILED:
        esErrCode = ES_ERRCODE_INVALID_ACCESSTOKEN;
        break;
    case ERRCI_DEVICE_NOT_FOUND:
        esErrCode = ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD;
        break;
    case ERRCI_USER_NOT_FOUND:
        esErrCode = ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD;
        break;
    default :
        esErrCode = ES_ERRCODE_UNKNOWN;
        break;
    }

    return esErrCode;
}

ESResult ESSetErrorCode(ESErrorCode esErrCode)
{
    THINGS_LOG_D(THINGS_DEBUG,ES_ENROLLEE_TAG, "ESSetErrorCode IN");

    if(esErrCode < ES_ERRCODE_NO_ERROR || esErrCode > ES_ERRCODE_UNKNOWN)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Invalid ESSetErrorCode : %d", esErrCode);
        return ES_ERROR;
    }

    if(SetEnrolleeErrCode(esErrCode) != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESSetErrorCode ES_ERROR");
        return ES_ERROR;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "Set ESErrorCode successfully : %d", esErrCode);
    return ES_OK;
}

ESResult ESSetCloudErrorCode(CIErrorCode esErrCode)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetCloudErrorCode IN");

    ESErrorCode esError = ES_ERRCODE_NO_ERROR;

    if(esErrCode < ERRCI_NO_ERROR || esErrCode > ERRCI_UNKNOWN)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Invalid ESSetCloudErrorCode : %d", esErrCode);
        return ES_ERROR;
    }

    esError = CIerrcode2ESerrcode(esErrCode);

    if(SetCloudErrCode(esErrCode) != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "ESSetCloudErrorCode ES_ERROR");
        return ES_ERROR;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "Set ESErrorCode successfully : %d", esErrCode);

    ESSetErrorCode(esError);

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESSetCloudErrorCode OUT");
    return ES_OK;
}

ESResult ESTerminateEnrollee()
{
    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESTerminateEnrollee IN");

    UnRegisterResourceEventCallBack();

    //Delete Prov resource
    if (DeleteEasySetupResources() != OC_STACK_OK)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_ENROLLEE_TAG, "Deleting prov resource ES_ERROR!!");
        return ES_ERROR;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_ENROLLEE_TAG, "ESTerminateEnrollee success");
    return ES_OK;
}
