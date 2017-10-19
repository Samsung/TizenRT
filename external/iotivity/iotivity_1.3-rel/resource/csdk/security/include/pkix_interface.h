/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef PKIX_INTERFACE_H
#define PKIX_INTERFACE_H

#include "cainterface.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * This method is used by mbedTLS/SRM to retrieve PKIX related info
 *
 * @param[out] inf structure with certificate, private key and crl to be filled.
 *
 */
void GetPkixInfo(PkiInfo_t * inf);
/**
 * This method is used by mbedTLS/SRM to retrieve manufacturer PKIX related info
 *
 * @param[out] inf structure with certificate, private key and crl to be filled.
 *
 */
void GetManufacturerPkixInfo(PkiInfo_t * inf);

/**
 * Used by CA to retrieve credential types
 *
 * @param[out] list TLS suites boolean map.
 * @param[in]  deviceId uuid.
 */
void InitCipherSuiteList(bool * list, const char* deviceId);

/**
 * Used by CA to retrieve manufacturer credential types
 *
 * @param[out] list TLS suites boolean map.
 * @param[in]  deviceId uuid.
 */
void InitManufacturerCipherSuiteList(bool * list, const char* deviceId);
#ifdef __cplusplus
}
#endif

#endif //PKIX_INTERFACE_H
