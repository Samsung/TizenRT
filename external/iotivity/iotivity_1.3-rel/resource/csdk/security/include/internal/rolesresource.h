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

#ifndef IOTVT_SRM_ROLESR_H
#define IOTVT_SRM_ROLESR_H

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)

#include "cainterface.h"
#include "experimental/securevirtualresourcetypes.h"
#include "octypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RoleCertChain {
    uint32_t                credId;             /**< locally assigned ID number for use with DELETE */
    OicSecKey_t             certificate;        /**< certificate chain including leaf and intermediate CA certificates */

    struct RoleCertChain    *next;              /**< next chain in list */
} RoleCertChain_t;

/**
 * Initialize the roles resource.
 *
 * @return OC_STACK_OK if successful; error otherwise
 */
OCStackResult InitRolesResource();

/**
 * De-initialize the roles resource.
 *
 * @return OC_STACK_OK if successful; error otherwise
 */
OCStackResult DeInitRolesResource();

/**
 * Register the PSK credential being used for authentication. This is used by the cred resource
 * to inform the roles resource a symmetric PSK has a particular role associated with it.
 *
 * @param[in]  cred         PSK credential to register
 *
 * @return OC_STACK_OK if role credential is successfully registered; error otherwise.
 */
OCStackResult RegisterSymmetricCredentialRole(const OicSecCred_t *cred);

/**
 * Retrieve the roles asserted by a given endpoint with certificates.
 *
 * @param[in] endpoint      Endpoint to retrieve roles for
 * @param[out] roles        Pointer to receive array of OicSecRole_t objects containing roles for this endpoint
 *                          On success, caller must free the received array with OICFree when finished
 * @param[out] roleCount    Variable to receive length of roles array.
 *
 * @note If the endpoint is found but has not asserted any roles with certificates,
 *       OC_STACK_OK will be returned, but NULL will be returned in roles and 0 in roleCount.
 *
 * @return OC_STACK_OK if list of roles is successfully populated; error otherwise.
 */
OCStackResult GetEndpointRoles(const CAEndpoint_t *endpoint, OicSecRole_t **roles, size_t *roleCount);

/**
 * This function converts a CBOR payload into a list of role certificates.
 * Caller needs to call 'OICFree' on *roleCertList after use.
 *
 * @param[in] cborPayload Received CBOR payload to extract the role cert list from
 * @param[in] size Size of cborPayload
 * @param[out] roleCertList Pointer to receive linked list of RoleCertChain_t objects
 *             On success, caller must call FreeRoleCertChainList on *roleCertList when finished
 *
 * @return OC_STACK_OK if payload is successfully converted; error code otherwise
 */
OCStackResult CBORPayloadToRoles(const uint8_t *cborPayload, size_t size, RoleCertChain_t **roleCertList);

/**
 * This function converts a list of role certificates into a CBOR payload.
 * Caller needs to call 'OICFree' on *cborPayload after use.
 *
 * @param[in] roles Linked list of RoleCertChain_t objects
 * @param[out] cborPayload Pointer to receive the CBOR payload
 *             On success, caller must call OICFree on *cborPayload when finished
 * @param[out] size Pointer to receive size of cborPayload
 *
 * @return OC_STACK_OK if payload is successfully converted; error code otherwise
 */
OCStackResult RolesToCBORPayload(const RoleCertChain_t *roles, uint8_t **cborPayload, size_t *cborSize);

/**
 * Free the memory used by a list of RoleCertChain_t objects created by CBORPayloadToRoles.
 *
 * @param[in] roleCertList List received from CBORPayloadToRoles
 */
void FreeRoleCertChainList(RoleCertChain_t *roleCertList);

#ifdef __cplusplus
}
#endif

#endif /* defined(__WITH_DTLS__) || defined(__WITH_TLS__) */

#endif /* IOTVT_SRM_ROLESR_H */
