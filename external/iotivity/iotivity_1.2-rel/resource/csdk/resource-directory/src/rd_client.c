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

#include <stdlib.h>
#include <string.h>

#include "oic_malloc.h"
#include "oic_string.h"
#include "octypes.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "payload_logging.h"

#define TAG "RD_CLIENT"

#ifdef RD_CLIENT

OCStackResult OCRDDiscover(OCDoHandle *handle, OCConnectivityType connectivityType,
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

OCStackResult OCRDPublish(OCDoHandle *handle, const char *host,
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

    return OCRDPublishWithDeviceId(handle, host, id, connectivityType, resourceHandles,
                                   nHandles, cbData, qos);
}

OCStackResult OCRDPublishWithDeviceId(OCDoHandle *handle, const char *host,
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
    snprintf(targetUri, MAX_URI_LENGTH, "%s%s?rt=%s", host,
             OC_RSRVD_RD_URI, OC_RSRVD_RESOURCE_TYPE_RDPUBLISH);
    OIC_LOG_V(DEBUG, TAG, "Target URI: %s", targetUri);

    OCRepPayload *rdPayload =  (OCRepPayload *)OCRepPayloadCreate();
    if (!rdPayload)
    {
        return OC_STACK_NO_MEMORY;
    }

    OCRepPayloadSetPropString(rdPayload, OC_RSRVD_DEVICE_ID, (const char *) id);

    char *deviceName = NULL;
    OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME, (void **) &deviceName);
    if (deviceName)
    {
        OCRepPayloadSetPropString(rdPayload, OC_RSRVD_DEVICE_NAME, deviceName);
        OICFree(deviceName);
    }

    char *platformModelName = NULL;
    OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MODEL_NUM, (void **) &platformModelName);
    if (platformModelName)
    {
        OCRepPayloadSetPropString(rdPayload, OC_DATA_MODEL_NUMBER, platformModelName);
        OICFree(platformModelName);
    }

    OCRepPayloadSetPropInt(rdPayload, OC_RSRVD_DEVICE_TTL, OIC_RD_PUBLISH_TTL);

    OCRepPayload **linkArr = OICCalloc(nPubResHandles, sizeof(OCRepPayload *));
    if (!linkArr)
    {
       OCRepPayloadDestroy(rdPayload);
       return OC_STACK_NO_MEMORY;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {nPubResHandles, 0, 0};

    for (uint8_t j = 0; j < nPubResHandles; j++)
    {
        OCResourceHandle handle = pubResHandle[j];
        if (handle)
        {
            OCRepPayload *link = OCRepPayloadCreate();

            const char *uri = OCGetResourceUri(handle);
            if (uri)
            {
                OCRepPayloadSetPropString(link, OC_RSRVD_HREF, uri);
            }

            uint8_t numElement = 0;
            if (OC_STACK_OK == OCGetNumberOfResourceTypes(handle, &numElement))
            {
                size_t rtDim[MAX_REP_ARRAY_DEPTH] = {numElement, 0, 0};
                char **rt = (char **)OICMalloc(sizeof(char *) * numElement);
                for (uint8_t i = 0; i < numElement; ++i)
                {
                    const char *value = OCGetResourceTypeName(handle, i);
                    OIC_LOG_V(DEBUG, TAG, "value: %s", value);
                    rt[i] = OICStrdup(value);
                }
                OCRepPayloadSetStringArrayAsOwner(link, OC_RSRVD_RESOURCE_TYPE, rt, rtDim);
            }

            numElement = 0;
            if (OC_STACK_OK == OCGetNumberOfResourceInterfaces(handle, &numElement))
            {
                size_t ifDim[MAX_REP_ARRAY_DEPTH] = {numElement, 0, 0};
                char **itf = (char **)OICMalloc(sizeof(char *) * numElement);
                for (uint8_t i = 0; i < numElement; ++i)
                {
                    const char *value = OCGetResourceInterfaceName(handle, i);
                    OIC_LOG_V(DEBUG, TAG, "value: %s", value);
                    itf[i] = OICStrdup(value);
                }
                OCRepPayloadSetStringArrayAsOwner(link, OC_RSRVD_INTERFACE, itf, ifDim);
            }

            uint8_t ins = 0;
            if (OC_STACK_OK == OCGetResourceIns(handle, &ins))
            {
                OCRepPayloadSetPropInt(link, OC_RSRVD_INS, ins);
            }

            size_t mtDim[MAX_REP_ARRAY_DEPTH] = {1, 0, 0};
            char **mediaType = (char **)OICMalloc(sizeof(char *) * 1);
            if (!mediaType)
            {
                OIC_LOG(ERROR, TAG, "Memory allocation failed!");

                for(uint8_t i = 0; i < nPubResHandles; i++)
                {
                    OCRepPayloadDestroy(linkArr[i]);
                }
                OICFree(linkArr);
                OCRepPayloadDestroy(rdPayload);
                return OC_STACK_NO_MEMORY;
            }

            mediaType[0] = OICStrdup(DEFAULT_MESSAGE_TYPE);
            OCRepPayloadSetStringArrayAsOwner(link, OC_RSRVD_MEDIA_TYPE, mediaType, mtDim);

            OCResourceProperty p = OCGetResourceProperties(handle);
            p = (OCResourceProperty) ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE));
            OCRepPayload *policy = OCRepPayloadCreate();
            OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, p);
            OCRepPayloadSetPropObjectAsOwner(link, OC_RSRVD_POLICY, policy);

            linkArr[j] = link;
        }
    }

    OCRepPayloadSetPropObjectArray(rdPayload, OC_RSRVD_LINKS, (const OCRepPayload **)linkArr, dimensions);
    OIC_LOG_PAYLOAD(DEBUG, (OCPayload *) rdPayload);

    for (uint8_t i = 0; i < nPubResHandles; i++)
    {
        OCRepPayloadDestroy(linkArr[i]);
    }
    OICFree(linkArr);

    return OCDoResource(handle, OC_REST_POST, targetUri, NULL, (OCPayload *)rdPayload,
                        connectivityType, qos, cbData, NULL, 0);
}

OCStackResult OCRDDelete(OCDoHandle *handle, const char *host,
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

    return OCRDDeleteWithDeviceId(handle, host, id, connectivityType, resourceHandles,
                                  nHandles, cbData, qos);
}

OCStackResult OCRDDeleteWithDeviceId(OCDoHandle *handle, const char *host,
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
    snprintf(targetUri, MAX_URI_LENGTH, "%s%s?di=%s", host, OC_RSRVD_RD_URI, id);

    uint8_t len = 0;
    char queryParam[MAX_URI_LENGTH] = { 0 };
    for (uint8_t j = 0; j < nHandles; j++)
    {
        OCResource *handle = (OCResource *) resourceHandles[j];
        uint8_t ins = 0;
        OCGetResourceIns(handle, &ins);
        len += snprintf(queryParam + len, MAX_URI_LENGTH, "&ins=%d", ins);
        OIC_LOG_V(DEBUG, TAG, "queryParam [%s]", queryParam);
    }

    OICStrcatPartial(targetUri, sizeof(targetUri), queryParam, strlen(queryParam));
    OIC_LOG_V(DEBUG, TAG, "Target URI: %s", targetUri);

    return OCDoResource(handle, OC_REST_DELETE, targetUri, NULL, NULL, connectivityType,
                        qos, cbData, NULL, 0);
}

#endif
