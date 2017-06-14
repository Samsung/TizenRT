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

#ifndef IOTVT_SRM_SVCR_H
#define IOTVT_SRM_SVCR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize SVC resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult InitSVCResource();

/**
 * Perform cleanup for SVC resources.
 */
void DeInitSVCResource();

/**
 * This function converts SVC data into CBOR format.
 * Caller needs to invoke 'free' when done using returned string.
 *
 * @param svc is the instance of @ref OicSecSvc_t structure. In case of NULL it
 * will return ::OC_STACK_INVALID_PARAM.
 * @param cborPayload is the converted cbor value of SVC structure.
 * @param cborSize is the size of the cbor payload. This value is the size of the
 * cborPayload. It should not be NON-NULL value.
 *
 * @return ::OC_STACK_OK for Success. ::OC_STACK_INVALID in case of invalid parameters.
 * ::OC_STACK_ERROR in case of error in converting to cbor.
 */
 OCStackResult SVCToCBORPayload(const OicSecSvc_t *svc, uint8_t **cborPayload,
                                size_t *cborSize);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_SVCR_H
