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

#include "NSConsumerCommunication.h"

#include "NSConstants.h"
#include "NSUtil.h"
#include "NSConsumerCommon.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"

#define NS_SYNC_URI "/notification/sync"

NSSyncInfo * NSCreateSyncInfo_consumer(uint64_t msgId, const char * providerId, NSSyncType state);

NSSyncInfo * NSGetSyncInfoc(OCClientResponse * clientResponse);
NSTopicLL * NSGetTopicLL(OCClientResponse * clientResponse);

char * NSGetCloudUri(const char * providerId, char * uri);

NSResult NSUpdateObserveResult(NSProvider_internal * prov, char * query)
{
    NSOICFree(query);

    NSProvider_internal * taskProvider = NSCopyProvider_internal(prov);
    NSTask * task = NSMakeTask(TASK_CONSUMER_SENT_REQ_OBSERVE, (void *) taskProvider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task,
                    NS_ERROR, NSRemoveProvider_internal(taskProvider));

    NSConsumerPushEvent(task);

    return NS_OK;
}

NSResult NSConsumerSubscribeProvider(NSProvider * provider)
{
    NSProvider_internal * provider_internal = (NSProvider_internal *) provider;
    NS_VERIFY_NOT_NULL(provider_internal, NS_ERROR);

    NSProviderConnectionInfo * connections = provider_internal->connection;
    while(connections)
    {
        if (connections->isSubscribing == true)
        {
            connections = connections->next;
            continue;
        }

        char * msgUri = OICStrdup(provider_internal->messageUri);
        NS_VERIFY_NOT_NULL(msgUri, NS_ERROR);
        char * syncUri = OICStrdup(provider_internal->syncUri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(syncUri, NS_ERROR, NSOICFree(msgUri));

        OCConnectivityType type = CT_DEFAULT;
        if (connections->addr->adapter == OC_ADAPTER_TCP)
        {
            type = CT_ADAPTER_TCP;
            if (connections->isCloudConnection == true)
            {
                msgUri = NSGetCloudUri(provider_internal->providerId, msgUri);
                NS_VERIFY_NOT_NULL(msgUri, NS_ERROR);
                syncUri = NSGetCloudUri(provider_internal->providerId, syncUri);
                NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(syncUri, NS_ERROR, NSOICFree(msgUri));
            }
        }

        NS_LOG_V(INFO_PRIVATE, "subscribe to %s:%d", connections->addr->addr, connections->addr->port);

        NS_LOG(DEBUG, "get subscribe message query");
        char * query = NULL;
        query = NSMakeRequestUriWithConsumerId(msgUri);
        NS_VERIFY_NOT_NULL(query, NS_ERROR);

        NS_LOG(DEBUG, "subscribe message");
        NS_LOG_V(DEBUG, "subscribe query : %s", query);
        OCStackResult ret = NSInvokeRequest(&(connections->messageHandle),
                              OC_REST_OBSERVE, connections->addr, query, NULL,
                              NSConsumerMessageListener, NULL, NULL, type);
        NS_VERIFY_STACK_SUCCESS_WITH_POST_CLEANING(NSOCResultToSuccess(ret),
                            NS_ERROR, NSUpdateObserveResult(provider_internal, query));
        NSOICFree(query);
        NSOICFree(msgUri);

        NS_LOG(DEBUG, "get subscribe sync query");
        query = NSMakeRequestUriWithConsumerId(syncUri);
        NS_VERIFY_NOT_NULL(query, NS_ERROR);

        NS_LOG(DEBUG, "subscribe sync");
        NS_LOG_V(DEBUG, "subscribe query : %s", query);
        ret = NSInvokeRequest(&(connections->syncHandle),
                              OC_REST_OBSERVE, connections->addr, query, NULL,
                              NSConsumerSyncInfoListener, NULL, NULL, type);
        NS_VERIFY_STACK_SUCCESS_WITH_POST_CLEANING(NSOCResultToSuccess(ret),
                            NS_ERROR, NSUpdateObserveResult(provider_internal, query));
        NSOICFree(query);
        NSOICFree(syncUri);

        connections->isSubscribing = true;

        connections = connections->next;
    }

    NSProvider_internal * taskProvider = NSCopyProvider_internal(provider_internal);
    NSTask * task = NSMakeTask(TASK_CONSUMER_SENT_REQ_OBSERVE, (void *) taskProvider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task,
                    NS_ERROR, NSRemoveProvider_internal(taskProvider));

    NSConsumerPushEvent(task);

    return NS_OK;
}

OCStackApplicationResult NSConsumerCheckPostResult(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(
            NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

    return OC_STACK_KEEP_TRANSACTION;
}

void NSRemoveSyncInfoObj(NSSyncInfo * sync)
{
    NSOICFree(sync);
}

OCStackApplicationResult NSConsumerSyncInfoListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(
            NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

    NS_LOG(DEBUG, "get NSSyncInfo");
    NSSyncInfo * newSync = NSGetSyncInfoc(clientResponse);
    NS_VERIFY_NOT_NULL(newSync, OC_STACK_KEEP_TRANSACTION);

    NSTaskType taskType = TASK_RECV_SYNCINFO;

    NS_LOG(DEBUG, "build NSTask");
    NSTask * task = NSMakeTask(taskType, (void *) newSync);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task,
               OC_STACK_KEEP_TRANSACTION, NSRemoveSyncInfoObj(newSync));

    NSConsumerPushEvent(task);

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSConsumerMessageListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

    NS_LOG(DEBUG, "build NSMessage");
    NSMessage * newNoti = NSGetMessage((OCRepPayload *) clientResponse->payload);
    NS_VERIFY_NOT_NULL(newNoti, OC_STACK_KEEP_TRANSACTION);

    NSTaskType type = TASK_CONSUMER_RECV_MESSAGE;

    if (newNoti->messageId == NS_ALLOW || newNoti->messageId == NS_DENY)
    {
        NS_LOG(DEBUG, "Receive subscribe result");
        type = TASK_CONSUMER_RECV_PROVIDER_CHANGED;
    }
    else if (newNoti->messageId == NS_TOPIC)
    {
        NS_LOG(DEBUG, "Receive Topic change");
        type = TASK_CONSUMER_REQ_TOPIC_URI;
    }
    else
    {
        NS_LOG(DEBUG, "Receive new message");
    }

    NS_LOG(DEBUG, "build NSTask");
    NSTask * task = NSMakeTask(type, (void *) newNoti);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task, NS_ERROR, NSRemoveMessage(newNoti));

    NSConsumerPushEvent(task);

    return OC_STACK_KEEP_TRANSACTION;
}

void NSGetMessagePostClean(char * pId, OCDevAddr * addr)
{
    NSOICFree(pId);
    NSOICFree(addr);
}

NSSyncInfo * NSGetSyncInfoc(OCClientResponse * clientResponse)
{
    NS_VERIFY_NOT_NULL(clientResponse->payload, NULL);

    OCRepPayload * payload = (OCRepPayload *)clientResponse->payload;

    NS_LOG(DEBUG, "get msg id");
    uint64_t id = NULL;
    bool getResult = OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, (int64_t *)&id);
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "get provider id");
    char * pId = NULL;
    getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, &pId);
    NS_VERIFY_NOT_NULL(getResult == true ? (void *) 1 : NULL, NULL);

    NS_LOG(DEBUG, "get state");
    int64_t state = 0;
    getResult = OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_STATE, & state);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(getResult == true ? (void *) 1 : NULL,
            NULL, NSOICFree(pId));

    NS_LOG(DEBUG, "create NSSyncInfo");
    NSSyncInfo * retSync = NSCreateSyncInfo_consumer(id, pId, (NSSyncType)state);
    NSOICFree(pId);
    NS_VERIFY_NOT_NULL(retSync, NULL);

    NS_LOG_V(DEBUG, "Sync ID : %lld", (long long int)retSync->messageId);
    NS_LOG_V(DEBUG, "Sync State : %d", (int) retSync->state);
    NS_LOG_V(INFO_PRIVATE, "Sync Provider ID : %s", retSync->providerId);

    return retSync;
}

NSSyncInfo * NSCreateSyncInfo_consumer(uint64_t msgId, const char * providerId, NSSyncType state)
{
    NS_VERIFY_NOT_NULL(providerId, NULL);

    NSSyncInfo * retSync = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));
    NS_VERIFY_NOT_NULL(retSync, NULL);

    retSync->messageId = msgId;
    retSync->state = state;
    OICStrcpy(retSync->providerId, sizeof(char) * NS_DEVICE_ID_LENGTH, providerId);

    return retSync;
}

OCStackResult NSSendSyncInfo(NSSyncInfo * syncInfo, OCDevAddr * addr)
{
    NS_VERIFY_NOT_NULL(syncInfo, OC_STACK_ERROR);
    NS_VERIFY_NOT_NULL(addr, OC_STACK_ERROR);

    OCRepPayload * payload = OCRepPayloadCreate();
    NS_VERIFY_NOT_NULL(payload, OC_STACK_ERROR);

    OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, (int64_t)syncInfo->messageId);
    OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_STATE, syncInfo->state);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, syncInfo->providerId);

    char * uri = (char*)OICStrdup(NS_SYNC_URI);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(uri, OC_STACK_ERROR, OCRepPayloadDestroy(payload));

    OCConnectivityType type = CT_DEFAULT;
    if(addr->adapter == OC_ADAPTER_TCP)
    {
        type = CT_ADAPTER_TCP;
        uri = NSGetCloudUri(syncInfo->providerId, uri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(uri, OC_STACK_ERROR, OCRepPayloadDestroy(payload));
    }

    OCStackResult ret = NSInvokeRequest(NULL, OC_REST_POST, addr,
                            uri, (OCPayload*)payload,
                            NSConsumerCheckPostResult, NULL, NULL, type);
    NSOICFree(uri);

    return ret;
}

char * NSGetCloudUri(const char * providerId, char * uri)
{
    size_t uriLen = NS_DEVICE_ID_LENGTH + 1 + strlen(uri) + 1 + 10;
    char * retUri = (char *)OICMalloc(uriLen);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(retUri, NULL, NSOICFree(uri));

    snprintf(retUri, uriLen, "/oic/route/%s%s", providerId, uri);
    NSOICFree(uri);
    NS_LOG_V(DEBUG, "Cloud uri : %s", retUri);

    return retUri;
}

void NSConsumerCommunicationTaskProcessing(NSTask * task)
{
    NS_VERIFY_NOT_NULL_V(task);

    NS_LOG_V(DEBUG, "Receive Event : %d", (int)task->taskType);
    if (task->taskType == TASK_CONSUMER_REQ_SUBSCRIBE)
    {
        NS_VERIFY_NOT_NULL_V(task->taskData);
        NS_LOG(DEBUG, "Request Subscribe");
        NSResult ret = NSConsumerSubscribeProvider((NSProvider *)task->taskData);
        NSRemoveProvider_internal((void *) task->taskData);
        NS_VERIFY_NOT_NULL_V(ret == NS_OK ? (void *)1 : NULL);
    }
    else if (task->taskType == TASK_SEND_SYNCINFO)
    {
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(task->taskData, NSOICFree(task));
        NSSyncInfo_internal * syncInfo = (NSSyncInfo_internal *)task->taskData;
        NSProviderConnectionInfo * info = syncInfo->connection;

        while(info)
        {
            OCStackResult ret = NSSendSyncInfo((NSSyncInfo *)(task->taskData), info->addr);
            if (ret != OC_STACK_OK)
            {
                NS_LOG_V(ERROR, "send sync info fail : %d", info->addr->adapter);
            }

            info = info->next;
        }

        NSRemoveConnections(syncInfo->connection);
        NSOICFree(syncInfo);
    }
    else if (task->taskType == TASK_CONSUMER_REQ_SUBSCRIBE_CANCEL)
    {
        NSProvider_internal * provider = (NSProvider_internal *)task->taskData;
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(provider,
        {
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        NSProviderConnectionInfo * connections = provider->connection;
        while(connections)
        {
            if (connections->isSubscribing == false)
            {
                NS_LOG_V(INFO_PRIVATE, "unsubscribed to %s:%d",
                     connections->addr->addr, connections->addr->port);
                connections = connections->next;
                continue;
            }
            NS_LOG_V(INFO_PRIVATE, "cancel subscribe to %s:%d",
                     connections->addr->addr, connections->addr->port);
            OCCancel(connections->messageHandle, NS_QOS, NULL, 0);
            OCCancel(connections->syncHandle, NS_QOS, NULL, 0);
            connections->messageHandle = NULL;
            connections->syncHandle = NULL;
            connections->isSubscribing = false;
            connections = connections->next;
        }

        NSRemoveProvider_internal(provider);
    }
    else if (task->taskType == TASK_CONSUMER_REQ_TOPIC_LIST)
    {
        NSProvider_internal * provider = NSCopyProvider_internal(task->taskData);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(provider,
        {
            NSRemoveProvider_internal((void *) task->taskData);
            NSOICFree(task);
        });
        NSRemoveProvider_internal((NSProvider_internal *)task->taskData);

        NSProviderConnectionInfo * connections = provider->connection;
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(connections,
        {
            NSRemoveProvider_internal((void *) provider);
            NSOICFree(task);
        });

        char * topicUri = OICStrdup(provider->topicUri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(topicUri,
        {
            NSRemoveProvider_internal((void *) provider);
            NSOICFree(task);
        });

        OCConnectivityType type = CT_DEFAULT;
        if (connections->addr->adapter == OC_ADAPTER_TCP)
        {
            type = CT_ADAPTER_TCP;
            if (connections->isCloudConnection == true)
            {
                topicUri = NSGetCloudUri(provider->providerId, topicUri);
            }
        }

        NS_LOG(DEBUG, "get topic query");
        char * query = NSMakeRequestUriWithConsumerId(topicUri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(query,
        {
            NSRemoveProvider_internal((void *) provider);
            NSOICFree(task);
        });
        NS_LOG_V(DEBUG, "topic query : %s", query);

        OCStackResult ret = NSInvokeRequest(NULL, OC_REST_GET, connections->addr,
                                query, NULL, NSIntrospectTopic, (void *) provider,
                                NSRemoveProvider_internal, type);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(NSOCResultToSuccess(ret) == true ? (void *) 1 : NULL,
        {
            NSRemoveProvider_internal((void *) provider);
            NSOICFree(task);
        });

        NSOICFree(query);
        NSOICFree(topicUri);
    }
    else if (task->taskType == TASK_CONSUMER_SELECT_TOPIC_LIST)
    {
        NSProvider_internal * provider = (NSProvider_internal *)task->taskData;
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(provider,
        {
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        NSProviderConnectionInfo * connections = provider->connection;
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(connections,
        {
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        OCRepPayload * payload = OCRepPayloadCreate();
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(payload,
        {
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        NSTopicLL * topicLL = provider->topicLL;
        NSTopicLL * iter = topicLL;
        int topicLLSize = 0;
        while (iter)
        {
            topicLLSize ++;
            NS_LOG_V(DEBUG, "[%d] Topic Name:%s\tTopic State:%d",
                                        topicLLSize, iter->topicName, iter->state);
            iter = (NSTopicLL *) iter->next;
        }

        OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_CONSUMER_ID, *NSGetConsumerId());
        NS_LOG_V(INFO_PRIVATE, "NS_ATTRIBUTE_CONSUMER_ID: %s", *NSGetConsumerId());

        iter = topicLL;
        int iterSize = 0;
        NS_LOG_V(DEBUG, "DimensionSize: %d", topicLLSize);

        OCRepPayload ** topicPayload = NULL;
        if (topicLLSize > 0)
        {
            topicPayload = (OCRepPayload **) OICMalloc(sizeof(OCRepPayload *)*topicLLSize);
            NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(topicPayload,
            {
                OCRepPayloadDestroy(payload);
                NSRemoveProvider_internal(provider);
                NSOICFree(task);
            });

            while (iter || iterSize < topicLLSize)
            {
                topicPayload[iterSize] = OCRepPayloadCreate();
                OCRepPayloadSetPropString(topicPayload[iterSize], NS_ATTRIBUTE_TOPIC_NAME,
                                            iter->topicName);
                OCRepPayloadSetPropInt(topicPayload[iterSize], NS_ATTRIBUTE_TOPIC_SELECTION,
                                            iter->state);
                NS_LOG_V(DEBUG, "NS_ATTRIBUTE_TOPIC_NAME: %s", iter->topicName);
                NS_LOG_V(DEBUG, "NS_ATTRIBUTE_TOPIC_SELECTION: %d", iter->state);
                iterSize++;
                iter = iter->next;
            }
            size_t dimensions[3] = {topicLLSize, 0, 0};

            OCRepPayloadSetPropObjectArrayAsOwner(payload, NS_ATTRIBUTE_TOPIC_LIST,
                                                    topicPayload, dimensions);
        }
        else
        {
            OCRepPayloadSetNull(payload, NS_ATTRIBUTE_TOPIC_LIST);
        }

        char * topicUri = OICStrdup(provider->topicUri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(topicUri,
        {
            OCRepPayloadDestroy(payload);
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        OCConnectivityType type = CT_DEFAULT;
        if (connections->addr->adapter == OC_ADAPTER_TCP)
        {
            type = CT_ADAPTER_TCP;
            if (connections->isCloudConnection == true)
            {
                topicUri = NSGetCloudUri(provider->providerId, topicUri);
            }
        }

        NS_LOG(DEBUG, "get topic query");
        char * query = NULL;
        query = NSMakeRequestUriWithConsumerId(topicUri);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(query,
        {
            NSOICFree(topicUri);
            OCRepPayloadDestroy(payload);
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });
        NS_LOG_V(DEBUG, "topic query : %s", query);

        OCStackResult ret = NSInvokeRequest(NULL, OC_REST_POST, connections->addr,
                                query, (OCPayload*)payload, NSConsumerCheckPostResult,
                                NULL, NULL, type);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(
                NSOCResultToSuccess(ret) == true ? (void *) 1 : NULL,
        {
            NSOICFree(query);
            NSOICFree(topicUri);
            NSRemoveProvider_internal(provider);
            NSOICFree(task);
        });

        NSRemoveProvider_internal(provider);
        NSOICFree(query);
        NSOICFree(topicUri);
    }
    else
    {
        NS_LOG(ERROR, "Unknown type message");
    }

    NSOICFree(task);
}

NSTopicLL * NSGetTopicLL(OCClientResponse * clientResponse)
{
    NS_LOG(DEBUG, "create NSTopicLL");
    NS_VERIFY_NOT_NULL(clientResponse->payload, NULL);

    OCRepPayload * payload = (OCRepPayload *)clientResponse->payload;
    OCRepPayloadValue * value = payload->values;
    while (value)
    {
        NS_LOG_V(DEBUG, "Payload Key : %s", value->name);
        value = value->next;
    }

    char * consumerId = NULL;
    OCRepPayload ** topicLLPayload = NULL;

    NS_LOG(DEBUG, "get information of consumerId");

    bool getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_CONSUMER_ID, & consumerId);
    NSOICFree(consumerId);

    OCRepPayloadValue * payloadValue = NULL;
    payloadValue = NSPayloadFindValue(payload, NS_ATTRIBUTE_TOPIC_LIST);
    NS_VERIFY_NOT_NULL(payloadValue, NULL);

    size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
    NS_LOG_V(DEBUG, "DimensionSize: %d", (int)dimensionSize);

    if (dimensionSize == 0 || payloadValue->type == OCREP_PROP_NULL ||
            payloadValue->arr.objArray == NULL)
    {
        NS_LOG(DEBUG, "No TopicLL");
        return NULL;
    }

    topicLLPayload = payloadValue->arr.objArray;

    NSTopicLL * topicLL = NULL;
    for (int i = 0; i < (int)dimensionSize; i++)
    {
        char * topicName = NULL;
        int64_t state = 0;

        NSTopicLL * topicNode = (NSTopicLL *) OICMalloc(sizeof(NSTopicLL));
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(topicNode, NULL, NSRemoveTopicLL(topicLL));

        NS_LOG(DEBUG, "get topic selection");
        getResult = OCRepPayloadGetPropInt(topicLLPayload[i],
                NS_ATTRIBUTE_TOPIC_SELECTION, & state);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(getResult == true ? (void *) 1 : NULL,
                NULL, NSRemoveTopicLL(topicLL));

        NS_LOG(DEBUG, "get topic name");
        getResult = OCRepPayloadGetPropString(topicLLPayload[i],
                NS_ATTRIBUTE_TOPIC_NAME, & topicName);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(getResult == true ? (void *) 1 : NULL,
                NULL, NSRemoveTopicLL(topicLL));
        NS_LOG_V(DEBUG, "topic name: %s", topicName);
        NS_LOG_V(DEBUG, "topic selection: %d", (int)state);

        topicNode->topicName = topicName;
        topicNode->state = state;

        if (i == 0)
        {
            topicLL = topicNode;
            topicNode->next = NULL;
            continue;
        }

        NSResult ret = NSInsertTopicNode(topicLL, topicNode);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(ret == NS_OK ? (void *)1 : NULL,
                                                    NULL, NSRemoveTopicLL(topicLL));
    }

    return topicLL;
}

OCStackApplicationResult NSIntrospectTopic(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result),
                            OC_STACK_KEEP_TRANSACTION)

    NS_LOG_V(INFO_PRIVATE, "GET response income : %s:%d",
            clientResponse->devAddr.addr, clientResponse->devAddr.port);
    NS_LOG_V(DEBUG, "GET response result : %d",
            clientResponse->result);
    NS_LOG_V(DEBUG, "GET response sequenceNum : %d",
            clientResponse->sequenceNumber);
    NS_LOG_V(DEBUG, "GET response resource uri : %s",
            clientResponse->resourceUri);
    NS_LOG_V(DEBUG, "GET response Transport Type : %d",
            clientResponse->devAddr.adapter);

    NSTopicLL * newTopicLL = NSGetTopicLL(clientResponse);

    NSProvider_internal * provider = NSCopyProvider_internal((NSProvider_internal *) ctx);
    NS_VERIFY_NOT_NULL(provider, OC_STACK_KEEP_TRANSACTION);
    NSRemoveTopicLL(provider->topicLL);
    provider->topicLL = NSCopyTopicLL(newTopicLL);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(provider->topicLL, OC_STACK_KEEP_TRANSACTION,
                          NSRemoveProvider_internal((void *) provider));

    NS_LOG(DEBUG, "build NSTask");
    NSTask * task = NSMakeTask(TASK_CONSUMER_RECV_TOPIC_LIST, (void *) provider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task, NS_ERROR, NSRemoveProvider_internal(provider));

    NSConsumerPushEvent(task);
    NSRemoveTopicLL(newTopicLL);

    return OC_STACK_KEEP_TRANSACTION;
}
