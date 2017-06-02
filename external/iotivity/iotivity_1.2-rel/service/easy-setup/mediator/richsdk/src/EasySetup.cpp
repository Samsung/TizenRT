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

#include "EasySetup.hpp"

#include "OCPlatform.h"
#include "logger.h"
#include "ESException.h"
#include "RemoteEnrollee.h"

using namespace OC;

namespace OIC
{
    namespace Service
    {
        #define EASYSETUP_TAG "ES_EASY_SETUP"

        EasySetup * EasySetup::s_instance = nullptr;

        EasySetup::EasySetup()
        {
        }

        EasySetup* EasySetup::getInstance ()
        {
            if (s_instance == nullptr)
            {
                s_instance = new EasySetup ();
            }
            return s_instance;
        }

        std::shared_ptr<RemoteEnrollee> EasySetup::createRemoteEnrollee (std::shared_ptr< OC::OCResource > resource)
        {
            OIC_LOG(INFO, EASYSETUP_TAG, "createRemoteEnrollee IN");

            if(resource)
            {
                if(resource->getResourceTypes().at(0) != OC_RSRVD_ES_RES_TYPE_EASYSETUP ||
                   resource->connectivityType() & CT_ADAPTER_TCP)
                {
                    OIC_LOG (ERROR, EASYSETUP_TAG, "Given resource is not valid due to wrong rt or conntype");
                    return nullptr;
                }

                auto interfaces = resource->getResourceInterfaces();
                for(auto interface : interfaces)
                {
                    if(interface.compare(BATCH_INTERFACE) == 0)
                    {
                        OIC_LOG (INFO, EASYSETUP_TAG, "RemoteEnrollee object is succeessfully created");
                        OIC_LOG_V (INFO_PRIVATE, EASYSETUP_TAG, "HOST: %s", resource->host().c_str());
                        OIC_LOG_V (INFO_PRIVATE, EASYSETUP_TAG, "URI: %s", resource->uri().c_str());
                        OIC_LOG_V (INFO_PRIVATE, EASYSETUP_TAG, "SID: %s", resource->sid().c_str());
                        OIC_LOG_V (INFO_PRIVATE, EASYSETUP_TAG, "CONNECTIVITY: %d", resource->connectivityType());
                        return std::shared_ptr< RemoteEnrollee > (new RemoteEnrollee(resource));
                    }
                }
            }

            OIC_LOG (ERROR, EASYSETUP_TAG, "Given resource is NULL");
            return nullptr;
        }
    }
}

