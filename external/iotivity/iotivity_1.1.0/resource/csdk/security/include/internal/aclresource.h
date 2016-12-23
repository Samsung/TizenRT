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

#ifndef IOTVT_SRM_ACLR_H
#define IOTVT_SRM_ACLR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize ACL resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult InitACLResource();

/**
 * Perform cleanup for ACL resources.
 *
 */
OCStackResult DeInitACLResource();

/**
 * This method is used by PolicyEngine to retrieve ACL for a Subject.
 *
 * @param subjectId ID of the subject for which ACL is required.
 * @param savePtr is used internally by @ref GetACLResourceData to maintain index between
 *                successive calls for same subjectId.
 *
 * @note On the first call to @ref GetACLResourceData, savePtr should point to NULL.
 *
 * @return reference to @ref OicSecAcl_t if ACL is found, else NULL.
 */
const OicSecAcl_t *GetACLResourceData(const OicUuid_t *subjectId, OicSecAcl_t **savePtr);

/**
 * This function converts ACL data into CBOR format.
 *
 * @param acl instance of @ref OicSecAcl_t structure.
 * @param outPayload is the pointer to allocated memory for cbor payload.
 * @param size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult AclToCBORPayload(const OicSecAcl_t *acl, uint8_t **outPayload, size_t *size);

/**
 * This function deletes ACL data.
 *
 * @param acl instance of @ref OicSecAcl_t structure to be deleted.
 */
void DeleteACLList(OicSecAcl_t *acl);

/**
 * This function installs a new ACL.
 *
 * @param payload cbor value representing a new ACL.
 * @param size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InstallNewACL(const uint8_t *payload, const size_t size);

/**
 * This function updates default ACL which is required for ownership transfer.
 * This function should be invoked after OTM is complete to prevent anonymous user access.
 *
 * @retval OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateDefaultSecProvACL();

/**
 * Internal function to update resource owner
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetAclRownerId(const OicUuid_t *newROwner);


/**
 * Gets the OicUuid_t value for the rownerid of the acl resource.
 *
 * @param rowneruuid a pointer to be assigned to the rowneruuid property
 * @return ::OC_STACK_OK if rowneruuid is assigned correctly, else ::OC_STACK_ERROR.
 */
OCStackResult GetAclRownerId(OicUuid_t *rowneruuid);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_ACLR_H


