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

#ifndef OCPROVISIONINGMANAGER_H_
#define OCPROVISIONINGMANAGER_H_

#include "octypes.h"
#include "pmtypes.h"
#include "casecurityinterface.h"
#include "ownershiptransfermanager.h"
#ifdef MULTIPLE_OWNER
#include "experimental/securevirtualresourcetypes.h"
#endif //MULTIPLE_OWNER

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * The function is responsible for initializaton of the provisioning manager. It will load
 * provisioning database which have owned device's list and their linked status.
 * TODO: In addition, if there is a device(s) which has not up-to-date credentials, this function will
 * automatically try to update the deivce(s).
 *
 * @see OCClosePM()
 *
 * @param[in] dbPath file path of the sqlite3 db
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCInitPM(const char* dbPath);

/**
 * This method is used by provisioning manager to close provisioning database.
 *
 * @see OCInitPM()
 *
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCClosePM();

/**
 * API to cleanup PDM in case of timeout.
 * It will remove the PDM_DEVICE_INIT state devices from PDM.
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCPDMCleanupForTimeout();

/**
 * The function is responsible for discovery of owned/unowned device is specified endpoint/deviceID.
 * It will return the found device even though timeout is not exceeded.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the device.
 * @param[in] deviceID         deviceID of target device.
 * @param[out] ppFoundDevice     OCProvisionDev_t of found device
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverSingleDevice(unsigned short timeout, const OicUuid_t* deviceID,
                             OCProvisionDev_t **ppFoundDevice);

/**
 * The function is responsible for discovery of owned/unowned device is specified endpoint/MAC
 * address.
 * It will return the found device even though timeout is not exceeded.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the device.
 * @param[in] deviceID         deviceID of target device.
 * @param[in] hostAddress       MAC address of target device.
 * @param[in] connType       ConnectivityType for discovery.
 * @param[out] ppFoundDevice     OCProvisionDev_t of found device.
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverSingleDeviceInUnicast(unsigned short timeout, const OicUuid_t* deviceID,
                             const char* hostAddress, OCConnectivityType connType,
                             OCProvisionDev_t **ppFoundDevice);

/**
 * The function is responsible for discovery of device is current subnet. It will list
 * all the device in subnet which are not yet owned. Please call OCInit with OC_CLIENT_SERVER as
 * OCMode.
 *
 * @param[in] waittime Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of candidate devices to be provisioned
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverUnownedDevices(unsigned short waittime, OCProvisionDev_t **ppList);

/**
 * Do ownership transfer for un-owned device.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDevices List of devices to perform ownership transfer.
 * @param[in] resultCallback Result callback function to be invoked when ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDoOwnershipTransfer(void* ctx,
                                    OCProvisionDev_t *targetDevices,
                                    OCProvisionResultCB resultCallback);

/**
 * API to set a allow status of OxM
 *
 * @param[in] oxm Owership transfer method (ref. OicSecOxm_t)
 * @param[in] allowStatus allow status (true = allow, false = not allow)
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCSetOxmAllowStatus(const OicSecOxm_t oxm, const bool allowStatus);

#ifdef MULTIPLE_OWNER
/**
 * API to perfrom multiple ownership transfer for MOT enabled device.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDevices List of devices to perform ownership transfer.
 * @param[in] resultCallback Result callback function to be invoked when ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDoMultipleOwnershipTransfer(void* ctx,
                                      OCProvisionDev_t *targetDevices,
                                      OCProvisionResultCB resultCallback);
#endif //MULTIPLE_OWNER

/**
 * API to register for particular OxM.
 *
 * @param[in] oxm transfer method.
 * @param[in] callbackData of callback functions for owership transfer.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData);

/**
 * The function is responsible for discovery of owned device is current subnet. It will list
 * all the device in subnet which are owned by calling provisioning client.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of device owned by provisioning tool.
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList);

#ifdef MULTIPLE_OWNER
/**
 * The function is responsible for the discovery of an MOT-enabled device with the specified deviceID.
 * The function will return when security information for device with deviceID has been obtained or the
 * timeout has been exceeded.
 *
 * @param[in]  timeoutSeconds  Maximum time, in seconds, this function will listen for responses from
 *                             servers before returning.
 * @param[in]  deviceID        deviceID of target device.
 * @param[out] ppFoundDevice   OCProvisionDev_t of discovered device. Caller should use
 *                             OCDeleteDiscoveredDevices to delete the device.
 * @return OC_STACK_OK in case of success and other values otherwise.
 */
OCStackResult OC_CALL OCDiscoverMultipleOwnerEnabledSingleDevice(unsigned short timeoutSeconds,
                                                         const OicUuid_t *deviceID,
                                                         OCProvisionDev_t **ppFoundDevice);

/**
 * The function is responsible for discovery of MOT enabled device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of MOT enabled devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverMultipleOwnerEnabledDevices(unsigned short timeout, OCProvisionDev_t **ppList);

/**
 * The function is responsible for discovery of Multiple Owned device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of Multiple Owned devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCDiscoverMultipleOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList);

/**
 * The function is responsible for determining if the caller is a subowner of the specified device.
 *
 * @param[in]  device      MOT enabled device that contains a list of subowners.
 * @param[out] isSubowner  Bool indicating whether the caller is a subowner of device.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCIsSubownerOfDevice(OCProvisionDev_t *device, bool *isSubowner);
#endif //MULTIPLE_OWNER

/**
 * API to provision credentials between two devices and ACLs for the devices who act as a server.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] keySize size of key
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance,respresenting device to be provisioned.
 * @param[in] pDev1Acl ACL for device 1. If this is not required set NULL.
 * @param[in] pDev2 Pointer to OCProvisionDev_t instance,respresenting device to be provisioned.
 * @param[in] pDev2Acl ACL for device 2. If this is not required set NULL.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize,
                                         const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl,
                                         const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl,
                                         OCProvisionResultCB resultCallback);

/**
 * API to send version 1 ACL information to device.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionACL(void *ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecAcl_t *acl,
                             OCProvisionResultCB resultCallback);

/**
 * API to send version 2 ACL information to device.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionACL2(void *ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecAcl_t *acl,
                              OCProvisionResultCB resultCallback);

/**
 * function to save ACL which has several ACE into Acl of SVR.
 *
 * @param acl ACL to be saved in Acl of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCSaveACL(const OicSecAcl_t* acl);

/**
 * this function requests CRED information to resource.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetCredResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback);

/**
 * this function requests version 1 ACL information to resource.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetACLResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback);

/**
 * this function requests version 2 ACL information to resource.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetACL2Resource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                               OCProvisionResultCB resultCallback);

/**
 * This function requests the device provide its Certificate Signing Request (CSR).
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetCSRResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                               OCGetCSRResultCB resultCallback);

/**
 * This function requests the device provide its roles resource, listing the role certificates
 * it has for the local requestor.
 *
 * @param[in] ctx Application context that is returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by the API user. Callback will be called when provisioning
 *                           request receives a response from the resource server.
 * @return OC_STACK_OK in case of success, and error value otherwise.
 */
OCStackResult OC_CALL OCGetRolesResource(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                 OCGetRolesResultCB resultCallback);

/**
 * This function requests the device delete a particular role certificate by credId.
 *
 * @param[in] ctx Application context that is returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by the API user. Callback will be called when request receives
 *            a response from the resource server.
 * @param[in] credId credId to request be deleted. If 0, delete all role certificates for this peer.
 *
 * @return OC_STACK_OK in case of success, and error value otherwise.
 */
OCStackResult OC_CALL OCDeleteRoleCertificateByCredId(void *ctx, const OCProvisionDev_t *selectedDeviceInfo,
                                              OCProvisionResultCB resultCallback, uint32_t credId);

/**
 * API to provision credential to devices.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] keySize size of key
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance, representing the resource to be provisioned.
 * @param[in] pDev2 Pointer to OCProvisionDev_t instance, representing the resource to be provisioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionCredentials(void *ctx, OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      OCProvisionResultCB resultCallback);

/**
 * API to provision symmetric pair-wise key credentials to devices that grant a role.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] keySize size of key
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance, representing the resource to be provisioned.
 * @param[in] pDev2 Pointer to OCProvisionDev_t instance, representing the resource to be provisioned. 
 *                  Use NULL to indicate the local device.
 * @param[in] role1 The role which the device indicated by pDev1 will have when communicating with pDev2.
 *                  Use NULL to associate no role with this credential.
 * @param[in] role2 The role which the device indicated by pDev2 will have when communicating with pDev1.
 *                  Use NULL to associate no role with this credential.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request receives a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionSymmetricRoleCredentials(void *ctx, OicSecCredType_t type, size_t keySize,
                                                  const OCProvisionDev_t *pDev1,
                                                  const OCProvisionDev_t *pDev2,
                                                  const OicSecRole_t *role1,
                                                  const OicSecRole_t *role2,
                                                  OCProvisionResultCB resultCallback);

/**
 * API to provision a certificate to a device.
 *
 * @param[in] ctx Application context returned in result callback.
 * @param[in] pDev Pointer to OCProvisionDev_t instance, respresenting the device to be provsioned.
 * @param[in] pemCert Certificate to provision, encoded as PEM
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request receives a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionCertificate(void *ctx,
                                     const OCProvisionDev_t *pDev,
                                     const char* pemCert,
                                     OCProvisionResultCB resultCallback);

#ifdef MULTIPLE_OWNER
/**
 * API to provision preconfigured PIN to device(NOT LIST).
 * If device does not support the Preconfigured PIN OxM,
 * OCProvisionPreconfigPin API will update the device's Doxm
 * and then try preconfigured PIN provisioning once again.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfigPin string of preconfigured PIN.
 * @param[in] preconfigPinLen string length of 'preconfigPin'.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionPreconfigPin(void *ctx,
                                      OCProvisionDev_t *targetDeviceInfo,
                                      const char *preconfigPin,
                                      size_t preconfigPinLen,
                                      OCProvisionResultCB resultCallback);

/**
 * API to add preconfigured PIN to local SVR DB.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfigPin Preconfig PIN which is used while multiple owner authentication
 * @param[in] preconfigPinLen Byte length of preconfigPin
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCAddPreconfigPin(const OCProvisionDev_t *targetDeviceInfo,
                                const char *preconfigPin,
                                size_t preconfigPinLen);

/**
 * API to update 'doxm.mom' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] momType Mode of multiple ownership transfer (ref. oic.sec.mom)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'mom' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCChangeMOTMode(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                            const OicSecMomType_t momType, OCProvisionResultCB resultCallback);

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.doxmtype)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                                 const OicSecOxm_t oxmSelValue, OCProvisionResultCB resultCallback);
#endif //MULTIPLE_OWNER

/**
 * Function to unlink devices.
 * This function will remove the credential & relasionship between the two devices.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] pTargetDev1 fitst device information to be unlinked.
 * @param[in] pTargetDev2 second device information to be unlinked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            device unlink is finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCUnlinkDevices(void* ctx,
                              const OCProvisionDev_t* pTargetDev1,
                              const OCProvisionDev_t* pTargetDev2,
                              OCProvisionResultCB resultCallback);

/**
 * Function for device revocation
 * This function will remove credential of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 *         if OC_STACK_OK is returned, the caller of this API should wait for callback.
 *         OC_STACK_CONTINUE means operation is success but no need to wait for callback.
 */
OCStackResult OC_CALL OCRemoveDevice(void* ctx,
                             unsigned short waitTimeForOwnedDeviceDiscovery,
                             const OCProvisionDev_t* pTargetDev,
                             OCProvisionResultCB resultCallback);

/**
 * Function to device revocation
 * This function will remove credential of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetUuid Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCRemoveDeviceWithUuid(void* ctx,
                                     unsigned short waitTimeForOwnedDeviceDiscovery,
                                     const OicUuid_t* pTargetUuid,
                                     OCProvisionResultCB resultCallback);

/**
 * Function to reset the target device.
 * This function will remove credential and ACL of target device from all devices in subnet.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
 * @param[in] pTargetDev Device information to be revoked.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCResetDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                            const OCProvisionDev_t* pTargetDev,
                            OCProvisionResultCB resultCallback);

/**
 * This function resets SVR DB to its factory setting.
 *
 *@return OC_STACK_OK in case of successful reset and other value otherwise.
 */
OCStackResult OC_CALL OCResetSVRDB(void);

/**
 * This function configures SVR DB as self-ownership.
 *
 *@return OC_STACK_OK in case of successful configue and other value otherwise.
 */
OCStackResult OC_CALL OCConfigSelfOwnership(void);

/**
 * API to get status of all the devices in current subnet. The status include endpoint information
 * and doxm information which can be extracted duing owned and unowned discovery. Along with this
 * information. The API will provide information about devices' status
 * Device can have following states
 *  - ON/OFF: Device is switched on or off.
 *
 * NOTE: Caller need to call OCDeleteDiscoveredDevices to delete memory allocated by this API for out
 * variables pOwnedDevList and pUnownedDevList.
 *
 * @param[in] waittime Wait time for the API. The wait time will be divided by 2, and half of wait time
 * will be used for unowned discovery and remaining half for owned discovery. So the wait time should be
 * equal to or more than 2.
 * @param[out] pOwnedDevList  list of owned devices.
 * @param[out] pUnownedDevList  list of unowned devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetDevInfoFromNetwork(unsigned short waittime,
                                       OCProvisionDev_t** pOwnedDevList,
                                       OCProvisionDev_t** pUnownedDevList);
/**
 * This method is used to get linked devices' IDs.
 *
 * @param[in] uuidOfDevice a target device's uuid.
 * @param[out] uuidList information about the list of linked devices' uuids.
 * @param[out] numOfDevices total number of linked devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCGetLinkedStatus(const OicUuid_t* uuidOfDevice,
                                  OCUuidList_t** uuidList,
                                  size_t* numOfDevices);

/**
 * Remove locally stored credentials with the specified subject UUID.
 *
 * @param[in] subjectUuid The subject UUID of the credentials to remove
 *
 * @return OC_STACK_RESOURCE_DELETED if credentials were removed, or
 * OC_STACK_ERROR if no credentials were removed.
 */
OCStackResult OC_CALL OCRemoveCredential(const OicUuid_t* subjectUuid);

/**
 * API to delete memory allocated to linked list created by OCDiscover_XXX_Devices API.
 *
 * @param[in] pList Pointer to OCProvisionDev_t which should be deleted.
 */
void OC_CALL OCDeleteDiscoveredDevices(OCProvisionDev_t *pList);

/**
 * API to delete memory allocated to OicUuid_t list.
 *
 * @param[in] pList Pointer to OicUuid_t list which should be deleted.
 */
void OC_CALL OCDeleteUuidList(OCUuidList_t* pList);

/**
 * This function deletes ACL data.
 *
 * @param pAcl Pointer to OicSecAcl_t structure.
 */
void OC_CALL OCDeleteACLList(OicSecAcl_t* pAcl);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * this function sends CRL information to resource.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] crl CRL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionCRL(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecCrl_t *crl,
                             OCProvisionResultCB resultCallback);

/**
 * function to provision Trust certificate chain to devices.
 *
 * @param[in] ctx Application context returned in the result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] credId CredId of trust certificate chain to be provisioned to the device.
 * @param[in] selectedDeviceInfo Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCProvisionTrustCertChain(void *ctx, OicSecCredType_t type, uint16_t credId,
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
OCStackResult OC_CALL OCSaveTrustCertChain(const uint8_t *trustCertChain, size_t chainSize,
                                   OicEncodingType_t encodingType, uint16_t *credId);

/**
 * Function to save an identity certificate chain into Cred of SVR.
 *
 * @param[in] cert Certificate chain to be saved in Cred of SVR, PEM encoded, null terminated
 * @param[in] key private key corresponding to the certificate, PEM encoded, null terminated
 * @param[out] credId CredId of saved certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCSaveOwnCertChain(const char* cert, const char* key, uint16_t *credId);

/**
 * Function to save own role certificate into Cred of SVR.
 *
 * @param[in] cert Certificate chain to be saved in Cred of SVR, PEM encoded, null terminated
 * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 *
 * @note The certificate public key must be the same as public key in the identity
 *       certificate (installed by OCSaveOwnCertChain).
 */
OCStackResult OC_CALL OCSaveOwnRoleCert(const char* cert, uint16_t *credId);

/**
 * function to register callback, for getting notification for TrustCertChain change.
 *
 * @param[in] TrustCertChainChangeCB notifier callback function
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCRegisterTrustCertChainNotifier(void *cb, TrustCertChainChangeCB CB);

/**
 * function to de-register TrustCertChain notification callback.
 */
void OC_CALL OCRemoveTrustCertChainNotifier(void);

/**
 * Function to read Trust certificate chain from SVR.
 * Caller must free when done using the returned trust certificate
 * @param[in] credId CredId of trust certificate chain in SVR.
 * @param[out] trustCertChain Trust certificate chain.
 * @param[out] chainSize Size of trust certificate chain
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OC_CALL OCReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize);

/**
 * Function to select appropriate security provisioning method.
 *
 * @param[in] supportedMethods   Array of supported methods
 * @param[in] numberOfMethods   number of supported methods
 * @param[out]  selectedMethod         Selected methods
 * @param[in] ownerType type of owner device (SUPER_OWNER or SUB_OWNER)
 * @return  OC_STACK_OK on success
 */
OCStackResult OC_CALL OCSelectOwnershipTransferMethod(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod, OwnerType_t ownerType);

/**
 * Function to set callback to check peer's Common Name field.
 *
 * @param[in] cb callback to utilize peer's certificate Common Name information.
 *            If set, callback is invoked during handshake after certificate
 *            verification. It takes peer's certificate CN field and provides
 *            it to application level. Application is to decide whether CN is
 *            valid or not. In case CA_STATUS_FAIL is returned, handshake will
 *            be dropped.
 */
void OC_CALL OCSetPeerCNVerifyCallback(PeerCNVerifyCallback cb);

#endif // __WITH_DTLS__ || __WITH_TLS__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* OCPROVISIONINGMANAGER_H_ */
