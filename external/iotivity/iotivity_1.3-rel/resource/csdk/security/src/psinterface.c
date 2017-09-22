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

#ifdef WITH_ARDUINO
#define __STDC_LIMIT_MACROS
#endif

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "cainterface.h"
#include "logger.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "payload_logging.h"
#include "resourcemanager.h"
#include "secureresourcemanager.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "pstatresource.h"
#include "doxmresource.h"
#include "ocresourcehandler.h"

#define TAG  "OIC_SRM_PSI"

/**
 * Helps cover adding the name of the resource, map addition, and ending while
 * performing CBOR encoding.
 */
#define CBOR_ENCODING_SIZE_ADDITION 255

/**
 * Virtual database buffer block size
 */
#ifdef _WIN32
#define DB_FILE_SIZE_BLOCK 1023
#else
const size_t DB_FILE_SIZE_BLOCK = 1023;
#endif

typedef enum _PSDatabase
{
    PS_DATABASE_SECURITY = 0,
    PS_DATABASE_DEVICEPROPERTIES
} PSDatabase;

/**
 * Writes CBOR payload to the specified database in persistent storage.
 *
 * @param databaseName is the name of the database to access through persistent storage.
 * @param payload      is the CBOR payload to write to the database in persistent storage.
 * @param size         is the size of payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
static OCStackResult WritePayloadToPS(const char *databaseName, uint8_t *payload, size_t size)
{
    if (!databaseName || !payload || (size <= 0))
    {
        return OC_STACK_INVALID_PARAM;
    }

    OCStackResult result = OC_STACK_ERROR;

    OIC_LOG_V(DEBUG, TAG, "Writing in the file: %" PRIuPTR, size);

    OCPersistentStorage* ps = OCGetPersistentStorageHandler();
    if (ps)
    {
        FILE *fp = ps->open(databaseName, "wb");
        if (fp)
        {
            size_t numberItems = ps->write(payload, 1, size, fp);
            if (size == numberItems)
            {
                OIC_LOG_V(DEBUG, TAG, "Written %" PRIuPTR " bytes into %s", size, databaseName);
                result = OC_STACK_OK;
            }
            else
            {
                OIC_LOG_V(ERROR, TAG, "Failed writing %" PRIuPTR " in %s", numberItems, databaseName);
            }
            ps->close(fp);
        }
        else
        {
            OIC_LOG(ERROR, TAG, "File open failed.");
        }
    }

    return result;
}

/**
 * Gets the database size
 *
 * @param ps            is a pointer to OCPersistentStorage for the Virtual Resource(s).
 * @param databaseName  is the name of the database to access through persistent storage.
 *
 * @return size_t - total size of the database
 */
static size_t GetDatabaseSize(const OCPersistentStorage *ps, const char *databaseName)
{
    if (!ps)
    {
        return 0;
    }
    size_t size = 0;
    char buffer[DB_FILE_SIZE_BLOCK];  // can not initialize with declaration
                                      // but maybe not needed to initialize
    FILE *fp = ps->open(databaseName, "rb");
    if (fp)
    {
        size_t bytesRead = 0;
        do
        {
            bytesRead = ps->read(buffer, 1, DB_FILE_SIZE_BLOCK, fp);
            size += bytesRead;
        } while (bytesRead);
        ps->close(fp);
    }
    return size;
}

/**
 * Reads the database from PS
 * 
 * @note Caller of this method MUST use OICFree() method to release memory
 *       referenced by the data argument.
 *
 * @param databaseName is the name of the database to access through persistent storage.
 * @param resourceName is the name of the field for which file content are read.
 *                     if the value is NULL it will send the content of the whole file.
 * @param data         is the pointer to the file contents read from the database.
 * @param size         is the size of the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult ReadDatabaseFromPS(const char *databaseName, const char *resourceName, uint8_t **data, size_t *size)
{
    OIC_LOG(DEBUG, TAG, "ReadDatabaseFromPS IN");

    if (!databaseName || !data || *data || !size)
    {
        return OC_STACK_INVALID_PARAM;
    }

    FILE *fp = NULL;
    uint8_t *fsData = NULL;
    size_t fileSize = 0;
    OCStackResult ret = OC_STACK_ERROR;

    OCPersistentStorage *ps = OCGetPersistentStorageHandler();
    VERIFY_NOT_NULL(TAG, ps, ERROR);

    fileSize = GetDatabaseSize(ps, databaseName);
    OIC_LOG_V(DEBUG, TAG, "File Read Size: %" PRIuPTR, fileSize);
    if (fileSize)
    {
        fsData = (uint8_t *) OICCalloc(1, fileSize);
        VERIFY_NOT_NULL(TAG, fsData, ERROR);

        fp = ps->open(databaseName, "rb");
        VERIFY_NOT_NULL(TAG, fp, ERROR);
        if (ps->read(fsData, 1, fileSize, fp) == fileSize)
        {
            if (resourceName)
            {
                CborParser parser;  // will be initialized in |cbor_parser_init|
                CborValue cbor;     // will be initialized in |cbor_parser_init|
                cbor_parser_init(fsData, fileSize, 0, &parser, &cbor);
                CborValue cborValue = {0};
                CborError cborFindResult = cbor_value_map_find_value(&cbor, resourceName, &cborValue);
                if (CborNoError == cborFindResult && cbor_value_is_byte_string(&cborValue))
                {
                    cborFindResult = cbor_value_dup_byte_string(&cborValue, data, size, NULL);
                    VERIFY_SUCCESS(TAG, CborNoError == cborFindResult, ERROR);
                    ret = OC_STACK_OK;
                }
                // in case of |else (...)|, svr_data not found
            }
            // return everything in case resourceName is NULL
            else
            {
                *size = fileSize;
                *data = (uint8_t *) OICCalloc(1, fileSize);
                VERIFY_NOT_NULL(TAG, *data, ERROR);
                memcpy(*data, fsData, fileSize);
                ret = OC_STACK_OK;
            }
        }
    }
    OIC_LOG(DEBUG, TAG, "ReadDatabaseFromPS OUT");

exit:
    if (fp)
    {
        ps->close(fp);
    }
    OICFree(fsData);
    return ret;
}

/**
 * This method updates the database in PS
 *
 * @param databaseName  is the name of the database to access through persistent storage.
 * @param resourceName  is the name of the resource that will be updated.
 * @param payload       is the pointer to memory where the CBOR payload is located.
 * @param size          is the size of the CBOR payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateResourceInPS(const char *databaseName, const char *resourceName, const uint8_t *payload, size_t size)
{
    OIC_LOG(DEBUG, TAG, "UpdateResourceInPS IN");
    if (!databaseName || !resourceName)
    {
        return OC_STACK_INVALID_PARAM;
    }

    size_t dbSize = 0;
    size_t outSize = 0;
    uint8_t *dbData = NULL;
    uint8_t *outPayload = NULL;

    uint8_t *aclCbor = NULL;
    uint8_t *pstatCbor = NULL;
    uint8_t *doxmCbor = NULL;
    uint8_t *amaclCbor = NULL;
    uint8_t *credCbor = NULL;
    uint8_t *resetPfCbor = NULL;
    uint8_t *crlCbor = NULL;
    uint8_t *dpCbor = NULL;

    int64_t cborEncoderResult = CborNoError;
    OCStackResult ret = ReadDatabaseFromPS(databaseName, NULL, &dbData, &dbSize);
    if (dbData && dbSize)
    {
        PSDatabase database = PS_DATABASE_SECURITY;
        size_t allocSize = 0;
        size_t aclCborLen = 0;
        size_t pstatCborLen = 0;
        size_t doxmCborLen = 0;
        size_t amaclCborLen = 0;
        size_t credCborLen = 0;
        size_t resetPfCborLen = 0;
        size_t crlCborLen = 0;
        size_t dpCborLen = 0;

        // Determine which database we are working with so we can scope our operations
        if (0 == strcmp(OC_DEVICE_PROPS_FILE_NAME, databaseName))
        {
            database = PS_DATABASE_DEVICEPROPERTIES;
        }

        // Gets each secure virtual resource from persistent storage
        // this local scoping intended, for destroying large cbor instances after use
        {
            CborParser parser;  // will be initialized in |cbor_parser_init|
            CborValue cbor;     // will be initialized in |cbor_parser_init|
            cbor_parser_init(dbData, dbSize, 0, &parser, &cbor);
            CborValue curVal = {0};
            CborError cborFindResult = CborNoError;

            // Only search for and copy resources owned by the target database
            if (PS_DATABASE_SECURITY == database)
            {
                // Security database
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_ACL_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &aclCbor, &aclCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ACL Name Value.");
                }
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_PSTAT_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &pstatCbor, &pstatCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PSTAT Name Value.");
                }
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_DOXM_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &doxmCbor, &doxmCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding DOXM Name Value.");
                }
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_AMACL_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &amaclCbor, &amaclCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding AMACL Name Value.");
                }
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_CRED_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &credCbor, &credCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CRED Name Value.");
                }
                cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_RESET_PF_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &resetPfCbor, &resetPfCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Reset Profile Name Value.");
                }
                int64_t cborFindCrlResult = cbor_value_map_find_value(&cbor, OIC_JSON_CRL_NAME, &curVal);
                if ((CborNoError == cborFindCrlResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindCrlResult = cbor_value_dup_byte_string(&curVal, &crlCbor, &crlCborLen, NULL);
                    if ((CborNoError != cborFindCrlResult) && (CborErrorOutOfMemory != cborFindCrlResult))
                    {
                        OIC_LOG(ERROR, TAG, "Failed Finding optional CRL Name Value.");
                    }
                    else
                    {
                        OIC_LOG(INFO, TAG, "Successfully Finding optional CRL Name Value.");
                    }
                }
            }
            else
            {
                // Device Properties database
                cborFindResult = cbor_value_map_find_value(&cbor, OC_JSON_DEVICE_PROPS_NAME, &curVal);
                if ((CborNoError == cborFindResult) && cbor_value_is_byte_string(&curVal))
                {
                    cborFindResult = cbor_value_dup_byte_string(&curVal, &dpCbor, &dpCborLen, NULL);
                    VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Device Properties Name Value.");
                }
            }
        }

        // Updates the added |payload| with the existing secure virtual resource(s)
        // this local scoping intended, for destroying large cbor instances after use
        {
            if (PS_DATABASE_SECURITY == database)
            {
                allocSize = aclCborLen + pstatCborLen + doxmCborLen + amaclCborLen
                          + credCborLen + /* pconfCborLen + */ resetPfCborLen + crlCborLen
                          + size + CBOR_ENCODING_SIZE_ADDITION;
            }
            else
            {
                allocSize = dpCborLen + size + CBOR_ENCODING_SIZE_ADDITION;
            }


            outPayload = (uint8_t *)OICCalloc(1, allocSize);
            VERIFY_NOT_NULL(TAG, outPayload, ERROR);
            CborEncoder encoder;  // will be initialized in |cbor_parser_init|
            cbor_encoder_init(&encoder, outPayload, allocSize, 0);
            CborEncoder resource;  // will be initialized in |cbor_encoder_create_map|
            cborEncoderResult |= cbor_encoder_create_map(&encoder, &resource, CborIndefiniteLength);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PS Map.");

            // Encode the updated payload and add it to our map so it will be stored in the database.
            if (payload && size)
            {
                cborEncoderResult |= cbor_encode_text_string(&resource, resourceName, strlen(resourceName));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value Tag");
                cborEncoderResult |= cbor_encode_byte_string(&resource, payload, size);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value.");
            }

            // Check all of the resources from a particular database to see if we need to encode them. If the resource
            // currently exists in the database and has not been updated then we need to encode it and add it to the
            // map. If the resource has been updated then it was encoded and added to the map above so we skip it to
            // avoid having duplicate entries for the resource in the database.
            if (PS_DATABASE_SECURITY == database)
            {
                // Security database
                if (strcmp(OIC_JSON_ACL_NAME, resourceName) && aclCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_ACL_NAME, strlen(OIC_JSON_ACL_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, aclCbor, aclCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Value.");
                }
                if (strcmp(OIC_JSON_PSTAT_NAME, resourceName) && pstatCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_PSTAT_NAME, strlen(OIC_JSON_PSTAT_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, pstatCbor, pstatCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Value.");
                }
                if (strcmp(OIC_JSON_DOXM_NAME, resourceName) && doxmCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_DOXM_NAME, strlen(OIC_JSON_DOXM_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, doxmCbor, doxmCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Value.");
                }
                if (strcmp(OIC_JSON_AMACL_NAME, resourceName) && amaclCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_AMACL_NAME, strlen(OIC_JSON_AMACL_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Amacl Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, amaclCbor, amaclCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Amacl Value.");
                }
                if (strcmp(OIC_JSON_CRED_NAME, resourceName) && credCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_CRED_NAME, strlen(OIC_JSON_CRED_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, credCbor, credCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Cred Value.");
                }
                if (strcmp(OIC_JSON_RESET_PF_NAME, resourceName) && resetPfCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_RESET_PF_NAME, strlen(OIC_JSON_RESET_PF_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Reset Profile Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, resetPfCbor, resetPfCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Reset Profile Value.");
                }
                if (strcmp(OIC_JSON_CRL_NAME, resourceName) && crlCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OIC_JSON_CRL_NAME, strlen(OIC_JSON_CRL_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Crl Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, crlCbor, crlCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Crl Value.");
                }
            }
            else
            {
                // Device Properties database
                if (strcmp(OC_JSON_DEVICE_PROPS_NAME, resourceName) && dpCborLen)
                {
                    cborEncoderResult |= cbor_encode_text_string(&resource, OC_JSON_DEVICE_PROPS_NAME, strlen(OC_JSON_DEVICE_PROPS_NAME));
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Properties Name.");
                    cborEncoderResult |= cbor_encode_byte_string(&resource, dpCbor, dpCborLen);
                    VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Device Properties Value.");
                }
            }

            cborEncoderResult |= cbor_encoder_close_container(&encoder, &resource);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
            outSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
        }
    }
    else if (payload && size)
    {
        size_t allocSize = size + CBOR_ENCODING_SIZE_ADDITION;

        outPayload = (uint8_t *)OICCalloc(1, allocSize);
        VERIFY_NOT_NULL(TAG, outPayload, ERROR);
        CborEncoder encoder;  // will be initialized in |cbor_parser_init|
        cbor_encoder_init(&encoder, outPayload, allocSize, 0);
        CborEncoder resource;  // will be initialized in |cbor_encoder_create_map|
        cborEncoderResult |= cbor_encoder_create_map(&encoder, &resource, CborIndefiniteLength);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PS Map.");

        cborEncoderResult |= cbor_encode_text_string(&resource, resourceName, strlen(resourceName));
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value Tag");
        cborEncoderResult |= cbor_encode_byte_string(&resource, payload, size);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Value.");

        cborEncoderResult |= cbor_encoder_close_container(&encoder, &resource);
        VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
        outSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
    }

    ret = WritePayloadToPS(databaseName, outPayload, outSize);
    VERIFY_SUCCESS(TAG, (OC_STACK_OK == ret), ERROR);

    OIC_LOG(DEBUG, TAG, "UpdateResourceInPS OUT");

exit:
    OICFree(dbData);
    OICFree(outPayload);
    OICFree(aclCbor);
    OICFree(pstatCbor);
    OICFree(doxmCbor);
    OICFree(amaclCbor);
    OICFree(credCbor);
    OICFree(resetPfCbor);
    OICFree(crlCbor);
    OICFree(dpCbor);
    return ret;
}

/**
 * Reads the Secure Virtual Database from PS
 *
 * @note Caller of this method MUST use OICFree() method to release memory
 *       referenced by return value.
 *
 * @param resourceName is the name of the field for which file content are read.
 *                     if the value is NULL it will send the content of the whole file.
 * @param data         is the pointer to the file contents read from the database.
 * @param size         is the size of the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult GetSecureVirtualDatabaseFromPS(const char *resourceName, uint8_t **data, size_t *size)
{
    return ReadDatabaseFromPS(SVR_DB_DAT_FILE_NAME, resourceName, data, size);
}

/**
 * This method updates the Secure Virtual Database in PS
 *
 * @param resourceName  is the name of the secure resource that will be updated.
 * @param payload       is the pointer to memory where the CBOR payload is located.
 * @param size          is the size of the CBOR payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSecureResourceInPS(const char *resourceName, const uint8_t *payload, size_t size)
{
    return UpdateResourceInPS(SVR_DB_DAT_FILE_NAME, resourceName, payload, size);
}

/**
 * Resets the Secure Virtual Resource(s).
 * This function reads the Reset Profile
 * and resets the secure virtual resources accordingly.
 *
 * @return OCStackResult - result of updating Secure Virtual Resource(s)
 */
OCStackResult ResetSecureResourceInPS(void)
{
    OIC_LOG(DEBUG, TAG, "ResetSecureResourceInPS IN");

    size_t dbSize = 0;
    size_t outSize = 0;
    uint8_t *dbData = NULL;
    uint8_t *outPayload = NULL;

    uint8_t *aclCbor = NULL;
    uint8_t *credCbor = NULL;
    uint8_t *pstatCbor = NULL;
    uint8_t *doxmCbor = NULL;
    uint8_t *resetPfCbor = NULL;

    int64_t cborEncoderResult = CborNoError;
    OCStackResult ret = GetSecureVirtualDatabaseFromPS(NULL, &dbData, &dbSize);
    if(dbData && dbSize)
    {
        size_t aclCborLen = 0;
        size_t credCborLen = 0;
        size_t pstatCborLen = 0;
        size_t doxmCborLen = 0;
        size_t resetPfCborLen = 0;

        // Gets the reset profile from persistent storage
        {
            CborParser parser;  // will be initialized in |cbor_parser_init|
            CborValue cbor;     // will be initialized in |cbor_parser_init|
            cbor_parser_init(dbData, dbSize, 0, &parser, &cbor);
            CborValue curVal = {0};
            CborError cborFindResult = CborNoError;
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_RESET_PF_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &resetPfCbor, &resetPfCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding Reset Profile Name Value.");
            }
        }

        // Gets each secure virtual resource from the reset profile
        {
            CborParser parser;  // will be initialized in |cbor_parser_init|
            CborValue cbor;     // will be initialized in |cbor_parser_init|
            cbor_parser_init(resetPfCbor, resetPfCborLen, 0, &parser, &cbor);
            CborValue curVal = {0};
            CborError cborFindResult = CborNoError;
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_ACL_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &aclCbor, &aclCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ACL Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_CRED_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &credCbor, &credCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ACL Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_PSTAT_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &pstatCbor, &pstatCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PSTAT Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_DOXM_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &doxmCbor, &doxmCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding DOXM Name Value.");
            }
        }

        {
            size_t size = aclCborLen + credCborLen + pstatCborLen + doxmCborLen + resetPfCborLen + CBOR_ENCODING_SIZE_ADDITION;

            outPayload = (uint8_t *) OICCalloc(1, size);
            VERIFY_NOT_NULL(TAG, outPayload, ERROR);
            CborEncoder encoder;
            cbor_encoder_init(&encoder, outPayload, size, 0);
            CborEncoder secRsrc;
            cborEncoderResult |= cbor_encoder_create_map(&encoder, &secRsrc, CborIndefiniteLength);

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_ACL_NAME, strlen(OIC_JSON_ACL_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, aclCbor, aclCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Value.");

            if (credCborLen)
            {
                cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_CRED_NAME, strlen(OIC_JSON_CRED_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CRED Name.");
                cborEncoderResult |= cbor_encode_byte_string(&secRsrc, credCbor, credCborLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CRED Value.");
            }

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_PSTAT_NAME, strlen(OIC_JSON_PSTAT_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, pstatCbor, pstatCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Value.");

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_DOXM_NAME, strlen(OIC_JSON_DOXM_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding DOXM Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, doxmCbor, doxmCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding DOXM Value.");

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_RESET_PF_NAME, strlen(OIC_JSON_RESET_PF_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Reset Profile Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, resetPfCbor, resetPfCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Reset Profile Value.");

            cborEncoderResult |= cbor_encoder_close_container(&encoder, &secRsrc);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
            outSize = cbor_encoder_get_buffer_size(&encoder, outPayload);
        }

        ret = WritePayloadToPS(SVR_DB_DAT_FILE_NAME, outPayload, outSize);
        VERIFY_SUCCESS(TAG, (OC_STACK_OK == ret), ERROR);
    }

    SRMDeInitSecureResources();
    InitSecureResources();
    OIC_LOG(DEBUG, TAG, "ResetSecureResourceINPS OUT");

exit:
    OICFree(dbData);
    OICFree(outPayload);
    OICFree(aclCbor);
    OICFree(credCbor);
    OICFree(pstatCbor);
    OICFree(doxmCbor);
    OICFree(resetPfCbor);
    return ret;
}

/**
 * Creates Reset Profile from the initial secure virtual resources.
 * This function copies the secure resources
 * and creates the Reset Profile in the Persistent Storage.
 * Device ID in doxm and pstat remains as same after reset.
 *
 * @return OCStackResult - result of updating Secure Virtual Resource(s)
 */
OCStackResult CreateResetProfile(void)
{
    OIC_LOG(DEBUG, TAG, "CreateResetProfile IN");

    size_t dbSize = 0;
    uint8_t *dbData = NULL;

    uint8_t *aclCbor = NULL;
    uint8_t *credCbor = NULL;
    uint8_t *pstatCbor = NULL;
    uint8_t *doxmCbor = NULL;
    uint8_t *resetPfCbor = NULL;

    OCStackResult ret = OC_STACK_ERROR;
    int64_t cborEncoderResult = CborNoError;
    ret = GetSecureVirtualDatabaseFromPS(NULL, &dbData, &dbSize);
    if (dbData && dbSize)
    {
        size_t aclCborLen = 0;
        size_t credCborLen = 0;
        size_t pstatCborLen = 0;
        size_t doxmCborLen = 0;
        size_t resetPfCborLen = 0;

        {
            CborParser parser;
            CborValue cbor;
            cbor_parser_init(dbData, dbSize, 0, &parser, &cbor);
            CborValue curVal = {0};
            CborError cborFindResult = CborNoError;

            // abort if reset profile exists
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_RESET_PF_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                OIC_LOG(DEBUG, TAG, "Reset Profile already exists!!");
                goto exit;
            }

            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_ACL_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &aclCbor, &aclCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding ACL Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_CRED_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &credCbor, &credCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding CRED Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_PSTAT_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &pstatCbor, &pstatCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult, "Failed Finding PSTAT Name Value.");
            }
            cborFindResult = cbor_value_map_find_value(&cbor, OIC_JSON_DOXM_NAME, &curVal);
            if (CborNoError == cborFindResult && cbor_value_is_byte_string(&curVal))
            {
                cborFindResult = cbor_value_dup_byte_string(&curVal, &doxmCbor, &doxmCborLen, NULL);
                VERIFY_CBOR_SUCCESS(TAG, cborFindResult,  "Failed Finding DOXM Name Value.");
            }
        }

        {
            size_t size = aclCborLen + credCborLen + pstatCborLen + doxmCborLen + CBOR_ENCODING_SIZE_ADDITION;
            resetPfCbor = (uint8_t *) OICCalloc(1, size);
            VERIFY_NOT_NULL(TAG, resetPfCbor, ERROR);
            CborEncoder encoder;  // will be initialized in |cbor_parser_init|
            cbor_encoder_init(&encoder, resetPfCbor, size, 0);
            CborEncoder secRsrc;  // will be initialized in |cbor_encoder_create_map|
            cborEncoderResult |= cbor_encoder_create_map(&encoder, &secRsrc, CborIndefiniteLength);

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_ACL_NAME, strlen(OIC_JSON_ACL_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, aclCbor, aclCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding ACL Value.");

            if (credCborLen)
            {
                cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_CRED_NAME, strlen(OIC_JSON_CRED_NAME));
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CRED Name.");
                cborEncoderResult |= cbor_encode_byte_string(&secRsrc, credCbor, credCborLen);
                VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding CRED Value.");
            }

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_PSTAT_NAME, strlen(OIC_JSON_PSTAT_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, pstatCbor, pstatCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding PSTAT Value.");

            cborEncoderResult |= cbor_encode_text_string(&secRsrc, OIC_JSON_DOXM_NAME, strlen(OIC_JSON_DOXM_NAME));
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Name.");
            cborEncoderResult |= cbor_encode_byte_string(&secRsrc, doxmCbor, doxmCborLen);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Adding Doxm Value.");

            cborEncoderResult |= cbor_encoder_close_container(&encoder, &secRsrc);
            VERIFY_CBOR_SUCCESS(TAG, cborEncoderResult, "Failed Closing Array.");
            resetPfCborLen = cbor_encoder_get_buffer_size(&encoder, resetPfCbor);
        }

        UpdateSecureResourceInPS(OIC_JSON_RESET_PF_NAME, resetPfCbor, resetPfCborLen);

    }
    OIC_LOG(DEBUG, TAG, "CreateResetProfile OUT");

exit:
    OICFree(dbData);
    OICFree(aclCbor);
    OICFree(credCbor);
    OICFree(pstatCbor);
    OICFree(doxmCbor);
    OICFree(resetPfCbor);
    return ret;
}
