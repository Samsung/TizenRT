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

#ifndef _CLOUD_CONNECTOR_H_
#define _CLOUD_CONNECTOR_H_

#include "ocstack.h"
#include "oicresource.h"
#include "oicwaithandler.h"
#include "escommon.h"

#define DEFAULT_CONTEXT_VALUE   0x99

#define DEFAULT_MQ_URI             "/oic/ps"

#define KEY_TOKEN_ACCESS           "accesstoken"      // mandatory
#define KEY_TOKEN_ACCESS_REFRESH   "refreshtoken"     // mandatory
#define KEY_TOKEN_TYPE			   "tokentype"	      // mandatory
#define KEY_EXPIRE_TIME			   "expiresin"	      // mandatory
#define KEY_ID_USER                "uid"              // mandatory
#define KEY_SERVER_REDIRECT_URI    "redirecturi"      // option
#define KEY_CERTIFICATE_FILE       "certificate"      // option
#define KEY_SERVER_ID              "sid"              // option
#define KEY_ID_DEVICE              "di"
#define KEY_STATUS                 "status"
#define KEY_LOGINOUT               "login"
#define KEY_ICORE_VER              "coreversion"
#define KEY_IOTIVITY_VER           "verticalversion"
#define KEY_TYPE_GRANT             "granttype"
#define KEY_TYPE_MESSAGE           "message"
#define CLOUD_ERROR_CODE            "code"
#define VALUE_TYPE_GRANT_TOKEN     "refresh_token"

/**
 * @brief              Entry Point for checking about internet & Cloud connection.
 * @param DomainName   wanted Domain-Name string to look-up IP address. (unit: const char*)
 * @param pIP          Variable pointer to store Founded IP address. (unit: char**)
 * @return             success : return 0 \n
 *                     failure : return > 0
 */
int CIConnectionPreCheck(const char* DomainName, char ** pIP);

/**
 * @brief                  Send Sign-UP request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param deviceId         Identifier of device. (unit: const char*)
 * @param eventData        Data structure pointer for Cloud-Provisioning Data.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudSignup(const char *host,
                            const char *deviceId,
                            const ESCloudProvData* eventData,
                            OCClientResponseHandler response,
                            CheckTimeOutCallFunc timeoutHandler,
                            TimeOut_s* timeout);

/**
 * @brief                  Send Sign-IN request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User. (unit: const char*)
 * @param deviceId         Identifier of device. (unit: const char*)
 * @param accesstoken      received access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudSession(const char *host,
                            const char *uId,
                            const char *deviceId,
                            const char *accesstoken,
                            bool isLogin,
                            OCClientResponseHandler response,
                            CheckTimeOutCallFunc timeoutHandler,
                            TimeOut_s* timeout);

/**
 * @brief                  Send Resources Publish request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*)\n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param list             Resource List pointer for Publishing to Cloud.
 * @param length           Count of Resource in Resource List(list).
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudRscPublish(char* host,
                            OicResource** list,
                            int length,
                            OCClientResponseHandler response,
                            CheckTimeOutCallFunc timeoutHandler,
                            TimeOut_s* timeout);

OCStackResult OCCloudRscPublishWithDeviceID(char* host,
                                            const char* id,
                                            OicResource** list,
                                            int length,
                                            OCClientResponseHandler response,
                                            CheckTimeOutCallFunc timeoutHandler,
                                            TimeOut_s* timeout);

OCStackResult OCCloudDevProfilePublish(char* host,
                                OCClientResponseHandler response,
                                CheckTimeOutCallFunc timeoutHandler,
                                TimeOut_s* timeout);

/**
 * @brief                  Send Refresh access-token request to Cloud.\n
 *                         Set should call refresh before expiresin of refresh-token or when receive 4.01 during sign-in
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User.
 * @param deviceId         Identifier of Device.
 * @param refreshtoken     Token for refreshing access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult OCCloudRefresh(const char *host,
                            const char *uId,
				const char *deviceId,
                            const char *refreshtoken,
                            OCClientResponseHandler response);

extern OCStackResult OCCloudTopicPublishTopic(const char *host,
                            const char *topic,
                            OCRepPayload * payload,
                            OCClientResponseHandler response);

#endif
