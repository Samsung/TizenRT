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


#ifndef EASYSETUP_ENROLLEE_H__
#define EASYSETUP_ENROLLEE_H__

#include "escommon.h"

/**
 * @file
 *
 * This file contains Enrollee APIs
 */


/*
 * Callback function for updating the Enrollee OnBoarding and Provisioning status result
 * to the application
 *
 * @param esResult ESResult provides the current state of the Enrollee Device
 */
typedef void (*ESEnrolleeEventCallback)(ESResult esResult, ESEnrolleeState enrolleeState);



typedef struct
{
    void (*WiFiProvCb) (ESWiFiProvData *);
    void (*DevConfProvCb) (ESDevConfProvData *);
    void (*CloudDataProvCb) (ESCloudProvData *);
} ESProvisioningCallbacks;

/**
 * This function Initializes the EasySetup. This API must be called prior to invoking any other API
 *
 * @param isSecured         True if the Enrollee is operating in secured mode.
 * @param resourceMask      Provisining Resource Type which application wants to make.
 *                          ES_WIFI_RESOURCE = 0x01,
 *                          ES_CLOUD_RESOURCE = 0x02,
 *                          ES_DEVCONF_RESOURCE = 0x04
 * @param callbacks         ESProvisioningCallbacks for updating Provisioning Resources' data to the application
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESInitEnrollee(bool isSecured, ESResourceMask resourceMask, ESProvisioningCallbacks callbacks);

/**
 * This function Notify Enrollee's State to THINGS_App.
 *
 */
void ESRegisterNotifyEasySetupState(void* func);

/**
 * This function Gets Enrollee's State.
 *
 * @return ESEnrolleeState State return.
 */
ESEnrolleeState ESGetState(void);

/**
 * This function Gets Cloud State if device was log-in to Cloud.
 *
 * @return bool return.
 */
bool ESGetCloudLogINState(void);

/**
 * This function Sets Device Information.
 *
 * @param deviceProperty   Contains device information composed of WiFi Structure & DevConf Structure
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESSetDeviceProperty(ESDeviceProperty *deviceProperty);


/**
 * This function Sets Enrollee's State.
 *
 * @param esState   Contains current enrollee's state.
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESSetState(ESEnrolleeState esState);


/**
 * This function Sets Enrollee's Error Code.
 *
 * @param esErrCode   Contains enrollee's error code.
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESSetErrorCode(ESErrorCode esErrCode);

/**
 * This function Sets cloud's Error Code.
 *
 * @param esErrCode   Contains cloud's error code.
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESSetCloudErrorCode(CIErrorCode esErrCode);

/**
 * This function performs termination of Provisioning and Network resources.
 * Also terminates the IoTivity core stack.
 *
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESTerminateEnrollee();



#endif /* EASYSETUP_ENROLLEE_H__ */

