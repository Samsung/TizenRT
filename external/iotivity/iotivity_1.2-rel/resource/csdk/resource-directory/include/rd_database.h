//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef OC_RESOURCE_DIRECTORY_DATABASE_H_
#define OC_RESOURCE_DIRECTORY_DATABASE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef RD_SERVER

/**
 * Opens the RD publish database.
 *
 * @param path to the database file.
 *
 * @return ::OC_STACK_OK in case of success or else other value.
 */
OCStackResult OCRDDatabaseInit(const char *path);

/**
 * Stores in database the published resource.
 *
 * @param payload is the the published resource payload.
 * @param address provide information about endpoint connectivity details.
 *
 * @return ::OC_STACK_OK in case of success or else other value.
 */
OCStackResult OCRDDatabaseStoreResources(const OCRepPayload *payload, const OCDevAddr *address);

/**
 * Updates the RD resource
 *
 * @param deviceId of the device for which resources will be update.
 *
 * @return ::OC_STACK_OK in case of success or else other value.
 */
OCStackResult OCRDDatabaseUpdateDevice(const char *deviceId);

/**
 * Delete the RD resource
 *
 * @param deviceId of the device for which resources will be deleted.
 *
 * @return ::OC_STACK_OK in case of success or else other value.
 */
OCStackResult OCRDDatabaseDeleteDevice(const char *deviceId);

/**
 * Close the RD publish database.
 *
 * @return ::OC_STACK_OK in case of success or else other value.
 */
OCStackResult OCRDDatabaseClose();

#endif

#ifdef __cplusplus
}
#endif

#endif
