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
#include "experimental/securevirtualresourcetypes.h"
#include "octypes.h"
#include "rolesresource.h"
#include <cbor.h>

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
OicSecCred_t* GetCredResourceData(const OicUuid_t* subjectId);

/**
 * This method is used by SRM to retrieve credential entry for given credId.
 *
 * @note Caller needs to release this memory by calling DeleteCredList().
 *
 * @param credId for which credential is required.
 *
 * @return reference to @ref OicSecCred_t, if credential is found, else NULL, if credential
 * not found.
 */
OicSecCred_t* GetCredEntryByCredId(const uint16_t credId);

/**
 * This function converts credential data into CBOR format.
 * Caller needs to invoke 'OICFree' when done using returned string.
 * Wrapper over the CredToCBORPayloadWithRowner.
 *
 * @param cred is the pointer to instance of OicSecCred_t structure.
 * @param cborPayload is the CBOR converted value.
 * @param cborSize is the size of the CBOR.
 * @param secureFlag shows fill or not private key.
 *
 * @return ::OC_STACK_OK if conversion is successful, else ::OC_STACK_ERROR if unsuccessful.
 */
OCStackResult CredToCBORPayload(const OicSecCred_t* cred, uint8_t **cborPayload,
                                size_t *cborSize, int secureFlag);

/**
 * This function converts credential data into CBOR format.
 * Caller needs to invoke 'OICFree' when done using returned string.
 *
 * @param cred is the pointer to instance of OicSecCred_t structure.
 * @param rownerId resource owner's UUID
 * @param cborPayload is the CBOR converted value.
 * @param cborSize is the size of the CBOR.
 * @param secureFlag shows fill or not private key.
 *
 * @return ::OC_STACK_OK if conversion is successful, else ::OC_STACK_ERROR if unsuccessful.
 */

OCStackResult CredToCBORPayloadWithRowner(const OicSecCred_t *credS, const OicUuid_t *rownerId, uint8_t **cborPayload,
                                size_t *cborSize, int secureFlag);

#ifdef MULTIPLE_OWNER
/**
 * Function to check the credential access of SubOwner
 *
 * @param[in] uuid SubOwner's UUID
 * @param[in] cborPayload CBOR payload of credential
 * @param[in] size Byte length of cborPayload
 *
 * @return ::true for valid access, otherwise invalid access
 */
bool IsValidCredentialAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, size_t size);
#endif //MULTIPLE_OWNER

/**
 * This function generates the bin credential data.
 *
 * @param subject pointer to subject of this credential.
 * @param credType credential type.
 * @param publicData public data such as public key.
 * @param privateData private data such as private key.
 * @param eownerID Entry owner's UUID.
 *
 * @return pointer to instance of @ref OicSecCred_t if successful. else NULL in case of error.

 */
OicSecCred_t * GenerateCredential(const OicUuid_t* subject, OicSecCredType_t credType,
                     const OicSecKey_t * publicData, const OicSecKey_t * privateData,
                     const OicUuid_t * eownerID);

/**
 * This function adds the new cred to the credential list.
 *
 * @param cred is the pointer to new credential.
 *
 * @return ::OC_STACK_OK, cred not NULL and persistent storage gets updated.
 * ::OC_STACK_ERROR, cred is NULL or fails to update persistent storage.
 */
OCStackResult AddCredential(OicSecCred_t * cred);

/**
 * Function to remove credentials from the SVR DB for the given subject UUID.
 * If multiple credentials exist for the UUID, they will all be removed.
 *
 * @param subject is the Credential Subject to be deleted.
 *
 * @return ::OC_STACK_RESOURCE_DELETED if credentials were removed, or
 * if there are no credentials with the given UUID.  An error is returned if
 * removing credentials failed.
 */
OCStackResult RemoveCredential(const OicUuid_t *subject);

/**
 * Function to remove the credential from SVR DB.
 *
 * @param credId is the Credential ID to be deleted.
 *
 * @return ::OC_STACK_OK for success, or errorcode otherwise.
 */
OCStackResult RemoveCredentialByCredId(uint16_t credId);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
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
int32_t GetDtlsPskCredentials( CADtlsPskCredType_t type,
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
OCStackResult AddTmpPskWithPIN(const OicUuid_t* tmpSubject, OicSecCredType_t credType,
                            const char * pin, size_t pinSize,
                            const OicUuid_t * rownerID,
                            OicUuid_t* tmpCredSubject);

#endif // __WITH_DTLS__ or __WITH_TLS__

/**
 * Function to getting credential list
 *
 * @return instance of @ref OicSecCred_t
 */
const OicSecCred_t* GetCredList();

/**
 * Function to deallocate allocated memory to OicSecCred_t.
 *
 * @param cred pointer to cred type.
 *
 */
void DeleteCredList(OicSecCred_t* cred);

/**
 * Internal function to update resource owner
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetCredRownerId(const OicUuid_t* newROwner);

/**
 * Gets the OicUuid_t value for the rownerid of the cred resource.
 *
 * @param rowneruuid a pointer to be assigned to the rowneruuid property
 * @return ::OC_STACK_OK if rowneruuid is assigned correctly, else ::OC_STACK_ERROR.
 */
OCStackResult GetCredRownerId(OicUuid_t *rowneruuid);

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)
/**
 * Used by role certificate validator to get CA certificates as PEM
 *
 * @param[out] crt certificates to be filled.
 * @param[in] usage credential usage string.
 */
OCStackResult GetPemCaCert(ByteArray_t * crt, const char * usage);

/**
 * Get a list of all role certificates. Used when asserting roles.
 *
 * @param[out] roleCerts list of role certificates
 * @return When ::OC_STACK_OK is returned, a list of certificates (roleCerts)
 *         that must be freed with FreeRoleCertChainList. roleCerts can still
 *         be NULL in this case, if no role certs are installed. On error, an
 *         error value is returned and roleCerts is NULL.
 */
OCStackResult GetAllRoleCerts(RoleCertChain_t** roleCerts);

/**
 * Used by mbedTLS to retrieve own certificate chain
 *
 * @param[out] crt certificate chain to be filled.
 * @param[in] usage credential usage string.
 */
void GetPemOwnCert(ByteArray_t * crt, const char * usage);
/**
 * Used by mbedTLS to retrieve own private key
 *
 * @param[out] key key to be filled.
 * @param[in] usage credential usage string.
 */
void GetDerKey(ByteArray_t * key, const char * usage);
/**
 * Used by mbedTLS to retrieve own primary cert private key
 *
 * @param[out] key key to be filled.
 */
void GetPrimaryCertKey(ByteArray_t * key);
/**
 * Used by CA to retrieve credential types
 *
 * @param[out] list list of suites to be filled.
 * @param[in] usage credential usage string.
 * @param[in] device uuid.
 */
void InitCipherSuiteListInternal(bool *list, const char * usage, const char* deviceId);
#endif // __WITH_TLS__

// Helpers shared by cred and roles resources
CborError SerializeEncodingToCbor(CborEncoder *rootMap, const char *tag, const OicSecKey_t *value);
CborError SerializeSecOptToCbor(CborEncoder *rootMap, const char *tag, const OicSecOpt_t *value);
CborError DeserializeEncodingFromCbor(CborValue *rootMap, OicSecKey_t *value);
CborError DeserializeSecOptFromCbor(CborValue *rootMap, OicSecOpt_t *value);
bool IsSameSecOpt(const OicSecOpt_t* sk1, const OicSecOpt_t* sk2);
bool IsSameSecKey(const OicSecKey_t* sk1, const OicSecKey_t* sk2);
/**
 * Delete OicSecCred_t
 *
 * @param[in] cred the pointer to credential usage.
 */
void FreeCred(OicSecCred_t *cred);

#ifdef __cplusplus
}
#endif

/*
 * Check cred rowner uuid
 */
bool IsCredRowneruuidTheNilUuid();

#endif //IOTVT_SRM_CREDR_H
