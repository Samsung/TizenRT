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
#include "iotivity_config.h"
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "ocstack.h"
#include "oic_malloc.h"
#include "payload_logging.h"
#include "utlist.h"
#include "ocrandom.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "cainterface.h"
#include "ocserverrequest.h"
#include "resourcemanager.h"
#include "doxmresource.h"
#include "pstatresource.h"
#include "aclresource.h"
#include "amaclresource.h"
#include "pconfresource.h"
#include "dpairingresource.h"
#include "psinterface.h"
#include "srmresourcestrings.h"
#include "securevirtualresourcetypes.h"
#include "credresource.h"
#include "srmutility.h"
#include "pinoxmcommon.h"
#include "oxmverifycommon.h"

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
#include "pkix_interface.h"
#endif

#define TAG  "OIC_SRM_DOXM"
#define CHAR_ZERO ('0')

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching belox max cbor size. */
static const uint16_t CBOR_SIZE = 512;

/** Max cbor size payload. */
static const uint16_t CBOR_MAX_SIZE = 4400;

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
/** MAX uuid seed size */
#define MAX_UUID_SEED_SIZE (64)
/** MIN uuid seed size */
#define MIN_UUID_SEED_SIZE (8)

/** Buffer to save the seed of device UUID */
static uint8_t gUuidSeed[MAX_UUID_SEED_SIZE];
static size_t gUuidSeedSize = 0;
#endif

#ifdef MULTIPLE_OWNER
#define MAX_SUBOWNER_SIZE (64)
#define MIN_SUBOWNER_SIZE (1)
#define DEFAULT_SUBOWNER_SIZE (32)

static size_t gMaxSubOwnerSize = DEFAULT_SUBOWNER_SIZE;
#endif

typedef enum ConfirmState{
    CONFIRM_STATE_READY = 0,
    CONFIRM_STATE_WAIT = 1,
    CONFIRM_STATE_ACCEPTED = 2,
    CONFIRM_STATE_DENIED = 3
}ConfirmState_t;

static OicSecDoxm_t        *gDoxm = NULL;
static OCResourceHandle    gDoxmHandle = NULL;

static OicSecOxm_t gOicSecDoxmJustWorks = OIC_JUST_WORKS;
static OicSecDoxm_t gDefaultDoxm =
{
    NULL,                   /* OicUrn_t *oxmType */
    0,                      /* size_t oxmTypeLen */
    &gOicSecDoxmJustWorks,  /* uint16_t *oxm */
    1,                      /* size_t oxmLen */
    OIC_JUST_WORKS,         /* uint16_t oxmSel */
    SYMMETRIC_PAIR_WISE_KEY,/* OicSecCredType_t sct */
    false,                  /* bool owned */
    {.id = {0}},            /* OicUuid_t deviceID */
    false,                  /* bool dpc */
    {.id = {0}},            /* OicUuid_t owner */
#ifdef MULTIPLE_OWNER
    NULL,                   /* OicSecSubOwner_t sub-owner list */
    NULL,                   /* OicSecMomType_t multiple owner mode */
#endif //MULTIPLE_OWNER
    {.id = {0}},            /* OicUuid_t rownerID */
};

static uint16_t gConfirmMsgId = 0;
static ConfirmState_t gConfirmState = CONFIRM_STATE_READY;


/**
 * This method is internal method.
 * the param roParsed is optionally used to know whether cborPayload has
 * at least read only property value or not.
 */
static OCStackResult CBORPayloadToDoxmBin(const uint8_t *cborPayload, size_t size,
                                OicSecDoxm_t **doxm, bool *roParsed);

void DeleteDoxmBinData(OicSecDoxm_t* doxm)
{
    if (doxm)
    {
        //Clean oxmType
        for (size_t i = 0; i < doxm->oxmTypeLen; i++)
        {
            OICFree(doxm->oxmType[i]);
        }
        OICFree(doxm->oxmType);

        //clean oxm
        OICFree(doxm->oxm);

#ifdef MULTIPLE_OWNER
        //clean mom
        OICFree(doxm->mom);

        //clean sub-owner list
        if(NULL != doxm->subOwners)
        {
            OicSecSubOwner_t* subowner = NULL;
            OicSecSubOwner_t* temp = NULL;
            LL_FOREACH_SAFE(doxm->subOwners, subowner, temp)
            {
                LL_DELETE(doxm->subOwners, subowner);
                OICFree(subowner);
            }
        }
#endif //MULTIPLE_OWNER

        //Clean doxm itself
        OICFree(doxm);
    }
}

OCStackResult DoxmToCBORPayload(const OicSecDoxm_t *doxm, uint8_t **payload, size_t *size,
                                bool rwOnly)
{
    if (NULL == doxm || NULL == payload || NULL != *payload || NULL == size)
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

    CborEncoder encoder;
    CborEncoder doxmMap;
    char* strUuid = NULL;

    int64_t cborEncoderResult = CborNoError;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    cborEncoderResult = cbor_encoder_create_map(&encoder, &doxmMap, CborIndefiniteLength);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Map.");

    //OxmType -- Not Mandatory
    if (doxm->oxmTypeLen > 0)
    {
        CborEncoder oxmType;
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_OXM_TYPE_NAME,
            strlen(OIC_JSON_OXM_TYPE_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmType Tag.");
        cborEncoderResult = cbor_encoder_create_array(&doxmMap, &oxmType, doxm->oxmTypeLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmType Array.");

        for (size_t i = 0; i < doxm->oxmTypeLen; i++)
        {
            cborEncoderResult = cbor_encode_text_string(&oxmType, doxm->oxmType[i],
                strlen(doxm->oxmType[i]));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmType Value.");
        }
        cborEncoderResult = cbor_encoder_close_container(&doxmMap, &oxmType);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing oxmType.");
    }

    //Oxm -- Not Mandatory
    if (doxm->oxmLen > 0 && false == rwOnly)
    {
        CborEncoder oxm;
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_OXMS_NAME,
            strlen(OIC_JSON_OXMS_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmName Tag.");
        cborEncoderResult = cbor_encoder_create_array(&doxmMap, &oxm, doxm->oxmLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmName Array.");

        for (size_t i = 0; i < doxm->oxmLen; i++)
        {
            cborEncoderResult = cbor_encode_int(&oxm, doxm->oxm[i]);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding oxmName Value");
        }
        cborEncoderResult = cbor_encoder_close_container(&doxmMap, &oxm);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing oxmName.");
    }

    //OxmSel -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_OXM_SEL_NAME,
        strlen(OIC_JSON_OXM_SEL_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Sel Tag.");
    cborEncoderResult = cbor_encode_int(&doxmMap, doxm->oxmSel);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Sel Value.");

    //sct -- Mandatory
    if (false == rwOnly)
    {
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_SUPPORTED_CRED_TYPE_NAME,
            strlen(OIC_JSON_SUPPORTED_CRED_TYPE_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Tag");
        cborEncoderResult = cbor_encode_int(&doxmMap, doxm->sct);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Type Value.");
    }

    //Owned -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_OWNED_NAME,
        strlen(OIC_JSON_OWNED_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Owned Tag.");
    cborEncoderResult = cbor_encode_boolean(&doxmMap, doxm->owned);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Owned Value.");

    if (false == rwOnly)
    {
        //DeviceId -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_DEVICE_ID_NAME,
            strlen(OIC_JSON_DEVICE_ID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Tag.");
        ret = ConvertUuidToStr(&doxm->deviceID, &strUuid);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
        cborEncoderResult = cbor_encode_text_string(&doxmMap, strUuid, strlen(strUuid));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Id Value.");
        OICFree(strUuid);
        strUuid = NULL;
    }

#ifdef MULTIPLE_OWNER
    //Device SubOwnerID -- Not Mandatory
    if(doxm->subOwners)
    {
        size_t subOwnerLen = 0;
        OicSecSubOwner_t* subOwner = NULL;
        LL_FOREACH(doxm->subOwners, subOwner)
        {
            subOwnerLen++;
        }

        CborEncoder subOwners;
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_SUBOWNERID_NAME,
            strlen(OIC_JSON_SUBOWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SubOwnerId Tag.");
        cborEncoderResult = cbor_encoder_create_array(&doxmMap, &subOwners, subOwnerLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SubOwner Array.");

        subOwner = NULL;
        LL_FOREACH(doxm->subOwners, subOwner)
        {
            char* strUuid = NULL;
            ret = ConvertUuidToStr(&subOwner->uuid, &strUuid);
            VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
            cborEncoderResult = cbor_encode_text_string(&subOwners, strUuid, strlen(strUuid));
            OICFree(strUuid);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SubOwnerId Value");
        }
        cborEncoderResult = cbor_encoder_close_container(&doxmMap, &subOwners);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing SubOwnerId.");
    }

    //Multiple Owner Mode -- Not Mandatory
    if(doxm->mom)
    {
        cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_MOM_NAME,
            strlen(OIC_JSON_MOM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding mom Tag");
        cborEncoderResult = cbor_encode_int(&doxmMap, (int64_t)doxm->mom->mode);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding mom Value.");
    }
#endif //MULTIPLE_OWNER

    //devownerid -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_DEVOWNERID_NAME,
        strlen(OIC_JSON_DEVOWNERID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Owner Id Tag.");
    ret = ConvertUuidToStr(&doxm->owner, &strUuid);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
    cborEncoderResult = cbor_encode_text_string(&doxmMap, strUuid, strlen(strUuid));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Owner Id Value.");
    OICFree(strUuid);
    strUuid = NULL;

    //ROwner -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_ROWNERID_NAME,
        strlen(OIC_JSON_ROWNERID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ROwner Id Tag.");
    ret = ConvertUuidToStr(&doxm->rownerID, &strUuid);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
    cborEncoderResult = cbor_encode_text_string(&doxmMap, strUuid, strlen(strUuid));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ROwner Id Value.");
    OICFree(strUuid);
    strUuid = NULL;

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&doxmMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_DOXM,
                strlen(OIC_RSRC_TYPE_SEC_DOXM));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&doxmMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
     CborEncoder ifArray;
     cborEncoderResult = cbor_encode_text_string(&doxmMap, OIC_JSON_IF_NAME,
             strlen(OIC_JSON_IF_NAME));
     VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
     cborEncoderResult = cbor_encoder_create_array(&doxmMap, &ifArray, 1);
     VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&doxmMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");

    cborEncoderResult = cbor_encoder_close_container(&encoder, &doxmMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing DoxmMap.");

    if (CborNoError == cborEncoderResult)
    {
        *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *payload = outPayload;
        ret = OC_STACK_OK;
    }
exit:
    if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
    {
        OIC_LOG(DEBUG, TAG, "Memory getting reallocated.");
        // reallocate and try again!
        OICFree(outPayload);
        outPayload = NULL;
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        OIC_LOG_V(DEBUG, TAG, "Doxm reallocation size : %zd.", cborLen);
        cborEncoderResult = CborNoError;
        ret = DoxmToCBORPayload(doxm, payload, &cborLen, rwOnly);
        *size = cborLen;
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

OCStackResult CBORPayloadToDoxm(const uint8_t *cborPayload, size_t size,
                                OicSecDoxm_t **secDoxm)
{
    return CBORPayloadToDoxmBin(cborPayload, size, secDoxm, NULL);
}

static OCStackResult CBORPayloadToDoxmBin(const uint8_t *cborPayload, size_t size,
                                OicSecDoxm_t **secDoxm, bool *roParsed)
{
    if (NULL == cborPayload || NULL == secDoxm || NULL != *secDoxm || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    *secDoxm = NULL;

    CborParser parser;
    CborError cborFindResult = CborNoError;
    char* strUuid = NULL;
    size_t len = 0;
    CborValue doxmCbor;

    cbor_parser_init(cborPayload, size, 0, &parser, &doxmCbor);
    CborValue doxmMap;
    OicSecDoxm_t *doxm = (OicSecDoxm_t *)OICCalloc(1, sizeof(*doxm));
    VERIFY_NON_NULL(TAG, doxm, ERROR);

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_OXM_TYPE_NAME, &doxmMap);
    //OxmType -- not Mandatory
    if (CborNoError == cborFindResult && cbor_value_is_array(&doxmMap))
    {
        CborValue oxmType;

        cborFindResult = cbor_value_get_array_length(&doxmMap, &doxm->oxmTypeLen);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding oxmTypeLen.");
        VERIFY_SUCCESS(TAG, doxm->oxmTypeLen != 0, ERROR);

        doxm->oxmType = (OicUrn_t *)OICCalloc(doxm->oxmTypeLen, sizeof(*doxm->oxmType));
        VERIFY_NON_NULL(TAG, doxm->oxmType, ERROR);

        cborFindResult = cbor_value_enter_container(&doxmMap, &oxmType);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering oxmType Array.")

        int i = 0;
        size_t len = 0;
        while (cbor_value_is_valid(&oxmType) && cbor_value_is_text_string(&oxmType))
        {
            cborFindResult = cbor_value_dup_text_string(&oxmType, &doxm->oxmType[i++],
                                                        &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding omxType text string.");
            cborFindResult = cbor_value_advance(&oxmType);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing oxmType.");
        }
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_OXMS_NAME, &doxmMap);
    //Oxm -- not Mandatory
    if (CborNoError == cborFindResult && cbor_value_is_array(&doxmMap))
    {
        CborValue oxm;
        cborFindResult = cbor_value_get_array_length(&doxmMap, &doxm->oxmLen);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding oxmName array Length.");
        VERIFY_SUCCESS(TAG, doxm->oxmLen != 0, ERROR);

        doxm->oxm = (OicSecOxm_t *)OICCalloc(doxm->oxmLen, sizeof(*doxm->oxm));
        VERIFY_NON_NULL(TAG, doxm->oxm, ERROR);

        cborFindResult = cbor_value_enter_container(&doxmMap, &oxm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering oxmName Array.")

        int i = 0;
        while (cbor_value_is_valid(&oxm) && cbor_value_is_integer(&oxm))
        {
            int tmp;

            cborFindResult = cbor_value_get_int(&oxm, &tmp);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding oxmName Value")
            doxm->oxm[i++] = (OicSecOxm_t)tmp;
            cborFindResult = cbor_value_advance(&oxm);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing oxmName.")
        }

        if (roParsed)
        {
            *roParsed = true;
        }
    }
    else
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        doxm->oxm = (OicSecOxm_t *) OICCalloc(gDoxm->oxmLen, sizeof(*doxm->oxm));
        VERIFY_NON_NULL(TAG, doxm->oxm, ERROR);
        doxm->oxmLen = gDoxm->oxmLen;
        for (size_t i = 0; i < gDoxm->oxmLen; i++)
        {
            doxm->oxm[i] = gDoxm->oxm[i];
        }
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_OXM_SEL_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&doxmMap))
    {
        int oxmSel;

        cborFindResult = cbor_value_get_int(&doxmMap, &oxmSel);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Sel Name Value.")
        doxm->oxmSel = (OicSecOxm_t)oxmSel;
    }
    else // PUT/POST JSON may not have oxmsel so set it to the gDoxm->oxmSel
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        doxm->oxmSel = gDoxm->oxmSel;
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_SUPPORTED_CRED_TYPE_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&doxmMap))
    {
        int sct;

        cborFindResult = cbor_value_get_int(&doxmMap, &sct);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Sct Name Value.")
        doxm->sct = (OicSecCredType_t)sct;

        if (roParsed)
        {
            *roParsed = true;
        }
    }
    else // PUT/POST JSON may not have sct so set it to the gDoxm->sct
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        doxm->sct = gDoxm->sct;
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_OWNED_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_boolean(&doxmMap))
    {
        cborFindResult = cbor_value_get_boolean(&doxmMap, &doxm->owned);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Owned Value.")
    }
    else // PUT/POST JSON may not have owned so set it to the gDomx->owned
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        doxm->owned = gDoxm->owned;
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_DEVICE_ID_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&doxmMap))
    {
        cborFindResult = cbor_value_dup_text_string(&doxmMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Device Id Value.");
        ret = ConvertStrToUuid(strUuid , &doxm->deviceID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid);
        strUuid  = NULL;
    }
    else
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        memcpy(doxm->deviceID.id, &gDoxm->deviceID.id, sizeof(doxm->deviceID.id));
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_DEVOWNERID_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&doxmMap))
    {
        cborFindResult = cbor_value_dup_text_string(&doxmMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Owner Value.");
        ret = ConvertStrToUuid(strUuid , &doxm->owner);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid);
        strUuid  = NULL;
    }
    else
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        memcpy(doxm->owner.id, gDoxm->owner.id, sizeof(doxm->owner.id));
    }

#ifdef MULTIPLE_OWNER
    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_MOM_NAME, &doxmMap);
    if(CborNoError == cborFindResult && cbor_value_is_integer(&doxmMap))
    {
        int mode = 0;
        cborFindResult = cbor_value_get_int(&doxmMap, &mode);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding mom Name Value.")
        if(NULL == doxm->mom)
        {
            doxm->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
            VERIFY_NON_NULL(TAG, doxm->mom, ERROR);
        }
        doxm->mom->mode = (OicSecMomType_t)mode;
    }
    else if(NULL != gDoxm && NULL != gDoxm->mom)
    {
        // PUT/POST JSON may not have 'mom' so set it to the gDomx->mom
        if(NULL == doxm->mom)
        {
            doxm->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
            VERIFY_NON_NULL(TAG, doxm->mom, ERROR);
        }
        doxm->mom->mode = gDoxm->mom->mode;
    }

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_SUBOWNERID_NAME, &doxmMap);
    if(CborNoError == cborFindResult && cbor_value_is_array(&doxmMap))
    {
        size_t subOwnerLen = 0;
        CborValue subOwnerCbor;
        cborFindResult = cbor_value_get_array_length(&doxmMap, &subOwnerLen);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SubOwner array Length.");
        VERIFY_SUCCESS(TAG, 0 != subOwnerLen, ERROR);

        cborFindResult = cbor_value_enter_container(&doxmMap, &subOwnerCbor);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering SubOwner Array.")

        while (cbor_value_is_valid(&subOwnerCbor) && cbor_value_is_text_string(&subOwnerCbor))
        {
            OCStackResult convertRes = OC_STACK_ERROR;
            OicSecSubOwner_t* subOwner = NULL;
            char* strUuid = NULL;
            size_t uuidLen = 0;

            cborFindResult = cbor_value_dup_text_string(&subOwnerCbor, &strUuid, &uuidLen, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SubOwnerId Value");

            subOwner = (OicSecSubOwner_t*)OICCalloc(1, sizeof(OicSecSubOwner_t));
            VERIFY_NON_NULL(TAG, subOwner, ERROR);

            convertRes = ConvertStrToUuid(strUuid, &subOwner->uuid);
            VERIFY_SUCCESS(TAG, OC_STACK_OK == convertRes, ERROR);
            subOwner->status = MOT_STATUS_DONE;
            LL_APPEND(doxm->subOwners, subOwner);

            cborFindResult = cbor_value_advance(&subOwnerCbor);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing SubOwnerId.")
        }
    }
    else if(NULL != gDoxm && NULL != gDoxm->subOwners)
    {
        // PUT/POST JSON may not have 'subOwners' so set it to the gDomx->subOwners
        OicSecSubOwner_t* subOwnerItor = NULL;
        LL_FOREACH(gDoxm->subOwners, subOwnerItor)
        {
            OicSecSubOwner_t* subOwnerId = (OicSecSubOwner_t*)OICCalloc(1, sizeof(OicSecSubOwner_t));
            VERIFY_NON_NULL(TAG, subOwnerId, ERROR);

            memcpy(&subOwnerId->uuid, &subOwnerItor->uuid, sizeof(OicUuid_t));
            subOwnerId->status = MOT_STATUS_DONE;

            LL_APPEND(doxm->subOwners, subOwnerId);
        }
    }
#endif //MULTIPLE_OWNER

    cborFindResult = cbor_value_map_find_value(&doxmCbor, OIC_JSON_ROWNERID_NAME, &doxmMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&doxmMap))
    {
        cborFindResult = cbor_value_dup_text_string(&doxmMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ROwner Value.");
        ret = ConvertStrToUuid(strUuid , &doxm->rownerID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid);
        strUuid  = NULL;
    }
    else
    {
        VERIFY_NON_NULL(TAG, gDoxm, ERROR);
        memcpy(doxm->rownerID.id, gDoxm->rownerID.id, sizeof(doxm->rownerID.id));
    }

    *secDoxm = doxm;
    ret = OC_STACK_OK;

exit:
    if (CborNoError != cborFindResult)
    {
        OIC_LOG (ERROR, TAG, "CBORPayloadToDoxm failed!!!");
        DeleteDoxmBinData(doxm);
        doxm = NULL;
        *secDoxm = NULL;
        ret = OC_STACK_ERROR;
    }
    return ret;
}

/**
 * @todo document this function including why code might need to call this.
 * The current suspicion is that it's not being called as much as it should.
 */
static bool UpdatePersistentStorage(OicSecDoxm_t * doxm)
{
    bool bRet = false;

    if (NULL != doxm)
    {
        // Convert Doxm data into CBOR for update to persistent storage
        uint8_t *payload = NULL;
        size_t size = 0;
        OCStackResult res = DoxmToCBORPayload(doxm, &payload, &size, false);
        if (payload && (OC_STACK_OK == res)
            && (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, payload, size)))
        {
                bRet = true;
        }
        OICFree(payload);
    }
    else
    {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, NULL, 0))
        {
                bRet = true;
        }
    }

    return bRet;
}

static bool ValidateQuery(const char * query)
{
    // Send doxm resource data if the state of doxm resource
    // matches with the query parameters.
    // else send doxm resource data as NULL
    // TODO Remove this check and rely on Policy Engine
    // and Provisioning Mode to enforce provisioning-state
    // access rules. Eventually, the PE and PM code will
    // not send a request to the /doxm Entity Handler at all
    // if it should not respond.
    OIC_LOG (DEBUG, TAG, "In ValidateQuery");
    if(NULL == gDoxm)
    {
        return false;
    }

    bool bOwnedQry = false;         // does querystring contains 'owned' query ?
    bool bOwnedMatch = false;       // does 'owned' query value matches with doxm.owned status?
    bool bDeviceIDQry = false;      // does querystring contains 'deviceid' query ?
    bool bDeviceIDMatch = false;    // does 'deviceid' query matches with doxm.deviceid ?
    bool bInterfaceQry = false;      // does querystring contains 'if' query ?
    bool bInterfaceMatch = false;    // does 'if' query matches with oic.if.baseline ?
#ifdef MULTIPLE_OWNER
    bool bMotMatch = false;       // does 'mom' query value is not '0' && does query value matches with doxm.owned status?
#endif //MULTIPLE_OWNER

    OicParseQueryIter_t parseIter = {.attrPos = NULL};

    ParseQueryIterInit((unsigned char*)query, &parseIter);

    while (GetNextQuery(&parseIter))
    {
        if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_OWNED_NAME, parseIter.attrLen) == 0)
        {
            bOwnedQry = true;
            if ((strncasecmp((char *)parseIter.valPos, OIC_SEC_TRUE, parseIter.valLen) == 0) &&
                    (gDoxm->owned))
            {
                bOwnedMatch = true;
            }
            else if ((strncasecmp((char *)parseIter.valPos, OIC_SEC_FALSE, parseIter.valLen) == 0)
                    && (!gDoxm->owned))
            {
                bOwnedMatch = true;
            }
        }

#ifdef MULTIPLE_OWNER
        if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_MOM_NAME, strlen(OIC_JSON_MOM_NAME)) == 0)
        {
            OicSecMomType_t momMode = (OicSecMomType_t)(parseIter.valPos[0] - CHAR_ZERO);
            if(NULL != gDoxm->mom && momMode != gDoxm->mom->mode)
            {
                if(GetNextQuery(&parseIter))
                {
                    if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_OWNED_NAME, parseIter.attrLen) == 0)
                    {
                        if ((strncasecmp((char *)parseIter.valPos, OIC_SEC_TRUE, parseIter.valLen) == 0) &&
                                (gDoxm->owned))
                        {
                            bMotMatch = true;
                        }
                    }
                }
            }
            return bMotMatch;
        }
#endif //MULTIPLE_OWNER

        if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_DEVICE_ID_NAME, parseIter.attrLen) == 0)
        {
            bDeviceIDQry = true;
            OicUuid_t subject = {.id={0}};

            memcpy(subject.id, parseIter.valPos, parseIter.valLen);
            if (0 == memcmp(&gDoxm->deviceID.id, &subject.id, sizeof(gDoxm->deviceID.id)))
            {
                bDeviceIDMatch = true;
            }
        }

        if (strncasecmp((char *)parseIter.attrPos, OC_RSRVD_INTERFACE, parseIter.attrLen) == 0)
        {
            bInterfaceQry = true;
            if ((strncasecmp((char *)parseIter.valPos, OC_RSRVD_INTERFACE_DEFAULT, parseIter.valLen) == 0))
            {
                bInterfaceMatch = true;
            }
            return (bInterfaceQry ? bInterfaceMatch: true);
        }
    }

    return ((bOwnedQry ? bOwnedMatch : true) &&
            (bDeviceIDQry ? bDeviceIDMatch : true));
}

static OCEntityHandlerResult HandleDoxmGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_OK;

    OIC_LOG(DEBUG, TAG, "Doxm EntityHandle processing GET request");

    //Checking if Get request is a query.
    if (ehRequest->query)
    {
        OIC_LOG_V(DEBUG,TAG,"query:%s",ehRequest->query);
        OIC_LOG(DEBUG, TAG, "HandleDoxmGetRequest processing query");
        if (!ValidateQuery(ehRequest->query))
        {
            ehRet = OC_EH_ERROR;
        }
    }

    /*
     * For GET or Valid Query request return doxm resource CBOR payload.
     * For non-valid query return NULL json payload.
     * A device will 'always' have a default Doxm, so DoxmToCBORPayload will
     * return valid doxm resource json.
     */
    uint8_t *payload = NULL;
    size_t size = 0;

    if (ehRet == OC_EH_OK)
    {
        if (OC_STACK_OK != DoxmToCBORPayload(gDoxm, &payload, &size, false))
        {
            OIC_LOG(WARNING, TAG, "DoxmToCBORPayload failed in HandleDoxmGetRequest");
        }
    }

    OIC_LOG(DEBUG, TAG, "Send payload for doxm GET request");
    OIC_LOG_BUFFER(DEBUG, TAG, payload, size);

    // Send response payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OICFree(payload);

    return ehRet;
}

static void updateWriteableProperty(const OicSecDoxm_t* src, OicSecDoxm_t* dst)
{
    if(src && dst)
   {
        // update oxmsel
        dst->oxmSel = src->oxmSel;

        //update owner
        memcpy(&(dst->owner), &(src->owner), sizeof(OicUuid_t));

        //update rowner
        memcpy(&(dst->rownerID), &(src->rownerID), sizeof(OicUuid_t));

        //update deviceuuid
        memcpy(&(dst->deviceID), &(src->deviceID), sizeof(OicUuid_t));

        //Update owned status
        if(dst->owned != src->owned)
        {
            dst->owned = src->owned;
        }

#ifdef MULTIPLE_OWNER
        if(src->mom)
        {
            OIC_LOG(DEBUG, TAG, "dectected 'mom' property");
            if(NULL == dst->mom)
            {
                dst->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
                if(NULL != dst->mom)
                {
                    dst->mom->mode = src->mom->mode;
                }
            }
        }
#endif //MULTIPLE_OWNER
    }
}

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
#ifdef MULTIPLE_OWNER
/**
 * Internal function to get number of sub-owner
 */
static size_t GetSubOwnerSize()
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);

    size_t numberOfSubOwner = 0;

    if (gDoxm && gDoxm->subOwners)
    {
        OicSecSubOwner_t* subowner = NULL;
        LL_FOREACH(gDoxm->subOwners, subowner)
        {
            numberOfSubOwner++;
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Numer of registered sub-owner=%d", numberOfSubOwner);
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);

    return numberOfSubOwner;
}

/**
 * Callback function to handle MOT DTLS handshake result.
 * @param[out]   endpoint           remote device information.
 * @param[out]   errorInfo        CA Error information.
 */
void MultipleOwnerDTLSHandshakeCB(const CAEndpoint_t *endpoint,
                                const CAErrorInfo_t *errorInfo)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (!endpoint || !errorInfo)
    {
        OIC_LOG(ERROR, TAG, "Invalid param");
        return;
    }

    if ((CA_STATUS_OK == errorInfo->result) && (true == gDoxm->owned)
        && (OIC_PRECONFIG_PIN == gDoxm->oxmSel) && (NULL != gDoxm->mom)
        && (OIC_MULTIPLE_OWNER_DISABLE != gDoxm->mom->mode) && (CA_ADAPTER_TCP != endpoint->adapter))
    {
        OIC_LOG_V(INFO, TAG, "DTLS session established for sub-owner authentication : (%s:%d)",
                  endpoint->addr, endpoint->port);

        const CASecureEndpoint_t* authenticatedSubOwnerInfo = CAGetSecureEndpointData(endpoint);
        if (authenticatedSubOwnerInfo)
        {
            if (0 == memcmp(authenticatedSubOwnerInfo->identity.id, gDoxm->owner.id,
                            authenticatedSubOwnerInfo->identity.id_length))
            {
                OIC_LOG(WARNING, TAG, "Super owner tried MOT, this request will be ignored.");
                return;
            }

            OicSecSubOwner_t* subOwnerInst = NULL;
            LL_FOREACH(gDoxm->subOwners, subOwnerInst)
            {
                if(0 == memcmp(subOwnerInst->uuid.id,
                               authenticatedSubOwnerInfo->identity.id,
                               authenticatedSubOwnerInfo->identity.id_length))
                {
                    break;
                }
            }

            if (NULL == subOwnerInst)
            {
                subOwnerInst = (OicSecSubOwner_t*)OICCalloc(1, sizeof(OicSecSubOwner_t));
                if (subOwnerInst)
                {
                    char* strUuid = NULL;
                    if (OC_STACK_OK != ConvertUuidToStr(&subOwnerInst->uuid, &strUuid))
                    {
                        OIC_LOG(WARNING, TAG, "ConvertUuidToStr error");
                    }
                    OIC_LOG_V(DEBUG, TAG, "Adding New SubOwner(%s)", strUuid);

                    if (gMaxSubOwnerSize > GetSubOwnerSize())
                    {
                        memcpy(subOwnerInst->uuid.id, authenticatedSubOwnerInfo->identity.id,
                               authenticatedSubOwnerInfo->identity.id_length);
                        LL_APPEND(gDoxm->subOwners, subOwnerInst);
                        if (!UpdatePersistentStorage(gDoxm))
                        {
                            OIC_LOG(ERROR, TAG, "Failed to register SubOwner UUID into Doxm");
                        }
                    }
                    else
                    {
                        OIC_LOG_V(ERROR, TAG, "Number of sub-owner exceeded : (MAX SIZE=%d)", gMaxSubOwnerSize);

                        //Close DTLS session
                        if (CA_STATUS_OK != CAcloseSslSession(endpoint))
                        {
                            OIC_LOG_V(ERROR, TAG, "CAcloseSslSession error for [%s:%d]", endpoint->addr, endpoint->port);
                        }

                        //Remove credential
                        if (OC_STACK_RESOURCE_DELETED != RemoveCredential(&subOwnerInst->uuid))
                        {
                            OIC_LOG_V(ERROR, TAG, "RemoveCredential error for [%s]", strUuid);
                        }

                        // TODO: How to send error to client side?
                    }

                    OICFree(strUuid);
                }
            }
        }
    }

    if(CA_STATUS_OK != CAregisterPskCredentialsHandler(GetDtlsPskCredentials))
    {
        OIC_LOG(WARNING, TAG, "Failed to revert the DTLS credential handler");
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}
#endif //MULTIPLE_OWNER
#endif // defined(__WITH_DTLS__) || defined (__WITH_TLS__)

/**
 * Function to validate oxmsel with oxms.
 *
 * @param[in] supportedMethods   Array of supported methods
 * @param[in] numberOfMethods   number of supported methods
 * @param[out]  selectedMethod         Selected methods
 * @return  TRUE on success
 */
static bool ValidateOxmsel(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod)
{
    bool isValidOxmsel = false;

    OIC_LOG(DEBUG, TAG, "IN ValidateOxmsel");
    if (numberOfMethods == 0 || !supportedMethods)
    {
        OIC_LOG(WARNING, TAG, "Could not find a supported OxM.");
        return isValidOxmsel;
    }

    for (size_t i = 0; i < numberOfMethods; i++)
    {
            if (*selectedMethod  == supportedMethods[i])
            {
                isValidOxmsel = true;
                break;
            }
    }
    if (!isValidOxmsel)
    {
        OIC_LOG(ERROR, TAG, "Not allowed Oxmsel.");
        return isValidOxmsel;
    }

    OIC_LOG(DEBUG, TAG, "OUT ValidateOxmsel");

    return isValidOxmsel;
}

static OCEntityHandlerResult HandleDoxmPostRequest(OCEntityHandlerRequest * ehRequest)
{
    OIC_LOG (DEBUG, TAG, "Doxm EntityHandle  processing POST request");
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicUuid_t emptyOwner = {.id = {0} };
    static uint16_t previousMsgId = 0;
    bool isDuplicatedMsg = false;

    /*
     * Convert CBOR Doxm data into binary. This will also validate
     * the Doxm data received.
     */
    OicSecDoxm_t *newDoxm = NULL;

    if (ehRequest->payload)
    {
        uint8_t *payload = ((OCSecurityPayload *)ehRequest->payload)->securityData;
        size_t size = ((OCSecurityPayload *)ehRequest->payload)->payloadSize;
        bool roParsed = false;
        OCStackResult res = CBORPayloadToDoxmBin(payload, size, &newDoxm, &roParsed);
        if (newDoxm && OC_STACK_OK == res)
        {
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

            if (isDuplicatedMsg && ehRequest->messageID == gConfirmMsgId)
            {
                if (CONFIRM_STATE_WAIT == gConfirmState)
                {
                    OIC_LOG(DEBUG, TAG, "Confirm callback already invoked.");
                    OIC_LOG(DEBUG, TAG, "This request will be ignored.");
                    DeleteDoxmBinData(newDoxm);
                    return OC_EH_OK;
                }
                else
                {
                    OIC_LOG_V(DEBUG, TAG, "Confirm request already done, Confirm Result = %s", (CONFIRM_STATE_ACCEPTED == gConfirmState ? "ACCEPTED" : "DENIED"));
                    ehRet = (CONFIRM_STATE_ACCEPTED == gConfirmState ? OC_EH_OK : OC_EH_NOT_ACCEPTABLE);
                    goto exit;
                }
            }

            // Check request on RO property
            if (true == roParsed)
            {
                OIC_LOG(ERROR, TAG, "Not acceptable request because of read-only propertys");
                ehRet = OC_EH_NOT_ACCEPTABLE;
                goto exit;
            }

            // in owned state
            if (true == gDoxm->owned)
            {
                if (false == ValidateOxmsel(gDoxm->oxm, gDoxm->oxmLen, &newDoxm->oxmSel))
                {
                    OIC_LOG(ERROR, TAG, "Not acceptable request because oxmsel does not support on Server");
                    ehRet = OC_EH_NOT_ACCEPTABLE;
                    goto exit;
                }
                //Update gDoxm based on newDoxm
                updateWriteableProperty(newDoxm, gDoxm);

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
#ifdef MULTIPLE_OWNER
                //handle mom
                if(gDoxm->mom)
                {
                    if(OIC_MULTIPLE_OWNER_DISABLE != gDoxm->mom->mode)
                    {
                        CAResult_t caRes = CA_STATUS_FAILED;
                        if(OIC_PRECONFIG_PIN == gDoxm->oxmSel || OIC_RANDOM_DEVICE_PIN == gDoxm->oxmSel)
                        {
                            caRes = CAEnableAnonECDHCipherSuite(false);
                            VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                            OIC_LOG(INFO, TAG, "ECDH_ANON CipherSuite is DISABLED");

                            caRes = CASelectCipherSuite((uint16_t)MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, ehRequest->devAddr.adapter);
                            VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                            OIC_LOG(INFO, TAG, "ECDHE_PSK CipherSuite will be used for MOT");

                            //Set the device id to derive temporal PSK
                            SetUuidForPinBasedOxm(&gDoxm->deviceID);
                        }
                        else
                        {
                            OIC_LOG(WARNING, TAG, "Unsupported OxM for Multiple Ownership Transfer.");
                        }

                        CAregisterSslHandshakeCallback(MultipleOwnerDTLSHandshakeCB);
                    }
                    else
                    {
                        //if MOM is disabled, revert the DTLS handshake callback
                        if(CA_STATUS_OK != CAregisterSslHandshakeCallback(NULL))
                        {
                            OIC_LOG(WARNING, TAG, "Error while revert the DTLS Handshake Callback.");
                        }
                    }
                }

                if(newDoxm->subOwners)
                {
                    OicSecSubOwner_t* subowner = NULL;
                    OicSecSubOwner_t* temp = NULL;

                    OIC_LOG(DEBUG, TAG, "dectected 'subowners' property");

                    if(gDoxm->subOwners)
                    {
                        LL_FOREACH_SAFE(gDoxm->subOwners, subowner, temp)
                        {
                            LL_DELETE(gDoxm->subOwners, subowner);
                            OICFree(subowner);
                        }
                    }

                    subowner = NULL;
                    temp = NULL;
                    LL_FOREACH_SAFE(newDoxm->subOwners, subowner, temp)
                    {
                        LL_DELETE(newDoxm->subOwners, subowner);
                        LL_APPEND(gDoxm->subOwners, subowner);
                    }
                }
#endif //MULTIPLE_OWNER
#endif // defined(__WITH_DTLS__) || defined (__WITH_TLS__)

                //Update new state in persistent storage
                if (UpdatePersistentStorage(gDoxm) == true)
                {
                    ehRet = OC_EH_OK;
                }
                else
                {
                    OIC_LOG(ERROR, TAG, "Failed to update DOXM in persistent storage");
                    ehRet = OC_EH_ERROR;
                }
                goto exit;
            }

            // in unowned state
            if ((false == gDoxm->owned) && (false == newDoxm->owned))
            {
                if (false == ValidateOxmsel(gDoxm->oxm, gDoxm->oxmLen, &newDoxm->oxmSel))
                {
                    OIC_LOG(ERROR, TAG, "Not acceptable request because oxmsel does not support on Server");
                    ehRet = OC_EH_NOT_ACCEPTABLE;
                    goto exit;
                }

#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
                if (memcmp(&(newDoxm->owner), &emptyOwner, sizeof(OicUuid_t)) == 0)
                {
                    InvokeOtmEventHandler(ehRequest->devAddr.addr, ehRequest->devAddr.port,
                                          NULL, OIC_OTM_STARTED);
                }
#endif

                if (OIC_JUST_WORKS == newDoxm->oxmSel || OIC_MV_JUST_WORKS == newDoxm->oxmSel)
                {
                    /*
                     * If current state of the device is un-owned, enable
                     * anonymous ECDH cipher in tinyDTLS so that Provisioning
                     * tool can initiate JUST_WORKS ownership transfer process.
                     */
                    if (memcmp(&(newDoxm->owner), &emptyOwner, sizeof(OicUuid_t)) == 0)
                    {
                        gDoxm->oxmSel = newDoxm->oxmSel;
                        //Update new state in persistent storage
                        if ((UpdatePersistentStorage(gDoxm) == true))
                        {
                            ehRet = OC_EH_OK;
                        }
                        else
                        {
                            OIC_LOG(WARNING, TAG, "Failed to update DOXM in persistent storage");
                            ehRet = OC_EH_ERROR;
                            goto exit;
                        }
                        OIC_LOG (INFO, TAG, "Doxm EntityHandle  enabling AnonECDHCipherSuite");
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                        ehRet = (CAEnableAnonECDHCipherSuite(true) == CA_STATUS_OK) ? OC_EH_OK : OC_EH_ERROR;
#endif // __WITH_DTLS__ or __WITH_TLS__
                        goto exit;
                    }
                    else
                    {
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                        //Save the owner's UUID to derive owner credential
                        memcpy(&(gDoxm->owner), &(newDoxm->owner), sizeof(OicUuid_t));

                        // Update new state in persistent storage
                        if (true == UpdatePersistentStorage(gDoxm))
                        {
                            ehRet = OC_EH_OK;
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Failed to update DOXM in persistent storage");
                            ehRet = OC_EH_ERROR;
                            goto exit;
                        }

                        /*
                         * Disable anonymous ECDH cipher in tinyDTLS since device is now
                         * in owned state.
                         */
                        CAResult_t caRes = CA_STATUS_OK;
                        caRes = CAEnableAnonECDHCipherSuite(false);
                        VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                        OIC_LOG(INFO, TAG, "ECDH_ANON CipherSuite is DISABLED");

                        //In case of Mutual Verified Just-Works, verify mutualVerifNum
                        if (OIC_MV_JUST_WORKS == newDoxm->oxmSel && false == newDoxm->owned &&
                            false == isDuplicatedMsg)
                        {
                            uint8_t preMutualVerifNum[OWNER_PSK_LENGTH_128] = {0};
                            uint8_t mutualVerifNum[MUTUAL_VERIF_NUM_LEN] = {0};
                            OicUuid_t deviceID = {.id = {0}};

                            //Generate mutualVerifNum
                            OCServerRequest * request = GetServerRequestUsingHandle(ehRequest->requestHandle);

                            char label[LABEL_LEN] = {0};
                            snprintf(label, LABEL_LEN, "%s%s", MUTUAL_VERIF_NUM, OXM_MV_JUST_WORKS);
                            if (OC_STACK_OK != GetDoxmDeviceID(&deviceID))
                            {
                                OIC_LOG(ERROR, TAG, "Error while retrieving Owner's device ID");
                                ehRet = OC_EH_ERROR;
                                goto exit;

                            }

                            CAResult_t pskRet = CAGenerateOwnerPSK((CAEndpoint_t *)&request->devAddr,
                                    (uint8_t *)label,
                                    strlen(label),
                                    gDoxm->owner.id, sizeof(gDoxm->owner.id),
                                    gDoxm->deviceID.id, sizeof(gDoxm->deviceID.id),
                                    preMutualVerifNum, OWNER_PSK_LENGTH_128);
                            if (CA_STATUS_OK != pskRet)
                            {
                                OIC_LOG(WARNING, TAG, "Failed to remove the invaild owner credential");
                                ehRet = OC_EH_ERROR;
                                goto exit;

                            }

                            memcpy(mutualVerifNum, preMutualVerifNum + OWNER_PSK_LENGTH_128 - sizeof(mutualVerifNum),
                                    sizeof(mutualVerifNum));

                            gConfirmMsgId = ehRequest->messageID;
                            gConfirmState = CONFIRM_STATE_WAIT;
                            //Wait for user confirmation
                            if (OC_STACK_OK != VerifyOwnershipTransfer(mutualVerifNum, DISPLAY_NUM | USER_CONFIRM))
                            {
                                ehRet = OC_EH_NOT_ACCEPTABLE;
                                gConfirmState = CONFIRM_STATE_DENIED;
                            }
                            else
                            {
                                ehRet = OC_EH_OK;
                                gConfirmState = CONFIRM_STATE_ACCEPTED;
                            }
                        }

#endif // __WITH_DTLS__ or __WITH_TLS__
                    }
                }
                else if (OIC_RANDOM_DEVICE_PIN == newDoxm->oxmSel)
                {
                    /*
                     * If current state of the device is un-owned, enable
                     * anonymous ECDH cipher in tinyDTLS so that Provisioning
                     * tool can initiate JUST_WORKS ownership transfer process.
                     */
                    if(memcmp(&(newDoxm->owner), &emptyOwner, sizeof(OicUuid_t)) == 0)
                    {
                        gDoxm->oxmSel = newDoxm->oxmSel;
                        //Update new state in persistent storage
                        if ((UpdatePersistentStorage(gDoxm) == true))
                        {
                            ehRet = OC_EH_OK;
                        }
                        else
                        {
                            OIC_LOG(WARNING, TAG, "Failed to update DOXM in persistent storage");
                            ehRet = OC_EH_ERROR;
                        }

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                        CAResult_t caRes = CA_STATUS_OK;

                        caRes = CAEnableAnonECDHCipherSuite(false);
                        VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                        OIC_LOG(INFO, TAG, "ECDH_ANON CipherSuite is DISABLED");

                        caRes = CASelectCipherSuite(MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256,
                                                    ehRequest->devAddr.adapter);
                        VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);

                        if (!isDuplicatedMsg)
                        {
                            char ranPin[OXM_RANDOM_PIN_MAX_SIZE + 1] = {0};
                            if (OC_STACK_OK == GeneratePin(ranPin, sizeof(ranPin)))
                            {
                                //Set the device id to derive temporal PSK
                                SetUuidForPinBasedOxm(&gDoxm->deviceID);

                                /**
                                 * Since PSK will be used directly by DTLS layer while PIN based ownership transfer,
                                 * Credential should not be saved into SVR.
                                 * For this reason, use a temporary get_psk_info callback to random PIN OxM.
                                 */
                                caRes = CAregisterPskCredentialsHandler(GetDtlsPskForRandomPinOxm);
                                VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                                ehRet = OC_EH_OK;
                            }
                            else
                            {
                                OIC_LOG(ERROR, TAG, "Failed to generate random PIN");
                                ehRet = OC_EH_ERROR;
                            }
                        }
#endif // __WITH_DTLS__ or __WITH_TLS__
                    }
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
                    else
                    {
                        //Save the owner's UUID to derive owner credential
                        memcpy(&(gDoxm->owner), &(newDoxm->owner), sizeof(OicUuid_t));

                        // In case of random-pin based OTM, close the PIN display if callback is registered.
                        if (!isDuplicatedMsg)
                        {
                            ClosePinDisplay();
                        }

                        //Update new state in persistent storage
                        if (UpdatePersistentStorage(gDoxm) == true)
                        {
                            ehRet = OC_EH_OK;
                        }
                        else
                        {
                            OIC_LOG(ERROR, TAG, "Failed to update DOXM in persistent storage");
                            ehRet = OC_EH_ERROR;
                        }
                    }
#endif // __WITH_DTLS__ or __WITH_TLS__
                }
#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
                else if (OIC_MANUFACTURER_CERTIFICATE ==  newDoxm->oxmSel || OIC_CON_MFG_CERT == newDoxm->oxmSel)
                {
                    //In case of Confirm Manufacturer Cert, get user confirmation
                    if (OIC_CON_MFG_CERT == newDoxm->oxmSel && false == newDoxm->owned &&
                        false == isDuplicatedMsg &&
                        memcmp(&(newDoxm->owner), &emptyOwner, sizeof(OicUuid_t)) != 0)
                    {
                        gConfirmMsgId = ehRequest->messageID;
                        gConfirmState = CONFIRM_STATE_WAIT;
                        if (OC_STACK_OK != VerifyOwnershipTransfer(NULL, USER_CONFIRM))
                        {
                            ehRet = OC_EH_NOT_ACCEPTABLE;
                            gConfirmState = CONFIRM_STATE_DENIED;
                            goto exit;
                        }
                        else
                        {
                            ehRet = OC_EH_OK;
                            gConfirmState = CONFIRM_STATE_ACCEPTED;
                        }
                    }

                    //Save the owner's UUID to derive owner credential
                    memcpy(&(gDoxm->owner), &(newDoxm->owner), sizeof(OicUuid_t));
                    gDoxm->oxmSel = newDoxm->oxmSel;
                    //Update new state in persistent storage
                    if (UpdatePersistentStorage(gDoxm))
                    {
                        ehRet = OC_EH_OK;
                    }
                    else
                    {
                        OIC_LOG(WARNING, TAG, "Failed to update DOXM in persistent storage");
                        ehRet = OC_EH_ERROR;
                    }
                    CAResult_t caRes = CAEnableAnonECDHCipherSuite(false);
                    VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                    OIC_LOG(INFO, TAG, "ECDH_ANON CipherSuite is DISABLED");

                    //Unset pre-selected ciphersuite, if any
                    caRes = CASelectCipherSuite(0, ehRequest->devAddr.adapter);
                    VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
                    OIC_LOG(DEBUG, TAG, "No ciphersuite preferred");

                    VERIFY_SUCCESS(TAG, CA_STATUS_OK == CAregisterPkixInfoHandler(GetManufacturerPkixInfo), ERROR);
                    VERIFY_SUCCESS(TAG, CA_STATUS_OK == CAregisterGetCredentialTypesHandler(InitManufacturerCipherSuiteList), ERROR);
                }
#endif // __WITH_DTLS__ or __WITH_TLS__
            }

            /*
             * When current state of the device is un-owned and Provisioning
             * Tool is attempting to change the state to 'Owned' with a
             * qualified value for the field 'Owner'
             */
            if ((false == gDoxm->owned) && (true == newDoxm->owned) &&
                    (memcmp(&(gDoxm->owner), &(newDoxm->owner), sizeof(OicUuid_t)) == 0))
            {
                //Change the SVR's resource owner as owner device.
                OCStackResult ownerRes = SetAclRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }
                ownerRes = SetAmaclRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }
                ownerRes = SetCredRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }
                ownerRes = SetPstatRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }
                ownerRes = SetDpairingRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }
                ownerRes = SetPconfRownerId(&gDoxm->owner);
                if(OC_STACK_OK != ownerRes && OC_STACK_NO_RESOURCE != ownerRes)
                {
                    ehRet = OC_EH_ERROR;
                    goto exit;
                }

                gDoxm->owned = true;
                memcpy(&gDoxm->rownerID, &gDoxm->owner, sizeof(OicUuid_t));

                // Update new state in persistent storage
                if (UpdatePersistentStorage(gDoxm))
                {
                    //Update default ACE of security resource to prevent anonymous user access.
                    if(OC_STACK_OK == UpdateDefaultSecProvACE())
                    {
                        ehRet = OC_EH_OK;
                    }
                    else
                    {
                        OIC_LOG(ERROR, TAG, "Failed to remove default ACL for security provisioning");
                        ehRet = OC_EH_ERROR;
                    }
                }
                else
                {
                    OIC_LOG(ERROR, TAG, "Failed to update DOXM in persistent storage");
                    ehRet = OC_EH_ERROR;
                }
#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
                if (OIC_MANUFACTURER_CERTIFICATE == gDoxm->oxmSel ||
                                            OIC_CON_MFG_CERT== gDoxm->oxmSel)
                {
                    CAregisterPkixInfoHandler(GetPkixInfo);
                    CAregisterGetCredentialTypesHandler(InitCipherSuiteList);
                }

                InvokeOtmEventHandler(ehRequest->devAddr.addr, ehRequest->devAddr.port,
                                      &gDoxm->owner, OIC_OTM_DONE);
#endif // __WITH_DTLS__ or __WITH_TLS__
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
        if(gDoxm)
        {
            if(!gDoxm->owned)
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

    previousMsgId = ehRequest->messageID;

    //Send payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    DeleteDoxmBinData(newDoxm);

    return ehRet;
}

#ifdef MULTIPLE_OWNER
static OCEntityHandlerResult HandleDoxmDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(DEBUG, TAG, "Processing DoxmDeleteRequest");

    OCEntityHandlerResult ehRet = OC_EH_BAD_REQ;

    if (NULL == ehRequest->query)
    {
        return ehRet;
    }

    OicParseQueryIter_t parseIter = { .attrPos=NULL };
    OicUuid_t subject = {.id={0}};

    //Parsing REST query to get the subject
    ParseQueryIterInit((unsigned char *)ehRequest->query, &parseIter);
    while (GetNextQuery(&parseIter))
    {
        if (strncasecmp((char *)parseIter.attrPos, OIC_JSON_SUBOWNERID_NAME,
                parseIter.attrLen) == 0)
        {
            if (0 == strncmp((const char*)parseIter.valPos, WILDCARD_RESOURCE_URI,
                             strlen(WILDCARD_RESOURCE_URI)))
            {
                if(OC_STACK_RESOURCE_DELETED == RemoveSubOwner(&WILDCARD_SUBJECT_ID))
                {
                    ehRet = OC_EH_RESOURCE_DELETED;
                }
            }
            else
            {
                OCStackResult ret = ConvertStrToUuid((const char*)parseIter.valPos, &subject);
                VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

                if(OC_STACK_RESOURCE_DELETED == RemoveSubOwner(&subject))
                {
                    ehRet = OC_EH_RESOURCE_DELETED;
                }
            }
        }
    }

    //Send response to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    return ehRet;
exit:
    return OC_EH_ERROR;
}
#endif //MULTIPLE_OWNER

OCEntityHandlerResult DoxmEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParam)
{
    (void)callbackParam;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if(NULL == ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");

        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleDoxmGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandleDoxmPostRequest(ehRequest);
                break;

#ifdef MULTIPLE_OWNER
            case OC_REST_DELETE:
                ehRet = HandleDoxmDeleteRequest(ehRequest);
                break;
#endif //MULTIPLE_OWNER

            default:
                ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                               OC_EH_OK : OC_EH_ERROR;
                break;
        }
    }

    return ehRet;
}

OCStackResult CreateDoxmResource()
{
    OCStackResult ret = OCCreateResource(&gDoxmHandle,
                                         OIC_RSRC_TYPE_SEC_DOXM,
                                         OC_RSRVD_INTERFACE_DEFAULT,
                                         OIC_RSRC_DOXM_URI,
                                         DoxmEntityHandler,
                                         NULL,
                                         OC_SECURE |
                                         OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (FATAL, TAG, "Unable to instantiate Doxm resource");
        DeInitDoxmResource();
    }
    return ret;
}

/**
 * Checks if DeviceID is generated during provisioning for the new device.
 * If DeviceID is NULL then generates the new DeviceID.
 * Once DeviceID is assigned to the device it does not change for the lifetime of the device.
 */
static OCStackResult CheckDeviceID()
{
    OCStackResult ret = OC_STACK_ERROR;
    bool validId = false;

    for (uint8_t i = 0; i < UUID_LENGTH; i++)
    {
        if (gDoxm->deviceID.id[i] != 0)
        {
            validId = true;
            break;
        }
    }

    if (!validId)
    {
        char* strUuid = NULL;
#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
        //If seed value is exist, generate UUID using seed with SHA256
        if (0 != gUuidSeedSize)
        {
            uint8_t hashVal[MBEDTLS_MD_MAX_SIZE] = {0};
            mbedtls_md_context_t sha_ctx;
            int mbedret = 1;

            OIC_LOG(DEBUG, TAG, "UUID will be generated using seed w/ SHA256");
            OIC_LOG(DEBUG, TAG, "Seed value : ");
            OIC_LOG_BUFFER(DEBUG, TAG, gUuidSeed, gUuidSeedSize);

            mbedtls_md_init( &sha_ctx );
            mbedret = mbedtls_md_setup( &sha_ctx, mbedtls_md_info_from_type( MBEDTLS_MD_SHA256 ), 1 );
            if (0 == mbedret)
            {
                mbedtls_md_starts( &sha_ctx );
                mbedtls_md_update( &sha_ctx, gUuidSeed, gUuidSeedSize);
                mbedtls_md_finish(&sha_ctx, (unsigned char*)hashVal);
                memcpy(gDoxm->deviceID.id, hashVal, sizeof(gDoxm->deviceID.id));
                ret = OC_STACK_OK;
            }
            else
            {
                OIC_LOG_V(ERROR, TAG,  "mbedtls_md_setup() returned -0x%04x\n", -mbedret);
                ret = OC_STACK_ERROR;
            }
            mbedtls_md_free( &sha_ctx );
        }
        else
        {
            if (RAND_UUID_OK != OCGenerateUuid(gDoxm->deviceID.id))
            {
                OIC_LOG(FATAL, TAG, "Generate UUID for Server Instance failed!");
                return OC_STACK_ERROR;
            }
            ret = OC_STACK_OK;
        }
#else
        if (RAND_UUID_OK != OCGenerateUuid(gDoxm->deviceID.id))
        {
            OIC_LOG(FATAL, TAG, "Generate UUID for Server Instance failed!");
            return ret;
        }
        ret = OC_STACK_OK;
#endif

        if (OC_STACK_OK == ConvertUuidToStr(&gDoxm->deviceID, &strUuid))
        {
            OIC_LOG_V(DEBUG, TAG, "Generated device UUID is [%s]", strUuid);
            OICFree(strUuid);
        }
        else
        {
            OIC_LOG(WARNING, TAG, "Failed to convert UUID to string");
        }


        if (!UpdatePersistentStorage(gDoxm))
        {
            //TODO: After registering PSI handler in all samples, do ret = OC_STACK_OK here.
            OIC_LOG(FATAL, TAG, "UpdatePersistentStorage failed!");
        }
    }
    else
    {
        ret = OC_STACK_OK;
    }
    return ret;
}

/**
 * Get the default value.
 *
 * @return the default value of doxm, @ref OicSecDoxm_t.
 */
static OicSecDoxm_t* GetDoxmDefault()
{
    OIC_LOG(DEBUG, TAG, "GetDoxmToDefault");
    return &gDefaultDoxm;
}

const OicSecDoxm_t* GetDoxmResourceData()
{
    return gDoxm;
}

#if defined(__WITH_DTLS__) && defined(MULTIPLE_OWNER)
/**
 * Internal API to prepare MOT
 */
static void PrepareMOT(const OicSecDoxm_t* doxm)
{
    OIC_LOG(INFO, TAG, "IN PrepareMOT");
    VERIFY_NON_NULL(TAG, doxm, ERROR);

    if(true == doxm->owned && NULL != doxm->mom && OIC_MULTIPLE_OWNER_DISABLE != doxm->mom->mode)
    {
        CAResult_t caRes = CA_STATUS_FAILED;

        OIC_LOG(INFO, TAG, "Multiple Ownership Transfer Enabled!");

        if(OIC_PRECONFIG_PIN == doxm->oxmSel)
        {
            caRes = CAEnableAnonECDHCipherSuite(false);
            VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
            OIC_LOG(INFO, TAG, "ECDH_ANON CipherSuite is DISABLED");

            caRes = CASelectCipherSuite((uint16_t)MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_IP);
            VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
#ifdef __WITH_TLS__
            caRes = CASelectCipherSuite((uint16_t)MBEDTLS_TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256, CA_ADAPTER_TCP);
            VERIFY_SUCCESS(TAG, caRes == CA_STATUS_OK, ERROR);
#endif
            OIC_LOG(INFO, TAG, "ECDHE_PSK CipherSuite will be used for MOT");

            //Set the device id to derive temporal PSK
            SetUuidForPinBasedOxm(&doxm->deviceID);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Unsupported OxM for Multiple Ownership Transfer.");
            return;
        }

        CAregisterSslHandshakeCallback(MultipleOwnerDTLSHandshakeCB);
    }

    OIC_LOG(INFO, TAG, "OUT PrepareMOT");
    return;
exit:
    OIC_LOG(WARNING, TAG, "Error in PrepareMOT");
}
#endif //defined(__WITH_DTLS__) && defined(MULTIPLE_OWNER)

OCStackResult InitDoxmResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    gConfirmState = CONFIRM_STATE_READY;
    gConfirmMsgId = 0;

    //Read DOXM resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_DOXM_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
       OIC_LOG (DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
       // Read DOXM resource from PS
       ret = CBORPayloadToDoxm(data, size, &gDoxm);
    }
    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default doxm is created
     * which allows user to initiate doxm provisioning again.
     */
     if ((OC_STACK_OK != ret) || !data || !gDoxm)
    {
        gDoxm = GetDoxmDefault();
    }

    //In case of the server is shut down unintentionally, we should initialize the owner
    if(gDoxm && (false == gDoxm->owned))
    {
        OicUuid_t emptyUuid = {.id={0}};
        memcpy(&gDoxm->owner, &emptyUuid, sizeof(OicUuid_t));
#if defined (__WITH_TLS__) || defined(__WITH_DTLS__)
        InvokeOtmEventHandler(NULL, 0, NULL, OIC_OTM_READY);
#endif
    }

    ret = CheckDeviceID();
    if (ret == OC_STACK_OK)
    {
        OIC_LOG_V(DEBUG, TAG, "Initial Doxm Owned = %d", gDoxm->owned);
        //Instantiate 'oic.sec.doxm'
        ret = CreateDoxmResource();
    }
    else
    {
        OIC_LOG (ERROR, TAG, "CheckDeviceID failed");
    }
    OICFree(data);

#if defined(__WITH_DTLS__) && defined(MULTIPLE_OWNER)
    //if MOT is enabled, MOT should be prepared.
    if(gDoxm && gDoxm->owned)
    {
        PrepareMOT(gDoxm);
    }
#endif // defined(__WITH_DTLS__) && defined(MULTIPLE_OWNER)

    return ret;
}

OCStackResult DeInitDoxmResource()
{
    OCStackResult ret = OCDeleteResource(gDoxmHandle);
    if (gDoxm  != &gDefaultDoxm)
    {
        DeleteDoxmBinData(gDoxm);
    }
    gDoxm = NULL;

    if (OC_STACK_OK == ret)
    {
        return OC_STACK_OK;
    }
    else
    {
        return OC_STACK_ERROR;
    }
}

OCStackResult GetDoxmDeviceID(OicUuid_t *deviceID)
{
    if (deviceID && gDoxm)
    {
       *deviceID = gDoxm->deviceID;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult GetDoxmIsOwned(bool *isOwned)
{
    if (isOwned && gDoxm)
    {
       *isOwned = gDoxm->owned;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
OCStackResult SetDoxmDeviceIDSeed(const uint8_t* seed, size_t seedSize)
{
    OIC_LOG_V(INFO, TAG, "In %s", __func__);

    if (NULL == seed)
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (MAX_UUID_SEED_SIZE < seedSize)
    {
        OIC_LOG_V(ERROR, TAG, "Seed size is too long (MAX size is %d bytes)", MAX_UUID_SEED_SIZE);
        return OC_STACK_INVALID_PARAM;
    }
    if (MIN_UUID_SEED_SIZE > seedSize)
    {
        OIC_LOG_V(ERROR, TAG, "Seed size is too small (MIN size is %d bytes)", MIN_UUID_SEED_SIZE);
        return OC_STACK_INVALID_PARAM;
    }

    memset(gUuidSeed, 0x00, sizeof(gUuidSeed));
    memcpy(gUuidSeed, seed, seedSize);
    gUuidSeedSize = seedSize;

    OIC_LOG_V(INFO, TAG, "Out %s", __func__);

    return OC_STACK_OK;
}
#endif

OCStackResult SetDoxmDeviceID(const OicUuid_t *deviceID)
{
    bool isOwnerUpdated = false;
    bool isRownerUpdated = false;
    if (NULL == deviceID)
    {
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == gDoxm)
    {
        OIC_LOG(ERROR, TAG, "Doxm resource is not initialized.");
        return OC_STACK_NO_RESOURCE;
    }

#ifdef __WITH_DTLS__
    //for normal device.
    if (true == gDoxm->owned &&
        memcmp(gDoxm->deviceID.id, gDoxm->owner.id, sizeof(gDoxm->owner.id)) != 0)
    {
        OIC_LOG(ERROR, TAG, "This device owned by owner's device.");
        OIC_LOG(ERROR, TAG, "Device UUID cannot be changed to guarantee the reliability of the connection.");
        return OC_STACK_ERROR;
    }
#endif //__WITH_DTLS

    //Save the previous UUID
    OicUuid_t prevUuid;
    memcpy(prevUuid.id, gDoxm->deviceID.id, sizeof(prevUuid.id));

    //Change the device UUID
    memcpy(gDoxm->deviceID.id, deviceID->id, sizeof(deviceID->id));

    //Change the owner ID if necessary
    if (memcmp(gDoxm->owner.id, prevUuid.id, sizeof(prevUuid.id)) == 0)
    {
        memcpy(gDoxm->owner.id, deviceID->id, sizeof(deviceID->id));
        isOwnerUpdated = true;
    }
    //Change the resource owner ID if necessary
    if (memcmp(gDoxm->rownerID.id, prevUuid.id, sizeof(prevUuid.id)) == 0)
    {
        memcpy(gDoxm->rownerID.id, deviceID->id, sizeof(deviceID->id));
        isRownerUpdated = true;
    }
    // TODO: T.B.D Change resource owner for pstat, acl and cred

    //Update PS
    if (!UpdatePersistentStorage(gDoxm))
    {
        //revert UUID in case of PSI error
        memcpy(gDoxm->deviceID.id, prevUuid.id, sizeof(prevUuid.id));
        if (isOwnerUpdated)
        {
            memcpy(gDoxm->owner.id, prevUuid.id, sizeof(prevUuid.id));
        }
        if (isRownerUpdated)
        {
            memcpy(gDoxm->rownerID.id, prevUuid.id, sizeof(prevUuid.id));
        }
        // TODO: T.B.D Revert resource owner for pstat, acl and cred

        OIC_LOG(ERROR, TAG, "Failed to update persistent storage");
        return OC_STACK_ERROR;
    }
    return OC_STACK_OK;
}

OCStackResult GetDoxmDevOwnerId(OicUuid_t *devownerid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gDoxm)
    {
        OIC_LOG_V(DEBUG, TAG, "GetDoxmDevOwnerId(): gDoxm owned =  %d.", \
            gDoxm->owned);
        if (gDoxm->owned)
        {
            *devownerid = gDoxm->owner;
            retVal = OC_STACK_OK;
        }
    }
    return retVal;
}

OCStackResult GetDoxmRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gDoxm)
    {
        if( gDoxm->owned )
        {
            *rowneruuid = gDoxm->rownerID;
                    retVal = OC_STACK_OK;
        }
    }
    return retVal;
}

#ifdef MULTIPLE_OWNER
/**
 * Compare the UUID to SubOwner.
 *
 * @param[in] uuid device UUID
 *
 * @return true if context->subjectId exist subowner list, else false.
 */
bool IsSubOwner(const OicUuid_t* uuid)
{
    bool retVal = false;

    if (NULL == uuid)
    {
        return retVal;
    }

    if (gDoxm && gDoxm->subOwners)
    {
        if (memcmp(gDoxm->owner.id, uuid->id, sizeof(gDoxm->owner.id)) == 0)
        {
            return false;
        }

        OicSecSubOwner_t* subOwner = NULL;
        LL_FOREACH(gDoxm->subOwners, subOwner)
        {
            if (memcmp(subOwner->uuid.id, uuid->id, sizeof(uuid->id)) == 0)
            {
                return true;
            }
        }
    }
    return retVal;
}
#endif //MULTIPLE_OWNER

OCStackResult SetMOTStatus(bool enable)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
#ifdef MULTIPLE_OWNER
    OCStackResult ret = OC_STACK_NO_MEMORY;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    bool isDeallocateRequired = false;

    if (NULL == gDoxm->mom && !enable)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return OC_STACK_OK;
    }

    if (NULL == gDoxm->mom)
    {
        gDoxm->mom = (OicSecMom_t*)OICCalloc(1, sizeof(OicSecMom_t));
        VERIFY_NON_NULL(TAG, gDoxm->mom, ERROR);
        isDeallocateRequired = true;
    }

    gDoxm->mom->mode = (enable ? OIC_MULTIPLE_OWNER_ENABLE : OIC_MULTIPLE_OWNER_DISABLE);
    gDoxm->oxmSel = OIC_PRECONFIG_PIN;

    ret = DoxmToCBORPayload(gDoxm, &cborPayload, &size, false);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

    ret = UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, cborPayload, size);
    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

    isDeallocateRequired = false;

exit:
    if (isDeallocateRequired)
    {
        OICFree(gDoxm->mom);
    }
    if (cborPayload)
    {
        OICFree(cborPayload);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s : %d", __func__, ret);
    return ret;
#else
    OIC_LOG(DEBUG, TAG, "Multiple Owner is not enabled.");
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_ERROR;
#endif //MULTIPLE_OWNER
}

OCStackResult RemoveSubOwner(const OicUuid_t* subOwner)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
#ifdef MULTIPLE_OWNER
    OCStackResult ret = OC_STACK_ERROR;
    bool isDeleted = false;

    if (NULL == subOwner)
    {
        OIC_LOG(ERROR, TAG, "Invalid sub owner UUID.");
        return OC_STACK_INVALID_PARAM;
    }
    if (NULL == gDoxm)
    {
        OIC_LOG(ERROR, TAG, "Doxm resource is NULL");
        return OC_STACK_NO_RESOURCE;
    }
    if ( NULL == gDoxm->subOwners)
    {
        OIC_LOG(WARNING, TAG, "Sub Owner list is empty.");
        return OC_STACK_ERROR;
    }

    OicSecSubOwner_t* curSubOwner = NULL;
    OicSecSubOwner_t* tempSubOwner = NULL;
    LL_FOREACH_SAFE(gDoxm->subOwners, curSubOwner, tempSubOwner)
    {
        if (memcmp(curSubOwner->uuid.id, subOwner->id, sizeof(subOwner->id)) == 0 ||
            memcmp(WILDCARD_SUBJECT_ID.id, subOwner->id, sizeof(OicUuid_t)) == 0)
        {
            char* strUuid = NULL;
            ret = ConvertUuidToStr(&curSubOwner->uuid, &strUuid);
            if (OC_STACK_OK != ret)
            {
                OIC_LOG_V(ERROR, TAG, "ConvertUuidToStr error : %d", ret);
                break;
            }

            OIC_LOG_V(INFO, TAG, "[%s] will be removed from sub owner list.", strUuid);
            LL_DELETE(gDoxm->subOwners, curSubOwner);

            //Remove the cred for sub owner
            ret = RemoveCredential(&curSubOwner->uuid);
            if (OC_STACK_RESOURCE_DELETED != ret)
            {
                OIC_LOG_V(WARNING, TAG, "RemoveCredential error for [%s] : %d", strUuid, ret);
                break;
            }

            // TODO: Remove the ACL for sub owner (Currently ACL is not required for sub-owner)

            OICFree(strUuid);

            isDeleted = true;
        }
    }

    if (isDeleted)
    {
        //Update persistent storage
        if (UpdatePersistentStorage(gDoxm))
        {
            ret = OC_STACK_RESOURCE_DELETED;
        }
        else
        {
            OIC_LOG(ERROR, TAG, "UpdatePersistentStorage error");
            ret = OC_STACK_ERROR;
        }
    }

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return ret;
#else
    OIC_LOG(DEBUG, TAG, "Multiple Owner is not enabled.");
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_ERROR;
#endif //MULTIPLE_OWNER

}

OCStackResult SetNumberOfSubOwner(size_t numOfSubOwner)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
#ifdef MULTIPLE_OWNER
    if (MAX_SUBOWNER_SIZE < numOfSubOwner || MIN_SUBOWNER_SIZE > numOfSubOwner)
    {
        OIC_LOG_V(ERROR, TAG, "Invalid number of sub owner : %d", numOfSubOwner);
        return OC_STACK_INVALID_PARAM;
    }

    gMaxSubOwnerSize = numOfSubOwner;
    OIC_LOG_V(DEBUG, TAG, "Number of SubOwner = %d", gMaxSubOwnerSize);

    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_OK;
#else
    OIC_LOG(DEBUG, TAG, "Multiple Owner is not enabled.");
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return OC_STACK_ERROR;
#endif //MULTIPLE_OWNER
}

/**
 * Function to restore doxm resurce to initial status.
 * This function will use in case of error while ownership transfer
 */
void RestoreDoxmToInitState()
{

    gConfirmState = CONFIRM_STATE_READY;
    gConfirmMsgId = 0;

    if(gDoxm)
    {
        OIC_LOG(INFO, TAG, "DOXM resource will revert back to initial status.");

        OicUuid_t emptyUuid = {.id={0}};
        memcpy(&(gDoxm->owner), &emptyUuid, sizeof(OicUuid_t));
        gDoxm->owned = false;
        gDoxm->oxmSel = OIC_JUST_WORKS;

        if(!UpdatePersistentStorage(gDoxm))
        {
            OIC_LOG(ERROR, TAG, "Failed to revert DOXM in persistent storage");
        }
    }
}

OCStackResult SetDoxmSelfOwnership(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;

    if(NULL == gDoxm)
    {
        ret = OC_STACK_NO_RESOURCE;
        return ret;
    }

    if( newROwner && (false == gDoxm->owned) )
    {
        gDoxm->owned = true;
        memcpy(gDoxm->owner.id, newROwner->id, sizeof(newROwner->id));
        memcpy(gDoxm->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = DoxmToCBORPayload(gDoxm, &cborPayload, &size, false);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    return ret;
}

