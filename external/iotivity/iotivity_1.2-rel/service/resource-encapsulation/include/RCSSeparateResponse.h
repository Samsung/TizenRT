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

#ifndef SERVERBUILDER_RCSSEPARATERESPONSE_H
#define SERVERBUILDER_RCSSEPARATERESPONSE_H

#include "RCSRequest.h"

namespace OIC
{
    namespace Service
    {
        /**
         * This class is to send a delayed response for request handlers of the RCSResourceObject.
         *
         * @see RCSResourceObject
         * @see RCSResourceObject::SetRequestHandler
         * @see RCSResourceObject::GetRequestHandler
         * @see RCSGetResponse::separate()
         * @see RCSSetResponse::separate()
         */
        class RCSSeparateResponse
        {
        public:
            /**
             * Constructs with a request.
             *
             * @note The request must be from a request handler which returns separate().
             *
             * @see RCSResourceObject::SetRequestHandler
             * @see RCSResourceObject::GetRequestHandler
             * @see RCSGetResponse::separate()
             * @see RCSSetResponse::separate()
             */
            explicit RCSSeparateResponse(const RCSRequest& request);

            /**
             * @overload
             */
            explicit RCSSeparateResponse(RCSRequest&& request);

            RCSSeparateResponse(const RCSSeparateResponse&) = delete;
            RCSSeparateResponse& operator=(const RCSSeparateResponse&) = delete;

            RCSSeparateResponse(RCSSeparateResponse&&) = default;
            RCSSeparateResponse& operator=(RCSSeparateResponse&&) =default;

            /**
             * Sends the response to the client.
             * The payload will be composed of properties(including attributes) of
             *      the resource object of the request.
             *
             * @throws RCSBadRequestException If the RCSResourceObject which receives the request
             *      is gone or it is already set.
             * @throws RCSPlatformException If the operation failed.
             */
            void set();

        private:
            RCSRequest m_request;

            bool m_done;
        };

    }
}

#endif // SERVERBUILDER_RCSSEPARATERESPONSE_H
