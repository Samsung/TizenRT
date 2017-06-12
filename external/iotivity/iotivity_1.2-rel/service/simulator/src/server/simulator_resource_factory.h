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

#ifndef SIMULATOR_RESOURCE_FACTORY_H_
#define SIMULATOR_RESOURCE_FACTORY_H_

#include "simulator_single_resource.h"
#include "simulator_collection_resource.h"
#include "RamlParser.h"

namespace RAML
{
    class RamlResource;
    class Properties;
    class JsonSchema;
    class RamlParser;
}

class SimulatorResourceFactory
{
    public:
        /**
         * API for getting singleton instance of SimulatorClient class.
         *
         * @return Singleton instance of SimulatorClient class.
         *
         */
        static SimulatorResourceFactory *getInstance();

        /**
         * API to create resource based on the given RAML file.
         *
         * @param configPath - RAML file path.
         *
         * @return SimulatorResource shared object created using RAML file.
         */
        std::shared_ptr<SimulatorResource> createResource(const std::string &configPath);

        /**
         * API to create resource based on the given RAML file.
         *
         * @param configPath - RAML file path.
         *
         * @return SimulatorResource shared object created using RAML file.
         */
        std::vector<std::shared_ptr<SimulatorResource> > createResource(
            const std::string &configPath, unsigned int count);

        /**
         * API to create simple resource.
         *
         * @param name - Name of resource.
         * @param uri - URI of resource.
         * @param resourceType - ResourceType of resource.
         *
         * @return SimulatorSimpleResource.
         */
        std::shared_ptr<SimulatorSingleResource> createSingleResource(
            const std::string &name, const std::string &uri, const std::string &resourceType);

        /**
         * API to create collection resource.
         *
         * @param name - Name of collection resource.
         * @param uri - URI of resource.
         * @param resourceType - ResourceType of collection resource.
         *
         * @return SimulatorCollectionResource.
         */
        std::shared_ptr<SimulatorCollectionResource> createCollectionResource(
            const std::string &name, const std::string &uri, const std::string &resourceType);

    private:
        std::shared_ptr<SimulatorResource> buildResource(
            const std::shared_ptr<RAML::RamlResource> &ramlResource);

        void addInterfaceFromQueryParameter(
            std::vector<std::string> queryParamValue, std::vector<std::string> &interfaceTypes);

        SimulatorResourceFactory() = default;
        SimulatorResourceFactory(const SimulatorResourceFactory &) = delete;
        SimulatorResourceFactory &operator=(const SimulatorResourceFactory &) = delete;
        SimulatorResourceFactory(SimulatorResourceFactory &&) = delete;
        SimulatorResourceFactory &operator=(SimulatorResourceFactory && ) = delete;
};

class ResourceURIFactory
{
    public:
        /**
         * API for getting singleton instance of SimulatorClient class.
         *
         * @return Singleton instance of SimulatorClient class.
         *
         */
        static ResourceURIFactory *getInstance();

        /**
         * API to construct unique URI from the given base @uri.
         *
         * @param uri - Base uri to be used to construct unique uri.
         *
         * @return Unique uri.
         */
        std::string makeUniqueURI(const std::string &uri);

    private:
        ResourceURIFactory();
        ResourceURIFactory(const ResourceURIFactory &) = delete;
        ResourceURIFactory &operator=(const ResourceURIFactory &) = delete;
        ResourceURIFactory(ResourceURIFactory &&) = delete;
        ResourceURIFactory &operator=(ResourceURIFactory && ) = delete;

        bool isUnique(const std::string &uri);
        void updateUri(const std::string &uri);

        unsigned int m_id;
        std::mutex m_lock;
        std::map<std::string, bool> m_uriList;
};

#endif
