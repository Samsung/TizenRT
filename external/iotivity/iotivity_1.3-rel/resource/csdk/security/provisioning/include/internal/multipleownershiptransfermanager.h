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

#ifndef OTM_MULTIPLE_OWNERSHIPTRANSFERMANAGER_H_
#define OTM_MULTIPLE_OWNERSHIPTRANSFERMANAGER_H_

#include "pmtypes.h"
#include "ocstack.h"
#include "octypes.h"
#include "experimental/securevirtualresourcetypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**********************************************************************
 * API for Super Owner
 **********************************************************************/

/**
 * API to add 'doxm.oxms' to resource server.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] newOxm  OxMs to be added (ref. oic.sec.doxmtype)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxms' request receives a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTAddMOTMethod(void *ctx, OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t newOxm, OCProvisionResultCB resultCallback);

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.doxmtype)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
        const OicSecOxm_t oxmSelValue, OCProvisionResultCB resultCallback);

/**
 * API to update 'doxm.mom' to resource server.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] momType Mode of multiple ownership transfer (ref. oic.sec.mom)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'mom' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTChangeMode(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
        const OicSecMomType_t momType, OCProvisionResultCB resultCallback);

/**
 * API to provision preconfigured PIN to resource server.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfPIN Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfPINLen Byte length of preconfig PIN
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST credential request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTProvisionPreconfigPIN(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const char* preconfPIN, size_t preconfPINLen, OCProvisionResultCB resultCallback);

/**********************************************************************
 * API for Sub Owner
 **********************************************************************/

/**
 * API to perform the multiple ownership transfer.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] selectedDevicelist linked list of multiple ownership transfer candidate devices.
 * @param[in] resultCallback Result callback function to be invoked when multiple ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTDoOwnershipTransfer(void* ctx,
                                     OCProvisionDev_t *selectedDevicelist,
                                     OCProvisionResultCB resultCallback);


/**
 * API to add preconfigured PIN to local SVR DB.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfPIN Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfPINLen Byte length of preconfig PIN
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult MOTAddPreconfigPIN(const OCProvisionDev_t *targetDeviceInfo,
                                 const char* preconfPIN, size_t preconfPINLen);

/**
 * The function checks is the device type MOT support.
 *
 * @param[in]  ptrDoxm     Doxm
 *
 * @return 0 in case of success and 1 otherwise.
 */
int MOTIsSupportedOnboardingType(OicSecDoxm_t *ptrDoxm);

#ifdef __cplusplus
}
#endif
#endif //OTM_MULTIPLE_OWNERSHIPTRANSFERMANAGER_H_
