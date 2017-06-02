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
 * @file   simulator_collection_resource.h
 *
 * @brief   This file provides a class and API to access simulated collection resource.
 */

#ifndef SIMULATOR_COLLECTION_RESOURCE_H_
#define SIMULATOR_COLLECTION_RESOURCE_H_

#include "simulator_resource.h"

/**
 * @class   SimulatorCollectionResource
 * @brief   This class provides a set of APIs for handling simulated collection resource.
 */
class SimulatorCollectionResource : public SimulatorResource
{
    public:

        /**
         * API to get list of resources types which collection supports.
         *
         * @return List of supported resources types.
         */
        virtual std::vector<std::string> getSupportedResources() = 0;

        /**
         * API to add a child resource to collection.
         *
         * @param resource - SimulatorResource shared object.
         */
        virtual void addChildResource(const std::shared_ptr<SimulatorResource> &resource) = 0;

        /**
         * API to remove a child resource from collection.
         *
         * @param resource - SimulatorResource shared object.
         */
        virtual void removeChildResource(const std::shared_ptr<SimulatorResource> &resource) = 0;

        /**
         * API to remove a child resource from collection.
         *
         * @param uri - URI of child resource to be removed.
         */
        virtual void removeChildResource(const std::string &uri) = 0;

        /**
         * API to get child resources of collection.
         *
         * @return List of child resources of collection.
         */
        virtual std::vector<SimulatorResourceSP> getChildResources() = 0;
};

typedef std::shared_ptr<SimulatorCollectionResource> SimulatorCollectionResourceSP;

#endif
