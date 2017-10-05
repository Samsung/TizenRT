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

// Do not remove the include below

#include <string.h>
#include "logger.h"
#include "ocstack.h"
#include "escommon.h"
#include "ESEnrolleeCommon.h"
#include "octypes.h"

#ifndef ES_RESOURCE_HANDLER_H_
#define ES_RESOURCE_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ESConnectRequestCB) (ESResult, ESConnectRequest *);
typedef void (*ESWiFiConfCB) (ESResult, ESWiFiConfData *);
typedef void (*ESCoapCloudConfCB) (ESResult, ESCoapCloudConfData *);
typedef void (*ESDevConfCB) (ESResult, ESDevConfData *);

typedef void (*ESWriteUserdataCb)(OCRepPayload* payload, char* resourceType);
typedef void (*ESReadUserdataCb)(OCRepPayload* payload, char* resourceType, void** userdata);

/* Structure to represent a Light resource */
typedef struct
{
    OCResourceHandle handle;
    ProvStatus status; // provisiong status
    ESErrorCode lastErrCode;
    ES_CONNECT_TYPE connectRequest[NUM_CONNECT_TYPE];
    int numRequest;
} EasySetupResource;

typedef struct
{
    OCResourceHandle handle;
    WIFI_MODE supportedMode[NUM_WIFIMODE];
    uint8_t numMode;        // the number of device's supported wifi modes
    WIFI_FREQ supportedFreq;
    char ssid[OIC_STRING_MAX_VALUE]; // SSID
    char cred[OIC_STRING_MAX_VALUE]; // credential information.
    WIFI_AUTHTYPE authType;
    WIFI_ENCTYPE encType;
} WiFiConfResource;

typedef struct
{
    OCResourceHandle handle;
    char authCode[OIC_STRING_MAX_VALUE];
    char accessToken[OIC_STRING_MAX_VALUE];
    OAUTH_TOKENTYPE accessTokenType;
    char authProvider[OIC_STRING_MAX_VALUE];
    char ciServer[OIC_STRING_MAX_VALUE];
} CoapCloudConfResource;

typedef struct
{
    OCResourceHandle handle;
    char devName[OIC_STRING_MAX_VALUE];
    char modelNumber[OIC_STRING_MAX_VALUE];
    char location[OIC_STRING_MAX_VALUE];
    char language[OIC_STRING_MAX_VALUE];
    char country[OIC_STRING_MAX_VALUE];
} DevConfResource;

/* Note: These values of structures are not thread-safety */
extern EasySetupResource g_ESEasySetupResource;
extern WiFiConfResource g_ESWiFiConfResource;
extern CoapCloudConfResource g_ESCoapCloudConfResource;
extern DevConfResource g_ESDevConfResource;

OCStackResult CreateEasySetupResources(bool isSecured, ESResourceMask resourceMask);
OCStackResult DeleteEasySetupResources();

OCStackResult SetDeviceProperty(ESDeviceProperty *deviceProperty);
OCStackResult SetEnrolleeState(ESEnrolleeState esState);
OCStackResult SetEnrolleeErrCode(ESErrorCode esErrCode);
OCEntityHandlerResult CheckEhRequestPayload(OCEntityHandlerRequest *ehRequest);

void RegisterWifiRsrcEventCallBack(ESWiFiConfCB);
void RegisterCloudRsrcEventCallBack(ESCoapCloudConfCB);
void RegisterDevConfRsrcEventCallBack(ESDevConfCB);
void UnRegisterResourceEventCallBack(void);
ESResult SetCallbackForUserData(ESReadUserdataCb readCb, ESWriteUserdataCb writeCb);

#ifdef __cplusplus
}
#endif

#endif //ES_RESOURCE_HANDLER_H_
