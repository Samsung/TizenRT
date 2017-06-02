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

#include "resource_update_automation.h"
#include "simulator_single_resource_impl.h"
#include "attribute_generator.h"
#include "simulator_exceptions.h"
#include "simulator_logger.h"
#include "logger.h"

#define ATAG "ATTRIBUTE_AUTOMATION"
#define RTAG "RESOURCE_AUTOMATION"

AttributeUpdateAutomation::AttributeUpdateAutomation(
    int id, std::shared_ptr<SimulatorSingleResourceImpl> resource, const std::string &name,
    AutoUpdateType type, int interval,
    const SimulatorSingleResource::AutoUpdateCompleteCallback &callback,
    std::function<void (const int)> finishedCallback)
    :   m_id(id),
        m_attrName(name),
        m_type(type),
        m_updateInterval(interval),
        m_stopRequested(false),
        m_resource(resource),
        m_callback(callback),
        m_finishedCallback(finishedCallback),
        m_thread(nullptr)
{
    if (m_updateInterval < 0)
        m_updateInterval = 0;
}

AttributeUpdateAutomation::~AttributeUpdateAutomation()
{
    if (!m_stopRequested)
        m_thread->detach();
}

void AttributeUpdateAutomation::start()
{
    SimulatorResourceAttribute attribute;
    if (false == m_resource->getAttribute(m_attrName, attribute))
    {
        OIC_LOG(ERROR, ATAG, "Attribute is not present in resource!");
        throw SimulatorException(SIMULATOR_ERROR, "Attribute is not present in resource!");
    }

    m_thread.reset(new std::thread(&AttributeUpdateAutomation::updateAttribute, this, attribute));
}

void AttributeUpdateAutomation::stop()
{
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_stopRequested = true;
    }

    m_condVariable.notify_one();
    if (m_thread)
        m_thread->join();
}

void AttributeUpdateAutomation::updateAttribute(SimulatorResourceAttribute attribute)
{
    std::unique_lock<std::mutex> lock(m_lock);
    std::chrono::system_clock::time_point now;

    AttributeGenerator attributeGen(attribute);
    do
    {
        try
        {
            SimulatorResourceAttribute attribute;
            while (!m_stopRequested && true == attributeGen.next(attribute))
            {
                if (false == m_resource->updateAttributeValue(attribute))
                    break;

                // Wait for interval
                now = std::chrono::system_clock::now();
                m_condVariable.wait_until(lock, now + std::chrono::milliseconds(m_updateInterval),
                                          [this] { return m_stopRequested; });
            }

            attributeGen.reset();
        }
        catch (SimulatorException &e)
        {
            break;
        }
    }
    while (!m_stopRequested && AutoUpdateType::REPEAT == m_type);

    if (!m_stopRequested)
    {
        OIC_LOG_V(DEBUG, ATAG, "Attribute:%s automation is completed!", m_attrName.c_str());
        SIM_LOG(ILogger::INFO, "Attribute automation completed [Name: \"" << m_attrName
                    << "\", id: " << m_id <<"].");
    }
    else
    {
        SIM_LOG(ILogger::INFO, "Attribute automation stopped [Name: \"" << m_attrName
                    << "\", id: " << m_id <<"].");
    }

    // Notify application through callback
    if (m_callback)
        m_callback(m_resource->getURI(), m_id);

    if (m_finishedCallback && !m_stopRequested)
    {
        std::thread notifyManager(m_finishedCallback, m_id);
        notifyManager.detach();
    }
}

ResourceUpdateAutomation::ResourceUpdateAutomation(
    int id, std::shared_ptr<SimulatorSingleResourceImpl> resource, AutoUpdateType type, int interval,
    const SimulatorSingleResource::AutoUpdateCompleteCallback &callback,
    std::function<void (const int)> finishedCallback)
    :   m_id(id),
        m_type(type),
        m_updateInterval(interval),
        m_stopRequested(false),
        m_resource(resource),
        m_callback(callback),
        m_finishedCallback(finishedCallback),
        m_thread(nullptr)
{
    if (m_updateInterval < 0)
        m_updateInterval = 0;
}

ResourceUpdateAutomation::~ResourceUpdateAutomation()
{
    if (!m_stopRequested)
        m_thread->detach();
}

void ResourceUpdateAutomation::start()
{
    std::vector<SimulatorResourceAttribute> attributes;
    for (auto &attributeEntry : m_resource->getAttributes())
    {
        attributes.push_back(attributeEntry.second);
    }

    if (0 == attributes.size())
    {
        OIC_LOG(ERROR, RTAG, "Resource has zero attributes!");
        throw SimulatorException(SIMULATOR_ERROR, "Resource has zero attributes!");
    }

    m_thread.reset(new std::thread(&ResourceUpdateAutomation::updateAttributes, this, attributes));
}

void ResourceUpdateAutomation::stop()
{
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_stopRequested = true;
    }

    m_condVariable.notify_one();
    if (m_thread)
        m_thread->join();
}

void ResourceUpdateAutomation::updateAttributes(
    std::vector<SimulatorResourceAttribute> attributes)
{
    std::unique_lock<std::mutex> lock(m_lock);
    std::chrono::system_clock::time_point now;

    do
    {
        AttributeCombinationGen attrCombGen(attributes);
        SimulatorResourceModel newResModel;
        while (!m_stopRequested && attrCombGen.next(newResModel))
        {
            SimulatorResourceModel updatedResModel;
            m_resource->updateResourceModel(newResModel, updatedResModel);

            // Wait for interval
            now = std::chrono::system_clock::now();
            m_condVariable.wait_until(lock, now + std::chrono::milliseconds(m_updateInterval),
                                      [this] { return m_stopRequested; });
        }
    }
    while (!m_stopRequested && AutoUpdateType::REPEAT == m_type);

    if (!m_stopRequested)
    {
        OIC_LOG_V(DEBUG, RTAG, "Resource update automation complete [id: %d]!", m_id);
        SIM_LOG(ILogger::INFO, "Resource automation completed [URI: \"" << m_resource->getURI()
                << "\", id: " << m_id << "].");
    }
    else
    {
        SIM_LOG(ILogger::INFO, "Resource automation stopped [URI: \"" << m_resource->getURI()
                << "\", id: " << m_id <<"].");
    }

    // Notify application
    if (m_callback)
        m_callback(m_resource->getURI(), m_id);

    if (m_finishedCallback)
    {
        std::thread notifyManager(m_finishedCallback, m_id);
        notifyManager.detach();
    }

}

