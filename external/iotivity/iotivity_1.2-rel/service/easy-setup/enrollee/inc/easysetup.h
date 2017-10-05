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
#include "ESEnrolleeCommon.h"

/**
 * @file
 *
 * This file contains Enrollee APIs
 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * A function pointer for registering a user-defined function to set user-specific properties to a
 * response going back to a client.
 * @param payload Represents a response. You can set a specific value with specific property key
 * to the payload. If a client receives the response and know the property key, then it can
 * extract the value.
 * @param resourceType Used to distinguish which resource the received property belongs to.
 */
typedef void (*ESWriteUserdataCb)(OCRepPayload* payload, char* resourceType);

/**
 * A function pointer for registering a user-defined function to parse user-specific properties from
 * received POST request.
 * @param payload Represents a received POST request. If you know user-specific property key,
 * then you can extract a corresponding value if it exists.
 * @param resourceType Used to distinguish which resource the received property belongs to
 * @param userdata User-specific data you want to deliver to desired users, i.e. application.
 * The user should know a data structure of passed userdata.
 */
typedef void (*ESReadUserdataCb)(OCRepPayload* payload, char* resourceType, void** userdata);

/**
 * This function Initializes the EasySetup. This API must be called prior to invoking any other API
 *
 * @param isSecured         True if the Enrollee is operating in secured mode.
 * @param resourceMask      Provisining Resource Type which application wants to make.
 *                          ES_WIFICONF_RESOURCE = 0x01,
 *                          ES_COAPCLOUDCONF_RESOURCE = 0x02,
 *                          ES_DEVCONF_RESOURCE = 0x04
 * @param callbacks         ESProvisioningCallbacks for updating Easy setup Resources' data to the application
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESInitEnrollee(bool isSecured, ESResourceMask resourceMask, ESProvisioningCallbacks callbacks);


/**
 * This function Sets Device Information.
 *
 * @param deviceProperty   Contains device information composed of WiFiConf Structure & DevConf Structure
 * @return ::ES_OK on success, some other value upon failure.
 *
 * @see ESDeviceProperty
 */
ESResult ESSetDeviceProperty(ESDeviceProperty *deviceProperty);


/**
 * This function Sets Enrollee's State.
 *
 * @param esState   Contains current enrollee's state.
 * @return ::ES_OK on success, some other value upon failure.
 *
 * @see ESEnrolleeState
 */
ESResult ESSetState(ESEnrolleeState esState);


/**
 * This function Sets Enrollee's Error Code.
 *
 * @param esErrCode   Contains enrollee's error code.
 * @return ::ES_OK on success, some other value upon failure.
 *
 * @see ESErrorCode
 */
ESResult ESSetErrorCode(ESErrorCode esErrCode);

/**
 * This function performs termination of all Easy setup resources.
 *
 * @return ::ES_OK on success, some other value upon failure.
 */
ESResult ESTerminateEnrollee();

/**
 * This function is to set two function pointer to handle user-specific properties in in-comming
 * POST request and to out-going response for GET or POST request.
 * If you register certain functions with this API, you have to handle OCRepPayload structure to
 * set and get properties you want.
 *
 * @param readCb a callback for parsing properties from POST request
 * @param writeCb a callback for putting properties to a response to be sent
 *
 * @return ::ES_OK on success, some other value upon failure.
 *
 * @see ESReadUserdataCb
 * @see ESWriteUserdataCb
 */
ESResult ESSetCallbackForUserdata(ESReadUserdataCb readCb, ESWriteUserdataCb writeCb);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif /* EASYSETUP_ENROLLEE_H__ */

