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

#ifndef SIMULATOR_COLLECTION_RESOURCE_IMPL_H_
#define SIMULATOR_COLLECTION_RESOURCE_IMPL_H_

#include "simulator_collection_resource.h"
#include "simulator_resource_model_schema.h"
#include "request_model.h"
#include "RamlParser.h"

class SimulatorResourceFactory;
class SimulatorCollectionResourceImpl : public SimulatorCollectionResource
{
    public:
        friend class SimulatorResourceFactory;

        bool isCollection() const;
        void setName(const std::string &name);
        void setURI(const std::string &uri);
        void setResourceType(const std::string &resourceType);
        void setInterface(const std::string &interfaceType);
        void setInterface(const std::vector<std::string> &interfaceTypes);
        void addInterface(const std::string &interfaceType);
        void setObservable(bool state);
        void setDiscoverable(bool state);
        void setObserverCallback(ObserverCallback callback);
        void setModelChangeCallback(ResourceModelUpdateCallback callback);
        bool isObservable() const;
        bool isDiscoverable() const;
        bool isStarted() const;
        void start();
        void stop();
        SimulatorResourceModel getResourceModel();
        std::vector<ObserverInfo> getObservers() const;
        void notify(int observerID);
        void notifyAll();

        std::vector<std::string> getSupportedResources();
        void addChildResource(const std::shared_ptr<SimulatorResource> &resource);
        void removeChildResource(const std::shared_ptr<SimulatorResource> &resource);
        void removeChildResource(const std::string &uri);
        std::vector<SimulatorResourceSP> getChildResources();

    private:
        SimulatorCollectionResourceImpl();
        void setResourceModel(const SimulatorResourceModel &resModel);
        void setResourceModelSchema(
            const std::shared_ptr<SimulatorResourceModelSchema> &resModelSchema);
        void setRequestModel(
            const std::unordered_map<std::string, std::shared_ptr<RequestModel>> &requestModels);

        OCEntityHandlerResult handleRequests(std::shared_ptr<OC::OCResourceRequest> request);
        std::shared_ptr<OC::OCResourceResponse> requestOnBaseLineInterface(
            std::shared_ptr<OC::OCResourceRequest> request);
        std::shared_ptr<OC::OCResourceResponse> requestOnLinkListInterface(
            std::shared_ptr<OC::OCResourceRequest> request);
        std::shared_ptr<OC::OCResourceResponse> requestOnBatchInterface(
            std::shared_ptr<OC::OCResourceRequest> request);
        OCEntityHandlerResult sendResponse(const std::shared_ptr<OC::OCResourceRequest> &request,
                                           const int errorCode, OCEntityHandlerResult responseResult);
        void sendNotification(OC::ObservationIds &observers);
        void addLink(const std::shared_ptr<SimulatorResource> &resource);
        void removeLink(std::string uri);
        bool isValidInterface(const std::string &interfaceType, const std::string &requestType);

        std::recursive_mutex m_objectLock;
        std::mutex m_modelLock;
        std::mutex m_childResourcesLock;
        std::mutex m_modelSchemaLock;
        SimulatorResourceModel m_resModel;
        std::shared_ptr<SimulatorResourceModelSchema> m_resModelSchema;
        std::unordered_map<std::string, std::shared_ptr<RequestModel>> m_requestModels;
        std::map<std::string, SimulatorResourceSP> m_childResources;
        std::vector<std::string> m_supportedTypes;
        std::vector<ObserverInfo> m_observersList;

        ObserverCallback m_observeCallback;
        ResourceModelUpdateCallback m_modelCallback;

        OCResourceProperty m_property;
        OCResourceHandle m_resourceHandle;
};

#endif
