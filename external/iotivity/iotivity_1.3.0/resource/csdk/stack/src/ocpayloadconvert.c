//******************************************************************
//
// Copyright 2015 Intel Mobile Communications GmbH All Rights Reserved.
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

#ifdef ARDUINO
#define __STDC_LIMIT_MACROS
#endif

#include "ocpayloadcbor.h"
#include "platform_features.h"
#include <stdlib.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocrandom.h"
#include "ocresourcehandler.h"
#include "cbor.h"
#include "ocendpoint.h"

#define TAG "OIC_RI_PAYLOADCONVERT"

// Arbitrarily chosen size that seems to contain the majority of packages
#define INIT_SIZE (255)

// Discovery Links Map Length.
#define LINKS_MAP_LEN (4)

// Endpoint Map length, it contains "ep", "pri".
#define EP_MAP_LEN (2)

// Functions all return either a CborError, or a negative version of the OC_STACK return values
static int64_t OCConvertPayloadHelper(OCPayload *payload, OCPayloadFormat format,
        uint8_t *outPayload, size_t *size);
static int64_t OCConvertDiscoveryPayload(OCDiscoveryPayload *payload, OCPayloadFormat format,
        uint8_t *outPayload, size_t *size);
static int64_t OCConvertRepPayload(OCRepPayload *payload, uint8_t *outPayload, size_t *size);
static int64_t OCConvertRepMap(CborEncoder *map, const OCRepPayload *payload);
static int64_t OCConvertPresencePayload(OCPresencePayload *payload, uint8_t *outPayload,
        size_t *size);
static int64_t OCConvertDiagnosticPayload(OCDiagnosticPayload *payload, uint8_t *outPayload,
        size_t *size);
static int64_t OCConvertSecurityPayload(OCSecurityPayload *payload, uint8_t *outPayload,
        size_t *size);
static int64_t OCConvertIntrospectionPayload(OCIntrospectionPayload *payload, uint8_t *outPayload,
        size_t *size);
static int64_t OCConvertSingleRepPayloadValue(CborEncoder *parent, const OCRepPayloadValue *value);
static int64_t OCConvertSingleRepPayload(CborEncoder *parent, const OCRepPayload *payload);
static int64_t OCConvertArray(CborEncoder *parent, const OCRepPayloadValueArray *valArray);

static int64_t AddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
        const char *value);
static int64_t ConditionalAddTextStringToMap(CborEncoder *map, const char *key, size_t keylen,
        const char *value);

OCStackResult OCConvertPayload(OCPayload* payload, OCPayloadFormat format,
        uint8_t** outPayload, size_t* size)
{
    // TinyCbor Version 47a78569c0 or better on master is required for the re-allocation
    // strategy to work.  If you receive the following assertion error, please do a git-pull
    // from the extlibs/tinycbor/tinycbor directory
    #define CborNeedsUpdating  (((unsigned int)CborErrorOutOfMemory) < ((unsigned int)CborErrorDataTooLarge))
    OC_STATIC_ASSERT(!CborNeedsUpdating, "tinycbor needs to be updated to at least 47a78569c0");
    #undef CborNeedsUpdating

    OCStackResult ret = OC_STACK_INVALID_PARAM;
    int64_t err = CborErrorOutOfMemory;
    uint8_t *out = NULL;
    size_t curSize = INIT_SIZE;

    VERIFY_PARAM_NON_NULL(TAG, payload, "Input param, payload is NULL");
    VERIFY_PARAM_NON_NULL(TAG, outPayload, "OutPayload parameter is NULL");
    VERIFY_PARAM_NON_NULL(TAG, size, "size parameter is NULL");

    OIC_LOG_V(INFO, TAG, "Converting payload of type %d", payload->type);
    if (PAYLOAD_TYPE_SECURITY == payload->type)
    {
        size_t securityPayloadSize = ((OCSecurityPayload *)payload)->payloadSize;
        if (securityPayloadSize > 0)
        {
            curSize = securityPayloadSize;
        }
    }
    if (PAYLOAD_TYPE_INTROSPECTION == payload->type)
    {
        size_t introspectionPayloadSize = ((OCIntrospectionPayload *)payload)->cborPayload.len;
        if (introspectionPayloadSize > 0)
        {
            curSize = introspectionPayloadSize;
        }
    }

    ret = OC_STACK_NO_MEMORY;

    for (;;)
    {
        out = (uint8_t *)OICCalloc(1, curSize);
        VERIFY_PARAM_NON_NULL(TAG, out, "Failed to allocate payload");
        err = OCConvertPayloadHelper(payload, format, out, &curSize);

        if (CborErrorOutOfMemory != err)
        {
            break;
        }

        OICFree(out);
    }

    if (err == CborNoError)
    {
        if ((curSize < INIT_SIZE) &&
            (PAYLOAD_TYPE_SECURITY != payload->type) &&
            (PAYLOAD_TYPE_INTROSPECTION != payload->type))
        {
            uint8_t *out2 = (uint8_t *)OICRealloc(out, curSize);
            VERIFY_PARAM_NON_NULL(TAG, out2, "Failed to increase payload size");
            out = out2;
        }

        *size = curSize;
        *outPayload = out;
        OIC_LOG_V(DEBUG, TAG, "Payload Size: %zd Payload : ", *size);
        OIC_LOG_BUFFER(DEBUG, TAG, *outPayload, *size);
        return OC_STACK_OK;
    }

    //TODO: Proper conversion from CborError to OCStackResult.
    ret = (OCStackResult)-err;

exit:
    OICFree(out);
    return ret;
}

static int64_t OCConvertPayloadHelper(OCPayload* payload, OCPayloadFormat format,
        uint8_t* outPayload, size_t* size)
{
    switch(payload->type)
    {
        case PAYLOAD_TYPE_DISCOVERY:
            return OCConvertDiscoveryPayload((OCDiscoveryPayload*)payload, format,
                    outPayload, size);
        case PAYLOAD_TYPE_REPRESENTATION:
            return OCConvertRepPayload((OCRepPayload*)payload, outPayload, size);
        case PAYLOAD_TYPE_PRESENCE:
            return OCConvertPresencePayload((OCPresencePayload*)payload, outPayload, size);
        case PAYLOAD_TYPE_DIAGNOSTIC:
            return OCConvertDiagnosticPayload((OCDiagnosticPayload*)payload, outPayload, size);
        case PAYLOAD_TYPE_SECURITY:
            return OCConvertSecurityPayload((OCSecurityPayload*)payload, outPayload, size);
        case PAYLOAD_TYPE_INTROSPECTION:
            return OCConvertIntrospectionPayload((OCIntrospectionPayload*)payload,
                                                 outPayload, size);
        default:
            OIC_LOG_V(INFO, TAG, "ConvertPayload default %d", payload->type);
            return CborErrorUnknownType;
    }
}

static int64_t checkError(int64_t err, CborEncoder* encoder, uint8_t* outPayload, size_t* size)
{
    if (err == CborErrorOutOfMemory)
    {
        *size += cbor_encoder_get_extra_bytes_needed(encoder);
        return err;
    }
    else if (err != CborNoError)
    {
        OIC_LOG_V(ERROR, TAG, "Convert Payload failed : %s", cbor_error_string(err));
        return err;
    }
    else
    {
        *size = cbor_encoder_get_buffer_size(encoder, outPayload);
        return err;
    }
}

static int64_t OCConvertSecurityPayload(OCSecurityPayload* payload, uint8_t* outPayload,
        size_t* size)
{
    memcpy(outPayload, payload->securityData, payload->payloadSize);
    *size = payload->payloadSize;

    return CborNoError;
}

static int64_t OCConvertIntrospectionPayload(OCIntrospectionPayload *payload,
        uint8_t *outPayload, size_t *size)
{
    memcpy(outPayload, payload->cborPayload.bytes, payload->cborPayload.len);
    *size = payload->cborPayload.len;

    return CborNoError;
}

static int64_t OCStringLLJoin(CborEncoder *map, char *type, OCStringLL *val)
{
    uint16_t count = 0;
    int64_t err = CborNoError;

    for (OCStringLL *temp = val; temp; temp = temp->next)
    {
        ++count;
    }
    if (count > 0)
    {
        CborEncoder array;
        err |= cbor_encode_text_string(map, type, strlen(type));
        err |= cbor_encoder_create_array(map, &array, count);
        while (val)
        {
            err |= cbor_encode_text_string(&array, val->value, strlen(val->value));
            val = val->next;
        }
        err |= cbor_encoder_close_container(map, &array);
    }

    return err;
}

static int64_t OCConvertResourcePayloadCbor(CborEncoder *linkArray, OCResourcePayload *resource,
                                            OCEndpointPayload *endpoint)
{
    int64_t err = CborNoError;

    // resource map inside the links array.
    size_t linkMapLen = LINKS_MAP_LEN;
    if (resource->rel)
    {
        ++linkMapLen;
    }
    CborEncoder linkMap;
    err |= cbor_encoder_create_map(linkArray, &linkMap, linkMapLen);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating links map");

    // Below are insertions of the resource properties into the map.

    // Uri
    if (endpoint)
    {
        char *endpointStr = OCCreateEndpointString(endpoint);
        if (!endpointStr)
        {
            err = CborErrorInternalError;
        }
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating endpoint string");
        char uri[MAX_URI_LENGTH];
        int snRet = snprintf(uri, MAX_URI_LENGTH, "%s%s", endpointStr, resource->uri);
        if(0 > snRet || snRet >= MAX_URI_LENGTH)
        {
            OICFree(endpointStr);
            VERIFY_CBOR_SUCCESS(TAG, CborErrorInternalError, "Error (snprintf)");
        }

        OICFree(endpointStr);

        err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                  uri);
    }
    else if (!strstr(resource->uri, OC_ENDPOINT_TPS_TOKEN) &&
             resource->rel && !strcmp(resource->rel, "self") &&
             resource->anchor)
    {
        char uri[MAX_URI_LENGTH];
        int snRet = snprintf(uri, MAX_URI_LENGTH, "%s%s", resource->anchor, resource->uri);
        if(0 > snRet || snRet >= MAX_URI_LENGTH)
        {
            VERIFY_CBOR_SUCCESS(TAG, CborErrorInternalError, "Error (snprintf)");
        }

        err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                  uri);
    }
    else
    {
        err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                  resource->uri);
    }
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding uri to links map");

    // Rel - Not a mandatory field
    err |= ConditionalAddTextStringToMap(&linkMap, OC_RSRVD_REL, sizeof(OC_RSRVD_REL) - 1,
                                         resource->rel);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rel to links map");

    // Resource Type
    err |= OCStringLLJoin(&linkMap, OC_RSRVD_RESOURCE_TYPE, resource->types);
    VERIFY_CBOR_SUCCESS(TAG, err,
                        "Failed adding resourceType tag/value to links map");

    // Interface Types
    err |= OCStringLLJoin(&linkMap, OC_RSRVD_INTERFACE, resource->interfaces);
    VERIFY_CBOR_SUCCESS(TAG, err,
                        "Failed adding interfaces tag/value to links map");

    // Policy
    CborEncoder policyMap;
    err |= cbor_encode_text_string(&linkMap, OC_RSRVD_POLICY,
                                   sizeof(OC_RSRVD_POLICY) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy tag to links map");
    err |= cbor_encoder_create_map(&linkMap, &policyMap, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy map to links map");

    // Bitmap
    err |=  cbor_encode_text_string(&policyMap, OC_RSRVD_BITMAP,
                                    sizeof(OC_RSRVD_BITMAP) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap tag to policy map");
    err |= cbor_encode_uint(&policyMap, resource->bitmap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap value to policy map");

    // Secure
    bool isSecure;
    isSecure = endpoint ? (endpoint->family & OC_FLAG_SECURE) : resource->secure;
    err |= cbor_encode_text_string(&policyMap, OC_RSRVD_SECURE,
                                   sizeof(OC_RSRVD_SECURE) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure tag to policy map");
    err |= cbor_encode_boolean(&policyMap, isSecure);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure value to policy map");

    if (isSecure)
    {
        err |= cbor_encode_text_string(&policyMap, OC_RSRVD_HOSTING_PORT,
                                       sizeof(OC_RSRVD_HOSTING_PORT) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure port tag");
        err |= cbor_encode_uint(&policyMap, endpoint ? endpoint->port : resource->port);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding secure port value");
    }

#ifdef TCP_ADAPTER
#ifdef __WITH_TLS__
    // tls
    if (isSecure)
    {
        err |= cbor_encode_text_string(&policyMap, OC_RSRVD_TLS_PORT,
                                       sizeof(OC_RSRVD_TLS_PORT) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tcp secure port tag");
        err |= cbor_encode_uint(&policyMap, endpoint ? endpoint->port : resource->tcpPort);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tcp secure port value");
    }

    // tcp
    else
#endif
    {
        err |= cbor_encode_text_string(&policyMap, OC_RSRVD_TCP_PORT,
                                       sizeof(OC_RSRVD_TCP_PORT) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tcp port tag");
        err |= cbor_encode_uint(&policyMap, endpoint ? endpoint->port : resource->tcpPort);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tcp port value");
    }
#endif
    err |= cbor_encoder_close_container(&linkMap, &policyMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing policy map");

    // Finished encoding a resource, close the map.
    err |= cbor_encoder_close_container(linkArray, &linkMap);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing link map");

exit:
    return err;
}

static int64_t OCConvertDiscoveryPayloadCbor(OCDiscoveryPayload *payload,
                                             uint8_t *outPayload, size_t *size)
{
    CborEncoder encoder;
    int64_t err = CborNoError;

    cbor_encoder_init(&encoder, outPayload, *size, 0);

    /*
    The format for the payload is "modelled" as JSON.

    [                                                  // rootArray
        {                                              // rootMap
            "di" : UUID,                               // device ID
            "rt": ["oic.wk.res"]
            "n":"MyDevice"
            "if":"oic.if.ll oic.if.baseline"
            "di": "0685B960-736F-46F7-BEC0-9E6CBD61ADC1",
            links :[                                   // linksArray contains maps of resources
                        {
                            href, rt, if, policy  // Resource 1
                        },
                        {
                            href, rt, if, policy  // Resource 2
                        },
                        .
                        .
                        .
                    ]
        }
        {                                              // rootMap
            ...
        }
    ]
    */

    // Open the main root array
    size_t arrayCount = 0;
    for (OCDiscoveryPayload *temp = payload; temp; temp = temp->next)
    {
        arrayCount++;
    }
    CborEncoder rootArray;
    err |= cbor_encoder_create_array(&encoder, &rootArray, arrayCount);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery root array");

    while (payload && payload->resources)
    {
        // Open the root map in the root array
        CborEncoder rootMap;
        err |= cbor_encoder_create_map(&rootArray, &rootMap, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery map");

        // Insert Name
        err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_DEVICE_NAME,
                sizeof(OC_RSRVD_DEVICE_NAME) - 1, payload->name);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting name");

        // Insert Device ID into the root map
        err |= AddTextStringToMap(&rootMap, OC_RSRVD_DEVICE_ID, sizeof(OC_RSRVD_DEVICE_ID) - 1,
                payload->sid);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting device id");

        // Insert Resource Type
        err |= OCStringLLJoin(&rootMap, OC_RSRVD_RESOURCE_TYPE, payload->type);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting RT");

        // Insert interfaces
        err |= OCStringLLJoin(&rootMap, OC_RSRVD_INTERFACE, payload->iface);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding interface types tag/value");

        // Insert Links into the root map.
        CborEncoder linkArray;
        err |= cbor_encode_text_string(&rootMap, OC_RSRVD_LINKS, sizeof(OC_RSRVD_LINKS) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array tag");
        size_t resourceCount =  OCDiscoveryPayloadGetResourceCount(payload);
        err |= cbor_encoder_create_array(&rootMap, &linkArray, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array");

        bool isSelf = false;
        const char *deviceId = OCGetServerInstanceIDString();
        if (NULL != deviceId)
        {
            isSelf = !strcmp(payload->sid, deviceId);
        }

        for (size_t i = 0; i < resourceCount; ++i)
        {
            OCResourcePayload *resource = OCDiscoveryPayloadGetResource(payload, i);
            VERIFY_PARAM_NON_NULL(TAG, resource, "Failed retrieving resource");

            if (isSelf || !resource->eps)
            {
                err |= OCConvertResourcePayloadCbor(&linkArray, resource, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array");
            }
            else
            {
                size_t epsCount = OCEndpointPayloadGetEndpointCount(resource->eps);
                for (size_t j = 0; j < epsCount; ++j)
                {
                    OCEndpointPayload* ep = OCEndpointPayloadGetEndpoint(resource->eps, j);
                    err |= OCConvertResourcePayloadCbor(&linkArray, resource, ep);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array");
                }
            }
        }

        // Close links array inside the root map.
        err |= cbor_encoder_close_container(&rootMap, &linkArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing link array");
        // close root map inside the root array.
        err |= cbor_encoder_close_container(&rootArray, &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");

        payload = payload->next;
    }

    // Close the final root array.
    err |= cbor_encoder_close_container(&encoder, &rootArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");

exit:
    return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertDiscoveryPayloadVndOcfCbor(OCDiscoveryPayload *payload,
                                                   uint8_t *outPayload, size_t *size)
{
    CborEncoder encoder;
    int64_t err = CborNoError;

    cbor_encoder_init(&encoder, outPayload, *size, 0);

    /*
    The format for the payload is "modelled" as JSON.

    [                                                  // rootArray
        {
            href, anchor, rt, if, policy, eps  // Resource 1
        },
        {
            href, anchor, rt, if, policy, eps  // Resource 2
        },
        .
        .
        .
    ]
    */

    CborEncoder rootArray;
    CborEncoder rootMap;
    CborEncoder linkArray;
    bool isBaseline = payload->name || payload->type || payload->iface;
    if (isBaseline)
    {
        // Open the root array
        err |= cbor_encoder_create_array(&encoder, &rootArray, 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery root array");

        // Open the root map
        err |= cbor_encoder_create_map(&rootArray, &rootMap, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery map");

        // Insert Name
        err |= ConditionalAddTextStringToMap(&rootMap, OC_RSRVD_DEVICE_NAME,
                sizeof(OC_RSRVD_DEVICE_NAME) - 1, payload->name);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting name");

        // Insert Resource Type
        err |= OCStringLLJoin(&rootMap, OC_RSRVD_RESOURCE_TYPE, payload->type);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting RT");

        // Insert interfaces
        err |= OCStringLLJoin(&rootMap, OC_RSRVD_INTERFACE, payload->iface);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding interface types tag/value");

        // Insert Links into the root map.
        err |= cbor_encode_text_string(&rootMap, OC_RSRVD_LINKS, sizeof(OC_RSRVD_LINKS) - 1);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array tag");

        err |= cbor_encoder_create_array(&rootMap, &linkArray, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting links array");
    }
    else
    {
        err |= cbor_encoder_create_array(&encoder, &linkArray, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery root array");
    }

    while (payload && payload->resources)
    {
        size_t resourceCount =  OCDiscoveryPayloadGetResourceCount(payload);
        for (size_t i = 0; i < resourceCount; ++i)
        {
            OCResourcePayload *resource = OCDiscoveryPayloadGetResource(payload, i);

            // Open a link map in the root array
            CborEncoder linkMap;
            err |= cbor_encoder_create_map(&linkArray, &linkMap, CborIndefiniteLength);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating discovery map");

            // Uri
            if (!strstr(resource->uri, OC_ENDPOINT_TPS_TOKEN) &&
                resource->rel && !strcmp(resource->rel, "self"))
            {
                char uri[MAX_URI_LENGTH];
                int snRet = snprintf(uri, MAX_URI_LENGTH, "ocf://%s%s", payload->sid, resource->uri);
                if(0 > snRet || snRet >= MAX_URI_LENGTH)
                {
                    VERIFY_CBOR_SUCCESS(TAG, CborErrorInternalError, "Error (snprintf)");
                }

                err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                          uri);
            }
            else
            {
                err |= AddTextStringToMap(&linkMap, OC_RSRVD_HREF, sizeof(OC_RSRVD_HREF) - 1,
                                          resource->uri);
            }
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding uri to links map");

            // Rel - Not a mandatory field
            err |= ConditionalAddTextStringToMap(&linkMap, OC_RSRVD_REL, sizeof(OC_RSRVD_REL) - 1,
                                                 resource->rel);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rel to links map");

            // Anchor
            char anchor[MAX_URI_LENGTH];
            int snRet = snprintf(anchor, MAX_URI_LENGTH, "ocf://%s", payload->sid);
            if(0 > snRet || snRet >= MAX_URI_LENGTH)
            {
                VERIFY_CBOR_SUCCESS(TAG, CborErrorInternalError, "Error (snprintf)");
            }

            err |= AddTextStringToMap(&linkMap, OC_RSRVD_URI, sizeof(OC_RSRVD_URI) - 1, anchor);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding anchor to links map");

            // Resource Type
            err |= OCStringLLJoin(&linkMap, OC_RSRVD_RESOURCE_TYPE, resource->types);
            VERIFY_CBOR_SUCCESS(TAG, err,
                                "Failed adding resourceType tag/value to links map");

            // Interface Types
            err |= OCStringLLJoin(&linkMap, OC_RSRVD_INTERFACE, resource->interfaces);
            VERIFY_CBOR_SUCCESS(TAG, err,
                                "Failed adding interfaces tag/value to links map");

            // Policy
            CborEncoder policyMap;
            err |= cbor_encode_text_string(&linkMap, OC_RSRVD_POLICY,
                                           sizeof(OC_RSRVD_POLICY) - 1);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy tag to links map");
            err |= cbor_encoder_create_map(&linkMap, &policyMap, CborIndefiniteLength);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding policy map to links map");

            // Bitmap
            err |=  cbor_encode_text_string(&policyMap, OC_RSRVD_BITMAP,
                                            sizeof(OC_RSRVD_BITMAP) - 1);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap tag to policy map");
            err |= cbor_encode_uint(&policyMap, resource->bitmap);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding bitmap value to policy map");

            err |= cbor_encoder_close_container(&linkMap, &policyMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing policy map");

            // Endpoints
            size_t epsCount = OCEndpointPayloadGetEndpointCount(resource->eps);

            // Embed Endpoints in discovery response when any endpoint exist on the resource.
            if (epsCount > 0)
            {
                CborEncoder epsArray;
                err |= cbor_encode_text_string(&linkMap, OC_RSRVD_ENDPOINTS,
                                               sizeof(OC_RSRVD_ENDPOINTS) - 1);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting endpoints array tag");

                err |= cbor_encoder_create_array(&linkMap, &epsArray, epsCount);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting endpoints array");

                for (size_t j = 0; j < epsCount; ++j)
                {
                    CborEncoder endpointMap;
                    OCEndpointPayload* endpoint = OCEndpointPayloadGetEndpoint(resource->eps, j);
                    VERIFY_PARAM_NON_NULL(TAG, endpoint, "Failed retrieving endpoint");

                    char* endpointStr = OCCreateEndpointString(endpoint);
                    VERIFY_PARAM_NON_NULL(TAG, endpointStr, "Failed creating endpoint string");

                    err |= cbor_encoder_create_map(&epsArray, &endpointMap, EP_MAP_LEN);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating endpoint map");

                    err |= AddTextStringToMap(&endpointMap, OC_RSRVD_ENDPOINT,
                                              sizeof(OC_RSRVD_ENDPOINT) - 1, endpointStr);
                    OICFree(endpointStr);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding endpoint to endpoint map");

                    err |= cbor_encode_text_string(&endpointMap, OC_RSRVD_PRIORITY,
                                                   sizeof(OC_RSRVD_PRIORITY) - 1);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding priority tag to endpoint map");
                    err |= cbor_encode_uint(&endpointMap, endpoint->pri);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding priority value to endpoint map");

                    err |= cbor_encoder_close_container(&epsArray, &endpointMap);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing endpoint map");
                }

                err |= cbor_encoder_close_container(&linkMap, &epsArray);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing endpoints map");
            }

            // Finished encoding a resource, close the map.
            err |= cbor_encoder_close_container(&linkArray, &linkMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing link map");
        }

        payload = payload->next;
    }

    if (isBaseline)
    {
        // Close the link array instead the root map.
        err |= cbor_encoder_close_container(&rootMap, &linkArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");

        // Close root map inside the root array.
        err |= cbor_encoder_close_container(&rootArray, &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");

        // Close the final root array.
        err |= cbor_encoder_close_container(&encoder, &rootArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");
    }
    else
    {
        // Close the final root array.
        err |= cbor_encoder_close_container(&encoder, &linkArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");
    }

exit:
    return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertDiscoveryPayload(OCDiscoveryPayload *payload, OCPayloadFormat format,
                                         uint8_t *outPayload, size_t *size)
{
    if (OC_FORMAT_VND_OCF_CBOR == format)
    {
        return OCConvertDiscoveryPayloadVndOcfCbor(payload, outPayload, size);
    }
    else
    {
        return OCConvertDiscoveryPayloadCbor(payload, outPayload, size);
    }
}

static int64_t OCConvertArrayItem(CborEncoder *array, const OCRepPayloadValueArray *valArray,
        size_t index)
{
    int64_t err = CborNoError;
    switch (valArray->type)
    {
        case OCREP_PROP_NULL:
            OIC_LOG(ERROR, TAG, "ConvertArray Invalid NULL");
            err = CborUnknownError;
            break;
        case OCREP_PROP_INT:
            if (valArray->iArray != 0)
            {
                err |= cbor_encode_int(array, valArray->iArray[index]);
            }
            break;
        case OCREP_PROP_DOUBLE:
            if (valArray->dArray != 0)
            {
                err |= cbor_encode_double(array, valArray->dArray[index]);
            }
            break;
        case OCREP_PROP_BOOL:
            if (valArray->bArray != 0)
            {
                err |= cbor_encode_boolean(array, valArray->bArray[index]);
            }
            break;
        case OCREP_PROP_STRING:
            if (valArray->strArray != 0)
            {
                err |= (!valArray->strArray[index]) ? cbor_encode_null(array) : cbor_encode_text_string(array,
                    valArray->strArray[index], strlen(valArray->strArray[index]));
            }
            break;
        case OCREP_PROP_BYTE_STRING:
            err |= (!valArray->ocByteStrArray[index].len) ? cbor_encode_null(array) : cbor_encode_byte_string(array,
                valArray->ocByteStrArray[index].bytes, valArray->ocByteStrArray[index].len);
            break;
        case OCREP_PROP_OBJECT:
            if (valArray->objArray != 0)
            {
                err |= (!valArray->objArray[index]) ? cbor_encode_null(array): OCConvertRepMap(array,
                        valArray->objArray[index]);
            }
            break;
        case OCREP_PROP_ARRAY:
            OIC_LOG(ERROR, TAG, "ConvertArray Invalid child array");
            err = CborUnknownError;
            break;
    }

    return err;
}

static int64_t OCConvertArray(CborEncoder *parent, const OCRepPayloadValueArray *valArray)
{
    int64_t err = CborNoError;
    CborEncoder array;
    err |= cbor_encoder_create_array(parent, &array, valArray->dimensions[0]);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array");
    for (size_t i = 0; i < valArray->dimensions[0]; ++i)
    {
        if (0 != valArray->dimensions[1])
        {
            CborEncoder array2;
            err |= cbor_encoder_create_array(&array, &array2, valArray->dimensions[1]);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array2");

            for (size_t j = 0; j < valArray->dimensions[1]; ++j)
            {
                if (0 != valArray->dimensions[2])
                {
                    CborEncoder array3;
                    err |= cbor_encoder_create_array(&array2, &array3, valArray->dimensions[2]);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep array3");

                    for(size_t k = 0; k < valArray->dimensions[2]; ++k)
                    {
                        err |= OCConvertArrayItem(&array3, valArray,
                                j * valArray->dimensions[2] +
                                i * valArray->dimensions[2] * valArray->dimensions[1] +
                                k);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array3 value");
                    }
                    err |= cbor_encoder_close_container(&array2, &array3);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array3");
                }
                else
                {
                    err |= OCConvertArrayItem(&array2, valArray, i * valArray->dimensions[1] + j);
                    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array2 value");
                }
            }
            err |= cbor_encoder_close_container(&array, &array2);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array2");
        }
        else
        {
            err |= OCConvertArrayItem(&array, valArray, i);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep array value");
        }
    }
    err |= cbor_encoder_close_container(parent, &array);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep array");

exit:
    return err;
}

static int64_t OCConvertRepMap(CborEncoder *map, const OCRepPayload *payload)
{
    int64_t err = CborNoError;
    CborEncoder encoder;
    OCRepPayloadValue *value = NULL;
    size_t arrayLength = 0;

    // Encode payload as an array when value names are consecutive
    // non-negative integers.  Otherwise encode as a map.
    arrayLength = 0;
    for (value = payload->values; value; value = value->next)
    {
        char *endp;
        long i = strtol(value->name, &endp, 0);
        if (*endp != '\0' || i < 0 || arrayLength != (size_t)i)
        {
            break;
        }
        ++arrayLength;
    }

    if (value)
    {
        err |= cbor_encoder_create_map(map, &encoder, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep map");
        err |= OCConvertSingleRepPayload(&encoder, payload);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed converting single rep payload");
        err |= cbor_encoder_close_container(map, &encoder);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep map");
    }
    else
    {
        err |= cbor_encoder_create_array(map, &encoder, arrayLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating rep map");
        for (value = payload->values; value; value = value->next)
        {
            err |= OCConvertSingleRepPayloadValue(&encoder, value);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed converting single rep value");
        }
        err |= cbor_encoder_close_container(map, &encoder);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing rep map");
    }

exit:
    return err;
}

static int64_t OCConvertSingleRepPayloadValue(CborEncoder *parent, const OCRepPayloadValue *value)
{
    int64_t err = CborNoError;
    switch (value->type)
    {
        case OCREP_PROP_NULL:
            err = cbor_encode_null(parent);
            break;
        case OCREP_PROP_INT:
            err = cbor_encode_int(parent, value->i);
            break;
        case OCREP_PROP_DOUBLE:
            err = cbor_encode_double(parent, value->d);
            break;
        case OCREP_PROP_BOOL:
            err = cbor_encode_boolean(parent, value->b);
            break;
        case OCREP_PROP_STRING:
            err = cbor_encode_text_string(parent, value->str, strlen(value->str));
            break;
        case OCREP_PROP_BYTE_STRING:
            err = cbor_encode_byte_string(parent, value->ocByteStr.bytes, value->ocByteStr.len);
            break;
        case OCREP_PROP_OBJECT:
            err = OCConvertRepMap(parent, value->obj);
            break;
        case OCREP_PROP_ARRAY:
            err = OCConvertArray(parent, &value->arr);
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "Invalid Prop type: %d", value->type);
            break;
    }
    return err;
}

static int64_t OCConvertSingleRepPayload(CborEncoder *repMap, const OCRepPayload *payload)
{
    int64_t err = CborNoError;
    OCRepPayloadValue *value = payload->values;
    while (value)
    {
        err |= cbor_encode_text_string(repMap, value->name, strlen(value->name));
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding tag name");

        err |= OCConvertSingleRepPayloadValue(repMap, value);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding single rep value");
        value = value->next;
    }

exit:
    return err;
}

static int64_t OCConvertRepPayload(OCRepPayload *payload, uint8_t *outPayload, size_t *size)
{
    CborEncoder encoder;
    int64_t err = CborNoError;

    cbor_encoder_init(&encoder, outPayload, *size, 0);

    size_t arrayCount = 0;
    for (OCRepPayload *temp = payload; temp; temp = temp->next)
    {
        arrayCount++;
    }
    CborEncoder rootArray;
    if (arrayCount > 1)
    {
        err |= cbor_encoder_create_array(&encoder, &rootArray, arrayCount);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep root map");
    }

    while (payload != NULL && (err == CborNoError))
    {
        CborEncoder rootMap;
        err |= cbor_encoder_create_map(((arrayCount == 1)? &encoder: &rootArray),
                                            &rootMap, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating root map");

        // Only in case of collection href is included.
        if (arrayCount > 1 && payload->uri && strlen(payload->uri) > 0)
        {
            OIC_LOG(INFO, TAG, "Payload has uri");
            err |= cbor_encode_text_string(&rootMap, OC_RSRVD_HREF, strlen(OC_RSRVD_HREF));
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep href tag");
            err |= cbor_encode_text_string(&rootMap, payload->uri, strlen(payload->uri));
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding rep href value");
        }
        if (payload->types)
        {
            OIC_LOG(INFO, TAG, "Payload has types");
            err |= OCStringLLJoin(&rootMap, OC_RSRVD_RESOURCE_TYPE, payload->types);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type.");
        }
        if (payload->interfaces)
        {
            OIC_LOG(INFO, TAG, "Payload has interfaces");
            err |= OCStringLLJoin(&rootMap, OC_RSRVD_INTERFACE, payload->interfaces);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding platform interface type.");
        }

        err |= OCConvertSingleRepPayload(&rootMap, payload);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting rep payload");

        // Close main array
        err |= cbor_encoder_close_container(((arrayCount == 1) ? &encoder: &rootArray),
                &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root map");
        payload = payload->next;
    }
    if (arrayCount > 1)
    {
        err |= cbor_encoder_close_container(&encoder, &rootArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing root array");
    }

exit:
    return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertPresencePayload(OCPresencePayload *payload, uint8_t *outPayload,
        size_t *size)
{
    int64_t err = CborNoError;
    CborEncoder encoder;

    cbor_encoder_init(&encoder, outPayload, *size, 0);
    CborEncoder map;
    err |= cbor_encoder_create_map(&encoder, &map, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed creating presence map");

    // Sequence Number
    err |= cbor_encode_text_string(&map, OC_RSRVD_NONCE, sizeof(OC_RSRVD_NONCE) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding nonce tag to presence map");
    err |= cbor_encode_uint(&map, payload->sequenceNumber);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding nonce value to presence map");

    // Max Age
    err |= cbor_encode_text_string(&map, OC_RSRVD_TTL, sizeof(OC_RSRVD_TTL) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding ttl tag to presence map");
    err |= cbor_encode_uint(&map, payload->maxAge);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding ttl value to presence map");

    // Trigger
    err |= cbor_encode_text_string(&map, OC_RSRVD_TRIGGER, sizeof(OC_RSRVD_TRIGGER) - 1);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding trigger tag to presence map");
    err |= cbor_encode_simple_value(&map, payload->trigger);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding trigger value to presence map");

    // Resource type name
    if (payload->trigger != OC_PRESENCE_TRIGGER_DELETE)
    {
        err |= ConditionalAddTextStringToMap(&map, OC_RSRVD_RESOURCE_TYPE,
                sizeof(OC_RSRVD_RESOURCE_TYPE) - 1, payload->resourceType);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding resource type to presence map");
    }

    // Close Map
    err |= cbor_encoder_close_container(&encoder, &map);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed closing presence map");

exit:
    return checkError(err, &encoder, outPayload, size);
}

static int64_t OCConvertDiagnosticPayload(OCDiagnosticPayload *payload, uint8_t *outPayload,
        size_t *size)
{
    int64_t err = CborNoError;
    CborEncoder encoder;

    cbor_encoder_init(&encoder, outPayload, *size, 0);

    // Message
    err |= cbor_encode_text_string(&encoder, payload->message, strlen(payload->message));
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed adding message");

exit:
    return checkError(err, &encoder, outPayload, size);
}

static int64_t AddTextStringToMap(CborEncoder* map, const char* key, size_t keylen,
        const char* value)
{
    if (!key || !value)
    {
        return CborErrorInvalidUtf8TextString;
    }
    int64_t err = cbor_encode_text_string(map, key, keylen);
    if (CborNoError != err)
    {
        return err;
    }
    return cbor_encode_text_string(map, value, strlen(value));
}

static int64_t ConditionalAddTextStringToMap(CborEncoder* map, const char* key, size_t keylen,
        const char* value)
{
    return value ? AddTextStringToMap(map, key, keylen, value) : 0;
}
