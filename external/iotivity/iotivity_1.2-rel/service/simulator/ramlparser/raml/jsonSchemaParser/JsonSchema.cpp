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

/**
 * @file   JsonSchema.cpp
 *
 * @brief   This file reads data from Json Schema.
 */

#include "JsonSchema.h"
#include <iostream>
namespace RAML
{

    void JsonSchema::readJson()
    {
        if (! m_cjson)
            return;

        cJSON *jsonId = cJSON_GetObjectItem(m_cjson, "id");
        if (jsonId)
        {
            m_id = jsonId->valuestring;
        }
        cJSON *jsonSchema = cJSON_GetObjectItem(m_cjson, "$schema");
        if (jsonSchema)
        {
            m_schema = jsonSchema->valuestring;
        }
        cJSON *jsonTitle = cJSON_GetObjectItem(m_cjson, "title");
        if (jsonTitle)
        {
            m_title = jsonTitle->valuestring;
        }
        cJSON *jsonType = cJSON_GetObjectItem(m_cjson, "type");
        if (jsonType)
        {
            m_type = jsonType->valuestring;
        }
        cJSON *jsonDescription = cJSON_GetObjectItem(m_cjson, "description");
        if (jsonDescription)
        {
            m_description = jsonDescription->valuestring;
        }
        cJSON *jsonDefinitions = cJSON_GetObjectItem(m_cjson, "definitions");
        if (jsonDefinitions)
        {
            cJSON *childDefinitions = jsonDefinitions->child;
            while (childDefinitions)
            {
                std::string defName = childDefinitions->string;
                addDefinition(defName, readDef(childDefinitions, defName));
                childDefinitions = childDefinitions->next;
            }
        }
        cJSON *jsonProperties = cJSON_GetObjectItem(m_cjson, "properties");
        if (jsonProperties)
        {
            cJSON *childProperties = jsonProperties->child;
            while (childProperties)
            {
                std::string attName = childProperties->string;
                addProperty(attName, readProp(childProperties, attName));
                childProperties = childProperties->next;
            }
        }
        if (m_type == "array")
        {
            PropertiesPtr property = std::make_shared<Properties>("array");
            readArray(m_cjson, property);
            addProperty("array" , property);
        }

        cJSON *jsonAdditionalProperties = cJSON_GetObjectItem(m_cjson, "additionalProperties");
        if (jsonAdditionalProperties)
            m_additionalProperties = jsonAdditionalProperties->type;
        else
            m_additionalProperties = cJSON_True;

        cJSON *jsonReference = cJSON_GetObjectItem(m_cjson, "$ref");
        if (jsonReference)
        {
            JsonParameters param;
            readJsonRef(jsonReference, param);

            for (auto it : param.getProperties())
            {
                addProperty(it.first, it.second);
            }
            for ( auto it : param.getRequired())
            {
                setRequiredValue(it);
            }
            if (m_type.empty())
                m_type = param.getType();
        }
        cJSON *jsonAllOf = cJSON_GetObjectItem(m_cjson, "allOf");
        if (jsonAllOf)
        {
            JsonParameters param;

            readAllOf(jsonAllOf, param);

            for (auto it : param.getProperties())
            {
                addProperty(it.first, it.second);
            }
            for ( auto it : param.getRequired())
            {
                setRequiredValue(it);
            }
            if (m_type.empty())
                m_type = param.getType();
        }
        cJSON *jsonRequiredValues = cJSON_GetObjectItem(m_cjson, "required");
        if (jsonRequiredValues)
        {
            int size = cJSON_GetArraySize(jsonRequiredValues);
            int index = 0;
            do
            {
                setRequiredValue(cJSON_GetArrayItem(jsonRequiredValues, index)->valuestring);
            }
            while ( ++index < size);
        }
    }

    DefinitionsPtr JsonSchema::readDef(cJSON *childDefinitions, const std::string &defName)
    {
        DefinitionsPtr definition = std::make_shared<Definitions>(defName);

        cJSON *defType = cJSON_GetObjectItem(childDefinitions, "type");
        if (defType)
        {
            std::string type = defType->valuestring;
            definition->setType(type);
            if (type == "array")
            {
                PropertiesPtr property = std::make_shared<Properties>("array");
                readArray(childDefinitions, property);
                definition->addProperty("array" , property);
            }
        }
        cJSON *defProperties = cJSON_GetObjectItem(childDefinitions, "properties");
        if (defProperties)
        {
            cJSON *childProperties = defProperties->child;
            while (childProperties)
            {
                std::string attName = childProperties->string;
                definition->addProperty(attName, readProp(childProperties, attName));
                childProperties = childProperties->next;
            }
        }
        cJSON *defRequiredValues = cJSON_GetObjectItem(childDefinitions, "required");
        if (defRequiredValues)
        {
            int size = cJSON_GetArraySize(defRequiredValues);
            int index = 0;
            do
            {
                definition->setRequiredValue(cJSON_GetArrayItem(defRequiredValues, index)->valuestring);
            }
            while ( ++index < size);
        }
        cJSON *defReference = cJSON_GetObjectItem(childDefinitions, "$ref");
        if (defReference)
        {
            JsonParameters param;
            readJsonRef(defReference, param);

            for (auto it : param.getProperties())
            {
                definition->addProperty(it.first, it.second);
            }
            for ( auto it : param.getRequired())
            {
                definition->setRequiredValue(it);
            }
            if (definition->getType().empty())
                definition->setType(param.getType());
        }
        cJSON *defAllOf = cJSON_GetObjectItem(childDefinitions, "allOf");
        if (defAllOf)
        {
            JsonParameters param;
            readAllOf(defAllOf, param);

            for (auto it : param.getProperties())
            {
                definition->addProperty(it.first, it.second);
            }
            for ( auto it : param.getRequired())
            {
                definition->setRequiredValue(it);
            }
            if (definition->getType().empty())
                definition->setType(param.getType());
        }
        return definition;
    }

    PropertiesPtr JsonSchema::readProp(cJSON *childProperties, const std::string &attName )
    {
        PropertiesPtr property = std::make_shared<Properties>(attName);

        cJSON *propertyDescription = cJSON_GetObjectItem(childProperties, "description");
        if (propertyDescription)
        {
            property->setDescription(propertyDescription->valuestring);
        }
        cJSON *propertyType = cJSON_GetObjectItem(childProperties, "type");
        std::string attType;
        if (propertyType)
        {
            if (propertyType->type == 4)
            {
                attType = propertyType->valuestring;
            }
            else if (propertyType->type == 5)
            {
                attType = cJSON_GetArrayItem(propertyType, 0)->valuestring;
            }
        }
        if (!(attType == "array") && !(attType == "object"))
        {
            cJSON *defaultValue = cJSON_GetObjectItem(childProperties, "default");
            if (defaultValue)
            {
                readDefaultValue(defaultValue, property, attType);
            }
        }
        readValues(childProperties, property, attType);
        cJSON *allowedvalues = cJSON_GetObjectItem(childProperties, "enum");
        if (allowedvalues)
        {
            readAllowedValues(allowedvalues, property, attType);
        }
        property->setTypeString(attType);
        return property;
    }

    void JsonSchema::readDefaultValue(cJSON *defaultValue,  PropertiesPtr &property,
                                      const std::string &attType)
    {
        if (defaultValue->type == 4)
        {
            property->setValue((std::string)defaultValue->valuestring);
        }
        else if (defaultValue->type == 3)
        {
            if (attType == "number")
                property->setValue((double)defaultValue->valuedouble);
            else
                property->setValue((int)defaultValue->valueint );
        }
        else if (defaultValue->type == 1)
        {
            property->setValue((bool)true);
        }
        else if (defaultValue->type == 0)
        {
            property->setValue((bool)false);
        }
    }

    void JsonSchema::readAllowedValues(cJSON *allowedvalues,  PropertiesPtr &property,
                                       std::string &attType)
    {
        if ((cJSON_GetArrayItem(allowedvalues, 0)->type) == 4)
        {
            int size = cJSON_GetArraySize(allowedvalues);
            int idx = 0;
            std::vector<std::string> allwdValues;
            do
            {
                allwdValues.push_back(cJSON_GetArrayItem(allowedvalues, idx)->valuestring);
            }
            while ( ++idx < size);
            property->setValueProperty(std::make_shared<ValueProperty>(allwdValues));
            if (attType.empty())
                attType = "string";
        }
        else if ((cJSON_GetArrayItem(allowedvalues, 0)->type) == 3)
        {
            int size = cJSON_GetArraySize(allowedvalues);
            int idx = 0;
            if (attType == "number")
            {
                std::vector<double> allwdValues;
                do
                {
                    allwdValues.push_back(cJSON_GetArrayItem(allowedvalues, idx)->valuedouble);
                }
                while ( ++idx < size);
                property->setValueProperty(std::make_shared<ValueProperty>(allwdValues));
            }
            else
            {
                std::vector<int> allwdValues;
                do
                {
                    allwdValues.push_back(cJSON_GetArrayItem(allowedvalues, idx)->valueint);
                }
                while ( ++idx < size);
                property->setValueProperty(std::make_shared<ValueProperty>(allwdValues));
                if (attType.empty())
                    attType = "integer";
            }
        }
        else if (((cJSON_GetArrayItem(allowedvalues, 0)->type) == 1)
                 || ((cJSON_GetArrayItem(allowedvalues, 0)->type) == 0))
        {
            int size = cJSON_GetArraySize(allowedvalues);
            int idx = 0;
            std::vector<bool> allwdValues;
            do
            {
                if (cJSON_GetArrayItem(allowedvalues, idx)->type)
                    allwdValues.push_back(true);
                else
                    allwdValues.push_back(false);
            }
            while ( ++idx < size);
            property->setValueProperty(std::make_shared<ValueProperty>(allwdValues));
            if (attType.empty())
                attType = "boolean";
        }
    }

    void JsonSchema::readValues(cJSON *childProperties,  PropertiesPtr &property ,
                                const std::string &attType)
    {
        if (attType == "string")
        {
            readString(childProperties, property);
        }
        else if (attType == "integer")
        {
            readInteger(childProperties, property);
        }
        else if (attType == "number")
        {
            readDouble(childProperties, property);
        }
        else if (attType == "array")
        {
            readArray(childProperties, property);
        }
        else if (attType == "object")
        {
            readObject(childProperties, property);
        }
    }

    void JsonSchema::readString(cJSON *childProperties, PropertiesPtr &property)
    {
        cJSON *stringMax = cJSON_GetObjectItem(childProperties, "maxLength");
        int min = INT_MIN, max = INT_MAX;
        if (stringMax)
        {
            cJSON *exclusiveMax = cJSON_GetObjectItem(childProperties, "exclusiveMaximum");
            if (exclusiveMax)
            {
                if (exclusiveMax->type == cJSON_True)
                    max = --(stringMax->valueint);
                else
                    max = stringMax->valueint;
            }
            else
                max = stringMax->valueint;
        }
        cJSON *stringMin = cJSON_GetObjectItem(childProperties, "minLength");
        if (stringMin)
        {
            cJSON *exclusiveMin = cJSON_GetObjectItem(childProperties, "exclusiveMinimum");
            if (exclusiveMin)
            {
                if (exclusiveMin->type == cJSON_True)
                    min = ++(stringMin->valueint);
                else
                    min = stringMin->valueint;
            }
            else
                min = stringMin->valueint;
        }
        if (min != INT_MIN || max != INT_MAX)
            property->setValueProperty(std::make_shared<ValueProperty>(min, max, 0));

        cJSON *stringFormat = cJSON_GetObjectItem(childProperties, "format");
        if (stringFormat)
        {
            property->setValueProperty(std::make_shared<ValueProperty>
                                       (ValueProperty::Type::FORMAT, (stringFormat->valuestring)));
        }

        cJSON *stringPattern = cJSON_GetObjectItem(childProperties, "pattern");
        if (stringPattern)
        {
            property->setValueProperty(std::make_shared<ValueProperty>
                                       (ValueProperty::Type::PATTERN, (stringPattern->valuestring)));
        }
    }

    void JsonSchema::readInteger(cJSON *childProperties,  PropertiesPtr &property)
    {
        cJSON *Max = cJSON_GetObjectItem(childProperties, "maximum");
        int min = INT_MIN, max = INT_MAX, multipleOf = INT_MAX;
        if (Max)
        {
            cJSON *exclusiveMax = cJSON_GetObjectItem(childProperties, "exclusiveMaximum");
            if (exclusiveMax)
            {
                if (exclusiveMax->type == cJSON_True)
                    max = --(Max->valueint);
                else
                    max = Max->valueint;
            }
            else
                max = Max->valueint;
        }
        cJSON *Min = cJSON_GetObjectItem(childProperties, "minimum");
        if (Min)
        {
            cJSON *exclusiveMin = cJSON_GetObjectItem(childProperties, "exclusiveMinimum");
            if (exclusiveMin)
            {
                if (exclusiveMin->type == cJSON_True)
                    min = ++(Min->valueint);
                else
                    min = Min->valueint;
            }
            else
                min = Min->valueint;
        }
        cJSON *MultipleOff = cJSON_GetObjectItem(childProperties, "multipleOf");
        if (MultipleOff)
        {
            multipleOf = MultipleOff->valueint;
        }
        if (min != INT_MIN || max != INT_MAX)
            property->setValueProperty(std::make_shared<ValueProperty>(min, max, multipleOf));

    }

    void JsonSchema::readDouble(cJSON *childProperties,  PropertiesPtr &property)
    {
        cJSON *Max = cJSON_GetObjectItem(childProperties, "maximum");
        double min = INT_MIN, max = INT_MAX;
        int multipleOf = INT_MAX;
        if (Max)
        {
            cJSON *exclusiveMax = cJSON_GetObjectItem(childProperties, "exclusiveMaximum");
            if (exclusiveMax)
            {
                if (exclusiveMax->type == cJSON_True)
                    max = --(Max->valuedouble);
                else
                    max = Max->valuedouble;
            }
            else
                max = Max->valuedouble;
        }
        cJSON *Min = cJSON_GetObjectItem(childProperties, "minimum");
        if (Min)
        {
            cJSON *exclusiveMin = cJSON_GetObjectItem(childProperties, "exclusiveMinimum");
            if (exclusiveMin)
            {
                if (exclusiveMin->type == cJSON_True)
                    min = ++(Min->valuedouble);
                else
                    min = Min->valuedouble;
            }
            else
                min = Min->valuedouble;
        }

        cJSON *MultipleOff = cJSON_GetObjectItem(childProperties, "multipleOf");
        if (MultipleOff)
        {
            multipleOf = MultipleOff->valueint;
        }
        if (min != INT_MIN || max != INT_MAX)
            property->setValueProperty(std::make_shared<ValueProperty>(min, max, multipleOf));

    }

    void JsonSchema::readArray(cJSON *childProperties,  PropertiesPtr &property)
    {
        cJSON *itemValues = cJSON_GetObjectItem(childProperties, "items");
        if (itemValues)
        {
            if (itemValues->type == 5)
            {
                //int item_size = cJSON_GetArraySize(itemValues);
                int item_index = 0;
                //do
                //{
                cJSON *item = cJSON_GetArrayItem(itemValues, item_index);
                readItems(item, property);
                //break;
                //}
                //while ( ++item_index < item_size);
            }
            else
            {
                readItems(itemValues, property);
            }
        }
        cJSON *itemsMax = cJSON_GetObjectItem(childProperties, "maxItems");
        int min = INT_MIN, max = INT_MAX;
        bool unique = cJSON_False, addItems = cJSON_False;
        if (itemsMax)
        {
            cJSON *exclusiveMax = cJSON_GetObjectItem(childProperties, "exclusiveMaximum");
            if (exclusiveMax)
            {
                if (exclusiveMax->type == cJSON_True)
                    max = --(itemsMax->valueint);
                else
                    max = itemsMax->valueint;
            }
            else
                max = itemsMax->valueint;
        }
        cJSON *itemsMin = cJSON_GetObjectItem(childProperties, "minItems");
        if (itemsMin)
        {
            cJSON *exclusiveMin = cJSON_GetObjectItem(childProperties, "exclusiveMinimum");
            if (exclusiveMin)
            {
                if (exclusiveMin->type == cJSON_True)
                    min = ++(itemsMin->valueint);
                else
                    min = itemsMin->valueint;
            }
            else
                min = itemsMin->valueint;
        }
        cJSON *uniqueItems = cJSON_GetObjectItem(childProperties, "uniqueItems");
        if (uniqueItems)
        {
            unique = uniqueItems->type;
        }
        cJSON *additionalItems = cJSON_GetObjectItem(childProperties, "additionalItems");
        if (additionalItems)
        {
            addItems = additionalItems->type;
        }
        property->setValueProperty(std::make_shared<ValueProperty>
                                   (ValueProperty::Type::ARRAY, min, max, unique, addItems));
    }

    void JsonSchema::readItems(cJSON *item, PropertiesPtr &property)
    {
        std::string type;
        JsonParameters param;
        cJSON *itemType = cJSON_GetObjectItem(item, "type");
        if (itemType)
        {
            type = itemType->valuestring;
        }

        cJSON *itemAllOf = cJSON_GetObjectItem(item, "allOf");
        if (itemAllOf)
        {
            readAllOf(itemAllOf , param);
        }
        else
        {
            cJSON *itemOneOf = cJSON_GetObjectItem(item, "oneOf");
            if (itemOneOf)
            {
                readAllOf(itemOneOf , param);
            }
        }

        cJSON *itemReference = cJSON_GetObjectItem(item, "$ref");
        if (itemReference)
        {
            readJsonRef(itemReference , param);
        }

        if (type == "object")
        {
            cJSON *itemProperties = cJSON_GetObjectItem(item, "properties");
            if (itemProperties)
            {
                cJSON *childProperties = itemProperties->child;
                std::vector<Properties> propertyVector;
                while (childProperties)
                {
                    std::string attName = childProperties->string;
                    PropertiesPtr prop = std::make_shared<Properties>(attName);
                    readProp(childProperties, attName);
                    propertyVector.push_back(*prop);
                    childProperties = childProperties->next;
                }
                property->setValue(propertyVector);
            }

            cJSON *itemRequiredValues = cJSON_GetObjectItem(item, "required");
            if (itemRequiredValues)
            {
                int size = cJSON_GetArraySize(itemRequiredValues);
                int index = 0;
                do
                {
                    property->setRequiredValue(cJSON_GetArrayItem(itemRequiredValues, index)->valuestring);
                }
                while ( ++index < size);
            }
        }

        else if (param.getType() == "object")
        {
            std::vector<Properties> propertyVector;
            for (auto prop : param.getProperties())
            {
                propertyVector.push_back(*(prop.second));
            }
            property->setValue(propertyVector);

            for (auto req : param.getRequired())
            {
                property->setRequiredValue(req);
            }
        }
        else
        {
            PropertiesPtr prop = std::make_shared<Properties>("property");

            cJSON *defaultValue = cJSON_GetObjectItem(item, "default");
            if (defaultValue)
            {
                readDefaultValue(defaultValue, prop, type);
            }
            cJSON *allowedvalues = cJSON_GetObjectItem(item, "enum");
            if (allowedvalues)
            {
                readAllowedValues(allowedvalues, prop, type);
            }
            readValues(item, prop, type);
            prop->setTypeString(type);
            property->setValue(*prop);
        }
    }

    void JsonSchema::readObject(cJSON *childProperties,  PropertiesPtr &property)
    {
        property->setTypeString("object");

        cJSON *subProperties = cJSON_GetObjectItem(childProperties, "properties");
        cJSON *itemRequiredValues = cJSON_GetObjectItem(childProperties, "required");
        if (subProperties)
        {
            cJSON *childProperties = subProperties->child;
            std::vector<Properties> propertyVector;
            while (childProperties)
            {
                std::string attName = childProperties->string;
                PropertiesPtr prop = std::make_shared<Properties>(attName);
                readProp(childProperties, attName);
                propertyVector.push_back(*prop);
                childProperties = childProperties->next;
            }
            property->setValue(propertyVector);
            if (itemRequiredValues)
            {
                int size = cJSON_GetArraySize(itemRequiredValues);
                int index = 0;
                do
                {
                    property->setRequiredValue(cJSON_GetArrayItem(itemRequiredValues, index)->valuestring);
                }
                while ( ++index < size);
            }
        }
        else
        {
            JsonParameters param;

            cJSON *itemAllOf = cJSON_GetObjectItem(childProperties, "allOf");
            if (itemAllOf)
            {
                readAllOf(itemAllOf , param);
            }
            cJSON *itemReference = cJSON_GetObjectItem(childProperties, "$ref");
            if (itemReference)
            {
                readJsonRef(itemReference , param);
            }

            if (param.getType() == "object")
            {
                std::vector<Properties> propertyVector;
                for (auto prop : param.getProperties())
                {
                    propertyVector.push_back(*(prop.second));
                }
                property->setValue(propertyVector);

                for (auto req : param.getRequired())
                {
                    property->setRequiredValue(req);
                }
            }
        }
    }

    void JsonSchema::readFile(std::string &fileName ,  JsonParameters &param)
    {
        std::string name = fileName;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.compare("oic.baseresource.json") == 0)
            return;

        cJSON *json = m_includeResolver->readToJson(fileName);
        JsonSchemaPtr Refparser = std::make_shared<JsonSchema>(json, m_includeResolver);

        param.addProperties(Refparser->getProperties());
        param.addRequired(Refparser->getRequiredValues());
        param.setType(Refparser->getType());
    }

    void JsonSchema::readFile(std::string &fileName , std::string &defName ,  JsonParameters &param)
    {
        std::string name = fileName;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.compare("oic.baseresource.json") == 0)
            return;

        cJSON *json = m_includeResolver->readToJson(fileName);
        JsonSchemaPtr Refparser = std::make_shared<JsonSchema>(json, m_includeResolver);

        DefinitionsPtr definition = Refparser->getDefinition(defName);
        if (definition == nullptr)
            throw JsonException("Definition Name Incorrect");

        param.addProperties(definition->getProperties());
        param.addRequired(definition->getRequiredValues());
        param.setType(definition->getType());
    }

    void JsonSchema::readRef(std::string ref ,  JsonParameters &param)
    {
        std::string delimiter1 = "#";
        std::string delimiter2 = "/";
        std::string fileName;
        if (! ref.empty())
        {
            std::size_t pos = std::string::npos;
            if ( (pos = ref.find(delimiter1)) != std::string::npos)
            {
                fileName = ref.substr(0, pos);
                ref.erase(0, pos);
            }
            ref.erase(0, delimiter1 .length());
            std::string defName;

            if (! ref.empty())
            {
                ref.erase(0, delimiter2 .length());
                std::string keyName;
                if ( (pos = ref.find(delimiter2)) != std::string::npos)
                {
                    keyName = ref.substr(0, pos);
                    ref.erase(0, pos + delimiter2.length());
                    if (keyName == "definitions")
                    {
                        if ( (pos = ref.find(delimiter2)) != std::string::npos)
                        {
                            defName = ref.substr(0, pos);
                        }
                        else if (! ref.empty())
                        {
                            defName = ref;
                        }
                    }
                }
            }
            if (!fileName.empty())
            {
                if (!(defName.empty()))
                {
                    readFile(fileName, defName, param);
                }
                else
                {
                    throw JsonException("Definition Name Empty");
                }
            }
            else
            {
                if (!(defName.empty()))
                {
                    if (getDefinition(defName) == nullptr)
                        throw JsonException("Definition Name Incorrect");
                    param.addProperties(getDefinition(defName)->getProperties());
                    param.addRequired(getDefinition(defName)->getRequiredValues());
                    param.setType(getDefinition(defName)->getType());
                }
                else
                {
                    throw JsonException("Definition Name Empty");
                }
            }
        }
    }

    void JsonSchema::readJsonRef(cJSON *jsonReference , JsonParameters &param)
    {
        std::string ref = jsonReference->valuestring;

        std::string web = "http://";
        std::string delimiter = "#";
        std::size_t pos = ref.find(web);

        if (pos == std::string::npos)   // If Web Link Is GIVEN TO READ
        {
            pos = ref.find(delimiter);
            if ( pos ==  (ref.length() - 1) )
            {
                std::string fileName = ref.substr(0, pos);
                readFile(fileName, param);
            }
            else
            {
                readRef(ref, param);
            }
        }
    }

    void JsonSchema::readAllOf(cJSON *allofValues ,  JsonParameters &allParams)
    {
        int size = cJSON_GetArraySize(allofValues);
        int index = 0;
        do
        {
            JsonParameters param;

            cJSON *childAllOf = cJSON_GetArrayItem(allofValues, index);
            cJSON *jsonReference = cJSON_GetObjectItem(childAllOf, "$ref");
            if (jsonReference)
            {
                readJsonRef(jsonReference, param);
                allParams.addProperties(param.getProperties());
                allParams.addRequired(param.getRequired());
                allParams.setType(param.getType());
            }
            cJSON *jsonRequiredValues = cJSON_GetObjectItem(childAllOf, "required");
            if (jsonRequiredValues)
            {
                int len = cJSON_GetArraySize(jsonRequiredValues);
                int idx = 0;
                do
                {
                    allParams.addRequired(cJSON_GetArrayItem(jsonRequiredValues, idx)->valuestring);
                }
                while ( ++idx < len);
            }
        }
        while ( ++index < size);
    }
}

