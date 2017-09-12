/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "ocendpoint.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include <string.h>
#include "cainterface.h"

#define VERIFY_NON_NULL(arg) { if (!arg) {OIC_LOG(FATAL, TAG, #arg " is NULL"); goto exit;} }
#define VERIFY_GT_ZERO(arg) { if (arg < 1) {OIC_LOG(FATAL, TAG, #arg " < 1"); goto exit;} }
#define VERIFY_GT(arg1, arg2) { if (arg1 <= arg2) {OIC_LOG(FATAL, TAG, #arg1 " <= " #arg2); goto exit;} }
#define VERIFY_LT_OR_EQ(arg1, arg2) { if (arg1 > arg2) {OIC_LOG(FATAL, TAG, #arg1 " > " #arg2); goto exit;} }
#define VERIFY_SNPRINTF_RET(arg1, arg2) \
    { if (0 > arg1 || arg1 >= arg2) {OIC_LOG(FATAL, TAG, "Error (snprintf)"); goto exit;} } \

#define TAG  "OIC_RI_ENDPOINT"

OCStackResult OCGetSupportedEndpointFlags(const OCTpsSchemeFlags givenFlags, OCTpsSchemeFlags* out)
{
    if (!out)
    {
        return OC_STACK_INVALID_PARAM;
    }

    CATransportAdapter_t SelectedNetwork = CAGetSelectedNetwork();
    if ((givenFlags & (OC_COAP | OC_COAPS)) && (SelectedNetwork & CA_ADAPTER_IP))
    {
        *out = (OCTpsSchemeFlags)(*out | OC_COAP);

        if (OC_SECURE)
        {
            *out = (OCTpsSchemeFlags)(*out | OC_COAPS);
        }
    }
#ifdef TCP_ADAPTER
    if ((givenFlags & (OC_COAP_TCP | OC_COAPS_TCP)) && (SelectedNetwork & CA_ADAPTER_TCP))
    {

        *out = (OCTpsSchemeFlags)(*out | OC_COAP_TCP);

        if (OC_SECURE)
        {
            *out = (OCTpsSchemeFlags)(*out | OC_COAPS_TCP);
        }
    }
#endif
#ifdef EDR_ADAPTER
    if ((givenFlags & OC_COAP_RFCOMM) && (SelectedNetwork & CA_ADAPTER_RFCOMM_BTEDR))
    {
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_RFCOMM);
    }
#endif
#ifdef LE_ADAPTER
    if ((givenFlags & OC_COAP_GATT) && (SelectedNetwork & CA_ADAPTER_GATT_BTLE))
    {
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_GATT);
    }
#endif
#ifdef NFC_ADAPTER
    if ((givenFlags & OC_COAP_NFC) && (SelectedNetwork & CA_ADAPTER_NFC))
    {
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_NFC);
    }
#endif
#ifdef RA_ADAPTER
    if ((givenFlags & OC_COAP_RA) && (SelectedNetwork & CA_ADAPTER_REMOTE_ACCESS))
    {
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_RA);
    }
#endif

    return OC_STACK_OK;
}

OCStackResult OCGetMatchedTpsFlags(const CATransportAdapter_t adapter,
                                   const CATransportFlags_t flags,
                                   OCTpsSchemeFlags* out)
{
    // return matched TPS Flags with given CA flags.
    if (!out)
    {
        OIC_LOG(ERROR, TAG, "out param is NULL!!!");
        return OC_STACK_INVALID_PARAM;
    }

    if ((adapter & OC_ADAPTER_IP) && (flags & (OC_IP_USE_V4 | OC_IP_USE_V6)))
    {
        if (flags & OC_FLAG_SECURE)
        {
            // OC_COAPS
            // typecasting to support C90(arduino)
            *out = (OCTpsSchemeFlags)(*out | OC_COAPS);
        }
        else
        {   // OC_COAP
            // typecasting to support C90(arduino)
            *out = (OCTpsSchemeFlags)(*out | OC_COAP);
        }
    }

#ifdef TCP_ADAPTER
    if ((adapter & OC_ADAPTER_TCP) && (flags & (OC_IP_USE_V4 | OC_IP_USE_V6)))
    {
        if (flags & OC_FLAG_SECURE)
        {
            // OC_COAPS_TCP
            // typecasting to support C90(arduino)
            *out = (OCTpsSchemeFlags)(*out | OC_COAPS_TCP);
        }
        else
        {
            // OC_COAP_TCP
            // typecasting to support C90(arduino)
            *out = (OCTpsSchemeFlags)(*out | OC_COAP_TCP);
        }
    }
#endif
#ifdef HTTP_ADAPTER
    // OC_HTTP
    // @todo iotivity not support http transport yet...

    // OC_HTTPS
    // @todo iotivity not support https transport yet...
#endif
#ifdef EDR_ADAPTER
    // OC_COAP_RFCOMM
    if (adapter & OC_ADAPTER_RFCOMM_BTEDR)
    {
        // typecasting to support C90(arduino)
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_RFCOMM);
    }
#endif
#ifdef LE_ADAPTER
    // OC_COAP_GATT
    if (adapter & CA_ADAPTER_GATT_BTLE)
    {
        // typecasting to support C90(arduino)
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_GATT);
    }
#endif
#ifdef NFC_ADAPTER
    // OC_COAP_NFC
    if (adapter & CA_ADAPTER_NFC)
    {
        // typecasting to support C90(arduino)
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_NFC);
    }
#endif
#ifdef RA_ADAPTER
    // OC_COAP_RA
    if (adapter & CA_ADAPTER_REMOTE_ACCESS)
    {
        // typecasting to support C90(arduino)
        *out = (OCTpsSchemeFlags)(*out | OC_COAP_RA);
    }
#endif
    return OC_STACK_OK;
}

static const char *ConvertTpsToString(const OCTpsSchemeFlags tps)
{
    switch (tps)
    {
        case OC_COAP:
            return COAP_STR;

        case OC_COAPS:
            return COAPS_STR;
#ifdef TCP_ADAPTER
        case OC_COAP_TCP:
            return COAP_TCP_STR;

        case OC_COAPS_TCP:
            return COAPS_TCP_STR;
#endif
#ifdef HTTP_ADAPTER
        case OC_HTTP:
            return HTTP_STR;

        case OC_HTTPS:
            return HTTPS_STR;
#endif
#ifdef EDR_ADAPTER
        case OC_COAP_RFCOMM:
            return COAP_RFCOMM_STR;
#endif
        default:
            return NULL;
    }
}

OCStackResult OCConvertTpsToString(const OCTpsSchemeFlags tps, char** out)
{
    // return given tps as string for payload
    // OC_COAP_IPV4 -> OC_COAP

    if (!out)
    {
        return OC_STACK_INVALID_PARAM;
    }

    *out = OICStrdup(ConvertTpsToString(tps));
    VERIFY_NON_NULL(*out);
    return OC_STACK_OK;

exit:
    return OC_STACK_NO_MEMORY;
}

char* OCCreateEndpointString(const OCEndpointPayload* endpoint)
{
    if (!endpoint)
    {
        return NULL;
    }

    char* buf = (char*)OICCalloc(MAX_ADDR_STR_SIZE, sizeof(char));
    VERIFY_NON_NULL(buf);

    if ((strcmp(endpoint->tps, COAP_STR) == 0) || (strcmp(endpoint->tps, COAPS_STR) == 0)
#ifdef TCP_ADAPTER
        || (strcmp(endpoint->tps, COAP_TCP_STR) == 0) ||(strcmp(endpoint->tps, COAPS_TCP_STR) == 0)
#endif
#ifdef HTTP_ADAPTER
        || (strcmp(endpoint->tps, HTTP_STR) == 0) ||(strcmp(endpoint->tps, HTTPS_STR) == 0)
#endif
        )
    {
        // checking addr is ipv4 or not
        if (endpoint->family & OC_IP_USE_V4)
        {
            // ipv4
            int snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://%s:%d", endpoint->tps,
                                  endpoint->addr, endpoint->port);
            VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
        }
        else
        {
            // ipv6
            int snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://[%s]:%d", endpoint->tps,
                                  endpoint->addr, endpoint->port);
            VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
        }
    }
#ifdef EDR_ADAPTER
    else if ((strcmp(endpoint->tps, COAP_RFCOMM_STR) == 0))
    {
        // coap+rfcomm
        int snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://%s",
                              endpoint->tps, endpoint->addr);
        VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
    }
#endif
    else
    {
        OIC_LOG_V(ERROR, TAG, "Payload has invalid TPS!!! %s", endpoint->tps);
        goto exit;
    }
    return buf;

exit:
    OICFree(buf);
    return NULL;
}

char* OC_CALL OCCreateEndpointStringFromCA(const CAEndpoint_t* endpoint)
{
    if (!endpoint || 0 == strlen(endpoint->addr))
    {
        return NULL;
    }

    int snRet = -1;
    char *buf = NULL;
    OCTpsSchemeFlags tps = OC_NO_TPS;
    OCStackResult result = OCGetMatchedTpsFlags(endpoint->adapter, endpoint->flags, &tps);
    if (OC_STACK_OK != result)
    {
        goto exit;
    }

    buf = (char*)OICCalloc(MAX_ADDR_STR_SIZE, sizeof(char));
    VERIFY_NON_NULL(buf);

    switch (tps)
    {
    case OC_COAP: case OC_COAPS:
#ifdef TCP_ADAPTER
    case OC_COAP_TCP: case OC_COAPS_TCP:
#endif
#ifdef HTTP_ADAPTER
    case OC_HTTP: case OC_HTTPS:
#endif
        if (!endpoint->port)
        {
            goto exit;
        }
        // checking addr is ipv4 or not
        if (endpoint->flags & CA_IPV4)
        {
            // ipv4
            snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://%s:%d", ConvertTpsToString(tps),
                                  endpoint->addr, endpoint->port);
            VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
        }
        else
        {
            // ipv6
            snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://[%s]:%d", ConvertTpsToString(tps),
                                  endpoint->addr, endpoint->port);
            VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
        }
        break;
#ifdef EDR_ADAPTER
    case OC_COAP_RFCOMM:
        // coap+rfcomm
        snRet = snprintf(buf, MAX_ADDR_STR_SIZE, "%s://%s", ConvertTpsToString(tps), endpoint->addr);
        VERIFY_SNPRINTF_RET(snRet, MAX_ADDR_STR_SIZE);
        break;
#endif
    default:
        OIC_LOG_V(ERROR, TAG, "Payload has invalid TPS!!! %d", tps);
        goto exit;
    }
    return buf;

exit:
    OICFree(buf);
    return NULL;
}

OCStackResult OCParseEndpointString(const char* endpointStr, OCEndpointPayload* out)
{
    if (!endpointStr || !out)
    {
        return OC_STACK_INVALID_PARAM;
    }

    char* tmp = NULL;
    void* ret = NULL;
    char* tps = NULL;
    char* addr = NULL;
    char* origin = NULL;
    char* tokPos = NULL;
    size_t tpsCharsToWrite = 0;
    size_t addrCharsToWrite = 0;
    OCStackResult isEnabledAdapter = OC_STACK_ADAPTER_NOT_ENABLED;
    OCTransportAdapter parsedAdapter = OC_DEFAULT_ADAPTER;
    bool isSecure = false;

    tps = (char*)OICCalloc(OC_MAX_TPS_STR_SIZE, sizeof(char));
    VERIFY_NON_NULL(tps);

    addr = (char*)OICCalloc(OC_MAX_ADDR_STR_SIZE, sizeof(char));
    VERIFY_NON_NULL(addr);

    origin = OICStrdup(endpointStr);
    VERIFY_NON_NULL(origin);

    // token start pos
    tokPos = strstr(origin, OC_ENDPOINT_TPS_TOKEN);
    VERIFY_NON_NULL(tokPos);

    // copy tps
    tpsCharsToWrite = tokPos - origin;
    VERIFY_GT_ZERO(tpsCharsToWrite);
    VERIFY_GT((size_t)OC_MAX_TPS_STR_SIZE, tpsCharsToWrite);
    ret = memcpy(tps, origin, tpsCharsToWrite);
    VERIFY_NON_NULL(ret);
    OIC_LOG_V(INFO, TAG, "parsed tps is:%s", tps);

    // check tps type
    if (strcmp(tps, COAP_STR) == 0)
    {
        isEnabledAdapter = OC_STACK_OK;
        parsedAdapter = OC_ADAPTER_IP;
    }
    else if (strcmp(tps, COAPS_STR) == 0)
    {
        isEnabledAdapter = OC_STACK_OK;
        parsedAdapter = OC_ADAPTER_IP;
        isSecure = true;
    }
#ifdef TCP_ADAPTER
    else if (strcmp(tps, COAP_TCP_STR) == 0)
    {
        isEnabledAdapter = OC_STACK_OK;
        parsedAdapter = OC_ADAPTER_TCP;
    }
    else if (strcmp(tps, COAPS_TCP_STR) == 0)
    {
        isEnabledAdapter = OC_STACK_OK;
        parsedAdapter = OC_ADAPTER_TCP;
        isSecure = true;
    }
#endif
#ifdef HTTP_ADAPTER
    // @todo iotivity not support http, https transport yet...
#endif
#ifdef EDR_ADAPTER
    else if (strcmp(tps, COAP_RFCOMM_STR) == 0)
    {
        isEnabledAdapter = OC_STACK_OK;
        parsedAdapter = OC_ADAPTER_RFCOMM_BTEDR;
    }
#endif
    // ignore unrecognized tps type
    if (isEnabledAdapter == OC_STACK_ADAPTER_NOT_ENABLED
        && parsedAdapter == OC_DEFAULT_ADAPTER)
    {
        OICFree(tps);
        OICFree(addr);
        OICFree(origin);
        return OC_STACK_ADAPTER_NOT_ENABLED;
    }

#ifdef EDR_ADAPTER
    if (parsedAdapter == OC_ADAPTER_RFCOMM_BTEDR)
    {
        // copy addr
        tokPos = tokPos + 3;
        ret = OICStrcpy(addr, OC_MAX_ADDR_STR_SIZE, tokPos);
        VERIFY_NON_NULL(ret);
        out->tps = tps;
        out->addr = addr;
        out->family = OC_DEFAULT_FLAGS;
        out->port = 0;
    }
    else
#endif
    {
        // first check epl has square bracket
        tmp = strchr(origin, OC_ENDPOINT_BRACKET_START);
        if (tmp)
        {
            out->family = OC_IP_USE_V6;
            tokPos = tokPos + 4;
            tmp = strrchr(origin, OC_ENDPOINT_BRACKET_END);
        }
        else
        {
            out->family = OC_IP_USE_V4;
            tokPos = tokPos + 3;
            tmp = strrchr(origin, OC_ENDPOINT_ADDR_TOKEN);
        }
        VERIFY_NON_NULL(tmp);
        if (isSecure)
        {
            out->family = (OCTransportFlags)(out->family | OC_FLAG_SECURE);
        }

        // copy addr
        addrCharsToWrite = tmp - tokPos;
        VERIFY_GT_ZERO(addrCharsToWrite);
        VERIFY_GT((size_t)OC_MAX_ADDR_STR_SIZE, addrCharsToWrite);
        ret = memcpy(addr, tokPos, addrCharsToWrite);
        VERIFY_NON_NULL(ret);
        OIC_LOG_V(INFO, TAG, "parsed addr is:%s", addr);

        tmp = strrchr(origin, OC_ENDPOINT_ADDR_TOKEN);
        VERIFY_NON_NULL(tmp);

        // port start pos
        tokPos = tmp + 1;
        char* end = NULL;
        long port = strtol(tokPos, &end, 10);
        VERIFY_GT_ZERO(port);
        VERIFY_LT_OR_EQ(port, UINT16_MAX);
        OIC_LOG_V(INFO, TAG, "parsed port is:%s", tokPos);

        out->tps = tps;
        out->addr = addr;
        out->port = (uint16_t)port;
    }

    OICFree(origin);
    origin = NULL;
    tokPos = NULL;
    tmp = NULL;
    ret = NULL;

    return OC_STACK_OK;
exit:
    OICFree(origin);
    OICFree(tps);
    OICFree(addr);
    origin = NULL;
    tokPos = NULL;
    tmp = NULL;
    ret = NULL;
    tps = NULL;
    addr = NULL;

    return OC_STACK_ERROR;
}

OCTpsSchemeFlags OCGetSupportedTpsFlags()
{
    OCTpsSchemeFlags ret = OC_NO_TPS;
    CATransportAdapter_t SelectedNetwork = CAGetSelectedNetwork();

    if (SelectedNetwork & CA_ADAPTER_IP)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP);

        if (OC_SECURE)
        {
            ret = (OCTpsSchemeFlags)(ret | OC_COAPS);
        }
    }
#ifdef TCP_ADAPTER
    if (SelectedNetwork & CA_ADAPTER_TCP)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP_TCP);

        if (OC_SECURE)
        {
            ret = (OCTpsSchemeFlags)(ret | OC_COAPS_TCP);
        }
    }
#endif
#ifdef EDR_ADAPTER
    if (SelectedNetwork & CA_ADAPTER_RFCOMM_BTEDR)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP_RFCOMM);
    }
#endif
#ifdef LE_ADAPTER
    if (SelectedNetwork & CA_ADAPTER_GATT_BTLE)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP_GATT);
    }
#endif
#ifdef NFC_ADAPTER
    if (SelectedNetwork & CA_ADAPTER_NFC)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP_NFC);
    }
#endif
#ifdef RA_ADAPTER
    if (SelectedNetwork & CA_ADAPTER_REMOTE_ACCESS)
    {
        ret = (OCTpsSchemeFlags)(ret | OC_COAP_RA);
    }
#endif
    return ret;
}
