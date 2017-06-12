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


#include "easysetup_wifi_conn.h"
#include "string.h"
#include "logger.h"
#include "escommon.h"

#include <wifi.h>
#include <tizen_error.h>
#include <glib.h>
#include <unistd.h>

/**
 * @var ES_WIFICONN_TAG
 * @brief Logging tag for module name.
 */
#define ES_WIFICONN_TAG "ESWIFICONN"

typedef struct{
    char wifiName[OIC_STRING_MAX_VALUE];
    char wifiPasswd[OIC_STRING_MAX_VALUE];
} TargetWifiInfo;

static bool gWiFiConnFlag;

char* PrintWifiErr(wifi_error_e errType)
{
    switch (errType) {
        case WIFI_ERROR_NONE:
            return "NONE";
        case WIFI_ERROR_INVALID_PARAMETER:
            return "INVALID_PARAMETER";
        case WIFI_ERROR_OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case WIFI_ERROR_INVALID_OPERATION:
            return "INVALID_OPERATION";
        case WIFI_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
            return "ADDRESS_FAMILY_NOT_SUPPORTED";
        case WIFI_ERROR_OPERATION_FAILED:
            return "OPERATION_FAILED";
        case WIFI_ERROR_NO_CONNECTION:
            return "NO_CONNECTION";
        case WIFI_ERROR_NOW_IN_PROGRESS:
            return "NOW_IN_PROGRESS";
        case WIFI_ERROR_ALREADY_EXISTS:
            return "ALREADY_EXISTS";
        case WIFI_ERROR_OPERATION_ABORTED:
            return "OPERATION_ABORTED";
        case WIFI_ERROR_DHCP_FAILED:
            return "DHCP_FAILED";
        case WIFI_ERROR_INVALID_KEY:
            return "INVALID_KEY";
        case WIFI_ERROR_NO_REPLY:
            return "NO_REPLY";
        case WIFI_ERROR_SECURITY_RESTRICTED:
            return "SECURITY_RESTRICTED";
        case WIFI_ERROR_PERMISSION_DENIED:
            return "PERMISSION_DENIED";
        case WIFI_ERROR_NOT_SUPPORTED:
            return "NOT_SUPPORTED";
    }

    return "UNKNOWN";
}

static void WiFiScanCallback(wifi_error_e err, void* data)
{
    if(data != NULL) {
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "UserData : %s", (char *) data);
    }

    OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Scan Callback Res : %s", PrintWifiErr(err));
}

static void WiFiActivateCallback(wifi_error_e result, void* user_data)
{
    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "__test_activated_callback IN");

    int ret = wifi_scan(WiFiScanCallback, NULL);
    if (ret != WIFI_ERROR_NONE) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "WiFi Scan Error");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Scan Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
    }

    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "__test_activated_callback OUT");

}

static void ConnectedCallback(wifi_error_e err, void* data)
{
    if(err == WIFI_ERROR_NONE) {
        OIC_LOG(DEBUG, ES_WIFICONN_TAG, "Success to Connect AP");        
    }
    else {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "Fail to Connect AP");        
    }
}

static bool WiFiFoundCallback(wifi_ap_h ap, void *data)
{
    TargetWifiInfo* targetInfo = (TargetWifiInfo *) data;

    char *foundAP = NULL;

    int ret = wifi_ap_get_essid(ap, &foundAP);

    if(ret != WIFI_ERROR_NONE) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "Fail to get AP");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
        return false;
    }

    OIC_LOG_V(DEBUG, ES_WIFICONN_TAG, "Found AP name : %s", foundAP);

    if(strstr(foundAP, targetInfo->wifiName) != NULL) {

        bool needPasswd = false;

        if (wifi_ap_is_passphrase_required(ap, &needPasswd) == WIFI_ERROR_NONE) {

            OIC_LOG(DEBUG, ES_WIFICONN_TAG, "Passsword required");

            ret = wifi_ap_set_passphrase(ap, targetInfo->wifiPasswd);
            if (ret != WIFI_ERROR_NONE) {
                OIC_LOG(ERROR, ES_WIFICONN_TAG, "Fail to Set Password");
                OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
                g_free(foundAP);
                return false;
            }

            ret = wifi_connect(ap, ConnectedCallback, targetInfo);
            if (ret != WIFI_ERROR_NONE) {
                OIC_LOG(ERROR, ES_WIFICONN_TAG, "Fail to connect wifi");
                OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Connect Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
                g_free(foundAP);
                return false;                
            }
            else {
                OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Success to connect wifi : %s", PrintWifiErr((wifi_error_e) ret));
                gWiFiConnFlag = true;
                g_free(foundAP);
                return false;                
            }


        }
        else {
            OIC_LOG(ERROR, ES_WIFICONN_TAG, "Fail to get Passphrase Required");
            OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
            return false;
        }

    }
    else {
        OIC_LOG(DEBUG, ES_WIFICONN_TAG, "This AP is not the one wanted to be connected");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Searching : %s / Searched : %s", targetInfo->wifiName, foundAP);
    }

    g_free(foundAP);

    return true;
}

WiFiConnErrCode TizenWiFiDeinit()
{
    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiDeinit IN");

    int ret = wifi_deinitialize();
    if(ret != WIFI_ERROR_NONE) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "wifi deinit error");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
        return WIFI_DEINIT_ERROR;
    }

    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiDeinit OUT");
    return WIFI_NO_ERROR;
}

WiFiConnErrCode TizenWiFiInit()
{
    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiInit IN");

    gWiFiConnFlag = false;
    int ret = wifi_initialize();
    if (ret != WIFI_ERROR_NONE) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "WiFi Init Error");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Init Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
        return WIFI_INIT_ERROR;
    }

    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiInit OUT");

    return WIFI_NO_ERROR;
}

WiFiConnErrCode TizenWiFiScanStart()
{
    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiScanStart IN");
    int ret;

    bool wifiState = false;
    wifi_is_activated(&wifiState);

    // if wifi is not activated, actviate & scan
    if (wifiState == false) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "WiFi Not Activated...Activate it!");
        ret = wifi_activate(WiFiActivateCallback, NULL);

        if(ret != WIFI_ERROR_NONE) {
            OIC_LOG(ERROR, ES_WIFICONN_TAG, "wifi activate error");
            return WIFI_ACTIVATE_ERROR;
        }
    }
    else{
        ret = wifi_scan(WiFiScanCallback, NULL);
        if (ret != WIFI_ERROR_NONE) {
            OIC_LOG(ERROR, ES_WIFICONN_TAG, "WiFi Scan Error");
            OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Scan Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
            return WIFI_SCAN_ERROR;
        }
    }

    OIC_LOG(DEBUG, ES_WIFICONN_TAG, "TizenWiFiScanStart OUT");

    return WIFI_NO_ERROR;
}


WiFiConnErrCode TizenWiFiConn(char *ssid, char* passwd)
{
    TargetWifiInfo targetInfo;

    memset(targetInfo.wifiName, 0, OIC_STRING_MAX_VALUE);
    memset(targetInfo.wifiPasswd, 0, OIC_STRING_MAX_VALUE);
    if(ssid != NULL)
    {
        strncpy(targetInfo.wifiName, ssid, strlen(ssid));
    }

    if(passwd != NULL)
    {
        strncpy(targetInfo.wifiPasswd, passwd, strlen(passwd));
    }

    int ret = wifi_foreach_found_aps(WiFiFoundCallback, &targetInfo);
    if(ret != WIFI_ERROR_NONE) {
        OIC_LOG(ERROR, ES_WIFICONN_TAG, "wifi_foreach_found_aps Error");
        OIC_LOG_V(INFO, ES_WIFICONN_TAG, "Fail Status : %s", PrintWifiErr((wifi_error_e) ret));
        memset(targetInfo.wifiName, 0, OIC_STRING_MAX_VALUE);
        memset(targetInfo.wifiPasswd, 0, OIC_STRING_MAX_VALUE);

        return WIFI_CONN_ERROR;
    }

    // try to find target AP during 10[sec]
    int times = 1;
    while(!gWiFiConnFlag) {
        sleep(1);

        if(times >= 10){
            OIC_LOG(ERROR, ES_WIFICONN_TAG, "Connection Error... try 10[sec]");
            memset(targetInfo.wifiName, 0, OIC_STRING_MAX_VALUE);
            memset(targetInfo.wifiPasswd, 0, OIC_STRING_MAX_VALUE);

            return WIFI_CONN_ERROR;
        }

        times ++;
    }

    gWiFiConnFlag = false;

    return WIFI_NO_ERROR;
}

