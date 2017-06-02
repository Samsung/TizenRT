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

#include "get_request_generator.h"
#include "request_model.h"
#include "simulator_exceptions.h"
#include "logger.h"

#define TAG "GET_REQUEST_GEN"

GETRequestGenerator::GETRequestGenerator(int id,
        const std::shared_ptr<OC::OCResource> &ocResource,
        const std::shared_ptr<RequestModel> &requestSchema,
        RequestGeneration::ProgressStateCallback callback)
    :   RequestGeneration(RequestType::RQ_TYPE_GET, id, callback),
        m_stopRequested(false),
        m_requestSchema(requestSchema),
        m_requestSender(ocResource) {}

void GETRequestGenerator::startSending()
{
    // Create thread and start sending requests in dispatched thread
    m_thread.reset(new std::thread(&GETRequestGenerator::SendAllRequests, this));
    m_thread->detach();
}

void GETRequestGenerator::stopSending()
{
    m_stopRequested = true;
}

void GETRequestGenerator::SendAllRequests()
{
    // Notify the progress status
    OIC_LOG(DEBUG, TAG, "Sending OP_START event");
    m_callback(m_id, OP_START);

    QPGenerator queryParamGen(m_requestSchema->getQueryParams());
    do
    {
        // Get the next possible queryParameter
        auto queryParam = queryParamGen.next();

        try
        {
            // Send the request
            m_requestSender.send(queryParam, std::bind(&GETRequestGenerator::onResponseReceived, this,
                                 std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            m_requestCnt++;
        }
        catch (SimulatorException &e)
        {
            m_stopRequested = true;
            break;
        }
    }
    while (!m_stopRequested && queryParamGen.hasNext());

    completed();
}

void GETRequestGenerator::onResponseReceived(SimulatorResult result,
        const SimulatorResourceModel &repModel, const RequestInfo &reqInfo)
{
    OIC_LOG(DEBUG, TAG, "Response recieved");
    m_responseCnt++;
    completed();
}

void GETRequestGenerator::completed()
{
    if (m_requestCnt == m_responseCnt)
    {
        if (m_stopRequested)
        {
            OIC_LOG(DEBUG, TAG, "Sending OP_ABORT event");
            m_callback(m_id, OP_ABORT);
        }
        else
        {
            OIC_LOG(DEBUG, TAG, "Sending OP_COMPLETE event");
            m_callback(m_id, OP_COMPLETE);
        }
    }
}