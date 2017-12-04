//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "utlist.h"
#include "crl_logging.h"
#include "payload_logging.h"
#include "psinterface.h"
#include "resourcemanager.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "doxmresource.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "crlresource.h"
#include "ocpayloadcbor.h"
#include "base64.h"
#include <time.h>

#define TAG  "OIC_SRM_CRL"

#define SEPARATOR                   ":"
#define SEPARATOR_LEN               (1)
#define CBOR_CRL_NAME               "\"CRL\""
#define CBOR_CRL_NAME_LEN           (5)
#define OIC_CBOR_CRL_NAME           "crl"
#define OIC_CBOR_CRL_ID             "CRLId"
#define OIC_CBOR_CRL_THIS_UPDATE    "ThisUpdate"
#define OIC_CBOR_CRL_DATA           "CRLData"
#define CRL_DEFAULT_CRL_ID          (1)
#define CRL_DEFAULT_THIS_UPDATE     "150101000000Z"
#define CRL_DEFAULT_LAST_UPDATE     "20150701000000"
#define CRL_DEFAULT_CRL_DATA        "-"
#define CRL_MAP_SIZE                (5)

static OCResourceHandle     gCrlHandle  = NULL;
static OicSecCrl_t         *gCrl        = NULL;

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching below max cbor size. */
static const uint16_t CBOR_SIZE = 1024;

// Max cbor size payload.
static const uint16_t CBOR_MAX_SIZE = 4400;

void DeleteCrl(OicSecCrl_t *crl)
{
    if (crl)
    {
        //Clean ThisUpdate
        OICFree(crl->ThisUpdate.data);

        //clean CrlData
        OICFree(crl->CrlData.data);

        //Clean crl itself
        OICFree(crl);
    }
}

static bool copyByteArray(const uint8_t *in, size_t in_len, uint8_t **out, size_t *out_len)
{
    OICFree(*out);
    uint8_t *tmp = OICMalloc(in_len);
    if (!tmp)
    {
        return false;
    }
    memcpy(tmp, in, in_len);
    *out = tmp;
    *out_len = in_len;
    return true;
}

static bool copyCrl(const OicSecCrl_t *in, OicSecCrl_t *out)
{
    bool result = true;

    if (!in || !out)
    {
        OIC_LOG(ERROR, TAG, "in or out crl is NULL");
        return false;
    }

    out->CrlId = in->CrlId;

    if (NULL != in->ThisUpdate.data)
    {
        result = copyByteArray(in->ThisUpdate.data, in->ThisUpdate.len, &out->ThisUpdate.data, &out->ThisUpdate.len);
        VERIFY_SUCCESS(TAG, result, ERROR);
    }

    if (NULL != in->CrlData.data)
    {
        result = copyByteArray(in->CrlData.data, in->CrlData.len, &out->CrlData.data, &out->CrlData.len);
        VERIFY_SUCCESS(TAG, result, ERROR);
    }

exit:
    return result;
}

static CborError setCrlData(CborEncoder *out, const char *name, const OicSecKey_t *value)
{
    if (!out || !name || !value)
    {
        OIC_LOG_V(ERROR, TAG, "%s: null input params", __func__);
        return CborErrorInternalError;
    }

    CborError result = CborErrorInternalError;

    size_t len = 0;
    size_t encodeBufferSize = B64ENCODE_OUT_SAFESIZE((value->len + 1));
    char *encodeBuffer = OICCalloc(1, encodeBufferSize);

    VERIFY_NOT_NULL(TAG, encodeBuffer, ERROR);

    B64Result b64result = b64Encode(value->data, value->len, encodeBuffer, encodeBufferSize, &len);
    VERIFY_SUCCESS(TAG, (B64_OK == b64result), ERROR);

    result = cbor_encode_text_string(out, name, strlen(name));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, result, "Failed Adding name Tag.");

    result = cbor_encode_text_string(out, (const char *)encodeBuffer, len);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, result, "Failed Adding data Tag.");

exit:
    OICFree(encodeBuffer);

    return result;
}

static CborError getCrlData(CborValue *in, const char *name, OicSecKey_t *value)
{
    if (!in || !name || !value)
    {
        OIC_LOG_V(ERROR, TAG, "%s: null input params", __func__);
        return CborErrorInternalError;
    }

    CborError result = CborNoError;
    CborValue crlNode = { .parser = NULL };
    char *decodeBuffer = NULL;
    size_t decodeBufferSize;

    result = cbor_value_map_find_value(in, name, &crlNode);
    if (CborNoError == result && cbor_value_is_text_string(&crlNode))
    {
        result = cbor_value_dup_text_string(&crlNode,
                (char **)&decodeBuffer, &decodeBufferSize, NULL);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, result, "Failed Advancing Byte Array.");

        value->len = B64DECODE_OUT_SAFESIZE(decodeBufferSize + 1);
        value->data = OICCalloc(1, value->len);
        VERIFY_NOT_NULL(TAG, value->data, ERROR);

        B64Result b64result = b64Decode(decodeBuffer, decodeBufferSize, value->data, value->len, &value->len);
        if (B64_OK != b64result)
        {
            OIC_LOG_V(ERROR, TAG, "CRL b64Decode error");
            result = CborErrorInternalError;
        }

    }

exit:
    OICFree(decodeBuffer);
    return result;
}

OCStackResult CrlToCBORPayload(const OicSecCrl_t *crl, uint8_t **payload, size_t *size, char *lastUpdate)
{
    if (NULL == crl || NULL == payload || NULL != *payload || NULL == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    size_t cborLen = *size;
    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    *payload = NULL;
    *size = 0;

    size_t mapSize = CRL_MAP_SIZE;
    if (lastUpdate)
    {
        mapSize++;
    }

    OIC_LOG_CRL(INFO, crl);

    OCStackResult ret = OC_STACK_ERROR;

    CborEncoder encoder;
    CborEncoder crlMap;

    CborError cborEncoderResult = CborNoError;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    cborEncoderResult = cbor_encoder_create_map(&encoder, &crlMap, mapSize);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to create CRL Map");

    //CRLId -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_CRL_ID,
        strlen(OC_RSRVD_CRL_ID));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add CRL ID");
    cborEncoderResult = cbor_encode_int(&crlMap, crl->CrlId);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add CRL Id value");

    //ThisUpdate -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_THIS_UPDATE,
        strlen(OC_RSRVD_THIS_UPDATE));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add Crl update");
    cborEncoderResult = cbor_encode_text_string(&crlMap, (const char *)crl->ThisUpdate.data,
                                                crl->ThisUpdate.len);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add Crl Update value");

    //CRLData -- Mandatory
    cborEncoderResult = setCrlData(&crlMap, OC_RSRVD_CRL, &crl->CrlData);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add CRLData object");

    //lastUpdate - internal field
    if (lastUpdate)
    {
        cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_LAST_UPDATE,
            strlen(OC_RSRVD_LAST_UPDATE));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add last Update tag");
        cborEncoderResult = cbor_encode_text_string(&crlMap, lastUpdate, strlen(lastUpdate));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add last Update value");
    }

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&crlMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&crlMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Value.");
    cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_CRL,
            strlen(OIC_RSRC_TYPE_SEC_CRL));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding RT Value.");
    cborEncoderResult = cbor_encoder_close_container(&crlMap, &rtArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&crlMap, OIC_JSON_IF_NAME,
       strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&crlMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Value.");
    cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
            strlen(OC_RSRVD_INTERFACE_DEFAULT));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding IF Value.");
    cborEncoderResult = cbor_encoder_close_container(&crlMap, &ifArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing IF.");

    cborEncoderResult = cbor_encoder_close_container(&encoder, &crlMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed to add close Crl map");

    *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
    *payload = outPayload;
    ret = OC_STACK_OK;

exit:
    if ((CborNoError != cborEncoderResult) || (OC_STACK_OK != ret))
    {
        OICFree(outPayload);
        if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
        {
            // Since the allocated initial memory failed, double the memory.
            cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
            cborEncoderResult = CborNoError;
            ret = CrlToCBORPayload(crl, payload, &cborLen, lastUpdate);
            if (OC_STACK_OK != ret)
            {
                outPayload = NULL;
                *payload = NULL;
                *size = 0;
                ret = OC_STACK_ERROR;
            }
        }
        else
        {
            outPayload = NULL;
            *payload = NULL;
            *size = 0;
            ret = OC_STACK_ERROR;
        }
    }

    return ret;
}

OCStackResult CBORPayloadToCrl(const uint8_t *cborPayload, const size_t size,
                               OicSecCrl_t **secCrl)
{
    if (NULL == cborPayload || NULL == secCrl || NULL != *secCrl || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    OicSecCrl_t *crl = NULL;

    CborValue crlCbor = {.parser = NULL};
    CborParser parser = {.end = NULL};
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &crlCbor);
    CborValue crlMap = { .parser = NULL};
    cborFindResult = cbor_value_enter_container(&crlCbor, &crlMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to enter Crl map");

    crl = (OicSecCrl_t *)OICCalloc(1, sizeof(OicSecCrl_t));
    VERIFY_NOT_NULL(TAG, crl, ERROR);

    cborFindResult = cbor_value_map_find_value(&crlCbor, OC_RSRVD_CRL_ID, &crlMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&crlMap))
    {
        int CrlId;

        cborFindResult = cbor_value_get_int(&crlMap, &CrlId);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Finding CrlId.");
        crl->CrlId = (uint16_t)CrlId;
    }

    cborFindResult = cbor_value_map_find_value(&crlCbor, OC_RSRVD_THIS_UPDATE, &crlMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&crlMap))
    {
        cborFindResult = cbor_value_dup_text_string(&crlMap,
                (char **)&crl->ThisUpdate.data, &crl->ThisUpdate.len, NULL);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Advancing Byte Array.");
    }

    cborFindResult = getCrlData(&crlCbor, OC_RSRVD_CRL, &crl->CrlData);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read CRL.");

    OIC_LOG_CRL(INFO, crl);

    *secCrl = crl;
    ret = OC_STACK_OK;
exit:
    if (CborNoError != cborFindResult)
    {
        // PUT/POST CBOR may not have mandatory values set default values.
        OIC_LOG (DEBUG, TAG, "Set default values");

        if (copyCrl(gCrl, crl))
        {
            *secCrl = crl;
            ret = OC_STACK_OK;
        }
        else
        {
            DeleteCrl(crl);
            crl = NULL;
            ret = OC_STACK_ERROR;
            OIC_LOG (ERROR, TAG, "Can't set default crl");
        }
    }
    return ret;
}

static void getCurrentUTCTime(char *out, size_t len)
{
    //TODO: how to implement it in cross-platform way?
    time_t rawtime;
    struct tm * timeinfo = NULL;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    if (NULL == timeinfo)
    {
        return;
    }

    snprintf(out, len, "%04d%02d%02d%02d%02d%02d",
            timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

OCStackResult UpdateCRLResource(OicSecCrl_t *crl)
{
    static uint16_t crlid = 0;
    uint8_t *payload = NULL;
    size_t size = 0;

    if ( NULL == gCrl )
    {
        OIC_LOG(ERROR, TAG, "Crl resourse isn't initialized");
        return OC_STACK_ERROR;
    }

    if ( 0 == crl->CrlId)
    {
        crl->CrlId = ++crlid;
    }
    else
    {
        if (crl->CrlId > crlid)
        {
            crlid = crl->CrlId;
        }
    }

    if (!copyCrl(crl, gCrl))
    {
        OIC_LOG(ERROR, TAG, "Can't update global crl");
        return OC_STACK_ERROR;
    }

    char currentTime[32] = {0};
    getCurrentUTCTime(currentTime, sizeof(currentTime));

    OCStackResult res = CrlToCBORPayload((const OicSecCrl_t *) gCrl, &payload, &size, currentTime);
    if (OC_STACK_OK != res)
    {
        return res;
    }

#if defined(__TIZENRT__)
    res = UpdateSecureResourceInPS(OIC_CBOR_CRL_NAME, payload, size);
    OICFree(payload);

    return res;
#else
    return UpdateSecureResourceInPS(OIC_CBOR_CRL_NAME, payload, size);
#endif
}

static bool ValidateQuery(const char * query)
{
    OIC_LOG (DEBUG, TAG, "In ValidateQuery");

    bool bInterfaceQry = false;      // does querystring contains 'if' query ?
    bool bInterfaceMatch = false;    // does 'if' query matches with oic.if.baseline ?

    OicParseQueryIter_t parseIter = {.attrPos = NULL};

    ParseQueryIterInit((unsigned char*)query, &parseIter);

    while (GetNextQuery(&parseIter))
    {
        if (strncasecmp((char *)parseIter.attrPos, OC_RSRVD_INTERFACE, parseIter.attrLen) == 0)
        {
            bInterfaceQry = true;
            if ((strncasecmp((char *)parseIter.valPos, OC_RSRVD_INTERFACE_DEFAULT, parseIter.valLen) == 0))
            {
                bInterfaceMatch = true;
            }
            return (bInterfaceQry ? bInterfaceMatch: false);
        }
    }

    return true;
}

static OCEntityHandlerResult HandleCRLGetRequest(const OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_OK;
    uint8_t* payload = NULL;
    size_t size = 0;

    if (ehRequest->query)
    {
        OIC_LOG_V(DEBUG,TAG,"query:%s",ehRequest->query);
        OIC_LOG(DEBUG, TAG, "HandleCRLGetRequest processing query");
        if (!ValidateQuery(ehRequest->query))
        {
            ehRet = OC_EH_ERROR;
        }
    }

    if ( NULL == gCrl )
    {
        OIC_LOG(ERROR, TAG, "Crl resourse isn't initialized");
        ehRet = OC_EH_ERROR;
    }

    if (OC_EH_OK == ehRet && OC_STACK_OK != CrlToCBORPayload(gCrl, &payload, &size, NULL))
    {
        OIC_LOG(ERROR, TAG, " HandleCRLGetRequest error");
        ehRet = OC_EH_ERROR;
    }

    ehRet = (payload ? OC_EH_OK : OC_EH_ERROR);
    OIC_LOG(DEBUG, TAG, "CRL payload with GET response");
    OIC_LOG_BUFFER(DEBUG, TAG, payload, size);

    // Send payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, payload, size))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleCRLGetRequest");
    }

    OICFree(payload);

    OIC_LOG_V(INFO, TAG, "%s RetVal %d", __func__, ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleCRLPostRequest(const OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicSecCrl_t *crl = NULL;
    uint8_t *payload = ((OCSecurityPayload *)ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
    bool create = (NULL == gCrl);

    if (payload)
    {
        OIC_LOG(INFO, TAG, "Update SVR DB...");
        CBORPayloadToCrl(payload, size, &crl);
        VERIFY_NOT_NULL(TAG, crl, ERROR);

        if (OC_STACK_OK == UpdateCRLResource(crl))
        {
            ehRet = create ? OC_EH_RESOURCE_CREATED : OC_EH_CHANGED;
        }

        DeleteCrl(crl);
    }

exit:
    // Send payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandleCRLPostRequest");
    }

    OIC_LOG_V(INFO, TAG, "%s RetVal %d", __func__, ehRet);
    return ehRet;
}


/**
 * This internal method is the entity handler for CRL resource and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
static OCEntityHandlerResult CRLEntityHandler(OCEntityHandlerFlag flag,
                                              OCEntityHandlerRequest *ehRequest,
                                              void *callbackParameter)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    (void)callbackParameter;

    if (!ehRequest)
    {
        return ehRet;
    }

    OIC_LOG(INFO, TAG, "Handle CRL resource");

    if (flag & OC_REQUEST_FLAG)
    {
        // TODO :  Handle PUT and DEL methods
        OIC_LOG (INFO, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleCRLGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandleCRLPostRequest(ehRequest);
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
        }
    }

    return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/crl' resource.
 */
static OCStackResult CreateCRLResource()
{
    OCStackResult ret = OCCreateResource(&gCrlHandle,
                                         OIC_RSRC_TYPE_SEC_CRL,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_CRL_URI,
                                         CRLEntityHandler,
                                         NULL,
                                         OC_SECURE |
                                         OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate CRL resource");
        DeInitCRLResource();
    }
    return ret;
}

/**
 * Get the default value.
 * @return defaultCrl for now.
 */
static OicSecCrl_t *GetCrlDefault()
{
    OicSecCrl_t *defaultCrl = (OicSecCrl_t *)OICCalloc(1, sizeof(OicSecCrl_t));
    if (NULL == defaultCrl)
    {
        return NULL;
    }

    defaultCrl->CrlId = CRL_DEFAULT_CRL_ID;
    defaultCrl->CrlData.encoding = OIC_ENCODING_DER;

    bool result1 = copyByteArray((const uint8_t *)CRL_DEFAULT_CRL_DATA,
                                 sizeof(CRL_DEFAULT_CRL_DATA),
                                 &defaultCrl->CrlData.data,
                                 &defaultCrl->CrlData.len);

    bool result2 = copyByteArray((const uint8_t *)CRL_DEFAULT_THIS_UPDATE,
                                 sizeof(CRL_DEFAULT_THIS_UPDATE),
                                 &defaultCrl->ThisUpdate.data,
                                 &defaultCrl->ThisUpdate.len);

    if (!result1 || !result2)
    {
        DeleteCrl(defaultCrl);
        return NULL;
    }

    return defaultCrl;
}

/**
 * Initialize CRL resource by loading data from persistent storage.
 *
 * @retval
 *     OC_STACK_OK    - no errors
 *     OC_STACK_ERROR - stack process error
 */
OCStackResult InitCRLResource()
{
    OCStackResult ret = OC_STACK_ERROR;
    // Read Crl resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        // Read CRL resource from PS
        ret = CBORPayloadToCrl(data, size, &gCrl);
    }

    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default CrlResource is created
     * which allows user to initiate CrlResource provisioning again.
     */
    if ((OC_STACK_OK != ret) || !data || !gCrl)
    {
        gCrl = GetCrlDefault();
    }

    ret = CreateCRLResource();
    OICFree(data);
    return ret;
}

/**
 * Perform cleanup for CRL resources.
 */
OCStackResult DeInitCRLResource()
{
    OCStackResult result = OCDeleteResource(gCrlHandle);
    gCrlHandle = NULL;
    DeleteCrl(gCrl);
    gCrl = NULL;
    return result;
}

OicSecCrl_t *GetCRLResource()
{
    OicSecCrl_t *crl =  NULL;

    //Read CRL resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    OCStackResult ret = GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size);
    if (data)
    {
        //Convert CBOR CRL into binary format
        ret = CBORPayloadToCrl(data, size, &crl);
    }
    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default CrlResource is created
     * which allows user to initiate CrlResource provisioning again.
     */
    if ((OC_STACK_OK != ret) || !data || !crl)
    {
        crl = GetCrlDefault();
    }
    OICFree(data);

    return crl;
}

OCStackResult getLastUpdateFromDB(char **lastUpdate)
{
    OCStackResult result = OC_STACK_OK;

    OCPayload *payload = NULL;
    uint8_t *data = NULL;
    size_t size = 0;

    if (!lastUpdate)
    {
        return OC_STACK_INVALID_PARAM;
    }

    result = GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size);
    if (result != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Can't get crl data from database");
        goto exit;
    }

    result = OCParsePayload(&payload, OC_FORMAT_CBOR, PAYLOAD_TYPE_REPRESENTATION, data, size);
    if (result != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "Can't parse cbor data from DB");
        goto exit;
    }

    if (!OCRepPayloadGetPropString((const OCRepPayload*)payload, OC_RSRVD_LAST_UPDATE, lastUpdate))
    {
        OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_LAST_UPDATE);
        result = OC_STACK_ERROR;
    }

exit:
    if (result != OC_STACK_OK)
    {
        OIC_LOG(DEBUG, TAG, "Assume you are first time get Crl, and it can be absent in database");
        OIC_LOG_V(DEBUG, TAG, "Return default last update time %s", CRL_DEFAULT_LAST_UPDATE);

        *lastUpdate = OICStrdup(CRL_DEFAULT_LAST_UPDATE);
        result = OC_STACK_OK;
    }
    OCPayloadDestroy((OCPayload *)payload);

    return result;
}

uint8_t *GetCrl()
{
    uint8_t *data = NULL;
    size_t size = 0;
    OicSecCrl_t *crl = NULL;
    if (OC_STACK_OK == GetSecureVirtualDatabaseFromPS(OIC_CBOR_CRL_NAME, &data, &size) && data &&
        OC_STACK_OK == CBORPayloadToCrl(data, size, &crl))
    {
        return crl->CrlData.data;
    }
    return NULL;
}

void GetDerCrl(ByteArray_t* out)
{
    if(NULL == out)
    {
        return;
    }

    OicSecCrl_t *crlRes = GetCRLResource();

    if(NULL == crlRes)
    {
        return;
    }

    OicSecKey_t *crl = &crlRes->CrlData;

    if (OIC_ENCODING_BASE64 == crl->encoding)
    {
        size_t decodeBufferSize = B64DECODE_OUT_SAFESIZE((crl->len + 1));
        uint8_t *decodeBuffer = OICCalloc(1, decodeBufferSize);
        if (!decodeBuffer)
        {
            OIC_LOG(ERROR, TAG, "Can't allocate memory for base64 str");
            return;
        }
        size_t len = 0;

        if(B64_OK == b64Decode((char*)crl->data, crl->len, decodeBuffer, decodeBufferSize, &len))
        {
            memcpy(crl->data, decodeBuffer, len);
            crl->len = (size_t)len;

            OIC_LOG (ERROR, TAG, "Crl successfully decoded to base64.");
        }
        else
        {
            OIC_LOG (ERROR, TAG, "Base64 decoding failed.");
        }

        OICFree(decodeBuffer);
    }

    out->len = 0;

    uint8_t *tmp = OICRealloc(out->data, crl->len);
    if (tmp)
    {
        out->data = tmp;
        memcpy(out->data, crl->data, crl->len);
        out->len = crl->len;
    }
    else
    {
        OICFree(out->data);
        out->data = NULL;
        OIC_LOG(ERROR, TAG, "Can't allocate memory for out->data");
    }
    DeleteCrl(crlRes);
}
