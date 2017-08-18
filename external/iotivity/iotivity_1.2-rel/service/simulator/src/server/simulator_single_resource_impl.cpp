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

#include "simulator_single_resource_impl.h"
#include "simulator_utils.h"
#include "oc_interface_details.h"
#include "simulator_logger.h"
#include "logger.h"

#define TAG "SIMULATOR_SINGLE_RESOURCE"

SimulatorSingleResourceImpl::SimulatorSingleResourceImpl()
{
    m_type = SimulatorResource::Type::SINGLE_RESOURCE;
    m_interfaces.push_back(OC::DEFAULT_INTERFACE);
    m_property = static_cast<OCResourceProperty>(OC_DISCOVERABLE | OC_OBSERVABLE);
    m_resModelSchema = SimulatorResourceModelSchema::build();

    // Set resource supports GET, PUT and POST by default
    m_requestModels["GET"] = nullptr;
    m_requestModels["POST"] = nullptr;
    m_requestModels["PUT"] = nullptr;

    m_resourceHandle = nullptr;
}

void SimulatorSingleResourceImpl::setName(const std::string &name)
{
    VALIDATE_INPUT(name.empty(), "Name is empty!")
    m_name = name;
}

void SimulatorSingleResourceImpl::setURI(const std::string &uri)
{
    VALIDATE_INPUT(uri.empty(), "Uri is empty!")

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "URI can not be set when resource is started!");
    }

    m_uri = uri;
}

void SimulatorSingleResourceImpl::setResourceType(const std::string &resourceType)
{
    VALIDATE_INPUT(resourceType.empty(), "Resource type is empty!")

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "Resource type cannot be set when resource is started!");
    }

    m_resourceType = resourceType;
}

void SimulatorSingleResourceImpl::setInterface(const std::string &interfaceType)
{
    VALIDATE_INPUT(interfaceType.empty(), "Interface type list is empty!")

    if (false == OCInterfaceDetails::getInstance()->isInterface(interfaceType))
    {
        OIC_LOG(ERROR, TAG, "Request is not OIC spec defined!");
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "Resource interface can not be reset when resource is started!");
    }

    if (interfaceType != OC::DEFAULT_INTERFACE)
        m_interfaces = {OC::DEFAULT_INTERFACE, interfaceType};
}

void SimulatorSingleResourceImpl::setInterface(const std::vector<std::string> &interfaceTypes)
{
    VALIDATE_INPUT(interfaceTypes.empty(), "Interface type list is empty!")

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "Resource interface can not be reset when resource is started!");
    }

    m_interfaces = {OC::DEFAULT_INTERFACE};
    for (auto &interfaceType : interfaceTypes)
    {
        if (false == OCInterfaceDetails::getInstance()->isInterface(interfaceType))
            continue;

        if (m_interfaces.end() ==
            std::find(m_interfaces.begin(), m_interfaces.end(), interfaceType))
        {
            m_interfaces.push_back(interfaceType);
        }
    }
}

void SimulatorSingleResourceImpl::addInterface(const std::string &interfaceType)
{
    VALIDATE_INPUT(interfaceType.empty(), "Interface type is empty!")

    if (false == OCInterfaceDetails::getInstance()->isInterface(interfaceType))
    {
        OIC_LOG(ERROR, TAG, "Request is not OIC spec defined!");
        return;
    }

    if (m_interfaces.end() != std::find(m_interfaces.begin(), m_interfaces.end(), interfaceType))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "Resource already supporting this Interface: " << interfaceType)
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        typedef OCStackResult (*bindInterfaceToResource)(const OCResourceHandle &,
                const std::string &);

        invokeocplatform(static_cast<bindInterfaceToResource>(
                             OC::OCPlatform::bindInterfaceToResource), m_resourceHandle,
                         interfaceType);
    }

    m_interfaces.push_back(interfaceType);
}

void SimulatorSingleResourceImpl::setObservable(bool state)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "Observation state can not be changed when resource is started!");
    }

    if (state)
        m_property = static_cast<OCResourceProperty>(m_property | OC_OBSERVABLE);
    else
        m_property = static_cast<OCResourceProperty>(m_property ^ OC_OBSERVABLE);
}

void SimulatorSingleResourceImpl::setDiscoverable(bool state)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_OPERATION_NOT_ALLOWED,
                                 "Discoverable state can not be changed when resource is started!");
    }

    if (state)
        m_property = static_cast<OCResourceProperty>(m_property | OC_DISCOVERABLE);
    else
        m_property = static_cast<OCResourceProperty>(m_property ^ OC_DISCOVERABLE);
}

void SimulatorSingleResourceImpl::setObserverCallback(ObserverCallback callback)
{
    VALIDATE_CALLBACK(callback)
    m_observeCallback = callback;
}

void SimulatorSingleResourceImpl::setModelChangeCallback(ResourceModelUpdateCallback callback)
{
    VALIDATE_CALLBACK(callback)
    m_modelCallback = callback;
}

bool SimulatorSingleResourceImpl::isObservable() const
{
    return ((m_property & OC_OBSERVABLE) == OC_OBSERVABLE);
}

bool SimulatorSingleResourceImpl::isDiscoverable() const
{
    return ((m_property & OC_DISCOVERABLE) == OC_DISCOVERABLE);
}

bool SimulatorSingleResourceImpl::isStarted() const
{
    return (nullptr != m_resourceHandle);
}

void SimulatorSingleResourceImpl::start()
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_resourceHandle)
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Resource already started!")
        return;
    }

    if (m_uri.empty() || m_resourceType.empty())
    {
        throw SimulatorException(SIMULATOR_ERROR, "Incomplete data to start resource!");
    }

    typedef OCStackResult (*RegisterResource)(OCResourceHandle &, std::string &,
            const std::string &, const std::string &, OC::EntityHandler, uint8_t);

    invokeocplatform(static_cast<RegisterResource>(OC::OCPlatform::registerResource),
                     m_resourceHandle, m_uri, m_resourceType, m_interfaces[0],
                     std::bind(&SimulatorSingleResourceImpl::handleRequests,
                               this, std::placeholders::_1), m_property);

    for (size_t index = 1; m_interfaces.size() > 1 && index < m_interfaces.size(); index++)
    {
        typedef OCStackResult (*bindInterfaceToResource)(const OCResourceHandle &,
                const std::string &);

        try
        {
            invokeocplatform(static_cast<bindInterfaceToResource>(
                                 OC::OCPlatform::bindInterfaceToResource), m_resourceHandle,
                             m_interfaces[index]);
        }
        catch (SimulatorException &e)
        {
            stop();
            throw;
        }
    }
}

void SimulatorSingleResourceImpl::stop()
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (!m_resourceHandle)
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Resource is not started yet!")
        return;
    }

    // Stop all the update automation of this resource
    m_updateAutomationMgr.stopAll();

    // Clear all the observers
    removeAllObservers();

    // Unregister the resource from stack
    typedef OCStackResult (*UnregisterResource)(const OCResourceHandle &);

    invokeocplatform(static_cast<UnregisterResource>(OC::OCPlatform::unregisterResource),
                     m_resourceHandle);

    m_resourceHandle = nullptr;
}

SimulatorResourceModel SimulatorSingleResourceImpl::getResourceModel()
{
    std::lock_guard<std::recursive_mutex> lock(m_modelLock);
    return m_resModel;
}

std::vector<ObserverInfo> SimulatorSingleResourceImpl::getObservers() const
{
    return m_observersList;
}

void SimulatorSingleResourceImpl::notify(int observerID)
{
    SimulatorResourceModel resModel;
    {
        std::lock_guard<std::recursive_mutex> lock(m_modelLock);
        resModel = m_resModel;
    }

    notify(observerID, resModel);
}

void SimulatorSingleResourceImpl::notifyAll()
{
    SimulatorResourceModel resModel;
    {
        std::lock_guard<std::recursive_mutex> lock(m_modelLock);
        resModel = m_resModel;
    }

    notifyAll(resModel);
}

bool SimulatorSingleResourceImpl::getAttribute(const std::string &attrName,
        SimulatorResourceAttribute &attribute)
{
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);

    if (m_resModel.contains(attrName))
    {
        attribute.setName(attrName);
        attribute.setValue(m_resModel.getAttributeValue(attrName));
        attribute.setProperty(m_resModelSchema->get(attrName));
        return true;
    }

    return false;
}

std::map<std::string, SimulatorResourceAttribute>
SimulatorSingleResourceImpl::getAttributes()
{
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);

    std::map<std::string, SimulatorResourceAttribute> attributes;
    for (auto &attributeEntry : m_resModel.getAttributeValues())
    {
        SimulatorResourceAttribute attribute(attributeEntry.first);
        attribute.setValue(attributeEntry.second);

        attribute.setProperty(m_resModelSchema->get(attributeEntry.first));
        attributes[attributeEntry.first] = attribute;
    }

    return attributes;
}

bool SimulatorSingleResourceImpl::addAttribute(
    const SimulatorResourceAttribute &attribute, bool notify)
{
    if (attribute.getName().empty() || nullptr == attribute.getProperty())
    {
        return false;
    }

    // Add attribute to resource representation and its schema
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);

    if (!m_resModel.add(attribute.getName(), attribute.getValue()))
    {
        return false;
    }

    m_resModelSchema->add(attribute.getName(), attribute.getProperty());

    if (notify && isStarted())
    {
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Error when notifying the observers!")
        }
        notifyApp();
    }

    return true;
}

bool SimulatorSingleResourceImpl::updateAttributeValue(
    const SimulatorResourceAttribute &attribute, bool notify)
{
    if (attribute.getName().empty())
    {
        return false;
    }

    // Validate the new value against attribute schema property
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);
    auto property = m_resModelSchema->get(attribute.getName());
    if (!(property->validate(attribute.getValue())))
    {
        return false;
    }

    // Update the attribute value
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);
    if (!m_resModel.update(attribute.getName(), attribute.getValue()))
    {
        return false;
    }

    if (notify && isStarted())
    {
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Error when notifying the observers!")
        }
        notifyApp();
    }

    return true;
}

bool SimulatorSingleResourceImpl::removeAttribute(
    const std::string &attrName, bool notify)
{
    if (attrName.empty())
    {
        return false;
    }

    // Remove attribute from resource representation and its schema
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);

    m_resModelSchema->remove(attrName);
    if (!m_resModel.remove(attrName))
    {
        return false;
    }

    if (notify && isStarted())
    {
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Error when notifying the observers!")
        }
        notifyApp();
    }

    return true;
}

int SimulatorSingleResourceImpl::startResourceUpdation(AutoUpdateType type,
        int updateInterval, AutoUpdateCompleteCallback callback)
{
    VALIDATE_CALLBACK(callback)

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (!m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_NO_RESOURCE, "Resource is not started!");
    }

    return m_updateAutomationMgr.startResourceAutomation(shared_from_this(), type,
            updateInterval, callback);
}

int SimulatorSingleResourceImpl::startAttributeUpdation(const std::string &attrName,
        AutoUpdateType type, int updateInterval, AutoUpdateCompleteCallback callback)
{
    VALIDATE_CALLBACK(callback)

    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (!m_resourceHandle)
    {
        throw SimulatorException(SIMULATOR_NO_RESOURCE, "Resource is not started!");
    }

    return m_updateAutomationMgr.startAttributeAutomation(shared_from_this(), attrName,
            type, updateInterval, callback);
}

std::vector<int> SimulatorSingleResourceImpl::getResourceUpdations()
{
    return m_updateAutomationMgr.getResourceAutomationIds();
}

std::vector<int> SimulatorSingleResourceImpl::getAttributeUpdations()
{
    return m_updateAutomationMgr.getAttributeAutomationIds();
}

void SimulatorSingleResourceImpl::stopUpdation(const int id)
{
    m_updateAutomationMgr.stop(id);
}

void SimulatorSingleResourceImpl::notifyApp()
{
    if (m_modelCallback)
    {
        m_modelCallback(m_uri, m_resModel);
    }
}

void SimulatorSingleResourceImpl::notifyApp(const SimulatorResourceModel &resModel)
{
    if (m_modelCallback)
    {
        m_modelCallback(m_uri, resModel);
    }
}

void SimulatorSingleResourceImpl::setResourceModel(const SimulatorResourceModel &resModel)
{
    std::lock_guard<std::recursive_mutex> lock(m_modelLock);
    m_resModel = resModel;
}

void SimulatorSingleResourceImpl::setResourceModelSchema(
    const std::shared_ptr<SimulatorResourceModelSchema> &resModelSchema)
{
    std::lock_guard<std::mutex> lock(m_modelSchemaLock);
    m_resModelSchema = resModelSchema;
}

void SimulatorSingleResourceImpl::setRequestModel(
    const std::unordered_map<std::string, std::shared_ptr<RequestModel>> &requestModels)
{
    m_requestModels = requestModels;
}

bool SimulatorSingleResourceImpl::updateResourceModel(const SimulatorResourceModel &reqResModel,
        SimulatorResourceModel &updatedResModel, bool overwrite, bool notify)
{
    std::lock_guard<std::mutex> schemaLock(m_modelSchemaLock);
    std::lock_guard<std::recursive_mutex> modelLock(m_modelLock);

    size_t reqModelSize = reqResModel.size();
    SimulatorResourceModel tempResModel;

    for (auto &attributeName : m_resModel.getAttributeNameSet())
    {
        // Break the loop if update request model has no more attributes for validation.
        if (!reqModelSize)
        {
            break;
        }

        // Check attribute presence criteria
        if (!reqResModel.contains(attributeName))
        {
            if (overwrite)
                return false;
            continue;
        }

        reqModelSize--;

        // Value type check
        if (!(m_resModel.getType(attributeName) == reqResModel.getType(attributeName)))
        {
            return false;
        }

        // Validate the new value against attribute schema property
        auto property = m_resModelSchema->get(attributeName);
        if (!(property->validate(reqResModel.getAttributeValue(attributeName))))
        {
            return false;
        }

        tempResModel.add(attributeName, reqResModel.getAttributeValue(attributeName));
    }

    // Validation is success, now update the attribute values.
    for (auto &valueEntry : tempResModel.getAttributeValues())
    {
        m_resModel.update(valueEntry.first, valueEntry.second);
    }

    updatedResModel = m_resModel;

    if (notify && isStarted())
    {
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Error when notifying the observers!")
        }
        notifyApp();
    }

    return true;
}

void SimulatorSingleResourceImpl::notify(int observerID, const SimulatorResourceModel &resModel)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (!m_resourceHandle)
        return;

    std::shared_ptr<OC::OCResourceResponse> response(new OC::OCResourceResponse);
    response->setErrorCode(200);
    response->setResponseResult(OC_EH_OK);
    response->setResourceRepresentation(resModel.asOCRepresentation(), m_interfaces[0]);

    OC::ObservationIds observers;
    observers.push_back(observerID);

    typedef OCStackResult (*NotifyListOfObservers)(OCResourceHandle, OC::ObservationIds &,
            const std::shared_ptr<OC::OCResourceResponse>);

    invokeocplatform(static_cast<NotifyListOfObservers>(OC::OCPlatform::notifyListOfObservers),
                     m_resourceHandle, observers, response);
}

void SimulatorSingleResourceImpl::notifyAll(const SimulatorResourceModel &resModel)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (!m_resourceHandle)
        return;

    if (!m_observersList.size())
        return;

    std::shared_ptr<OC::OCResourceResponse> response(new OC::OCResourceResponse);
    response->setErrorCode(200);
    response->setResponseResult(OC_EH_OK);
    response->setResourceRepresentation(resModel.asOCRepresentation(), m_interfaces[0]);

    OC::ObservationIds observers;
    for (auto &observer : m_observersList)
        observers.push_back(observer.id);

    typedef OCStackResult (*NotifyListOfObservers)(OCResourceHandle, OC::ObservationIds &,
            const std::shared_ptr<OC::OCResourceResponse>);

    invokeocplatform(static_cast<NotifyListOfObservers>(OC::OCPlatform::notifyListOfObservers),
                     m_resourceHandle, observers, response);
}

void SimulatorSingleResourceImpl::setCommonProperties(OC::OCRepresentation &ocResRep)
{
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    ocResRep.setValue("rt", m_resourceType);
    ocResRep.setValue("if", m_interfaces);
    ocResRep.setValue("n", m_name);
}

OCEntityHandlerResult SimulatorSingleResourceImpl::handleRequests(
    std::shared_ptr<OC::OCResourceRequest> request)
{
    if (!request)
    {
        OIC_LOG(ERROR, TAG, "Request received from stack is null!");
        return OC_EH_ERROR;
    }

    OCEntityHandlerResult result = OC_EH_OK;

    // Handle GET, PUT, POST and DELETE requests
    if (OC::RequestHandlerFlag::RequestFlag & request->getRequestHandlerFlag())
    {
        {
            OC::OCRepresentation rep = request->getResourceRepresentation();
            std::string payload = getPayloadString(rep);
            SIM_LOG(ILogger::INFO, "[" << m_uri << "] " << request->getRequestType()
                    << " request received. \n**Payload details**\n" << payload)
        }

        if ("GET" == request->getRequestType())
        {
            result = handleGET(request);
        }
        else if ("PUT" == request->getRequestType())
        {
            result = handlePUT(request);
        }
        else if ("POST" == request->getRequestType())
        {
            result = handlePOST(request);
        }
        else if ("DELETE" == request->getRequestType())
        {
            result = handleDELETE(request);
        }
    }

    // Handle OBSERVE requests
    if ((OC::RequestHandlerFlag::ObserverFlag & request->getRequestHandlerFlag())
        && isObservable())
    {
        OC::ObservationInfo observationInfo = request->getObservationInfo();
        if (OC::ObserveAction::ObserveRegister == observationInfo.action)
        {
            addObserver(observationInfo);
            SIM_LOG(ILogger::INFO, "[" << m_uri << "] Observer added [id: "
                    << (int)observationInfo.obsId << ", address: " << observationInfo.address
                    << ", port: " << observationInfo.port << "].");
        }
        else if (OC::ObserveAction::ObserveUnregister == observationInfo.action)
        {
            removeObserver(observationInfo);
            SIM_LOG(ILogger::INFO, "[" << m_uri << "] Observer removed [id: "
                    << (int)observationInfo.obsId << ", address: " << observationInfo.address
                    << ", port: " << observationInfo.port << "].");
        }
    }

    return result;
}

void SimulatorSingleResourceImpl::addObserver(const OC::ObservationInfo &ocObserverInfo)
{
    ObserverInfo info {ocObserverInfo.obsId, ocObserverInfo.address, ocObserverInfo.port};
    m_observersList.push_back(info);

    if (m_observeCallback)
        m_observeCallback(m_uri, ObservationStatus::REGISTER, info);
}

void SimulatorSingleResourceImpl::removeObserver(const OC::ObservationInfo &ocObserverInfo)
{
    bool found = false;
    ObserverInfo info;
    for (auto iter = m_observersList.begin(); iter != m_observersList.end(); iter++)
    {
        if ((*iter).id == ocObserverInfo.obsId)
        {
            info = *iter;
            found = true;
            m_observersList.erase(iter);
            break;
        }
    }

    if (found && m_observeCallback)
        m_observeCallback(m_uri, ObservationStatus::UNREGISTER, info);
}

void SimulatorSingleResourceImpl::removeAllObservers()
{
    std::vector<ObserverInfo> observerList = m_observersList;
    m_observersList.clear();
    for (size_t index = 0; index < observerList.size(); index++)
    {
        if (m_observeCallback)
            m_observeCallback(m_uri, ObservationStatus::UNREGISTER, observerList[index]);
    }
}

OCEntityHandlerResult SimulatorSingleResourceImpl::handleGET(
    const std::shared_ptr<OC::OCResourceRequest> &request)
{
    // Check if resource support GET request
    if (m_requestModels.end() == m_requestModels.find("GET"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "Resource does not support GET request!")
        return sendResponse(request, 405, OC_EH_ERROR);
    }

    // Handling interface query parameter "if"
    auto interfaceType = m_interfaces[0];
    auto requestQueryParams = request->getQueryParameters();
    if (requestQueryParams.end() != requestQueryParams.find("if"))
    {
        interfaceType = requestQueryParams["if"];
    }

    if (!isValidInterface(interfaceType, "GET"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "GET request received on invalid interface : " << interfaceType)
        return OC_EH_ERROR;
    }

    OC::OCRepresentation resourceRep = getResourceModel().asOCRepresentation();

    // Add common properties to resource representation if interface is
    // neither "oic.if.a" nor "oic.if.s".
    if (OC::ACTUATOR_INTERFACE != interfaceType &&
        OC::SENSOR_INTERFACE != interfaceType)
    {
        setCommonProperties(resourceRep);
    }

    return sendResponse(request, 200, OC_EH_OK, resourceRep, interfaceType);
}

OCEntityHandlerResult SimulatorSingleResourceImpl::handlePUT(
    const std::shared_ptr<OC::OCResourceRequest> &request)
{
    // Check if resource support PUT request
    if (m_requestModels.end() == m_requestModels.find("PUT"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "Resource does not support PUT request!")
        return sendResponse(request, 405, OC_EH_ERROR);
    }

    // Handling interface query parameter "if"
    auto interfaceType = m_interfaces[0];
    auto requestQueryParams = request->getQueryParameters();
    if (requestQueryParams.end() != requestQueryParams.find("if"))
    {
        interfaceType = requestQueryParams["if"];
    }

    if (!isValidInterface(interfaceType, "PUT"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "PUT request received on invalid interface : " << interfaceType)
        return OC_EH_ERROR;
    }

    OC::OCRepresentation reqOcRep = request->getResourceRepresentation();
    SimulatorResourceModel reqResModel = SimulatorResourceModel::build(reqOcRep);
    SimulatorResourceModel updatedResModel;
    if (true == updateResourceModel(reqResModel, updatedResModel, true, false))
    {
        auto ocRep = updatedResModel.asOCRepresentation();
        auto result = sendResponse(request, 200, OC_EH_OK,
                                   ocRep, m_interfaces[0]);
        notifyApp(updatedResModel);
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                    << "Error when notifying the observers!")
        }

        return result;
    }

    SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Updating resource representation failed!")
    return sendResponse(request, 400, OC_EH_ERROR);
}

OCEntityHandlerResult SimulatorSingleResourceImpl::handlePOST(
    const std::shared_ptr<OC::OCResourceRequest> &request)
{
    // Check if resource support PUT request
    if (m_requestModels.end() == m_requestModels.find("POST"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "Resource does not support POST request!")
        return sendResponse(request, 405, OC_EH_ERROR);
    }

    // Handling interface query parameter "if"
    auto interfaceType = m_interfaces[0];
    auto requestQueryParams = request->getQueryParameters();
    if (requestQueryParams.end() != requestQueryParams.find("if"))
    {
        interfaceType = requestQueryParams["if"];
    }

    if (!isValidInterface(interfaceType, "POST"))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                << "POST request received on invalid interface : " << interfaceType)
        return OC_EH_ERROR;
    }

    OC::OCRepresentation reqOcRep = request->getResourceRepresentation();
    SimulatorResourceModel reqResModel = SimulatorResourceModel::build(reqOcRep);
    SimulatorResourceModel updatedResModel;
    if (true == updateResourceModel(reqResModel, updatedResModel, false, false))
    {
        auto ocRep = updatedResModel.asOCRepresentation();
        auto result = sendResponse(request, 200, OC_EH_OK,
                                   ocRep, m_interfaces[0]);
        notifyApp(updatedResModel);
        try
        {
            notifyAll();
        }
        catch (SimulatorException &e)
        {
            SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
                    << "Error when notifying the observers!")
        }

        return result;
    }

    SIM_LOG(ILogger::ERROR, "[" << m_uri << "] " << "Updating resource representation failed!")
    return sendResponse(request, 400, OC_EH_ERROR);
}

OCEntityHandlerResult SimulatorSingleResourceImpl::handleDELETE(
    const std::shared_ptr<OC::OCResourceRequest> &request)
{
    SIM_LOG(ILogger::ERROR, "[" << m_uri << "] "
            << "Resource does not support DELETE request!")
    return sendResponse(request, 405, OC_EH_ERROR);;
}

bool SimulatorSingleResourceImpl::isValidInterface(const std::string &interfaceType,
        const std::string &requestType)
{
    // Is this OIC defined interface ?
    if (false ==
        OCInterfaceDetails::getInstance()->isRequestSupported(interfaceType, requestType))
    {
        return false;
    }

    // Does resource support this interface ?
    std::lock_guard<std::recursive_mutex> lock(m_objectLock);
    if (m_interfaces.end() ==
        std::find(m_interfaces.begin(), m_interfaces.end(), interfaceType))
    {
        return false;
    }

    return true;
}

OCEntityHandlerResult SimulatorSingleResourceImpl::sendResponse(
    const std::shared_ptr<OC::OCResourceRequest> &request, const int errorCode,
    OCEntityHandlerResult responseResult)
{
    std::shared_ptr<OC::OCResourceResponse> response(new OC::OCResourceResponse());
    response->setRequestHandle(request->getRequestHandle());
    response->setResourceHandle(request->getResourceHandle());
    response->setErrorCode(errorCode);
    response->setResponseResult(responseResult);
    if (OC_STACK_OK != OC::OCPlatform::sendResponse(response))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] Failed to send response for " <<
            request->getRequestType() <<" request!")
        return OC_EH_ERROR;
    }

    SIM_LOG(ILogger::INFO, "[" << m_uri << "] Sent response for " <<
        request->getRequestType() << " request.")

    return OC_EH_OK;
}

OCEntityHandlerResult SimulatorSingleResourceImpl::sendResponse(
    const std::shared_ptr<OC::OCResourceRequest> &request, const int errorCode,
    OCEntityHandlerResult responseResult, OC::OCRepresentation &payload,
    const std::string &interfaceType)
{
    std::shared_ptr<OC::OCResourceResponse> response(new OC::OCResourceResponse());
    response->setRequestHandle(request->getRequestHandle());
    response->setResourceHandle(request->getResourceHandle());
    response->setErrorCode(errorCode);
    response->setResponseResult(responseResult);
    response->setResourceRepresentation(payload, interfaceType);
    if (OC_STACK_OK != OC::OCPlatform::sendResponse(response))
    {
        SIM_LOG(ILogger::ERROR, "[" << m_uri << "] Failed to send response for " <<
            request->getRequestType() <<" request!")
        return OC_EH_ERROR;
    }

    SIM_LOG(ILogger::INFO, "[" << m_uri << "] Sent response for " <<
        request->getRequestType() << " request \n\n" <<
        "**Payload details**" << getPayloadString(payload))

    return OC_EH_OK;
}
