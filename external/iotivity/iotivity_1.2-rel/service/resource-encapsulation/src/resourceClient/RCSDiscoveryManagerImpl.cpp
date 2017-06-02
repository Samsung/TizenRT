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

#include "RCSDiscoveryManagerImpl.h"

#include "OCPlatform.h"
#include "PresenceSubscriber.h"
#include "RCSAddressDetail.h"
#include "RCSAddress.h"
#include "RCSRemoteResourceObject.h"

namespace
{
    constexpr unsigned int POLLING_INTERVAL_TIME = 60000;

    std::string makeResourceId(const std::shared_ptr< OIC::Service::PrimitiveResource >& resource)
    {
        return resource->getSid() + resource->getUri();
    }

    void validateTypes(const std::vector< std::string >& resourceTypes) {
        if (resourceTypes.size() == 1) return;

        for (const auto& type : resourceTypes)
        {
            if (type == OIC::Service::RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE)
            {
                throw OIC::Service::RCSBadRequestException{
                    "resource types must have no empty string!" };
            }
        }
    }
}

namespace OIC
{
    namespace Service
    {
        constexpr RCSDiscoveryManagerImpl::ID RCSDiscoveryManagerImpl::INVALID_ID;
        constexpr char const* RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE;

        RCSDiscoveryManagerImpl::RCSDiscoveryManagerImpl()
        {
            subscribePresenceWithMulticast();

            m_timer.post(POLLING_INTERVAL_TIME,
                    std::bind(&RCSDiscoveryManagerImpl::onPolling, this));
        }

        RCSDiscoveryManagerImpl* RCSDiscoveryManagerImpl::getInstance()
        {
            static RCSDiscoveryManagerImpl instance;
            return &instance;
        }

        void RCSDiscoveryManagerImpl::onResourceFound(
                std::shared_ptr< PrimitiveResource > resource, ID discoveryId,
                const RCSDiscoveryManager::ResourceDiscoveredCallback& discoverCB)
        {
            {
                std::lock_guard < std::mutex > lock(m_mutex);
                auto it = m_discoveryMap.find(discoveryId);

                if (it == m_discoveryMap.end()) return;
                if (it->second.isKnownResource(resource)) return;

                it->second.addKnownResource(resource);
            }
            discoverCB(std::make_shared < RCSRemoteResourceObject > (resource));
        }

        RCSDiscoveryManager::DiscoveryTask::Ptr RCSDiscoveryManagerImpl::startDiscovery(
                const RCSAddress& address, const std::string& relativeUri,
                const std::vector< std::string >& resourceTypes,
                RCSDiscoveryManager::ResourceDiscoveredCallback cb)
        {
            if (!cb)
            {
                throw RCSInvalidParameterException{ "Callback is empty" };
            }

            validateTypes(resourceTypes);

            const ID discoveryId = createId();

            DiscoveryRequestInfo discoveryInfo(address, relativeUri, resourceTypes,
                    std::bind(&RCSDiscoveryManagerImpl::onResourceFound, this,
                            std::placeholders::_1, discoveryId, std::move(cb)));

            discoveryInfo.discover();

            {
                std::lock_guard < std::mutex > lock(m_mutex);
                m_discoveryMap.insert(std::make_pair(discoveryId, std::move(discoveryInfo)));
            }

            return std::unique_ptr< RCSDiscoveryManager::DiscoveryTask >(
                    new RCSDiscoveryManager::DiscoveryTask(discoveryId));
        }

        void RCSDiscoveryManagerImpl::subscribePresenceWithMulticast()
        {
            using namespace std::placeholders;

            constexpr char MULTICAST_PRESENCE_ADDRESS[] = "coap://" OC_MULTICAST_PREFIX;

            OCDoHandle presenceHandle;
            subscribePresence(presenceHandle, MULTICAST_PRESENCE_ADDRESS,
                    OCConnectivityType::CT_DEFAULT,
                    std::bind(&RCSDiscoveryManagerImpl::onPresence, this, _1, _2, _3));
        }

        void RCSDiscoveryManagerImpl::onPolling()
        {
            {
                std::lock_guard < std::mutex > lock(m_mutex);

                for (const auto& it : m_discoveryMap)
                {
                    it.second.discover();
                }
            }
            m_timer.post(POLLING_INTERVAL_TIME,
                    std::bind(&RCSDiscoveryManagerImpl::onPolling, this));
        }

        void RCSDiscoveryManagerImpl::onPresence(OCStackResult result, const unsigned int /*seq*/,
                const std::string& address)
        {
            if (result != OC_STACK_OK && result != OC_STACK_RESOURCE_CREATED) return;

            std::lock_guard < std::mutex > lock(m_mutex);
            for (const auto& it : m_discoveryMap)
            {
                if (it.second.isMatchedAddress(address))
                {
                    it.second.discover();
                }
            }
        }

        RCSDiscoveryManagerImpl::ID RCSDiscoveryManagerImpl::createId() const
        {
            static ID s_nextId = INVALID_ID + 1;

            std::lock_guard < std::mutex > lock(m_mutex);

            while (s_nextId == INVALID_ID || m_discoveryMap.find(s_nextId) != m_discoveryMap.end())
            {
                ++s_nextId;
            }

            assert(s_nextId != INVALID_ID && "Invalid ID!");

            return s_nextId++;
        }

        void RCSDiscoveryManagerImpl::cancel(ID id)
        {
            std::lock_guard < std::mutex > lock(m_mutex);
            m_discoveryMap.erase(id);
        }

        DiscoveryRequestInfo::DiscoveryRequestInfo(const RCSAddress& address,
                const std::string& relativeUri, const std::vector< std::string >& resourceTypes,
                DiscoverCallback cb) :
                m_address{ address },
                m_relativeUri{ relativeUri },
                m_resourceTypes{ resourceTypes },
                m_knownResourceIds{ },
                m_discoverCb{ std::move(cb) }
        {
            if (m_resourceTypes.empty())
            {
                m_resourceTypes.push_back(RCSDiscoveryManagerImpl::ALL_RESOURCE_TYPE);
            }
        }

        void DiscoveryRequestInfo::discover() const
        {
            for (const auto& it : m_resourceTypes)
            {
                discoverResource(m_address, m_relativeUri + "?rt=" + it, m_discoverCb);
            }
        }

        bool DiscoveryRequestInfo::isKnownResource(
                const std::shared_ptr< PrimitiveResource >& resource) const
        {
            return m_knownResourceIds.find(makeResourceId(resource)) != m_knownResourceIds.end();
        }

        void DiscoveryRequestInfo::addKnownResource(
                const std::shared_ptr< PrimitiveResource >& resource)
        {
            m_knownResourceIds.insert(makeResourceId(resource));
        }

        bool DiscoveryRequestInfo::isMatchedAddress(const std::string& address) const
        {
            return RCSAddressDetail::getDetail(m_address)->isMulticast()
                    || RCSAddressDetail::getDetail(m_address)->getAddress() == address;
        }
    }
}
