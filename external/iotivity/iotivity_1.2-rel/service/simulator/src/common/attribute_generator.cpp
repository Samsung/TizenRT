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

#include "attribute_generator.h"

AttributeGenerator::AttributeGenerator(const SimulatorResourceAttribute &attribute)
{
    m_name = attribute.getName();
    if (attribute.getProperty())
    {
        m_valueGen = AttributeValueGenFactory::create(attribute.getProperty());
    }
}

AttributeGenerator::AttributeGenerator(const std::string &name,
                                       const std::shared_ptr<AttributeProperty> &property)
    :   m_name(name)
{
    if (property)
    {
        m_valueGen = AttributeValueGenFactory::create(property);
    }
}

bool AttributeGenerator::hasNext()
{
    if (m_valueGen)
        return m_valueGen->hasNext();
    return false;
}

bool AttributeGenerator::next(SimulatorResourceAttribute &attribute)
{
    if (!hasNext())
        return false;

    if (m_valueGen)
    {
        attribute.setName(m_name);
        attribute.setValue(m_valueGen->next());
        return true;
    }

    return false;
}

SimulatorResourceAttribute AttributeGenerator::current()
{
    if (m_valueGen)
    {
        return SimulatorResourceAttribute(m_name, m_valueGen->value());
    }

    return SimulatorResourceAttribute(m_name);
}

void AttributeGenerator::reset()
{
    if (m_valueGen)
        m_valueGen->reset();
}

AttributeCombinationGen::AttributeCombinationGen(
    const std::vector<SimulatorResourceAttribute> &attributes)
{
    for (auto &attr : attributes)
    {
        AttributeGenerator attrGen(attr);
        m_attrGenList.push_back(attr);
        m_resModel.add(attr.getName(), attr.getValue());
    }

    m_index = -1;
}

bool AttributeCombinationGen::next(SimulatorResourceModel &resModel)
{
    if (!m_attrGenList.size())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_lock);

    // This block will execute for only first time
    if (-1 == m_index)
    {
        for (size_t index = 0; index < m_attrGenList.size(); index++)
        {
            // Add the attribute on resource model
            updateAttributeInModel(index);
        }

        m_index = m_attrGenList.size() - 1;
        resModel = m_resModel;
        return true;
    }

    // Get the next attribute from statck top element
    if (m_attrGenList[m_index].hasNext())
    {
        updateAttributeInModel(m_index);
        resModel = m_resModel;
    }
    else
    {
        for (int index = m_index; index >= 0; index--)
        {
            if (!m_attrGenList[index].hasNext())
            {
                if (!index)
                    return false;

                m_attrGenList[index].reset();
                updateAttributeInModel(index);
            }
            else
            {
                updateAttributeInModel(index);
                break;
            }
        }

        resModel = m_resModel;
    }

    return true;
}

void AttributeCombinationGen::updateAttributeInModel(int index)
{
    SimulatorResourceAttribute attribute;
    if (m_attrGenList[index].next(attribute))
        m_resModel.update(attribute.getName(), attribute.getValue());
}
