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

#ifndef RESPONSE_MODEL_H_
#define RESPONSE_MODEL_H_

#include "simulator_resource_model_schema.h"
#include "simulator_error_codes.h"

class RequestModelBuilder;
class ResponseModel
{
    public:
        friend class RequestModelBuilder;

        std::shared_ptr<SimulatorResourceModelSchema> getSchema();
        SimulatorResult verifyResponse(const SimulatorResourceModel &resModel);

    private:
        ResponseModel(int code);
        void setResponseBodyModel(const std::shared_ptr<SimulatorResourceModelSchema> &repSchema);

        int m_code;
        std::shared_ptr<SimulatorResourceModelSchema> m_repSchema;
};

typedef std::shared_ptr<ResponseModel> ResponseModelSP;

#endif
