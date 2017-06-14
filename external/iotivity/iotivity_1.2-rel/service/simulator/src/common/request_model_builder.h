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

#ifndef SIMULATOR_REQUEST_MODEL_BUILDER_H_
#define SIMULATOR_REQUEST_MODEL_BUILDER_H_

#include "request_model.h"
#include "response_model.h"

namespace RAML
{
    class Raml;
    class RamlResource;
    class Action;
    class Response;
    class RequestResponseBody;
}

class RequestModelBuilder
{
    public:
        std::unordered_map<std::string, RequestModelSP> build(
            const std::shared_ptr<RAML::Raml> &raml, const std::string &uri);
        std::unordered_map<std::string, RequestModelSP> build(
            const std::shared_ptr<RAML::RamlResource> &resource);

    private:
        RequestModelSP createRequestModel(const std::shared_ptr<RAML::Action> &action);
        ResponseModelSP createResponseModel(int code,
                                            const std::shared_ptr<RAML::Response> &response);
        std::shared_ptr<SimulatorResourceModelSchema> createRepSchema(
            const std::shared_ptr<RAML::RequestResponseBody> &rep);
};

#endif

