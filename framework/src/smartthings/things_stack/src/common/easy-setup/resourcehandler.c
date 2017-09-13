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

#include "resourcehandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef OCF_RTOS
#include <net/lwip/ipv4/inet.h>
#else
#include <arpa/inet.h>
#endif
#include "thingsapi.h"
#include "octypes.h"
#include "ocpayload.h"
#include "oic_string.h"
#include "thingslogger.h"
#include "cloud_manager.h"
#include "oicstringutil.h"
#include "thingsmalloc.h"
#include "cautilinterface.h"
#include "oiccommon.h"
#include "oicnetwork.h"
#include "things_time.h"

/**
 * @var ES_RH_TAG
 * @brief Logging tag for module name.
 */
#define ES_RH_TAG "[ezsetup-RH]"
//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

/**
 * @var gProvResource
 * @brief Structure for holding the Provisioning status and target information required to
 * connect to the target network
 */
static ProvResource gProvResource;
static WiFiResource gWiFiResource;
static CloudResource gCloudResource;
static DevConfResource gDevConfResource;

/**
 * @var gWiFiData
 * @brief Structure for holding the target information required to
 * connect to the target network
 */
 static ESWiFiProvData gWiFiData;

/**
 * @var gDevConfData
 * @brief Structure for holding the device information
 */
 static ESDevConfProvData gDevConfData;

/**
 * @var gCloudData
 * @brief Structure for holding the cloud information required to
 * connect to CI Server
 */
 static ESCloudProvData gCloudData;

static WiFiProvState ESWiFiProvState = WIFI_INIT;

static pthread_mutex_t gSTATUSmutex = PTHREAD_MUTEX_INITIALIZER;

//-----------------------------------------------------------------------------
// Private internal function prototypes
//-----------------------------------------------------------------------------
OCEntityHandlerResult OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *ehRequest,
        void *callback);
OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
OCEntityHandlerResult ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
void updateProvResource(OCEntityHandlerRequest* ehRequest, OCRepPayload* input);
void updateWiFiResource(OCRepPayload* input);
bool updateCloudResource(OCRepPayload* input);
void updateDevConfResource(OCRepPayload* input);
static int CheckCIServer_IPv4(char* ciServer, ESCloudProvData* cloudData);

ESWiFiCB gWifiRsrcEvtCb = NULL;
ESCloudCB gCloudRsrcEvtCb = NULL;
ESDevConfCB gDevConfRsrcEvtCb = NULL;

static inline void deleteChildPayload(OCRepPayload* payload)
{
    if( payload == NULL )
        return ;

    deleteChildPayload(payload->next);
    OCPayloadDestroy((OCPayload*)payload);
}

static bool CompareResourceInterface(const char *from, const char *iface)
{
    bool ret = false;
    char *str = NULL;
    char *ptr = NULL;

    if( from == NULL || iface == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "invalid input data.(from=%s, iface=%s)", from, iface);
        return ret;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Input data.(from=%s, iface=%s)", from, iface);

    if( (str = OICStrdup(from)) == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "OICStrdup function is failed.");
        return ret;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "strFrom=%s", str);

    ptr = str;
    while( (ptr=strtok(ptr, ";")) != NULL )
    {
        if(strstr(ptr, ".if."))
        {
            char *if_ptr = NULL;
            if_ptr = strtok(ptr, "=");
            if_ptr = strtok(NULL, "=");

            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "if_ptr=%s", if_ptr);

            if(if_ptr != NULL && !strcmp(if_ptr, iface))
            {
                THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Found interface.(%s)", iface);
                ret = true;
                break;
            }
        }

        ptr = NULL;
    }

    free(str);
    return ret;
}

static OCRepPayload* makeRepPayload(OCResourceHandle rscHandle, OCDevAddr* devAddr)
{
    OCRepPayload *add = NULL;
    char *resourceType[1] = {NULL, };
    char *resourceInterface[1] = {NULL, };
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};

    if( rscHandle == NULL || devAddr == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid input data.(rscHandle=0x%X, devAddr=0x%X)", rscHandle, devAddr);
        return NULL;
    }

    if( (add = OCRepPayloadCreate()) == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    add->base.type = PAYLOAD_TYPE_REPRESENTATION;

    if(rscHandle == gWiFiResource.handle)
    {
        resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_WIFI);
        resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_WIFI);
    }else if(rscHandle == gDevConfResource.handle)
    {
        resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_DEVCONF);
        resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_DEVCONF);
    }else if(rscHandle == gCloudResource.handle)
    {
        resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_CLOUDSERVER);
        resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_CLOUDSERVER);
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "Not supported resource handler(rscHandle=0x%X)", rscHandle);
        OCPayloadDestroy((OCPayload*)add);
        return NULL;
    }


    OCRepPayloadSetStringArray(add, OC_RSRVD_RESOURCE_TYPE,
                                    (const char **)resourceType, dimensions);
    OCRepPayloadSetStringArray(add, OC_RSRVD_INTERFACE,
                                    (const char **)resourceInterface, dimensions);
    /*! Added by DA for Memory leak fix*/
    if(resourceType[0]) free(resourceType[0]);
    if(resourceInterface[0]) free(resourceInterface[0]);

    OCResourceProperty p = OCGetResourceProperties(rscHandle);  //(OCResourceHandle *)gCloudResource.handle);
    OCRepPayload *policy = OCRepPayloadCreate();
    if (!policy)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        OCPayloadDestroy((OCPayload*)add);
        return NULL;
    }

    OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP,
                            ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
    if (p & OC_SECURE)
    {
        OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);
        uint16_t securePort = CAGetAssignedPortNumber(devAddr->adapter, devAddr->flags);
        OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);
    }

    OCRepPayloadSetPropObject(add, OC_RSRVD_POLICY, policy);
    OCPayloadDestroy((OCPayload*)policy);

    return add;
}

void SetWiFiProvState(WiFiProvState value)
{
    if( ESWiFiProvState == (value-1) || value == WIFI_INIT )
    {
        ESWiFiProvState = value;
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Set WiFiProvState=%d", ESWiFiProvState);
    }
}

WiFiProvState GetWiFiProvState(void)
{
    return ESWiFiProvState;
}

void RegisterWifiRsrcEventCallBack(ESWiFiCB cb)
{
    gWifiRsrcEvtCb = cb;
}

void RegisterCloudRsrcEventCallBack(ESCloudCB cb)
{
    gCloudRsrcEvtCb = cb;
}

void RegisterDevConfRsrcEventCallBack(ESDevConfCB cb)
{
    gDevConfRsrcEvtCb = cb;
}

void UnRegisterResourceEventCallBack()
{
    if (gWifiRsrcEvtCb)
    {
        gWifiRsrcEvtCb = NULL;
    }
    if (gCloudRsrcEvtCb)
    {
        gCloudRsrcEvtCb = NULL;
    }
    if (gDevConfRsrcEvtCb)
    {
        gDevConfRsrcEvtCb = NULL;
    }
}

void GetTargetNetworkInfoFromProvResource(char *name, char *pass)
{
    if (name != NULL && pass != NULL)
    {
        DAStrncpy(name, gWiFiResource.ssid, MAX_SSIDLEN);
        DAStrncpy(pass, gWiFiResource.cred, MAX_CREDLEN);
    }
}

OCStackResult initProvResource(bool isSecured)
{
    ESSetState(ES_STATE_INIT);
    gProvResource.lastErrCode = ES_ERRCODE_NO_ERROR;
    gProvResource.vdErrCode = ERRCI_NO_ERROR;

    OCStackResult res = OC_STACK_ERROR;
    if (isSecured)
    {
        res = OCCreateResource(&gProvResource.handle, OC_RSRVD_ES_RES_TYPE_PROV,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_PROV, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&gProvResource.handle, OC_RSRVD_ES_RES_TYPE_PROV,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_PROV, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }
    if(res)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created Prov resource with result: %s", getResult(res));
        return res;
    }

    res = OCBindResourceTypeToResource(gProvResource.handle, OC_RSRVD_RESOURCE_TYPE_COLLECTION);
    if(res)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Binding Resource type with result: %s", getResult(res));
        return res;
    }

    res = OCBindResourceInterfaceToResource(gProvResource.handle, OC_RSRVD_INTERFACE_LL);
    if(res)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created Prov resource with result: %s", getResult(res));
        return res;
    }
    res = OCBindResourceInterfaceToResource(gProvResource.handle, OC_RSRVD_INTERFACE_BATCH);
    if(res)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created Prov resource with result: %s", getResult(res));
        return res;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created Prov resource with result: %s", getResult(res));
    return res;
}

OCStackResult initWiFiResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    gWiFiResource.supportedFreq = WiFi_BOTH;
    gWiFiResource.supportedMode[0] = WiFi_11A;
    gWiFiResource.supportedMode[1] = WiFi_11B;
    gWiFiResource.supportedMode[2] = WiFi_11G;
    gWiFiResource.supportedMode[3] = WiFi_11N;
    gWiFiResource.supportedMode[4] = WiFi_11AC;
    gWiFiResource.numMode = 5;
    gWiFiResource.authType = NONE_AUTH;
    gWiFiResource.encType = NONE_ENC;
    memset(gWiFiResource.ssid, 0,sizeof(char)*MAX_SSIDLEN);
    memset(gWiFiResource.cred, 0,sizeof(char)*MAX_CREDLEN);
    gWiFiResource.discoveryChannel = 1;

    if (isSecured)
    {
        res = OCCreateResource(&gWiFiResource.handle, OC_RSRVD_ES_RES_TYPE_WIFI,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_WIFI, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&gWiFiResource.handle, OC_RSRVD_ES_RES_TYPE_WIFI,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_WIFI, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    OICApplyCurrentAP();

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created WiFi resource with result: %s", getResult(res));
    return res;

}

static void initCloudResourceData(CloudResource* pCloudResource)
{
    if( pCloudResource == NULL )
        return ;

    memset(pCloudResource->authCode,     0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->authProvider, 0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->ciServer,     0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->serverID,     0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->clientID,     0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->accesstoken,  0,sizeof(char)*OIC_STRING_MAX_VALUE);
    pCloudResource->actokenType = 0;
    memset(pCloudResource->refreshtoken, 0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudResource->uid,          0,sizeof(char)*OIC_STRING_MAX_VALUE);

}

void initESCloudProvData( ESCloudProvData* pCloudData)
{
    if( pCloudData == NULL )
        return;

    memset(pCloudData->authCode,     0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->authProvider, 0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->ciServer,     0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->hostName,     0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->ip,           0, sizeof(char)*IP_PORT);
    memset(pCloudData->port,         0, sizeof(char)*IP_PORT);
    memset(pCloudData->serverID,     0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->clientID,     0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->accesstoken,  0, sizeof(char)*OIC_STRING_MAX_VALUE);
    pCloudData->actokenType = 0;
    memset(pCloudData->refreshtoken, 0, sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(pCloudData->uid,          0, sizeof(char)*OIC_STRING_MAX_VALUE);
}

void cloneESCloudProvData( ESCloudProvData* des, ESCloudProvData* src )
{
    if( des == NULL || src == NULL )
        return;

    memcpy(des->authCode,     src->authCode,    sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->authProvider, src->authProvider,sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->ciServer,     src->ciServer,    sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->hostName,     src->hostName,    sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->ip,           src->ip,          sizeof(char)*IP_PORT);
    memcpy(des->port,         src->port,        sizeof(char)*IP_PORT);
    memcpy(des->serverID,     src->serverID,    sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->clientID,     src->clientID,    sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->accesstoken,  src->accesstoken, sizeof(char)*OIC_STRING_MAX_VALUE);
    des->actokenType = src->actokenType;
    memcpy(des->refreshtoken, src->refreshtoken,sizeof(char)*OIC_STRING_MAX_VALUE);
    memcpy(des->uid,          src->uid,         sizeof(char)*OIC_STRING_MAX_VALUE);
}

static void initESWiFiProvData(ESWiFiProvData* pWiFiData)
{
    if( pWiFiData == NULL )
        return;

    memset(pWiFiData->ssid,  0, sizeof(char)*MAX_SSIDLEN);
    memset(pWiFiData->pwd,   0, sizeof(char)*MAX_CREDLEN);
    pWiFiData->authtype = -1;
    pWiFiData->enctype = -1;
    pWiFiData->discoveryChannel = -1;
}

void setSSIDinWiFiResource(const char* ssid)
{
    if( ssid == NULL )
    {
        memset(gWiFiResource.ssid, 0, sizeof(char)*MAX_SSIDLEN);
        memset(gWiFiResource.cred, 0, sizeof(char)*MAX_CREDLEN);
        gWiFiResource.authType = NONE_AUTH;
        gWiFiResource.encType = NONE_ENC;
        gWiFiResource.discoveryChannel = 0;
    }
    else if( strcmp(ssid, gWiFiData.ssid) == 0 )
    {
        DAStrncpy(gWiFiResource.ssid, gWiFiData.ssid, sizeof(char)*MAX_SSIDLEN);
        DAStrncpy(gWiFiResource.cred, gWiFiData.pwd, sizeof(char)*MAX_CREDLEN);
        gWiFiResource.authType = gWiFiData.authtype;
        gWiFiResource.encType = gWiFiData.enctype;
        gWiFiResource.discoveryChannel = gWiFiData.discoveryChannel;
    }
    else
    {
        DAStrncpy(gWiFiResource.ssid, ssid, sizeof(char)*MAX_SSIDLEN);
        memset(gWiFiResource.cred,    0,sizeof(char)*MAX_CREDLEN);
        gWiFiResource.authType = NONE_AUTH;
        gWiFiResource.encType = NONE_ENC;
        gWiFiResource.discoveryChannel = 1;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Connected SSID=%s", ssid);
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.ssid=%s", gWiFiData.ssid);
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiResource.ssid=%s", gWiFiResource.ssid);
}

OCStackResult initCloudServerResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    initCloudResourceData(&gCloudResource);

    if (isSecured)
    {
        res = OCCreateResource(&gCloudResource.handle, OC_RSRVD_ES_RES_TYPE_CLOUDSERVER,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_CLOUDSERVER, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&gCloudResource.handle, OC_RSRVD_ES_RES_TYPE_CLOUDSERVER,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_CLOUDSERVER, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created CloudServer resource with result: %s", getResult(res));
    return res;

}

OCStackResult initDevConfResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    memset(gDevConfResource.devName,       0,sizeof(char)*MAX_DEVICELEN);
    memset(gDevConfResource.country,       0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(gDevConfResource.language,      0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(gDevConfResource.deviceType,    0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(gDevConfResource.deviceSubType, 0,sizeof(char)*OIC_STRING_MAX_VALUE);
    memset(gDevConfResource.datetime,      0,sizeof(char)*OIC_STRING_MAX_VALUE);

    if (isSecured)
    {
        res = OCCreateResource(&gDevConfResource.handle, OC_RSRVD_ES_RES_TYPE_DEVCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_DEVCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&gDevConfResource.handle, OC_RSRVD_ES_RES_TYPE_DEVCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_DEVCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created DevConf resource with result: %s", getResult(res));
    return res;

}

void updateProvResource(OCEntityHandlerRequest* ehRequest, OCRepPayload* input)
{
    THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "gProvResource.status %d", GetEnrolleeState());

    if(ehRequest->query)
    {
        if(strstr(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))
        {
        // When Provisioning resource has a POST with BatchInterface
            updateCloudResource(input);
            updateWiFiResource(input);
            updateDevConfResource(input);
        }
    }
}

void updateWiFiResource(OCRepPayload* input)
{
    initESWiFiProvData(&gWiFiData);

    char* ssid = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_SSID, &ssid) == false || ssid == NULL)
        return ;

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Current AP SSID: %s", gWiFiResource.ssid);
    if( strcmp(ssid, gWiFiResource.ssid) == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Already connected SSID(%s).", ssid);
        free(ssid);
        return ;
    }

    DAStrncpy(gWiFiData.ssid, ssid, sizeof(gWiFiData.ssid));
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.ssid : %s", gWiFiData.ssid);

    char* cred = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_CRED, &cred))
    {
        DAStrncpy(gWiFiData.pwd, cred, sizeof(gWiFiData.pwd));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.pwd %s", gWiFiData.pwd);
    }

    int64_t authType = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_AUTHTYPE, &authType))
    {
        gWiFiData.authtype = authType;
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.authtype %u", gWiFiData.authtype);
    }

    int64_t encType = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_ENCTYPE, &encType))
    {
        gWiFiData.enctype = encType;
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.enctype %u", gWiFiData.enctype);
    }

    int64_t channel = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_VENDOR_DISCOVERYCHANNEL, &channel))
    {
        gWiFiData.discoveryChannel = channel;
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gWiFiData.discoveryChannel %u", gWiFiData.discoveryChannel);
    }

    if(ssid || cred || authType!= -1 || encType != -1)
    {
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Send WiFiRsrc Callback To ES");
        PROFILING_TIME("WiFi Provisioning Start.");

        // TODO : Need to check appropriateness of gWiFiData
        if(gWifiRsrcEvtCb != NULL)
        {
            gWifiRsrcEvtCb(ES_OK, &gWiFiData);
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "gWifiRsrcEvtCb is NULL");
        }
    }

    /*! Added by DA for memory Leak fix
     */
    if(ssid) free(ssid);
    if(cred) free(cred);
}

bool updateCloudResource(OCRepPayload* input)
{
    bool res = false;
    initCloudResourceData(&gCloudResource);
    initESCloudProvData(&gCloudData);

    char *authCode = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_AUTHCODE, &authCode))
    {
        DAStrncpy(gCloudResource.authCode, authCode, sizeof(gCloudResource.authCode));
        DAStrncpy(gCloudData.authCode, authCode, sizeof(gCloudData.authCode));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.authCode %s", gCloudResource.authCode);
    }

    char *accesstoken = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN, &accesstoken))
    {
        DAStrncpy(gCloudResource.accesstoken, accesstoken, sizeof(gCloudResource.accesstoken));
        DAStrncpy(gCloudData.accesstoken, accesstoken, sizeof(gCloudData.accesstoken));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.accesstoken %s", gCloudResource.accesstoken);
    }

    int accesstokenType = NULL;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, &accesstokenType))
    {
        gCloudResource.actokenType = accesstokenType;
        gCloudData.actokenType = accesstokenType;
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.actokenType %d", gCloudResource.actokenType);
    }

    char *refreshtoken = NULL;
    if (OCRepPayloadGetPropString(input, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, &refreshtoken))
    {
        DAStrncpy(gCloudResource.refreshtoken, refreshtoken, sizeof(gCloudResource.refreshtoken));
        DAStrncpy(gCloudData.refreshtoken, refreshtoken, sizeof(gCloudData.refreshtoken));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.refreshtoken %s", gCloudResource.refreshtoken);
    }

    char *uid = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_VENDOR_USER_ID, &uid))
    {
        DAStrncpy(gCloudResource.uid, uid, sizeof(gCloudResource.uid));
        DAStrncpy(gCloudData.uid, uid, sizeof(gCloudData.uid));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.uid %s", gCloudResource.uid);
    }

    char *authProvider = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_AUTHPROVIDER, &authProvider))
    {
        DAStrncpy(gCloudResource.authProvider, authProvider, sizeof(gCloudResource.authProvider));
        DAStrncpy(gCloudData.authProvider, authProvider, sizeof(gCloudData.authProvider));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.authProvider %s", gCloudResource.authProvider);
    }

    char *ciServer = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_CISERVER, &ciServer))
    {
        if( CheckCIServer_IPv4(ciServer, &gCloudData) == 1 )
        {
            DAStrncpy(gCloudResource.ciServer, ciServer, sizeof(gCloudResource.ciServer));
            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.ciServer %s", gCloudResource.ciServer);
        }
        else
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "CheckCIServer_IPv4 is failed.");
            gProvResource.lastErrCode = ER_ERRCODE_INVALID_PROV_PAYLOAD;
            gProvResource.vdErrCode = ERRCI_NO_ERROR;
        }
    }

    char *serverID = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_SERVERID, &serverID))
    {
        DAStrncpy(gCloudResource.serverID, serverID, sizeof(gCloudResource.serverID));
        DAStrncpy(gCloudData.serverID, serverID, sizeof(gCloudData.serverID));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.serverID %s", gCloudResource.serverID);
    }

    char *clientID = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_VENDOR_CLIENTID, &clientID))
    {
        DAStrncpy(gCloudResource.clientID, clientID, sizeof(gCloudResource.clientID));
        DAStrncpy(gCloudData.clientID, clientID, sizeof(gCloudData.clientID));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gCloudResource.clientID %s", gCloudResource.clientID);
    }

    if( (gCloudData.authCode[0] || (gCloudData.accesstoken[0] && gCloudData.refreshtoken[0])) &&
         gCloudData.authProvider[0] && gCloudData.ciServer[0] )
    {
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Send CloudRsrc Callback To ES");
        PROFILING_TIME("Cloud Provisioning Start.");

        if(gCloudRsrcEvtCb != NULL)
        {
            ESSetState(ES_STATE_INIT);
            gProvResource.lastErrCode = ERRCI_NO_ERROR;
            gProvResource.vdErrCode = ERRCI_NO_ERROR;

            gCloudRsrcEvtCb(ES_OK, &gCloudData);
            res = true;
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "gCloudRsrcEvtCb is NULL");
            gProvResource.lastErrCode = ER_ERRCODE_SYSTEM_ERROR;
            gProvResource.vdErrCode = ERRCI_NO_ERROR;
        }
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Cloud Provisioning Data is invalid: authCode=%s authProvider=%s ciServer=%s", gCloudData.authCode, gCloudData.authProvider, gCloudData.ciServer);
    }

    if(authCode)
        DAFree(authCode);

    if(accesstoken)
        DAFree(accesstoken);

    /*! Added by DA for leak fix*/
    if(refreshtoken)
           DAFree(refreshtoken);
    if(uid)
        DAFree(uid);

    if(authProvider)
        DAFree(authProvider);

    if(ciServer)
        DAFree(ciServer);

    if(serverID)
        DAFree(serverID);

    if(clientID)
        DAFree(clientID);

    return res;
}

void updateDevConfResource(OCRepPayload* input)
{
    char *country = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_COUNTRY, &country))
    {
        DAStrncpy(gDevConfResource.country, country, sizeof(gDevConfResource.country));
        DAStrncpy(gDevConfData.country, country, sizeof(gDevConfData.country));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gDevConfResource.country %s", gDevConfResource.country);
    }

    char *language = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_LANGUAGE, &language))
    {
        DAStrncpy(gDevConfResource.language, language, sizeof(gDevConfResource.language));
        DAStrncpy(gDevConfData.language, language, sizeof(gDevConfData.language));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gDevConfResource.language %s", gDevConfResource.language);
    }

    char *datetime = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_VENDOR_UTC_DATE_TIME, &datetime))
    {
        DAStrncpy(gDevConfResource.datetime, datetime, sizeof(gDevConfResource.datetime));
        DAStrncpy(gDevConfData.datetime, datetime, sizeof(gDevConfData.datetime));
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "gDevConfResource.datetime %s", gDevConfResource.datetime);

#if 1 //jun[time]
        //Added by DA to set time in appliance yyyy-mm-ddThh-mm-ss
        char chYear[5], chMonth[3], chDay[3], chHour[3], chMin[3], chSec[3];
        things_time_info stTime;
        memset(&stTime, 0, sizeof(things_time_info));

        int itr = 0;
        for( ; itr<4; itr++){
        	chYear[itr] = gDevConfResource.datetime[itr];
        }
        chYear[itr] = '\0';
        stTime.year = (atoi(chYear) - 1900);

        chMonth[0] = gDevConfResource.datetime[++itr];
        chMonth[1] = gDevConfResource.datetime[++itr];
        chMonth[2] = '\0';
        stTime.month = (atoi(chMonth) -1 );

        itr++;
        chDay[0] = gDevConfResource.datetime[++itr];
        chDay[1] = gDevConfResource.datetime[++itr];
        chDay[2] = '\0';
        stTime.day = atoi(chDay);

        itr++;
        chHour[0] = gDevConfResource.datetime[++itr];
        chHour[1] = gDevConfResource.datetime[++itr];
        chHour[2] = '\0';
        stTime.hour = atoi(chHour);

        itr++;
        chMin[0] = gDevConfResource.datetime[++itr];
        chMin[1] = gDevConfResource.datetime[++itr];
        chMin[2] = '\0';
        stTime.minute = atoi(chMin);

        itr++;
        chSec[0] = gDevConfResource.datetime[++itr];
        chSec[1] = gDevConfResource.datetime[++itr];
        chSec[2] = '\0';
        stTime.second = atoi(chSec);

        unsigned int epTime;
        if( THINGS_SUCCESS == things_time_info_to_utc_time( &stTime,  &epTime)){
        	things_time_set( epTime, 0);
        }
        #endif
    }

    if(country || language || datetime)
    {
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Send DevConfRsrc Callback To ES");

        // TODO : Need to check appropriateness of gDevConfData
        if(gDevConfRsrcEvtCb != NULL)
        {
            gDevConfRsrcEvtCb(ES_OK, &gDevConfData);
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "gDevConfRsrcEvtCb is NULL");
        }
    }

    /*! Added by DA for memory Leak fix
     */
    if(country) free(country);
    if(language) free(language);
    if(datetime) free(datetime);
}

OCRepPayload* constructResponseOfWiFi(const char* interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "constructResponse wifi res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_WIFI);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {   // Temporary Code.
        char *interfaces[1] = {NULL,};
        char *resourceTypes[1] = {NULL,};
        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};

        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate repPayload");
            goto GOTO_ERROR;
        }

        // Swap
        tempPayload = payload;
        payload = repPayload;

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfacesDimensions);

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_WIFI);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resourceTypes, resourceTypesDimensions);
        /*! Added by DA for memory Leak fix
         */
        if(interfaces[0]) free(interfaces[0]);
        if(resourceTypes[0]) free(resourceTypes[0]);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_WIFI);
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {gWiFiResource.numMode, 0, 0};
    int64_t *modes_64 = (int64_t *)malloc(gWiFiResource.numMode * sizeof(int64_t));
    for(int i = 0 ; i < gWiFiResource.numMode ; ++i)
        modes_64[i] = gWiFiResource.supportedMode[i];

    OCRepPayloadSetIntArray(payload, OC_RSRVD_ES_SUPPORTEDWIFIMODE, (int64_t *)modes_64, dimensions);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_SUPPORTEDWIFIFREQ, gWiFiResource.supportedFreq);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_SSID, gWiFiResource.ssid);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_CRED, gWiFiResource.cred);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_AUTHTYPE, (int) gWiFiResource.authType);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_ENCTYPE, (int) gWiFiResource.encType);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_VENDOR_DISCOVERYCHANNEL,
                                                (int) gWiFiResource.discoveryChannel);

    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
        OCPayloadDestroy((OCPayload*)repPayload);
        repPayload = NULL;
        tempPayload = NULL;
    }
    /*! Added by DA for memory Leak fix
     */
    if(tempPayload != NULL) OCPayloadDestroy((OCPayload*)tempPayload);
    /*! Added by DA for memory Leak fix
     */
    if(modes_64 != NULL) free(modes_64);

    return payload;

GOTO_ERROR:
    if( payload != NULL )
    {
        OCPayloadDestroy((OCPayload*)payload);
        payload = NULL;
    }

    return NULL;
}

OCRepPayload* constructResponseOfCloud(const char* interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    THINGS_LOG(THINGS_INFO, ES_RH_TAG, "constructResponse cloud res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_CLOUDSERVER);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {   // Temporary Code.
        char *interfaces[1] = {NULL,};
        char *resourceTypes[1] = {NULL,};
        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};

        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate repPayload");
            goto GOTO_ERROR;
        }

        // Swap
        tempPayload = payload;
        payload = repPayload;

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfacesDimensions);

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_CLOUDSERVER);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resourceTypes, resourceTypesDimensions);
        /*! Added by DA for memory Leak fix
         */
       if(interfaces[0]) free(interfaces[0]);
       if(resourceTypes[0])  free(resourceTypes[0]);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_CLOUDSERVER);
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_AUTHCODE, gCloudResource.authCode);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_AUTHPROVIDER, gCloudResource.authProvider);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_CISERVER, gCloudResource.ciServer);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_SERVERID, gCloudResource.serverID);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_CLIENTID, gCloudResource.clientID);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN, gCloudResource.accesstoken);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, gCloudResource.actokenType);
    OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, gCloudResource.refreshtoken);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_USER_ID, gCloudResource.uid);

    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
        OCPayloadDestroy((OCPayload*)repPayload);
        repPayload = NULL;
        tempPayload = NULL;
    }
    /*! Added by DA for memory Leak fix
     */
    if(tempPayload != NULL) OCPayloadDestroy((OCPayload*)tempPayload);

    return payload;

GOTO_ERROR:
    if( payload != NULL )
    {
        OCPayloadDestroy((OCPayload*)payload);
        payload = NULL;
    }

    return NULL;
}

OCRepPayload* constructResponseOfDevConf(const char* interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    THINGS_LOG(THINGS_INFO, ES_RH_TAG, "constructResponse devConf res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_DEVCONF);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {   // Temporary Code.
        char *interfaces[1] = {NULL,};
        char *resourceTypes[1] = {NULL,};
        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};

        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate repPayload");
            goto GOTO_ERROR;
        }

        // Swap
        tempPayload = payload;
        payload = repPayload;

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfacesDimensions);

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_DEVCONF);
        OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resourceTypes, resourceTypesDimensions);
  /*! Added by DA for memory Leak fix
     */
       if(interfaces[0]) free(interfaces[0]);
       if(resourceTypes[0]) free(resourceTypes[0]);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_DEVCONF);
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_DEVNAME, gDevConfResource.devName);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_LANGUAGE, gDevConfResource.language);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_COUNTRY, gDevConfResource.country);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_DEVTYPE, gDevConfResource.deviceType);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_DEVSUBTYPE,
                                                    gDevConfResource.deviceSubType);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_UTC_DATE_TIME, gDevConfResource.datetime);

    if(interface != NULL && !strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
        OCPayloadDestroy((OCPayload*)repPayload);
        repPayload = NULL;
        tempPayload = NULL;
    }
    /*! Added by DA for memory Leak fix
     */
    if(tempPayload != NULL) OCPayloadDestroy((OCPayload*)tempPayload);

    return payload;

GOTO_ERROR:
    if( payload != NULL )
    {
        OCPayloadDestroy((OCPayload*)payload);
        payload = NULL;
    }

    return NULL;
}

OCRepPayload* constructResponseOfProv(OCEntityHandlerRequest *ehRequest)
{
    int childResCnt = 0;
    const OCRepPayload *arrayPayload[3] = {NULL, };
    OCRepPayload* payload = NULL;

    if( gWiFiResource.handle == NULL || gDevConfResource.handle == NULL || gCloudResource.handle == NULL )
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "mandatory Resource handle is NULL.(WiFi=0x%X, DevConf=0x%X, Cloud=0x%X)", gWiFiResource.handle, gDevConfResource.handle, gCloudResource.handle );
        return NULL;
    }

    if ( (payload= OCRepPayloadCreate()) == NULL )
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    // Requested interface is Link list interface
    if(!ehRequest->query ||
        (ehRequest->query && !strcmp(ehRequest->query, "")) ||
        (ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_LL)) ||
        (ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_DEFAULT)))
    {
        if( (arrayPayload[childResCnt] = makeRepPayload(gWiFiResource.handle, &ehRequest->devAddr)) == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "It's failed making payload of WiFiResource-Representation");
            goto GOTO_FAILED;
        }
        childResCnt++;

        if( (arrayPayload[childResCnt] = makeRepPayload(gDevConfResource.handle, &ehRequest->devAddr)) == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "It's failed making payload of DevConfResource-Representation");
            goto GOTO_FAILED;
        }
        childResCnt++;

        if( (arrayPayload[childResCnt] = makeRepPayload(gCloudResource.handle, &ehRequest->devAddr)) == NULL )
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "It's failed making payload of CloudResource-Representation");
            goto GOTO_FAILED;
        }
        childResCnt++;

        size_t dimensions[MAX_REP_ARRAY_DEPTH] = {childResCnt, 0, 0};

        if( !ehRequest->query || !strcmp(ehRequest->query, "") ||
                CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_LL) == false )
        {
            THINGS_LOG(THINGS_INFO, ES_RH_TAG, "constructResponse prov res");
            OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_PROV);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_LL);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_BATCH);
            OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_PROV);
            OCRepPayloadAddResourceType(payload, OC_RSRVD_RESOURCE_TYPE_COLLECTION);

            OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_PROVSTATUS, GetEnrolleeState());
            OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_LAST_ERRORCODE, gProvResource.lastErrCode);
            OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_VENDOR_ERRORCODE, gProvResource.vdErrCode);
        }

        OCRepPayloadSetPropObjectArray(payload, OC_RSRVD_ES_LINKS, arrayPayload, dimensions);

        for(int i=0; i < childResCnt; i++)
            OCPayloadDestroy((OCPayload*)arrayPayload[i]);

    } else if (
        ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))

    {
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "constructResponse prov res");
        OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_PROV);

        OCRepPayload* repPayload = NULL;
        char *interfaces[3] = {NULL,};
        char *resourceTypes[2] = {NULL,};
        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {3, 0, 0};
        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {2, 0, 0};

        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate repPayload");
            goto GOTO_FAILED;
        }

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        interfaces[1] = OICStrdup(OC_RSRVD_INTERFACE_LL);
        interfaces[2] = OICStrdup(OC_RSRVD_INTERFACE_BATCH);
        OCRepPayloadSetStringArray(repPayload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfacesDimensions);

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_PROV);
        resourceTypes[1] = OICStrdup(OC_RSRVD_RESOURCE_TYPE_COLLECTION);
        OCRepPayloadSetStringArray(repPayload, OC_RSRVD_RESOURCE_TYPE, (const char **)resourceTypes, resourceTypesDimensions);

        OCRepPayloadSetPropInt(repPayload, OC_RSRVD_ES_PROVSTATUS, GetEnrolleeState());
        OCRepPayloadSetPropInt(repPayload, OC_RSRVD_ES_LAST_ERRORCODE, gProvResource.lastErrCode);
        OCRepPayloadSetPropInt(repPayload, OC_RSRVD_ES_VENDOR_ERRORCODE, gProvResource.vdErrCode);

        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
        OCPayloadDestroy((OCPayload*)repPayload);
        /*! Added by DA for memory Leak fix
         */
       if(interfaces[0]) free(interfaces[0]);
       if(interfaces[1]) free(interfaces[1]);
       if(interfaces[2]) free(interfaces[2]);
       if(resourceTypes[0]) free(resourceTypes[0]);
       if(resourceTypes[1]) free(resourceTypes[1]);
    }

    if(ehRequest->query)
    {
        if(CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))
        {   // When Provisioning resource has a GET with BatchInterface
            OCRepPayload* head = payload;

            payload->next = constructResponseOfWiFi(OC_RSRVD_INTERFACE_BATCH);
            if(payload->next == NULL)
            {
                goto GOTO_FAILED;
            }

            payload = payload->next;
            payload->next = constructResponseOfCloud(OC_RSRVD_INTERFACE_BATCH);
            if(payload->next == NULL)
            {
                payload = head;
                goto GOTO_FAILED;
            }

            payload = payload->next;
            payload->next = constructResponseOfDevConf(OC_RSRVD_INTERFACE_BATCH);
            if(payload->next == NULL)
            {
                payload = head;
                goto GOTO_FAILED;
            }

            payload = head;
        }
    }

    return payload;

GOTO_FAILED:
    for(int i=0; i < childResCnt; i++)
        OCPayloadDestroy((OCPayload*)arrayPayload[i]);

    deleteChildPayload(payload);

    return NULL;
}

OCStackResult CreateEasySetupResources(bool isSecured, ESResourceMask resourceMask)
{
    OCStackResult res = OC_STACK_ERROR;
    bool maskFlag = false;

    res = initProvResource(isSecured);
    if(res != OC_STACK_OK)
    {
        // TODO: destroy logic will be added
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "initProvResource result: %s", getResult(res));

        return res;
    }

    if((resourceMask & ES_WIFI_RESOURCE) == ES_WIFI_RESOURCE)
    {
        maskFlag = true;
        res = initWiFiResource(isSecured);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "initWiFiResource result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(gProvResource.handle, gWiFiResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Bind WiFiResource result: %s", getResult(res));
            return res;
        }

    }

    if((resourceMask & ES_CLOUD_RESOURCE) == ES_CLOUD_RESOURCE)
    {
        maskFlag = true;
        res = initCloudServerResource(isSecured);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "initCloudResource result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(gProvResource.handle, gCloudResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Bind CloudResource result: %s", getResult(res));
            return res;
        }
    }

    if((resourceMask & ES_DEVCONF_RESOURCE) == ES_DEVCONF_RESOURCE)
    {
        maskFlag = true;
        res = initDevConfResource(isSecured);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "initDevConf result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(gProvResource.handle, gDevConfResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Bind DevConfResource result: %s", getResult(res));
            return res;
        }
    }


    if(maskFlag == false)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid ResourceMask");
        return OC_STACK_ERROR;

    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Created all resources with result: %s", getResult(res));

    return res;
}

OCStackResult DeleteProvisioningResource()
{
    OCStackResult res = OCDeleteResource(gProvResource.handle);
    if (res != OC_STACK_OK)
    {
        THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "Deleting Prov resource error with result: %s", getResult(res));
    }

    return res;
}

OCStackResult DeleteEasySetupResources()
{
    OCStackResult res = OC_STACK_ERROR;
    if (gWiFiResource.handle != NULL)
    {
        res = OCUnBindResource(gProvResource.handle, gWiFiResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Unbind WiFiResource error with result: %s", getResult(res));
        }
    }
    if (gCloudResource.handle != NULL)
    {
        res = OCUnBindResource(gProvResource.handle, gCloudResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Unbind CloudResource error with result: %s", getResult(res));
        }
    }
    if (gDevConfResource.handle != NULL)
    {
        res = OCUnBindResource(gProvResource.handle, gDevConfResource.handle);
        if(res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Unbind DevConfResource error with result: %s", getResult(res));
        }
    }

    if (gWiFiResource.handle != NULL)
    {
        res = OCDeleteResource(gWiFiResource.handle);
        if (res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Deleting WiFi resource error with result: %s", getResult(res));
        }
    }

    if(gCloudResource.handle != NULL)
    {
        res = OCDeleteResource(gCloudResource.handle);
        if (res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Deleting CloudServer resource error with result: %s", getResult(res));
        }
    }

    if(gDevConfResource.handle != NULL)
    {
        res = OCDeleteResource(gDevConfResource.handle);
        if (res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Deleting DevConf resource error with result: %s", getResult(res));
        }
    }

    if(gProvResource.handle != NULL)
    {
        res = OCDeleteResource(gProvResource.handle);
        if (res != OC_STACK_OK)
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Deleting Prov resource error with result: %s", getResult(res));
        }
    }

    return res;
}

OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if (!ehRequest)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Request is Null");
        return ehResult;
    }
    if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Incoming payload not a representation");
        return ehResult;
    }

    OCRepPayload *getResp = NULL;

    if(ehRequest->resource == gProvResource.handle)
        getResp = constructResponseOfProv(ehRequest);
    else if(ehRequest->resource == gWiFiResource.handle)
        getResp = constructResponseOfWiFi(OC_RSRVD_INTERFACE_DEFAULT);
    else if(ehRequest->resource == gCloudResource.handle)
        getResp = constructResponseOfCloud(OC_RSRVD_INTERFACE_DEFAULT);
    else if(ehRequest->resource == gDevConfResource.handle)
        getResp = constructResponseOfDevConf(OC_RSRVD_INTERFACE_DEFAULT);

    if (!getResp)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "constructResponse failed");
        return OC_EH_ERROR;
    }

    *payload = getResp;
    ehResult = OC_EH_OK;
    //ehResult = OC_EH_CONTENT;

    return ehResult;
}

OCEntityHandlerResult ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "ProcessPostRequest enter");
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Incoming payload not a representation");
        return ehResult;
    }

    OCRepPayload* input = (OCRepPayload*) (ehRequest->payload);
    if (!input)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to parse");
        return ehResult;
    }

    // TBD : Discuss about triggering flag (to be existed or not)
    // ES_PS_PROVISIONING_COMPLETED state indicates that already provisioning is completed.
    // A new request for provisioning means overriding existing network provisioning information.

    if(ehRequest->resource == gProvResource.handle)
        updateProvResource(ehRequest, input);
    else if(ehRequest->resource == gWiFiResource.handle)
        updateWiFiResource(input);
    else if(ehRequest->resource == gCloudResource.handle)
        updateCloudResource(input);
    else if(ehRequest->resource == gDevConfResource.handle)
        updateDevConfResource(input);

    OCRepPayload *getResp = NULL;
    if(ehRequest->resource == gProvResource.handle)
        getResp = constructResponseOfProv(ehRequest);
    else if(ehRequest->resource == gWiFiResource.handle)
        getResp = constructResponseOfWiFi(OC_RSRVD_INTERFACE_DEFAULT);
    else if(ehRequest->resource == gCloudResource.handle)
        getResp = constructResponseOfCloud(OC_RSRVD_INTERFACE_DEFAULT);
    else if(ehRequest->resource == gDevConfResource.handle)
        getResp = constructResponseOfDevConf(OC_RSRVD_INTERFACE_DEFAULT);


    if (!getResp)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "constructResponse failed");
        return OC_EH_ERROR;
    }

    *payload = getResp;
    ehResult = OC_EH_OK;
    //ehResult = OC_EH_CHANGED;

    return ehResult;
}

OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest * ehRequest,
        OCRepPayload** payload)
{
    (void) ehRequest;
    (void) payload;
    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    return ehResult;
}

/**
 * This is the entity handler for the registered resource.
 * This is invoked by OCStack whenever it recevies a request for this resource.
 */
OCEntityHandlerResult OCEntityHandlerCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest* entityHandlerRequest, void *callback)
{
    (void) callback;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OCEntityHandlerResponse response;
    OCRepPayload* payload = NULL;

    THINGS_LOG(THINGS_DEBUG, ES_RH_TAG, "Called OCEntityHandler.");

    // Validate pointer
    if (!entityHandlerRequest)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }

    if ( flag & OC_REQUEST_FLAG )
    {
        if( OICGetResetMask(RST_EASY_SETUP_DISABLE) == true )
        {
            THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Easy Setup Disable.");
            ehRet = OC_EH_NOT_ACCEPTABLE;
        }
        else if (gProvResource.handle != NULL)
        {
            if (OC_REST_GET == entityHandlerRequest->method)
            {
                THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Received GET request");

                ehRet = ProcessGetRequest(entityHandlerRequest, &payload);

            }
//        else if (OC_REST_PUT == entityHandlerRequest->method)
//        {
//            THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Received PUT request");
//
//            //PUT request will be handled in the internal implementation
//            if (gProvResource.handle != NULL)
//            {
//                ehRet = ProcessPutRequest(entityHandlerRequest, &payload);
//            }
//            else
//            {
//                THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Cannot process put");
//                ehRet = OC_EH_ERROR;
//            }
//        }
            else if (OC_REST_POST == entityHandlerRequest->method)
            {
                THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Received OC_REST_POST from client");

                ehRet = ProcessPostRequest(entityHandlerRequest, &payload);
            }
            else
            {
                THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "Not yet Supported Request case(%d).", entityHandlerRequest->method);
                ehRet = OC_EH_ERROR;
            }
        }
        else
        {
            THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "EZ-Setup Resource not Registered Yet~!!!!!");
        }
       // By default it is ==> ehRet = OC_EH_ERROR;
        //

        // If the result isn't an error or forbidden, send response
        //if (!((ehRet == OC_EH_ERROR) || (ehRet == OC_EH_FORBIDDEN)))
        {
            THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "\t\tRespone : (%d) ", (int)ehRet);

            response.numSendVendorSpecificHeaderOptions = 0;
            memset(response.sendVendorSpecificHeaderOptions, 0,
                    sizeof(response.sendVendorSpecificHeaderOptions));
            memset(response.resourceUri, 0, sizeof(response.resourceUri));

            // Format the response.  Note this requires some info about the request
            response.requestHandle = entityHandlerRequest->requestHandle;
            response.resourceHandle = entityHandlerRequest->resource;
            response.ehResult = ehRet;
            response.payload = (OCPayload*) (payload);
            // Indicate that response is NOT in a persistent buffer
            response.persistentBufferFlag = 0;

            // Send the response
            if (OCDoResponse(&response) != OC_STACK_OK)
            {
                THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Error sending response");
                ehRet = OC_EH_ERROR;
            }
            /*! Added by DA for memory Leak fix
             */
            if(payload) OCPayloadDestroy((OCPayload*)payload);
            payload = NULL;
        }
        // else
        // {
        //     THINGS_LOG_D_ERROR(THINGS_ERROR, ES_RH_TAG, "Return value of Request Process has a error(%d).", ehRet);
        // }
    }

    if ( flag & OC_OBSERVE_FLAG )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Flag includes OC_OBSERVE_FLAG");
        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
        {
            THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Received OC_OBSERVE_REGISTER from client");
        }
        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
        {
            THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Received OC_OBSERVE_DEREGISTER from client");
        }
    }

    THINGS_LOG(THINGS_DEBUG, ES_RH_TAG, "Common Exit OCEntityHandler.");

    SetWiFiProvState(WIFI_RESPONSE);

    return ehRet;
}

OCStackResult ProvRSCNotifyAllObservers(void)
{
    OCStackResult ret = OCNotifyAllObservers(gProvResource.handle, OC_HIGH_QOS);

    switch(ret)
    {
    case OC_STACK_OK:
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Sent notification to Cloud Resource Observers");
        break;
    case OC_STACK_NO_OBSERVERS:
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "There is no exist registered Observer client.");
        break;
    default:
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Send notification Error : %d", ret);
        break;
    }

    ret = OCNotifyAllObservers(gCloudResource.handle, OC_HIGH_QOS);

    switch(ret)
    {
    case OC_STACK_OK:
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "Sent notification to Cloud Resource Observers");
        break;
    case OC_STACK_NO_OBSERVERS:
        THINGS_LOG(THINGS_INFO, ES_RH_TAG, "There is no exist registered Observer client.");
        break;
    default:
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Send notification Error : %d", ret);
        break;
    }

    return ret;
}

ESEnrolleeState GetEnrolleeState(void)
{
    ESEnrolleeState Status = ES_STATE_INIT;

    pthread_mutex_lock(&gSTATUSmutex);
    Status = gProvResource.status;
    pthread_mutex_unlock(&gSTATUSmutex);

    return Status;
}

OCStackResult SetDeviceProperty(ESDeviceProperty *deviceProperty)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetDeviceProperty IN");

    gWiFiResource.supportedFreq = (deviceProperty->WiFi).freq;
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "WiFi Freq : %d", gWiFiResource.supportedFreq);

    int modeIdx = 0;
    while((deviceProperty->WiFi).mode[modeIdx] != WiFi_EOF)
    {
        gWiFiResource.supportedMode[modeIdx] = (deviceProperty->WiFi).mode[modeIdx];
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "WiFi Mode : %d", gWiFiResource.supportedMode[modeIdx]);
        modeIdx ++;
    }
    gWiFiResource.numMode = modeIdx;

    DAStrncpy(gDevConfResource.devName, (deviceProperty->DevConf).deviceName, MAX_DEVICELEN);
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Device Name : %s", gDevConfResource.devName);

    DAStrncpy(gDevConfResource.deviceType, (deviceProperty->DevConf).deviceType, OIC_STRING_MAX_VALUE);
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Device deviceType : %s", gDevConfResource.deviceType);

    DAStrncpy(gDevConfResource.deviceSubType, (deviceProperty->DevConf).deviceSubType, OIC_STRING_MAX_VALUE);
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Device deviceSubType : %s", gDevConfResource.deviceSubType);

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetDeviceProperty OUT");
    return OC_STACK_OK;
}

OCStackResult SetEnrolleeState(ESEnrolleeState esState)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetEnrolleeState IN");

    pthread_mutex_lock(&gSTATUSmutex);
    OCStackResult result = OC_STACK_ERROR;
    ESEnrolleeState preState = gProvResource.status;

    if( gProvResource.lastErrCode == ER_ERRCODE_SYSTEM_ERROR )
    {
        gProvResource.status = ES_STATE_INIT;
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Please Check your System.(Status=%s , ErrCode=%s)", getProvStatus(gProvResource.status), getProvErrcode(gProvResource.lastErrCode));
        goto GOTO_OUT;
    }

    switch(esState)
    {
    case ES_STATE_REGISTERING_TO_CLOUD:
        if( preState != ES_STATE_CONNECTED_TO_ENROLLER && preState != esState )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", getProvStatus(esState), getProvStatus(gProvResource.status));
            goto GOTO_OUT;
        }
        break;
    case ES_STATE_REGISTERED_TO_CLOUD:
        if( preState != ES_STATE_REGISTERING_TO_CLOUD && preState != esState )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", getProvStatus(esState), getProvStatus(gProvResource.status));
            goto GOTO_OUT;
        }
        break;
    case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
        if( preState != ES_STATE_REGISTERED_TO_CLOUD && preState != ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD && preState != esState )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", getProvStatus(esState), getProvStatus(gProvResource.status));
            goto GOTO_OUT;
        }
        break;
    case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
        if( preState != ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD && preState != esState )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", getProvStatus(esState), getProvStatus(gProvResource.status));
            goto GOTO_OUT;
        }
        break;
    }

    if( gProvResource.status == esState )
    {
        result = OC_STACK_DUPLICATE_UUID;
    }
    else
    {
        gProvResource.status = esState;
        result = OC_STACK_OK;
    }

    THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "Enrollee Status : %s (%d)", getProvStatus(gProvResource.status), gProvResource.status);

GOTO_OUT:
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetEnrolleeState OUT");
    pthread_mutex_unlock(&gSTATUSmutex);
    return result;
}

OCStackResult SetCloudErrCode(CIErrorCode esErrCode)
{
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetCloudErrCode IN");

    gProvResource.vdErrCode = esErrCode;
    THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "Cloud ErrorCode : %d", gProvResource.vdErrCode);

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetCloudErrCode OUT");
    return OC_STACK_OK;
}

OCStackResult SetEnrolleeErrCode(ESErrorCode esErrCode)
{
    THINGS_LOG_D(THINGS_DEBUG,ES_RH_TAG, "SetEnrolleeErrCode IN");

    if( gProvResource.lastErrCode == ER_ERRCODE_SYSTEM_ERROR )
    {
        ESSetState(ES_STATE_INIT);
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Please Check your System.(Status=%s , ErrCode=%s)", getProvStatus(GetEnrolleeState()), getProvErrcode(gProvResource.lastErrCode));
        return OC_STACK_ERROR;
    }

    gProvResource.lastErrCode = esErrCode;
    THINGS_LOG_V(THINGS_INFO, ES_RH_TAG, "Enrollee ErrorCode : %s(%d)", getProvErrcode(gProvResource.lastErrCode) , gProvResource.lastErrCode);

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "SetEnrolleeErrCode OUT");
    return OC_STACK_OK;
}

const char *getResult(OCStackResult result)
{
    switch (result)
    {
        case OC_STACK_OK:
            return "OC_STACK_OK";
        case OC_STACK_INVALID_URI:
            return "OC_STACK_INVALID_URI";
        case OC_STACK_INVALID_QUERY:
            return "OC_STACK_INVALID_QUERY";
        case OC_STACK_INVALID_IP:
            return "OC_STACK_INVALID_IP";
        case OC_STACK_RESOURCE_CHANGED:
            return "OC_STACK_RESOURCE_CHANGED";
        case OC_STACK_INVALID_PORT:
            return "OC_STACK_INVALID_PORT";
        case OC_STACK_INVALID_CALLBACK:
            return "OC_STACK_INVALID_CALLBACK";
        case OC_STACK_INVALID_METHOD:
            return "OC_STACK_INVALID_METHOD";
        case OC_STACK_NO_MEMORY:
            return "OC_STACK_NO_MEMORY";
        case OC_STACK_COMM_ERROR:
            return "OC_STACK_COMM_ERROR";
        case OC_STACK_INVALID_PARAM:
            return "OC_STACK_INVALID_PARAM";
        case OC_STACK_NOTIMPL:
            return "OC_STACK_NOTIMPL";
        case OC_STACK_NO_RESOURCE:
            return "OC_STACK_NO_RESOURCE";
        case OC_STACK_RESOURCE_ERROR:
            return "OC_STACK_RESOURCE_ERROR";
        case OC_STACK_SLOW_RESOURCE:
            return "OC_STACK_SLOW_RESOURCE";
        case OC_STACK_NO_OBSERVERS:
            return "OC_STACK_NO_OBSERVERS";
        case OC_STACK_ERROR:
            return "OC_STACK_ERROR";
        case OC_STACK_UNAUTHORIZED_REQ:
            return "OC_STACK_UNAUTHORIZED_REQ";
        default:
            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Not Supported result value.(%d)", result);
            return "Not defined OCResult.";
    }
}

const char* getProvStatus(ESEnrolleeState status)
{
    switch (status)
    {
        case ES_STATE_INIT:
            return "ES_STATE_INIT";
        case ES_STATE_CONNECTING_TO_ENROLLER:
            return "ES_STATE_CONNECTING_TO_ENROLLER";
        case ES_STATE_CONNECTED_TO_ENROLLER:
            return "ES_STATE_CONNECTED_TO_ENROLLER";
        case ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER:
            return "ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER";
        case ES_STATE_REGISTERING_TO_CLOUD:
            return "ES_STATE_REGISTERING_TO_CLOUD";
        case ES_STATE_REGISTERED_TO_CLOUD:
            return "ES_STATE_REGISTERED_TO_CLOUD";
        case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD:
            return "ES_STATE_FAILED_TO_REGISTER_TO_CLOUD";
        case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
            return "ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD";
        case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
            return "ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD";
        case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
            return "ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD";
        case ES_STATE_LOGOUT_FROM_CLOUD:
            return "ES_STATE_LOGOUT_FROM_CLOUD";
        default:
            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Not Supported State value.(%d)", status);
            return "Not defined State";
    }
}

const char* getProvErrcode(ESErrorCode lastErrcode)
{
    switch (lastErrcode)
    {
        case ES_ERRCODE_NO_ERROR:
            return "ES_ERRCODE_NO_ERROR";
        case ES_ERRCODE_SSID_NOT_FOUND:
            return "ES_ERRCODE_SSID_NOT_FOUND";
        case ES_ERRCODE_PW_WRONG:
            return "ES_ERRCODE_PW_WRONG";
        case ES_ERRCODE_IP_NOT_ALLOCATED:
            return "ES_ERRCODE_IP_NOT_ALLOCATED";
        case ES_ERRCODE_NO_INTERNETCONNECTION:
            return "ES_ERRCODE_NO_INTERNETCONNECTION";
        case ES_ERRCODE_TIMEOUT:
            return "ES_ERRCODE_TIMEOUT";
        case ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER:
            return "ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER";
        case ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER:
            return "ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER";
        case ES_ERRCODE_INVALID_AUTHCODE:
            return "ES_ERRCODE_INVALID_AUTHCODE";
        case ES_ERRCODE_INVALID_ACCESSTOKEN:
            return "ES_ERRCODE_INVALID_ACCESSTOKEN";
        case ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN:
            return "ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN";
        case ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD:
            return "ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD";
        case ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD:
            return "ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD";
        case ER_ERRCODE_INVALID_PROV_PAYLOAD:
            return "ER_ERRCODE_INVALID_PROV_PAYLOAD";
        case ER_ERRCODE_SYSTEM_ERROR:
            return "ER_ERRCODE_SYSTEM_ERROR";
        case ES_ERRCODE_UNKNOWN:
            return "ES_ERRCODE_VENDOR_ERROR";
        default:
            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Not Supported Error value.(%d)", lastErrcode);
            return "Not Defined lastErrCode";
    }
}

/*********************************************************************
 *
 * Support CI server format.
 *
 * - IP:Port
 * - coaps+tcp://IP
 * - coaps+tcp://IP:Port
 * - coaps+tcp://HostName
 * - coaps+tcp://HostName:Port
 *
 *********************************************************************/
static int CheckCIServer_IPv4(char* ciServer, ESCloudProvData* cloudData)
{
    int length = 0;
    char* pContext = NULL;
    char* originCIServer = ciServer;
    char* port = NULL;
    struct in_addr ip;
    ip.s_addr = 0;

    if(ciServer == NULL || cloudData == NULL)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "ciServer(0x%X)/cloudData(0x%X) is NULL", ciServer, cloudData);
        return 0;
    }

    if( strlen(ciServer) == 0)
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "ciServer's length is 0");
        return 0;
    }

    if(strstr(ciServer, UNSUPPORT_COAP_TCP_HOST) != NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "%s is not support Type", UNSUPPORT_COAP_TCP_HOST);
        return -1;
    }

    // Port Number analysis.
    if( (pContext=strstr(ciServer, DEFAULT_COAP_TCP_HOST)) != NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "%s type format", DEFAULT_COAP_TCP_HOST);

        ciServer = pContext + strlen(DEFAULT_COAP_TCP_HOST);
        port = DEFAULT_COAP_PORT;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "[Port] ciServer = %s, port=%s", ciServer, port );

    if( (pContext=strchr(ciServer, ':')) != NULL )
    {
        if( *(pContext+1) == 0 || strlen(pContext+1) > 5 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Port Num is out of range.(%s)", pContext+1);
            return -1;
        }

        if( atoi(pContext+1) == 0 )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid Port Num.(Port is not number : %s)", pContext+1);
            return -1;
        }

        port = pContext+1;
        *pContext = 0;
    }

    if( port == NULL || (length = strlen(ciServer)) == 0 )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Unsupported CI-server format.(port-num is NULL)", originCIServer);
        return -1;
    }

    // ciServer analysis (IP/HostName)
    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "[IP] ciServer = %s", ciServer);

    if( inet_aton(ciServer, &ip) == 0 || ip.s_addr == 0 )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "ciServer is Host Name.(%s)", ciServer);
        DAStrncpy(cloudData->hostName, ciServer, sizeof(cloudData->hostName));
        memset(cloudData->ip, 0 , IP_PORT);
        DAStrncpy(cloudData->port, port, sizeof(cloudData->port));
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "ciServer is ip address.(%s)", ciServer);
        DAStrncpy(cloudData->ip, ciServer, sizeof(cloudData->ip));
        memset(cloudData->hostName, 0 , OIC_STRING_MAX_VALUE);
        DAStrncpy(cloudData->port, port, sizeof(cloudData->port));
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Host=%s, IP=%s, Port=%s", cloudData->hostName, cloudData->ip, cloudData->port );

    DAStrncpy(cloudData->ciServer, ciServer, sizeof(cloudData->ciServer));
    cloudData->ciServer[length] = ':';
    cloudData->ciServer[length+1] = 0;
    if( DAStrcat(cloudData->ciServer, OIC_STRING_MAX_VALUE, (const char*)cloudData->port) == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "DAStrcat is failed.");
        return -1;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "cloudData->ciServer = %s", cloudData->ciServer);

    return 1;
}

bool StartCloudProvisioning(void* cloud_info)
{
    bool res = false;
    int length = 0;
    char* host = NULL;
    char* CIServer = NULL;
    OICCloudInfo* CIinfo = (OICCloudInfo*)cloud_info;
    OCRepPayload* payload = NULL;

    if( CIinfo == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid arguments.");
        return false;
    }

    if( (payload = OCRepPayloadCreate()) == NULL)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return false;
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Domain = %s, IP = %s", CIinfo->domain, CIinfo->ip);

    if( CIinfo->domain )
    {
        length = strlen(DEFAULT_COAP_TCP_HOST) + strlen(CIinfo->domain) + 2;
        if( CIinfo->port != NULL )
            length += strlen(CIinfo->port);
        host = CIinfo->domain;
        if( (CIServer = (char*)DAMalloc(sizeof(char) * length)) == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Memory allocation is failed.");
            goto GOTO_OUT;
        }
        memset( CIServer, 0, sizeof(char) * length );
        DAStrcat( CIServer, length, DEFAULT_COAP_TCP_HOST );
    }
    else if( CIinfo->ip )
    {
        length = strlen(CIinfo->ip) + 2;
        if( CIinfo->port != NULL )
            length += strlen(CIinfo->port);
        host = CIinfo->ip;
        if( (CIServer = (char*)DAMalloc(sizeof(char) * length)) == NULL )
        {
            THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Memory allocation is failed.");
            goto GOTO_OUT;
        }
        memset( CIServer, 0, sizeof(char) * length );
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, ES_RH_TAG, "Invalid arguments.(domain = 0x%X, ip = 0x%X)", CIinfo->domain, CIinfo->ip);
        goto GOTO_OUT;
    }

    DAStrcat( CIServer, length, host );
    if( CIinfo->port != NULL && strlen(CIinfo->port) > 0 )
    {
        int LCIserver = strlen(CIServer);
        CIServer[LCIserver] = ':';
        CIServer[LCIserver+1] = NULL;
        DAStrcat( CIServer, length, CIinfo->port );
    }

    THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "CIServer = %s", CIServer);

    if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN, CIinfo->access_token) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_VENDOR_ACCESS_TOKEN, CIinfo->access_token);
        goto GOTO_OUT;
    }

    if( CIinfo->access_token_type != NULL )
    {
        if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, CIinfo->access_token_type) == false )
        {
            THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, CIinfo->access_token_type);
            goto GOTO_OUT;
        }
    }

    if( OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, CIinfo->refresh_token) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, CIinfo->refresh_token);
        goto GOTO_OUT;
    }

    if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_USER_ID, CIinfo->user_id) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_VENDOR_USER_ID, CIinfo->user_id);
        goto GOTO_OUT;
    }

    if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_AUTHPROVIDER, CIinfo->auth_provider) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_AUTHPROVIDER, CIinfo->auth_provider);
        goto GOTO_OUT;
    }

    if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_CISERVER, CIServer) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_CISERVER, CIServer);
        goto GOTO_OUT;
    }

    if( OCRepPayloadSetPropString(payload, OC_RSRVD_ES_VENDOR_CLIENTID, CIinfo->client_id) == false )
    {
        THINGS_LOG_D(THINGS_DEBUG, ES_RH_TAG, "Inserting \"%s\" property is failed.(value=\"%s\")", OC_RSRVD_ES_VENDOR_CLIENTID, CIinfo->client_id);
        goto GOTO_OUT;
    }

    res = updateCloudResource(payload);

GOTO_OUT:
    if( CIServer )
    {
        DAFree(CIServer);
        CIServer = NULL;
    }

    if( payload )
    {
        OCRepPayloadDestroy(payload);
        payload = NULL;
    }

    THINGS_LOG(THINGS_DEBUG, ES_RH_TAG, "Exit.");
    return res;
}

