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

#include "CloudResource.h"

#include "OCPlatform.h"
#include "ESException.h"
#include "OCResource.h"
#include "logger.h"

namespace OIC
{
    namespace Service
    {
        #define ES_CLOUD_RES_TAG "ES_CLOUD_RESOURCE"

        static const char ES_BASE_RES_URI[] = "/oic/res";

        CloudResource::CloudResource(std::shared_ptr< OC::OCResource > resource)
        {
            m_ocResource = resource;
        }

        void CloudResource::onCloudProvResponseSafetyCb(const HeaderOptions& headerOptions,
                                                                const OCRepresentation& rep,
                                                                const int eCode,
                                                                ESCloudResourceCb cb,
                                                                std::weak_ptr<CloudResource> this_ptr)
        {
            OIC_LOG(DEBUG, ES_CLOUD_RES_TAG, "onCloudProvResponseSafetyCb");
            std::shared_ptr<CloudResource> Ptr = this_ptr.lock();
            if(Ptr)
            {
                cb(headerOptions, rep, eCode);
            }
        }


        void CloudResource::provisionProperties(const CloudProp& cloudProp)
        {
            OIC_LOG(DEBUG, ES_CLOUD_RES_TAG, "provisionProperties IN");

            OCRepresentation provisioningRepresentation = cloudProp.toOCRepresentation();

            ESCloudResourceCb cb = std::bind(&CloudResource::onCloudProvResponseSafetyCb,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                            static_cast<ESCloudResourceCb>(
                            std::bind(&CloudResource::onCloudProvResponse, this,
                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)),
                            shared_from_this());

            m_ocResource->post(OC_RSRVD_ES_RES_TYPE_EASYSETUP, BATCH_INTERFACE,
                        provisioningRepresentation, QueryParamsMap(), cb, OC::QualityOfService::HighQos);

            OIC_LOG(DEBUG, ES_CLOUD_RES_TAG, "provisionProperties OUT");
        }

        void CloudResource::onCloudProvResponse(const HeaderOptions& /*headerOptions*/,
                const OCRepresentation& /*rep*/, const int eCode)
        {
            OIC_LOG_V(DEBUG, ES_CLOUD_RES_TAG, "onCloudProvResponse : eCode = %d",
                        eCode);

            if (eCode > OCStackResult::OC_STACK_RESOURCE_CHANGED)
            {
                ESResult result = ESResult::ES_ERROR;

                OIC_LOG(DEBUG, ES_CLOUD_RES_TAG,"onCloudProvResponse : onCloudProvResponse is failed ");

                if(eCode == OCStackResult::OC_STACK_COMM_ERROR)
                {
                    OIC_LOG(DEBUG, ES_CLOUD_RES_TAG,
                            "can't receive any response from Enrollee by a timeout threshold.");
                    result = ESResult::ES_COMMUNICATION_ERROR;
                }

                std::shared_ptr< CloudPropProvisioningStatus > provStatus = std::make_shared<
                        CloudPropProvisioningStatus >(result);
                m_cloudPropProvStatusCb(provStatus);
            }
            else
            {
                OIC_LOG(DEBUG, ES_CLOUD_RES_TAG,"onCloudProvResponse : onCloudProvResponse is success ");
                std::shared_ptr< CloudPropProvisioningStatus > provStatus =
                    std::make_shared<CloudPropProvisioningStatus >(ESResult::ES_OK);
                m_cloudPropProvStatusCb(provStatus);
            }
        }

        void CloudResource::registerCloudPropProvisioningStatusCallback(
            const CloudPropProvStatusCb callback)
        {
            m_cloudPropProvStatusCb = callback;
        }
    }
}
