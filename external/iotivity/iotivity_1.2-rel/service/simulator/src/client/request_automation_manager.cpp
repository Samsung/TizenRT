/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include "request_automation_manager.h"
#include "get_request_generator.h"
#include "put_request_generator.h"
#include "post_request_generator.h"
#include "request_model.h"
#include "simulator_exceptions.h"
#include "logger.h"

#define TAG "AUTO_REQ_GEN_MNGR"

RequestAutomationMngr::RequestAutomationMngr(
    const std::shared_ptr<OC::OCResource> &ocResource)
    :   m_id(0), m_ocResource(ocResource) {}

int RequestAutomationMngr::startOnGET(
    const std::shared_ptr<RequestModel> &requestSchema,
    RequestGeneration::ProgressStateCallback callback)
{
    if (!requestSchema)
    {
        OIC_LOG(ERROR, TAG, "Request schema is null!");
        throw InvalidArgsException(SIMULATOR_INVALID_PARAM, "Request model is null!");
    }

    if (!callback)
    {
        OIC_LOG(ERROR, TAG, "Invalid callback!");
        throw InvalidArgsException(SIMULATOR_INVALID_CALLBACK, "Invalid callback!");
    }

    // Check is there auto request generation session already going on for GET requests
    if (isInProgress(RequestType::RQ_TYPE_GET))
    {
        OIC_LOG(ERROR, TAG, "Auto requesting for GET is already in progress!");
        throw OperationInProgressException(
            "Another GET request generation session is already in progress!");
    }

    // Create request generation session
    RequestGeneration::ProgressStateCallback localCallback = std::bind(
                &RequestAutomationMngr::onProgressChange, this,
                std::placeholders::_1, std::placeholders::_2, callback);

    std::lock_guard<std::mutex> lock(m_lock);
    std::shared_ptr<RequestGeneration> requestGen(
        new GETRequestGenerator(m_id, m_ocResource, requestSchema, localCallback));
    m_requestGenList[m_id] = requestGen;

    try
    {
        requestGen->start();
    }
    catch (OperationInProgressException &e)
    {
        m_requestGenList.erase(m_requestGenList.find(m_id));
        throw;
    }

    return m_id++;
}

int RequestAutomationMngr::startOnPUT(const std::shared_ptr<RequestModel> &requestSchema,
                                      RequestGeneration::ProgressStateCallback callback)
{
    if (!requestSchema)
    {
        OIC_LOG(ERROR, TAG, "Request schema is null!");
        throw InvalidArgsException(SIMULATOR_INVALID_PARAM, "Request model is null!");
    }

    if (!callback)
    {
        OIC_LOG(ERROR, TAG, "Invalid callback!");
        throw InvalidArgsException(SIMULATOR_INVALID_CALLBACK, "Invalid callback!");
    }

    // Check is there auto request generation session already going on for GET requests
    if (isInProgress(RequestType::RQ_TYPE_PUT))
    {
        OIC_LOG(ERROR, TAG, "Auto requesting for PUT is already in progress!");
        throw OperationInProgressException(
            "Another PUT request generation session is already in progress!");
    }

    // Create request generation session
    RequestGeneration::ProgressStateCallback localCallback = std::bind(
                &RequestAutomationMngr::onProgressChange, this,
                std::placeholders::_1, std::placeholders::_2, callback);

    // Create and make the entry in list
    std::lock_guard<std::mutex> lock(m_lock);
    std::shared_ptr<RequestGeneration> requestGen(
        new PUTRequestGenerator(m_id, m_ocResource, requestSchema, localCallback));
    m_requestGenList[m_id] = requestGen;

    try
    {
        requestGen->start();
    }
    catch (OperationInProgressException &e)
    {
        m_requestGenList.erase(m_requestGenList.find(m_id));
        throw;
    }

    return m_id++;
}

int RequestAutomationMngr::startOnPOST(const std::shared_ptr<RequestModel> &requestSchema,
                                       RequestGeneration::ProgressStateCallback callback)
{
    if (!requestSchema)
    {
        OIC_LOG(ERROR, TAG, "Request schema is null!");
        throw InvalidArgsException(SIMULATOR_INVALID_PARAM, "Request model is null!");
    }

    if (!callback)
    {
        OIC_LOG(ERROR, TAG, "Invalid callback!");
        throw InvalidArgsException(SIMULATOR_INVALID_CALLBACK, "Invalid callback!");
    }

    // Check is there auto request generation session already going on for GET requests
    if (isInProgress(RequestType::RQ_TYPE_POST))
    {
        OIC_LOG(ERROR, TAG, "Auto requesting for POST is already in progress!");
        throw OperationInProgressException(
            "Another POST request generation session is already in progress!");
    }

    // Create request generation session
    RequestGeneration::ProgressStateCallback localCallback = std::bind(
                &RequestAutomationMngr::onProgressChange, this,
                std::placeholders::_1, std::placeholders::_2, callback);

    // Create and make the entry in list
    std::lock_guard<std::mutex> lock(m_lock);
    std::shared_ptr<RequestGeneration> requestGen(
        new POSTRequestGenerator(m_id, m_ocResource, requestSchema, localCallback));
    m_requestGenList[m_id] = requestGen;

    try
    {
        requestGen->start();
    }
    catch (OperationInProgressException &e)
    {
        m_requestGenList.erase(m_requestGenList.find(m_id));
        throw;
    }

    return m_id++;
}

void RequestAutomationMngr::stop(int id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (m_requestGenList.end() != m_requestGenList.find(id))
    {
        m_requestGenList[id]->stop();
    }
}

void RequestAutomationMngr::onProgressChange(int sessionId, OperationState state,
        RequestGeneration::ProgressStateCallback clientCallback)
{
    if (!isValid(sessionId))
        return;

    // Remove the request generator from list if it is completed
    if (state == OP_COMPLETE || state == OP_ABORT)
    {
        remove(sessionId);
    }

    // Delegate notification to app callback
    clientCallback(sessionId, state);
}

bool RequestAutomationMngr::isValid(int id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (m_requestGenList.end() != m_requestGenList.find(id))
    {
        return true;
    }

    return false;
}

bool RequestAutomationMngr::isInProgress(RequestType type)
{
    std::lock_guard<std::mutex> lock(m_lock);
    for (auto &session : m_requestGenList)
    {
        if ((session.second)->type() == type)
            return true;
    }

    return false;
}

void RequestAutomationMngr::remove(int id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (m_requestGenList.end() != m_requestGenList.find(id))
    {
        m_requestGenList.erase(m_requestGenList.find(id));
    }
}
