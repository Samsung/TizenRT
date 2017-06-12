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

#include "resource_update_automation_mngr.h"
#include "simulator_single_resource_impl.h"
#include "simulator_exceptions.h"
#include "simulator_logger.h"
#include "logger.h"

#define TAG "UPDATE_AUTOMATION_MNGR"

int UpdateAutomationMngr::startResourceAutomation(
    std::shared_ptr<SimulatorSingleResourceImpl> resource,
    AutoUpdateType type, int interval,
    const SimulatorSingleResource::AutoUpdateCompleteCallback &callback)
{
    ResourceUpdateAutomationSP resourceAutomation(new ResourceUpdateAutomation(
                m_id, resource, type, interval, callback,
                std::bind(&UpdateAutomationMngr::automationCompleted, this, std::placeholders::_1)));

    std::lock_guard<std::mutex> lock(m_lock);
    resourceAutomation->start();

    SIM_LOG(ILogger::INFO, "Resource automation started [URI: \"" << resource->getURI()
            << "\", id: " << m_id << "].");

    m_resourceUpdationList[m_id] = resourceAutomation;
    return m_id++;
}

int UpdateAutomationMngr::startAttributeAutomation(
    std::shared_ptr<SimulatorSingleResourceImpl> resource,
    const std::string &attrName, AutoUpdateType type, int interval,
    const SimulatorSingleResource::AutoUpdateCompleteCallback &callback)
{
    AttributeUpdateAutomationSP attributeAutomation(new AttributeUpdateAutomation(
                m_id, resource, attrName, type, interval, callback,
                std::bind(&UpdateAutomationMngr::automationCompleted, this, std::placeholders::_1)));

    std::lock_guard<std::mutex> lock(m_lock);
    attributeAutomation->start();

    SIM_LOG(ILogger::INFO, "Attribute automation started [Name: \"" << attrName << "\", id: "
            << m_id << "].");

    m_attrUpdationList[m_id] = attributeAutomation;
    return m_id++;
}

std::vector<int> UpdateAutomationMngr::getResourceAutomationIds()
{
    std::vector<int> ids;
    std::lock_guard<std::mutex> lock(m_lock);
    for (auto &automation : m_resourceUpdationList)
        ids.push_back(automation.first);

    return ids;
}

std::vector<int> UpdateAutomationMngr::getAttributeAutomationIds()
{
    std::vector<int> ids;
    std::lock_guard<std::mutex> lock(m_lock);
    for (auto &automation : m_attrUpdationList)
        ids.push_back(automation.first);

    return ids;
}

void UpdateAutomationMngr::stop(int id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (m_resourceUpdationList.end() != m_resourceUpdationList.find(id))
    {
        m_resourceUpdationList[id]->stop();
        m_resourceUpdationList.erase(m_resourceUpdationList.find(id));
        return;
    }
    else if (m_attrUpdationList.end() != m_attrUpdationList.find(id))
    {
        m_attrUpdationList[id]->stop();
        m_attrUpdationList.erase(m_attrUpdationList.find(id));
        return;
    }
}

void UpdateAutomationMngr::stopAll()
{
    std::lock_guard<std::mutex> lock(m_lock);
    std::for_each(m_resourceUpdationList.begin(),
                  m_resourceUpdationList.end(), [] (std::pair<int, ResourceUpdateAutomationSP> element)
    {
        element.second->stop();
    });
    m_resourceUpdationList.clear();

    std::for_each(m_attrUpdationList.begin(),
                  m_attrUpdationList.end(), [] (std::pair<int, AttributeUpdateAutomationSP> element)
    {
        element.second->stop();
    });

    m_attrUpdationList.clear();
}

void UpdateAutomationMngr::automationCompleted(int id)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (m_resourceUpdationList.end() != m_resourceUpdationList.find(id))
    {
        m_resourceUpdationList.erase(m_resourceUpdationList.find(id));
    }
    else if (m_attrUpdationList.end() != m_attrUpdationList.find(id))
    {
        m_attrUpdationList.erase(m_attrUpdationList.find(id));
    }
}

