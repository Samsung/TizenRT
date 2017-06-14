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

#ifndef SIMULATOR_GET_REQUEST_GEN_H_
#define SIMULATOR_GET_REQUEST_GEN_H_

#include "request_generation.h"
#include "query_param_generator.h"
#include "request_sender.h"

class RequestModel;
class GETRequestGenerator : public RequestGeneration
{
    public:
        GETRequestGenerator(int id, const std::shared_ptr<OC::OCResource> &ocResource,
                            const std::shared_ptr<RequestModel> &requestSchema,
                            ProgressStateCallback callback);

        void startSending();
        void stopSending();

    private:
        void SendAllRequests();
        void onResponseReceived(SimulatorResult result,
                                const SimulatorResourceModel &repModel, const RequestInfo &reqInfo);
        void completed();

        bool m_stopRequested;
        std::unique_ptr<std::thread> m_thread;
        std::shared_ptr<RequestModel> m_requestSchema;
        GETRequestSender m_requestSender;
};

typedef std::shared_ptr<GETRequestGenerator> GETRequestGeneratorSP;

#endif
