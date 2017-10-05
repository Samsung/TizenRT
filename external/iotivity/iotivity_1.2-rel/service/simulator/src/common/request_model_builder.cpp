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

#include "request_model_builder.h"
#include "resource_model_schema_builder.h"
#include "logger.h"
#include "Raml.h"

#define TAG "REQ_MODEL_BUILDER"

static std::string getRequestType(RAML::ActionType actionType)
{
    switch (actionType)
    {
        case RAML::ActionType::GET:
            return "GET";
        case RAML::ActionType::PUT:
            return "PUT";
        case RAML::ActionType::POST:
            return "POST";
        case RAML::ActionType::DELETE:
            return "DELETE";
    }

    return ""; // This code should never reach
}

std::unordered_map<std::string, RequestModelSP> RequestModelBuilder::build(
    const std::shared_ptr<RAML::Raml> &raml, const std::string &uri)
{
    std::unordered_map<std::string, RequestModelSP> requestModels;

    if (!raml)
    {
        OIC_LOG(ERROR, TAG, "Raml pointer is null!");
        return requestModels;
    }

    for (auto &resource : raml->getResources())
    {
        // Pick the resource based on the resource uri.
        if (std::string::npos == uri.find((resource.second)->getResourceUri()))
            continue;

        // Construct Request and Response Model from RAML::Action
        for (auto &action :  (resource.second)->getActions())
        {
            RequestModelSP requestModel = createRequestModel(action.second);
            if (requestModel)
            {
                requestModels[requestModel->getType()] = requestModel;
            }
        }

        break;
    }

    return requestModels;
}

std::unordered_map<std::string, RequestModelSP> RequestModelBuilder::build(
    const std::shared_ptr<RAML::RamlResource> &resource)
{
    std::unordered_map<std::string, RequestModelSP> requestModels;

    if (!resource)
    {
        OIC_LOG(ERROR, TAG, "Resource pointer is null!");
        return requestModels;
    }

    // Construct Request and Response Model from RAML::Action
    for (auto &action :  resource->getActions())
    {
        RequestModelSP requestModel = createRequestModel(action.second);
        if (requestModel)
        {
            requestModels[requestModel->getType()] = requestModel;
        }
    }

    return requestModels;
}

RequestModelSP RequestModelBuilder::createRequestModel(const RAML::ActionPtr &action)
{
    // Validate the action type. Only GET, PUT, POST and DELETE are supported.
    RAML::ActionType actionType = action->getType();
    if (actionType != RAML::ActionType::GET
        && actionType != RAML::ActionType::PUT
        && actionType != RAML::ActionType::POST
        && actionType != RAML::ActionType::DELETE)
    {
        OIC_LOG(ERROR, TAG, "Request model is of unknown type!");
        return nullptr;
    }

    RequestModelSP requestModel(new RequestModel(getRequestType(actionType)));

    // Get the allowed query parameters of the request
    for (auto &qpEntry : action->getQueryParameters())
    {
        for (auto &value :  (qpEntry.second)->getEnumeration())
        {
            requestModel->addQueryParam(qpEntry.first, value);
        }
    }

    // Set the request body schema
    RAML::RequestResponseBodyPtr requestBody = action->getRequestBody("application/json");
    requestModel->setRequestBodyModel(createRepSchema(requestBody));

    // Corresponsing responses for this request
    for (auto   &responseEntry :  action->getResponses())
    {
        std::string codeStr = responseEntry.first;
        int code = boost::lexical_cast<int>(codeStr);
        ResponseModelSP responseModel = createResponseModel(code, responseEntry.second);
        if (nullptr != responseModel)
        {
            requestModel->setResponseModel(code, responseModel);
        }
    }

    return requestModel;
}

ResponseModelSP RequestModelBuilder::createResponseModel(int code,
        const RAML::ResponsePtr &response)
{
    ResponseModelSP responseModel(new ResponseModel(code));
    RAML::RequestResponseBodyPtr responseBody = response->getResponseBody("application/json");
    responseModel->setResponseBodyModel(createRepSchema(responseBody));
    return responseModel;
}

std::shared_ptr<SimulatorResourceModelSchema> RequestModelBuilder::createRepSchema(
    const RAML::RequestResponseBodyPtr &responseBody)
{
    if (!responseBody)
    {
        OIC_LOG(ERROR, TAG, "Response body is null!");
        return nullptr;
    }

    return ResourceModelSchemaBuilder(responseBody).build();
}

