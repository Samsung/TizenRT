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

#include "easysetup.h"
#include "escommon.h"

#ifndef EASYSETUPX_ENROLLEE_H__
#define EASYSETUPX_ENROLLEE_H__

#define MAXLEN_STRING 1000
#define MAXNUM_LOCATION 20
#define MAXLEN_DATE_TIME 33

#define SC_RSRVD_ES_VENDOR_NETCONNECTION_STATE  "x.com.samsung.ncs"
#define SC_RSRVD_ES_VENDOR_DISCOVERY_CHANNEL    "x.com.samsung.chn"
#define SC_RSRVD_ES_VENDOR_DEVICE_TYPE          "x.com.samsung.dt"
#define SC_RSRVD_ES_VENDOR_DEVICE_SUBTYPE       "x.com.samsung.sdt"
#define SC_RSRVD_ES_VENDOR_LOCATION             "x.com.samsung.location"
#define SC_RSRVD_ES_VENDOR_CLIENTID             "x.com.samsung.clientid"
#define SC_RSRVD_ES_VENDOR_REGISTER_MOBILE_DEV  "x.com.samsung.rmd"
#define SC_RSRVD_ES_VENDOR_REGISTER_SET_DEV     "x.com.samsung.rsd"
#define SC_RSRVD_ES_VENDOR_ACCOUNT              "x.com.samsung.account"
#define SC_RSRVD_ES_VENDOR_AAC                  "x.com.samsung.aac"
#define SC_RSRVD_ES_VENDOR_TNC_HEADER           "x.com.samsung.tcheader"
#define SC_RSRVD_ES_VENDOR_TNC_VERSION          "x.com.samsung.tcversion"
#define SC_RSRVD_ES_VENDOR_TNC_RESULT           "x.com.samsung.tcresult"
#define SC_RSRVD_ES_VENDOR_TNC_STATUS           "x.com.samsung.tcstatus"
#define SC_RSRVD_ES_VENDOR_REFRESH_TOKEN        "x.com.samsung.refreshtoken"
#define SC_RSRVD_ES_VENDOR_UID                  "x.com.samsung.uid"
#define SC_RSRVD_ES_VENDOR_BSSID                "x.com.samsung.bssid"
#define SC_RSRVD_ES_VENDOR_PNP_PIN              "x.com.samsung.pnppin"
#define SC_RSRVD_ES_VENDOR_MODEL_NUMBER         "x.com.samsung.modelnumber"
#define SC_RSRVD_ES_VENDOR_LANGUAGE             "x.com.samsung.language"
#define SC_RSRVD_ES_VENDOR_COUNTRY              "x.com.samsung.country"
#define SC_RSRVD_ES_VENDOR_GPSLOCATION          "x.com.samsung.gpslocation"
#define SC_RSRVD_ES_VENDOR_UTC_DATE_TIME        "x.com.samsung.datetime"
#define SC_RSRVD_ES_VENDOR_REGIONAL_DATE_TIME   "x.com.samsung.regionaldatetime"

#define WIFI_DISCOVERY_CHANNEL_INIT             -1

/**
 * @brief  Supported WIFI frequency like 2.4G and 5G
 */
typedef enum
{
    NET_STATE_INIT = -1,                /**< Init state **/
    NET_STATE_WIRED_CONNECTED = 0,      /**< Wired connected **/
    NET_STATE_WIRELESS_CONNECTED,       /**< Wireless connected **/
    NET_STATE_NOT_CONNECTED             /**< Not connected, at all **/
} NETCONNECTION_STATE;

typedef struct SCWiFiConfProperties
{
    int discoveryChannel;                   /**< Wi-Fi AP Channel used for fast discovery **/
    char bssid[MAXLEN_STRING];              /**< Wi-Fi bssid information. **/
} SCWiFiConfProperties;

typedef struct SCTncInfo
{
    char header[MAXLEN_STRING]; /**< Terms & Conditions header **/
    char version[MAXLEN_STRING];    /**< Terms & Conditions version **/
}SCTncInfo;

typedef struct SCDevConfProperties
{
    int numLocation;
    char location[MAXNUM_LOCATION][MAXLEN_STRING];  /**< Samsung-specific location-related information **/
    char regMobileDev[MAXLEN_STRING];               /**< Samsung-specific mobile device information for 'register TV' **/
    char account[MAXLEN_STRING];  /**< Samsung-specific account-related information **/
    SCTncInfo scTnCInfo;    /**< Samsung-specific Terms & Conditions information **/
    char modelNumber[MAXLEN_STRING];                /**< Samsung-specific model number **/
    char language[MAXLEN_STRING];                   /**< IETF language tag using ISO 639X **/
    char country[MAXLEN_STRING];                    /**< ISO Country Code (ISO 3166-1 Alpha-2) **/
    char gpsLocation[MAXLEN_STRING];                /**< GPS information of device. Longitude and latitude in json format **/
    char utcDateTime[MAXLEN_DATE_TIME];             /**< UTC date time **/
    char regionalDateTime[MAXLEN_DATE_TIME];        /**< Regional date time **/
} SCDevConfProperties;

typedef struct SCCoapCloudServerConfProperties
{
    char clientID[MAXLEN_STRING];               /**< Samsung-specific clientId for sign-up to IoT Cloud **/
    char aac[MAXLEN_STRING];                    /**< Samsung-specific aac information **/
    char tncResult[MAXLEN_STRING];              /**< Samsung-specific Terms & Conditions result **/
    char refreshToken[MAXLEN_STRING];           /**< Samsung-specific refreshToken information. Indicate refresh token to be used if the access token is expired**/
    char uid[MAXLEN_STRING];                    /**< Samsung-specific aac information. Indicate user ID corresponding to user account **/
} SCCoapCloudServerConfProperties;

typedef struct SCProperties
{
    NETCONNECTION_STATE netConnectionState; /**< A state of network connection **/
    int discoveryChannel;                   /**< Wi-Fi AP Channel used for fast discovery **/
    char deviceType[MAXLEN_STRING];         /**< Generated with Device Type + Icon Type **/
    char deviceSubType[MAXLEN_STRING];      /**< Device Sub Category **/
    int numLocation;
    char location[MAXNUM_LOCATION][MAXLEN_STRING];  /**< Samsung-specific location-related information **/
    char clientID[MAXLEN_STRING];           /**< Samsung-specific clientId for sign-up to IoT Cloud **/
    char regMobileDev[MAXLEN_STRING];       /**< Samsung-specific mobile device information for 'register TV' **/
    char regSetDev[MAXLEN_STRING];          /**< Samsung-specific set device information for 'register TV' **/
    char account[MAXLEN_STRING];            /**< Samsung-specific account-related information **/
    char aac[MAXLEN_STRING];                /**< Samsung-specific aac information **/
    SCTncInfo tncInfo;                      /**< Samsung-specific Terms & Conditions information **/
    char tncResult[MAXLEN_STRING];          /**< Samsung-specific Terms & Conditions result **/
    int tncStatus;                          /**< Samsung-specific Terms & Conditions status **/
    char refreshToken[MAXLEN_STRING];       /**< Samsung-specific refreshToken information. Indicate refresh token to be used if the access token is expired**/
    char uid[MAXLEN_STRING];                /**< Samsung-specific aac information. Indicate user ID corresponding to user account **/
    char bssid[MAXLEN_STRING];              /**< Samsung-specific Wi-Fi bssid information. **/
    char pnpPin[MAXLEN_STRING];             /**< Samsung-specific PnP Pin **/
    char modelNumber[MAXLEN_STRING];                /**< Samsung-specific model number **/
    char language[MAXLEN_STRING];                   /**< IETF language tag using ISO 639X **/
    char country[MAXLEN_STRING];                    /**< ISO Country Code (ISO 3166-1 Alpha-2) **/
    char gpsLocation[MAXLEN_STRING];                /**< GPS information of device. Longitude and latitude in json format **/
    char utcDateTime[MAXLEN_DATE_TIME];             /**< UTC date time **/
    char regionalDateTime[MAXLEN_DATE_TIME];        /**< Regional date time **/
} SCProperties;

void ReadUserdataCb(OCRepPayload* payload, char* resourceType, void** userdata);
void WriteUserdataCb(OCRepPayload* payload, char* resourceType);

ESResult SetSCProperties(const SCProperties *prop);

ESResult SetRegisterSetDevice(const char *regSetDevice);

ESResult SetSCTncInfo(SCTncInfo *tncInfo);
ESResult SetSCTncStatus(int status);
ESResult SetSCNetConnectionState(NETCONNECTION_STATE netConnectionState);
ESResult SetSCPnPPin(const char *pnp);

#endif /* EASYSETUPX_ENROLLEE_H__ */
