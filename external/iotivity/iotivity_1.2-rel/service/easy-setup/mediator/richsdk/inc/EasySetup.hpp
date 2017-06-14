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

#ifndef EASYSETUP_HPP_
#define EASYSETUP_HPP_

#include <memory>
#include <vector>

#include "ESRichCommon.h"
#include "RemoteEnrollee.h"

using namespace OC;
namespace OIC
{
    namespace Service
    {
        class RemoteEnrollee;

        /**
         * This provides an API to instanciate a new RemoteEnrollee object correspondent to Enrollee
         * Device to be setup.
         */
        class EasySetup
        {
        public:
            /**
             * API for getting the instance of EasySetup singleton class.
             * @return EasySetup instance.
             */
            static EasySetup* getInstance();

            /**
             * This API is used for creating a remote Enrollee instance.
             *
             * @param enrolleeResource an OCResource object corresponding to enrollee resource
             *        discovered in a network. The OCResource object can be obtained by calling
             *        OCPlatform.findResource() API. What resource you have to discover with
             *        the OCPlatform.findResource() API is a "easysetup" resource with a certain
             *        resource type, i.e. oic.r.easysetup
             *
             * @throws ESBadRequestException If createEnrolleeDevice is invoked with the same
             *         provisioning information.
             *
             * @return Pointer to RemoteEnrollee instance.
             */
            std::shared_ptr<RemoteEnrollee> createRemoteEnrollee(
                                        std::shared_ptr< OC::OCResource > enrolleeResource);

        private:
            EasySetup();
            ~EasySetup();

        private:
            std::shared_ptr< OC::OCResource > m_ocResource;
            static EasySetup *s_instance;
        };
    }
}

#endif /* EASYSETUP_HPP_ */
