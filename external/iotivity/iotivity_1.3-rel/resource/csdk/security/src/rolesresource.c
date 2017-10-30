//******************************************************************
//
// Copyright 2017 Microsoft
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
//******************************************************************

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "iotivity_config.h"
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "oic_string.h"
#include "cainterface.h"
#include "payload_logging.h"
#include "ocstack.h"
#include "ocrandom.h"
#include "cacommon.h"
#include "srmresourcestrings.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "credresource.h"
#include "doxmresource.h"
#include "srmutility.h"
#include "certhelpers.h"
#include "resourcemanager.h"
#include "utlist.h"
#include "ca_adapter_net_ssl.h"
#include "ocstackinternal.h"
#include "rolesresource.h"
#include "secureresourcemanager.h"

#define TAG  "OIC_SRM_ROLES"

typedef struct RolesEntry {
    uint8_t                 *publicKey;         /**< DER-encoded public key */
    size_t                  publicKeyLength;    /**< length of publicKey */
    RoleCertChain_t         *chains;            /**< stored certificates */

    struct tm               cacheValidUntil;    /**< Cache valid until time; use 0 if cache is not yet set */
    OicSecRole_t            *cachedRoles;       /**< Cached roles; must free with OICFree */
    size_t                  cachedRolesLength;  /**< Length of cachedRoles array */

    struct RolesEntry       *next;
} RolesEntry_t;

typedef struct SymmetricRoleEntry {
    OicUuid_t                 subject;          /**< Subject of the symmetric credential */
    OicSecRole_t              role;             /**< Role of the symmetric credential */

    struct SymmetricRoleEntry *next;
} SymmetricRoleEntry_t;

static OCResourceHandle     gRolesHandle        = NULL;
static RolesEntry_t         *gRoles             = NULL;
static SymmetricRoleEntry_t *gSymmetricRoles    = NULL;
static uint32_t             gIdCounter          = 1;

/**
 * Default cbor payload size. This value is increased in case of CborErrorOutOfMemory.
 * The value of payload size is increased until reaching max cbor size.
 */
static const uint16_t CBOR_SIZE = 2048;

/**
 * Zero subject UUID.
 */
static const char EMPTY_UUID[] = "00000000-0000-0000-0000-000000000000";

/**
 * Mandatory parts of a role map cred entry: 4 key/value pairs: credId, subject, publicData, and credType.
 */
static const uint8_t ROLE_MAP_SIZE = 4;

static void InvalidateRoleCache(RolesEntry_t *entry)
{
    memset(&entry->cacheValidUntil, 0, sizeof(entry->cacheValidUntil));
    OICFree(entry->cachedRoles);
    entry->cachedRoles = NULL;
    entry->cachedRolesLength = 0;
}

/* Caller must call OICFree on publicKey when finished. */
static OCStackResult GetPeerPublicKeyFromEndpoint(const CAEndpoint_t *endpoint,
                                                  uint8_t **publicKey,
                                                  size_t *publicKeyLength)
{
    CASecureEndpoint_t sep;
    CAResult_t res = GetCASecureEndpointData(endpoint, &sep);
    if (CA_STATUS_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "%s: Failed to GetCASecureEndpointData: %d", __func__, res);
        return OC_STACK_INVALID_PARAM;
    }

    if ((NULL == sep.publicKey) || (0 == sep.publicKeyLength))
    {
        OIC_LOG_V(WARNING, TAG, "%s: Peer did not have a public key", __func__);
        return OC_STACK_NO_RESOURCE;
    }

    *publicKey = OICCalloc(1, sep.publicKeyLength);
    if (NULL == *publicKey)
    {
        OIC_LOG_V(ERROR, TAG, "%s: No memory for publicKey", __func__);
        return OC_STACK_NO_MEMORY;
    }

    memcpy(*publicKey, sep.publicKey, sep.publicKeyLength);
    *publicKeyLength = sep.publicKeyLength;

    return OC_STACK_OK;
}

/* Caller must call OICFree on publicKey when finished. */
static OCStackResult GetPeerPublicKey(const OCDevAddr *peer, uint8_t **publicKey, size_t *publicKeyLength)
{
    CAEndpoint_t endpoint;
    CopyDevAddrToEndpoint(peer, &endpoint);

    return GetPeerPublicKeyFromEndpoint(&endpoint, publicKey, publicKeyLength);
}

static void FreeRoleCertChain(RoleCertChain_t *roleCert)
{
    if (NULL == roleCert)
    {
        return;
    }

    OICFree(roleCert->certificate.data);
    OICFree(roleCert);
}

void FreeRoleCertChainList(RoleCertChain_t *roleCertList)
{
    if (NULL == roleCertList)
    {
        return;
    }

    RoleCertChain_t *certTmp1 = NULL;
    RoleCertChain_t *certTmp2 = NULL;
    LL_FOREACH_SAFE(roleCertList, certTmp1, certTmp2)
    {
        LL_DELETE(roleCertList, certTmp1);
        FreeRoleCertChain(certTmp1);
    }
}

static void FreeRolesEntry(RolesEntry_t *rolesEntry)
{
    if (NULL == rolesEntry)
    {
        return;
    }

    FreeRoleCertChainList(rolesEntry->chains);
    OICFree(rolesEntry->publicKey);
    OICFree(rolesEntry->cachedRoles);
    OICFree(rolesEntry);
}

static void FreeRolesList(RolesEntry_t *roles)
{
    if (NULL != roles)
    {
        RolesEntry_t *entryTmp1 = NULL;
        RolesEntry_t *entryTmp2 = NULL;
        LL_FOREACH_SAFE(roles, entryTmp1, entryTmp2)
        {
            LL_DELETE(roles, entryTmp1);
            FreeRolesEntry(entryTmp1);
        }
    }
}

static void FreeSymmetricRoleEntry(SymmetricRoleEntry_t *symRoleEntry)
{
    OICFree(symRoleEntry);
}

static void FreeSymmetricRolesList(SymmetricRoleEntry_t *head)
{
    if (NULL != head)
    {
        SymmetricRoleEntry_t *entryTmp1 = NULL;
        SymmetricRoleEntry_t *entryTmp2 = NULL;

        LL_FOREACH_SAFE(head, entryTmp1, entryTmp2)
        {
            LL_DELETE(head, entryTmp1);
            FreeSymmetricRoleEntry(entryTmp1);
        }
    }
}

static bool AddNullTerminator(OicSecKey_t *key)
{
    size_t length = key->len;
    uint8_t *data = key->data;

    if ((length > 0) && (data != NULL) && (data[length - 1] != 0))
    {
        key->data = OICRealloc(data, length + 1);

        if (key->data == NULL)
        {
            OIC_LOG_V(ERROR, TAG, "%s: OICRealloc failed", __func__);
            OICFree(data);
            key->len = 0;
            return false;
        }

        OIC_LOG(DEBUG, TAG, "Adding key null terminator");
        key->data[length] = 0;
        key->len++;
    }

    return true;
}

OCStackResult RegisterSymmetricCredentialRole(const OicSecCred_t *cred)
{
    VERIFY_NON_NULL_RET(cred, TAG, "Parameter cred is NULL", OC_STACK_INVALID_PARAM);
    VERIFY_SUCCESS_RETURN(TAG, (SYMMETRIC_PAIR_WISE_KEY == cred->credType), ERROR, OC_STACK_INVALID_PARAM);

    SymmetricRoleEntry_t *curr = NULL;

    LL_FOREACH(gSymmetricRoles, curr)
    {
        if (0 == memcmp(&cred->subject, &curr->subject, sizeof(curr->subject)))
        {
            if (!IsNonEmptyRole(&cred->roleId))
            {
                LL_DELETE(gSymmetricRoles, curr);
            }
            else
            {
                curr->role = cred->roleId;
            }

            return OC_STACK_OK;
        }
    }

    /* No entry found; add a new one if we're setting a role. */
    if (IsNonEmptyRole(&cred->roleId))
    {
        curr = (SymmetricRoleEntry_t *)OICCalloc(1, sizeof(SymmetricRoleEntry_t));
        if (NULL == curr)
        {
            OIC_LOG(ERROR, TAG, "No memory allocating new symmetric role entry");
            return OC_STACK_NO_MEMORY;
        }
        LL_APPEND(gSymmetricRoles, curr);
        curr->subject = cred->subject;
        curr->role = cred->roleId;
    }

    return OC_STACK_OK;
}

static OCStackResult DuplicateRoleCertChain(const RoleCertChain_t *roleCert, RoleCertChain_t **duplicate)
{
    OIC_LOG(DEBUG, TAG, "DuplicateRoleCertChain IN");

    OCStackResult res = OC_STACK_ERROR;
    RoleCertChain_t *tmp = NULL;

    if ((NULL == roleCert) || (NULL == duplicate))
    {
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    tmp = (RoleCertChain_t *)OICCalloc(1, sizeof(RoleCertChain_t));
    if (NULL == tmp)
    {
        OIC_LOG(ERROR, TAG, "No memory for tmp");
        res = OC_STACK_NO_MEMORY;
        goto exit;
    }

    tmp->certificate.data = (uint8_t *)OICCalloc(1, roleCert->certificate.len);
    if (NULL == tmp->certificate.data)
    {
        OIC_LOG(ERROR, TAG, "No memory for certificate data");
        res = OC_STACK_NO_MEMORY;
        goto exit;
    }
    tmp->credId = roleCert->credId;
    tmp->certificate.len = roleCert->certificate.len;
    tmp->certificate.encoding = roleCert->certificate.encoding;
    memcpy(tmp->certificate.data, roleCert->certificate.data, roleCert->certificate.len);

    *duplicate = tmp;
    res = OC_STACK_OK;

exit:

    OIC_LOG_V(DEBUG, TAG, "DuplicateRoleCertChain OUT: %d", res);

    if (OC_STACK_OK != res)
    {
        FreeRoleCertChain(tmp);
    }

    return res;
}

static bool RoleCertChainContains(RoleCertChain_t *chain, const RoleCertChain_t* roleCert)
{
    RoleCertChain_t *temp = NULL;

    LL_FOREACH(chain, temp)
    {
        if (IsSameSecKey(&temp->certificate, &roleCert->certificate))
        {
            return true;
        }
    }

    return false;
}

static OCStackResult AddRoleCertificate(const RoleCertChain_t *roleCert, const uint8_t *pubKey, size_t pubKeyLength)
{
    OCStackResult res = OC_STACK_ERROR;
    RolesEntry_t *targetEntry = NULL;
    RoleCertChain_t *copy = NULL;

    OIC_LOG(DEBUG, TAG, "AddRoleCertificate IN");

    if ((NULL == pubKey) || (0 == pubKeyLength))
    {
        assert(!"AddRoleCertificate called with no public key");
        res = OC_STACK_INVALID_PARAM;
        goto exit;
    }

    for (targetEntry = gRoles; NULL != targetEntry; targetEntry = targetEntry->next)
    {
        if ((targetEntry->publicKeyLength == pubKeyLength) &&
            (0 == memcmp(targetEntry->publicKey, pubKey, pubKeyLength)))
        {
            break;
        }
    }

    if (NULL != targetEntry)
    {
        InvalidateRoleCache(targetEntry);
    }
    else
    {
        /* We haven't seen this public key before and need a new entry. */
        targetEntry = (RolesEntry_t *)OICCalloc(1, sizeof(RolesEntry_t));
        if (NULL == targetEntry)
        {
            OIC_LOG(ERROR, TAG, "No memory for new targetEntry");
            res = OC_STACK_NO_MEMORY;
            goto exit;
        }
        targetEntry->publicKey = (uint8_t *)OICCalloc(1, pubKeyLength);
        if (NULL == targetEntry->publicKey)
        {
            OIC_LOG(ERROR, TAG, "No memory for new targetEntry public key");
            OICFree(targetEntry);
            res = OC_STACK_NO_MEMORY;
            goto exit;
        }
        targetEntry->publicKeyLength = pubKeyLength;
        memcpy(targetEntry->publicKey, pubKey, pubKeyLength);

        LL_PREPEND(gRoles, targetEntry);
    }

    if (!RoleCertChainContains(targetEntry->chains, roleCert))
    {
        res = DuplicateRoleCertChain(roleCert, &copy);
        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "%s: Could not duplicate role cert chain: %d", __func__, res);
            goto exit;
        }

        // Assign our own credId.
        copy->credId = gIdCounter++;
        LL_APPEND(targetEntry->chains, copy);
    }
    else
    {
        OIC_LOG_V(DEBUG, TAG, "%s: Role cert chain already present, not going to add it again", __func__);
    }

    res = OC_STACK_OK;

exit:

    if (OC_STACK_OK != res)
    {
        FreeRoleCertChain(copy);
    }

    OIC_LOG_V(DEBUG, TAG, "AddRoleCertificate Out: %d", res);

    return res;
}

OCStackResult RolesToCBORPayload(const RoleCertChain_t *roles, uint8_t **cborPayload,
                                        size_t *cborSize)
{
    OCStackResult ret = OC_STACK_OK;

    CborError cborEncoderResult = CborNoError;
    uint8_t *outPayload = NULL;
    CborEncoder encoder;
    CborEncoder rolesRootMap;
    CborEncoder rolesArray;
    size_t roleCount = 0;
    const RoleCertChain_t *currChain = NULL;

    VERIFY_NOT_NULL_RETURN(TAG, cborPayload, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, cborSize, ERROR, OC_STACK_INVALID_PARAM);

    size_t cborLen = *cborSize;
    *cborSize = 0;
    *cborPayload = NULL;

    if (0 == cborLen)
    {
        cborLen = CBOR_SIZE;
    }

    outPayload = (uint8_t *)OICCalloc(1, cborLen);
    VERIFY_NOT_NULL_RETURN(TAG, outPayload, ERROR, OC_STACK_NO_MEMORY);
    cbor_encoder_init(&encoder, outPayload, cborLen, 0);

    // Create roles root map (roles, rt, if)
    cborEncoderResult = cbor_encoder_create_map(&encoder, &rolesRootMap, 3);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding roles root map");

    // Roles array
    cborEncoderResult = cbor_encode_text_string(&rolesRootMap, OIC_JSON_ROLES_NAME, strlen(OIC_JSON_ROLES_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding roles name tag");
    VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for roles name tag")

    // If roles is NULL, the "roles" array will be empty
    for (currChain = roles; NULL != currChain; currChain = currChain->next)
    {
        roleCount++;
    }

    cborEncoderResult = cbor_encoder_create_array(&rolesRootMap, &rolesArray, roleCount);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding roles array");

    for (currChain = roles; NULL != currChain; currChain = currChain->next)
    {
        CborEncoder roleMap;
        size_t mapSize = ROLE_MAP_SIZE;

        cborEncoderResult = cbor_encoder_create_map(&rolesArray, &roleMap, mapSize);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding role map");

        // credId - mandatory
        cborEncoderResult = cbor_encode_text_string(&roleMap, OIC_JSON_CREDID_NAME, strlen(OIC_JSON_CREDID_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding credId tag");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for credId tag")
        cborEncoderResult = cbor_encode_int(&roleMap, currChain->credId);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding credId value");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for credId value")

        // subjectuuid - mandatory - always zero for role certificates
        cborEncoderResult = cbor_encode_text_string(&roleMap, OIC_JSON_SUBJECTID_NAME, strlen(OIC_JSON_SUBJECTID_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding subject tag");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for subject tag")
        cborEncoderResult = cbor_encode_text_string(&roleMap, EMPTY_UUID, sizeof(EMPTY_UUID) - 1);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding subject value");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for subject value")

        // publicData - mandatory
        cborEncoderResult = SerializeEncodingToCbor(&roleMap, OIC_JSON_PUBLICDATA_NAME, &currChain->certificate);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding publicData");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for publicData")

        // credType - mandatory
        cborEncoderResult = cbor_encode_text_string(&roleMap, OIC_JSON_CREDTYPE_NAME, strlen(OIC_JSON_CREDTYPE_NAME));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding credType tag");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for  credType tag")
        // Per security spec, only SIGNED_ASYMMETRIC_KEY is supported here.
        cborEncoderResult = cbor_encode_int(&roleMap, SIGNED_ASYMMETRIC_KEY);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed adding credType value");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for credType value")

        cborEncoderResult = cbor_encoder_close_container(&rolesArray, &roleMap);
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed closing role map");
    }

    cborEncoderResult = cbor_encoder_close_container(&rolesRootMap, &rolesArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed closing roles array");

    // RT -- Mandatory
    CborEncoder rtArray;
    cborEncoderResult = cbor_encode_text_string(&rolesRootMap, OIC_JSON_RT_NAME,
        strlen(OIC_JSON_RT_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Name Tag.");
    VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for RT Name Tag")
    cborEncoderResult = cbor_encoder_create_array(&rolesRootMap, &rtArray, 1);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding RT Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&rtArray, OIC_RSRC_TYPE_SEC_ROLES,
            strlen(OIC_RSRC_TYPE_SEC_ROLES));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding RT Value.");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for RT Value")
    }
    cborEncoderResult = cbor_encoder_close_container(&rolesRootMap, &rtArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing RT.");

    // IF-- Mandatory
    CborEncoder ifArray;
    cborEncoderResult = cbor_encode_text_string(&rolesRootMap, OIC_JSON_IF_NAME,
        strlen(OIC_JSON_IF_NAME));
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Name Tag.");
    VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for IF Name Tag")
    cborEncoderResult = cbor_encoder_create_array(&rolesRootMap, &ifArray, 1);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Addding IF Value.");
    for (size_t i = 0; i < 1; i++)
    {
        cborEncoderResult = cbor_encode_text_string(&ifArray, OC_RSRVD_INTERFACE_DEFAULT,
            strlen(OC_RSRVD_INTERFACE_DEFAULT));
        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Adding IF Value.");
        VERIFY_CBOR_NOT_OUTOFMEMORY(TAG, cborEncoderResult, "Not enough memory for IF Value")
    }
    cborEncoderResult = cbor_encoder_close_container(&rolesRootMap, &ifArray);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed Closing IF.");

    // Close roles root map
    cborEncoderResult = cbor_encoder_close_container(&encoder, &rolesRootMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborEncoderResult, "Failed closing roles root map");

    *cborSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
    *cborPayload = outPayload;

exit:
    if (CborErrorOutOfMemory == cborEncoderResult)
    {
        OIC_LOG(DEBUG, TAG, "RolesToCBORPayload:CborErrorOutOfMemory : retry with more memory");

        // reallocate and try again!
        OICFree(outPayload);
        // Since the initially-allocated memory failed, double the memory.
        cborLen *= 2;
        OIC_LOG_V(DEBUG, TAG, "Roles reallocation size: %" PRIuPTR ".", cborLen);
        ret = RolesToCBORPayload(roles, cborPayload, &cborLen);
        *cborSize = cborLen;
    }
    else if (cborEncoderResult != CborNoError)
    {
        OIC_LOG(ERROR, TAG, "Failed to RolesToCBORPayload");
        OICFree(outPayload);
        *cborSize = 0;
        *cborPayload = NULL;
        ret = OC_STACK_ERROR;
    }

    return ret;
}

/* Caller must call FreeRoleCertChainList on roleEntries when finished. */
OCStackResult CBORPayloadToRoles(const uint8_t *cborPayload, size_t size, RoleCertChain_t **roleEntries)
{
    if (NULL == cborPayload || 0 == size || NULL == roleEntries)
    {
        return OC_STACK_INVALID_PARAM;
    }

    CborValue rolesCbor = { .parser = NULL };
    CborParser parser = { .end = NULL };
    CborError cborFindResult = CborNoError;
    RoleCertChain_t *headRoleCertChain = NULL;
    char* tagName = NULL;
    size_t len = 0;

    cborFindResult = cbor_parser_init(cborPayload, size, 0, &parser, &rolesCbor);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to initialize parser.");

    if (!cbor_value_is_container(&rolesCbor))
    {
        return OC_STACK_ERROR;
    }

    // Enter roles Root Map
    CborValue rolesRootMap;
    memset(&rolesRootMap, 0, sizeof(rolesRootMap));
    cborFindResult = cbor_value_enter_container(&rolesCbor, &rolesRootMap);
    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed Entering roles Root Map.");

    while (cbor_value_is_valid(&rolesRootMap))
    {
        if (NULL != tagName)
        {
            free(tagName);
            tagName = NULL;
        }
        len = 0;
        CborType type = cbor_value_get_type(&rolesRootMap);
        if (type == CborTextStringType && cbor_value_is_text_string(&rolesRootMap))
        {
            cborFindResult = cbor_value_dup_text_string(&rolesRootMap, &tagName, &len, NULL);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed finding name in roles Root Map.");
            cborFindResult = cbor_value_advance(&rolesRootMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing value in roles Root Map.");
        }
        else
        {
            OIC_LOG_V(WARNING, TAG, "Value is not of type string, but of type %x", type);
        }

        if (NULL != tagName)
        {
            if (strcmp(tagName, OIC_JSON_ROLES_NAME) == 0)
            {
                // Enter role array
                int roleCount = 0;
                RoleCertChain_t *currEntry = NULL;
                CborValue roleArray;
                memset(&roleArray, 0, sizeof(roleArray));

                cborFindResult = cbor_value_enter_container(&rolesRootMap, &roleArray);
                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed entering role array.");

                while (cbor_value_is_valid(&roleArray))
                {
                    roleCount++;
                    CborValue roleMap;
                    memset(&roleMap, 0, sizeof(roleMap));
                    cborFindResult = cbor_value_enter_container(&roleArray, &roleMap);
                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed entering role map.");

                    if (NULL == currEntry)
                    {
                        assert(NULL == headRoleCertChain);
                        headRoleCertChain = currEntry = (RoleCertChain_t *)OICCalloc(1, sizeof(RoleCertChain_t));
                        VERIFY_NOT_NULL(TAG, currEntry, ERROR);
                    }
                    else
                    {
                        assert(NULL != headRoleCertChain);
                        currEntry->next = (RoleCertChain_t *)OICCalloc(1, sizeof(RoleCertChain_t));
                        VERIFY_NOT_NULL(TAG, currEntry->next, ERROR);
                        currEntry = currEntry->next;
                    }

                    while (cbor_value_is_valid(&roleMap))
                    {
                        free(tagName);
                        tagName = NULL;
                        CborType innerType = cbor_value_get_type(&roleMap);
                        if (innerType == CborTextStringType)
                        {
                            cborFindResult = cbor_value_dup_text_string(&roleMap, &tagName, &len, NULL);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed finding name in role map.");
                            cborFindResult = cbor_value_advance(&roleMap);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing value in role map.");
                        }
                        if (NULL != tagName)
                        {
                            //credid
                            if (strcmp(tagName, OIC_JSON_CREDID_NAME) == 0)
                            {
                                uint64_t credId64 = 0;
                                cborFindResult = cbor_value_get_uint64(&roleMap, &credId64);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed retrieving credId.");
                                if (UINT32_MAX < credId64)
                                {
                                    cborFindResult = CborUnknownError;
                                    VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "credId was too large.");
                                }
                                currEntry->credId = (uint32_t)credId64;
                            }
                            else if (strcmp(tagName, OIC_JSON_PUBLICDATA_NAME) == 0)
                            {
                                cborFindResult = DeserializeEncodingFromCbor(&roleMap, &currEntry->certificate);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed to read publicData");

                                /* mbedtls_x509_crt_parse requires null string terminator */
                                VERIFY_TRUE_OR_EXIT(TAG, AddNullTerminator(&currEntry->certificate), ERROR);
                            }
                            else if (strcmp(tagName, OIC_JSON_CREDTYPE_NAME) == 0)
                            {
                                uint64_t credType = 0;
                                cborFindResult = cbor_value_get_uint64(&roleMap, &credType);
                                VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed retrieving credType.");
                                /* Only SIGNED_ASYMMETRIC_KEY is supported. */
                                if (SIGNED_ASYMMETRIC_KEY != (OicSecCredType_t)credType)
                                {
                                    OIC_LOG_V(ERROR, TAG, "Unsupported role credential type: %" PRIu64 , credType);
                                    goto exit;
                                }
                            }
                            // Silently ignore subject field; log anything else.
                            else if (strcmp(tagName, OIC_JSON_SUBJECTID_NAME) != 0)
                            {
                                OIC_LOG_V(WARNING, TAG, "Unknown role map tag: %s", tagName);
                            }
                        }

                        if (cbor_value_is_valid(&roleMap))
                        {
                            cborFindResult = cbor_value_advance(&roleMap);
                            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing role map.");
                        }
                    }

                    if (NULL == currEntry->certificate.data)
                    {
                        OIC_LOG(ERROR, TAG, "Role credential did not have publicData");
                        goto exit;
                    }

                    if (cbor_value_is_valid(&roleArray))
                    {
                        cborFindResult = cbor_value_advance(&roleArray);
                        VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing role array.");
                    }
                }
            }
            else
            {
                // Ignore any other tag type for now.
                OIC_LOG_V(WARNING, TAG, "Unknown role root map tag %s", tagName);
            }

        }
        if (cbor_value_is_valid(&rolesRootMap))
        {
            cborFindResult = cbor_value_advance(&rolesRootMap);
            VERIFY_CBOR_SUCCESS_OR_OUT_OF_MEMORY(TAG, cborFindResult, "Failed advancing CSR Root Map");
        }
    }

    *roleEntries = headRoleCertChain;

exit:
    if (NULL != tagName)
    {
        free(tagName);
    }

    if (CborNoError != cborFindResult)
    {
        if (NULL != headRoleCertChain)
        {
            FreeRoleCertChainList(headRoleCertChain);
        }
        return OC_STACK_ERROR;
    }
    else
    {
        return OC_STACK_OK;
    }
}

static OCEntityHandlerResult HandleGetRequest(OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(DEBUG, TAG, "Roles HandleGetRequest IN");

    OCStackResult res = OC_STACK_ERROR;
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    size_t size = 0;
    uint8_t *payload = NULL;
    const RoleCertChain_t *roles = NULL;
    uint8_t *publicKey = NULL;
    size_t publicKeyLength = 0;

    res = GetPeerPublicKey(&ehRequest->devAddr, &publicKey, &publicKeyLength);
    // OC_STACK_NO_RESOURCE means that the Peer doesn't have a Public Key.
    if ((OC_STACK_OK != res) && (OC_STACK_NO_RESOURCE != res))
    {
        OIC_LOG_V(ERROR, TAG, "Could not get remote peer's public key: %d", res);
        ehRet = OC_EH_ERROR;
        goto exit;
    }

    if (NULL != publicKey)
    {
        for (const RolesEntry_t *entry = gRoles; NULL != entry; entry = entry->next)
        {
            if ((entry->publicKeyLength == publicKeyLength) &&
                (0 == memcmp(entry->publicKey, publicKey, publicKeyLength)))
            {
                roles = entry->chains;
                break;
            }
        }
    }

    // If roles is NULL, we will return success with an empty "roles" array
    res = RolesToCBORPayload(roles, &payload, &size);
    ehRet = (OC_STACK_OK == res) ? OC_EH_OK : OC_EH_ERROR;

exit:

    ehRet = ((SendSRMResponse(ehRequest, ehRet, payload, size)) == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    OICFree(payload);
    OICFree(publicKey);

    OIC_LOG(DEBUG, TAG, "Roles HandleGetRequest OUT");
    return ehRet;
}

static OCEntityHandlerResult HandlePostRequest(OCEntityHandlerRequest *ehRequest)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    uint8_t *pubKey = NULL;
    size_t pubKeyLength = 0;
    uint8_t *peerPubKey = NULL;
    size_t peerPubKeyLen = 0;

    OIC_LOG(DEBUG, TAG, "Roles HandlePostRequest IN");

    RoleCertChain_t *chains = NULL;
    uint8_t *payload = (((OCSecurityPayload*)ehRequest->payload)->securityData);
    size_t size = (((OCSecurityPayload*)ehRequest->payload)->payloadSize);

    OCStackResult res = GetPeerPublicKey(&ehRequest->devAddr, &peerPubKey, &peerPubKeyLen);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Could not get peer's public key: %d", res);
        ehRet = OC_EH_ERROR;
        goto exit;
    }

    res = CBORPayloadToRoles(payload, size, &chains);
    if (OC_STACK_OK == res)
    {
        RoleCertChain_t *curr;

        for (curr = chains; NULL != curr; curr = curr->next)
        {
            if (OC_STACK_OK != OCInternalIsValidRoleCertificate(curr->certificate.data, curr->certificate.len,
                &pubKey, &pubKeyLength))
            {
                OIC_LOG(ERROR, TAG, "Could not verify certificate is a valid role certificate");
                ehRet = OC_EH_ERROR;
                goto exit;
            }

            if ((pubKeyLength != peerPubKeyLen) ||
                (0 != memcmp(pubKey, peerPubKey, pubKeyLength)))
            {
                OIC_LOG(ERROR, TAG, "Peer sent us certificate not for its public key");
                continue;
            }

            if (OC_STACK_OK != AddRoleCertificate(curr, pubKey, pubKeyLength))
            {
                OIC_LOG(ERROR, TAG, "Could not AddRoleCertificate");
                ehRet = OC_EH_ERROR;
                goto exit;
            }
        }

        ehRet = OC_EH_OK;
    }

exit:

    ehRet = (SendSRMResponse(ehRequest, ehRet, NULL, 0) == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    FreeRoleCertChainList(chains);
    OICFree(pubKey);
    OICFree(peerPubKey);

    OIC_LOG(DEBUG, TAG, "Roles HandlePostRequest OUT");

    return ehRet;
}

static OCEntityHandlerResult HandleDeleteRequest(OCEntityHandlerRequest *ehRequest)
{
    OIC_LOG(DEBUG, TAG, "Roles HandleDeleteRequest IN");

    OCEntityHandlerResult ehRet = OC_EH_ERROR;
    uint8_t *peerPubKey = NULL;
    size_t peerPubKeyLen = 0;
    OicParseQueryIter_t parseIter = { .attrPos = NULL };
    uint32_t credId = 0;

    if (NULL == ehRequest->query)
    {
        return ehRet;
    }

    // Parsing REST query to get the credId
    ParseQueryIterInit((unsigned char *)ehRequest->query, &parseIter);
    while (GetNextQuery(&parseIter))
    {
        if (strncasecmp((const char *)parseIter.attrPos, OIC_JSON_CREDID_NAME,
            parseIter.attrLen) == 0)
        {
            int ret = sscanf((const char *)parseIter.valPos, "%u", &credId);
            if (1 > ret)
            {
                OIC_LOG_V(ERROR, TAG, "credId was not valid: %s", parseIter.valPos);
                ehRet = OC_EH_ERROR;
                goto exit;
            }
        }
        else
        {
            OIC_LOG_V(DEBUG, TAG, "Unexpected in query string: %s=%s", parseIter.attrPos, parseIter.valPos);
        }
    }

    OCStackResult res = GetPeerPublicKey(&ehRequest->devAddr, &peerPubKey, &peerPubKeyLen);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Could not get peer's public key: %d", res);
        ehRet = OC_EH_RESOURCE_DELETED;
        goto exit;
    }

    RolesEntry_t *entry = NULL;
    for (entry = gRoles; NULL != entry; entry = entry->next)
    {
        assert((0 < entry->publicKeyLength) && (NULL != entry->publicKey));
        if ((entry->publicKeyLength == peerPubKeyLen) &&
            (0 == memcmp(entry->publicKey, peerPubKey, peerPubKeyLen)))
        {
            break;
        }
    }

    if (NULL == entry)
    {
        /* No entry for this peer. */
        OIC_LOG(WARNING, TAG, "No roles for this peer's public key");
        // if no entry, the request is successful by definition
        ehRet = OC_EH_RESOURCE_DELETED;
        goto exit;
    }

    InvalidateRoleCache(entry);

    if (NULL != entry->chains)
    {
        RoleCertChain_t *curr1 = NULL;
        RoleCertChain_t *curr2 = NULL;
        LL_FOREACH_SAFE(entry->chains, curr1, curr2)
        {
            // credId of zero means delete all creds; we never assign zero as a credId.
            if ((0 == credId) || (curr1->credId == credId))
            {
                LL_DELETE(entry->chains, curr1);
                FreeRoleCertChain(curr1);
                ehRet = OC_EH_RESOURCE_DELETED;
                if (0 != credId)
                {
                    break;
                }
            }
        }
    }
    else
    {
        /* No cert chains are present in the entry. */
        OIC_LOG(WARNING, TAG, "No cert chains are present in the entry");
        /* Request is successful since everything has been removed. */
        ehRet = OC_EH_RESOURCE_DELETED;
        goto exit;
    }

exit:

    ehRet = ((SendSRMResponse(ehRequest, ehRet, NULL, 0)) == OC_STACK_OK) ? OC_EH_OK : OC_EH_ERROR;

    OIC_LOG(DEBUG, TAG, "Roles HandleDeleteRequest OUT");
    OICFree(peerPubKey);

    return ehRet;
}

static OCEntityHandlerResult RolesEntityHandler(OCEntityHandlerFlag flag,
                                                OCEntityHandlerRequest * ehRequest,
                                                void* callbackParameter)
{
    OCEntityHandlerResult ehRet = OC_EH_ERROR;

    OC_UNUSED(callbackParameter);

    if (!ehRequest)
    {
        return OC_EH_ERROR;
    }

    if (flag & OC_REQUEST_FLAG)
    {
        OIC_LOG(DEBUG, TAG, "Flag includes OC_REQUEST_FLAG");
        switch (ehRequest->method)
        {
        case OC_REST_GET:
            ehRet = HandleGetRequest(ehRequest);
            break;
        case OC_REST_PUT:
        case OC_REST_POST:
            ehRet = HandlePostRequest(ehRequest);
            break;
        case OC_REST_DELETE:
            ehRet = HandleDeleteRequest(ehRequest);
            break;
        default:
            ehRet = ((SendSRMResponse(ehRequest, OC_EH_ERROR, NULL, 0)) == OC_STACK_OK) ?
                OC_EH_OK : OC_EH_ERROR;
            break;
        }
    }

    return ehRet;
}

OCStackResult InitRolesResource()
{
    OCStackResult res = OCCreateResource(&gRolesHandle,
        OIC_RSRC_TYPE_SEC_ROLES,
        OC_RSRVD_INTERFACE_DEFAULT,
        OIC_RSRC_ROLES_URI,
        RolesEntityHandler,
        NULL,
        OC_SECURE |
        OC_DISCOVERABLE);

    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(FATAL, TAG, "Unable to instantiate roles resource: %d", res);
    }

    return res;
}

OCStackResult DeInitRolesResource()
{
    OCStackResult res = OCDeleteResource(gRolesHandle);

    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Failed to delete roles resource: %d", res);
    }

    gRolesHandle = NULL;

    FreeRolesList(gRoles);
    FreeSymmetricRolesList(gSymmetricRoles);

    gRoles = NULL;

    return res;
}

/*
 * true if before < after, false if before >= after
 */
static bool IsBefore(const struct tm *before, const struct tm *after)
{
    if (before->tm_year > after->tm_year)
    {
        return false;
    }

    if (before->tm_year == after->tm_year)
    {
        if (before->tm_mon > after->tm_mon)
        {
            return false;
        }

        if (before->tm_mon == after->tm_mon)
        {
            if (before->tm_mday > after->tm_mday)
            {
                return false;
            }

            if (before->tm_mday == after->tm_mday)
            {
                if (before->tm_hour > after->tm_hour)
                {
                    return false;
                }

                if (before->tm_hour == after->tm_hour)
                {
                    if (before->tm_min > after->tm_min)
                    {
                        return false;
                    }

                    if (before->tm_min == after->tm_min)
                    {
                        return (before->tm_sec < after->tm_sec);
                    }
                }
            }
        }
    }

    return true;
}

OCStackResult GetEndpointRoles(const CAEndpoint_t *endpoint, OicSecRole_t **roles, size_t *roleCount)
{
    VERIFY_NOT_NULL_RETURN(TAG, endpoint, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, roles, ERROR, OC_STACK_INVALID_PARAM);
    VERIFY_NOT_NULL_RETURN(TAG, roleCount, ERROR, OC_STACK_INVALID_PARAM);

    uint8_t *publicKey = NULL;
    size_t publicKeyLength = 0;
    RolesEntry_t *targetEntry = NULL;
    OicSecRole_t *rolesToReturn = NULL;
    size_t rolesToReturnCount = 0;
    ByteArray_t trustedCaCerts;
    memset(&trustedCaCerts, 0, sizeof(trustedCaCerts));

    OCStackResult res = GetPeerPublicKeyFromEndpoint(endpoint, &publicKey, &publicKeyLength);
    if ((OC_STACK_INVALID_PARAM == res) || (OC_STACK_NO_RESOURCE == res))
    {
        /*
         * OC_STACK_INVALID_PARAM or OC_STACK_NO_RESOURCE indicate the endpoint didn't authenticate
         * with a certificate. Look for a symmetric key-based role and return that if present.
         */
        CASecureEndpoint_t sep;
        CAResult_t caRes = GetCASecureEndpointData(endpoint, &sep);
        if (CA_STATUS_OK != caRes)
        {
            *roles = NULL;
            *roleCount = 0;
            return OC_STACK_OK;
        }

        SymmetricRoleEntry_t *curr = NULL;
        LL_FOREACH(gSymmetricRoles, curr)
        {
            if ((UUID_LENGTH == sep.identity.id_length) &&
                (0 == memcmp(curr->subject.id, sep.identity.id, sizeof(curr->subject.id))))
            {
                *roles = (OicSecRole_t *)OICCalloc(1, sizeof(OicSecRole_t));
                if (NULL == *roles)
                {
                    OIC_LOG(ERROR, TAG, "No memory allocating roles for symmetric credential");
                    return OC_STACK_NO_MEMORY;
                }

                (*roles)[0] = curr->role;
                *roleCount = 1;
                return OC_STACK_OK;
            }
        }

        /* No symmetric role found. Return empty list. */
        *roles = NULL;
        *roleCount = 0;
        return OC_STACK_OK;
    }
    else if (OC_STACK_OK != res)
    {
        /* Any other error is a fatal error. */
        OIC_LOG_V(ERROR, TAG, "Could not GetPeerPublicKeyFromEndpoint: %d", res);
        return res;
    }

    for (targetEntry = gRoles; NULL != targetEntry; targetEntry = targetEntry->next)
    {
        if ((targetEntry->publicKeyLength == publicKeyLength) &&
            (0 == memcmp(targetEntry->publicKey, publicKey, publicKeyLength)))
        {
            break;
        }
    }

    if (NULL == targetEntry)
    {
        /* No roles for this peer. */
        *roles = NULL;
        *roleCount = 0;
        OICFree(publicKey);
        return OC_STACK_OK;
    }

    /* Is the cache still valid? */
    struct tm now;
    memset(&now, 0, sizeof(now));
#ifndef WITH_ARDUINO /* No reliable time on Arduino, so assume the cache is valid if present. */
    time_t nowTimeT = 0;
    nowTimeT = time(NULL);
    /* If we failed to get the current time, assume the cache is valid if present. */
    if ((time_t)-1 != nowTimeT)
    {
        /* gmtime_{s,r} should not ever fail. */
#ifdef HAVE_WINDOWS_H
        if (0 != gmtime_s(&now, &nowTimeT))
        {
            assert(!"gmtime_s failed");
            OIC_LOG(WARNING, TAG, "gmtime_s failed; assuming role cache is valid");
            memset(&now, 0, sizeof(now));
        }
#else
        if (NULL == gmtime_r(&nowTimeT, &now))
        {
            assert(!"gmtime_r failed");
            OIC_LOG(WARNING, TAG, "gmtime_r failed; assuming role cache is valid");
            memset(&now, 0, sizeof(now));
        }
#endif
    }
#endif /* WITH_ARDUINO */

    if (IsBefore(&now, &targetEntry->cacheValidUntil))
    {
        /* now < cacheValidUntil: provide caller with a copy of the cached roles */
        *roles = (OicSecRole_t *)OICCalloc(targetEntry->cachedRolesLength, sizeof(OicSecRole_t));
        if (NULL == *roles)
        {
            OICFree(publicKey);
            return OC_STACK_NO_MEMORY;
        }
        memcpy(*roles, targetEntry->cachedRoles, (targetEntry->cachedRolesLength * sizeof(OicSecRole_t)));
        *roleCount = targetEntry->cachedRolesLength;

        OICFree(publicKey);
        return OC_STACK_OK;
    }

    InvalidateRoleCache(targetEntry);

    /* Retrieve the current set of trusted CAs from the cred resource. */
    res = GetPemCaCert(&trustedCaCerts, TRUST_CA);
    if (OC_STACK_OK != res)
    {
        OIC_LOG_V(ERROR, TAG, "Could not get CA certs: %d", res);
        OICFree(publicKey);
        return res;
    }

    RoleCertChain_t *chain = targetEntry->chains;
    while (NULL != chain)
    {
        RoleCertChain_t *chainToRemove = NULL;
        OicSecRole_t *currCertRoles = NULL;
        size_t currCertRolesCount = 0;
        struct tm notValidAfter;
        memset(&notValidAfter, 0, sizeof(notValidAfter));

        res = OCInternalVerifyRoleCertificate(&chain->certificate, trustedCaCerts.data,
                                              trustedCaCerts.len, &currCertRoles,
                                              &currCertRolesCount, &notValidAfter);

        if (OC_STACK_OK != res)
        {
            OIC_LOG_V(ERROR, TAG, "Failed to verify a role certificate: %d", res);
            /* Remove the invalid cert chain, but don't exit; try all certificates presented. */
            LL_DELETE(targetEntry->chains, chain);
            chainToRemove = chain;
        }
        else
        {
            /* Add returned roles to current list. */
            OicSecRole_t *savePtr = rolesToReturn;
            rolesToReturn = (OicSecRole_t *)OICRealloc(rolesToReturn,
                sizeof(rolesToReturn[0]) * (rolesToReturnCount + currCertRolesCount));
            if (NULL == rolesToReturn)
            {
                OIC_LOG(ERROR, TAG, "No memory reallocating rolesToReturn");
                memset(&targetEntry->cacheValidUntil, 0, sizeof(targetEntry->cacheValidUntil));
                OICFree(trustedCaCerts.data);
                OICFree(savePtr);
                OICFree(currCertRoles);
                OICFree(publicKey);
                return OC_STACK_NO_MEMORY;
            }
            memcpy((rolesToReturn + rolesToReturnCount),
                   currCertRoles,
                   (currCertRolesCount * sizeof(currCertRoles[0])));
            rolesToReturnCount += currCertRolesCount;
            OICFree(currCertRoles);
        }

        /*
         * Set the cacheValidUntil value to be the earliest notValidUntil date amongst
         * all the certificates.
         *
         * Assumption is that if tm_year is zero, the cacheValidUntil is all zero, and so the
         * cacheValidUntil value hasn't yet been set. tm_year of 0 means the year 1900, and we
         * should never see this value in a certificate.
         */
        if ((0 == targetEntry->cacheValidUntil.tm_year) ||
            IsBefore(&notValidAfter, &targetEntry->cacheValidUntil))
        {
            memcpy(&targetEntry->cacheValidUntil, &notValidAfter, sizeof(targetEntry->cacheValidUntil));
        }

        /*
         * If the cert chain was invalid it has already been removed from the list.
         * We clean it up here so that we can continue checking all of the certificates.
         */
        chain = chain->next;
        FreeRoleCertChain(chainToRemove);
    }

    targetEntry->cachedRoles = rolesToReturn;
    targetEntry->cachedRolesLength = rolesToReturnCount;

    /* Make a copy for the caller. */
    *roles = (OicSecRole_t *)OICCalloc(targetEntry->cachedRolesLength, sizeof(OicSecRole_t));
    if (NULL == *roles)
    {
        OICFree(publicKey);
        OICFree(trustedCaCerts.data);
        return OC_STACK_NO_MEMORY;
    }
    memcpy(*roles, targetEntry->cachedRoles, (targetEntry->cachedRolesLength * sizeof(OicSecRole_t)));
    *roleCount = targetEntry->cachedRolesLength;

    OICFree(publicKey);
    OICFree(trustedCaCerts.data);
    return OC_STACK_OK;
}

#endif /* defined(__WITH_DTLS__) || defined(__WITH_TLS__) */
