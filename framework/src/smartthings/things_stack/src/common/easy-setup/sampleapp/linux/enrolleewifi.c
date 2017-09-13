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

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#include "oicthread.h"

#define STR_INPUT_BUF       255
#define szType              0x09



void *listeningFunc(void*);

/**
 * @var ssid
 * @brief Target SSID of the Soft Access point to which the device has to connect
 */
static char ssid[] = "EasySetup123";

/**
 * @var passwd
 * @brief Password of the Soft Access point to which the device has to connect
 */
static char passwd[] = "EasySetup123";

/**
 * Secure Virtual Resource database for Iotivity Server
 * It contains Server's Identity and the PSK credentials
 * of other devices which the server trusts
 */
static char CRED_FILE[] = "oic_svr_db_server_justworks.json";

OCPersistentStorage ps ;


/**
 * @var gIsSecured
 * @brief Variable to check if secure mode is enabled or not.
 */
static bool gIsSecured = false;

void PrintMenu()
{
    printf("============\n");
    printf("S: Enabled Security\n");
    printf("I: Init easy setup\n");
    printf("P: start provisioning resources\n");
    printf("T: terminate\n");
    printf("Q: quit\n");
    printf("============\n");
}

void EventCallbackInApp(ESResult esResult, ESEnrolleeState enrolleeState, ESEventData eventData)
{
    printf("Easy setup event callback\n");

    if(esResult == ES_OK)
    {
        if(enrolleeState == ES_ON_BOARDED_STATE)
        {
            printf("Device is successfully OnBoared on Adhoc network\n");
        }
        else if (enrolleeState == ES_PROVISIONED_STATE)
        {
            ESProvisionedData * data = (ESProvisionedData *) eventData;
            printf("============\n");
            printf("SSID:           %s\n", data->networkData.WIFI.ssid);
            printf("Cred:           %s\n", data->networkData.WIFI.pwd);
            printf("AuthType:       %d\n", data->networkData.WIFI.authType);
            printf("EncType:        %d\n", data->networkData.WIFI.encType);
            printf("AuthCode:       %s\n", data->cloudData.authCode);
            printf("AuthServerUri:  %s\n", data->cloudData.authServerUrl);
            printf("APIServerUri:   %s\n", data->cloudData.apiServerUrl);
            printf("============\n");
            printf("Device is provisioned with target network's credentials\n");
        }
        else if (enrolleeState == ES_ON_BOARDED_TARGET_NETWORK_STATE)
        {
            printf("Device is onboarded/connected with target network\n");
        }
        else
        {
            printf("Wrong state !! Easy setup is failed at Enrollee state\n");
        }
    }
    else
    {
        printf("Easy stup is failed at Enrollee state\n");
    }

    PrintMenu();
}

FILE* server_fopen(const char *path, const char *mode)
{
    (void) path;
    return fopen(CRED_FILE, mode);
}

char* ReadStringInput(const char *comment)
{
    static char buf[STR_INPUT_BUF];
    printf("\t%s: ", comment);
    if (fgets(buf, STR_INPUT_BUF, stdin) != NULL)
    {
        buf[strlen(buf)-1] = '\0';
    }
    else
    {
        buf[0] = '\0';
    }
    return buf;
}

void EnableSecurity()
{
    printf("Inside EnableSecurity API..\n");

    gIsSecured = true;

    // // Initialize Persistent Storage for SVR database
    // ps = { server_fopen, fread, fwrite, fclose, unlink };
    // OCRegisterPersistentStorageHandler(&ps);
}

void StartEasySetup()
{
    printf("StartEasySetup and onboarding started..\n");

    if(ESInitEnrollee(CT_ADAPTER_IP, ssid, passwd, gIsSecured, EventCallbackInApp) == ES_ERROR)
    {
        printf("StartEasySetup and onboarding Fail!!\n");
        return;
    }
}

void ESInitResources()
{
    printf("Starting Enrollee Provisioning\n");

    // Initialize the OC Stack in Server mode
    if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK)
    {
        printf("OCStack init error!!\n");
        return;
    }

    if (ESInitProvisioning() == ES_ERROR)
    {
        printf("Init Provisioning Failed!!\n");
        return;
    }

    pthread_t thread_handle;
    if (OICThread_create(&thread_handle, NULL, listeningFunc, NULL))
    {
        printf("Thread creation failed\n");
    }

    printf("ESInitProvisioning Success\n");
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
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    PrintMenu();
    char option;

    while(true)
    {
        scanf("%c", &option);
        switch (option)
        {
            case 'H': // help
            case 'h':
                PrintMenu();
                break;

            case 'Q': // quit
            case 'q':
                printf("quit\n");
                break;
            
            case 'S': // Enable Security
            case 's':
                EnableSecurity();
                break;

            case 'I': // Init EasySetup
            case 'i':
                StartEasySetup();
                break;

            case 'P': // start provisioning
            case 'p':
                ESInitResources();
                break;

            case 'T': // stop easy setup
            case 't':
                StopEasySetup();
                break;

            default:
                printf("wrong option\n");
                break;
        }
        if (option == 'Q' || option == 'q') break;
    }
    return 0;
}

void *listeningFunc(void* prt)
{
    OCStackResult result;

    while (true)
    {
        result = OCProcess();
        if (result != OC_STACK_OK)
        {
           printf("OCStack stop error\n");
        }
    }
    return NULL;
}

