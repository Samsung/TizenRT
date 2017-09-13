//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * This file contains private internal callback function
 * prototypes for Easy setup [Enrollee]
 */

#ifndef EASYSETUP_ENROLLEE_CALLBACKS_H__
#define EASYSETUP_ENROLLEE_CALLBACKS_H__

#include "escommon.h"

// #ifdef __cplusplus
// extern "C" {
// #endif // __cplusplus

//-----------------------------------------------------------------------------
// Private internal callback function prototypes for Easy setup [Enrollee]
//-----------------------------------------------------------------------------

 /*
 * Callback for on boarding
 */
extern void ESOnboardingCallback(ESResult esResult);

 /*
 * Callback for provisioning
 */
extern void ESProvisioningCallback(ESResult esResult, ESProvisionedData *data);

 /*
 * Callback for on boarding target Network
 */
extern void ESOnboardingCallbackTargetNet(ESResult esResult);

 /*
 * Function for validating the parameter for ESInitEnrollee API
 */
// static bool ESEnrolleeValidateParam(OCConnectivityType networkType, const char *ssid,
//                                                 const char *passwd, ESEnrolleeEventCallback cb);

// #ifdef __cplusplus
// }
// #endif // __cplusplus


#endif /* EASYSETUP_ENROLLEE_CALLBACKS_H__ */


