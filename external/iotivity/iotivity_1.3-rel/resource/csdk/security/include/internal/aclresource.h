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
 * @return reference to @ref OicSecAce_t if ACE is found, else NULL.
 */
const OicSecAce_t* GetACLResourceData(const OicUuid_t* subjectId, OicSecAce_t **savePtr);

/**
 * This method is used by PolicyEngine to retrieve ACLs for a set of roles.
 *
 * @param[in] roles Array of roles to check for.
 * @param[in] roleCount Length of roles array.
 * @param[out] savePtr is used internally by @ref GetACLResourceDataByRoles to maintain index between
 *                     successive calls for the same roles list.
 *
 * @note On the first call to @ref GetACLResourceDataByRoles, savePtr should point to NULL.
 *
 * @return reference to @ref OicSecAce_t if ACE is found, else NULL.
 */
const OicSecAce_t* GetACLResourceDataByRoles(const OicSecRole_t *roles, size_t roleCount, OicSecAce_t **savePtr);

/**
 * This method is used by PolicyEngine to retrieve ACLs for a given conntype.
 *
 * @param[in] conntype Conntype to match in ACE.
 * @param[out] savePtr is used internally by @ref GetACLResourceDataByConntype to maintain index between
 *                     successive calls for the same roles list.
 *
 * @note On the first call to @ref GetACLResourceDataByConntype, savePtr should point to NULL.
 *
 * @return reference to @ref OicSecAce_t if ACE is found, else NULL.
 */
const OicSecAce_t* GetACLResourceDataByConntype(const OicSecConntype_t conntype, OicSecAce_t **savePtr);

/**
 * This function converts ACL data into CBOR format.
 *
 * @param acl instance of @ref OicSecAcl_t structure.
 * @param aclVersion Version of ACL resource to produce; can be OIC_SEC_ACL_V1 or OIC_SEC_ACL_V2
 * @param outPayload is the pointer to allocated memory for cbor payload.
 * @param size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult AclToCBORPayload(const OicSecAcl_t *acl, OicSecAclVersion_t aclVersion, uint8_t **outPayload, size_t *size);

#ifdef MULTIPLE_OWNER
/**
 * Function to check the ACL access of SubOwner
 *
 * @param[in] uuid SubOwner's UUID
 * @param[in] cborPayload CBOR payload of ACL
 * @param[in] size Byte length of cborPayload
 *
 * @return ::true for valid access, otherwise invalid access
 */
bool IsValidAclAccessForSubOwner(const OicUuid_t* uuid, const uint8_t *cborPayload, const size_t size);
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
OCStackResult RemoveACE(const OicUuid_t * subject, const char * resource);

/**
 * This function deletes ACL data.
 *
 * @param acl instance of @ref OicSecAcl_t structure to be deleted.
 */
void DeleteACLList(OicSecAcl_t* acl);

/**
 * This function frees OicSecRsrc_t object's fields and object itself.
 *
 * @param rsrc instance of @ref OicSecRsrc_t structure to be deleted.
 */
void FreeRsrc(OicSecRsrc_t *rsrc);

/**
 * This function check the duplication with pre-installed ACL and installs only new ACEs.
 *
 * @param acl  acl to install.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InstallACL(const OicSecAcl_t* acl);

/**
 * This function appends a new ACL.
 *
 * @param payload cbor value representing a new ACL.
 * @param size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult AppendACL(const uint8_t* payload, const size_t size);

/**
 * This function appends a new ACL.
 *
 * @param acl  new acl to append.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult AppendACLObject(const OicSecAcl_t* acl);

/**
 * This function updates default ACE which is required for ownership transfer.
 * This function should be invoked after OTM is complete to prevent anonymous user access.
 *
 * @retval OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateDefaultSecProvACE();

/**
 * Internal function to update resource owner
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetAclRownerId(const OicUuid_t* newROwner);


/**
 * Gets the OicUuid_t value for the rownerid of the acl resource.
 *
 * @param rowneruuid a pointer to be assigned to the rowneruuid property
 * @return ::OC_STACK_OK if rowneruuid is assigned correctly, else ::OC_STACK_ERROR.
 */
OCStackResult GetAclRownerId(OicUuid_t *rowneruuid);

#if defined(TCP_ADAPTER) && defined(WITH_CLOUD)
/**
 * This function converts CBOR data into ACL.
 *
 * @param cborPayload is the pointer to cbor payload to parse.
 * @param size of the cbor payload.
 *
 * @return instance of @ref OicSecAcl_t structure or NULL if error occurs
 */
OicSecAcl_t* CBORPayloadToCloudAcl(const uint8_t *cborPayload, const size_t size);
#endif //defined(TCP_ADAPTER) && defined(WITH_CLOUD)

/*
 * Check acl rowner uuid 
 */
bool IsAclRowneruuidTheNilUuid();

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_ACLR_H


