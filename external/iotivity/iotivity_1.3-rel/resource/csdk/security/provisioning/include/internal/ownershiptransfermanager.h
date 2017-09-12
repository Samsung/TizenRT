/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

#ifndef OTM_OWNERSHIPTRANSFERMANAGER_H_
#define OTM_OWNERSHIPTRANSFERMANAGER_H_

#include "pmtypes.h"
#include "ocstack.h"
#include "octypes.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define OXM_STRING_MAX_LENGTH 32
#define WRONG_PIN_MAX_ATTEMP 5

typedef struct OTMCallbackData OTMCallbackData_t;
typedef struct OTMContext OTMContext_t;

/**
 * Do ownership transfer for the unowned devices.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] selectedDeviceList linked list of ownership transfer candidate devices.
 * @param[in] resultCB Result callback function to be invoked when ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OTMDoOwnershipTransfer(void* ctx,
                                     OCProvisionDev_t* selectedDeviceList, OCProvisionResultCB resultCB);

/**
 * API to set a allow status of OxM
 *
 * @param[in] oxm Owership transfer method (ref. OicSecOxm_t)
 * @param[in] allowStatus allow status (true = allow, false = not allow)
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OTMSetOxmAllowStatus(const OicSecOxm_t oxm, const bool allowStatus);


/**
 *Callback for load secret for temporal secure session
 *
 * e.g) in case of PIN based, input the pin through this callback
 *       in case of X.509 based, input the certificate through this callback
 */
typedef OCStackResult (*OTMLoadSecret)(OTMContext_t* otmCtx);

/**
 * Callback for create secure channel using secret inputed from OTMLoadSecret callback
 */
typedef OCStackResult (*OTMCreateSecureSession)(OTMContext_t* otmCtx);

/**
 * Callback for creating CoAP payload.
 */
typedef OCStackResult (*OTMCreatePayloadCallback)(OTMContext_t* otmCtx, uint8_t **payload,
                                                  size_t *size);

/**
 * Required callback for performing ownership transfer
 */
struct OTMCallbackData
{
    OTMLoadSecret loadSecretCB;
    OTMCreateSecureSession createSecureSessionCB;
    OTMCreatePayloadCallback createSelectOxmPayloadCB;
    OTMCreatePayloadCallback createOwnerTransferPayloadCB;
};

/**
 * Context for ownership transfer(OT)
 */
struct OTMContext{
    void* userCtx;                            /**< Context for user.*/
    OCProvisionDev_t* selectedDeviceInfo;     /**< Selected device info for OT. */
    OicUuid_t subIdForPinOxm;                 /**< Subject Id which uses PIN based OTM. */
    OCProvisionResultCB ctxResultCallback;    /**< Function pointer to store result callback. */
    OCProvisionResult_t* ctxResultArray;      /**< Result array having result of all device. */
    size_t ctxResultArraySize;                /**< No of elements in result array. */
    bool ctxHasError;                         /**< Does OT process have any error. */
    OCDoHandle ocDoHandle;                    /**< A handle for latest request message. */
    OTMCallbackData_t otmCallback;            /**< OTM callbacks to perform the OT/MOT. */
#ifdef MULTIPLE_OWNER
    OicSecDoxm_t* doxm;                       /**< Device Owner Transfer Method. */
    OicSecCred_t* cred;                       /**< Credential data. */
#endif // MULTIPLE_OWNER
    int attemptCnt;
};

// TODO: Remove this OTMSetOwnershipTransferCallbackData, Please see the jira ticket IOT-1484
/**
 * Set the callbacks for ownership transfer
 *
 * @param[in] oxm Ownership transfer method
 * @param[in] callbackData the implementation of the ownership transfer function for each step.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OTMSetOwnershipTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData);

/**
 * API to assign the OTMCallback for each OxM.
 *
 * @param[out] callbacks Instance of OTMCallback_t
 * @param[in] oxm Ownership transfer method
 * @return  OC_STACK_OK on success
 */
OCStackResult OTMSetOTCallback(OicSecOxm_t oxm, OTMCallbackData_t* callbacks);

/**
 * Function to select appropriate security provisioning method.
 *
 * @param[in] supportedMethods   Array of supported methods
 * @param[in] numberOfMethods   number of supported methods
 * @param[out]  selectedMethod         Selected methods
 * @param[in] ownerType type of owner device (SUPER_OWNER or SUB_OWNER)
 * @return  OC_STACK_OK on success
 */
OCStackResult OTMSelectOwnershipTransferMethod(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod, OwnerType_t ownerType);

/**
 * This function configures SVR DB as self-ownership.
 *
 *@return OC_STACK_OK in case of successful configue and other value otherwise.
 */
OCStackResult ConfigSelfOwnership(void);

#ifdef __cplusplus
}
#endif
#endif //OTM_OWNERSHIPTRANSFERMANAGER_H_
