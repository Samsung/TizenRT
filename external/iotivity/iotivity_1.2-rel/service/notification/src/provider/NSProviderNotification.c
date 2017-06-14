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

#include "NSProviderNotification.h"
#include "NSProviderListener.h"
#include "NSProviderSystem.h"

NSResult NSSetMessagePayload(NSMessage *msg, OCRepPayload** msgPayload)
{
    NS_LOG(DEBUG, "NSSetMessagePayload - IN");

    *msgPayload = msg->extraInfo != NULL ? msg->extraInfo : OCRepPayloadCreate();

    if (!*msgPayload)
    {
        NS_LOG(ERROR, "Failed to allocate payload");
        return NS_ERROR;
    }

    OCRepPayloadSetUri(*msgPayload, NS_COLLECTION_MESSAGE_URI);
    OCRepPayloadSetPropInt(*msgPayload, NS_ATTRIBUTE_MESSAGE_ID, msg->messageId);
    OCRepPayloadSetPropString(*msgPayload, NS_ATTRIBUTE_PROVIDER_ID, msg->providerId);

    NSDuplicateSetPropertyInt(msgPayload, NS_ATTRIBUTE_TYPE, msg->type);
    NSDuplicateSetPropertyInt(msgPayload, NS_ATTRIBUTE_TTL, msg->ttl);
    NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_DATETIME, msg->dateTime);
    NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_TITLE, msg->title);
    NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_TEXT, msg->contentText);
    NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_SOURCE, msg->sourceName);
    NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_TOPIC_NAME, msg->topic);

    if (msg->mediaContents)
    {
        NSDuplicateSetPropertyString(msgPayload, NS_ATTRIBUTE_ICON_IMAGE,
                msg->mediaContents->iconImage);
    }

    NS_LOG(DEBUG, "NSSetMessagePayload - OUT");
    return NS_OK;
}

NSResult NSSetSyncPayload(NSSyncInfo *sync, OCRepPayload** syncPayload)
{
    NS_LOG(DEBUG, "NSSetSyncPayload - IN");

    *syncPayload = OCRepPayloadCreate();

    if (!*syncPayload)
    {
        NS_LOG(ERROR, "Failed to allocate payload");
        return NS_ERROR;
    }

    OCRepPayloadSetUri(*syncPayload, NS_COLLECTION_SYNC_URI);

    OCRepPayloadSetPropString(*syncPayload, NS_ATTRIBUTE_PROVIDER_ID, sync->providerId);
    OCRepPayloadSetPropInt(*syncPayload, NS_ATTRIBUTE_MESSAGE_ID, sync->messageId);
    OCRepPayloadSetPropInt(*syncPayload, NS_ATTRIBUTE_STATE, sync->state);

    NS_LOG(DEBUG, "NSSetSyncPayload - OUT");
    return NS_OK;
}

#ifdef WITH_MQ
OCStackResult NSProviderPublishTopic(OCRepPayload * payload, OCClientResponseHandler response)
{
    NS_LOG(DEBUG, "NSProviderPublishTopic - IN");
    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = response;
    cbData.cd = NULL;
    cbData.context = NULL;

    NSMQServerInfo * serverInfo = NSGetMQServerInfo();

    if (!serverInfo)
    {
        NS_LOG(DEBUG, "serverInfo is not NULL");
        NS_LOG_V(DEBUG, "serverInfo->serverUri = %s", serverInfo->serverUri);
    }

    NS_LOG(DEBUG, "NSProviderPublishTopic - OUT");

    return OCDoResource(NULL, OC_REST_POST, serverInfo->serverUri, serverInfo->devAddr,
            (OCPayload *)payload, CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
}
#endif

NSResult NSSendNotification(NSMessage *msg)
{
    NS_LOG(DEBUG, "NSSendMessage - IN");

    OCResourceHandle rHandle;
    OCObservationId obArray[255] = { 0, };
    int obCount = 0, i;

    if (NSPutMessageResource(msg, &rHandle) != NS_OK)
    {
        NS_LOG(ERROR, "fail to Put notification resource");
        return NS_ERROR;
    }

    OCRepPayload* payload = NULL;

    if (NSSetMessagePayload(msg, &payload) != NS_OK)
    {
        NS_LOG(ERROR, "fail to Get message payload");
        return NS_ERROR;
    }

#ifdef WITH_MQ
    if (NSGetMQServerInfo())
    {
        NSProviderPublishTopic(OCRepPayloadClone(payload), NSProviderPublishMQResponseCB);
    }
#endif

    if (consumerSubList->head == NULL)
    {
        NS_LOG(ERROR, "SubList->head is NULL, empty SubList");
        OCRepPayloadDestroy(payload);
        msg->extraInfo = NULL;
        return NS_ERROR;
    }

    NSCacheElement * it = consumerSubList->head;

    while (it)
    {
        NSCacheSubData * subData = (NSCacheSubData *) it->data;
        NS_LOG_V(INFO_PRIVATE, "message subData->id = %s", subData->id);
        NS_LOG_V(DEBUG, "subData->messageId = %d", subData->messageObId);
        NS_LOG_V(DEBUG, "subData->cloud_messageId = %d", subData->remote_messageObId);
        NS_LOG_V(DEBUG, "subData->syncId = %d", subData->syncObId);
        NS_LOG_V(DEBUG, "subData->cloud_syncId = %d", subData->remote_syncObId);
        NS_LOG_V(DEBUG, "subData->isWhite = %d", subData->isWhite);

        if (subData->isWhite)
        {
            if(subData->messageObId != 0)
            {
                if (msg->topic && (msg->topic)[0] != '\0')
                {
                    NS_LOG_V(DEBUG, "this is topic message: %s", msg->topic);

                    if (NSProviderIsTopicSubScribed(consumerTopicList->head, subData->id, msg->topic))
                    {
                        obArray[obCount++] = subData->messageObId;
                    }
                }
                else
                {
                    obArray[obCount++] = subData->messageObId;
                }
            }

#if (defined WITH_CLOUD)
            if (subData->remote_messageObId != 0)
            {
                if (msg->topic && (msg->topic)[0] != '\0')
                {
                    NS_LOG_V(DEBUG, "this is topic message via remote server: %s", msg->topic);
                    if (NSProviderIsTopicSubScribed(consumerTopicList->head, subData->id, msg->topic))
                    {
                        obArray[obCount++] = subData->remote_messageObId;
                    }
                }
                else
                {
                    obArray[obCount++] = subData->remote_messageObId;
                }
            }
#endif

        }
        it = it->next;
    }

    for (i = 0; i < obCount; ++i)
    {
        NS_LOG(DEBUG, "-------------------------------------------------------message\n");
        NS_LOG_V(DEBUG, "SubScription WhiteList[%d] = %d", i, obArray[i]);
        NS_LOG(DEBUG, "-------------------------------------------------------message\n");
    }

    if (!obCount)
    {
        NS_LOG(ERROR, "observer count is zero");
        OCRepPayloadDestroy(payload);
        msg->extraInfo = NULL;
        return NS_ERROR;
    }

    OCStackResult ocstackResult = OCNotifyListOfObservers(rHandle, obArray, obCount, payload,
            OC_LOW_QOS);

    NS_LOG_V(DEBUG, "Message ocstackResult = %d", ocstackResult);

    if (ocstackResult != OC_STACK_OK)
    {
        NS_LOG(ERROR, "fail to send message");
        OCRepPayloadDestroy(payload);
        msg->extraInfo = NULL;
        return NS_ERROR;
    }

    OCRepPayloadDestroy(payload);
    msg->extraInfo = NULL;

    NS_LOG(DEBUG, "NSSendMessage - OUT");
    return NS_OK;
}

NSResult NSSendSync(NSSyncInfo *sync)
{
    NS_LOG(DEBUG, "NSSendSync - IN");

    OCObservationId obArray[255] = { 0, };
    int obCount = 0;
    int i;

    OCResourceHandle rHandle;
    if (NSPutSyncResource(sync, &rHandle) != NS_OK)
    {
        NS_LOG(ERROR, PCF("Fail to put sync resource"));
        return NS_ERROR;
    }

    NSCacheElement * it = consumerSubList->head;

    while (it)
    {
        NSCacheSubData * subData = (NSCacheSubData *) it->data;
        NS_LOG_V(INFO_PRIVATE, "sync subData->id = %s", subData->id);
        NS_LOG_V(DEBUG, "subData->messageId = %d", subData->messageObId);
        NS_LOG_V(DEBUG, "subData->cloud_messageId = %d", subData->remote_messageObId);
        NS_LOG_V(DEBUG, "subData->syncId = %d", subData->syncObId);
        NS_LOG_V(DEBUG, "subData->cloud_syncId = %d", subData->remote_syncObId);
        NS_LOG_V(DEBUG, "subData->isWhite = %d", subData->isWhite);

        if (subData->isWhite)
        {
            if (subData->syncObId != 0)
            {
                obArray[obCount++] = subData->syncObId;
            }

#if (defined WITH_CLOUD)
            if (subData->remote_syncObId != 0)
            {
                obArray[obCount++] = subData->remote_syncObId;
            }
#endif
        }
        it = it->next;
    }

    OCRepPayload* payload = NULL;
    if (NSSetSyncPayload(sync, &payload) != NS_OK)
    {
        NS_LOG(ERROR, "Failed to allocate payload");
        return NS_ERROR;
    }

#ifdef WITH_MQ
    if (NSGetMQServerInfo())
    {
        OCRepPayload* MQPayload = OCRepPayloadClone(payload);
        NSMessageType MQType = 0;

        if (sync->state == NS_SYNC_READ)
        {
            MQType = NS_MESSAGE_READ;
        }
        else if (sync->state == NS_SYNC_DELETED)
        {
            MQType = NS_MESSAGE_DELETED;
        }

        OCRepPayloadSetPropInt(MQPayload, NS_ATTRIBUTE_TYPE, (int64_t) MQType);
        NSProviderPublishTopic(MQPayload, NSProviderPublishMQResponseCB);
    }
#endif

    for (i = 0; i < obCount; ++i)
    {
        NS_LOG(DEBUG, "-------------------------------------------------------message\n");
        NS_LOG_V(DEBUG, "Sync WhiteList[%d] = %d", i, obArray[i]);
        NS_LOG(DEBUG, "-------------------------------------------------------message\n");
    }

    OCStackResult ocstackResult = OCNotifyListOfObservers(rHandle, obArray,
            obCount, payload, OC_LOW_QOS);

    NS_LOG_V(DEBUG, "Sync ocstackResult = %d", ocstackResult);
    if (ocstackResult != OC_STACK_OK)
    {
        NS_LOG(ERROR, "fail to send Sync");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;
    }

    OCRepPayloadDestroy(payload);

    NS_LOG(DEBUG, "NSSendSync - OUT");
    return NS_OK;
}

void * NSNotificationSchedule(void *ptr)
{
    if (ptr == NULL)
    {
        NS_LOG(DEBUG, "Create NSNotifiactionSchedule");
    }

    while (NSIsRunning[NOTIFICATION_SCHEDULER])
    {
        sem_wait(&NSSemaphore[NOTIFICATION_SCHEDULER]);
        pthread_mutex_lock(&NSMutex[NOTIFICATION_SCHEDULER]);

        if (NSHeadMsg[NOTIFICATION_SCHEDULER] != NULL)
        {
            NSTask *node = NSHeadMsg[NOTIFICATION_SCHEDULER];
            NSHeadMsg[NOTIFICATION_SCHEDULER] = node->nextTask;

            switch (node->taskType)
            {
                case TASK_SEND_NOTIFICATION:
                {
                    NS_LOG(DEBUG, "CASE TASK_SEND_NOTIFICATION : ");
                    NSSendNotification((NSMessage *)node->taskData);
                    NSFreeMessage((NSMessage *)node->taskData);
                }
                    break;
                case TASK_SEND_READ:
                    NS_LOG(DEBUG, "CASE TASK_SEND_READ : ");
                    NSSendSync((NSSyncInfo*) node->taskData);
                    NSFreeSync((NSSyncInfo*) node->taskData);
                    break;
                case TASK_RECV_READ:
                    NS_LOG(DEBUG, "CASE TASK_RECV_READ : ");
                    NSSendSync((NSSyncInfo*) node->taskData);
                    NSPushQueue(CALLBACK_RESPONSE_SCHEDULER, TASK_CB_SYNC, node->taskData);
                    break;
                default:
                    NS_LOG(ERROR, "Unknown type message");
                    break;

            }
            NSOICFree(node);
        }

        pthread_mutex_unlock(&NSMutex[NOTIFICATION_SCHEDULER]);
    }

    NS_LOG(INFO, "Destroy NSNotificationSchedule");
    return NULL;
}
