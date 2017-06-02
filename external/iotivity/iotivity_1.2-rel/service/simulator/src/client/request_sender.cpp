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

#include "request_sender.h"
#include "simulator_logger.h"
#include "simulator_utils.h"
#include "logger.h"

#define TAG "GET_REQUEST_SNDR"

GETRequestSender::GETRequestSender(const std::shared_ptr<OC::OCResource> &ocResource)
    :   m_ocResource(ocResource) {}

SimulatorResult GETRequestSender::send(const ResponseCallback &callback)
{
    std::map<std::string, std::string> queryParams;
    return send(queryParams, callback);
}

SimulatorResult GETRequestSender::send(const std::map<std::string, std::string> &queryParams,
                                       const ResponseCallback &callback)
{
    // Create request info
    RequestInfo requestInfo;
    requestInfo.type = RequestType::RQ_TYPE_GET;
    requestInfo.queryParams = queryParams;

    SIM_LOG(ILogger::INFO, "Sending GET request." << "\n" << getRequestString(queryParams))

    OCStackResult ocResult =  m_ocResource->get(queryParams,
                              std::bind(&GETRequestSender::onResponseReceived, this, std::placeholders::_1,
                                        std::placeholders::_2, std::placeholders::_3, requestInfo, callback));
    return static_cast<SimulatorResult>(ocResult);
}

void GETRequestSender::onResponseReceived(const OC::HeaderOptions &headerOptions,
        const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
        ResponseCallback callback)
{
    SIM_LOG(ILogger::INFO, "Response received for GET." << "\n" << getPayloadString(rep));
    SimulatorResourceModel resourceModel = SimulatorResourceModel::build(rep);
    callback(static_cast<SimulatorResult>(errorCode), resourceModel, requestInfo);
}

PUTRequestSender::PUTRequestSender(const std::shared_ptr<OC::OCResource> &ocResource)
    :   m_ocResource(ocResource) {}

SimulatorResult PUTRequestSender::send(const SimulatorResourceModel &representation,
                                       const ResponseCallback &callback)
{
    std::map<std::string, std::string> queryParams;
    return send(queryParams, representation, callback);
}

SimulatorResult PUTRequestSender::send(const std::map<std::string, std::string> &queryParams,
                                       const SimulatorResourceModel &representation, const ResponseCallback &callback)
{
    // Create request info
    RequestInfo requestInfo;
    requestInfo.type = RequestType::RQ_TYPE_PUT;
    requestInfo.queryParams = queryParams;
    requestInfo.payLoad = representation;

    OC::OCRepresentation ocRep = representation.asOCRepresentation();
    SIM_LOG(ILogger::INFO, "Sending PUT request." << "\n**Payload Details**\n" << getRequestString(
                queryParams, ocRep));

    OCStackResult ocResult =  m_ocResource->put(ocRep, queryParams,
                              std::bind(&PUTRequestSender::onResponseReceived, this, std::placeholders::_1,
                                        std::placeholders::_2, std::placeholders::_3, requestInfo, callback));
    return static_cast<SimulatorResult>(ocResult);
}

void PUTRequestSender::onResponseReceived(const OC::HeaderOptions &headerOptions,
        const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
        ResponseCallback callback)
{
    SIM_LOG(ILogger::INFO, "Response received for PUT." << "\n" << getPayloadString(rep));
    SimulatorResourceModel resourceModel = SimulatorResourceModel::build(rep);
    callback(static_cast<SimulatorResult>(errorCode), resourceModel, requestInfo);
}

POSTRequestSender::POSTRequestSender(const std::shared_ptr<OC::OCResource> &ocResource)
    :   m_ocResource(ocResource) {}

SimulatorResult POSTRequestSender::send(const SimulatorResourceModel &representation,
                                        const ResponseCallback &callback)
{
    std::map<std::string, std::string> queryParams;
    return send(queryParams, representation, callback);
}

SimulatorResult POSTRequestSender::send(const std::map<std::string, std::string> &queryParams,
                                        const SimulatorResourceModel &representation, const ResponseCallback &callback)
{
    // Create request info
    RequestInfo requestInfo;
    requestInfo.type = RequestType::RQ_TYPE_POST;
    requestInfo.queryParams = queryParams;
    requestInfo.payLoad = representation;

    OC::OCRepresentation ocRep = representation.asOCRepresentation();
    SIM_LOG(ILogger::INFO, "Sending POST request." << "\n**Payload Details**\n" << getRequestString(
                queryParams, ocRep));

    OCStackResult ocResult =  m_ocResource->post(ocRep, queryParams,
                              std::bind(&POSTRequestSender::onResponseReceived, this, std::placeholders::_1,
                                        std::placeholders::_2, std::placeholders::_3, requestInfo, callback));
    return static_cast<SimulatorResult>(ocResult);
}

void POSTRequestSender::onResponseReceived(const OC::HeaderOptions &headerOptions,
        const OC::OCRepresentation &rep, const int errorCode, RequestInfo &requestInfo,
        ResponseCallback callback)
{
    SIM_LOG(ILogger::INFO, "Response received for POST." << "\n" << getPayloadString(rep));
    SimulatorResourceModel resourceModel = SimulatorResourceModel::build(rep);
    callback(static_cast<SimulatorResult>(errorCode), resourceModel, requestInfo);
}
