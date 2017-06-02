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

#ifdef WITH_MQ
#include "NSConstants.h"
#include "NSConsumerCommon.h"
#include "NSConsumerMQPlugin.h"
#include "NSUtil.h"

#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"

void NSHandleMQSubscription(NSMQTopicAddress * address);

OCStackApplicationResult NSConsumerIntrospectMQTopic(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse);

OCStackApplicationResult NSConsumerMQListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse);

void NSConsumerMQTaskProcessing(NSTask * task)
{
    NS_VERIFY_NOT_NULL_V(task);

    NS_LOG_V(DEBUG, "Receive Event : %d", (int) task->taskType);

    switch (task->taskType)
    {
        case TASK_MQ_REQ_SUBSCRIBE:
        {
            NSMQTopicAddress * mqTopic = task->taskData;
            NSHandleMQSubscription(mqTopic);
            NSOICFree(mqTopic);
            break;
        }
        default:
        {
            NS_LOG(ERROR, "Unknown type of task");
            break;
        }
    }

    NSOICFree(task);
}

void NSHandleMQSubscription(NSMQTopicAddress * topicAddr)
{
    char * serverUri = topicAddr->serverAddr;
    char * topicName = topicAddr->topicName;

    OCDevAddr * addr = NSChangeAddress(serverUri);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(addr,
                    {
                        NSOICFree(topicAddr->serverAddr);
                        NSOICFree(topicAddr->topicName);
                    });

    char requestUri[100] = "coap+tcp://";
    OICStrcat(requestUri, strlen(requestUri)+strlen(serverUri)+1, serverUri);
    OICStrcat(requestUri, strlen(requestUri)+ strlen("/oic/ps") + 1, "/oic/ps");
    NS_LOG_V(DEBUG, "requestUri = %s", requestUri);

    OCStackResult ret = NSInvokeRequest(NULL, OC_REST_GET, addr, requestUri, NULL,
                      NSConsumerIntrospectMQTopic, topicName, OICFree, CT_DEFAULT);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING_V(NSOCResultToSuccess(ret) == true ? (void *)1 : NULL,
                   {
                       NSOICFree(serverUri);
                       NSOICFree(topicName);
                   });

    NSOICFree(serverUri);
}

OCStackApplicationResult NSConsumerIntrospectMQTopic(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) handle;

    if (!NSIsStartedConsumer())
    {
        return OC_STACK_DELETE_TRANSACTION;
    }

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_NOT_NULL(clientResponse->payload, OC_STACK_KEEP_TRANSACTION);

    NS_LOG(DEBUG, "income get response of MQ broker");
    NS_LOG_V(INFO_PRIVATE, "MQ GET response income : %s:%d",
            clientResponse->devAddr.addr, clientResponse->devAddr.port);
    NS_LOG_V(DEBUG, "MQ GET response result : %d",
            clientResponse->result);
    NS_LOG_V(DEBUG, "MQ GET response sequenceNum : %d",
            clientResponse->sequenceNumber);
    NS_LOG_V(DEBUG, "MQ GET response resource uri : %s",
            clientResponse->resourceUri);
    NS_LOG_V(DEBUG, "MQ GET response Transport Type : %d",
                    clientResponse->devAddr.adapter);

    char ** topicList = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    OCRepPayloadGetStringArray((OCRepPayload *) clientResponse->payload,
                               NS_ATTIRBUTE_MQ_TOPICLIST, & topicList, dimensions);

    char * interestTopicName = (char *) ctx;
    for (size_t i = 0; i < dimensions[0]; ++i)
    {
        NS_LOG_V(DEBUG, "found MQ topic : %s", topicList[i]);
        if (!strcmp(topicList[i], interestTopicName))
        {
            NS_LOG(DEBUG, "subscribe to MQ notification");

            OCStackResult ret = NSInvokeRequest(NULL,
                                  OC_REST_OBSERVE, clientResponse->addr, topicList[i], NULL,
                                  NSConsumerMQListener, NULL, NULL, CT_DEFAULT);

            if (!NSOCResultToSuccess(ret))
            {
                NS_LOG(DEBUG, "fail to subscribe to MQ notification");
                continue;
            }
        }
    }


    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSConsumerMQListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    if (!NSIsStartedConsumer())
    {
        return OC_STACK_DELETE_TRANSACTION;
    }

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_NOT_NULL(clientResponse->payload, OC_STACK_KEEP_TRANSACTION);

    NS_LOG(DEBUG, "income observe response of MQ notification");
    NS_LOG_V(INFO_PRIVATE, "MQ OBS response income : %s:%d",
            clientResponse->devAddr.addr, clientResponse->devAddr.port);
    NS_LOG_V(DEBUG, "MQ OBS response result : %d",
            clientResponse->result);
    NS_LOG_V(DEBUG, "MQ OBS response sequenceNum : %d",
            clientResponse->sequenceNumber);
    NS_LOG_V(DEBUG, "MQ OBS response resource uri : %s",
            clientResponse->resourceUri);
    NS_LOG_V(DEBUG, "MQ OBS response Transport Type : %d",
                    clientResponse->devAddr.adapter);

    NSMessage * newMsg = NSGetMessage((OCRepPayload *)clientResponse->payload);
    NS_VERIFY_NOT_NULL(newMsg, OC_STACK_KEEP_TRANSACTION);

    NSTask * task = NULL;

    if (newMsg->type == NS_MESSAGE_READ || newMsg->type == NS_MESSAGE_DELETED)
    {
        NSSyncInfo * syncInfo = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(syncInfo,
                              OC_STACK_KEEP_TRANSACTION, NSRemoveMessage(newMsg));

        syncInfo->messageId = newMsg->messageId;
        syncInfo->state = (newMsg->type == NS_MESSAGE_READ) ? NS_SYNC_READ : NS_SYNC_DELETED;
        OICStrcpy(syncInfo->providerId, sizeof(char) * NS_DEVICE_ID_LENGTH, newMsg->providerId);

        NSRemoveMessage(newMsg);

        NS_LOG(DEBUG, "build NSTask for MQ message sync");
        task = NSMakeTask(TASK_RECV_SYNCINFO, (void *) syncInfo);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task, OC_STACK_KEEP_TRANSACTION, NSOICFree(syncInfo));
    }
    else
    {
        NS_LOG(DEBUG, "build NSTask for MQ message receive");
        task = NSMakeTask(TASK_CONSUMER_RECV_MESSAGE, (void *) newMsg);
        NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task, OC_STACK_KEEP_TRANSACTION,
                              NSRemoveMessage(newMsg));
    }

    NSConsumerPushEvent(task);

    return OC_STACK_KEEP_TRANSACTION;
}
#endif
