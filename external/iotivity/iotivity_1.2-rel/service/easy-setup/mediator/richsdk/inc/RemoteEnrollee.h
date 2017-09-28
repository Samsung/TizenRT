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

#ifndef REMOTE_ENROLLEE_H_
#define REMOTE_ENROLLEE_H_

#include <memory>
#include <iostream>
#include <condition_variable>

#include "ESRichCommon.h"
#include "OCApi.h"

using namespace OC;

namespace OIC
{
    namespace Service
    {
        class OCResource;
        class EnrolleeResource;
        class CloudResource;
        class EnrolleeSecurity;

        typedef std::function<void(const std::shared_ptr<OC::OCResource> resource)>  onDeviceDiscoveredCb;

        /**
         * This class represents Remote Enrollee device instance. What operation the class provides:
         * 1) Ownership transfer for enabling secured communication between Mediator and Enrollee
         * devices.
         * 2) Provision WiFi AP information used for which Enrollee is going to connect to the AP
         * 3) Provision Device confiruation setting, i.e. language, country, and etc
         * 4) Provision Cloud information used for which Enrollee is going to register to the cloud
         */
        class RemoteEnrollee : public std::enable_shared_from_this<RemoteEnrollee>
        {
        public:
            ~RemoteEnrollee() = default;

            /**
             * Get an Enrollee's status which includes provisioning status and last error code
             *
             * @param callback will give the requested status
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see GetStatusCb
             */
            void getStatus(const GetStatusCb callback);

            /**
             * Get an Enrollee's configuration which includes WiFi supported frequency and device name
             *
             * @param callback will give the requested configuration
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see GetConfigurationStatusCb
             */
            void getConfiguration(const GetConfigurationStatusCb callback);

            /**
             * Do security provisioning such as ownership tranfer to Enrollee.
             *
             * @param callback will give the result if the security provisioning succeeds or fails for some reasons
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see SecurityProvStatusCb
             */
            void provisionSecurity(const SecurityProvStatusCb callback);

            /**
             * Do security provisioning such as ownership tranfer to Enrollee which may require more
             * specific user selections like a type of ownership transfer method or pre-configured
             * pin number used to Pre-configured pin-based MOT.
             *
             * @param callback will give the result if the security provisioning succeeds or fails for some reasons.
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see SecurityProvStatusCb
             */
            void provisionSecurity(const SecurityProvStatusCbWithOption callback);

            /**
             * Provision WiFi AP information and device configuration to Enrollee
             * 1. WiFi AP information includes a SSID, password, auth type, and encryption type.
             * 2. Device configuration includes a language (IETF language tags) and country (ISO 3166-1 Alpha-2)
             *
             * @param devProp a data structure storing the above information to be delivered
             * @param callback will give the result if the provisioning succeeds or fails
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see DeviceProp
             * @see DevicePropProvStatusCb
             */
            void provisionDeviceProperties(const DeviceProp& devProp,
                                               const DevicePropProvStatusCb callback);

            /**
             * Provision Cloud information to Enrollee, which includes Auth code, auth provider,
             * Cloud interface server URL, and etc.
             * In this function, Discovery for the Enrollee will happen again in a given network.
             * Because, this function is expected to call *AFTER* the Enrollee disconnects its Soft AP
             * and successfully connects to the certain WiFi AP. In that case, Mediator should discover
             * the Enrollee with a certain Device ID in the network.
             *
             * @param cloudProp a data structure storing the above information to be delivered
             * @param callback will give the result if the provisioning succeeds or fails
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see CloudProp
             * @see CloudPropProvStatusCb
             */
            void provisionCloudProperties(const CloudProp& cloudProp,
                                              const CloudPropProvStatusCb callback);

            /**
             * Provision Cloud information to Enrollee, which includes Auth code, auth provider,
             * Cloud interface server URL, and etc.
             * Note that, this API is skipping finding Enrollee in a given network. Instead, an OCResource
             * given as a first parameter will be considered to the Enrollee for cloud provisioning.
             *
             * @param resource an OCResource corresponding to a target Enrollee for cloud provisioning
             * @param cloudProp a data structure storing the above information to be delivered
             * @param callback will give the result if the provisioning succeeds or fails
             *
             * @throws ESInvalidParameterException If callback is null.
             * @throws ESBadRequestException If RemoteEnrollee device not created prior to this call.
             *
             * @see CloudProp
             * @see CloudPropProvStatusCb
             */
            void provisionCloudProperties(const std::shared_ptr< OC::OCResource > resource,
                                            const CloudProp& cloudProp,
                                            const CloudPropProvStatusCb callback);

            /**
             * Notify an Enrollee to Connect WiFi/Cloud
             *
             * @param connectTypes Target configurations to be connected. E.g. WiFi and coap cloud server
             * @param callback will give the result if the connect request succeeds or fails
             *
             * @see ES_CONNECT_TYPE
             * @see ConnectRequestStatusCb
             */
            void requestToConnect(const std::vector<ES_CONNECT_TYPE> &connectTypes, const ConnectRequestStatusCb callback);

        private:
            RemoteEnrollee(const std::shared_ptr< OC::OCResource > resource);

            ESResult discoverResource();

            static void onDiscoveredCallback(const std::shared_ptr<OC::OCResource> resource,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onGetStatusHandlerCallback(
                const std::shared_ptr< GetEnrolleeStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onGetConfigurationStatusHandlerCallback(
                const std::shared_ptr< GetConfigurationStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onDevicePropProvisioningStatusHandlerCallback(
                const std::shared_ptr< DevicePropProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onCloudPropProvisioningStatusHandlerCallback(
                const std::shared_ptr< CloudPropProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onConnectRequestStatusHandlerCallback(
                const std::shared_ptr< ConnectRequestStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static void onSecurityStatusHandlerCallback(
                const std::shared_ptr< SecProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            static ESOwnershipTransferData onSecurityStatusWithOptionHandlerCallback(
                const std::shared_ptr< SecProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr);

            void onDeviceDiscovered(const std::shared_ptr<OC::OCResource> resource);
            void initCloudResource();

            void getStatusHandler
                (const std::shared_ptr< GetEnrolleeStatus > status) const;
            void getConfigurationStatusHandler
                (const std::shared_ptr< GetConfigurationStatus > status) const;
            void devicePropProvisioningStatusHandler
                (const std::shared_ptr< DevicePropProvisioningStatus > status) const;
            void cloudPropProvisioningStatusHandler
                (const std::shared_ptr< CloudPropProvisioningStatus > status) const;
            void connectRequestStatusHandler(
                const std::shared_ptr< ConnectRequestStatus > status) const;
            void securityStatusHandler
                (const std::shared_ptr< SecProvisioningStatus > status) const;
            ESOwnershipTransferData securityStatusWithOptionHandler
                (const std::shared_ptr< SecProvisioningStatus > status) const;

        private:
            std::shared_ptr< OC::OCResource > m_ocResource;
            std::shared_ptr< EnrolleeResource > m_enrolleeResource;
            std::shared_ptr< EnrolleeSecurity > m_localEnrolleeSecurity;
            std::shared_ptr< EnrolleeSecurity > m_cloudEnrolleeSecurity;
            std::shared_ptr< CloudResource > m_cloudResource;

            std::string  m_deviceId;
            bool m_discoveryResponse;

            std::mutex m_discoverymtx;
            std::condition_variable m_cond;

            SecurityProvStatusCb m_securityProvStatusCb;
            SecurityProvStatusCbWithOption m_securityProvStatusCbWithOption;
            GetStatusCb m_getStatusCb;
            GetConfigurationStatusCb m_getConfigurationStatusCb;
            SecurityPinCb m_securityPinCb;
            SecProvisioningDbPathCb m_secProvisioningDbPathCb;
            DevicePropProvStatusCb m_devicePropProvStatusCb;
            CloudPropProvStatusCb m_cloudPropProvStatusCb;
            ConnectRequestStatusCb m_connectRequestStatusCb;

            friend class EasySetup;
        };
    }
}

#endif //REMOTE_ENROLLEE_H_
