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

#include "NSConsumerCommon.h"
#include "NSConstants.h"
#include "NSThread.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"

#include <pthread.h>

static bool NSIsExtraValue(const char * name);
static void NSCopyPayloadValueArray(OCRepPayloadValue* dest, OCRepPayloadValue* source);
static OCRepPayloadValue * NSCopyPayloadValue(OCRepPayloadValue * value);

pthread_mutex_t ** NSGetStackMutex()
{
    static pthread_mutex_t * g_stackMutext = NULL;
    if (g_stackMutext == NULL)
    {
        g_stackMutext = (pthread_mutex_t *)OICMalloc(sizeof(pthread_mutex_t));
        NS_VERIFY_NOT_NULL(g_stackMutext, NULL);
        pthread_mutex_init(g_stackMutext, NULL);
    }

    return & g_stackMutext;
}

char ** NSGetConsumerId()
{
    static char * g_consumerId = NULL;
    return & g_consumerId;
}

void NSSetConsumerId(char * cId)
{
    NS_VERIFY_NOT_NULL_V(cId);
    char ** consumerId = NSGetConsumerId();
    NSOICFree(*consumerId);
    *consumerId = (char *)OICMalloc(sizeof(char) * NS_DEVICE_ID_LENGTH);
    NS_VERIFY_NOT_NULL_V(*consumerId);

    OICStrcpy(*consumerId, sizeof(char) * NS_DEVICE_ID_LENGTH, cId);
}

char * NSMakeRequestUriWithConsumerId(const char * uri)
{
    NS_VERIFY_NOT_NULL(uri, NULL);

    char * consumerId = OICStrdup(*NSGetConsumerId());
    NS_VERIFY_NOT_NULL(consumerId, NULL);

    size_t uriLen = strlen(uri) + 1;
    size_t qKeyLen = sizeof(NS_QUERY_CONSUMER_ID);
    size_t queryLen = NS_DEVICE_ID_LENGTH + uriLen + qKeyLen + 2;

    char * retQuery = (char *)OICMalloc(sizeof(char) * queryLen);
    NS_VERIFY_NOT_NULL(retQuery, NULL);

    size_t index = 0;
    OICStrcpy((retQuery + index), uriLen, uri);
    index += uriLen - 1;
    OICStrcpy((retQuery + index), 2, "?");
    index += 1;
    OICStrcpy((retQuery + index), qKeyLen, NS_QUERY_CONSUMER_ID);
    index += qKeyLen - 1;
    OICStrcpy((retQuery + index), 2, "=");
    index += 1;
    OICStrcpy((retQuery + index), NS_DEVICE_ID_LENGTH, consumerId);

    NSOICFree(consumerId);

    return retQuery;
}

bool * NSGetBoneIsStartedConsumer()
{
    static bool g_isStartedConsumer = false;

    return & g_isStartedConsumer;
}

void NSSetIsStartedConsumer(bool setValue)
{
    * NSGetBoneIsStartedConsumer() = setValue;

    if (setValue == false)
    {
        pthread_mutex_destroy(*NSGetStackMutex());
        NSOICFree(*NSGetStackMutex());
        *NSGetStackMutex() = NULL;

        NSOICFree(*NSGetConsumerId());
        *NSGetConsumerId() = NULL;
    }
}

bool NSIsStartedConsumer()
{
    return * NSGetBoneIsStartedConsumer();
}

NSProviderStateCallback * NSGetProviderChangedCb()
{
    static NSProviderStateCallback g_changedCb = NULL;

    return & g_changedCb;
}

void NSSetProviderChangedCb(NSProviderStateCallback cb)
{
    *(NSGetProviderChangedCb()) = cb;
}

typedef struct
{
    NSProvider * provider;
    NSProviderState state;
} NSProviderChangedData;

void * NSProviderChangedFunc(void * obj)
{
    NSProviderChangedData * data = (NSProviderChangedData *) obj;
    (*(NSGetProviderChangedCb()))(data->provider, data->state);
    NSOICFree(data);
    return NULL;
}

void NSProviderChanged(NSProvider * provider, NSProviderState response)
{
    NS_VERIFY_NOT_NULL_V(provider);

    NSProvider * retProvider = NSCopyProvider((NSProvider_internal *) provider);
    NS_VERIFY_NOT_NULL_V(retProvider);

    NSProviderChangedData * data =
            (NSProviderChangedData *)OICMalloc(sizeof(NSProviderChangedData));
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(data, NSRemoveProvider(retProvider));

    data->provider = retProvider;
    data->state = response;
#ifndef __TIZENRT__
    NSConsumerThread * thread = NSThreadInit(NSProviderChangedFunc, (void *) data);
#else
    NSConsumerThread * thread = NSThreadInit(NSProviderChangedFunc, (void *) data, "NSProviderChangedFunc", 1024);
#endif
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(thread,
    {
        NSRemoveProvider(retProvider);
        NSOICFree(data);
    });

    NSDestroyThreadHandle(thread);
    NSOICFree(thread);
}

NSSyncInfoReceivedCallback * NSGetBoneNotificationSyncCb()
{
    static NSSyncInfoReceivedCallback g_syncCb = NULL;

    return & g_syncCb;
}

void NSSetNotificationSyncCb(NSSyncInfoReceivedCallback cb)
{
    * NSGetBoneNotificationSyncCb() = cb;
}

void * NSNotificationSyncFunc(void * obj)
{
    (* NSGetBoneNotificationSyncCb())((NSSyncInfo *) obj);
    return NULL;
}

void NSNotificationSync(NSSyncInfo * sync)
{
    NS_VERIFY_NOT_NULL_V(sync);

    NSSyncInfo * retSync = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));
    NS_VERIFY_NOT_NULL_V(retSync);
    memcpy(retSync, sync, sizeof(NSSyncInfo));
#ifndef __TIZENRT__
    NSConsumerThread * thread = NSThreadInit(NSNotificationSyncFunc, (void *) retSync);
#else
    NSConsumerThread * thread = NSThreadInit(NSNotificationSyncFunc, (void *) retSync, "NSNotificationSyncFunc", 1024);
#endif
    NS_VERIFY_NOT_NULL_V(thread);

    NSDestroyThreadHandle(thread);
    NSOICFree(thread);
}

NSMessageReceivedCallback  * NSGetBoneMessagePostedCb()
{
    static NSMessageReceivedCallback  g_postCb = NULL;

    return & g_postCb;
}

void NSSetMessagePostedCb(NSMessageReceivedCallback  cb)
{
    * NSGetBoneMessagePostedCb() = cb;
}

NSMessageReceivedCallback  NSGetMessagePostedCb()
{
    return * NSGetBoneMessagePostedCb();
}

void * NSMessagePostFunc(void * obj)
{
    NSGetMessagePostedCb()((NSMessage *) obj);
    return NULL;
}

void NSMessagePost(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL_V(msg);

    NSMessage * retMsg = NSCopyMessage(msg);
    NS_VERIFY_NOT_NULL_V(retMsg);
#ifndef __TIZENRT__
    NSConsumerThread * thread = NSThreadInit(NSMessagePostFunc, (void *) retMsg);
#else
    NSConsumerThread * thread = NSThreadInit(NSMessagePostFunc, (void *) retMsg, "NSMessagePostFunc", 1024);
#endif
    NS_VERIFY_NOT_NULL_V(thread);

    NSDestroyThreadHandle(thread);
    NSOICFree(thread);
}

NSTask * NSMakeTask(NSTaskType type, void * data)
{
    NSTask * retTask = OICMalloc(sizeof(NSTask));
    NS_VERIFY_NOT_NULL(retTask, NULL);

    retTask->taskType = type;
    retTask->taskData = data;
    retTask->nextTask = NULL;

    return retTask;
}

static NSMessage * NSCreateMessage_internal(uint64_t id, const char * providerId)
{
    NSMessage * retMsg = (NSMessage *)OICMalloc(sizeof(NSMessage));
    NS_VERIFY_NOT_NULL(retMsg, NULL);

    retMsg->messageId = id;
    OICStrcpy(retMsg->providerId, sizeof(char) * NS_DEVICE_ID_LENGTH, providerId);
    retMsg->title = NULL;
    retMsg->contentText = NULL;
    retMsg->sourceName = NULL;
    retMsg->topic = NULL;
    retMsg->type = NS_MESSAGE_INFO;
    retMsg->dateTime = NULL;
    retMsg->ttl = 0;
    retMsg->mediaContents = NULL;
    retMsg->extraInfo = NULL;

    return retMsg;
}

static OCRepPayload * NSGetExtraInfo(OCRepPayload * payload)
{
    NS_LOG(DEBUG, "get extra info");
    OCRepPayload * extraInfo = OCRepPayloadCreate();
    NS_VERIFY_NOT_NULL(extraInfo, NULL);
    OCRepPayload * origin = OCRepPayloadClone(payload);

    bool isFirstExtra = true;
    OCRepPayloadValue * headValue = NULL;
    OCRepPayloadValue * curValue = NULL;
    OCRepPayloadValue * value = origin->values;
    while(value)
    {
        if (NSIsExtraValue(value->name))
        {
            curValue = NSCopyPayloadValue(value);
            NS_LOG_V(DEBUG, " key : %s", curValue->name);
            if (isFirstExtra)
            {
                headValue = curValue;
                extraInfo->values = headValue;
                isFirstExtra = false;
            }
            else
            {
                headValue->next = curValue;
                headValue = curValue;
            }
            curValue = NULL;
        }
        value = value->next;
    }
    OCRepPayloadDestroy(origin);


    if (!isFirstExtra && extraInfo->values)
    {
        return extraInfo;
    }
    else
    {
        OCRepPayloadDestroy(extraInfo);
        return NULL;
    }
}

NSMessage * NSGetMessage(OCRepPayload * payload)
{
    NS_LOG(DEBUG, "get msg id");
    uint64_t id = NULL;
    bool getResult = OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, (int64_t *)&id);
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "get provider id");
    char * pId = NULL;
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, &pId);
    NS_LOG_V (INFO_PRIVATE, "provider id: %s", pId);
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "create NSMessage");
    NSMessage * retMsg = NSCreateMessage_internal(id, pId);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(retMsg, NULL, NSOICFree(pId));
    NSOICFree(pId);

    NS_LOG(DEBUG, "get msg optional field");
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_TITLE, &retMsg->title);
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_TEXT, &retMsg->contentText);
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_SOURCE, &retMsg->sourceName);
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_TOPIC_NAME, &retMsg->topic);

    OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_TYPE, (int64_t *)&retMsg->type);
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_DATETIME, &retMsg->dateTime);
    OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_TTL, (int64_t *)&retMsg->ttl);

    char * icon = NULL;
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_ICON_IMAGE, &icon);

    if (icon && strlen(icon))
    {
        NSMediaContents * contents = (NSMediaContents *)OICMalloc(sizeof(NSMediaContents));
        if (contents)
        {
            contents->iconImage = icon;
            retMsg->mediaContents = contents;
        }
        else
        {
            NSOICFree(icon);
        }
    }

    retMsg->extraInfo = NSGetExtraInfo(payload);

    NS_LOG_V(DEBUG, "Msg ID      : %lld", (long long int)retMsg->messageId);
    NS_LOG_V(DEBUG, "Msg Title   : %s", retMsg->title);
    NS_LOG_V(DEBUG, "Msg Content : %s", retMsg->contentText);
    NS_LOG_V(DEBUG, "Msg Source  : %s", retMsg->sourceName);
    NS_LOG_V(DEBUG, "Msg Topic   : %s", retMsg->topic);
    NS_LOG_V(DEBUG, "Msg Type    : %d", retMsg->type);
    NS_LOG_V(DEBUG, "Msg Date    : %s", retMsg->dateTime);
    NS_LOG_V(DEBUG, "Msg ttl     : %lld", (long long int)retMsg->ttl);

    return retMsg;
}

NSMessage * NSCopyMessage(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL(msg, NULL);

    NSMessage * newMsg = (NSMessage *)OICMalloc(sizeof(NSMessage));
    NS_VERIFY_NOT_NULL(newMsg, NULL);

    OICStrcpy(newMsg->providerId, NS_DEVICE_ID_LENGTH, msg->providerId);

    newMsg->messageId = msg->messageId;
    newMsg->title = OICStrdup(msg->title);
    newMsg->contentText = OICStrdup(msg->contentText);
    newMsg->sourceName = OICStrdup(msg->sourceName);
    newMsg->dateTime = OICStrdup(msg->dateTime);
    newMsg->type = msg->type;
    newMsg->ttl= msg->ttl;

    newMsg->topic = NULL;
    if (msg->topic && strlen(msg->topic) > 0)
    {
        newMsg->topic = OICStrdup(msg->topic);
    }

    newMsg->mediaContents = NULL;
    if (msg->mediaContents)
    {
        newMsg->mediaContents = (NSMediaContents *)OICMalloc(sizeof(NSMediaContents));
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(
                newMsg->mediaContents, NULL, NSRemoveMessage(newMsg));
        newMsg->mediaContents->iconImage =
                (char *)OICMalloc(sizeof(char)*strlen(msg->mediaContents->iconImage) + 1);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(
                newMsg->mediaContents->iconImage, NULL, NSRemoveMessage(newMsg));
        memcpy(newMsg->mediaContents->iconImage, msg->mediaContents->iconImage,
               strlen(msg->mediaContents->iconImage) + 1);
    }

    newMsg->extraInfo = NULL;
    if (msg->extraInfo)
    {
        newMsg->extraInfo = OCRepPayloadClone(msg->extraInfo);
    }

    return newMsg;
}
void NSRemoveMessage(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL_V(msg);

    msg->messageId = 0;
    NSOICFree(msg->title);
    NSOICFree(msg->contentText);
    NSOICFree(msg->sourceName);
    NSOICFree(msg->dateTime);
    NSOICFree(msg->topic);

    if (msg->mediaContents)
    {
        NSOICFree(msg->mediaContents->iconImage);
    }
    NSOICFree(msg->mediaContents);

    if (msg->extraInfo)
    {
        OCRepPayloadDestroy(msg->extraInfo);
        msg->extraInfo = NULL;
    }

    NSOICFree(msg);
}

void NSGetProviderPostClean(
        char * pId, char * mUri, char * sUri, char * tUri, NSProviderConnectionInfo * connection)
{
    NSOICFree(pId);
    NSOICFree(mUri);
    NSOICFree(sUri);
    NSOICFree(tUri);
    NSRemoveConnections(connection);
}

NSProvider_internal * NSGetProvider(OCClientResponse * clientResponse)
{
    NS_LOG(DEBUG, "create NSProvider");
    NS_VERIFY_NOT_NULL(clientResponse->payload, NULL);

    OCRepPayloadPropType accepterType = OCREP_PROP_BOOL;

    OCRepPayload * payload = (OCRepPayload *)clientResponse->payload;
    OCRepPayloadValue * value = payload->values;
    while (value)
    {
        NS_LOG_V(DEBUG, "Payload Key : %s", value->name);
        NS_LOG_V(DEBUG, "Payload Type : %d", (int) value->type);
        if (!strcmp(value->name, NS_ATTRIBUTE_POLICY))
        {
            accepterType = value->type;
        }
        value = value->next;
    }

    char * providerId = NULL;
    char * messageUri = NULL;
    char * syncUri = NULL;
    char * topicUri = NULL;
    bool bAccepter = 0;
    int64_t iAccepter = 0;
    NSProviderConnectionInfo * connection = NULL;

    NS_LOG(DEBUG, "get information of accepter");
    bool getResult = false;
    if (accepterType == OCREP_PROP_BOOL)
    {
        getResult = OCRepPayloadGetPropBool(payload, NS_ATTRIBUTE_POLICY, & bAccepter);
    }
    else if (accepterType == OCREP_PROP_INT)
    {
        getResult = OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_POLICY, & iAccepter);
    }
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "get provider ID");
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, & providerId);
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "get message URI");
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_MESSAGE, & messageUri);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(getResult == true ? (void *) 1 : NULL, NULL,
            NSGetProviderPostClean(providerId, messageUri, syncUri, topicUri, connection));

    NS_LOG(DEBUG, "get sync URI");
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_SYNC, & syncUri);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(getResult == true ? (void *) 1 : NULL, NULL,
            NSGetProviderPostClean(providerId, messageUri, syncUri, topicUri, connection));

    NS_LOG(DEBUG, "get topic URI");
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_TOPIC, & topicUri);

    NS_LOG(DEBUG, "get provider connection information");
    NS_VERIFY_NOT_NULL(clientResponse->addr, NULL);
    connection = NSCreateProviderConnections(clientResponse->addr);
    NS_VERIFY_NOT_NULL(connection, NULL);

    NSProvider_internal * newProvider
        = (NSProvider_internal *)OICMalloc(sizeof(NSProvider_internal));
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(newProvider, NULL,
          NSGetProviderPostClean(providerId, messageUri, syncUri, topicUri, connection));

    OICStrcpy(newProvider->providerId, sizeof(char) * NS_DEVICE_ID_LENGTH, providerId);
    NSOICFree(providerId);
    newProvider->messageUri = messageUri;
    newProvider->syncUri = syncUri;
    newProvider->topicUri = NULL;
    if (topicUri && strlen(topicUri) > 0)
    {
        newProvider->topicUri = topicUri;
    }
    if (accepterType == OCREP_PROP_BOOL)
    {
        newProvider->accessPolicy = (NSSelector)bAccepter;
    }
    else if (accepterType == OCREP_PROP_INT)
    {
        newProvider->accessPolicy = (NSSelector)iAccepter;
    }

    newProvider->connection = connection;
    newProvider->topicLL = NULL;
    newProvider->state = NS_DISCOVERED;

    return newProvider;
}

void NSRemoveConnections(NSProviderConnectionInfo * connections)
{
    NS_VERIFY_NOT_NULL_V(connections);

    NSProviderConnectionInfo * tmp = connections;

    while(tmp)
    {
        tmp->messageHandle = NULL;
        tmp->syncHandle = NULL;
        NSOICFree(tmp->addr);
        NSProviderConnectionInfo * next = tmp->next;
        NSOICFree(tmp);
        tmp = next;
    }
}

NSProviderConnectionInfo * NSCreateProviderConnections(OCDevAddr * inAddr)
{
    NSProviderConnectionInfo * connections
        = (NSProviderConnectionInfo *)OICMalloc(sizeof(NSProviderConnectionInfo));
    NS_VERIFY_NOT_NULL(connections, NULL);

    connections->addr = NULL;
    connections->messageHandle = NULL;
    connections->syncHandle = NULL;
    connections->isCloudConnection = false;
    connections->isSubscribing = false;
    connections->next = NULL;

    if (inAddr)
    {
        connections->addr = (OCDevAddr *)OICMalloc(sizeof(OCDevAddr));
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(connections->addr, NULL, NSRemoveConnections(connections));
        memcpy(connections->addr, inAddr, sizeof(OCDevAddr));
    }

    return connections;
}

NSProviderConnectionInfo * NSCopyProviderConnections(NSProviderConnectionInfo * conn)
{
    NS_VERIFY_NOT_NULL(conn, NULL);
    NSProviderConnectionInfo * tmp = conn;

    NSProviderConnectionInfo * retInfo = NSCreateProviderConnections(tmp->addr);
    NS_VERIFY_NOT_NULL(retInfo, NULL);
    retInfo->messageHandle = tmp->messageHandle;
    retInfo->syncHandle = tmp->syncHandle;
    retInfo->isCloudConnection = tmp->isCloudConnection;
    retInfo->isSubscribing = tmp->isSubscribing;

    tmp = tmp->next;
    NSProviderConnectionInfo * copyInfo = retInfo;

    while(tmp)
    {
        NSProviderConnectionInfo * tmpInfo = NSCreateProviderConnections(tmp->addr);
        NS_VERIFY_NOT_NULL(tmpInfo, NULL);

        tmpInfo->messageHandle = tmp->messageHandle;
        tmpInfo->syncHandle = tmp->syncHandle;
        tmpInfo->isCloudConnection = tmp->isCloudConnection;
        tmpInfo->isSubscribing = tmp->isSubscribing;
        tmp = tmp->next;
        copyInfo->next = tmpInfo;
        copyInfo = tmpInfo;
    }

    return retInfo;
}

void NSRemoveTopicNode(NSTopicLL * topicNode)
{
    NS_VERIFY_NOT_NULL_V(topicNode);

    NSOICFree(topicNode->topicName);
    topicNode->next = NULL;

    NSOICFree(topicNode);
}

NSTopicLL * NSCopyTopicNode(NSTopicLL * topicNode)
{
    NS_VERIFY_NOT_NULL(topicNode, NULL);

    NSTopicLL * newTopicNode = (NSTopicLL *)OICMalloc(sizeof(NSTopicLL));
    NS_VERIFY_NOT_NULL(newTopicNode, NULL);

    newTopicNode->topicName = OICStrdup(topicNode->topicName);
    newTopicNode->state = topicNode->state;
    newTopicNode->next = NULL;

    return newTopicNode;
}

NSResult NSInsertTopicNode(NSTopicLL * topicHead, NSTopicLL * topicNode)
{
    NS_VERIFY_NOT_NULL(topicHead, NS_ERROR);
    NS_VERIFY_NOT_NULL(topicNode, NS_ERROR);

    NSTopicLL * iter = topicHead;
    NSTopicLL * prev = NULL;

    while (iter)
    {
        prev = iter;
        iter = (NSTopicLL *) iter->next;
    }

    prev->next = topicNode;
    topicNode->next = NULL;

    return NS_OK;
}

void NSRemoveTopicLL(NSTopicLL * topicHead)
{
    NS_VERIFY_NOT_NULL_V(topicHead);

    NSTopicLL * iter = topicHead;
    NSTopicLL * following = NULL;

    while (iter)
    {
        following = iter->next;

        NSRemoveTopicNode(iter);

        iter = following;
    }
}

NSTopicLL * NSCopyTopicLL(NSTopicLL * topicHead)
{
    NS_VERIFY_NOT_NULL(topicHead, NULL);

    NSTopicLL * iter = topicHead;

    NS_LOG_V(DEBUG, "[NSCopyTopicLL] Name:%s\t State:%d", iter->topicName, iter->state);
    NSTopicLL * newTopicHead = NSCopyTopicNode(iter);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(newTopicHead, NULL, NSRemoveTopicLL(newTopicHead));

    iter = (NSTopicLL *) iter->next;

    while (iter)
    {
        NS_LOG_V(DEBUG, "[NSCopyTopicLL] Name:%s\t State:%d", iter->topicName, iter->state);
        NSTopicLL * newTopicNode = NSCopyTopicNode(iter);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(newTopicNode, NULL, NSRemoveTopicLL(newTopicHead));

        NSResult ret = NSInsertTopicNode(newTopicHead, newTopicNode);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(ret == NS_OK ? (void *)1 : NULL,
                                                    NULL, NSRemoveTopicLL(newTopicHead));

        iter = (NSTopicLL *) iter->next;
    }

    return newTopicHead;
}

void NSCopyProviderPostClean(
        NSProviderConnectionInfo * connections, NSProvider_internal * provider)
{
    NSRemoveConnections(connections);
    NSOICFree(provider);
}

NSProvider_internal * NSCopyProvider_internal(NSProvider_internal * prov)
{
    NS_VERIFY_NOT_NULL(prov, NULL);

    NSProviderConnectionInfo * connections = NSCopyProviderConnections(prov->connection);
    NS_VERIFY_NOT_NULL(connections, NULL);

    NSProvider_internal * newProv = (NSProvider_internal *) OICMalloc(sizeof(NSProvider_internal));
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(newProv, NULL, NSRemoveConnections(connections));

    newProv->topicLL = NULL;

    if (prov->topicLL)
    {
        NSTopicLL * newTopicLL = NSCopyTopicLL(prov->topicLL);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(newTopicLL, NULL,
                    NSCopyProviderPostClean(connections, newProv));

        newProv->topicLL = newTopicLL;
    }

    newProv->connection = connections;
    OICStrcpy(newProv->providerId, NS_DEVICE_ID_LENGTH, prov->providerId);
    newProv->messageUri = OICStrdup(prov->messageUri);
    newProv->syncUri = OICStrdup(prov->syncUri);
    newProv->topicUri = OICStrdup(prov->topicUri);
    newProv->accessPolicy = prov->accessPolicy;
    newProv->state = prov->state;

    return newProv;
}

NSProvider * NSCopyProvider(NSProvider_internal * prov)
{
    NS_VERIFY_NOT_NULL(prov, NULL);

    NSProvider * newProv = (NSProvider *) OICMalloc(sizeof(NSProvider));
    NS_VERIFY_NOT_NULL(newProv, NULL);

    OICStrcpy(newProv->providerId, NS_DEVICE_ID_LENGTH, prov->providerId);

    return newProv;
}

void NSRemoveProvider_internal(void * data)
{
    NS_VERIFY_NOT_NULL_V(data);

    NSProvider_internal * prov = (NSProvider_internal *) data;

    NSOICFree(prov->messageUri);
    NSOICFree(prov->syncUri);
    NSOICFree(prov->topicUri);
    NSRemoveConnections(prov->connection);
    if (prov->topicLL)
    {
        NSRemoveTopicLL(prov->topicLL);
    }

    NSOICFree(prov);
}

void NSRemoveProvider(NSProvider * prov)
{
    NS_VERIFY_NOT_NULL_V(prov);
    NSOICFree(prov);
}

OCStackResult NSInvokeRequest(OCDoHandle * handle,
        OCMethod method, const OCDevAddr * addr,
        const char * queryUrl, OCPayload * payload,
        void * callbackFunc, void * callbackData,
        OCClientContextDeleter cd, OCConnectivityType type)
{
    int mutexRet = pthread_mutex_lock(*(NSGetStackMutex()));
    NS_VERIFY_NOT_NULL(mutexRet != 0 ? NULL : (void *)1, OC_STACK_ERROR);

    OCCallbackData cbdata = { NULL, NULL, NULL };

    cbdata.cb = callbackFunc;
    cbdata.context = callbackData;
    cbdata.cd = cd;

    OCStackResult ret = OCDoResource(handle, method, queryUrl, addr,
                                     payload, type, NS_QOS, &cbdata, NULL, 0);

    mutexRet = pthread_mutex_unlock(*(NSGetStackMutex()));
    NS_VERIFY_NOT_NULL(mutexRet != 0 ? NULL : (void *)1, OC_STACK_ERROR);

    return ret;
}

bool NSIsExtraValue(const char * name)
{
    if (!strcmp(name, NS_ATTRIBUTE_MESSAGE_ID) ||
        !strcmp(name, NS_ATTRIBUTE_PROVIDER_ID) ||
        !strcmp(name, NS_ATTRIBUTE_TITLE) ||
        !strcmp(name, NS_ATTRIBUTE_TEXT) ||
        !strcmp(name, NS_ATTRIBUTE_SOURCE) ||
        !strcmp(name, NS_ATTRIBUTE_TOPIC_NAME) ||
        !strcmp(name, NS_ATTRIBUTE_TYPE) ||
        !strcmp(name, NS_ATTRIBUTE_DATETIME) ||
        !strcmp(name, NS_ATTRIBUTE_TTL) ||
        !strcmp(name, NS_ATTRIBUTE_ICON_IMAGE))
    {
        return false;
    }

    return true;
}


void NSCopyPayloadValueArray(OCRepPayloadValue* dest, OCRepPayloadValue* source)
{
    NS_VERIFY_NOT_NULL_V(source);

    size_t dimTotal = calcDimTotal(source->arr.dimensions);
    switch(source->arr.type)
    {
        case OCREP_PROP_INT:
            dest->arr.iArray = (int64_t*)OICMalloc(dimTotal * sizeof(int64_t));
            NS_VERIFY_NOT_NULL_V(dest->arr.iArray);
            memcpy(dest->arr.iArray, source->arr.iArray, dimTotal * sizeof(int64_t));
            break;
        case OCREP_PROP_DOUBLE:
            dest->arr.dArray = (double*)OICMalloc(dimTotal * sizeof(double));
            NS_VERIFY_NOT_NULL_V(dest->arr.dArray);
            memcpy(dest->arr.dArray, source->arr.dArray, dimTotal * sizeof(double));
            break;
        case OCREP_PROP_BOOL:
            dest->arr.bArray = (bool*)OICMalloc(dimTotal * sizeof(bool));
            NS_VERIFY_NOT_NULL_V(dest->arr.bArray);
            memcpy(dest->arr.bArray, source->arr.bArray, dimTotal * sizeof(bool));
            break;
        case OCREP_PROP_STRING:
            dest->arr.strArray = (char**)OICMalloc(dimTotal * sizeof(char*));
            NS_VERIFY_NOT_NULL_V(dest->arr.strArray);
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.strArray[i] = OICStrdup(source->arr.strArray[i]);
            }
            break;
        case OCREP_PROP_OBJECT:
            dest->arr.objArray = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));
            NS_VERIFY_NOT_NULL_V(dest->arr.objArray);
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.objArray[i] = OCRepPayloadClone(source->arr.objArray[i]);
            }
            break;
        case OCREP_PROP_ARRAY:
            dest->arr.objArray = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));
            NS_VERIFY_NOT_NULL_V(dest->arr.objArray);
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.objArray[i] = OCRepPayloadClone(source->arr.objArray[i]);
            }
            break;
        case OCREP_PROP_BYTE_STRING:
            dest->arr.ocByteStrArray = (OCByteString*)OICMalloc(dimTotal * sizeof(OCByteString));
            NS_VERIFY_NOT_NULL_V(dest->arr.ocByteStrArray);
            for (size_t i = 0; i < dimTotal; ++i)
            {
                OCByteStringCopy(&dest->arr.ocByteStrArray[i], &source->arr.ocByteStrArray[i]);
                NS_VERIFY_NOT_NULL_V(dest->arr.ocByteStrArray[i].bytes);
            }
            break;
        default:
            break;
    }
}

OCRepPayloadValue * NSCopyPayloadValue(OCRepPayloadValue * value)
{
    OCRepPayloadValue * retValue = (OCRepPayloadValue *)OICMalloc(sizeof(OCRepPayloadValue));
    NS_VERIFY_NOT_NULL(retValue, NULL);

    * retValue = * value;
    retValue->next = NULL;
    retValue->name = OICStrdup(value->name);

    switch(value->type)
    {
        case OCREP_PROP_STRING:
            retValue->str = OICStrdup(value->str);
            break;
        case OCREP_PROP_BYTE_STRING:
            retValue->ocByteStr.bytes = (uint8_t * )OICMalloc(value->ocByteStr.len * sizeof(uint8_t));
            NS_VERIFY_NOT_NULL(retValue->ocByteStr.bytes, NULL);
            retValue->ocByteStr.len = value->ocByteStr.len;
            memcpy(retValue->ocByteStr.bytes, value->ocByteStr.bytes, retValue->ocByteStr.len);
            break;
        case OCREP_PROP_OBJECT:
            retValue->obj = OCRepPayloadClone(value->obj);
            break;
        case OCREP_PROP_ARRAY:
            NSCopyPayloadValueArray(retValue, value);
            break;
        default:
            break;
    }

    return retValue;
}
