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
#ifdef HAVE_STRING_H
#include <string.h>
#elif defined(HAVE_STRINGS_H)
#include <strings.h>
#endif
#include <stdlib.h>

#include "utlist.h"
#include "ocstack.h"
#include "octypes.h"
#include "ocserverrequest.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocrandom.h"
#include "ocpayload.h"
#include "utlist.h"
#include "payload_logging.h"
#include "srmresourcestrings.h"
#include "aclresource.h"
#include "doxmresource.h"
#include "resourcemanager.h"
#include "srmutility.h"
#include "psinterface.h"
#include "ocpayloadcbor.h"

#include "security_internals.h"

#define TAG  "OIC_SRM_ACL"
#define NUMBER_OF_SEC_PROV_RSCS 3
#define NUMBER_OF_DEFAULT_SEC_RSCS 2
#define STRING_UUID_SIZE (UUID_LENGTH * 2 + 5)

static const uint8_t ACL_MAP_SIZE = 4;
static const uint8_t ACL_ACLIST_MAP_SIZE = 1;
static const uint8_t ACL_ACES_MAP_SIZE = 3;
static const uint8_t ACL_RESOURCE_MAP_SIZE = 3;


// CborSize is the default cbor payload size being used.
static const uint16_t CBOR_SIZE = 2048*8;

static OicSecAcl_t *gAcl = NULL;
static OCResourceHandle gAclHandle = NULL;

void FreeRsrc(OicSecRsrc_t *rsrc)
{
    //Clean each member of resource
    OICFree(rsrc->href);
    OICFree(rsrc->rel);
    //Clean "rt"
    if(0 < rsrc->typeLen && rsrc->types)
    {
        for(size_t i = 0; i < rsrc->typeLen; i++)
        {
            OICFree(rsrc->types[i]);
        }
        OICFree(rsrc->types);
        rsrc->types = NULL;
    }
    //Clean "if"
    if(0 < rsrc->interfaceLen && rsrc->interfaces)
    {
        for(size_t i = 0; i < rsrc->interfaceLen; i++)
        {
            OICFree(rsrc->interfaces[i]);
        }
        OICFree(rsrc->interfaces);
        rsrc->interfaces = NULL;
    }
    OICFree(rsrc);
    rsrc = NULL;
}

/**
 * This function frees OicSecAcl_t object's fields and object itself.
 */
static void FreeACE(OicSecAce_t *ace)
{
    if (NULL == ace)
    {
        OIC_LOG(ERROR, TAG, "Invalid Parameter");
        return;
    }

    //Clean Resources
    OicSecRsrc_t* rsrc = NULL;
    OicSecRsrc_t* tmpRsrc = NULL;
    LL_FOREACH_SAFE(ace->resources, rsrc, tmpRsrc)
    {
        LL_DELETE(ace->resources, rsrc);
        FreeRsrc(rsrc);
    }

    //Clean Validities
    OicSecValidity_t *validity = NULL;
    OicSecValidity_t *tmpValidity = NULL;
    LL_FOREACH_SAFE(ace->validities, validity, tmpValidity)
    {
        LL_DELETE(ace->validities, validity);

        //Clean period
        OICFree(validity->period);

        //Clean recurrence
        for(size_t i = 0; i < validity->recurrenceLen; i++)
        {
            OICFree(validity->recurrences[i]);
        }
        OICFree(validity->recurrences);
        OICFree(validity);
        validity = NULL;
    }

#ifdef MULTIPLE_OWNER
    OICFree(ace->eownerID);
#endif

    //Clean ACE
    OICFree(ace);
    ace = NULL;
}

void DeleteACLList(OicSecAcl_t* acl)
{
    if (acl)
    {
        OicSecAce_t *ace = NULL;
        OicSecAce_t *tmpAce = NULL;
        LL_FOREACH_SAFE(acl->aces, ace, tmpAce)
        {
            LL_DELETE(acl->aces, ace);
            FreeACE(ace);
        }
        acl->aces = NULL;
        OICFree(acl);
    }
}

OicSecAce_t* DuplicateACE(const OicSecAce_t* ace)
{
    OicSecAce_t* newAce = NULL;
    size_t allocateSize = 0;

    if(ace)
    {
        newAce = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
        VERIFY_NON_NULL(TAG, newAce, ERROR);

        //Subject uuid
        memcpy(&newAce->subjectuuid, &ace->subjectuuid, sizeof(OicUuid_t));

        OicSecRsrc_t* rsrc = NULL;
        LL_FOREACH(ace->resources, rsrc)
        {
            OicSecRsrc_t* newRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
            VERIFY_NON_NULL(TAG, newRsrc, ERROR);
            LL_APPEND(newAce->resources, newRsrc);

            //href is mandatory
            VERIFY_NON_NULL(TAG, rsrc->href, ERROR);
            allocateSize = strlen(rsrc->href) + 1;
            newRsrc->href = (char*)OICMalloc(sizeof(char) * allocateSize);
            VERIFY_NON_NULL(TAG, newRsrc->href, ERROR);
            OICStrcpy(newRsrc->href, allocateSize, rsrc->href);

            if(rsrc->rel)
            {
                allocateSize = strlen(rsrc->rel) + 1;
                newRsrc->rel = (char*)OICMalloc(sizeof(char) * allocateSize);
                VERIFY_NON_NULL(TAG, newRsrc->rel, ERROR);
                OICStrcpy(newRsrc->rel, allocateSize, rsrc->rel);
            }

            if(rsrc->types && 0 < rsrc->typeLen)
            {
                newRsrc->typeLen = rsrc->typeLen;
                newRsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                VERIFY_NON_NULL(TAG, (newRsrc->types), ERROR);
                for(size_t i = 0; i < rsrc->typeLen; i++)
                {
                    newRsrc->types[i] = OICStrdup(rsrc->types[i]);
                    VERIFY_NON_NULL(TAG, (newRsrc->types[i]), ERROR);
                }
            }

            if(rsrc->interfaces && 0 < rsrc->interfaceLen)
            {
                newRsrc->interfaceLen = rsrc->interfaceLen;
                newRsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char*));
                VERIFY_NON_NULL(TAG, (newRsrc->interfaces), ERROR);
                for(size_t i = 0; i < rsrc->interfaceLen; i++)
                {
                    newRsrc->interfaces[i] = OICStrdup(rsrc->interfaces[i]);
                    VERIFY_NON_NULL(TAG, (newRsrc->interfaces[i]), ERROR);
                }
            }
        }

        //Permission
        newAce->permission = ace->permission;

        //Validity
        if(ace->validities)
        {
            OicSecValidity_t* validity = NULL;
            LL_FOREACH(ace->validities, validity)
            {
                OicSecValidity_t* newValidity = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
                VERIFY_NON_NULL(TAG, newValidity, ERROR);
                LL_APPEND(newAce->validities, newValidity);

                if(validity->period)
                {
                    allocateSize = strlen(validity->period) + 1;
                    newValidity->period = (char*)OICMalloc(sizeof(char) * allocateSize);
                    VERIFY_NON_NULL(TAG, newValidity->period, ERROR);
                    OICStrcpy(newValidity->period, allocateSize, validity->period);
                }

                if(validity->recurrences && 0 < validity->recurrenceLen)
                {
                    newValidity->recurrenceLen = validity->recurrenceLen;

                    newValidity->recurrences = (char**)OICMalloc(sizeof(char*) * validity->recurrenceLen);
                    VERIFY_NON_NULL(TAG, newValidity->recurrences, ERROR);

                    for(size_t i = 0; i < validity->recurrenceLen; i++)
                    {
                        allocateSize = strlen(validity->recurrences[i]) + 1;
                        newValidity->recurrences[i] = (char*)OICMalloc(sizeof(char) * allocateSize);
                        VERIFY_NON_NULL(TAG, (newValidity->recurrences[i]), ERROR);
                        OICStrcpy(newValidity->recurrences[i], allocateSize, validity->recurrences[i]);
                    }
                }
            }
        }

#ifdef MULTIPLE_OWNER
        if (ace->eownerID)
        {
            if (NULL == newAce->eownerID)
            {
                newAce->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
                VERIFY_NON_NULL(TAG, (newAce->eownerID), ERROR);
            }
            memcpy(newAce->eownerID->id, ace->eownerID->id, sizeof(ace->eownerID->id));
        }
#endif

        newAce->next = NULL;
    }

    return newAce;

exit:
    FreeACE(newAce);
    return NULL;
}

static size_t OicSecAclSize(const OicSecAcl_t *secAcl)
{
    if (!secAcl)
    {
        return 0;
    }
    OicSecAce_t *ace= (OicSecAce_t *)secAcl->aces;
    size_t size = 0;
    while (ace)
    {
       size++;
       ace = ace->next;
    }
    return size;
}

OCStackResult AclToCBORPayload(const OicSecAcl_t *secAcl, uint8_t **payload, size_t *size)
{
     if (NULL == secAcl || NULL == payload || NULL != *payload || NULL == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult ret = OC_STACK_ERROR;
    CborError cborEncoderResult = CborNoError;
    OicSecAcl_t *acl = (OicSecAcl_t *)secAcl;
    OicSecAce_t* ace = NULL;
    CborEncoder encoder;
    CborEncoder aclMap;
    CborEncoder aclListMap;
    CborEncoder acesArray;
    uint8_t *outPayload = NULL;
    size_t cborLen = *size;
    *size = 0;
    *payload = NULL;

    if (cborLen == 0)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NON_NULL(TAG, outPayload, ERROR);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create ACL Map (aclist, rownerid)
    cborEncoderResult = cbor_encoder_create_map(&encoder, &aclMap, ACL_MAP_SIZE);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACL Map.");

    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ACLIST_NAME,
        strlen(OIC_JSON_ACLIST_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aclist Name Tag.");

    // Create ACLIST Map (aces)
    cborEncoderResult = cbor_encoder_create_map(&aclMap, &aclListMap, ACL_ACLIST_MAP_SIZE);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACLIST Map.");

    cborEncoderResult = cbor_encode_text_string(&aclListMap, OIC_JSON_ACES_NAME,
        strlen(OIC_JSON_ACES_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACES Name Tag.");

    // Create ACES Array
    cborEncoderResult = cbor_encoder_create_array(&aclListMap, &acesArray, OicSecAclSize(secAcl));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACES Array.");

    ace = NULL;
    LL_FOREACH (acl->aces, ace)
    {
        CborEncoder oicSecAclMap;
        // ACL Map size - Number of mandatory items
        uint8_t aclMapSize = ACL_ACES_MAP_SIZE;
        size_t inLen = 0;

        OicSecValidity_t* validityElts = ace->validities;
        while(validityElts)
        {
            if(validityElts->period)
            {
                aclMapSize++;
            }
            if(validityElts->recurrences)
            {
                aclMapSize++;
            }
            validityElts = validityElts->next;
        }

#ifdef MULTIPLE_OWNER
        if(ace->eownerID)
        {
            aclMapSize++;
        }
#endif //MULTIPLE_OWNER

        cborEncoderResult = cbor_encoder_create_map(&acesArray, &oicSecAclMap, aclMapSize);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACES Map");

        // Subject -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_SUBJECTID_NAME,
            strlen(OIC_JSON_SUBJECTID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Subject Name Tag.");
        inLen = (memcmp(&(ace->subjectuuid), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0) ?
            WILDCARD_SUBJECT_ID_LEN : sizeof(OicUuid_t);
        if(inLen == WILDCARD_SUBJECT_ID_LEN)
        {
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, WILDCARD_RESOURCE_URI,
                strlen(WILDCARD_RESOURCE_URI));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject Id wildcard Value.");
        }
        else
        {
            char *subject = NULL;
            ret = ConvertUuidToStr(&ace->subjectuuid, &subject);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, subject, strlen(subject));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Subject UUID Value.");
            OICFree(subject);
        }

        // Resources
        {
            CborEncoder resources;
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_RESOURCES_NAME,
                strlen(OIC_JSON_RESOURCES_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Name Tag.");

            size_t rsrcLen = 0;
            OicSecRsrc_t* rsrcElts = NULL;
            LL_FOREACH(ace->resources, rsrcElts)
            {
                rsrcLen++;
            }

            cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &resources, rsrcLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Name Array.");

            OicSecRsrc_t* rsrc = NULL;
            LL_FOREACH(ace->resources, rsrc)
            {

                CborEncoder rMap;
                size_t rsrcMapSize = ACL_RESOURCE_MAP_SIZE;
                if(rsrc->rel)
                {
                    rsrcMapSize++;
                }

                cborEncoderResult = cbor_encoder_create_map(&resources, &rMap, rsrcMapSize);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding Resource Map.");

                //href -- Mandatory
                VERIFY_NON_NULL(TAG, rsrc->href, ERROR);
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_HREF_NAME,
                        strlen(OIC_JSON_HREF_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Name Tag.");
                cborEncoderResult = cbor_encode_text_string(&rMap, rsrc->href, strlen(rsrc->href));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding HREF Value in Map.");

                //resource type -- Mandatory
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_RT_NAME,
                        strlen(OIC_JSON_RT_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");

                CborEncoder resourceTypes;
                cborEncoderResult = cbor_encoder_create_array(&rMap, &resourceTypes, rsrc->typeLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Array.");
                for(size_t i = 0; i < rsrc->typeLen; i++)
                {
                    cborEncoderResult = cbor_encode_text_string(&resourceTypes, rsrc->types[i], strlen(rsrc->types[i]));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
                }
                cborEncoderResult = cbor_encoder_close_container(&rMap, &resourceTypes);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing resourceTypes.");

                //interface -- Mandatory
                cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_IF_NAME,
                        strlen(OIC_JSON_IF_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");

                CborEncoder interfaces;
                cborEncoderResult = cbor_encoder_create_array(&rMap, &interfaces, rsrc->interfaceLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Array.");
                for(size_t i = 0; i < rsrc->interfaceLen; i++)
                {
                    cborEncoderResult = cbor_encode_text_string(&interfaces, rsrc->interfaces[i], strlen(rsrc->interfaces[i]));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
                }
                cborEncoderResult = cbor_encoder_close_container(&rMap, &interfaces);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing interfaces.");

                //rel
                if(rsrc->rel)
                {
                    cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_REL_NAME,
                            strlen(OIC_JSON_REL_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Name Tag.");
                    cborEncoderResult = cbor_encode_text_string(&rMap, rsrc->rel, strlen(rsrc->rel));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding REL Value.");
                }

                cborEncoderResult = cbor_encoder_close_container(&resources, &rMap);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Map.");
            }
            cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &resources);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Name Array.");
        }

        // Permissions -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_PERMISSION_NAME,
            strlen(OIC_JSON_PERMISSION_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Tag.");
        cborEncoderResult = cbor_encode_int(&oicSecAclMap, ace->permission);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Value.");

        // TODO: Need to verfication for validity
        // Validity(Time-interval) -- Not Mandatory
        if(ace->validities)
        {
            size_t validityLen = 0;
            validityElts = NULL;
            LL_FOREACH(ace->validities, validityElts)
            {
                validityLen++;
            }

            CborEncoder validities;
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_VALIDITY_NAME,
                strlen(OIC_JSON_VALIDITY_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Validity Tag.");
            cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &validities, validityLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Validities Array.");

            //How to add the value w/o "title" using tinycobr...? :(

            validityElts = NULL;
            LL_FOREACH(ace->validities, validityElts)
            {
                 CborEncoder validity;
                 size_t validitySize = 0;
                 if(validityElts->period)
                {
                    validitySize++;
                }
                if(validityElts->recurrences)
                {
                    validitySize++;
                }

                cborEncoderResult = cbor_encoder_create_array(&validities, &validity, validitySize);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Validity Array.");

                // Period
                if (validityElts->period)
                {
                    cborEncoderResult = cbor_encode_text_string(&validity, validityElts->period,
                        strlen(validityElts->period));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Period Value.");
                }

                // Recurrence
                if (validityElts->recurrences)
                {
                    CborEncoder recurrences;
                    cborEncoderResult = cbor_encoder_create_array(&validity, &recurrences, validityElts->recurrenceLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Recurrence Array.");

                    for (size_t i = 0; i < validityElts->recurrenceLen; i++)
                    {
                        cborEncoderResult = cbor_encode_text_string(&recurrences, validityElts->recurrences[i],
                            strlen(validityElts->recurrences[i]));
                        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Recurrence Array Value.");
                    }
                    cborEncoderResult = cbor_encoder_close_container(&validity, &recurrences);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Recurrence Array");
                }

                cborEncoderResult = cbor_encoder_close_container(&validities, &validity);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Validity Array.");
            }

            cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &validities);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Validities Array.");
        }

#ifdef MULTIPLE_OWNER
        // Eownerid -- Not Mandatory
        if(ace->eownerID)
        {
            char *eowner = NULL;
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_EOWNERID_NAME,
                strlen(OIC_JSON_EOWNERID_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding eownerId Name Tag.");
            ret = ConvertUuidToStr(ace->eownerID, &eowner);
            VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, eowner, strlen(eowner));
            OICFree(eowner);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding eownerId Value.");
        }
#endif //MULTIPLE_OWNER

        cborEncoderResult = cbor_encoder_close_container(&acesArray, &oicSecAclMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACES Map.");
    }

    // Close ACES Array
    cborEncoderResult = cbor_encoder_close_container(&aclListMap, &acesArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACES Array.");

    // Close ACLIST Map
    cborEncoderResult = cbor_encoder_close_container(&aclMap, &aclListMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACLIST Map.");

    // Rownerid
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding rownerid Name.");
        ret = ConvertUuidToStr(&secAcl->rownerID, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&aclMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding rownerid Value.");
        OICFree(rowner);
    }

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&aclMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_ACL,
                strlen(OIC_RSRC_TYPE_SEC_ACL));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&aclMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");

    //IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_IF_NAME,
             strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
    cborEncoderResult = cbor_encoder_create_array(&aclMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
    }
    cborEncoderResult = cbor_encoder_close_container(&aclMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");

    // Close ACL Map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &aclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACL Map.");

    if (CborNoError == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "AclToCBORPayload Successed");
        *size = cbor_encoder_get_buffer_size(&encoder, outPayload);
        *payload = outPayload;
        ret = OC_STACK_OK;
    }
exit:
    if (CborErrorOutOfMemory == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "AclToCBORPayload:CborErrorOutOfMemory : retry with more memory");

        // reallocate and try again!
        OICFree(outPayload);
        // Since the allocated initial memory failed, double the memory.
        cborLen += cbor_encoder_get_buffer_size(&encoder, encoder.end);
        cborEncoderResult = CborNoError;
        ret = AclToCBORPayload(secAcl, payload, &cborLen);
        *size = cborLen;
    }
    else if (cborEncoderResult != CborNoError)
    {
        OIC_LOG(ERROR, TAG, "Failed to AclToCBORPayload");
        OICFree(outPayload);
        outPayload = NULL;
        *size = 0;
        *payload = NULL;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

// This function converts CBOR format to ACL data.
// Caller needs to invoke 'free' when done using
// It parses { "aclist" : [ { ... } ] } instead of { "aclist" : { "aces" : [ ] } }
OicSecAcl_t* CBORPayloadToAcl2(const uint8_t *cborPayload, const size_t size)
{
    if (NULL == cborPayload || 0 == size)
    {
        return NULL;
    }
    OCStackResult ret = OC_STACK_ERROR;
    CborValue aclCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    cbor_parser_init(cborPayload, size, 0, &parser, &aclCbor);

    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    VERIFY_NON_NULL(TAG, acl, ERROR);

    // Enter ACL Map
    CborValue aclMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    cborFindResult = cbor_value_enter_container(&aclCbor, &aclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Map.");

    while (cbor_value_is_valid(&aclMap))
    {
        char* tagName = NULL;
        size_t len = 0;
        CborType type = cbor_value_get_type(&aclMap);
        if (type == CborTextStringType)
        {
            cborFindResult = cbor_value_dup_text_string(&aclMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACL Map.");
            cborFindResult = cbor_value_advance(&aclMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACL Map.");
        }
        if(tagName)
        {
            if (strcmp(tagName, OIC_JSON_ACLIST_NAME)  == 0)
            {
                // Enter ACES Array
                CborValue acesArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                cborFindResult = cbor_value_enter_container(&aclMap, &acesArray);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACES Array.");

                int acesCount = 0;
                while (cbor_value_is_valid(&acesArray))
                {
                    acesCount++;
                    CborValue aceMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                    cborFindResult = cbor_value_enter_container(&acesArray, &aceMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACE Map.");

                    OicSecAce_t *ace = NULL;
                    ace = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
                    VERIFY_NON_NULL(TAG, ace, ERROR);
                    LL_APPEND(acl->aces, ace);

                    while (cbor_value_is_valid(&aceMap))
                    {
                        char* name = NULL;
                        size_t len = 0;
                        CborType type = cbor_value_get_type(&aceMap);
                        if (type == CborTextStringType)
                        {
                            cborFindResult = cbor_value_dup_text_string(&aceMap, &name, &len, NULL);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACE Map.");
                            cborFindResult = cbor_value_advance(&aceMap);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACE Map.");
                        }
                        if (name)
                        {
                            // Subject -- Mandatory
                            if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
                            {
                                char *subject = NULL;
                                cborFindResult = cbor_value_dup_text_string(&aceMap, &subject, &len, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                                if(strcmp(subject, WILDCARD_RESOURCE_URI) == 0)
                                {
                                    ace->subjectuuid.id[0] = '*';
                                }
                                else
                                {
                                    OIC_LOG_V(DEBUG, TAG, "Converting subjectuuid = %s to uuid...", subject);
                                    ret = ConvertStrToUuid(subject, &ace->subjectuuid);
                                    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                                }
                                OICFree(subject);
                            }

                            // Resources -- Mandatory
                            if (strcmp(name, OIC_JSON_RESOURCES_NAME) == 0)
                            {
                                CborValue resources = { .parser = NULL };
                                cborFindResult = cbor_value_enter_container(&aceMap, &resources);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering a Resource Array.");

                                while (cbor_value_is_valid(&resources))
                                {
                                    // rMap
                                    CborValue rMap = { .parser = NULL  };
                                    cborFindResult = cbor_value_enter_container(&resources, &rMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering Resource Map");

                                    OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
                                    VERIFY_NON_NULL(TAG, rsrc, ERROR);
                                    LL_APPEND(ace->resources, rsrc);

                                    while(cbor_value_is_valid(&rMap))
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
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->href, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                                        }

                                        // "rt"
                                        if (0 == strcmp(OIC_JSON_RT_NAME, rMapName) && cbor_value_is_array(&rMap))
                                        {
                                            cbor_value_get_array_length(&rMap, &rsrc->typeLen);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT array length.");
                                            VERIFY_SUCCESS(TAG, (0 != rsrc->typeLen), ERROR);

                                            rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                                            VERIFY_NON_NULL(TAG, rsrc->types, ERROR);

                                            CborValue resourceTypes;
                                            cborFindResult = cbor_value_enter_container(&rMap, &resourceTypes);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering RT Array.");

                                            for(size_t i = 0; cbor_value_is_valid(&resourceTypes) && cbor_value_is_text_string(&resourceTypes); i++)
                                            {
                                                size_t readLen = 0;
                                                cborFindResult = cbor_value_dup_text_string(&resourceTypes, &(rsrc->types[i]), &readLen, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding resource type.");
                                                cborFindResult = cbor_value_advance(&resourceTypes);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing resource type.");
                                            }
                                        }

                                        // "if"
                                        if (0 == strcmp(OIC_JSON_IF_NAME, rMapName) && cbor_value_is_array(&rMap))
                                        {
                                            cbor_value_get_array_length(&rMap, &rsrc->interfaceLen);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF array length.");
                                            VERIFY_SUCCESS(TAG, (0 != rsrc->interfaceLen), ERROR);

                                            rsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char*));
                                            VERIFY_NON_NULL(TAG, rsrc->interfaces, ERROR);

                                            CborValue interfaces;
                                            cborFindResult = cbor_value_enter_container(&rMap, &interfaces);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering IF Array.");

                                            for(size_t i = 0; cbor_value_is_valid(&interfaces) && cbor_value_is_text_string(&interfaces); i++)
                                            {
                                                size_t readLen = 0;
                                                cborFindResult = cbor_value_dup_text_string(&interfaces, &(rsrc->interfaces[i]), &readLen, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF type.");
                                                cborFindResult = cbor_value_advance(&interfaces);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing IF type.");
                                            }
                                        }

                                        // "rel"
                                        if (0 == strcmp(OIC_JSON_REL_NAME, rMapName))
                                        {
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->rel, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding REL Value.");
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
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Resource Array.");
                                    }
                                }
                            }

                            // Permissions -- Mandatory
                            if (strcmp(name, OIC_JSON_PERMISSION_NAME) == 0)
                            {
                                uint64_t tmp64;
                                cborFindResult = cbor_value_get_uint64(&aceMap, &tmp64);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a PERM Value.");
                                ace->permission = (uint16_t)tmp64;
                            }

                            // TODO: Need to verfication for validity
                            // Validity -- Not mandatory
                            if(strcmp(name, OIC_JSON_VALIDITY_NAME) == 0)
                            {
                                CborValue validitiesMap = {.parser = NULL};
                                size_t validitySize = 0;

                                cborFindResult = cbor_value_get_array_length(&aceMap, &validitySize);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Validity Array Length.");

                                cborFindResult = cbor_value_enter_container(&aceMap, &validitiesMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Array Map.");

                                while(cbor_value_is_valid(&validitiesMap))
                                {
                                    OicSecValidity_t* validity = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
                                    VERIFY_NON_NULL(TAG, validity, ERROR);
                                    LL_APPEND(ace->validities, validity);

                                    CborValue validityMap  = {.parser = NULL};
                                    //period (string)
                                    cborFindResult = cbor_value_enter_container(&validitiesMap, &validityMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Map.");

                                    size_t len = 0;
                                    cborFindResult =cbor_value_dup_text_string(&validityMap, &validity->period, &len, NULL);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period value.");

                                    //recurrence (string array)
                                    CborValue recurrenceMap  = {.parser = NULL};
                                    cborFindResult = cbor_value_enter_container(&validityMap, &recurrenceMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence array.");

                                    cborFindResult = cbor_value_get_array_length(&recurrenceMap, &validity->recurrenceLen);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array.");

                                    validity->recurrences = (char**)OICCalloc(validity->recurrenceLen, sizeof(char*));
                                    VERIFY_NON_NULL(TAG, validity->recurrences, ERROR);

                                    for(size_t i = 0; cbor_value_is_text_string(&recurrenceMap) && i < validity->recurrenceLen; i++)
                                    {
                                        cborFindResult = cbor_value_dup_text_string(&recurrenceMap, &validity->recurrences[i], &len, NULL);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence Value.");

                                        cborFindResult = cbor_value_advance(&recurrenceMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a recurrences Array.");
                                    }

                                    cborFindResult = cbor_value_advance(&validitiesMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a validities Array.");
                                }
                            }
                            OICFree(name);
                        }

                        if (type != CborMapType && cbor_value_is_valid(&aceMap))
                        {
                            cborFindResult = cbor_value_advance(&aceMap);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing the Array.");
                        }
                    }

                    if (cbor_value_is_valid(&acesArray))
                    {
                        cborFindResult = cbor_value_advance(&acesArray);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Array.");
                    }
                }
            }

            //rownerID -- Mandatory
            if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0)
            {
                char *stRowner = NULL;
                cborFindResult = cbor_value_dup_text_string(&aclMap, &stRowner, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Rownerid Value.");
                OIC_LOG_V(DEBUG, TAG, "Converting rownerid = %s to uuid...", stRowner);
                ret = ConvertStrToUuid(stRowner, &acl->rownerID);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(stRowner);
            }
            OICFree(tagName);
        }
        if (cbor_value_is_valid(&aclMap))
        {
            cborFindResult = cbor_value_advance(&aclMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Map.");
        }
    }

exit:
    if (cborFindResult != CborNoError)
    {
        OIC_LOG(ERROR, TAG, "Failed to CBORPayloadToAcl");
        DeleteACLList(acl);
        acl = NULL;
    }

    return acl;
}

// This function converts CBOR format to ACL data.
// Caller needs to invoke 'free' when done using
// note: This function is used in unit test hence not declared static,
OicSecAcl_t* CBORPayloadToAcl(const uint8_t *cborPayload, const size_t size)
{
    if (NULL == cborPayload || 0 == size)
    {
        return NULL;
    }
    OCStackResult ret = OC_STACK_ERROR;
    CborValue aclMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
    CborValue aclCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;

    cbor_parser_init(cborPayload, size, 0, &parser, &aclCbor);

    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    VERIFY_NON_NULL(TAG, acl, ERROR);

    // Enter ACL Map
    cborFindResult = cbor_value_enter_container(&aclCbor, &aclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Map.");

    while (cbor_value_is_valid(&aclMap))
    {
        char* tagName = NULL;
        size_t len = 0;
        CborType type = cbor_value_get_type(&aclMap);
        if (type == CborTextStringType)
        {
            cborFindResult = cbor_value_dup_text_string(&aclMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACL Map.");
            cborFindResult = cbor_value_advance(&aclMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACL Map.");
        }
        if(tagName)
        {
            if (strcmp(tagName, OIC_JSON_ACLIST_NAME)  == 0)
            {
                // Enter ACLIST Map
                CborValue aclistMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                cborFindResult = cbor_value_enter_container(&aclMap, &aclistMap);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACLIST Map.");

                while (cbor_value_is_valid(&aclistMap))
                {
                    char* acName = NULL;
                    size_t readLen = 0;
                    CborType acType = cbor_value_get_type(&aclistMap);
                    if (acType == CborTextStringType)
                    {
                        cborFindResult = cbor_value_dup_text_string(&aclistMap, &acName, &readLen, NULL);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACLIST Map.");
                        cborFindResult = cbor_value_advance(&aclistMap);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACLIST Map.");
                    }

                    if(acName)
                    {
                        if (strcmp(acName, OIC_JSON_ACES_NAME)  == 0)
                        {
                            // Enter ACES Array
                            CborValue acesArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                            cborFindResult = cbor_value_enter_container(&aclistMap, &acesArray);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACES Array.");

                            int acesCount = 0;
                            while (cbor_value_is_valid(&acesArray))
                            {
                                acesCount++;
                                CborValue aceMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                                cborFindResult = cbor_value_enter_container(&acesArray, &aceMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACE Map.");

                                OicSecAce_t *ace = NULL;
                                ace = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
                                VERIFY_NON_NULL(TAG, ace, ERROR);
                                LL_APPEND(acl->aces, ace);

                                while (cbor_value_is_valid(&aceMap))
                                {
                                    char* name = NULL;
                                    size_t len = 0;
                                    CborType type = cbor_value_get_type(&aceMap);
                                    if (type == CborTextStringType)
                                    {
                                        cborFindResult = cbor_value_dup_text_string(&aceMap, &name, &len, NULL);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACE Map.");
                                        cborFindResult = cbor_value_advance(&aceMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACE Map.");
                                    }
                                    if (name)
                                    {
                                        // Subject -- Mandatory
                                        if (strcmp(name, OIC_JSON_SUBJECTID_NAME)  == 0)
                                        {
                                            char *subject = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&aceMap, &subject, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                                            if(strcmp(subject, WILDCARD_RESOURCE_URI) == 0)
                                            {
                                                ace->subjectuuid.id[0] = '*';
                                            }
                                            else
                                            {
                                                ret = ConvertStrToUuid(subject, &ace->subjectuuid);
                                                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                                            }
                                            OICFree(subject);
                                        }

                                        // Resources -- Mandatory
                                        if (strcmp(name, OIC_JSON_RESOURCES_NAME) == 0)
                                        {
                                            CborValue resources = { .parser = NULL };
                                            cborFindResult = cbor_value_enter_container(&aceMap, &resources);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering a Resource Array.");

                                            while (cbor_value_is_valid(&resources))
                                            {
                                                // rMap
                                                CborValue rMap = { .parser = NULL  };
                                                cborFindResult = cbor_value_enter_container(&resources, &rMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering Resource Map");

                                                OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
                                                VERIFY_NON_NULL(TAG, rsrc, ERROR);
                                                LL_APPEND(ace->resources, rsrc);

                                                while(cbor_value_is_valid(&rMap))
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
                                                        cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->href, &len, NULL);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                                                    }

                                                    // "rt"
                                                    if (0 == strcmp(OIC_JSON_RT_NAME, rMapName) && cbor_value_is_array(&rMap))
                                                    {
                                                        cbor_value_get_array_length(&rMap, &rsrc->typeLen);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT array length.");
                                                        VERIFY_SUCCESS(TAG, (0 != rsrc->typeLen), ERROR);

                                                        rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                                                        VERIFY_NON_NULL(TAG, rsrc->types, ERROR);

                                                        CborValue resourceTypes;
                                                        cborFindResult = cbor_value_enter_container(&rMap, &resourceTypes);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering RT Array.");

                                                        for(size_t i = 0; cbor_value_is_valid(&resourceTypes) && cbor_value_is_text_string(&resourceTypes); i++)
                                                        {
                                                            cborFindResult = cbor_value_dup_text_string(&resourceTypes, &(rsrc->types[i]), &readLen, NULL);
                                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding resource type.");
                                                            cborFindResult = cbor_value_advance(&resourceTypes);
                                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing resource type.");
                                                        }
                                                    }

                                                    // "if"
                                                    if (0 == strcmp(OIC_JSON_IF_NAME, rMapName) && cbor_value_is_array(&rMap))
                                                    {
                                                        cbor_value_get_array_length(&rMap, &rsrc->interfaceLen);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF array length.");
                                                        VERIFY_SUCCESS(TAG, (0 != rsrc->interfaceLen), ERROR);

                                                        rsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char*));
                                                        VERIFY_NON_NULL(TAG, rsrc->interfaces, ERROR);

                                                        CborValue interfaces;
                                                        cborFindResult = cbor_value_enter_container(&rMap, &interfaces);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering IF Array.");

                                                        for(size_t i = 0; cbor_value_is_valid(&interfaces) && cbor_value_is_text_string(&interfaces); i++)
                                                        {
                                                            cborFindResult = cbor_value_dup_text_string(&interfaces, &(rsrc->interfaces[i]), &readLen, NULL);
                                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF type.");
                                                            cborFindResult = cbor_value_advance(&interfaces);
                                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing IF type.");
                                                        }
                                                    }

                                                    // "rel"
                                                    if (0 == strcmp(OIC_JSON_REL_NAME, rMapName))
                                                    {
                                                        cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->rel, &len, NULL);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding REL Value.");
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
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Resource Array.");
                                                }
                                            }
                                        }

                                        // Permissions -- Mandatory
                                        if (strcmp(name, OIC_JSON_PERMISSION_NAME) == 0)
                                        {
                                            uint64_t tmp64;
                                            cborFindResult = cbor_value_get_uint64(&aceMap, &tmp64);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a PERM Value.");
                                            ace->permission = (uint16_t)tmp64;
                                        }

                                        // TODO: Need to verfication for validity
                                        // Validity -- Not mandatory
                                        if(strcmp(name, OIC_JSON_VALIDITY_NAME) == 0)
                                        {
                                            CborValue validitiesMap = {.parser = NULL};
                                            size_t validitySize = 0;

                                            cborFindResult = cbor_value_get_array_length(&aceMap, &validitySize);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Validity Array Length.");

                                            cborFindResult = cbor_value_enter_container(&aceMap, &validitiesMap);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Array Map.");

                                            while(cbor_value_is_valid(&validitiesMap))
                                            {
                                                OicSecValidity_t* validity = (OicSecValidity_t*)OICCalloc(1, sizeof(OicSecValidity_t));
                                                VERIFY_NON_NULL(TAG, validity, ERROR);
                                                LL_APPEND(ace->validities, validity);

                                                CborValue validityMap  = {.parser = NULL};
                                                //period (string)
                                                cborFindResult = cbor_value_enter_container(&validitiesMap, &validityMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Map.");

                                                size_t len = 0;
                                                cborFindResult =cbor_value_dup_text_string(&validityMap, &validity->period, &len, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period value.");

                                                //recurrence (string array)
                                                CborValue recurrenceMap  = {.parser = NULL};
                                                cborFindResult = cbor_value_enter_container(&validityMap, &recurrenceMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence array.");

                                                cborFindResult = cbor_value_get_array_length(&recurrenceMap, &validity->recurrenceLen);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array.");

                                                validity->recurrences = (char**)OICCalloc(validity->recurrenceLen, sizeof(char*));
                                                VERIFY_NON_NULL(TAG, validity->recurrences, ERROR);

                                                for(size_t i = 0; cbor_value_is_text_string(&recurrenceMap) && i < validity->recurrenceLen; i++)
                                                {
                                                    cborFindResult = cbor_value_dup_text_string(&recurrenceMap, &validity->recurrences[i], &len, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence Value.");

                                                    cborFindResult = cbor_value_advance(&recurrenceMap);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a recurrences Array.");
                                                }

                                                cborFindResult = cbor_value_advance(&validitiesMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing a validities Array.");
                                            }
                                        }

#ifdef MULTIPLE_OWNER
                                        // eowner uuid -- Not Mandatory
                                        if (strcmp(name, OIC_JSON_EOWNERID_NAME)  == 0)
                                        {
                                            char *eowner = NULL;
                                            cborFindResult = cbor_value_dup_text_string(&aceMap, &eowner, &len, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding eownerId Value.");
                                            if(NULL == ace->eownerID)
                                            {
                                                ace->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
                                                VERIFY_NON_NULL(TAG, ace->eownerID, ERROR);
                                            }
                                            ret = ConvertStrToUuid(eowner, ace->eownerID);
                                            OICFree(eowner);
                                            VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
                                        }
#endif //MULTIPLE_OWNER
                                        OICFree(name);
                                    }

                                    if (type != CborMapType && cbor_value_is_valid(&aceMap))
                                    {
                                        cborFindResult = cbor_value_advance(&aceMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing the Array.");
                                    }
                                }

                                if (cbor_value_is_valid(&acesArray))
                                {
                                    cborFindResult = cbor_value_advance(&acesArray);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Array.");
                                }
                            }
                        }
                        OICFree(acName);
                    }

                    if (cbor_value_is_valid(&aclistMap))
                    {
                        cborFindResult = cbor_value_advance(&aclistMap);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACLIST Map.");
                    }
                }
            }

            //rownerID -- Mandatory
            if (strcmp(tagName, OIC_JSON_ROWNERID_NAME)  == 0)
            {
                char *stRowner = NULL;
                cborFindResult = cbor_value_dup_text_string(&aclMap, &stRowner, &len, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Rownerid Value.");
                ret = ConvertStrToUuid(stRowner, &acl->rownerID);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                OICFree(stRowner);
            }
            else if (NULL != gAcl)
            {
                memcpy(&(acl->rownerID), &(gAcl->rownerID), sizeof(OicUuid_t));
            }
            OICFree(tagName);
        }
        if (cbor_value_is_valid(&aclMap))
        {
            cborFindResult = cbor_value_advance(&aclMap);
            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Map.");
        }
    }

exit:
    if (cborFindResult != CborNoError)
    {
        OIC_LOG(ERROR, TAG, "Failed to CBORPayloadToAcl");
        DeleteACLList(acl);
        acl = NULL;
    }

    return acl;
}

#ifdef MULTIPLE_OWNER
bool IsValidAclAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, const size_t size)
{
    bool retValue = false;
    OicSecAcl_t* acl = NULL;

    VERIFY_NON_NULL(TAG, uuid, ERROR);
    VERIFY_NON_NULL(TAG, cborPayload, ERROR);
    VERIFY_SUCCESS(TAG, 0 != size, ERROR);

    acl = CBORPayloadToAcl(cborPayload, size);
    VERIFY_NON_NULL(TAG, acl, ERROR);

    OicSecAce_t* ace = NULL;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH_SAFE(acl->aces, ace, tempAce)
    {
        OicSecRsrc_t* rsrc = NULL;
        OicSecRsrc_t* tempRsrc = NULL;

        VERIFY_NON_NULL(TAG, ace->eownerID, ERROR);
        VERIFY_SUCCESS(TAG, memcmp(ace->eownerID->id, uuid->id, sizeof(uuid->id)) == 0, ERROR);

        LL_FOREACH_SAFE(ace->resources, rsrc, tempRsrc)
        {
            VERIFY_SUCCESS(TAG, strcmp(rsrc->href, OIC_RSRC_TYPE_SEC_DOXM) != 0, ERROR);
            VERIFY_SUCCESS(TAG, strcmp(rsrc->href, OIC_RSRC_TYPE_SEC_CRED) != 0, ERROR);
            VERIFY_SUCCESS(TAG, strcmp(rsrc->href, OIC_RSRC_TYPE_SEC_ACL) != 0, ERROR);
            VERIFY_SUCCESS(TAG, strcmp(rsrc->href, OIC_RSRC_TYPE_SEC_PSTAT) != 0, ERROR);
            VERIFY_SUCCESS(TAG, strcmp(rsrc->href, OIC_RSRC_TYPE_SEC_CRL) != 0, ERROR);
        }
    }

    retValue = true;

exit:
    DeleteACLList(acl);

    return retValue;
}
#endif //MULTIPLE_OWNER

/**
 * This method removes ACE for the subject and resource from the ACL
 *
 * @param subject of the ACE
 * @param resource of the ACE
 *
 * @return
 *     ::OC_STACK_RESOURCE_DELETED on success
 *     ::OC_STACK_NO_RESOURCE on failure to find the appropriate ACE
 *     ::OC_STACK_INVALID_PARAM on invalid parameter
 */
OCStackResult RemoveACE(const OicUuid_t * subject, const char * resource)
{
    OIC_LOG(DEBUG, TAG, "IN RemoveACE");

    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;
    bool deleteFlag = false;
    OCStackResult ret = OC_STACK_NO_RESOURCE;

    if (memcmp(subject->id, &WILDCARD_SUBJECT_ID, sizeof(subject->id)) == 0)
    {
        OIC_LOG_V(ERROR, TAG, "%s received invalid parameter", __func__ );
        return  OC_STACK_INVALID_PARAM;
    }

    //If resource is NULL then delete all the ACE for the subject.
    if (NULL == resource || resource[0] == '\0')
    {
        LL_FOREACH_SAFE(gAcl->aces, ace, tempAce)
        {
            if (memcmp(ace->subjectuuid.id, subject->id, sizeof(subject->id)) == 0)
            {
                LL_DELETE(gAcl->aces, ace);
                FreeACE(ace);
                deleteFlag = true;
            }
        }
    }
    else
    {
        //Looping through ACL to find the right ACE to delete. If the required resource is the only
        //resource in the ACE for the subject then delete the whole ACE. If there are more resources
        //than the required resource in the ACE, for the subject then just delete the resource from
        //the resource array
        LL_FOREACH_SAFE(gAcl->aces, ace, tempAce)
        {
            if (memcmp(ace->subjectuuid.id, subject->id, sizeof(subject->id)) == 0)
            {
                OicSecRsrc_t* rsrc = NULL;
                OicSecRsrc_t* tempRsrc = NULL;
                LL_FOREACH_SAFE(ace->resources, rsrc, tempRsrc)
                {
                    if(strcmp(rsrc->href, resource) == 0)
                    {
                        LL_DELETE(ace->resources, rsrc);
                        FreeRsrc(rsrc);
                        deleteFlag = true;
                    }
                }

                //If resource list is empty
                if(NULL == ace->resources && true == deleteFlag)
                {
                    //Remove the ACE from ACL
                    LL_DELETE(gAcl->aces, ace);
                    FreeACE(ace);
                }
            }
        }
    }

    if (deleteFlag)
    {
        // In case of unit test do not update persistant storage.
        if (memcmp(subject->id, &WILDCARD_SUBJECT_B64_ID, sizeof(subject->id)) == 0)
        {
            ret = OC_STACK_RESOURCE_DELETED;
        }
        else
        {
            uint8_t *payload = NULL;
            size_t size = 0;
            if (OC_STACK_OK == AclToCBORPayload(gAcl, &payload, &size))
            {
                if (OC_STACK_OK == UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size))
                {
                    ret = OC_STACK_RESOURCE_DELETED;
                }
                OICFree(payload);
            }
        }
    }
    return ret;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'subject' field.
 *
 * @param query querystring passed in REST request
 * @param subject subject UUID parsed from query string
 *
 * @return true if query parsed successfully and found 'subject', else false.
 */
static bool GetSubjectFromQueryString(const char *query, OicUuid_t *subject)
{
    OicParseQueryIter_t parseIter = { .attrPos = NULL };

    ParseQueryIterInit((unsigned char *) query, &parseIter);

    while (GetNextQuery (&parseIter))
    {
        if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_SUBJECTID_NAME, parseIter.attrLen) == 0)
        {
            char strUuid[STRING_UUID_SIZE] = {0};
            VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);
            memcpy(strUuid, parseIter.valPos, parseIter.valLen);
            OCStackResult res = ConvertStrToUuid(strUuid, subject);
            VERIFY_SUCCESS(TAG, OC_STACK_OK == res, ERROR);
            return true;
        }
    }

exit:
    return false;
}

/**
 * This method parses the query string received for REST requests and
 * retrieves the 'resource' field.
 *
 * @param query querystring passed in REST request
 * @param resource resource parsed from query string
 * @param resourceSize size of the memory pointed to resource
 *
 * @return true if query parsed successfully and found 'resource', else false.
 */
static bool GetResourceFromQueryString(const char *query, char *resource, size_t resourceSize)
{
    OicParseQueryIter_t parseIter = { .attrPos = NULL };

    ParseQueryIterInit((unsigned char *) query, &parseIter);

    while (GetNextQuery (&parseIter))
    {
        if (strncasecmp((char *) parseIter.attrPos, OIC_JSON_RESOURCES_NAME, parseIter.attrLen)
                == 0)
        {
            VERIFY_SUCCESS(TAG, 0 != parseIter.valLen, ERROR);
            OICStrcpy(resource, resourceSize, (char *) parseIter.valPos);

            return true;
        }
    }

exit:
   return false;
}

static size_t GetNumberOfResource(OicSecRsrc_t* resources)
{
    size_t ret = 0;
    if(resources)
    {
        OicSecRsrc_t* rsrc = NULL;
        LL_FOREACH(resources, rsrc)
        {
            ret++;
        }
    }
    return ret;
}

static size_t GetNumberOfValidity(OicSecValidity_t* val)
{
    size_t ret = 0;

    if(val)
    {
        OicSecValidity_t* temp = NULL;
        LL_FOREACH(val, temp)
        {
            ret++;
        }
    }
    return ret;
}


static bool IsSameStringArray(char** strArr1, size_t strArr1Len,
                              char** strArr2, size_t strArr2Len)
{

    if(NULL == strArr1 && NULL == strArr2)
    {
        return true;
    }

    if(strArr1 && strArr2 && NULL == *strArr1 && NULL == *strArr2)
    {
        return true;
    }

    if(strArr1 && strArr2)
    {
        if(*strArr1 && *strArr2 && strArr1Len == strArr2Len)
        {
            size_t matchedStr = 0;
            for(size_t i = 0; i < strArr1Len; i++)
            {
                for(size_t j = 0; j < strArr2Len; j++)
                {
                    if(strcmp(strArr1[i], strArr2[j]) == 0)
                    {
                        matchedStr++;
                    }
                }
            }
            if(matchedStr == strArr1Len)
            {
                return true;
            }
        }
    }

    return false;
}

static bool IsSameResources(OicSecRsrc_t* resources1, OicSecRsrc_t* resources2)
{
    size_t numOfRsrc1 = 0;
    size_t numOfRsrc2 = 0;
    size_t numOfMatchedRsrc = 0;
    OicSecRsrc_t* rsrc1 = NULL;
    OicSecRsrc_t* rsrc2 = NULL;

    if(NULL == resources1 && NULL == resources2)
    {
        return true;
    }

    if(resources1 && resources2)
    {
        numOfRsrc1 = GetNumberOfResource(resources1);
        numOfRsrc2 = GetNumberOfResource(resources2);

        if(0 == numOfRsrc1 && 0 == numOfRsrc2)
        {
            return true;
        }

        LL_FOREACH(resources1, rsrc1)
        {
            rsrc2 = NULL;
            LL_FOREACH(resources2, rsrc2)
            {
                if(rsrc1 && rsrc2)
                {
                    if(strcmp(rsrc1->href, rsrc2->href) == 0 &&
                        IsSameStringArray(rsrc1->interfaces, rsrc1->interfaceLen,
                                          rsrc2->interfaces, rsrc2->interfaceLen) &&
                        IsSameStringArray(rsrc1->types, rsrc1->typeLen,
                                          rsrc2->types, rsrc2->typeLen))
                    {
                        // TODO: Update codes to compare 'rel' property
                        numOfMatchedRsrc++;
                    }
                }
            }
        }

        if(numOfMatchedRsrc == numOfRsrc1)
        {
            return true;
        }
    }

    return false;
}

static bool IsSameValidities(OicSecValidity_t* validities1, OicSecValidity_t* validities2)
{
    size_t numOfVal1 = 0;
    size_t numOfVal2 = 0;
    size_t numOfMatchedVal = 0;
    OicSecValidity_t* val1 = NULL;
    OicSecValidity_t* val2 = NULL;

    if(NULL == validities1 && NULL == validities2)
    {
        return true;
    }

    if(validities1 && validities2)
    {
        numOfVal1 = GetNumberOfValidity(validities1);
        numOfVal2 = GetNumberOfValidity(validities2);
        if(0 == numOfVal1 && 0 == numOfVal2)
        {
            return true;
        }

        if(numOfVal1 == numOfVal2)
        {
            LL_FOREACH(validities1, val1)
            {
                LL_FOREACH(validities2, val2)
                {
                    if(strcmp(val1->period, val2->period) == 0 &&
                        IsSameStringArray(val1->recurrences, val1->recurrenceLen,
                                          val2->recurrences, val2->recurrenceLen))
                    {
                        numOfMatchedVal++;
                    }
                }
            }
            if(numOfVal1 == numOfMatchedVal)
            {
                return true;
            }
        }
    }

    return false;
}

#ifdef MULTIPLE_OWNER
static bool IsSameEowner(OicUuid_t* eowner1, OicUuid_t* eowner2)
{
    if (NULL != eowner1 && NULL != eowner2)
    {
        if (memcmp(eowner1->id, eowner2->id, sizeof(eowner1->id)) == 0)
        {
            return true;
        }
    }
    else if (NULL == eowner1 && NULL == eowner2)
    {
        OIC_LOG(DEBUG, TAG, "Both eowner1 and eowner2 are NULL");
        return true;
    }

    return false;
}
#endif

static bool IsSameACE(OicSecAce_t* ace1, OicSecAce_t* ace2)
{
    if(ace1 && ace2)
    {
        if(memcmp(ace1->subjectuuid.id, ace2->subjectuuid.id, sizeof(ace1->subjectuuid.id)) != 0)
        {
            return false;
        }

        if(false == IsSameResources(ace1->resources, ace2->resources))
        {
            return false;
        }

        if(ace1->permission != ace2->permission)
        {
            return false;
        }

        if(false == IsSameValidities(ace1->validities, ace2->validities))
        {
            return false;
        }

#ifdef MULTIPLE_OWNER
        if(false == IsSameEowner(ace1->eownerID, ace2->eownerID))
        {
            return false;
        }
#endif

        return true;
    }

    return false;
}

/**
 * Internal function to remove all ACL data on ACL resource and persistent storage
 *
 * @retval
 *     OC_STACK_RESOURCE_DELETED  - no errors
 *     Otherwise                  - error
 */
static OCStackResult RemoveAllAce(void)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t* aclBackup = NULL;
    size_t backupSize = 0;
    uint8_t* payload = NULL;
    size_t size = 0;
    OicSecAce_t* aceItem = NULL;
    OicSecAce_t* tempAce = NULL;

    OIC_LOG(INFO, TAG, "IN RemoveAllAce");

    //Backup the current ACL
    ret = AclToCBORPayload(gAcl, &aclBackup, &backupSize);
    if(OC_STACK_OK == ret)
    {
        // Remove all ACE from ACL
        LL_FOREACH_SAFE(gAcl->aces, aceItem, tempAce)
        {
            LL_DELETE(gAcl->aces, aceItem);
            FreeACE(aceItem);
        }

        //Generate empty ACL payload
        ret = AclToCBORPayload(gAcl, &payload, &size);
        if (OC_STACK_OK == ret )
        {
            //Update the PS.
            ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size);
            if (OC_STACK_OK != ret)
            {
                OIC_LOG_V(ERROR, TAG, "Error in UpdateSecureResourceInPS : %d", ret);
            }
        }

        if(OC_STACK_OK != ret)
        {
            OIC_LOG_V(ERROR, TAG, "Error while DELETE ACE : %d", ret);

            //If some erorr is occured, revert back.
            OicSecAcl_t* originAcl = CBORPayloadToAcl(aclBackup, backupSize);
            if( originAcl )
            {
                ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, aclBackup, backupSize);
                if (OC_STACK_OK == ret)
                {
                    DeleteACLList(gAcl);
                    gAcl = originAcl;
                }
                else
                {
                    OIC_LOG_V(ERROR, TAG, "Error in UpdateSecureResourceInPS : %d", ret);
                }
            }
            else
            {
                OIC_LOG(FATAL, TAG, "Error in CBORPayloadToAcl");
                ret = OC_STACK_ERROR;
            }
        }
    }

    OICFree(aclBackup);
    OICFree(payload);

    OIC_LOG(INFO, TAG, "OUT RemoveAllAce");

    return (OC_STACK_OK == ret ? OC_STACK_RESOURCE_DELETED : ret);
}

static OCEntityHandlerResult HandleACLGetRequest(const OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(INFO, TAG, "HandleACLGetRequest processing the request");
    uint8_t* payload = NULL;
    size_t size = 0;
    OCEntityHandlerResult ehRet;

    OicUuid_t subject = {.id= { 0 } };

    // In case, 'subject' field is included in REST request.
    if (ehRequest->query && GetSubjectFromQueryString(ehRequest->query, &subject))
    {
        OIC_LOG(DEBUG,TAG,"'subject' field is inculded in REST request.");
        OIC_LOG(DEBUG, TAG, "HandleACLGetRequest processing query");

        char resource[MAX_URI_LENGTH] = { 0 };

        OicSecAce_t *savePtr = NULL;
        const OicSecAce_t *currentAce = NULL;
        OicSecAcl_t targetAcl;

        memcpy(&targetAcl.rownerID, &gAcl->rownerID, sizeof(OicUuid_t));
        targetAcl.aces = NULL;

        // 'Subject' field is MUST for processing a querystring in REST request.
        GetResourceFromQueryString(ehRequest->query, resource, sizeof(resource));

        /*
         * TODO : Currently, this code only provides one ACE for a Subject.
         * Below code needs to be updated for scenarios when Subject have
         * multiple ACE's in ACL resource.
         */
        while ((currentAce = GetACLResourceData(&subject, &savePtr)))
        {
            targetAcl.aces = (OicSecAce_t*)currentAce;

            /*
             * If REST querystring contains a specific resource, we need
             * to search for that resource in ACE.
             */
            if (resource[0] != '\0')
            {
                OicSecRsrc_t *rsrc = NULL;
                LL_FOREACH(currentAce->resources, rsrc)
                {
                    if(0 == strcmp(rsrc->href, resource) ||
                        0 == strcmp(WILDCARD_RESOURCE_URI, rsrc->href))
                    {
                        // Convert ACL data into CBOR format for transmission
                        if (OC_STACK_OK != AclToCBORPayload(&targetAcl, &payload, &size))
                        {
                            ehRet = OC_EH_ERROR;
                        }
                        goto exit;
                    }
                }
            }
            else
            {
                // Convert ACL data into CBOR format for transmission
                if (OC_STACK_OK != AclToCBORPayload(&targetAcl, &payload, &size))
                {
                    ehRet = OC_EH_ERROR;
                }
                goto exit;
            }
        }
    }
    // In case, 'subject' field is not included in REST request.
    else
    {
        OIC_LOG(DEBUG,TAG,"'subject' field is not inculded in REST request.");
        // Convert ACL data into CBOR format for transmission.
        if (OC_STACK_OK != AclToCBORPayload(gAcl, &payload, &size))
        {
            ehRet = OC_EH_ERROR;
        }
    }
exit:
    // A device should always have a default acl. Therefore, payload should never be NULL.
    ehRet = (payload ? OC_EH_OK : OC_EH_ERROR);
    OIC_LOG(DEBUG, TAG, "ACL payload with GET response");
    OIC_LOG_BUFFER(DEBUG, TAG, payload, size);

    //Send payload to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OICFree(payload);

    OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__, ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleACLPostRequest(const OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(INFO, TAG, "HandleACLPostRequest processing the request");
    OCEntityHandlerResult ehRet = OC_EH_OK;

    // Convert CBOR into ACL data and update to SVR buffers. This will also validate the ACL data received.
    uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;
    if (payload)
    {
        OicSecAcl_t *newAcl = NULL;
        OIC_LOG(DEBUG, TAG, "ACL payload from POST request << ");
        OIC_LOG_BUFFER(DEBUG, TAG, payload, size);

        newAcl = CBORPayloadToAcl(payload, size);
        if (newAcl)
        {
            bool isNewAce = true;
            OicSecAce_t* existAce = NULL;
            OicSecAce_t* newAce = NULL;
            OicSecAce_t* tempAce1 = NULL;
            OicSecAce_t* tempAce2 = NULL;

            LL_FOREACH_SAFE(newAcl->aces, newAce, tempAce1)
            {
                isNewAce = true;
                LL_FOREACH_SAFE(gAcl->aces, existAce, tempAce2)
                {
                    if(IsSameACE(newAce, existAce))
                    {
                        isNewAce = false;
                    }
                }
                if(isNewAce)
                {
                    OIC_LOG(DEBUG, TAG, "NEW ACE dectected.");

                    OicSecAce_t* insertAce = DuplicateACE(newAce);
                    if(insertAce)
                    {
                        OIC_LOG(DEBUG, TAG, "Appending new ACE..");
                        LL_PREPEND(gAcl->aces, insertAce);
                    }
                    else
                    {
                        OIC_LOG(ERROR, TAG, "Failed to duplicate ACE.");
                        ehRet = OC_EH_ERROR;
                    }
                }
            }
            memcpy(&(gAcl->rownerID), &(newAcl->rownerID), sizeof(OicUuid_t));

            DeleteACLList(newAcl);

            if(OC_EH_OK == ehRet)
            {
                size_t size = 0;
                uint8_t *cborPayload = NULL;

                if (OC_STACK_OK == AclToCBORPayload(gAcl, &cborPayload, &size))
                {
                    if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, cborPayload, size) == OC_STACK_OK)
                    {
                        ehRet = OC_EH_CHANGED;
                    }
                    OICFree(cborPayload);
                }

                if(OC_EH_CHANGED != ehRet)
                {
                    ehRet = OC_EH_ERROR;
                }
            }
        }
    }

    //Send response to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    OIC_LOG_V(DEBUG, TAG, "%s RetVal %d", __func__, ehRet);
    return ehRet;
}

static OCEntityHandlerResult HandleACLDeleteRequest(const OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(DEBUG, TAG, "Processing ACLDeleteRequest");
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    OicUuid_t subject = { .id= { 0 } };
    char resource[MAX_URI_LENGTH] = { 0 };

    VERIFY_NON_NULL(TAG, ehRequest->query, ERROR);

    // If 'Subject' field exist, processing a querystring in REST request.
    if(GetSubjectFromQueryString(ehRequest->query, &subject))
    {
        GetResourceFromQueryString(ehRequest->query, resource, sizeof(resource));

        if (OC_STACK_RESOURCE_DELETED == RemoveACE(&subject, resource))
        {
            ehRet = OC_EH_RESOURCE_DELETED;
        }
    }
    // If 'subject field not exist, remove all ACL data from ACL resource
    else
    {
        OIC_LOG(WARNING, TAG, "Can not find the 'subject' in querystring, All ACL list will be removed.");

        if(OC_STACK_RESOURCE_DELETED == RemoveAllAce())
        {
            ehRet = OC_EH_RESOURCE_DELETED;
        }
    }

exit:
    //Send response to request originator
    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                   OC_EH_OK : OC_EH_ERROR;

    return ehRet;
}

OCEntityHandlerResult ACLEntityHandler(OCEntityHandlerFlag flag, OCEntityHandlerRequest * ehRequest,
        void* callbackParameter)
{
    OIC_LOG(DEBUG, TAG, "Received request ACLEntityHandler");
    (void)callbackParameter;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    if (!ehRequest)
    {
        return ehRet;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        // TODO :  Handle PUT method
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
            case OC_REST_GET:
                ehRet = HandleACLGetRequest(ehRequest);
                break;

            case OC_REST_POST:
                ehRet = HandleACLPostRequest(ehRequest);
                break;

            case OC_REST_DELETE:
                ehRet = HandleACLDeleteRequest(ehRequest);
                break;

            default:
                ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ?
                               OC_EH_OK : OC_EH_ERROR;
        }
    }

    return ehRet;
}

/**
 * This internal method is used to create '/oic/sec/acl' resource.
 */
static OCStackResult CreateACLResource()
{
    OCStackResult ret;

    ret = OCCreateResource(&gAclHandle,
                           OIC_RSRC_TYPE_SEC_ACL,
                           OC_RSRVD_INTERFACE_DEFAULT,
                           OIC_RSRC_ACL_URI,
                           ACLEntityHandler,
                           NULL,
                           OC_SECURE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate ACL resource");
        DeInitACLResource();
    }
    return ret;
}

// This function sets the default ACL and is defined for the unit test only.
OCStackResult SetDefaultACL(OicSecAcl_t *acl)
{
    gAcl = acl;
    return OC_STACK_OK;
}

OCStackResult GetDefaultACL(OicSecAcl_t** defaultAcl)
{
    OCStackResult ret = OC_STACK_ERROR;
    OicUuid_t ownerId = { .id = { 0 } };
    OicSecAcl_t *acl = NULL;
    OicSecAce_t *ace = NULL;
    OicSecRsrc_t* resRsrc = NULL;
    OicSecRsrc_t* deviceRsrc = NULL;
    OicSecRsrc_t* platformRsrc = NULL;
    OicSecRsrc_t* aclRsrc = NULL;
    OicSecRsrc_t* doxmRsrc = NULL;
    OicSecRsrc_t* pstatRsrc = NULL;

    /*
     * TODO In future, when new virtual resources will be added in OIC
     * specification, Iotivity stack should be able to add them in
     * existing SVR database. To support this, we need to add 'versioning'
     * mechanism in SVR database.
     */

    if (!defaultAcl)
    {
        return OC_STACK_INVALID_PARAM;
    }

    acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    VERIFY_NON_NULL(TAG, acl, ERROR);

    ace = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NON_NULL(TAG, ace, ERROR);

    LL_APPEND(acl->aces, ace);

    // Subject -- Mandatory
    memcpy(ace->subjectuuid.id, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t));

    // Resources -- Mandatory
    // /oic/res
    resRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, resRsrc, ERROR);
    LL_APPEND(ace->resources, resRsrc);
    resRsrc->href = OICStrdup(OC_RSRVD_WELL_KNOWN_URI);
    VERIFY_NON_NULL(TAG, (resRsrc->href), ERROR);
    resRsrc->typeLen = 1;
    resRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, resRsrc->types, ERROR);
    resRsrc->types[0] = OICStrdup(OC_RSRVD_RESOURCE_TYPE_RES);
    VERIFY_NON_NULL(TAG, resRsrc->types[0], ERROR);
    resRsrc->interfaceLen = 2;
    resRsrc->interfaces = (char**)OICCalloc(resRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, resRsrc->interfaces, ERROR);
    resRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, resRsrc->interfaces[0], ERROR);
    resRsrc->interfaces[1] = OICStrdup(OC_RSRVD_INTERFACE_READ);
    VERIFY_NON_NULL(TAG, resRsrc->interfaces[1], ERROR);

    // /oic/d
    deviceRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, deviceRsrc, ERROR);
    LL_APPEND(ace->resources, deviceRsrc);
    deviceRsrc->href = OICStrdup(OC_RSRVD_DEVICE_URI);
    VERIFY_NON_NULL(TAG, (deviceRsrc->href), ERROR);
    deviceRsrc->typeLen = 1;
    deviceRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, deviceRsrc->types, ERROR);
    deviceRsrc->types[0] = OICStrdup(OC_RSRVD_RESOURCE_TYPE_DEVICE);
    VERIFY_NON_NULL(TAG, deviceRsrc->types[0], ERROR);
    deviceRsrc->interfaceLen = 2;
    deviceRsrc->interfaces = (char**)OICCalloc(deviceRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, deviceRsrc->interfaces, ERROR);
    deviceRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, deviceRsrc->interfaces[0], ERROR);
    deviceRsrc->interfaces[1] = OICStrdup(OC_RSRVD_INTERFACE_READ);
    VERIFY_NON_NULL(TAG, deviceRsrc->interfaces[1], ERROR);

    // /oic/p
    platformRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, platformRsrc, ERROR);
    LL_APPEND(ace->resources, platformRsrc);
    platformRsrc->href = OICStrdup(OC_RSRVD_PLATFORM_URI);
    VERIFY_NON_NULL(TAG, (platformRsrc->href), ERROR);
    platformRsrc->typeLen = 1;
    platformRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, platformRsrc->types, ERROR);
    platformRsrc->types[0] = OICStrdup(OC_RSRVD_RESOURCE_TYPE_PLATFORM);
    VERIFY_NON_NULL(TAG, platformRsrc->types[0], ERROR);
    platformRsrc->interfaceLen = 2;
    platformRsrc->interfaces = (char**)OICCalloc(platformRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, platformRsrc->interfaces, ERROR);
    platformRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, platformRsrc->interfaces[0], ERROR);
    platformRsrc->interfaces[1] = OICStrdup(OC_RSRVD_INTERFACE_READ);
    VERIFY_NON_NULL(TAG, platformRsrc->interfaces[1], ERROR);

    // /oic/sec/acl
    aclRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, aclRsrc, ERROR);
    LL_APPEND(ace->resources, aclRsrc);
    aclRsrc->href = OICStrdup(OIC_RSRC_ACL_URI);
    VERIFY_NON_NULL(TAG, (aclRsrc->href), ERROR);
    aclRsrc->typeLen = 1;
    aclRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, aclRsrc->types, ERROR);
    aclRsrc->types[0] = OICStrdup(OIC_RSRC_TYPE_SEC_ACL);
    VERIFY_NON_NULL(TAG, aclRsrc->types[0], ERROR);
    aclRsrc->interfaceLen = 1;
    aclRsrc->interfaces = (char**)OICCalloc(aclRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, aclRsrc->interfaces, ERROR);
    aclRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, aclRsrc->interfaces[0], ERROR);

    // /oic/sec/doxm
    doxmRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, doxmRsrc, ERROR);
    LL_APPEND(ace->resources, doxmRsrc);
    doxmRsrc->href = OICStrdup(OIC_RSRC_DOXM_URI);
    VERIFY_NON_NULL(TAG, (doxmRsrc->href), ERROR);
    doxmRsrc->typeLen = 1;
    doxmRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, doxmRsrc->types, ERROR);
    doxmRsrc->types[0] = OICStrdup(OIC_RSRC_TYPE_SEC_DOXM);
    VERIFY_NON_NULL(TAG, doxmRsrc->types[0], ERROR);
    doxmRsrc->interfaceLen = 1;
    doxmRsrc->interfaces = (char**)OICCalloc(doxmRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, doxmRsrc->interfaces, ERROR);
    doxmRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, doxmRsrc->interfaces[0], ERROR);

    // /oic/sec/pstat
    pstatRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, pstatRsrc, ERROR);
    LL_APPEND(ace->resources, pstatRsrc);
    pstatRsrc->href = OICStrdup(OIC_RSRC_PSTAT_URI);
    VERIFY_NON_NULL(TAG, (pstatRsrc->href), ERROR);
    pstatRsrc->typeLen = 1;
    pstatRsrc->types = (char**)OICCalloc(1, sizeof(char*));
    VERIFY_NON_NULL(TAG, pstatRsrc->types, ERROR);
    pstatRsrc->types[0] = OICStrdup(OIC_RSRC_TYPE_SEC_PSTAT);
    VERIFY_NON_NULL(TAG, pstatRsrc->types[0], ERROR);
    pstatRsrc->interfaceLen = 1;
    pstatRsrc->interfaces = (char**)OICCalloc(pstatRsrc->interfaceLen, sizeof(char*));
    VERIFY_NON_NULL(TAG, pstatRsrc->interfaces, ERROR);
    pstatRsrc->interfaces[0] = OICStrdup(OC_RSRVD_INTERFACE_DEFAULT);
    VERIFY_NON_NULL(TAG, pstatRsrc->interfaces[0], ERROR);

    ace->permission = PERMISSION_READ;
    ace->validities = NULL;

    // Device ID is the owner of this default ACL
    if (GetDoxmResourceData() != NULL)
    {
        ret = GetDoxmDeviceID(&ownerId);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, FATAL);
    }
    else
    {
        OCRandomUuidResult rdm = OCGenerateUuid(ownerId.id);
        VERIFY_SUCCESS(TAG, RAND_UUID_OK == rdm, FATAL);
    }

    memcpy(&acl->rownerID, &ownerId, sizeof(OicUuid_t));

    *defaultAcl = acl;
    ret = OC_STACK_OK;

exit:

    if (ret != OC_STACK_OK)
    {
        DeleteACLList(acl);
        acl = NULL;
    }

    return ret;
}

OCStackResult InitACLResource()
{
    OCStackResult ret = OC_STACK_ERROR;

    uint8_t *data = NULL;
    size_t size = 0;
    ret = GetSecureVirtualDatabaseFromPS(OIC_JSON_ACL_NAME, &data, &size);
    // If database read failed
    if (OC_STACK_OK != ret)
    {
        OIC_LOG(DEBUG, TAG, "ReadSVDataFromPS failed");
    }
    if (data)
    {
        // Read ACL resource from PS
        gAcl = CBORPayloadToAcl(data, size);
        OICFree(data);
    }
    /*
     * If SVR database in persistent storage got corrupted or
     * is not available for some reason, a default ACL is created
     * which allows user to initiate ACL provisioning again.
     */
    if (!gAcl)
    {
        ret = GetDefaultACL(&gAcl);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "Failed to create default ACL");
        }
        // TODO Needs to update persistent storage
    }
    VERIFY_NON_NULL(TAG, gAcl, FATAL);

    // Instantiate 'oic.sec.acl'
    ret = CreateACLResource();

exit:
    if (OC_STACK_OK != ret)
    {
        DeInitACLResource();
    }
    return ret;
}

OCStackResult DeInitACLResource()
{
    OCStackResult ret =  OCDeleteResource(gAclHandle);
    gAclHandle = NULL;

    if (gAcl)
    {
        DeleteACLList(gAcl);
        gAcl = NULL;
    }
    return ret;
}

const OicSecAce_t* GetACLResourceData(const OicUuid_t* subjectId, OicSecAce_t **savePtr)
{
    OicSecAce_t *ace = NULL;
    OicSecAce_t *begin = NULL;

    if (NULL == subjectId || NULL == savePtr || NULL == gAcl)
    {
        return NULL;
    }

    /*
     * savePtr MUST point to NULL if this is the 'first' call to retrieve ACL for
     * subjectID.
     */
    if (NULL == *savePtr)
    {
        begin = gAcl->aces;
    }
    else
    {
        /*
         * If this is a 'successive' call, search for location pointed by
         * savePtr and assign 'begin' to the next ACL after it in the linked
         * list and start searching from there.
         */
        LL_FOREACH(gAcl->aces, ace)
        {
            if (ace == *savePtr)
            {
                begin = ace->next;
            }
        }
    }

    // Find the next ACL corresponding to the 'subjectID' and return it.
    LL_FOREACH(begin, ace)
    {
        if (memcmp(&(ace->subjectuuid), subjectId, sizeof(OicUuid_t)) == 0)
        {
            *savePtr = ace;
            return ace;
        }
    }

    // Cleanup in case no ACL is found
    *savePtr = NULL;
    return NULL;
}

void printACL(const OicSecAcl_t* acl)
{
    OIC_LOG(INFO, TAG, "Print ACL:");

    if (NULL == acl)
    {
        OIC_LOG(INFO, TAG, "Received NULL acl");
        return;
    }

    char *rowner = NULL;
    if (OC_STACK_OK == ConvertUuidToStr(&acl->rownerID, &rowner))
    {
        OIC_LOG_V(INFO, TAG, "rowner id = %s", rowner);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Can't convert rowner uuid to string");
    }
    OICFree(rowner);

    const OicSecAce_t *ace = acl->aces;
    int ace_count = 0;
    while (ace)
    {
        ace_count++;
        OIC_LOG_V(INFO, TAG, "Print ace[%d]:", ace_count);

        OIC_LOG_V(INFO, TAG, "ace permission = %d", ace->permission);

        char *subjectuuid = NULL;
        if (OC_STACK_OK == ConvertUuidToStr(&ace->subjectuuid, &subjectuuid))
        {
            OIC_LOG_V(INFO, TAG, "ace subject uuid = %s", subjectuuid);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "Can't convert subjectuuid to string");
        }
        OICFree(subjectuuid);

        OicSecRsrc_t *res = ace->resources;
        int res_count = 0;
        while (res)
        {
            res_count++;
            OIC_LOG_V(INFO, TAG, "Print resources[%d]:", res_count);

            OIC_LOG_V(INFO, TAG, "href = %s", res->href);

            for (size_t i = 0; i < res->typeLen; i++)
            {
                OIC_LOG_V(INFO, TAG, "if[%zu] = %s", i, res->types[i]);
            }
            for (size_t i = 0; i < res->interfaceLen; i++)
            {
                OIC_LOG_V(INFO, TAG, "if[%zu] = %s", i, res->interfaces[i]);
            }

            res= res->next;
        }

        OicSecValidity_t *vals = ace->validities;
        int vals_count = 0;
        while (vals)
        {
            vals_count++;
            OIC_LOG_V(INFO, TAG, "Print validities[%d]:", vals_count);

            OIC_LOG_V(INFO, TAG, "period = %s", vals->period);
            for (size_t i = 0; i < vals->recurrenceLen; i++)
            {
                OIC_LOG_V(INFO, TAG, "recurrences[%zu] = %s", i, vals->recurrences[i]);
            }
        }

        ace = ace->next;
    }
}

OCStackResult AppendACL2(const OicSecAcl_t* acl)
{
    OCStackResult ret = OC_STACK_ERROR;

    if (!acl)
    {
        return OC_STACK_INVALID_PARAM;
    }

    // Append the new ACE to existing ACE list
    // Can't use LL_APPEND because it sets ace->next to NULL
    OicSecAce_t* ace = gAcl->aces;
    if (ace)
    {
        while (ace->next)
        {
            ace = ace->next;
        }
        ace->next = acl->aces;
    }
    else
    {
        gAcl->aces = acl->aces;
    }

    printACL(gAcl);

    size_t size = 0;
    uint8_t *payload = NULL;
    ret = AclToCBORPayload(gAcl, &payload, &size);
    if (OC_STACK_OK == ret)
    {
        ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size);
        OICFree(payload);
    }

    return ret;
}

OCStackResult AppendACL(const uint8_t *cborPayload, const size_t size)
{
    // Convert CBOR format to ACL data. This will also validate the ACL data received.
    OicSecAcl_t* newAcl = CBORPayloadToAcl(cborPayload, size);

    return AppendACL2(newAcl);
}

OCStackResult InstallACL(const OicSecAcl_t* acl)
{
    OCStackResult ret = OC_STACK_ERROR;

    if (!acl)
    {
        return OC_STACK_INVALID_PARAM;
    }

    bool isNewAce = true;
    OicSecAce_t* existAce = NULL;
    OicSecAce_t* newAce = NULL;
    OicSecAce_t* tempAce1 = NULL;
    OicSecAce_t* tempAce2 = NULL;
    OicSecAcl_t* newInstallAcl = NULL;

    LL_FOREACH_SAFE(acl->aces, newAce, tempAce1)
    {
        isNewAce = true;
        LL_FOREACH_SAFE(gAcl->aces, existAce, tempAce2)
        {
            if(IsSameACE(newAce, existAce))
            {
                OIC_LOG(DEBUG, TAG, "Duplicated ACE dectected.");
                ret = OC_STACK_DUPLICATE_REQUEST;
                isNewAce = false;
            }
        }
        if(isNewAce)
        {
            // Append new ACE to existing ACL
            OIC_LOG(DEBUG, TAG, "NEW ACE dectected.");

            OicSecAce_t* insertAce = DuplicateACE(newAce);
            if(insertAce)
            {
                OIC_LOG(DEBUG, TAG, "Appending new ACE..");

                if (!newInstallAcl)
                {
                    newInstallAcl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
                    if (NULL == newInstallAcl)
                    {
                        OIC_LOG(ERROR, TAG, "Failed to acllocate ACL");
                        return OC_STACK_NO_MEMORY;
                    }
                }
                LL_PREPEND(newInstallAcl->aces, insertAce);
            }
            else
            {
                OIC_LOG(ERROR, TAG, "Failed to duplicate ACE");
                DeleteACLList(newInstallAcl);
                return OC_STACK_ERROR;
            }
        }
    }

    if (newInstallAcl)
    {
        ret = AppendACL2(newInstallAcl);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "Failed to append ACL");
        }
        OICFree(newInstallAcl);
    }

    return ret;
}

/**
 * This function generates default ACE for security resource in case of owned status.
 *
 * @return Default ACE for security resource.
 */
static OicSecAce_t* GetSecDefaultACE()
{
    const int NUM_OF_DOXM_RT = 1;
    const int NUM_OF_DOXM_IF  = 1;
    const int NUM_OF_PSTAT_RT = 1;
    const int NUM_OF_PSTAT_IF = 1;
    const char *doxmRt[] = { OIC_RSRC_TYPE_SEC_DOXM };
    const char *pstatRt[] = { OIC_RSRC_TYPE_SEC_PSTAT };
    const char *doxmIf[] = { OC_RSRVD_INTERFACE_DEFAULT };
    const char *pstatIf[] = { OC_RSRVD_INTERFACE_DEFAULT };
    OicSecRsrc_t* doxmRsrc = NULL;
    OicSecRsrc_t* pstatRsrc = NULL;

    //Generate default ACE
    OicSecAce_t* newAce = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NON_NULL(TAG, newAce, ERROR);

    // Subject -- Mandatory
    memcpy(newAce->subjectuuid.id, &WILDCARD_SUBJECT_ID, WILDCARD_SUBJECT_ID_LEN);

    //Resources -- Mandatory
    //Add doxm
    doxmRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, doxmRsrc, ERROR);
    LL_APPEND(newAce->resources, doxmRsrc);
    // pstat-href
    doxmRsrc->href = OICStrdup(OIC_RSRC_DOXM_URI);
    VERIFY_NON_NULL(TAG, (doxmRsrc->href), ERROR);
    // pstat-rt
    doxmRsrc->typeLen = NUM_OF_DOXM_RT;
    doxmRsrc->types = (char**)OICCalloc(NUM_OF_DOXM_RT, sizeof(char*));
    VERIFY_NON_NULL(TAG, (doxmRsrc->types), ERROR);
    for(int i = 0; i < NUM_OF_DOXM_RT; i++)
    {
        doxmRsrc->types[i] = OICStrdup(doxmRt[i]);
        VERIFY_NON_NULL(TAG, (doxmRsrc->types[i]), ERROR);
    }
    // pstat-if
    doxmRsrc->interfaceLen = NUM_OF_DOXM_IF;
    doxmRsrc->interfaces = (char**)OICCalloc(NUM_OF_DOXM_IF, sizeof(char*));
    VERIFY_NON_NULL(TAG, (doxmRsrc->interfaces), ERROR);
    for(int i = 0; i < NUM_OF_DOXM_IF; i++)
    {
        doxmRsrc->interfaces[i] = OICStrdup(doxmIf[i]);
        VERIFY_NON_NULL(TAG, (doxmRsrc->interfaces[i]), ERROR);
    }

    //Add pstat
    pstatRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NON_NULL(TAG, pstatRsrc, ERROR);
    LL_APPEND(newAce->resources, pstatRsrc);
    //pstat-href
    pstatRsrc->href = OICStrdup(OIC_RSRC_PSTAT_URI);
    VERIFY_NON_NULL(TAG, (pstatRsrc->href), ERROR);
    //pstat-rt
    pstatRsrc->typeLen = NUM_OF_PSTAT_RT;
    pstatRsrc->types = (char**)OICCalloc(NUM_OF_PSTAT_RT, sizeof(char*));
    VERIFY_NON_NULL(TAG, (pstatRsrc->types), ERROR);
    for(int i = 0; i < NUM_OF_PSTAT_RT; i++)
    {
        pstatRsrc->types[i] = OICStrdup(pstatRt[i]);
        VERIFY_NON_NULL(TAG, (pstatRsrc->types[i]), ERROR);
    }
    // pstat-if
    pstatRsrc->interfaceLen = NUM_OF_PSTAT_IF;
    pstatRsrc->interfaces = (char**)OICCalloc(NUM_OF_PSTAT_IF, sizeof(char*));
    VERIFY_NON_NULL(TAG, (pstatRsrc->interfaces), ERROR);
    for(int i = 0; i < NUM_OF_PSTAT_IF; i++)
    {
        pstatRsrc->interfaces[i] = OICStrdup(pstatIf[i]);
        VERIFY_NON_NULL(TAG, (pstatRsrc->interfaces[i]), ERROR);
    }

    // Permissions -- Mandatory
    newAce->permission = PERMISSION_READ;

    //Period -- Not Mandatory
    newAce->validities = NULL;

    return newAce;
exit:
    FreeACE(newAce);
    return NULL;

}

OCStackResult UpdateDefaultSecProvACE()
{
    OCStackResult ret = OC_STACK_OK;
    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;

    if(gAcl)
    {
        int matchedRsrc = 0;
        bool isRemoved = false;

        LL_FOREACH_SAFE(gAcl->aces, ace, tempAce)
        {
            //Find default security resource ACL
            if(memcmp(&ace->subjectuuid, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0 &&
                ((PERMISSION_READ | PERMISSION_WRITE) == ace->permission))
            {
                matchedRsrc = 0;

                OicSecRsrc_t* rsrc = NULL;
                LL_FOREACH(ace->resources, rsrc)
                {
                    if(strncmp(rsrc->href, OIC_RSRC_DOXM_URI,
                               strlen(OIC_RSRC_DOXM_URI) + 1) == 0 ||
                       strncmp(rsrc->href, OIC_RSRC_CRED_URI,
                               strlen(OIC_RSRC_CRED_URI) + 1) == 0 ||
                       strncmp(rsrc->href, OIC_RSRC_PSTAT_URI,
                               strlen(OIC_RSRC_PSTAT_URI) + 1) == 0)
                    {
                        matchedRsrc++;
                    }
                }

                //If default security resource ACL is detected, delete it.
                if(NUMBER_OF_SEC_PROV_RSCS == matchedRsrc)
                {
                    LL_DELETE(gAcl->aces, ace);
                    FreeACE(ace);
                    isRemoved = true;
                }
            }
        }

        if(isRemoved)
        {
            /*
             * Generate new security resource ACE as follows :
             *      subject : "*"
             *      resources :  '/oic/sec/doxm', '/oic/sec/pstat'
             *      permission : READ
             */
            OicSecAce_t *secDefaultAce = GetSecDefaultACE();
            if (secDefaultAce)
            {
                LL_APPEND(gAcl->aces, secDefaultAce);

                size_t size = 0;
                uint8_t *payload = NULL;
                if (OC_STACK_OK == AclToCBORPayload(gAcl, &payload, &size))
                {
                    if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size) == OC_STACK_OK)
                    {
                        ret = OC_STACK_OK;
                    }
                    OICFree(payload);
                }
            }
        }
    }

    return ret;
}

OCStackResult SetAclRownerId(const OicUuid_t* newROwner)
{
    OCStackResult ret = OC_STACK_ERROR;
    uint8_t *cborPayload = NULL;
    size_t size = 0;
    OicUuid_t prevId = {.id={0}};

    if(NULL == newROwner)
    {
        ret = OC_STACK_INVALID_PARAM;
    }
    if(NULL == gAcl)
    {
        ret = OC_STACK_NO_RESOURCE;
    }

    if(newROwner && gAcl)
    {
        memcpy(prevId.id, gAcl->rownerID.id, sizeof(prevId.id));
        memcpy(gAcl->rownerID.id, newROwner->id, sizeof(newROwner->id));

        ret = AclToCBORPayload(gAcl, &cborPayload, &size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, cborPayload, size);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, ERROR);

        OICFree(cborPayload);
    }

    return ret;

exit:
    OICFree(cborPayload);
    memcpy(gAcl->rownerID.id, prevId.id, sizeof(prevId.id));
    return ret;
}

OCStackResult GetAclRownerId(OicUuid_t *rowneruuid)
{
    OCStackResult retVal = OC_STACK_ERROR;
    if (gAcl)
    {
        *rowneruuid = gAcl->rownerID;
        retVal = OC_STACK_OK;
    }
    return retVal;
}
