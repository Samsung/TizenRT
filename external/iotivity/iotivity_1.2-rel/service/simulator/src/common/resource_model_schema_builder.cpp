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

#include "resource_model_schema_builder.h"
#include "simulator_resource_model_schema.h"
#include "Raml.h"
#include "Properties.h"
#include "logger.h"

#define TAG "RES_MODEL_BUILDER"

ResourceModelSchemaBuilder::ResourceModelSchemaBuilder(
    const std::shared_ptr<RAML::RequestResponseBody> &ramlSchema)
    :   m_ramlSchema (ramlSchema) {}

std::shared_ptr<SimulatorResourceModelSchema> ResourceModelSchemaBuilder::build()
{
    if (!m_ramlSchema)
    {
        OIC_LOG(ERROR, TAG, "RAML schema pointer is null!");
        return nullptr;
    }

    std::shared_ptr<SimulatorResourceModelSchema> modelSchema =
        SimulatorResourceModelSchema::build();

    auto jsonSchema = m_ramlSchema->getSchema()->getProperties();
    for (auto &propertyElement : jsonSchema->getProperties())
    {
        auto property = propertyElement.second;
        auto attributeProperty = buildAttribute(property);

        if (!attributeProperty)
            return nullptr;

        modelSchema->add(property->getName(), attributeProperty);
    }

    return modelSchema;
}

std::shared_ptr<AttributeProperty> ResourceModelSchemaBuilder::buildAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    switch (property->getType().type())
    {
        case RAML::VariantType::INTEGER:
            return buildIntegerAttribute(property);

        case RAML::VariantType::DOUBLE:
            return buildDoubleAttribute(property);

        case RAML::VariantType::BOOLEAN:
            return buildBooleanAttribute(property);

        case RAML::VariantType::STRING:
            return buildStringAttribute(property);

        case RAML::VariantType::PROPERTY:
            return buildArrayAttribute(property);

        case RAML::VariantType::ARRAY:
            return buildModelArrayAttribute(property);

        case RAML::VariantType::OBJECT:
            return buildModelAttribute(property);

        default:
            OIC_LOG(ERROR, TAG, "Unknown type!");
            return nullptr;
    }

    return nullptr;
}

std::shared_ptr<IntegerProperty> ResourceModelSchemaBuilder::buildIntegerAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<IntegerProperty> integerProperty = IntegerProperty::build();

    // Set Default value
    if (property->isDefaultValue())
    {
        integerProperty->setDefaultValue(property->getValue<int>());
    }

    // Set the Range/Enum property
    for (auto &valueProperty : property->getValueProperties())
    {
        if (RAML::ValueProperty::Type::RANGE == valueProperty->type())
        {
            integerProperty->setRange(valueProperty->min(), valueProperty->max());
            break;
        }
        else if (RAML::ValueProperty::Type::VALUE_SET == valueProperty->type())
        {
            std::vector<int> values;
            for (auto &value : valueProperty->valueSet())
                values.push_back(boost::get<int>(value));
            integerProperty->setValues(values);
            break;
        }
    }

    return integerProperty;
}

std::shared_ptr<DoubleProperty> ResourceModelSchemaBuilder::buildDoubleAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<DoubleProperty> doubleProperty = DoubleProperty::build();

    // Set Default value
    if (property->isDefaultValue())
    {
        doubleProperty->setDefaultValue(property->getValue<double>());
    }

    // Set the Range/Enum property
    for (auto &valueProperty : property->getValueProperties())
    {
        if (RAML::ValueProperty::Type::RANGE == valueProperty->type())
        {
            doubleProperty->setRange(valueProperty->min(), valueProperty->max());
            break;
        }
        else if (RAML::ValueProperty::Type::VALUE_SET == valueProperty->type())
        {
            std::vector<double> values;
            for (auto &value : valueProperty->valueSet())
                values.push_back(boost::get<double>(value));
            doubleProperty->setValues(values);
            break;
        }
    }

    return doubleProperty;
}

std::shared_ptr<BooleanProperty> ResourceModelSchemaBuilder::buildBooleanAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<BooleanProperty> boolProperty = BooleanProperty::build();

    // Set Default value
    if (property->isDefaultValue())
    {
        boolProperty->setDefaultValue(property->getValue<bool>());
    }

    return boolProperty;
}

std::shared_ptr<StringProperty> ResourceModelSchemaBuilder::buildStringAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<StringProperty> stringProperty = StringProperty::build();

    // Set Default value
    if (property->isDefaultValue())
    {
        stringProperty->setDefaultValue(property->getValue<std::string>());
    }

    // Set the Range/Enum property
    for (auto &valueProperty : property->getValueProperties())
    {
        if (RAML::ValueProperty::Type::RANGE == valueProperty->type())
        {
            stringProperty->setRange(valueProperty->min(), valueProperty->max());
            break;
        }
        else if (RAML::ValueProperty::Type::VALUE_SET == valueProperty->type())
        {
            std::vector<std::string> values;
            for (auto &value : valueProperty->valueSet())
                values.push_back(boost::get<std::string>(value));
            stringProperty->setValues(values);
            break;
        }
    }

    return stringProperty;
}

std::shared_ptr<ArrayProperty> ResourceModelSchemaBuilder::buildArrayAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<ArrayProperty> arrayProperty = ArrayProperty::build();

    auto arrayAttribute = boost::get<RAML::Properties>(property->getValue());
    switch (arrayAttribute.getType().type())
    {
        case RAML::VariantType::INTEGER:
            {
                std::shared_ptr<RAML::Properties> elementAttribute =
                    std::make_shared<RAML::Properties>(arrayAttribute);

                std::shared_ptr<IntegerProperty> elementProperty =
                    buildIntegerAttribute(elementAttribute);

                setArrayProperties(property, arrayProperty);
                arrayProperty->setElementProperty(elementProperty);
            }
            break;

        case RAML::VariantType::DOUBLE :
            {
                std::shared_ptr<RAML::Properties> elementAttribute =
                    std::make_shared<RAML::Properties>(arrayAttribute);

                std::shared_ptr<DoubleProperty> elementProperty =
                    buildDoubleAttribute(elementAttribute);

                setArrayProperties(property, arrayProperty);
                arrayProperty->setElementProperty(elementProperty);
            }
            break;

        case RAML::VariantType::BOOLEAN :
            {
                std::shared_ptr<RAML::Properties> elementAttribute =
                    std::make_shared<RAML::Properties>(arrayAttribute);

                std::shared_ptr<BooleanProperty> elementProperty =
                    buildBooleanAttribute(elementAttribute);

                setArrayProperties(property, arrayProperty);
                arrayProperty->setElementProperty(elementProperty);
            }
            break;

        case RAML::VariantType::STRING :
            {
                std::shared_ptr<RAML::Properties> elementAttribute =
                    std::make_shared<RAML::Properties>(arrayAttribute);

                std::shared_ptr<StringProperty> elementProperty =
                    buildStringAttribute(elementAttribute);

                setArrayProperties(property, arrayProperty);
                arrayProperty->setElementProperty(elementProperty);
            }
            break;

        default:
            OIC_LOG(ERROR, TAG, "Unknown array element type!");
            return nullptr;
    }

    return arrayProperty;
}

std::shared_ptr<ArrayProperty> ResourceModelSchemaBuilder::buildModelArrayAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::vector<RAML::Properties> attributes =
        boost::get<std::vector<RAML::Properties> >(property->getValue());

    std::shared_ptr<ModelProperty> modelProperty = ModelProperty::build();

    for (auto attributeProp : attributes)
    {
        auto elementProperty = std::make_shared<RAML::Properties>(attributeProp);
        modelProperty->add(elementProperty->getName(), buildAttribute(elementProperty));
    }

    std::shared_ptr<ArrayProperty> arrayProperty = ArrayProperty::build();
    setArrayProperties(property, arrayProperty);
    arrayProperty->setElementProperty(modelProperty);

    return arrayProperty;
}

std::shared_ptr<ModelProperty> ResourceModelSchemaBuilder::buildModelAttribute(
    const std::shared_ptr<RAML::Properties> &property)
{
    std::shared_ptr<ModelProperty> modelProperty = ModelProperty::build();

    std::vector<RAML::Properties> attributes =
        boost::get<std::vector<RAML::Properties> >(property->getValue());

    for (auto attributeProp : attributes)
    {
        auto elementProperty = std::make_shared<RAML::Properties>(attributeProp);
        modelProperty->add(elementProperty->getName(), buildAttribute(elementProperty));
    }

    return modelProperty;
}

void ResourceModelSchemaBuilder::setArrayProperties(
    const std::shared_ptr<RAML::Properties> &property,
    const std::shared_ptr<ArrayProperty> &arrayProperty)
{
    for (auto &valueProperty : property->getValueProperties())
    {
        if (RAML::ValueProperty::Type::ARRAY == valueProperty->type())
        {
            int minItems = 0;
            int maxItems = 0;
            bool unique = false;
            bool additionalItems = false;

            valueProperty->valueArray(minItems, maxItems, unique, additionalItems);
            arrayProperty->setRange(minItems, maxItems);
            arrayProperty->setUnique(unique);
            arrayProperty->setVariable(additionalItems);
            break;
        }
    }
}