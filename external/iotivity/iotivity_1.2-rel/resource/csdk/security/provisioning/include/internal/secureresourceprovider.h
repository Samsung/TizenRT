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

#ifndef SRP_SECURERESOURCEPROVIDER_H
#define SRP_SECURERESOURCEPROVIDER_H

#include "ocstack.h"
#include "securevirtualresourcetypes.h"
#include "pmtypes.h"
#include "octypes.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * API to send ACL information to resource.
 *
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionACL(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                        OicSecAcl_t *acl, OCProvisionResultCB resultCallback);

/**
 * API to save ACL which has several ACE into Acl of SVR.
 *
 * @param acl ACL to be saved in Acl of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPSaveACL(const OicSecAcl_t *acl);

/**
 * API to request CRED information to resource.
 *
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPGetCredResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OCProvisionResultCB resultCallback);

/**
 * API to request ACL information to resource.
 *
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPGetACLResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OCProvisionResultCB resultCallback);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)

/**
 * function to provision Trust certificate chain to devices.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] credId CredId of trust certificate chain to be provisioned to the device.
 * @param[in] selectedDeviceInfo Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionTrustCertChain(void *ctx, OicSecCredType_t type, uint16_t credId,
                                      const OCProvisionDev_t *selectedDeviceInfo,
                                      OCProvisionResultCB resultCallback);

/**
 * function to save Trust certificate chain into Cred of SVR.
 *
 * @param[in] trustCertChain Trust certificate chain to be saved in Cred of SVR.
 * @param[in] chainSize Size of trust certificate chain to be saved in Cred of SVR
 * @param[in] encodingType Encoding type of trust certificate chain to be saved in Cred of SVR
 * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize,
                                        OicEncodingType_t encodingType,uint16_t *credId);

/**
 * function to save own certificate chain into Cred of SVR.
 *
 * @param[in] cert own certificate chain to be saved in Cred of SVR.
 * @param[in] key own secret key to be saved in Cred of SVR.
 * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPSaveOwnCertChain(OicSecKey_t * cert, OicSecKey_t * key, uint16_t *credId);

/**
 * function to register callback, for getting notification for TrustCertChain change.
 *
 * @param[in] ctx user context to be passed.
 * @param[in] TrustCertChainChangeCB notifier callback function
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPRegisterTrustCertChainNotifier(void *ctx, TrustCertChainChangeCB callback);

/**
 * function to de-register TrustCertChain notification callback.
 */
void SRPRemoveTrustCertChainNotifier(void);

#endif // __WITH_DTLS__ || __WITH_TLS__
/**
 * API to send Direct-Pairing Configuration to a device.
 *
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] pconf PCONF pointer.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionDirectPairing(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                        OicSecPconf_t *pconf, OCProvisionResultCB resultCallback);

/**
 * API to send Direct-Pairing Configuration to a device.
 *
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] pconf PCONF pointer.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionDirectPairing(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                        OicSecPconf_t *pconf, OCProvisionResultCB resultCallback);

/**
 * API to provision credential to devices.
 *
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] pDev1 Pointer to PMOwnedDeviceInfo_t instance,respresenting resource to be provsioned.
   @param[in] pDev2 Pointer to PMOwnedDeviceInfo_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionCredentials(void *ctx,OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      OCProvisionResultCB resultCallback);

/**
 * Function to unlink devices.
 * This function will remove the credential & relationship between the two devices.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] pTargetDev1 first device information to be unlinked.
 * @param[in] pTargetDev2 second device information to be unlinked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            device unlink is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPUnlinkDevices(void* ctx,
                              const OCProvisionDev_t* pTargetDev1,
                              const OCProvisionDev_t* pTargetDev2,
                              OCProvisionResultCB resultCallback);

/*
 * Function to device revocation.
 * This function will remove credential of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         If OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
 */
OCStackResult SRPRemoveDevice(void* ctx,
                              unsigned short waitTimeForOwnedDeviceDiscovery,
                              const OCProvisionDev_t* pTargetDev,
                              OCProvisionResultCB resultCallback);

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] pOwnedDevList List of owned devices
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
* @return  OC_STACK_OK in case of success and other value otherwise.
*          If OC_STACK_OK is returned, the caller of this API should wait for callback.
*          OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
*/
OCStackResult SRPRemoveDeviceWithoutDiscovery(void* ctx, const OCProvisionDev_t* pOwnedDevList,
                             const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback);

/*
 * Function to sync-up credential and ACL of the target device.
 * This function will remove credential and ACL of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         If OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
 */
OCStackResult SRPSyncDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                         const OCProvisionDev_t* pTargetDev, OCProvisionResultCB resultCallback);

/*
 * Function for remote reset
 * This function will send pstat POST(modify) message to the target device
 * to change current mode to reset state in order to initiate remote reset.
 *
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 *            when there is an error, this user callback is called immediately.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         If OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no request is need to be initiated.
 */
OCStackResult SRPResetDevice(const OCProvisionDev_t* pTargetDev,
        OCProvisionResultCB resultCallback);

/*
 * Function to read Trust certificate chain from SVR.
 * Caller must free when done using the returned trust certificate
 * @param[in] credId CredId of trust certificate chain in SVR.
 * @param[out] trustCertChain Trust certificate chain.
 * @param[out] chainSize Size of trust certificate chain
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize);
#ifdef __cplusplus
}
#endif
#endif //SRP_SECURERESOURCEPROVIDER_H
