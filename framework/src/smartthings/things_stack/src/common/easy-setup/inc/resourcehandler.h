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
//#include "logger.h"
#include "ocstack.h"
#include "escommon.h"
#include "octypes.h"

#ifndef ES_RESOURCE_HANDLER_H_
#define ES_RESOURCE_HANDLER_H_

typedef enum {
 WIFI_INIT = 0,
 WIFI_READY = 1,
 WIFI_RESPONSE = 2,
 WIFI_SET = 3,
 WIFI_DONE
}WiFiProvState;

typedef void (*ESWiFiCB) (ESResult, ESWiFiProvData *);
typedef void (*ESCloudCB) (ESResult, ESCloudProvData *);
typedef void (*ESDevConfCB) (ESResult, ESDevConfProvData *);

/* Structure to represent a Light resource */
typedef struct PROVRESOURCE
{
    OCResourceHandle handle;
    ProvStatus status; // provisiong status
    ESErrorCode lastErrCode;
    CIErrorCode vdErrCode;
} ProvResource;

typedef struct
{
    OCResourceHandle handle;
    WIFI_MODE supportedMode[NUM_WIFIMODE];
    uint8_t numMode;        // the number of device's supported wifi modes
    WIFI_FREQ supportedFreq;
    char ssid[MAX_SSIDLEN]; // target network name, i.e. SSID for WLAN, MAC address for BT.
    char cred[MAX_CREDLEN]; // credential information.
    WIFI_AUTHTYPE authType;
    WIFI_ENCTYPE encType;
    int discoveryChannel;
} WiFiResource;

typedef struct
{
    OCResourceHandle handle;
    char authCode[OIC_STRING_MAX_VALUE];
    char authProvider[OIC_STRING_MAX_VALUE];
    char ciServer[OIC_STRING_MAX_VALUE];
    char serverID[OIC_STRING_MAX_VALUE];
    char clientID[OIC_STRING_MAX_VALUE];
    char accesstoken[OIC_STRING_MAX_VALUE];
    int  actokenType;
    char refreshtoken[OIC_STRING_MAX_VALUE];
    char uid[OIC_STRING_MAX_VALUE];
} CloudResource;

typedef struct
{
    OCResourceHandle handle;
    char devName[MAX_DEVICELEN];
    char language[OIC_STRING_MAX_VALUE];
    char country[OIC_STRING_MAX_VALUE];
    char deviceType[OIC_STRING_MAX_VALUE];
    char deviceSubType[OIC_STRING_MAX_VALUE];
    char datetime[OIC_STRING_MAX_VALUE];
} DevConfResource;


void setSSIDinWiFiResource(const char* ssid);
void initESCloudProvData( ESCloudProvData* pCloudData);
void cloneESCloudProvData( ESCloudProvData* des, ESCloudProvData* src );
void SetWiFiProvState(WiFiProvState value);
WiFiProvState GetWiFiProvState(void);

OCStackResult CreateEasySetupResources(bool isSecured, ESResourceMask resourceMask);
OCStackResult DeleteEasySetupResources();
OCStackResult ProvRSCNotifyAllObservers(void);

ESEnrolleeState GetEnrolleeState(void);

OCStackResult SetDeviceProperty(ESDeviceProperty *deviceProperty);
OCStackResult SetEnrolleeState(ESEnrolleeState esState);
OCStackResult SetEnrolleeErrCode(ESErrorCode esErrCode);

void GetTargetNetworkInfoFromProvResource(char *, char *);
void RegisterWifiRsrcEventCallBack(ESWiFiCB);
void RegisterCloudRsrcEventCallBack(ESCloudCB);
void RegisterDevConfRsrcEventCallBack(ESDevConfCB);
void UnRegisterResourceEventCallBack(void);

const char *getResult(OCStackResult result);
const char* getProvStatus(ESEnrolleeState status);
const char* getProvErrcode(ESErrorCode lastErrcode);

bool StartCloudProvisioning(void* CIinfo);

#endif //ES_RESOURCE_HANDLER_H_
