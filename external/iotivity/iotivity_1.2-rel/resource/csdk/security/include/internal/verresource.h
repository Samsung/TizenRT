/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#ifndef IOTVT_SRM_VER_H
#define IOTVT_SRM_VER_H

#include "octypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize VER resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult InitVerResource();

/**
 * Perform cleanup for VER resources.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult DeInitVerResource();

/**
 * This method is used by SRM to retrieve VER resource data..
 *
 * @return reference to @ref OicSecDoxm_t, binary format of Doxm resource data.
 */
const OicSecVer_t* GetVerResourceData();

/**
 * This method converts CBOR VER into binary VER.
 * The CBOR VER can be from persistent database or
 * or received as PUT/POST request.
 *
 * @param cborPayload is a ver data in cbor.
 * @note Caller needs to invoke OCFree after done using the return pointer.
 * @param doxm is the pointer to @ref OicSecVer_t.
 * @param size of the cborPayload. In case value is 0, CBOR_SIZE value is assigned.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult CBORPayloadToVer(const uint8_t *cborPayload, size_t size,
                                OicSecVer_t **ver);

/**
 * This method converts VER data into CBOR format.
 * Caller needs to invoke 'free' when finished done using
 * return string.
 *
 * @param ver Pointer to @ref OicSecVer_t.
 * @note Caller needs to invoke OCFree after done using the return pointer.
 * @param cborPayload is the payload of the cbor.
 * @param cborSize is the size of the cbor payload. Passed parameter should not be NULL.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult VerToCBORPayload(const OicSecVer_t * ver, uint8_t **cborPayload,
                                size_t *cborSize);

/**
 * Get the security version.
 *
 * @return the version string of security.
 */
const char* GetSecVersion();

/** This function deallocates the memory for OicSecVer_t .
 *
 * @param ver is the pointer to @ref OicSecVer_t.
 */
void DeleteVerBinData(OicSecVer_t* ver);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_VER_H