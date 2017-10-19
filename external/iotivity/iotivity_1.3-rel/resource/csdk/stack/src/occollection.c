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
#include "ocendpoint.h"
#include "ocstack.h"
#include "ocstackinternal.h"
#include "oicgroup.h"
#include "oic_string.h"
#include "payload_logging.h"
#include "cainterface.h"
#define TAG "OIC_RI_COLLECTION"

static bool AddRTSBaselinePayload(OCRepPayload **linkArray, int size, OCRepPayload **colPayload)
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
    OCStackResult ret = OC_STACK_ERROR;
    size_t dim[MAX_REP_ARRAY_DEPTH] = {size, 0, 0};
    OCRepPayload **linkArr = NULL;

    if (!(linkArr = OCLinksPayloadArrayCreate(collResource->uri, ehRequest, false, NULL)))
    {
        OIC_LOG_V(ERROR, TAG, "Failed getting LinksPayloadArray");
        ret = OC_STACK_ERROR;
        goto exit;
    }

    if (size < 1)
    {
        ret = OC_STACK_NO_RESOURCE;
        goto exit;
    }

    OCPayloadFormat contentFormat = OC_FORMAT_UNDEFINED;
    OCGetRequestPayloadVersion(ehRequest, &contentFormat, NULL);
    // from the OCF1.0 linklist specification, ll has array of links
    if ((0 == strcmp(ifQueryParam, OC_RSRVD_INTERFACE_LL)) && (contentFormat == OC_FORMAT_VND_OCF_CBOR))
    {
        for (int n = 0; n < (int)size - 1; n++)
        {
            linkArr[n]->next = linkArr[n + 1];
        }
        colPayload = linkArr[0];
        OICFree(linkArr);
        ret = OC_STACK_OK;
        goto exit;
    }
    else if ((contentFormat == OC_FORMAT_VND_OCF_CBOR || contentFormat == OC_FORMAT_CBOR))
    {
        colPayload = OCRepPayloadCreate();
        VERIFY_PARAM_NON_NULL(TAG, linkArr, "Failed creating LinksPayloadArray");
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
            AddRTSBaselinePayload(linkArr, size, &colPayload);
        }
        OCRepPayloadSetPropObjectArrayAsOwner(colPayload, OC_RSRVD_LINKS, linkArr, dim);
        ret = OC_STACK_OK;
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
    OIC_LOG_V(INFO, TAG, "Send Response result from HandleLinkedListInterface = %d", (int)ret);
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

static bool addPolicyPayload(OCResourceHandle* resourceHandle, OCDevAddr* devAddr,
                             bool isOCFContentFormat, OCRepPayload** outPolicy)
{
    if (resourceHandle == NULL || devAddr == NULL || outPolicy == NULL)
    {
        return false;
    }

    OCResourceProperty p = OCGetResourceProperties(resourceHandle);
    OCRepPayload* policy = OCRepPayloadCreate();
    if (policy)
    {
        OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
        if (!isOCFContentFormat)
        {
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

#if defined(TCP_ADAPTER) && defined(__WITH_TLS__)
                // tls
                if (devAddr)
                {
                    uint16_t tlsPort = 0;
                    GetTCPPortInfo(devAddr, &tlsPort, true);
                    OCRepPayloadSetPropInt(policy, OC_RSRVD_TLS_PORT, tlsPort);
                }
#endif
            }
#ifdef TCP_ADAPTER
#ifdef  __WITH_TLS__
            if (!(p & OC_SECURE))
            {
#endif
                // tcp
                if (devAddr)
                {
                    uint16_t tcpPort = 0;
                    GetTCPPortInfo(devAddr, &tcpPort, false);
                    OCRepPayloadSetPropInt(policy, OC_RSRVD_TCP_PORT, tcpPort);
                }
#ifdef  __WITH_TLS__
            }
#endif
#endif
        }
    }
    else
    {
        return false;
    }

    *outPolicy = policy;
    return true;
}

static bool translateEndpointsPayload(OCEndpointPayload* epPayloadOrg,
                                      size_t size, OCRepPayload*** outArrayPayload)
{
    bool result = false;
    OCRepPayload** arrayPayload = (OCRepPayload**)OICMalloc(sizeof(OCRepPayload*) * (size));
    VERIFY_PARAM_NON_NULL(TAG, arrayPayload, "Failed creating arrayPayload");
    VERIFY_PARAM_NON_NULL(TAG, epPayloadOrg, "Invalid Parameter epPayload");
    VERIFY_PARAM_NON_NULL(TAG, outArrayPayload, "Invalid Parameter outArrayPayload");
    OCEndpointPayload* epPayload = epPayloadOrg;

    for (size_t i = 0; (i < size) && (epPayload != NULL) ; i++)
    {
        arrayPayload[i] = OCRepPayloadCreate();
        if (!arrayPayload[i])
        {
            for (size_t j = 0; j < i; j++)
            {
                OCRepPayloadDestroy(arrayPayload[j]);
            }
            result = false;
            goto exit;
        }
        char* createdEPStr = OCCreateEndpointString(epPayload);
        OIC_LOG_V(DEBUG, TAG, " OCCreateEndpointString() = %s", createdEPStr);
        OCRepPayloadSetPropString(arrayPayload[i], OC_RSRVD_ENDPOINT, createdEPStr);

        // in case of pri as 1, skip set property
        if (epPayload->pri != 1 )
            OCRepPayloadSetPropInt(arrayPayload[i], OC_RSRVD_PRIORITY, epPayload->pri);

        epPayload = epPayload->next;
        result = true;
    }
    *outArrayPayload = arrayPayload;
exit:
    OCEndpointPayloadDestroy(epPayloadOrg);
    if (result == false)
    {
        OICFree(arrayPayload);
    }
    return result;
}

OCRepPayload** BuildCollectionLinksPayloadArray(const char* resourceUri,
    bool isOCFContentFormat, OCDevAddr* devAddr, bool insertSelfLink, size_t* createdArraySize)
{
    bool result = false;
    OCRepPayload** arrayPayload = NULL;
    size_t childCount = 0;

    const OCResourceHandle colResourceHandle = OCGetResourceHandleAtUri(resourceUri);
    VERIFY_PARAM_NON_NULL(TAG, colResourceHandle, "Failed geting colResourceHandle");

    const OCChildResource* childResource = ((OCResource*)colResourceHandle)->rsrcChildResourcesHead;
    VERIFY_PARAM_NON_NULL(TAG, childResource, "Failed geting childResource");

    //children resources count calculation
    const OCChildResource* childCountResource = childResource;
    do {
        childCount++;
        childCountResource = childCountResource->next;
    } while (childCountResource);

    if (insertSelfLink)
    {
        childCount++;
    }

    arrayPayload = (OCRepPayload**)OICMalloc(sizeof(OCRepPayload*) * (childCount));
    VERIFY_PARAM_NON_NULL(TAG, arrayPayload, "Failed creating arrayPayload");

    OCResource* iterResource = childResource->rsrcResource;
    for (size_t i = 0; i < childCount; i++)
    {
        arrayPayload[i] = OCRepPayloadCreate();
        if (!arrayPayload[i])
        {
            for (size_t j = 0; j < i; j++)
            {
                OCRepPayloadDestroy(arrayPayload[j]);
            }
            result = false;
            goto exit;
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

        OCRepPayload* outPolicy = NULL;
        //Policy Map will have tls and tcp properties for legacy support,
        // in case contents format is cbor instead of vnd.ocf/cbor
        if (!addPolicyPayload((OCResourceHandle*)iterResource, devAddr, isOCFContentFormat,
                               &outPolicy) ||
            !OCRepPayloadSetPropObjectAsOwner(arrayPayload[i], OC_RSRVD_POLICY, outPolicy))
        {
            OCRepPayloadDestroy(outPolicy);
            for (size_t j = 0; j <= i; j++)
            {
                OCRepPayloadDestroy(arrayPayload[j]);
            }
            result = false;
            goto exit;
        }

        //EP is added in case contents format is vnd.ocf/cbor
        if (isOCFContentFormat)
        {
            CAEndpoint_t *info = NULL;
            size_t networkSize = 0;
            size_t epSize = 0;
            CAGetNetworkInformation(&info, &networkSize);
            OIC_LOG_V(DEBUG, TAG, "Network Information size = %d", (int) networkSize);

            OCEndpointPayload *listHead = NULL;
            CreateEndpointPayloadList(iterResource,
                devAddr, info, networkSize, &listHead, &epSize, NULL);
            OICFree(info);
            OIC_LOG_V(DEBUG, TAG, "Result of CreateEndpointPayloadList() = %s",
                                  listHead ? "true":"false");

            OCRepPayload** epArrayPayload = NULL;
            size_t epsDim[MAX_REP_ARRAY_DEPTH] = { epSize, 0, 0 };

            if (!translateEndpointsPayload(listHead, epSize, &epArrayPayload) ||
                !OCRepPayloadSetPropObjectArrayAsOwner(arrayPayload[i],
                            OC_RSRVD_ENDPOINTS, epArrayPayload, epsDim))
            {
                if (epArrayPayload)
                {
                    for (size_t j = 0; j < epSize; j++)
                    {
                        OCRepPayloadDestroy(epArrayPayload[j]);
                    }
                    OICFree(epArrayPayload);
                }

                for (size_t j = 0; j <= i; j++)
                {
                    OCRepPayloadDestroy(arrayPayload[j]);
                }
                result = false;
                goto exit;
            }
        }

        if (iterResource != colResourceHandle)
        {
            childResource = childResource->next;
            if (childResource)
            {
                iterResource = childResource->rsrcResource;
            }
            else if (insertSelfLink)
            {
                iterResource = colResourceHandle;
            }
        }
        else // handling selfLink case
        {
            OIC_LOG(INFO, TAG, "adding rel for self link");
            const char* relArray[2] = { "self", "item" };
            size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 2, 0, 0 };
            if (!OCRepPayloadSetStringArray(arrayPayload[i], OC_RSRVD_REL, relArray, dimensions))
            {
                OIC_LOG(ERROR, TAG, "Failed setting rel property");
                result = false;
                goto exit;
            } 
        }
        result = true;
    }

exit:
    if (!result && (arrayPayload != NULL))
    {
        OICFree(arrayPayload);
        arrayPayload = NULL;
    }

    if (arrayPayload != NULL && createdArraySize != NULL)
        *createdArraySize = childCount;
    else if (createdArraySize != NULL)
        *createdArraySize = 0;

    return arrayPayload;
}
