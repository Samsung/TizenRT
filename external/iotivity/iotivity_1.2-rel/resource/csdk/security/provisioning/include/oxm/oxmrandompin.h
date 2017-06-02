//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#ifndef OXM_RANDOM_PIN_H_
#define OXM_RANDOM_PIN_H_

#include "ocstack.h"
#include "securevirtualresourcetypes.h"
#include "ownershiptransfermanager.h"
#include "pmtypes.h"
#include "pinoxmcommon.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Callback implementation to input the PIN code from user.
 *
 * @param otmCtx Context of OTM, It includes current device information.
 *
 * @return ::OC_STACK_SUCCESS in case of success and other value otherwise.
 */
OCStackResult InputPinCodeCallback(OTMContext_t *otmCtx);

/**
 * Callback implemenration to establish a secure channel with PSK cipher suite.
 *
 * @param otmCtx Context of OTM, It includes current device information.
 *
 * @return ::OC_STACK_SUCCESS in case of success and other value otherwise.
 */
OCStackResult CreateSecureSessionRandomPinCallback(OTMContext_t *otmCtx);

/**
 * Generate payload for select OxM request.
 *
 * @param otmCtx Context of OTM, It includes current device information.
 * @param cborPaylaod is the DOXM CBOR payload including the selected OxM.
 * @note Returned memory should be deallocated by caller.
 * @param cborSize is the size of the cborPayload.
 *
 * @return ::OC_STACK_SUCCESS in case of success and other value otherwise.
 */
OCStackResult CreatePinBasedSelectOxmPayload(OTMContext_t *otmCtx, uint8_t **cborPayload,
                                             size_t *cborSize);

/**
 * Generate payload for owner transfer request.
 *
 * @param otmCtx Context of OTM, It includes current device information.
 * @param cborPaylaod is the DOXM CBOR payload including the owner information.
 * @note Returned memory should be deallocated by caller.
 * @param cborSize is the size of the cborPayload.
 *
 * @return ::OC_STACK_SUCCESS in case of success and other value otherwise.
 */
OCStackResult CreatePinBasedOwnerTransferPayload(OTMContext_t *otmCtx, uint8_t **cborPayload,
                                                 size_t *cborSize);

#ifdef __cplusplus
}
#endif
#endif //OXM_RANDOM_PIN_H_
