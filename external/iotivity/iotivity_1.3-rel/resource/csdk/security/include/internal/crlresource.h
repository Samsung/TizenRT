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

#ifndef IOTVT_SRM_CRLR_H
#define IOTVT_SRM_CRLR_H

#include "ocpayload.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function stores CRL in SRM.
 *
 * @param crl to be stored in SRM.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult UpdateCRLResource(OicSecCrl_t *crl);

/**
 * This function get encoded with base64 CRL from SRM.
 *
 * @note Caller responsible for resulting string memory (use OICFree to remove it).
 *
 * @return NULL if error occured (e.g. CRL did not set).
 */
uint8_t* GetCrl();

/**
 * This function get encoded with DER CRL from SRM.
 *
 * @return encoded CRL with DER format. array len is 0 if error occured (e.g. CRL did not set).
 */
void  GetDerCrl(ByteArray_t* crlArray);

/**
 * This function converts CRL to CBOR
 *
 * @param crl is a pointer to buffer that contains crl. Shoul be not NULL. Buffer
 * will be allocated by the function and content of *crl will be ignored.
 * @param payload is the converted cbor value.
 * @param size is a pointer to length of the CRL buffer. Should be not NULL.
 * @param lastUpdate optional field contains CRL last update time
 *
 * @note Caller responsible for crl buffer memory (use OICFree to free it).
 *
 * @return ::OC_STACK_OK if success, otherwise some error value.
 */
OCStackResult CrlToCBORPayload(const OicSecCrl_t *crl, uint8_t **payload, size_t *size,
                               char *lastUpdate);

/**
 * This function converts CBOR to CRL
 *
 * will be allocated by the function and content of *crl will be ignored.
 * @param cborPayload is the cbor vlaue
 * @param size is a pointer to length of the CRL buffer. Should be not NULL.
 * @param secCrl is a pointer to buffer that contains crl. Shoul be not NULL. Buffer
 *
 * @note Caller responsible for crl buffer memory (use OICFree to free it).
 *
 * @return ::OC_STACK_OK if success, otherwise some error value.
 */
OCStackResult CBORPayloadToCrl(const uint8_t *cborPayload, const size_t size,
                               OicSecCrl_t **secCrl);
/**
 * Initialize CRL resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult InitCRLResource();

/**
 * Perform cleanup for CRL resources.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult DeInitCRLResource();

/**
 * Get an instance of CRL resource.
 *
 * @return reference to the OicSecCrl_t, holding reference to CRL resource.
 */
OicSecCrl_t *GetCRLResource();

/**
 * This function frees resources inside given OicSecCrl_t object
 *
 * @param crl  crl object
 */
void DeleteCrl(OicSecCrl_t *crl);

/**
 * This function gets time when CRL was last updated in database
 * Function allocates memory for lastUpdate and
 * Caller is responsible to free that memory
 *
 * @param lastUpdate    pointer to last update string.
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult getLastUpdateFromDB(char **lastUpdate);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_CRLR_H
