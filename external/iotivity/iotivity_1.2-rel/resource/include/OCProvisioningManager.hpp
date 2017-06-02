//****************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef OC_PROVISIONINGMANAGER_CXX_H_
#define OC_PROVISIONINGMANAGER_CXX_H_

#include <thread>

#include "pinoxmcommon.h"
#ifdef __APPLE__
#include "../csdk/security/provisioning/include/ocprovisioningmanager.h"
#else
#include "ocprovisioningmanager.h"
#endif
#include "OCApi.h"
#include "OCPlatform_impl.h"
#include "oxmverifycommon.h"

namespace OC
{
    class OCSecureResource;

    typedef std::vector<std::shared_ptr<OCSecureResource>> DeviceList_t;
    typedef std::vector<OicUuid_t> UuidList_t;
    typedef std::vector<OCProvisionResult_t> PMResultList_t;
    typedef std::function<void(PMResultList_t *result, int hasError)> ResultCallBack;
    typedef std::function<void(uint16_t credId, uint8_t *trustCertChain,
            size_t chainSize)>CertChainCallBack;
    typedef std::function<OCStackResult(uint8_t verifNum[])> DisplayNumCB;
    typedef std::function<OCStackResult()> UserConfirmNumCB;

    struct ProvisionContext
    {
        ResultCallBack callback;
        ProvisionContext(ResultCallBack cb) : callback(cb){}
    };

    struct TrustCertChainContext
    {
        CertChainCallBack callback;
        TrustCertChainContext(CertChainCallBack cb) : callback(cb){}
    };

    struct DisplayNumContext
    {
        DisplayNumCB callback;
        DisplayNumContext(DisplayNumCB cb) : callback(cb){}
    };

    struct UserConfirmNumContext
    {
        UserConfirmNumCB callback;
        UserConfirmNumContext(UserConfirmNumCB cb) : callback(cb){}
    };

    /**
     * This class is for credential's to be set to devices.
     * The types supported are
     *              0:  no security mode
     *              1:  symmetric pair-wise key
     *              2:  symmetric group key
     *              4:  asymmetric key
     *              8:  signed asymmetric key (aka certificate)
     *              16: PIN /password
     */
    class Credential
    {
            OicSecCredType_t type;
            size_t keySize;
        public:
            Credential() = default;
            Credential(OicSecCredType_t type, size_t size) : type(type), keySize(size)
            {}

            /**
             * API to get credential type of device.
             * @return credential type of device.
             */
            OicSecCredType_t getCredentialType() const
            {
                return type;
            }

            /**
             * API to get size of credential key type.
             * @return size of credential key type.
             */
            size_t getCredentialKeySize() const
            {
                return keySize;
            }

            /**
             * API to set credential type of device.
             * Device can have following credential types
             *  - symmetric pair-wise key
             *  - symmetric group key
             *  - asymmetric key
             *  - signed asymmetric key (aka certificate)
             *  - PIN /password
             * @param type credential type.
             */
            void setCredentialType(OicSecCredType_t type)
            {
                this->type = type;
            }

            /**
             * API to set size of credential key type.
             * @param keySize credential key size.
             * @note can be either 128 or 256 for symmetric pair-wise key
             */
            void setCredentialKeySize(size_t keySize)
            {
                this->keySize = keySize;
            }
    };

    class OCSecure
    {
        public:
            /**
             * The API is responsible for initialization of the provisioning manager. It will load
             * provisioning database which have owned device's list and their linked status.
             *
             * @param dbPath file path of the sqlite3 database.
             *
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult provisionInit(const std::string& dbPath);

            /**
             * API to terminate the OTM process
             *
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult terminatePM();

            /**
             * API is responsible for discovery of devices in it's subnet. It will list
             * all the device in subnet which are not yet owned.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the list of devices.
             * @param list List of candidate devices to be provisioned.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult discoverUnownedDevices(unsigned short timeout,
                    DeviceList_t &list);

            /**
             * API is responsible for discovery of devices in it's subnet. It will list
             * all the device in subnet which are already owned by calling provisioning client.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the list of devices.
             * @param list List of owned devices.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult discoverOwnedDevices(unsigned short timeout,
                    DeviceList_t &list);

            /**
             * API is responsible for discovery of devices in specified endpoint/deviceID.
             * And this function will only return the specified device's response.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the specified device.
             * @param deviceID  deviceID of target device
             * @param foundDevice OCSecureResource object of found device.
             * @return ::OC_STACK_OK in case of success and other value otherwise.\n
             *         ::OC_STACK_INVALID_PARAM when deviceID is NULL or ppFoundDevice is not
             *                                  initailized.
             */
            static OCStackResult discoverSingleDevice(unsigned short timeout,
                    const OicUuid_t* deviceID,
                    std::shared_ptr<OCSecureResource> &foundDevice);

            /**
             * API is responsible for discovery of devices in specified endpoint/deviceID.
             * And this function will only return the specified device's response.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the specified device.
             * @param deviceID  deviceID of target device.
             * @param hostAddress  MAC address of target device.
             * @param connType  ConnectivityType for discovery.
             * @param foundDevice OCSecureResource object of found device.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             *         ::OC_STACK_INVALID_PARAM when deviceID is NULL or ppFoundDevice is not
             *                                  initailized.
             */
            static OCStackResult discoverSingleDeviceInUnicast(unsigned short timeout,
                    const OicUuid_t* deviceID,
                    const std::string& hostAddress,
                    OCConnectivityType connType,
                    std::shared_ptr<OCSecureResource> &foundDevice);

#ifdef MULTIPLE_OWNER
             /**
             * API is responsible for discovery of MOT(Mutilple Owner Transfer)
             * devices in current subnet.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the list of devices.
             * @param list List of MOT enabled devices.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult discoverMultipleOwnerEnabledDevices(unsigned short timeout,
                    DeviceList_t &list);

             /**
             * API is responsible for discovery of Multiple owned device in
             * current subnet.
             *
             * @param timeout Timeout in seconds, time until which function will listen to
             *                    responses from server before returning the list of devices.
             * @param list List of Multiple Owned devices.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult discoverMultipleOwnedDevices(unsigned short timeout,
                    DeviceList_t &list);

#endif

            /**
             * API for registering Pin Callback.
             *
             * @param InputPinCallback inputPin callback function.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult setInputPinCallback(InputPinCallback inputPin);

           /**
             * API for de-registering Pin Callback.
             *
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult unsetInputPinCallback();

            /**
             * API to set Pin Type policy.
             *
             * @param  pinSize pin Size
             * @param  pinType Type of the pin.
             * @return OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult setRandomPinPolicy(size_t pinSize, OicSecPinType_t pinType);

            /**
             * API to get status of all the devices in current subnet. The status include endpoint
             * information and doxm information which can be extracted during owned and unowned
             * discovery. Along with this information, API will provide information about
             * devices' status.
             * Device can have following states
             *  - ON/OFF: Device is switched on or off.
             *
             * @param timeout Wait time for the API.
             * @param ownedDevList  List of owned devices.
             * @param unownedDevList  List of unowned devices.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult getDevInfoFromNetwork(unsigned short timeout,
                    DeviceList_t &ownedDevList,
                    DeviceList_t &unownedDevList);
            /**
             * Server API to register callback to display stack generated PIN.
             *
             * @param displayPin Callback Method to Display generated PIN.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult setDisplayPinCB(GeneratePinCallback displayPin);

            /**
             * API to remove device credential and ACL from all devices in subnet.
             *
             * @param resultCallback Callback provided by API user, callback will be called when
             *            credential revocation is finished.
             * @param uuid Device uuid to be revoked.
             * @param waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device
             *            discovery in seconds.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult removeDeviceWithUuid(unsigned short waitTimeForOwnedDeviceDiscovery,
                    std::string uuid,
                    ResultCallBack resultCallback);

            /**
             * API to save ACL which has several ACE into Acl of SVR.
             *
             * @param acl ACL to be saved in Acl of SVR.
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult saveACL(const OicSecAcl_t* acl);

            /**
             *  api to register Callback for displaying verifNum in verification Just-Works
             *
             * @param displayNumCB Callback which is to be registered.
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult registerDisplayNumCallback(DisplayNumCB displayNumCB);

             /**
             * API to De-register Callback for displaying verifNum in verification Just-Works
             *
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult deregisterDisplayNumCallback();

            /**
             * API to reister Callback for getting user confirmation in verification Just-Works
             *@param userConfirmCB Callback which is to be registered.
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult registerUserConfirmCallback(UserConfirmNumCB userConfirmCB);

             /**
             * API to De-register Callback for getting user confirmation in verification Just-Works
             *
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult deregisterUserConfirmCallback();

             /*
             * Set option for Mutual Verified Just-Works
             * The default is both display PIN and get user confirmation.
             */
            static OCStackResult setVerifyOptionMask(VerifyOptionBitmask_t optionMask);

            /**
             * Callback function to display Verification Number.
             *
             * @param[in] ctx  User context returned in callback
             * @param[in] verifNum  Array of MUTUAL_VERIF_NUM_LEN size bytes
             *
             * @return OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult displayNumCallbackWrapper(void* ctx,
                    uint8_t verifNum[MUTUAL_VERIF_NUM_LEN]);

             /**
             * Callback function to get 'Num' verification result.
             *
             * @return OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult confirmUserCallbackWrapper(void* ctx);

            /**
             * API to cleanup PDM in case of timeout.
             * It will remove the PDM_DEVICE_INIT state devices from PDM.
             *
             * @return OC_STACK_OK in case of success and other value otherwise.
             */
             static OCStackResult pdmCleanupForTimeout();

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            /**
             * API to save Trust certificate chain into Cred of SVR.
             *
             * @param[in] trustCertChain Trust certificate chain to be saved in Cred of SVR.
             * @param[in] chainSize Size of trust certificate chain to be saved in Cred of SVR
             * @param[in] encodingType Encoding type of trust certificate chain to be saved in Cred of SVR
             * @param[out] credId CredId of saved trust certificate chain in Cred of SVR.
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult saveTrustCertChain(uint8_t *trustCertChain, size_t chainSize,
                                        OicEncodingType_t encodingType, uint16_t *credId);

            /*
            * API to read Trust certificate chain from SVR.
            * Caller must free when done using the returned trust certificate
            * @param[in] credId CredId of trust certificate chain in SVR.
            * @param[out] trustCertChain Trust certificate chain.
            * @param[out] chainSize Size of trust certificate chain
            * @return  OC_STACK_OK in case of success and other value otherwise.
            */
            static OCStackResult readTrustCertChain(uint16_t credId, uint8_t **trustCertChain,
                                     size_t *chainSize);

            /**
             * API to register Notifier for trustCertChain change.
             *
             * @param[in] TrustCertChainChangeCB trustCertChain Change will be
             * notified asynchronously. User need to "delete[]" trustCertChain
             * in the callback function.
             * @return  OC_STACK_OK in case of success and other value otherwise.
             */
            static OCStackResult registerTrustCertChangeNotifier(CertChainCallBack);

            /**
             * API to remove Already registered Notifier.
             *
             *@return  OC_STACK_OK always, kept it for symmetry.
             */
            static OCStackResult removeTrustCertChangeNotifier();

            /**
             * Notifier wrapper for trustCertChain change.
             *
             * @param[in] ctx  User context returned in callback
             * @param[in] credId  trustCertChain changed for this ID
             * @param[in] trustCertChain trustcertchain binary blob
             * @param[in] chainSize size of trustCertChain
             */
            static void certCallbackWrapper(void* ctx, uint16_t credId, uint8_t *trustCertChain,
                                size_t chainSize);

            /**
             * Wrapper to save the seed value to generate device UUID
             *
             * @param[in] seed  buffer of seed value
             * @param[in] seedSize byte length of seed
             */
             static OCStackResult setDeviceIdSeed(const uint8_t* seed, size_t seedSize);
#endif // __WITH_DTLS__ || __WITH_TLS__


            /**
             * This function configures SVR DB as self-ownership.
             *
             *@return OC_STACK_OK in case of successful configue and other value otherwise.
             */
            static OCStackResult configSelfOwnership();

    };

    /**
     * This class represents a secure virtual device, which can be provisioned by the
     * provisioning client.
     */
    class OCSecureResource
    {
        private:
            std::weak_ptr<std::recursive_mutex> m_csdkLock;
            OCProvisionDev_t *devPtr;   // pointer to device.

        public:
            OCSecureResource();
            OCSecureResource(std::weak_ptr<std::recursive_mutex> csdkLock, OCProvisionDev_t *dPtr);

            ~OCSecureResource();

            /**
             * API to provision credentials between two devices and ACLs for the devices who
             * act as a server.
             *
             * @param cred  Type of credentials & key size to be provisioned to the device.
             * @param acl1  ACL for device 1. If this is not required set NULL.
             * @param device2  Second device to be provisioned.
             * @param acl2  ACL for device 2. If this is not required set NULL.
             * @param resultCallback Callback will be called when provisioning request receives
             *                           a response from first resource server.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult provisionPairwiseDevices(const Credential &cred, const OicSecAcl_t* acl1,
                    const OCSecureResource &device2, const OicSecAcl_t* acl2,
                    ResultCallBack resultCallback);

            /**
             * API to do ownership transfer for un-owned device.
             *
             * @param resultCallback Result callback function to be invoked when
             *                           ownership transfer finished.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult doOwnershipTransfer(ResultCallBack resultCallback);

            /**
             * API to send ACL information to resource.
             *
             * @param acl ACL to provision.
             * @param resultCallback Callback will be called when provisioning request
             *                           receives a response from resource server.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult provisionACL(const OicSecAcl_t* acl,
                    ResultCallBack resultCallback);

            /**
             * API to provision credential to devices.
             *
             * @param cred Type of credentials to be provisioned to the device.
             * @param device2 Second device' instance, representing resource to be provisioned.
             * @param resultCallback Callback will be called when provisioning request receives
             *                           a response from first resource server.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult provisionCredentials(const Credential &cred,
                    const OCSecureResource &device2,
                    ResultCallBack resultCallback);

           /**
            * API to remove the credential & relationship between the two devices.
            *
            * @param device2 Second device information to be unlinked.
            * @param resultCallback Callback provided by API user, callback will be called when
            *            device unlink is finished.
            * @return  ::OC_STACK_OK in case of success and other value otherwise.
            */
            OCStackResult unlinkDevices(const OCSecureResource &device2,
                    ResultCallBack resultCallback);

            /**
             * API to remove device credential from all devices in subnet.
             *
             * @param resultCallback Callback provided by API user, callback will be called when
             *            credential revocation is finished.
             * @param waitTimeForOwnedDeviceDiscovery Maximum wait time for owned device
             *            discovery in seconds.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult removeDevice(unsigned short waitTimeForOwnedDeviceDiscovery,
                    ResultCallBack resultCallback);

            /**
             * API to provision DirectPairing to devices.
             *
             * @param pconf pointer to PCONF (Pairing Configuration).
             * @param resultCallback Callback will be called when provisioning request receives
             *                           a response from first resource server.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult provisionDirectPairing(const OicSecPconf_t *pconf,
                    ResultCallBack resultCallback);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
            /**
             * API to provision cert.
             *
             * @param type type of cred.
             * @param credId id of cert.
             * @param resultCallback Callback will be called when provisioning request
             *                           receives a response from resource server.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult provisionTrustCertChain(OicSecCredType_t type, uint16_t credId,
                    ResultCallBack resultCallback);

#endif // __WITH_DTLS__ or __WITH_TLS__

            /**
             * This method is used to get linked devices' IDs.
             *
             * @param uuidList Information about the list of linked devices uuids.
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult getLinkedDevices(UuidList_t &uuidList);

            /**
             * API to get the device ID of this resource.
             * @return device ID.
             */
            std::string getDeviceID();

            /**
             * API to get the information of device for provisioning.
             * @return  @ref OCProvisionDev_t Reference provides information of device for provisioning.
             */
            OCProvisionDev_t* getDevPtr()const;

            /**
             * This function returns the device's IP address.
             * @return device address.
             */
            std::string getDevAddr();

            /**
             * This function returns the device's Status.
             * @return Device status (1 = ON and 2 = OFF).
             */
            int getDeviceStatus();

            /**
             * This function provides the owned status of the device.
             * @return Device owned status.
             */
            bool getOwnedStatus();

            /**
             * API to get the proper OxM for OT.
             *
             * @param oxm Address to save the selected OxM.
             *
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult getOTMethod(OicSecOxm_t* oxm);

            /**
             * Common callback wrapper, which will be called from OC-APIs.
             */
            static void callbackWrapper(void* ctx, int nOfRes,
                    OCProvisionResult_t *arr, bool hasError);

#ifdef MULTIPLE_OWNER
            /**
             * API to update 'doxm.oxmsel' to resource server.
             *
             * @param resultCallback Callback provided by API user, callback will be
             *            called when credential revocation is finished.
             * @param oxmSelVal Method of multiple ownership transfer (ref. oic.sec.oxm)
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult selectMOTMethod( const OicSecOxm_t oxmSelVal,
                    ResultCallBack resultCallback);

            /**
             * API to update 'doxm.mom' to resource server.
             *
             * @param resultCallback Callback provided by API user, callback will be
             *            called when credential revocation is finished.
             * @param momType Mode of multiple ownership transfer (ref. oic.sec.mom)
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
             OCStackResult changeMOTMode( const OicSecMomType_t momType,
                    ResultCallBack resultCallback);

            /**
             * API to add preconfigured PIN to local SVR DB.
             *
             * @param preconfPIN Preconfig PIN which is used while multiple owner authentication
             * @param preconfPINLength Byte length of preconfig PIN
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
             OCStackResult addPreconfigPIN(const char* preconfPIN,
                    size_t preconfPINLength);

            /**
             * API to provision preconfigured PIN.
             *
             * @param resultCallback Callback provided by API user, callback will be called when
             *            credential revocation is finished.
             * @param preconfPin Preconfig PIN which is used while multiple owner authentication
             * @param preconfPinLength Byte length of preconfig PIN
             * @return  ::OC_STACK_OK in case of success and other value otherwise.
             */
             OCStackResult provisionPreconfPin(const char * preconfPin,
                    size_t preconfPinLength, ResultCallBack resultCallback);

             /**
             * API to do multiple ownership transfer for MOT enabled device.
             *
             * @param resultCallback Result callback function to be invoked when
             *                           multiple ownership transfer finished.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult doMultipleOwnershipTransfer(ResultCallBack resultCallback);

            /**
             * API to remove sub-owner from resource server
             *
             * @param[in] subOwner sub-owner UUID to be removed
             * @param[in] resultCallback callback provided by API user, callback will be invoked when
             *            DELETE 'subowneruuid' request recieves a response from resource server.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult removeSubOwner(const OicUuid_t* subOwnerId, ResultCallBack resultCallback);

            /**
             * API to remove all sub-owner from resource server
             *
             * @param[in] resultCallback callback provided by API user, callback will be invoked when
             *            DELETE 'subowneruuid' request recieves a response from resource server.
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult removeAllSubOwner(ResultCallBack resultCallback);

            /**
             * API to get a sub-owner list
             *
             * @param[out] Sub-owner list of resource server
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult getSubOwnerList(UuidList_t &uuidList);

            /**
             * API to get the proper OxM for MOT.
             *
             * @param oxm Address to save the selected OxM.
             *
             * @return ::OC_STACK_OK in case of success and other value otherwise.
             */
            OCStackResult getMOTMethod( OicSecOxm_t* oxm);

            /**
             * API to check whether MOT is supported.
             *
             * @return ::true in case of MOT supported.
             */
            bool isMOTSupported();

            /**
             * API to check whether MOT is enabled.
             *
             * @return ::true in case of MOT enabled.
             */
            bool isMOTEnabled();


#endif // MULTIPLE_OWNER

        private:
            void validateSecureResource();
    };

}
#endif // OC_PROVISIONINGMANAGER_CXX_H_
