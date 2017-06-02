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

#ifndef SIMULATOR_REQUEST_MODEL_H_
#define SIMULATOR_REQUEST_MODEL_H_

#include "simulator_client_types.h"
#include "simulator_resource_model_schema.h"

class SimulatorRequestModel
{
    public:
        SimulatorRequestModel();
        SimulatorRequestModel(RequestType type);

        RequestType getType() const;
        std::map<std::string, std::vector<std::string>> getQueryParams() const;
        std::shared_ptr<SimulatorResourceModelSchema> getRequestBodySchema() const;

        void setQueryParams(const std::map<std::string, std::vector<std::string>> &queryParams);
        void setRequestBodySchema(const std::shared_ptr<SimulatorResourceModelSchema> &repSchema);

    private:
        RequestType m_type;
        std::map<std::string, std::vector<std::string>> m_queryParams;
        std::shared_ptr<SimulatorResourceModelSchema> m_reqBodySchema;
};

#endif
