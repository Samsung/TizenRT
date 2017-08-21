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
#include "deviceonboardingstate.h"

#define TAG  "OIC_SRM_PSTAT"

/** Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching below max cbor size. */
static const uint16_t CBOR_SIZE = 512;

// Max cbor size payload.
static const uint16_t CBOR_MAX_SIZE = 4400;

// starting pstat map size before adding any Properties, +2 for rt and if.
static const uint8_t PSTAT_EMPTY_MAP_SIZE = 2; // rt, if are included, in addition to props

// .dos Property map size
static const uint8_t PSTAT_DOS_MAP_SIZE = 2; // s, p

static OicSecDpom_t gSm = SINGLE_SERVICE_CLIENT_DRIVEN;

static OicSecPstat_t gDefaultPstat =
{
    {DOS_RFOTM, false},                       // OicSecDostype_t dos
    false,                                    // bool isop
    TAKE_OWNER,                               // OicSecDpm_t cm
    NORMAL,                                   // OicSecDpm_t tm
    SINGLE_SERVICE_CLIENT_DRIVEN,             // OicSecDpom_t om */
    1,                                        // the number of elts in Sms
    &gSm,                                     // OicSecDpom_t *sm
    0,                                        // uint16_t commitHash
    {.id = {0}},                              // OicUuid_t rowneruuid
};

static OicSecPstat_t    *gPstat = NULL;

static OCResourceHandle gPstatHandle = NULL;

#define R PERMISSION_READ
#define W PERMISSION_WRITE
#define RW PERMISSION_READ | PERMISSION_WRITE

static const uint8_t gPstatPropertyAccessModes[PSTAT_PROPERTY_COUNT][DOS_STATE_COUNT] =
{ // RESET RFOTM  RFPRO   RFNOP   SRESET
    { R,    RW,     RW,     RW,     RW  }, // .dos
    { R,    R,      R,      R,      R   }, // .isop
    { R,    R,      R,      R,      R   }, // .cm
    { R,    RW,     RW,     RW,     RW  }, // .tm
    { R,    RW,     RW,     RW,     RW  }, // .om
    { R,    R,      R,      R,      R   }, // .sm
    { R,    RW,     R,      R,      RW  }  // .rowneruuid
};

#undef R
#undef W
#undef RW

static bool IsPropertyReadOnly(PstatProperty_t p,
                               OicSecDeviceOnboardingState_t s)
{
    bool ret = false;

    if (PERMISSION_READ == gPstatPropertyAccessModes[p][s])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: property %d is read-only in state %d.",
           __func__, p, s);
        ret = true;
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

/**
 * Internal method converts CBOR into PSTAT data, and determines if a read-only
 *  Property was parsed in the CBOR payload.
 *
 * @param cborPayload The pstat data in cbor format.
 * @param cborSize Size of the cborPayload. In case 0 is provided it assigns CBOR_SIZE (255) value.
 * @param pstat Pointer to @ref OicSecPstat_t.
 * @param roParsed Ptr to bool marked "true" iff a read-only Property is parsed
 * @param stateForReadOnlyCheck The state to use when determining if a Property is
 *                              read-only.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
*/
static OCStackResult CBORPayloadToPstatBin(const uint8_t *cborPayload,
                                           const size_t size,
                                           OicSecPstat_t **secPstat,
                                           bool *roParsed,
                                           OicSecDeviceOnboardingState_t stateForReadOnlyCheck);


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

/**
 * Function to update persistent storage
 */
static bool UpdatePersistentStorage(OicSecPstat_t *pstat)
{
    bool bRet = false;

    size_t size = 0;
    uint8_t *cborPayload = NULL;
    OCStackResult ret = PstatToCBORPayload(pstat, &cborPayload, &size);
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

OCStackResult PstatToCBORPayloadPartial(const OicSecPstat_t *pstat,
                                        uint8_t **payload,
                                        size_t *size,
                                        const bool *propertiesToInclude)
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
    size_t pstatMapSize = PSTAT_EMPTY_MAP_SIZE;
    CborEncoder encoder;
    CborEncoder pstatMap;
    char* strUuid = NULL;

    int64_t cborEncoderResult = CborNoError;

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Calculate map size
    for (int i = 0; i < PSTAT_PROPERTY_COUNT; i++)
    {
        if (propertiesToInclude[i])
        {
            pstatMapSize++;
        }
    }
    OIC_LOG_V(INFO, TAG, "%s: creating pstat CBOR payload with %d Properties.",
        __func__, pstatMapSize - PSTAT_EMPTY_MAP_SIZE);

    // Top Level Pstat Map
    cborEncoderResult = cbor_encoder_create_map(&encoder, &pstatMap, pstatMapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Pstat Map.");

    // dos Property
    if (propertiesToInclude[PSTAT_DOS])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including dos Property.", __func__);
        // Device Onboarding State Property tag
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_DOS_NAME,
            strlen(OIC_JSON_DOS_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding dos Name Tag.");

        // Device Onboarding State Property map
        CborEncoder dosMap;
        cborEncoderResult = cbor_encoder_create_map(&pstatMap, &dosMap, PSTAT_DOS_MAP_SIZE);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating pstat.dos map");

        cborEncoderResult = cbor_encode_text_string(&dosMap, OIC_JSON_S_NAME,
            strlen(OIC_JSON_S_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding pstat.dos.s tag.");

        cborEncoderResult = cbor_encode_int(&dosMap, pstat->dos.state);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding pstat.dos.s value.");

        cborEncoderResult = cbor_encode_text_string(&dosMap, OIC_JSON_P_NAME,
            strlen(OIC_JSON_P_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding pstat.dos.p tag.");

        cborEncoderResult = cbor_encode_boolean(&dosMap, pstat->dos.pending);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding pstat.dos.p value.");

        cborEncoderResult = cbor_encoder_close_container(&pstatMap, &dosMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing pstat.dos map");
    }

    // isop Property
    if (propertiesToInclude[PSTAT_ISOP])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including isop Property.", __func__);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_ISOP_NAME,
            strlen(OIC_JSON_ISOP_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ISOP Name Tag.");
        cborEncoderResult = cbor_encode_boolean(&pstatMap, pstat->isOp);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ISOP Name Value.");
    }

    // cm Property
    if (propertiesToInclude[PSTAT_CM])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including cm Property.", __func__);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_CM_NAME,
            strlen(OIC_JSON_CM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CM Name Tag.");
        cborEncoderResult = cbor_encode_int(&pstatMap, pstat->cm);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CM Name Value.");
    }

    // tm Property
    if (propertiesToInclude[PSTAT_TM])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including tm Property.", __func__);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_TM_NAME,
            strlen(OIC_JSON_TM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding TM Name Tag.");
        cborEncoderResult = cbor_encode_int(&pstatMap, pstat->tm);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding TM Name Value.");
    }

    // om Property
    if (propertiesToInclude[PSTAT_OM])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including om Property.", __func__);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_OM_NAME,
            strlen(OIC_JSON_OM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding OM Name Tag.");
        cborEncoderResult = cbor_encode_int(&pstatMap, pstat->om);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding OM Name Value.");
    }

    // sm Property
    if (propertiesToInclude[PSTAT_SM])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including sm Property.", __func__);
        cborEncoderResult = cbor_encode_text_string(&pstatMap, OIC_JSON_SM_NAME,
            strlen(OIC_JSON_SM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SM Name Tag.");
        cborEncoderResult = cbor_encode_int(&pstatMap, pstat->sm[0]);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding SM Name Value.");
    }

    // rowneruuid property
    if (propertiesToInclude[PSTAT_ROWNERUUID])
    {
        OIC_LOG_V(DEBUG, TAG, "%s: including rowneruuid Property.", __func__);
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
        ret = PstatToCBORPayloadPartial(pstat, payload, &cborLen, propertiesToInclude);
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

OCStackResult PstatToCBORPayload(const OicSecPstat_t *pstat,
                                 uint8_t **payload, size_t *size)
{
    bool allProps[PSTAT_PROPERTY_COUNT];

    for (int i = 0; i < PSTAT_PROPERTY_COUNT; i++)
    {
        allProps[i] = true;
    }

    return PstatToCBORPayloadPartial(pstat, payload, size, allProps);
}

OCStackResult CBORPayloadToPstat(const uint8_t *cborPayload, const size_t size,
                                 OicSecPstat_t **secPstat)
{
    return CBORPayloadToPstatBin(cborPayload, size, secPstat, NULL, DOS_RESET);
}

static OCStackResult CBORPayloadToPstatBin(const uint8_t *cborPayload,
                                           const size_t size,
                                           OicSecPstat_t **secPstat,
                                           bool *roParsed,
                                           OicSecDeviceOnboardingState_t stateForReadOnlyCheck)
{
    if (NULL == cborPayload || NULL == secPstat || NULL != *secPstat || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    CborValue pstatCbor = { .parser = NULL };
    CborValue pstatMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    CborValue dosMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    char *strUuid = NULL;
    char *dosTagName = NULL;
    size_t len = 0;
    size_t dosLen = 0;
    OicSecPstat_t *pstat = NULL;

    *secPstat = NULL;

    // init cbor parser
    cbor_parser_init(cborPayload, size, 0, &parser, &pstatCbor);

    // allocate pstat struct
    pstat = (OicSecPstat_t *)OICCalloc(1, sizeof(OicSecPstat_t));
    VERIFY_NOT_NULL(TAG, pstat, ERROR);

    // Individual Properties missing from cbor representation may result in
    // pstat Property assignment to existing gPstat values.  Therefore,
    // we must ensure that gPstat to a valid value, to avoid null deref.
    if (!gPstat)
    {
        gPstat = GetPstatDefault();
    }
    VERIFY_NOT_NULL(TAG, gPstat, FATAL);

    // Enter pstat Map
    cborFindResult = cbor_value_enter_container(&pstatCbor, &pstatMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering pstat Map.");

    // Find pstat.dos tag
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_DOS_NAME, &pstatMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding dos tag.");

    if (CborInvalidType != pstatMap.type)
    {
        // found pstat.dos tag "dos" in pstatMap
        OIC_LOG(DEBUG, TAG, "Found pstat.dos tag in pstatMap.");
        if (CborNoError == cborFindResult && cbor_value_is_container(&pstatMap))
        {
            OIC_LOG(DEBUG, TAG, "Found pstat.dos cbor container; entering.");
            cborFindResult = cbor_value_enter_container(&pstatMap, &dosMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering dos map.");
            if (roParsed)
            {
                if (IsPropertyReadOnly(PSTAT_DOS, stateForReadOnlyCheck))
                {
                    *roParsed = true;
                }
            }
        }
        while (cbor_value_is_valid(&dosMap) && cbor_value_is_text_string(&dosMap))
        {
            cborFindResult = cbor_value_dup_text_string(&dosMap, &dosTagName, &dosLen, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting dos map next tag.");
            cborFindResult = cbor_value_advance(&dosMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing dos map.");

            if (NULL != dosTagName)
            {
                if (strcmp(dosTagName, OIC_JSON_S_NAME) == 0)
                {
                    OIC_LOG(DEBUG, TAG, "Found pstat.dos.s tag; getting int value.");
                    int s = -1;
                    cborFindResult = cbor_value_get_int(&dosMap, &s);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting pstat.dos.s value.");
                    OIC_LOG_V(DEBUG, TAG, "Read pstat.dos.s value = %d.", s);
                    pstat->dos.state = (OicSecDeviceOnboardingState_t)s;
                }
                else if (strcmp(dosTagName, OIC_JSON_P_NAME) == 0)
                {
                    OIC_LOG(DEBUG, TAG, "Found pstat.dos.p tag; getting boolean value.");
                    bool p = false;
                    cborFindResult = cbor_value_get_boolean(&dosMap, &p);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting pstat.dos.p value.");
                    OIC_LOG_V(DEBUG, TAG, "Read pstat.dos.p value = %s.", p?"true":"false");
                    pstat->dos.pending = p;
                }
                else
                {
                    OIC_LOG_V(WARNING, TAG, "Unknown tag name in dos map: %s", dosTagName);
                }
                free(dosTagName);
                dosTagName = NULL;
            }

            if (cbor_value_is_valid(&dosMap))
            {
                cborFindResult = cbor_value_advance(&dosMap);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing dos map.");
            }
        }
    }
    else
    {
        // didn't find pstat.dos tag "dos" in pstatMap
        OIC_LOG(WARNING, TAG, "Did not find mandatory pstat.dos tag in pstatMap.");
        OIC_LOG(WARNING, TAG, "If this is not an intentionally-partial pstat representation,");
        OIC_LOG(WARNING, TAG, "it may be an outdated .dat file that is missing the \"dos\" Property.");
        OIC_LOG(WARNING, TAG, "Using existing pstat.dos value from gPstat.");
        pstat->dos.state = gPstat->dos.state;
        pstat->dos.pending = gPstat->dos.pending;
        cborFindResult = CborNoError;
    }

    // pstat.isop Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_ISOP_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_boolean(&pstatMap))
    {
        cborFindResult = cbor_value_get_boolean(&pstatMap, &pstat->isOp);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding isOp Value.");

        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_ISOP, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }
    }
    else
    {
        pstat->isOp = gPstat->isOp;
        cborFindResult = CborNoError;
    }

    // pstat.cm Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_CM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int cm;

        cborFindResult = cbor_value_get_int(&pstatMap, &cm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CM.");
        pstat->cm = (OicSecDpm_t)cm;

        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_CM, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }
    }
    else
    {
        pstat->cm = gPstat->cm;
        cborFindResult = CborNoError;
    }

    // pstat.tm Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_TM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int tm;

        cborFindResult = cbor_value_get_int(&pstatMap, &tm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding TM.");
        pstat->tm = (OicSecDpm_t)tm;

        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_TM, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }

    }
    else
    {
        pstat->tm = gPstat->tm;
        cborFindResult = CborNoError;
    }

    // pstat.om Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_OM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int om;

        cborFindResult = cbor_value_get_int(&pstatMap, &om);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding OM.");
        pstat->om = (OicSecDpom_t)om;

        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_OM, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }
    }
    else
    {
        pstat->om = gPstat->om;
        cborFindResult = CborNoError;
    }

    // pstat.sm Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_SM_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_integer(&pstatMap))
    {
        int sm;

        pstat->smLen = 1;
        pstat->sm = (OicSecDpom_t*)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
        VERIFY_NOT_NULL(TAG, pstat->sm, ERROR);
        cborFindResult = cbor_value_get_int(&pstatMap, &sm);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding SM.");
        pstat->sm[0] = (OicSecDpom_t)sm;

        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_SM, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }
    }
    else
    {
        VERIFY_NOT_NULL(TAG, gPstat, ERROR);
        pstat->smLen = gPstat->smLen;
        pstat->sm = (OicSecDpom_t*)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
        VERIFY_NOT_NULL(TAG, pstat->sm, ERROR);
        *pstat->sm = *gPstat->sm;
        cborFindResult = CborNoError;
    }

    // pstat.rowneruuid Property
    cborFindResult = cbor_value_map_find_value(&pstatCbor, OIC_JSON_ROWNERID_NAME, &pstatMap);
    if (CborNoError == cborFindResult && cbor_value_is_text_string(&pstatMap))
    {
        cborFindResult = cbor_value_dup_text_string(&pstatMap, &strUuid , &len, NULL);
        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ROwner Id Value.");
        ret = ConvertStrToUuid(strUuid , &pstat->rownerID);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        OICFree(strUuid );
        strUuid  = NULL;
        if (roParsed)
        {
            if (IsPropertyReadOnly(PSTAT_ROWNERUUID, stateForReadOnlyCheck))
            {
                *roParsed = true;
            }
        }
    }
    else
    {
        VERIFY_NOT_NULL(TAG, gPstat, ERROR);
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
     * For GET or Valid Query request return pstat resource CBOR payload.
     * For non-valid query return NULL payload.
     * A device will 'always' have a default Pstat, so PstatToCBORPayload will
     * return valid pstat resource json.
     */
    size_t size = 0;
    uint8_t *payload = NULL;
    if (ehRet == OC_EH_OK)
    {
        if(OC_STACK_OK != PstatToCBORPayload(gPstat, &payload, &size))
        {
            OIC_LOG_V(WARNING, TAG, "%s PstatToCBORPayload failed.", __func__);
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
 * For pstat, it updates only dos, tm, om, and rowneruuid.
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
        VERIFY_NOT_NULL(TAG, payload, ERROR);

        bool roParsed = false;
        OCStackResult ret = CBORPayloadToPstatBin(payload, size, &pstat,
                                                  &roParsed, gPstat->dos.state);
        VERIFY_NOT_NULL(TAG, pstat, ERROR);

        // if CBOR parsing OK
        if (OC_STACK_OK == ret)
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

            if (true == roParsed)
            {
                    OIC_LOG(ERROR, TAG, "Not acceptable request because of read-only properties");
                    ehRet = OC_EH_NOT_ACCEPTABLE;
                    goto exit;
            }

            // operation mode (om) should be one of supported modes (sm)
            bool supportedOm = false;
            for(size_t i = 0; i < gPstat->smLen; i++)
            {
                if(gPstat->sm[i] == pstat->om)
                {
                    OIC_LOG_V(ERROR, TAG, "%s: %d is a supported Operation Mode",
                        __func__, (int) pstat->om);
                    supportedOm = true;
                }
            }
            if (!supportedOm)
            {
                OIC_LOG_V(ERROR, TAG, "%s: %d is NOT a supported Operation Mode",
                    __func__, (int) pstat->om);
                ehRet = OC_EH_BAD_REQ;
                goto exit;
            }

            if (!(gPstat->tm & VERIFY_SOFTWARE_VERSION)
                && (pstat->tm & VERIFY_SOFTWARE_VERSION)) { // ISVV bit goes from 0 to 1
                OIC_LOG (INFO, TAG, "Software Version Validation process initiated");
                pstat->cm &= ~VERIFY_SOFTWARE_VERSION; // Unset the cm bit, per spec
            }

            if (!(gPstat->tm & UPDATE_SOFTWARE)
                && (pstat->tm & UPDATE_SOFTWARE)) { // ISSU bit goes from 0 to 1
                OIC_LOG (INFO, TAG, "Software Update process initiated");
                pstat->cm &= ~UPDATE_SOFTWARE; // Unset the cm bit, per spec
            }

            if (pstat->dos.state != gPstat->dos.state)
            {
                OCStackResult stateChangeResult = OC_STACK_ERROR;
                stateChangeResult = SetDosState(pstat->dos.state);
                switch (stateChangeResult)
                {
                    case OC_STACK_OK:
                    OIC_LOG_V(INFO, TAG, "%s: DOS state changed SUCCESSFULLY to %d.", \
                        __func__, pstat->dos.state);
                    ehRet = OC_EH_OK;
                    break;

                    case OC_STACK_FORBIDDEN_REQ:
                    OIC_LOG_V(WARNING, TAG, "%s: DOS state change change to %d NOT ALLOWED.", \
                        __func__, pstat->dos.state);
                    ehRet = OC_EH_NOT_ACCEPTABLE;
                    goto exit;
                    break;

                    case OC_STACK_INTERNAL_SERVER_ERROR:
                    default:
                    OIC_LOG_V(ERROR, TAG, "%s: DOS state change change to %d FAILED. \
                        Internal error - SVRs may be in bad state.", \
                        __func__, pstat->dos.state);
                    ehRet = OC_EH_INTERNAL_SERVER_ERROR;
                    goto exit;
                    break;
                }
            }

            // update om
            gPstat->om = pstat->om;

            // update rownerID
            gPstat->rownerID = pstat->rownerID;

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
                    OIC_LOG_V(ERROR, TAG, "%s: SendSRMResponse failed.", __func__);
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

    // TODO [IOT-1796] This is another place error code returns need to be
    // cleaned up.
    if (OC_EH_OK != ehRet)
    {
        /*
         * If some error is occured while ownership transfer,
         * ownership transfer related resource should be revert back to initial status.
         */
        OIC_LOG(WARNING, TAG, "The operation failed during handle pstat POST request");
        const OicSecDoxm_t* doxm = GetDoxmResourceData();
        if (doxm)
        {
            if (!doxm->owned)
            {
                if (!isDuplicatedMsg)
                {
                    OIC_LOG(WARNING, TAG, "DOXM and PSTAT will be reverted.");
                    RestoreDoxmToInitState();
                    RestorePstatToInitState();
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
        if (ehRequest->devAddr.adapter == OC_ADAPTER_IP)
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

OCStackResult InitPstatResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    // Read Pstat resource from PS
    uint8_t *data = NULL;
    size_t size = 0;
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
    VERIFY_NOT_NULL(TAG, gPstat, FATAL);

    // TODO [IOT-2023]: after all SVRs are initialized, need to call SetDosState()
    // using the just-loaded pstat.dos.s

    // Instantiate 'oic.sec.pstat'
    ret = CreatePstatResource();

exit:
    if (OC_STACK_OK != ret)
    {
        DeInitPstatResource();
    }
    return ret;
}

OCStackResult InitPstatResourceToDefault()
{
    gPstat = GetPstatDefault();
    VERIFY_NOT_NULL(TAG, gPstat, FATAL);

    return OC_STACK_OK;
exit:
    return OC_STACK_ERROR;
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
        gPstat->dos.state = DOS_RFOTM;
        gPstat->dos.pending = false;
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

OCStackResult GetPstatRownerId(OicUuid_t *rowneruuid)
{
    if (gPstat && rowneruuid)
    {
        memcpy(&(rowneruuid->id), &(gPstat->rownerID.id), sizeof(rowneruuid->id));
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatRownerId(const OicUuid_t *rowneruuid)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == rowneruuid)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gPstat)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(rowneruuid && gPstat)
    {
        memcpy(prevId.id, gPstat->rownerID.id, sizeof(prevId.id));
        memcpy(gPstat->rownerID.id, rowneruuid->id, sizeof(gPstat->rownerID.id));

        ret = PstatToCBORPayload(gPstat, &cborPayload, &size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gPstat->rownerID.id, prevId.id, sizeof(gPstat->rownerID.id));
    return ret;
}

OCStackResult GetPstatDosS(OicSecDeviceOnboardingState_t *s)
{
    if (gPstat && s)
    {
        *s = gPstat->dos.state;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatDosS(const OicSecDeviceOnboardingState_t s)
{
    if (gPstat)
    {
        gPstat->dos.state = s;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult GetPstatDosP(bool *p)
{
    if (gPstat && p)
    {
        *p = gPstat->dos.pending;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatDosP(const bool p)
{
    if (gPstat)
    {
        gPstat->dos.pending = p;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult GetPstatIsop(bool *isop)
{
    if (gPstat && isop)
    {
        *isop = gPstat->isOp;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatIsop(const bool isop)
{
    if (gPstat)
    {
        gPstat->isOp = isop;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult GetPstatCm(OicSecDpm_t *cm)
{
    if (gPstat && cm)
    {
        *cm = gPstat->cm;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatCm(const OicSecDpm_t cm)
{
    if (gPstat)
    {
        gPstat->cm = cm;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult GetPstatTm(OicSecDpm_t *tm)
{
    if (gPstat && tm)
    {
        *tm = gPstat->tm;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}

OCStackResult SetPstatTm(const OicSecDpm_t tm)
{
    if (gPstat)
    {
        gPstat->tm = tm;
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
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

    if( newROwner && (false == gPstat->isOp) && (TAKE_OWNER & gPstat->cm) )
    {
        gPstat->cm = (OicSecDpm_t)(gPstat->cm & (~TAKE_OWNER));
        gPstat->isOp = true;

        memcpy(gPstat->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = PstatToCBORPayload(gPstat, &cborPayload, &size);
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

