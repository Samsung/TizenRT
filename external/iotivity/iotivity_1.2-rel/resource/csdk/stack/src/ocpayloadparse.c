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

// Defining _POSIX_C_SOURCE macro with 200112L (or greater) as value
// causes header files to expose definitions
// corresponding to the POSIX.1-2001 base
// specification (excluding the XSI extension).
// For POSIX.1-2001 base specification,
// Refer http://pubs.opengroup.org/onlinepubs/009695399/
// Required for strok_r
#define _POSIX_C_SOURCE 200112L

#include <string.h>
#include <stdlib.h>
#include "oic_string.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "ocstackinternal.h"
#include "payload_logging.h"
#include "platform_features.h"

#define TAG "OIC_RI_PAYLOADPARSE"

static OCStackResult OCParseDiscoveryPayload(OCPayload **outPayload, CborValue *arrayVal);
static CborError OCParseSingleRepPayload(OCRepPayload **outPayload, CborValue *repParent, bool isRoot);
static OCStackResult OCParseRepPayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParsePresencePayload(OCPayload **outPayload, CborValue *arrayVal);
static OCStackResult OCParseSecurityPayload(OCPayload **outPayload, const uint8_t *payload, size_t size);

OCStackResult OCParsePayload(OCPayload **outPayload, OCPayloadType payloadType,
        const uint8_t *payload, size_t payloadSize)
{
    OCStackResult result = OC_STACK_MALFORMED_RESPONSE;
    CborError err;

    VERIFY_PARAM_NON_NULL(TAG, outPayload, "Conversion of outPayload failed");
    VERIFY_PARAM_NON_NULL(TAG, payload, "Invalid cbor payload value");

    OIC_LOG_V(INFO, TAG, "CBOR Parsing size: %zu of Payload Type: %d, Payload:",
            payloadSize, payloadType);

    CborParser parser;
    CborValue rootValue;

    err = cbor_parser_init(payload, payloadSize, 0, &parser, &rootValue);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed initializing init value")

    switch(payloadType)
    {
        case PAYLOAD_TYPE_DISCOVERY:
            result = OCParseDiscoveryPayload(outPayload, &rootValue);
            break;
        case PAYLOAD_TYPE_REPRESENTATION:
            result = OCParseRepPayload(outPayload, &rootValue);
            break;
        case PAYLOAD_TYPE_PRESENCE:
            result = OCParsePresencePayload(outPayload, &rootValue);
            break;
        case PAYLOAD_TYPE_SECURITY:
            result = OCParseSecurityPayload(outPayload, payload, payloadSize);
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "ParsePayload Type default: %d", payloadType);
            result = OC_STACK_INVALID_PARAM;
            break;
    }

    OIC_LOG_V(INFO, TAG, "Finished parse payload, result is %d", result);

exit:
    return result;
}

static OCStackResult OCParseSecurityPayload(OCPayload** outPayload, const uint8_t *payload,
        size_t size)
{
    if (size > 0)
    {
        *outPayload = (OCPayload *)OCSecurityPayloadCreate(payload, size);
    }
    else
    {
        *outPayload = NULL;
    }
    return OC_STACK_OK;
}

static char* InPlaceStringTrim(char* str)
{
    while (str[0] == ' ')
    {
        ++str;
    }

    size_t lastchar = strlen(str);

    while (str[lastchar] == ' ')
    {
        str[lastchar] = '\0';
        --lastchar;
    }

    return str;
}

static CborError OCParseStringLL(CborValue *map, char *type, OCStringLL **resource)
{
    CborValue val;
    CborError err = cbor_value_map_find_value(map, type, &val);
    VERIFY_CBOR_SUCCESS(TAG, err, "to find StringLL TAG");

    if (cbor_value_is_array(&val))
    {
        CborValue txtStr;
        err = cbor_value_enter_container(&val, &txtStr);
        VERIFY_CBOR_SUCCESS(TAG, err, "to enter container");
        while (cbor_value_is_text_string(&txtStr))
        {
            size_t len = 0;
            char *input = NULL;
            err = cbor_value_dup_text_string(&txtStr, &input, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, err, "to find StringLL value.");
            if (input)
            {
                char *savePtr = NULL;
                char *curPtr = strtok_r(input, " ", &savePtr);
                while (curPtr)
                {
                    char *trimmed = InPlaceStringTrim(curPtr);
                    if (trimmed && strlen(trimmed) > 0)
                    {
                        if (!OCResourcePayloadAddStringLL(resource, trimmed))
                        {
                            return CborErrorOutOfMemory;
                        }
                    }
                    curPtr = strtok_r(NULL, " ", &savePtr);
                }
                OICFree(input);
            }
            if (cbor_value_is_text_string(&txtStr))
            {
                err = cbor_value_advance(&txtStr);
                VERIFY_CBOR_SUCCESS(TAG, err, "to advance string value");
            }
        }
    }
exit:
    return err;
}

static OCStackResult OCParseDiscoveryPayload(OCPayload **outPayload, CborValue *rootValue)
{
    OCStackResult ret = OC_STACK_INVALID_PARAM;
    OCResourcePayload *resource = NULL;
    OCDiscoveryPayload *temp = NULL;
    OCDiscoveryPayload *rootPayload = NULL;
    OCDiscoveryPayload *curPayload = NULL;
    size_t len = 0;
    CborError err = CborNoError;
    *outPayload = NULL;

    VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
    VERIFY_PARAM_NON_NULL(TAG, rootValue, "Invalid Parameter rootValue");
    if (cbor_value_is_array(rootValue))
    {
        // Root value is already inside the main root array
        CborValue rootMap;

        // Enter the main root map
        ret = OC_STACK_MALFORMED_RESPONSE;
        err = cbor_value_enter_container(rootValue, &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "to enter root map container");
        while (cbor_value_is_map(&rootMap))
        {
            ret = OC_STACK_NO_MEMORY;
            temp = OCDiscoveryPayloadCreate();
            VERIFY_PARAM_NON_NULL(TAG, temp, "Failed error initializing discovery payload");

            // Look for DI
            CborValue curVal;
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_DEVICE_ID, &curVal);
            VERIFY_CBOR_SUCCESS(TAG, err, "to find device id tag");
            if (cbor_value_is_valid(&curVal))
            {
                if (cbor_value_is_byte_string(&curVal))
                {
                    err = cbor_value_dup_byte_string(&curVal, (uint8_t **)&(temp->sid), &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, err, "to copy device id value");
                }
                else if (cbor_value_is_text_string(&curVal))
                {
                    err = cbor_value_dup_text_string(&curVal, &(temp->sid), &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, err, "to copy device id value");
                }
            }

            // BaseURI - Not a mandatory field
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_BASE_URI, &curVal);
            VERIFY_CBOR_SUCCESS(TAG, err, "to find uri tag");
            if (cbor_value_is_text_string(&curVal))
            {
                err = cbor_value_dup_text_string(&curVal, &(temp->baseURI), &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find base uri value");
            }

            // RT - Not a mandatory field
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_RESOURCE_TYPE, &curVal);
            if (cbor_value_is_valid(&curVal))
            {
                err = OCParseStringLL(&rootMap, OC_RSRVD_RESOURCE_TYPE, &temp->type);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find resource type");
            }

            // IF - Not a mandatory field
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_INTERFACE, &curVal);
            if (cbor_value_is_valid(&curVal))
            {
                err =  OCParseStringLL(&rootMap, OC_RSRVD_INTERFACE, &temp->iface);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find interface");
            }

            // Name - Not a mandatory field
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_DEVICE_NAME, &curVal);
            if (cbor_value_is_text_string(&curVal))
            {
                err = cbor_value_dup_text_string(&curVal, &temp->name, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find device name");
            }

            // Look for Links which will have an array as the value
            CborValue linkMap;
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_LINKS, &linkMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "to find links tag");

            // Enter the links array and start iterating through the array processing
            // each resource which shows up as a map.
            CborValue resourceMap;
            err = cbor_value_enter_container(&linkMap, &resourceMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "to enter link map");

            while (cbor_value_is_map(&resourceMap))
            {
                int bitmap;

                resource = (OCResourcePayload *)OICCalloc(1, sizeof(OCResourcePayload));
                VERIFY_PARAM_NON_NULL(TAG, resource, "Failed allocating resource payload");

                // Uri
                err = cbor_value_map_find_value(&resourceMap, OC_RSRVD_HREF, &curVal);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find href tag");
                err = cbor_value_dup_text_string(&curVal, &(resource->uri), &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find href value");

                // ResourceTypes
                err =  OCParseStringLL(&resourceMap, OC_RSRVD_RESOURCE_TYPE, &resource->types);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find resource type tag/value");

                // Interface Types
                err =  OCParseStringLL(&resourceMap, OC_RSRVD_INTERFACE, &resource->interfaces);
                if (CborNoError != err)
                {
                    if (!OCResourcePayloadAddStringLL(&resource->interfaces, OC_RSRVD_INTERFACE_LL))
                    {
                        OIC_LOG(ERROR, TAG, "Failed to add string to StringLL");
                        goto exit;
                    }
                }

                // Policy
                CborValue policyMap;
                err = cbor_value_map_find_value(&resourceMap, OC_RSRVD_POLICY, &policyMap);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find policy tag");

                // Bitmap
                err = cbor_value_map_find_value(&policyMap, OC_RSRVD_BITMAP, &curVal);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find bitmap tag");
                err = cbor_value_get_int(&curVal, &bitmap);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find bitmap value");
                resource->bitmap = (uint8_t)bitmap;

                // Secure Flag
                err = cbor_value_map_find_value(&policyMap, OC_RSRVD_SECURE, &curVal);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find secure tag");
                if (cbor_value_is_boolean(&curVal))
                {
                    err = cbor_value_get_boolean(&curVal, &(resource->secure));
                    VERIFY_CBOR_SUCCESS(TAG, err, "to find secure value");
                }

                // Port
                err = cbor_value_map_find_value(&policyMap, OC_RSRVD_HOSTING_PORT, &curVal);
                VERIFY_CBOR_SUCCESS(TAG, err, "to find port tag");
                if (cbor_value_is_integer(&curVal))
                {
                    int port;

                    err = cbor_value_get_int(&curVal, &port);
                    VERIFY_CBOR_SUCCESS(TAG, err, "to find port value");
                    resource->port = (uint16_t)port;
                }

#ifdef TCP_ADAPTER
                // TCP Port
                err = cbor_value_map_find_value(&policyMap, OC_RSRVD_TCP_PORT, &curVal);
                if (cbor_value_is_integer(&curVal))
                {
                    int tcpPort;

                    err = cbor_value_get_int(&curVal, &tcpPort);
                    VERIFY_CBOR_SUCCESS(TAG, err, "to find tcp port value");
                    resource->tcpPort = (uint16_t)tcpPort;
                }

#ifdef __WITH_TLS__
                // TLS Port
                err = cbor_value_map_find_value(&policyMap, OC_RSRVD_TLS_PORT, &curVal);
                if (cbor_value_is_integer(&curVal))
                {
                    int tlsPort;

                    err = cbor_value_get_int(&curVal, &tlsPort);
                    VERIFY_CBOR_SUCCESS(TAG, err, "to find tcp tls port value");
                    resource->tcpPort = (uint16_t)tlsPort;
                }
#endif
#endif

                err = cbor_value_advance(&resourceMap);
                VERIFY_CBOR_SUCCESS(TAG, err, "to advance resource map");

                OCDiscoveryPayloadAddNewResource(temp, resource);
            }

            err = cbor_value_leave_container(&linkMap, &resourceMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "to leave resource map");

            err = cbor_value_advance(&rootMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "to advance root map");

            if(rootPayload == NULL)
            {
                rootPayload = temp;
                curPayload = temp;
            }
            else
            {
                curPayload->next = temp;
                curPayload = curPayload->next;
            }
        }

        err = cbor_value_leave_container(rootValue, &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "to leave root map");

    }
    else
    {
        OIC_LOG(ERROR, TAG, "Malformed packet ");
        goto exit;
    }

    *outPayload = (OCPayload *)rootPayload;
    OIC_LOG_PAYLOAD(DEBUG, *outPayload);

    return OC_STACK_OK;

exit:
    OCDiscoveryResourceDestroy(resource);
    OCDiscoveryPayloadDestroy(rootPayload);
    return ret;
}

static OCRepPayloadPropType DecodeCborType(CborType type)
{
    switch (type)
    {
        case CborNullType:
            return OCREP_PROP_NULL;
        case CborIntegerType:
            return OCREP_PROP_INT;
        case CborDoubleType:
        case CborFloatType:
            return OCREP_PROP_DOUBLE;
        case CborBooleanType:
            return OCREP_PROP_BOOL;
        case CborTextStringType:
            return OCREP_PROP_STRING;
        case CborByteStringType:
            return OCREP_PROP_BYTE_STRING;
        case CborMapType:
            return OCREP_PROP_OBJECT;
        case CborArrayType:
            return OCREP_PROP_ARRAY;
        default:
            return OCREP_PROP_NULL;
    }
}
static CborError OCParseArrayFindDimensionsAndType(const CborValue *parent,
        size_t dimensions[MAX_REP_ARRAY_DEPTH], OCRepPayloadPropType *type)
{
    CborValue insideArray;
    *type = OCREP_PROP_NULL;
    dimensions[0] = dimensions[1] = dimensions[2] = 0;

    CborError err = cbor_value_enter_container(parent, &insideArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed to enter container");

    while (cbor_value_is_valid(&insideArray))
    {
        OCRepPayloadPropType tempType = DecodeCborType(cbor_value_get_type(&insideArray));

        if (tempType == OCREP_PROP_ARRAY)
        {
            size_t subdim[MAX_REP_ARRAY_DEPTH];
            tempType = OCREP_PROP_NULL;
            err = OCParseArrayFindDimensionsAndType(&insideArray, subdim, &tempType);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed to parse array");

            if (subdim[2] != 0)
            {
                OIC_LOG(ERROR, TAG, "Parse array helper, sub-array too deep");
            }

            dimensions[1] = dimensions[1] >= subdim[0] ? dimensions[1] : subdim[0];
            dimensions[2] = dimensions[2] >= subdim[1] ? dimensions[2] : subdim[1];

            if (*type != OCREP_PROP_NULL && tempType != OCREP_PROP_NULL && *type != tempType)
            {
                OIC_LOG(ERROR, TAG, "Array parse failed, mixed arrays not allowed (subtype)");
                return CborUnknownError;
            }
            else if (*type == OCREP_PROP_NULL)
            {
                // We don't know the type of this array yet, so the assignment is OK
                *type = tempType;
            }
        }
        else if (*type == OCREP_PROP_NULL)
        {
            // We don't know the type of this array yet, so the assignment is OK
            *type = tempType;
        }
        // tempType is allowed to be NULL, since it might now know the answer yet
        else if (tempType != OCREP_PROP_NULL && *type != tempType)
        {
            // this is an invalid situation!
            OIC_LOG(ERROR, TAG, "Array parse failed, mixed arrays not allowed");
            return CborUnknownError;
        }

        ++dimensions[0];
        err = cbor_value_advance(&insideArray);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed to advance array");
    }

exit:
    return err;
}

static size_t getAllocSize(OCRepPayloadPropType type)
{
    switch (type)
    {
        case OCREP_PROP_INT:
            return sizeof (int64_t);
        case OCREP_PROP_DOUBLE:
            return sizeof (double);
        case OCREP_PROP_BOOL:
            return sizeof (bool);
        case OCREP_PROP_STRING:
            return sizeof (char*);
        case OCREP_PROP_BYTE_STRING:
            return sizeof (OCByteString);
        case OCREP_PROP_OBJECT:
            return sizeof (OCRepPayload*);
        default:
            return 0;
    }
}

static size_t arrayStep(size_t dimensions[MAX_REP_ARRAY_DEPTH], size_t elementNum)
{
    return
        (dimensions[1] == 0 ? 1 : dimensions[1]) *
        (dimensions[2] == 0 ? 1 : dimensions[2]) *
        elementNum;
}

static CborError OCParseArrayFillArray(const CborValue *parent,
        size_t dimensions[MAX_REP_ARRAY_DEPTH], OCRepPayloadPropType type, void *targetArray)
{
    CborValue insideArray;

    size_t i = 0;
    char *tempStr = NULL;
    OCByteString ocByteStr = { .bytes = NULL, .len = 0};
    size_t tempLen = 0;
    OCRepPayload *tempPl = NULL;

    size_t newdim[MAX_REP_ARRAY_DEPTH];
    newdim[0] = dimensions[1];
    newdim[1] = dimensions[2];
    newdim[2] = 0;

    CborError err = cbor_value_enter_container(parent, &insideArray);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed to enter container");

    while (!err && i < dimensions[0] && cbor_value_is_valid(&insideArray))
    {
        bool noAdvance = false;
        if (cbor_value_get_type(&insideArray) != CborNullType)
        {
            switch (type)
            {
                case OCREP_PROP_INT:
                    if (dimensions[1] == 0)
                    {
                        err = cbor_value_get_int64(&insideArray, &(((int64_t*)targetArray)[i]));
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                            &(((int64_t*)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                case OCREP_PROP_DOUBLE:
                    if (dimensions[1] == 0)
                    {
                        double *d = &(((double*)targetArray)[i]);
                        if (cbor_value_get_type(&insideArray) == CborDoubleType)
                        {
                            err = cbor_value_get_double(&insideArray, d);
                        }
                        else
                        {
                            /* must be float */
                            float f;
                            err = cbor_value_get_float(&insideArray, &f);
                            if (!err)
                                *d = f;
                        }
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                            &(((double*)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                case OCREP_PROP_BOOL:
                    if (dimensions[1] == 0)
                    {
                        err = cbor_value_get_boolean(&insideArray, &(((bool*)targetArray)[i]));
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                            &(((bool*)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                case OCREP_PROP_STRING:
                    if (dimensions[1] == 0)
                    {
                        err = cbor_value_dup_text_string(&insideArray, &tempStr, &tempLen, NULL);
                        ((char**)targetArray)[i] = tempStr;
                        tempStr = NULL;
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                            &(((char**)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                case OCREP_PROP_BYTE_STRING:
                    if (dimensions[1] == 0)
                    {
                        err = cbor_value_dup_byte_string(&insideArray, &(ocByteStr.bytes),
                                &(ocByteStr.len), NULL);
                        ((OCByteString*)targetArray)[i] = ocByteStr;
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                                &(((OCByteString*)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                case OCREP_PROP_OBJECT:
                    if (dimensions[1] == 0)
                    {
                        err = OCParseSingleRepPayload(&tempPl, &insideArray, false);
                        ((OCRepPayload**)targetArray)[i] = tempPl;
                        tempPl = NULL;
                        noAdvance = true;
                    }
                    else
                    {
                        err = OCParseArrayFillArray(&insideArray, newdim, type,
                            &(((OCRepPayload**)targetArray)[arrayStep(dimensions, i)]));
                    }
                    break;
                default:
                    OIC_LOG(ERROR, TAG, "Invalid Array type in Parse Array");
                    err = CborErrorUnknownType;
                    break;
            }
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting repPayload");
        }
        ++i;
        if (!noAdvance && cbor_value_is_valid(&insideArray))
        {
            err = cbor_value_advance(&insideArray);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed advnce insideArray");
        }
    }

exit:
    return err;
}

static CborError OCParseArray(OCRepPayload *out, const char *name, CborValue *container)
{
    void *arr = NULL;

    OCRepPayloadPropType type = OCREP_PROP_NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0 };

    size_t dimTotal = 0;
    size_t allocSize = 0;
    bool res = true;
    CborError err = OCParseArrayFindDimensionsAndType(container, dimensions, &type);
    VERIFY_CBOR_SUCCESS(TAG, err, "Array details weren't clear");

    if (type == OCREP_PROP_NULL)
    {
        res = OCRepPayloadSetNull(out, name);
        err = (CborError) !res;
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting value");
        container = container + 1;
        return err;
    }

    dimTotal = calcDimTotal(dimensions);
    allocSize = getAllocSize(type);
    arr = OICCalloc(dimTotal, allocSize);
    VERIFY_PARAM_NON_NULL(TAG, arr, "Array Parse allocation failed");

    res = OCParseArrayFillArray(container, dimensions, type, arr);
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed parse array");

    switch (type)
    {
        case OCREP_PROP_INT:
            res = OCRepPayloadSetIntArrayAsOwner(out, name, (int64_t *)arr, dimensions);
            break;
        case OCREP_PROP_DOUBLE:
            res = OCRepPayloadSetDoubleArrayAsOwner(out, name, (double *)arr, dimensions);
            break;
        case OCREP_PROP_BOOL:
            res = OCRepPayloadSetBoolArrayAsOwner(out, name, (bool *)arr, dimensions);
            break;
        case OCREP_PROP_STRING:
            res = OCRepPayloadSetStringArrayAsOwner(out, name, (char **)arr, dimensions);
            break;
        case OCREP_PROP_BYTE_STRING:
            res = OCRepPayloadSetByteStringArrayAsOwner(out, name, (OCByteString *)arr, dimensions);
            break;
        case OCREP_PROP_OBJECT:
            res = OCRepPayloadSetPropObjectArrayAsOwner(out, name, (OCRepPayload**)arr, dimensions);
            break;
        default:
            OIC_LOG(ERROR, TAG, "Invalid Array type in Parse Array");
            break;
    }
    err = (CborError) !res;
    VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting array parameter");
    return CborNoError;
exit:
    if (type == OCREP_PROP_STRING)
    {
        for(size_t i = 0; i < dimTotal; ++i)
        {
            OICFree(((char**)arr)[i]);
        }
    }
    if (type == OCREP_PROP_BYTE_STRING)
    {
        for(size_t i = 0; i < dimTotal; ++i)
        {
            OICFree(((OCByteString*)arr)[i].bytes);
        }
    }
    if (type == OCREP_PROP_OBJECT)
    {
        for(size_t i = 0; i < dimTotal; ++i)
        {
            OCRepPayloadDestroy(((OCRepPayload**)arr)[i]);
        }
    }
    OICFree(arr);
    return err;
}

static CborError OCParseSingleRepPayload(OCRepPayload **outPayload, CborValue *objMap, bool isRoot)
{
    CborError err = CborUnknownError;
    char *name = NULL;
    bool res = false;
    VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
    VERIFY_PARAM_NON_NULL(TAG, objMap, "Invalid Parameter objMap");

    if (cbor_value_is_map(objMap))
    {
        if (!*outPayload)
        {
            *outPayload = OCRepPayloadCreate();
            if (!*outPayload)
            {
                return CborErrorOutOfMemory;
            }
        }

        OCRepPayload *curPayload = *outPayload;

        size_t len = 0;
        CborValue repMap;
        err = cbor_value_enter_container(objMap, &repMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed entering repMap");

        while (!err && cbor_value_is_valid(&repMap))
        {
            if (cbor_value_is_text_string(&repMap))
            {
                err = cbor_value_dup_text_string(&repMap, &name, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding tag name in the map");
                err = cbor_value_advance(&repMap);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed advancing rootMap");
                if (name &&
                    isRoot &&
                    ((0 == strcmp(OC_RSRVD_HREF, name)) ||
                     (0 == strcmp(OC_RSRVD_RESOURCE_TYPE, name)) ||
                    (0 == strcmp(OC_RSRVD_INTERFACE, name))))
                {
                    err = cbor_value_advance(&repMap);
                    OICFree(name);
                    name = NULL;
                    continue;
                }
            }
            CborType type = cbor_value_get_type(&repMap);
            switch (type)
            {
                case CborNullType:
                    res = OCRepPayloadSetNull(curPayload, name);
                    break;
                case CborIntegerType:
                    {
                        int64_t intval = 0;
                        err = cbor_value_get_int64(&repMap, &intval);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting int value");
                        res = OCRepPayloadSetPropInt(curPayload, name, intval);
                    }
                    break;
                case CborDoubleType:
                    {
                        double doubleval = 0;
                        err = cbor_value_get_double(&repMap, &doubleval);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting double value");
                        res = OCRepPayloadSetPropDouble(curPayload, name, doubleval);
                    }
                    break;
                case CborBooleanType:
                    {
                        bool boolval = false;
                        err = cbor_value_get_boolean(&repMap, &boolval);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting boolean value");
                        res = OCRepPayloadSetPropBool(curPayload, name, boolval);
                    }
                    break;
                case CborTextStringType:
                    {
                        char *strval = NULL;
                        err = cbor_value_dup_text_string(&repMap, &strval, &len, NULL);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting string value");
                        res = OCRepPayloadSetPropStringAsOwner(curPayload, name, strval);
                    }
                    break;
                case CborByteStringType:
                    {
                        uint8_t* bytestrval = NULL;
                        err = cbor_value_dup_byte_string(&repMap, &bytestrval, &len, NULL);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed getting byte string value");
                        OCByteString tmp = {.bytes = bytestrval, .len = len};
                        res = OCRepPayloadSetPropByteStringAsOwner(curPayload, name, &tmp);
                    }
                    break;
                case CborMapType:
                    {
                        OCRepPayload *pl = NULL;
                        err = OCParseSingleRepPayload(&pl, &repMap, false);
                        VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting parse single rep");
                        res = OCRepPayloadSetPropObjectAsOwner(curPayload, name, pl);
                    }
                    break;
                case CborArrayType:
                    err = OCParseArray(curPayload, name, &repMap);
                    break;
                default:
                    OIC_LOG_V(ERROR, TAG, "Parsing rep property, unknown type %d", repMap.type);
                    res = false;
            }
            if (type != CborArrayType)
            {
                err = (CborError) !res;
            }
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed setting value");

            if (type != CborMapType && cbor_value_is_valid(&repMap))
            {
                err = cbor_value_advance(&repMap);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed advance repMap");
            }
            OICFree(name);
            name = NULL;
        }
        if (cbor_value_is_container(objMap))
        {
            err = cbor_value_leave_container(objMap, &repMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed to leave container");
        }
        return err;
    }

exit:
    OICFree(name);
    OCRepPayloadDestroy(*outPayload);
    *outPayload = NULL;
    return err;
}

static OCStackResult OCParseRepPayload(OCPayload **outPayload, CborValue *root)
{
    OCStackResult ret = OC_STACK_INVALID_PARAM;
    CborError err;
    OCRepPayload *temp = NULL;
    OCRepPayload *rootPayload = NULL;
    OCRepPayload *curPayload = NULL;
    CborValue rootMap = *root;
    VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");
    VERIFY_PARAM_NON_NULL(TAG, root, "Invalid Parameter root");

    *outPayload = NULL;
    if (cbor_value_is_array(root))
    {
        err = cbor_value_enter_container(root, &rootMap);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed entering repMap");
    }
    while (cbor_value_is_valid(&rootMap))
    {
        temp = OCRepPayloadCreate();
        ret = OC_STACK_NO_MEMORY;
        VERIFY_PARAM_NON_NULL(TAG, temp, "Failed allocating memory");

        CborValue curVal;
        ret = OC_STACK_MALFORMED_RESPONSE;

        // temporary fix to check for malformed cbor payload
        if (!cbor_value_is_map(&rootMap) && !cbor_value_is_array(&rootMap)){
            goto exit;
        }

        if (cbor_value_is_map(&rootMap))
        {
            err = cbor_value_map_find_value(&rootMap, OC_RSRVD_HREF, &curVal);
            VERIFY_CBOR_SUCCESS(TAG, err, "to find href tag");
            if (cbor_value_is_text_string(&curVal))
            {
                size_t len = 0;
                err = cbor_value_dup_text_string(&curVal, &temp->uri, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find uri");
            }
        }

        // Resource types
        if (cbor_value_is_map(&rootMap))
        {
            if (CborNoError == cbor_value_map_find_value(&rootMap, OC_RSRVD_RESOURCE_TYPE, &curVal))
            {
                err =  OCParseStringLL(&rootMap, OC_RSRVD_RESOURCE_TYPE, &temp->types);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find rt type tag/value");
            }
        }

        // Interface Types
        if (cbor_value_is_map(&rootMap))
        {
            if (CborNoError == cbor_value_map_find_value(&rootMap, OC_RSRVD_INTERFACE, &curVal))
            {
                err =  OCParseStringLL(&rootMap, OC_RSRVD_INTERFACE, &temp->interfaces);
                VERIFY_CBOR_SUCCESS(TAG, err, "Failed to find interfaces tag/value");
            }
        }

        if (cbor_value_is_map(&rootMap))
        {
            err = OCParseSingleRepPayload(&temp, &rootMap, true);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed to parse single rep payload");
        }

        if(rootPayload == NULL)
        {
            rootPayload = temp;
            curPayload = temp;
        }
        else
        {
            curPayload->next = temp;
            curPayload = curPayload->next;
        }

        if (cbor_value_is_array(&rootMap))
        {
            err = cbor_value_advance(&rootMap);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed to advance single rep payload");
        }
    }
    *outPayload = (OCPayload *)rootPayload;
    return OC_STACK_OK;

exit:
    OCRepPayloadDestroy(temp);
    OCRepPayloadDestroy(rootPayload);
    OIC_LOG(ERROR, TAG, "CBOR error in ParseRepPayload");
    return ret;
}

static OCStackResult OCParsePresencePayload(OCPayload **outPayload, CborValue *rootValue)
{
    OCStackResult ret = OC_STACK_INVALID_PARAM;
    OCPresencePayload *payload = NULL;
    VERIFY_PARAM_NON_NULL(TAG, outPayload, "Invalid Parameter outPayload");

    *outPayload = NULL;

    payload = (OCPresencePayload *)OICCalloc(1, sizeof(OCPresencePayload));
    ret = OC_STACK_NO_MEMORY;
    VERIFY_PARAM_NON_NULL(TAG, payload, "Failed allocating presence payload");
    payload->base.type = PAYLOAD_TYPE_PRESENCE;
    ret = OC_STACK_MALFORMED_RESPONSE;

    if (cbor_value_is_map(rootValue))
    {
        CborValue curVal;
        uint64_t temp = 0;
        uint8_t trigger;

        // Sequence Number
        CborError err = cbor_value_map_find_value(rootValue, OC_RSRVD_NONCE, &curVal);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding nonce tag");
        err = cbor_value_get_uint64(&curVal, &temp);
        payload->sequenceNumber = (uint32_t)temp;
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding nonce value");

        // Max Age
        err = cbor_value_map_find_value(rootValue, OC_RSRVD_TTL, &curVal);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding ttl tag");
        temp = 0;
        err = cbor_value_get_uint64(&curVal, &temp);
        payload->maxAge = (uint32_t)temp;
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding ttl value");

        // Trigger
        err = cbor_value_map_find_value(rootValue, OC_RSRVD_TRIGGER, &curVal);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding trigger tag");
        err = cbor_value_get_simple_type(&curVal, &trigger);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding trigger value");
        payload->trigger = (OCPresenceTrigger)trigger;

        // Resource type name
        err = cbor_value_map_find_value(rootValue, OC_RSRVD_RESOURCE_TYPE, &curVal);
        VERIFY_CBOR_SUCCESS(TAG, err, "to find res type tag");
        if (cbor_value_is_text_string(&curVal))
        {
            size_t len = 0;
            err = cbor_value_dup_text_string(&curVal, &payload->resourceType, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, err, "Failed finding resource type value");
        }

        err = cbor_value_advance(rootValue);
        VERIFY_CBOR_SUCCESS(TAG, err, "Failed advancing root value");

        *outPayload = (OCPayload *)payload;
        return OC_STACK_OK;
    }
exit:
    OIC_LOG(ERROR, TAG, "CBOR error Parse Presence Payload");
    OCPresencePayloadDestroy(payload);
    return ret;
}
