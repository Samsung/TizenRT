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

#ifndef _CLOUD_MANAGER_H_
#define _CLOUD_MANAGER_H_

#include "oicresource.h"
#include "escommon.h"
#include "cloud_connector.h"
//#include "notification_manager.h"
#include "oicserverbuilder.h"
#include <stdint.h>

#define CI_COMMON_REFRESH_TIME      12      // access token Refresh period time without accesstoken-expire. (unit: hour)
#define CI_TOKEN_EXPIRECHECK_TIME   3000    // access token Check period time whether it's expired or available. (unit: second)

#define CLOUD_EXPIRESIN_INVALID		-1

#ifdef __SECURED__
    #define DEFAULT_COAP_TCP_HOST "coaps+tcp://"
    #define UNSUPPORT_COAP_TCP_HOST "coap+tcp://"
    #define DEFAULT_COAP_PORT "443"
#else
    #define DEFAULT_COAP_TCP_HOST "coap+tcp://"
    #define UNSUPPORT_COAP_TCP_HOST "coaps+tcp://"
    #define DEFAULT_COAP_PORT "5683"
#endif

typedef enum _target_Publish_
{
    RscPub_ALL          = 0,
    RscPub_MainOnly     = 1,
    RscPub_SubALL       = 2,
    RscPub_NeedSubOnly  = 3,
} Enum_RPTarget;

typedef struct ESCloud_SignUp {
    char * domain;
    char * address;
    char * port;
	char * accessToken;
	char * refreshToken;
	char * tokenType;
	int64_t expireTime;
	char * uid;
	char * redirectURI;
	char * certificate;
	char * sid;
}ESCloud_SignUp;

void* ESCloud_Init(OICServerBuilder* serverBuilder);

void ESCloud_Terminate(void);

void ESCloud_SignUp_clear(ESCloud_SignUp * data);

bool ESCloud_SignUp_init(ESCloud_SignUp ** data);

bool ESCloud_SessionStopTrigger(ESEnrolleeState esState);

/**
 * @brief                 Call function to send request for Publishing Resource to Cloud.
 * @param list            Resources-list pointer. (to be publishing)
 * @param length          EA of Resources in "list" pointer.
 * @param timeout         time-out value how long wait cloud-response when send request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult PublishResourceIntoCloud(Enum_RPTarget target, TimeOut_s* timeout);

/**
 * @brief                 Call function to send request for login/logout to Cloud.
 * @param value           true  : login \n
 *                        false : logout
 * @param timeout         time-out value how long wait cloud-response when send request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult LogInOutToCloud(bool value, TimeOut_s* timeout);

/**
 * @brief                 Call function to send request for Finding myself-resource to Cloud.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult FindCloudResource(void);

/**
 * @brief                 Call function to send request for Refreshing access-token from Cloud.
 * @details               It's Entry-Point to send "RefreshToken" request.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult RefreshTokenIntoCloud(void);

/**
 * @brief                 Entry-Point when receive Cloud-Provisioning.
 * @details               Managing Cloud session for Cloud Connection safety against Multiple-Provisioning & Auto Sign-IN.
 * @param eventData       Received Cloud-Provisioning Data.
 * @return                Always return NULL
 */
void* cloudDataCbESM(ESCloudProvData *eventData);

/**
 * @brief                 Cloud Re-SignIN Process.
 * @details               Entry-Pointer for Auto-Sign-IN process.
 * @param timeout         Time-out value how long wait cloud-response when send request. \n
 *                        NULL : Use with default Time-Out Counter. \n
 *                        Other: Use with User-defined Time-Out Counter.
 * @return                Always return 0
 */
int CloudRetrySignIN(TimeOut_s* timeout);

int CIRetryStop_byWiFiCB( bool isRetry );

int CIRetryStop_byTCPCB( const char* addrIP, const int port );

int PushNotificationToCloud(const char* uri, OCRepPayload* payload);

void CIStopCloudConnection(void* CBfunc);

#endif  // _CLOUD_MANAGER_H_
