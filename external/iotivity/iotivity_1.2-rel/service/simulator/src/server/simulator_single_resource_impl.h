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

#ifndef SIMULATOR_SINGLE_RESOURCE_IMPL_H_
#define SIMULATOR_SINGLE_RESOURCE_IMPL_H_

#include "simulator_single_resource.h"
#include "simulator_resource_model_schema.h"
#include "request_model.h"
#include "resource_update_automation_mngr.h"
#include "RamlParser.h"

class SimulatorResourceFactory;
class SimulatorSingleResourceImpl : public SimulatorSingleResource,
    public std::enable_shared_from_this<SimulatorSingleResourceImpl>
{
    public:
        friend class SimulatorResourceFactory;

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

        bool getAttribute(const std::string &attrName, SimulatorResourceAttribute &attribute);
        std::map<std::string, SimulatorResourceAttribute> getAttributes();
        bool addAttribute(const SimulatorResourceAttribute &attribute, bool notify = true);
        bool updateAttributeValue(const SimulatorResourceAttribute &attribute,
                                  bool notify = true);
        bool removeAttribute(const std::string &attrName, bool notify = true);
        int startResourceUpdation(AutoUpdateType type, int updateInterval,
                                  AutoUpdateCompleteCallback callback);
        int startAttributeUpdation(const std::string &attrName, AutoUpdateType type,
                                   int updateInterval, AutoUpdateCompleteCallback callback);
        std::vector<int> getResourceUpdations();
        std::vector<int> getAttributeUpdations();
        void stopUpdation(const int id);

        bool updateResourceModel(const SimulatorResourceModel &reqResModel,
                                 SimulatorResourceModel &updatedResModel, bool overwrite = false,
                                 bool notify = true);
        void notifyApp();
        void notifyApp(const SimulatorResourceModel &resModel);

    private:
        SimulatorSingleResourceImpl();
        void setResourceModel(const SimulatorResourceModel &resModel);
        void setResourceModelSchema(
            const std::shared_ptr<SimulatorResourceModelSchema> &resModelSchema);
        void setRequestModel(
            const std::unordered_map<std::string, std::shared_ptr<RequestModel>> &requestModels);
        void notify(int observerID, const SimulatorResourceModel &resModel);
        void notifyAll(const SimulatorResourceModel &resModel);
        void addObserver(const OC::ObservationInfo &ocObserverInfo);
        void removeObserver(const OC::ObservationInfo &ocObserverInfo);
        void removeAllObservers();
        void setCommonProperties(OC::OCRepresentation &ocResRep);
        OCEntityHandlerResult handleRequests(std::shared_ptr<OC::OCResourceRequest> request);
        OCEntityHandlerResult handleGET(const std::shared_ptr<OC::OCResourceRequest> &request);
        OCEntityHandlerResult handlePOST(const std::shared_ptr<OC::OCResourceRequest> &request);
        OCEntityHandlerResult handlePUT(const std::shared_ptr<OC::OCResourceRequest> &request);
        OCEntityHandlerResult handleDELETE(const std::shared_ptr<OC::OCResourceRequest> &request);
        bool isValidInterface(const std::string &interfaceType, const std::string &requestType);
        OCEntityHandlerResult sendResponse(const std::shared_ptr<OC::OCResourceRequest> &request,
                                           const int errorCode, OCEntityHandlerResult responseResult);
        OCEntityHandlerResult sendResponse(const std::shared_ptr<OC::OCResourceRequest> &request,
                                           const int errorCode, OCEntityHandlerResult responseResult, OC::OCRepresentation &Payload,
                                           const std::string &interfaceType);

        std::recursive_mutex m_objectLock;
        std::recursive_mutex m_modelLock;
        std::mutex m_modelSchemaLock;
        SimulatorResourceModel m_resModel;
        std::shared_ptr<SimulatorResourceModelSchema> m_resModelSchema;
        std::unordered_map<std::string, std::shared_ptr<RequestModel>> m_requestModels;
        UpdateAutomationMngr m_updateAutomationMgr;
        std::vector<ObserverInfo> m_observersList;

        ResourceModelUpdateCallback m_modelCallback;
        ObserverCallback m_observeCallback;

        OCResourceProperty m_property;
        OCResourceHandle m_resourceHandle;
};

typedef std::shared_ptr<SimulatorSingleResourceImpl> SimulatorSingleResourceImplSP;

#endif
