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

#include "RemoteEnrollee.h"
#include "EnrolleeResource.h"
#include "CloudResource.h"
#include "OCPlatform.h"
#include "ESException.h"
#include "logger.h"
#include "OCResource.h"
#include "oic_string.h"
#ifdef __WITH_DTLS__
#include "EnrolleeSecurity.h"
#include "base64.h"
#include "oic_malloc.h"
#include "cacommon.h"
#endif //__WITH_DTLS

namespace OIC
{
    namespace Service
    {
        static const char ES_BASE_RES_URI[] = "/oic/res";
        #define ES_REMOTE_ENROLLEE_TAG "ES_REMOTE_ENROLLEE"
        #define DISCOVERY_TIMEOUT 1

        RemoteEnrollee::RemoteEnrollee(const std::shared_ptr< OC::OCResource > resource)
        {
            m_ocResource = resource;
            m_enrolleeResource = std::make_shared<EnrolleeResource>(m_ocResource);
            m_securityProvStatusCb = nullptr;
            m_getConfigurationStatusCb = nullptr;
            m_securityPinCb = nullptr;
            m_secProvisioningDbPathCb = nullptr;
            m_devicePropProvStatusCb = nullptr;
            m_cloudPropProvStatusCb = nullptr;
            m_connectRequestStatusCb = nullptr;

            m_deviceId = resource->sid();
        }

        void RemoteEnrollee::onSecurityStatusHandlerCallback(
                const std::shared_ptr< SecProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onSecurityStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->securityStatusHandler(status);
            }
        }

        void RemoteEnrollee::securityStatusHandler(
                const std::shared_ptr< SecProvisioningStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "securityStatusHandlr IN");
            OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "UUID = %s", status->getDeviceUUID().c_str());
            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG, "ESResult = %d", status->getESResult());

            if(status->getESResult() == ES_OK)
            {
                OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "Ownership transfer is successfully done.");
                m_securityProvStatusCb(status);
            }
            else
            {
                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_TAG, "Ownership transfer is failed.");
                m_securityProvStatusCb(status);
            }
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "securityStatusHandlr OUT");
        }

        ESOwnershipTransferData RemoteEnrollee::onSecurityStatusWithOptionHandlerCallback(
                const std::shared_ptr< SecProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onSecurityStatusWithOptionHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                return Ptr->securityStatusWithOptionHandler(status);
            }
            return ESOwnershipTransferData();
        }

        ESOwnershipTransferData RemoteEnrollee::securityStatusWithOptionHandler(
                const std::shared_ptr< SecProvisioningStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "securityStatusWithOptionHandler IN");
            OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "UUID = %s", status->getDeviceUUID().c_str());
            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG, "ESResult = %d", status->getESResult());

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "securityStatusWithOptionHandler OUT");
            return m_securityProvStatusCbWithOption(status);
        }

        void RemoteEnrollee::onGetStatusHandlerCallback(
                const std::shared_ptr< GetEnrolleeStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onGetStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->getStatusHandler(status);
            }
        }

        void RemoteEnrollee::getStatusHandler(
                const std::shared_ptr< GetEnrolleeStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getStatusHandler IN");

            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG, "getStatusHandler = %d",
                                                    status->getESResult());
            m_getStatusCb(status);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getStatusHandler OUT");
        }

        void RemoteEnrollee::onGetConfigurationStatusHandlerCallback(
                const std::shared_ptr< GetConfigurationStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO,ES_REMOTE_ENROLLEE_TAG,"onGetConfigurationStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->getConfigurationStatusHandler(status);
            }
        }

        void RemoteEnrollee::getConfigurationStatusHandler (
                const std::shared_ptr< GetConfigurationStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getConfigurationStatusHandler IN");

            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG,"GetConfigurationStatus = %d",
                                                    status->getESResult());
            m_getConfigurationStatusCb(status);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getConfigurationStatusHandler OUT");
        }

        void RemoteEnrollee::onDevicePropProvisioningStatusHandlerCallback(
                const std::shared_ptr< DevicePropProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onDevicePropProvisioningStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->devicePropProvisioningStatusHandler(status);
            }
        }

        void RemoteEnrollee::devicePropProvisioningStatusHandler(
                const std::shared_ptr< DevicePropProvisioningStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "devicePropProvisioningStatusHandler IN");

            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG, "DeviceProvStatus = %d", status->getESResult());
            m_devicePropProvStatusCb(status);
#ifdef __WITH_DTLS__
            if( m_ocResource.get() != nullptr &&
                    !(m_ocResource->connectivityType() & CT_ADAPTER_GATT_BTLE) &&
                    ES_OK == status->getESResult() )
            {
                // NOTE: Temporary patch
                CAEndpoint_t endpoint = {.adapter = CA_ADAPTER_IP};

                OCDevAddr address = m_ocResource->getDevAddr();
                OICStrcpy(endpoint.addr, MAX_ADDR_STR_SIZE, address.addr);
                endpoint.port = address.port;

                OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_TAG, "HOST = %s", endpoint.addr);
                OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_TAG, "PORT = %u", endpoint.port);
                CAcloseSslSession(&endpoint);
            }
#endif
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "devicePropProvisioningStatusHandler OUT");
        }

        void RemoteEnrollee::onCloudPropProvisioningStatusHandlerCallback(
                const std::shared_ptr< CloudPropProvisioningStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onCloudPropProvisioningStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->cloudPropProvisioningStatusHandler(status);
            }
        }

        void RemoteEnrollee::cloudPropProvisioningStatusHandler (
                const std::shared_ptr< CloudPropProvisioningStatus > status) const
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "cloudPropProvisioningStatusHandler IN");

            OIC_LOG_V(INFO,ES_REMOTE_ENROLLEE_TAG,"CloudProvStatus = %d",
                                                    status->getESResult());
            m_cloudPropProvStatusCb(status);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "cloudPropProvisioningStatusHandler OUT");
        }

        void RemoteEnrollee::onConnectRequestStatusHandlerCallback(
                const std::shared_ptr< ConnectRequestStatus > status,
                std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG_V(DEBUG,ES_REMOTE_ENROLLEE_TAG,"onConnectRequestStatusHandlerCallback");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->connectRequestStatusHandler(status);
            }
        }

        void RemoteEnrollee::connectRequestStatusHandler(
                const std::shared_ptr< ConnectRequestStatus > status) const
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_TAG, "connectRequestStatusHandler IN");

            OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_TAG, "RequestConnectStatus = %d", status->getESResult());
            m_connectRequestStatusCb(status);

            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_TAG, "connectRequestStatusHandler OUT");
        }

        void RemoteEnrollee::onDiscoveredCallback(const std::shared_ptr<OC::OCResource> resource,
            std::weak_ptr<RemoteEnrollee> this_ptr)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"onDiscoveredCallback()");
            std::shared_ptr<RemoteEnrollee> Ptr = this_ptr.lock();
            if(Ptr)
            {
                Ptr->onDeviceDiscovered(resource);
            }
        }

        void RemoteEnrollee::onDeviceDiscovered(std::shared_ptr<OC::OCResource> resource)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "onDeviceDiscovered IN");

            try
            {
                if(resource)
                {
                    if(!(resource->connectivityType() & CT_ADAPTER_TCP))
                    {
                        std::string resourceURI;
                        std::string hostAddress;
                        std::string hostDeviceID;

                        // Get the resource URI
                        resourceURI = resource->uri();
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG,
                                "URI of the resource: %s", resourceURI.c_str());

                        // Get the resource host address
                        hostAddress = resource->host();
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG,
                                "Host address of the resource: %s", hostAddress.c_str());

                        hostDeviceID = resource->sid();
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG,
                                "Host DeviceID of the resource: %s", hostDeviceID.c_str());

                        if(!m_deviceId.empty() && m_deviceId == hostDeviceID)
                        {
                            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "Find matched resource for cloud provisioning");
                            m_ocResource = resource;
                            m_discoveryResponse = true;
                            m_cond.notify_all();
                        }
                    }
                }
            }
            catch(std::exception& e)
            {
                OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG,
                        "Exception in foundResource: %s", e.what());
            }

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "onDeviceDiscovered OUT");
        }

        ESResult RemoteEnrollee::discoverResource()
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "discoverResource IN");

            std::string query("");
            query.append(ES_BASE_RES_URI);
            query.append("?rt=");
            query.append(OC_RSRVD_ES_RES_TYPE_EASYSETUP);

            OIC_LOG_V(INFO, ES_REMOTE_ENROLLEE_TAG, "query = %s", query.c_str());

            m_discoveryResponse = false;

            onDeviceDiscoveredCb cb = std::bind(&RemoteEnrollee::onDiscoveredCallback,
                                                std::placeholders::_1,
                                                shared_from_this());

            OCStackResult result = OC::OCPlatform::findResource("", query, CT_DEFAULT, cb);

            if (result != OCStackResult::OC_STACK_OK)
            {
                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_TAG,
                        "Failed discoverResource");
                return ES_ERROR;
            }

            std::unique_lock<std::mutex> lck(m_discoverymtx);
            m_cond.wait_for(lck, std::chrono::seconds(DISCOVERY_TIMEOUT));

            if (!m_discoveryResponse)
            {
                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_TAG,
                        "Failed discoverResource because timeout");
                return ES_ERROR;
            }

            return ES_OK;
        }

        void RemoteEnrollee::provisionSecurity(const SecurityProvStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionSecurity IN");
#ifdef __WITH_DTLS__
            ESResult res = ESResult::ES_ERROR;
            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }
            m_securityProvStatusCb = callback;

            SecurityProvStatusCb securityProvStatusCb = std::bind(
                    &RemoteEnrollee::onSecurityStatusHandlerCallback,
                    std::placeholders::_1,
                    shared_from_this());
            //TODO : DBPath is passed empty as of now. Need to take dbpath from application.
            if(!m_localEnrolleeSecurity.get())
            {
                m_localEnrolleeSecurity = std::make_shared <EnrolleeSecurity> (m_ocResource);
            }

            res = m_localEnrolleeSecurity->provisionOwnership(NULL);

            std::shared_ptr< SecProvisioningStatus > securityProvisioningStatus =
                            std::make_shared< SecProvisioningStatus >(m_localEnrolleeSecurity->getUUID(), res);
            securityProvStatusCb(securityProvisioningStatus);
#else
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"Mediator is unsecured built.");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }
            std::shared_ptr< SecProvisioningStatus > securityProvisioningStatus =
                     std::make_shared< SecProvisioningStatus >
                                   ("", ESResult::ES_SEC_OPERATION_IS_NOT_SUPPORTED);
            callback(securityProvisioningStatus);
#endif
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionSecurity OUT");
        }

        void RemoteEnrollee::provisionSecurity(const SecurityProvStatusCbWithOption callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionSecurity IN");
#ifdef __WITH_DTLS__
            ESResult res = ESResult::ES_ERROR;
            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }
            m_securityProvStatusCbWithOption = callback;

            SecurityProvStatusCbWithOption securityProvStatusCbWithOption = std::bind(
                                    &RemoteEnrollee::onSecurityStatusWithOptionHandlerCallback,
                                    std::placeholders::_1,
                                    shared_from_this());

            if(!m_localEnrolleeSecurity.get())
            {
                m_localEnrolleeSecurity = std::make_shared <EnrolleeSecurity> (m_ocResource);
            }

            res = m_localEnrolleeSecurity->provisionOwnership(securityProvStatusCbWithOption);

            std::shared_ptr< SecProvisioningStatus > securityProvisioningStatus =
                            std::make_shared< SecProvisioningStatus >(m_localEnrolleeSecurity->getUUID(), res);
            securityProvStatusCbWithOption(securityProvisioningStatus);
#else
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG,"Mediator is unsecured built.");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }
            std::shared_ptr< SecProvisioningStatus > securityProvisioningStatus =
                     std::make_shared< SecProvisioningStatus >
                                   ("", ESResult::ES_SEC_OPERATION_IS_NOT_SUPPORTED);
            callback(securityProvisioningStatus);
#endif
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionSecurity OUT");
        }

        void RemoteEnrollee::getStatus(const GetStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getStatus IN");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }

            if (m_enrolleeResource == nullptr)
            {
                throw ESBadRequestException ("Device not created");
            }

            m_getStatusCb = callback;

            GetStatusCb getStatusCb = std::bind(
                &RemoteEnrollee::onGetStatusHandlerCallback,
                std::placeholders::_1,
                shared_from_this());

            m_enrolleeResource->registerGetStatusCallback(getStatusCb);
            m_enrolleeResource->getStatus();

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getStatus OUT");
        }

        void RemoteEnrollee::getConfiguration(const GetConfigurationStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getConfiguration IN");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }

            if (m_enrolleeResource == nullptr)
            {
                throw ESBadRequestException ("Device not created");
            }

            m_getConfigurationStatusCb = callback;

            GetConfigurationStatusCb getConfigurationStatusCb = std::bind(
                    &RemoteEnrollee::onGetConfigurationStatusHandlerCallback,
                    std::placeholders::_1,
                    shared_from_this());

            m_enrolleeResource->registerGetConfigurationStatusCallback(getConfigurationStatusCb);
            m_enrolleeResource->getConfiguration();

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "getConfiguration OUT");
        }

        void RemoteEnrollee::provisionDeviceProperties(const DeviceProp& deviceProp,
                                                            const DevicePropProvStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionDeviceProperties IN");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }

            m_devicePropProvStatusCb = callback;

            if (m_enrolleeResource == nullptr)
            {
                throw ESBadRequestException ("Device not created");
            }

            if(deviceProp.getSsid().empty())
            {
                throw ESBadRequestException ("Invalid Provisiong Data.");
            }

            DevicePropProvStatusCb devicePropProvStatusCb = std::bind(
                    &RemoteEnrollee::onDevicePropProvisioningStatusHandlerCallback,
                    std::placeholders::_1,
                    shared_from_this());

            m_enrolleeResource->registerDevicePropProvStatusCallback(devicePropProvStatusCb);
            m_enrolleeResource->provisionProperties(deviceProp);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionDeviceProperties OUT");
        }

        void RemoteEnrollee::initCloudResource()
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "initCloudResource IN");

            ESResult result = ES_ERROR;

            result = discoverResource();

            if (result == ES_ERROR)
            {
                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_TAG,
                                    "Failed to create resource object using discoverResource");
                throw ESBadRequestException ("Resource object not created");
            }

            else
            {
                if(m_ocResource != nullptr)
                {
                    m_cloudResource = std::make_shared<CloudResource>(m_ocResource);
                }
                else
                {
                    throw ESBadGetException ("Resource handle is invalid");
                }
            }

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "initCloudResource OUT");
        }

        void RemoteEnrollee::provisionCloudProperties(const CloudProp& cloudProp,
                                                            const CloudPropProvStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionCloudProperties w/o OCResource IN");

            provisionCloudProperties(NULL, cloudProp, callback);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionCloudProperties w/o OCResource OUT");
        }

        void RemoteEnrollee::provisionCloudProperties(const std::shared_ptr< OC::OCResource > resource,
                                                        const CloudProp& cloudProp,
                                                        const CloudPropProvStatusCb callback)
        {
            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionCloudProperties IN");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }

            m_cloudPropProvStatusCb = callback;

            if((cloudProp.getAuthCode().empty() && cloudProp.getAccessToken().empty()) ||
                cloudProp.getAuthProvider().empty() ||
                cloudProp.getCiServer().empty())
            {
                throw ESBadRequestException ("Invalid Cloud Provisiong Info.");
            }

            if(resource)
            {
                if(resource->getResourceTypes().at(0) != OC_RSRVD_ES_RES_TYPE_EASYSETUP ||
                                resource->connectivityType() & CT_ADAPTER_TCP)
                {
                    OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_TAG, "Given resource is not valid due to wrong rt or conntype");
                    throw ESInvalidParameterException("A given OCResource is wrong");
                }

                auto interfaces = resource->getResourceInterfaces();
                bool isFound = false;
                for(auto interface : interfaces)
                {
                    if(interface.compare(BATCH_INTERFACE) == 0)
                    {
                        OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "RemoteEnrollee object is succeessfully created");
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "HOST: %s", resource->host().c_str());
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "URI: %s", resource->uri().c_str());
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "SID: %s", resource->sid().c_str());
                        OIC_LOG_V(INFO_PRIVATE, ES_REMOTE_ENROLLEE_TAG, "CONNECTIVITY: %d", resource->connectivityType());
                        isFound = true;
                    }
                }

                if(!isFound)
                {
                    throw ESInvalidParameterException("A given OCResource has no batch interface");
                }
            }

            try
            {
                if(resource == NULL)
                {
                    initCloudResource();
                }
                else
                {
                    OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "Skip to find a provisioning resource");
                    m_ocResource = resource;
                    m_cloudResource = std::make_shared<CloudResource>(m_ocResource);
                }
            }
            catch (const std::exception& e)
            {
                OIC_LOG_V(ERROR, ES_REMOTE_ENROLLEE_TAG,
                    "Exception caught in provisionCloudProperties = %s", e.what());

                std::shared_ptr< CloudPropProvisioningStatus > provStatus = std::make_shared<
                        CloudPropProvisioningStatus >(ESResult::ES_ENROLLEE_DISCOVERY_FAILURE);
                m_cloudPropProvStatusCb(provStatus);
                return;
            }
#if defined(__WITH_DTLS__) && defined(__WITH_TLS__)
            if(!(cloudProp.getCloudID().empty() && cloudProp.getCredID() <= 0))
            {
                ESResult res = ESResult::ES_ERROR;
                if(!m_cloudEnrolleeSecurity.get())
                {
                    m_cloudEnrolleeSecurity = std::make_shared <EnrolleeSecurity> (m_ocResource);
                }


                res = m_cloudEnrolleeSecurity->provisionSecurityForCloudServer(cloudProp.getCloudID(),
                                                                          cloudProp.getCredID());

                if(res != ESResult::ES_OK)
                {
                    m_cloudResource = nullptr;
                    std::shared_ptr< CloudPropProvisioningStatus > provStatus = std::make_shared<
                            CloudPropProvisioningStatus >(res);
                    m_cloudPropProvStatusCb(provStatus);
                    return;
                }
            }
            else
            {
                OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "ACL and Cert. provisioning are skipped.");
            }
#endif //defined(__WITH_DTLS__) && defined(__WITH_TLS__)

            if (m_cloudResource == nullptr)
            {
                throw ESBadRequestException ("Cloud Resource not created");
            }

            CloudPropProvStatusCb cloudPropProvStatusCb = std::bind(
                    &RemoteEnrollee::onCloudPropProvisioningStatusHandlerCallback,
                    std::placeholders::_1,
                    shared_from_this());

            m_cloudResource->registerCloudPropProvisioningStatusCallback(cloudPropProvStatusCb);
            m_cloudResource->provisionProperties(cloudProp);

            OIC_LOG(INFO, ES_REMOTE_ENROLLEE_TAG, "provisionCloudProperties OUT");
        }

        void RemoteEnrollee::requestToConnect(const std::vector<ES_CONNECT_TYPE> &connectTypes, const ConnectRequestStatusCb callback)
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_TAG, "connect IN");

            if(!callback)
            {
                throw ESInvalidParameterException("Callback is empty");
            }

            m_connectRequestStatusCb = callback;

            ConnectRequestStatusCb connectRequestStatusCb = std::bind(
                        &RemoteEnrollee::onConnectRequestStatusHandlerCallback,
                        std::placeholders::_1,
                        shared_from_this());

            m_enrolleeResource->registerConnectRequestStatusCallback(connectRequestStatusCb);
            m_enrolleeResource->requestToConnect(connectTypes);

            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_TAG, "connect OUT");
        }
    }
}


