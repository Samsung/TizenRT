/*
 * //******************************************************************
 * //
 * // Copyright 2016 Samsung Electronics All Rights Reserved.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * //
 * // Licensed under the Apache License, Version 2.0 (the "License");
 * // you may not use this file except in compliance with the License.
 * // You may obtain a copy of the License at
 * //
 * //      http://www.apache.org/licenses/LICENSE-2.0
 * //
 * // Unless required by applicable law or agreed to in writing, software
 * // distributed under the License is distributed on an "AS IS" BASIS,
 * // WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * // See the License for the specific language governing permissions and
 * // limitations under the License.
 * //
 * //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "oic_string.h"
#include "srmresourcestrings.h"
#include "pmutility.h"

#include "cloudCommon.h"
#include "cloudWrapper.h"

#define TAG "cloudDiscovery"

#define DISCOVERY_URI "/oic/res"

static OCDevAddr endPoint;
static OCConnectivityType ocConnType = CT_DEFAULT;
static char gUri[MAX_URI_LENGTH] = {0};

bool withTcp = true;

/**
 * This function clears internal data
 */
static void clearData()
{
    memset(&endPoint, 0, sizeof(endPoint));
    memset(gUri, 0, sizeof(gUri));
    ocConnType = CT_DEFAULT;
}

/**
 * This function parses Discovery payload
 * It tries to find secure devices
 *
 * @param[in] clientResponse   response from peer
 */
static void parseClientResponse(OCClientResponse * clientResponse)
{
    OCResourcePayload* res = ((OCDiscoveryPayload*)clientResponse->payload)->resources;

    while (res)
    {
        char *uri = res->uri;
        OIC_LOG_V(INFO, TAG, "Uri -- %s", uri);

        if (0 == strcmp(uri, OIC_RSRC_DOXM_URI))
        {
            OIC_LOG(INFO,TAG,"Skip: doxm is secure virtual resource");
            goto next;
        }
        if (0 == strcmp(uri, OIC_RSRC_PSTAT_URI))
        {
            OIC_LOG(INFO,TAG,"Skip: pstat is secure virtual resource");
            goto next;
        }

        OIC_LOG_V(INFO, TAG, "Secure -- %s", res->secure ? "YES" : "NO");

        if (res->secure)
        {
            ocConnType = clientResponse->connType;
            endPoint = clientResponse->devAddr;

            if(withTcp)
            {
                OIC_LOG_V(INFO,TAG,"SECUREPORT tcp: %d",res->tcpPort);
                endPoint.port = res->tcpPort;
            }
            else
            {
                OIC_LOG_V(INFO,TAG,"SECUREPORT udp: %d",res->port);
                endPoint.port = res->port;
            }
            OICStrcpy(gUri, sizeof(gUri), uri);
            break;
        }

        next:
        res = res->next;
    }
}

/**
 * This function handles discovery response
 *
 * @param[in] ctx                    context
 * @param[in] handle                 handle
 * @param[in] response               response from peer
 * @return  OCStackApplicationResult application result
 */
static OCStackApplicationResult handleDiscoveryResponse(void *ctx, OCDoHandle handle,
                                                        OCClientResponse * clientResponse)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    if (NULL == clientResponse)
    {
        OIC_LOG(ERROR, TAG, "Receive NULL response");
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (OC_STACK_OK == clientResponse->result)
    {
        OIC_LOG(INFO, TAG, "Discovery successful!");
    }
    else
    {
        OIC_LOG_V(INFO, TAG, "Discovery fails with result: %d", clientResponse->result);
    }

    if (clientResponse->payload)
    {
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Receive NULL payload");
    }

    OIC_LOG_V(INFO, TAG,
            "Device =============> Discovered @ %s:%d",
            clientResponse->devAddr.addr,
            clientResponse->devAddr.port);

    parseClientResponse(clientResponse);

    return OC_STACK_KEEP_TRANSACTION;
}

/**
 * This function handles any response from secure device
 *
 * @param[in] ctx                    context
 * @param[in] handle                 handle
 * @param[in] response               response from peer
 * @return  OCStackApplicationResult application result
 */
static OCStackApplicationResult handleResponse(void *ctx, OCDoHandle handle,
                                               OCClientResponse * clientResponse)
{
    OC_UNUSED(ctx);
    OC_UNUSED(handle);

    if(!clientResponse)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Received NULL response",  __func__);
        return OC_STACK_DELETE_TRANSACTION;
    }

    if (NULL == ctx)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Received NULL context, exit",  __func__);
        return OC_STACK_DELETE_TRANSACTION;
    }

    char *str = NULL;
    OCMethod method = *((OCMethod*)ctx);

    switch (method)
    {
        case OC_REST_GET:
            str = "Get";
            break;
        case OC_REST_PUT:
            str = "Put";
            break;
        case OC_REST_POST:
            str = "Post";
            break;
        default:
            OIC_LOG_V(INFO, TAG, "Received Wrong method %d. Skip response", method);
            return OC_STACK_ERROR;
    }

    OIC_LOG_V(INFO, TAG, "StackResult: %d",  clientResponse->result);
    OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);
    OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
    OIC_LOG_V(INFO, TAG, "=============> %s Response", str);

    return OC_STACK_DELETE_TRANSACTION;
}

OCStackResult InitDiscovery()
{
    clearData();

    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cb = handleDiscoveryResponse;
    cbData.cd = unlockMenu;

    return OCDoResource(NULL, OC_REST_DISCOVER, DISCOVERY_URI, NULL, 0,
                        CT_DEFAULT, OC_LOW_QOS, &cbData, NULL, 0);
}

OCStackResult InitRequest(OCMethod method)
{
    if (0 == endPoint.port || 0 == endPoint.addr[0])
    {
        OIC_LOG(ERROR, TAG, "Host or port are missing. Please execute Discovery first!");
        return OC_STACK_ERROR;
    }

    OCMethod *tmp = OICCalloc(1, sizeof(OCMethod));
    if (!tmp)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate tmp");
        return OC_STACK_NO_MEMORY;
    }

    *tmp = method;

    OIC_LOG_V(INFO, TAG, "Executing uri = %s", gUri);
    OIC_LOG_V(INFO, TAG, "Send Request to %s:%d", endPoint.addr, endPoint.port);

    endPoint.flags = (OCTransportFlags)(endPoint.flags | OC_SECURE);
    endPoint.adapter = OC_ADAPTER_TCP;


    OCCallbackData cbData;
    memset(&cbData, 0, sizeof(OCCallbackData));
    cbData.cd = unlockMenu;
    cbData.cb = handleResponse;
    cbData.context = (void *)tmp;

    return OCDoResource(NULL, method, gUri, &endPoint, 0,
                        ocConnType, OC_LOW_QOS, &cbData, NULL, 0);
}
