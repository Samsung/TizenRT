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

#ifndef SECURITYRESOURCEMANAGER_H_
#define SECURITYRESOURCEMANAGER_H_

#include "experimental/securevirtualresourcetypes.h"
#include "cainterface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SubjectIdentityType
{
    SUBJECT_ID_TYPE_ERROR = 0,
    SUBJECT_ID_TYPE_UUID,     // Subject refers to a UUID
    SUBJECT_ID_TYPE_ROLE,     // Subject refers to a Role
} SubjectIdentityType_t;

/**
 * The context for a single request to be processed by the Security
 * Resource Manager.
 */
typedef struct SRMRequestContext
{
    const CAEndpoint_t      *endPoint;                          // ptr to the Endpoint for this request
    OicSecSvrType_t         resourceType;                       // SVR type (or "not an SVR")
    char                    resourceUri[MAX_URI_LENGTH + 1];    // URI of the requested resource
    uint16_t                requestedPermission;                // bitmask permissions of request
    CAResponseInfo_t        responseInfo;                       // The response for this request
    bool                    responseSent;                       // Is servicing this request complete?
    SRMAccessResponse_t     responseVal;                        // The SRM internal response code
    const CARequestInfo_t   *requestInfo;                       // ptr to info for this request
    bool                    secureChannel;                      // Was request recv'd over secure channel?
    bool                    slowResponseSent;                   // Is a full response still needed?
    OicSecDiscoverable_t    discoverable;                       // Is resource discoverable?
    SubjectIdentityType_t   subjectIdType;                      // The type of Subject ID in this
                                                                // request.
    OicUuid_t               subjectUuid;                        // The UUID of the Subject (valid
                                                                // iff IdType is UUID_TYPE).
    // Developer note: when adding support for an additional type (e.g.
    // ROLE_TYPE) suggest adding a new var to hold the Subject ID for that type.
#ifdef MULTIPLE_OWNER
    uint8_t*                payload;
    size_t                  payloadSize;
#endif //MULTIPLE_OWNER
} SRMRequestContext_t;

/**
 * Register Persistent storage callback.
 *
 * @param  persistentStorageHandler [IN] Pointers to open, read, write, close & unlink handlers.
 *
 * @return ::OC_STACK_OK  is no errors and successful. ::OC_STACK_INVALID_PARAM for invalid parameter.
 */
OCStackResult SRMRegisterPersistentStorageHandler(OCPersistentStorage* persistentStorageHandler);

/**
 * Get Persistent storage handler pointer.
 *
 * @return The pointer to Persistent Storage callback handler.
 */
OCPersistentStorage* SRMGetPersistentStorageHandler();

/**
 * Register request and response callbacks. Requests and responses are delivered in these callbacks.
 *
 * @param reqHandler Request handler callback ( for GET,PUT ..etc)
 * @param respHandler Response handler callback.
 * @param errHandler Error handler callback.
 *
 * @return ::OC_STACK_OK  is no errors and successful. ::OC_STACK_INVALID_PARAM for invalid parameter.
 */
OCStackResult SRMRegisterHandler(CARequestCallback reqHandler,
                                 CAResponseCallback respHandler,
                                 CAErrorCallback errHandler);

/**
 * Initialize all secure resources ( /oic/sec/cred, /oic/sec/acl, /oic/sec/pstat etc).
 * @return  ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SRMInitSecureResources();

/**
 * Perform cleanup for secure resources ( /oic/sec/cred, /oic/sec/acl, /oic/sec/pstat etc).
 */
void SRMDeInitSecureResources();

/**
 * Provisioning API response callback.
 *
 * @param object endpoint instance.
 * @param responseInfo instance of CAResponseInfo_t structure.
 *
 * @return true if received response is for provisioning API false otherwise.
 */
typedef bool (*SPResponseCallback) (const CAEndpoint_t *object,
                                    const CAResponseInfo_t *responseInfo);

/**
 * Check the security resource URI.
 * @param uri Pointers to security resource URI.
 * @return true if the URI is one of security resources, otherwise false.
 */
bool SRMIsSecurityResourceURI(const char* uri);

/**
 * Get the resource type from the URI.
 * @param   uri [IN] Pointers to security resource URI.
 * @return  SVR type (note that "NOT_A_SVR_RESOURCE" is returned if not a SVR)
 */
OicSecSvrType_t GetSvrTypeFromUri(const char* uri);

extern const OicSecRole_t EMPTY_ROLE;

/**
 * Determine if a role is non-empty.
 *
 * @param[in]  role     Role to check
 *
 * @return true if role is non-empty, false if role is empty
 */
bool IsNonEmptyRole(const OicSecRole_t *role);

#ifdef __cplusplus
}
#endif

#endif /* SECURITYRESOURCEMANAGER_H_ */
