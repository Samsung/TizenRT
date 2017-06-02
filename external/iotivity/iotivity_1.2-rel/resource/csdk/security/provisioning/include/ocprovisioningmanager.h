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
#include "ownershiptransfermanager.h"
#ifdef MULTIPLE_OWNER
#include "securevirtualresourcetypes.h"
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
 * @param[in] dbPath file path of the sqlite3 db
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCInitPM(const char* dbPath);

/**
 * API to termiante the provisioning manager
 */
void OCTerminatePM();

/**
 * API to cleanup PDM in case of timeout.
 * It will remove the PDM_DEVICE_INIT state devices from PDM.
 *
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCPDMCleanupForTimeout();

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
OCStackResult OCDiscoverSingleDevice(unsigned short timeout, const OicUuid_t* deviceID,
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
OCStackResult OCDiscoverSingleDeviceInUnicast(unsigned short timeout, const OicUuid_t* deviceID,
                             const char* hostAddress, OCConnectivityType connType,
                             OCProvisionDev_t **ppFoundDevice);

/**
 * The function is responsible for discovery of device is current subnet. It will list
 * all the device in subnet which are not yet owned. Please call OCInit with OC_CLIENT_SERVER as
 * OCMode.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of candidate devices to be provisioned
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverUnownedDevices(unsigned short waittime, OCProvisionDev_t **ppList);

/**
 * Do ownership transfer for un-owned device.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDevices List of devices to perform ownership transfer.
 * @param[in] resultCallback Result callback function to be invoked when ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDoOwnershipTransfer(void* ctx,
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
OCStackResult OCSetOxmAllowStatus(const OicSecOxm_t oxm, const bool allowStatus);


#ifdef MULTIPLE_OWNER
/**
 * API to perfrom multiple ownership transfer for MOT enabled device.
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDevices List of devices to perform ownership transfer.
 * @param[in] resultCallback Result callback function to be invoked when ownership transfer finished.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDoMultipleOwnershipTransfer(void* ctx,
                                      OCProvisionDev_t *targetDevices,
                                      OCProvisionResultCB resultCallback);

/**
 * API to remove sub-owner from resource server
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] subOwner sub-owner UUID to be removed
 * @param[in] resultCallback callback provided by API user, callback will be invoked when
 *            DELETE 'subowneruuid' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCRemoveSubOwner(void* ctx,
                                const OCProvisionDev_t *targetDeviceInfo,
                                const OicUuid_t* subOwner,
                                OCProvisionResultCB resultCallback);

/**
 * API to remove all sub-owner from resource server
 *
 * @param[in] ctx Application context would be returned in result callback
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be invoked when
 *            DELETE 'subowneruuid' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCRemoveAllSubOwner(void* ctx,
                                const OCProvisionDev_t *targetDeviceInfo,
                                OCProvisionResultCB resultCallback);

#endif //MULTIPLE_OWNER

/**
 * API to register for particular OxM.
 *
 * @param[in] Ownership transfer method.
 * @param[in] Implementation of callback functions for owership transfer.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSetOwnerTransferCallbackData(OicSecOxm_t oxm, OTMCallbackData_t* callbackData);

/**
 * The function is responsible for discovery of owned device is current subnet. It will list
 * all the device in subnet which are owned by calling provisioning client.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of device owned by provisioning tool.
 * @return OTM_SUCCESS in case of success and other value otherwise.
 */
OCStackResult OCDiscoverOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList);

#ifdef MULTIPLE_OWNER
/**
 * The function is responsible for discovery of MOT enabled device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of MOT enabled devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDiscoverMultipleOwnerEnabledDevices(unsigned short timeout, OCProvisionDev_t **ppList);

/**
 * The function is responsible for discovery of Multiple Owned device is current subnet.
 *
 * @param[in] timeout Timeout in seconds, value till which function will listen to responses from
 *                    server before returning the list of devices.
 * @param[out] ppList List of Multiple Owned devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCDiscoverMultipleOwnedDevices(unsigned short timeout, OCProvisionDev_t **ppList);
#endif //MULTIPLE_OWNER

/**
 * API to provision credentials between two devices and ACLs for the devices who act as a server.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance,respresenting device to be provisioned.
 * @param[in] acl ACL for device 1. If this is not required set NULL.
 * @param[in] pDev2 Pointer to OCProvisionDev_t instance,respresenting device to be provisioned.
 * @param[in] acl ACL for device 2. If this is not required set NULL.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionPairwiseDevices(void* ctx, OicSecCredType_t type, size_t keySize,
                                         const OCProvisionDev_t *pDev1, OicSecAcl_t *pDev1Acl,
                                         const OCProvisionDev_t *pDev2, OicSecAcl_t *pDev2Acl,
                                         OCProvisionResultCB resultCallback);

/**
 * API to send ACL information to device.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] acl ACL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionACL(void *ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecAcl_t *acl,
                             OCProvisionResultCB resultCallback);

/**
 * function to save ACL which has several ACE into Acl of SVR.
 *
 * @param acl ACL to be saved in Acl of SVR.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSaveACL(const OicSecAcl_t* acl);

/**
 * this function requests CRED information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCGetCredResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback);

/**
 * this function requests ACL information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCGetACLResource(void* ctx, const OCProvisionDev_t *selectedDeviceInfo,
                             OCProvisionResultCB resultCallback);

/**
 * this function sends Direct-Pairing Configuration to a device.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] pconf PCONF pointer.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionDirectPairing(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecPconf_t *pconf,
                             OCProvisionResultCB resultCallback);

/**
 * API to provision credential to devices.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] type Type of credentials to be provisioned to the device.
 * @param[in] pDev1 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
   @param[in] pDev2 Pointer to OCProvisionDev_t instance,respresenting resource to be provsioned.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionCredentials(void *ctx, OicSecCredType_t type, size_t keySize,
                                      const OCProvisionDev_t *pDev1,
                                      const OCProvisionDev_t *pDev2,
                                      OCProvisionResultCB resultCallback);

#ifdef MULTIPLE_OWNER
/**
 * API to provision preconfigured PIN to device(NOT LIST).
 * If device does not support the Preconfigured PIN OxM,
 * OCProvisionPreconfigPin API will update the device's Doxm
 * and then try preconfigured PIN provisioning once again.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] preconfigPin string of preconfigured PIN.
 * @param[in] preconfigPinLen string length of 'preconfigPin'.
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            provisioning request recieves a response from first resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionPreconfigPin(void *ctx,
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
OCStackResult OCAddPreconfigPin(const OCProvisionDev_t *targetDeviceInfo,
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
OCStackResult OCChangeMOTMode(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
                            const OicSecMomType_t momType, OCProvisionResultCB resultCallback);

/**
 * API to update 'doxm.oxmsel' to resource server.
 *
 * @param[in] targetDeviceInfo Selected target device.
 * @param[in] oxmSelValue Method of multiple ownership transfer (ref. oic.sec.oxm)
 * @param[in] resultCallback callback provided by API user, callback will be called when
 *            POST 'oxmsel' request recieves a response from resource server.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCSelectMOTMethod(void *ctx, const OCProvisionDev_t *targetDeviceInfo,
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
OCStackResult OCUnlinkDevices(void* ctx,
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
OCStackResult OCRemoveDevice(void* ctx,
                             unsigned short waitTimeForOwnedDeviceDiscovery,
                             const OCProvisionDev_t* pTargetDev,
                             OCProvisionResultCB resultCallback);

/*
* Function to device revocation
* This function will remove credential of target device from all devices in subnet.
*
* @param[in] ctx Application context would be returned in result callback
* @param[in] waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device discovery.(seconds)
* @param[in] pTargetDev Device information to be revoked.
* @param[in] resultCallback callback provided by API user, callback will be called when
*            credential revocation is finished.
 * @return  OC_STACK_OK in case of success and other value otherwise.
*/
OCStackResult OCRemoveDeviceWithUuid(void* ctx,
                                     unsigned short waitTimeForOwnedDeviceDiscovery,
                                     const OicUuid_t* pTargetUuid,
                                     OCProvisionResultCB resultCallback);

/*
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
OCStackResult OCResetDevice(void* ctx, unsigned short waitTimeForOwnedDeviceDiscovery,
                            const OCProvisionDev_t* pTargetDev,
                            OCProvisionResultCB resultCallback);

/**
 * This function resets SVR DB to its factory setting.
 *
 *@return OC_STACK_OK in case of successful reset and other value otherwise.
 */
OCStackResult OCResetSVRDB(void);

/**
 * This function configures SVR DB as self-ownership.
 *
 *@return OC_STACK_OK in case of successful configue and other value otherwise.
 */
OCStackResult OCConfigSelfOwnership(void);

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
 * @param[in] waitime Wait time for the API. The wait time will be divided by 2, and half of wait time
 * will be used for unowned discovery and remaining half for owned discovery. So the wait time should be
 * equal to or more than 2.
 * @param[out] pOwnedDevList  list of owned devices.
 * @param[out] pUnownedDevList  list of unowned devices.
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCGetDevInfoFromNetwork(unsigned short waittime,
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
OCStackResult OCGetLinkedStatus(const OicUuid_t* uuidOfDevice,
                                  OCUuidList_t** uuidList,
                                  size_t* numOfDevices);

/**
 * API to delete memory allocated to linked list created by OCDiscover_XXX_Devices API.
 *
 * @param[in] pList Pointer to OCProvisionDev_t which should be deleted.
 */
void OCDeleteDiscoveredDevices(OCProvisionDev_t *pList);

/**
 * API to delete memory allocated to OicUuid_t list.
 *
 * @param[in] pList Pointer to OicUuid_t list which should be deleted.
 */
void OCDeleteUuidList(OCUuidList_t* pList);

/**
 * This function deletes ACL data.
 *
 * @param pAcl Pointer to OicSecAcl_t structure.
 */
void OCDeleteACLList(OicSecAcl_t* pAcl);

/**
 * This function deletes PDACL data.
 *
 * @param pPdAcl Pointer to OicSecPdAcl_t structure.
 */
void OCDeletePdAclList(OicSecPdAcl_t* pPdAcl);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
/**
 * this function sends CRL information to resource.
 *
 * @param[in] ctx Application context would be returned in result callback.
 * @param[in] selectedDeviceInfo Selected target device.
 * @param[in] crl CRL to provision.
 * @param[in] resultCallback callback provided by API user, callback will be called when provisioning
              request recieves a response from resource server.
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCProvisionCRL(void* ctx, const OCProvisionDev_t *selectedDeviceInfo, OicSecCrl_t *crl,
                             OCProvisionResultCB resultCallback);

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
OCStackResult OCProvisionTrustCertChain(void *ctx, OicSecCredType_t type, uint16_t credId,
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
OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize,
                                        OicEncodingType_t encodingType, uint16_t *credId);
/**
 * function to register callback, for getting notification for TrustCertChain change.
 *
 * @param[in] TrustCertChainChangeCB notifier callback function
 * @return OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCRegisterTrustCertChainNotifier(void *cb, TrustCertChainChangeCB CB);

/**
 * function to de-register TrustCertChain notification callback.
 */
void OCRemoveTrustCertChainNotifier(void);

/*
 * Function to read Trust certificate chain from SVR.
 * Caller must free when done using the returned trust certificate
 * @param[in] credId CredId of trust certificate chain in SVR.
 * @param[out] trustCertChain Trust certificate chain.
 * @param[out] chainSize Size of trust certificate chain
 * @return  OC_STACK_OK in case of success and other value otherwise.
 */
OCStackResult OCReadTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
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
OCStackResult OCSelectOwnershipTransferMethod(const OicSecOxm_t *supportedMethods,
        size_t numberOfMethods, OicSecOxm_t *selectedMethod, OwnerType_t ownerType);

#endif // __WITH_DTLS__ || __WITH_TLS__


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* OCPROVISIONINGMANAGER_H_ */
