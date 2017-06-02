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

#include "NSProviderSubscription.h"
#include "NSProviderListener.h"

NSResult NSInitSubscriptionList()
{
    NS_LOG(DEBUG, "NSInitSubscriptionList - IN");

    consumerSubList = NSProviderStorageCreate();
    NS_VERIFY_NOT_NULL(consumerSubList, NS_FAIL);
    consumerSubList->cacheType = NS_PROVIDER_CACHE_SUBSCRIBER;

    NS_LOG(DEBUG, "NSInitSubscriptionList - OUT");
    return NS_OK;
}

NSResult NSSetSubscriptionAccessPolicy(bool policy)
{
    NS_LOG(DEBUG, "NSSetSubscriptionAcceptPolicy - IN");

    if (policy == NS_POLICY_PROVIDER)
    {
        NS_LOG(DEBUG, "Place Provider as a subscription accepter");
    }
    else if (policy == NS_POLICY_CONSUMER)
    {
        NS_LOG(DEBUG, "Place Consumer as a subscription accepter");
    }

    NSSetPolicy(policy);

    NS_LOG(DEBUG, "NSSetSubscriptionAcceptPolicy - OUT");
    return NS_OK;
}

NSResult NSSendAccessPolicyResponse(OCEntityHandlerRequest *entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSSendAccessPolicyResponse - IN");

    // put notification resource
    OCResourceHandle notificationResourceHandle = NULL;
    if (NSPutNotificationResource(NSGetPolicy(), &notificationResourceHandle)
            != NS_OK)
    {
        NS_LOG(ERROR, "Fail to put notification resource");
        return NS_ERROR;
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
        return NS_ERROR;
    }

    NS_LOG_V(INFO_PRIVATE, "NS Provider ID: %s", NSGetProviderInfo()->providerId);

    char * copyReq = OICStrdup(entityHandlerRequest->query);
    char * reqInterface = NSGetValueFromQuery(copyReq, NS_QUERY_INTERFACE);

    if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) == 0)
    {
        OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_BASELINE);
        OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_READ);
        OCResourcePayloadAddStringLL(&payload->types, NS_ROOT_TYPE);
    }

    NSOICFree(copyReq);
    OCRepPayloadSetUri(payload, NS_ROOT_URI);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, NSGetProviderInfo()->providerId);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_VERSION, VERSION);
    OCRepPayloadSetPropBool(payload, NS_ATTRIBUTE_POLICY, NSGetPolicy());
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_MESSAGE, NS_COLLECTION_MESSAGE_URI);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_SYNC, NS_COLLECTION_SYNC_URI);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_TOPIC, NS_COLLECTION_TOPIC_URI);

    response.requestHandle = entityHandlerRequest->requestHandle;
    response.resourceHandle = entityHandlerRequest->resource;
    response.persistentBufferFlag = 0;
    response.ehResult = OC_EH_OK;
    response.payload = (OCPayload *) payload;

    // Send Response
    if (OCDoResponse(&response) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to AccessPolicy send response");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;
    }
    OCRepPayloadDestroy(payload);
    NSFreeOCEntityHandlerRequest(entityHandlerRequest);

    NS_LOG(DEBUG, "NSSendAccessPolicyResponse - OUT");
    return NS_OK;
}

void NSHandleSubscription(OCEntityHandlerRequest *entityHandlerRequest, NSResourceType resourceType)
{
    NS_LOG(DEBUG, "NSHandleSubscription - IN");

    char * copyReq = OICStrdup(entityHandlerRequest->query);
    char * id = NSGetValueFromQuery(copyReq, NS_QUERY_CONSUMER_ID);

    if (!id)
    {
        NSOICFree(copyReq);
        NSFreeOCEntityHandlerRequest(entityHandlerRequest);
        NS_LOG(ERROR, "Invalid ConsumerID");
        return;
    }

    NS_LOG_V(INFO_PRIVATE, "consumerId = %s", id);
    if (resourceType == NS_RESOURCE_MESSAGE)
    {
        NS_LOG(DEBUG, "resourceType == NS_RESOURCE_MESSAGE");
        NSCacheElement * element = (NSCacheElement *) OICMalloc(sizeof(NSCacheElement));
        NS_VERIFY_NOT_NULL_V(element);
        NSCacheSubData * subData = (NSCacheSubData *) OICMalloc(sizeof(NSCacheSubData));
        NS_VERIFY_NOT_NULL_V(subData);

        OICStrcpy(subData->id, UUID_STRING_SIZE, id);
        NS_LOG_V(INFO_PRIVATE, "SubList ID = [%s]", subData->id);

        NS_LOG_V(INFO_PRIVATE, "Consumer Address: %s", entityHandlerRequest->devAddr.addr);

        subData->remote_messageObId = subData->messageObId = 0;

        bool iSRemoteServer = false;

#if(defined WITH_CLOUD)
        iSRemoteServer = NSIsRemoteServerAddress(entityHandlerRequest->devAddr.addr);
        if (iSRemoteServer)
        {
            NS_LOG(DEBUG, "Requested by remote server");
            subData->remote_messageObId = entityHandlerRequest->obsInfo.obsId;
            NS_LOG_V(DEBUG, "SubList message observation ID = [%d]", subData->remote_messageObId);
        }
#endif

        if (!iSRemoteServer)
        {
            NS_LOG(DEBUG, "Requested by local consumer");
            subData->messageObId = entityHandlerRequest->obsInfo.obsId;
            NS_LOG_V(DEBUG, "SubList message observation ID = [%d]", subData->messageObId);
        }

        subData->isWhite = false;
        subData->remote_syncObId = 0;
        subData->syncObId = 0;

        element->data = (void*) subData;
        element->next = NULL;

        if (NSProviderStorageWrite(consumerSubList, element) != NS_OK)
        {
            NS_LOG(DEBUG, "fail to write cache");
        }

        bool currPolicy = NSGetPolicy();
        NSAskAcceptanceToUser(NSCopyOCEntityHandlerRequest(entityHandlerRequest));

        if (currPolicy == NS_POLICY_PROVIDER)
        {
            NS_LOG(DEBUG, "NSGetSubscriptionAccepter == NS_ACCEPTER_PROVIDER");
        }
        else if (currPolicy == NS_POLICY_CONSUMER)
        {
            NS_LOG(DEBUG, "NSGetSubscriptionAccepter == NS_ACCEPTER_CONSUMER");
            NSSendConsumerSubResponse(NSCopyOCEntityHandlerRequest(entityHandlerRequest));
        }

        NSFreeOCEntityHandlerRequest(entityHandlerRequest);
    }
    else if (resourceType == NS_RESOURCE_SYNC)
    {
        NS_LOG(DEBUG, "resourceType == NS_RESOURCE_SYNC");
        NSCacheElement * element = (NSCacheElement *) OICMalloc(sizeof(NSCacheElement));
        NS_VERIFY_NOT_NULL_V(element);
        NSCacheSubData * subData = (NSCacheSubData *) OICMalloc(sizeof(NSCacheSubData));
        NS_VERIFY_NOT_NULL_V(subData);

        OICStrcpy(subData->id, UUID_STRING_SIZE, id);
        NS_LOG_V(INFO_PRIVATE, "SubList ID = [%s]", subData->id);

        NS_LOG_V(INFO_PRIVATE, "Consumer Address: %s", entityHandlerRequest->devAddr.addr);

        subData->remote_syncObId = subData->syncObId = 0;
        bool isRemoteServer = false;

#if (defined WITH_CLOUD)
        isRemoteServer = NSIsRemoteServerAddress(entityHandlerRequest->devAddr.addr);
        if (isRemoteServer)
        {
            NS_LOG(DEBUG, "Requested by remote server");
            subData->remote_syncObId = entityHandlerRequest->obsInfo.obsId;
            NS_LOG_V(DEBUG, "SubList sync observation ID = [%d]", subData->remote_syncObId);
        }
#endif

        if (!isRemoteServer)
        {
            NS_LOG(DEBUG, "Requested by local consumer");
            subData->syncObId = entityHandlerRequest->obsInfo.obsId;
            NS_LOG_V(DEBUG, "SubList sync observation ID = [%d]", subData->syncObId);
        }

        subData->isWhite = false;
        subData->messageObId = 0;
        subData->remote_messageObId = 0;

        element->data = (void*) subData;
        element->next = NULL;

        if (NS_OK != NSProviderStorageWrite(consumerSubList, element))
        {
            NS_LOG(ERROR, "Fail to write cache");
        }

        NSFreeOCEntityHandlerRequest(entityHandlerRequest);
    }
    NSOICFree(copyReq);

    NS_LOG(DEBUG, "NSHandleSubscription - OUT");
}

void NSHandleUnsubscription(OCEntityHandlerRequest *entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSHandleUnsubscription - IN");

    consumerSubList->cacheType = NS_PROVIDER_CACHE_SUBSCRIBER_OBSERVE_ID;

    while (NSProviderStorageDelete(consumerSubList, (char *)
            &(entityHandlerRequest->obsInfo.obsId)) != NS_FAIL);

    consumerSubList->cacheType = NS_PROVIDER_CACHE_SUBSCRIBER;
    NSFreeOCEntityHandlerRequest(entityHandlerRequest);
    NS_LOG(DEBUG, "NSHandleUnsubscription - OUT");
}

void NSAskAcceptanceToUser(OCEntityHandlerRequest *entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSAskAcceptanceToUser - IN");

    NSPushQueue(CALLBACK_RESPONSE_SCHEDULER, TASK_CB_SUBSCRIPTION, entityHandlerRequest);

    NS_LOG(DEBUG, "NSAskAcceptanceToUser - OUT");
}

NSResult NSSendResponse(const char * id, bool accepted)
{
    NS_LOG(DEBUG, "NSSendResponse - IN");

    OCRepPayload* payload = OCRepPayloadCreate();
    if (!payload)
    {
        NS_LOG(ERROR, "fail to create playload");
        return NS_ERROR;
    }

    OCResourceHandle rHandle = NULL;
    if (NSPutMessageResource(NULL, &rHandle) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to put notification resource");
        return NS_ERROR;
    }

    OCRepPayloadSetUri(payload, NS_COLLECTION_MESSAGE_URI);
    (accepted) ? OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, NS_ALLOW)
        : OCRepPayloadSetPropInt(payload, NS_ATTRIBUTE_MESSAGE_ID, NS_DENY);
    OCRepPayloadSetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, NSGetProviderInfo()->providerId);

    NSCacheElement * element = NSProviderStorageRead(consumerSubList, id);

    if (element == NULL)
    {
        NS_LOG(ERROR, "element is NULL");
        return NS_ERROR;
    }

    NSCacheSubData * subData = (NSCacheSubData*) element->data;

    if (OCNotifyListOfObservers(rHandle, (OCObservationId*)&subData->messageObId, 1,
            payload, OC_LOW_QOS) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "fail to send Acceptance");
        OCRepPayloadDestroy(payload);
        return NS_ERROR;

    }

    OCRepPayloadDestroy(payload);
    NS_LOG(DEBUG, "NSSendResponse - OUT");
    return NS_OK;
}

NSResult NSSendConsumerSubResponse(OCEntityHandlerRequest * entityHandlerRequest)
{
    NS_LOG(DEBUG, "NSSendSubscriptionResponse - IN");

    if (!entityHandlerRequest)
    {
        NS_LOG(ERROR, "Invalid request pointer");
        return NS_ERROR;
    }

    char * copyReq = OICStrdup(entityHandlerRequest->query);
    char * id = NSGetValueFromQuery(copyReq, NS_QUERY_CONSUMER_ID);

    if (!id)
    {
        NSOICFree(copyReq);
        NSFreeOCEntityHandlerRequest(entityHandlerRequest);
        NS_LOG(ERROR, "Invalid ConsumerID");
        return NS_ERROR;
    }

    NSCacheUpdateSubScriptionState(consumerSubList, id, true);
    NSSendResponse(id, true);
    NSOICFree(copyReq);
    NSFreeOCEntityHandlerRequest(entityHandlerRequest);
    NS_LOG(DEBUG, "NSSendSubscriptionResponse - OUT");
    return NS_OK;
}

#ifdef WITH_MQ
void NSProviderMQSubscription(NSMQTopicAddress * topicAddr)
{
    char * serverUri = topicAddr->serverAddr;
    char * topicName = topicAddr->topicName;

    NS_LOG_V(DEBUG, "input Topic Name2 : %s", topicAddr->topicName);

    OCDevAddr * addr = NSChangeAddress(serverUri);
    OCCallbackData cbdata = { NULL, NULL, NULL };
    cbdata.cb = NSProviderGetMQResponseCB;
    cbdata.context = OICStrdup(topicName);
    cbdata.cd = NSOICFree;

    char requestUri[100] = "coap+tcp://";

    NS_LOG_V(DEBUG, "requestUri1 = %s", requestUri);
    OICStrcat(requestUri, strlen(requestUri)+strlen(serverUri)+1, serverUri);
    NS_LOG_V(DEBUG, "requestUri2 = %s", requestUri);
    OICStrcat(requestUri, strlen(requestUri)+ strlen("/oic/ps") + 1, "/oic/ps");
    NS_LOG_V(DEBUG, "requestUri3 = %s", requestUri);
    OCStackResult ret = OCDoResource(NULL, OC_REST_GET, requestUri, addr,
                                     NULL, CT_DEFAULT, OC_HIGH_QOS, &cbdata, NULL, 0);

    NSOCResultToSuccess(ret);

    NSOICFree(topicAddr->serverAddr);
    NSOICFree(topicAddr->topicName);
    NSOICFree(topicAddr);
}
#endif

void * NSSubScriptionSchedule(void *ptr)
{
    if (ptr == NULL)
    {
        NS_LOG(DEBUG, "Create NSSubScriptionSchedule");
    }

    while (NSIsRunning[SUBSCRIPTION_SCHEDULER])
    {
        sem_wait(&NSSemaphore[SUBSCRIPTION_SCHEDULER]);
        pthread_mutex_lock(&NSMutex[SUBSCRIPTION_SCHEDULER]);

        if (NSHeadMsg[SUBSCRIPTION_SCHEDULER] != NULL)
        {
            NSTask *node = NSHeadMsg[SUBSCRIPTION_SCHEDULER];
            NSHeadMsg[SUBSCRIPTION_SCHEDULER] = node->nextTask;

            switch (node->taskType)
            {
                case TASK_SEND_POLICY:
                    NS_LOG(DEBUG, "CASE TASK_SEND_POLICY : ");
                    NSSendAccessPolicyResponse((OCEntityHandlerRequest*) node->taskData);
                    break;

                case TASK_RECV_SUBSCRIPTION:
                    NS_LOG(DEBUG, "CASE TASK_RECV_SUBSCRIPTION : ");
                    NSHandleSubscription((OCEntityHandlerRequest*) node->taskData,
                            NS_RESOURCE_MESSAGE);
                    break;

                case TASK_RECV_UNSUBSCRIPTION:
                    NS_LOG(DEBUG, "CASE TASK_RECV_UNSUBSCRIPTION : ");
                    NSHandleUnsubscription((OCEntityHandlerRequest*) node->taskData);
                    break;

                case TASK_SEND_ALLOW:
                {
                    NS_LOG(DEBUG, "CASE TASK_SEND_ALLOW : ");
                    char * consumerId = (char *) node->taskData;

                    NSCacheUpdateSubScriptionState(consumerSubList, consumerId, true);
                    NSSendResponse(consumerId, true);
                    NSOICFree(consumerId);
                    break;
                }
                case TASK_SEND_DENY:
                {
                    NS_LOG(DEBUG, "CASE TASK_SEND_DENY : ");
                    char * consumerId = (char *) node->taskData;

                    NSCacheUpdateSubScriptionState(consumerSubList, consumerId, false);
                    NSSendResponse(consumerId, false);
                    NSOICFree(consumerId);

                    break;
                }
                case TASK_SYNC_SUBSCRIPTION:
                    NS_LOG(DEBUG, "CASE TASK_SYNC_SUBSCRIPTION : ");
                    NSHandleSubscription((OCEntityHandlerRequest*) node->taskData,
                            NS_RESOURCE_SYNC);
                    break;
#ifdef WITH_MQ
                case TASK_MQ_REQ_SUBSCRIBE:
                    NS_LOG(DEBUG, "CASE TASK_MQ_REQ_SUBSCRIBE : ");
                    NSProviderMQSubscription((NSMQTopicAddress*) node->taskData);
                    break;
#endif
                default:
                    break;

            }
            NSOICFree(node);
        }

        pthread_mutex_unlock(&NSMutex[SUBSCRIPTION_SCHEDULER]);

    }
    NS_LOG(INFO, "Destroy NSSubScriptionSchedule");
    return NULL;
}
