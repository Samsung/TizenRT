/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

#include <mbedtls/ssl.h>
#include "ss_emul.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Error-definition for hw interface
 */

typedef enum HwifResult
{
    HWIF_ERR_INVALID_PARAM    = -100, /**< Invalid Paramter */
    HWIF_ERR_OUT_OF_MEMORY   = -99,  /**< Out of memory */
    HWIF_ERR_NO_DATA                = -98,  /**< No data found */
    HWIF_ERROR         = -1,  /**< Internal Error */
    HWIF_SUCCESS     = 0,    /**< No Error */
} HwifResult_t;


/**
 * This callback will be invoked to get a key context based on specific name indication
 * (service name, key usage, key type). The key context may be same with the alias name.
 *
 * @param[in] service  service name indicates first category name
 * @param[in] usage  usage name indicates sub-category name
 * @param[in] keytype  (optional) key type name indication if any, otherwise it usually will be NULL
 * @return  void type pointer value on success, otherwise NULL
 */
void* HWGetKeyContext(const char* service, const char* usage, const char* keytype);

/**
 * This callback will deallocate the key context that was retrieved from TZ
 * by calling GetHwKeyContext callback.
 *
 * @param[in] keyContext  key context object to be deallocated,
 *                   which was obtained from GetHwKeyContext callback function
 * @return  0 on success, otherwise a negative value
 */
int HWFreeKeyContext(void* keyContext);

/**
 * This callback will be invoked to load own(i.e., pre-injected) certificate from HW(e.g., TZ, eSE)
 *
 * @param[in] keyContext  key context object that identifies proper certificate chain
 * @param[out] cert_chain  certificate chain in binary
 * @param[out] cert_chain_len  total length of certificate chain
 * @return  0 on success, otherwise a negative value
 */
int HWGetOwnCertificateChain(const void* keyContext,
                    unsigned char** cert_chain, size_t* cert_chain_len);

/**
 * This callback should provide setting up alternative functions (e.g., rsa_sign, key_len, etc)
 * of which HW(e.g., TZ, eSE) management library to the specified mbedtls context
 * that will be used during handshake.
 *
 * @param[in] ctx  pointer of pk context of mbedtls
 * @param[in] keyContext  key context object that identifies proper public/private key
 * @return  0 on success, otherwise a negative value
 */
int HWSetupPkContext(mbedtls_pk_context* ctx, void* key_context);

#ifdef __cplusplus
}
#endif

#endif // HW_INTERFACE_H

