/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include "iotivity_config.h"
#include <stdlib.h>
#include <string.h>
#include "ocstack.h"
#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "base64.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"
#include "resourcemanager.h"
#include "pconfresource.h"
#include "psinterface.h"
#include "utlist.h"
#include "srmresourcestrings.h"
#include "doxmresource.h"
#include "srmutility.h"
#include "ocserverrequest.h"
#include "psinterface.h"
#include "security_internals.h"
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#define TAG  "OIC_SRM_PCONF"

static const uint16_t CBOR_SIZE = 1024;
static const uint64_t CBOR_MAX_SIZE = 4400;
static const uint8_t  PCONF_MAP_SIZE = 4;
static const uint8_t  PCONF_RESOURCE_MAP_SIZE = 4;

static OicSecPconf_t          *gPconf = NULL;
static OCResourceHandle   gPconfHandle = NULL;
static OicSecPconf_t         gDefaultPconf =
{
    false,                  /* bool edp */
    NULL,                  /* OicSecPrm *prm */
    0,                       /* size_t prmLen */
    {.val = {0}},       /* OicDpPin_t pin */
    NULL,                  /* OicSecPdAcl_t *pdacls */
    NULL,                  /* OicUuid_t *pddevs */
    0,                       /* size_t pddevLen */
    {.id = {0}},        /* OicUuid_t deviceID */
    {.id = {0}},        /* OicUuid_t rowner */
};

/**
 * This function frees OicSecPdAcl_t object's fields and object itself.
 */
void FreePdAclList(OicSecPdAcl_t* pdacls)
{
    if (pdacls)
    {
        size_t i = 0;

        //Clean pdacl objecs
        OicSecPdAcl_t *aclTmp1 = NULL;
        OicSecPdAcl_t *aclTmp2 = NULL;
        LL_FOREACH_SAFE(pdacls, aclTmp1, aclTmp2)
        {
            LL_DELETE(pdacls, aclTmp1);

            // Clean Resources
            for (i = 0; i < aclTmp1->resourcesLen; i++)
            {
                OICFree(aclTmp1->resources[i]);
            }
            OICFree(aclTmp1->resources);

            //Clean Period
            if(aclTmp1->periods)
            {
                for(i = 0; i < aclTmp1->prdRecrLen; i++)
                {
                    OICFree(aclTmp1->periods[i]);
                }
                OICFree(aclTmp1->periods);
            }

            //Clean Recurrence
            if(aclTmp1->recurrences)
            {
                for(i = 0; i < aclTmp1->prdRecrLen; i++)
                {
                    OICFree(aclTmp1->recurrences[i]);
                }
                OICFree(aclTmp1->recurrences);
            }
        }

        //Clean pconf itself
        OICFree(pdacls);
    }
}

void DeletePconfBinData(OicSecPconf_t* pconf)
{
    if (pconf)
    {
        //Clean prm
        OICFree(pconf->prm);

        //Clean pdacl
        if (pconf->pdacls)
        {
            FreePdAclList(pconf->pdacls);
        }

        //Clean pddev
        OICFree(pconf->pddevs);

        //Clean pconf itself
        OICFree(pconf);
    }
}

static size_t OicPdAclSize(const OicSecPdAcl_t *pdAcl)
{
    if (!pdAcl)
    {
        return 0;
    }

    OicSecPdAcl_t *tmp = (OicSecPdAcl_t *)pdAcl;
    size_t size = 0;
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }
    return size;
}

OCStackResult PconfToCBORPayload(const OicSecPconf_t *pconf,uint8_t **payload,size_t *size)
{
    if (NULL == pconf || NULL == payload || NULL != *payload || NULL == size)
    {
        return OC_STACK_INVALID_PARAM;
    }
    size_t cborLen = *size;
    if(0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }
    *payload = NULL;

    OCStackResult ret = OC_STACK_ERROR;
    CborEncoder encoder;
    CborEncoder pconfMap;

    int64_t cborEncoderResult = CborNoError;
    uint8_t mapSize = PCONF_MAP_SIZE;

    if (pconf->prmLen > 0)
    {
        mapSize++;
    }
    if (pconf->pdacls)
    {
        mapSize++;
    }
    if (pconf->pddevs)
    {
        mapSize++;
    }

    uint8_t *outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);
    cborEncoderResult = cbor_encoder_create_map(&encoder, &pconfMap, mapSize);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating Pconf Map.");

    //edp  -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_EDP_NAME,
            strlen(OIC_JSON_EDP_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Encode EDP String.");
    cborEncoderResult = cbor_encode_boolean(&pconfMap, pconf->edp);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Convert PconfEDP value");

    //PRM type -- Not Mandatory
    if(pconf->prmLen > 0)
    {
        CborEncoder prm;
        cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_PRM_NAME,
                strlen(OIC_JSON_PRM_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Convert Pconf PRM NAME");
        cborEncoderResult = cbor_encoder_create_array(&pconfMap, &prm, pconf->prmLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Convert Pconf PRM value");

        for (size_t i = 0; i < pconf->prmLen; i++)
        {
            cborEncoderResult = cbor_encode_int(&prm, pconf->prm[i]);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Convert Pconf PRM Array");
        }
        cborEncoderResult = cbor_encoder_close_container(&pconfMap, &prm);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close encode array");
    }

    //PIN -- Mandatory
    cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_PIN_NAME,
            strlen(OIC_JSON_PIN_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create OIC_JSON_PIN_NAME");
    cborEncoderResult = cbor_encode_byte_string(&pconfMap, pconf->pin.val, sizeof(pconf->pin.val));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to convert pin value");

    //PDACL -- Mandatory
    if (pconf->pdacls)
    {
        OicSecPdAcl_t *pdacl = pconf->pdacls;
        CborEncoder pdAclArray;
        cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_PDACL_NAME,
                strlen(OIC_JSON_PDACL_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create OIC_JSON_PDACL_NAME");
        cborEncoderResult = cbor_encoder_create_array(&pconfMap, &pdAclArray,
                OicPdAclSize(pconf->pdacls));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to creeate _pdacl array");

        while(pdacl)
        {
            CborEncoder pdAclMap;
            // PDACL Map size - Number of mandatory items
            uint8_t aclMapSize = 2;

            if (pdacl->prdRecrLen)
            {
                ++aclMapSize;
            }
            if (pdacl->recurrences)
            {
                ++aclMapSize;
            }

            cborEncoderResult = cbor_encoder_create_map(&pdAclArray, &pdAclMap, aclMapSize);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,  "Failed to creeate _pdacl array");

            // Resources -- Mandatory
            cborEncoderResult = cbor_encode_text_string(&pdAclMap, OIC_JSON_RESOURCES_NAME,
                    strlen(OIC_JSON_RESOURCES_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,  "Failed to encode resource result");

            CborEncoder resources;
            cborEncoderResult = cbor_encoder_create_array(&pdAclMap, &resources,
                    pdacl->resourcesLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,  "Failed to create resource array");

            for (size_t i = 0; i < pdacl->resourcesLen; i++)
            {
                CborEncoder rMap;
                cborEncoderResult = cbor_encoder_create_map(&resources, &rMap,
                        PCONF_RESOURCE_MAP_SIZE);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Resource Map.");

                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_HREF_NAME,
                        strlen(OIC_JSON_HREF_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Name Tag.");
                cborEncoderResult = cbor_encode_text_string(&rMap, pdacl->resources[i],
                        strlen(pdacl->resources[i]));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Value in Map.");

                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_REL_NAME,
                        strlen(OIC_JSON_REL_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Name Tag.");

                // TODO : Need to assign real value of REL
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                        strlen(OIC_JSON_EMPTY_STRING));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Value.");

                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_RT_NAME,
                        strlen(OIC_JSON_RT_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");

                // TODO : Need to assign real value of RT
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                        strlen(OIC_JSON_EMPTY_STRING));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");

                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_IF_NAME,
                        strlen(OIC_JSON_IF_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");

                // TODO : Need to assign real value of IF
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_EMPTY_STRING,
                        strlen(OIC_JSON_EMPTY_STRING));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");

                cborEncoderResult = cbor_encoder_close_container(&resources, &rMap);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Map.");
            }

            cborEncoderResult = cbor_encoder_close_container(&pdAclMap, &resources);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,  "Failed to close resource array");

            // Permissions -- Mandatory
            cborEncoderResult = cbor_encode_text_string(&pdAclMap, OIC_JSON_PERMISSION_NAME,
                    strlen(OIC_JSON_PERMISSION_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,  "Failed to create permition string");
            cborEncoderResult = cbor_encode_int(&pdAclMap, pdacl->permission);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode permition calue");

            // Period -- Not Mandatory
            if (pdacl->periods)
            {
                CborEncoder period;
                cborEncoderResult = cbor_encode_text_string(&pdAclMap, OIC_JSON_PERIODS_NAME,
                        strlen(OIC_JSON_PERIODS_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode period value");
                cborEncoderResult = cbor_encoder_create_array(&pdAclMap, &period,
                        pdacl->prdRecrLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create array");

                for (size_t i = 0; i < pdacl->prdRecrLen; i++)
                {
                    cborEncoderResult = cbor_encode_text_string(&period, pdacl->periods[i],
                            strlen(pdacl->periods[i]));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode period");
                }
                cborEncoderResult = cbor_encoder_close_container(&pdAclMap, &period);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,"Failed to close period array");
            }

            // Period -- Not Mandatory
            if(0 != pdacl->prdRecrLen && pdacl->recurrences)
            {
                CborEncoder recurrences;
                cborEncoderResult = cbor_encode_text_string(&pdAclMap, OIC_JSON_RECURRENCES_NAME,
                        strlen(OIC_JSON_RECURRENCES_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult,"Failed to encode recurrences");
                cborEncoderResult = cbor_encoder_create_array(&pdAclMap, &recurrences,
                        pdacl->prdRecrLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create rec array");

                for (size_t i = 0; i < pdacl->prdRecrLen; i++)
                {
                    cborEncoderResult = cbor_encode_text_string(&recurrences,
                            pdacl->recurrences[i], strlen(pdacl->recurrences[i]));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode recurrences");
                }
                cborEncoderResult = cbor_encoder_close_container(&pdAclMap, &recurrences);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close rec array");
            }
            cborEncoderResult = cbor_encoder_close_container(&pdAclArray, &pdAclMap);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close acl map");

            pdacl = pdacl->next;
        }
        //clsoe the array
        cborEncoderResult = cbor_encoder_close_container(&pconfMap, &pdAclArray);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close acl array");
    }

    //PDDev -- Mandatory
    //There may not be paired devices if it did not pairing before
    if (pconf->pddevs && 0 < pconf->pddevLen)
    {
        CborEncoder pddev;
        cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_PDDEV_LIST_NAME,
                strlen(OIC_JSON_PDDEV_LIST_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode pddev");
        cborEncoderResult = cbor_encoder_create_array(&pconfMap, &pddev,  pconf->pddevLen);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to create array");

        for (size_t i = 0; i < pconf->pddevLen; i++)
        {
            char *pddevId = NULL;
            ret = ConvertUuidToStr(&pconf->pddevs[i], &pddevId);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&pddev, pddevId, strlen(pddevId));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding pddev Id Value.");
            OICFree(pddevId);
        }
        cborEncoderResult = cbor_encoder_close_container(&pconfMap, &pddev);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close pddev array");
    }

    //DeviceId -- Mandatory
    //There may not be devicd id if caller is provisoning tool
    cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_DEVICE_ID_NAME,
            strlen(OIC_JSON_DEVICE_ID_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode device id");
    {
        char *deviceId = NULL;
        ret = ConvertUuidToStr(&pconf->deviceID, &deviceId);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&pconfMap, deviceId, strlen(deviceId));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode deviceID value");
        OICFree(deviceId);
    }

    //ROwner -- Mandatory
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&pconfMap, OIC_JSON_ROWNERID_NAME,
                strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode rowner string");
        ret = ConvertUuidToStr(&pconf->rownerID, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&pconfMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to encode rwoner value");
        OICFree(rowner);
    }

    cborEncoderResult = cbor_encoder_close_container(&encoder, &pconfMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to close pconfMap");

    *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
    *payload = outPayload;
    ret = OC_STACK_OK;
exit:
    if ((CborErrorOutOfMemory == cborEncoderResult) && (cborLen < CBOR_MAX_SIZE))
    {
        // reallocate and try again!
        OICFree(outPayload);
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        cborEncoderResult = CborNoError;
        ret = PconfToCBORPayload(pconf, payload, &cborLen);
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

OCStackResult CBORPayloadToPconf(const uint8_t *cborPayload, size_t size, OicSecPconf_t **secPconf)
{
    if (NULL == cborPayload || NULL == secPconf || NULL != *secPconf || 0 == size)
    {
        return OC_STACK_INVALID_PARAM;
    }
    OCStackResult ret = OC_STACK_ERROR;
    *secPconf = NULL;
    CborValue pconfCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &pconfCbor);
    CborValue pconfMap = { .parser = NULL } ;
    OicSecPconf_t *pconf = NULL;
    cborFindResult = cbor_value_enter_container(&pconfCbor, &pconfMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter map");
    pconf = (OicSecPconf_t *)OICCalloc(1, sizeof(*pconf));
    VERIFY_NON_NULL(TAG, pconf, ERROR);
    while (cbor_value_is_valid(&pconfMap))
    {
        char *name = NULL;
        size_t len = 0;
        CborType type = cbor_value_get_type(&pconfMap);
        if (type == CborTextStringType && cbor_value_is_text_string(&pconfMap))
        {
            cborFindResult = cbor_value_dup_text_string(&pconfMap, &name, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
            cborFindResult = cbor_value_advance(&pconfMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
        }

        if (name)
        {
            //EDP -- Mandatory
            if(0 == strcmp(OIC_JSON_EDP_NAME, name) && cbor_value_is_boolean(&pconfMap))
            {
                cborFindResult = cbor_value_get_boolean(&pconfMap, &pconf->edp);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
            }
            if (0 == strcmp(OIC_JSON_PRM_NAME, name))
            {
                int i = 0;
                CborValue prm = { .parser = NULL };
                cborFindResult = cbor_value_get_array_length(&pconfMap, &pconf->prmLen);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get length");
                VERIFY_SUCCESS(TAG, pconf->prmLen != 0, ERROR);

                pconf->prm = (OicSecPrm_t *)OICCalloc(pconf->prmLen, sizeof(OicSecPrm_t));
                VERIFY_NON_NULL(TAG, pconf->prm, ERROR);
                cborFindResult = cbor_value_enter_container(&pconfMap, &prm);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to eneter array");

                while (cbor_value_is_valid(&prm) && cbor_value_is_integer(&prm))
                {
                    int prm_val;

                    cborFindResult = cbor_value_get_int(&prm, &prm_val);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
                    pconf->prm[i++] = (OicSecPrm_t)prm_val;
                    cborFindResult = cbor_value_advance(&prm);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
                }
            }
            //PIN -- Mandatory
            if (0 == strcmp(OIC_JSON_PIN_NAME, name) && cbor_value_is_byte_string(&pconfMap))
            {
                uint8_t *pin = NULL;
                cborFindResult = cbor_value_dup_byte_string(&pconfMap, &pin, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
                memcpy(pconf->pin.val, pin, len);
                OICFree(pin);
            }

            //PDACL -- Mandatory
            if (0 == strcmp(OIC_JSON_PDACL_NAME, name))
            {
                CborValue pdAclArray = { .parser = NULL};
                OicSecPdAcl_t *headPdacl = NULL;

                cborFindResult = cbor_value_enter_container(&pconfMap, &pdAclArray);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");

                while (cbor_value_is_valid(&pdAclArray))
                {
                    CborValue pdAclMap = { .parser = NULL};
                    OicSecPdAcl_t *pdacl = (OicSecPdAcl_t *) OICCalloc(1, sizeof(OicSecPdAcl_t));
                    VERIFY_NON_NULL(TAG, pdacl, ERROR);

                    cborFindResult = cbor_value_enter_container(&pdAclArray, &pdAclMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");

                    while (cbor_value_is_valid(&pdAclMap))
                    {
                        char* name = NULL;
                        size_t len = 0;
                        CborType type = cbor_value_get_type(&pdAclMap);
                        if (type == CborTextStringType && cbor_value_is_text_string(&pdAclMap))
                        {
                            cborFindResult = cbor_value_dup_text_string(&pdAclMap, &name,
                                    &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
                            cborFindResult = cbor_value_advance(&pdAclMap);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance value");
                        }
                        if (name)
                        {
                            // Resources -- Mandatory
                            if (strcmp(name, OIC_JSON_RESOURCES_NAME) == 0 && cbor_value_is_array(&pdAclMap))
                            {
                                int i = 0;
                                CborValue resources = { .parser = NULL };
                                cborFindResult = cbor_value_get_array_length(&pdAclMap,
                                        &pdacl->resourcesLen);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get length");
                                cborFindResult = cbor_value_enter_container(&pdAclMap, &resources);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");
                                pdacl->resources = (char **) OICCalloc(pdacl->resourcesLen,
                                        sizeof(char*));
                                VERIFY_NON_NULL(TAG, pdacl->resources, ERROR);

                                while (cbor_value_is_valid(&resources))
                                {
                                    CborValue rMap = { .parser = NULL  };
                                    cborFindResult = cbor_value_enter_container(&resources, &rMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering Resource Map");

                                    while(cbor_value_is_valid(&rMap) && cbor_value_is_text_string(&rMap))
                                    {
                                        char *rMapName = NULL;
                                        size_t rMapNameLen = 0;
                                        cborFindResult = cbor_value_dup_text_string(&rMap, &rMapName, &rMapNameLen, NULL);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RMap Data Name Tag.");
                                        cborFindResult = cbor_value_advance(&rMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RMap Data Value.");

                                        // "href"
                                        if (0 == strcmp(OIC_JSON_HREF_NAME, rMapName))
                                        {
                                            // TODO : Need to check data structure of OicSecPdAcl_t based on RAML spec.
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &pdacl->resources[i++], &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                                        }

                                        // "rel"
                                        if (0 == strcmp(OIC_JSON_REL_NAME, rMapName))
                                        {
                                            // TODO : Need to check data structure of OicSecPdAcl_t and assign based on RAML spec.
                                            char *relData = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &relData, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding REL Value.");
                                            OICFree(relData);
                                        }

                                        // "rt"
                                        if (0 == strcmp(OIC_JSON_RT_NAME, rMapName))
                                        {
                                            // TODO : Need to check data structure of OicSecPdAcl_t and assign based on RAML spec.
                                            char *rtData = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rtData, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT Value.");
                                            OICFree(rtData);
                                        }

                                        // "if"
                                        if (0 == strcmp(OIC_JSON_IF_NAME, rMapName))
                                        {
                                            // TODO : Need to check data structure of OicSecPdAcl_t and assign based on RAML spec.
                                            char *ifData = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &ifData, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF Value.");
                                            OICFree(ifData);
                                        }

                                        if (cbor_value_is_valid(&rMap))
                                        {
                                            cborFindResult = cbor_value_advance(&rMap);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Rlist Map.");
                                        }
                                        OICFree(rMapName);
                                    }

                                    if (cbor_value_is_valid(&resources))
                                    {
                                        cborFindResult = cbor_value_advance(&resources);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                                    }
                                }
                            }

                            // Permissions -- Mandatory
                            if (strcmp(name, OIC_JSON_PERMISSION_NAME) == 0 && cbor_value_is_unsigned_integer(&pdAclMap))
                            {
                                uint64_t permission = 0;
                                cborFindResult = cbor_value_get_uint64(&pdAclMap, &permission);
                                pdacl->permission = (uint16_t)permission;
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
                            }

                            // Period -- Not mandatory
                            if (strcmp(name, OIC_JSON_PERIODS_NAME) == 0 && cbor_value_is_array(&pdAclMap))
                            {
                                int i = 0;
                                CborValue period = { .parser = NULL };
                                cborFindResult = cbor_value_get_array_length(&pdAclMap,
                                        &pdacl->prdRecrLen);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get length");
                                cborFindResult = cbor_value_enter_container(&pdAclMap, &period);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");
                                pdacl->periods = (char **) OICCalloc(pdacl->prdRecrLen, sizeof(char*));
                                VERIFY_NON_NULL(TAG, pdacl->periods, ERROR);

                                while (cbor_value_is_text_string(&period) && cbor_value_is_text_string(&period))
                                {
                                    cborFindResult = cbor_value_dup_text_string(&period,
                                            &pdacl->periods[i++], &len, NULL);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get text");
                                    cborFindResult = cbor_value_advance(&period);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                                    pdacl->prdRecrLen++;
                                }
                            }

                            // Recurrence -- Not mandatory
                            if (strcmp(name, OIC_JSON_RECURRENCES_NAME) == 0 && cbor_value_is_array(&pdAclMap))
                            {
                                int i = 0;
                                CborValue recurrences = { .parser = NULL };
                                cborFindResult = cbor_value_get_array_length(&pdAclMap, &pdacl->prdRecrLen);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get length");
                                cborFindResult = cbor_value_enter_container(&pdAclMap, &recurrences);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");
                                pdacl->recurrences = (char **) OICCalloc(pdacl->prdRecrLen, sizeof(char*));
                                VERIFY_NON_NULL(TAG, pdacl->recurrences, ERROR);

                                while (cbor_value_is_text_string(&recurrences) && cbor_value_is_text_string(&recurrences))
                                {
                                    cborFindResult = cbor_value_dup_text_string(&recurrences,
                                            &pdacl->recurrences[i++], &len, NULL);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
                                    cborFindResult = cbor_value_advance(&recurrences);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                                }
                            }
                            if (type != CborMapType && cbor_value_is_valid(&pdAclMap))
                            {
                                cborFindResult = cbor_value_advance(&pdAclMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                            }
                        }
                        if (cbor_value_is_valid(&pdAclArray))
                        {
                            cborFindResult = cbor_value_advance(&pdAclArray);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                        }
                        OICFree(name);
                        name = NULL;
                    }
                    pdacl->next = NULL;
                    if (headPdacl == NULL)
                    {
                        headPdacl = pdacl;
                    }
                    else
                    {
                        OicSecPdAcl_t *temp = headPdacl;
                        while (temp->next)
                        {
                            temp = temp->next;
                        }
                        temp->next = pdacl;
                    }
                }
                pconf->pdacls = headPdacl;
            }

            //PDDev -- Mandatory
            if (strcmp(name, OIC_JSON_PDDEV_LIST_NAME) == 0 && cbor_value_is_array(&pconfMap))
            {
                int i = 0;
                CborValue pddevs = { .parser = NULL };
                cborFindResult = cbor_value_get_array_length(&pconfMap, &pconf->pddevLen);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get length");
                cborFindResult = cbor_value_enter_container(&pconfMap, &pddevs);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to enter");

                pconf->pddevs = (OicUuid_t *)OICMalloc(pconf->pddevLen * sizeof(OicUuid_t));
                VERIFY_NON_NULL(TAG, pconf->pddevs, ERROR);
                while (cbor_value_is_valid(&pddevs) && cbor_value_is_text_string(&pddevs))
                {
                    char *pddev = NULL;
                    cborFindResult = cbor_value_dup_text_string(&pddevs, &pddev, &len, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get value");
                    cborFindResult = cbor_value_advance(&pddevs);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
                    ret = ConvertStrToUuid(pddev, &pconf->pddevs[i++]);
                    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                    OICFree(pddev);
                }
            }

            //Mandatory - Device Id
            if (0 == strcmp(OIC_JSON_DEVICE_ID_NAME, name) && cbor_value_is_text_string(&pconfMap))
            {
                char *deviceId = NULL;
                cborFindResult = cbor_value_dup_text_string(&pconfMap, &deviceId, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get deviceID");
                ret = ConvertStrToUuid(deviceId, &pconf->deviceID);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(deviceId);
            }

            // ROwner -- Mandatory
            if (0 == strcmp(OIC_JSON_ROWNERID_NAME, name) && cbor_value_is_text_string(&pconfMap))
            {
                char *rowner = NULL;
                cborFindResult = cbor_value_dup_text_string(&pconfMap, &rowner, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to get rowner");
                ret = ConvertStrToUuid(rowner, &pconf->rownerID);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(rowner);
            }
        }
        if (CborMapType != type && cbor_value_is_valid(&pconfMap))
        {
            cborFindResult = cbor_value_advance(&pconfMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed to advance");
        }
        OICFree(name);
        name = NULL;
    }
    *secPconf=pconf;
    ret = OC_STACK_OK;
exit:
    if (CborNoError != cborFindResult)
    {
        OIC_LOG (ERROR, TAG, "CBORPayloadToPconf failed");
        DeletePconfBinData(pconf);
        pconf = NULL;
        *secPconf = NULL;
        ret = OC_STACK_ERROR;
    }
    return ret;
}

static bool UpdatePersistentStorage(const OicSecPconf_t * pconf)
{
    bool ret = false;

    // Convert PCONF data into Cborpayload for update to persistent storage
    uint8_t *payload = NULL;
    size_t size = 0;
    if (OC_STACK_OK == PconfToCBORPayload(pconf, &payload, &size) && NULL !=payload)
    {
        if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_PCONF_NAME, payload, size))
        {
            ret = true;
        }
        OICFree(payload);
    }
    return ret;
}

static OCEntityHandlerResult HandlePconfGetRequest (const OCEntityHandlerRequest * ehRequest)
{
    uint8_t* payload = NULL;
    size_t size = 0;
    OicSecDoxm_t *m_doxm = NULL;
    OCEntityHandlerResult ehRet = OC_EH_OK;

    OicSecPconf_t pconf;
    memset(&pconf, 0, sizeof(OicSecPconf_t));

    OIC_LOG (DEBUG, TAG, "Pconf EntityHandle processing GET request");

    m_doxm = GetDoxmResourceData();
    if (NULL == m_doxm)
    {
      OIC_LOG (DEBUG, TAG, "Doxm resource Data is NULL");
    }

    if ((m_doxm) && (true == m_doxm->dpc))
    {
        //Making response elements for Get request
        if( (true == gPconf->edp) &&
            (gPconf->prm && 0 < gPconf->prmLen) &&
            (0 < strlen((const char*)gPconf->deviceID.id)) &&
            (0 < strlen((const char*)gPconf->rownerID.id)))
        {
            pconf.edp = true;
            pconf.prm = gPconf->prm;
            pconf.prmLen = gPconf->prmLen;
            memcpy(&pconf.deviceID, &gPconf->deviceID, sizeof(OicUuid_t));
            memcpy(&pconf.rownerID, &gPconf->rownerID, sizeof(OicUuid_t));
            OIC_LOG (DEBUG, TAG, "PCONF - direct pairing enabled");
        }
        else if (false == gPconf->edp)
        {
            pconf.edp = false;
            memcpy(&pconf.rownerID, &gPconf->rownerID, sizeof(OicUuid_t));
            OIC_LOG (DEBUG, TAG, "PCONF - direct pairing disable");
        }
        else
        {
            ehRet= OC_EH_ERROR;
            OIC_LOG (DEBUG, TAG, "PCONF - error");
        }
    }
    else
    {
        OIC_LOG (DEBUG, TAG, "DPC == false : Direct-Pairing Disabled");
    }


    if (OC_STACK_OK != PconfToCBORPayload(gPconf, &payload, &size))
    {
        ehRet = OC_EH_ERROR;
    }

    if(OC_EH_OK == ehRet)
    {
        ehRet = (payload ? OC_EH_OK : OC_EH_ERROR);
    }
    else
    {
        OICFree(payload);
        payload = NULL;
        size = 0;
    }

    // Send response payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, payload, size))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandlePconfGetRequest");
    }
    OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__, ehRet);

    OICFree(payload);
    return ehRet;
}

static OCEntityHandlerResult HandlePconfPostRequest (const OCEntityHandlerRequest * ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_OK;
    OCStackResult res=OC_STACK_OK;
    OicSecPconf_t* newPconf = NULL;

    if (NULL != GetDoxmResourceData() && true == GetDoxmResourceData()->dpc)
    {
        // Convert CBOR PCONF data into binary. This will also validate the PCONF data received.
        uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
        size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;

        if(payload){
            res = CBORPayloadToPconf(payload, size, &newPconf);
        }
    }
    else
    {
        OIC_LOG (DEBUG, TAG, "DPC == false : Direct-Pairing Disabled");
        ehRet = OC_EH_ERROR;
    }

    if (newPconf && res == OC_STACK_OK)
    {
        // Check if valid Post request
        if ((true == newPconf->edp) && (0 < newPconf->prmLen) &&
                DP_PIN_LENGTH == sizeof(newPconf->pin.val))
        {
            OicSecPrm_t *oldPrm = gPconf->prm;
            OicSecPdAcl_t *oldPdacl = gPconf->pdacls;

            // Update local PCONF with new PCONF
            gPconf->edp = true;
            memcpy(&gPconf->pin, &newPconf->pin, sizeof(OicDpPin_t));
            gPconf->prm = newPconf->prm;
            gPconf->prmLen = newPconf->prmLen;
            gPconf->pdacls = newPconf->pdacls;
            memcpy(&gPconf->rownerID, &newPconf->rownerID, sizeof(OicUuid_t));

            // to delete old value(prm, pdacl)
            newPconf->prm = oldPrm;
            newPconf->pdacls = oldPdacl;
        }
        else if (false == newPconf->edp)
        {
            gPconf->edp = false;
        }
        else
        {
            ehRet = OC_EH_ERROR;
        }

        // Update storage
        if(OC_EH_ERROR != ehRet && true == UpdatePersistentStorage(gPconf))
        {
            ehRet = OC_EH_CHANGED;
        }

        DeletePconfBinData(newPconf);
    }
    else
    {
        ehRet = OC_EH_ERROR;
    }

    // Send payload to request originator
    if (OC_STACK_OK != SendSRMResponse(ehRequest, ehRet, NULL, 0))
    {
        ehRet = OC_EH_ERROR;
        OIC_LOG(ERROR, TAG, "SendSRMResponse failed in HandlePconfPostRequest");
    }

    OIC_LOG_V (DEBUG, TAG, "%s RetVal %d", __func__ , ehRet);
    return ehRet;
}

/*
 * This internal method is the entity handler for PCONF resources and
 * will handle REST request (POST) for them.
 */
OCEntityHandlerResult PconfEntityHandler (OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParameter)
{
    OIC_LOG(DEBUG, TAG, "Received request PconfEntityHandler");
    (void)callbackParameter;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if (!ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG (DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandlePconfGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandlePconfPostRequest(ehRequest);
                break;

            case OC_REST_DELETE:
                break;

            default:
                ehRet = OC_EH_ERROR;
                SendSRMResponse(ehRequest, ehRet, NULL, 0);
        }
    }

    return ehRet;
}

/*
 * This internal method is used to create '/oic/sec/pconf' resource.
 */
OCStackResult CreatePconfResource()
{
    OCStackResult ret;

    ret = OCCreateResource(&gPconfHandle,
                           OIC_RSRC_TYPE_SEC_PCONF,
                           OC_RSRVD_INTERFACE_DEFAULT,
                           OIC_RSRC_PCONF_URI,
                           PconfEntityHandler,
                           NULL,
                           OC_SECURE | OC_EXPLICIT_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG (ERROR, TAG, "Unable to instantiate PCONF resource");
        DeInitPconfResource();
    }
    return ret;
}

/**
 * Get the default value.
 * @retval  the gDefaultPconf pointer;
 */
static OicSecPconf_t* GetPconfDefault()
{
    OIC_LOG (DEBUG, TAG, "GetPconfDefault");

    return &gDefaultPconf;
}

/**
 * This method is used by SRM to retrieve PCONF resource data..
 *
 * @retval  reference to @ref OicSecPconf_t, binary format of Pconf resource data
 */
const OicSecPconf_t* GetPconfResourceData()
{
    return gPconf;
}

/**
 * Initialize PCONF resource by loading data from persistent storage.
 *
 * @retval  OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InitPconfResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    uint8_t *data = NULL;
    size_t size = 0;

    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_PCONF_NAME, &data, &size);
    // If database read failed
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        CBORPayloadToPconf(data, size, &gPconf);
    }

    if (!data || !gPconf)
    {
        gPconf = GetPconfDefault();

        // device id from doxm
        OicUuid_t deviceId = {.id = {0}};
        OCStackResult ret = GetDoxmDeviceID( &deviceId);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);
        memcpy(&gPconf->deviceID, &deviceId, sizeof(OicUuid_t));
    }
    VERIFY_NON_NULL(TAG, gPconf, ERROR);

    // Instantiate 'oic.sec.pconf'
    ret = CreatePconfResource();

exit:
    if (OC_STACK_OK != ret)
    {
        DeInitPconfResource();
    }
    OICFree(data);
    return ret;
}

/**
 * Perform cleanup for PCONF resources.
 *
 * @return
 * OC_STACK_OK    - no error
 * OC_STACK_ERROR - stack process error
 *
 */
OCStackResult DeInitPconfResource()
{
    OCStackResult ret = OCDeleteResource(gPconfHandle);
    if(gPconf!= &gDefaultPconf)
    {
        DeletePconfBinData(gPconf);
    }
    gPconf = NULL;

    if(OC_STACK_OK == ret)
    {
        return OC_STACK_OK;
    }
    else
    {
        return OC_STACK_ERROR;
    }
}

/**
 * This method might be used to add a paired device id after direct-pairing process complete.
 *
 * @param pdeviceId ID of the paired device.
 *
 * @retval  OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult AddPairedDevice(OicUuid_t *pdeviceId)
{
    if (!gPconf || !pdeviceId)
    {
        return OC_STACK_INVALID_PARAM;
    }


    OicUuid_t *prevList = gPconf->pddevs;
    gPconf->pddevs = (OicUuid_t *)OICCalloc(gPconf->pddevLen+1, sizeof(OicUuid_t));
    if(!gPconf->pddevs)
    {
        return OC_STACK_NO_MEMORY;
    }
    for (size_t i=0; i<gPconf->pddevLen; i++)
    {
        memcpy(&gPconf->pddevs[i], &prevList[i], sizeof(OicUuid_t));
    }

    // add new paired device id
    memcpy(&gPconf->pddevs[gPconf->pddevLen], pdeviceId, sizeof(OicUuid_t));
    gPconf->pddevLen++;

    // Update storage
    if(true != UpdatePersistentStorage(gPconf))
    {
        OIC_LOG (ERROR, TAG, "Fail to update pconf resource");
        return OC_STACK_ERROR;
    }

    OIC_LOG (ERROR, TAG, "Add paired device success");
    return OC_STACK_OK;
}

/**
 * This method might be used by PolicyEngine to retrieve PDACL for a Subject.
 *
 * @param subjectId ID of the subject for which PDACL is required.
 * @param savePtr is used internally by @ref GetACLResourceData to maintain index between
 *                successive calls for same subjectId.
 *
 * @retval  reference to @ref OicSecPdAcl_t if PDACL is found, else NULL
 */
const OicSecPdAcl_t* GetPdAclData(const OicUuid_t* subjectId, OicSecPdAcl_t **savePtr)
{
    OicSecPdAcl_t *pdacl = NULL;

    if ( NULL == subjectId)
    {
        return NULL;
    }

    /*
     * savePtr MUST point to NULL if this is the 'first' call to retrieve PDACL for
     * subjectID.
     */
    if (NULL == *savePtr)
    {
        pdacl = gPconf->pdacls;

        // Find if 'subjectID' is in paired device list.
        for(size_t i=0; i<gPconf->pddevLen; i++)
        {
            if (memcmp(&(gPconf->pddevs[i]), subjectId, sizeof(OicUuid_t)) == 0)
            {
                *savePtr = pdacl;
                return pdacl;
            }
        }
    }
    else
    {
        OicSecPdAcl_t *temp = NULL;

        /*
         * If this is a 'successive' call, search for location pointed by
         * savePtr and assign 'begin' to the next PDACL after it in the linked
         * list and start searching from there.
         */
        LL_FOREACH(gPconf->pdacls, temp)
        {
            if (temp == *savePtr)
            {
                pdacl = temp->next;
                *savePtr = pdacl;
                return pdacl;
            }
        }
    }

    // Cleanup in case no PDACL is found
    *savePtr = NULL;
    return NULL;
}

/**
 * This method return whether device is paired or not.
 *
 * @param pdeviceId Target device ID to find in paired list.
 * @retval  ture if device is already paired, else false
 */
bool IsPairedDevice(const OicUuid_t* pdeviceId)
{
    // Find if 'pdeviceId' is in paired device list.
    for(size_t i=0; i<gPconf->pddevLen; i++)
    {
        if (memcmp(&(gPconf->pddevs[i]), pdeviceId, sizeof(OicUuid_t)) == 0)
        {
            return true;
        }
    }
    return false;
}

OCStackResult SetPconfRownerId(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gPconf)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(newROwner && gPconf)
    {
        memcpy(prevId.id, gPconf->rownerID.id, sizeof(prevId.id));
        memcpy(gPconf->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = PconfToCBORPayload(gPconf, &cborPayload, &size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_PCONF_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gPconf->rownerID.id, prevId.id, sizeof(prevId.id));
    return ret;
}

OCStackResult GetPconfRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gPconf)
    {
        *rowneruuid = gPconf->rownerID;
        retVal = OC_STACK_OK;
    }
    return retVal;
}
