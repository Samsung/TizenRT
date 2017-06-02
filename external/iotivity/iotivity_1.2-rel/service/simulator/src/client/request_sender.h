/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef SIMULATOR_REQUEST_SENDER_H_
#define SIMULATOR_REQUEST_SENDER_H_

#include "simulator_client_types.h"
#include "simulator_resource_model.h"
#include "simulator_error_codes.h"

struct RequestInfo
{
    RequestType type;
    std::map<std::string, std::string> queryParams;
    SimulatorResourceModel payLoad;
};

class GETRequestSender
{
    public:
        typedef std::function<void (SimulatorResult result, const SimulatorResourceModel &resModel,
                                    const RequestInfo &reqInfo)> ResponseCallback;

        GETRequestSender(const std::shared_ptr<OC::OCResource> &ocResource);

        SimulatorResult send(const ResponseCallback &callback);

        SimulatorResult send(const std::map<std::string, std::string> &queryParams,
                             const ResponseCallback &callback);

        void onResponseReceived(const OC::HeaderOptions &headerOptions,
                                const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
                                ResponseCallback callback);

    private:
        std::shared_ptr<OC::OCResource> m_ocResource;
};

class PUTRequestSender
{
    public:
        typedef std::function<void (SimulatorResult result, const SimulatorResourceModel &resModel,
                                    const RequestInfo &reqInfo)> ResponseCallback;

        PUTRequestSender(const std::shared_ptr<OC::OCResource> &ocResource);

        SimulatorResult send(const SimulatorResourceModel &representation,
                             const ResponseCallback &callback);

        SimulatorResult send(const std::map<std::string, std::string> &queryParams,
                             const SimulatorResourceModel &representation,
                             const ResponseCallback &callback);

        void onResponseReceived(const OC::HeaderOptions &headerOptions,
                                const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
                                ResponseCallback callback);

    private:
        std::shared_ptr<OC::OCResource> m_ocResource;
};

class POSTRequestSender
{
    public:
        typedef std::function<void (SimulatorResult result, const SimulatorResourceModel &resModel,
                                    const RequestInfo &reqInfo)> ResponseCallback;

        POSTRequestSender(const std::shared_ptr<OC::OCResource> &ocResource);

        SimulatorResult send(const SimulatorResourceModel &representation,
                             const ResponseCallback &callback);

        SimulatorResult send(const std::map<std::string, std::string> &queryParams,
                             const SimulatorResourceModel &representation,
                             const ResponseCallback &callback);

        void onResponseReceived(const OC::HeaderOptions &headerOptions,
                                const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
                                ResponseCallback callback);

    private:
        std::shared_ptr<OC::OCResource> m_ocResource;
};

#endif
