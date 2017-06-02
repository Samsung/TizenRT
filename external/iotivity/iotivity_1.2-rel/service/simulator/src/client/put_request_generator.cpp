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

#include "put_request_generator.h"
#include "request_model.h"
#include "simulator_exceptions.h"
#include "logger.h"

#define TAG "PUT_REQUEST_GEN"

PUTRequestGenerator::PUTRequestGenerator(int id,
        const std::shared_ptr<OC::OCResource> &ocResource,
        const std::shared_ptr<RequestModel> &requestSchema,
        RequestGeneration::ProgressStateCallback callback)
    :   RequestGeneration(RequestType::RQ_TYPE_GET, id, callback),
        m_stopRequested(false),
        m_requestSchema(requestSchema),
        m_requestSender(ocResource) {}

void PUTRequestGenerator::startSending()
{
    // Create thread and start sending requests in dispatched thread
    m_thread.reset(new std::thread(&PUTRequestGenerator::SendAllRequests, this));
    m_thread->detach();
}

void PUTRequestGenerator::stopSending()
{
    m_stopRequested = true;
}

void PUTRequestGenerator::SendAllRequests()
{
    OIC_LOG(DEBUG, TAG, "Sending OP_START event");
    m_callback(m_id, OP_START);

    std::shared_ptr<SimulatorResourceModelSchema> repSchema =
        m_requestSchema->getRequestRepSchema();

    if (!repSchema)
    {
        OIC_LOG(ERROR, TAG, "Request representation model is null!");
        m_callback(m_id, OP_ABORT);
        return;
    }

    SimulatorResourceModel representation = repSchema->buildResourceModel();

    // Create attribute combination generator for generating resource model
    // with different attribute values
    std::vector<SimulatorResourceAttribute> attributes;
    for (auto &attributeElement : representation.getAttributeValues())
    {
        SimulatorResourceAttribute attribute;
        attribute.setName(attributeElement.first);
        attribute.setValue(attributeElement.second);
        attribute.setProperty(repSchema->get(attributeElement.first));
        attributes.push_back(attribute);
    }

    if (!attributes.size())
    {
        OIC_LOG(ERROR, TAG, "Zero attribute found from resource model!");
        m_callback(m_id, OP_COMPLETE);
        return;
    }

    QPGenerator queryParamGen(m_requestSchema->getQueryParams());
    do
    {
        // Get the next possible queryParameter
        std::map<std::string, std::string> queryParam = queryParamGen.next();

        AttributeCombinationGen attrCombGen(attributes);

        // Get the new model from attribute combination generator
        SimulatorResourceModel resModel;
        while (!m_stopRequested && attrCombGen.next(resModel))
        {
            // Send the request
            m_requestSender.send(queryParam, resModel,
                                 std::bind(&PUTRequestGenerator::onResponseReceived, this,
                                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            m_requestCnt++;
        }
    }
    while (!m_stopRequested && queryParamGen.hasNext());

    completed();
}

void PUTRequestGenerator::onResponseReceived(SimulatorResult result,
        const SimulatorResourceModel &repModel, const RequestInfo &reqInfo)
{
    OIC_LOG(DEBUG, TAG, "Response recieved");
    m_responseCnt++;
    completed();
}

void PUTRequestGenerator::completed()
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