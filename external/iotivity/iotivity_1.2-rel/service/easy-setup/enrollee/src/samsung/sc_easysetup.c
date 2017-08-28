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


#include "samsung/sc_easysetup.h"
#include "string.h"
#include "oic_malloc.h"
#include "logger.h"
#include "stdio.h"

#include "ocpayload.h"
#include "oic_string.h"
#include "oic_malloc.h"

#include "resourcehandler.h"

/**
 * @var SC_ENROLLEE_TAG
 * @brief Logging tag for module name.
 */
#define SC_ENROLLEE_TAG "ES_SC_ENROLLEE"

EasySetupResource g_ESEasySetupResource;
WiFiConfResource g_ESWiFiConfResource;
CoapCloudConfResource g_ESCoapCloudConfResource;
DevConfResource g_ESDevConfResource;

SCProperties g_SCProperties;

static void ReadAccountData(OCRepPayload* payload,void** userdata);
static void ReadTnCdata(OCRepPayload* payload,void** userdata);
static void WriteTnCdata(OCRepPayload* payload, char* resourceType);
static void WriteWifiData(OCRepPayload* payload, char* resourceType);

ESResult SetSCProperties(const SCProperties *prop)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "SetSCProperties IN");
    if(prop != NULL)
    {
        memcpy(&g_SCProperties, prop, sizeof(SCProperties));
        OIC_LOG(INFO, SC_ENROLLEE_TAG, "SetSCProperties OUT");
        return ES_OK;
    }
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "SetSCProperties OUT");
    return ES_ERROR;
}

static void ReadAccountData(OCRepPayload* payload,void** userdata)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadAccountData IN");

    char* account = NULL;

    if(OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_ACCOUNT, &account))
    {
        if(*userdata == NULL)
        {
            *userdata = (void*)OICMalloc(sizeof(SCDevConfProperties));
            if( *userdata == NULL )
            {
                OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCDevConfProperties is failed");
                return ;
            }
        }

        SCDevConfProperties *pDevConfProp = (SCDevConfProperties*)(*userdata);
        OICStrcpy(pDevConfProp->account, MAXLEN_STRING, account);
        OICStrcpy(g_SCProperties.account, MAXLEN_STRING, account);

        OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                SC_RSRVD_ES_VENDOR_ACCOUNT, pDevConfProp->account);

    }
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadAccountData OUT");

}

ESResult SetSCTncInfo(SCTncInfo *tncInfo)
{
    if(tncInfo == NULL)
    {
        return ES_ERROR;
    }
    g_SCProperties.tncInfo = *tncInfo;
    return ES_OK;
}

ESResult SetSCTncStatus(int status)
{
    g_SCProperties.tncStatus = status;
    return ES_OK;
}

ESResult SetSCNetConnectionState(NETCONNECTION_STATE netConnectionState)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "SetSCNetConnectionState IN");

    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "SetSCNetConnectionState: %d", netConnectionState);
    g_SCProperties.netConnectionState = netConnectionState;

    if(OC_STACK_NO_OBSERVERS == OCNotifyAllObservers(g_ESEasySetupResource.handle, OC_HIGH_QOS))
    {
        OIC_LOG(DEBUG, SC_ENROLLEE_TAG, "provResource doesn't have any observers.");
    }

    OIC_LOG(INFO, SC_ENROLLEE_TAG, "SetSCNetConnectionState OUT");
    return ES_OK;
}

static void ReadTnCdata(OCRepPayload* payload,void** userdata)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadTnCdata IN");

    char* tncResult = NULL;

    if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_TNC_RESULT, &tncResult))
    {
        if(*userdata == NULL)
        {
            *userdata = (void*)OICMalloc(sizeof(SCCoapCloudServerConfProperties));
            if( *userdata == NULL )
            {
                OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCCoapCloudServerConfProperties is failed");
                return ;
            }
        }

        SCCoapCloudServerConfProperties *pProp = (SCCoapCloudServerConfProperties*)(*userdata);
        OICStrcpy(pProp->tncResult, MAXLEN_STRING, tncResult);
        OICStrcpy(g_SCProperties.tncResult, MAXLEN_STRING, tncResult);

        OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                SC_RSRVD_ES_VENDOR_TNC_RESULT, pProp->tncResult);
    }

    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadTnCdata OUT");
}

void WriteTnCdata(OCRepPayload* payload, char* resourceType)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteTnCdata IN");

    if(payload == NULL || resourceType == NULL)
    {
        OIC_LOG(DEBUG, SC_ENROLLEE_TAG, "Invalid Params payload or resourceType is NULL");
        OIC_LOG(DEBUG, SC_ENROLLEE_TAG, "WriteTnCdata OUT");
        return;
    }
    if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_EASYSETUP))
    {
        OCRepPayloadSetPropInt(payload, SC_RSRVD_ES_VENDOR_TNC_STATUS, g_SCProperties.tncStatus);
    }
    else if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_DEVCONF))
    {
        OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_TNC_HEADER,
                g_SCProperties.tncInfo.header);
        OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_TNC_VERSION,
                g_SCProperties.tncInfo.version);
    }
    else if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF))
    {
        OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_TNC_RESULT,
                g_SCProperties.tncResult);
    }
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteTnCdata OUT");
}

void WriteWifiData(OCRepPayload* payload, char* resourceType)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteWifiData IN");

    if(payload == NULL || resourceType == NULL)
    {
        OIC_LOG(DEBUG, SC_ENROLLEE_TAG, "Invalid Params payload or resourceType is NULL");
        OIC_LOG(DEBUG, SC_ENROLLEE_TAG, "WriteWifiData OUT");
        return;
    }

    if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_WIFICONF))
    {
        OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_BSSID,
                g_SCProperties.bssid);
    }
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteWifiData OUT");
}

ESResult SetRegisterSetDevice(const char *regSetDevice)
{
    if(regSetDevice != NULL)
    {
        OICStrcpy(g_SCProperties.regSetDev, sizeof(g_SCProperties.regSetDev), regSetDevice);
        return ES_OK;
    }
    return ES_ERROR;
}

ESResult SetSCPnPPin(const char *pnp)
{
    if(pnp != NULL)
    {
        OICStrcpy(g_SCProperties.pnpPin, sizeof(g_SCProperties.pnpPin), pnp);
        return ES_OK;
    }
    return ES_ERROR;
}

void ReadUserdataCb(OCRepPayload* payload, char* resourceType, void** userdata)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadUserdataCb IN");

    if(payload != NULL)
    {
        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_WIFICONF))
        {
            int64_t channel = -1;
            char *bssid = NULL;
            if (OCRepPayloadGetPropInt(payload, SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL, &channel))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(SCWiFiConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCWiFiConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCWiFiConfProperties));
                }
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %ld",
                                                            SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL, channel);
                ((SCWiFiConfProperties*)(*userdata))->discoveryChannel = (int) channel;
                g_SCProperties.discoveryChannel = channel;
            }
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_BSSID, &bssid))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*) OICMalloc(sizeof(SCWiFiConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCWiFiConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCWiFiConfProperties));
                }
                if (*userdata != NULL)
                {
                    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                            SC_RSRVD_ES_VENDOR_BSSID, bssid);
                    SCWiFiConfProperties* pWifiConfProp = (SCWiFiConfProperties*)(*userdata);
                    OICStrcpy(pWifiConfProp->bssid, sizeof(pWifiConfProp->bssid), bssid);
                    OICStrcpy(g_SCProperties.bssid, sizeof(g_SCProperties.bssid), bssid);
                    OICFree(bssid);
                }
            }
        }
        else if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_DEVCONF))
        {
            if(*userdata == NULL)
            {
                *userdata = (void*)OICMalloc(sizeof(SCDevConfProperties));
                if( *userdata == NULL )
                {
                    OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCDevConfProperties is failed");
                    return ;
                }
                memset(*userdata, 0, sizeof(SCDevConfProperties));
            }

            SCDevConfProperties *pDevConfProp = (SCDevConfProperties*)(*userdata);

            char**locationList = NULL;
            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
            if(OCRepPayloadGetStringArray(payload, SC_RSRVD_ES_VENDOR_LOCATION, &locationList, dimensions))
            {
                for(size_t idx = 0; idx < dimensions[0]; idx++)
                {
                    OICStrcpy(pDevConfProp->location[idx], strlen(locationList[idx])+1, locationList[idx]);
                    OICStrcpy(g_SCProperties.location[idx], strlen(locationList[idx])+1, locationList[idx]);

                    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                                        SC_RSRVD_ES_VENDOR_LOCATION, pDevConfProp->location[idx]);
                }

                ((SCDevConfProperties*)(*userdata))->numLocation = (int)dimensions[0];
                g_SCProperties.numLocation = (int)dimensions[0];
            }

            ReadAccountData(payload,userdata);

            char *regMobileDev = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV, &regMobileDev))
            {
                OICStrcpy(pDevConfProp->regMobileDev, strlen(regMobileDev)+1, regMobileDev);
                OICStrcpy(g_SCProperties.regMobileDev, strlen(regMobileDev)+1, regMobileDev);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.regMobileDev %s", g_SCProperties.regMobileDev);
            }

            char *country = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_COUNTRY, &country))
            {
                OICStrcpy(pDevConfProp->country, strlen(country)+1, country);
                OICStrcpy(g_SCProperties.country, strlen(country)+1, country);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.country %s", g_SCProperties.country);
            }

            char *language = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_LANGUAGE, &language))
            {
                OICStrcpy(pDevConfProp->language, strlen(language)+1, language);
                OICStrcpy(g_SCProperties.language, strlen(language)+1, language);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.language %s", g_SCProperties.language);
            }

            char *gpsLocation = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_GPSLOCATION, &gpsLocation))
            {
                OICStrcpy(pDevConfProp->gpsLocation, strlen(gpsLocation)+1, gpsLocation);
                OICStrcpy(g_SCProperties.gpsLocation, strlen(gpsLocation)+1, gpsLocation);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.gpsLocation %s", g_SCProperties.gpsLocation);
            }

            char *utcDateTime = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_UTC_DATE_TIME, &utcDateTime))
            {
                OICStrcpy(pDevConfProp->utcDateTime, strlen(utcDateTime)+1, utcDateTime);
                OICStrcpy(g_SCProperties.utcDateTime, strlen(utcDateTime)+1, utcDateTime);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.utcDateTime %s", g_SCProperties.utcDateTime);
            }

            char *regionalDateTime = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_REGIONAL_DATE_TIME, &regionalDateTime))
            {
                OICStrcpy(pDevConfProp->regionalDateTime, strlen(regionalDateTime)+1, regionalDateTime);
                OICStrcpy(g_SCProperties.regionalDateTime, strlen(regionalDateTime)+1, regionalDateTime);
                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "pDevConfProp.regionalDateTime %s", g_SCProperties.regionalDateTime);
            }
        }
        else if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_COAPCLOUDCONF))
        {
            char* clientID = NULL;
            if(OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_CLIENTID, &clientID))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(SCCoapCloudServerConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCCoapCloudServerConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCCoapCloudServerConfProperties));
                }

                SCCoapCloudServerConfProperties *pCloudProp =
                                                    (SCCoapCloudServerConfProperties*)(*userdata);

                OICStrcpy(pCloudProp->clientID, strlen(clientID)+1, clientID);
                OICStrcpy(g_SCProperties.clientID, strlen(clientID)+1, clientID);

                OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                                        SC_RSRVD_ES_VENDOR_CLIENTID, pCloudProp->clientID);
            }

            //SC_RSRVD_ES_VENDOR_AAC
            char *aac = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_AAC, &aac))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(SCCoapCloudServerConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCCoapCloudServerConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCCoapCloudServerConfProperties));
                }

                if (*userdata != NULL)
                {
                    SCCoapCloudServerConfProperties *pCloudProp =
                                                    (SCCoapCloudServerConfProperties*) (*userdata);
                    pCloudProp->aac[0] = '\0';

                    OICStrcpy(pCloudProp->aac, MAXLEN_STRING, aac);
                    OICStrcpy(g_SCProperties.aac, MAXLEN_STRING, aac);
                    OICFree(aac);

                    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                            SC_RSRVD_ES_VENDOR_AAC, pCloudProp->aac);
                }
            }

            //SC_RSRVD_ES_VENDOR_UID
            char *uid = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_UID, &uid))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(SCCoapCloudServerConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCCoapCloudServerConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCCoapCloudServerConfProperties));
                }

                if (*userdata != NULL)
                {
                    SCCoapCloudServerConfProperties *pCloudProp =
                                                    (SCCoapCloudServerConfProperties*) (*userdata);
                    pCloudProp->uid[0] = '\0';

                    OICStrcpy(pCloudProp->uid, MAXLEN_STRING, uid);
                    OICStrcpy(g_SCProperties.uid, MAXLEN_STRING, uid);
                    OICFree(uid);

                    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                            SC_RSRVD_ES_VENDOR_UID, pCloudProp->uid);
                }
            }

            //SC_RSRVD_ES_VENDOR_REFRESH_TOKEN
            char *refreshToken = NULL;
            if (OCRepPayloadGetPropString(payload, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, &refreshToken))
            {
                if(*userdata == NULL)
                {
                    *userdata = (void*)OICMalloc(sizeof(SCCoapCloudServerConfProperties));
                    if( *userdata == NULL )
                    {
                        OIC_LOG(ERROR, SC_ENROLLEE_TAG, "OICMalloc for SCCoapCloudServerConfProperties is failed");
                        return ;
                    }
                    memset(*userdata, 0, sizeof(SCCoapCloudServerConfProperties));
                }

                if (*userdata != NULL)
                {
                    SCCoapCloudServerConfProperties *pCloudProp =
                                                    (SCCoapCloudServerConfProperties*) (*userdata);
                    pCloudProp->refreshToken[0] = '\0';

                    OICStrcpy(pCloudProp->refreshToken, MAXLEN_STRING, refreshToken);
                    OICStrcpy(g_SCProperties.refreshToken, MAXLEN_STRING, refreshToken);
                    OICFree(refreshToken);

                    OIC_LOG_V(INFO_PRIVATE, SC_ENROLLEE_TAG, "[User specific property] %s : %s",
                            SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, pCloudProp->refreshToken);
                }
            }

            ReadTnCdata(payload,userdata);
        }
    }

    OIC_LOG(INFO, SC_ENROLLEE_TAG, "ReadUserdataCb OUT");
}

void WriteUserdataCb(OCRepPayload* payload, char* resourceType)
{
    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteUserdataCb IN");

    if(payload != NULL)
    {
        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_EASYSETUP))
        {
            OCRepPayloadSetPropInt(payload, SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE, (int) g_SCProperties.netConnectionState);
        }

        if(strstr(resourceType, OC_RSRVD_ES_RES_TYPE_DEVCONF))
        {
#ifndef __TIZENRT__
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_DEVICE_TYPE, g_SCProperties.deviceType);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE, g_SCProperties.deviceSubType);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV, g_SCProperties.regSetDev);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV, g_SCProperties.regMobileDev);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_PNP_PIN, g_SCProperties.pnpPin);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_MODEL_NUMBER, g_SCProperties.modelNumber);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_COUNTRY, g_SCProperties.country);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_LANGUAGE, g_SCProperties.language);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_GPSLOCATION, g_SCProperties.gpsLocation);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_UTC_DATE_TIME, g_SCProperties.utcDateTime);
            OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REGIONAL_DATE_TIME, g_SCProperties.regionalDateTime);
#else
            if(g_SCProperties.deviceType != NULL)
            {
                OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_DEVICE_TYPE, g_SCProperties.deviceType);
            }
            if(g_SCProperties.deviceSubType != NULL)
            {
                OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE, g_SCProperties.deviceSubType);
            }
            if(g_SCProperties.regSetDev != NULL)
            {
                OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV, g_SCProperties.regSetDev);
            }
            if(g_SCProperties.regMobileDev != NULL)
            {
                OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV, g_SCProperties.regMobileDev);
            }
            if(g_SCProperties.pnpPin != NULL)
            {
               OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_PNP_PIN, g_SCProperties.pnpPin);
            }
            if(g_SCProperties.modelNumber != NULL)
            {
               OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_MODEL_NUMBER, g_SCProperties.modelNumber);
            }
            if(g_SCProperties.country != NULL)
            {
               OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_COUNTRY, g_SCProperties.country);
            }
            if(g_SCProperties.language != NULL)
            {
               OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_LANGUAGE, g_SCProperties.language);
            }
            if(g_SCProperties.gpsLocation != NULL)
            {
               OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_GPSLOCATION, g_SCProperties.gpsLocation);
            }
#endif
        }
    }

    WriteTnCdata(payload, resourceType);
    WriteWifiData(payload, resourceType);

    OIC_LOG(INFO, SC_ENROLLEE_TAG, "WriteUserdataCb OUT");
}
