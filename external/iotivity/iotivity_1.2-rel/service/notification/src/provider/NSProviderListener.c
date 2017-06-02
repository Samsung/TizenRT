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

#include "NSProviderListener.h"

bool NSProviderIsSyncAttributes(OCRepPayload * payload);
bool NSProviderIsTopicAttributes(OCRepPayload * payload);
OCStackResult NSProviderSendResponse(OCEntityHandlerRequest * entityHandlerRequest,
        OCRepPayload * payload, char * reqInterface, OCEntityHandlerResult ehResult,
        NSInterfaceType interfaceType, NSResourceType resourceType);

OCEntityHandlerResult NSEntityHandlerNotificationCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
{
    NS_LOG(DEBUG, "NSEntityHandlerNotificationCb - IN");

    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    (void)callback;

    if (!entityHandlerRequest)
    {
        NS_LOG(ERROR, "Invalid request pointer");
        return ehResult;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        NS_LOG(DEBUG, "Flag includes OC_REQUEST_FLAG");

        if (OC_REST_GET == entityHandlerRequest->method)
        {
            NS_LOG(DEBUG, "NSEntityHandlerNotificationCb - OC_REST_GET");

            char * copyQuery = OICStrdup(entityHandlerRequest->query);
            char * reqInterface = NSGetValueFromQuery(copyQuery, NS_QUERY_INTERFACE);

            if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) != 0
                    && strcmp(reqInterface, NS_INTERFACE_READ) != 0)
            {
                NS_LOG(ERROR, "Invalid interface");
                NSOICFree(copyQuery);
                return ehResult;
            }

            NSOICFree(copyQuery);
            NSPushQueue(SUBSCRIPTION_SCHEDULER, TASK_SEND_POLICY,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));

            ehResult = OC_EH_OK;
        }
        else
        {
            NS_LOG_V (DEBUG, "Received method %d from client", entityHandlerRequest->method);
        }
    }

    NS_LOG(DEBUG, "NSEntityHandlerNotificationCb - OUT");
    return ehResult;
}

OCEntityHandlerResult NSEntityHandlerMessageCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
{
    NS_LOG(DEBUG, "NSEntityHandlerMessageCb - IN");

    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    char * reqInterface = NULL;
    OCRepPayload * payload = NULL;

    (void)callback;

    if (!entityHandlerRequest)
    {
        NS_LOG (ERROR,"Invalid request pointer");
        return ehResult;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        NS_LOG(DEBUG, "Flag includes OC_REQUEST_FLAG");
        NS_LOG_V(DEBUG, "Received method %d from client", entityHandlerRequest->method);

        if (OC_REST_GET == entityHandlerRequest->method)
        {
            NS_LOG(DEBUG, "NSEntityHandlerMessageCb - OC_REST_GET");

            char * copyQuery = OICStrdup(entityHandlerRequest->query);
            reqInterface = OICStrdup(NSGetValueFromQuery(copyQuery, NS_QUERY_INTERFACE));
            NSOICFree(copyQuery);

            if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) != 0
                    && strcmp(reqInterface, NS_INTERFACE_READ) != 0)
            {
                NS_LOG(ERROR, "Invalid interface");
                return ehResult;
            }
            ehResult = OC_EH_OK;
        }
        else
        {
            NS_LOG_V (DEBUG, "Received method %d from client", entityHandlerRequest->method);
        }
    }

    if (flag & OC_OBSERVE_FLAG)
    {
        NS_LOG(DEBUG, "NSEntityHandlerMessageCb - OC_OBSERVE_FLAG");

        OCObserveAction ocObAction = entityHandlerRequest->obsInfo.action;

        if (ocObAction == OC_OBSERVE_REGISTER)
        {
            NS_LOG(DEBUG, "NSEntityHandlerMessageCb - OC_OBSERVE_REGISTER");
            NS_LOG_V(DEBUG, "NSEntityHandlerMessageCb\n"
                    "Register message observerID : %d\n", entityHandlerRequest->obsInfo.obsId);

            NSPushQueue(SUBSCRIPTION_SCHEDULER, TASK_RECV_SUBSCRIPTION,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));
            ehResult = OC_EH_OK;
        }
        else if (ocObAction == OC_OBSERVE_DEREGISTER)
        {
            NS_LOG(DEBUG, "NSEntityHandlerMessageCb - OC_OBSERVE_DEREGISTER");
            NS_LOG_V(DEBUG, "NSEntityHandlerMessageCb\n - "
                    "Deregister Message observerID : %d\n", entityHandlerRequest->obsInfo.obsId);
            NSPushQueue(SUBSCRIPTION_SCHEDULER, TASK_RECV_UNSUBSCRIPTION,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));
            ehResult = OC_EH_OK;
        }
    }

    ehResult = NSProviderSendResponse(entityHandlerRequest, payload, reqInterface, ehResult,
            NS_INTERFACE_TYPE_READ, NS_RESOURCE_MESSAGE);
    NSOICFree(reqInterface);
    NS_LOG(DEBUG, "NSEntityHandlerMessageCb - OUT");
    return ehResult;
}

OCEntityHandlerResult NSEntityHandlerSyncCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
{
    NS_LOG(DEBUG, "NSEntityHandlerSyncCb - IN");
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    char * reqInterface = NULL;
    OCRepPayload * payload = NULL;

    (void)callback;

    if (!entityHandlerRequest)
    {
        NS_LOG(ERROR, "Invalid request pointer");
        return ehResult;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        NS_LOG(DEBUG, "Flag includes OC_REQUEST_FLAG");

        if (OC_REST_GET == entityHandlerRequest->method)
        {

            char * copyQuery = OICStrdup(entityHandlerRequest->query);
            reqInterface = OICStrdup(NSGetValueFromQuery(copyQuery, NS_QUERY_INTERFACE));
            NSOICFree(copyQuery);

            if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) != 0
                    && strcmp(reqInterface, NS_INTERFACE_READWRITE) != 0)
            {
                NS_LOG(ERROR, "Invalid interface");
                return ehResult;
            }

            ehResult = OC_EH_OK;
        }
        else if (OC_REST_POST == entityHandlerRequest->method)
        {
            /** Receive sync data from consumer which read or dismiss notification message.
                And broadcast the sync data to all subscribers including provider app
                to synchronize the notification message status. */

            NS_LOG(DEBUG, "NSEntityHandlerSyncCb - OC_REST_POST");

            if (NSProviderIsSyncAttributes((OCRepPayload *)entityHandlerRequest->payload))
            {
                NSPushQueue(NOTIFICATION_SCHEDULER, TASK_RECV_READ,
                                    NSGetSyncInfo(entityHandlerRequest->payload));
                ehResult = OC_EH_OK;
            }
        }
        else
        {
            NS_LOG_V(DEBUG, "Received method %d from client", entityHandlerRequest->method);
        }
    }

    if (flag & OC_OBSERVE_FLAG)
    {
        /** Requested by consumers to synchronize notification message status.
            Store the observer IDs to storage or cache */

        NS_LOG(DEBUG, "NSEntityHandlerSyncCb - OC_OBSERVE_FLAG");

        OCObserveAction ocObAction = entityHandlerRequest->obsInfo.action;

        if (ocObAction == OC_OBSERVE_REGISTER)
        {
            NS_LOG(DEBUG, "NSEntityHandlerSyncCb - OC_OBSERVE_REGISTER");
            NS_LOG_V(DEBUG, "NSEntityHandlerSyncCb\n - "
                    "Register Sync observerID : %d\n", entityHandlerRequest->obsInfo.obsId);
            NSPushQueue(SUBSCRIPTION_SCHEDULER, TASK_SYNC_SUBSCRIPTION,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));
        }
        else if (ocObAction == OC_OBSERVE_DEREGISTER)
        {
            NS_LOG(DEBUG, "NSEntityHandlerSyncCb - OC_OBSERVE_DEREGISTER");
            NS_LOG_V(DEBUG, "NSEntityHandlerSyncCb\n - "
                    "Deregister Sync observerID : %d\n", entityHandlerRequest->obsInfo.obsId);
            NSPushQueue(SUBSCRIPTION_SCHEDULER, TASK_RECV_UNSUBSCRIPTION,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));
        }
    }

    NS_LOG(DEBUG, "NSEntityHandlerSyncCb - OUT");

    ehResult = NSProviderSendResponse(entityHandlerRequest, payload, reqInterface, ehResult,
            NS_INTERFACE_TYPE_READ, NS_RESOURCE_MESSAGE);
    NSOICFree(reqInterface);

    return ehResult;
}

OCEntityHandlerResult NSEntityHandlerTopicCb(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *entityHandlerRequest, void* callback)
{
    NS_LOG(DEBUG, "NSEntityHandlerTopicCb - IN");
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    char * reqInterface = NULL;
    OCRepPayload * payload = NULL;

    (void)callback;

    if (!entityHandlerRequest)
    {
        NS_LOG(ERROR, "Invalid request pointer");
        return ehResult;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        NS_LOG(DEBUG, "Flag includes OC_REQUEST_FLAG");

        if (OC_REST_GET == entityHandlerRequest->method)
        {
            NS_LOG(DEBUG, "NSEntityHandlerTopicCb - OC_REST_GET");

            char * copyReq = OICStrdup(entityHandlerRequest->query);
            reqInterface = OICStrdup(NSGetValueFromQuery(copyReq, NS_QUERY_INTERFACE));
            NSOICFree(copyReq);

            if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) != 0
                    && strcmp(reqInterface, NS_INTERFACE_READWRITE) != 0)
            {
                NS_LOG(ERROR, "Invalid interface");
                return ehResult;
            }
            // send consumer's interesting topic list if consumer id exists
            // otherwise send  registered topic list
            NSPushQueue(TOPIC_SCHEDULER, TASK_SEND_TOPICS,
                    NSCopyOCEntityHandlerRequest(entityHandlerRequest));

            ehResult = OC_EH_OK;
        }
        else if (OC_REST_POST == entityHandlerRequest->method)
        {
            // Receive interesting topic list from consumers
            // Send topic notice message(id = TOPIC) to the consumer
            // which requests to post.
            NS_LOG(DEBUG, "NSEntityHandlerTopicCb - OC_REST_POST");
            // Accepter is provider. our service is not support sendtopiclist from OC_REST_POST
            // Accepter is consumer. our service is support sendtopiclist from OC_REST_POST
            if (NSGetPolicy() == false &&
                    NSProviderIsTopicAttributes(OCRepPayloadClone((OCRepPayload *)
                            entityHandlerRequest->payload)))
            {
                NSPushQueue(TOPIC_SCHEDULER, TASK_POST_TOPIC,
                        NSCopyOCEntityHandlerRequest(entityHandlerRequest));
                ehResult = OC_EH_OK;
            }
        }
        else
        {
            NS_LOG_V(DEBUG, "Received unsupported method %d from client",
                    entityHandlerRequest->method);
            ehResult = OC_EH_ERROR;
        }
    }

    NS_LOG(DEBUG, "NSEntityHandlerTopicCb - OUT");
    ehResult = NSProviderSendResponse(entityHandlerRequest, payload, reqInterface, ehResult,
            NS_INTERFACE_TYPE_READWRITE, NS_RESOURCE_TOPIC);
    NSOICFree(reqInterface);
    return ehResult;
}

#ifdef WITH_MQ
OCStackApplicationResult NSProviderMQListener(void * ctx, OCDoHandle handle,
        OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    NS_LOG_V(DEBUG, "clientResponse->sequenceNumber = %d", clientResponse->sequenceNumber);

    if (clientResponse->sequenceNumber == OC_OBSERVE_REGISTER)
    {
        NS_LOG(DEBUG, "MQ OC_OBSERVE_RIGSTER");
        NSSetMQServerInfo(clientResponse->resourceUri, &(clientResponse->devAddr));
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

    OCRepPayload * payload = (OCRepPayload *)clientResponse->payload;
    NS_VERIFY_NOT_NULL(payload, OC_STACK_KEEP_TRANSACTION);

    NSMessageType type = -1;
    bool getResult = OCRepPayloadGetPropInt(payload, NS_ATTRIBUTE_TYPE, (int64_t *) &type);
    NS_LOG_V (DEBUG, "message sync type : %d", (int) type);

    if (!getResult && (type == NS_MESSAGE_READ || type == NS_MESSAGE_DELETED))
    {
        char * pId = NULL;
        getResult = OCRepPayloadGetPropString(payload, NS_ATTRIBUTE_PROVIDER_ID, &pId);
        NS_LOG_V (INFO_PRIVATE, "provider id: %s", pId);

        if (getResult && strcmp(pId, NSGetProviderInfo()->providerId) == 0)
        {
            NSSyncInfo * syncInfo = (NSSyncInfo *)OICMalloc(sizeof(NSSyncInfo));
            syncInfo->state = (type == NS_MESSAGE_READ) ? NS_SYNC_READ : NS_SYNC_DELETED;
            OICStrcpy(syncInfo->providerId, NS_UUID_STRING_SIZE, pId);
            NSOICFree(pId);
            NSPushQueue(NOTIFICATION_SCHEDULER, TASK_RECV_READ, (void*) syncInfo);
        }
    }

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSProviderGetMQResponseCB(void * ctx, OCDoHandle handle,
        OCClientResponse * clientResponse)
{
    NS_LOG(DEBUG, "NSProviderGetMQResponseCB - IN");

    (void) handle;

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

    NS_LOG_V(DEBUG, "interestTopicName = %s", interestTopicName);
    for (size_t i = 0; i < dimensions[0]; ++i)
    {
        NS_LOG_V(DEBUG, "found MQ topic : %s", topicList[i]);
        if (!strcmp(topicList[i], interestTopicName))
        {
            NS_LOG(DEBUG, "subscribe to MQ notification");

            OCCallbackData cbdata = { NULL, NULL, NULL };
            cbdata.cb = NSProviderMQListener;
            cbdata.context = NULL;
            cbdata.cd = NULL;

            OCStackResult ret = OCDoResource(NULL, OC_REST_OBSERVE, topicList[i],
                    clientResponse->addr, NULL, CT_DEFAULT, OC_HIGH_QOS, &cbdata, NULL, 0);

            if (!NSOCResultToSuccess(ret))
            {
                NS_LOG(DEBUG, "fail to subscribe to MQ notification");
                continue;
            }
        }
    }

    NS_LOG(DEBUG, "NSProviderGetMQResponseCB - OUT");
    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSProviderPublishMQResponseCB(void *ctx, OCDoHandle handle,
        OCClientResponse *clientResponse)
{
    (void) ctx;
    (void) handle;
    NS_LOG(DEBUG, "Publish Topic callback received");

    OCStackApplicationResult res = OC_STACK_ERROR;

    NS_LOG_V(DEBUG, "Publish Topic response received code: (%d)", clientResponse->result);

    if (clientResponse->payload != NULL &&
        clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION)
    {
        NS_LOG(DEBUG, "PAYLOAD_TYPE_REPRESENTATION received");

        OCRepPayloadValue *val = ((OCRepPayload *)clientResponse->payload)->values;
        while (val)
        {
            if( val->type == OCREP_PROP_INT)
            {
                NS_LOG_V(DEBUG, "Key: %s, Value: %lld, int", val->name, val->i);
            }
            else if( val->type == OCREP_PROP_STRING)
            {
                NS_LOG_V(DEBUG, "Key: %s, Value: %s, string", val->name, val->str);
            }
            else
            {
                NS_LOG_V(DEBUG, "Un supported val Type.(0x%d)", val->type);
            }

            val = val->next;
        }

        res = OC_STACK_KEEP_TRANSACTION;
    }

    return res;
}
#endif

void NSProviderConnectionStateListener(const CAEndpoint_t * info, bool connected)
{
    NS_LOG(DEBUG, "NSProviderConnectionStateListener - IN");

    if (connected)
    {
        NS_LOG(DEBUG, "CONNECTED");

        // Set Connection State
        NSSetProviderConnectionState(CONNECTED);

        // Start Presence
        NSPushQueue(DISCOVERY_SCHEDULER, TASK_START_PRESENCE, NULL);

        if (info->adapter == CA_ADAPTER_TCP)
        {
            NS_LOG_V(INFO_PRIVATE, "TCP Connected remote address: %s:%d", info->addr, info->port);
        }
    }
    else
    {
        NS_LOG(DEBUG, "DISCONNECTED");

        // Set Connection State
        NSSetProviderConnectionState(DISCONNECTED);

        if (info->adapter == CA_ADAPTER_TCP)
        {
            NS_LOG_V(INFO_PRIVATE, "TCP Disconnected remote address: %s:%d", info->addr, info->port);
        }
    }

    NS_LOG(DEBUG, "NSProviderConnectionStateListener - OUT");
}

void NSProviderAdapterStateListener(CATransportAdapter_t adapter, bool enabled)
{
    (void)adapter;

    NS_LOG(DEBUG, "NSProviderAdapterStateListener - IN");

    if (enabled)
    {
        NS_LOG(DEBUG, "CONNECTED");

        // Set Connection State
        NSSetProviderConnectionState(CONNECTED);

        // Start Presence
        NSPushQueue(DISCOVERY_SCHEDULER, TASK_START_PRESENCE, NULL);
    }
    else
    {

        NS_LOG(DEBUG, "DISCONNECTED");

        // Set Connection State
        NSSetProviderConnectionState(DISCONNECTED);
    }

    NS_LOG(DEBUG, "NSProviderAdapterStateListener - OUT");
}

bool NSProviderCompareSyncAttributes(const char * name)
{
    if (!strcmp(name, NS_ATTRIBUTE_MESSAGE_ID) ||
        !strcmp(name, NS_ATTRIBUTE_PROVIDER_ID) ||
        !strcmp(name, NS_ATTRIBUTE_STATE))
    {
        return true;
    }

    return false;
}

bool NSProviderIsSyncAttributes(OCRepPayload * payload)
{
    NS_LOG(DEBUG, "get extra info");
    OCRepPayloadValue * curr = payload->values;

    while (curr)
    {
        if (!NSProviderCompareSyncAttributes(curr->name))
        {
            return false;
        }

        curr = curr->next;
    }

    return true;
}

bool NSProviderCompareTopicAttributes(const char * name)
{
    if (!strcmp(name, NS_ATTRIBUTE_TOPIC_LIST) ||
        !strcmp(name, NS_ATTRIBUTE_CONSUMER_ID))
    {
        return true;
    }

    return false;
}

bool NSProviderCompareSubTopicAttributes(const char * name)
{
    if (!strcmp(name, NS_ATTRIBUTE_TOPIC_NAME) ||
        !strcmp(name, NS_ATTRIBUTE_TOPIC_SELECTION))
    {
        return true;
    }

    return false;
}

bool NSProviderIsTopicAttributes(OCRepPayload * payload)
{
    NS_LOG(DEBUG, "get extra info");
    OCRepPayloadValue * curr = payload->values;

    while (curr)
    {
        if (!NSProviderCompareTopicAttributes(curr->name))
        {
            OCRepPayloadDestroy(payload);
            return false;
        }

        if (!strcmp(curr->name, NS_ATTRIBUTE_TOPIC_LIST))
        {
            OCRepPayload ** topicListPayload = NULL;
            OCRepPayloadValue * payloadValue = NULL;
            payloadValue = NSPayloadFindValue(payload, NS_ATTRIBUTE_TOPIC_LIST);
            size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
            size_t dimensions[3] = { dimensionSize, 0, 0 };

            if (!dimensionSize)
            {
                OCRepPayloadDestroy(payload);
                return false;
            }

            OCRepPayloadGetPropObjectArray(payload, NS_ATTRIBUTE_TOPIC_LIST, &topicListPayload,
                    dimensions);

            for (int i = 0; i < (int) dimensionSize; i++)
            {
                OCRepPayloadValue * subCurr = topicListPayload[i]->values;

                while (subCurr)
                {
                    if (!NSProviderCompareSubTopicAttributes(subCurr->name))
                    {
                        for(int j = i; j < (int) dimensionSize; ++j)
                        {
                            OCRepPayloadDestroy(topicListPayload[j]);
                        }

                        NSOICFree(topicListPayload);
                        OCRepPayloadDestroy(payload);
                        return false;
                    }
                    subCurr = subCurr->next;
                }
                OCRepPayloadDestroy(topicListPayload[i]);
            }
            NSOICFree(topicListPayload);
        }
        curr = curr->next;
    }

    OCRepPayloadDestroy(payload);
    return true;
}

OCStackResult NSProviderSendResponse(OCEntityHandlerRequest * entityHandlerRequest,
        OCRepPayload * payload, char * reqInterface, OCEntityHandlerResult ehResult,
        NSInterfaceType interfaceType, NSResourceType resourceType)
{
    if (reqInterface && strcmp(reqInterface, NS_INTERFACE_BASELINE) == 0)
    {
        payload = OCRepPayloadCreate();

        if (!payload)
        {
            NS_LOG(ERROR, "payload is NULL");
            return ehResult;
        }

        OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_BASELINE);
        interfaceType == NS_INTERFACE_TYPE_READ ?
                OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_READ)
                : OCResourcePayloadAddStringLL(&payload->interfaces, NS_INTERFACE_READWRITE);

        char * rtStr = NULL;
        switch (resourceType)
        {
            case NS_RESOURCE_MESSAGE:
                rtStr = NS_COLLECTION_MESSAGE_TYPE;
                break;
            case NS_RESOURCE_SYNC:
                rtStr = NS_COLLECTION_SYNC_TYPE;
                break;
            case NS_RESOURCE_TOPIC:
                rtStr = NS_COLLECTION_TOPIC_TYPE;
                break;
            default:
                NS_LOG(ERROR, "sendResponseError");
                return ehResult;
        }

        OCResourcePayloadAddStringLL(&payload->types, rtStr);
    }

    if (resourceType == NS_RESOURCE_TOPIC && entityHandlerRequest->method == OC_REST_GET)
    {
        OCRepPayloadDestroy(payload);
        return ehResult;
    }

    OCEntityHandlerResponse response;
    response.numSendVendorSpecificHeaderOptions = 0;
    memset(response.sendVendorSpecificHeaderOptions, 0,
            sizeof response.sendVendorSpecificHeaderOptions);
    memset(response.resourceUri, 0, sizeof response.resourceUri);

    response.requestHandle = entityHandlerRequest->requestHandle;
    response.resourceHandle = entityHandlerRequest->resource;
    response.persistentBufferFlag = 0;
    response.ehResult = ehResult;
    response.payload = (OCPayload *) payload;

    if (OCDoResponse(&response) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to AccessPolicy send response");
        ehResult = OC_STACK_ERROR;
    }

    OCRepPayloadDestroy(payload);
    return ehResult;
}
