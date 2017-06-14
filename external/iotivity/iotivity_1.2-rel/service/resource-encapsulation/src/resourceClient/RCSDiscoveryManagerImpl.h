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

/**
 * @file
 *
 * This file contains the RCSActiveDiscoveryManagerImpl class which provide APIs to discover the Resource in the network
 * and discovery requests management.
 *
 */

#ifndef RCSDISCOVERYMANAGER_IMPL_H
#define RCSDISCOVERYMANAGER_IMPL_H

#include <memory>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "RCSAddress.h"
#include "RCSDiscoveryManager.h"
#include "ExpiryTimer.h"
#include "PrimitiveResource.h"

namespace OIC
{
    namespace Service
    {
        /**
         * The class contains discovery request information
         *
         * @see RCSDiscoveryManager
         */
        class DiscoveryRequestInfo
        {
            public:
                DiscoveryRequestInfo(const RCSAddress&, const std::string&,
                        const std::vector< std::string >&, DiscoverCallback);

            public:
                void discover() const;
                bool isKnownResource(const std::shared_ptr< PrimitiveResource >&) const;
                void addKnownResource(const std::shared_ptr< PrimitiveResource >&);
                bool isMatchedAddress(const std::string&) const;

            private:
                RCSAddress m_address;
                std::string m_relativeUri;
                std::vector< std::string > m_resourceTypes;
                std::unordered_set< std::string > m_knownResourceIds;
                DiscoverCallback m_discoverCb;
        };

        /**
         * The class contains the resource discovery and management requests methods.
         */
        class RCSDiscoveryManagerImpl
        {
            public:

                /*
                 * Typedef for discovery request ID
                 *
                 * @note This is generated for each discovery request
                 */
                typedef unsigned int ID;
                constexpr static char const* ALL_RESOURCE_TYPE = "";

            public:

                static RCSDiscoveryManagerImpl* getInstance();

                /**
                 * Start discovery of resource
                 *
                 * @return DiscoverTask pointer
                 *
                 * @param address        A RCSAddress object
                 * @param relativeURI    The relative uri of resource to be searched
                 * @param resourceType   Resource Type
                 * @param cb             A callback to obtain discovered resource
                 *
                 * @throws InvalidParameterException If cb is empty
                 *
                 * @note If relativeURI is empty, will be discovered after be changed into
                 * "OC_RSRVD_WELL_KNOWN_URI"
                 * @note If resourceType is empty, will be discovered all resources in network
                 *
                 * @see RCSAddress
                 * @see RCSDiscoveryManager
                 */
                RCSDiscoveryManager::DiscoveryTask::Ptr startDiscovery(const RCSAddress& address,
                        const std::string& relativeURI,
                        const std::vector< std::string >& resourceTypes,
                        RCSDiscoveryManager::ResourceDiscoveredCallback cb);

                void cancel(ID);

            private:
                RCSDiscoveryManagerImpl();
                ~RCSDiscoveryManagerImpl() = default;

                void subscribePresenceWithMulticast();

                /**
                 * Check duplicated callback and invoke callback when resource is discovered
                 *
                 * @param resource     A pointer of discovered resource
                 * @param discoverID   The ID of discovery request
                 * @param cb           Callback
                 *
                 * @see PrimitiveResource
                 */
                void onResourceFound(std::shared_ptr< PrimitiveResource > resource, ID discoveryId,
                        const RCSDiscoveryManager::ResourceDiscoveredCallback& cb);

                /**
                 * Discover resource on all requests and posting timer when timer is expired
                 */
                void onPolling();

                /**
                 * Discover resource on all requests when supporting presence function resource
                 * enter into network
                 */
                void onPresence(OCStackResult, const unsigned int seq, const std::string& address);

                /**
                 * Create unique id
                 *
                 * @return Returns the id
                 */
                ID createId() const;

            public:
                constexpr static ID INVALID_ID = 0;

            private:
                ExpiryTimer m_timer;

                std::unordered_map< ID, DiscoveryRequestInfo > m_discoveryMap;

                mutable std::mutex m_mutex;
        };
    }
}
#endif // RCSDISCOVERYMANAGER_IMPL_H
