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

#include "request_model.h"
#include "logger.h"

RequestModel::RequestModel(const std::string &type)
    : m_type(type) {}

std::string RequestModel::getType() const
{
    return m_type;
}

SupportedQueryParams RequestModel::getQueryParams()
{
    return m_queryParams;
}

std::vector<std::string> RequestModel::getQueryParams(const std::string &key)
{
    if (m_queryParams.end() != m_queryParams.find(key))
    {
        return m_queryParams[key];
    }

    return std::vector<std::string>();
}

std::shared_ptr<SimulatorResourceModelSchema> RequestModel::getRequestRepSchema()
{
    return m_repSchema;
}

ResponseModelSP RequestModel::getResponseModel(int code)
{
    if (m_responseList.end() != m_responseList.find(code))
        return m_responseList[code];

    return nullptr;
}

void RequestModel::setType(const std::string &type)
{
    m_type = type;
}

void RequestModel::setQueryParams(const std::string &key,
                                  const std::vector<std::string> &values)
{
    if (0 != values.size())
        m_queryParams[key] = values;
}

void RequestModel::addQueryParam(const std::string &key, const std::string &value)
{
    m_queryParams[key].push_back(value);
}

void RequestModel::setResponseModel(int code, ResponseModelSP &responseModel)
{
    if (responseModel)
        m_responseList[code] = responseModel;
}

void RequestModel::setRequestBodyModel(const std::shared_ptr<SimulatorResourceModelSchema>
                                       &repSchema)
{
    m_repSchema = repSchema;
}

SimulatorResult RequestModel::validateResponse(int code, const SimulatorResourceModel &resModel)
{
    if (m_responseList.end() == m_responseList.find(code))
    {
        return SIMULATOR_INVALID_RESPONSE_CODE;
    }

    return m_responseList[code]->verifyResponse(resModel);
}

