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

#ifndef PAYLOAD_LOGGING_H_
#define PAYLOAD_LOGGING_H_

#include "logger.h"
#ifdef __TIZEN__
#include <dlog.h>
#endif

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <inttypes.h>
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocstack.h"

#ifdef __cplusplus
extern "C"
{
#endif

// PL_TAG is made as generic predefined tag because of build problems in arduino for using logging
#define PL_TAG "PayloadLog"

#ifdef TB_LOG
    #define OIC_LOG_PAYLOAD(level, payload) OCPayloadLog((level),(payload))
    #define UUID_SIZE (16)

const char *convertTriggerEnumToString(OCPresenceTrigger trigger);
OCPresenceTrigger convertTriggerStringToEnum(const char * triggerStr);

INLINE_API void OCPayloadLogRep(LogLevel level, OCRepPayload* payload);

INLINE_API void OCPayloadLogRepValues(LogLevel level, OCRepPayloadValue* val)
{
    while (val)
    {
        switch(val->type)
        {
            case OCREP_PROP_NULL:
                OIC_LOG_V(level, PL_TAG, "\t\t%s: NULL", val->name);
                break;
            case OCREP_PROP_INT:
                OIC_LOG_V(level, PL_TAG, "\t\t%s(int):%zd", val->name, val->i);
                break;
            case OCREP_PROP_DOUBLE:
                OIC_LOG_V(level, PL_TAG, "\t\t%s(double):%f", val->name, val->d);
                break;
            case OCREP_PROP_BOOL:
                OIC_LOG_V(level, PL_TAG, "\t\t%s(bool):%s", val->name, val->b ? "true" : "false");
                break;
            case OCREP_PROP_STRING:
                OIC_LOG_V(level, PL_TAG, "\t\t%s(string):%s", val->name, val->str);
                break;
            case OCREP_PROP_BYTE_STRING:
                OIC_LOG_V(level, PL_TAG, "\t\t%s(binary):", val->name);
                OIC_LOG_BUFFER(level, PL_TAG, val->ocByteStr.bytes, val->ocByteStr.len);
                break;
            case OCREP_PROP_OBJECT:
                // Note: Only prints the URI (if available), to print further, you'll
                // need to dig into the object better!
                OIC_LOG_V(level, PL_TAG, "\t\t%s(object):", val->name);
                OCPayloadLogRep(level, val->obj);
                break;
            case OCREP_PROP_ARRAY:
                switch(val->arr.type)
                {
                    case OCREP_PROP_INT:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(int array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");
                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OIC_LOG_V(level, PL_TAG, "\t\t\t %zd", val->arr.iArray[i]);
                        }
                        break;
                    case OCREP_PROP_DOUBLE:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(double array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");
                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OIC_LOG_V(level, PL_TAG, "\t\t\t %lf", val->arr.dArray[i]);
                        }
                        break;
                    case OCREP_PROP_BOOL:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(bool array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");
                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OIC_LOG_V(level, PL_TAG, "\t\t\t %d", val->arr.bArray[i]);
                        }
                        break;
                    case OCREP_PROP_STRING:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(string array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");
                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OIC_LOG_V(level, PL_TAG, "\t\t\t %s", val->arr.strArray[i]);
                        }
                        break;
                    case OCREP_PROP_BYTE_STRING:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(byte array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");
                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OIC_LOG_BUFFER(level, PL_TAG, val->arr.ocByteStrArray[i].bytes, val->arr.ocByteStrArray[i].len);
                        }
                        break;
                    case OCREP_PROP_OBJECT:
                        OIC_LOG_V(level, PL_TAG, "\t\t%s(object array):%zu x %zu x %zu: ",
                                val->name,
                                val->arr.dimensions[0], val->arr.dimensions[1],
                                val->arr.dimensions[2]);
                        OIC_LOG(level, PL_TAG, "\t\t Values:");

                        for (size_t i = 0; i < val->arr.dimensions[0]; i++)
                        {
                            OCPayloadLogRep(level, val->arr.objArray[i]);
                        }
                        break;
                    case OCREP_PROP_ARRAY: //Seems as nested arrays doesn't not supported in API
                    default:
                        OIC_LOG_V(ERROR, PL_TAG, "%s <-- Unknown/unsupported array type!",
                                val->name);
                        break;
                }
                break;
            default:
                OIC_LOG_V(ERROR, PL_TAG, "%s <-- Unknown type!", val->name);
                break;
        }
        val = val -> next;
    }
}

INLINE_API void OCPayloadLogRep(LogLevel level, OCRepPayload* payload)
{
    OIC_LOG(level, (PL_TAG), "Payload Type: Representation");
    int i = 1;
    for (OCRepPayload* rep = payload; rep; rep = rep->next, ++i)
    {
        OIC_LOG_V(level, PL_TAG, "\tResource #%d", i);
        if (rep->uri)
        {
            OIC_LOG_V(level, PL_TAG, "\tURI:%s", rep->uri);
        }
        if (rep->types)
        {
            OIC_LOG(level, PL_TAG, "\tResource Types:");
            for (OCStringLL* strll = rep->types; strll; strll = strll->next)
            {
                OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
            }
        }
        if (rep->interfaces)
        {
            OIC_LOG(level, PL_TAG, "\tInterfaces:");
            for (OCStringLL* strll = rep->interfaces; strll; strll = strll->next)
            {
                OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
            }
        }
        OIC_LOG(level, PL_TAG, "\tValues:");
        OCPayloadLogRepValues(level, rep->values);
    }
}

static void OCStringLLPrint(LogLevel level, OCStringLL *type)
{
    for (OCStringLL *strll = type; strll; strll = strll->next)
    {
        OIC_LOG_V(level, PL_TAG, "\t\t %s", strll->value);
    }
}

INLINE_API void OCPayloadLogDiscovery(LogLevel level, OCDiscoveryPayload* payload)
{
    OIC_LOG(level, PL_TAG, "Payload Type: Discovery");

    while(payload && payload->resources)
    {
        OIC_LOG_V(level, PL_TAG, "\tDI: %s", payload->sid);
        if (payload->baseURI)
        {
            OIC_LOG_V(level, PL_TAG, "\tBase URI:%s", payload->baseURI);
        }
        if (payload->name)
        {
            OIC_LOG_V(level, PL_TAG, "\tNAME: %s", payload->name);
        }

        if (payload->type)
        {
            OIC_LOG(level, PL_TAG, "\tResource Type:");
            OCStringLLPrint(level, payload->type);
        }

        if (payload->iface)
        {
            OIC_LOG(level, PL_TAG, "\tInterface:");
            OCStringLLPrint(level, payload->iface);
        }

        OCResourcePayload* res = payload->resources;

        int i = 1;
        while(res)
        {
            OIC_LOG_V(level, PL_TAG, "\tLink#%d", i);
            OIC_LOG_V(level, PL_TAG, "\tURI:%s", res->uri);
            OIC_LOG(level, PL_TAG, "\tResource Types:");
            OCStringLL* strll =  res->types;
            while(strll)
            {
                OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
                strll = strll->next;
            }
            OIC_LOG(level, PL_TAG, "\tInterfaces:");
            strll =  res->interfaces;
            while(strll)
            {
                OIC_LOG_V(level, PL_TAG, "\t\t%s", strll->value);
                strll = strll->next;
            }

            OIC_LOG_V(level, PL_TAG, "\tBitmap: %u", res->bitmap);
            OIC_LOG_V(level, PL_TAG, "\tSecure?: %s", res->secure ? "true" : "false");
            OIC_LOG_V(level, PL_TAG, "\tPort: %u", res->port);
            OIC_LOG(level, PL_TAG, "");
            res = res->next;
            ++i;
        }
        payload = payload->next;
    }
}

INLINE_API void OCPayloadLogPresence(LogLevel level, OCPresencePayload* payload)
{
    OIC_LOG(level, PL_TAG, "Payload Type: Presence");
    OIC_LOG_V(level, PL_TAG, "\tSequence Number:%u", payload->sequenceNumber);
    OIC_LOG_V(level, PL_TAG, "\tMax Age:%d", payload->maxAge);
    OIC_LOG_V(level, PL_TAG, "\tTrigger:%s", convertTriggerEnumToString(payload->trigger));
    OIC_LOG_V(level, PL_TAG, "\tResource Type:%s", payload->resourceType);
}

INLINE_API void OCPayloadLogSecurity(LogLevel level, OCSecurityPayload* payload)
{
    size_t payloadSize = payload->payloadSize;
    OIC_LOG(level, PL_TAG, "Payload Type: Security");

    if (payloadSize > 0)
    {
        // Add a zero-character string terminator.
        char *securityData = (char *)OICMalloc(payloadSize + 1);

        if (securityData)
        {
            memcpy(securityData, payload->securityData, payloadSize);
            // assert(securityData[payloadSize - 1] != '\0');
            securityData[payloadSize] = '\0';
            OIC_LOG_V(level, PL_TAG, "\tSecurity Data: %s", securityData);
            OICFree(securityData);
        }
    }
}

INLINE_API void OCPayloadLog(LogLevel level, OCPayload* payload)
{
    if(!payload)
    {
        OIC_LOG(level, PL_TAG, "NULL Payload");
        return;
    }
    switch(payload->type)
    {
        case PAYLOAD_TYPE_REPRESENTATION:
            OCPayloadLogRep(level, (OCRepPayload*)payload);
            break;
        case PAYLOAD_TYPE_DISCOVERY:
            OCPayloadLogDiscovery(level, (OCDiscoveryPayload*)payload);
            break;
        case PAYLOAD_TYPE_PRESENCE:
            OCPayloadLogPresence(level, (OCPresencePayload*)payload);
            break;
        case PAYLOAD_TYPE_SECURITY:
            OCPayloadLogSecurity(level, (OCSecurityPayload*)payload);
            break;
        default:
            OIC_LOG_V(level, PL_TAG, "Unknown Payload Type: %d", payload->type);
            break;
    }
}
#else
    #define OIC_LOG_PAYLOAD(level, payload)
#endif

#ifdef __cplusplus
}
#endif

#endif
