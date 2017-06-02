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

#include <functional>

#include "EnrolleeResource.h"

#include "OCPlatform.h"
#include "ESException.h"
#include "OCResource.h"
#include "logger.h"

namespace OIC
{
    namespace Service
    {
        #define ES_REMOTE_ENROLLEE_RES_TAG "ES_ENROLLEE_RESOURCE"

        EnrolleeResource::EnrolleeResource(std::shared_ptr< OC::OCResource > resource)
        {
            m_ocResource = resource;
            m_getStatusCb = nullptr;
            m_getConfigurationStatusCb = nullptr;
            m_devicePropProvStatusCb = nullptr;
            m_connectRequestStatusCb = nullptr;
        }

        void EnrolleeResource::onEnrolleeResourceSafetyCB(const HeaderOptions& headerOptions,
                                                        const OCRepresentation& rep,
                                                        const int eCode,
                                                        ESEnrolleeResourceCb cb,
                                                        std::weak_ptr<EnrolleeResource> this_ptr)
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "onEnrolleeResourceSafetyCB");
            std::shared_ptr<EnrolleeResource> Ptr = this_ptr.lock();
            if(Ptr)
            {
                cb(headerOptions, rep, eCode);
            }
        }

        void EnrolleeResource::onProvisioningResponse(const HeaderOptions& /*headerOptions*/,
                const OCRepresentation& /*rep*/, const int eCode)
        {
            OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "onProvisioningResponse : eCode = %d",
                        eCode);

            if (eCode > OCStackResult::OC_STACK_RESOURCE_CHANGED)
            {
                ESResult result = ESResult::ES_ERROR;

                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                            "onProvisioningResponse : Provisioning is failed ");

                if(eCode == OCStackResult::OC_STACK_COMM_ERROR)
                {
                    OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                        "can't receive any response from Enrollee by a timeout threshold.");
                    result = ESResult::ES_COMMUNICATION_ERROR;
                }

                std::shared_ptr< DevicePropProvisioningStatus > provStatus = std::make_shared<
                        DevicePropProvisioningStatus >(result);
                m_devicePropProvStatusCb(provStatus);
                return;
            }

            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG,
                    "onProvisioningResponse : Provisioning is success. ");

            std::shared_ptr< DevicePropProvisioningStatus > provStatus = std::make_shared<
                    DevicePropProvisioningStatus >(ESResult::ES_OK);
            m_devicePropProvStatusCb(provStatus);
        }

        void EnrolleeResource::onGetStatusResponse(const HeaderOptions& /*headerOptions*/,
                const OCRepresentation& rep, const int eCode)
        {
            OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "onGetStatusResponse : eCode = %d",
                        eCode);

            if (eCode > OCStackResult::OC_STACK_RESOURCE_CHANGED)
            {
                ESResult result = ESResult::ES_ERROR;

                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                            "onGetStatusResponse : onGetStatusResponse is failed ");

                if(eCode == OCStackResult::OC_STACK_COMM_ERROR)
                {
                    OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                        "can't receive any response from Enrollee by a timeout threshold.");
                    result = ESResult::ES_COMMUNICATION_ERROR;
                }

                EnrolleeStatus enrolleeStatus(rep);
                std::shared_ptr< GetEnrolleeStatus > getEnrolleeStatus = std::make_shared<
                        GetEnrolleeStatus >(result, enrolleeStatus);

                m_getStatusCb(getEnrolleeStatus);
            }
            else
            {
                EnrolleeStatus enrolleeStatus(rep);
                std::shared_ptr< GetEnrolleeStatus > getEnrolleeStatus = std::make_shared<
                        GetEnrolleeStatus >(ESResult::ES_OK, enrolleeStatus);

                m_getStatusCb(getEnrolleeStatus);
            }
        }

        void EnrolleeResource::onGetConfigurationResponse(const HeaderOptions& /*headerOptions*/,
                const OCRepresentation& rep, const int eCode)
        {
            OIC_LOG_V(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "onGetConfigurationResponse : eCode = %d",
                        eCode);

            if (eCode > OCStackResult::OC_STACK_RESOURCE_CHANGED)
            {
                ESResult result = ESResult::ES_ERROR;

                OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                            "onGetConfigurationResponse : onGetConfigurationResponse is failed ");

                if(eCode == OCStackResult::OC_STACK_COMM_ERROR)
                {
                    OIC_LOG(ERROR, ES_REMOTE_ENROLLEE_RES_TAG,
                        "can't receive any response from Enrollee by a timeout threshold.");
                    result = ESResult::ES_COMMUNICATION_ERROR;
                }

                EnrolleeConf enrolleeConf(rep);
                std::shared_ptr< GetConfigurationStatus > getConfigurationStatus = std::make_shared<
                        GetConfigurationStatus >(result, enrolleeConf);
                m_getConfigurationStatusCb(getConfigurationStatus);
            }
            else
            {
                EnrolleeConf enrolleeConf(rep);

                std::shared_ptr< GetConfigurationStatus > getConfigurationStatus = std::make_shared<
                        GetConfigurationStatus >(ESResult::ES_OK, enrolleeConf);
                m_getConfigurationStatusCb(getConfigurationStatus);
            }
        }

         void EnrolleeResource::onConnectRequestResponse(const HeaderOptions& /*headerOptions*/,
                const OCRepresentation& /*rep*/, const int eCode)
        {
            OIC_LOG_V (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "onConnectRequestResponse : eCode = %d",
                        eCode);

            if (eCode > OCStackResult::OC_STACK_RESOURCE_CHANGED)
            {
                ESResult result = ESResult::ES_ERROR;

                OIC_LOG_V (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG,
                            "onConnectRequestResponse : onConnectRequestResponse is failed ");

                if(eCode == OCStackResult::OC_STACK_COMM_ERROR)
                {
                    OIC_LOG_V (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG,
                        "can't receive any response from Enrollee by a timeout threshold.");
                    result = ESResult::ES_COMMUNICATION_ERROR;
                }

                std::shared_ptr< ConnectRequestStatus > connectRequestStatus = std::make_shared<
                        ConnectRequestStatus >(result);
                m_connectRequestStatusCb(connectRequestStatus);
                return;
            }

            OIC_LOG_V (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG,
                    "onConnectRequestResponse : Provisioning is success. ");

            std::shared_ptr< ConnectRequestStatus > connectRequestStatus = std::make_shared<
                    ConnectRequestStatus >(ESResult::ES_OK);
            m_connectRequestStatusCb(connectRequestStatus);
        }


        void EnrolleeResource::registerGetStatusCallback(
            const GetStatusCb callback)
        {
            m_getStatusCb = callback;
        }

        void EnrolleeResource::registerGetConfigurationStatusCallback(
            const GetConfigurationStatusCb callback)
        {
            m_getConfigurationStatusCb = callback;
        }

        void EnrolleeResource::registerDevicePropProvStatusCallback(
            const DevicePropProvStatusCb callback)
        {
            m_devicePropProvStatusCb = callback;
        }

        void EnrolleeResource::registerConnectRequestStatusCallback(
            const ConnectRequestStatusCb callback)
        {
            m_connectRequestStatusCb = callback;
        }

        void EnrolleeResource::getStatus()
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "getStatus IN");

            if (m_ocResource == nullptr)
            {
                throw ESBadRequestException("Resource is not initialized");
            }

            OC::QueryParamsMap query;
            OC::OCRepresentation rep;

            std::function< OCStackResult(void) > getStatus = [&]
            {
                ESEnrolleeResourceCb cb = std::bind(&EnrolleeResource::onEnrolleeResourceSafetyCB,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                static_cast<ESEnrolleeResourceCb>(
                                std::bind(&EnrolleeResource::onGetStatusResponse, this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
                                shared_from_this());

                return m_ocResource->get(m_ocResource->getResourceTypes().at(0),
                        DEFAULT_INTERFACE, query, cb, OC::QualityOfService::HighQos);
            };

            OCStackResult result = getStatus();

            if (result != OCStackResult::OC_STACK_OK)
            {
                EnrolleeStatus enrolleeStatus(rep);// = {ES_STATE_INIT, ES_ERRCODE_NO_ERROR};
                std::shared_ptr< GetEnrolleeStatus > getEnrolleeStatus = std::make_shared<
                        GetEnrolleeStatus >(ESResult::ES_ERROR, enrolleeStatus);

                m_getStatusCb(getEnrolleeStatus);

                return;
            }
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "getStatus OUT");
        }

        void EnrolleeResource::getConfiguration()
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "getConfiguration IN");

            if (m_ocResource == nullptr)
            {
                throw ESBadRequestException("Resource is not initialized");
            }

            OC::QueryParamsMap query;
            OC::OCRepresentation rep;

            std::function< OCStackResult(void) > getConfigurationStatus = [&]
            {
                ESEnrolleeResourceCb cb = std::bind(&EnrolleeResource::onEnrolleeResourceSafetyCB,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                static_cast<ESEnrolleeResourceCb>(
                                std::bind(&EnrolleeResource::onGetConfigurationResponse, this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
                                shared_from_this());

                return m_ocResource->get(m_ocResource->getResourceTypes().at(0),
                        BATCH_INTERFACE, query, cb, OC::QualityOfService::HighQos);
            };

            OCStackResult result = getConfigurationStatus();

            if (result != OCStackResult::OC_STACK_OK)
            {
                EnrolleeConf enrolleeConf(rep);
                std::shared_ptr< GetConfigurationStatus > getConfigurationStatus = std::make_shared<
                        GetConfigurationStatus >(ESResult::ES_ERROR, enrolleeConf);
                m_getConfigurationStatusCb(getConfigurationStatus);
                return;
            }

            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "getConfiguration OUT");
        }

        void EnrolleeResource::provisionProperties(const DeviceProp& deviceProp)
        {
            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "provisionProperties IN");
            if (m_ocResource == nullptr)
            {
                throw ESBadRequestException("Resource is not initialized");
            }

            OC::QueryParamsMap query;
            OC::OCRepresentation provisioningRepresentation = deviceProp.toOCRepresentation();

            ESEnrolleeResourceCb cb = std::bind(&EnrolleeResource::onEnrolleeResourceSafetyCB,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                            static_cast<ESEnrolleeResourceCb>(
                            std::bind(&EnrolleeResource::onProvisioningResponse, this,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
                            shared_from_this());

            m_ocResource->post(OC_RSRVD_ES_RES_TYPE_EASYSETUP, BATCH_INTERFACE,
                    provisioningRepresentation, QueryParamsMap(), cb, OC::QualityOfService::HighQos);

            OIC_LOG(DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "provisionProperties OUT");
        }

        void EnrolleeResource::requestToConnect(const std::vector<ES_CONNECT_TYPE> &connectTypes)
        {
            OIC_LOG (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "requestToConnect IN");
            if (m_ocResource == nullptr)
            {
                throw ESBadRequestException("Resource is not initialized");
            }

            OC::QueryParamsMap query;
            OC::OCRepresentation requestRepresentation;
            std::vector<int> connectTypes_int;
            connectTypes_int.clear();

            for(auto it : connectTypes)
            {
                connectTypes_int.push_back(static_cast<int>(it));
            }

            requestRepresentation.setValue<std::vector<int>>(OC_RSRVD_ES_CONNECT, connectTypes_int);

            ESEnrolleeResourceCb cb = std::bind(&EnrolleeResource::onEnrolleeResourceSafetyCB,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                            static_cast<ESEnrolleeResourceCb>(
                            std::bind(&EnrolleeResource::onConnectRequestResponse, this,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
                            shared_from_this());

            m_ocResource->post(OC_RSRVD_ES_RES_TYPE_EASYSETUP, OC_RSRVD_INTERFACE_DEFAULT,
                    requestRepresentation, QueryParamsMap(), cb, OC::QualityOfService::HighQos);

            OIC_LOG (DEBUG, ES_REMOTE_ENROLLEE_RES_TAG, "requestToConnect OUT");
        }
    }
}
