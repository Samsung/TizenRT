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


#include "easysetup.h"
#include "easysetup_wifi_conn.h"

#include <unistd.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <glib.h>
#include <limits>
#include <unistd.h>
#define TAG "TIZEN_ES"

using namespace std;

gboolean mainThread(GIOChannel *source, GIOCondition condition, gpointer data);
void *listeningFunc(void *);

GMainLoop *gMainloop;
static pthread_t gThreadHandle = 0;


/**
 * Secure Virtual Resource database for Iotivity Server
 * It contains Server's Identity and the PSK credentials
 * of other devices which the server trusts
 */
static char CRED_FILE[] = "oic_svr_db_server.dat";

OCPersistentStorage ps;

/**
 * @var gIsSecured
 * @brief Variable to check if secure mode is enabled or not.
 */
static bool gIsSecured = false;
static bool gWiFiCBflag = false;
static char gSSID[OIC_STRING_MAX_VALUE];
static char gPasswd[OIC_STRING_MAX_VALUE];

void PrintMenu()
{
    cout << "========================" << endl;
    cout << "A: Enabled Security" << endl;
    cout << "B: Init & Start EasySetup" << endl;
    cout << "C: Set DeviceInfo" << endl;
    cout << "D: Connect to TargetAP" << endl;
    cout << "E: Show Menu......." << endl;
    cout << "Q: Terminate" << endl;
    cout << "========================" << endl;
}

void WiFiConfProvCbInApp(ESWiFiConfData *eventData)
{
    cout << "WiFiConfProvCbInApp IN" << endl;
    gWiFiCBflag = true;

    ESSetState(ES_STATE_CONNECTING_TO_ENROLLER);

    if(eventData == NULL)
    {
        cout << "ESWiFiConfData is NULL" << endl;
        return ;
    }

    cout << "SSID : " << eventData->ssid << endl;
    cout << "Password : " << eventData->pwd << endl;
    cout << "AuthType : " << eventData->authtype << endl;
    cout << "EncType : " << eventData->enctype << endl;

    memset(gSSID, 0, OIC_STRING_MAX_VALUE);
    memset(gPasswd, 0, OIC_STRING_MAX_VALUE);
    if(eventData->ssid != NULL)
    {
        strncpy(gSSID, eventData->ssid, strlen(eventData->ssid));
    }

    if(eventData->pwd != NULL)
    {
        strncpy(gPasswd, eventData->pwd, strlen(eventData->pwd));
    }

    cout << "WiFiConfProvCbInApp OUT" << endl;
    PrintMenu();
}

void DevConfProvCbInApp(ESDevConfData *eventData)
{
    cout << "DevConfProvCbInApp IN" << endl;

    if(eventData == NULL)
    {
        cout << "ESDevConfData is NULL" << endl;
        return ;
    }

    cout << "Language : " << eventData->language << endl;
    cout << "Country : " << eventData->country << endl;
    cout << "DevConfProvCbInApp OUT" << endl;
    PrintMenu();
}

void CoapCloudConfProvCbInApp(ESCoapCloudConfData *eventData)
{
    cout << "CoapCloudConfProvCbInApp IN" << endl;

    if(eventData == NULL)
    {
        cout << "ESCoapCloudConfData is NULL" << endl;
        return ;
    }

    cout << "AuthCode : " << eventData->authCode << endl;
    cout << "AuthProvider : " << eventData->authProvider << endl;
    cout << "CI Server : " << eventData->ciServer << endl;
    cout << "CoapCloudConfProvCbInApp OUT" << endl;
    PrintMenu();
}

ESProvisioningCallbacks gCallbacks = {
    .WiFiConfProvCb = &WiFiConfProvCbInApp,
    .DevConfProvCb = &DevConfProvCbInApp,
    .CoapCloudConfProvCb = &CoapCloudConfProvCbInApp
};

FILE* server_fopen(const char *path, const char *mode)
{
    (void) path;
    return fopen(CRED_FILE, mode);
}

void EnableSecurity()
{
    printf("Inside EnableSecurity API..\n");

    gIsSecured = true;

    // Initialize Persistent Storage for SVR database
    ps = (OCPersistentStorage){ server_fopen, fread, fwrite, fclose, unlink };
    OCRegisterPersistentStorageHandler(&ps);
}

void StartEasySetup()
{
    cout << "StartEasySetup IN" << endl;

    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        cout << "OCStack init error!!" << endl;
        return;
    }

    ESResourceMask resourcemMask = (ESResourceMask) (ES_WIFICONF_RESOURCE | ES_COAPCLOUDCONF_RESOURCE | ES_DEVCONF_RESOURCE);
    cout << "resourcemMask : " << resourcemMask << endl;
    if(ESInitEnrollee(gIsSecured, resourcemMask, gCallbacks) != ES_OK)
    {
        cout << "OCStack init error!!" << endl;
        return;
    }

    cout << "ESInitEnrollee Success" << endl;

    if(gThreadHandle == 0) {

        if (pthread_create(&gThreadHandle, NULL, listeningFunc, NULL)) {
            cout << "Thread creation failed" << endl;
            return;
        }

    }

    ESSetState(ES_STATE_INIT);
    ESSetErrorCode(ES_ERRCODE_NO_ERROR);

    int ret = 0;

    ret = TizenWiFiInit();
    if(ret != WIFI_NO_ERROR) {
        cout << "WiFi Init Error" << endl;
    }
    else
        cout << "WiFi Init Success" << endl;

    ret = TizenWiFiScanStart();
    if(ret != WIFI_NO_ERROR) {
        cout << "WiFi Scan Error" << endl;
    }
    else
        cout << "WiFi Scan Success" << endl;

    cout << "StartEasySetup OUT" << endl;
}

void SetDeviceInfo()
{
    cout << "SetDeviceInfo IN" << endl;

    ESDeviceProperty deviceProperty = {
        {{WIFI_11G, WIFI_11N, WIFI_11AC, WiFi_EOF}, WIFI_5G}, {"Tizen Device"}
    };

    if(ESSetDeviceProperty(&deviceProperty) == ES_ERROR)
    {
        cout << "ESSetDeviceProperty Error" << endl;
    }

    // Set user properties if needed

    cout << "SetDeviceInfo OUT" << endl;
}

void StopEasySetup()
{
    cout << "StopEasySetup IN" << endl;
    if (ESTerminateEnrollee() == ES_ERROR)
    {
        cout << "ESTerminateEnrollee Failed!!" << endl;
        return;
    }

    // Deinit Tizen Wifi
    TizenWiFiDeinit();

    // Stop OC Stack
    if (OCStop() != OC_STACK_OK)
    {
        cout << "OCStack stop failed!!" << endl;
        return;
    }

    if(gThreadHandle != 0) {
        pthread_cancel(gThreadHandle);
        pthread_join(gThreadHandle, NULL);
    }

    cout << "StopEasySetup OUT" << endl;
}

void ConnectToTargetAP()
{
    if(!gWiFiCBflag){
        cout << "WiFi Provisioning is needed to be preceded" << endl;
        return;
    }

    WiFiConnErrCode ret = WIFI_NO_ERROR;
    ret = TizenWiFiScanStart();
    if(ret != WIFI_NO_ERROR){
        ESSetState(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);
        ESSetErrorCode(ES_ERRCODE_UNKNOWN);
        cout << "WiFi Scan Error" << endl;
        return;
    }
    else{
        cout << "WiFi Scan Succss" << endl;
    }

    ret = TizenWiFiConn(gSSID, gPasswd);
    if(ret != WIFI_NO_ERROR) {
        ESSetState(ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER);

        if(ret == WIFI_NOTFOUND_SSID_ERROR) {
            ESSetErrorCode(ES_ERRCODE_SSID_NOT_FOUND);
        }
        else if(ret == WIFI_WRONG_PWD_ERROR) {
            ESSetErrorCode(ES_ERRCODE_PW_WRONG);
        }
        else {
            ESSetErrorCode(ES_ERRCODE_TIMEOUT);
        }
        cout << "WiFi Connection Error" << endl;
        return;
    }
    else {
        cout << "WIFI Connection Success" << endl;
        ESSetState(ES_STATE_CONNECTED_TO_ENROLLER);
        ESSetErrorCode(ES_ERRCODE_NO_ERROR);
    }
}

gboolean mainThread(GIOChannel *source, GIOCondition condition, gpointer data)
{
    char entered;
    cin >> entered;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (entered) {
        case 'Q': // quit
        case 'q':
            StopEasySetup();
            break;

        case 'A': // Enable Security
        case 'a':
#ifdef __WITH_DTLS__
            EnableSecurity();
#else
            cout << "Sample is not built with secured mode" << endl;
#endif
            PrintMenu();
            break;

        case 'B': // Init EasySetup
        case 'b':
            StartEasySetup();
            PrintMenu();
            break;

        case 'C': // Set Device Info
        case 'c':
            SetDeviceInfo();
            PrintMenu();
            break;

        case 'D': // Start to connect target AP
        case 'd':
            ConnectToTargetAP();
            PrintMenu();
            break;

        case 'E': // Print Menu
        case 'e':
            PrintMenu();
            break;

        default:
            cout << "Wrong option" << endl;
            PrintMenu();
            break;
    }

    if(entered == 'q' || entered == 'Q'){
        g_main_loop_quit(gMainloop);
    }

    return TRUE;
}

int main()
{
    cout << "#########################" << endl;
    cout << "EasySetup Enrollee SAMPLE" << endl;
    cout << "#########################" << endl;
    PrintMenu();

    gMainloop = g_main_loop_new (NULL, FALSE);
    if(gMainloop == NULL) {
        cout << "Create Main Loop Error" << endl;
        return 0;
    }

    GIOChannel *channel = g_io_channel_unix_new(0);
    g_io_add_watch(channel, (GIOCondition)(G_IO_IN|G_IO_ERR|G_IO_HUP|G_IO_NVAL), mainThread, NULL);

    g_main_loop_run (gMainloop);

    cout << "#########################" << endl;
    cout << "Terminate Enrollee SAMPLE" << endl;
    cout << "#########################" << endl;

    return 0;
}

void *listeningFunc(void*)
{
    OCStackResult result;

    while (true)
    {
        result = OCProcess();
        if (result != OC_STACK_OK)
        {
           cout<<"OCStack stop error";
        }
    }
    return NULL;
}

