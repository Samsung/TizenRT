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

// Required for strok_r
#define _POSIX_C_SOURCE 200112L

#include "iotivity_config.h"
#include <stdio.h>
#include "ocpayload.h"
#include "octypes.h"
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocstackinternal.h"
#include "ocresource.h"
#include "logger.h"

#define TAG "OIC_RI_PAYLOAD"
#define CSV_SEPARATOR ','

static void OCFreeRepPayloadValueContents(OCRepPayloadValue* val);

void OCPayloadDestroy(OCPayload* payload)
{
    if (!payload)
    {
        return;
    }

    switch(payload->type)
    {
        case PAYLOAD_TYPE_REPRESENTATION:
            OCRepPayloadDestroy((OCRepPayload*)payload);
            break;
        case PAYLOAD_TYPE_DISCOVERY:
            OCDiscoveryPayloadDestroy((OCDiscoveryPayload*)payload);
            break;
        case PAYLOAD_TYPE_PRESENCE:
            OCPresencePayloadDestroy((OCPresencePayload*)payload);
            break;
        case PAYLOAD_TYPE_SECURITY:
            OCSecurityPayloadDestroy((OCSecurityPayload*)payload);
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "Unsupported payload type in destroy: %d", payload->type);
            OICFree(payload);
            break;
    }
}

OCRepPayload* OCRepPayloadCreate()
{
    OCRepPayload* payload = (OCRepPayload*)OICCalloc(1, sizeof(OCRepPayload));

    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_REPRESENTATION;

    return payload;
}

void OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child)
{
    if (!parent)
    {
        return;
    }

    while(parent->next)
    {
        parent = parent->next;
    }

    parent->next= child;
    child->next = NULL;
}

static OCRepPayloadValue* OCRepPayloadFindValue(const OCRepPayload* payload, const char* name)
{
    if (!payload || !name)
    {
        return NULL;
    }

    OCRepPayloadValue* val = payload->values;
    while(val)
    {
        if (0 == strcmp(val->name, name))
        {
            return val;
        }
        val = val->next;
    }

    return NULL;
}

static void OCCopyPropertyValueArray(OCRepPayloadValue* dest, OCRepPayloadValue* source)
{
    if (!dest || !source)
    {
        return;
    }

    size_t dimTotal = calcDimTotal(source->arr.dimensions);
    switch(source->arr.type)
    {
        case OCREP_PROP_INT:
            dest->arr.iArray = (int64_t*)OICMalloc(dimTotal * sizeof(int64_t));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.iArray, "Failed allocating memory");
            memcpy(dest->arr.iArray, source->arr.iArray, dimTotal * sizeof(int64_t));
            break;
        case OCREP_PROP_DOUBLE:
            dest->arr.dArray = (double*)OICMalloc(dimTotal * sizeof(double));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.dArray, "Failed allocating memory");
            memcpy(dest->arr.dArray, source->arr.dArray, dimTotal * sizeof(double));
            break;
        case OCREP_PROP_BOOL:
            dest->arr.bArray = (bool*)OICMalloc(dimTotal * sizeof(bool));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.bArray, "Failed allocating memory");
            memcpy(dest->arr.bArray, source->arr.bArray, dimTotal * sizeof(bool));
            break;
        case OCREP_PROP_STRING:
            dest->arr.strArray = (char**)OICMalloc(dimTotal * sizeof(char*));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.strArray, "Failed allocating memory");
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.strArray[i] = OICStrdup(source->arr.strArray[i]);
            }
            break;
        case OCREP_PROP_OBJECT:
            dest->arr.objArray = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.objArray, "Failed allocating memory");
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.objArray[i] = OCRepPayloadClone(source->arr.objArray[i]);
            }
            break;
        case OCREP_PROP_ARRAY:
            dest->arr.objArray = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.objArray, "Failed allocating memory");
            for(size_t i = 0; i < dimTotal; ++i)
            {
                dest->arr.objArray[i] = OCRepPayloadClone(source->arr.objArray[i]);
            }
            break;
        case OCREP_PROP_BYTE_STRING:
            dest->arr.ocByteStrArray = (OCByteString*)OICMalloc(dimTotal * sizeof(OCByteString));
            VERIFY_PARAM_NON_NULL(TAG, dest->arr.ocByteStrArray, "Failed allocating memory");
            for (size_t i = 0; i < dimTotal; ++i)
            {
                OCByteStringCopy(&dest->arr.ocByteStrArray[i], &source->arr.ocByteStrArray[i]);
                VERIFY_PARAM_NON_NULL(TAG, dest->arr.ocByteStrArray[i].bytes, "Failed allocating memory");
            }
            break;
        default:
            OIC_LOG(ERROR, TAG, "CopyPropertyValueArray invalid type");
            break;
    }
exit:
    return;
}

static void OCCopyPropertyValue (OCRepPayloadValue *dest, OCRepPayloadValue *source)
{
    if (!source || !dest)
    {
        return;
    }

    switch(source->type)
    {
        case OCREP_PROP_STRING:
            dest->str = OICStrdup(source->str);
            break;
        case OCREP_PROP_BYTE_STRING:
            dest->ocByteStr.bytes = (uint8_t*)OICMalloc(source->ocByteStr.len * sizeof(uint8_t));
            VERIFY_PARAM_NON_NULL(TAG, dest->ocByteStr.bytes, "Failed allocating memory");
            dest->ocByteStr.len = source->ocByteStr.len;
            memcpy(dest->ocByteStr.bytes, source->ocByteStr.bytes, dest->ocByteStr.len);
            break;
        case OCREP_PROP_OBJECT:
            dest->obj = OCRepPayloadClone(source->obj);
            break;
        case OCREP_PROP_ARRAY:
            OCCopyPropertyValueArray(dest, source);
            break;
        default:
            // Nothing to do for the trivially copyable types.
            break;
    }
exit:
    return;
}

static void OCFreeRepPayloadValueContents(OCRepPayloadValue* val)
{
    if (!val)
    {
        return;
    }

    if (val->type == OCREP_PROP_STRING)
    {
        if (val->str != NULL)
        {
            OICFree(val->str);
        }
    }
    else if (val->type == OCREP_PROP_BYTE_STRING)
    {
        OICFree(val->ocByteStr.bytes);
    }
    else if (val->type == OCREP_PROP_OBJECT)
    {
        OCRepPayloadDestroy(val->obj);
    }
    else if (val->type == OCREP_PROP_ARRAY)
    {
        size_t dimTotal = calcDimTotal(val->arr.dimensions);
        switch(val->arr.type)
        {
            case OCREP_PROP_INT:
            case OCREP_PROP_DOUBLE:
            case OCREP_PROP_BOOL:
                // Since this is a union, iArray will
                // point to all of the above
                OICFree(val->arr.iArray);
                break;
            case OCREP_PROP_STRING:
                if (val->arr.strArray != NULL)
                {
                    for(size_t i = 0; i < dimTotal; ++i)
                    {
                        OICFree(val->arr.strArray[i]);
                    }
                    OICFree(val->arr.strArray);
                }
                break;
            case OCREP_PROP_BYTE_STRING:
                if (val->arr.ocByteStrArray != NULL)
                {
                    for (size_t i = 0; i < dimTotal; ++i)
                    {
                        if (val->arr.ocByteStrArray[i].bytes)
                        {
                            OICFree(val->arr.ocByteStrArray[i].bytes);
                        }
                    }
                    OICFree(val->arr.ocByteStrArray);
                }
                break;
            case OCREP_PROP_OBJECT: // This case is the temporary fix for string input
                if (val->arr.objArray != NULL)
                {
                    for(size_t i = 0; i< dimTotal; ++i)
                    {
                        OCRepPayloadDestroy(val->arr.objArray[i]);
                    }
                    OICFree(val->arr.objArray);
                }
                break;
            case OCREP_PROP_NULL:
            case OCREP_PROP_ARRAY:
                OIC_LOG_V(ERROR, TAG, "FreeRepPayloadValueContents: Illegal type\
                        inside an array: %d", val->arr.type);
                break;
        }
    }
}

static void OCFreeRepPayloadValue(OCRepPayloadValue* val)
{
    if (!val)
    {
        return;
    }

    OICFree(val->name);
    OCFreeRepPayloadValueContents(val);
    OCFreeRepPayloadValue(val->next);
    OICFree(val);
}
static OCRepPayloadValue* OCRepPayloadValueClone (OCRepPayloadValue* source)
{
    if (!source)
    {
        return NULL;
    }

    OCRepPayloadValue *sourceIter = source;
    OCRepPayloadValue *destIter = (OCRepPayloadValue*) OICCalloc(1, sizeof(OCRepPayloadValue));
    if (!destIter)
    {
        return NULL;
    }

    OCRepPayloadValue *headOfClone = destIter;

    // Copy payload type and non pointer types in union.
    *destIter = *sourceIter;
    destIter->name = OICStrdup (sourceIter->name);
    OCCopyPropertyValue (destIter, sourceIter);

    sourceIter = sourceIter->next;

    while (sourceIter)
    {
        destIter->next = (OCRepPayloadValue*) OICCalloc(1, sizeof(OCRepPayloadValue));
        if (!destIter->next)
        {
            OCFreeRepPayloadValue (headOfClone);
            return NULL;
        }

        *(destIter->next) = *sourceIter;
        destIter->next->name = OICStrdup (sourceIter->name);
        OCCopyPropertyValue (destIter->next, sourceIter);

        sourceIter = sourceIter->next;
        destIter = destIter->next;
    }
    return headOfClone;
}

static OCRepPayloadValue* OCRepPayloadFindAndSetValue(OCRepPayload* payload, const char* name,
        OCRepPayloadPropType type)
{
    if (!payload || !name)
    {
        return NULL;
    }

    OCRepPayloadValue* val = payload->values;
    if (val == NULL)
    {
        payload->values = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
        if (!payload->values)
        {
            return NULL;
        }
        payload->values->name = OICStrdup(name);
        if (!payload->values->name)
        {
            OICFree(payload->values);
            payload->values = NULL;
            return NULL;
        }
        payload->values->type =type;
        return payload->values;
    }

    while(val)
    {
        if (0 == strcmp(val->name, name))
        {
            OCFreeRepPayloadValueContents(val);
            val->type = type;
            return val;
        }
        else if (val->next == NULL)
        {
            val->next = (OCRepPayloadValue*)OICCalloc(1, sizeof(OCRepPayloadValue));
            if (!val->next)
            {
                return NULL;
            }
            val->next->name = OICStrdup(name);
            if (!val->next->name)
            {
                OICFree(val->next);
                val->next = NULL;
                return NULL;
            }
            val->next->type =type;
            return val->next;
        }

        val = val->next;
    }

    OIC_LOG(ERROR, TAG, "FindAndSetValue reached point after while loop, pointer corruption?");
    return NULL;
}

bool OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType)
{
    return OCRepPayloadAddResourceTypeAsOwner(payload, OICStrdup(resourceType));
}

bool OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType)
{
    if (!payload || !resourceType)
    {
        return false;
    }

    if (payload->types)
    {
        OCStringLL* cur = payload->types;
        while(cur->next)
        {
            cur = cur->next;
        }
        cur->next = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));

        if (!cur->next)
        {
            return false;
        }

        cur->next->value = resourceType;
        return true;
    }
    else
    {
        payload->types = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
        if (!payload->types)
        {
            return false;
        }
        payload->types->value = resourceType;
        return true;
    }
}

bool OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface)
{
    return OCRepPayloadAddInterfaceAsOwner(payload, OICStrdup(iface));
}

bool OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface)
{
    if (!payload || !iface)
    {
        return false;
    }

    if (payload->interfaces)
    {
        OCStringLL* cur = payload->interfaces;
        while(cur->next)
        {
            cur = cur->next;
        }
        cur->next = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));

        if (!cur->next)
        {
            return false;
        }
        cur->next->value = iface;
        return true;
    }
    else
    {
        payload->interfaces = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
        if (!payload->interfaces)
        {
            return false;
        }
        payload->interfaces->value = iface;
        return true;
    }
}

bool OCRepPayloadSetUri(OCRepPayload* payload, const char*  uri)
{
    if (!payload)
    {
        return false;
    }
    OICFree(payload->uri);
    payload->uri = OICStrdup(uri);
    return payload->uri != NULL;
}

bool OCRepPayloadIsNull(const OCRepPayload* payload, const char* name)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val)
    {
        return true;
    }

    return val->type == OCREP_PROP_NULL;
}

static bool OCRepPayloadSetProp(OCRepPayload* payload, const char* name,
        void* value, OCRepPayloadPropType type)
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, type);
    if (!val)
    {
        return false;
    }
    switch(type)
    {
        case OCREP_PROP_INT:
               val->i = *(int64_t*)value;
               break;
        case OCREP_PROP_DOUBLE:
               val->d = *(double*)value;
               break;
        case OCREP_PROP_BOOL:
               val->b = *(bool*)value;
               break;
        case OCREP_PROP_OBJECT:
               val->obj = (OCRepPayload*)value;
               break;
        case OCREP_PROP_STRING:
               val->str = (char*)value;
               return val->str != NULL;
        case OCREP_PROP_BYTE_STRING:
               val->ocByteStr = *(OCByteString*)value;
               return val->ocByteStr.bytes != NULL;
               break;
        case OCREP_PROP_NULL:
               return val != NULL;
        case OCREP_PROP_ARRAY:
        default:
               return false;
    }

    return true;
}

bool OCRepPayloadSetNull(OCRepPayload* payload, const char* name)
{
    return OCRepPayloadSetProp(payload, name, NULL, OCREP_PROP_NULL);
}

bool OCRepPayloadSetPropInt(OCRepPayload* payload,
        const char* name, int64_t value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_INT);
}

bool OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_INT)
    {
        return false;
    }

    *value = val->i;
    return true;
}

bool OCRepPayloadSetPropDouble(OCRepPayload* payload,
                               const char* name, double value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_DOUBLE);
}

bool OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (val)
    {
        if (val->type == OCREP_PROP_DOUBLE)
        {
            *value = val->d;
            return true;
        }
        else if (val->type == OCREP_PROP_INT)
        {
            *value = val->i;
            return true;
        }
    }

    return false;
}

bool OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value)
{
    char* temp = OICStrdup(value);
    bool b = OCRepPayloadSetPropStringAsOwner(payload, name, temp);

    if (!b)
    {
        OICFree(temp);
    }
    return b;
}

bool OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_STRING);
}

bool OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_STRING)
    {
        return false;
    }

    *value = OICStrdup(val->str);
    return *value != NULL;
}

bool OCRepPayloadSetPropByteString(OCRepPayload* payload, const char* name, OCByteString value)
{
    if (!value.bytes || !value.len)
    {
        return false;
    }

    OCByteString ocByteStr = {NULL, 0};
    bool b = OCByteStringCopy(&ocByteStr, &value);

    if (b)
    {
        b = OCRepPayloadSetPropByteStringAsOwner(payload, name, &ocByteStr);
    }
    if (!b)
    {
        OICFree(ocByteStr.bytes);
    }
    return b;
}

bool OCRepPayloadSetPropByteStringAsOwner(OCRepPayload* payload, const char* name, OCByteString* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_BYTE_STRING);
}

bool OCRepPayloadGetPropByteString(const OCRepPayload* payload, const char* name, OCByteString* value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_BYTE_STRING)
    {
        return false;
    }

    if (!value)
    {
        return false;
    }

    value->bytes = (uint8_t*)OICMalloc(val->ocByteStr.len * sizeof(uint8_t));
    if (!value->bytes)
    {
        return false;
    }
    value->len = val->ocByteStr.len;
    memcpy(value->bytes, val->ocByteStr.bytes, value->len);

    return true;
}

bool OCRepPayloadSetPropBool(OCRepPayload* payload,
                             const char* name, bool value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_BOOL);
}

bool OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_BOOL)
    {
        return false;
    }

    *value = val->b;
    return true;
}

#ifdef __WITH_TLS__
static char *getStringFromEncodingType(OicEncodingType_t type)
{
    char *str = NULL;
    switch (type)
    {
        case OIC_ENCODING_BASE64: str = OC_RSRVD_BASE64; break;
        case OIC_ENCODING_DER: str = OC_RSRVD_DER; break;
        case OIC_ENCODING_PEM: str = OC_RSRVD_PEM; break;
        case OIC_ENCODING_RAW: str = OC_RSRVD_RAW; break;
        default: str = OC_RSRVD_UNKNOWN; break;
    }
    char encoding[32];
    snprintf(encoding, sizeof(encoding), "%s.%s.%s", OC_OIC_SEC, OC_RSRVD_ENCODING, str);

    return OICStrdup(encoding);
}

bool OCRepPayloadSetPropPubDataTypeAsOwner(OCRepPayload *payload, const char *name,
                                           const OicSecKey_t *value)
{
    if (!payload || !name || !value)
    {
        return false;
    }

    bool binary_field = false;
    if (OIC_ENCODING_RAW == value->encoding || OIC_ENCODING_DER == value->encoding)
    {
        binary_field = true;
    }

    OCRepPayload *heplerPayload = OCRepPayloadCreate();
    if (!heplerPayload)
    {
        return false;
    }

    char *encoding = getStringFromEncodingType(value->encoding);
    if (!OCRepPayloadSetPropString(heplerPayload, OC_RSRVD_ENCODING, encoding))
    {
        OIC_LOG_V(ERROR, TAG, "Can't set %s", OC_RSRVD_ENCODING);
    }

    OCByteString val = {.bytes = value->data, .len = value->len};
    if (binary_field)
    {
        if (!OCRepPayloadSetPropByteString(heplerPayload, OC_RSRVD_DATA, val))
        {
            OIC_LOG_V(ERROR, TAG, "Can't set %s", OC_RSRVD_DATA);
        }
    }
    else
    {
        if (!OCRepPayloadSetPropString(heplerPayload, OC_RSRVD_DATA, (char *)val.bytes))
        {
            OIC_LOG_V(ERROR, TAG, "Can't set %s", OC_RSRVD_DATA);
        }
    }

    if (!OCRepPayloadSetPropObject(payload, name, (const OCRepPayload *)heplerPayload))
    {
        OIC_LOG_V(ERROR, TAG, "Can't set %s", name);
    }

    OCRepPayloadDestroy(heplerPayload);
    OICFree(encoding);

    return true;
}

bool OCRepPayloadSetPropPubDataType(OCRepPayload *payload, const char *name,
                                    const OicSecKey_t *value)
{
    return OCRepPayloadSetPropPubDataTypeAsOwner(payload, name, value);
}

static OicEncodingType_t getEncodingTypeFromString(char *encoding)
{
    OicEncodingType_t type = OIC_ENCODING_UNKNOW;

    char *str = strrchr(encoding, '.');
    if (NULL == str)
    {
        OIC_LOG_V(ERROR, TAG, "Can't find . in %s", encoding);
        return type;
    }
    str++; //go to encoding itself

    if (0 == strcmp(str, OC_RSRVD_BASE64)) type = OIC_ENCODING_BASE64;
    else if (0 == strcmp(str, OC_RSRVD_DER)) type = OIC_ENCODING_DER;
    else if (0 == strcmp(str, OC_RSRVD_PEM)) type = OIC_ENCODING_PEM;
    else if (0 == strcmp(str, OC_RSRVD_RAW)) type = OIC_ENCODING_RAW;

    return type;
}

bool OCRepPayloadGetPropPubDataType(const OCRepPayload *payload, const char *name, OicSecKey_t *value)
{
    OCRepPayload *heplerPayload = NULL;
    char *encoding = NULL;
    OCByteString val;

    if (!payload || !name || !value)
    {
        return false;
    }

    if (!OCRepPayloadGetPropObject(payload, name, &heplerPayload))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get object with name %s", name);
        return false;
    }

    if (!OCRepPayloadGetPropString(heplerPayload, OC_RSRVD_ENCODING, &encoding))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get %s", OC_RSRVD_ENCODING);
    }
    else
    {
        value->encoding = getEncodingTypeFromString(encoding);
        OICFree(encoding);
    }

    if (!OCRepPayloadGetPropByteString(heplerPayload, OC_RSRVD_DATA, &val))
    {
        if (!OCRepPayloadGetPropString(heplerPayload, OC_RSRVD_DATA, (char **)&val.bytes))
        {
            OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_DATA);
        }
        else
        {
            value->data = val.bytes;
            value->len  = strlen(val.bytes);
        }
    }
    else
    {
        value->data = val.bytes;
        value->len  = val.len;
    }

    OCRepPayloadDestroy(heplerPayload);
    return true;
}
#endif

bool OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value)
{
    OCRepPayload* temp = OCRepPayloadClone(value);
    bool b = OCRepPayloadSetPropObjectAsOwner(payload, name, temp);

    if (!b)
    {
        OCRepPayloadDestroy(temp);
    }
    return b;
}

bool OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name, OCRepPayload* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_OBJECT);
}

bool OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_OBJECT)
    {
        return false;
    }

    *value = OCRepPayloadClone(val->obj);
    return *value != NULL;
}

size_t calcDimTotal(const size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    if (dimensions[0] == 0)
    {
        return 0;
    }

    size_t total = 1;
    for(uint8_t i = 0; i < MAX_REP_ARRAY_DEPTH && dimensions[i] != 0; ++i)
    {
        total *= dimensions[i];
    }
    return total;
}


bool OCRepPayloadSetByteStringArrayAsOwner(OCRepPayload* payload, const char* name,
        OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_BYTE_STRING;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.ocByteStrArray = array;

    return true;
}

bool OCRepPayloadSetByteStringArray(OCRepPayload* payload, const char* name,
        const OCByteString* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    if (!array)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    OCByteString* newArray = (OCByteString*)OICCalloc(dimTotal, sizeof(OCByteString));

    if (!newArray)
    {
        return false;
    }

    for (size_t i = 0; i < dimTotal; ++i)
    {
        newArray[i].bytes = (uint8_t*)OICMalloc(array[i].len * sizeof(uint8_t));
        if (NULL == newArray[i].bytes)
        {
            for (size_t j = 0; j < i; ++j)
            {
                OICFree(newArray[j].bytes);
            }

            OICFree(newArray);
            return false;
        }
        newArray[i].len = array[i].len;
        memcpy(newArray[i].bytes, array[i].bytes, newArray[i].len);
    }

    bool b = OCRepPayloadSetByteStringArrayAsOwner(payload, name, newArray, dimensions);
    if (!b)
    {
        for (size_t i = 0; i < dimTotal; ++i)
        {
            OICFree(newArray[i].bytes);
        }

        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetByteStringArray(const OCRepPayload* payload, const char* name,
        OCByteString** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY || val->arr.type != OCREP_PROP_BYTE_STRING
            || !val->arr.ocByteStrArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    *array = (OCByteString*)OICCalloc(dimTotal, sizeof(OCByteString));
    if (!*array)
    {
        return false;
    }

    for (size_t i = 0; i < dimTotal; ++i)
    {
        OCByteString* tmp = &(*array)[i];
        tmp->bytes = (uint8_t*)OICMalloc(val->arr.ocByteStrArray[i].len * sizeof(uint8_t));
        if (NULL == tmp->bytes)
        {
            for (size_t j = 0; j < i; ++j)
            {
                OCByteString* tmp = &(*array)[j];
                OICFree(tmp->bytes);
            }
            OICFree(*array);
            *array = NULL;

            return false;
        }
        tmp->len = val->arr.ocByteStrArray[i].len;
        memcpy(tmp->bytes, val->arr.ocByteStrArray[i].bytes, tmp->len);
    }

    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}


bool OCRepPayloadSetIntArrayAsOwner(OCRepPayload* payload, const char* name,
        int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_INT;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.iArray = array;

    return true;
}

bool OCRepPayloadSetIntArray(OCRepPayload* payload, const char* name,
        const int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    int64_t* newArray = (int64_t*)OICMalloc(dimTotal * sizeof(int64_t));

    if (!newArray)
    {
        return false;
    }

    memcpy(newArray, array, dimTotal * sizeof(int64_t));


    bool b = OCRepPayloadSetIntArrayAsOwner(payload, name, newArray, dimensions);
    if (!b)
    {
        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetIntArray(const OCRepPayload* payload, const char* name,
        int64_t** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY || val->arr.type != OCREP_PROP_INT
            || !val->arr.iArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }
    *array = (int64_t*)OICMalloc(dimTotal * sizeof(int64_t));
    if (!*array)
    {
        return false;
    }

    memcpy(*array, val->arr.iArray, dimTotal * sizeof(int64_t));
    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}

bool OCRepPayloadSetDoubleArrayAsOwner(OCRepPayload* payload, const char* name,
        double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_DOUBLE;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.dArray = array;

    return true;
}
bool OCRepPayloadSetDoubleArray(OCRepPayload* payload, const char* name,
        const double* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    double* newArray = (double*)OICMalloc(dimTotal * sizeof(double));

    if (!newArray)
    {
        return false;
    }

    memcpy(newArray, array, dimTotal * sizeof(double));

    bool b = OCRepPayloadSetDoubleArrayAsOwner(payload, name, newArray, dimensions);
    if (!b)
    {
        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetDoubleArray(const OCRepPayload* payload, const char* name,
        double** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY ||
        (val->arr.type != OCREP_PROP_DOUBLE && val->arr.type != OCREP_PROP_INT)
            || !val->arr.dArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }
    *array = (double*)OICMalloc(dimTotal * sizeof(double));
    if (!*array)
    {
        return false;
    }

    if (val->arr.type == OCREP_PROP_DOUBLE)
    {
        memcpy(*array, val->arr.dArray, dimTotal * sizeof(double));
    }
    else
    {
        /* need to convert from integer */
        size_t n = 0;
        for ( ; n < dimTotal; ++n)
        {
            (*array)[n] = val->arr.iArray[n];
        }
    }
    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}

bool OCRepPayloadSetStringArrayAsOwner(OCRepPayload* payload, const char* name,
        char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_STRING;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.strArray = array;

    return true;
}
bool OCRepPayloadSetStringArray(OCRepPayload* payload, const char* name,
        const char** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    char** newArray = (char**)OICMalloc(dimTotal * sizeof(char*));

    if (!newArray)
    {
        return false;
    }

    for(size_t i = 0; i < dimTotal; ++i)
    {
        newArray[i] = OICStrdup(array[i]);
    }

    bool b = OCRepPayloadSetStringArrayAsOwner(payload, name, newArray, dimensions);

    if (!b)
    {
        for(size_t i = 0; i < dimTotal; ++i)
        {
            OICFree(newArray[i]);
        }
        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetStringArray(const OCRepPayload* payload, const char* name,
        char*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY || val->arr.type != OCREP_PROP_STRING
            || !val->arr.strArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }
    *array = (char**)OICMalloc(dimTotal * sizeof(char*));
    if (!*array)
    {
        return false;
    }

    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));

    for(size_t i = 0; i < dimTotal; ++i)
    {
        (*array)[i] = OICStrdup(val->arr.strArray[i]);
    }

    return true;

}

bool OCRepPayloadSetBoolArrayAsOwner(OCRepPayload* payload, const char* name,
        bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{

    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_BOOL;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.bArray = array;

    return true;
}
bool OCRepPayloadSetBoolArray(OCRepPayload* payload, const char* name,
        const bool* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    bool* newArray = (bool*)OICMalloc(dimTotal * sizeof(bool));

    if (!newArray)
    {
        return false;
    }

    memcpy(newArray, array, dimTotal * sizeof(bool));


    bool b = OCRepPayloadSetBoolArrayAsOwner(payload, name, newArray, dimensions);
    if (!b)
    {
        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetBoolArray(const OCRepPayload* payload, const char* name,
        bool** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY || val->arr.type != OCREP_PROP_BOOL
            || !val->arr.bArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }
    *array = (bool*)OICMalloc(dimTotal * sizeof(bool));
    if (!*array)
    {
        return false;
    }

    memcpy(*array, val->arr.bArray, dimTotal * sizeof(bool));
    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}

bool OCRepPayloadSetPropObjectArrayAsOwner(OCRepPayload* payload, const char* name,
        OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindAndSetValue(payload, name, OCREP_PROP_ARRAY);

    if (!val)
    {
        return false;
    }

    val->arr.type = OCREP_PROP_OBJECT;
    memcpy(val->arr.dimensions, dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    val->arr.objArray = array;

    return true;
}

bool OCRepPayloadSetPropObjectArray(OCRepPayload* payload, const char* name,
        const OCRepPayload** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);
    if (dimTotal == 0)
    {
        return false;
    }

    OCRepPayload** newArray = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));

    if (!newArray)
    {
        return false;
    }

    for(size_t i = 0; i < dimTotal; ++i)
    {
        newArray[i] = OCRepPayloadClone(array[i]);
    }

    bool b = OCRepPayloadSetPropObjectArrayAsOwner(payload, name, newArray, dimensions);

    if (!b)
    {
        for(size_t i = 0; i < dimTotal; ++i)
        {
           OCRepPayloadDestroy(newArray[i]);
        }
        OICFree(newArray);
    }
    return b;
}

bool OCRepPayloadGetPropObjectArray(const OCRepPayload* payload, const char* name,
        OCRepPayload*** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_ARRAY || val->arr.type != OCREP_PROP_OBJECT
            || !val->arr.objArray)
    {
        return false;
    }

    size_t dimTotal = calcDimTotal(val->arr.dimensions);
    if (dimTotal == 0)
    {
        return false;
    }
    *array = (OCRepPayload**)OICMalloc(dimTotal * sizeof(OCRepPayload*));
    if (!*array)
    {
        return false;
    }

    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));

    for(size_t i = 0; i < dimTotal; ++i)
    {
        (*array)[i] = OCRepPayloadClone(val->arr.objArray[i]);
    }

    return true;
}

void OCFreeOCStringLL(OCStringLL* ll)
{
    if (!ll)
    {
        return;
    }

    OCFreeOCStringLL(ll->next);
    OICFree(ll->value);
    OICFree(ll);
}

OCStringLL* CloneOCStringLL (OCStringLL* ll)
{
    if (!ll)
    {
        return NULL;
    }

    OCStringLL *sourceIter = ll;

    OCStringLL *destIter = (OCStringLL*)OICCalloc (1, sizeof (OCStringLL));
    if (!destIter)
    {
        return NULL;
    }
    destIter->value = OICStrdup (sourceIter->value);

    OCStringLL *headOfClone = destIter;

    sourceIter = sourceIter->next;

    while (sourceIter)
    {
        destIter->next  = (OCStringLL*)OICCalloc (1, sizeof (OCStringLL));
        if (!destIter->next)
        {
            OCFreeOCStringLL (headOfClone);
            return NULL;
        }
        destIter->next->value = OICStrdup (sourceIter->value);

        destIter = destIter->next;
        sourceIter = sourceIter->next;
    }
    return headOfClone;
}

OCStringLL* OCCreateOCStringLL(const char* text)
{
    char *token = NULL;
    char *head = NULL;
    char *tail = NULL;
    char *backup  = NULL;
    OCStringLL* result = NULL;
    OCStringLL* iter = NULL;
    OCStringLL* prev = NULL;
    static const char delim[] = { CSV_SEPARATOR, '\0' };

    VERIFY_PARAM_NON_NULL(TAG, text, "Invalid parameter");
    backup = OICStrdup(text);
    VERIFY_PARAM_NON_NULL(TAG, backup, "Failed allocating memory");

    for (head = backup; ; head = NULL)
    {
        token = (char *) strtok_r(head, delim, &tail);
        if (!token)
        {
            break;
        }
        iter = (OCStringLL *)OICCalloc(1,sizeof(OCStringLL));
        VERIFY_PARAM_NON_NULL(TAG, iter, "Failed allocating memory");
        if (!result)
        {
            result = iter;
        }
        else
        {
            prev->next = iter;
        }
        iter->value = OICStrdup(token);
        VERIFY_PARAM_NON_NULL(TAG, iter->value, "Failed allocating memory");
        prev = iter;
        iter = iter->next;
    }
    OICFree(backup);
    return result;

exit:
    OICFree(backup);
    OCFreeOCStringLL(result);
    return NULL;
}

char* OCCreateString(const OCStringLL* ll)
{
    if (!ll)
    {
        return NULL;
    }

    char *str = NULL;
    char *pos = NULL;
    size_t len = 0;
    size_t sublen = 0;
    int count = 0;

    for (const OCStringLL *it = ll; it; it = it->next)
    {
        len += strlen(it->value) + 1;
    }
    len--; // remove trailing separator (just added above)
    str = (char*) OICMalloc(len + 1);
    if (!str)
    {
        return NULL;
    }

    pos = str;
    const OCStringLL *it = ll;
    while (it)
    {
        sublen = strlen(it->value);
        count = snprintf(pos, len + 1, "%s", it->value);
        if ((size_t)count < sublen)
        {
            OICFree(str);
            return NULL;
        }
        len -= sublen;
        pos += count;

        it = it->next;
        if (it)
        {
            *pos = CSV_SEPARATOR;
            len--;
            *(++pos) = '\0';
       }
    }

    return str;
}

bool OCByteStringCopy(OCByteString* dest, const OCByteString* source)
{
    VERIFY_PARAM_NON_NULL(TAG, source, "Bad input");

    if (!dest)
    {
        dest = (OCByteString *)OICMalloc(sizeof(OCByteString));
        VERIFY_PARAM_NON_NULL(TAG, dest, "Failed allocating memory");
    }
    if (dest->bytes)
    {
        OICFree(dest->bytes);
    }
    dest->bytes = (uint8_t*)OICMalloc(source->len * sizeof(uint8_t));
    VERIFY_PARAM_NON_NULL(TAG, dest->bytes, "Failed allocating memory");
    memcpy(dest->bytes, source->bytes, source->len * sizeof(uint8_t));
    dest->len = source->len;
    return true;

exit:
    if (dest)
    {
        dest->len = 0;
        OICFree(dest->bytes);
        dest->bytes = NULL;
    }

    return false;
}

OCRepPayload* OCRepPayloadClone (const OCRepPayload* payload)
{
    if (!payload)
    {
        return NULL;
    }

    OCRepPayload *clone = OCRepPayloadCreate();

    if (!clone)
    {
        return NULL;
    }

    clone->uri = OICStrdup (payload->uri);
    clone->types = CloneOCStringLL (payload->types);
    clone->interfaces = CloneOCStringLL (payload->interfaces);
    clone->values = OCRepPayloadValueClone (payload->values);

    return clone;
}

OCRepPayload* OCRepPayloadBatchClone(const OCRepPayload* repPayload)
{
    OCRepPayload *newPayload = OCRepPayloadCreate();
    if (!newPayload)
    {
        return NULL;
    }

    newPayload->uri = OICStrdup(repPayload->uri);
    OCRepPayload *clone = OCRepPayloadCreate();
    if (!clone)
    {
        OCPayloadDestroy((OCPayload *)newPayload);
        return NULL;
    }

    clone->types  = CloneOCStringLL(repPayload->types);
    clone->interfaces  = CloneOCStringLL(repPayload->interfaces);
    clone->values = OCRepPayloadValueClone(repPayload->values);
    OCRepPayloadSetPropObjectAsOwner(newPayload, OC_RSRVD_REPRESENTATION, clone);

    return newPayload;
}

void OCRepPayloadDestroy(OCRepPayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICFree(payload->uri);
    OCFreeOCStringLL(payload->types);
    OCFreeOCStringLL(payload->interfaces);
    OCFreeRepPayloadValue(payload->values);
    OCRepPayloadDestroy(payload->next);
    OICFree(payload);
}

OCDiscoveryPayload* OCDiscoveryPayloadCreate()
{
    OCDiscoveryPayload* payload = (OCDiscoveryPayload*)OICCalloc(1, sizeof(OCDiscoveryPayload));

    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_DISCOVERY;

    return payload;
}

OCSecurityPayload* OCSecurityPayloadCreate(const uint8_t* securityData, size_t size)
{
    OCSecurityPayload* payload = (OCSecurityPayload*)OICCalloc(1, sizeof(OCSecurityPayload));

    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_SECURITY;
    payload->securityData = (uint8_t *)OICCalloc(1, size);
    if (!payload->securityData)
    {
        OICFree(payload);
        return NULL;
    }
    memcpy(payload->securityData, (uint8_t *)securityData, size);
    payload->payloadSize = size;

    return payload;
}

void OCSecurityPayloadDestroy(OCSecurityPayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICClearMemory(payload->securityData, payload->payloadSize);
    OICFree(payload->securityData);
    OICFree(payload);
}

size_t OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload)
{
    size_t i = 0;
    OCResourcePayload* p = payload->resources;
    while(p)
    {
        ++i;
        p = p->next;
    }
    return i;
}

OCResourcePayload* OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload, size_t index)
{
    size_t i = 0;
    OCResourcePayload* p = payload->resources;
    while(p)
    {
        if (i == index)
        {
            return p;
        }
        ++i;
        p = p->next;
    }
    return NULL;
}

#if !defined(TCP_ADAPTER) || defined(DISABLE_TCP_SERVER)
static OCResourcePayload* OCCopyResource(const OCResource* res, uint16_t securePort)
#else
static OCResourcePayload* OCCopyResource(const OCResource* res, uint16_t securePort,
                                         uint16_t tcpPort)
#endif
{
    OCResourcePayload* pl = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
    if (!pl)
    {
        return NULL;
    }

    pl->uri = OICStrdup(res->uri);

    if (!pl->uri)
    {
        OCDiscoveryResourceDestroy(pl);
        return NULL;
    }

    // types
    OCResourceType* typePtr = res->rsrcType;

    if (typePtr != NULL)
    {
        pl->types = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
        if (!pl->types)
        {
            OCDiscoveryResourceDestroy(pl);
            return NULL;
        }
        pl->types->value = OICStrdup(typePtr->resourcetypename);
        if (!pl->types->value)
        {
            OCDiscoveryResourceDestroy(pl);
            return NULL;
        }

        OCStringLL* cur = pl->types;
        typePtr = typePtr->next;
        while(typePtr)
        {
            cur->next = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
            if (!cur->next)
            {
                OCDiscoveryResourceDestroy(pl);
                return NULL;
            }
            cur->next->value = OICStrdup(typePtr->resourcetypename);
            if (!cur->next->value)
            {
                OCDiscoveryResourceDestroy(pl);
                return NULL;
            }
            cur = cur->next;
            typePtr = typePtr->next;
        }
    }

    // interfaces
    OCResourceInterface* ifPtr = res->rsrcInterface;
    if (ifPtr != NULL)
    {
        pl->interfaces = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
        if (!pl->interfaces)
        {
            OCDiscoveryResourceDestroy(pl);
            return NULL;
        }
        pl->interfaces->value = OICStrdup(ifPtr->name);
        if (!pl->interfaces->value)
        {
            OCDiscoveryResourceDestroy(pl);
            return NULL;
        }

        OCStringLL* cur = pl->interfaces;
        ifPtr = ifPtr->next;
        while(ifPtr && cur)
        {
            cur->next = (OCStringLL*)OICCalloc(1, sizeof(OCStringLL));
            if (!cur->next)
            {
                OCDiscoveryResourceDestroy(pl);
                return NULL;
            }
            cur->next->value = OICStrdup(ifPtr->name);
            if (!cur->next->value)
            {
                OCDiscoveryResourceDestroy(pl);
                return NULL;
            }
            cur = cur->next;
            ifPtr = ifPtr->next;
        }
    }

    pl->bitmap = res->resourceProperties & (OC_OBSERVABLE | OC_DISCOVERABLE
#ifdef MQ_PUBLISHER
                                            | OC_MQ_PUBLISHER
#endif
                                            );
    pl->secure = (res->resourceProperties & OC_SECURE) != 0;
    pl->port = securePort;
#if defined(TCP_ADAPTER) && !defined(DISABLE_TCP_SERVER)
    pl->tcpPort = tcpPort;
#endif
    return pl;
}

#if !defined(TCP_ADAPTER) || defined(DISABLE_TCP_SERVER)
void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort)
{
    OCDiscoveryPayloadAddNewResource(payload, OCCopyResource(res, securePort));
}
#else
void OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                   uint16_t securePort, uint16_t tcpPort)
{
    OCDiscoveryPayloadAddNewResource(payload, OCCopyResource(res, securePort, tcpPort));
}
#endif

bool OCResourcePayloadAddStringLL(OCStringLL **stringLL, const char *value)
{
    char *dup = NULL;
    VERIFY_PARAM_NON_NULL(TAG, value, "Invalid Parameters");
    dup = OICStrdup(value);
    VERIFY_PARAM_NON_NULL(TAG, dup, "Failed copying string");

    if (!*stringLL)
    {
        *stringLL = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
        VERIFY_PARAM_NON_NULL(TAG, *stringLL, "Failed allocating memory");
        (*stringLL)->value = dup;
        return true;
    }
    else
    {
        OCStringLL *temp = *stringLL;
        while(temp->next)
        {
            temp = temp->next;
        }
        temp->next = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
        VERIFY_PARAM_NON_NULL(TAG, temp->next, "Failed allocating memory");
        temp->next->value = dup;
        return true;
    }
exit:
    OICFree(dup);
    return false;
}

void OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload, OCResourcePayload* res)
{
    if (!payload)
    {
        return;
    }

    if (!payload->resources)
    {
        payload->resources = res;
    }
    else
    {
        OCResourcePayload* p = payload->resources;
        while(p->next)
        {
            p = p->next;
        }
        p->next = res;
    }
}

void OCDiscoveryResourceDestroy(OCResourcePayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICFree(payload->uri);
    OCFreeOCStringLL(payload->types);
    OCFreeOCStringLL(payload->interfaces);
    OCDiscoveryResourceDestroy(payload->next);
    OICFree(payload);

}
void OCDiscoveryPayloadDestroy(OCDiscoveryPayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->sid);
    OICFree(payload->baseURI);
    OCFreeOCStringLL(payload->type);
    OICFree(payload->name);
    OCFreeOCStringLL(payload->iface);
    OCDiscoveryResourceDestroy(payload->resources);
    OCDiscoveryPayloadDestroy(payload->next);
    OICFree(payload);
}

OCPresencePayload* OCPresencePayloadCreate(uint32_t seqNum, uint32_t maxAge,
        OCPresenceTrigger trigger, const char* resourceType)
{
    OCPresencePayload* payload = (OCPresencePayload*)OICCalloc(1, sizeof(OCPresencePayload));
    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_PRESENCE;
    payload->sequenceNumber = seqNum;
    payload->maxAge = maxAge;
    payload->trigger = trigger;
    payload->resourceType = OICStrdup(resourceType);
    return payload;
}

void OCPresencePayloadDestroy(OCPresencePayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->resourceType);
    OICFree(payload);
}
