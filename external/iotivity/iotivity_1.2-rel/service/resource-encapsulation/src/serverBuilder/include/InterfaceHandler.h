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

#ifndef RE_INTERFACEHANDLER_H_
#define RE_INTERFACEHANDLER_H_

#include <string>
#include <functional>

namespace OIC
{
    namespace Service
    {
        class RCSRequest;
        class RCSResourceObject;
        class RCSRepresentation;
        class RCSResourceAttributes;

        const std::string BASELINE_INTERFACE = "oic.if.baseline";
        const std::string ACTUATOR_INTERFACE = "oic.if.a";
        const std::string SENSOR_INTERFACE = "oic.if.s";
        const std::string BATCH_INTERFACE = "oic.if.b";

        class InterfaceHandler
        {
        public:
            typedef std::function< RCSRepresentation(RCSRequest, const RCSResourceObject&) >
                GetResponseBuilder;

            typedef std::function< RCSRepresentation(RCSRequest, const RCSResourceObject&) >
                SetResponseBuilder;

        public:
            InterfaceHandler(GetResponseBuilder, SetResponseBuilder);

            bool isGetSupported() const;
            bool isSetSupported() const;

            GetResponseBuilder getGetResponseBuilder() const;
            SetResponseBuilder getSetResponseBuilder() const;

        private:
            GetResponseBuilder m_getBuilder;
            SetResponseBuilder m_setBuilder;
        };

        InterfaceHandler getDefaultInterfaceHandler(const std::string&, const std::string&);
    }
}

#endif /* RE_INTERFACEHANDLER_H_ */
