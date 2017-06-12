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

#include "NSProviderTopic.h"
#include "oic_string.h"
#include "oic_malloc.h"
#include <pthread.h>

NSResult NSSendTopicUpdation();

NSResult NSInitTopicList()
{
    NS_LOG(DEBUG, "NSInitTopicList - IN");

    consumerTopicList = NSProviderStorageCreate();
    NS_VERIFY_NOT_NULL(consumerTopicList, NS_FAIL);
    consumerTopicList->cacheType = NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME;

    registeredTopicList = NSProviderStorageCreate();
    NS_VERIFY_NOT_NULL(registeredTopicList, NS_FAIL);
    registeredTopicList->cacheType = NS_PROVIDER_CACHE_REGISTER_TOPIC;

    NS_LOG(DEBUG, "NSInitTopicList - OUT");
    return NS_OK;
}

size_t NSProviderGetTopicListSize(NSTopicLL * firstElement)
{
    if (!firstElement)
    {
        return 0;
    }

    int cnt = 0;

    NSTopicLL * iter = firstElement;

    while (iter)
    {
        cnt++;
        iter = iter->next;
    }

    return cnt;
}

NSResult NSRegisterTopic(const char * topicName)
{
    NS_LOG(DEBUG, "NSWriteTopicsToStorage()");

    NSCacheTopicData * data = (NSCacheTopicData *) OICMalloc(sizeof(NSCacheTopicData));
    NS_VERIFY_NOT_NULL(data, NS_FAIL);
    data->topicName = (char *) topicName;
    data->state = NS_TOPIC_UNSUBSCRIBED;

    NSCacheElement * element = (NSCacheElement *) OICMalloc(sizeof(NSCacheElement));
    if (!element)
    {
        NSOICFree(data->topicName);
        NSOICFree(data);
        return NS_FAIL;
    }

    element->data = (void *) data;
    element->next = NULL;

    if (NSProviderStorageWrite(registeredTopicList, element) != NS_OK)
    {
        NS_LOG(DEBUG, "fail to write cache");
        return NS_FAIL;
    }

    NSSendTopicUpdation();
    NS_LOG(DEBUG, "NSWriteTopicsToStorage() NS_OK");
    return NS_OK;
}

NSResult NSUnregisterTopic(const char * topicName)
{
    NS_LOG(DEBUG, "NSDeleteTopics()");
    NSResult result = NS_OK;

    if (!topicName)
    {
        NS_LOG(ERROR, "topicName is NULL");
        return NS_ERROR;
    }

    result = NSProviderStorageDelete(registeredTopicList, topicName);

    while (NSProviderStorageDelete(consumerTopicList, topicName) != NS_FAIL)
    {
    }

    if (result == NS_OK)
    {
        NSSendTopicUpdation();
    }

    return result;
}

NSResult NSSendTopicUpdation()
{
    NS_LOG(DEBUG, "NSSendTopicUpdation - IN");

    OCRepPayload* payload = OCRepPayloadCreate();

    if (!payload)
    {
        NS_LOG(ERROR, "fail to create playload");
        return NS_ERROR;
    }

    OCResourceHandle rHandle = NULL;
    if (NSPutMessageResource(NULL, &rHandle) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to put message resource");
        return NS_ERROR;
    }

    OCRepPayloadSetUri(payload, NS_COLLECTION_MESSAGE_URI);
    OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, NS_TOPIC);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, NSGetProviderInfo()->providerId);

    OCObservationId obArray[255] =
    { 0, };
    int obCount = 0;

    NSCacheElement * it = consumerSubList->head;

    while (it)
    {
        NSCacheSubData * subData = (NSCacheSubData *) it->data;

        if (subData->isWhite)
        {
            if (subData->messageObId != 0)
            {
                obArray[obCount++] = subData->messageObId;
            }

#if (defined WITH_CLOUD)
            if (subData->remote_messageObId != 0)
            {
                obArray[obCount++] = subData->remote_messageObId;
            }
#endif
        }

        it = it->next;
    }

    if (!obCount)
    {
        NS_LOG(ERROR, "observer count is zero");
        return NS_ERROR;
    }

    if (OCNotifyListOfObservers(rHandle, obArray, obCount, payload, OC_HIGH_QOS) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "fail to send topic updation");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;

    }
    OCRepPayloadDestroy(payload);

    NS_LOG(DEBUG, "NSSendTopicUpdation - OUT");
    return NS_OK;
}

NSResult NSSendTopicUpdationToConsumer(char *consumerId)
{
    NS_LOG(DEBUG, "NSSendTopicUpdationToConsumer - IN");

    OCRepPayload* payload = OCRepPayloadCreate();

    if (!payload)
    {
        NS_LOG(ERROR, "fail to create playload");
        return NS_ERROR;
    }

    OCResourceHandle rHandle = NULL;
    if (NSPutMessageResource(NULL, &rHandle) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to put message resource");
        return NS_ERROR;
    }

    OCRepPayloadSetUri(payload, NS_COLLECTION_MESSAGE_URI);
    OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, NS_TOPIC);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, NSGetProviderInfo()->providerId);

    NSCacheElement * element = NSProviderStorageRead(consumerSubList, consumerId);

    if (element == NULL)
    {
        NS_LOG(ERROR, "element is NULL");
        return NS_ERROR;
    }

    NSCacheSubData * subData = (NSCacheSubData*) element->data;

    if (OCNotifyListOfObservers(rHandle, (OCObservationId*) &subData->messageObId, 1, payload,
            OC_HIGH_QOS) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "fail to send topic updation");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;
    }

    OCRepPayloadDestroy(payload);

    NS_LOG(DEBUG, "NSSendTopicUpdationToConsumer - OUT");
    return NS_OK;
}

NSResult NSSendTopicList(OCEntityHandlerRequest * entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSSendTopicList - IN");

    char * copyReq = OICStrdup(entityHandlerRequest->query);
    char * id = NSGetValueFromQuery(copyReq, NS_QUERY_CONSUMER_ID);
    NSTopicLL * topics = NULL;

    if (!id)
    {
        NS_LOG(DEBUG, "Send registered topic list");
        topics = NSProviderGetTopicsCacheData(registeredTopicList);
    }
    else
    {
        NS_LOG(DEBUG, "Send subscribed topic list to consumer");
        topics = NSProviderGetConsumerTopicsCacheData(registeredTopicList, consumerTopicList, id);
        if (!topics)
        {
            topics = NSProviderGetTopicsCacheData(registeredTopicList);
        }
    }

    // make response for the Get Request
    OCEntityHandlerResponse response;
    response.numSendVendorSpecificHeaderOptions = 0;
    memset(response.sendVendorSpecificHeaderOptions, 0,
            sizeof response.sendVendorSpecificHeaderOptions);
    memset(response.resourceUri, 0, sizeof response.resourceUri);

    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        NS_LOG(ERROR, "payload is NULL");
        NSOICFree(copyReq);
        return NS_ERROR;
    }

    OCRepPayloadSetUri(payload, NS_COLLECTION_TOPIC_URI);
    if (id)
    {
        OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_CONSUMER_ID, id);
    }
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, NSGetProviderInfo()->providerId);
    NSOICFree(copyReq);

    if (topics)
    {
        NS_LOG(DEBUG, "topicList is NULL");
        size_t dimensionSize = (size_t) NSProviderGetTopicListSize(topics);
        NS_LOG_V(DEBUG, "dimensionSize = %d", (int)dimensionSize);

        if (!dimensionSize)
        {
            return NS_ERROR;
        }

        OCRepPayload** payloadTopicArray = (OCRepPayload **) OICMalloc(
                sizeof(OCRepPayload *) * dimensionSize);
        NS_VERIFY_NOT_NULL(payloadTopicArray, NS_ERROR);

        size_t dimensions[3] = { dimensionSize, 0, 0 };

        for (int i = 0; i < (int) dimensionSize; i++)
        {
            NS_LOG_V(DEBUG, "topicName = %s", topics->topicName);
            NS_LOG_V(DEBUG, "topicState = %d",(int) topics->state);

            payloadTopicArray[i] = OCRepPayloadCreate();
            NS_VERIFY_NOT_NULL(payloadTopicArray[i], NS_ERROR);
            OCRepPayloadSetPropString(payloadTopicArray[i], NS_ATTRIBUTE_TOPIC_NAME,
                    topics->topicName);
            OCRepPayloadSetPropInt(payloadTopicArray[i], NS_ATTRIBUTE_TOPIC_SELECTION,
                    (int) topics->state);

            NSTopicLL * next = topics->next;
            NSOICFree(topics->topicName);
            NSOICFree(topics);
            topics = next;
        }

        OCRepPayloadSetPropObjectArrayAsOwner(payload, NS_ATTRIBUTE_TOPIC_LIST,
                    payloadTopicArray, dimensions);
    }
    else
    {
        size_t dimensions[3] = { 0, 0, 0 };

        OCRepPayloadSetPropObjectArrayAsOwner(payload, NS_ATTRIBUTE_TOPIC_LIST,
                (OCRepPayload **) NULL, dimensions);
    }

    copyReq = OICStrdup(entityHandlerRequest->query);
    char * reqInterface = NSGetValueFromQuery(copyReq, NS_QUERY_INTERFACE);

    if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) == 0)
    {
        OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_BASELINE);
        OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_READ);
        OCResourcePayloadAddStringLL(&payload->types, NS_ROOT_TYPE);
    }

    NSOICFree(copyReq);
    response.requestHandle = entityHandlerRequest->requestHandle;
    response.resourceHandle = entityHandlerRequest->resource;
    response.persistentBufferFlag = 0;
    response.ehResult = OC_EH_OK;
    response.payload = (OCPayload *) payload;

    if (OCDoResponse(&response) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to response topic list");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;
    }

    OCRepPayloadDestroy(payload);
    NS_LOG(DEBUG, "NSSendTopicList - OUT");
    return NS_OK;
}

NSResult NSPostConsumerTopics(OCEntityHandlerRequest * entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSPostConsumerTopics() - IN");

    char * consumerId = NULL;
    OCRepPayload * payload = (OCRepPayload *) entityHandlerRequest->payload;
    OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_CONSUMER_ID, &consumerId);

    if (!consumerId)
    {
        NS_LOG(DEBUG, "Invalid consumer ID");
        return NS_FAIL;
    }

    NS_LOG_V(INFO_PRIVATE, "TOPIC consumer ID = %s", consumerId);

    consumerTopicList->cacheType = NS_PROVIDER_CACHE_CONSUMER_TOPIC_CID;

    while (NSProviderStorageDelete(consumerTopicList, consumerId) != NS_FAIL)
    {
    }

    consumerTopicList->cacheType = NS_PROVIDER_CACHE_CONSUMER_TOPIC_NAME;
    OCRepPayload ** topicListPayload = NULL;
    OCRepPayloadValue * payloadValue = NULL;
    payloadValue = NSPayloadFindValue(payload, NS_ATTRIBUTE_TOPIC_LIST);
    size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
    size_t dimensions[3] = { dimensionSize, 0, 0 };
    OCRepPayloadGetPropObjectArray(payload, NS_ATTRIBUTE_TOPIC_LIST, &topicListPayload, dimensions);

    for (int i = 0; i < (int) dimensionSize; i++)
    {
        char * topicName = NULL;
        int64_t topicState = 0;

        OCRepPayloadGetPropString(topicListPayload[i], NS_ATTRIBUTE_TOPIC_NAME, &topicName);
        if (OCRepPayloadGetPropInt(topicListPayload[i], NS_ATTRIBUTE_TOPIC_SELECTION, &topicState))
        {
            NS_LOG_V(DEBUG, "Topic Name(state):  %s(%d)", topicName, (int)topicState);
        }

        if (NS_TOPIC_SUBSCRIBED == (NSTopicState) topicState)
        {
            NSCacheTopicSubData * topicSubData = (NSCacheTopicSubData *) OICMalloc(
                    sizeof(NSCacheTopicSubData));
            NS_VERIFY_NOT_NULL(topicSubData, NS_FAIL);

            OICStrcpy(topicSubData->id, NS_UUID_STRING_SIZE, consumerId);
            topicSubData->topicName = topicName;

            NSCacheElement * newObj = (NSCacheElement *) OICMalloc(sizeof(NSCacheElement));

            if (!newObj)
            {
                NSOICFree(topicSubData->topicName);
                NSOICFree(topicSubData);
                NSOICFree(consumerId);
                return NS_FAIL;
            }

            newObj->data = (NSCacheData *) topicSubData;
            newObj->next = NULL;

            if (NS_OK != NSProviderStorageWrite(consumerTopicList, newObj))
            {
                NS_LOG(ERROR, "Fail to write cache");
            }
        }
    }
    NSSendTopicUpdationToConsumer(consumerId);
    NSOICFree(consumerId);
    NS_LOG(DEBUG, "NSPostConsumerTopics() - OUT");
    return NS_OK;
}

void * NSTopicSchedule(void * ptr)
{
    if (ptr == NULL)
    {
        NS_LOG(DEBUG, "Create NSTopicSchedule");
    }

    while (NSIsRunning[TOPIC_SCHEDULER])
    {
        sem_wait(&NSSemaphore[TOPIC_SCHEDULER]);
        pthread_mutex_lock(&NSMutex[TOPIC_SCHEDULER]);

        if (NSHeadMsg[TOPIC_SCHEDULER] != NULL)
        {
            NSTask *node = NSHeadMsg[TOPIC_SCHEDULER];
            NSHeadMsg[TOPIC_SCHEDULER] = node->nextTask;

            switch (node->taskType)
            {
                case TASK_SEND_TOPICS:
                    NS_LOG(DEBUG, "CASE TASK_SEND_TOPICS : ");
                    NSSendTopicList((OCEntityHandlerRequest*) node->taskData);
                    NSFreeOCEntityHandlerRequest((OCEntityHandlerRequest*) node->taskData);
                    break;
                case TASK_SUBSCRIBE_TOPIC:
                {
                    NS_LOG(DEBUG, "CASE TASK_SUBSCRIBE_TOPIC : ");
                    NSTopicSyncResult * topicSyncResult = (NSTopicSyncResult *) node->taskData;
                    pthread_mutex_lock(topicSyncResult->mutex);
                    NSCacheElement * newObj = (NSCacheElement *) OICMalloc(sizeof(NSCacheElement));
                    NSCacheTopicSubData * subData =
                            (NSCacheTopicSubData *) topicSyncResult->topicData;
                    if (!newObj)
                    {
                        NSOICFree(subData->topicName);
                        NSOICFree(subData);
                        pthread_cond_signal(topicSyncResult->condition);
                        pthread_mutex_unlock(topicSyncResult->mutex);
                    }
                    else
                    {
                        if (NSProviderStorageRead(registeredTopicList, subData->topicName))
                        {
                            newObj->data = topicSyncResult->topicData;
                            newObj->next = NULL;

                            if (NSProviderStorageWrite(consumerTopicList, newObj) == NS_OK)
                            {
                                NSSendTopicUpdationToConsumer(subData->id);
                                topicSyncResult->result = NS_OK;
                            }
                        }
                        else
                        {
                            NSOICFree(subData->topicName);
                            NSOICFree(subData);
                            NSOICFree(newObj);
                        }
                    }
                    pthread_cond_signal(topicSyncResult->condition);
                    pthread_mutex_unlock(topicSyncResult->mutex);
                }
                    break;
                case TASK_UNSUBSCRIBE_TOPIC:
                {
                    NS_LOG(DEBUG, "CASE TASK_UNSUBSCRIBE_TOPIC : ");
                    NSTopicSyncResult * topicSyncResult = (NSTopicSyncResult *) node->taskData;
                    pthread_mutex_lock(topicSyncResult->mutex);
                    NSCacheTopicSubData * topicSubData =
                            (NSCacheTopicSubData *) topicSyncResult->topicData;

                    if (NSProviderDeleteConsumerTopic(consumerTopicList, topicSubData) == NS_OK)
                    {
                        NSSendTopicUpdationToConsumer(topicSubData->id);
                        topicSyncResult->result = NS_OK;
                    }

                    NSOICFree(topicSubData->topicName);
                    NSOICFree(topicSubData);
                    pthread_cond_signal(topicSyncResult->condition);
                    pthread_mutex_unlock(topicSyncResult->mutex);

                }
                    break;
                case TASK_REGISTER_TOPIC:
                {
                    NS_LOG(DEBUG, "CASE TASK_ADD_TOPIC : ");
                    NSTopicSyncResult * topicSyncResult = (NSTopicSyncResult *) node->taskData;

                    pthread_mutex_lock(topicSyncResult->mutex);
                    topicSyncResult->result = NSRegisterTopic(
                            (const char *) topicSyncResult->topicData);
                    pthread_cond_signal(topicSyncResult->condition);
                    pthread_mutex_unlock(topicSyncResult->mutex);
                }
                    break;
                case TASK_UNREGISTER_TOPIC:
                {
                    NS_LOG(DEBUG, "CASE_TASK_DELETE_TOPIC : ");
                    NSTopicSyncResult * topicSyncResult = (NSTopicSyncResult *) node->taskData;
                    pthread_mutex_lock(topicSyncResult->mutex);
                    topicSyncResult->result = NSUnregisterTopic(
                            (const char *) topicSyncResult->topicData);
                    pthread_cond_signal(topicSyncResult->condition);
                    pthread_mutex_unlock(topicSyncResult->mutex);
                }
                    break;
                case TASK_POST_TOPIC:
                {
                    NS_LOG(DEBUG, "TASK_POST_TOPIC : ");
                    NSPostConsumerTopics((OCEntityHandlerRequest*) node->taskData);
                    NSFreeOCEntityHandlerRequest((OCEntityHandlerRequest*) node->taskData);
                }
                    break;
                case TASK_GET_TOPICS:
                {
                    NS_LOG(DEBUG, "TASK_GET_TOPICS : ");
                    NSTopicSync * topicSync = (NSTopicSync *) node->taskData;
                    pthread_mutex_lock(topicSync->mutex);
                    NSTopicLL * topics = NSProviderGetTopicsCacheData(registeredTopicList);
                    topicSync->topics = topics;
                    pthread_cond_signal(topicSync->condition);
                    pthread_mutex_unlock(topicSync->mutex);
                }
                    break;
                case TAST_GET_CONSUMER_TOPICS:
                {
                    NS_LOG(DEBUG, "TASK_GET_CONSUMER_TOPICS : ");
                    NSTopicSync * topicSync = (NSTopicSync *) node->taskData;
                    pthread_mutex_lock(topicSync->mutex);
                    NSTopicLL * topics = NSProviderGetConsumerTopicsCacheData(registeredTopicList,
                            consumerTopicList, topicSync->consumerId);
                    topicSync->topics = topics;
                    pthread_cond_signal(topicSync->condition);
                    pthread_mutex_unlock(topicSync->mutex);
                }
                    break;
                default:
                    break;
            }

            NSOICFree(node);
        }

        pthread_mutex_unlock(&NSMutex[TOPIC_SCHEDULER]);
    }

    NS_LOG(DEBUG, "Destroy NSTopicSchedule");
    return NULL;
}
