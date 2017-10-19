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
#include "occollection.h"
#include "octypes.h"
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocstackinternal.h"
#include "ocresource.h"
#include "logger.h"
#include "ocendpoint.h"
#include "cacommon.h"
#include "ocstack.h"

#define TAG "OIC_RI_PAYLOAD"
#define CSV_SEPARATOR ','
#define MASK_SECURE_FAMS (OC_FLAG_SECURE | OC_MASK_FAMS)

static void OCFreeRepPayloadValueContents(OCRepPayloadValue* val);

void OC_CALL OCPayloadDestroy(OCPayload* payload)
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
        case PAYLOAD_TYPE_DIAGNOSTIC:
            OCDiagnosticPayloadDestroy((OCDiagnosticPayload*)payload);
            break;
        case PAYLOAD_TYPE_SECURITY:
            OCSecurityPayloadDestroy((OCSecurityPayload*)payload);
            break;
        case PAYLOAD_TYPE_INTROSPECTION:
            OCIntrospectionPayloadDestroy((OCIntrospectionPayload*)payload);
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "Unsupported payload type in destroy: %d", payload->type);
            OICFree(payload);
            break;
    }
}

OCRepPayload* OC_CALL OCRepPayloadCreate()
{
    OCRepPayload* payload = (OCRepPayload*)OICCalloc(1, sizeof(OCRepPayload));

    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_REPRESENTATION;

    return payload;
}

void OC_CALL OCRepPayloadAppend(OCRepPayload* parent, OCRepPayload* child)
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

static OCRepPayloadValue* OC_CALL OCRepPayloadFindValue(const OCRepPayload* payload, const char* name)
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

static void OC_CALL OCCopyPropertyValueArray(OCRepPayloadValue* dest, OCRepPayloadValue* source)
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
                VERIFY_PARAM_NON_NULL(TAG, dest->arr.strArray[i], "Failed to duplicate string");
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
            dest->arr.ocByteStrArray = (OCByteString*)OICCalloc(dimTotal, sizeof(OCByteString));
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

static void OC_CALL OCCopyPropertyValue (OCRepPayloadValue *dest, OCRepPayloadValue *source)
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
        OICFree(val->str);
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
                for(size_t i = 0; i < dimTotal; ++i)
                {
                    OICFree(val->arr.strArray[i]);
                }
                OICFree(val->arr.strArray);
                break;
            case OCREP_PROP_BYTE_STRING:
                for (size_t i = 0; i < dimTotal; ++i)
                {
                    if (val->arr.ocByteStrArray[i].bytes)
                    {
                        OICFree(val->arr.ocByteStrArray[i].bytes);
                    }
                }
                OICFree(val->arr.ocByteStrArray);
                break;
            case OCREP_PROP_OBJECT: // This case is the temporary fix for string input
                for(size_t i = 0; i< dimTotal; ++i)
                {
                    OCRepPayloadDestroy(val->arr.objArray[i]);
                }
                OICFree(val->arr.objArray);
                break;
            case OCREP_PROP_NULL:
            case OCREP_PROP_ARRAY:
                OIC_LOG_V(ERROR, TAG, "FreeRepPayloadValueContents: Illegal type\
                        inside an array: %d", val->arr.type);
                break;
        }
    }
}

static void OC_CALL OCFreeRepPayloadValue(OCRepPayloadValue* val)
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
static OCRepPayloadValue* OC_CALL OCRepPayloadValueClone (OCRepPayloadValue* source)
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

static OCRepPayloadValue* OC_CALL OCRepPayloadFindAndSetValue(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadAddResourceType(OCRepPayload* payload, const char* resourceType)
{
    return OCRepPayloadAddResourceTypeAsOwner(payload, OICStrdup(resourceType));
}

bool OC_CALL OCRepPayloadAddResourceTypeAsOwner(OCRepPayload* payload, char* resourceType)
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

bool OC_CALL OCRepPayloadAddInterface(OCRepPayload* payload, const char* iface)
{
    return OCRepPayloadAddInterfaceAsOwner(payload, OICStrdup(iface));
}

bool OC_CALL OCRepPayloadAddInterfaceAsOwner(OCRepPayload* payload, char* iface)
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

bool OC_CALL OCRepPayloadSetUri(OCRepPayload* payload, const char*  uri)
{
    if (!payload)
    {
        return false;
    }
    OICFree(payload->uri);
    payload->uri = OICStrdup(uri);
    return payload->uri != NULL;
}

bool OC_CALL OCRepPayloadIsNull(const OCRepPayload* payload, const char* name)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val)
    {
        return true;
    }

    return val->type == OCREP_PROP_NULL;
}

static bool OC_CALL OCRepPayloadSetProp(OCRepPayload* payload, const char* name,
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
               break;
        case OCREP_PROP_NULL:
               return val != NULL;
        case OCREP_PROP_ARRAY:
        default:
               return false;
    }

    return true;
}

bool OC_CALL OCRepPayloadSetNull(OCRepPayload* payload, const char* name)
{
    return OCRepPayloadSetProp(payload, name, NULL, OCREP_PROP_NULL);
}

bool OC_CALL OCRepPayloadSetPropInt(OCRepPayload* payload,
        const char* name, int64_t value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_INT);
}

bool OC_CALL OCRepPayloadGetPropInt(const OCRepPayload* payload, const char* name, int64_t* value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_INT)
    {
        return false;
    }

    *value = val->i;
    return true;
}

bool OC_CALL OCRepPayloadSetPropDouble(OCRepPayload* payload,
                               const char* name, double value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_DOUBLE);
}

bool OC_CALL OCRepPayloadGetPropDouble(const OCRepPayload* payload, const char* name, double* value)
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
// Should be removed once IOT-1705 is fixed.
#ifdef _MSC_VER
#pragma warning( suppress : 4244 )
            *value = val->i;
#else
            *value = val->i;
#endif
            return true;
        }
    }

    return false;
}

bool OC_CALL OCRepPayloadSetPropString(OCRepPayload* payload, const char* name, const char* value)
{
    char* temp = OICStrdup(value);
    bool b = OCRepPayloadSetPropStringAsOwner(payload, name, temp);

    if (!b)
    {
        OICFree(temp);
    }
    return b;
}

bool OC_CALL OCRepPayloadSetPropStringAsOwner(OCRepPayload* payload, const char* name, char* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_STRING);
}

bool OC_CALL OCRepPayloadGetPropString(const OCRepPayload* payload, const char* name, char** value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_STRING)
    {
        return false;
    }

    *value = OICStrdup(val->str);
    return *value != NULL;
}

bool OC_CALL OCRepPayloadSetPropByteString(OCRepPayload* payload, const char* name, OCByteString value)
{
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

bool OC_CALL OCRepPayloadSetPropByteStringAsOwner(OCRepPayload* payload, const char* name, OCByteString* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_BYTE_STRING);
}

bool OC_CALL OCRepPayloadGetPropByteString(const OCRepPayload* payload, const char* name, OCByteString* value)
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

    if (val->ocByteStr.len)
    {
        value->bytes = (uint8_t*)OICMalloc(val->ocByteStr.len * sizeof(uint8_t));
        if (!value->bytes)
        {
            return false;
        }
    }
    else
    {
        value->bytes = NULL;
    }
    value->len = val->ocByteStr.len;
    memcpy(value->bytes, val->ocByteStr.bytes, value->len);

    return true;
}

bool OC_CALL OCRepPayloadSetPropBool(OCRepPayload* payload,
                             const char* name, bool value)
{
    return OCRepPayloadSetProp(payload, name, &value, OCREP_PROP_BOOL);
}

bool OC_CALL OCRepPayloadGetPropBool(const OCRepPayload* payload, const char* name, bool* value)
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

bool OC_CALL OCRepPayloadSetPropPubDataTypeAsOwner(OCRepPayload *payload, const char *name,
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

bool OC_CALL OCRepPayloadSetPropPubDataType(OCRepPayload *payload, const char *name,
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

bool OC_CALL OCRepPayloadGetPropPubDataType(const OCRepPayload *payload, const char *name, OicSecKey_t *value)
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
            value->len  = strlen((const char*)val.bytes);
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

bool OC_CALL OCRepPayloadSetPropObject(OCRepPayload* payload, const char* name, const OCRepPayload* value)
{
    OCRepPayload* temp = OCRepPayloadClone(value);
    bool b = OCRepPayloadSetPropObjectAsOwner(payload, name, temp);

    if (!b)
    {
        OCRepPayloadDestroy(temp);
    }
    return b;
}

bool OC_CALL OCRepPayloadSetPropObjectAsOwner(OCRepPayload* payload, const char* name, OCRepPayload* value)
{
    return OCRepPayloadSetProp(payload, name, value, OCREP_PROP_OBJECT);
}

bool OC_CALL OCRepPayloadGetPropObject(const OCRepPayload* payload, const char* name, OCRepPayload** value)
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val || val->type != OCREP_PROP_OBJECT)
    {
        return false;
    }

    *value = OCRepPayloadClone(val->obj);
    return *value != NULL;
}

size_t OC_CALL calcDimTotal(const size_t dimensions[MAX_REP_ARRAY_DEPTH])
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


bool OC_CALL OCRepPayloadSetByteStringArrayAsOwner(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetByteStringArray(OCRepPayload* payload, const char* name,
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
        if (array[i].len)
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

bool OC_CALL OCRepPayloadGetByteStringArray(const OCRepPayload* payload, const char* name,
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
        if (val->arr.ocByteStrArray[i].len)
        {
            tmp->bytes = (uint8_t*)OICMalloc(val->arr.ocByteStrArray[i].len * sizeof(uint8_t));
            if (NULL == tmp->bytes)
            {
                for (size_t j = 0; j < i; ++j)
                {
                    OCByteString* temp = &(*array)[j];
                    OICFree(temp->bytes);
                }
                OICFree(*array);
                *array = NULL;

                return false;
            }
        }
        tmp->len = val->arr.ocByteStrArray[i].len;
        memcpy(tmp->bytes, val->arr.ocByteStrArray[i].bytes, tmp->len);
    }

    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}


bool OC_CALL OCRepPayloadSetIntArrayAsOwner(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetIntArray(OCRepPayload* payload, const char* name,
        const int64_t* array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    size_t dimTotal = calcDimTotal(dimensions);

    int64_t* newArray = (int64_t*)OICMalloc(dimTotal * sizeof(int64_t));

    if (newArray && array)
    {
        memcpy(newArray, array, dimTotal * sizeof(int64_t));
    }

    bool b = OCRepPayloadSetIntArrayAsOwner(payload, name, newArray, dimensions);
    if (!b)
    {
        OICFree(newArray);
    }
    return b;
}

bool OC_CALL OCRepPayloadGetIntArray(const OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetDoubleArrayAsOwner(OCRepPayload* payload, const char* name,
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
bool OC_CALL OCRepPayloadSetDoubleArray(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadGetDoubleArray(const OCRepPayload* payload, const char* name,
        double** array, size_t dimensions[MAX_REP_ARRAY_DEPTH])
{
    OCRepPayloadValue* val = OCRepPayloadFindValue(payload, name);

    if (!val ||
        (val->type != OCREP_PROP_ARRAY) ||
        ((val->arr.type != OCREP_PROP_DOUBLE) &&
         (val->arr.type != OCREP_PROP_INT)) ||
        !val->arr.dArray)
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
// Should be removed once IOT-1705 is fixed.
#ifdef _MSC_VER
#pragma warning( suppress : 4244 )
            (*array)[n] = val->arr.iArray[n];
#else
            (*array)[n] = val->arr.iArray[n];
#endif
        }
    }
    memcpy(dimensions, val->arr.dimensions, MAX_REP_ARRAY_DEPTH * sizeof(size_t));
    return true;
}

bool OC_CALL OCRepPayloadSetStringArrayAsOwner(OCRepPayload* payload, const char* name,
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
bool OC_CALL OCRepPayloadSetStringArray(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadGetStringArray(const OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetBoolArrayAsOwner(OCRepPayload* payload, const char* name,
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
bool OC_CALL OCRepPayloadSetBoolArray(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadGetBoolArray(const OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetPropObjectArrayAsOwner(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadSetPropObjectArray(OCRepPayload* payload, const char* name,
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

bool OC_CALL OCRepPayloadGetPropObjectArray(const OCRepPayload* payload, const char* name,
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

void OC_CALL OCFreeOCStringLL(OCStringLL* ll)
{
    if (!ll)
    {
        return;
    }

    OCFreeOCStringLL(ll->next);
    OICFree(ll->value);
    OICFree(ll);
}

OCStringLL* OC_CALL CloneOCStringLL (OCStringLL* ll)
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

OCStringLL* OC_CALL OCCreateOCStringLL(const char* text)
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

char* OC_CALL OCCreateString(const OCStringLL* ll)
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

bool OC_CALL OCByteStringCopy(OCByteString* dest, const OCByteString* source)
{
    VERIFY_PARAM_NON_NULL(TAG, dest, "Bad dest");
    VERIFY_PARAM_NON_NULL(TAG, source, "Bad source");

    if (dest->bytes)
    {
        OICFree(dest->bytes);
    }
    if (source->len)
    {
        dest->bytes = (uint8_t*)OICMalloc(source->len * sizeof(uint8_t));
        VERIFY_PARAM_NON_NULL(TAG, dest->bytes, "Failed allocating memory");
        memcpy(dest->bytes, source->bytes, source->len * sizeof(uint8_t));
    }
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

OCRepPayload* OC_CALL OCRepPayloadClone (const OCRepPayload* payload)
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

OCRepPayload* OC_CALL OCRepPayloadBatchClone(const OCRepPayload* repPayload)
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

void OC_CALL OCRepPayloadDestroy(OCRepPayload* payload)
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

OCDiscoveryPayload* OC_CALL OCDiscoveryPayloadCreate()
{
    OCDiscoveryPayload* payload = (OCDiscoveryPayload*)OICCalloc(1, sizeof(OCDiscoveryPayload));

    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_DISCOVERY;

    return payload;
}

OCSecurityPayload* OC_CALL OCSecurityPayloadCreate(const uint8_t* securityData, size_t size)
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

void OC_CALL OCSecurityPayloadDestroy(OCSecurityPayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICClearMemory(payload->securityData, payload->payloadSize);
    OICFree(payload->securityData);
    OICFree(payload);
}

OCIntrospectionPayload* OC_CALL OCIntrospectionPayloadCreateFromCbor(const uint8_t* cborData,
    size_t size)
{
    OCIntrospectionPayload* payload = NULL;
    payload = (OCIntrospectionPayload*)OICCalloc(1, sizeof(OCIntrospectionPayload));
    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_INTROSPECTION;
    payload->cborPayload.bytes = (uint8_t*)OICCalloc(1, size);
    if (!payload->cborPayload.bytes)
    {
        OICFree(payload);
        return NULL;
    }
    memcpy(payload->cborPayload.bytes, cborData, size);
    payload->cborPayload.len = size;

    return payload;
}

void OC_CALL OCIntrospectionPayloadDestroy(OCIntrospectionPayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICFree(payload->cborPayload.bytes);
    OICFree(payload);
}

size_t OC_CALL OCDiscoveryPayloadGetResourceCount(OCDiscoveryPayload* payload)
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

OCResourcePayload* OC_CALL OCDiscoveryPayloadGetResource(OCDiscoveryPayload* payload, size_t index)
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

size_t OC_CALL OCEndpointPayloadGetEndpointCount(OCEndpointPayload* payload)
{
    size_t i = 0;
    OCEndpointPayload* ep = payload;
    while (ep)
    {
        ++i;
        ep = ep->next;
    }
    return i;
}

OCEndpointPayload* OC_CALL OCEndpointPayloadGetEndpoint(OCEndpointPayload* payload, size_t index)
{
    size_t i = 0;
    OCEndpointPayload* ep = payload;
    while (ep)
    {
        if (i == index)
        {
            return ep;
        }
        ++i;
        ep = ep->next;
    }
    return NULL;
}

void OC_CALL OCResourcePayloadAddNewEndpoint(OCResourcePayload* payload, OCEndpointPayload* endpoint)
{
    if (!payload)
    {
        return;
    }

    if (!payload->eps)
    {
        payload->eps = endpoint;
    }
    else
    {
        OCEndpointPayload* ep = payload->eps;
        while (ep->next)
        {
            ep = ep->next;
        }
        ep->next = endpoint;
    }
}

OCEndpointPayload* CreateEndpointPayloadList(const OCResource *resource, const OCDevAddr *devAddr,
                                            CAEndpoint_t *networkInfo, size_t infoSize,
                         OCEndpointPayload **listHead, size_t* epSize, OCEndpointPayload** selfEp)
{
    OCEndpointPayload *headNode = NULL;
    OCEndpointPayload *lastNode = NULL;

    VERIFY_PARAM_NON_NULL(TAG, resource, "Invalid resource parameter");
    VERIFY_PARAM_NON_NULL(TAG, devAddr, "Invalid devAddr parameter");
    VERIFY_PARAM_NON_NULL(TAG, networkInfo, "Invalid networkInfo parameter");
    VERIFY_PARAM_NON_NULL(TAG, listHead, "Invalid listHead parameter");
    if (epSize != NULL) *epSize = 0;

    bool includeSecure = resource->resourceProperties & OC_SECURE;
    bool includeNonsecure = resource->resourceProperties & OC_NONSECURE;

    if ((OC_ADAPTER_IP | OC_ADAPTER_TCP) & (devAddr->adapter))
    {
        for (size_t i = 0; i < infoSize; i++)
        {
            CAEndpoint_t *info = networkInfo + i;

            if (((CA_ADAPTER_IP | CA_ADAPTER_TCP) & info->adapter &&
                 info->ifindex == devAddr->ifindex) ||
                info->adapter == CA_ADAPTER_RFCOMM_BTEDR)
            {
                OCTpsSchemeFlags matchedTps = OC_NO_TPS;
                if (OC_STACK_OK != OCGetMatchedTpsFlags(info->adapter,
                                                        info->flags,
                                                        &matchedTps))
                {
                    OIC_LOG_V(ERROR, TAG, "OCGetMatchedTpsFlags err");
                    goto exit;
                }

                bool isSecure = (info->flags & OC_FLAG_SECURE);
                if (((resource->endpointType) & matchedTps) &&
                        ((isSecure && includeSecure) || (!isSecure && includeNonsecure)))
                {
                    // create payload
                    OCEndpointPayload* tmpNode = (OCEndpointPayload*)
                        OICCalloc(1, sizeof(OCEndpointPayload));
                    if (!tmpNode)
                    {
                        OIC_LOG_V(ERROR, TAG, "Fail creating tmpNode->addr");
                        goto exit;
                    }

                    OCStackResult ret = OCConvertTpsToString(matchedTps, &(tmpNode->tps));
                    if (ret != OC_STACK_OK)
                    {
                        OIC_LOG_V(DEBUG, TAG, "OCConvertTpsToString(%s) is false", tmpNode->tps);
                        OCDiscoveryEndpointDestroy(tmpNode);
                        goto exit;
                    }

                    tmpNode->addr = (char*)OICCalloc(MAX_ADDR_STR_SIZE, sizeof(char));
                    if (!tmpNode->addr)
                    {
                        OIC_LOG_V(ERROR, TAG, "Fail creating tmpNode->addr");
                        OCDiscoveryEndpointDestroy(tmpNode);
                        goto exit;
                    }

                    memcpy(tmpNode->addr, info->addr, sizeof(info->addr));
                    tmpNode->family = (OCTransportFlags)(info->flags);
                    tmpNode->port = info->port;
                    tmpNode->pri  = 1;
                    tmpNode->next = NULL;

                    // remember endpoint that matches devAddr for use in anchor
                    OCTransportFlags infoFlagsSecureFams = (OCTransportFlags)
                            (info->flags & MASK_SECURE_FAMS);
                    if ((selfEp != NULL) &&
                            ((infoFlagsSecureFams & devAddr->flags) == infoFlagsSecureFams))
                    {
                        *selfEp = tmpNode;
                    }

                    // store in list
                    if (!headNode)
                    {
                        headNode = tmpNode;
                        lastNode = tmpNode;
                    }
                    else
                    {
                        lastNode->next = tmpNode;
                        lastNode = tmpNode;
                    }
                    if (epSize != NULL)
                    {
                        (*epSize)++;
                    }
                }
            }
            else
            {
                OIC_LOG_V(DEBUG, TAG, "CATransportAdapter_t value = %d", info->adapter);
            }
        }
    }

    *listHead = headNode;
    return headNode;
exit:
    OCDiscoveryEndpointDestroy(headNode);
    return NULL;
}

static OCResourcePayload* OCCopyResource(const OCResource* res, uint16_t securePort,
                                         CAEndpoint_t *networkInfo, size_t infoSize,
                                         const OCDevAddr *devAddr
#ifndef TCP_ADAPTER
                                                                                    )
#else
                                         , uint16_t tcpPort)
#endif
{
    OCResourcePayload* pl = (OCResourcePayload*)OICCalloc(1, sizeof(OCResourcePayload));
    if (!pl)
    {
        return NULL;
    }

    OCEndpointPayload *selfEp = NULL;
    if (networkInfo && infoSize && devAddr)
    {
        CreateEndpointPayloadList(res, devAddr, networkInfo, infoSize,
                                      &(pl->eps), NULL, &selfEp);
    }

    pl->uri = OICStrdup(res->uri);
    if (!pl->uri)
    {
        OCDiscoveryResourceDestroy(pl);
        return NULL;
    }

    // relation is always the default unless the resource is the well known URI
    if (0 == strcmp(res->uri, OC_RSRVD_WELL_KNOWN_URI))
    {
        pl->rel = OICStrdup("self");
        if (!pl->rel)
        {
            OCDiscoveryResourceDestroy(pl);
            return NULL;
        }
    }

    // anchor
    char *anchor = OCCreateEndpointString(selfEp);
    if (anchor)
    {
        pl->anchor = anchor;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Can't determine anchor");
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
#ifdef TCP_ADAPTER
    pl->tcpPort = tcpPort;
#endif

    return pl;
}

#ifndef TCP_ADAPTER
void OC_CALL OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                           uint16_t securePort)
{
    OCDiscoveryPayloadAddNewResource(payload, OCCopyResource(res, securePort, NULL, 0, NULL));
}
#else
void OC_CALL OCDiscoveryPayloadAddResource(OCDiscoveryPayload* payload, const OCResource* res,
                                           uint16_t securePort, uint16_t tcpPort)
{
    OCDiscoveryPayloadAddNewResource(payload, OCCopyResource(res, securePort, NULL, 0, NULL,
                                                             tcpPort));
}
#endif

#ifndef TCP_ADAPTER
void OCDiscoveryPayloadAddResourceWithEps(OCDiscoveryPayload* payload, const OCResource* res,
                                                  uint16_t securePort, void *networkInfo, size_t infoSize,
                                                  const OCDevAddr *devAddr)
{
    OCDiscoveryPayloadAddNewResource(payload,
                                     OCCopyResource(res, securePort, (CAEndpoint_t *)networkInfo,
                                                    infoSize, devAddr));
}
#else
void OCDiscoveryPayloadAddResourceWithEps(OCDiscoveryPayload* payload, const OCResource* res,
                                                  uint16_t securePort, void *networkInfo, size_t infoSize,
                                                  const OCDevAddr *devAddr, uint16_t tcpPort)
{
    OCDiscoveryPayloadAddNewResource(payload,
                                     OCCopyResource(res, securePort, (CAEndpoint_t *)networkInfo,
                                                    infoSize, devAddr, tcpPort));
}
#endif

bool OC_CALL OCResourcePayloadAddStringLL(OCStringLL **stringLL, const char *value)
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

void OC_CALL OCDiscoveryPayloadAddNewResource(OCDiscoveryPayload* payload, OCResourcePayload* res)
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

void OC_CALL OCDiscoveryEndpointDestroy(OCEndpointPayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICFree(payload->tps);
    OICFree(payload->addr);
    OCDiscoveryEndpointDestroy(payload->next);
    OICFree(payload);
}

void OC_CALL OCDiscoveryResourceDestroy(OCResourcePayload* payload)
{
    if (!payload)
    {
        return;
    }

    OICFree(payload->uri);
    OICFree(payload->rel);
    OICFree(payload->anchor);
    OCFreeOCStringLL(payload->types);
    OCFreeOCStringLL(payload->interfaces);
    OCDiscoveryEndpointDestroy(payload->eps);
    OCDiscoveryResourceDestroy(payload->next);
    OICFree(payload);
}

void OC_CALL OCDiscoveryPayloadDestroy(OCDiscoveryPayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->sid);
    OCFreeOCStringLL(payload->type);
    OICFree(payload->name);
    OCFreeOCStringLL(payload->iface);
    OCDiscoveryResourceDestroy(payload->resources);
    OCDiscoveryPayloadDestroy(payload->next);
    OICFree(payload);
}

OCPresencePayload* OC_CALL OCPresencePayloadCreate(uint32_t seqNum, uint32_t maxAge,
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

void OC_CALL OCPresencePayloadDestroy(OCPresencePayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->resourceType);
    OICFree(payload);
}

OCDiagnosticPayload* OC_CALL OCDiagnosticPayloadCreate(const char* message)
{
    if (!message)
    {
        return NULL;
    }

    OCDiagnosticPayload* payload = (OCDiagnosticPayload*)OICCalloc(1, sizeof(OCDiagnosticPayload));
    if (!payload)
    {
        return NULL;
    }

    payload->base.type = PAYLOAD_TYPE_DIAGNOSTIC;
    payload->message = OICStrdup(message);
    return payload;
}

void OC_CALL OCDiagnosticPayloadDestroy(OCDiagnosticPayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->message);
    OICFree(payload);
}

void OC_CALL OCEndpointPayloadDestroy(OCEndpointPayload* payload)
{
    if (!payload)
    {
        return;
    }
    OICFree(payload->tps);
    OICFree(payload->addr);
    OCEndpointPayloadDestroy(payload->next);
    OICFree(payload);
}

OCRepPayload** OC_CALL OCLinksPayloadArrayCreate(const char* resourceUri,
                       OCEntityHandlerRequest *ehRequest, bool insertSelfLink, size_t* createdArraySize)
{
    OIC_LOG(DEBUG, TAG, "OCLinksPayloadValueCreate");
    OCRepPayload** linksRepPayloadArray = NULL;
    if ((resourceUri != NULL) && (ehRequest != NULL))
    {
        OCPayloadFormat contentFormat = OC_FORMAT_UNDEFINED;
        if ((OC_STACK_OK != OCGetRequestPayloadVersion(ehRequest, &contentFormat, NULL)) &&
            (contentFormat == OC_FORMAT_VND_OCF_CBOR || contentFormat == OC_FORMAT_CBOR))
            return NULL;

        linksRepPayloadArray = BuildCollectionLinksPayloadArray(resourceUri, contentFormat, &ehRequest->devAddr,
            insertSelfLink, createdArraySize);

        OIC_LOG_V(DEBUG, TAG, "return value of BuildCollectionLinksPayloadArray() = %s",
                 (linksRepPayloadArray != NULL) ? "true" : "false");
    }
    return linksRepPayloadArray;
}

OCStackResult OC_CALL OCGetRequestPayloadVersion(OCEntityHandlerRequest *ehRequest,
                                  OCPayloadFormat* pContentFormat, uint16_t* pAcceptVersion)
{
    if ((ehRequest == NULL)||(pContentFormat == NULL))
        return OC_STACK_ERROR;

    OCServerRequest* serverRequest = (OCServerRequest*) ehRequest->requestHandle;
    switch (serverRequest->acceptFormat)
    {
        case OC_FORMAT_CBOR:
        case OC_FORMAT_VND_OCF_CBOR:
        case OC_FORMAT_JSON:
        case OC_FORMAT_UNDEFINED:
        case OC_FORMAT_UNSUPPORTED:
            *pContentFormat = serverRequest->acceptFormat;
            OIC_LOG_V(INFO, TAG,
                      "Content format is %d, application/cbor = 0, application/vnd.ocf+cbor = 1",
                      (int) *pContentFormat);
            break;
        default:
            return OC_STACK_INVALID_OPTION;
    }

    // accepting NULL input parameter in case version is not required
    if (pAcceptVersion == NULL)
    {
        return OC_STACK_OK;
    }

    uint8_t vOptionData[MAX_HEADER_OPTION_DATA_LENGTH];
    size_t vOptionDataSize = sizeof(vOptionData);
    uint16_t actualDataSize = 0;

    OCGetHeaderOption(ehRequest->rcvdVendorSpecificHeaderOptions,
                      ehRequest->numRcvdVendorSpecificHeaderOptions,
                      COAP_OPTION_ACCEPT_VERSION, vOptionData, vOptionDataSize, &actualDataSize);

    // Check if "OCF-Accept-Content-Format-Version" is present,
    // and size of its value is as expected (2 bytes).
    if (actualDataSize != 2)
    {
        return OC_STACK_INVALID_OPTION;
    }

    *pAcceptVersion = vOptionData[0] * 256 + vOptionData[1];

    return OC_STACK_OK;
}
