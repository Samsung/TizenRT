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

#ifndef RESOURCE_UPDATE_AUTOMATION_H_
#define RESOURCE_UPDATE_AUTOMATION_H_

#include <thread>
#include <condition_variable>
#include <atomic>

#include "attribute_generator.h"
#include "simulator_single_resource.h"

class SimulatorSingleResourceImpl;
class AttributeUpdateAutomation
{
    public:
        AttributeUpdateAutomation(int id, std::shared_ptr<SimulatorSingleResourceImpl> resource,
                                  const std::string &name, AutoUpdateType type, int interval,
                                  const SimulatorSingleResource::AutoUpdateCompleteCallback &callback,
                                  std::function<void (const int)> finishedCallback);

        ~AttributeUpdateAutomation();
        void start();
        void stop();

    private:
        void updateAttribute(SimulatorResourceAttribute attribute);

        int m_id;
        std::string m_attrName;
        AutoUpdateType m_type;
        int m_updateInterval;
        bool m_stopRequested;
        std::shared_ptr<SimulatorSingleResourceImpl> m_resource;
        SimulatorSingleResource::AutoUpdateCompleteCallback m_callback;
        std::function<void (const int)> m_finishedCallback;
        std::unique_ptr<std::thread> m_thread;

        std::mutex m_lock;
        std::condition_variable m_condVariable;
};

typedef std::shared_ptr<AttributeUpdateAutomation> AttributeUpdateAutomationSP;

class ResourceUpdateAutomation
{
    public:
        ResourceUpdateAutomation(int id, std::shared_ptr<SimulatorSingleResourceImpl> resource,
                                 AutoUpdateType type, int interval,
                                 const SimulatorSingleResource::AutoUpdateCompleteCallback &callback,
                                 std::function<void (const int)> finishedCallback);

        ~ResourceUpdateAutomation();
        void start();
        void stop();

    private:
        void updateAttributes(std::vector<SimulatorResourceAttribute> attributes);

        int m_id;
        AutoUpdateType m_type;
        int m_updateInterval;
        bool m_stopRequested;
        std::shared_ptr<SimulatorSingleResourceImpl> m_resource;
        SimulatorSingleResource::AutoUpdateCompleteCallback m_callback;
        std::function<void (const int)> m_finishedCallback;
        std::unique_ptr<std::thread> m_thread;

        std::mutex m_lock;
        std::condition_variable m_condVariable;
};

typedef std::shared_ptr<ResourceUpdateAutomation> ResourceUpdateAutomationSP;

#endif
