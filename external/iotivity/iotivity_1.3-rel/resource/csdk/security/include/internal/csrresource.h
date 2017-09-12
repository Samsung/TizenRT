//******************************************************************
//
// Copyright 2017 Microsoft
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
//******************************************************************

#ifndef IOTVT_SRM_CSRR_H
#define IOTVT_SRM_CSRR_H

#include "cainterface.h"
#include "experimental/securevirtualresourcetypes.h"
#include "octypes.h"

#if defined(__WITH_TLS__) || defined(__WITH_DTLS__)
#ifdef __cplusplus
extern "C" {
#endif
/**
 * Initialize the CSR resource.
 *
 * @return OC_STACK_OK if successful; error otherwise.
 */
OCStackResult InitCSRResource();

/**
 * De-initialize the CSR resource.
 *
 * @return OC_STACK_OK if successful; error otherwise.
 */
OCStackResult DeInitCSRResource();

/**
 * This function converts a CBOR payload into a CSR.
 * Caller needs to call 'OICFree' on *csr after use.
 *
 * @param[in] cborPayload Received CBOR payload to extract the CSR from
 * @param[in] size Size of cborPayload
 * @param[out] csr Buffer containing the CSR
 * @param[out] csrLen Length of csr
 * @param[out] encoding Encoding of CSR (OIC_ENCODING_PEM or OIC_ENCODING_DER)
 */
OCStackResult CBORPayloadToCSR(const uint8_t *cborPayload, size_t size, 
                               uint8_t **csr, size_t *csrLen, 
                               OicEncodingType_t *encoding);
#ifdef __cplusplus
}
#endif
#endif

#endif
