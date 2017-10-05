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

#ifndef IOTVT_SRM_PSI_H
#define IOTVT_SRM_PSI_H

#if defined (__TIZENRT__)
#include <json/cJSON.h>
#else
#include "cJSON.h"
#endif

// Persistent Storage status definition
typedef enum
{
    PS_NORMAL = 0,      // Persistent Storage operating normally
    PS_OPEN_FAIL,       // Failed to open persistent storage
    PS_PARSE_FAIL,     // Failed to cbor parse persistent storage
    PS_NO_EXTERNAL_DB_SET = 99, // no external DB set (initial state)
} PSStatus_t;

/**
 * Reads the Secure Virtual Database from PS into dynamically allocated
 * memory buffer.
 *
 * @note Caller of this method MUST use OCFree() method to release memory
 *       referenced by return value.
 *
 * @return char * reference to memory buffer containing SVR database.
 */
char * GetSVRDatabase();

/**
 * This method is used by a entity handlers of SVR's to update
 * SVR database.
 *
 * @param rsrcName string denoting the SVR name ("acl", "cred", "pstat" etc).
 * @param jsonObj JSON object containing the SVR contents.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSVRDatabase(const char* rsrcName, cJSON* jsonObj);

/**
 * Reads the Secure Virtual Database from PS
 *
 * @note Caller of this method MUST use OCFree() method to release memory
 *       referenced by return value.
 *
 * @param rsrcName is the name of the field for which file content are read.
                   if the value is NULL it will send the content of the whole file.
 * @param data is the pointer to the file contents read from the database.
 * @param size is the size to the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult GetSecureVirtualDatabaseFromPS(const char *rsrcName, uint8_t **data, size_t *size);

/**
 * Gets the Secure Virtual Database from the Persistent Storage
 *
 * @param ps - Persistent Storage handler
 * @param rsrcName - pointer of character string for the SVR name (e.g. "acl")
 * @param data - pointer of the returned Secure Virtual Resource(s)
 * @param size - pointer of the returned size of Secure Virtual Resource(s)
 *
 * @return OCStackResult - result of getting Secure Virtual Resource(s)
 */
OCStackResult GetSecureVirtualDatabaseFromPS2(OCPersistentStorage* ps, const char *rsrcName,
                                              uint8_t **data, size_t *size);


/**
 * This method converts updates the persistent storage.
 *
 * @param rsrcName is the name of the secure resource that will be updated.
 * @param cborPayload is the pointer holding cbor payload.
 * @param cborPayload is the size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSecureResourceInPS(const char* rsrcName, const uint8_t* cborPayload, size_t size);

/**
 * This method resets the secure resources according to the reset profile.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult ResetSecureResourceInPS(void);

/**
 * This method creates the reset profile from the persistent storage.
 * The reset profile is the copy of the initial state of SVR resources
 * when secure resources are initiated at first.
 * In remote reset, the SVR will be reset according to the reset profile.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult CreateResetProfile(void);

/**
 * This method sets the persistent storage status.
 *
 * @param status is the current persistent storage status
 */
void SetPSStatus(PSStatus_t status);

/**
 * This method prints debug log that displays persistent storage status.
 */
void PrintPSStatus(void);

/**
 * Init the Persistent Storage Database.
 */
OCStackResult InitPersistentStorageInterface(void);

/**
 * DeInit the Persistent Storage Database.
 */
void DeinitPersistentStorageInterface(void);

/**
 * API to encrypt the un-encrypted DB file before OCRegisterPersistentStorageHandler
 * If the API is successful, un-encrypted file will be removed, and if the encrypted file
 * is currupted, then it restores encrypted file using rescue file.
 *
 * @param[in] key key used for encryption
 * @param[in] psPlain OCPersistentStorage for the plain DB
 * @param[in] psEnc OCPersistentStorage for the encrypted DB
 * @param[in] psRescue OCPersistentStorage for the rescue DB
 *
 * @return ::OC_STACK_OK on success and other value otherwise.
 */
OCStackResult setSecurePSI(const unsigned char *key, const OCPersistentStorage *psPlain,
        const OCPersistentStorage *psEnc, const OCPersistentStorage *psRescue);

#endif //IOTVT_SRM_PSI_H
