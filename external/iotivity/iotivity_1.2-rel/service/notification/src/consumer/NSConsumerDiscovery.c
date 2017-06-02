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

#include "NSConsumerDiscovery.h"

#include <string.h>
#include "NSCommon.h"
#include "NSUtil.h"
#include "NSConsumerCommon.h"
#include "NSConstants.h"
#include "ocpayload.h"
#include "oic_malloc.h"
#include "oic_string.h"

#define NS_DISCOVER_QUERY "/oic/res?rt=x.org.iotivity.notification"
#define NS_PRESENCE_SUBSCRIBE_QUERY_TCP "/oic/ad?rt=x.org.iotivity.notification"

OCStackApplicationResult NSConsumerPresenceListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) ctx;
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_NOT_NULL(clientResponse->payload, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(
            NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

    NS_LOG_V(INFO_PRIVATE, "Presence income : %s:%d",
            clientResponse->devAddr.addr, clientResponse->devAddr.port);
    NS_LOG_V(DEBUG, "Presence result : %d",
            clientResponse->result);
    NS_LOG_V(DEBUG, "Presence sequenceNum : %d",
            clientResponse->sequenceNumber);
    NS_LOG_V(DEBUG, "Presence Transport Type : %d",
                clientResponse->devAddr.adapter);

    if (!NSIsStartedConsumer())
    {
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCPresencePayload * payload = (OCPresencePayload *)clientResponse->payload;
    if (payload->trigger == OC_PRESENCE_TRIGGER_DELETE ||
            clientResponse->result == OC_STACK_PRESENCE_STOPPED)
    {
        NS_LOG(DEBUG, "stopped presence or resource is deleted.");
        NS_LOG(DEBUG, "build NSTask");
        OCDevAddr * addr = (OCDevAddr *)OICMalloc(sizeof(OCDevAddr));
        NS_VERIFY_NOT_NULL(addr, OC_STACK_KEEP_TRANSACTION);
        memcpy(addr, clientResponse->addr, sizeof(OCDevAddr));

        NSTask * task = NSMakeTask(TASK_CONSUMER_PROVIDER_DELETED, addr);
        NS_VERIFY_NOT_NULL(task, OC_STACK_KEEP_TRANSACTION);

        NSConsumerPushEvent(task);
    }

    else if (payload->trigger == OC_PRESENCE_TRIGGER_CREATE)
    {
        NS_LOG(DEBUG, "started presence or resource is created.");
        NSInvokeRequest(NULL, OC_REST_DISCOVER, clientResponse->addr,
            NS_DISCOVER_QUERY, NULL, NSProviderDiscoverListener, NULL, NULL,
            clientResponse->addr->adapter);
    }

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSProviderDiscoverListener(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_NOT_NULL(clientResponse->payload, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

    NS_LOG_V(INFO_PRIVATE, "Discover income : %s:%d",
            clientResponse->devAddr.addr, clientResponse->devAddr.port);
    NS_LOG_V(DEBUG, "Discover result : %d",
            clientResponse->result);
    NS_LOG_V(DEBUG, "Discover sequenceNum : %d",
            clientResponse->sequenceNumber);
    NS_LOG_V(DEBUG, "Discover Transport Type : %d",
                    clientResponse->devAddr.adapter);

    if (!NSIsStartedConsumer())
    {
        return OC_STACK_DELETE_TRANSACTION;
    }

    OCDiscoveryPayload * discoveryPayload = (OCDiscoveryPayload *)clientResponse->payload;

    while (discoveryPayload)
    {
        OCResourcePayload * resource = discoveryPayload->resources;
        while (resource)
        {
            NS_LOG_V(DEBUG, "Discovered resource uri : %s", resource->uri);
            NS_VERIFY_NOT_NULL(resource->uri, OC_STACK_KEEP_TRANSACTION);
            if (strstr(resource->uri, NS_RESOURCE_URI))
            {
                NS_LOG_V(DEBUG, "Request GET to provider : %s", resource->uri);
                OCConnectivityType type = CT_DEFAULT;
                if (clientResponse->addr->adapter == OC_ADAPTER_TCP)
                {
                    type = CT_ADAPTER_TCP;
                }

                OCDevAddr * addr = clientResponse->addr;
                if (resource->secure)
                {
                    addr->port = resource->port;
                    addr->flags |= OC_FLAG_SECURE;
                }

                NSInvokeRequest(NULL, OC_REST_GET, addr,
                        resource->uri, NULL, NSIntrospectProvider, ctx,
                        NULL, type);
            }
            resource = resource->next;
        }
        discoveryPayload = discoveryPayload->next;
    }

    return OC_STACK_KEEP_TRANSACTION;
}

OCStackApplicationResult NSIntrospectProvider(
        void * ctx, OCDoHandle handle, OCClientResponse * clientResponse)
{
    (void) handle;

    NS_VERIFY_NOT_NULL(clientResponse, OC_STACK_KEEP_TRANSACTION);
    NS_VERIFY_STACK_SUCCESS(NSOCResultToSuccess(clientResponse->result), OC_STACK_KEEP_TRANSACTION);

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

    if (!NSIsStartedConsumer())
    {
        return OC_STACK_DELETE_TRANSACTION;
    }

    NSProvider_internal * newProvider = NSGetProvider(clientResponse);
    NS_VERIFY_NOT_NULL(newProvider, OC_STACK_KEEP_TRANSACTION);
    if (ctx && ctx == (void *)NS_DISCOVER_CLOUD )
    {
        newProvider->connection->isCloudConnection = true;
    }

    NS_LOG(DEBUG, "build NSTask");
    NSTask * task = NSMakeTask(TASK_CONSUMER_PROVIDER_DISCOVERED, (void *) newProvider);
    NS_VERIFY_NOT_NULL_WITH_POST_CLEANING(task, NS_ERROR, NSRemoveProvider_internal(newProvider));

    NSConsumerPushEvent(task);

    return OC_STACK_KEEP_TRANSACTION;
}

void NSConsumerHandleRequestDiscover(OCDevAddr * address, NSConsumerDiscoverType rType)
{
    OCConnectivityType type = CT_ADAPTER_IP;
    NSConsumerDiscoverType * callbackData = NULL;

    if (address)
    {
        if (address->adapter == OC_ADAPTER_IP)
        {
            NS_LOG(DEBUG, "Request discover [UDP]");
        }
        else if (address->adapter == OC_ADAPTER_TCP)
        {
            type = CT_ADAPTER_TCP;
            NS_LOG(DEBUG, "Request discover and subscribe presence [TCP]");
            NS_LOG(DEBUG, "Subscribe presence [TCP]");
            NSInvokeRequest(NULL, OC_REST_PRESENCE, address, NS_PRESENCE_SUBSCRIBE_QUERY_TCP,
                    NULL, NSConsumerPresenceListener, NULL, NULL, type);

            if (rType == NS_DISCOVER_CLOUD)
            {
                callbackData = (void *) NS_DISCOVER_CLOUD;
            }
        }
        else
        {
            NS_LOG_V(INFO_PRIVATE, "Request discover But Adapter is not IP : %d", address->adapter);
        }
    }
    else
    {
        NS_LOG(DEBUG, "Request Multicast discover [UDP]");
    }

    NSInvokeRequest(NULL, OC_REST_DISCOVER, address, NS_DISCOVER_QUERY,
            NULL, NSProviderDiscoverListener, (void *)callbackData, NULL, type);
}

void NSConsumerDiscoveryTaskProcessing(NSTask * task)
{
    NS_VERIFY_NOT_NULL_V(task);

    NS_LOG_V(DEBUG, "Receive Event : %d", (int)task->taskType);
    if (task->taskType == TASK_CONSUMER_REQ_DISCOVER)
    {
        char * address = (char *) task->taskData;
        NSConsumerDiscoverType dType = NS_DISCOVER_DEFAULT;

        OCDevAddr * addr = NULL;
        if (address)
        {
            addr = NSChangeAddress(address);
            dType = NS_DISCOVER_CLOUD;
        }

        NSConsumerHandleRequestDiscover(addr, dType);
        NSOICFree(task->taskData);
        NSOICFree(addr);
    }
    else if (task->taskType == TASK_EVENT_CONNECTED || task->taskType == TASK_EVENT_CONNECTED_TCP)
    {
        NSConsumerHandleRequestDiscover((OCDevAddr *) task->taskData, NS_DISCOVER_DEFAULT);
        NSOICFree(task->taskData);
    }
    else
    {
        NS_LOG(ERROR, "Unknown type message");
    }

    NSOICFree(task);
}
