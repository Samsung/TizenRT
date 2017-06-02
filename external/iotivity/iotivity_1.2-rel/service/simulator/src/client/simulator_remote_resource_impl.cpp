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

#include "simulator_remote_resource_impl.h"
#include "request_model_builder.h"
#include "simulator_exceptions.h"
#include "simulator_utils.h"
#include "simulator_logger.h"
#include "logger.h"

#define TAG "SIMULATOR_REMOTE_RESOURCE"

static std::string requestTypeToString(RequestType type)
{
    switch (type)
    {
        case RequestType::RQ_TYPE_GET:
            return "GET";
        case RequestType::RQ_TYPE_PUT:
            return "PUT";
        case RequestType::RQ_TYPE_POST:
            return "POST";
        case RequestType::RQ_TYPE_DELETE:
            return "DELETE";
        default:
            return "";
    }

    return ""; // Control should not reach here
}

static RequestType requestTypeToEnum(const std::string &type)
{
    if (type == "GET")
    {
        return RequestType::RQ_TYPE_GET;
    }
    else if (type == "PUT")
    {
        return RequestType::RQ_TYPE_PUT;
    }
    else if (type == "POST")
    {
        return RequestType::RQ_TYPE_POST;
    }
    else if (type == "DELETE")
    {
        return RequestType::RQ_TYPE_DELETE;
    }

    return RequestType::RQ_TYPE_UNKNOWN;
}

SimulatorRemoteResourceImpl::SimulatorRemoteResourceImpl(
    const std::shared_ptr<OC::OCResource> &ocResource)
    :   m_observeState(false),
        m_getRequestSender(ocResource),
        m_putRequestSender(ocResource),
        m_postRequestSender(ocResource),
        m_requestAutomationMngr(ocResource),
        m_ocResource(ocResource)
{
    m_id = m_ocResource->sid().append(m_ocResource->uri());
}

std::string SimulatorRemoteResourceImpl::getURI() const
{
    return m_ocResource->uri();
}

std::string SimulatorRemoteResourceImpl::getHost() const
{
    return m_ocResource->host();
}

std::string SimulatorRemoteResourceImpl::getID() const
{
    return m_id;
}

SimulatorConnectivityType SimulatorRemoteResourceImpl::getConnectivityType() const
{
    return convertConnectivityType(m_ocResource->connectivityType());
}

std::vector < std::string > SimulatorRemoteResourceImpl::getResourceTypes() const
{
    return m_ocResource->getResourceTypes();
}

std::vector < std::string > SimulatorRemoteResourceImpl::getInterface() const
{
    return m_ocResource->getResourceInterfaces();
}

bool SimulatorRemoteResourceImpl::isObservable() const
{
    return m_ocResource->isObservable();
}

void SimulatorRemoteResourceImpl::observe(ObserveType type,
        ObserveNotificationCallback callback)
{
    VALIDATE_CALLBACK(callback)

    std::lock_guard<std::mutex> lock(m_observeLock);
    if (m_observeState)
    {
        OIC_LOG(ERROR, TAG, "Resource is already being observed!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Resource is already being observed!")

        throw SimulatorException(SIMULATOR_ERROR, "Resource is already being observed!");
    }

    OC::ObserveCallback observeCallback = std::bind(
            [](const OC::HeaderOptions & headerOptions, const OC::OCRepresentation & ocRep,
               const int errorCode, const int sqNum, std::string id, ObserveNotificationCallback callback)
    {
        SIM_LOG(ILogger::INFO, "Response received for OBSERVE request."
                << "\n" << getPayloadString(ocRep))

        SimulatorResourceModel resourceModel = SimulatorResourceModel::build(ocRep);
        callback(id, static_cast<SimulatorResult>(errorCode), resourceModel, sqNum);
    }, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
    m_id, callback);

    OC::ObserveType observeType = OC::ObserveType::Observe;
    if (type == ObserveType::OBSERVE_ALL)
        observeType = OC::ObserveType::ObserveAll;

    try
    {
        OCStackResult ocResult = m_ocResource->observe(observeType, OC::QueryParamsMap(), observeCallback);
        if (OC_STACK_OK != ocResult)
            throw SimulatorException(static_cast<SimulatorResult>(ocResult), OC::OCException::reason(ocResult));

        SIM_LOG(ILogger::INFO, "[URI: " << getURI() << "] Sent OBSERVE request.")
    }
    catch (OC::OCException &e)
    {
        throw SimulatorException(static_cast<SimulatorResult>(e.code()), e.reason());
    }

    m_observeState = true;
}

void SimulatorRemoteResourceImpl::cancelObserve()
{
    try
    {
        OCStackResult ocResult = m_ocResource->cancelObserve(OC::QualityOfService::HighQos);
        if (OC_STACK_OK != ocResult)
        {
            OIC_LOG(ERROR, TAG, "Cancelling observe failed!");
            SIM_LOG(ILogger::INFO, "[URI: " << getURI() << "] Sending OBSERVE cancel request failed!")

            throw SimulatorException(static_cast<SimulatorResult>(ocResult),
                                     OC::OCException::reason(ocResult));
        }

        SIM_LOG(ILogger::INFO, "[URI: " << getURI() << "] Sent OBSERVE cancel request.")
    }
    catch (OC::OCException &e)
    {
        throw SimulatorException(static_cast<SimulatorResult>(e.code()), e.reason());
    }

    std::lock_guard<std::mutex> lock(m_observeLock);
    m_observeState = false;
}

void SimulatorRemoteResourceImpl::get(const GetResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_getRequestSender.send(std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send GET request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send GET request!")

        throw SimulatorException(result, "Failed to send GET request!");
    }
}

void SimulatorRemoteResourceImpl::get(const std::map<std::string, std::string> &queryParams,
                                      const GetResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_getRequestSender.send(queryParams, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send GET request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send GET request!")

        throw SimulatorException(result, "Failed to send GET request!");
    }
}

void SimulatorRemoteResourceImpl::get(const std::string &interfaceType,
                                      const std::map<std::string, std::string> &queryParams,
                                      const GetResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    std::map<std::string, std::string> queryParamsCpy = queryParams;
    if (!interfaceType.empty() && queryParamsCpy.end() == queryParamsCpy.find("if"))
    {
        queryParamsCpy["if"] = interfaceType;
    }

    SimulatorResult result = m_getRequestSender.send(queryParamsCpy, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send GET request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send GET request!")

        throw SimulatorException(result, "Failed to send GET request!");
    }
}

void SimulatorRemoteResourceImpl::put(const SimulatorResourceModel &representation,
                                      const PutResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_putRequestSender.send(representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send PUT request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send PUT request!")

        throw SimulatorException(result, "Failed to send PUT request!");
    }
}

void SimulatorRemoteResourceImpl::put(const std::map<std::string, std::string> &queryParams,
                                      const SimulatorResourceModel &representation,
                                      const PutResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_putRequestSender.send(queryParams, representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send PUT request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send PUT request!")

        throw SimulatorException(result, "Failed to send PUT request!");
    }
}

void SimulatorRemoteResourceImpl::put(const std::string &interfaceType,
                                      const std::map<std::string, std::string> &queryParams,
                                      const SimulatorResourceModel &representation,
                                      const PutResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    std::map<std::string, std::string> queryParamsCpy = queryParams;
    if (!interfaceType.empty() && queryParamsCpy.end() == queryParamsCpy.find("if"))
    {
        queryParamsCpy["if"] = interfaceType;
    }

    SimulatorResult result = m_putRequestSender.send(queryParamsCpy, representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send PUT request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send PUT request!")

        throw SimulatorException(result, "Failed to send PUT request!");
    }
}

void SimulatorRemoteResourceImpl::post(const SimulatorResourceModel &representation,
                                       const PostResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_postRequestSender.send(representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send POST request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send POST request!")

        throw SimulatorException(result, "Failed to send POST request!");
    }
}

void SimulatorRemoteResourceImpl::post(const std::map<std::string, std::string> &queryParams,
                                       const SimulatorResourceModel &representation,
                                       const PostResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    SimulatorResult result = m_postRequestSender.send(queryParams, representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send POST request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send POST request!")

        throw SimulatorException(result, "Failed to send POST request!");
    }
}

void SimulatorRemoteResourceImpl::post(const std::string &interfaceType,
                                       const std::map<std::string, std::string> &queryParams,
                                       const SimulatorResourceModel &representation,
                                       const PostResponseCallback &callback)
{
    VALIDATE_CALLBACK(callback)

    std::map<std::string, std::string> queryParamsCpy = queryParams;
    if (!interfaceType.empty() && queryParamsCpy.end() == queryParamsCpy.find("if"))
    {
        queryParamsCpy["if"] = interfaceType;
    }

    SimulatorResult result = m_postRequestSender.send(queryParamsCpy, representation, std::bind(
                                 &SimulatorRemoteResourceImpl::onResponseReceived, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3, callback));

    if (SIMULATOR_OK != result)
    {
        OIC_LOG(ERROR, TAG, "Failed to send POST request!");
        SIM_LOG(ILogger::ERROR, "[URI: " << getURI() << "] Failed to send POST request!")

        throw SimulatorException(result, "Failed to send POST request!");
    }
}

std::map<RequestType, SimulatorRequestModel>
SimulatorRemoteResourceImpl::configure(const std::string &path)
{
    VALIDATE_INPUT(path.empty(), "Path is empty!")

    try
    {
        std::shared_ptr<RAML::RamlParser> ramlParser =
            std::make_shared<RAML::RamlParser>(path);
        RAML::RamlPtr raml = ramlParser->getRamlPtr();

        m_requestModels = RequestModelBuilder().build(raml, m_ocResource->uri());
    }
    catch (RAML::RamlException &e)
    {
        throw SimulatorException(SIMULATOR_ERROR, "Failed to configure resource!");
    }

    std::map<RequestType, SimulatorRequestModel> requestModels;
    for (auto &requestModelEntry : m_requestModels)
    {
        RequestType requestType = requestTypeToEnum(requestModelEntry.first);
        SimulatorRequestModel requestModel(requestType);
        requestModel.setQueryParams((requestModelEntry.second)->getQueryParams());
        requestModel.setRequestBodySchema((requestModelEntry.second)->getRequestRepSchema());

        requestModels[requestType] = requestModel;
    }

    return requestModels;
}

int SimulatorRemoteResourceImpl::startAutoRequesting(RequestType type,
        AutoRequestGenerationCallback callback)
{
    VALIDATE_CALLBACK(callback)

    // Check if resource supports request type
    std::string requestType = requestTypeToString(type);
    if (m_requestModels.end() == m_requestModels.find(requestType))
    {
        OIC_LOG(ERROR, TAG, "Resource is not configured for this request type!");
        throw NoSupportException("Resource is not configured for this request type!");
    }

    std::shared_ptr<RequestModel> requestModel = m_requestModels[requestType];
    switch (type)
    {
        case RequestType::RQ_TYPE_GET:
            return m_requestAutomationMngr.startOnGET(requestModel,
                    std::bind(&SimulatorRemoteResourceImpl::onAutoRequestingState, this,
                              std::placeholders::_1, std::placeholders::_2, callback));

        case RequestType::RQ_TYPE_PUT:
            return m_requestAutomationMngr.startOnPUT(requestModel,
                    std::bind(&SimulatorRemoteResourceImpl::onAutoRequestingState, this,
                              std::placeholders::_1, std::placeholders::_2, callback));

        case RequestType::RQ_TYPE_POST:
            return m_requestAutomationMngr.startOnPOST(requestModel,
                    std::bind(&SimulatorRemoteResourceImpl::onAutoRequestingState, this,
                              std::placeholders::_1, std::placeholders::_2, callback));

        case RequestType::RQ_TYPE_DELETE:
        default:
            throw NoSupportException("Not implemented!");
    }

    return -1; // Code should not reach here
}

void SimulatorRemoteResourceImpl::stopAutoRequesting(int id)
{
    m_requestAutomationMngr.stop(id);
}

void SimulatorRemoteResourceImpl::onResponseReceived(SimulatorResult result,
        const SimulatorResourceModel &resourceModel, const RequestInfo &reqInfo,
        ResponseCallback callback)
{
    callback(m_id, result, resourceModel);
}

void SimulatorRemoteResourceImpl::onAutoRequestingState(int sessionId, OperationState state,
        AutoRequestGenerationCallback callback)
{
    callback(m_id, sessionId, state);
}

SimulatorConnectivityType SimulatorRemoteResourceImpl::convertConnectivityType(
    OCConnectivityType type) const
{
    switch (type)
    {
        case CT_ADAPTER_IP:
            return SIMULATOR_CT_ADAPTER_IP;

        case CT_IP_USE_V4:
            return SIMULATOR_CT_IP_USE_V4 ;

        case CT_IP_USE_V6:
            return SIMULATOR_CT_IP_USE_V6;

        case CT_ADAPTER_GATT_BTLE:
            return SIMULATOR_CT_ADAPTER_GATT_BTLE;

        case CT_ADAPTER_RFCOMM_BTEDR:
            return SIMULATOR_CT_ADAPTER_RFCOMM_BTEDR;

        default:
            return SIMULATOR_CT_DEFAULT;
    }
}
