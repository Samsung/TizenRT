//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include "rd_server.h"

#include "rd_database.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
#ifdef HAVE_IN6ADDR_H
#include <in6addr.h>
#endif
#include "payload_logging.h"
#include "ocpayload.h"
#include "octypes.h"
#include "oic_string.h"
#include "cainterface.h"

#define TAG PCF("OIC_RD_SERVER")

#ifdef RD_SERVER

// This is temporary hardcoded value for bias factor.
static const int OC_RD_DISC_SEL = 100;

static OCResourceHandle rdHandle;

static OCStackResult sendResponse(const OCEntityHandlerRequest *ehRequest, OCRepPayload *rdPayload,
    OCEntityHandlerResult ehResult)
{
    OCEntityHandlerResponse response = { 0 };
    response.requestHandle = ehRequest->requestHandle;
    response.resourceHandle = ehRequest->resource;
    response.ehResult = ehResult;
    response.payload = (OCPayload*)(rdPayload);
    return OCDoResponse(&response);
}

/**
 * This internal method handles RD discovery request.
 * Responds with the RD discovery payload message.
 */
static OCEntityHandlerResult handleGetRequest(const OCEntityHandlerRequest *ehRequest)
{
    if (!ehRequest)
    {
        OIC_LOG(DEBUG, TAG, "Invalid request pointer.");
        return OC_EH_ERROR;
    }

    OCEntityHandlerResult ehResult = OC_EH_OK;
    OIC_LOG_V(DEBUG, TAG, "Received OC_REST_GET from client with query: %s.", ehRequest->query);

    OCRepPayload *rdPayload =  (OCRepPayload *)OCRepPayloadCreate();
    if (!rdPayload)
    {
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropInt(rdPayload, OC_RSRVD_RD_DISCOVERY_SEL, OC_RD_DISC_SEL);
    OCRepPayloadAddResourceType(rdPayload, OC_RSRVD_RESOURCE_TYPE_RD);
    OCRepPayloadAddInterface(rdPayload, OC_RSRVD_INTERFACE_DEFAULT);
    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *) rdPayload);

    if (OC_STACK_OK != sendResponse(ehRequest, rdPayload, OC_EH_OK))
    {
        OIC_LOG(ERROR, TAG, "Sending response failed.");
        ehResult = OC_EH_ERROR;
    }

    return ehResult;
}

static bool isRequestFromThisHost(const OCEntityHandlerRequest *ehRequest)
{
    /* ehRequest->devAddr.addr includes zone ID which inet_pton doesn't like */
    struct in6_addr addr6;
    if (ehRequest->devAddr.flags & OC_IP_USE_V6)
    {
        char addr[MAX_ADDR_STR_SIZE];
        const char *src = ehRequest->devAddr.addr;
        char *dst = addr;
        while (*src && *src != '%')
        {
            *dst++ = *src++;
        }
        *dst = '\0';
        inet_pton(AF_INET6, addr, &addr6);
    }

    bool fromThisHost = false;
    CAEndpoint_t *caEps = NULL;
    size_t nCaEps = 0;
    if (CA_STATUS_OK == CAGetNetworkInformation(&caEps, &nCaEps))
    {
        for (size_t i = 0; i < nCaEps; ++i)
        {
            if (caEps[i].flags & ehRequest->devAddr.flags & OC_IP_USE_V6)
            {
                struct in6_addr ca6;
                if ((1 == inet_pton(AF_INET6, caEps[i].addr, &ca6)) &&
                        !memcmp(&ca6, &addr6, sizeof(struct in6_addr)))
                {
                    fromThisHost = true;
                    break;
                }
            }
            else if (!strcmp(caEps[i].addr, ehRequest->devAddr.addr))
            {
                fromThisHost = true;
                break;
            }
        }
        if (caEps)
        {
            OICFree(caEps);
        }
    }
    return fromThisHost;
}

/**
 * This internal method handles RD publish request.
 * Responds with the RD success message.
 */
static OCEntityHandlerResult handlePublishRequest(const OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehResult;

    if (!ehRequest)
    {
        OIC_LOG(DEBUG, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }

    OIC_LOG_V(DEBUG, TAG, "Received OC_REST_POST from client with query: %s.", ehRequest->query);

    OCRepPayload *payload = (OCRepPayload *)ehRequest->payload;
    OCRepPayload *resPayload = NULL;
    OCStackResult result;
    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *) payload);
    result = OCRDDatabaseInit();
    if (OC_STACK_OK == result)
    {
        if (isRequestFromThisHost(ehRequest))
        {
            result = OCRDDatabaseStoreResourcesFromThisHost(payload);
        }
        else
        {
            result = OCRDDatabaseStoreResources(payload);
        }
    }
    if (OC_STACK_OK == result)
    {
        OIC_LOG_V(DEBUG, TAG, "Stored resources.");
        resPayload = payload;
        ehResult = OC_EH_OK;
    }
    else
    {
        resPayload = (OCRepPayload *)OCRepPayloadCreate();
        ehResult = OC_EH_ERROR;
    }

    // Send Response
    if (OC_EH_OK == ehResult)
    {
        if (OC_STACK_OK != sendResponse(ehRequest, resPayload, ehResult))
        {
            OIC_LOG(ERROR, TAG, "Sending response failed.");
            ehResult = OC_EH_ERROR;
        }
    }

    if (OC_EH_OK == ehResult)
    {
        OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_WELL_KNOWN_URI);
        assert(handle);
        result = OCNotifyAllObservers(handle, OC_NA_QOS);
        if (OC_STACK_NO_OBSERVERS != result && OC_STACK_OK != result)
        {
            OIC_LOG(ERROR, TAG, "Notifying observers failed.");
        }
    }

    return ehResult;
}

static OCEntityHandlerResult handleDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    char *key = NULL;
    char *value = NULL;
    char *queryDup = NULL;
    char *restOfQuery = NULL;
    char *keyValuePair = NULL;
    char *di = NULL;
    uint16_t nIns = 0;
    int64_t *ins = NULL;

    if (!ehRequest)
    {
        OIC_LOG(DEBUG, TAG, "Invalid request pointer");
        return OC_EH_ERROR;
    }

    OIC_LOG_V(DEBUG, TAG, "Received OC_REST_DELETE from client with query: %s.", ehRequest->query);

    if (OC_STACK_OK != OCRDDatabaseInit())
    {
        goto exit;
    }

#define OC_RSRVD_INS_KEY OC_RSRVD_INS OC_KEY_VALUE_DELIMITER /* "ins=" */
    keyValuePair = strstr(ehRequest->query, OC_RSRVD_INS_KEY);
    while (keyValuePair)
    {
        ++nIns;
        keyValuePair = strstr(keyValuePair + sizeof(OC_RSRVD_INS_KEY), OC_RSRVD_INS_KEY);
    }
    if (nIns)
    {
        ins = OICMalloc(nIns * sizeof(*ins));
        if (!ins)
        {
            OIC_LOG_V(ERROR, TAG, "ins is NULL");
            goto exit;
        }
    }

    nIns = 0;
    queryDup = OICStrdup(ehRequest->query);
    if (NULL == queryDup)
    {
        OIC_LOG_V(ERROR, TAG, "Creating duplicate string failed!");
        goto exit;
    }
    keyValuePair = strtok_r(queryDup, OC_QUERY_SEPARATOR, &restOfQuery);
    while (keyValuePair)
    {
        key = strtok_r(keyValuePair, OC_KEY_VALUE_DELIMITER, &value);
        if (!key || !value)
        {
            OIC_LOG_V(ERROR, TAG, "Invalid query parameter!");
            ehResult = OC_EH_BAD_REQ;
            goto exit;
        }
        else if (0 == strncasecmp(key, OC_RSRVD_DEVICE_ID, sizeof(OC_RSRVD_DEVICE_ID) - 1))
        {
            di = value;
        }
        else if (0 == strncasecmp(key, OC_RSRVD_INS, sizeof(OC_RSRVD_INS) - 1))
        {
            // Arduino's AVR-GCC doesn't support strtoll().
            int64_t i;
            int matchedItems = sscanf(value, "%lld", &i);
            if (0 == matchedItems)
            {
                OIC_LOG_V(ERROR, TAG, "Invalid ins query parameter: %s", value);
                ehResult = OC_EH_BAD_REQ;
                goto exit;
            }

            ins[nIns++] = i;
        }

        keyValuePair = strtok_r(NULL, OC_QUERY_SEPARATOR, &restOfQuery);
    }
    if (!di && !nIns)
    {
        OIC_LOG_V(ERROR, TAG, "Missing required query parameters!");
        ehResult = OC_EH_BAD_REQ;
        goto exit;
    }

    if (OC_STACK_OK == OCRDDatabaseDeleteResources(di, ins, nIns))
    {
        OIC_LOG_V(DEBUG, TAG, "Deleted resource(s).");
        ehResult = OC_EH_OK;
    }

    if (OC_EH_OK == ehResult)
    {
        if (OC_STACK_OK != sendResponse(ehRequest, NULL, ehResult))
        {
            OIC_LOG(ERROR, TAG, "Sending response failed.");
            ehResult = OC_EH_ERROR;
        }
    }

    if (OC_EH_OK == ehResult)
    {
        OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_WELL_KNOWN_URI);
        assert(handle);
        OCStackResult result = OCNotifyAllObservers(handle, OC_NA_QOS);
        if (OC_STACK_NO_OBSERVERS != result && OC_STACK_OK != result)
        {
            OIC_LOG(ERROR, TAG, "Notifying observers failed.");
        }
    }

exit:
    OICFree(ins);
    OICFree(queryDup);
    return ehResult;
}

/*
 * This internal method is the entity handler for RD resources and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
static OCEntityHandlerResult rdEntityHandler(OCEntityHandlerFlag flag,
        OCEntityHandlerRequest *ehRequest, void *callbackParameter)
{
    OC_UNUSED(callbackParameter);

    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if (!ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG.");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
            case OC_REST_DISCOVER:
                ehRet = handleGetRequest(ehRequest);
                break;
            case OC_REST_POST:
                ehRet = handlePublishRequest(ehRequest);
                break;
            case OC_REST_DELETE:
                ehRet = handleDeleteRequest(ehRequest);
                break;
            case OC_REST_PUT:
            case OC_REST_OBSERVE:
            case OC_REST_OBSERVE_ALL:
            case OC_REST_PRESENCE:
            case OC_REST_NOMETHOD:
                break;
        }
    }

    return ehRet;
}

/**
 * Registers RD resource
 */
OCStackResult OC_CALL OCRDStart()
{
    OCStackResult result = OCCreateResource(&rdHandle,
                                OC_RSRVD_RESOURCE_TYPE_RD,
                                OC_RSRVD_INTERFACE_DEFAULT,
                                OC_RSRVD_RD_URI,
                                rdEntityHandler,
                                NULL,
                                (OC_ACTIVE | OC_DISCOVERABLE | OC_SECURE));

    if (OC_STACK_OK == result)
    {
        OIC_LOG(DEBUG, TAG, "Resource Directory resource created.");
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Failed creating Resource Directory resource.");
        return result;
    }

    return result;
}

/**
 * Stops resource directory server
 */
OCStackResult OC_CALL OCRDStop()
{
    if (!rdHandle)
    {
        OIC_LOG(ERROR, TAG, "Resource Directory resource handle is not initialized.");
        return OC_STACK_NO_RESOURCE;
    }

    OCStackResult result = OCDeleteResource(rdHandle);

    if (OC_STACK_OK == result)
    {
      OIC_LOG(DEBUG, TAG, "Resource Directory resource deleted.");
    }
    else
    {
      OIC_LOG(ERROR, TAG, "Resource Directory resource not deleted.");
    }

    return result;
}

#endif
