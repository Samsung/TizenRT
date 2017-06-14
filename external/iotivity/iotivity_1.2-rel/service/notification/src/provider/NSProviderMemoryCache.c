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

#include "NSProviderMemoryCache.h"
#include <string.h>

#define NS_PROVIDER_DELETE_REGISTERED_TOPIC_DATA(it, topicData, newObj) \
    { \
        if (it) \
        { \
            NS_LOG(DEBUG, "already registered for topic name"); \
            NSOICFree(topicData->topicName); \
            NSOICFree(topicData); \
            NSOICFree(newObj); \
            pthread_mutex_unlock(&NSCacheMutex); \
            return NS_FAIL; \
        } \
    }

NSCacheList * NSProviderStorageCreate()
{
    pthread_mutex_lock(&NSCacheMutex);
    NSCacheList * newList = (NSCacheList *) OICMalloc(sizeof(NSCacheList));

    if (!newList)
    {
        pthread_mutex_unlock(&NSCacheMutex);
        return NULL;
    }

    newList->head = newList->tail = NULL;

    pthread_mutex_unlock(&NSCacheMutex);
    NS_LOG(DEBUG, "NSCacheCreate");

    return newList;
}

NSCacheElement * NSProviderStorageRead(NSCacheList * list, const char * findId)
{
    pthread_mutex_lock(&NSCacheMutex);

    NS_LOG(DEBUG, "NSCacheRead - IN");

    NSCacheElement * iter = list->head;
    NSCacheElement * next = NULL;
    NSCacheType type = list->cacheType;

    NS_LOG_V(INFO_PRIVATE, "Find ID - %s", findId);

    while (iter)
    {
        next = iter->next;

        if (NSProviderCompareIdCacheData(type, iter->data, findId))
        {
            NS_LOG(DEBUG, "Found in Cache");
            pthread_mutex_unlock(&NSCacheMutex);
            return iter;
        }

        iter = next;
    }

    NS_LOG(DEBUG, "Not found in Cache");
    NS_LOG(DEBUG, "NSCacheRead - OUT");
    pthread_mutex_unlock(&NSCacheMutex);

    return NULL;
}

NSResult NSCacheUpdateSubScriptionState(NSCacheList * list, char * id, bool state)
{
    pthread_mutex_lock(&NSCacheMutex);

    NS_LOG(DEBUG, "NSCacheUpdateSubScriptionState - IN");

    if (id == NULL)
    {
        NS_LOG(DEBUG, "id is NULL");
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_ERROR;
    }

    NSCacheElement * it = NSProviderStorageRead(list, id);

    if (it)
    {
        NSCacheSubData * itData = (NSCacheSubData *) it->data;
        if (strcmp(itData->id, id) == 0)
        {
            NS_LOG(DEBUG, "Update Data - IN");

            NS_LOG_V(INFO_PRIVATE, "currData_ID = %s", itData->id);
            NS_LOG_V(DEBUG, "currData_MsgObID = %d", itData->messageObId);
            NS_LOG_V(DEBUG, "currData_SyncObID = %d", itData->syncObId);
            NS_LOG_V(DEBUG, "currData_Cloud_MsgObID = %d", itData->remote_messageObId);
            NS_LOG_V(DEBUG, "currData_Cloud_SyncObID = %d", itData->remote_syncObId);
            NS_LOG_V(DEBUG, "currData_IsWhite = %d", itData->isWhite);

            NS_LOG_V(DEBUG, "update state = %d", state);

            itData->isWhite = state;

            NS_LOG(DEBUG, "Update Data - OUT");
            pthread_mutex_unlock(&NSCacheMutex);
            return NS_OK;
        }
    }
    else
    {
        NS_LOG(DEBUG, "Not Found Data");
    }

    NS_LOG(DEBUG, "NSCacheUpdateSubScriptionState - OUT");
    pthread_mutex_unlock(&NSCacheMutex);
    return NS_ERROR;
}

NSResult NSProviderStorageWrite(NSCacheList * list, NSCacheElement * newObj)
{
    pthread_mutex_lock(&NSCacheMutex);

    NSCacheType type = list->cacheType;

    NS_LOG(DEBUG, "NSCacheWrite - IN");

    if (newObj == NULL)
    {
        NS_LOG(DEBUG, "newObj is NULL - IN");
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_ERROR;
    }

    if (type == NS_PROVIDER_CACHE_SUBSCRIBER)
    {
        NS_LOG(DEBUG, "Type is SUBSCRIBER");

        NSCacheSubData * subData = (NSCacheSubData *) newObj->data;
        NSCacheElement * it = NSProviderStorageRead(list, subData->id);

        if (it)
        {
            NSCacheSubData * itData = (NSCacheSubData *) it->data;

            if (strcmp(itData->id, subData->id) == 0)
            {
                NS_LOG(DEBUG, "Update Data - IN");

                NS_LOG_V(INFO_PRIVATE, "currData_ID = %s", itData->id);
                NS_LOG_V(DEBUG, "currData_MsgObID = %d", itData->messageObId);
                NS_LOG_V(DEBUG, "currData_SyncObID = %d", itData->syncObId);
                NS_LOG_V(DEBUG, "currData_Cloud_MsgObID = %d", itData->remote_messageObId);
                NS_LOG_V(DEBUG, "currData_Cloud_SyncObID = %d", itData->remote_syncObId);
                NS_LOG_V(DEBUG, "currData_IsWhite = %d", itData->isWhite);

                NS_LOG_V(INFO_PRIVATE, "subData_ID = %s", subData->id);
                NS_LOG_V(DEBUG, "subData_MsgObID = %d", subData->messageObId);
                NS_LOG_V(DEBUG, "subData_SyncObID = %d", subData->syncObId);
                NS_LOG_V(DEBUG, "subData_Cloud_MsgObID = %d", subData->remote_messageObId);
                NS_LOG_V(DEBUG, "subData_Cloud_SyncObID = %d", subData->remote_syncObId);
                NS_LOG_V(DEBUG, "subData_IsWhite = %d", subData->isWhite);

                if (subData->messageObId != 0)
                {
                    itData->messageObId = subData->messageObId;
                }

                if (subData->syncObId != 0)
                {
                    itData->syncObId = subData->syncObId;
                }

                if (subData->remote_messageObId != 0)
                {
                    itData->remote_messageObId = subData->remote_messageObId;
                }

                if (subData->remote_syncObId != 0)
                {
                    itData->remote_syncObId = subData->remote_syncObId;
                    NS_LOG_V(DEBUG, "sync id cached: %d", itData->remote_syncObId);
                }

                NS_LOG(DEBUG, "Update Data - OUT");
                NSOICFree(subData);
                NSOICFree(newObj);
                pthread_mutex_unlock(&NSCacheMutex);
                return NS_OK;
            }
        }

    }
    else if (type == NS_PROVIDER_CACHE_REGISTER_TOPIC)
    {
        NS_LOG(DEBUG, "Type is REGITSTER TOPIC");

        NSCacheTopicData * topicData = (NSCacheTopicData *) newObj->data;
        NSCacheElement * it = NSProviderStorageRead(list, topicData->topicName);

        NS_PROVIDER_DELETE_REGISTERED_TOPIC_DATA(it, topicData, newObj);
    }
    else if (type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME)
    {
        NS_LOG(DEBUG, "Type is REGITSTER TOPIC");

        NSCacheTopicSubData * topicData = (NSCacheTopicSubData *) newObj->data;
        NSCacheElement * it = NSProviderStorageRead(list, topicData->topicName);

        NS_PROVIDER_DELETE_REGISTERED_TOPIC_DATA(it, topicData, newObj);
    }
    else if (type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_CID)
    {
        NS_LOG(DEBUG, "Type is REGITSTER TOPIC");

        NSCacheTopicSubData * topicData = (NSCacheTopicSubData *) newObj->data;
        NSCacheElement * it = NSProviderStorageRead(list, topicData->id);

        NS_PROVIDER_DELETE_REGISTERED_TOPIC_DATA(it, topicData, newObj);
    }

    if (list->head == NULL)
    {
        NS_LOG(DEBUG, "list->head is NULL, Insert First Data");
        list->head = list->tail = newObj;
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_OK;
    }

    list->tail = list->tail->next = newObj;
    NS_LOG(DEBUG, "list->head is not NULL");
    pthread_mutex_unlock(&NSCacheMutex);
    return NS_OK;
}

NSResult NSProviderStorageDestroy(NSCacheList * list)
{
    NSCacheElement * iter = list->head;
    NSCacheElement * next = NULL;
    NSCacheType type = list->cacheType;

    while (iter)
    {
        next = (NSCacheElement *) iter->next;
        NSProviderDeleteCacheData(type, iter->data);
        NSOICFree(iter);
        iter = next;
    }

    NSOICFree(list);
    return NS_OK;
}

bool NSIsSameObId(NSCacheSubData * data, OCObservationId id)
{
    if (id == data->messageObId || id == data->syncObId || id == data->remote_messageObId ||
                id == data->remote_syncObId)
    {
        return true;
    }

    return false;
}

bool NSProviderCompareIdCacheData(NSCacheType type, void * data, const char * id)
{
    NS_LOG(DEBUG, "NSProviderCompareIdCacheData - IN");

    if (data == NULL)
    {
        return false;
    }

    NS_LOG_V(INFO_PRIVATE, "Data(compData) = [%s]", id);

    if (type == NS_PROVIDER_CACHE_SUBSCRIBER)
    {
        NSCacheSubData * subData = (NSCacheSubData *) data;

        NS_LOG_V(INFO_PRIVATE, "Data(subData) = [%s]", subData->id);

        if (strcmp(subData->id, id) == 0)
        {
            NS_LOG(DEBUG, "SubData is Same");
            return true;
        }

        NS_LOG(DEBUG, "Message Data is Not Same");
        return false;
    }
    else if (type == NS_PROVIDER_CACHE_SUBSCRIBER_OBSERVE_ID)
    {
        NSCacheSubData * subData = (NSCacheSubData *) data;

        NS_LOG_V(INFO_PRIVATE, "Data(subData) = [%s]", subData->id);

        OCObservationId currID = *id;

        if (NSIsSameObId(subData, currID))
        {
            NS_LOG(DEBUG, "SubData is Same");
            return true;
        }

        NS_LOG(DEBUG, "Message Data is Not Same");
        return false;
    }
    else if (type == NS_PROVIDER_CACHE_REGISTER_TOPIC)
    {
        NSCacheTopicData * topicData = (NSCacheTopicData *) data;

        NS_LOG_V(DEBUG, "Data(topicData) = [%s]", topicData->topicName);

        if (strcmp(topicData->topicName, id) == 0)
        {
            NS_LOG(DEBUG, "SubData is Same");
            return true;
        }

        NS_LOG(DEBUG, "Message Data is Not Same");
        return false;
    }
    else if (type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME)
    {
        NSCacheTopicSubData * topicData = (NSCacheTopicSubData *) data;

        NS_LOG_V(DEBUG, "Data(topicData) = [%s]", topicData->topicName);

        if (strcmp(topicData->topicName, id) == 0)
        {
            NS_LOG(DEBUG, "SubData is Same");
            return true;
        }

        NS_LOG(DEBUG, "Message Data is Not Same");
        return false;
    }
    else if (type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_CID)
    {
        NSCacheTopicSubData * topicData = (NSCacheTopicSubData *) data;

        NS_LOG_V(INFO_PRIVATE, "Data(topicData) = [%s]", topicData->id);

        if (strcmp(topicData->id, id) == 0)
        {
            NS_LOG(DEBUG, "SubData is Same");
            return true;
        }

        NS_LOG(DEBUG, "Message Data is Not Same");
        return false;
    }


    NS_LOG(DEBUG, "NSProviderCompareIdCacheData - OUT");
    return false;
}

NSResult NSProviderDeleteCacheData(NSCacheType type, void * data)
{
    if (!data)
    {
        return NS_ERROR;
    }

    if (type == NS_PROVIDER_CACHE_SUBSCRIBER || type == NS_PROVIDER_CACHE_SUBSCRIBER_OBSERVE_ID)
    {
        NSCacheSubData * subData = (NSCacheSubData *) data;

        (subData->id)[0] = '\0';
        NSOICFree(subData);
        return NS_OK;
    }
    else if (type == NS_PROVIDER_CACHE_REGISTER_TOPIC)
    {

        NSCacheTopicData * topicData = (NSCacheTopicData *) data;
        NS_LOG_V(DEBUG, "topicData->topicName = %s, topicData->state = %d", topicData->topicName,
                (int)topicData->state);

        NSOICFree(topicData->topicName);
        NSOICFree(topicData);
    }
    else if (type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME ||
            type == NS_PROVIDER_CACHE_CONSUMER_TOPIC_CID)
    {
        NSCacheTopicSubData * topicData = (NSCacheTopicSubData *) data;
        NSOICFree(topicData->topicName);
        NSOICFree(topicData);
    }

    return NS_OK;
}

NSResult NSProviderStorageDelete(NSCacheList * list, const char * delId)
{
    pthread_mutex_lock(&NSCacheMutex);
    NSCacheElement * prev = list->head;
    NSCacheElement * del = list->head;

    NSCacheType type = list->cacheType;

    if (!del)
    {
        NS_LOG(DEBUG, "list head is NULL");
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_FAIL;
    }

    if (NSProviderCompareIdCacheData(type, del->data, delId))
    {
        if (del == list->head) // first object
        {
            if (del == list->tail) // first object (one object)
            {
                list->tail = del->next;
            }

            list->head = del->next;
            NSProviderDeleteCacheData(type, del->data);
            NSOICFree(del);
            pthread_mutex_unlock(&NSCacheMutex);
            return NS_OK;
        }
    }

    del = del->next;

    while (del)
    {
        if (NSProviderCompareIdCacheData(type, del->data, delId))
        {
            if (del == list->tail) // delete object same to last object
            {
                list->tail = prev;
            }

            prev->next = del->next;
            NSProviderDeleteCacheData(type, del->data);
            NSOICFree(del);
            pthread_mutex_unlock(&NSCacheMutex);
            return NS_OK;
        }

        prev = del;
        del = del->next;
    }

    pthread_mutex_unlock(&NSCacheMutex);
    return NS_FAIL;
}

NSTopicLL * NSProviderGetTopicsCacheData(NSCacheList * regTopicList)
{
    NS_LOG(DEBUG, "NSProviderGetTopicsCache - IN");
    pthread_mutex_lock(&NSCacheMutex);

    NSCacheElement * iter = regTopicList->head;

    if (!iter)
    {
        pthread_mutex_unlock(&NSCacheMutex);
        return NULL;
    }

    NSTopicLL * iterTopic = NULL;
    NSTopicLL * newTopic = NULL;
    NSTopicLL * topics = NULL;

    while (iter)
    {
        NSCacheTopicData * curr = (NSCacheTopicData *) iter->data;
        newTopic = (NSTopicLL *) OICMalloc(sizeof(NSTopicLL));

        if (!newTopic)
        {
            pthread_mutex_unlock(&NSCacheMutex);
            return NULL;
        }

        newTopic->state = curr->state;
        newTopic->next = NULL;
        newTopic->topicName = OICStrdup(curr->topicName);

        if (!topics)
        {
            iterTopic = topics = newTopic;
        }
        else
        {
            iterTopic->next = newTopic;
            iterTopic = newTopic;
        }

        iter = iter->next;
    }

    pthread_mutex_unlock(&NSCacheMutex);
    NS_LOG(DEBUG, "NSProviderGetTopicsCache - OUT");

    return topics;
}

NSTopicLL * NSProviderGetConsumerTopicsCacheData(NSCacheList * regTopicList,
        NSCacheList * conTopicList, const char * consumerId)
{
    NS_LOG(DEBUG, "NSProviderGetConsumerTopicsCacheData - IN");

    pthread_mutex_lock(&NSCacheMutex);
    NSTopicLL * topics = NSProviderGetTopicsCacheData(regTopicList);

    if (!topics)
    {
        pthread_mutex_unlock(&NSCacheMutex);
        return NULL;
    }

    NSCacheElement * iter = conTopicList->head;
    conTopicList->cacheType = NS_PROVIDER_CACHE_CONSUMER_TOPIC_CID;

    while (iter)
    {
        NSCacheTopicSubData * curr = (NSCacheTopicSubData *)iter->data;

        if (curr && strcmp(curr->id, consumerId) == 0)
        {
            NS_LOG_V(INFO_PRIVATE, "curr->id = %s", curr->id);
            NS_LOG_V(DEBUG, "curr->topicName = %s", curr->topicName);
            NSTopicLL * topicIter = topics;

            while (topicIter)
            {
                if (strcmp(topicIter->topicName, curr->topicName) == 0)
                {
                    topicIter->state = NS_TOPIC_SUBSCRIBED;
                    break;
                }

                topicIter = topicIter->next;
            }
        }

        iter = iter->next;
    }

    conTopicList->cacheType = NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME;
    pthread_mutex_unlock(&NSCacheMutex);
    NS_LOG(DEBUG, "NSProviderGetConsumerTopics - OUT");

    return topics;
}

bool NSProviderIsTopicSubScribed(NSCacheElement * conTopicList, char * cId, char * topicName)
{
    pthread_mutex_lock(&NSCacheMutex);

    if (!conTopicList || !cId || !topicName)
    {
        pthread_mutex_unlock(&NSCacheMutex);
        return false;
    }

    NSCacheElement * iter = conTopicList;

    while (iter)
    {
        NSCacheTopicSubData * curr = (NSCacheTopicSubData *) iter->data;

        if ( (strcmp(curr->id, cId) == 0) && (strcmp(curr->topicName, topicName) == 0) )
        {
            pthread_mutex_unlock(&NSCacheMutex);
            return true;
        }

        iter = iter->next;
    }

    pthread_mutex_unlock(&NSCacheMutex);
    return false;
}

NSResult NSProviderDeleteConsumerTopic(NSCacheList * conTopicList,
        NSCacheTopicSubData * topicSubData)
{
    pthread_mutex_lock(&NSCacheMutex);

    char * cId = topicSubData->id;
    char * topicName = topicSubData->topicName;

    if (!conTopicList || !cId || !topicName)
    {
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_ERROR;
    }

    NSCacheElement * prev = conTopicList->head;
    NSCacheElement * del = conTopicList->head;

    NSCacheType type = conTopicList->cacheType;

    if (!del)
    {
        NS_LOG(DEBUG, "list head is NULL");
        pthread_mutex_unlock(&NSCacheMutex);
        return NS_FAIL;
    }

    NSCacheTopicSubData * curr = (NSCacheTopicSubData *) del->data;
    NS_LOG_V(INFO_PRIVATE, "compareid = %s", cId);
    NS_LOG_V(DEBUG, "comparetopicName = %s", topicName);
    NS_LOG_V(INFO_PRIVATE, "curr->id = %s", curr->id);
    NS_LOG_V(DEBUG, "curr->topicName = %s", curr->topicName);

    if ( (strncmp(curr->id, cId, NS_UUID_STRING_SIZE) == 0) &&
            (strcmp(curr->topicName, topicName) == 0) )
    {
        if (del == conTopicList->head) // first object
        {
            if (del == conTopicList->tail) // first object (one object)
            {
                conTopicList->tail = del->next;
            }

            conTopicList->head = del->next;
            NSProviderDeleteCacheData(type, del->data);
            NSOICFree(del);
            pthread_mutex_unlock(&NSCacheMutex);
            return NS_OK;
        }
    }

    curr = NULL;
    del = del->next;

    while (del)
    {
        curr = (NSCacheTopicSubData *) del->data;
        if ( (strncmp(curr->id, cId, NS_UUID_STRING_SIZE) == 0) &&
                (strcmp(curr->topicName, topicName) == 0) )
        {
            if (del == conTopicList->tail) // delete object same to last object
            {
                conTopicList->tail = prev;
            }

            prev->next = del->next;
            NSProviderDeleteCacheData(type, del->data);
            NSOICFree(del);
            pthread_mutex_unlock(&NSCacheMutex);
            return NS_OK;
        }

        prev = del;
        del = del->next;
    }

    pthread_mutex_unlock(&NSCacheMutex);
    return NS_FAIL;
}
