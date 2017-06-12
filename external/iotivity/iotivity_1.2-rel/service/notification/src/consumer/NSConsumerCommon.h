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

#ifndef _NS_CONSUMER_COMMON_H_
#define _NS_CONSUMER_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <stdbool.h>

#include "NSConsumerInterface.h"
#include "NSStructs.h"
#include "ocstack.h"

#define NS_QOS OC_HIGH_QOS
#define NS_RESOURCE_TYPE "x.org.iotivity.notification"
#define NS_RESOURCE_URI "/notification"
#define NS_INTERFACE_BASELINE "oic.if.baseline"
#define NS_RESOURCE_QUERY "/oic/res"

#define NS_DISCOVER_QUERY "/oic/res?rt=x.org.iotivity.notification"
#define NS_DEVICE_ID_LENGTH 37

typedef enum
{
    NS_DISCOVER_DEFAULT, // will work for adapter_ip.
    NS_DISCOVER_UDP,
    NS_DISCOVER_TCP,
    NS_DISCOVER_CLOUD
} NSConsumerDiscoverType;

typedef enum
{
    NS_SELECTION_CONSUMER = 0,
    NS_SELECTION_PROVIDER = 1
} NSSelector;

typedef struct NSProviderConnectionInfo
{
    OCDevAddr * addr;

    OCDoHandle messageHandle;
    OCDoHandle syncHandle;

    bool isCloudConnection;
    bool isSubscribing;

    struct NSProviderConnectionInfo * next;

} NSProviderConnectionInfo;

typedef struct
{
    char providerId[NS_DEVICE_ID_LENGTH];

    NSTopicLL * topicLL;

    char * messageUri;
    char * syncUri;
    char * topicUri;

    NSSelector accessPolicy;
    NSProviderState state;

    NSProviderConnectionInfo * connection;

} NSProvider_internal;

typedef struct
{
    uint64_t messageId;
    char providerId[NS_DEVICE_ID_LENGTH];
    NSSyncType state;

    NSProviderConnectionInfo * connection;

} NSSyncInfo_internal;

bool NSIsStartedConsumer();
void NSSetIsStartedConsumer(bool setValue);

void NSSetProviderChangedCb(NSProviderStateCallback cb);
void NSProviderChanged(NSProvider * provider, NSProviderState response);

void NSSetMessagePostedCb(NSMessageReceivedCallback  cb);
void NSMessagePost(NSMessage * obj);

void NSSetNotificationSyncCb(NSSyncInfoReceivedCallback cb);
void NSNotificationSync(NSSyncInfo * sync);

char ** NSGetConsumerId();
void NSSetConsumerId(char * cId);

char * NSMakeRequestUriWithConsumerId(const char * uri);

NSTask * NSMakeTask(NSTaskType, void *);
NSResult NSConsumerPushEvent(NSTask *);

NSMessage * NSGetMessage(OCRepPayload * payload);
NSMessage * NSCopyMessage(NSMessage *);
void NSRemoveMessage(NSMessage *);

void NSGetProviderPostClean(
        char * pId, char * mUri, char * sUri, char * tUri, NSProviderConnectionInfo * connection);

NSProvider_internal * NSGetProvider(OCClientResponse * clientResponse);
NSProviderConnectionInfo * NSCreateProviderConnections(OCDevAddr *);
NSProviderConnectionInfo * NSCopyProviderConnections(NSProviderConnectionInfo *);
void NSRemoveConnections(NSProviderConnectionInfo *);

NSProvider_internal * NSCopyProvider_internal(NSProvider_internal *);
NSProvider * NSCopyProvider(NSProvider_internal *);
void NSRemoveProvider_internal(void *);
void NSRemoveProvider(NSProvider *);

NSTopicLL * NSCopyTopicNode(NSTopicLL *);
void NSRemoveTopicNode(NSTopicLL *);
NSResult NSInsertTopicNode(NSTopicLL *, NSTopicLL *);

NSTopicLL * NSCopyTopicLL(NSTopicLL *);
void NSRemoveTopicLL(NSTopicLL *);

OCStackResult NSInvokeRequest(OCDoHandle * handle,
        OCMethod method, const OCDevAddr * addr,
        const char * queryUrl, OCPayload * payload,
        void * callbackFunc, void * callbackData,
        OCClientContextDeleter cd, OCConnectivityType type);

bool NSOCResultToSuccess(OCStackResult ret);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _NS_CONSUMER_COMMON_H_
