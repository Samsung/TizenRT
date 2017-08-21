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
#include "acl_logging.h"
#include "payload_logging.h"
#include "srmresourcestrings.h"
#include "aclresource.h"
#include "doxmresource.h"
#include "rolesresource.h"
#include "resourcemanager.h"
#include "srmutility.h"
#include "psinterface.h"
#include "ocpayloadcbor.h"
#include "secureresourcemanager.h"
#include "deviceonboardingstate.h"

#include "security_internals.h"

#define TAG  "OIC_SRM_ACL"
#define NUMBER_OF_SEC_PROV_RSCS 3
#define NUMBER_OF_DEFAULT_SEC_RSCS 2
#define STRING_UUID_SIZE (UUID_LENGTH * 2 + 5)

static const uint8_t ACL_MAP_SIZE = 4; // aclist, rowneruuid, RT and IF
static const uint8_t ACL_ACLIST_MAP_SIZE = 1; // aces object
static const uint8_t ACL_ACE_MAP_SIZE = 3; // subject, resource, permissions
static const uint8_t ACL_ACE2_MAP_SIZE = 4; // aceid, subject, resource, permissions
static const uint8_t ACE_DID_MAP_SIZE = 1;
static const uint8_t ACE_ROLE_MAP_SIZE = 1;
static const uint8_t ACE_CONN_MAP_SIZE = 1;

// CborSize is the default cbor payload size being used.
static const uint16_t CBOR_SIZE = 2048*8;

static OicSecAcl_t *gAcl = NULL;
static OCResourceHandle gAclHandle = NULL;
static OCResourceHandle gAcl2Handle = NULL;

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

    if(ace)
    {
        newAce = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
        VERIFY_NOT_NULL(TAG, newAce, ERROR);

        //Subject
        newAce->subjectType = ace->subjectType;
        switch (newAce->subjectType)
        {
        case OicSecAceUuidSubject:
            memcpy(&newAce->subjectuuid, &ace->subjectuuid, sizeof(OicUuid_t));
            break;
        case OicSecAceRoleSubject:
            memcpy(&newAce->subjectRole, &ace->subjectRole, sizeof(ace->subjectRole));
            break;
        case OicSecAceConntypeSubject:
            newAce->subjectConn = ace->subjectConn;
            break;
        default:
            assert(!"Unsupported ACE type");
            OICFree(newAce);
            return NULL;
        }

        OicSecRsrc_t* rsrc = NULL;
        LL_FOREACH(ace->resources, rsrc)
        {
            OicSecRsrc_t* newRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
            VERIFY_NOT_NULL(TAG, newRsrc, ERROR);
            LL_APPEND(newAce->resources, newRsrc);

            // href is not mandatory in OCF 1.0/ACE2
            if (rsrc->href)
            {
                newRsrc->href = OICStrdup(rsrc->href);
                VERIFY_NOT_NULL(TAG, newRsrc->href, ERROR);
            }

            if(rsrc->rel)
            {
                newRsrc->rel = OICStrdup(rsrc->rel);
                VERIFY_NOT_NULL(TAG, newRsrc->rel, ERROR);
            }

            if(rsrc->types && 0 < rsrc->typeLen)
            {
                newRsrc->typeLen = rsrc->typeLen;
                newRsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                VERIFY_NOT_NULL(TAG, (newRsrc->types), ERROR);
                for(size_t i = 0; i < rsrc->typeLen; i++)
                {
                    newRsrc->types[i] = OICStrdup(rsrc->types[i]);
                    VERIFY_NOT_NULL(TAG, (newRsrc->types[i]), ERROR);
                }
            }

            if(rsrc->interfaces && 0 < rsrc->interfaceLen)
            {
                newRsrc->interfaceLen = rsrc->interfaceLen;
                newRsrc->interfaces = (char**)OICCalloc(rsrc->interfaceLen, sizeof(char*));
                VERIFY_NOT_NULL(TAG, (newRsrc->interfaces), ERROR);
                for(size_t i = 0; i < rsrc->interfaceLen; i++)
                {
                    newRsrc->interfaces[i] = OICStrdup(rsrc->interfaces[i]);
                    VERIFY_NOT_NULL(TAG, (newRsrc->interfaces[i]), ERROR);
                }
            }

            newRsrc->wildcard = rsrc->wildcard;
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
                VERIFY_NOT_NULL(TAG, newValidity, ERROR);
                LL_APPEND(newAce->validities, newValidity);

                if(validity->period)
                {
                    newValidity->period = OICStrdup(validity->period);
                    VERIFY_NOT_NULL(TAG, newValidity->period, ERROR);
                }

                if(validity->recurrences && 0 < validity->recurrenceLen)
                {
                    newValidity->recurrenceLen = validity->recurrenceLen;

                    newValidity->recurrences = (char**)OICCalloc(validity->recurrenceLen, sizeof(char*));
                    VERIFY_NOT_NULL(TAG, newValidity->recurrences, ERROR);

                    for(size_t i = 0; i < validity->recurrenceLen; i++)
                    {
                        newValidity->recurrences[i] = OICStrdup(validity->recurrences[i]);
                        VERIFY_NOT_NULL(TAG, (newValidity->recurrences[i]), ERROR);
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
                VERIFY_NOT_NULL(TAG, (newAce->eownerID), ERROR);
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

OCStackResult AclToCBORPayload(const OicSecAcl_t *secAcl,
    OicSecAclVersion_t aclVersion, uint8_t **payload, size_t *size)
{
    if (NULL == secAcl || NULL == payload || NULL != *payload || NULL == size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    /* aclVersion parameter validation. */
    switch (aclVersion)
    {
    case OIC_SEC_ACL_V1:
        OIC_LOG_V(DEBUG, TAG, "%s encoding v1 ACL.", __func__);
        break;
    case OIC_SEC_ACL_V2:
        OIC_LOG_V(DEBUG, TAG, "%s encoding v2 ACL.", __func__);
        break;
    default:
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
    char *subjectuuidstring = NULL;
    const char *aclRsrcType = NULL;
    const char *wcstring = NULL;

    if (cborLen == 0)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_ERROR);

    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create ACL Map which contains aclist or aclist2, rowneruuid, rt and if
    cborEncoderResult = cbor_encoder_create_map(&encoder, &aclMap, ACL_MAP_SIZE);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACL Map.");
    OIC_LOG_V(DEBUG, TAG, "%s starting encoding of %s resource.",
        __func__, (OIC_SEC_ACL_V1 == aclVersion)?"v1 acl":"v2 acl2");

    // v1 uses "aclist" as the top-level tag, containing an "aces" object
    if (OIC_SEC_ACL_V1 == aclVersion)
    {
        cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ACLIST_NAME,
            strlen(OIC_JSON_ACLIST_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aclist Tag.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1 %s tag.", __func__, OIC_JSON_ACLIST_NAME);
    }
    // v2 uses "aclist2" as the top-level tag, containing an array of ace2 objects
    else
    {
        cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ACLIST2_NAME,
            strlen(OIC_JSON_ACLIST2_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aclist2 Tag.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_ACLIST2_NAME);
    }

    // v1 puts "aces" inside an "aclist" object, so create size 1 map
    if (OIC_SEC_ACL_V1 == aclVersion)
    {
        cborEncoderResult = cbor_encoder_create_map(&aclMap, &aclListMap, ACL_ACLIST_MAP_SIZE);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating aclist Map.");
        OIC_LOG_V(DEBUG, TAG, "%s created v1 %s map.", __func__, OIC_JSON_ACLIST_NAME);

        // v1 precedes ACE object array with "ace" tag
        cborEncoderResult = cbor_encode_text_string(&aclListMap, OIC_JSON_ACES_NAME,
            strlen(OIC_JSON_ACES_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACES Name Tag.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1 %s tag.", __func__, OIC_JSON_ACES_NAME);
    }

    // v1 Create ACE Array in aclist map
    if (OIC_SEC_ACL_V1 == aclVersion)
    {
        cborEncoderResult = cbor_encoder_create_array(&aclListMap, &acesArray, OicSecAclSize(secAcl));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACE Array.");
        OIC_LOG_V(DEBUG, TAG, "%s created v1 ACE array.", __func__);
    }
    else
    // v2 create ACE2 array in acl map
    {
        cborEncoderResult = cbor_encoder_create_array(&aclMap, &acesArray, OicSecAclSize(secAcl));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACE2 Array.");
        OIC_LOG_V(DEBUG, TAG, "%s created v2 ACE2 array.", __func__);
    }

    // encode the ACE objects
    ace = NULL;
    LL_FOREACH (acl->aces, ace)
    {
        CborEncoder oicSecAclMap;

        uint8_t aceMapSize = 0;
        // ACE object Map size
        if(OIC_SEC_ACL_V1 == aclVersion)
        {
            aceMapSize = ACL_ACE_MAP_SIZE;
        }
        else
        {
            aceMapSize = ACL_ACE2_MAP_SIZE; // v2 has "aceid" so 1 more item
        }

        // Version 1 doesn't support role subjects. If we have any, we can't comply with the request.
        if ((OIC_SEC_ACL_V2 > aclVersion) && (OicSecAceRoleSubject == ace->subjectType))
        {
            cborEncoderResult = CborUnknownError;
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "ACE has a role subject; can't create a V1 ACL payload for this ACL.");
        }

        OicSecValidity_t* validityElts = ace->validities;
        while(validityElts)
        {
            if(validityElts->period)
            {
                aceMapSize++;
            }
            if(validityElts->recurrences)
            {
                aceMapSize++;
            }
        }

#ifdef MULTIPLE_OWNER
        if(ace->eownerID)
        {
            aceMapSize++;
        }
#endif //MULTIPLE_OWNER

        cborEncoderResult = cbor_encoder_create_map(&acesArray, &oicSecAclMap, aceMapSize);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Creating ACES Map");
        OIC_LOG_V(DEBUG, TAG, "%s created %s map.",
            __func__, OIC_SEC_ACL_V1 == aclVersion?"v1 ACE":"v2 ACE2");

        // v2 has an "aceid" property
        if (OIC_SEC_ACL_V2 == aclVersion) // v2
        {
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_ACEID_NAME,
                strlen(OIC_JSON_ACEID_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aceid Tag.");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_ACEID_NAME);
            cborEncoderResult = cbor_encode_int(&oicSecAclMap, ace->aceid);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding aceid Value.");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 aceid value %d.", __func__, ace->aceid);
        }

        // v1 has a "subjectuuid" tag, followed by a UUID string
        if (OIC_SEC_ACL_V1 == aclVersion) //v1
        {
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_SUBJECTID_NAME,
                strlen(OIC_JSON_SUBJECTID_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding subjectuuid Tag.");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v1 %s tag.", __func__, OIC_JSON_SUBJECTID_NAME);
        }
        // v2 has a "subject" tag, and then one of three different subject types: UUID, roletype or conntype
        else
        {
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_SUBJECT_NAME,
                strlen(OIC_JSON_SUBJECT_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding subject Tag.");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_SUBJECT_NAME);
        }

        // UUID type v1 or v2
        if (OicSecAceUuidSubject == ace->subjectType)
        {
            size_t len = (0 == memcmp(&(ace->subjectuuid), &WILDCARD_SUBJECT_ID, sizeof(ace->subjectuuid))) ?
                WILDCARD_SUBJECT_ID_LEN : sizeof(ace->subjectuuid);
            if (WILDCARD_SUBJECT_ID_LEN == len)
            {
                if (OIC_SEC_ACL_V1 == aclVersion) // v1
                {
                    cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, WILDCARD_RESOURCE_URI,
                        strlen(WILDCARD_RESOURCE_URI));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding subjectuud wildcard URI value.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded subjectuuid wildcard value %s.", __func__, WILDCARD_RESOURCE_URI);
                }
                else // v2
                {
                    cborEncoderResult = CborUnknownError;
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "ACE has wildcard subjectuuid; cannot encode into oic.sec.ace2 object.");
                }
            }
            else
            {
                if (NULL != subjectuuidstring)
                {
                    OICFree(subjectuuidstring);
                    subjectuuidstring = NULL;
                }
                ret = ConvertUuidToStr(&ace->subjectuuid, &subjectuuidstring);
                cborEncoderResult = (OC_STACK_OK == ret) ? CborNoError : CborUnknownError;
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed to convert subject UUID to string");
                OIC_LOG_V(DEBUG, TAG, "%s converted UUID to %s", __func__, subjectuuidstring);

                // v1 encodes the UUID immediately after the "subjectuuid" tag encoded above
                if (OIC_SEC_ACL_V1 == aclVersion)
                {
                    cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, subjectuuidstring, strlen(subjectuuidstring));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding subjectuuid Value");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1 UUID value %s.", __func__, subjectuuidstring);
                }
                // v2 encodes another tag and then the value, depending on the type (UUID, role or conn)
                else
                {
                    // v2 UUID type uses "uuid" tag and then UUID string value
                    CborEncoder didMap;

                    cborEncoderResult = cbor_encoder_create_map(&oicSecAclMap, &didMap, ACE_DID_MAP_SIZE);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating did map");
                    OIC_LOG_V(DEBUG, TAG, "%s created v2 did map.", __func__);

                    cborEncoderResult = cbor_encode_text_string(&didMap, OIC_JSON_UUID_NAME, strlen(OIC_JSON_UUID_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding uuid tag");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_UUID_NAME);

                    cborEncoderResult = cbor_encode_text_string(&didMap, subjectuuidstring, strlen(subjectuuidstring));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding Subject UUID Value");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v2 UUID value %s.", __func__, subjectuuidstring);
                    if (NULL != subjectuuidstring)
                    {
                        OICFree(subjectuuidstring);
                        subjectuuidstring = NULL;
                    }

                    cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &didMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing did map");
                    OIC_LOG_V(DEBUG, TAG, "%s closed v2 did map.", __func__);
                }
            }
        }
        // v2 roletype
        else if (OicSecAceRoleSubject == ace->subjectType)
        {
            assert(OIC_SEC_ACL_V2 <= aclVersion);
            CborEncoder roleMap;

            bool includeAuthority = (0 != memcmp(&ace->subjectRole.authority, &EMPTY_ROLE.authority, sizeof(EMPTY_ROLE.authority)));

            cborEncoderResult = cbor_encoder_create_map(&oicSecAclMap, &roleMap, ACE_ROLE_MAP_SIZE + includeAuthority?1:0);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating role map");
            OIC_LOG_V(DEBUG, TAG, "%s created v2 role map.", __func__);

            cborEncoderResult = cbor_encode_text_string(&roleMap, OIC_JSON_ROLE_NAME, strlen(OIC_JSON_ROLE_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding roleid tag");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_ROLE_NAME);

            cborEncoderResult = cbor_encode_text_string(&roleMap, ace->subjectRole.id, strlen(ace->subjectRole.id));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding roleid value");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 roleid value %s.", __func__, ace->subjectRole.id);

            if (includeAuthority)
            {
                cborEncoderResult = cbor_encode_text_string(&roleMap, OIC_JSON_AUTHORITY_NAME, strlen(OIC_JSON_AUTHORITY_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding authority tag");
                OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_AUTHORITY_NAME);

                cborEncoderResult = cbor_encode_text_string(&roleMap, ace->subjectRole.authority, strlen(ace->subjectRole.authority));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding authority value");
                OIC_LOG_V(DEBUG, TAG, "%s encoded v2 authority value %s.", __func__, ace->subjectRole.authority);
            }

            cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &roleMap);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing role map");
            OIC_LOG_V(DEBUG, TAG, "%s closed v2 role map.", __func__);
        }
        // v2 conntype
        else if (OicSecAceConntypeSubject == ace->subjectType)
        {
            assert(OIC_SEC_ACL_V2 <= aclVersion);
            CborEncoder connMap;

            cborEncoderResult = cbor_encoder_create_map(&oicSecAclMap, &connMap, ACE_CONN_MAP_SIZE);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating conn map");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 conn map.", __func__);

            cborEncoderResult = cbor_encode_text_string(&connMap, OIC_JSON_CONNTYPE_NAME, strlen(OIC_JSON_CONNTYPE_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding conntype tag");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_CONNTYPE_NAME);

            if (AUTH_CRYPT == ace->subjectConn)
            {
                cborEncoderResult = cbor_encode_text_string(&connMap, OIC_JSON_AUTHCRYPT_NAME, strlen(OIC_JSON_AUTHCRYPT_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding auth-crypt value");
                OIC_LOG_V(DEBUG, TAG, "%s encoded v2 conntype value %s.", __func__, OIC_JSON_AUTHCRYPT_NAME);
            }
            else if (ANON_CLEAR == ace->subjectConn)
            {
                cborEncoderResult = cbor_encode_text_string(&connMap, OIC_JSON_ANONCLEAR_NAME, strlen(OIC_JSON_AUTHCRYPT_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed adding anon-clear value");
                OIC_LOG_V(DEBUG, TAG, "%s encoded v2 conntype value %s.", __func__, OIC_JSON_ANONCLEAR_NAME);
            }
            else
            {
                OIC_LOG_V(ERROR, TAG, "%s unknown conntype value %d.", __func__, ace->subjectConn);
                assert(!"unknown conntype value");
                cborEncoderResult = CborUnknownError;
                ret = OC_STACK_ERROR;
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "unknown conntype value");
            }

            cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &connMap);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed closing conn map");
            ret = OC_STACK_ERROR;
            OIC_LOG_V(DEBUG, TAG, "%s closed v2 conn map.", __func__);
        }
        else
        {
            assert(!"Unknown ACE subject type");
            cborEncoderResult = CborUnknownError;
            ret = OC_STACK_ERROR;
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Unknown ACE subject type");
        }

        // Resources
        {
            CborEncoder resources;
            cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_RESOURCES_NAME,
                strlen(OIC_JSON_RESOURCES_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Name Tag.");
            OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_RESOURCES_NAME);

            size_t rsrcLen = 0;
            OicSecRsrc_t* rsrcElts = NULL;
            LL_FOREACH(ace->resources, rsrcElts)
            {
                rsrcLen++;
            }

            cborEncoderResult = cbor_encoder_create_array(&oicSecAclMap, &resources, rsrcLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating resources Array.");
            OIC_LOG_V(DEBUG, TAG, "%s created v1/v2 resources array.", __func__);

            OicSecRsrc_t* rsrc = NULL;
            LL_FOREACH(ace->resources, rsrc)
            {
                CborEncoder rMap;
                size_t rsrcMapSize = 0;
                if (NULL != rsrc->href)
                {
                    rsrcMapSize++;
                }
                if (rsrc->typeLen > 0)
                {
                    rsrcMapSize++;
                }
                if (rsrc->interfaceLen > 0)
                {
                    rsrcMapSize++;
                }
                if (NULL != rsrc->rel)
                {
                    rsrcMapSize++;
                }
                if (NO_WILDCARD != rsrc->wildcard)
                {
                    rsrcMapSize++;
                }

                OIC_LOG_V(DEBUG, TAG, "%s resource map size = "PRIuPTR, __func__, rsrcMapSize);

                cborEncoderResult = cbor_encoder_create_map(&resources, &rMap, rsrcMapSize);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Resource Map.");
                OIC_LOG_V(DEBUG, TAG, "%s created v1/v2 resource map.", __func__);

                // href
                if (NULL != rsrc->href)
                {
                    cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_HREF_NAME,
                        strlen(OIC_JSON_HREF_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding HREF Name Tag.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_HREF_NAME);
                    cborEncoderResult = cbor_encode_text_string(&rMap, rsrc->href, strlen(rsrc->href));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding HREF Value in Map.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 href value %s.", __func__, rsrc->href);
                }

                // resource type
                if (rsrc->typeLen > 0)
                {
                    cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_RT_NAME,
                        strlen(OIC_JSON_RT_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Name Tag.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_RT_NAME);

                    CborEncoder resourceTypes;
                    cborEncoderResult = cbor_encoder_create_array(&rMap, &resourceTypes, rsrc->typeLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating RT Array.");
                    OIC_LOG_V(DEBUG, TAG, "%s created v1/v2 rt array.", __func__);
                    for(size_t i = 0; i < rsrc->typeLen; i++)
                    {
                        cborEncoderResult = cbor_encode_text_string(&resourceTypes, rsrc->types[i], strlen(rsrc->types[i]));
                        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
                        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 rt value %s.", __func__, rsrc->types[i]);
                    }
                    cborEncoderResult = cbor_encoder_close_container(&rMap, &resourceTypes);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing resourceTypes.");
                    OIC_LOG_V(DEBUG, TAG, "%s closed v1/v2 resource map.", __func__);
                }

                // interface
                if (rsrc->interfaceLen > 0)
                {
                    cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_IF_NAME,
                        strlen(OIC_JSON_IF_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Name Tag.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_IF_NAME);

                    CborEncoder interfaces;
                    cborEncoderResult = cbor_encoder_create_array(&rMap, &interfaces, rsrc->interfaceLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating IF Array.");
                    OIC_LOG_V(DEBUG, TAG, "%s created v1/v2 IF array.", __func__);
                    for(size_t i = 0; i < rsrc->interfaceLen; i++)
                    {
                        cborEncoderResult = cbor_encode_text_string(&interfaces, rsrc->interfaces[i], strlen(rsrc->interfaces[i]));
                        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
                        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 IF value %s.", __func__, rsrc->interfaces[i]);
                    }
                    cborEncoderResult = cbor_encoder_close_container(&rMap, &interfaces);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing interfaces.");
                }

                // rel
                if(NULL != rsrc->rel)
                {
                    cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_REL_NAME,
                            strlen(OIC_JSON_REL_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding REL Name Tag.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_REL_NAME);
                    cborEncoderResult = cbor_encode_text_string(&rMap, rsrc->rel, strlen(rsrc->rel));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding REL Value.");
                    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 rel value %s.", __func__, rsrc->rel);
                }

                // v2 supports wildcard
                if (OIC_SEC_ACL_V2 == aclVersion)
                {
                    if (NO_WILDCARD != rsrc->wildcard)
                    {
                        cborEncoderResult = cbor_encode_text_string(&rMap, OIC_JSON_WC_NAME,
                            strlen(OIC_JSON_WC_NAME));
                        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding WC Name Tag.");
                        OIC_LOG_V(DEBUG, TAG, "%s encoded v2 %s tag.", __func__, OIC_JSON_WC_NAME);
                        switch(rsrc->wildcard)
                        {
                            case ALL_DISCOVERABLE:
                            wcstring = OIC_JSON_WC_PLUS_NAME;
                            break;
                            case ALL_NON_DISCOVERABLE:
                            wcstring = OIC_JSON_WC_MINUS_NAME;
                            break;
                            case ALL_RESOURCES:
                            wcstring = OIC_JSON_WC_ASTERISK_NAME;
                            break;
                            default:
                            OIC_LOG_V(ERROR, TAG, "%s: unknown ACE2 wildcard type.", __func__);
                            VERIFY_CBOR_SUCCESS(TAG, CborUnknownError, "unknown ACE2 wildcard type");
                            break;
                        }
                        cborEncoderResult = cbor_encode_text_string(&rMap, wcstring, strlen(wcstring));
                        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding WC Value.");
                        OIC_LOG_V(DEBUG, TAG, "%s encoded v2 wc value %s.", __func__, wcstring);
                        wcstring = NULL;
                    }
                }

                cborEncoderResult = cbor_encoder_close_container(&resources, &rMap);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Map.");
                OIC_LOG_V(DEBUG, TAG, "%s closed v1/v2 resource map.", __func__);
            }
            cborEncoderResult = cbor_encoder_close_container(&oicSecAclMap, &resources);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Resource Name Array.");
            OIC_LOG_V(DEBUG, TAG, "%s closed v1/v2 resources array.", __func__);
        }

        // Permissions -- Mandatory
        cborEncoderResult = cbor_encode_text_string(&oicSecAclMap, OIC_JSON_PERMISSION_NAME,
            strlen(OIC_JSON_PERMISSION_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Tag.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_PERMISSION_NAME);
        cborEncoderResult = cbor_encode_int(&oicSecAclMap, ace->permission);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Permission Name Value.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 permission value %d.", __func__, ace->permission);

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
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding eownerId Value.");
        }
#endif //MULTIPLE_OWNER

        // close the map of the oic.sec.ace/ace2 object
        cborEncoderResult = cbor_encoder_close_container(&acesArray, &oicSecAclMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACES Map.");
        OIC_LOG_V(DEBUG, TAG, "%s closed v1/v2 aces map.", __func__);
    }

    // close the v1 array of ace objects
    if (OIC_SEC_ACL_V1 == aclVersion)
    {
        cborEncoderResult = cbor_encoder_close_container(&aclListMap, &acesArray);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACE Array.");
        OIC_LOG_V(DEBUG, TAG, "%s closed v1 ACE array.", __func__);
    }
    else
    {
        cborEncoderResult = cbor_encoder_close_container(&aclMap, &acesArray);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACE2 Array.");
        OIC_LOG_V(DEBUG, TAG, "%s closed v2 ACE2 array.", __func__);
    }

    // v1 has the aclist map to close (v2 just has the array, no extra map)
    if (OIC_SEC_ACL_V1 == aclVersion)
    {
        cborEncoderResult = cbor_encoder_close_container(&aclMap, &aclListMap);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing aclist/aclist2 Map.");
        OIC_LOG_V(DEBUG, TAG, "%s closed v1/v2 aclist/aclist2 map.", __func__);
    }

    // Rownerid
    {
        char *rowner = NULL;
        cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_ROWNERID_NAME,
            strlen(OIC_JSON_ROWNERID_NAME));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding rownerid Name.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_ROWNERID_NAME);
        ret = ConvertUuidToStr(&secAcl->rownerID, &rowner);
        VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
        cborEncoderResult = cbor_encode_text_string(&aclMap, rowner, strlen(rowner));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding rownerid Value.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 rowneruuid value %s.", __func__, rowner);
        OICFree(rowner);
    }

    //RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_RT_NAME,
            strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Tag.");
    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_RT_NAME);
    cborEncoderResult = cbor_encoder_create_array(&aclMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating RT array.");
    OIC_LOG_V(DEBUG, TAG, "%s created %s array.", __func__, OIC_JSON_RT_NAME);
    aclRsrcType = (OIC_SEC_ACL_V1 == aclVersion)?OIC_RSRC_TYPE_SEC_ACL:OIC_RSRC_TYPE_SEC_ACL2;
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, aclRsrcType,
                strlen(aclRsrcType));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding RT Value.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, aclRsrcType);
    }
    aclRsrcType = NULL;
    cborEncoderResult = cbor_encoder_close_container(&aclMap, &rtArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing RT.");
    OIC_LOG_V(DEBUG, TAG, "%s closed %s array.", __func__, OIC_JSON_RT_NAME);

    //IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&aclMap, OIC_JSON_IF_NAME,
             strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Name Tag.");
    OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 %s tag.", __func__, OIC_JSON_IF_NAME);
    cborEncoderResult = cbor_encoder_create_array(&aclMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed creating IF array.");
    OIC_LOG_V(DEBUG, TAG, "%s created %s array.", __func__, OIC_JSON_IF_NAME);
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
                strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding IF Value.");
        OIC_LOG_V(DEBUG, TAG, "%s encoded v1/v2 IF value %s.", __func__, OC_RSRVD_INTERFACE_DEFAULT);
    }
    cborEncoderResult = cbor_encoder_close_container(&aclMap, &ifArray);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing IF.");
    OIC_LOG_V(DEBUG, TAG, "%s closed IF array.", __func__);

    // Close ACL Map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &aclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing ACL Map.");
    OIC_LOG_V(DEBUG, TAG, "%s closed ACL map.", __func__);

    if (CborNoError == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "AclToCBORPayload Succeeded");
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
        ret = AclToCBORPayload(secAcl, aclVersion, payload, &cborLen);
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

    if(NULL != subjectuuidstring)
    {
        OICFree(subjectuuidstring);
        subjectuuidstring = NULL;
    }

    return ret;
}

// This function converts CBOR format to ACL data.
// Caller needs to invoke 'free' when done using
// It parses { "aclist" : [ { ... } ] } instead of { "aclist" : { "aces" : [ ] } }
OicSecAcl_t* CBORPayloadToCloudAcl(const uint8_t *cborPayload, const size_t size)
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
    VERIFY_NOT_NULL_RETURN(TAG, acl, ERROR, NULL);

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
                    VERIFY_NOT_NULL(TAG, ace, ERROR);
                    LL_APPEND(acl->aces, ace);

                    VERIFY_NOT_NULL(TAG, acl, ERROR);

                    while (cbor_value_is_valid(&aceMap))
                    {
                        char* name = NULL;
                        size_t tempLen = 0;
                        CborType aceMapType = cbor_value_get_type(&aceMap);
                        if (aceMapType == CborTextStringType)
                        {
                            cborFindResult = cbor_value_dup_text_string(&aceMap, &name, &tempLen, NULL);
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
                                cborFindResult = cbor_value_dup_text_string(&aceMap, &subject, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                                if(strcmp(subject, WILDCARD_RESOURCE_URI) == 0)
                                {
                                    ace->subjectuuid.id[0] = '*';
                                    free(subject);
                                }
                                else
                                {
                                    OIC_LOG_V(DEBUG, TAG, "Converting subjectuuid = %s to uuid...", subject);
                                    ret = ConvertStrToUuid(subject, &ace->subjectuuid);
                                    free(subject);
                                    VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
                                }
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
                                    VERIFY_NOT_NULL(TAG, rsrc, ERROR);
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
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->href, &tempLen, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                                        }

                                        // "rt"
                                        if (0 == strcmp(OIC_JSON_RT_NAME, rMapName) && cbor_value_is_array(&rMap))
                                        {
                                            cbor_value_get_array_length(&rMap, &rsrc->typeLen);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT array length.");
                                            VERIFY_SUCCESS(TAG, (0 != rsrc->typeLen), ERROR);

                                            rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                                            VERIFY_NOT_NULL(TAG, rsrc->types, ERROR);

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
                                            VERIFY_NOT_NULL(TAG, rsrc->interfaces, ERROR);

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
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->rel, &tempLen, NULL);
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
                                    VERIFY_NOT_NULL(TAG, validity, ERROR);
                                    LL_APPEND(ace->validities, validity);

                                    CborValue validityMap  = {.parser = NULL};
                                    //period (string)
                                    cborFindResult = cbor_value_enter_container(&validitiesMap, &validityMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Map.");

                                    size_t vmLen = 0;
                                    cborFindResult = cbor_value_dup_text_string(&validityMap, &validity->period, &vmLen, NULL);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period value.");

                                    //recurrence (string array)
                                    CborValue recurrenceMap  = {.parser = NULL};
                                    cborFindResult = cbor_value_enter_container(&validityMap, &recurrenceMap);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence array.");

                                    cborFindResult = cbor_value_get_array_length(&recurrenceMap, &validity->recurrenceLen);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array.");

                                    validity->recurrences = (char**)OICCalloc(validity->recurrenceLen, sizeof(char*));
                                    VERIFY_NOT_NULL(TAG, validity->recurrences, ERROR);

                                    for(size_t i = 0; cbor_value_is_text_string(&recurrenceMap) && i < validity->recurrenceLen; i++)
                                    {
                                        cborFindResult = cbor_value_dup_text_string(&recurrenceMap, &validity->recurrences[i], &vmLen, NULL);
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

                        if (aceMapType != CborMapType && cbor_value_is_valid(&aceMap))
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
                free(stRowner);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            }
            // Strings allocated with cbor_value_dup_text_string must be freed with free, not OICFree.
            free(tagName);
            tagName = NULL;
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
// Callers should normally invoke "CBORPayloadToAcl()" unless wishing to check
// version of payload only.
// Caller needs to invoke 'OICFree' on returned value when done using
// TODO IOT-2220 this function is a prime example of why the SVR CBOR functions need
// to be re-factored throughout.  It's even worse with the addition of /acl2.
// @param[in]  cborPayload The CBOR data to be decoded into OicSecAcl_t
// @param[in]  size The size of the CBOR data
// @param[out] versionCheck If included, this function will determine the version of
//              ACL in the payload, assign to 'versionCheck', and return NULL
//              without decoding the rest of the payload.  If NULL, this function will complete
//              decoding as normal, and will not assign a value to 'versionCheck'.
static OicSecAcl_t* CBORPayloadToAclVersionOpt(const uint8_t *cborPayload, const size_t size,
    OicSecAclVersion_t *versionCheck)
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
    char *tagName = NULL;
    char *subjectTag = NULL;
    char *rMapName = NULL;
    OicSecAclVersion_t aclistVersion = OIC_SEC_ACL_LATEST;
    bool aclistTagJustFound = false;
    bool aceArrayIsNextItem = false;
    char *acName = NULL;
    size_t readLen = 0;

    cbor_parser_init(cborPayload, size, 0, &parser, &aclCbor);

    OicSecAcl_t *acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
    VERIFY_NOT_NULL(TAG, acl, ERROR);

    // Enter ACL Map
    cborFindResult = cbor_value_enter_container(&aclCbor, &aclMap);
    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACL Map.");
    while (cbor_value_is_valid(&aclMap))
    {
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
            OIC_LOG_V(DEBUG, TAG, "%s found %s tag.", __func__, tagName);
            if (0 == strcmp(tagName, OIC_JSON_ACLIST_NAME))
            {
                if (NULL != versionCheck)
                {
                    OIC_LOG_V(DEBUG, TAG, "%s Found v1 ACL; assigning 'versionCheck' and returning NULL.", __func__);
                    *versionCheck = OIC_SEC_ACL_V1;
                    OICFree(acl);
                    return NULL;
                }
                OIC_LOG_V(DEBUG, TAG, "%s decoding v1 ACL.", __func__);
                aclistVersion = OIC_SEC_ACL_V1;
                aclistTagJustFound = true;

            }
            else if (0 == strcmp(tagName, OIC_JSON_ACLIST2_NAME))
            {
                if (NULL != versionCheck)
                {
                    OIC_LOG_V(DEBUG, TAG, "%s Found v2 ACL; assigning 'versionCheck' and returning NULL.", __func__);
                    *versionCheck = OIC_SEC_ACL_V2;
                    OICFree(acl);
                    return NULL;
                }
                OIC_LOG_V(DEBUG, TAG, "%s decoding v2 ACL.", __func__);
                aclistVersion = OIC_SEC_ACL_V2;
                aclistTagJustFound = true;
            }

            CborValue aclistMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
            if (aclistTagJustFound && OIC_SEC_ACL_V1 == aclistVersion)
            {
                aclistTagJustFound = false; // don't enter this check a second time
                // Enter ACLIST Map
                cborFindResult = cbor_value_enter_container(&aclMap, &aclistMap);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACLIST Map.");
                OIC_LOG_V(DEBUG, TAG, "%s entered 'aclist' map.", __func__);

                bool parsedAcesTag = false;

                // aclist always contains just "aces" tag and then the array so parse the tag
                CborType acType = cbor_value_get_type(&aclistMap);
                if (acType == CborTextStringType)
                {
                    cborFindResult = cbor_value_dup_text_string(&aclistMap, &acName, &readLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACLIST Map.");
                    cborFindResult = cbor_value_advance(&aclistMap);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACLIST Map.");
                    if (acName)
                    {
                        if (strcmp(acName, OIC_JSON_ACES_NAME)  == 0)
                        {
                            OIC_LOG_V(DEBUG, TAG, "%s found %s tag.", __func__, acName);
                            parsedAcesTag = true;
                        }
                    }
                }
                if (parsedAcesTag)
                {
                    aceArrayIsNextItem = true;
                }
                else
                {
                    OIC_LOG_V(WARNING, TAG, "%s expected aces tag, not found!", __func__);
                }

            }
            else if (aclistTagJustFound && OIC_SEC_ACL_V2 == aclistVersion)
            {
                aclistTagJustFound = false; // don't enter this check a second time
                aceArrayIsNextItem = true;
                aclistMap = aclMap;
            }

            // just found either "aclist2" or "aces" tag; time to parse array
            if (aceArrayIsNextItem)
            {
                aceArrayIsNextItem = false; // don't enter this block again
                bool aclistMapDone = false;
                while (cbor_value_is_valid(&aclistMap) && !aclistMapDone)
                {
                    // Enter ACES Array
                    CborValue acesArray = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                    cborFindResult = cbor_value_enter_container(&aclistMap, &acesArray);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACES Array.");
                    OIC_LOG_V(DEBUG, TAG, "%s entered ace object array.", __func__);

                    // decode array of ace or ace2 objects
                    int acesCount = 0;
                    OIC_LOG_V(DEBUG, TAG, "%s begin decoding ace/ace2 array.", __func__);
                    while (cbor_value_is_valid(&acesArray))
                    {
                        acesCount++;
                        CborValue aceMap = { .parser = NULL, .ptr = NULL, .remaining = 0, .extra = 0, .type = 0, .flags = 0 };
                        cborFindResult = cbor_value_enter_container(&acesArray, &aceMap);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering ACE Map.");
                        OIC_LOG_V(DEBUG, TAG, "%s entered %s map.", __func__,
                            (OIC_SEC_ACL_V1 == aclistVersion)?"ace":"ace2");

                        OicSecAce_t *ace = NULL;
                        ace = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
                        VERIFY_NOT_NULL(TAG, ace, ERROR);
                        LL_APPEND(acl->aces, ace);

                        // parse this ACE/ACE2 object
                        while (cbor_value_is_valid(&aceMap))
                        {
                            char* name = NULL;
                            size_t tempLen = 0;
                            CborType aceMapType = cbor_value_get_type(&aceMap);
                            if (aceMapType == CborTextStringType)
                            {
                                cborFindResult = cbor_value_dup_text_string(&aceMap, &name, &tempLen, NULL);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Name in ACE Map.");
                                OIC_LOG_V(DEBUG, TAG, "%s found %s tag.", __func__, name);
                                cborFindResult = cbor_value_advance(&aceMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Value in ACE Map.");
                                OIC_LOG_V(DEBUG, TAG, "%s advanced to next value.", __func__);
                            }
                            if (name)
                            {
                                // aceid
                                if (0 == strcmp(name, OIC_JSON_ACEID_NAME))
                                {
                                    uint64_t tmp64;
                                    cborFindResult = cbor_value_get_uint64(&aceMap, &tmp64);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding aceid Value.");
                                    OIC_LOG_V(DEBUG, TAG, "%s read aceid value %d.", __func__, (uint16_t)tmp64);
                                    ace->aceid = (uint16_t)tmp64;
                                }
                                // subjectuuid
                                if (0 == strcmp(name, OIC_JSON_SUBJECTID_NAME)) // ace v1
                                {
                                    if (cbor_value_is_text_string(&aceMap))
                                    {
                                        char *subject = NULL;
                                        cborFindResult = cbor_value_dup_text_string(&aceMap, &subject, &tempLen, NULL);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                                        if (0 == strcmp(subject, WILDCARD_RESOURCE_URI))
                                        {
                                            ace->subjectuuid.id[0] = '*';
                                            ace->subjectType = OicSecAceUuidSubject;
                                            OIC_LOG_V(DEBUG, TAG, "%s found subjectuuid wildcard = '*'.", __func__);
                                        }
                                        else
                                        {
                                            ret = ConvertStrToUuid(subject, &ace->subjectuuid);
                                            if (OC_STACK_OK != ret)
                                            {
                                                cborFindResult = CborUnknownError;
                                                free(subject);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed converting subject UUID");
                                            }
                                            OIC_LOG_V(DEBUG, TAG, "%s found subjectuuid = %s.", __func__, subject);
                                            ace->subjectType = OicSecAceUuidSubject;
                                        }
                                        free(subject);
                                    }
                                }
                                // subject
                                if (0 == strcmp(name, OIC_JSON_SUBJECT_NAME)) // ace2
                                {
                                    CborValue subjectMap;
                                    memset(&subjectMap, 0, sizeof(subjectMap));
                                    size_t unusedLen = 0;

                                    if (cbor_value_is_container(&aceMap))
                                    {
                                        // next container within subject is either didtype, roletype, or conntype
                                        cborFindResult = cbor_value_enter_container(&aceMap, &subjectMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed entering subject map.");
                                        OIC_LOG_V(DEBUG, TAG, "%s entered acl2 subject map.", __func__);

                                        while (cbor_value_is_valid(&subjectMap) && cbor_value_is_text_string(&subjectMap))
                                        {
                                            cborFindResult = cbor_value_dup_text_string(&subjectMap, &subjectTag, &unusedLen, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting subject type tag name");
                                            OIC_LOG_V(DEBUG, TAG, "%s found %s tag.", __func__, subjectTag);
                                            cborFindResult = cbor_value_advance(&subjectMap);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing subjectMap");
                                            OIC_LOG_V(DEBUG, TAG, "%s advanced acl2 subject map.", __func__);
                                            if (NULL != subjectTag)
                                            {
                                                // didtype
                                                if (0 == strcmp(subjectTag, OIC_JSON_UUID_NAME))
                                                {
                                                    char *subject = NULL;
                                                    if (cbor_value_is_valid(&subjectMap) && cbor_value_is_text_string(&subjectMap))
                                                    {
                                                        cborFindResult = cbor_value_dup_text_string(&subjectMap, &subject, &tempLen, NULL);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding subject Value.");
                                                        OIC_LOG_V(DEBUG, TAG, "%s read UUID = %s", __func__, subject);
                                                        ret = ConvertStrToUuid(subject, &ace->subjectuuid);
                                                        if (OC_STACK_OK != ret)
                                                        {
                                                            cborFindResult = CborUnknownError;
                                                            free(subject);
                                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed converting subject UUID");
                                                        }
                                                        ace->subjectType = OicSecAceUuidSubject;
                                                        free(subject);
                                                    }
                                                    else
                                                    {
                                                        OIC_LOG_V(WARNING, TAG, "%s uuid tag not followed by valid text string value.", __func__);
                                                    }
                                                }
                                                // role
                                                else if (0 == strcmp(subjectTag, OIC_JSON_ROLE_NAME))
                                                {
                                                    ace->subjectType = OicSecAceRoleSubject;
                                                    char *roleId = NULL;
                                                    cborFindResult = cbor_value_dup_text_string(&subjectMap, &roleId, &unusedLen, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting role id value");
                                                    if (strlen(roleId) >= sizeof(ace->subjectRole.id))
                                                    {
                                                        cborFindResult = CborUnknownError;
                                                        free(roleId);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Role ID is too long");
                                                    }
                                                    OICStrcpy(ace->subjectRole.id, sizeof(ace->subjectRole.id), roleId);
                                                    free(roleId);
                                                }
                                                // authority
                                                else if (0 == strcmp(subjectTag, OIC_JSON_AUTHORITY_NAME))
                                                {
                                                    ace->subjectType = OicSecAceRoleSubject;
                                                    char *authorityName = NULL;
                                                    cborFindResult = cbor_value_dup_text_string(&subjectMap, &authorityName, &unusedLen, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting role authority value");
                                                    if (strlen(authorityName) >= sizeof(ace->subjectRole.authority))
                                                    {
                                                        cborFindResult = CborUnknownError;
                                                        free(authorityName);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Authority name is too long");
                                                    }
                                                    OICStrcpy(ace->subjectRole.authority, sizeof(ace->subjectRole.authority), authorityName);
                                                    ace->subjectType = OicSecAceRoleSubject;
                                                    free(authorityName);
                                                }
                                                // conntype
                                                else if (0 == strcmp(subjectTag, OIC_JSON_CONNTYPE_NAME))
                                                {
                                                    char *conntype = NULL;
                                                    cborFindResult = cbor_value_dup_text_string(&subjectMap, &conntype, &unusedLen, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed getting conntype value");
                                                    OIC_LOG_V(DEBUG, TAG, "%s read conntype value %s.", __func__, conntype);
                                                    if (0 == strcmp(conntype, OIC_JSON_AUTHCRYPT_NAME))
                                                    {
                                                        ace->subjectConn = AUTH_CRYPT;
                                                    }
                                                    else if (0 == strcmp(conntype, OIC_JSON_ANONCLEAR_NAME))
                                                    {
                                                        ace->subjectConn = ANON_CLEAR;
                                                    }
                                                    else
                                                    {
                                                        cborFindResult = CborUnknownError;
                                                        free(conntype);
                                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "conntype value not recognized.");
                                                    }
                                                    ace->subjectType = OicSecAceConntypeSubject;
                                                    free(conntype);
                                                }
                                                else
                                                {
                                                    OIC_LOG_V(WARNING, TAG, "Unknown tag in subject map: %s", subjectTag);
                                                }
                                            }

                                            // If roletype, make sure at least the id is present.
                                            if(OicSecAceRoleSubject == ace->subjectType)
                                            {
                                                if ('\0' == ace->subjectRole.id[0])
                                                {
                                                    cborFindResult = CborUnknownError;
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "ID for role was not present in role map");
                                                }
                                            }

                                            // advance to next elt in subject map
                                            if (cbor_value_is_valid(&subjectMap))
                                            {
                                                cborFindResult = cbor_value_advance(&subjectMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing subject map");
                                                OIC_LOG_V(DEBUG, TAG, "%s advanced ace2 subject map.", __func__);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        cborFindResult = CborUnknownError;
                                        OIC_LOG_V(ERROR, TAG, "Unknown subject value type: %d", cbor_value_get_type(&aceMap));
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Type of subject value was not expected");
                                    }
                                }

                                // Resources -- Mandatory
                                if (0 == strcmp(name, OIC_JSON_RESOURCES_NAME))
                                {
                                    CborValue resources = { .parser = NULL };
                                    cborFindResult = cbor_value_enter_container(&aceMap, &resources);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering a Resource Array.");
                                    OIC_LOG_V(DEBUG, TAG, "%s entered resources array.", __func__);
                                    int resourceCount = 0;

                                    while (cbor_value_is_valid(&resources))
                                    {
                                        // rMap is the map of the current Resource being decoded
                                        CborValue rMap = { .parser = NULL  };
                                        cborFindResult = cbor_value_enter_container(&resources, &rMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering Resource Map");
                                        resourceCount++;
                                        OIC_LOG_V(DEBUG, TAG, "%s entered resource map for resource #%d.", __func__, resourceCount);

                                        OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
                                        VERIFY_NOT_NULL(TAG, rsrc, ERROR);
                                        LL_APPEND(ace->resources, rsrc);

                                        while(cbor_value_is_valid(&rMap))
                                        {
                                            if (NULL != rMapName)
                                            {
                                                free(rMapName);
                                                rMapName = NULL;
                                            }
                                            size_t rMapNameLen = 0;
                                            cborFindResult = cbor_value_dup_text_string(&rMap, &rMapName, &rMapNameLen, NULL);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RMap Data Name Tag.");
                                            OIC_LOG_V(DEBUG, TAG, "%s found %s tag in resource #%d map.", __func__, rMapName, resourceCount);
                                            cborFindResult = cbor_value_advance(&rMap);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed advancing RMap Data Value.");
                                            OIC_LOG_V(DEBUG, TAG, "%s advanced resource map.", __func__);

                                            // "href"
                                            if (0 == strcmp(OIC_JSON_HREF_NAME, rMapName))
                                            {
                                                cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->href, &tempLen, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Href Value.");
                                                OIC_LOG_V(DEBUG, TAG, "%s found href = %s.", __func__, rsrc->href);
                                                // OCF 1.0 shouldn't use "*" href even though it's supported; instead,
                                                // use "wc" object.
                                                if (0 == strcmp(WILDCARD_RESOURCE_URI, rsrc->href))
                                                {
                                                    free(rsrc->href);
                                                    rsrc->href = NULL;
                                                    rsrc->wildcard = ALL_RESOURCES;
                                                    OIC_LOG_V(DEBUG, TAG, "%s: replaced \"*\" href with wildcard = ALL_RESOURCES.",
                                                        __func__);
                                                }
                                                else
                                                {
                                                    rsrc->wildcard = NO_WILDCARD;
                                                }
                                            }

                                            // "rt"
                                            if (0 == strcmp(OIC_JSON_RT_NAME, rMapName) && cbor_value_is_array(&rMap))
                                            {
                                                cbor_value_get_array_length(&rMap, &rsrc->typeLen);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding RT array length.");
                                                VERIFY_SUCCESS(TAG, (0 != rsrc->typeLen), ERROR);

                                                rsrc->types = (char**)OICCalloc(rsrc->typeLen, sizeof(char*));
                                                VERIFY_NOT_NULL(TAG, rsrc->types, ERROR);

                                                CborValue resourceTypes;
                                                cborFindResult = cbor_value_enter_container(&rMap, &resourceTypes);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering RT Array.");

                                                for(size_t i = 0; cbor_value_is_valid(&resourceTypes) && cbor_value_is_text_string(&resourceTypes); i++)
                                                {
                                                    cborFindResult = cbor_value_dup_text_string(&resourceTypes, &(rsrc->types[i]), &readLen, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding resource type.");
                                                    OIC_LOG_V(DEBUG, TAG, "%s found rt = %s.", __func__, rsrc->types[i]);
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
                                                VERIFY_NOT_NULL(TAG, rsrc->interfaces, ERROR);

                                                CborValue interfaces;
                                                cborFindResult = cbor_value_enter_container(&rMap, &interfaces);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Entering IF Array.");

                                                for(size_t i = 0; cbor_value_is_valid(&interfaces) && cbor_value_is_text_string(&interfaces); i++)
                                                {
                                                    cborFindResult = cbor_value_dup_text_string(&interfaces, &(rsrc->interfaces[i]), &readLen, NULL);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding IF type.");
                                                    OIC_LOG_V(DEBUG, TAG, "%s found if = %s.", __func__, rsrc->interfaces[i]);
                                                    cborFindResult = cbor_value_advance(&interfaces);
                                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing IF type.");
                                                }
                                            }

                                            // "rel"
                                            if (0 == strcmp(OIC_JSON_REL_NAME, rMapName))
                                            {
                                                cborFindResult = cbor_value_dup_text_string(&rMap, &rsrc->rel, &tempLen, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding REL Value.");
                                                OIC_LOG_V(DEBUG, TAG, "%s found rel = %s.", __func__, rsrc->rel);
                                            }

                                            // "wc"
                                            if (0 == strcmp(OIC_JSON_WC_NAME, rMapName))
                                            {
                                                char *wc = NULL;
                                                cborFindResult = cbor_value_dup_text_string(&rMap, &wc, &tempLen, NULL);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding wc Value.");
                                                OIC_LOG_V(DEBUG, TAG, "%s found wc = %s.", __func__, wc);
                                                if (0 == strcmp(OIC_JSON_WC_ASTERISK_NAME, wc))
                                                {
                                                    rsrc->wildcard = ALL_RESOURCES;
                                                    OIC_LOG_V(DEBUG, TAG, "%s set wildcard = ALL_RESOURCES.", __func__);
                                                }
                                                else if (0 == strcmp(OIC_JSON_WC_PLUS_NAME, wc))
                                                {
                                                    rsrc->wildcard = ALL_DISCOVERABLE;
                                                    OIC_LOG_V(DEBUG, TAG, "%s set wildcard = ALL_DISCOVERABLE.", __func__);
                                                }
                                                else if (0 == strcmp(OIC_JSON_WC_MINUS_NAME, wc))
                                                {
                                                    rsrc->wildcard = ALL_NON_DISCOVERABLE;
                                                    OIC_LOG_V(DEBUG, TAG, "%s set wildcard = ALL_NON_DISCOVERABLE.", __func__);
                                                }
                                                else
                                                {
                                                    rsrc->wildcard = NO_WILDCARD;
                                                    OIC_LOG_V(DEBUG, TAG, "%s set wildcard = NO_WILDCARD.", __func__);
                                                }
                                                free(wc);
                                            }

                                            if (cbor_value_is_valid(&rMap))
                                            {
                                                cborFindResult = cbor_value_advance(&rMap);
                                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing rMap.");
                                                OIC_LOG_V(DEBUG, TAG, "%s advanced rMap.", __func__);
                                            }
                                        }

                                        OIC_LOG_V(DEBUG, TAG, "%s finished decoding resource #%d.", __func__, resourceCount);

                                        if (cbor_value_is_valid(&resources))
                                        {
                                            cborFindResult = cbor_value_advance(&resources);
                                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing Resource Array.");
                                            OIC_LOG_V(DEBUG, TAG, "%s advanced resources map.", __func__);
                                        }
                                    }
                                } // end resources decoding

                                // Permissions -- Mandatory
                                if (strcmp(name, OIC_JSON_PERMISSION_NAME) == 0)
                                {
                                    uint64_t tmp64;
                                    cborFindResult = cbor_value_get_uint64(&aceMap, &tmp64);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a PERM Value.");
                                    ace->permission = (uint16_t)tmp64;
                                    OIC_LOG_V(DEBUG, TAG, "%s found permission = %d.", __func__, ace->permission);
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
                                        VERIFY_NOT_NULL(TAG, validity, ERROR);
                                        LL_APPEND(ace->validities, validity);

                                        CborValue validityMap  = {.parser = NULL};
                                        //period (string)
                                        cborFindResult = cbor_value_enter_container(&validitiesMap, &validityMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a validity Map.");

                                        size_t vmLen = 0;
                                        cborFindResult =cbor_value_dup_text_string(&validityMap, &validity->period, &vmLen, NULL);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a Period value.");

                                        //recurrence (string array)
                                        CborValue recurrenceMap  = {.parser = NULL};
                                        cborFindResult = cbor_value_enter_container(&validityMap, &recurrenceMap);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding a recurrence array.");

                                        cborFindResult = cbor_value_get_array_length(&recurrenceMap, &validity->recurrenceLen);
                                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Adding Recurrence Array.");

                                        validity->recurrences = (char**)OICCalloc(validity->recurrenceLen, sizeof(char*));
                                        VERIFY_NOT_NULL(TAG, validity->recurrences, ERROR);

                                        for(size_t i = 0; cbor_value_is_text_string(&recurrenceMap) && i < validity->recurrenceLen; i++)
                                        {
                                            cborFindResult = cbor_value_dup_text_string(&recurrenceMap, &validity->recurrences[i], &vmLen, NULL);
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
                                    cborFindResult = cbor_value_dup_text_string(&aceMap, &eowner, &tempLen, NULL);
                                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding eownerId Value.");
                                    if(NULL == ace->eownerID)
                                    {
                                        ace->eownerID = (OicUuid_t*)OICCalloc(1, sizeof(OicUuid_t));
                                        VERIFY_NOT_NULL(TAG, ace->eownerID, ERROR);
                                    }
                                    ret = ConvertStrToUuid(eowner, ace->eownerID);
                                    OICFree(eowner);
                                    VERIFY_SUCCESS(TAG, OC_STACK_OK == ret , ERROR);
                                }
#endif //MULTIPLE_OWNER
                                OIC_LOG_V(DEBUG, TAG, "%s finished decoding %s.", __func__, name);
                                OICFree(name);
                            }

                            if (aceMapType != CborMapType && cbor_value_is_valid(&aceMap))
                            {
                                cborFindResult = cbor_value_advance(&aceMap);
                                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing the Array.");
                                OIC_LOG_V(DEBUG, TAG, "%s advanced aceMap.", __func__);
                            }
                        } // end decoding ACE/ACE2 object

                        OIC_LOG_V(DEBUG, TAG, "%s finished decoding ace #%d.", __func__, acesCount);

                        // advance to next ACE/ACE2 object in acesArray
                        if (cbor_value_is_valid(&acesArray))
                        {
                            cborFindResult = cbor_value_advance(&acesArray);
                            VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACL Array.");
                        }
                    } // end ace array while loop

                    OIC_LOG_V(DEBUG, TAG, "%s finished decoding ace/ace2 array.", __func__);

                    if (cbor_value_is_valid(&aclistMap))
                    {
                        cborFindResult = cbor_value_advance(&aclistMap);
                        VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Advancing ACLIST Map.");
                        OIC_LOG_V(DEBUG, TAG, "%s advanced aclist map.", __func__);
                    }

                    if (OIC_SEC_ACL_V2 == aclistVersion)
                    {
                        aclistMapDone = true;
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
                free(stRowner);
                VERIFY_SUCCESS(TAG, ret == OC_STACK_OK, ERROR);
            }
            else if (NULL != gAcl)
            {
                memcpy(&(acl->rownerID), &(gAcl->rownerID), sizeof(OicUuid_t));
            }
            free(tagName);
            tagName = NULL;
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

    if(NULL != subjectTag)
    {
        free(subjectTag);
        subjectTag = NULL;
    }

    if (NULL != rMapName)
    {
        free(rMapName);
        rMapName = NULL;
    }

    free(tagName);

    return acl;
}

// This function converts CBOR format to ACL data.
// Caller needs to invoke 'OICFree' on returned value when done using
// note: This function is used in unit test hence not declared static.
OicSecAcl_t* CBORPayloadToAcl(const uint8_t *cborPayload, const size_t size)
{
    return CBORPayloadToAclVersionOpt(cborPayload, size, NULL);
}

#ifdef MULTIPLE_OWNER
bool IsValidAclAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, const size_t size)
{
    bool retValue = false;
    OicSecAcl_t* acl = NULL;

    VERIFY_NOT_NULL(TAG, uuid, ERROR);
    VERIFY_NOT_NULL(TAG, cborPayload, ERROR);
    VERIFY_SUCCESS(TAG, 0 != size, ERROR);

    acl = CBORPayloadToAcl(cborPayload, size);
    VERIFY_NOT_NULL(TAG, acl, ERROR);

    OicSecAce_t* ace = NULL;
    OicSecAce_t* tempAce = NULL;
    LL_FOREACH_SAFE(acl->aces, ace, tempAce)
    {
        OicSecRsrc_t* rsrc = NULL;
        OicSecRsrc_t* tempRsrc = NULL;

        VERIFY_NOT_NULL(TAG, ace->eownerID, ERROR);
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
OCStackResult RemoveACE(const OicUuid_t *subject, const char *resource)
{
    OIC_LOG(DEBUG, TAG, "IN RemoveACE");

    VERIFY_NOT_NULL_RETURN(TAG, gAcl, ERROR, OC_STACK_INVALID_PARAM);

    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;
    bool deleteFlag = false;
    OCStackResult ret = OC_STACK_NO_RESOURCE;

    if (memcmp(subject, &WILDCARD_SUBJECT_ID, sizeof(*subject)) == 0)
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
            if (OC_STACK_OK == AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &payload, &size))
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
            if (sizeof(strUuid) < parseIter.valLen)
            {
                OIC_LOG(ERROR, TAG, "Uuid is too long");
                goto exit;
            }

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
                if (rsrc1 && rsrc2 &&
                    rsrc1->href && rsrc2->href)
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
        if(ace1->subjectType != ace2->subjectType)
        {
            return false;
        }
        if(ace1->subjectType == OicSecAceUuidSubject &&
            memcmp(ace1->subjectuuid.id, ace2->subjectuuid.id, sizeof(ace1->subjectuuid.id)) != 0)
        {
            return false;
        }
        else if(ace1->subjectType == OicSecAceRoleSubject &&
            strcmp(ace1->subjectRole.id, ace2->subjectRole.id) != 0 &&
            strcmp(ace1->subjectRole.authority, ace2->subjectRole.authority) != 0)
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
    ret = AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &aclBackup, &backupSize);
    if(OC_STACK_OK == ret)
    {
        // Remove all ACE from ACL
        LL_FOREACH_SAFE(gAcl->aces, aceItem, tempAce)
        {
            LL_DELETE(gAcl->aces, aceItem);
            FreeACE(aceItem);
        }

        //Generate empty ACL payload
        ret = AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &payload, &size);
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

static OCEntityHandlerResult HandleACLGetRequest(const OCEntityHandlerRequest *ehRequest, OicSecAclVersion_t aclVersion)
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

        if ( NULL != gAcl )
        {
            memcpy(&targetAcl.rownerID, &gAcl->rownerID, sizeof(OicUuid_t));
        }
        else
        {
            OIC_LOG_V(WARNING, TAG, "%s: gAcl is NULL", __func__);
        }

        targetAcl.aces = NULL;

        // 'Subject' field is MUST for processing a querystring in REST request.
        GetResourceFromQueryString(ehRequest->query, resource, sizeof(resource));

        /*
         * TODO : Currently, this code only provides one ACE for a Subject.
         * Below code needs to be updated for scenarios when Subject have
         * multiple ACE's in ACL resource.
         */
        for (currentAce = GetACLResourceData(&subject, &savePtr);
             NULL != currentAce;
             currentAce = GetACLResourceData(&subject, &savePtr))
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
                    if (0 == strcmp(rsrc->href, resource) ||
                        0 == strcmp(WILDCARD_RESOURCE_URI, rsrc->href))
                    {
                        // Convert ACL data into CBOR format for transmission
                        if (OC_STACK_OK != AclToCBORPayload(&targetAcl, aclVersion, &payload, &size))
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
                if (OC_STACK_OK != AclToCBORPayload(&targetAcl, aclVersion, &payload, &size))
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
        if (OC_STACK_OK != AclToCBORPayload(gAcl, aclVersion, &payload, &size))
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
    OCEntityHandlerResult ehRet = OC_EH_INTERNAL_SERVER_ERROR;

    // Convert CBOR into ACL data and update to SVR buffers. This will also validate the ACL data received.
    uint8_t *payload = ((OCSecurityPayload *) ehRequest->payload)->securityData;
    size_t size = ((OCSecurityPayload *) ehRequest->payload)->payloadSize;

    OicSecDostype_t dos;
    VERIFY_SUCCESS(TAG, OC_STACK_OK == GetDos(&dos), ERROR);
    ehRet = OC_EH_OK;

    if ((DOS_RESET == dos.state) ||
        (DOS_RFNOP == dos.state))
    {
        OIC_LOG_V(WARNING, TAG, "%s /acl resource is read-only in RESET and RFNOP.", __func__);
        ehRet = OC_EH_NOT_ACCEPTABLE;
        goto exit;
    }
    else
    {
        ehRet = OC_EH_OK;
    }

    ehRet = OC_EH_OK;

    if (payload)
    {
        // Clients should not POST v1 ACL to OCF 1.0 Server
        OicSecAclVersion_t payloadVersionReceived = OIC_SEC_ACL_V1;
        CBORPayloadToAclVersionOpt(payload, size, &payloadVersionReceived);
        if (OIC_SEC_ACL_V2 != payloadVersionReceived)
        {
            OIC_LOG_V(WARNING, TAG, "%s /acl Resource is v2; POST of v1 ACL not acceptable.", __func__);
            ehRet = OC_EH_NOT_ACCEPTABLE;
            goto exit;
        }
        OicSecAcl_t *newAcl = NULL;
        OIC_LOG(DEBUG, TAG, "ACL payload from POST request << ");
        OIC_LOG_BUFFER(DEBUG, TAG, payload, size);

        newAcl = CBORPayloadToAcl(payload, size);
        if (NULL != newAcl && NULL != gAcl)
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
                    OIC_LOG(DEBUG, TAG, "NEW ACE detected:");

                    OicSecAce_t* insertAce = DuplicateACE(newAce);
                    if(insertAce)
                    {
                        OIC_LOG(DEBUG, TAG, "Prepending new ACE:");
                        OIC_LOG_ACE(DEBUG, insertAce);
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
                size_t cborSize = 0;
                uint8_t *cborPayload = NULL;

                if (OC_STACK_OK == AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &cborPayload, &cborSize))
                {
                    if (UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, cborPayload, cborSize) == OC_STACK_OK)
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
        else
        {
            OIC_LOG_V(WARNING, TAG, "%s: %s", __func__, (NULL == newAcl) ? "no new ACL" : "gAcl is NULL");
        }
    }
    else
    {
        OIC_LOG(ERROR, TAG, "ACL post request with no payload.");
        ehRet = OC_EH_ERROR;
    }

exit:

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

    VERIFY_NOT_NULL(TAG, ehRequest->query, ERROR);

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
                ehRet = HandleACLGetRequest(ehRequest, OIC_SEC_ACL_V2);
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

OCEntityHandlerResult ACL2EntityHandler(OCEntityHandlerFlag flag, OCEntityHandlerRequest * ehRequest,
        void* callbackParameter)
{
    OIC_LOG(DEBUG, TAG, "Received request ACL2EntityHandler");
    OC_UNUSED(callbackParameter);
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
                ehRet = HandleACLGetRequest(ehRequest, OIC_SEC_ACL_V2);
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
 * This internal method is used to create the '/oic/sec/acl' and '/oic/sec/acl2' resources.
 */
static OCStackResult CreateACLResource()
{
    OCStackResult ret;

    ret = OCCreateResource(&gAcl2Handle,
                           OIC_RSRC_TYPE_SEC_ACL2,
                           OC_RSRVD_INTERFACE_DEFAULT,
                           OIC_RSRC_ACL2_URI,
                           ACL2EntityHandler,
                           NULL,
                           OC_SECURE |
                           OC_DISCOVERABLE);

    if (OC_STACK_OK != ret)
    {
        OIC_LOG(FATAL, TAG, "Unable to instantiate ACL2 resource");
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
    OicSecAce_t *readOnlyAceAnon = NULL;
    OicSecAce_t *readOnlyAceAuth = NULL;
    OicSecAce_t *readWriteDeleteAceAnon = NULL;
    OicSecAce_t *readWriteDeleteAceAuth = NULL;
    OicSecRsrc_t* resRsrc = NULL;
    OicSecRsrc_t* deviceRsrc = NULL;
    OicSecRsrc_t* platformRsrc = NULL;
    OicSecRsrc_t* doxmRsrc = NULL;
    OicSecRsrc_t* rolesRsrc = NULL;

    /*
     * Note that all Ace_t and Rsrc_t objects will be freed on error by
     * DeleteACLList(acl). We LL_APPEND these objects to the acl object as soon
     * as they are allocated.
     */

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
    VERIFY_NOT_NULL(TAG, acl, ERROR);

    // ACE allowing read-only access to /res, /d and /p by "ANON_CLEAR" subjects
    readOnlyAceAnon = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NOT_NULL(TAG, readOnlyAceAnon, ERROR);
    readOnlyAceAnon->aceid = 1;
    readOnlyAceAnon->permission = PERMISSION_READ;
    readOnlyAceAnon->validities = NULL;
    LL_APPEND(acl->aces, readOnlyAceAnon);

    // Subject is conntype "ANON_CLEAR" (e.g. CoAP) wildcard
    readOnlyAceAnon->subjectType = OicSecAceConntypeSubject;
    readOnlyAceAnon->subjectConn = ANON_CLEAR;

    // Resources are /res, /d and /p
    // /oic/res
    resRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, resRsrc, ERROR);
    LL_APPEND(readOnlyAceAnon->resources, resRsrc);
    resRsrc->href = OICStrdup(OC_RSRVD_WELL_KNOWN_URI);
    VERIFY_NOT_NULL(TAG, (resRsrc->href), ERROR);

    // /oic/d
    deviceRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, deviceRsrc, ERROR);
    LL_APPEND(readOnlyAceAnon->resources, deviceRsrc);
    deviceRsrc->href = OICStrdup(OC_RSRVD_DEVICE_URI);
    VERIFY_NOT_NULL(TAG, (deviceRsrc->href), ERROR);

    // /oic/p
    platformRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, platformRsrc, ERROR);
    LL_APPEND(readOnlyAceAnon->resources, platformRsrc);
    platformRsrc->href = OICStrdup(OC_RSRVD_PLATFORM_URI);
    VERIFY_NOT_NULL(TAG, (platformRsrc->href), ERROR);

    // ACE allowing read-only access to /res, /d and /p by "AUTH_CRYPT" subjects
    readOnlyAceAuth = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NOT_NULL(TAG, readOnlyAceAuth, ERROR);
    readOnlyAceAuth->aceid = 2;
    readOnlyAceAuth->permission = PERMISSION_READ;
    readOnlyAceAuth->validities = NULL;
    LL_APPEND(acl->aces, readOnlyAceAuth);

    // Subject is conntype "AUTH_CRYPT" (e.g. CoAPS) wildcard
    readOnlyAceAuth->subjectType = OicSecAceConntypeSubject;
    readOnlyAceAuth->subjectConn = AUTH_CRYPT;

    // Resources are /res, /d and /p
    // /oic/res
    resRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, resRsrc, ERROR);
    LL_APPEND(readOnlyAceAuth->resources, resRsrc);
    resRsrc->href = OICStrdup(OC_RSRVD_WELL_KNOWN_URI);
    VERIFY_NOT_NULL(TAG, (resRsrc->href), ERROR);

    // /oic/d
    deviceRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, deviceRsrc, ERROR);
    LL_APPEND(readOnlyAceAuth->resources, deviceRsrc);
    deviceRsrc->href = OICStrdup(OC_RSRVD_DEVICE_URI);
    VERIFY_NOT_NULL(TAG, (deviceRsrc->href), ERROR);

    // /oic/p
    platformRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, platformRsrc, ERROR);
    LL_APPEND(readOnlyAceAuth->resources, platformRsrc);
    platformRsrc->href = OICStrdup(OC_RSRVD_PLATFORM_URI);
    VERIFY_NOT_NULL(TAG, (platformRsrc->href), ERROR);

    // ACE allowing read, write and delete access to /doxm,
    // to "ANON_CLEAR" (e.g. CoAP) subjects, for ownership transfer
    readWriteDeleteAceAnon = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NOT_NULL(TAG, readWriteDeleteAceAnon, ERROR);
    readWriteDeleteAceAnon->aceid = 3;
    readWriteDeleteAceAnon->permission = PERMISSION_READ | PERMISSION_WRITE | PERMISSION_DELETE;
    readWriteDeleteAceAnon->validities = NULL;
    LL_APPEND(acl->aces, readWriteDeleteAceAnon);

    // Subject is conntype "ANON_CLEAR" (e.g. CoAP) wildcard
    readWriteDeleteAceAnon->subjectType = OicSecAceConntypeSubject;
    readWriteDeleteAceAnon->subjectConn = ANON_CLEAR;

    // Resource is /doxm
    // /oic/sec/doxm
    doxmRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, doxmRsrc, ERROR);
    LL_APPEND(readWriteDeleteAceAnon->resources, doxmRsrc);
    doxmRsrc->href = OICStrdup(OIC_RSRC_DOXM_URI);
    VERIFY_NOT_NULL(TAG, (doxmRsrc->href), ERROR);

    // ACE allowing read, write and delete access to /doxm and /roles,
    // to "AUTH_CRYPT" (e.g. CoAPS) subjects, for ownership transfer
    readWriteDeleteAceAuth = (OicSecAce_t *) OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NOT_NULL(TAG, readWriteDeleteAceAuth, ERROR);
    readWriteDeleteAceAuth->aceid = 4;
    readWriteDeleteAceAuth->permission = PERMISSION_READ | PERMISSION_WRITE | PERMISSION_DELETE;
    readWriteDeleteAceAuth->validities = NULL;
    LL_APPEND(acl->aces, readWriteDeleteAceAuth);

    // Subject is conntype "AUTH_CRYPT" (e.g. CoAPS) wildcard
    readWriteDeleteAceAuth->subjectType = OicSecAceConntypeSubject;
    readWriteDeleteAceAuth->subjectConn = AUTH_CRYPT;

    // Resources are /doxm and /roles
    // /oic/sec/doxm
    doxmRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, doxmRsrc, ERROR);
    LL_APPEND(readWriteDeleteAceAuth->resources, doxmRsrc);
    doxmRsrc->href = OICStrdup(OIC_RSRC_DOXM_URI);
    VERIFY_NOT_NULL(TAG, (doxmRsrc->href), ERROR);

    // /oic/sec/roles
    rolesRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, rolesRsrc, ERROR);
    LL_APPEND(readWriteDeleteAceAuth->resources, rolesRsrc);
    rolesRsrc->href = OICStrdup(OIC_RSRC_ROLES_URI);
    VERIFY_NOT_NULL(TAG, (rolesRsrc->href), ERROR);

    // Device ID is the owner of this default ACL
    if (GetDoxmResourceData() != NULL)
    {
        ret = GetDoxmDeviceID(&ownerId);
        VERIFY_SUCCESS(TAG, OC_STACK_OK == ret, FATAL);
    }
    else
    {
        VERIFY_SUCCESS(TAG, OCGenerateUuid(ownerId.id), FATAL);
    }

    memcpy(&acl->rownerID, &ownerId, sizeof(OicUuid_t));

    *defaultAcl = acl;
    ret = OC_STACK_OK;

exit:

    if (ret != OC_STACK_OK)
    {
        DeleteACLList(acl);
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
    VERIFY_NOT_NULL(TAG, gAcl, FATAL);

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
    OCStackResult ret2 = OCDeleteResource(gAcl2Handle);
    gAcl2Handle = NULL;

    if (gAcl)
    {
        DeleteACLList(gAcl);
        gAcl = NULL;
    }
    return (OC_STACK_OK != ret) ? ret : ret2;
}

const OicSecAce_t* GetACLResourceData(const OicUuid_t* subjectId, OicSecAce_t **savePtr)
{
    OicSecAce_t *ace = NULL;
    OicSecAce_t *begin = NULL;

    if (NULL == subjectId || NULL == savePtr || NULL == gAcl)
    {
        return NULL;
    }

    OIC_LOG(DEBUG, TAG, "GetACLResourceData: searching for ACE matching subject:");
    OIC_LOG_BUFFER(DEBUG, TAG, subjectId->id, sizeof(subjectId->id));

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
        if ((OicSecAceUuidSubject == ace->subjectType) &&
            (0 == memcmp(&(ace->subjectuuid), subjectId, sizeof(OicUuid_t))))
        {
            OIC_LOG(DEBUG, TAG, "GetACLResourceData: found matching ACE:");
            OIC_LOG_ACE(DEBUG, ace);
            *savePtr = ace;
            return ace;
        }
    }

    // Cleanup in case no ACL is found
    *savePtr = NULL;
    return NULL;
}

const OicSecAce_t* GetACLResourceDataByRoles(const OicSecRole_t *roles, size_t roleCount, OicSecAce_t **savePtr)
{
    OicSecAce_t *ace = NULL;
    OicSecAce_t *begin = NULL;

    if ((NULL == savePtr) || (NULL == gAcl))
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters to GetACLResourceDataByRoles");
        return NULL;
    }

    if ((NULL == roles) || (0 == roleCount))
    {
        /* Not an error; just nothing to do. */
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

    // Find the next ACL corresponding to the 'roleID' and return it.
    LL_FOREACH(begin, ace)
    {
        if (OicSecAceRoleSubject == ace->subjectType)
        {
            for (size_t i = 0; i < roleCount; i++)
            {
                if ((0 == strcmp(ace->subjectRole.id, roles[i].id) &&
                    (0 == strcmp(ace->subjectRole.authority, roles[i].authority))))
                {
                    *savePtr = ace;
                    return ace;
                }
            }
        }
    }

    // Cleanup in case no ACL is found
    *savePtr = NULL;
    return NULL;
}

const OicSecAce_t* GetACLResourceDataByConntype(const OicSecConntype_t conntype, OicSecAce_t **savePtr)
{
    OicSecAce_t *ace = NULL;
    OicSecAce_t *begin = NULL;

    OIC_LOG_V(DEBUG, TAG, "IN: %s(%d)", __func__, conntype);

    if ((NULL == savePtr) || (NULL == gAcl))
    {
        OIC_LOG(ERROR, TAG, "Invalid parameters to GetACLResourceDataByConntype");
        return NULL;
    }

    // savePtr MUST point to NULL if this is the 'first' call to retrieve ACL.
    if (NULL == *savePtr)
    {
        begin = gAcl->aces;
    }
    else
    {
        // If this is a 'successive' call, search for location pointed by
        // savePtr and assign 'begin' to the next ACE after it in the linked
        // list and start searching from there.
        LL_FOREACH(gAcl->aces, ace)
        {
            if (ace == *savePtr)
            {
                begin = ace->next;
            }
        }
    }

    // Find the next ACE corresponding to the conntype, and return it.
    LL_FOREACH(begin, ace)
    {
        if (OicSecAceConntypeSubject == ace->subjectType)
        {
            if (conntype == ace->subjectConn)
            {
                *savePtr = ace;
                return ace;
            }
        }
    }

    // Cleanup in case no ACE is found
    *savePtr = NULL;

    OIC_LOG_V(DEBUG, TAG, "OUT: %s(%d)", __func__, conntype);

    return NULL;
}

OCStackResult AppendACLObject(const OicSecAcl_t* acl)
{
    OCStackResult ret = OC_STACK_ERROR;

    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    VERIFY_NOT_NULL_RETURN(TAG, gAcl, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, acl, ERROR, OC_STACK_INVALID_PARAM);

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

    OIC_LOG_ACL(INFO, gAcl);

    size_t size = 0;
    uint8_t *payload = NULL;
    ret = AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &payload, &size);
    if (OC_STACK_OK == ret)
    {
        ret = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, payload, size);
        OICFree(payload);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

    return ret;
}

OCStackResult AppendACL(const uint8_t *cborPayload, const size_t size)
{
    // Convert CBOR format to ACL data. This will also validate the ACL data received.
    OicSecAcl_t* newAcl = CBORPayloadToAcl(cborPayload, size);

    return AppendACLObject(newAcl);
}

OCStackResult InstallACL(const OicSecAcl_t* acl)
{
    OIC_LOG_V(DEBUG, TAG, "IN: %s", __func__);

    OCStackResult ret = OC_STACK_ERROR;

    VERIFY_NOT_NULL_RETURN(TAG, gAcl, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, acl, ERROR, OC_STACK_INVALID_PARAM);

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
                OIC_LOG(DEBUG, TAG, "Duplicated ACE detected.");
                ret = OC_STACK_DUPLICATE_REQUEST;
                isNewAce = false;
            }
        }
        if(isNewAce)
        {
            // Append new ACE to existing ACL
            OIC_LOG(DEBUG, TAG, "NEW ACE detected.");

            OicSecAce_t* insertAce = DuplicateACE(newAce);
            if(insertAce)
            {
                OIC_LOG(DEBUG, TAG, "Prepending new ACE:");
                OIC_LOG_ACE(DEBUG, insertAce);

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
        ret = AppendACLObject(newInstallAcl);
        if (OC_STACK_OK != ret)
        {
            OIC_LOG(ERROR, TAG, "Failed to append ACL");
        }
        OICFree(newInstallAcl);
    }

    OIC_LOG_V(DEBUG, TAG, "OUT: %s", __func__);

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

#ifdef _MSC_VER
    // OIC_RSRC_TYPE_SEC_DOXM and OIC_RSRC_TYPE_SEC_PSTAT are const but also extern so they cause warnings.
#pragma warning(push)
#pragma warning(disable:4204)
#endif

    const char *doxmRt[] = { OIC_RSRC_TYPE_SEC_DOXM };
    const char *pstatRt[] = { OIC_RSRC_TYPE_SEC_PSTAT };

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    const char *doxmIf[] = { OC_RSRVD_INTERFACE_DEFAULT };
    const char *pstatIf[] = { OC_RSRVD_INTERFACE_DEFAULT };
    OicSecRsrc_t* doxmRsrc = NULL;
    OicSecRsrc_t* pstatRsrc = NULL;

    //Generate default ACE
    OicSecAce_t* newAce = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
    VERIFY_NOT_NULL(TAG, newAce, ERROR);

    // Subject -- Mandatory
    newAce->subjectType = OicSecAceUuidSubject;
    memcpy(&newAce->subjectuuid, &WILDCARD_SUBJECT_ID, WILDCARD_SUBJECT_ID_LEN);

    //Resources -- Mandatory
    //Add doxm
    doxmRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, doxmRsrc, ERROR);
    LL_APPEND(newAce->resources, doxmRsrc);
    // pstat-href
    doxmRsrc->href = OICStrdup(OIC_RSRC_DOXM_URI);
    VERIFY_NOT_NULL(TAG, (doxmRsrc->href), ERROR);
    // pstat-rt
    doxmRsrc->typeLen = NUM_OF_DOXM_RT;
    doxmRsrc->types = (char**)OICCalloc(NUM_OF_DOXM_RT, sizeof(char*));
    VERIFY_NOT_NULL(TAG, (doxmRsrc->types), ERROR);
    for(int i = 0; i < NUM_OF_DOXM_RT; i++)
    {
        doxmRsrc->types[i] = OICStrdup(doxmRt[i]);
        VERIFY_NOT_NULL(TAG, (doxmRsrc->types[i]), ERROR);
    }
    // pstat-if
    doxmRsrc->interfaceLen = NUM_OF_DOXM_IF;
    doxmRsrc->interfaces = (char**)OICCalloc(NUM_OF_DOXM_IF, sizeof(char*));
    VERIFY_NOT_NULL(TAG, (doxmRsrc->interfaces), ERROR);
    for(int i = 0; i < NUM_OF_DOXM_IF; i++)
    {
        doxmRsrc->interfaces[i] = OICStrdup(doxmIf[i]);
        VERIFY_NOT_NULL(TAG, (doxmRsrc->interfaces[i]), ERROR);
    }

    //Add pstat
    pstatRsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
    VERIFY_NOT_NULL(TAG, pstatRsrc, ERROR);
    LL_APPEND(newAce->resources, pstatRsrc);
    //pstat-href
    pstatRsrc->href = OICStrdup(OIC_RSRC_PSTAT_URI);
    VERIFY_NOT_NULL(TAG, (pstatRsrc->href), ERROR);
    //pstat-rt
    pstatRsrc->typeLen = NUM_OF_PSTAT_RT;
    pstatRsrc->types = (char**)OICCalloc(NUM_OF_PSTAT_RT, sizeof(char*));
    VERIFY_NOT_NULL(TAG, (pstatRsrc->types), ERROR);
    for(int i = 0; i < NUM_OF_PSTAT_RT; i++)
    {
        pstatRsrc->types[i] = OICStrdup(pstatRt[i]);
        VERIFY_NOT_NULL(TAG, (pstatRsrc->types[i]), ERROR);
    }
    // pstat-if
    pstatRsrc->interfaceLen = NUM_OF_PSTAT_IF;
    pstatRsrc->interfaces = (char**)OICCalloc(NUM_OF_PSTAT_IF, sizeof(char*));
    VERIFY_NOT_NULL(TAG, (pstatRsrc->interfaces), ERROR);
    for(int i = 0; i < NUM_OF_PSTAT_IF; i++)
    {
        pstatRsrc->interfaces[i] = OICStrdup(pstatIf[i]);
        VERIFY_NOT_NULL(TAG, (pstatRsrc->interfaces[i]), ERROR);
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
            if(OicSecAceUuidSubject == ace->subjectType &&
                memcmp(&ace->subjectuuid, &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0 &&
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
                if (OC_STACK_OK == AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &payload, &size))
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

        ret = AclToCBORPayload(gAcl, OIC_SEC_ACL_V2, &cborPayload, &size);
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
    if (gAcl && rowneruuid)
    {
        memcpy(&(rowneruuid->id), &(gAcl->rownerID.id), sizeof(rowneruuid->id));
        return OC_STACK_OK;
    }
    return OC_STACK_ERROR;
}
