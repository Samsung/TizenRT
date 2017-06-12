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
#include "easysetup_x.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <oic_string.h>
#include "oic_malloc.h"

#define TAG "ENROLLEE_SAMPLE"

void *listeningFunc(void *);
pthread_t thread_handle = NULL;

/**
 * Secure Virtual Resource database for Iotivity Server
 * It contains Server's Identity and the PSK credentials
 * of other devices which the server trusts
 */
#ifdef MULTIPLE_OWNER
static char CRED_FILE[] = "oic_svr_db_server_MOT.dat";
#else
static char CRED_FILE[] = "oic_svr_db_server.dat";
#endif

OCPersistentStorage ps;

/**
 * @var gIsSecured
 * @brief Variable to check if secure mode is enabled or not.
 */
static bool gIsSecured = false;

UserProperties g_userProperties;

void PrintMenu()
{
    printf("============\n");
    printf("S: Enabled Security\n");
    printf("I: Init & Start EasySetup\n");
    printf("D: Set DeviceInfo\n");
    printf("T: Terminate\n");
    printf("U: set Callback for userdata\n");
    printf("Q: Quit\n");
    printf("============\n");
}

void ConnectRequestCbInApp(ESConnectRequest *connectRequest)
{
    printf("ConnectRequestCbInApp IN\n");

    if(connectRequest == NULL)
    {
        printf("connectRequest is NULL\n");
        return ;
    }

    for(int i = 0 ; i < connectRequest->numRequest ; ++i)
    {
        printf("connect : %d\n", connectRequest->connect[i]);
    }

    printf("ConnectRequestCbInApp OUT\n");
}

void WiFiConfProvCbInApp(ESWiFiConfData *eventData)
{
    printf("WiFiConfProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESWiFiConfData is NULL\n");
        return ;
    }

    printf("SSID : %s\n", eventData->ssid);
    printf("Password : %s\n", eventData->pwd);
    printf("AuthType : %d\n", eventData->authtype);
    printf("EncType : %d\n", eventData->enctype);

    if(eventData->userdata != NULL)
    {
        printf("userValue : %d\n", ((UserProperties *)(eventData->userdata))->userValue_int);
    }

    printf("WiFiConfProvCbInApp OUT\n");
}

void DevConfProvCbInApp(ESDevConfData *eventData)
{
    printf("DevConfProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESDevConfData is NULL\n");
        return ;
    }

    printf("Language : %s\n", eventData->language);
    printf("Country : %s\n", eventData->country);

    printf("DevConfProvCbInApp OUT\n");
}

void CoapCloudConfProvCbInApp(ESCoapCloudConfData *eventData)
{
    printf("CoapCloudConfProvCbInApp IN\n");

    if(eventData == NULL)
    {
        printf("ESCoapCloudConfData is NULL\n");
        return ;
    }

    printf("AuthCode : %s\n", eventData->authCode);
    printf("AcessToken : %s\n", eventData->accessToken);
    printf("AcessTokenType : %d\n", eventData->accessTokenType);
    printf("AuthProvider : %s\n", eventData->authProvider);
    printf("CI Server : %s\n", eventData->ciServer);

    printf("CoapCloudConfProvCbInApp OUT\n");
}

ESProvisioningCallbacks gCallbacks = {
    .ConnectRequestCb = &ConnectRequestCbInApp,
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
        {{WIFI_11G, WIFI_11N, WIFI_11AC, WiFi_EOF}, WIFI_5G}, {"Test Device"}
    };

    // Set user properties if needed
    char userValue_str[] = "user_str";
    g_userProperties.userValue_int = 0;

    strncpy(g_userProperties.userValue_str, userValue_str, strlen(userValue_str) + 1);
    SetUserProperties(&g_userProperties);

    if(ESSetDeviceProperty(&deviceProperty) == ES_ERROR)
    {
        printf("ESSetDeviceProperty Error\n");
    }

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

    if (0 != pthread_cancel(thread_handle))
    {
        printf("Thread cancellation failed\n");
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

void SetCallbackForUserdata()
{
    ESSetCallbackForUserdata(&ReadUserdataCb, &WriteUserdataCb);
}

int main()
{
    printf("#########################\n");
    printf("EasySetup Enrollee SAMPLE\n");
    printf("#########################\n");
    PrintMenu();
    char option = '\0';

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
                case 'U': // set callback
                case 'u':
                    SetCallbackForUserdata();
                    PrintMenu();
                    break;

                default:
                    printf("wrong option\n");
                    PrintMenu();
                    break;
            }
            if (option == 'Q' || option == 'q')
            {
                break;
            }
        }
    }
    return 0;
}

void *listeningFunc(void * data)
{
    (void)data;
    OCStackResult result = OC_STACK_ERROR;

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

