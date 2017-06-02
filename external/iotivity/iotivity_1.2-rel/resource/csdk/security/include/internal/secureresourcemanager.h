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

#include "securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 * Initialize Policy Engine context.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SRMInitPolicyEngine();

/**
 * Cleanup Policy Engine context.
 */
void SRMDeInitPolicyEngine();

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
 * Function to register provisoning API's response callback.
 *
 * @param respHandler response handler callback.
 */
void SRMRegisterProvisioningResponseHandler(SPResponseCallback respHandler);

/**
 * Check the security resource URI.
 * @param uri Pointers to security resource URI.
 * @return true if the URI is one of security resources, otherwise false.
 */
bool SRMIsSecurityResourceURI(const char* uri);

/**
 * Check whether persistent storage is valid
 * @return OC_STACK_OK if valid, other errors otherwise;
 */
OCStackResult CheckPersistentStorage(OCPersistentStorage* persistentStorageHandler);

/**
 * Get the resource type from the URI.
 * @param   uri [IN] Pointers to security resource URI.
 * @return  SVR type (note that "NOT_A_SVR_RESOURCE" is returned if not a SVR)
 */
OicSecSvrType_t GetSvrTypeFromUri(const char* uri);

/**
 * Sends Response
 * @param   resposeVal       SRMAccessResponse_t value
 * @return  NONE
 */
void SRMSendResponse(SRMAccessResponse_t responseVal);


#ifdef __cplusplus
}
#endif

#endif /* SECURITYRESOURCEMANAGER_H_ */
