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
#include "experimental/securevirtualresourcetypes.h"
#include "pmtypes.h"
#include "octypes.h"

// Enum type index for data types.
typedef enum
{
    CHAIN_TYPE = 0,                       /**< Certificate trust chain.**/
    ACL_TYPE,                             /**< Access control list.**/
    PSK_TYPE,                             /**< Pre-Shared Key.**/
    CERT_TYPE,                            /**< X.509 certificate.**/
    MOT_TYPE                              /**< Multiple Ownership Transfer.**/
} DataType_t;

/**
 * Structure to carry general data to callback.
 */
typedef struct Data
{
    void *ctx;                                   /**< Pointer to user context.**/
    DataType_t type;                             /**< Data type of the context.**/
} Data_t;


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * API to send ACL information to resource.
 *
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] aclVersion Version of the ACL resource to access
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionACL(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                        OicSecAcl_t *acl, OicSecAclVersion_t aclVersion, OCProvisionResultCB resultCallback);

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
 * @param[in] ctx Application context to be returned in result callback.
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
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] aclVersion Version of ACL resource to query
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPGetACLResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
        OicSecAclVersion_t aclVersion, OCProvisionResultCB resultCallback);

/**
 * API to request the Certificate Signing Request (CSR) resource.
 *
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPGetCSRResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                OCGetCSRResultCB resultCallback);

/**
 * API to request the Roles resource.
 *
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback Callback provided by API user. Callback will be called when
 *            provisioning request receives a response from resource server.
 * @return OC_STACK_OK in case of success or error value otherwise.
 */
OCStackResult SRPGetRolesResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                  OCGetRolesResultCB resultCallback);

/**
 * This function requests the device delete a particular role certificate by credId.
 *
 * @param[in] ctx Application context that is returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by the API user. Callback will be called when request receives
 *            a response from the resource server.
 * @param[in] credId credId to request be deleted.
 *
 * @return OC_STACK_OK in case of success, and error value otherwise.
 */
OCStackResult SRPDeleteRoleCertificateByCredId(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                               OCProvisionResultCB resultCallback, uint32_t credId);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)

/**
 * function to provision Trust certificate chain to devices.
 *
 * @param[in] ctx Application context to be returned in result callback.
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
OCStackResult SRPSaveTrustCertChain(const uint8_t *trustCertChain, size_t chainSize,
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
 * function to save own role certificate into Cred of SVR.
 *
 * @param[in] cert Certificate chain to be saved in Cred of SVR
 * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 *
 * @note The certificate public key must be the same as public key in the identity
 *       certificate (installed by SRPSaveOwnCertChain).
 */
OCStackResult SRPSaveOwnRoleCert(OicSecKey_t * cert, uint16_t *credId);

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
 * API to provision credential to devices.
 *
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] keySize size of key
 * @param[in] pDev1 Pointer to PMOwnedDeviceInfo_t instance, representing the resource to be provisioned.
 * @param[in] pDev2 Pointer to PMOwnedDeviceInfo_t instance, representing the resource to be provisioned.
 *                  Use NULL to indicate the local device.
 * @param[in] pemCert When provisioning a certificate (type is SIGNED_ASYMMETRIC_KEY), this is the
 *                    certificate, encoded as PEM.
 * @param[in] role1 When provisioning a PSK (type is SYMMETRIC_PAIR_WISE_KEY), this is the role which
 *                  the device indicated by pDev1 will also have when communicating with pDev2. Use NULL
 *                  to associate no role with this credential.
 * @param[in] role2 When provisioning a PSK (type is SYMMETRIC_PAIR_WISE_KEY), this is the role which
 *                  the device indicated by pDev1 will also have when communicating with pDev2. Use NULL
 *                  to associate no role with this credential.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionCredentials(void *ctx,OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      const char* pemCert,
                                      const OicSecRole_t *role1,
                                      const OicSecRole_t *role2,
                                      OCProvisionResultCB resultCallback);
 /**
 * API to provision credential to devices with DOS.
 *
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] keySize size of key
 * @param[in] pDev1 Pointer to PMOwnedDeviceInfo_t instance, representing the resource to be provisioned.
 * @param[in] pDev2 Pointer to PMOwnedDeviceInfo_t instance, representing the resource to be provisioned.
 * @param[in] role1 When provisioning a PSK (type is SYMMETRIC_PAIR_WISE_KEY), this is the role which
 *                  the device indicated by pDev1 will also have when communicating with pDev2. Use NULL
 *                  to associate no role with this credential.
 * @param[in] role2 When provisioning a PSK (type is SYMMETRIC_PAIR_WISE_KEY), this is the role which
 *                  the device indicated by pDev1 will also have when communicating with pDev2. Use NULL
 *                  to associate no role with this credential.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionCredentialsDos(void *ctx,OicSecCredType_t type, size_t keySize,
                                         const OCProvisionDev_t *pDev1,
                                         const OCProvisionDev_t *pDev2,
                                         const OicSecRole_t *role1,
                                         const OicSecRole_t *role2,
                                         OCProvisionResultCB resultCallback);

/**
 * Function to unlink devices.
 * This function will remove the credential & relationship between the two devices.
 *
 * @param[in] ctx Application context to be returned in result callback
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

/**
 * Function to device revocation.
 * This function will remove credential of target device from all devices in subnet.
 *
 * @param[in] ctx Application context to be returned in result callback
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

/**
 * Function to device revocation
 * This function will remove credential of target device from all devices in subnet.
 *
 * @param[in] ctx Application context to be returned in result callback
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

/**
 * Function to sync-up credential and ACL of the target device.
 * This function will remove credential and ACL of target device from all devices in subnet.
 *
 * @param[in] ctx Application context to be returned in result callback
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

/**
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

/**
 * Function to read Trust certificate chain from SVR.
 * Caller must free when done using the returned trust certificate
 * @param[in] credId CredId of trust certificate chain in SVR.
 * @param[out] trustCertChain Trust certificate chain.
 * @param[out] chainSize Size of trust certificate chain
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize);

/**
 * Function for certificate provisioning.
 * @param[in] ctx Application context to be returned in result callback.
 * @param[in] pDev Selected target device.
 * @param[in] pemCert the certificate in PEM.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request receives a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SRPProvisionCertificate(void *ctx, const OCProvisionDev_t *pDev,
        const char* pemCert, OCProvisionResultCB resultCallback);

/**
 * Updates pstat resource of server.
 *
 * @param[in] data Structure for the application context
 * @param[in] dos DOS mode
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult SetDOS(const Data_t *data, OicSecDeviceOnboardingState_t dos,
                            OCClientResponseHandler resultCallback);

void FreeData(Data_t *data);

#ifdef __cplusplus
}
#endif
#endif //SRP_SECURERESOURCEPROVIDER_H
