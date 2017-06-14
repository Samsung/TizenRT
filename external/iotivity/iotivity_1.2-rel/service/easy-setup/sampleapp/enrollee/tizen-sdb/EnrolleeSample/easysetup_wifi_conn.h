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

#ifndef EASYSETUP_WIFI_CONN_H__
#define EASYSETUP_WIFI_CONN_H__
#ifdef __cplusplus

extern "C"
{
#endif

typedef enum
{
    /** No error.*/ 
    WIFI_NO_ERROR = 0,
    /** Tizen WiFi Init Error.*/
    WIFI_INIT_ERROR,
    /** Tizen WiFi Deinit Error.*/
    WIFI_DEINIT_ERROR,
    /** Tizen WiFi Activate Error.*/
    WIFI_ACTIVATE_ERROR,
    /** Tizen WiFi Scan Error.*/
    WIFI_SCAN_ERROR,
    /** Tizen WiFi Connection Error.*/
    WIFI_CONN_ERROR,
    /** Tizen WiFi Not Found SSID Error.*/
    WIFI_NOTFOUND_SSID_ERROR,
    /** Tizen WiFi Wrong Password Error.*/
    WIFI_WRONG_PWD_ERROR,
} WiFiConnErrCode;

WiFiConnErrCode TizenWiFiInit(void);
WiFiConnErrCode TizenWiFiDeinit(void);
WiFiConnErrCode TizenWiFiScanStart(void);
WiFiConnErrCode TizenWiFiConn(char *ssid, char* passwd);

#ifdef __cplusplus
}
#endif

#endif /* EASYSETUP_WIFI_CONN_H__ */
