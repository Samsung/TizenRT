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

#ifndef ENROLLEE_SECURITY_H_
#define ENROLLEE_SECURITY_H_

#include <functional>
#include <atomic>
#include <condition_variable>


#include "ESRichCommon.h"
#include "OCProvisioningManager.hpp"

namespace OIC
{
    namespace Service
    {
        #define ENROLEE_SECURITY_TAG "ES_ENROLLEE_SECURITY"
        #define UUID_SIZE (16)
        #define UUID_STRING_SIZE (37)


        class EnrolleeResource;
        class OCSecureResource;

        typedef std::vector<OCProvisionResult_t> PMResultList_t;
        typedef std::function<void(OC::PMResultList_t *result, int hasError)> ESSecurityCb;

        /**
         * This class contains the methods needed for security  layer interaction.
         *
         * @see EnrolleeSecurity
         */
        class EnrolleeSecurity : public std::enable_shared_from_this<EnrolleeSecurity>
        {
        public:
            EnrolleeSecurity(std::shared_ptr< OC::OCResource > resource);
            ESResult provisionOwnership(SecurityProvStatusCbWithOption callback);
            std::string getUUID() const;

        private:
            std::shared_ptr< OC::OCResource > m_ocResource;
            SecurityProvStatusCb m_securityProvStatusCb;
            SecurityPinCb m_securityPinCb;
            SecProvisioningDbPathCb m_secProvisioningDbPathCb;

            std::mutex m_mtx;
            std::condition_variable m_cond;
            std::atomic<bool> otmResult;
            std::atomic<bool> enableMOTModeResult;
            std::atomic<bool> motMethodProvResult;
            std::atomic<bool> preConfigPinProvResult;
            std::atomic<bool> removeDeviceResult;
            std::atomic<bool> aclResult;
            std::atomic<bool> certResult;
            std::string m_mediatorID;

            std::shared_ptr< OC::OCSecureResource > m_securedResource;

            static void onEnrolleeSecuritySafetyCB(OC::PMResultList_t *result,
                                    int hasError,
                                    ESSecurityCb cb,
                                    std::weak_ptr<EnrolleeSecurity> this_ptr);

            ESResult performOwnershipTransfer();
            bool isOwnedDeviceRegisteredInDB();
            void removeDeviceWithUuidCB(OC::PMResultList_t *result, int hasError);
            ESResult discoverTargetSecureResource();
            ESOwnershipTransferData getOwnershipTransferDataFromUser
                                        (SecurityProvStatusCbWithOption callback);
            ESResult syncUpWithMediatorDB();
#ifdef MULTIPLE_OWNER
            ESResult performMultipleOwnershipTransfer(const ESOwnershipTransferData& MOTdata);
            void changeMOTMethodCB(PMResultList_t *result, int hasError);

            void selectMOTMethodCB(PMResultList_t *result, int hasError);
            void preconfigPinProvCB(PMResultList_t *result, int hasError);
            void multipleOwnershipTransferCb(OC::PMResultList_t *result, int hasError);
            bool isSubOwnerIDMatched(std::shared_ptr< OC::OCSecureResource > foundDevice);
            ESResult requestSetPreconfPinData(const ESOwnershipTransferData& MOTData);
            ESResult requestSetMOTMethod(const ESOwnershipTransferData& MOTData);
            ESResult requestEnableMOTMode();
            ESResult provisionMOTConfig(const ESOwnershipTransferData& MOTData);
#endif
            void ownershipTransferCb(OC::PMResultList_t *result, int hasError, ESResult& res);
            void convertUUIDToString(const uint8_t uuid[UUID_SIZE],
                                                std::string& uuidString);
            std::string getResourceDeviceAddress(const std::string& host);
            bool isOwnerIDMatched(std::shared_ptr< OC::OCSecureResource > foundDevice);
            std::string getMediatorDevID();

#if defined(__WITH_DTLS__) && defined(__WITH_TLS__)
        public:
            ESResult provisionSecurityForCloudServer(
                std::string cloudUuid, int credId);
        private:
            ESResult performCertProvisioningForCloudServer(
                std::shared_ptr< OC::OCSecureResource > ownedDevice,
                int credId);
            ESResult performACLProvisioningForCloudServer(
                std::shared_ptr< OC::OCSecureResource > ownedDevice,
                std::string& cloudUuid);
            OicSecAcl_t* createAcl(const OicUuid_t cloudUuid);
            void aclProvisioningCb(PMResultList_t *result, int hasError);
            void certProvisioningCb(PMResultList_t *result, int hasError);
#endif //defined(__WITH_DTLS__) && defined(__WITH_TLS__)
        };
    }
}

#endif /* ENROLLEE_SECURITY_H_*/

