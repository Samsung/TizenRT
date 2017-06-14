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

#include "ocpayload.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include "cautilinterface.h"

/**
 * @var ES_RH_TAG
 * @brief Logging tag for module name.
 */
#define ES_RH_TAG "ES_RH"
//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

/**
 * @var g_ESEasySetupResource
 * @brief Structure for holding the Provisioning status
 */
EasySetupResource g_ESEasySetupResource;
WiFiConfResource g_ESWiFiConfResource;
CoapCloudConfResource g_ESCoapCloudConfResource;
DevConfResource g_ESDevConfResource;

//-----------------------------------------------------------------------------
// Private internal function prototypes
//-----------------------------------------------------------------------------
OCEntityHandlerResult OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *ehRequest,
        void *callback);
OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
OCEntityHandlerResult ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload);
void updateEasySetupResource(OCEntityHandlerRequest* ehRequest, OCRepPayload* input);
void updateWiFiConfResource(OCRepPayload* input);
void updateCoapCloudConfResource(OCRepPayload* input);
void updateDevConfResource(OCRepPayload* input);
const char *getResult(OCStackResult result);

ESConnectRequestCB gConnectRequestEvtCb = NULL;
ESWiFiConfCB gWifiConfRsrcEvtCb = NULL;
ESCoapCloudConfCB gCoapCloudConfRsrcEvtCb = NULL;
ESDevConfCB gDevConfRsrcEvtCb = NULL;

ESReadUserdataCb gReadUserdataCb = NULL;
ESWriteUserdataCb gWriteUserdataCb = NULL;

bool CompareResourceInterface(char *from, char *iface)
{
    char *str = OICStrdup(from);
    char *ptr = strtok(str, ";");

    if(ptr == NULL)
    {
        OICFree(str);
        return false;
    }

    do
    {
        if(strstr(ptr, ".if."))
        {
            char *if_ptr = NULL;
            if_ptr = strtok(ptr, "=");
            if_ptr = strtok(NULL, "=");

            if(!strcmp(if_ptr, iface))
            {
                OICFree(str);
                return true;
            }
        }

    } while ((ptr = strtok(NULL, ";")));

    OICFree(str);
    return false;
}

ESResult SetCallbackForUserData(ESReadUserdataCb readCb, ESWriteUserdataCb writeCb)
{
    if(!readCb && !writeCb)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Both of callbacks for user data are null");
        return ES_ERROR;
    }
    gReadUserdataCb = readCb;
    gWriteUserdataCb = writeCb;
    return ES_OK;
}

void RegisterWifiRsrcEventCallBack(ESWiFiConfCB cb)
{
    gWifiConfRsrcEvtCb = cb;
}

void RegisterCloudRsrcEventCallBack(ESCoapCloudConfCB cb)
{
    gCoapCloudConfRsrcEvtCb = cb;
}

void RegisterDevConfRsrcEventCallBack(ESDevConfCB cb)
{
    gDevConfRsrcEvtCb = cb;
}

void RegisterConnectRequestEventCallBack(ESConnectRequestCB cb)
{
    gConnectRequestEvtCb = cb;
}

void UnRegisterResourceEventCallBack()
{
    if (gWifiConfRsrcEvtCb)
    {
        gWifiConfRsrcEvtCb = NULL;
    }
    if (gCoapCloudConfRsrcEvtCb)
    {
        gCoapCloudConfRsrcEvtCb = NULL;
    }
    if (gDevConfRsrcEvtCb)
    {
        gDevConfRsrcEvtCb = NULL;
    }
    if (gConnectRequestEvtCb)
    {
        gConnectRequestEvtCb = NULL;
    }
}

OCStackResult initEasySetupResource(bool isSecured)
{
    g_ESEasySetupResource.status = ES_STATE_INIT;
    g_ESEasySetupResource.lastErrCode = ES_ERRCODE_NO_ERROR;
    for( int i = 0 ; i < NUM_CONNECT_TYPE ; ++i )
    {
        g_ESEasySetupResource.connectRequest[i] = ES_CONNECT_NONE;
    }
    g_ESEasySetupResource.numRequest = 0;

    OCStackResult res = OC_STACK_ERROR;
    if (isSecured)
    {
        res = OCCreateResource(&g_ESEasySetupResource.handle, OC_RSRVD_ES_RES_TYPE_EASYSETUP,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_EASYSETUP, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&g_ESEasySetupResource.handle, OC_RSRVD_ES_RES_TYPE_EASYSETUP,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_EASYSETUP, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }
    if(res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, ES_RH_TAG, "Created EasySetup resource with result: %s", getResult(res));
        return res;
    }

    res = OCBindResourceTypeToResource(g_ESEasySetupResource.handle, OC_RSRVD_ES_RES_TYPE_COL);
    if(res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, ES_RH_TAG, "Binding Resource type with result: %s", getResult(res));
        return res;
    }

    res = OCBindResourceInterfaceToResource(g_ESEasySetupResource.handle, OC_RSRVD_INTERFACE_LL);
    if(res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, ES_RH_TAG, "Binding Resource interface with result: %s", getResult(res));
        return res;
    }
    res = OCBindResourceInterfaceToResource(g_ESEasySetupResource.handle, OC_RSRVD_INTERFACE_BATCH);
    if(res != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, ES_RH_TAG, "Binding Resource interface with result: %s", getResult(res));
        return res;
    }

    OIC_LOG_V(DEBUG, ES_RH_TAG, "Created EasySetup resource with result: %s", getResult(res));
    return res;
}

OCStackResult initWiFiConfResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    g_ESWiFiConfResource.supportedFreq = WIFI_BOTH;
    g_ESWiFiConfResource.supportedMode[0] = WIFI_11A;
    g_ESWiFiConfResource.supportedMode[1] = WIFI_11B;
    g_ESWiFiConfResource.supportedMode[2] = WIFI_11G;
    g_ESWiFiConfResource.supportedMode[3] = WIFI_11N;
    g_ESWiFiConfResource.numMode = 4;
    g_ESWiFiConfResource.authType = NONE_AUTH;
    g_ESWiFiConfResource.encType = NONE_ENC;
    OICStrcpy(g_ESWiFiConfResource.ssid, sizeof(g_ESWiFiConfResource.ssid), "");
    OICStrcpy(g_ESWiFiConfResource.cred, sizeof(g_ESWiFiConfResource.cred), "");

    if (isSecured)
    {
        res = OCCreateResource(&g_ESWiFiConfResource.handle, OC_RSRVD_ES_RES_TYPE_WIFICONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_WIFICONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&g_ESWiFiConfResource.handle, OC_RSRVD_ES_RES_TYPE_WIFICONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_WIFICONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    OIC_LOG_V(DEBUG, ES_RH_TAG, "Created WiFiConf resource with result: %s", getResult(res));
    return res;

}

OCStackResult initCoapCloudConfResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    OICStrcpy(g_ESCoapCloudConfResource.authCode, sizeof(g_ESCoapCloudConfResource.authCode), "");
    OICStrcpy(g_ESCoapCloudConfResource.accessToken, sizeof(g_ESCoapCloudConfResource.accessToken), "");
    g_ESCoapCloudConfResource.accessTokenType = NONE_OAUTH_TOKENTYPE;
    OICStrcpy(g_ESCoapCloudConfResource.authProvider, sizeof(g_ESCoapCloudConfResource.authProvider), "");
    OICStrcpy(g_ESCoapCloudConfResource.ciServer, sizeof(g_ESCoapCloudConfResource.ciServer), "");

    if (isSecured)
    {
        res = OCCreateResource(&g_ESCoapCloudConfResource.handle, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_COAPCLOUDCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&g_ESCoapCloudConfResource.handle, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_COAPCLOUDCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    OIC_LOG_V(DEBUG, ES_RH_TAG, "Created CoapCloudConf resource with result: %s", getResult(res));
    return res;

}

OCStackResult initDevConfResource(bool isSecured)
{
    OCStackResult res = OC_STACK_ERROR;

    OICStrcpy(g_ESDevConfResource.devName, sizeof(g_ESDevConfResource.devName), "");
    OICStrcpy(g_ESDevConfResource.modelNumber, sizeof(g_ESDevConfResource.modelNumber), "");
    OICStrcpy(g_ESDevConfResource.location, sizeof(g_ESDevConfResource.location), "");
    OICStrcpy(g_ESDevConfResource.country, sizeof(g_ESDevConfResource.country), "");
    OICStrcpy(g_ESDevConfResource.language, sizeof(g_ESDevConfResource.language), "");

    if (isSecured)
    {
        res = OCCreateResource(&g_ESDevConfResource.handle, OC_RSRVD_ES_RES_TYPE_DEVCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_DEVCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
    }else
    {
        res = OCCreateResource(&g_ESDevConfResource.handle, OC_RSRVD_ES_RES_TYPE_DEVCONF,
        OC_RSRVD_INTERFACE_DEFAULT,
        OC_RSRVD_ES_URI_DEVCONF, OCEntityHandlerCb,
        NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
    }

    OIC_LOG_V(DEBUG, ES_RH_TAG, "Created DevConf resource with result: %s", getResult(res));
    return res;

}

void updateEasySetupResource(OCEntityHandlerRequest* ehRequest, OCRepPayload* input)
{
    OIC_LOG_V(DEBUG, ES_RH_TAG, "g_ESEasySetupResource.status %d", g_ESEasySetupResource.status);

    int64_t *connect_req = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };
    if (OCRepPayloadGetIntArray(input, OC_RSRVD_ES_CONNECT, &connect_req, dimensions))
    {
        ESConnectRequest* connectRequest = (ESConnectRequest*)OICMalloc(sizeof(ESConnectRequest));
        if( !connectRequest )
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return;
        }

        int cntRequest = 0;
        for (int i = 0 ; i < NUM_CONNECT_TYPE ; ++i)
        {
            g_ESEasySetupResource.connectRequest[i] = ES_CONNECT_NONE;
            connectRequest->connect[i] = ES_CONNECT_NONE;

            if(i < dimensions[0] &&
                (connect_req[i] == ES_CONNECT_WIFI || connect_req[i] == ES_CONNECT_COAPCLOUD))
            {
                g_ESEasySetupResource.connectRequest[cntRequest] = connect_req[i];
                connectRequest->connect[cntRequest] = connect_req[i];
                OIC_LOG_V(DEBUG, ES_RH_TAG, "g_ESEasySetupResource.connectType[%d] : %d",
                                                    cntRequest, g_ESEasySetupResource.connectRequest[cntRequest]);
                cntRequest++;
            }
        }
        connectRequest->numRequest = cntRequest;
        g_ESEasySetupResource.numRequest = cntRequest;

        if(g_ESEasySetupResource.connectRequest[0] != ES_CONNECT_NONE)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Send ConnectRequest Callback To ES");

            // TODO : Need to check appropriateness of gWiFiData
            if(gConnectRequestEvtCb != NULL)
            {
                gConnectRequestEvtCb(ES_OK, connectRequest);
            }
            else
            {
                OIC_LOG(ERROR, ES_RH_TAG, "gConnectRequestEvtCb is NULL");
            }
        }
    }

    if(ehRequest->query)
    {
        if(CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))
        {
            // When Provisioning resource has a POST with BatchInterface
            updateCoapCloudConfResource(input);
            updateWiFiConfResource(input);
            updateDevConfResource(input);
        }
    }
}

void updateWiFiConfResource(OCRepPayload* input)
{
    ESWiFiConfData* wiFiData = (ESWiFiConfData*)OICMalloc(sizeof(ESWiFiConfData));

    if(wiFiData == NULL)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "OICMalloc is failed");
        return ;
    }

    memset(wiFiData->ssid, 0, OIC_STRING_MAX_VALUE);
    memset(wiFiData->pwd, 0, OIC_STRING_MAX_VALUE);
    wiFiData->authtype = NONE_AUTH;
    wiFiData->enctype = NONE_AUTH;
    wiFiData->userdata = NULL;

    char* ssid = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_SSID, &ssid))
    {
        OICStrcpy(g_ESWiFiConfResource.ssid, sizeof(g_ESWiFiConfResource.ssid), ssid);
        OICStrcpy(wiFiData->ssid, sizeof(wiFiData->ssid), ssid);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESWiFiConfResource.ssid : %s", g_ESWiFiConfResource.ssid);
    }

    char* cred = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_CRED, &cred))
    {
        OICStrcpy(g_ESWiFiConfResource.cred, sizeof(g_ESWiFiConfResource.cred), cred);
        OICStrcpy(wiFiData->pwd, sizeof(wiFiData->pwd), cred);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESWiFiConfResource.cred %s", g_ESWiFiConfResource.cred);
    }

    int64_t authType = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_AUTHTYPE, &authType))
    {
        g_ESWiFiConfResource.authType = authType;
        wiFiData->authtype = g_ESWiFiConfResource.authType;
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESWiFiConfResource.authType %u", g_ESWiFiConfResource.authType);
    }

    int64_t encType = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_ENCTYPE, &encType))
    {
        g_ESWiFiConfResource.encType = encType;
        wiFiData->enctype = g_ESWiFiConfResource.encType;
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESWiFiConfResource.encType %u", g_ESWiFiConfResource.encType);
    }

    if(gReadUserdataCb)
    {
        gReadUserdataCb(input, OC_RSRVD_ES_RES_TYPE_WIFICONF, &wiFiData->userdata);
    }

    if(ssid || cred || authType!= -1 || encType != -1)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Send WiFiConfRsrc Callback To ES");

        // TODO : Need to check appropriateness of gWiFiData
        if(gWifiConfRsrcEvtCb != NULL)
        {
            gWifiConfRsrcEvtCb(ES_OK, wiFiData);
        }
        else
        {
            OIC_LOG(ERROR, ES_RH_TAG, "gWifiConfRsrcEvtCb is NULL");
        }
    }

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESWiFiConfResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Enrollee doesn't have any observer.");
    }

    OICFree(wiFiData);
}

void updateCoapCloudConfResource(OCRepPayload* input)
{
    ESCoapCloudConfData* cloudData = (ESCoapCloudConfData*)OICMalloc(sizeof(ESCoapCloudConfData));

    if(cloudData == NULL)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "OICMalloc is failed");
        return;
    }

    memset(cloudData->authCode, 0, OIC_STRING_MAX_VALUE);
    memset(cloudData->accessToken, 0, OIC_STRING_MAX_VALUE);
    g_ESCoapCloudConfResource.accessTokenType = NONE_OAUTH_TOKENTYPE;
    memset(cloudData->authProvider, 0, OIC_STRING_MAX_VALUE);
    memset(cloudData->ciServer, 0, OIC_STRING_MAX_VALUE);
    cloudData->userdata = NULL;

    char *authCode = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_AUTHCODE, &authCode))
    {
        OICStrcpy(g_ESCoapCloudConfResource.authCode, sizeof(g_ESCoapCloudConfResource.authCode), authCode);
        OICStrcpy(cloudData->authCode, sizeof(cloudData->authCode), authCode);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESCoapCloudConfResource.authCode %s", g_ESCoapCloudConfResource.authCode);
    }

    char *accessToken = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_ACCESSTOKEN, &accessToken))
    {
        OICStrcpy(g_ESCoapCloudConfResource.accessToken, sizeof(g_ESCoapCloudConfResource.accessToken), accessToken);
        OICStrcpy(cloudData->accessToken, sizeof(cloudData->accessToken), accessToken);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESCoapCloudConfResource.accessToken %s", g_ESCoapCloudConfResource.accessToken);
    }

    int64_t accessTokenType = -1;
    if (OCRepPayloadGetPropInt(input, OC_RSRVD_ES_ACCESSTOKEN_TYPE, &accessTokenType))
    {
        g_ESCoapCloudConfResource.accessTokenType = accessTokenType;
        cloudData->accessTokenType = g_ESCoapCloudConfResource.accessTokenType;
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESCoapCloudConfResource.accessTokenType %d", g_ESCoapCloudConfResource.accessTokenType);
    }

    char *authProvider = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_AUTHPROVIDER, &authProvider))
    {
        OICStrcpy(g_ESCoapCloudConfResource.authProvider, sizeof(g_ESCoapCloudConfResource.authProvider), authProvider);
        OICStrcpy(cloudData->authProvider, sizeof(cloudData->authProvider), authProvider);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESCoapCloudConfResource.authServerUrl %s", g_ESCoapCloudConfResource.authProvider);
    }

    char *ciServer = NULL;
    if (OCRepPayloadGetPropString(input, OC_RSRVD_ES_CISERVER, &ciServer))
    {
        OICStrcpy(g_ESCoapCloudConfResource.ciServer, sizeof(g_ESCoapCloudConfResource.ciServer), ciServer);
        OICStrcpy(cloudData->ciServer, sizeof(cloudData->ciServer), ciServer);
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "g_ESCoapCloudConfResource.ciServer %s", g_ESCoapCloudConfResource.ciServer);
    }

    if(gReadUserdataCb)
    {
        gReadUserdataCb(input, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF, &cloudData->userdata);
    }

    if(authCode || accessToken || authProvider || ciServer)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Send CoapCloudConfRsrc Callback To ES");

        // TODO : Need to check appropriateness of gCloudData
        if(gCoapCloudConfRsrcEvtCb != NULL)
        {
            gCoapCloudConfRsrcEvtCb(ES_OK, cloudData);
        }
        else
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "gCoapCloudConfRsrcEvtCb is NULL");
        }
    }

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESCoapCloudConfResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "CoapCloudConf resource doesn't have any observer.");
    }

    OICFree(cloudData);
}

void updateDevConfResource(OCRepPayload* input)
{
    ESDevConfData* devConfData = (ESDevConfData*)OICMalloc(sizeof(ESDevConfData));

    if(devConfData == NULL)
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "OICMalloc is failed");
        return;
    }
    devConfData->userdata = NULL;

    if (gReadUserdataCb)
    {
        gReadUserdataCb(input, OC_RSRVD_ES_RES_TYPE_DEVCONF, &devConfData->userdata);
    }

    if( devConfData->userdata != NULL )
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Send DevConfRsrc Callback To ES");

        // TODO : Need to check appropriateness of gDevConfData
        if(gDevConfRsrcEvtCb != NULL)
        {
            gDevConfRsrcEvtCb(ES_OK, devConfData);
        }
        else
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "gDevConfRsrcEvtCb is NULL");
        }
    }

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESDevConfResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "devConfResource doesn't have any observer.");
    }

    OICFree(devConfData);
}

OCRepPayload* constructResponseOfWiFiConf(char *interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    if(g_ESWiFiConfResource.handle == NULL)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "WiFiConf resource is not created");
        return NULL;
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "constructResponse WiFiConf res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_WIFICONF);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        tempPayload = payload;
        payload = repPayload;

        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **interfaces = (char **)OICMalloc(3 * sizeof(char*));
        if( !interfaces )
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_INTERFACE, (char **)interfaces, interfacesDimensions);

        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **resourceTypes = (char **)OICMalloc(2 * sizeof(char*));
        if( !resourceTypes )
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_WIFICONF);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_RES_TYPE, (char **)resourceTypes, resourceTypesDimensions);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_WIFICONF);
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {g_ESWiFiConfResource.numMode, 0, 0};
    int64_t *modes_64 = (int64_t *)OICMalloc(g_ESWiFiConfResource.numMode * sizeof(int64_t));
    if( !modes_64 )
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    for(int i = 0 ; i < g_ESWiFiConfResource.numMode ; ++i)
    {
        modes_64[i] = g_ESWiFiConfResource.supportedMode[i];
    }
    OCRepPayloadSetIntArray(payload, OC_RSRVD_ES_SUPPORTEDWIFIMODE, (int64_t *)modes_64, dimensions);

    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_SUPPORTEDWIFIFREQ, g_ESWiFiConfResource.supportedFreq);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_SSID, g_ESWiFiConfResource.ssid);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_CRED, g_ESWiFiConfResource.cred);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_AUTHTYPE, (int) g_ESWiFiConfResource.authType);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_ENCTYPE, (int) g_ESWiFiConfResource.encType);

    if(gWriteUserdataCb)
    {
        gWriteUserdataCb(payload, OC_RSRVD_ES_RES_TYPE_WIFICONF);
    }

    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
    }

    return payload;
}

OCRepPayload* constructResponseOfCoapCloudConf(char *interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    if(g_ESCoapCloudConfResource.handle == NULL)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "CoapCloudConf resource is not created");
        return NULL;
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "constructResponse CoapCloudConf res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_COAPCLOUDCONF);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        tempPayload = payload;
        payload = repPayload;

        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **interfaces = (char **)OICMalloc(3 * sizeof(char*));
        if(!interfaces)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_INTERFACE, (char **)interfaces, interfacesDimensions);

        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **resourceTypes = (char **)OICMalloc(2 * sizeof(char*));
        if(!resourceTypes)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_RES_TYPE, (char **)resourceTypes, resourceTypesDimensions);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF);
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_AUTHCODE, g_ESCoapCloudConfResource.authCode);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_ACCESSTOKEN, g_ESCoapCloudConfResource.accessToken);
    OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_ACCESSTOKEN_TYPE, (int)g_ESCoapCloudConfResource.accessTokenType);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_AUTHPROVIDER, g_ESCoapCloudConfResource.authProvider);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_CISERVER, g_ESCoapCloudConfResource.ciServer);

    if(gWriteUserdataCb)
    {
        gWriteUserdataCb(payload, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF);
    }

    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
    }

    return payload;
}

OCRepPayload* constructResponseOfDevConf(char *interface)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    if(g_ESDevConfResource.handle == NULL)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "DevConf resource is not created");
        return NULL;
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "constructResponse DevConf res");
    OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_DEVCONF);

    OCRepPayload* repPayload = NULL;
    OCRepPayload* tempPayload = NULL;
    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        tempPayload = payload;
        payload = repPayload;

        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **interfaces = (char **)OICMalloc(3 * sizeof(char*));
        if( !interfaces )
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_INTERFACE, (char **)interfaces, interfacesDimensions);

        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
        char **resourceTypes = (char **)OICMalloc(2 * sizeof(char*));
        if( !resourceTypes )
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_DEVCONF);

        OCRepPayloadSetStringArray(payload, OC_RSRVD_ES_RES_TYPE, (char **)resourceTypes, resourceTypesDimensions);
    }
    else
    {
        OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
        OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_DEVCONF);
    }

    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_DEVNAME, g_ESDevConfResource.devName);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_MODELNUMBER, g_ESDevConfResource.modelNumber);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_LOCATION, g_ESDevConfResource.location);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_LANGUAGE, g_ESDevConfResource.language);
    OCRepPayloadSetPropString(payload, OC_RSRVD_ES_COUNTRY, g_ESDevConfResource.country);

    if(gWriteUserdataCb)
    {
        gWriteUserdataCb(payload, OC_RSRVD_ES_RES_TYPE_DEVCONF);
    }

    if(!strcmp(interface, OC_RSRVD_INTERFACE_BATCH))
    {
        payload = tempPayload;
        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
    }

    return payload;
}

OCRepPayload* constructResponseOfEasySetup(OCEntityHandlerRequest *ehRequest)
{
    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
        return NULL;
    }

    // Requested interface is Link list interface
    //if(ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_LL))
    if(!ehRequest->query ||
        (ehRequest->query && !strcmp(ehRequest->query, "")) ||
        (ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_LL)) ||
        (ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_DEFAULT)))
    {
        const OCRepPayload *arrayPayload[3] = {NULL};

        int childResCnt = 0;

        if(g_ESWiFiConfResource.handle != NULL)
        {
            OCRepPayload *add = OCRepPayloadCreate();
            if(!add)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
            char **resourceType = NULL;
            resourceType = (char **)OICMalloc(sizeof(char *) * 1);
            char **resourceInterface = NULL;
            resourceInterface = (char **)OICMalloc(sizeof(char *) * 1);

            if(!resourceType || !resourceInterface)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_WIFICONF);
            resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

            add->base.type = PAYLOAD_TYPE_REPRESENTATION;
            OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_WIFICONF);
            OCRepPayloadSetStringArray(add, OC_RSRVD_RESOURCE_TYPE,
                                            (const char **)resourceType, dimensions);
            OCRepPayloadSetStringArray(add, OC_RSRVD_INTERFACE,
                                            (const char **)resourceInterface, dimensions);

            OCResourceProperty p = OCGetResourceProperties((OCResourceHandle *)g_ESWiFiConfResource.handle);
            OCRepPayload *policy = OCRepPayloadCreate();
            if (!policy)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP,
                                    ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
            if (p & OC_SECURE)
            {
                OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);
                uint16_t securePort = CAGetAssignedPortNumber(ehRequest->devAddr.adapter,
                                                                    ehRequest->devAddr.flags);
                OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);
            }

            OCRepPayloadSetPropObject(add, OC_RSRVD_POLICY, policy);

            arrayPayload[childResCnt++] = add;
        }

        if(g_ESDevConfResource.handle != NULL)
        {
            OCRepPayload *add = OCRepPayloadCreate();
            if(!add)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
            char **resourceType = NULL;
            resourceType = (char **)OICMalloc(sizeof(char *) * 1);
            char **resourceInterface = NULL;
            resourceInterface = (char **)OICMalloc(sizeof(char *) * 1);

            if(!resourceType || !resourceInterface)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_DEVCONF);
            resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

            add->base.type = PAYLOAD_TYPE_REPRESENTATION;
            OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_DEVCONF);
            OCRepPayloadSetStringArray(add, OC_RSRVD_RESOURCE_TYPE,
                                            (const char **)resourceType, dimensions);
            OCRepPayloadSetStringArray(add, OC_RSRVD_INTERFACE,
                                            (const char **)resourceInterface, dimensions);

            OCResourceProperty p = OCGetResourceProperties((OCResourceHandle *)g_ESDevConfResource.handle);
            OCRepPayload *policy = OCRepPayloadCreate();
            if (!policy)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP,
                                    ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
            if (p & OC_SECURE)
            {
                OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);
                uint16_t securePort = CAGetAssignedPortNumber(ehRequest->devAddr.adapter,
                                                                    ehRequest->devAddr.flags);
                OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);
            }

            OCRepPayloadSetPropObject(add, OC_RSRVD_POLICY, policy);

            arrayPayload[childResCnt++] = add;
        }

        if(g_ESCoapCloudConfResource.handle != NULL)
        {
            OCRepPayload *add = OCRepPayloadCreate();
            if(!add)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
            char **resourceType = NULL;
            resourceType = (char **)OICMalloc(sizeof(char *) * 1);
            char **resourceInterface = NULL;
            resourceInterface = (char **)OICMalloc(sizeof(char *) * 1);

            if(!resourceType || !resourceInterface)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            resourceType[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF);
            resourceInterface[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);

            add->base.type = PAYLOAD_TYPE_REPRESENTATION;
            OCRepPayloadSetPropString(add, OC_RSRVD_HREF, OC_RSRVD_ES_URI_COAPCLOUDCONF);
            OCRepPayloadSetStringArray(add, OC_RSRVD_RESOURCE_TYPE,
                                            (const char **)resourceType, dimensions);
            OCRepPayloadSetStringArray(add, OC_RSRVD_INTERFACE,
                                            (const char **)resourceInterface, dimensions);

            OCResourceProperty p = OCGetResourceProperties((OCResourceHandle *)g_ESCoapCloudConfResource.handle);
            OCRepPayload *policy = OCRepPayloadCreate();
            if (!policy)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP,
                                    ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
            if (p & OC_SECURE)
            {
                OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);
                uint16_t securePort = CAGetAssignedPortNumber(ehRequest->devAddr.adapter,
                                                                    ehRequest->devAddr.flags);
                OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);
            }

            OCRepPayloadSetPropObject(add, OC_RSRVD_POLICY, policy);

            arrayPayload[childResCnt++] = add;
        }

        size_t dimensions[MAX_REP_ARRAY_DEPTH] = {childResCnt, 0, 0};

        if(!ehRequest->query ||
            (ehRequest->query && !strcmp(ehRequest->query, "")) ||
            (ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_DEFAULT)))
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "constructResponse EasySetup res");
            OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_EASYSETUP);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_LL);
            OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_BATCH);
            OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_EASYSETUP);
            OCRepPayloadAddResourceType(payload, OC_RSRVD_ES_RES_TYPE_COL);

            OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_PROVSTATUS, g_ESEasySetupResource.status);
            OCRepPayloadSetPropInt(payload, OC_RSRVD_ES_LAST_ERRORCODE, g_ESEasySetupResource.lastErrCode);

            if(g_ESEasySetupResource.numRequest > 0)
            {
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {g_ESEasySetupResource.numRequest, 0, 0};
                int64_t *connectRequest = (int64_t *)OICMalloc(g_ESEasySetupResource.numRequest  * sizeof(int64_t));
                if(!connectRequest)
                {
                    OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                    return NULL;
                }

                for(int i = 0 ; i < g_ESEasySetupResource.numRequest  ; ++i)
                {
                    connectRequest[i] = g_ESEasySetupResource.connectRequest[i];
                }
                OCRepPayloadSetIntArray(payload, OC_RSRVD_ES_CONNECT, (int64_t *)connectRequest, dimensions);
            }
            else
            {
                OIC_LOG(DEBUG, ES_RH_TAG, "g_ESEasySetupResource.numRequest is 0");
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
                OCRepPayloadSetIntArray(payload, OC_RSRVD_ES_CONNECT, NULL, dimensions);
            }

            if(gWriteUserdataCb)
            {
                gWriteUserdataCb(payload, OC_RSRVD_ES_RES_TYPE_EASYSETUP);
            }

            OCRepPayloadSetPropObjectArray(payload, OC_RSRVD_ES_LINKS, arrayPayload, dimensions);
        }
        else    // link list interface
        {
            OCRepPayloadSetPropObjectArray(payload, OC_RSRVD_ES_LINKS, arrayPayload, dimensions);
        }
    } else if (
        ehRequest->query && CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))

    {
        OIC_LOG(DEBUG, ES_RH_TAG, "constructResponse EasySetup res");
        OCRepPayloadSetUri(payload, OC_RSRVD_ES_URI_EASYSETUP);

        OCRepPayload* repPayload = NULL;

        repPayload = OCRepPayloadCreate();
        if (!repPayload)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        size_t interfacesDimensions[MAX_REP_ARRAY_DEPTH] = {3, 0, 0};
        char **interfaces = (char **)OICMalloc(3 * sizeof(char*));
        if(!interfaces)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
        interfaces[1] = OICStrdup(OC_RSRVD_INTERFACE_LL);
        interfaces[2] = OICStrdup(OC_RSRVD_INTERFACE_BATCH);

        OCRepPayloadSetStringArray(repPayload, OC_RSRVD_ES_INTERFACE, (char **)interfaces, interfacesDimensions);

        size_t resourceTypesDimensions[MAX_REP_ARRAY_DEPTH] = {2, 0, 0};
        char **resourceTypes = (char **)OICMalloc(2 * sizeof(char*));
        if(!resourceTypes)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
            return NULL;
        }

        resourceTypes[0] = OICStrdup(OC_RSRVD_ES_RES_TYPE_EASYSETUP);
        resourceTypes[1] = OICStrdup(OC_RSRVD_ES_RES_TYPE_COL);

        OCRepPayloadSetStringArray(repPayload, OC_RSRVD_ES_RES_TYPE, (char **)resourceTypes, resourceTypesDimensions);

        OCRepPayloadSetPropInt(repPayload, OC_RSRVD_ES_PROVSTATUS, g_ESEasySetupResource.status);
        OCRepPayloadSetPropInt(repPayload, OC_RSRVD_ES_LAST_ERRORCODE, g_ESEasySetupResource.lastErrCode);
        if(g_ESEasySetupResource.numRequest > 0)
        {
            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {g_ESEasySetupResource.numRequest, 0, 0};
            int64_t *connectRequest = (int64_t *)OICMalloc(g_ESEasySetupResource.numRequest  * sizeof(int64_t));
            if(!connectRequest)
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Failed to allocate Payload");
                return NULL;
            }

            for(int i = 0 ; i < g_ESEasySetupResource.numRequest ; ++i)
            {
                connectRequest[i] = g_ESEasySetupResource.connectRequest[i];
            }
            OCRepPayloadSetIntArray(repPayload, OC_RSRVD_ES_CONNECT, (int64_t *)connectRequest, dimensions);
        }
        else
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "g_ESEasySetupResource.numRequest is 0");
            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
            OCRepPayloadSetIntArray(repPayload, OC_RSRVD_ES_CONNECT, NULL, dimensions);
        }

        if(gWriteUserdataCb)
        {
            gWriteUserdataCb(repPayload, OC_RSRVD_ES_RES_TYPE_EASYSETUP);
        }

        OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, repPayload);
    }

    if(ehRequest->query)
    {
        if(CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH))
        {// When Provisioning resource has a GET with BatchInterface
            OCRepPayload* head = payload;
            OCRepPayload* nextPayload = NULL;

            nextPayload = constructResponseOfWiFiConf(OC_RSRVD_INTERFACE_BATCH);
            if(nextPayload != NULL)
            {
                payload->next = nextPayload;
                payload = payload->next;
            }

            nextPayload = constructResponseOfCoapCloudConf(OC_RSRVD_INTERFACE_BATCH);
            if(nextPayload != NULL)
            {
                payload->next = nextPayload;
                payload = payload->next;
            }

            nextPayload = constructResponseOfDevConf(OC_RSRVD_INTERFACE_BATCH);
            if(nextPayload != NULL)
            {
                payload->next = nextPayload;
            }

            payload = head;
        }
    }

    return payload;
}


OCStackResult CreateEasySetupResources(bool isSecured, ESResourceMask resourceMask)
{
    OCStackResult res = OC_STACK_ERROR;
    bool maskFlag = false;

    res = initEasySetupResource(isSecured);
    if(res != OC_STACK_OK)
    {
        // TODO: destroy logic will be added
        OIC_LOG_V(ERROR, ES_RH_TAG, "initEasySetupResource result: %s", getResult(res));

        return res;
    }

    if((resourceMask & ES_WIFICONF_RESOURCE) == ES_WIFICONF_RESOURCE)
    {
        maskFlag = true;
        res = initWiFiConfResource(isSecured);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "initWiFiConfResource result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(g_ESEasySetupResource.handle, g_ESWiFiConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Bind WiFiConfResource result: %s", getResult(res));
            return res;
        }

    }

    if((resourceMask & ES_COAPCLOUDCONF_RESOURCE) == ES_COAPCLOUDCONF_RESOURCE)
    {
        maskFlag = true;
        res = initCoapCloudConfResource(isSecured);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "initCoapCloudConfResource result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(g_ESEasySetupResource.handle, g_ESCoapCloudConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Bind CoapCloudConfResource result: %s", getResult(res));
            return res;
        }
    }

    if((resourceMask & ES_DEVCONF_RESOURCE) == ES_DEVCONF_RESOURCE)
    {
        maskFlag = true;
        res = initDevConfResource(isSecured);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "initDevConf result: %s", getResult(res));
            return res;
        }

        res = OCBindResource(g_ESEasySetupResource.handle, g_ESDevConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Bind DevConfResource result: %s", getResult(res));
            return res;
        }
    }

    if(maskFlag == false)
    {
        OIC_LOG_V(ERROR, ES_RH_TAG, "Invalid ResourceMask");
        return OC_STACK_ERROR;

    }

    OIC_LOG_V(DEBUG, ES_RH_TAG, "Created all resources with result: %s", getResult(res));

    return res;
}

OCStackResult DeleteEasySetupResources()
{
    OCStackResult res = OC_STACK_ERROR;
    if (g_ESWiFiConfResource.handle != NULL)
    {
        res = OCUnBindResource(g_ESEasySetupResource.handle, g_ESWiFiConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Unbind WiFi resource error with result: %s", getResult(res));
        }
    }
    if (g_ESCoapCloudConfResource.handle != NULL)
    {
        res = OCUnBindResource(g_ESEasySetupResource.handle, g_ESCoapCloudConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Unbind CloudServer resource error with result: %s", getResult(res));
        }
    }
    if (g_ESDevConfResource.handle != NULL)
    {
        res = OCUnBindResource(g_ESEasySetupResource.handle, g_ESDevConfResource.handle);
        if(res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Unbind DevConf resource error with result: %s", getResult(res));
        }
    }

    if (g_ESWiFiConfResource.handle != NULL)
    {
        res = OCDeleteResource(g_ESWiFiConfResource.handle);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Deleting WiFi resource error with result: %s", getResult(res));
        }
    }

    if(g_ESCoapCloudConfResource.handle != NULL)
    {
        res = OCDeleteResource(g_ESCoapCloudConfResource.handle);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Deleting CloudServer resource error with result: %s", getResult(res));
        }
    }

    if(g_ESDevConfResource.handle != NULL)
    {
        res = OCDeleteResource(g_ESDevConfResource.handle);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Deleting DevConf resource error with result: %s", getResult(res));
        }
    }

    if(g_ESEasySetupResource.handle != NULL)
    {
        res = OCDeleteResource(g_ESEasySetupResource.handle);
        if (res != OC_STACK_OK)
        {
            OIC_LOG_V(ERROR, ES_RH_TAG, "Deleting Prov resource error with result: %s", getResult(res));
        }
    }

    return res;
}

OCEntityHandlerResult ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if (!ehRequest)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Request is Null");
        return ehResult;
    }
    if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Incoming payload not a representation");
        return ehResult;
    }

    OCRepPayload *getResp = NULL;
    *payload = NULL;

    if(ehRequest->resource == g_ESEasySetupResource.handle)
    {
        if(ehRequest->query &&
            strcmp(ehRequest->query, "") &&
            !CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_LL) &&
            !CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH) &&
            !CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_DEFAULT))
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            getResp = constructResponseOfEasySetup(ehRequest);
        }
    }
    else if(ehRequest->resource == g_ESWiFiConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            getResp = constructResponseOfWiFiConf(OC_RSRVD_INTERFACE_DEFAULT);
        }
    }
    else if(ehRequest->resource == g_ESCoapCloudConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            getResp = constructResponseOfCoapCloudConf(OC_RSRVD_INTERFACE_DEFAULT);
        }
    }
    else if(ehRequest->resource == g_ESDevConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            getResp = constructResponseOfDevConf(OC_RSRVD_INTERFACE_DEFAULT);
        }
    }

    if (!getResp)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "constructResponse failed");
        return OC_EH_ERROR;
    }

    *payload = getResp;
    ehResult = OC_EH_OK;

    return ehResult;
}

OCEntityHandlerResult ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload** payload)
{
    OIC_LOG(DEBUG, ES_RH_TAG, "ProcessPostRequest enter");
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Incoming payload not a representation");
        return ehResult;
    }

    OCRepPayload* input = (OCRepPayload*) (ehRequest->payload);
    if (!input)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Failed to parse");
        return ehResult;
    }

    if(ehRequest->resource == g_ESEasySetupResource.handle)
    {
        if(ehRequest->query &&
            strcmp(ehRequest->query, "") &&
            !CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_BATCH) &&
            !CompareResourceInterface(ehRequest->query, OC_RSRVD_INTERFACE_DEFAULT))
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            updateEasySetupResource(ehRequest, input);
        }
    }
    else if(ehRequest->resource == g_ESWiFiConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            updateWiFiConfResource(input);
        }
    }
    else if(ehRequest->resource == g_ESCoapCloudConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            updateCoapCloudConfResource(input);
        }
    }
    else if(ehRequest->resource == g_ESDevConfResource.handle)
    {
        if(CheckEhRequestPayload(ehRequest) != OC_EH_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
            return OC_EH_BAD_REQ;
        }
        else
        {
            updateDevConfResource(input);
        }
    }

    OCRepPayload *getResp = NULL;
    if(ehRequest->resource == g_ESEasySetupResource.handle)
    {
        getResp = constructResponseOfEasySetup(ehRequest);
    }
    else if(ehRequest->resource == g_ESWiFiConfResource.handle)
    {
        getResp = constructResponseOfWiFiConf(OC_RSRVD_INTERFACE_DEFAULT);
    }
    else if(ehRequest->resource == g_ESCoapCloudConfResource.handle)
    {
        getResp = constructResponseOfCoapCloudConf(OC_RSRVD_INTERFACE_DEFAULT);
    }
    else if(ehRequest->resource == g_ESDevConfResource.handle)
    {
        getResp = constructResponseOfDevConf(OC_RSRVD_INTERFACE_DEFAULT);
    }

    if (!getResp)
    {
        OIC_LOG(ERROR, ES_RH_TAG, "constructResponse failed");
        return OC_EH_ERROR;
    }

    *payload = getResp;
    ehResult = OC_EH_OK;

    return ehResult;
}

OCEntityHandlerResult ProcessPutRequest(OCEntityHandlerRequest * ehRequest,
        OCRepPayload** payload)
{
    (void) ehRequest;
    (void) payload;
    OCEntityHandlerResult ehResult = OC_EH_BAD_REQ;

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
    OCEntityHandlerResult ehRet = OC_EH_OK;
    OCEntityHandlerResponse response =
    { 0, 0, OC_EH_ERROR, 0, 0,
    { },
    { 0 }, false };
    OCRepPayload* payload = NULL;

    if (entityHandlerRequest && (flag & OC_REQUEST_FLAG))
    {
        if (OC_REST_GET == entityHandlerRequest->method)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Received GET request");
            ehRet = ProcessGetRequest(entityHandlerRequest, &payload);
        }
        else if (OC_REST_PUT == entityHandlerRequest->method)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Received PUT request");

            //PUT request will be handled in the internal implementation
            if (g_ESEasySetupResource.handle != NULL)
            {
                ehRet = ProcessPutRequest(entityHandlerRequest, &payload);
            }
            else
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Cannot process put");
                ehRet = OC_EH_ERROR;
            }
        }
        else if (OC_REST_POST == entityHandlerRequest->method)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Received OC_REST_POST from client");
            if (g_ESEasySetupResource.handle != NULL)
            {
                ehRet = ProcessPostRequest(entityHandlerRequest, &payload);
            }
            else
            {
                OIC_LOG(ERROR, ES_RH_TAG, "Cannot process put");
                ehRet = OC_EH_ERROR;
            }
        }

        // Format the response.  Note this requires some info about the request
        response.requestHandle = entityHandlerRequest->requestHandle;
        response.resourceHandle = entityHandlerRequest->resource;
        response.ehResult = ehRet;
        //response uses OCPaylod while all get,put methodes use OCRepPayload
        response.payload = (OCPayload*) (payload);
        response.numSendVendorSpecificHeaderOptions = 0;
        memset(response.sendVendorSpecificHeaderOptions, 0,
                sizeof(response.sendVendorSpecificHeaderOptions));
        memset(response.resourceUri, 0, sizeof(response.resourceUri));
        // Indicate that response is NOT in a persistent buffer
        response.persistentBufferFlag = 0;

        // Send the response
        if (OCDoResponse(&response) != OC_STACK_OK)
        {
            OIC_LOG(ERROR, ES_RH_TAG, "Error sending response");
            ehRet = OC_EH_ERROR;
        }
    }
    if (entityHandlerRequest && (flag & OC_OBSERVE_FLAG))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "Flag includes OC_OBSERVE_FLAG");

        if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Received OC_OBSERVE_REGISTER from Mediator");
        }
        else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action)
        {
            OIC_LOG(DEBUG, ES_RH_TAG, "Received OC_OBSERVE_DEREGISTER from Mediator");
        }
    }
    return ehRet;
}

OCStackResult SetDeviceProperty(ESDeviceProperty *deviceProperty)
{
    OIC_LOG(DEBUG, ES_RH_TAG, "SetDeviceProperty IN");

    g_ESWiFiConfResource.supportedFreq = (deviceProperty->WiFi).freq;
    OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "WiFi Freq : %d", g_ESWiFiConfResource.supportedFreq);

    int modeIdx = 0;
    while((deviceProperty->WiFi).mode[modeIdx] != WiFi_EOF)
    {
        g_ESWiFiConfResource.supportedMode[modeIdx] = (deviceProperty->WiFi).mode[modeIdx];
        OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "WiFi Mode : %d", g_ESWiFiConfResource.supportedMode[modeIdx]);
        modeIdx ++;
    }
    g_ESWiFiConfResource.numMode = modeIdx;

    OICStrcpy(g_ESDevConfResource.devName, OIC_STRING_MAX_VALUE, (deviceProperty->DevConf).deviceName);
    OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "Device Name : %s", g_ESDevConfResource.devName);

    if (OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESWiFiConfResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "wifiResource doesn't have any observers.");
    }

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESDevConfResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "devConfResource doesn't have any observers.");
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "SetDeviceProperty OUT");
    return OC_STACK_OK;
}

OCStackResult SetEnrolleeState(ESEnrolleeState esState)
{
    OIC_LOG(DEBUG, ES_RH_TAG, "SetEnrolleeState IN");

    g_ESEasySetupResource.status = esState;
    OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "Enrollee Status : %d", g_ESEasySetupResource.status);

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESEasySetupResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "provResource doesn't have any observers.");
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "SetEnrolleeState OUT");
    return OC_STACK_OK;
}

OCStackResult SetEnrolleeErrCode(ESErrorCode esErrCode)
{
    OIC_LOG(DEBUG, ES_RH_TAG, "SetEnrolleeErrCode IN");

    g_ESEasySetupResource.lastErrCode = esErrCode;
    OIC_LOG_V(INFO_PRIVATE, ES_RH_TAG, "Enrollee ErrorCode : %d", g_ESEasySetupResource.lastErrCode);

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESEasySetupResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, ES_RH_TAG, "provResource doesn't have any observers.");
    }

    OIC_LOG(DEBUG, ES_RH_TAG, "SetEnrolleeErrCode OUT");
    return OC_STACK_OK;
}

OCEntityHandlerResult CheckEhRequestPayload(OCEntityHandlerRequest *ehRequest)
{
    if( !(ehRequest->query) ||
                (ehRequest->query &&
                (strcmp(ehRequest->query, "") && !CompareResourceInterface(ehRequest->query,
                                                                        OC_RSRVD_INTERFACE_DEFAULT))))
    {
        OIC_LOG(ERROR, ES_RH_TAG, "Not supported Interface");
        return OC_EH_BAD_REQ;
    }
    return OC_EH_OK;
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
        default:
            return "UNKNOWN";
    }
}
