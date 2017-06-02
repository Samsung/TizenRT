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
#include "simulator_request_model.h"

SimulatorRequestModel::SimulatorRequestModel()
    : m_type(RequestType::RQ_TYPE_UNKNOWN),
        m_reqBodySchema(nullptr) {}

SimulatorRequestModel::SimulatorRequestModel(RequestType type)
    : m_type(type) {}

RequestType SimulatorRequestModel::getType() const
{
    return m_type;
}

std::map<std::string, std::vector<std::string>> SimulatorRequestModel::getQueryParams() const
{
    return m_queryParams;
}

std::shared_ptr<SimulatorResourceModelSchema> SimulatorRequestModel::getRequestBodySchema() const
{
    return m_reqBodySchema;
}

void SimulatorRequestModel::setQueryParams(
    const std::map<std::string, std::vector<std::string>> &queryParams)
{
    m_queryParams = queryParams;
}

void SimulatorRequestModel::setRequestBodySchema(
    const std::shared_ptr<SimulatorResourceModelSchema> &repSchema)
{
    m_reqBodySchema = repSchema;
}

