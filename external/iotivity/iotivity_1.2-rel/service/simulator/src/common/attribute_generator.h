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

#ifndef SIMULATOR_ATTRIBUTE_GENERATOR_H_
#define SIMULATOR_ATTRIBUTE_GENERATOR_H_

#include <vector>
#include "simulator_resource_model.h"
#include "attribute_value_generator.h"

class AttributeGenerator
{
    public:
        AttributeGenerator(const SimulatorResourceAttribute &attribute);
        AttributeGenerator(const std::string &name,
                           const std::shared_ptr<AttributeProperty> &property);

        bool hasNext();
        bool next(SimulatorResourceAttribute &attribute);
        SimulatorResourceAttribute current();
        void reset();

    private:
        std::string m_name;
        std::unique_ptr<AttributeValueGen> m_valueGen;
};

class AttributeCombinationGen
{
    public:
        AttributeCombinationGen(const std::vector<SimulatorResourceAttribute> &attributes);
        bool next(SimulatorResourceModel &resModel);

    private:
        void updateAttributeInModel(int index);

        std::mutex m_lock;
        std::vector<AttributeGenerator> m_attrGenList;
        int m_index;
        SimulatorResourceModel m_resModel;
};

#endif

