//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "RCSDiscoveryManager.h"

#include "RCSDiscoveryManagerImpl.h"

namespace OIC
{
    namespace Service
    {
        RCSDiscoveryManager::DiscoveryTask::DiscoveryTask(unsigned int id) :
                m_id { id } {}

        RCSDiscoveryManager::DiscoveryTask::~DiscoveryTask() = default;

        bool RCSDiscoveryManager::DiscoveryTask::isCanceled()
        {
            return m_id == RCSDiscoveryManagerImpl::INVALID_ID;
        }

        void RCSDiscoveryManager::DiscoveryTask::cancel()
        {
            if (isCanceled())
                return;

            RCSDiscoveryManagerImpl::getInstance()->cancel(m_id);
            m_id = RCSDiscoveryManagerImpl::INVALID_ID;
        }

        RCSDiscoveryManager* RCSDiscoveryManager::getInstance()
        {
            static RCSDiscoveryManager instance;
            return &instance;
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResource(
                const RCSAddress& address, ResourceDiscoveredCallback cb)
        {
            return discoverResourceByType(address, OC_RSRVD_WELL_KNOWN_URI,
                    RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE, std::move(cb));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResource(
                const RCSAddress& address, const std::string& relativeUri,
                ResourceDiscoveredCallback cb)
        {
            return discoverResourceByType(address, relativeUri,
                    RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE, std::move(cb));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResourceByType(
                const RCSAddress& address, const std::string& resourceType,
                ResourceDiscoveredCallback cb)
        {
            return discoverResourceByType(address, OC_RSRVD_WELL_KNOWN_URI, resourceType,
                    std::move(cb));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResourceByTypes(
                const RCSAddress& address, const std::vector< std::string >& resourceTypes,
                ResourceDiscoveredCallback cb)
        {
            return discoverResourceByTypes(address, OC_RSRVD_WELL_KNOWN_URI, resourceTypes,
                    std::move(cb));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResourceByType(
                const RCSAddress& address, const std::string& relativeUri,
                const std::string& resourceType, ResourceDiscoveredCallback cb)
        {
            return discoverResourceByTypes(address, relativeUri,
                    std::vector< std::string >{ resourceType }, std::move(cb));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManager::discoverResourceByTypes(
                const RCSAddress& address, const std::string& relativeUri,
                const std::vector< std::string >& resourceTypes, ResourceDiscoveredCallback cb)
        {
            return RCSDiscoveryManagerImpl::getInstance()->startDiscovery(address,
                    relativeUri.empty() ? OC_RSRVD_WELL_KNOWN_URI : relativeUri,
                    resourceTypes.empty() ? std::vector< std::string >{
                            RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE } : resourceTypes,
                    std::move(cb));
        }
    }
}
