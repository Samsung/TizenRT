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

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <oic_string.h>
#include "oic_malloc.h"

#define TAG "ENROLLEE_SAMPLE"

void *listeningFunc(void *);

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

static SCProperties g_SCProperties = {0, 0, "", "", 0, {""}, ""};

void PrintMenu()
{
    printf("============\n");
    printf("S: Enabled Security\n");
    printf("I: Init & Start EasySetup\n");
    printf("D: Set DeviceInfo\n");
    printf("T: Terminate\n");
    printf("Q: Quit\n");
    printf("============\n");
}

void WiFiProvCbInApp(ESWiFiConfData *eventData)
{
    printf("WiFiProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESWiFiProvData is NULL\n");
        return ;
    }

    printf("SSID : %s\n", eventData->ssid);
    printf("Password : %s\n", eventData->pwd);
    printf("AuthType : %d\n", eventData->authtype);
    printf("EncType : %d\n", eventData->enctype);

    if(eventData->userdata != NULL)
    {
        SCWiFiConfProperties *data = eventData->userdata;
        printf("[SC] DiscoveryChannel : %d\n", data->discoveryChannel);
    }

    printf("WiFiProvCbInApp OUT\n");
}

void DevConfProvCbInApp(ESDevConfData *eventData)
{
    printf("DevConfProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESDevConfProvData is NULL\n");
        return ;
    }

    printf("Language : %s\n", eventData->language);
    printf("Country : %s\n", eventData->country);
    printf("Location : %s\n", eventData->location);

    if(eventData->userdata != NULL)
    {
        SCDevConfProperties *data = eventData->userdata;
        for(int i = 0 ; i < data->numLocation ; ++i)
        {
            printf("[SC] Location : %s\n", data->location[i]);
        }
        printf("[SC] Register Mobile Device : %s\n", data->regMobileDev);
        printf("[SC] Country : %s\n", data->country);
        printf("[SC] Language : %s\n", data->language);
        printf("[SC] GPS Location : %s\n", data->gpsLocation);
    }

    printf("DevConfProvCbInApp OUT\n");
}

void CloudDataProvCbInApp(ESCoapCloudConfData *eventData)
{
    printf("CloudDataProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESCloudProvData is NULL\n");
        return ;
    }

    printf("AuthCode : %s\n", eventData->authCode);
    printf("AuthProvider : %s\n", eventData->authProvider);
    printf("CI Server : %s\n", eventData->ciServer);

    if(eventData->userdata != NULL)
    {
        SCCoapCloudServerConfProperties *data = eventData->userdata;

        if(data->clientID != NULL)
        {
            printf("[SC] ClientID : %s\n", data->clientID);
        }
    }

    printf("CloudDataProvCbInApp OUT\n");
}

ESProvisioningCallbacks gCallbacks = {
    .WiFiConfProvCb = &WiFiProvCbInApp,
    .DevConfProvCb = &DevConfProvCbInApp,
    .CoapCloudConfProvCb = &CloudDataProvCbInApp
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
    printf("StartEasySetup IN\n");

    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        printf("OCStack init error!!\n");
        return;
    }

    ESResourceMask resourcemMask = ES_WIFICONF_RESOURCE | ES_COAPCLOUDCONF_RESOURCE | ES_DEVCONF_RESOURCE;
    if(ESInitEnrollee(gIsSecured, resourcemMask, gCallbacks) != ES_OK)
    {
        printf("OCStack init error!!\n");
        return;
    }
    printf("ESInitEnrollee Success\n");

    // Set callbacks for Samsung
    ESSetCallbackForUserdata(&ReadUserdataCb, &WriteUserdataCb);

    pthread_t thread_handle;
    if (pthread_create(&thread_handle, NULL, listeningFunc, NULL))
    {
        printf("Thread creation failed\n");
    }

    printf("StartEasySetup OUT\n");
}

void SetDeviceInfo()
{
    printf("SetDeviceInfo IN\n");

    ESDeviceProperty deviceProperty = {
        {{WIFI_11G, WIFI_11N, WIFI_11AC, WiFi_EOF}, WIFI_5G},
            {"Test Device"}
    };

    if(ESSetDeviceProperty(&deviceProperty) == ES_ERROR)
        printf("ESSetDeviceProperty Error\n");

    // Set user properties if needed
    strncpy(g_SCProperties.deviceType, "deviceType", MAXLEN_STRING);
    strncpy(g_SCProperties.deviceSubType, "deviceSubType", MAXLEN_STRING);
    g_SCProperties.netConnectionState = NET_STATE_INIT;
    g_SCProperties.discoveryChannel = WIFI_DISCOVERY_CHANNEL_INIT;
    strncpy(g_SCProperties.regSetDev, "{\"wm\":\"00:11:22:33:44:55\",\"pm\":\"00:11:22:33:44:55\",\"bm\":\"00:11:22:33:44:55\",\"rk\":[\"VOICE\",\"EXTRA\",\"BTHIDPOWERON\"],\"sl\":[\"TV2MOBILE\",\"MOBILE2TV\",\"BTWAKEUP\",\"WOWLAN\",\"BTREMOTECON\",\"DLNADMR\"]}", MAXLEN_STRING);
    strncpy(g_SCProperties.pnpPin, "pinNumber", MAXLEN_STRING);
    strncpy(g_SCProperties.modelNumber, "Model Number", MAXLEN_STRING);

    if(SetSCProperties(&g_SCProperties) == ES_ERROR)
        printf("SetSCProperties Error\n");

    printf("SetDeviceInfo OUT\n");
}

void StopEasySetup()
{
    printf("StopEasySetup IN\n");

    if (ESTerminateEnrollee() == ES_ERROR)
    {
        printf("ESTerminateEnrollee Failed!!\n");
        return;
    }

    //stop OC Stack
    if (OCStop() != OC_STACK_OK)
    {
        printf("OCStack stop failed!!\n");
        return;
    }

    printf("StopEasySetup OUT\n");
}

int main()
{
    printf("#########################\n");
    printf("EasySetup Enrollee SAMPLE\n");
    printf("#########################\n");
    PrintMenu();
    char option;

    while(true)
    {
        if(scanf("%c", &option) != 1)
        {
            printf("Failed to read input data\n");
            continue;
        }

        if(option!= '\n')
        {
            switch (option)
            {
                case 'H': // help
                case 'h':
                    PrintMenu();
                    break;

                case 'Q': // quit
                case 'q':
                    printf("quit");
                    break;

                case 'S': // Enable Security
                case 's':
                    EnableSecurity();
                    PrintMenu();
                    break;

                case 'I': // Init EasySetup
                case 'i':
                    StartEasySetup();
                    PrintMenu();
                    break;

                case 'D': // Set Device Info
                case 'd':
                    SetDeviceInfo();
                    PrintMenu();
                    break;
                case 'T': // stop easy setup
                case 't':
                    StopEasySetup();
                    PrintMenu();
                    break;
                default:
                    printf("wrong option\n");
                    PrintMenu();
                    break;
            }
            if (option == 'Q' || option == 'q') break;
        }
    }
    return 0;
}

void *listeningFunc(void * data)
{
    (void)data;
    OCStackResult result;

    while (true)
    {
        result = OCProcess();
        if (result != OC_STACK_OK)
        {
           printf("OCStack stop error");
        }
    }
    return NULL;
}

