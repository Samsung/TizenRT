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

#ifndef IOTVT_SRM_CREDR_H
#define IOTVT_SRM_CREDR_H

#include "cainterface.h"
#include "securevirtualresourcetypes.h"
#include "octypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize credential resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK, if initialization is successful, else ::OC_STACK_ERROR if
 * initialization fails.
 */
OCStackResult InitCredResource();

/**
 * Perform cleanup for credential resources.
 *
 * @return ::OC_STACK_OK, if no errors. ::OC_STACK_ERROR, if stack process error.
 * ::OC_STACK_NO_RESOURCE, if resource not found.
 * ::OC_STACK_INVALID_PARAM, if invalid param.
 */
OCStackResult DeInitCredResource();

/**
 * This method is used by tinydtls/SRM to retrieve credential for given subject.
 *
 * @param subjectId for which credential is required.
 *
 * @return reference to @ref OicSecCred_t, if credential is found, else NULL, if credential
 * not found.
 */
const OicSecCred_t *GetCredResourceData(const OicUuid_t *subjectId);

/**
 * This function converts credential data into CBOR format.
 * Caller needs to invoke 'free' when done using returned string.
 *
 * @param cred is the pointer to instance of OicSecCred_t structure.
 * @param cborPayload is the CBOR converted value.
 * @param cborSize is the size of the CBOR.
 *
 * @return ::OC_STACK_OK if conversion is successful, else ::OC_STACK_ERROR if unsuccessful.
 */
OCStackResult CredToCBORPayload(const OicSecCred_t *cred, uint8_t **cborPayload,
                                size_t *cborSize);

/**
 * This function generates the bin credential data.
 *
 * @param subject pointer to subject of this credential.
 * @param credType credential type.
 * @param publicData public data such as public key.
 * @param privateData private data such as private key.
 * @param rownerID Resource owner's UUID.
 *
 * @return pointer to instance of @ref OicSecCred_t if successful. else NULL in case of error.

 */
OicSecCred_t *GenerateCredential(const OicUuid_t *subject, OicSecCredType_t credType,
                                 const OicSecCert_t *publicData, const OicSecKey_t *privateData,
                                 const OicUuid_t *rownerID);

/**
 * This function adds the new cred to the credential list.
 *
 * @param cred is the pointer to new credential.
 *
 * @return ::OC_STACK_OK, cred not NULL and persistent storage gets updated.
 * ::OC_STACK_ERROR, cred is NULL or fails to update persistent storage.
 */
OCStackResult AddCredential(OicSecCred_t *cred);

/**
 * Function to remove the credential from SVR DB.
 *
 * @param credId is the Credential ID to be deleted.
 *
 * @return ::OC_STACK_OK for success, or errorcode otherwise.
 */
OCStackResult RemoveCredential(const OicUuid_t *credId);

#if defined(__WITH_DTLS__)
/**
 * This internal callback is used by lower stack (i.e. CA layer) to
 * retrieve PSK credentials from RI security layer.
 *
 * @param type of PSK data required by CA layer during DTLS handshake.
 * @param desc Additional request information.
 * @param desc_len is the actual length of desc.
 * @param result  is must be filled with the requested information.
 * @param result_length is the maximum size of @p result.
 *
 * @return The number of bytes written to @p result or a value
 *         less than zero on error.
 */
int32_t GetDtlsPskCredentials(CADtlsPskCredType_t type,
                              const unsigned char *desc, size_t desc_len,
                              unsigned char *result, size_t result_length);

/**
 * Add temporal PSK to PIN based OxM.
 *
 * @param tmpSubject is the UUID of target device
 * @param credType is the type of credential to be added
 * @param pin is the numeric characters
 * @param pinSize is the length of 'pin'
 * @param rownerID Resource owner's UUID
 * @param tmpCredSubject is the generated credential's subject.
 *
 * @return ::OC_STACK_OK for success or else errorcode.
 */
OCStackResult AddTmpPskWithPIN(const OicUuid_t *tmpSubject, OicSecCredType_t credType,
                               const char *pin, size_t pinSize,
                               const OicUuid_t *rownerID,
                               OicUuid_t *tmpCredSubject);

#endif /* __WITH_DTLS__ */

#ifdef __WITH_X509__
/**
 * This function is used toretrieve certificate credentials from RI security layer.
 *
 * @param credInfo is the binary structure containing certificate credentials
 *
 * @return 0 on success.
 */
int GetDtlsX509Credentials(CADtlsX509Creds_t *credInfo);
#endif /*__WITH_X509__*/

/**
 * Function to deallocate allocated memory to OicSecCred_t.
 *
 * @param cred pointer to cred type.
 *
 */
void DeleteCredList(OicSecCred_t *cred);

/**
 * Internal function to update resource owner
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetCredRownerId(const OicUuid_t *newROwner);

/**
 * Gets the OicUuid_t value for the rownerid of the cred resource.
 *
 * @param rowneruuid a pointer to be assigned to the rowneruuid property
 * @return ::OC_STACK_OK if rowneruuid is assigned correctly, else ::OC_STACK_ERROR.
 */
OCStackResult GetCredRownerId(OicUuid_t *rowneruuid);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_CREDR_H
