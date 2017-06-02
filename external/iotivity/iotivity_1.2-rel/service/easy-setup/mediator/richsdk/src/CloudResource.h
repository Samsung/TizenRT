//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef CLOUD_RESOURCE_H_
#define CLOUD_RESOURCE_H_

#include <mutex>
#include <memory>

#include "ESRichCommon.h"

#include "OCApi.h"

using namespace OC;

namespace OIC
{
    namespace Service
    {
        class OCResource;
        typedef std::function<void(const HeaderOptions& headerOptions,
                                   const OCRepresentation& rep,
                                   const int eCode)> ESCloudResourceCb;
        /**
         * This class contains the resource discovery methods.
         *
         * @see CloudResource
         */
        class CloudResource : public std::enable_shared_from_this<CloudResource>
        {
        public:
            CloudResource(std::shared_ptr< OC::OCResource > resource);
            ~CloudResource() = default;

            void registerCloudPropProvisioningStatusCallback(
                    const CloudPropProvStatusCb callback);
            void provisionProperties(const CloudProp& CloudProp);

        private:
            static void onCloudProvResponseSafetyCb(const HeaderOptions& headerOptions,
                                                    const OCRepresentation& rep,
                                                    const int eCode,
                                                    ESCloudResourceCb cb,
                                                    std::weak_ptr<CloudResource> this_ptr);
            void onCloudProvResponse(const HeaderOptions& headerOptions,
                                                const OCRepresentation& rep,
                                                const int eCode);

        private:
            std::shared_ptr< OC::OCResource > m_ocResource;
            CloudPropProvStatusCb m_cloudPropProvStatusCb;
        };
    }
}
#endif
