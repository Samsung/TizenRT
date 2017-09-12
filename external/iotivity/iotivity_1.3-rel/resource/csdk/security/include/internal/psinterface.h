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

/**
 * Reads the database from PS
 *
 * @note Caller of this method MUST use OICFree() method to release memory
 *       referenced by the data argument.
 *
 * @param databaseName is the name of the database to access through persistent storage.
 * @param resourceName is the name of the field for which file content are read.
 *                     if the value is NULL it will send the content of the whole file.
 * @param data         is the pointer to the file contents read from the database.
 * @param size         is the size of the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult ReadDatabaseFromPS(const char *databaseName, const char *resourceName, uint8_t **data, size_t *size);

/**
 * This method updates the database in PS
 *
 * @param databaseName  is the name of the database to access through persistent storage.
 * @param resourceName  is the name of the resource that will be updated.
 * @param payload       is the pointer to memory where the CBOR payload is located.
 * @param size          is the size of the CBOR payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateResourceInPS(const char *databaseName, const char *resourceName, const uint8_t *payload, size_t size);

/**
 * Reads the Secure Virtual Database from PS into dynamically allocated
 * memory buffer.
 *
 * @note Caller of this method MUST use OICFree() method to release memory
 *       referenced by return value.
 *
 * @return char * reference to memory buffer containing SVR database.
 */
char * GetSVRDatabase();

/**
 * Reads the Secure Virtual Database from PS
 *
 * @note Caller of this method MUST use OICFree() method to release memory
 *       referenced by return value.
 *
 * @param resourceName is the name of the field for which file content are read.
 *                     if the value is NULL it will send the content of the whole file.
 * @param data         is the pointer to the file contents read from the database.
 * @param size         is the size of the file contents read.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult GetSecureVirtualDatabaseFromPS(const char *resourceName, uint8_t **data, size_t *size);

/**
 * This method updates the Secure Virtual Database in PS
 *
 * @param resourceName  is the name of the secure resource that will be updated.
 * @param payload       is the pointer to memory where the CBOR payload is located.
 * @param size          is the size of the CBOR payload.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult UpdateSecureResourceInPS(const char *resourceName, const uint8_t *payload, size_t size);

/**
 * This method resets the secure resources according to the reset profile.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult ResetSecureResourceInPS(void);

/**
 * This method creates the reset profile from the persistent storage.
 * The reset profile is the copy of the initial state of SVR resources
 * when secure resources are initiated at first.
 * In remote reset, the SVR will be reset according to the reset profile.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult CreateResetProfile(void);

#endif //IOTVT_SRM_PSI_H
