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

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)

#ifndef OCSECURITY_H_
#define OCSECURITY_H_

#include "cacommon.h"
#include "octypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback function used when calling OCAssertRoles
 *
 * @param[in] ctx      Context string used by callback in debug output
 * @param[out] hasError Set to true if the asserting roles succeeded
 */
typedef void(*OCAssertRolesCB)(void* ctx, bool hasError);

/**
 * Assert all roles to a device. This POSTs all role certificates from the
 * local cred resource to /oic/sec/roles.
 * 
 * @param[in] ctx User context to be passed to resultCallback
 * @param[in] devAddr  The device to assert the roles to
 * @param[in] resultCallback Callback that is called with the response from the device
 * @return ::OC_STACK_OK in case of success and other value otherwise. If no role certificates
 *         are installed, the error code OC_STACK_INCONSISTENT_DB is returned.
 *
 * @note The port field of the devAddr parameter MUST contain the secure port of the peer. 
 *
 */
OCStackResult OC_CALL OCAssertRoles(void *ctx, const OCDevAddr *devAddr, OCAssertRolesCB resultCallback);

#ifdef __cplusplus
}
#endif

#endif /* OCSECURITY_H_ */

#endif /* defined(__WITH_TLS__) || defined(__WITH_DTLS__) */