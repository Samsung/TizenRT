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

#ifndef SERVERBUILDER_REQUESTHANDLER_H
#define SERVERBUILDER_REQUESTHANDLER_H

#include "RCSResponse.h"
#include "RCSResourceAttributes.h"

#include "OCRepresentation.h"

namespace OIC
{
    namespace Service
    {

        class RCSResourceObject;

        class RequestHandler
        {
        public:
            RequestHandler();

            RequestHandler(const RequestHandler&) = delete;
            RequestHandler(RequestHandler&&) = default;

            RequestHandler(int errorCode);

            RequestHandler(const RCSResourceAttributes&, int errorCode = DEFAULT_ERROR_CODE);

            RequestHandler(RCSResourceAttributes&&, int errorCode = DEFAULT_ERROR_CODE);

            virtual ~RequestHandler() { };

            int getErrorCode() const;

            bool hasCustomRepresentation() const;

            OC::OCRepresentation getRepresentation() const;

        public:
            static constexpr int DEFAULT_ERROR_CODE = 200;

        private:
            const int m_errorCode;
            const bool m_customRep;
            const OC::OCRepresentation m_ocRep;
        };

        class SetRequestHandler: public RequestHandler
        {
        private:
            typedef std::pair< std::string, RCSResourceAttributes::Value > AttrKeyValuePair;
            typedef std::vector< AttrKeyValuePair > AttrKeyValuePairs;

        public:
            SetRequestHandler(const SetRequestHandler&) = delete;
            SetRequestHandler(SetRequestHandler&&) = default;

            SetRequestHandler();

            SetRequestHandler(int errorCode);

            SetRequestHandler(const RCSResourceAttributes&, int errorCode = DEFAULT_ERROR_CODE);

            SetRequestHandler(RCSResourceAttributes&&, int errorCode = DEFAULT_ERROR_CODE);

            AttrKeyValuePairs applyAcceptanceMethod(RCSSetResponse::AcceptanceMethod,
                    RCSResourceObject&, const RCSResourceAttributes&) const;
        };

    }
}

#endif // SERVERBUILDER_REQUESTHANDLER_H
