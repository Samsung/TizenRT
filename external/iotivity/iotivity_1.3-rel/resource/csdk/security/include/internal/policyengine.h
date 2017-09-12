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

#ifndef IOTVT_SRM_PE_H
#define IOTVT_SRM_PE_H

#include "ocstack.h"
#include "logger.h"
#include "experimental/securevirtualresourcetypes.h"
#include "secureresourcemanager.h"
#include "cainterface.h"
#include <stdlib.h>
#include <stdint.h>

/**
 * Check whether a request should be allowed.
 *
 * @param context is the pointer to SRM Request Context to use.
 *
 * Upon return, context->responseVal will be set to ACCESS_GRANTED if request
 * should be processed, otherwise context->responseVal will be set to
 * some flavor of ACCESS_DENIED.
 */
void CheckPermission( SRMRequestContext_t *context );

/**
 * Get CRUDN permission for a method.
 *
 * @param method is CRUDN permission being seeked.
 *
 * @return the uint16_t CRUDN permission .
 */
uint16_t GetPermissionFromCAMethod_t(const CAMethod_t method);

typedef OCStackResult (*GetSvrRownerId_t)(OicUuid_t *rowner);

#endif //IOTVT_SRM_PE_H
