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
#define CRL_MAP_SIZE                (3)

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

void printCrl(const OicSecCrl_t *crl)
{
    if (NULL == crl)
    {
        OIC_LOG(INFO, TAG, "Received NULL CRL");
        return;
    }

    OIC_LOG(INFO, TAG, "Crl object contain:");
    OIC_LOG_V(INFO, TAG, "id = %d", crl->CrlId);
    OIC_LOG_V(INFO, TAG, "this update = %s", crl->ThisUpdate.data);

    OIC_LOG(INFO, TAG, "crl:");
    OIC_LOG_V(INFO, TAG, "encoding = %d", crl->CrlData.encoding);
    OIC_LOG_V(INFO, TAG, "data (length = %zu):", crl->CrlData.len);
    OIC_LOG_BUFFER(INFO, TAG, crl->CrlData.data, crl->CrlData.len);
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
    bool result = false;

    if (!in || !out)
    {
        OIC_LOG(ERROR, TAG, "in or out crl is NULL");
        return false;
    }

    out->CrlId = in->CrlId;

    result = copyByteArray(in->ThisUpdate.data, in->ThisUpdate.len, &out->ThisUpdate.data, &out->ThisUpdate.len);
    if (!result)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate memory for ThisUpdate");
        return false;
    }

    result = copyByteArray(in->CrlData.data, in->CrlData.len, &out->CrlData.data, &out->CrlData.len);
    if (!result)
    {
        OIC_LOG(ERROR, TAG, "Can't allocate memory for CrlData");
        return false;
    }

    return result;
}

static CborError setPubDataType(CborEncoder *out, const char *name, const OicSecKey_t *value)
{
    if (!out || !name || !value)
    {
        OIC_LOG_V(ERROR, TAG, "%s: null input params", __func__);
        return CborErrorInternalError;
    }

    CborEncoder map;

    const char *encoding = NULL;
    bool binary_field = false;

    size_t mapSize = 0;

    mapSize++;
    switch(value->encoding)
    {
        case OIC_ENCODING_RAW:
            binary_field = true;
            encoding = OIC_SEC_ENCODING_RAW;
            break;
        case OIC_ENCODING_BASE64:
            encoding = OIC_SEC_ENCODING_BASE64;
            break;
        case OIC_ENCODING_DER:
            binary_field = true;
            encoding = OIC_SEC_ENCODING_DER;
            break;
        case OIC_ENCODING_PEM:
            encoding = OIC_SEC_ENCODING_PEM;
            break;
        default:
            OIC_LOG(ERROR, TAG, "Received UNKNOWN encoding, exit!");
            return CborErrorInternalError;
    }

    if (value->data)
    {
        mapSize++;
    }

    CborError result = CborNoError;
    result = cbor_encode_text_string(out, name, strlen(name));
    VERIFY_CBOR_SUCCESS(TAG, result, "Failed Adding name Tag.");

    result = cbor_encoder_create_map(out, &map, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, result, "Failed creating name map");

    if (encoding)
    {
        result = cbor_encode_text_string(&map, OIC_JSON_ENCODING_NAME,
                                         strlen(OIC_JSON_ENCODING_NAME));
        VERIFY_CBOR_SUCCESS(TAG, result, "Failed to add encoding tag.")
        result = cbor_encode_text_string(&map, encoding, strlen(encoding));
        VERIFY_CBOR_SUCCESS(TAG, result, "Failed to add encoding value.");
    };

    if (value->data)
    {
        result = cbor_encode_text_string(&map, OIC_JSON_DATA_NAME, strlen(OIC_JSON_DATA_NAME));
        VERIFY_CBOR_SUCCESS(TAG, result, "Failed to add data tag.");
        if (binary_field)
        {
            result = cbor_encode_byte_string(&map, value->data, value->len);
        }
        else
        {
            result = cbor_encode_text_string(&map, (const char *)value->data, value->len);
        }
        VERIFY_CBOR_SUCCESS(TAG, result, "Failed to add data value.");
    }

    result = cbor_encoder_close_container(out, &map);
    VERIFY_CBOR_SUCCESS(TAG, result, "Failed Closing PrivateData Map.");

exit:
    return result;
}

static CborError getPubDataType(CborValue *in, const char *name, OicSecKey_t *value)
{
    if (!in || !name || !value)
    {
        OIC_LOG_V(ERROR, TAG, "%s: null input params", __func__);
        return CborErrorInternalError;
    }

    CborError result = CborNoError;
    char *encoding = NULL;

    CborValue crlNode = { .parser = NULL };
    result = cbor_value_map_find_value(in, name, &crlNode);
    if (CborNoError == result && cbor_value_is_map(&crlNode))
    {
        CborValue crlMap = { .parser = NULL };
        result = cbor_value_enter_container(&crlNode, &crlMap);

        while(cbor_value_is_valid(&crlMap) && cbor_value_is_text_string(&crlMap))
        {
            char *property = NULL;
            size_t length = 0;
            result = cbor_value_dup_text_string(&crlMap, &property, &length, NULL);
            VERIFY_CBOR_SUCCESS(TAG, result, "Failed Get first crl ojbject tag.");
            result = cbor_value_advance(&crlMap);
            VERIFY_CBOR_SUCCESS(TAG, result, "Failed to advance crlMap");

            if (0 == strcmp(OIC_JSON_DATA_NAME, property))
            {
                if (cbor_value_is_byte_string(&crlMap))
                {
                    result = cbor_value_dup_byte_string(&crlMap, &value->data, &value->len, NULL);
                }
                else if(cbor_value_is_text_string(&crlMap))
                {
                    char *buffer = NULL;
                    result = cbor_value_dup_text_string(&crlMap, &buffer, &value->len, NULL);
                    value->data = (uint8_t *)buffer;
                }
                else
                {
                    result = CborErrorUnknownType;
                    OIC_LOG(ERROR, TAG, "Unknown type for crl->data.");
                }
                VERIFY_CBOR_SUCCESS(TAG, result, "Failed to read crl->data");
            }
            else if (0 == strcmp(OIC_JSON_ENCODING_NAME, property))
            {
                size_t encoding_len = 0;
                result = cbor_value_dup_text_string(&crlMap, &encoding, &encoding_len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, result, "Failed to read crl->encdoing");
            }
            OICFree(property);
        }
    }
    VERIFY_CBOR_SUCCESS(TAG, result, "Failed to find root node");

    if (encoding)
    {
        OicEncodingType_t type = OIC_ENCODING_UNKNOW;
        if (0 == strcmp(encoding, OIC_SEC_ENCODING_BASE64)) type = OIC_ENCODING_BASE64;
        else if (0 == strcmp(encoding, OIC_SEC_ENCODING_DER)) type = OIC_ENCODING_DER;
        else if (0 == strcmp(encoding, OIC_SEC_ENCODING_PEM)) type = OIC_ENCODING_PEM;
        else if (0 == strcmp(encoding, OIC_SEC_ENCODING_RAW)) type = OIC_ENCODING_RAW;

        value->encoding = type;
    }
exit:
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

    printCrl(crl);

    OCStackResult ret = OC_STACK_ERROR;

    CborEncoder encoder;
    CborEncoder crlMap;

    CborError cborEncoderResult = CborNoError;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    cborEncoderResult = cbor_encoder_create_map(&encoder, &crlMap, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create CRL Map");

    //CRLId -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_CRL_ID,
        strlen(OC_RSRVD_CRL_ID));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add CRL ID");
    cborEncoderResult = cbor_encode_int(&crlMap, crl->CrlId);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add CRL Id value");

    //ThisUpdate -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_THIS_UPDATE,
        strlen(OC_RSRVD_THIS_UPDATE));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl update");
    cborEncoderResult = cbor_encode_text_string(&crlMap, (const char *)crl->ThisUpdate.data,
                                                crl->ThisUpdate.len);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add Crl Update value");

    //CRLData -- Mandatory
    cborEncoderResult = setPubDataType(&crlMap, OC_RSRVD_CRL, &crl->CrlData);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add CRLData object");

    //lastUpdate - internal field
    if (lastUpdate)
    {
        cborEncoderResult = cbor_encode_text_string(&crlMap, OC_RSRVD_LAST_UPDATE,
            strlen(OC_RSRVD_LAST_UPDATE));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add last Update tag");
        cborEncoderResult = cbor_encode_text_string(&crlMap, lastUpdate, strlen(lastUpdate));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add last Update value");
    }

    cborEncoderResult = cbor_encoder_close_container(&encoder, &crlMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to add close Crl map");

    *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
    *payload = outPayload;
    ret = OC_STACK_OK;

exit:
    if ((CborNoError != cborEncoderResult) || (OC_STACK_OK != ret))
    {
        OICFree(outPayload);
        outPayload = NULL;
        *payload = NULL;
        *size = 0;
        ret = OC_STACK_ERROR;
        if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
        {
            // Since the allocated initial memory failed, double the memory.
            cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
            cborEncoderResult = CborNoError;
            ret = CrlToCBORPayload(crl, payload, &cborLen, lastUpdate);
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
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter Crl map");

    crl = (OicSecCrl_t *)OICCalloc(1, sizeof(OicSecCrl_t));
    VERIFY_NON_NULL(TAG, crl, ERROR);

    cborFindResult = cbor_value_map_find_value(&crlCbor, OC_RSRVD_CRL_ID, &crlMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&crlMap))
    {
        int CrlId;

        cborFindResult = cbor_value_get_int(&crlMap, &CrlId);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CrlId.");
        crl->CrlId = (uint16_t)CrlId;
    }

    cborFindResult = cbor_value_map_find_value(&crlCbor, OC_RSRVD_THIS_UPDATE, &crlMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&crlMap))
    {
        cborFindResult = cbor_value_dup_text_string(&crlMap,
                (char **)&crl->ThisUpdate.data, &crl->ThisUpdate.len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Byte Array.");
    }

    cborFindResult = getPubDataType(&crlCbor, OC_RSRVD_CRL, &crl->CrlData);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to read CRL.");

    printCrl(crl);

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

    crl->CrlId = crlid++;

    if (!copyCrl(crl, gCrl))
    {
        OIC_LOG(ERROR, TAG, "Can't update global crl");
        return OC_STACK_ERROR;
    }

    char currentTime[32] = {0};
    getCurrentUTCTime(currentTime, sizeof(currentTime));

    OCStackResult res = CrlToCBORPayload((const OicSecCrl_t *) crl, &payload, &size, currentTime);
    if (OC_STACK_OK != res)
    {
        return res;
    }

    return UpdateSecureResourceInPS(OIC_CBOR_CRL_NAME, payload, size);
}

static OCEntityHandlerResult HandleCRLPostRequest(const OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicSecCrl_t *crl = NULL;
    uint8_t *payload = ((OCSecurityPayload *)ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;

    if (payload)
    {
        OIC_LOG(INFO, TAG, "Update SVR DB...");
        CBORPayloadToCrl(payload, size, &crl);
        VERIFY_NON_NULL(TAG, crl, ERROR);

        if (OC_STACK_OK == UpdateCRLResource(crl))
        {
            ehRet = OC_EH_RESOURCE_CREATED;
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
                OIC_LOG (INFO, TAG, "Not implemented request method.");
                //ehRet = HandleCRLGetRequest(ehRequest);
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
                                         OC_OBSERVABLE);

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
                                 strlen(CRL_DEFAULT_CRL_DATA),
                                 &defaultCrl->CrlData.data,
                                 &defaultCrl->CrlData.len);

    bool result2 = copyByteArray((const uint8_t *)CRL_DEFAULT_THIS_UPDATE,
                                 strlen(CRL_DEFAULT_THIS_UPDATE),
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
        // Read ACL resource from PS
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
 * Perform cleanup for ACL resources.
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

    result = OCParsePayload(&payload, PAYLOAD_TYPE_REPRESENTATION, data, size);
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
        size_t outSize = B64DECODE_OUT_SAFESIZE((crl->len + 1));
        uint8_t *out = OICCalloc(1, outSize);
        if (!out)
        {
            OIC_LOG(ERROR, TAG, "Can't allocate memory for base64 str");
            return;
        }
        uint32_t len = 0;

        if(B64_OK == b64Decode((char*)crl->data, crl->len, out, outSize, &len))
        {
            memcpy(crl->data, out, len);
            crl->len = (size_t)len;

            OIC_LOG (ERROR, TAG, "Crl successfully decoded to base64.");
        }
        else
        {
            OIC_LOG (ERROR, TAG, "Base64 decoding failed.");
        }
        OICFree(out);
    }

    out->len = 0;

    out->data = OICRealloc(out->data, crl->len);
    if (out->data)
    {
        memcpy(out->data, crl->data, crl->len);
        out->len = crl->len;
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Can't allocate memory for out->data");
    }
    DeleteCrl(crlRes);
}
