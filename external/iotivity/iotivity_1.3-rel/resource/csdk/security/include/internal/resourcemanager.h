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

#ifndef IOTVT_SRM_RM_H
#define IOTVT_SRM_RM_H

#include <stdlib.h>
#include "ocstack.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize all secure resources ( /oic/sec/cred, /oic/sec/acl, /oic/sec/pstat etc).
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InitSecureResources();

/**
 * Perform cleanup for secure resources ( /oic/sec/cred, /oic/sec/acl, /oic/sec/pstat etc).
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult DestroySecureResources();

/**
 * Reset SVRs to backup values kept in persistent storage.  These backup values
 * were created at first device boot, and should be a copy of the values
 * configured by the mfr in the initial persistent storage data file.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult ResetSecureResources();

/**
 * This method is used by all secure resource modules to send responses to REST queries.
 *
 * @param ehRequest pointer to entity handler request data structure.
 * @param ehRet result code from entity handler.
 * @param cborPayload response payload.
 * @param size is the cborpayload size
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SendSRMResponse(const OCEntityHandlerRequest *ehRequest,
        OCEntityHandlerResult ehRet, uint8_t *cborPayload, size_t size);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_RM_H

