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

#include <stdlib.h>
#include <string.h>

#include "ocstack.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"
#include "resourcemanager.h"
#include "pstatresource.h"
#include "doxmresource.h"
#include "psinterface.h"
#include "srmresourcestrings.h"
#include "srmutility.h"

#define TAG  "OIC_SRM_PSTAT"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching below max cbor size. */
static const uint16_t CBOR_SIZE = 512;

// Max cbor size payload.
static const uint16_t CBOR_MAX_SIZE = 4400;

// PSTAT Map size - Number of mandatory items
static const uint8_t PSTAT_MAP_SIZE = 6;

// Number of writeable property
static const uint8_t WRITEABLE_PROPERTY_SIZE = 3;

static OicSecDpom_t gSm = SINGLE_SERVICE_CLIENT_DRIVEN;
static OicSecPstat_t gDefaultPstat =
{
    false,                                    // bool isop
    (OicSecDpm_t)(BOOTSTRAP_SERVICE | SECURITY_MANAGEMENT_SERVICES |
    PROVISION_CREDENTIALS | PROVISION_ACLS),   // OicSecDpm_t cm
    (OicSecDpm_t)(TAKE_OWNER | BOOTSTRAP_SERVICE | SECURITY_MANAGEMENT_SERVICES |
    PROVISION_CREDENTIALS | PROVISION_ACLS),   // OicSecDpm_t tm
    {.id = {0}},                              // OicUuid_t deviceID
    SINGLE_SERVICE_CLIENT_DRIVEN,             // OicSecDpom_t om */
    1,                                        // the number of elts in Sms
    &gSm,                                     // OicSecDpom_t *sm
    0,                                        // uint16_t commitHash
    {.id = {0}},                              // OicUuid_t rownerID
};

static OicSecPstat_t    *gPstat = NULL;

static OCResourceHandle gPstatHandle = NULL;

/**
 * This method is internal method.
 * the param roParsed is optionally used to know whether cborPayload has
 * at least read only property value or not.
 */
static OCStackResult CBORPayloadToPstatBin(const uint8_t *cborPayload, const size_t size,
                                 OicSecPstat_t **secPstat, bool *roParsed);

void DeletePstatBinData(OicSecPstat_t* pstat)
{
    if (pstat)
    {
        //Clean 'supported modes' field
        OICFree(pstat->sm);

        //Clean pstat itself
        OICFree(pstat);
    }
}

OCStackResult PstatToCBORPayload(const OicSecPstat_t *pstat, uint8_t **payload, size_t *size,
                                 bool writableOnly)
{
    if (NULL == pstat || NULL == payload || NULL != *payload || NULL == size)
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

    OCStackResult ret = OC_STACK_ERROR;
    size_t pstatMapSize = PSTAT_MAP_SIZE;
    CborEncoder encoder;
    CborEncoder pstatMap;
    char* strUuid = NULL;

    int64_t cborEncoderResult = CborNoError;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    if (false == writableOnly)
    {
        pstatMapSize += WRITEABLE_PROPERTY_SIZE;
    }

    cborEncoderResult = cbor_encoder_create_map(&encoder, &pstatMap, pstatMapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pstat Map.");

    cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_ISOP_NAME,
        strlen(OIC_JSON_ISOP_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ISOP Name Tag.");
    cborEncoderResult = cbor_encode_boolean(&pstatMap, pstat->isOp);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ISOP Name Value.");

    cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_CM_NAME,
        strlen(OIC_JSON_CM_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CM Name Tag.");
    cborEncoderResult = cbor_encode_int(&pstatMap, pstat->cm);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CM Name Value.");

    cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_TM_NAME,
        strlen(OIC_JSON_TM_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding TM Name Tag.");
    cborEncoderResult = cbor_encode_int(&pstatMap, pstat->tm);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding TM Name Value.");

    cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_OM_NAME,
        strlen(OIC_JSON_OM_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding OM Name Tag.");
    cborEncoderResult = cbor_encode_int(&pstatMap, pstat->om);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding OM Name Value.");

    if (false == writableOnly)
    {
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_SM_NAME,
            strlen(OIC_JSON_SM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SM Name Tag.");
        cborEncoderResult = cbor_encode_int(&pstatMap, pstat->sm[0]);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SM Name Value.");

        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_DEVICE_ID_NAME,
            strlen(OIC_JSON_DEVICE_ID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Tag.");
        ret = ConvertUuidToStr(&pstat->deviceID, &strUuid);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, strUuid, strlen(strUuid));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Value.");
        OICFree(strUuid);
        strUuid = NULL;

        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ROwner Id Tag.");
        ret = ConvertUuidToStr(&pstat->rownerID, &strUuid);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, strUuid, strlen(strUuid));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ROwner Id Value.");
        OICFree(strUuid);
        strUuid = NULL;
    }

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&pstatMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_PSTAT,
                strlen(OIC_RSRC_TYPE_SEC_PSTAT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&pstatMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
     CborEncoder ifArray;
     cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_IF_NAME,
             strlen(OIC_JSON_IF_NAME));
     VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
     cborEncoderResult = cbor_encoder_create_array(&pstatMap, &ifArray, 1);
     VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&pstatMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");

    cborEncoderResult = cbor_encoder_close_container(&encoder, &pstatMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Closing PSTAT Map.");

    if (CborNoError == cborEncoderResult)
    {
        *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *payload = outPayload;
        ret = OC_STACK_OK;
    }
exit:
    if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
    {
        // reallocate and try again!
        OICFree(outPayload);
        outPayload = NULL;
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        cborEncoderResult = CborNoError;
        ret = PstatToCBORPayload(pstat, payload, &cborLen, writableOnly);
        if (OC_STACK_OK == ret)
        {
            *size = cborLen;
        }
    }

    if ((CborNoError != cborEncoderResult) || (OC_STACK_OK != ret))
    {
        OICFree(outPayload);
        outPayload = NULL;
        *payload = NULL;
        *size = 0;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

OCStackResult CBORPayloadToPstat(const uint8_t *cborPayload, const size_t size,
                                 OicSecPstat_t **secPstat)
{
    return CBORPayloadToPstatBin(cborPayload, size, secPstat, NULL);
}

static OCStackResult CBORPayloadToPstatBin(const uint8_t *cborPayload, const size_t size,
                                 OicSecPstat_t **secPstat, bool *roParsed)
{
    if (NULL == cborPayload || NULL == secPstat || NULL != *secPstat || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    *secPstat = NULL;

    CborValue pstatCbor;
    CborParser parser;
    CborError cborFindResult = CborNoError;
    char *strUuid = NULL;
    size_t len = 0;

    cbor_parser_init(cborPayload, size, 0, &parser, &pstatCbor);
    CborValue pstatMap = { .parser = NULL };

    OicSecPstat_t *pstat = NULL;
    cborFindResult = cbor_value_enter_container(&pstatCbor, &pstatMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PSTAT Map.");

    pstat = (OicSecPstat_t *)OICCalloc(1, sizeof(OicSecPstat_t));
    VERIFY_NON_NULL(TAG, pstat, ERROR);

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_ISOP_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_boolean(&pstatMap))
    {
        cborFindResult = cbor_value_get_boolean(&pstatMap, &pstat->isOp);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding isOp Value.");
    }
    else
    {
        pstat->isOp = gPstat->isOp;
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_DEVICE_ID_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&pstatMap))
    {
        cborFindResult = cbor_value_dup_text_string(&pstatMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Device Id Value.");
        ret = ConvertStrToUuid(strUuid , &pstat->deviceID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid );
        strUuid  = NULL;

    }
    else
    {
        memcpy(&pstat->deviceID, &gPstat->deviceID, sizeof(OicUuid_t));
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_CM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int cm;

        cborFindResult = cbor_value_get_int(&pstatMap, &cm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CM.");
        pstat->cm = (OicSecDpm_t)cm;
    }
    else
    {
        pstat->cm = gPstat->cm;
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_TM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int tm;

        cborFindResult = cbor_value_get_int(&pstatMap, &tm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding TM.");
        pstat->tm = (OicSecDpm_t)tm;
    }
    else
    {
        pstat->tm = gPstat->tm;
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_OM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int om;

        cborFindResult = cbor_value_get_int(&pstatMap, &om);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding OM.");
        pstat->om = (OicSecDpom_t)om;
    }
    else
    {
        pstat->om = gPstat->om;
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_SM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int sm;

        pstat->smLen = 1;
        pstat->sm = (OicSecDpom_t*)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
        VERIFY_NON_NULL(TAG, pstat->sm, ERROR);
        cborFindResult = cbor_value_get_int(&pstatMap, &sm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SM.");
        pstat->sm[0] = (OicSecDpom_t)sm;

        if (roParsed)
        {
            *roParsed = true;
        }
    }
    else
    {
        VERIFY_NON_NULL(TAG, gPstat, ERROR);
        pstat->smLen = gPstat->smLen;
        pstat->sm = (OicSecDpom_t*)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
        VERIFY_NON_NULL(TAG, pstat->sm, ERROR);
        *pstat->sm = *gPstat->sm;
        cborFindResult = CborNoError;
    }

    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_ROWNERID_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&pstatMap))
    {
        cborFindResult = cbor_value_dup_text_string(&pstatMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ROwner Id Value.");
        ret = ConvertStrToUuid(strUuid , &pstat->rownerID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid );
        strUuid  = NULL;
    }
    else
    {
        VERIFY_NON_NULL(TAG, gPstat, ERROR);
        memcpy(pstat->rownerID.id, gPstat->rownerID.id, sizeof(gPstat->rownerID.id));
        cborFindResult = CborNoError;
    }

    *secPstat = pstat;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        OIC_LOG(ERROR, TAG, "CBORPayloadToPstat failed");
        DeletePstatBinData(pstat);
        pstat = NULL;
        *secPstat = NULL;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

/**
 * Function to update persistent storage
 */
static bool UpdatePersistentStorage(OicSecPstat_t *pstat)
{
    bool bRet = false;

    size_t size = 0;
    uint8_t *cborPayload = NULL;
    OCStackResult ret = PstatToCBORPayload(pstat, &cborPayload, &size, false);
    if (OC_STACK_OK == ret)
    {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, cborPayload, size))
        {
            bRet = true;
        }
        OICFree(cborPayload);
    }

    return bRet;
}

static bool ValidateQuery(const char * query)
{
    OIC_LOG (DEBUG, TAG, "In ValidateQuery");
    if(NULL == gPstat)
    {
        return false;
    }

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
        }
    }
    return (bInterfaceQry ? bInterfaceMatch: true);
}

#ifdef MULTIPLE_OWNER
bool IsValidPstatAccessForSubOwner(const uint8_t *cborPayload, size_t size)
{
    OicSecPstat_t* pstat = NULL;
    bool isValidPstat = true;

    OIC_LOG_BUFFER(DEBUG, TAG, cborPayload, size);
    VERIFY_NON_NULL(TAG, cborPayload, ERROR);
    VERIFY_SUCCESS(TAG, 0 != size, ERROR);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == CBORPayloadToPstat(cborPayload, size, &pstat), ERROR);
    VERIFY_NON_NULL(TAG, pstat, ERROR);

    if (RESET & pstat->cm)
    {
        OIC_LOG(ERROR, TAG, "SubOwner can't reset the server.");
        isValidPstat = false;
    }

exit:
    DeletePstatBinData(pstat);
    return isValidPstat;
}
#endif //MULTIPLE_OWNER

/**
 * The entity handler determines how to process a GET request.
 */
static OCEntityHandlerResult HandlePstatGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_OK;

    OIC_LOG(INFO, TAG, "HandlePstatGetRequest  processing GET request");

    //Checking if Get request is a query.
    if (ehRequest->query)
    {
        OIC_LOG_V(DEBUG,TAG,"query:%s",ehRequest->query);
        OIC_LOG(DEBUG, TAG, "HandlePstatGetRequest processing query");
        if (!ValidateQuery(ehRequest->query))
        {
            ehRet = OC_EH_ERROR;
        }
    }

    /*
     * For GET or Valid Query request return doxm resource CBOR payload.
     * For non-valid query return NULL json payload.
     * A device will 'always' have a default Pstat, so PstatToCBORPayload will
     * return valid pstat resource json.
     */
    size_t size = 0;
    uint8_t *payload = NULL;
    if (ehRet == OC_EH_OK)
    {
        if(OC_STACK_OK != PstatToCBORPayload(gPstat, &payload, &size, false))
        {
            OIC_LOG(WARNING, TAG, "PstatToCBORPayload failed in HandlePstatGetRequest");
        }
    }

    // Send response payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;
    OICFree(payload);
    return ehRet;
}

/**
 * The entity handler determines how to process a POST request.
 * Per the REST paradigm, POST can also be used to update representation of existing
 * resource or create a new resource.
 * For pstat, it updates only tm and om.
 */
static OCEntityHandlerResult HandlePstatPostRequest(OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OIC_LOG(INFO, TAG, "HandlePstatPostRequest  processing POST request");
    OicSecPstat_t *pstat = NULL;
    static uint16_t previousMsgId = 0;
    bool isDuplicatedMsg = false;

    if (ehRequest->payload && NULL != gPstat)
    {
        uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
        size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
        VERIFY_NON_NULL(TAG, payload, ERROR);

        bool roParsed = false;
        OCStackResult ret = CBORPayloadToPstatBin(payload, size, &pstat, &roParsed);
        VERIFY_NON_NULL(TAG, pstat, ERROR);
        if (OC_STACK_OK == ret)
        {
            bool validReq = false;

            /*
             * message ID is supported for CoAP over UDP only according to RFC 7252
             * So we should check message ID to prevent duplicate request handling in case of OC_ADAPTER_IP.
             * In case of other transport adapter, duplicate message check is not required.
             */
            if (OC_ADAPTER_IP == ehRequest->devAddr.adapter &&
                 previousMsgId == ehRequest->messageID)
            {
                isDuplicatedMsg = true;
            }

            if (true == roParsed)
            {
                    OIC_LOG(ERROR, TAG, "Not acceptable request because of read-only properties");
                    ehRet = OC_EH_NOT_ACCEPTABLE;
                    goto exit;
            }

            //operation mode(om) should be one of supported modes(sm)
            for(size_t i = 0; i < gPstat->smLen; i++)
            {
                if(gPstat->sm[i] == pstat->om)
                {
                    validReq = true;
                    break;
                }
            }

            if(!validReq)
            {
                OIC_LOG_V(ERROR, TAG, "%d is unsupported Operation Mode", (int) pstat->om);
                ehRet = OC_EH_BAD_REQ;
                goto exit;
            }
            validReq = false;

            //Currently, IoTivity only supports Single Service Client Directed provisioning
            if (pstat->om == SINGLE_SERVICE_CLIENT_DRIVEN)
            {
                if ((pstat->cm & RESET) && false == pstat->isOp)
                {
                    validReq = true;
                    OIC_LOG(INFO, TAG, "State changed to Ready for Reset");
                }
                else if ((pstat->cm & TAKE_OWNER) && false == pstat->isOp)
                {
                    validReq = true;
                    OIC_LOG (INFO, TAG, "State changed to Ready for Ownership transfer");
                }
                else if (false == (pstat->cm & TAKE_OWNER) && false == pstat->isOp)
                {
                    validReq = true;
                    OIC_LOG(INFO, TAG, "State changed to Ready for Provisioning");
                }
                else if (false == (pstat->cm & TAKE_OWNER) && true == pstat->isOp)
                {
                    validReq = true;
                    OIC_LOG (INFO, TAG, "State changed to Ready for Normal Operation");
                }
                else
                {
                    OIC_LOG(DEBUG, TAG, "Invalid Device provisionig state");
                    OIC_LOG_BUFFER(DEBUG, TAG, payload, size);
                    ehRet = OC_EH_BAD_REQ;
                    goto exit;
                }
            }

            if (!validReq)
            {
                OIC_LOG(DEBUG, TAG, "Bad request for PSTAT");
                ehRet = OC_EH_BAD_REQ;
                goto exit;
            }

            gPstat->isOp = pstat->isOp;
            gPstat->om = pstat->om;
            gPstat->tm = pstat->tm;
            gPstat->cm = pstat->cm;
            memcpy(&(gPstat->deviceID), &(pstat->deviceID), sizeof(OicUuid_t));
            memcpy(&(gPstat->rownerID), &(pstat->rownerID), sizeof(OicUuid_t));

            // Convert pstat data into CBOR for update to persistent storage
            if (UpdatePersistentStorage(gPstat))
            {
                ehRet = OC_EH_OK;
            }
            if (true == (pstat->cm & RESET))
            {
                if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
                {
                    ehRet = OC_EH_ERROR;
                    OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandlePstatPostRequest");
                    DeletePstatBinData(pstat);
                    return ehRet;
                }
                ret = ResetSecureResourceInPS();
                if (OC_STACK_OK == ret)
                {
                    ehRet = OC_EH_OK;
                }
                DeletePstatBinData(pstat);
                return ehRet;
            }
        }
    }
 exit:

     if(OC_EH_OK != ehRet)
     {
         /*
           * If some error is occured while ownership transfer,
           * ownership transfer related resource should be revert back to initial status.
           */
         const OicSecDoxm_t* doxm = GetDoxmResourceData();
         if(doxm)
         {
             if(!doxm->owned)
             {
                OIC_LOG(WARNING, TAG, "The operation failed during handle DOXM request");

                if (!isDuplicatedMsg)
                {
#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
                    InvokeOtmEventHandler(ehRequest->devAddr.addr, ehRequest->devAddr.port,
                                          NULL, OIC_OTM_ERROR);
#endif
                    RestoreDoxmToInitState();
                    RestorePstatToInitState();
                    OIC_LOG(WARNING, TAG, "DOXM will be reverted.");
                }
             }
         }
         else
         {
             OIC_LOG(ERROR, TAG, "Invalid DOXM resource.");
         }
     }
     else
     {
        if(ehRequest->devAddr.adapter == OC_ADAPTER_IP)
        {
            previousMsgId = ehRequest->messageID;
        }
     }

    // Send response payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    DeletePstatBinData(pstat);
    return ehRet;
}

/**
 * This internal method is the entity handler for pstat resources.
 */
 OCEntityHandlerResult PstatEntityHandler(OCEntityHandlerFlag flag,
                                          OCEntityHandlerRequest * ehRequest,
                                          void *callbackParam)
{
    (void)callbackParam;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    // This method will handle REST request (GET/POST) for /oic/sec/pstat
    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(INFO, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandlePstatGetRequest(ehRequest);
                break;
            case OC_REST_POST:
                ehRet = HandlePstatPostRequest(ehRequest);
                break;
            default:
                ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                               OC_EH_OK : OC_EH_ERROR;
                break;
        }
    }
    return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/pstat' resource.
 */
 OCStackResult CreatePstatResource()
{
    OCStackResult ret = OCCreateResource(&gPstatHandle,
                                         OIC_RSRC_TYPE_SEC_PSTAT,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_PSTAT_URI,
                                         PstatEntityHandler,
                                         NULL,
                                         OC_SECURE |
                                         OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate pstat resource");
        DeInitPstatResource();
    }
    return ret;
}

/**
 * Get the default value.
 *
 * @return the gDefaultPstat pointer.
 */
static OicSecPstat_t* GetPstatDefault()
{
    return &gDefaultPstat;
}

OCStackResult InitPstatResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    // Read Pstat resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    OicUuid_t emptyUuid = {.id={0}};
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_PSTAT_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        // Read ACL resource from PS
        ret = CBORPayloadToPstat(data, size, &gPstat);
        OICFree(data);
    }
    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default pstat is created
     * which allows user to initiate pstat provisioning again.
     */
    if ((OC_STACK_OK != ret) || !gPstat)
    {
        gPstat = GetPstatDefault();
    }
    VERIFY_NON_NULL(TAG, gPstat, FATAL);

    //In case of Pstat's device id is empty, fill the device id as doxm's device id.
    if(0 == memcmp(&gPstat->deviceID, &emptyUuid, sizeof(OicUuid_t)))
    {
        OicUuid_t doxmUuid = {.id={0}};
        if(OC_STACK_OK == GetDoxmDeviceID(&doxmUuid))
        {
            memcpy(&gPstat->deviceID, &doxmUuid, sizeof(OicUuid_t));
        }
    }

    // Instantiate 'oic.sec.pstat'
    ret = CreatePstatResource();

exit:
    if (OC_STACK_OK != ret)
    {
        DeInitPstatResource();
    }
    return ret;
}

OCStackResult DeInitPstatResource()
{
    if (gPstat != &gDefaultPstat)
    {
        DeletePstatBinData(gPstat);
        gPstat = NULL;
    }
    return OCDeleteResource(gPstatHandle);
}

/**
 * Function to restore pstat resurce to initial status.
 * This function will use in case of error while ownership transfer
 */
void RestorePstatToInitState()
{
    if(gPstat)
    {
        OIC_LOG(INFO, TAG, "PSTAT resource will revert back to initial status.");

        gPstat->cm = (OicSecDpm_t)(gPstat->cm | TAKE_OWNER);
        gPstat->tm = (OicSecDpm_t)(gPstat->tm & (~TAKE_OWNER));
        gPstat->om = SINGLE_SERVICE_CLIENT_DRIVEN;
        if(gPstat->sm && 0 < gPstat->smLen)
        {
            gPstat->sm[0] = SINGLE_SERVICE_CLIENT_DRIVEN;
        }

        if (!UpdatePersistentStorage(gPstat))
        {
            OIC_LOG(ERROR, TAG, "Failed to revert PSTAT in persistent storage");
        }
    }
}

OCStackResult SetPstatRownerId(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gPstat)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(newROwner && gPstat)
    {
        memcpy(prevId.id, gPstat->rownerID.id, sizeof(prevId.id));
        memcpy(gPstat->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = PstatToCBORPayload(gPstat, &cborPayload, &size, false);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gPstat->rownerID.id, prevId.id, sizeof(prevId.id));
    return ret;
}

/**
 * This function returns the "isop" status of the device.
 *
 * @return true iff pstat.isop == 1, else false
 */
bool GetPstatIsop()
{
    return gPstat->isOp;
}

OCStackResult GetPstatRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gPstat)
    {
        *rowneruuid = gPstat->rownerID;
        retVal = OC_STACK_OK;
    }
    return retVal;
}

OCStackResult SetPstatSelfOwnership(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;

    if(NULL == gPstat)
    {
        ret = OC_STACK_NO_RESOURCE;
        return ret;
    }

    if( newROwner && (false == gPstat->isOp) && (true == (TAKE_OWNER && gPstat->cm)) )
    {
        gPstat->cm = (OicSecDpm_t)(gPstat->cm & (~TAKE_OWNER));
        gPstat->isOp = true;

        memcpy(gPstat->deviceID.id, newROwner->id, sizeof(newROwner->id));
        memcpy(gPstat->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = PstatToCBORPayload(gPstat, &cborPayload, &size, false);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "The state of PSTAT is not Ready For OTM");
    }

    return ret;

exit:
    OICFree(cborPayload);
    return ret;
}

