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
#define _POSIX_C_SOURCE 200112L
#include <string.h>

#include "srmutility.h"
#include "srmresourcestrings.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "base64.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "cacommon.h"
#include "casecurityinterface.h"

#define TAG  "OIC_SRM_UTILITY"

void ParseQueryIterInit(const unsigned char * query, OicParseQueryIter_t * parseIter)
{
    OIC_LOG(INFO, TAG, "Initializing coap iterator");
    if ((NULL == query) || (NULL == parseIter))
    {
        return;
    }

    parseIter->attrPos = NULL;
    parseIter->attrLen = 0;
    parseIter->valPos = NULL;
    parseIter->valLen = 0;
    coap_parse_iterator_init((unsigned char *)query, strlen((char *)query),
                             (unsigned char *)OIC_SEC_REST_QUERY_SEPARATOR,
                             (unsigned char *) "", 0, &parseIter->pi);
}

OicParseQueryIter_t * GetNextQuery(OicParseQueryIter_t * parseIter)
{
    OIC_LOG(INFO, TAG, "Getting Next Query");
    if (NULL == parseIter)
    {
        return NULL;
    }

    unsigned char * qrySeg = NULL;
    char * delimPos;

    // Get the next query. Querys are separated by OIC_SEC_REST_QUERY_SEPARATOR.
    qrySeg = coap_parse_next(&parseIter->pi);

    if (qrySeg)
    {
        delimPos = strchr((char *)qrySeg, OIC_SEC_REST_QUERY_DELIMETER);
        if (delimPos)
        {
            parseIter->attrPos = parseIter->pi.pos;
            parseIter->attrLen = (unsigned char *)delimPos - parseIter->pi.pos;
            parseIter->valPos  = (unsigned char *)delimPos + 1;
            parseIter->valLen  = &qrySeg[parseIter->pi.segment_length] - parseIter->valPos;
            return parseIter;
        }
    }
    return NULL;
}

// TODO This functionality is replicated in all SVR's and therefore we need
// to encapsulate it in a common method. However, this may not be the right
// file for this method.
OCStackResult AddUuidArray(const cJSON* jsonRoot, const char* arrayItem,
                           size_t *numUuids, OicUuid_t** uuids)
{
    size_t idxx = 0;
    cJSON* jsonObj = cJSON_GetObjectItem((cJSON *)jsonRoot, arrayItem);
    VERIFY_NON_NULL(TAG, jsonObj, ERROR);
    VERIFY_SUCCESS(TAG, cJSON_Array == jsonObj->type, ERROR);

    *numUuids = (size_t)cJSON_GetArraySize(jsonObj);
    VERIFY_SUCCESS(TAG, *numUuids > 0, ERROR);
    *uuids = (OicUuid_t*)OICCalloc(*numUuids, sizeof(OicUuid_t));
    VERIFY_NON_NULL(TAG, *uuids, ERROR);

    do
    {
        unsigned char base64Buff[sizeof(((OicUuid_t*)0)->id)] = {0};
        uint32_t outLen = 0;
        B64Result b64Ret = B64_OK;

        cJSON *jsonOwnr = cJSON_GetArrayItem(jsonObj, idxx);
        VERIFY_NON_NULL(TAG, jsonOwnr, ERROR);
        VERIFY_SUCCESS(TAG, cJSON_String == jsonOwnr->type, ERROR);

        outLen = 0;
        b64Ret = b64Decode(jsonOwnr->valuestring, strlen(jsonOwnr->valuestring), base64Buff,
               sizeof(base64Buff), &outLen);

        VERIFY_SUCCESS(TAG, (b64Ret == B64_OK && outLen <= sizeof((*uuids)[idxx].id)),
               ERROR);
        memcpy((*uuids)[idxx].id, base64Buff, outLen);
    } while ( ++idxx < *numUuids);

    return OC_STACK_OK;

exit:
    return OC_STACK_ERROR;

}

/**
 * Function to getting string of ownership transfer method
 *
 * @prarm oxmType ownership transfer method
 *
 * @return string value of ownership transfer method
 */
const char* GetOxmString(OicSecOxm_t oxmType)
{
    switch(oxmType)
    {
        case OIC_JUST_WORKS:
            return OXM_JUST_WORKS;
        case OIC_RANDOM_DEVICE_PIN:
            return OXM_RANDOM_DEVICE_PIN;
        case OIC_MANUFACTURER_CERTIFICATE:
            return OXM_MANUFACTURER_CERTIFICATE;
#ifdef MULTIPLE_OWNER
        case OIC_PRECONFIG_PIN:
            return OXM_PRECONF_PIN;
#endif //MULTIPLE_OWNER
        case OIC_MV_JUST_WORKS:
            return OXM_MV_JUST_WORKS;
        case OIC_CON_MFG_CERT:
            return OXM_CON_MFG_CERT;
        default:
            return NULL;
    }
}

OCStackResult ConvertUuidToStr(const OicUuid_t* uuid, char** strUuid)
{
    if(NULL == uuid || NULL == strUuid || NULL != *strUuid)
    {
        OIC_LOG(ERROR, TAG, "ConvertUuidToStr : Invalid param");
        return OC_STACK_INVALID_PARAM;
    }

    size_t uuidIdx = 0;
    size_t urnIdx = 0;
    const size_t urnBufSize = (UUID_LENGTH * 2) + 4 + 1;
    char* convertedUrn = (char*)OICCalloc(urnBufSize, sizeof(char));
    VERIFY_NON_NULL(TAG, convertedUrn, ERROR);

    for(uuidIdx=0, urnIdx=0;  uuidIdx < UUID_LENGTH && urnIdx < urnBufSize; uuidIdx++, urnIdx+=2)
    {
        // canonical format for UUID has '8-4-4-4-12'
        if(uuidIdx==4 || uuidIdx==6 || uuidIdx==8 || uuidIdx==10)
        {
            snprintf(convertedUrn + urnIdx, 2, "%c", '-');
            urnIdx++;
        }
        snprintf(convertedUrn + urnIdx, 3, "%02x", (uint8_t)(uuid->id[uuidIdx]));
    }
    convertedUrn[urnBufSize - 1] = '\0';

    *strUuid = convertedUrn;
    return OC_STACK_OK;

exit:
    return OC_STACK_NO_MEMORY;
}

OCStackResult ConvertStrToUuid(const char* strUuid, OicUuid_t* uuid)
{
    if(NULL == strUuid || NULL == uuid)
    {
        OIC_LOG(ERROR, TAG, "ConvertStrToUuid : Invalid param");
        return OC_STACK_INVALID_PARAM;
    }

    size_t urnIdx = 0;
    size_t uuidIdx = 0;
    size_t strUuidLen = 0;
    char convertedUuid[UUID_LENGTH * 2] = {0};

    strUuidLen = strlen(strUuid);
    if(0 == strUuidLen)
    {
        OIC_LOG(INFO, TAG, "The empty string detected, The UUID will be converted to "\
                           "\"00000000-0000-0000-0000-000000000000\"");
    }
    else if(UUID_LENGTH * 2 + 4 == strUuidLen)
    {
        for(uuidIdx=0, urnIdx=0; uuidIdx < UUID_LENGTH ; uuidIdx++, urnIdx+=2)
        {
            if(*(strUuid + urnIdx) == '-')
            {
                urnIdx++;
            }
            sscanf(strUuid + urnIdx, "%2hhx", &convertedUuid[uuidIdx]);
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Invalid string uuid format, Please set the uuid as correct format");
        OIC_LOG(ERROR, TAG, "e.g) \"72616E64-5069-6E44-6576-557569643030\" (4-2-2-2-6)");
        OIC_LOG(ERROR, TAG, "e.g) \"\"");

        return OC_STACK_INVALID_PARAM;
    }

    memcpy(uuid->id, convertedUuid, UUID_LENGTH);

    return OC_STACK_OK;
}

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
OCStackResult SetDeviceIdSeed(const uint8_t* seed, size_t seedSize)
{
    return SetDoxmDeviceIDSeed(seed, seedSize);
}

static OicSecOtmEventHandler_t gOtmEventHandler = NULL;
static char ptAddr[256] = {0};
static uint16_t ptPort = 0;

void SetOtmEventHandler(OicSecOtmEventHandler_t otmEventHandler)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    memset(ptAddr, 0x00, sizeof(ptAddr));
    ptPort = 0;
    gOtmEventHandler = otmEventHandler;
    OIC_LOG_V(DEBUG, TAG, "Out%s", __func__);
}

/**
 * Function to handle the handshake result in OTM.
 * This function will be invoked after DTLS handshake
 * @param   endPoint  [IN] The remote endpoint.
 * @param   errorInfo [IN] Error information from the endpoint.
 * @return  NONE
 */
static void DTLSHandshakeServerCB(const CAEndpoint_t *endpoint, const CAErrorInfo_t *info)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if(NULL != endpoint && NULL != info)
    {
        OIC_LOG_V(INFO, TAG, "Received status from remote device(%s:%d) : %d",
                 endpoint->addr, endpoint->port, info->result);

        //We can't know about PT's secure port, so compare only adress to identify the PT.
        if (strncmp(endpoint->addr, ptAddr, strlen(ptAddr)) == 0)
        {
            OIC_LOG_V(INFO, TAG, "Normal port is [%s:%d]", ptAddr, ptPort);

            //If DTLS handshake error occurred, revert secure resource and notify error event to application.
            if (CA_STATUS_OK != info->result)
            {
                OIC_LOG(ERROR, TAG, "Failed to establish a secure session with owner device.");
                OIC_LOG(ERROR, TAG, "Doxm/Pstat resource will be reverted to init state.");
                RestoreDoxmToInitState();
                RestorePstatToInitState();
                InvokeOtmEventHandler(endpoint->addr, endpoint->port, NULL, OIC_OTM_ERROR);
            }
        }
        else
        {
            OIC_LOG_V(WARNING, TAG, "[%s:%d] is not a owner device", endpoint->addr, endpoint->port);
        }
    }
    else
    {
        OIC_LOG(WARNING, TAG, "Invalid param.");
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}


void InvokeOtmEventHandler(const char* addr, uint16_t port,
                           const OicUuid_t* uuid, OicSecOtmEvent_t event)
{
    char* strUuid = NULL;
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    if (NULL == gOtmEventHandler)
    {
        OIC_LOG(WARNING, TAG, "OTM event handler is not registered.");
        goto exit;
    }

    //addr can be NULL for init state
    //port can be '0' for BLE and init state
    //uuid can be NULL for init state & coap

    switch(event)
    {
        case OIC_OTM_READY:
        case OIC_OTM_STARTED:
            if (addr)
            {
                OICStrcpy(ptAddr, sizeof(ptAddr), addr);
                ptPort = port;
            }
            //Register TLS event handler to catch the tls event while handshake
            if(CA_STATUS_OK != CAregisterSslHandshakeCallback(DTLSHandshakeServerCB))
            {
                OIC_LOG(WARNING, TAG, "Failed to register (D)TLS handshake callback.");
            }
            break;
        case OIC_OTM_DONE:
        case OIC_OTM_ERROR:
            memset(ptAddr, 0x00, sizeof(ptAddr));
            ptPort = 0;
            //Register TLS event handler to catch the tls event while handshake
            if(CA_STATUS_OK != CAregisterSslHandshakeCallback(NULL))
            {
                OIC_LOG(WARNING, TAG, "Failed to register (D)TLS handshake callback.");
            }
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "Unknow OTM event : %d", event);
            goto exit;
    }

    if (uuid)
    {
        if(OC_STACK_OK != ConvertUuidToStr(uuid, &strUuid))
        {
            OIC_LOG(ERROR, TAG, "Failed to convert UUID to String.");
            goto exit;
        }
    }

    OIC_LOG(DEBUG, TAG, "=================================");
    OIC_LOG(DEBUG, TAG, "[OTM Event]");
    OIC_LOG_V(DEBUG, TAG, "PT UUID : %s", (strUuid ? strUuid : "NULL"));
    OIC_LOG_V(DEBUG, TAG, "PT Addr=%s:%d", (addr ? addr : "NULL"), port);
    OIC_LOG_V(DEBUG, TAG, "Event Code=%d", event);
    OIC_LOG(DEBUG, TAG, "=================================");

    OIC_LOG(DEBUG, TAG, "Invoking callback to notify OTM state..");
    gOtmEventHandler(addr, port, strUuid, (int)event);

exit:
    if (strUuid)
    {
        OICFree(strUuid);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}
#endif
