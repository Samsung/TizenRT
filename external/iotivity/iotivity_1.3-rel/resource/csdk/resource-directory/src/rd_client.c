//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License a
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
#include "rd_client.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "oic_malloc.h"
#include "oic_string.h"
#include "octypes.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "ocendpoint.h"
#include "payload_logging.h"
#include "cainterface.h"

#define TAG "OIC_RD_CLIENT"

#ifdef RD_CLIENT

OCStackResult OC_CALL OCRDDiscover(OCDoHandle *handle, OCConnectivityType connectivityType,
                                   OCCallbackData *cbBiasFactor, OCQualityOfService qos)
{
    if (!cbBiasFactor || !cbBiasFactor->cb)
    {
        OIC_LOG(DEBUG, TAG, "No callback function specified.");
        return OC_STACK_INVALID_CALLBACK;
    }

    /* Start a discovery query*/
    char queryUri[MAX_URI_LENGTH] = { '\0' };
    snprintf(queryUri, MAX_URI_LENGTH, "coap://%s%s", OC_MULTICAST_PREFIX, OC_RSRVD_RD_URI);
    OIC_LOG_V(DEBUG, TAG, "Querying RD: %s\n", queryUri);

    return OCDoResource(handle, OC_REST_DISCOVER, queryUri, NULL, NULL, connectivityType, qos,
                        cbBiasFactor, NULL, 0);
}

static void RDPublishContextDeleter(void *ctx)
{
    OCCallbackData *cbData = (OCCallbackData*)ctx;
    if (cbData->cd)
    {
        cbData->cd(cbData->context);
    }
    OICFree(cbData);
}

OCStackApplicationResult RDPublishCallback(void *ctx,
                                           OCDoHandle handle,
                                           OCClientResponse *clientResponse)
{
    OCCallbackData *cbData = (OCCallbackData*)ctx;

    // Update resource unique id in stack.
    OCRepPayload **links = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    if (clientResponse && clientResponse->payload)
    {
        OCRepPayload *rdPayload = (OCRepPayload *) clientResponse->payload;
        if (!OCRepPayloadGetPropObjectArray(rdPayload, OC_RSRVD_LINKS, &links, dimensions))
        {
            OIC_LOG(DEBUG, TAG, "No links in publish response");
            goto exit;
        }
        for(size_t i = 0; i < dimensions[0]; i++)
        {
            char *uri = NULL;
            if (!OCRepPayloadGetPropString(links[i], OC_RSRVD_HREF, &uri))
            {
                OIC_LOG(ERROR, TAG, "Missing 'href' in publish response");
                goto next;
            }
            OCResourceHandle resourceHandle = OCGetResourceHandleAtUri(uri);
            if (resourceHandle == NULL)
            {
                OIC_LOG_V(ERROR, TAG, "No resource exists with uri: %s", uri);
                goto next;
            }
            int64_t ins = 0;
            if (!OCRepPayloadGetPropInt(links[i], OC_RSRVD_INS, &ins))
            {
                OIC_LOG(ERROR, TAG, "Missing 'ins' in publish response");
                goto next;
            }
            OCBindResourceInsToResource(resourceHandle, ins);
        next:
            OICFree(uri);
        }
    }

exit:
    if (links)
    {
        for (size_t i = 0; i < dimensions[0]; i++)
        {
            OCRepPayloadDestroy(links[i]);
        }
        OICFree(links);
    }
    return cbData->cb(cbData->context, handle, clientResponse);
}

OCStackResult OC_CALL OCRDPublish(OCDoHandle *handle, const char *host,
                                  OCConnectivityType connectivityType,
                                  OCResourceHandle *resourceHandles, uint8_t nHandles,
                                  OCCallbackData *cbData, OCQualityOfService qos)
{
    // Validate input parameters.
    if (!host)
    {
        return OC_STACK_INVALID_IP;
    }

    if (!cbData || !cbData->cb)
    {
        return OC_STACK_INVALID_CALLBACK;
    }

    // Get Device ID from stack.
    const unsigned char *id = (const unsigned char *) OCGetServerInstanceIDString();

    return OCRDPublishWithDeviceId(handle, host, id, connectivityType, resourceHandles, nHandles,
                                   cbData, qos);
}

static OCRepPayload *RDPublishPayloadCreate(const unsigned char *id,
        const OCResourceHandle *resourceHandles, uint8_t nHandles)
{
    assert(id);

    OCStackResult result = OC_STACK_ERROR;
    OCRepPayload *rdPayload = NULL;
    size_t dim[MAX_REP_ARRAY_DEPTH] = {0};
    CAEndpoint_t *caEps = NULL;
    size_t nCaEps = 0;
    OCRepPayload **eps = NULL;

    rdPayload =  (OCRepPayload *)OCRepPayloadCreate();
    if (!rdPayload)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        goto exit;
    }

    // Common properties
    dim[0] = 1;
    char **rts = (char **)OICCalloc(dim[0], sizeof(char *));
    if (!rts)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        goto exit;
    }
    rts[0] = OICStrdup(OC_RSRVD_RESOURCE_TYPE_RES);
    OCRepPayloadSetStringArrayAsOwner(rdPayload, OC_RSRVD_RESOURCE_TYPE, rts, dim);

    dim[0] = 2;
    char **ifs = (char **)OICCalloc(dim[0], sizeof(char *));
    if (!ifs)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed!");
        goto exit;
    }
    ifs[0] = OICStrdup(OC_RSRVD_INTERFACE_LL);
    ifs[1] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    OCRepPayloadSetStringArrayAsOwner(rdPayload, OC_RSRVD_INTERFACE, ifs, dim);

    char *n = NULL;
    OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, (void **) &n);
    if (n)
    {
        OCRepPayloadSetPropStringAsOwner(rdPayload, OC_RSRVD_DEVICE_NAME, n);
    }

    // oic.wk.rdpub properties
    OCRepPayloadSetPropString(rdPayload, OC_RSRVD_DEVICE_ID, (const char*) id);
    OCRepPayloadSetPropInt(rdPayload, OC_RSRVD_DEVICE_TTL, OIC_RD_PUBLISH_TTL);

    dim[0] = nHandles;
    OCRepPayload **links = (OCRepPayload **)OICCalloc(dim[0], sizeof(OCRepPayload *));
    if (!links)
    {
        goto exit;
    }
    OCRepPayloadSetPropObjectArrayAsOwner(rdPayload, OC_RSRVD_LINKS, links, dim);
    for (uint8_t j = 0; j < nHandles; j++)
    {
        OCResourceHandle handle = resourceHandles[j];
        if (handle)
        {
            links[j] = OCRepPayloadCreate();

            const char *uri = OCGetResourceUri(handle);
            if (uri)
            {
                OCRepPayloadSetPropString(links[j], OC_RSRVD_HREF, uri);
            }

            uint8_t resourceTypesNumber;
            if (OC_STACK_OK == OCGetNumberOfResourceTypes(handle, &resourceTypesNumber))
            {
                dim[0] = resourceTypesNumber;
                char **resourceTypesNames = (char **)OICCalloc(resourceTypesNumber, sizeof(char *));
                for (uint8_t i = 0; i < resourceTypesNumber; i++)
                {
                    resourceTypesNames[i] = OICStrdup(OCGetResourceTypeName(handle, i));
                    OIC_LOG_V(DEBUG, TAG, "value: %s", resourceTypesNames[i]);
                }
                OCRepPayloadSetStringArrayAsOwner(links[j], OC_RSRVD_RESOURCE_TYPE, resourceTypesNames, dim);
            }

            uint8_t resourceInterfacesNumber;
            if (OC_STACK_OK == OCGetNumberOfResourceInterfaces(handle, &resourceInterfacesNumber))
            {
                dim[0] = resourceInterfacesNumber;
                char **interfacesNames = (char **)OICCalloc(resourceInterfacesNumber, sizeof(char *));
                for (uint8_t i = 0; i < resourceInterfacesNumber; i++)
                {
                    interfacesNames[i] = OICStrdup(OCGetResourceInterfaceName(handle, i));
                    OIC_LOG_V(DEBUG, TAG, "value: %s", interfacesNames[i]);
                }
                OCRepPayloadSetStringArrayAsOwner(links[j], OC_RSRVD_INTERFACE, interfacesNames, dim);
            }

            // rel is always the default ("hosts")

            char anchor[MAX_URI_LENGTH];
            snprintf(anchor, MAX_URI_LENGTH, "ocf://%s", id);
            OCRepPayloadSetPropString(links[j], OC_RSRVD_URI, anchor);

            int64_t ins;
            if (OC_STACK_OK == OCGetResourceIns(handle, &ins))
            {
                OCRepPayloadSetPropInt(links[j], OC_RSRVD_INS, ins);
            }

            OCResourceProperty p = OCGetResourceProperties(handle);
            bool includeSecure = (p & OC_SECURE);
            bool includeNonsecure = (p & OC_NONSECURE);
            p &= (OC_DISCOVERABLE | OC_OBSERVABLE);
            OCRepPayload *policy = OCRepPayloadCreate();
            if (!policy)
            {
                OIC_LOG(ERROR, TAG, "Memory allocation failed!");
                goto exit;
            }
            OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, p);
            OCRepPayloadSetPropObjectAsOwner(links[j], OC_RSRVD_POLICY, policy);

            CAResult_t caResult = CAGetNetworkInformation(&caEps, &nCaEps);
            if (CA_STATUS_FAILED == caResult)
            {
                OIC_LOG(ERROR, TAG, "CAGetNetworkInformation failed!");
                goto exit;
            }
            if (nCaEps)
            {
                eps = (OCRepPayload **)OICCalloc(nCaEps, sizeof(OCRepPayload *));
                if (!eps)
                {
                    OIC_LOG(ERROR, TAG, "Memory allocation failed!");
                    goto exit;
                }
                uint32_t k = 0;
                for (size_t i = 0; i < nCaEps; i++)
                {
                    bool isSecure = (caEps[i].flags & OC_FLAG_SECURE);
                    if ((isSecure && includeSecure) || (!isSecure && includeNonsecure))
                    {
                        char *epStr = OCCreateEndpointStringFromCA(&caEps[i]);
                        if (!epStr)
                        {
                            OIC_LOG(INFO, TAG, "Create endpoint string failed!");
                            continue;
                        }
                        eps[k] = OCRepPayloadCreate();
                        if (!eps[k])
                        {
                            OIC_LOG(ERROR, TAG, "Memory allocation failed!");
                            OICFree(epStr);
                            break;
                        }
                        OCRepPayloadSetPropStringAsOwner(eps[k], OC_RSRVD_ENDPOINT, epStr);
                        OCRepPayloadSetPropInt(eps[k], OC_RSRVD_PRIORITY, 1);
                        ++k;
                    }
                }
                dim[0] = k;
                OCRepPayloadSetPropObjectArrayAsOwner(links[j], OC_RSRVD_ENDPOINTS, eps, dim);
            }
            OICFreeAndSetToNull((void*) &caEps);
        }
    }

    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *) rdPayload);
    result = OC_STACK_OK;

exit:
    OICFreeAndSetToNull((void*) &caEps);
    if (OC_STACK_OK != result)
    {
        OCRepPayloadDestroy(rdPayload);
        rdPayload = NULL;
    }
    return rdPayload;
}

OCStackResult OC_CALL OCRDPublishWithDeviceId(OCDoHandle *handle, const char *host,
                                              const unsigned char *id,
                                              OCConnectivityType connectivityType,
                                              OCResourceHandle *resourceHandles, uint8_t nHandles,
                                              OCCallbackData *cbData, OCQualityOfService qos)
{
    // Validate input parameters.
    if (!host || !cbData || !cbData->cb || !id)
    {
        return OC_STACK_INVALID_CALLBACK;
    }

    OIC_LOG_V(DEBUG, TAG, "Publish Resource to RD with device id [%s]", id);

    OCStackResult result = OC_STACK_OK;
    OCHeaderOption options[2];
    size_t numOptions = 0;
    uint16_t format = COAP_MEDIATYPE_APPLICATION_VND_OCF_CBOR;

    result = OCSetHeaderOption(options, &numOptions, CA_OPTION_CONTENT_FORMAT, &format, sizeof(format));
    if (OC_STACK_OK != result)
    {
        return result;
    }

    result = OCSetHeaderOption(options, &numOptions, CA_OPTION_ACCEPT, &format, sizeof(format));
    if (OC_STACK_OK != result)
    {
        return result;
    }

    OCResourceHandle *pubResHandle = resourceHandles;
    OCResourceHandle defaultResHandles[OIC_RD_DEFAULT_RESOURCE] = { 0 };
    uint8_t nPubResHandles = nHandles;

    // if resource handles is null, "/oic/p" and "/oic/d" resource will be published to RD.
    if (!pubResHandle)
    {
        // get "/oic/d" and "/oic/p" resource handle from stack.
        defaultResHandles[0] = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
        defaultResHandles[1] = OCGetResourceHandleAtUri(OC_RSRVD_PLATFORM_URI);

        for (uint8_t j = 0; j < OIC_RD_DEFAULT_RESOURCE; j++)
        {
            if (defaultResHandles[j])
            {
                OIC_LOG_V(DEBUG, TAG, "Add virtual resource(%s) to resource handle list",
                          OCGetResourceUri(defaultResHandles[j]));
            }
        }

        pubResHandle = defaultResHandles;
        nPubResHandles = OIC_RD_DEFAULT_RESOURCE;
    }

    char targetUri[MAX_URI_LENGTH] = { 0 };
    snprintf(targetUri, MAX_URI_LENGTH, "%s%s", host, OC_RSRVD_RD_URI);
    OIC_LOG_V(DEBUG, TAG, "Target URI: %s", targetUri);

    OCRepPayload *rdPayload = RDPublishPayloadCreate(id, pubResHandle, nPubResHandles);
    if (!rdPayload)
    {
        return OC_STACK_ERROR;
    }

    OCCallbackData *rdPublishContext = (OCCallbackData*)OICMalloc(sizeof(OCCallbackData));
    if (!rdPublishContext)
    {
        return OC_STACK_NO_MEMORY;
    }
    memcpy(rdPublishContext, cbData, sizeof(OCCallbackData));
    OCCallbackData rdPublishCbData;
    rdPublishCbData.context = rdPublishContext;
    rdPublishCbData.cb = RDPublishCallback;
    rdPublishCbData.cd = RDPublishContextDeleter;

    return OCDoResource(handle, OC_REST_POST, targetUri, NULL, (OCPayload *)rdPayload,
                        connectivityType, qos, &rdPublishCbData, options, (uint8_t)numOptions);
}

OCStackResult OC_CALL OCRDDelete(OCDoHandle *handle, const char *host,
                                 OCConnectivityType connectivityType,
                                 OCResourceHandle *resourceHandles, uint8_t nHandles,
                                 OCCallbackData *cbData, OCQualityOfService qos)
{
    // Validate input parameters
    if (!host)
    {
        return OC_STACK_INVALID_IP;
    }

    if (!cbData || !cbData->cb)
    {
        return OC_STACK_INVALID_CALLBACK;
    }

    const unsigned char *id = (const unsigned char *) OCGetServerInstanceIDString();

    return OCRDDeleteWithDeviceId(handle, host, id, connectivityType, resourceHandles, nHandles,
                                  cbData, qos);
}

OCStackResult OC_CALL OCRDDeleteWithDeviceId(OCDoHandle *handle, const char *host,
                                             const unsigned char *id,
                                             OCConnectivityType connectivityType,
                                             OCResourceHandle *resourceHandles, uint8_t nHandles,
                                             OCCallbackData *cbData, OCQualityOfService qos)
{
    // Validate input parameters
    if (!host || !cbData || !cbData->cb || !id)
    {
        return OC_STACK_INVALID_CALLBACK;
    }

    OIC_LOG_V(DEBUG, TAG, "Delete Resource to RD with device id [%s]", id);

    char targetUri[MAX_URI_LENGTH] = { 0 };
    int targetUriBufferRequired = snprintf(targetUri, MAX_URI_LENGTH, "%s%s?di=%s", host, OC_RSRVD_RD_URI, id);
    if (targetUriBufferRequired >= MAX_URI_LENGTH || targetUriBufferRequired < 0)
    {
        return OC_STACK_INVALID_URI;
    }


    int queryLength = 0;
    char queryParam[MAX_URI_LENGTH] = { 0 };
    for (uint8_t j = 0; j < nHandles; j++)
    {
        OCResource *resourceHandle = (OCResource *) resourceHandles[j];
        int64_t ins = 0;
        if (OC_STACK_OK == OCGetResourceIns(resourceHandle, &ins))
        {
            int lenBufferRequired = snprintf((queryParam + queryLength),
                    (MAX_URI_LENGTH - queryLength), "&ins=%" PRId64, ins);
            if (lenBufferRequired >= (MAX_URI_LENGTH - queryLength) || lenBufferRequired < 0)
            {
                return OC_STACK_INVALID_URI;
            }
            queryLength += lenBufferRequired;
            OIC_LOG_V(DEBUG, TAG, "queryParam [%s]", queryParam);
        }
    }

    if (targetUriBufferRequired + queryLength + 1 > MAX_URI_LENGTH)
    {
        return OC_STACK_INVALID_URI;
    }

    OICStrcatPartial(targetUri, sizeof(targetUri), queryParam, strlen(queryParam));
    OIC_LOG_V(DEBUG, TAG, "Target URI: %s", targetUri);

    return OCDoResource(handle, OC_REST_DELETE, targetUri, NULL, NULL, connectivityType,
                        qos, cbData, NULL, 0);
}
#endif
