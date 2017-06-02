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
#ifdef HW_PKIX
#include "mbedtls/ssl.h"
#endif //HW_PKIX

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pre-Definition for key context
 */
// IoTivity service name for HW key context query
#define HWKEY_SVC_IOTIVITY "iotivity"

// Default usage name for HW key context query
#define HWKEY_USAGE_PRIMARY "primary"


/**
 * Callback function for HW
 */
//this callback will be invoked to get key context based on key usage
typedef void* (*GetHwKeyContext)(const char* service, const char* usage, const char* keytype);

//this callback will free key context that was retreived from TZ
typedef int (*FreeHwKeyContext)(void* keyContext);

//this callback will be invoked to load own certificate in case of TZ
typedef int (*GetOwnCertFromHwCallback)(const void* keyContext, uint8_t** certChain, size_t* certChainLen);

//this callback will be invoked to load private key in case of TZ
typedef int (*SetupPkContextFromHwCallback)(mbedtls_pk_context* ctx, void* keyContext);

typedef struct HWPkixContext {
    GetHwKeyContext getHwKeyContext;
    FreeHwKeyContext freeHwKeyContext;
    GetOwnCertFromHwCallback getOwnCertCb;
    SetupPkContextFromHwCallback setupPkContextCb;
    void* hwKeyCtx;
}HWPkixContext_t;

/**
 * API to seting callbacks which is requried to use H/W based PKI
 *
 * @param[in] getHwKeyContext callback to get hw key context
 * @param[in] freeHwKeyContext callback to free hw key context
 * @param[in] getOwnCertCb callback to load certificate chain
 * @param[in] setupPkContextCb callback to setup PK context
 *
 * return 0 on success
 */
int SetHwPkixCallbacks(GetHwKeyContext getHwKeyContext,
                        FreeHwKeyContext freeHwKeyContext,
                        GetOwnCertFromHwCallback getOwnCertCb,
                        SetupPkContextFromHwCallback setupPkContextCb);

/**
 * API to invoke the callback for setup PK context with H/W based PKI
 *
 * @param[in] pkCtx mbedtls's PK context
 *
 * return 0 on success
 */
int SetupHwPkContext(mbedtls_pk_context* pkCtx);

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
 */
void InitCipherSuiteList(bool * list);

/**
 * Used by CA to retrieve manufacturer credential types
 *
 * @param[out] list TLS suites boolean map.
 */
void InitManufacturerCipherSuiteList(bool * list);
#ifdef __cplusplus
}
#endif

#endif //PKIX_INTERFACE_H
