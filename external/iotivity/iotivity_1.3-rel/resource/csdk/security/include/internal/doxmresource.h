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

#ifndef IOTVT_SRM_DOXM_H
#define IOTVT_SRM_DOXM_H

#include "octypes.h"
#ifdef MULTIPLE_OWNER
#include "cacommon.h"
#endif //MULTIPLE_OWNER

#ifdef __cplusplus
extern "C" {
#endif

// NOTE that this enum must match the gDoxmPropertyAccessModes
// table in doxmresource.c
typedef enum {
    DOXM_OXMS = 1,
    DOXM_OXMSEL,
    DOXM_SCT,
    DOXM_OWNED,
#ifdef MULTIPLE_OWNER
    DOXM_SUBOWNER,
    DOXM_MOM,
#endif // MULTIPLE_OWNER
    DOXM_DEVICEUUID,
    DOXM_DEVOWNERUUID,
    DOXM_ROWNERUUID,
    DOXM_PROPERTY_COUNT
} DoxmProperty_t;

/**
 * Initialize DOXM resource by loading data from persistent storage.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult InitDoxmResource();

/**
 * Perform cleanup for DOXM resources.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult DeInitDoxmResource();

/**
 * This method is used by SRM to retrieve DOXM resource data.
 *
 * @return reference to @ref OicSecDoxm_t, binary format of Doxm resource data.
 */
const OicSecDoxm_t* GetDoxmResourceData();

/**
 * This method converts CBOR DOXM into binary DOXM.
 * The CBOR DOXM can be from persistent database or
 * or received as PUT/POST request.
 *
 * @param cborPayload is a doxm data in cbor.
 * @note Caller needs to invoke OCFree after done using the return pointer.
 * @param doxm is the pointer to @ref OicSecDoxm_t.
 * @param size of the cborPayload. In case value is 0, CBOR_SIZE value is assigned.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult CBORPayloadToDoxm(const uint8_t *cborPayload, size_t size,
                                OicSecDoxm_t **doxm);

/**
 * Converts DOXM into CBOR payload, including only the
 * Properties marked "true" in the propertiesToInclude array.
 *
 * @param doxm pointer to the initialized doxm structure.
 * @param payload pointer to doxm cbor payload.
 * @param size of the cbor payload converted. It is 0 in case of error,
 *     else a positive value if succcessful.
 * @param propertiesToInclude Array of bools, size "DOXM_PROPERTY_COUNT",
 *     where "true" indicates the corresponding property should be
 *     included in the CBOR representation that is created.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
 OCStackResult DoxmToCBORPayloadPartial(const OicSecDoxm_t *doxm,
 	uint8_t **payload, size_t *size, const bool *propertiesToInclude);


/**
 * Converts DOXM into cbor payload, including all Properties for a
 * full representation.
 *
 * @param doxm pointer to the initialized doxm structure.
 * @param payload pointer to doxm cbor payload.
 * @param size of the cbor payload converted. It is 0 in case of error,
 *     else a positive value if succcessful.
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult DoxmToCBORPayload(const OicSecDoxm_t *doxm,
	uint8_t **payload, size_t *size);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * Enables Anon DH ciphersuite if device unowned and Just Works OTM is selected.
 * Otherwise, does nothing.
 *
 * @param[out] enabled TRUE if Anon DH ciphersuite enabled, else FALSE
 *
 * @retval  ::OC_STACK_OK No errors.
 * @retval  ::OC_STACK_ERROR An error occured.
 */
OCStackResult EnableAnonCipherSuiteIfUnOwnedAndJustWorksSelected(bool *enabled);
#endif // __WITH_DTLS__ or __WITH_TLS__

#if defined(__WITH_DTLS__) || defined (__WITH_TLS__)
/**
 * API to save the seed value to generate device UUID.
 *
 * @param seed buffer of seed value.
 * @param seedSize byte length of seed
 *
 * @return ::OC_STACK_OK for Success, otherwise some error value.
 */
OCStackResult SetDoxmDeviceIDSeed(const uint8_t* seed, size_t seedSize);
#endif

/**
 * Get the doxm.deviceuuid value for this device.
 *
 * @param[out] deviceuuid ptr to contain a copy of doxm.deviceuuid value.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult GetDoxmDeviceID(OicUuid_t *deviceuuid);

/**
 * Set the doxm.deviceuuid value for this device.
 *
 * @param[in] deviceuuid ptr to value to be copied into doxm.deviceuuid.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult SetDoxmDeviceID(const OicUuid_t *deviceuuid);

/**
 * Get the doxm.devowneruuid value for this device.
 *
 * @param[out] devowneruuid ptr to contain a copy of doxm.devowneruuid value.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult GetDoxmDevOwnerId(OicUuid_t *devowneruuid);

/**
 * Set the doxm.deviceuuid value for this device.
 *
 * @param[in] deviceuuid ptr to value to be copied into doxm.deviceuuid.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult SetDoxmDevOwnerId(const OicUuid_t *devowneruuid);

/**
 * Get the doxm.isowned value for this device.
 *
 * @param[out] isowned ptr to contain a copy of doxm.isowned value.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult GetDoxmIsOwned(bool *isowned);

/**
 * Set the doxm.isowned value for this device.
 *
 * @param[in] isowned ptr to value to be copied into doxm.isowned.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult SetDoxmIsOwned(const bool isowned);

/**
 * Get the doxm.rowneruuid value for this device.
 *
 * @param[out] rowneruuid ptr to contain a copy of rowneruuid value.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult GetDoxmRownerId(OicUuid_t *rowneruuid);

/**
 * Set the doxm.rowneruuid value for this device.
 *
 * @param[in] rowneruuid ptr to value to be copied into doxm.rowneruuid.
 * @return ::OC_STACK_OK if value is copied successfully, else ::OC_STACK_ERROR.
 */
OCStackResult SetDoxmRownerId(const OicUuid_t *rowneruuid);

#ifdef MULTIPLE_OWNER
/**
 * Compare the UUID to SubOwner.
 *
 * @param[in] uuid device UUID
 *
 * @return true if uuid exists in the SubOwner list of doxm, else false.
 */
bool IsSubOwner(const OicUuid_t* uuid);

/**
 * Function to set a MOT status
 *
 * @param enable whether the MOT is enabled. (true=enable, false=disable)
 */
OCStackResult SetMOTStatus(bool enable);

#endif //MULTIPLE_OWNER

/** This function deallocates the memory for OicSecDoxm_t .
 *
 * @param doxm is the pointer to @ref OicSecDoxm_t.
 */
void DeleteDoxmBinData(OicSecDoxm_t* doxm);

/** This function checks if two sets of /oic/sec/doxm properties are identical.
 *
 * @param doxm1 is a pointer to the first @ref OicSecDoxm_t data.
 * @param doxm2 is a pointer to the second @ref OicSecDoxm_t data.
 *
 * @return true if all of the properties are identical, else false.
 */
bool AreDoxmBinPropertyValuesEqual(OicSecDoxm_t* doxm1, OicSecDoxm_t* doxm2);

#if defined(__WITH_DTLS__) && defined(MULTIPLE_OWNER)
/**
 * Callback function to handle MOT DTLS handshake result.
 * @param[out]   object           remote device information.
 * @param[out]   errorInfo        CA Error information.
 */
CAResult_t MultipleOwnerDTLSHandshakeCB(const CAEndpoint_t *object,
                                        const CAErrorInfo_t *errorInfo);
#endif //__WITH_DTLS__ && MULTIPLE_OWNER

/**
 * Internal function to change doxm resource to Ready for Normal Operation.
 *
 * @param newROwner new owner
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult SetDoxmSelfOwnership(const OicUuid_t* newROwner);

/**
 * Internal function to update the writable properties of an /oic/sec/doxm
 * struct with the values from another /oic/sec/doxm struct.
 *
 * @param src is a pointer to the source @ref OicSecDoxm_t data.
 * @param dst is a pointer to the destination @ref OicSecDoxm_t data.
 *
 * @retval ::OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult DoxmUpdateWriteableProperty(const OicSecDoxm_t* src, OicSecDoxm_t* dst);

#ifdef __cplusplus
}
#endif

/*
 * Check doxm is owned
 */
bool IsDoxmOwned();

/*
 * Check doxm owner id
 */
bool IsDoxmDevowneruuidTheNilUuid();

/*
 * Check doxm device id
 */
bool IsDoxmDeviceuuidTheNilUuid();

/*
 * Check doxm rowner uuid
 */
bool IsDoxmRowneruuidTheNilUuid();

#endif //IOTVT_SRM_DOXMR_H
