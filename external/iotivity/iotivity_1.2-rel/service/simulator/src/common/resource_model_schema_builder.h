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

#ifndef SIMULATOR_RESOURCE_MODEL_SCHEMA_BUILDER_H_
#define SIMULATOR_RESOURCE_MODEL_SCHEMA_BUILDER_H_

#include "simulator_resource_model_schema.h"

namespace RAML
{
    class RequestResponseBody;
    class Properties;
    class ValueProperty;
    enum class VariantType;
}

class ResourceModelSchemaBuilder
{
    public:
        ResourceModelSchemaBuilder(const std::shared_ptr<RAML::RequestResponseBody> &ramlSchema);
        std::shared_ptr<SimulatorResourceModelSchema> build();

    private:
        std::shared_ptr<AttributeProperty> buildAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<IntegerProperty> buildIntegerAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<DoubleProperty> buildDoubleAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<BooleanProperty> buildBooleanAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<StringProperty> buildStringAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<ArrayProperty> buildArrayAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<ArrayProperty> buildModelArrayAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        std::shared_ptr<ModelProperty> buildModelAttribute(
            const std::shared_ptr<RAML::Properties> &property);

        void setArrayProperties(
            const std::shared_ptr<RAML::Properties> &property,
            const std::shared_ptr<ArrayProperty> &arrayProperty);

        std::shared_ptr<RAML::RequestResponseBody> m_ramlSchema;
};

#endif
