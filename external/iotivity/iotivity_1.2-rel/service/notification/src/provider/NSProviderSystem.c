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
#include "NSProviderSystem.h"

#if (defined WITH_CLOUD)
#define MAX_SERVER_ADDRESS 32
static char NSRemoteServerAddress[MAX_SERVER_ADDRESS] = {0,};
#endif

#ifdef WITH_MQ
static NSMQServerInfo * mqServerInfo = NULL;
#endif

static NSConnectionState NSProviderConnectionState;

NSProviderInfo * providerInfo;
bool NSPolicy = true;
bool NSResourceSecurity = false;

void NSSetProviderConnectionState(NSConnectionState state)
{
    NS_LOG(DEBUG, "NSSetProviderConnectionState");

    NSProviderConnectionState = state;
}

NSConnectionState NSGetProviderConnectionState()
{
    NS_LOG(DEBUG, "Change Connection State");

    return NSProviderConnectionState;
}

#if (defined WITH_CLOUD)
void NSSetRemoteServerAddress(char *serverAddress)
{

    OICStrcpy(NSRemoteServerAddress, MAX_SERVER_ADDRESS, serverAddress);
}

void NSDeleteRemoteServerAddress(char *serverAddress)
{
    NS_LOG_V(INFO_PRIVATE, "Delete cloud address: %s", serverAddress);

    memset(NSRemoteServerAddress, 0, MAX_SERVER_ADDRESS);
}

bool NSIsRemoteServerAddress(char *serverAddress)
{
    NS_LOG_V(INFO_PRIVATE, "Check server address: %s", serverAddress);

    if (serverAddress != NULL)
    {
        return strstr(NSRemoteServerAddress, serverAddress);
    }

    return false;
}
#endif

void NSInitProviderInfo(const char * userInfo)
{
    NS_LOG(DEBUG, "NSInitProviderInfo");

    providerInfo = (NSProviderInfo *) OICMalloc(sizeof(NSProviderInfo));
    const char * generatedUuid = (char *)OCGetServerInstanceIDString();
    NS_LOG_V(INFO_PRIVATE, "Generate Provider ID: %s", generatedUuid);
    OICStrcpy(providerInfo->providerId, UUID_STRING_SIZE, generatedUuid);

    providerInfo->providerName = NULL;
    providerInfo->userInfo = NULL;

    if (userInfo)
    {
        providerInfo->userInfo = OICStrdup(userInfo);
    }
}

void NSDeinitProviderInfo()
{
    NS_LOG(DEBUG, "NSDeinitProviderInfo");

    if (!providerInfo)
    {
        NS_LOG(DEBUG, "providerInfo is NULL");
        return;
    }

    if (providerInfo->providerName)
    {
        NSOICFree(providerInfo->providerName);
        providerInfo->providerName = NULL;
    }

    if (providerInfo->userInfo)
    {
        NSOICFree(providerInfo->userInfo);
        providerInfo->userInfo = NULL;
    }

    NSOICFree(providerInfo);
    providerInfo = NULL;
}

NSProviderInfo * NSGetProviderInfo()
{
    NS_LOG_V(INFO_PRIVATE, "ProviderInfo: %s", providerInfo->providerId);

    return providerInfo;
}

bool NSGetPolicy()
{
    return NSPolicy;
}

void NSSetPolicy(bool policy)
{
    NSPolicy = policy;
}

bool NSGetResourceSecurity()
{
    return NSResourceSecurity;
}

void NSSetResourceSecurity(bool secured)
{
    NSResourceSecurity = secured;
}

const char * NSGetUserInfo()
{
    return providerInfo->userInfo;
}

#ifdef WITH_MQ
void NSSetMQServerInfo(const char * serverUri, OCDevAddr * devAddr)
{
    if (!mqServerInfo)
    {
        NS_LOG(DEBUG, "setMqServer");
        mqServerInfo = (NSMQServerInfo *)OICMalloc(sizeof(NSMQServerInfo));
        mqServerInfo->serverUri = OICStrdup(serverUri);
        mqServerInfo->devAddr = (OCDevAddr *)OICMalloc(sizeof(OCDevAddr));
        memcpy(mqServerInfo->devAddr, devAddr, sizeof(OCDevAddr));
    }
}

NSMQServerInfo * NSGetMQServerInfo()
{
    return mqServerInfo;
}
#endif
