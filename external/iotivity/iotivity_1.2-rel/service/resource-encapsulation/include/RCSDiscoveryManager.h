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
 * This file contains the RCSDiscoveryManager class which provides APIs to discover the
 * Resource in the network
 *
 */

#ifndef RCS_DISCOVERYMANAGER_H_
#define RCS_DISCOVERYMANAGER_H_

#include <memory>
#include <functional>
#include <vector>

namespace OIC
{
    namespace Service
    {
        class RCSAddress;
        class RCSRemoteResourceObject;
        /**
         * This class contains the resource discovery methods.
         *
         * @see RCSRemoteResourceObject
         */
        class RCSDiscoveryManager
        {
        public:

            /**
             * This class represents a discovery task.
             *
             */
            class DiscoveryTask
            {
            public:
                typedef std::unique_ptr< DiscoveryTask > Ptr;

                ~DiscoveryTask();

                DiscoveryTask(const DiscoveryTask&) = delete;
                DiscoveryTask(DiscoveryTask&&) = delete;
                DiscoveryTask& operator =(const DiscoveryTask&) const = delete;
                DiscoveryTask& operator =(DiscoveryTask&&) const = delete;

                /**
                 * It cancels the task of discovery.
                 * If it is already canceled, the operation is ignored.
                 */
                void cancel();

                /**
                 * Returns whether the discovery request is canceled or not.
                 */
                bool isCanceled();

            private:
                explicit DiscoveryTask(unsigned int);

            private:
                unsigned int m_id;

                friend class RCSDiscoveryManagerImpl;
            };

        public:

            /**
             * Typedef for callback of discoverResource APIs
             *
             * @see discoverResource
             */
            typedef std::function< void(std::shared_ptr< RCSRemoteResourceObject >) >
                        ResourceDiscoveredCallback;

            /**
             * @return RCSDiscoveryManager instance.
             *
             */
            static RCSDiscoveryManager* getInstance();

            /**
             * Discovers resources of interest, regardless of uri and resource type.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address         A RCSAddress object
             * @param cb              A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             *
             * @note The callback will be invoked in an internal thread.
             *
             */
            DiscoveryTask::Ptr discoverResource(const RCSAddress& address,
                    ResourceDiscoveredCallback cb);

            /**
             * Discovers resources of interest, regardless of resource type.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address          A RCSAddress object
             * @param relativeUri      The relative uri of resource to be searched
             * @param cb               A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             *
             * @note The callback will be invoked in an internal thread.
             *
             */
            DiscoveryTask::Ptr discoverResource(const RCSAddress& address,
                    const std::string& relativeUri, ResourceDiscoveredCallback cb);

            /**
             * Discovers resources of interest by resource type.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address          A RCSAddress object
             * @param resourceType     Resource Type
             * @param cb               A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             *
             * @note The callback will be invoked in an internal thread.
             *
             */
            DiscoveryTask::Ptr discoverResourceByType(const RCSAddress& address,
                    const std::string& resourceType, ResourceDiscoveredCallback cb);

            /**
             * Discovers resources of interest by resource types.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address          A RCSAddress object
             * @param resourceTypes    List of Resource Types
             * @param cb               A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             * @throws RCSBadRequestException If resourceTypes contain more than one element and
             * any of them is empty.
             *
             * @note The callback will be invoked in an internal thread.
             * @note If resourceTypes is empty, discovers resource by all resource types.
             *
             */
            DiscoveryTask::Ptr discoverResourceByTypes(const RCSAddress& address,
                    const std::vector< std::string >& resourceTypes,
                    ResourceDiscoveredCallback cb);

            /**
             * Discovers resources of interest by a resource type with provided relativeUri.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address          A RCSAddress object
             * @param relativeUri      The relative uri of resource to be searched
             * @param resourceType     Resource Type
             * @param cb               A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             *
             * @note The callback will be invoked in an internal thread.
             *
             */
            DiscoveryTask::Ptr discoverResourceByType(const RCSAddress& address,
                    const std::string& relativeUri, const std::string& resourceType,
                    ResourceDiscoveredCallback cb);

            /**
             * Discovers resources of interest by resource types with provided relativeUri.
             * It will consistently discover resources until the discovery task is canceled.
             *
             * @param address          A RCSAddress object
             * @param relativeUri      The relative uri of resource to be searched
             * @param resourceTypes     List of Resource Types
             * @param cb               A callback to obtain discovered resource
             *
             * @throws InvalidParameterException If cb is empty.
             * @throws RCSBadRequestException If resourceTypes contain more than one element and
             * any of them is empty.
             *
             * @note The callback will be invoked in an internal thread.
             * @note If resourceTypes is empty, discovers resource by all resource types.
             *
             */
            DiscoveryTask::Ptr discoverResourceByTypes(const RCSAddress& address,
                    const std::string& relativeUri,
                    const std::vector< std::string >& resourceTypes,
                    ResourceDiscoveredCallback cb);

        private:

            RCSDiscoveryManager() = default;
            ~RCSDiscoveryManager() = default;
        };
    }
}
#endif // RCSDISCOVERYMANAGER_H
