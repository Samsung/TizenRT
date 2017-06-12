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
 * @file   Definitions.cpp
 *
 * @brief   This file provides data Model for Json Schema Definitions.
 */

#include "Definitions.h"

namespace RAML
{
    std::string Definitions::getName() const
    {
        return m_defName;
    }

    void Definitions::setName(const std::string &name)
    {
        m_defName = name;
    }

    std::string Definitions::getType() const
    {
        return m_type;
    }

    void Definitions::setType(const std::string &type)
    {
        m_type = type;
    }

    std::vector<std::string> const &Definitions::getRequiredValues() const
    {
        return m_required;
    }

    void Definitions::setRequiredValue(const std::string &reqValue)
    {
        auto it = m_required.begin();
        for (; it != m_required.end(); ++it)
        {
            if (*it == reqValue)
                break;
        }
        if (m_required.end() == it)
        {
            m_required.push_back(reqValue);
        }
    }

    PropertiesPtr Definitions::getproperty(const std::string &propName )
    {
        if (m_properties.end() != m_properties.find(propName))
        {
            return m_properties[propName];
        }
        return nullptr;
    }

    std::map<std::string, PropertiesPtr> const  &Definitions::getProperties()
    {
        return m_properties;
    }

    void Definitions::addProperty(const std::string &propName, const PropertiesPtr &property)
    {
        if (m_properties.end() == m_properties.find(propName))
        {
            m_properties[propName] =  property;
        }
    }
}
