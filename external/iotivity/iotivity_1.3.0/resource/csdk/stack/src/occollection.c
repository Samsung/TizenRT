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

// Defining _POSIX_C_SOURCE macro with 200112L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1-2001 base
// specification (excluding the XSI extension).
// For POSIX.1-2001 base specification,
// Refer http://pubs.opengroup.org/onlinepubs/009695399/
#define _POSIX_C_SOURCE 200112L

#include "occollection.h"
#include "ocpayload.h"
#include "ocstack.h"
#include "ocstackinternal.h"
#include "oicgroup.h"
#include "oic_string.h"
#include "payload_logging.h"

#define TAG "OIC_RI_COLLECTION"

static bool AddRTSBaslinePayload(OCRepPayload **linkArray, int size, OCRepPayload **colPayload)
{
    size_t arraySize = 0;
    for (int j = 0; j < size; j++)
    {
        size_t rtDim[MAX_REP_ARRAY_DEPTH] = {0};
        char **rt = NULL;
        OCRepPayloadGetStringArray(linkArray[j], OC_RSRVD_RESOURCE_TYPE, &rt, rtDim);
        arraySize += rtDim[0];
        for (size_t l = 0; l < rtDim[0]; l++)
        {
            OICFree(rt[l]);
        }
        OICFree(rt);
    }

    for (OCStringLL *rsrcType = (*colPayload)->types; rsrcType; rsrcType = rsrcType->next, arraySize++);

    OIC_LOG_V(DEBUG, TAG, "Number of RTS elements : %zd", arraySize);
    size_t dim[MAX_REP_ARRAY_DEPTH] = {arraySize, 0, 0};
    char **rts = (char **)OICMalloc(sizeof(char *) * arraySize);
    if (!rts)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        return OC_STACK_NO_MEMORY;
    }
    int k = 0;
    for (int j = 0; j < size; j++)
    {
        size_t rtDim[MAX_REP_ARRAY_DEPTH] = {0};
        char **rt = NULL;
        OCRepPayloadGetStringArray(linkArray[j], OC_RSRVD_RESOURCE_TYPE, &rt, rtDim);
        for (size_t l = 0; l < rtDim[0]; l++)
        {
            rts[k++] = OICStrdup(rt[l]);
            OICFree(rt[l]);
        }
        OICFree(rt);
    }
    for (OCStringLL *rsrcType = (*colPayload)->types; rsrcType; rsrcType = rsrcType->next, size++)
    {
        rts[k++] = OICStrdup(rsrcType->value);
    }

    bool b = OCRepPayloadSetStringArrayAsOwner(*colPayload, OC_RSRVD_RTS, rts, dim);

    if (!b)
    {
        for (size_t j = 0; j < arraySize; j++)
        {
            OICFree(rts[j]);
        }
        OICFree(rts);
    }

    return b;
}

static OCStackResult SendResponse(const OCRepPayload *payload, const OCEntityHandlerRequest *ehRequest,
    const OCResource* collResource, OCEntityHandlerResult ehResult)
{
    OCEntityHandlerResponse response = {0};
    response.ehResult = ehResult;
    response.payload = (OCPayload*)payload;
    response.persistentBufferFlag = 0;
    response.requestHandle = (OCRequestHandle) ehRequest->requestHandle;
    response.resourceHandle = (OCResourceHandle) collResource;
    return OCDoResponse(&response);
}

uint8_t GetNumOfResourcesInCollection(const OCResource *collResource)
{
    uint8_t size = 0;
    for (OCChildResource *tempChildResource = collResource->rsrcChildResourcesHead;
        tempChildResource; tempChildResource = tempChildResource->next)
    {
        size++;
    }
    return size;
}

static OCStackResult HandleLinkedListInterface(OCEntityHandlerRequest *ehRequest, char *ifQueryParam)
{
    if (!ehRequest)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCResource *collResource = (OCResource *)ehRequest->resource;
    if (!collResource)
    {
        return OC_STACK_INVALID_PARAM;
    }

    uint8_t size = GetNumOfResourcesInCollection(collResource);
    OCRepPayload *colPayload = NULL;
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    int i = 0;
    OCStackResult ret = OC_STACK_OK;
    size_t dim[MAX_REP_ARRAY_DEPTH] = {size, 0, 0};
    OCRepPayload **linkArr = (OCRepPayload **)OICCalloc(size, sizeof(OCRepPayload *));
    VERIFY_PARAM_NON_NULL(TAG, linkArr, "Failed creating LinkArray");

    for (OCChildResource *tempChildResource = collResource->rsrcChildResourcesHead;
        tempChildResource && ret == OC_STACK_OK; tempChildResource = tempChildResource->next)
    {
        OCResource* temp = tempChildResource->rsrcResource;
        if (temp)
        {
            ret = BuildResponseRepresentation(temp, &linkArr[i++], &ehRequest->devAddr);
        }
    }

    if (ret == OC_STACK_OK)
    {
        colPayload = OCRepPayloadCreate();
        if (colPayload)
        {
            if (0 == strcmp(OC_RSRVD_INTERFACE_DEFAULT, ifQueryParam))
            {
                //TODO : Add resource type filtering once collections
                // start supporting queries.
                OCRepPayloadAddResourceType(colPayload, OC_RSRVD_RESOURCE_TYPE_COLLECTION);
                for (OCResourceType *types = collResource->rsrcType; types; types = types->next)
                {
                    if (0 != strcmp(OC_RSRVD_RESOURCE_TYPE_COLLECTION, types->resourcetypename))
                    {
                        OCRepPayloadAddResourceType(colPayload, types->resourcetypename);
                    }
                }
                for (OCResourceInterface *itf = collResource->rsrcInterface; itf; itf = itf->next)
                {
                    OCRepPayloadAddInterface(colPayload, itf->name);
                }
                AddRTSBaslinePayload(linkArr, i, &colPayload);
            }
            OCRepPayloadSetPropObjectArrayAsOwner(colPayload, OC_RSRVD_LINKS, linkArr, dim);
        }
    }

exit:
    if (ret == OC_STACK_OK)
    {
        ehResult = OC_EH_OK;
    }
    else
    {
        ehResult = (ret == OC_STACK_NO_RESOURCE) ? OC_EH_RESOURCE_NOT_FOUND : OC_EH_ERROR;
    }
    ret = SendResponse(colPayload, ehRequest, collResource, ehResult);
    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *)colPayload);
    OCRepPayloadDestroy(colPayload);
    return ret;
}

static OCStackResult HandleBatchInterface(OCEntityHandlerRequest *ehRequest)
{
    if (!ehRequest)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult stackRet = OC_STACK_OK;
    char *storeQuery = NULL;
    OCResource *collResource = (OCResource *)ehRequest->resource;

    if (stackRet == OC_STACK_OK)
    {

        if (collResource->rsrcChildResourcesHead)
        {
            storeQuery = ehRequest->query;
            ehRequest->query = NULL;
            OIC_LOG_V(DEBUG, TAG, "Query : %s", ehRequest->query);
        }

        uint8_t numRes = 0;
        for (OCChildResource *tempChildResource = collResource->rsrcChildResourcesHead;
            tempChildResource; tempChildResource = tempChildResource->next, numRes++)
        {
            OCResource* tempRsrcResource = tempChildResource->rsrcResource;
            if (tempRsrcResource)
            {
                // Note that all entity handlers called through a collection
                // will get the same pointer to ehRequest, the only difference
                // is ehRequest->resource
                ehRequest->resource = (OCResourceHandle) tempRsrcResource;
                OCEntityHandlerResult ehResult = tempRsrcResource->entityHandler(OC_REQUEST_FLAG,
                                           ehRequest, tempRsrcResource->entityHandlerCallbackParam);

                // The default collection handler is returning as OK
                if (stackRet != OC_STACK_SLOW_RESOURCE)
                {
                    stackRet = OC_STACK_OK;
                }
                // if a single resource is slow, then entire response will be treated
                // as slow response
                if (ehResult == OC_EH_SLOW)
                {
                    OIC_LOG(INFO, TAG, "This is a slow resource");
                    ((OCServerRequest *)ehRequest->requestHandle)->slowFlag = 1;
                    stackRet = EntityHandlerCodeToOCStackCode(ehResult);
                }
            }
            else
            {
                break;
            }
        }
        ehRequest->resource = (OCResourceHandle) collResource;
    }
    ehRequest->query = storeQuery;
    return stackRet;
}

OCStackResult DefaultCollectionEntityHandler(OCEntityHandlerFlag flag, OCEntityHandlerRequest *ehRequest)
{
    if (!ehRequest || !ehRequest->query)
    {
        return OC_STACK_INVALID_PARAM;
    }
    // Delete is not supported for any interface query method.
    if (ehRequest->method == OC_REST_DELETE || flag != OC_REQUEST_FLAG)
    {
        return OC_STACK_ERROR;
    }
    OIC_LOG_V(INFO, TAG, "DefaultCollectionEntityHandler with query %s", ehRequest->query);

    char *ifQueryParam = NULL;
    char *rtQueryParam = NULL;
    OCStackResult result = ExtractFiltersFromQuery(ehRequest->query, &ifQueryParam, &rtQueryParam);
    if (result != OC_STACK_OK)
    {
        result = OC_STACK_NO_RESOURCE;
        goto exit;
    }
    if (!ifQueryParam)
    {
        ifQueryParam = OICStrdup(OC_RSRVD_INTERFACE_LL);
    }

    VERIFY_PARAM_NON_NULL(TAG, ifQueryParam, "Invalid Parameter ifQueryParam");

    if (0 == strcmp(ifQueryParam, OC_RSRVD_INTERFACE_LL) || 0 == strcmp (ifQueryParam, OC_RSRVD_INTERFACE_DEFAULT))
    {
        if (ehRequest->method == OC_REST_PUT || ehRequest->method == OC_REST_POST)
        {
            result =  OC_STACK_ERROR;
        }
        else
        {
            result = HandleLinkedListInterface(ehRequest, ifQueryParam);
        }
    }
    else if (0 == strcmp(ifQueryParam, OC_RSRVD_INTERFACE_BATCH))
    {
        OCServerRequest *request = GetServerRequestUsingHandle((OCServerRequest *)ehRequest->requestHandle);
        if (request)
        {
            request->numResponses = GetNumOfResourcesInCollection((OCResource *)ehRequest->resource);
            request->ehResponseHandler = HandleAggregateResponse;
            result = HandleBatchInterface(ehRequest);
        }
    }
    else if (0 == strcmp(ifQueryParam, OC_RSRVD_INTERFACE_GROUP))
    {
        OIC_LOG_V(INFO, TAG, "IF_COLLECTION %d with request ::\n", ehRequest->method);
        OIC_LOG_PAYLOAD(INFO, ehRequest->payload);
        result = BuildCollectionGroupActionCBORResponse(ehRequest->method, (OCResource *) ehRequest->resource, ehRequest);
    }
exit:
    if (result != OC_STACK_OK)
    {
        result = SendResponse(NULL, ehRequest, (OCResource *)ehRequest->resource, OC_EH_BAD_REQ);
    }
    OICFree(ifQueryParam);
    OICFree(rtQueryParam);
    return result;
}

static OCRepPayload* addPolicyPayload(OCResourceHandle* resourceHandle, OCDevAddr* devAddr)
{
    OCResourceProperty p = OCGetResourceProperties(resourceHandle);
    OCRepPayload* policy = OCRepPayloadCreate();
    if (policy)
    {
        OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
        OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);

        if (p & OC_SECURE)
        {
            uint16_t securePort = 0;
            if (devAddr)
            {
                if (devAddr->adapter == OC_ADAPTER_IP)
                {
                    if (devAddr->flags & OC_IP_USE_V6)
                    {
                        securePort = caglobals.ip.u6s.port;
                    }
                    else if (devAddr->flags & OC_IP_USE_V4)
                    {
                        securePort = caglobals.ip.u4s.port;
                    }
                }
            }
            OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);

#ifdef TCP_ADAPTER
#ifdef __WITH_TLS__
            // tls
            if (devAddr)
            {
                uint16_t tlsPort = 0;
                GetTCPPortInfo(devAddr, &tlsPort, true);
                OCRepPayloadSetPropInt(policy, OC_RSRVD_TLS_PORT, tlsPort);
            }
#else
        }
        // tcp
        if (devAddr)
        {
            uint16_t tcpPort = 0;
            GetTCPPortInfo(devAddr, &tcpPort, false);
            OCRepPayloadSetPropInt(policy, OC_RSRVD_TCP_PORT, tcpPort);
#endif
#endif
        }
    }
    return policy;
}

OCStackResult BuildCollectionLinksPayloadValue(const char* resourceUri, OCRepPayloadValue** linksRepPayloadValue,
    OCDevAddr* devAddr)
{
    OCStackResult result = OC_STACK_ERROR;

    OCRepPayloadValue* createdPayloadValue = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
    if (!createdPayloadValue)
    {
        return result;
    }
    createdPayloadValue->name = OC_RSRVD_LINKS;
    createdPayloadValue->type = OCREP_PROP_ARRAY;

    const OCResourceHandle colResourceHandle = OCGetResourceHandleAtUri(resourceUri);
    if (!colResourceHandle)
    {
        //in case input resource is not registered resource.
        OICFree(createdPayloadValue);
        return result;
    }

    const OCChildResource* childResource = ((OCResource*)colResourceHandle)->rsrcChildResourcesHead;
    if (!childResource)
    {
        //in case input resource is not collection resource.
        OICFree(createdPayloadValue);
        return result;
    }

    //children resources count calculation
    size_t childCount = 0;
    const OCChildResource* childCountResource = childResource;
    do {
        childCount++;
        childCountResource = childCountResource->next;
    } while (childCountResource);

    OCRepPayload** arrayPayload = (OCRepPayload** )OICMalloc(sizeof(OCRepPayload*) * (childCount + 1));
    if (!arrayPayload)
    {
        OICFree(createdPayloadValue);
        return result;
    }

    OCResource* iterResource = (OCResource*) colResourceHandle;
    for (size_t i = 0; i < childCount + 1; i++)
    {
        arrayPayload[i] = OCRepPayloadCreate();
        if (!arrayPayload[i])
        {
            OICFree(createdPayloadValue);

            for (size_t j = 0; j < i; j++)
            {
                OCRepPayloadDestroy(arrayPayload[j]);
            }

            OICFree(arrayPayload);

            return result;
        }

        OCRepPayloadSetUri(arrayPayload[i], iterResource->uri);

        for (OCResourceType* resType = iterResource->rsrcType; resType;
            resType = resType->next)
        {
            OCRepPayloadAddResourceType(arrayPayload[i], resType->resourcetypename);
        }

        for (OCResourceInterface* resInterface = iterResource->rsrcInterface; resInterface;
                                  resInterface = resInterface->next)
        {
            OCRepPayloadAddInterface(arrayPayload[i], resInterface->name);
        }

        //@todo selectively fill in the data between Policy Map for OIC and EP Array for OCF
        if (!OCRepPayloadSetPropObjectAsOwner(arrayPayload[i], OC_RSRVD_POLICY,
                             addPolicyPayload((OCResourceHandle*)iterResource, devAddr)))
        {
            OICFree(createdPayloadValue);

            for (size_t j = 0; j <= i; j++)
            {
                OCRepPayloadDestroy(arrayPayload[j]);
            }

            OICFree(arrayPayload);

            return result;
        }

        if (i == 0)
        {
            iterResource = childResource->rsrcResource;
        }
        else
        {
            childResource = childResource->next;
            if (childResource)
            {
                iterResource = childResource->rsrcResource;
            }
        }
    }

    //create OCRepPayloadValue internal structure and pass
    //createdPayloadValue:OCRepPayloadValue.createdPayloadValueArray:arr->arrayPayload:objArray
    OCRepPayloadValueArray* createdPayloadValueArray = &createdPayloadValue->arr;
    size_t linkDim[MAX_REP_ARRAY_DEPTH] = { childCount + 1, 0, 0 };
    OC_STATIC_ASSERT(sizeof(createdPayloadValueArray->dimensions) == sizeof(linkDim), "Array size mismatch!");
    memcpy(createdPayloadValueArray->dimensions, linkDim, sizeof(linkDim));
    createdPayloadValueArray->type = OCREP_PROP_OBJECT;
    createdPayloadValueArray->objArray = arrayPayload;

    *linksRepPayloadValue = createdPayloadValue;

    result = OC_STACK_OK;
    return result;
}
