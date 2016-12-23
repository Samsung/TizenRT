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

#ifndef IOTVT_SRM_PSI_H
#define IOTVT_SRM_PSI_H

#include "cJSON.h"

/**
 * Reads the Secure Virtual Database from PS into dynamically allocated
 * memory buffer.
 *
 * @note Caller of this method MUST use OCFree() method to release memory
 *       referenced by return value.
 *
 * @return char * reference to memory buffer containing SVR database.
 */
char *GetSVRDatabase();

/**
 * This method is used by a entity handlers of SVR's to update
 * SVR database.
 *
 * @param rsrcName string denoting the SVR name ("acl", "cred", "pstat" etc).
 * @param jsonObj JSON object containing the SVR contents.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSVRDatabase(const char *rsrcName, cJSON *jsonObj);

/**
 * Reads the Secure Virtual Database from PS
 *
 * @note Caller of this method MUST use OCFree() method to release memory
 *       referenced by return value.
 *
 * @param rsrcName is the name of the field for which file content are read.
                   if the value is NULL it will send the content of the whole file.
 * @param data is the pointer to the file contents read from the database.
 * @param size is the size to the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult GetSecureVirtualDatabaseFromPS(const char *rsrcName, uint8_t **data, size_t *size);

/**
 * This method converts updates the persistent storage.
 *
 * @param rsrcName is the name of the secure resource that will be updated.
 * @param cborPayload is the pointer holding cbor payload.
 * @param cborPayload is the size of the cbor payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSecureResourceInPS(const char *rsrcName, uint8_t *cborPayload, size_t size);

#endif //IOTVT_SRM_PSI_H
