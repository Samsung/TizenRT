//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#include "iotivity_config.h"
#include "occlientcb.h"
#include "logger.h"
#include "trace.h"
#include "oic_malloc.h"
#include <string.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_ARDUINO_TIME_H
#include "Time.h"
#endif

#include "cacommon.h"
#include "cainterface.h"
#include <coap/coap.h>

/// Module Name
#define TAG "OIC_RI_CLIENTCB"

struct ClientCB *cbList = NULL;

OCStackResult
AddClientCB (ClientCB** clientCB, OCCallbackData* cbData,
             CAToken_t token, uint8_t tokenLength,
             OCDoHandle *handle, OCMethod method,
             OCDevAddr *devAddr, char * requestUri,
             char * resourceTypeName, uint32_t ttl)
{
    if (!clientCB || !cbData || !handle || tokenLength > CA_MAX_TOKEN_LEN)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OIC_TRACE_BEGIN(%s:AddClientCB, TAG);

    ClientCB *cbNode = NULL;

#ifdef WITH_PRESENCE
    if (method == OC_REST_PRESENCE)
    {   // Retrieve the presence callback structure for this specific requestUri.
        cbNode = GetClientCB(NULL, 0, NULL, requestUri);
    }

    if (!cbNode)// If it does not already exist, create new node.
#endif // WITH_PRESENCE
    {
        cbNode = (ClientCB*) OICMalloc(sizeof(ClientCB));
        if (!cbNode)
        {
            *clientCB = NULL;
            goto exit;
        }
        else
        {
            OIC_LOG(INFO, TAG, "Adding client callback with token");
            OIC_LOG_BUFFER(INFO, TAG, (const uint8_t *)token, tokenLength);
            OIC_TRACE_BUFFER("OIC_RI_CLIENTCB:AddClientCB:token",
                             (const uint8_t *) token, tokenLength);
            cbNode->callBack = cbData->cb;
            cbNode->context = cbData->context;
            cbNode->deleteCallback = cbData->cd;
            //Note: token memory is allocated in the caller OCDoResource
            //but freed in DeleteClientCB
            cbNode->token = token;
            cbNode->tokenLength = tokenLength;
            cbNode->handle = *handle;
            cbNode->method = method;
            cbNode->sequenceNumber = 0;
#ifdef WITH_PRESENCE
            cbNode->presence = NULL;
            cbNode->filterResourceType = NULL;
#endif // WITH_PRESENCE

            if (method == OC_REST_PRESENCE ||
                method == OC_REST_OBSERVE  ||
                method == OC_REST_OBSERVE_ALL)
            {
                cbNode->TTL = 0;
            }
            else
            {
                cbNode->TTL = ttl;
            }
            cbNode->requestUri = requestUri;    // I own it now
            cbNode->devAddr = devAddr;          // I own it now
            OIC_LOG_V(INFO, TAG, "Added Callback for uri : %s", requestUri);
            OIC_TRACE_MARK(%s:AddClientCB:uri:%s, TAG, requestUri);
            LL_APPEND(cbList, cbNode);
            *clientCB = cbNode;
        }
    }
#ifdef WITH_PRESENCE
    else
    {
        // Ensure that the handle the SDK hands back up to the application layer for the
        // OCDoResource call matches the found ClientCB Node.
        *clientCB = cbNode;

        if (cbData->cd)
        {
            cbData->cd(cbData->context);
        }

        OICFree(token);
        OICFree(*handle);
        OICFree(requestUri);
        OICFree(devAddr);
        *handle = cbNode->handle;
    }

    if (method == OC_REST_PRESENCE && resourceTypeName)
    {
        OIC_TRACE_END();
        // Amend the found or created node by adding a new resourceType to it.
        return InsertResourceTypeFilter(cbNode,(char *)resourceTypeName);
        // I own resourceTypName now.
    }
    else
    {
        OICFree(resourceTypeName);
    }
#else
    OICFree(resourceTypeName);
#endif

    OIC_TRACE_END();
    return OC_STACK_OK;

exit:
    OIC_TRACE_END();
    return OC_STACK_NO_MEMORY;
}

void DeleteClientCB(ClientCB * cbNode)
{
    OIC_TRACE_BEGIN(%s:DeleteClientCB, TAG);
    if (cbNode)
    {
        LL_DELETE(cbList, cbNode);
        OIC_LOG (INFO, TAG, "Deleting token");
        OIC_LOG_BUFFER(INFO, TAG, (const uint8_t *)cbNode->token, cbNode->tokenLength);
        OIC_TRACE_BUFFER("OIC_RI_CLIENTCB:DeleteClientCB:token",
                         (const uint8_t *)cbNode->token, cbNode->tokenLength);
        CADestroyToken (cbNode->token);
        OICFree(cbNode->devAddr);
        OICFree(cbNode->handle);
        if (cbNode->requestUri)
        {
            OIC_LOG_V (INFO, TAG, "Deleting callback with uri %s", cbNode->requestUri);
            OIC_TRACE_MARK(%s:DeleteClientCB:uri:%s, TAG, cbNode->requestUri);
            OICFree(cbNode->requestUri);
        }
        if (cbNode->deleteCallback)
        {
            cbNode->deleteCallback(cbNode->context);
        }

#ifdef WITH_PRESENCE
        if (cbNode->presence)
        {
            OICFree(cbNode->presence->timeOut);
            OICFree(cbNode->presence);
        }
        if (cbNode->method == OC_REST_PRESENCE)
        {
            OCResourceType * pointer = cbNode->filterResourceType;
            OCResourceType * next = NULL;
            while(pointer)
            {
                next = pointer->next;
                OICFree(pointer->resourcetypename);
                OICFree(pointer);
                pointer = next;
            }
        }
#endif // WITH_PRESENCE
        OICFree(cbNode);
        cbNode = NULL;
    }
    OIC_TRACE_END();
}

/*
 * This function checks if the node is past its time to live and
 * deletes it if timed-out. Calling this function with a  presence or observe
 * callback with ttl set to 0 will not delete anything as presence nodes have
 * their own mechanisms for timeouts. A null argument will cause the function to
 * silently return.
 */
static void CheckAndDeleteTimedOutCB(ClientCB* cbNode)
{
    if (!cbNode)
    {
        return;
    }
    if (cbNode->TTL == 0)
    {
        return;
    }
    coap_tick_t now;
    coap_ticks(&now);

    if (cbNode->TTL < now)
    {
        OIC_LOG(INFO, TAG, "Deleting timed-out callback");
        DeleteClientCB(cbNode);
    }
}

ClientCB* GetClientCB(const CAToken_t token, uint8_t tokenLength,
                      OCDoHandle handle, const char * requestUri)
{
    ClientCB* out = NULL;

    if (token && tokenLength <= CA_MAX_TOKEN_LEN && tokenLength > 0)
    {
        OIC_LOG (INFO, TAG,  "Looking for token");
        OIC_LOG_BUFFER(INFO, TAG, (const uint8_t *)token, tokenLength);
        LL_FOREACH(cbList, out)
        {
            /* de-annotate below line if want to see all token in cbList */
            //OIC_LOG_BUFFER(INFO, TAG, (const uint8_t *)out->token, tokenLength);
            if (memcmp(out->token, token, tokenLength) == 0)
            {
                OIC_LOG(INFO, TAG, "Found in callback list");
                return out;
            }
            CheckAndDeleteTimedOutCB(out);
        }
    }
    else if (handle)
    {
        OIC_LOG (INFO, TAG,  "Looking for handle");
        LL_FOREACH(cbList, out)
        {
            if (out->handle == handle)
            {
                OIC_LOG(INFO, TAG, "Found in callback list");
                return out;
            }
            CheckAndDeleteTimedOutCB(out);
        }
    }
    else if (requestUri)
    {
        OIC_LOG_V(INFO, TAG, "Looking for uri %s", requestUri);
        LL_FOREACH(cbList, out)
        {
            /* de-annotate below line if want to see all uri in cbList */
            //OIC_LOG_V(INFO, TAG, "%s", out->requestUri);
            if (out->requestUri && strcmp(out->requestUri, requestUri ) == 0)
            {
                OIC_LOG(INFO, TAG, "Found in callback list");
                return out;
            }
            CheckAndDeleteTimedOutCB(out);
        }
    }
    OIC_LOG(INFO, TAG, "Callback Not found !!");
    return NULL;
}

#ifdef WITH_PRESENCE
OCStackResult InsertResourceTypeFilter(ClientCB * cbNode, char * resourceTypeName)
{
    OCResourceType * newResourceType = NULL;
    if (cbNode && resourceTypeName)
    {
        // Form a new resourceType member.
        newResourceType = (OCResourceType *) OICMalloc(sizeof(OCResourceType));
        if (!newResourceType)
        {
            return OC_STACK_NO_MEMORY;
        }

        newResourceType->next = NULL;
        newResourceType->resourcetypename = resourceTypeName;

        LL_APPEND(cbNode->filterResourceType, newResourceType);
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}
#endif // WITH_PRESENCE

void DeleteClientCBList()
{
    ClientCB* out;
    ClientCB* tmp;
    LL_FOREACH_SAFE(cbList, out, tmp)
    {
        DeleteClientCB(out);
    }
    cbList = NULL;
}

void FindAndDeleteClientCB(ClientCB * cbNode)
{
    ClientCB* tmp;
    if (cbNode)
    {
        LL_FOREACH(cbList, tmp)
        {
            if (cbNode == tmp)
            {
                DeleteClientCB(tmp);
                break;
            }
        }
    }
}
