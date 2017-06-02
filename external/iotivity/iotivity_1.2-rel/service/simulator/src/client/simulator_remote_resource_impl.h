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

/**
 * @file simulator_remote_resource_impl.h
 *
 * @brief This file provides internal implementation of simulator remote resource functionalities.
 *
 */

#ifndef SIMULATOR_REMOTE_RESOURCE_IMPL_H_
#define SIMULATOR_REMOTE_RESOURCE_IMPL_H_

#include "simulator_remote_resource.h"
#include "request_automation_manager.h"
#include "RamlParser.h"
#include "request_model.h"
#include "request_sender.h"

#include <mutex>

class SimulatorRemoteResourceImpl : public SimulatorRemoteResource
{
    public:
        SimulatorRemoteResourceImpl(const std::shared_ptr<OC::OCResource> &ocResource);
        std::string getURI() const;
        std::string getHost() const;
        std::string getID() const;
        SimulatorConnectivityType getConnectivityType() const;
        std::vector < std::string > getResourceTypes() const;
        std::vector < std::string > getInterface() const;
        bool isObservable() const;

        void observe(ObserveType type, ObserveNotificationCallback callback);
        void cancelObserve();

        void get(const GetResponseCallback &callback);
        void get(const std::map<std::string, std::string> &queryParams,
                 const GetResponseCallback &callback);
        void get(const std::string &interfaceType,
                 const std::map<std::string, std::string> &queryParams,
                 const GetResponseCallback &callback);

        void put(const SimulatorResourceModel &representation,
                 const PutResponseCallback &callback);
        void put(const std::map<std::string, std::string> &queryParams,
                 const SimulatorResourceModel &representation,
                 const PutResponseCallback &callback);
        void put(const std::string &interfaceType,
                 const std::map<std::string, std::string> &queryParams,
                 const SimulatorResourceModel &representation,
                 const PutResponseCallback &callback);

        void post(const SimulatorResourceModel &resourceModel,
                  const PostResponseCallback &callback);
        void post(const std::map<std::string, std::string> &queryParams,
                  const SimulatorResourceModel &representation,
                  const PostResponseCallback &callback);
        void post(const std::string &interfaceType,
                  const std::map<std::string, std::string> &queryParams,
                  const SimulatorResourceModel &representation,
                  const PostResponseCallback &callback);

        std::map<RequestType, SimulatorRequestModel> configure(
            const std::string &path);
        int startAutoRequesting(RequestType type, AutoRequestGenerationCallback callback);
        void stopAutoRequesting(int id);

    private:
        void configure(const std::shared_ptr<RAML::Raml> &raml);
        void onResponseReceived(SimulatorResult result, const SimulatorResourceModel &resourceModel,
                                const RequestInfo &reqInfo, ResponseCallback callback);
        void onAutoRequestingState(int sessionId, OperationState state,
                                   AutoRequestGenerationCallback callback);
        SimulatorConnectivityType convertConnectivityType(OCConnectivityType type) const;

        std::string m_id;
        std::mutex m_observeLock;
        bool m_observeState;
        GETRequestSender m_getRequestSender;
        PUTRequestSender m_putRequestSender;
        POSTRequestSender m_postRequestSender;

        RequestAutomationMngr m_requestAutomationMngr;
        std::unordered_map<std::string, std::shared_ptr<RequestModel>> m_requestModels;
        std::shared_ptr<OC::OCResource> m_ocResource;
};

#endif
