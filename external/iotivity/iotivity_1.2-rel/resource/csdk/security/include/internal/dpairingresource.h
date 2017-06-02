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

#ifndef IOTVT_SRM_DPAIRING_H
#define IOTVT_SRM_DPAIRING_H

#include "octypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize DPAIRING resource by loading data from persistent storage.
 *
 * @retval  OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult InitDpairingResource();

/**
 * Perform cleanup for DPAIRING resources.
 *
 * @retval  OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult DeInitDpairingResource();

/**
 * This method converts CBOR DPAIRING into binary DPAIRING.
 * The CBOR DPAIRING can be from persistent database or
 * or received as POST request.
 *
 * @param cborPayload pconf data in cbor format.
 * @param size size of the cbor payload
 * @param secDpair binary Dpairing resource
 * @return OC_STACK_OK for Success, otherwise some error value.
 *
 * @note Caller needs to invoke OICFree after done
 *       using the return pointer
 */
OCStackResult CBORPayloadToDpair(const uint8_t *cborPayload, size_t size,
        OicSecDpairing_t **secDpair);

/**
 * This method converts DPAIRING data into CBOR format.
 *
 * @param dpair  Pointer to OicSecDpairing_t.
 * @param payload CBOR format converted from binary dpairing
 * @param size Size of the coverted payload
 * @return OC_STACK_OK for Success, otherwise some error value.
 *
 * @note Caller needs to invoke OICFree after done
 *       using the return pointer
 */
OCStackResult DpairingToCBORPayload(const OicSecDpairing_t *dpair, uint8_t **payload, size_t *size);

/** This function deallocates the memory for OicSecPconf_t .
 *
 * @param[in] dpair  Pointer to OicSecDpairing_t.
 */
void DeleteDpairingBinData(OicSecDpairing_t* dpair);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Function to save PairingPSK.
 *
 * @param[in] endpoint   current endpoint.
 * @param[in] peerDevID   peer device indentitiy.
 * @param[in] isPairingServer   indicate if it generates PairingPSK for server or client.
 *
 * @return  OC_STACK_OK on success
 */
OCStackResult SavePairingPSK(OCDevAddr *endpoint,
            OicUuid_t *peerDevID, OicUuid_t *owner, bool isPairingServer);
#endif // __WITH_DTLS__ or __WITH_TLS__

/**
 * Gets the OicUuid_t value for the rownerid of the Dpairing resource.
 *
 * @param rowneruuid a pointer to be assigned to the rowneruuid property
 * @return ::OC_STACK_OK if rowneruuid is assigned correctly, else ::OC_STACK_ERROR.
 */
OCStackResult GetDpairingRownerId(OicUuid_t *rowneruuid);

/**
 * Internal function to update resource owner
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetDpairingRownerId(const OicUuid_t* newROwner);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_DPAIRING_H




