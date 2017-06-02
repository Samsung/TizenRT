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

#include "NSConstants.h"
#include "NSConsumerCommon.h"
#include "NSConsumerInternalTaskController.h"
#include "NSStructs.h"

#include "oic_malloc.h"
#include "oic_string.h"

#define NS_RESERVED_MESSAGEID 10

// MessageState storage structure
typedef struct _NSMessageStateLL
{
    uint64_t messageId;
    NSSyncType state;
    struct _NSMessageStateLL * next;

} NSMessageStateLL;

typedef struct
{
    NSMessageStateLL * head;
    NSMessageStateLL * tail;

} NSMessageStateList;

// Mutex of MessageState storage
pthread_mutex_t ** NSGetMessageListMutex();
void NSLockMessageListMutex();
void NSUnlockMessageListMutex();

// Function for MessageState
NSMessageStateList * NSGetMessageStateList();
NSMessageStateLL * NSFindMessageState(uint64_t msgId);
bool NSUpdateMessageState(uint64_t msgId, NSSyncType state);
bool NSDeleteMessageState(uint64_t msgId);
bool NSInsertMessageState(uint64_t msgId, NSSyncType state);
void NSDestroyMessageStateList();

NSCacheList ** NSGetProviderCacheList()
{
    static NSCacheList * providerCache = NULL;
    return & providerCache;
}

void NSSetProviderCacheList(NSCacheList * cache)
{
    *(NSGetProviderCacheList()) = cache;
}

void NSDestroyInternalCachedList()
{
    NSCacheList * cache = *(NSGetProviderCacheList());
    if (cache)
    {
        NSConsumerStorageDestroy(cache);
    }

    NSSetProviderCacheList(NULL);

    NSDestroyMessageStateList();
    pthread_mutex_destroy(*NSGetMessageListMutex());
    NSOICFree(*NSGetMessageListMutex());
}

NSProvider_internal * NSProviderCacheFind(const char * providerId)
{
    NS_VERIFY_NOT_NULL(providerId, NULL);

    NSCacheList * ProviderCache = *(NSGetProviderCacheList());
    if (!ProviderCache)
    {
        NS_LOG(DEBUG, "Provider Cache Init");
        ProviderCache = NSConsumerStorageCreate();
        NS_VERIFY_NOT_NULL(ProviderCache, NULL);

        ProviderCache->cacheType = NS_CONSUMER_CACHE_PROVIDER;
        NSSetProviderCacheList(ProviderCache);
    }

    NSCacheElement * cacheElement = NSConsumerStorageRead(ProviderCache, providerId);
    NS_VERIFY_NOT_NULL(cacheElement, NULL);

    return NSCopyProvider_internal((NSProvider_internal *) cacheElement->data);
}

NSProvider_internal * NSFindProviderFromAddr(OCDevAddr * addr)
{
    NS_VERIFY_NOT_NULL(addr, NULL);

    NSCacheList * ProviderCache = *(NSGetProviderCacheList());
    if (!ProviderCache)
    {
        NS_LOG(DEBUG, "Provider Cache does not intialized.");
        return NULL;
    }

    NSCacheElement * cacheElement =
            NSGetProviderFromAddr(ProviderCache, addr->addr, addr->port);

    NS_VERIFY_NOT_NULL(cacheElement, NULL);

    return NSCopyProvider_internal((NSProvider_internal *) cacheElement->data);
}

NSResult NSProviderCacheUpdate(NSProvider_internal * provider)
{
    NSCacheList * ProviderCache = *(NSGetProviderCacheList());
    if (!ProviderCache)
    {
        NS_LOG(DEBUG, "Provider Cache Init");
        ProviderCache = NSConsumerStorageCreate();
        NS_VERIFY_NOT_NULL(ProviderCache, NS_ERROR);

        ProviderCache->cacheType = NS_CONSUMER_CACHE_PROVIDER;
        NSSetProviderCacheList(ProviderCache);
    }

    NS_VERIFY_NOT_NULL(provider, NS_ERROR);

    NSCacheElement * obj = (NSCacheElement *)OICMalloc(sizeof(NSCacheElement));
    NS_VERIFY_NOT_NULL(obj, NS_ERROR);

    obj->data = (NSCacheData *) provider;
    obj->next = NULL;

    NS_LOG(DEBUG, "try to write to storage");
    NSResult ret = NSConsumerStorageWrite(ProviderCache, obj);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(ret == NS_OK ? (void *) 1 : NULL,
            NS_ERROR, NSOICFree(obj));

    NSOICFree(obj);

    return NS_OK;
}

void NSCancelAllSubscription()
{
    NSCacheList * ProviderCache = *(NSGetProviderCacheList());
    if (!ProviderCache)
    {
        NS_LOG(DEBUG, "Provider Cache Init");
        ProviderCache = NSConsumerStorageCreate();
        NS_VERIFY_NOT_NULL_V(ProviderCache);

        ProviderCache->cacheType = NS_CONSUMER_CACHE_PROVIDER;
        NSSetProviderCacheList(ProviderCache);
    }

    NSCacheElement * obj = NSPopProviderCacheList(ProviderCache);
    while (obj)
    {
        NS_LOG(DEBUG, "build NSTask");
        NSProvider * prov = NSCopyProvider((NSProvider_internal *) obj->data);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(prov,
                    NSRemoveProvider_internal((void *) obj->data));

        NSTask * task = NSMakeTask(TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL, prov);
        NS_VERIFY_NOT_NULL_V(task);

        NSConsumerPushEvent(task);
        NSRemoveProvider_internal((void *) obj->data);
        NSOICFree(obj);

        obj = NSPopProviderCacheList(ProviderCache);
    }
}

void NSConsumerHandleProviderDiscovered(NSProvider_internal * provider)
{
    NS_VERIFY_NOT_NULL_V(provider);

    bool isAdded = true;
    bool isSubscribing = false;

    NSProvider_internal * providerCacheDataFromAddr
        = NSFindProviderFromAddr(provider->connection->addr);

    if (providerCacheDataFromAddr)
    {
        if (!strcmp(providerCacheDataFromAddr->providerId, provider->providerId))
        {
            NSProviderConnectionInfo * infos = providerCacheDataFromAddr->connection;
            while (infos)
            {
                isSubscribing |= infos->isSubscribing;
                infos = infos->next;
            }

            if (isSubscribing == false)
            {
                NSProvider * providerForCb = NSCopyProvider(providerCacheDataFromAddr);
                NSProviderChanged(providerForCb, NS_DISCOVERED);
                NSRemoveProvider(providerForCb);
            }
            NSRemoveProvider_internal(providerCacheDataFromAddr);
            return ;
        }
        NSRemoveProvider_internal(providerCacheDataFromAddr);
    }

    NSProvider_internal * providerCacheData = NSProviderCacheFind(provider->providerId);

    if (providerCacheData == NULL)
    {
        isAdded = false;
    }
    else
    {
        providerCacheData->accessPolicy = provider->accessPolicy;
        NSProviderConnectionInfo * infos = providerCacheData->connection;
        OCTransportAdapter newAdapter = provider->connection->addr->adapter;
        while (infos)
        {
            isSubscribing |= infos->isSubscribing;
            if (infos->addr->adapter == newAdapter && infos->isSubscribing == true)
            {
                NS_LOG_V(INFO_PRIVATE, "This provider already discovered : %s:%d",
                         infos->addr->addr, infos->addr->port);
                NS_LOG_V(DEBUG, "Subscription : %d", infos->isSubscribing);
                return;
            }
            infos = infos->next;
        }
    }

    NSResult ret = NSProviderCacheUpdate(provider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(ret == NS_OK ? (void *) 1 : NULL,
            NSRemoveProvider_internal(providerCacheData));

    if (isAdded == false)
    {
        NS_LOG(DEBUG, "New provider is discovered");
    }
    else
    {
        provider = providerCacheData;
        NS_LOG(DEBUG, "provider's connection is updated.");
    }

    if (provider->accessPolicy == NS_SELECTION_CONSUMER && isSubscribing == false)
    {
        NS_LOG(DEBUG, "accepter is NS_ACCEPTER_CONSUMER, Callback to user");
        NSProvider * prov = NSCopyProvider(provider);
        NSProviderChanged(prov, NS_DISCOVERED);
        NSRemoveProvider(prov);
    }
    else
    {
        NS_LOG(DEBUG, "accepter is NS_ACCEPTER_PROVIDER, request subscribe");
        NSProvider * subProvider = NSCopyProvider(provider);
        NSTask * task = NSMakeTask(TASK_CONSUMER_REQ_SUBSCRIBE, (void *) subProvider);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(task,
                NSRemoveProvider_internal(providerCacheData));

        NSConsumerPushEvent(task);
    }

    NSRemoveProvider_internal(providerCacheData);
}

void NSConsumerHandleProviderDeleted(NSProvider_internal * provider)
{
    NS_VERIFY_NOT_NULL_V(provider);

    NSCacheList * providerCache = *(NSGetProviderCacheList());
    NS_VERIFY_NOT_NULL_V(providerCache);

    NSResult ret = NSConsumerStorageDelete(providerCache, provider->providerId);
    NS_VERIFY_NOT_NULL_V(ret == NS_OK ? (void *)1 : NULL);

    NS_LOG_V(INFO_PRIVATE, "Stopped Provider : %s", provider->providerId);
    NSProvider * prov = NSCopyProvider(provider);
    NSProviderChanged(prov, NS_STOPPED);
    NSRemoveProvider(prov);
}

void NSConsumerHandleSubscribeSucceed(NSProvider_internal * provider)
{
    NS_VERIFY_NOT_NULL_V(provider);

    NSCacheList * ProviderCache = *(NSGetProviderCacheList());

    NSCacheElement * cacheElement = NSConsumerStorageRead(ProviderCache, provider->providerId);
    NS_VERIFY_NOT_NULL_V(cacheElement);

    pthread_mutex_t * mutex = NSGetCacheMutex();
    pthread_mutex_lock(mutex);

    NS_VERIFY_NOT_NULL_V(cacheElement);
    NSProvider_internal * prov = (NSProvider_internal *)cacheElement->data;
    NSProviderConnectionInfo *infos = prov->connection;
    while(infos)
    {
        infos->isSubscribing = true;
        infos = infos->next;
    }

    pthread_mutex_unlock(mutex);
}

void NSConsumerHandleRecvProviderChanged(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL_V(msg);

    NS_LOG_V(INFO_PRIVATE, "confirmed by : %s", msg->providerId);

    NSCacheList * ProviderCache = *(NSGetProviderCacheList());

    NSCacheElement * cacheElement = NSConsumerStorageRead(ProviderCache, msg->providerId);
    NS_VERIFY_NOT_NULL_V(cacheElement);

    pthread_mutex_t * mutex = NSGetCacheMutex();
    pthread_mutex_lock(mutex);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(cacheElement, pthread_mutex_unlock(mutex));
    NSProvider_internal * provider = (NSProvider_internal *) cacheElement->data;
    if (provider->state == (NSProviderState) msg->messageId)
    {
        NS_LOG_V(DEBUG, "Already receive message(ALLOW/DENY) : %d", (int) msg->messageId);
        pthread_mutex_unlock(mutex);
        return;
    }

    NS_LOG_V(DEBUG, "Provider State Changed %d -> %d",
             (int) provider->state, (int) msg->messageId);
    NS_LOG(DEBUG, "call back to user");
    provider->state = (NSProviderState) msg->messageId;

    NSProvider * prov = NSCopyProvider(provider);

    pthread_mutex_unlock(mutex);
    NSProviderChanged(prov, (NSProviderState) msg->messageId);
    NSRemoveProvider(prov);
}

void NSConsumerHandleRecvMessage(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL_V(msg);

    if (NSInsertMessageState(msg->messageId, NS_SYNC_UNREAD))
    {
        NSMessagePost(msg);
    }
}

void NSConsumerHandleRecvSyncInfo(NSSyncInfo * sync)
{
    NS_VERIFY_NOT_NULL_V(sync);

    if (NSUpdateMessageState(sync->messageId, sync->state))
    {
        NSNotificationSync(sync);
    }

    if (sync->state == NS_SYNC_DELETED)
    {
        NSDeleteMessageState(sync->messageId);
    }
}

void NSConsumerHandleMakeSyncInfo(NSSyncInfo * sync)
{
    NS_VERIFY_NOT_NULL_V(sync);

    NSProvider_internal * provider = NSProviderCacheFind(sync->providerId);
    NS_VERIFY_NOT_NULL_V (provider);

    NSProviderConnectionInfo * connections = NSCopyProviderConnections(provider->connection);
    NSRemoveProvider_internal((void *) provider);
    NS_VERIFY_NOT_NULL_V (connections);

    NSSyncInfo_internal * syncInfo = (NSSyncInfo_internal *)OICMalloc(sizeof(NSSyncInfo_internal));
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(syncInfo, NSRemoveConnections(connections));

    OICStrcpy(syncInfo->providerId, sizeof(char) * NS_DEVICE_ID_LENGTH, sync->providerId);
    syncInfo->messageId = sync->messageId;
    syncInfo->state = sync->state;
    syncInfo->connection = connections;

    NSTask * syncTask = NSMakeTask(TASK_SEND_SYNCINFO, (void *) syncInfo);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(syncTask, NSOICFree(syncInfo));

    NSConsumerPushEvent(syncTask);
}

void NSConsumerHandleGetTopicUri(NSMessage * msg)
{
    NS_VERIFY_NOT_NULL_V(msg);

    NSProvider_internal * provider = NSProviderCacheFind(msg->providerId);
    NS_VERIFY_NOT_NULL_V(provider);

    NSTask * topicTask = NSMakeTask(TASK_CONSUMER_REQ_TOPIC_LIST, (void *) provider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(topicTask, NSRemoveProvider_internal(provider));

    NSConsumerPushEvent(topicTask);
}

void NSConsumerHandleRecvTopicLL(NSProvider_internal * provider)
{
    NS_VERIFY_NOT_NULL_V(provider);

    NSRemoveConnections(provider->connection);
    provider->connection = NULL;

    NSProvider_internal * cachedProvider = NSProviderCacheFind(provider->providerId);
    NS_VERIFY_NOT_NULL_V(cachedProvider);

    if (!cachedProvider->topicLL && !provider->topicLL)
    {
        NS_LOG(DEBUG, "topic is null and previous status is same.");
        NSRemoveProvider_internal(cachedProvider);
        return;
    }
    NSRemoveProvider_internal(cachedProvider);

    NSResult ret = NSProviderCacheUpdate(provider);
    NS_VERIFY_NOT_NULL_V(ret == NS_OK ? (void *) 1 : NULL);

    NS_LOG(DEBUG, "call back to user");
    NSProvider * prov = NSCopyProvider(provider);
    NSProviderChanged((NSProvider *) prov, (NSProviderState) NS_TOPIC);
    NSRemoveProvider(prov);
}

void NSConsumerInternalTaskProcessing(NSTask * task)
{
    NS_VERIFY_NOT_NULL_V(task);

    NS_LOG_V(DEBUG, "Receive Event : %d", (int)task->taskType);
    switch (task->taskType)
    {
        case TASK_CONSUMER_SENT_REQ_OBSERVE:
        {
            NS_LOG(DEBUG, "Receive Subscribe succeed from provider.");
            NSConsumerHandleSubscribeSucceed((NSProvider_internal *)task->taskData);
            NSRemoveProvider_internal((void *)task->taskData);
            break;
        }
        case TASK_CONSUMER_RECV_PROVIDER_CHANGED:
        {
            NS_LOG(DEBUG, "Receive Provider Changed");
            NSConsumerHandleRecvProviderChanged((NSMessage *)task->taskData);
            NSRemoveMessage((NSMessage *)task->taskData);
            break;
        }
        case TASK_CONSUMER_RECV_MESSAGE:
        {
            NS_LOG(DEBUG, "Receive New Notification");
            NSConsumerHandleRecvMessage((NSMessage *)task->taskData);
            NSRemoveMessage((NSMessage *)task->taskData);
            break;
        }
        case TASK_CONSUMER_PROVIDER_DISCOVERED:
        {
            NS_LOG(DEBUG, "Receive New Provider is discovered.");
            NSConsumerHandleProviderDiscovered((NSProvider_internal *)task->taskData);
            NSRemoveProvider_internal((NSProvider_internal *)task->taskData);
            break;
        }
        case TASK_RECV_SYNCINFO:
        {
            NS_LOG(DEBUG, "Receive SyncInfo.");
            NSConsumerHandleRecvSyncInfo((NSSyncInfo *)task->taskData);
            NSOICFree(task->taskData);
            break;
        }
        case TASK_MAKE_SYNCINFO:
        {
            NS_LOG(DEBUG, "Make SyncInfo, get Provider's Addr");
            NSConsumerHandleMakeSyncInfo((NSSyncInfo *)task->taskData);
            NSOICFree(task->taskData);
            break;
        }
        case TASK_CONSUMER_REQ_TOPIC_URI:
        {
            NS_LOG(DEBUG, "Request Topic Uri");
            NSConsumerHandleGetTopicUri((NSMessage *)task->taskData);
            NSRemoveMessage((NSMessage *)task->taskData);
            break;
        }
        case TASK_CONSUMER_RECV_TOPIC_LIST:
        {
            NS_LOG(DEBUG, "Receive Topic List");
            NSConsumerHandleRecvTopicLL((NSProvider_internal *)task->taskData);
            NSRemoveProvider_internal((NSProvider_internal *)task->taskData);
            break;
        }
        case TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL:
        {
            NS_LOG(DEBUG, "Make Subscribe cancel from provider.");
            NSConsumerHandleProviderDeleted((NSProvider_internal *)task->taskData);
            NSRemoveProvider_internal((NSProvider_internal *)task->taskData);
            break;
        }
        default :
        {
            NS_LOG(ERROR, "Unknown TASK Type");
            return ;
        }
    }
    NSOICFree(task);
}

// implements of MessageState function
pthread_mutex_t ** NSGetMessageListMutex()
{
    static pthread_mutex_t * g_mutex = NULL;
    if (g_mutex == NULL)
    {
        g_mutex = (pthread_mutex_t *) OICMalloc(sizeof(pthread_mutex_t));
        NS_VERIFY_NOT_NULL(g_mutex, NULL);

        pthread_mutex_init(g_mutex, NULL);
    }
    return & g_mutex;
}

void NSLockMessageListMutex()
{
    NS_LOG_V(DEBUG, "%s", __func__);
    pthread_mutex_lock(*NSGetMessageListMutex());
}

void NSUnlockMessageListMutex()
{
    NS_LOG_V(DEBUG, "%s", __func__);
    pthread_mutex_unlock(*NSGetMessageListMutex());
}

NSMessageStateList ** NSGetMessageStateListAddr()
{
    static NSMessageStateList * g_messageStateList = NULL;
    if (g_messageStateList == NULL)
    {
        g_messageStateList = (NSMessageStateList *)OICMalloc(sizeof(NSMessageStateList));
        NS_VERIFY_NOT_NULL(g_messageStateList, NULL);

        g_messageStateList->head = NULL;
        g_messageStateList->tail = NULL;
    }

    return & g_messageStateList;
}

NSMessageStateList * NSGetMessageStateList()
{
    return * NSGetMessageStateListAddr();
}

NSMessageStateLL * NSFindMessageState(uint64_t msgId)
{
    NS_LOG_V(DEBUG, "%s", __func__);
    if (msgId <= NS_RESERVED_MESSAGEID)
    {
        return NULL;
    }
    NSMessageStateLL * iter = NULL;

    NSLockMessageListMutex();
    if (NSGetMessageStateList()->head == NULL)
    {
        NSUnlockMessageListMutex();
        return NULL;
    }

    for (iter = NSGetMessageStateList()->head; iter; iter = iter->next)
    {
        if (iter->messageId == msgId)
        {
            NSUnlockMessageListMutex();
            return iter;
        }
    }

    NSUnlockMessageListMutex();
    return NULL;
}

bool NSUpdateMessageState(uint64_t msgId, NSSyncType state)
{
    NS_LOG_V(DEBUG, "%s", __func__);
    if (msgId <= NS_RESERVED_MESSAGEID)
    {
        return false;
    }
    NSMessageStateLL * iter = NULL;

    NSLockMessageListMutex();
    for (iter = NSGetMessageStateList()->head; iter; iter = iter->next)
    {
        if (iter->messageId == msgId && state != iter->state)
        {
            iter->state = state;
            NSUnlockMessageListMutex();
            return true;
        }
    }

    NSUnlockMessageListMutex();
    return false;
}

bool NSDeleteMessageState(uint64_t msgId)
{
    NS_LOG_V(DEBUG, "%s", __func__);
    if (msgId <= NS_RESERVED_MESSAGEID)
    {
        return false;
    }

    NSMessageStateLL * iter = NULL;
    NSMessageStateLL * prev = NULL;

    NSLockMessageListMutex();
    for (iter = NSGetMessageStateList()->head; iter; iter = iter->next)
    {
        if (iter->messageId == msgId)
        {
            if (iter == NSGetMessageStateList()->head)
            {
                NSGetMessageStateList()->head = NULL;
                NSGetMessageStateList()->tail = NULL;
            }
            else if (iter == NSGetMessageStateList()->tail)
            {
                prev->next = NULL;
                NSGetMessageStateList()->tail = prev;
            }
            else
            {
                prev->next = iter->next;
            }
            NSUnlockMessageListMutex();

            NSOICFree(iter);
            return true;
        }
        prev = iter;
    }

    NSUnlockMessageListMutex();
    return false;
}

bool NSInsertMessageState(uint64_t msgId, NSSyncType state)
{
    NS_LOG_V(DEBUG, "%s", __func__);
    if (NSFindMessageState(msgId))
    {
        return false;
    }

    NSMessageStateLL * insertMsg = (NSMessageStateLL * )OICMalloc(sizeof(NSMessageStateLL));
    NS_VERIFY_NOT_NULL(insertMsg, false);

    insertMsg->messageId = msgId;
    insertMsg->state = state;
    insertMsg->next = NULL;

    NSLockMessageListMutex();
    if (NSGetMessageStateList()->head == NULL)
    {
        NSGetMessageStateList()->head = insertMsg;
    }
    else
    {
        NSGetMessageStateList()->tail->next = insertMsg;
    }
    NSGetMessageStateList()->tail = insertMsg;
    NSUnlockMessageListMutex();

    return true;
}

void NSDestroyMessageStateList()
{
    NS_LOG_V(DEBUG, "%s", __func__);
    NSLockMessageListMutex();

    NSMessageStateLL * iter = NSGetMessageStateList()->head;
    while (iter)
    {
        NSMessageStateLL * del = iter;
        iter = iter->next;
        NSOICFree(del);
    }

    NSGetMessageStateList()->head = NULL;
    NSGetMessageStateList()->tail = NULL;

    NSUnlockMessageListMutex();

    pthread_mutex_t * mu = *NSGetMessageListMutex();
    pthread_mutex_destroy(mu);
    NSOICFree(mu);
    *NSGetMessageListMutex() = NULL;

    NSMessageStateList * list = NSGetMessageStateList();
    NSOICFree(list);
    *NSGetMessageStateListAddr() = NULL;
}
