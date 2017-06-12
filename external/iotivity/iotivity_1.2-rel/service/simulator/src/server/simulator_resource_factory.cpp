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

#include "simulator_resource_factory.h"
#include "simulator_single_resource_impl.h"
#include "simulator_collection_resource_impl.h"
#include "simulator_logger.h"
#include "logger.h"
#include "request_model_builder.h"

#define TAG "SIM_RESOURCE_FACTORY"

SimulatorResourceFactory *SimulatorResourceFactory::getInstance()
{
    static SimulatorResourceFactory s_instance;
    return &s_instance;
}

std::shared_ptr<SimulatorResource> SimulatorResourceFactory::createResource(
    const std::string &configPath)
{
    // Parse the RAML file
    std::shared_ptr<RAML::RamlParser> ramlParser = std::make_shared<RAML::RamlParser>(configPath);
    if (!ramlParser)
    {
        OIC_LOG(ERROR, TAG, "RAML parser returned NULL!");
        return nullptr;
    }

    RAML::RamlPtr raml = ramlParser->getRamlPtr();
    if (!raml)
    {
        OIC_LOG(ERROR, TAG, "RAML pointer is NULL!");
        return nullptr;
    }

    // Get the first resource model from RAML
    RAML::RamlResourcePtr ramlResource;
    if (0 == raml->getResources().size()
        || nullptr == (ramlResource = raml->getResources().begin()->second))
    {
        OIC_LOG(ERROR, TAG, "Zero resources detected from RAML!");
        return nullptr;
    }

    return buildResource(ramlResource);
}

std::vector<std::shared_ptr<SimulatorResource> > SimulatorResourceFactory::createResource(
    const std::string &configPath, unsigned int count)
{
    std::vector<std::shared_ptr<SimulatorResource>> resources;

    // Parse the RAML file
    std::shared_ptr<RAML::RamlParser> ramlParser = std::make_shared<RAML::RamlParser>(configPath);
    if (!ramlParser)
    {
        OIC_LOG(ERROR, TAG, "RAML parser returned NULL!");
        return resources;
    }

    RAML::RamlPtr raml = ramlParser->getRamlPtr();
    if (!raml)
    {
        OIC_LOG(ERROR, TAG, "RAML pointer is NULL!");
        return resources;
    }

    // Get the first resource model from RAML
    RAML::RamlResourcePtr ramlResource;
    if (0 == raml->getResources().size()
        || nullptr == (ramlResource = raml->getResources().begin()->second))
    {
        OIC_LOG(ERROR, TAG, "Zero resources detected from RAML!");
        return resources;
    }

    while (count--)
    {
        std::shared_ptr<SimulatorResource> resource = buildResource(ramlResource);
        if (!resource)
        {
            OIC_LOG(ERROR, TAG, "Failed to create resource!");
            return resources;
        }

        resources.push_back(resource);
    }

    return resources;
}

std::shared_ptr<SimulatorSingleResource> SimulatorResourceFactory::createSingleResource(
    const std::string &name, const std::string &uri, const std::string &resourceType)
{
    SimulatorSingleResourceImpl *simpleResource = new SimulatorSingleResourceImpl();
    simpleResource->setName(name);
    simpleResource->setURI(uri);
    simpleResource->setResourceType(resourceType);
    return std::shared_ptr<SimulatorSingleResource>(simpleResource);
}

std::shared_ptr<SimulatorCollectionResource> SimulatorResourceFactory::createCollectionResource(
    const std::string &name, const std::string &uri, const std::string &resourceType)
{
    SimulatorCollectionResourceImpl *collectionResource = new SimulatorCollectionResourceImpl();
    collectionResource->setName(name);
    collectionResource->setURI(uri);
    collectionResource->setResourceType(resourceType);
    return std::shared_ptr<SimulatorCollectionResource>(collectionResource);
}

std::shared_ptr<SimulatorResource> SimulatorResourceFactory::buildResource(
    const std::shared_ptr<RAML::RamlResource> &ramlResource)
{
    // Build resource request and respone model schema
    RequestModelBuilder requestModelBuilder;
    std::unordered_map<std::string, RequestModelSP> requestModels =
        requestModelBuilder.build(ramlResource);

    // Build SimulatorResourceModel from "GET" response schema
    if (requestModels.end() == requestModels.find("GET"))
    {
        OIC_LOG(ERROR, TAG, "Resource's RAML does not have GET request model!");
        return nullptr;
    }

    RequestModelSP getRequestModel = requestModels["GET"];
    ResponseModelSP getResponseModel = getRequestModel->getResponseModel(200);
    if (!getResponseModel)
    {
        OIC_LOG(ERROR, TAG, "Resource's RAML does not have response for GET request!");
        return nullptr;
    }

    std::shared_ptr<SimulatorResourceModelSchema> responseSchema =
        getResponseModel->getSchema();
    if (!responseSchema)
    {
        OIC_LOG(ERROR, TAG, "Failed to get schema from response model!");
        return nullptr;
    }

    SimulatorResourceModel resourceModel = responseSchema->buildResourceModel();

    // Remove the common properties from  resource Model
    std::string resourceURI = ramlResource->getResourceUri();
    std::string resourceName = ramlResource->getDisplayName();
    std::string resourceType;

    // Extracting resource type.
    if (resourceModel.contains("rt"))
    {
        resourceType = resourceModel.get<std::string>("rt");
        resourceModel.remove("rt");
    }
    else if (resourceModel.contains("resourceType"))
    {
        resourceType = resourceModel.get<std::string>("resourceType");
        resourceModel.remove("resourceType");
    }

    // Extracting interface type.
    std::vector<std::string> interfaceTypes;
    if (resourceModel.contains("if"))
    {
        SimulatorResourceModel::TypeInfo typeInfo = resourceModel.getType("if");
        if(AttributeValueType::STRING == typeInfo.type())
        {
            interfaceTypes.push_back(resourceModel.get<std::string>("if"));
        }
        else if(AttributeValueType::VECTOR == typeInfo.type()
            && AttributeValueType::STRING == typeInfo.baseType()
            && typeInfo.depth() == 1)
        {
            interfaceTypes = resourceModel.get<std::vector<std::string>>("if");
            if (interfaceTypes.size() > 1)
                interfaceTypes.erase(interfaceTypes.begin()+1, interfaceTypes.end());
        }

        resourceModel.remove("if");
    }

    for (auto &requestModel : requestModels)
    {
        if (requestModel.second)
        {
            addInterfaceFromQueryParameter((requestModel.second)->getQueryParams("if"),
                interfaceTypes);
        }
    }

    // Remove properties which are not part of resource representation
    resourceModel.remove("p");
    resourceModel.remove("n");
    resourceModel.remove("id");

    // Create simple/collection resource
    std::shared_ptr<SimulatorResource> simResource;
    if (resourceModel.contains("links"))
    {
        std::shared_ptr<SimulatorCollectionResourceImpl> collectionRes(
            new SimulatorCollectionResourceImpl());

        collectionRes->setName(resourceName);
        if(!resourceType.empty())
            collectionRes->setResourceType(resourceType);
        if (interfaceTypes.size() > 0)
            collectionRes->setInterface(interfaceTypes);
        collectionRes->setURI(ResourceURIFactory::getInstance()->makeUniqueURI(resourceURI));

        // Set the resource model and its schema to simulated resource
        collectionRes->setResourceModel(resourceModel);
        collectionRes->setResourceModelSchema(responseSchema);
        collectionRes->setRequestModel(requestModels);

        simResource = collectionRes;
    }
    else
    {
        std::shared_ptr<SimulatorSingleResourceImpl> singleRes(
            new SimulatorSingleResourceImpl());

        singleRes->setName(resourceName);
        if(!resourceType.empty())
            singleRes->setResourceType(resourceType);
        if (interfaceTypes.size() > 0)
            singleRes->setInterface(interfaceTypes);
        singleRes->setURI(ResourceURIFactory::getInstance()->makeUniqueURI(resourceURI));

        // Set the resource model and its schema to simulated resource
        singleRes->setResourceModel(resourceModel);
        singleRes->setResourceModelSchema(responseSchema);
        singleRes->setRequestModel(requestModels);

        simResource = singleRes;
    }

    return simResource;
}

void SimulatorResourceFactory::addInterfaceFromQueryParameter(
    std::vector<std::string> queryParamValue, std::vector<std::string> &interfaceTypes)
{
    for (auto &interfaceType : queryParamValue)
    {
        if (interfaceTypes.end() ==
            std::find(interfaceTypes.begin(), interfaceTypes.end(), interfaceType))
        {
            interfaceTypes.push_back(interfaceType);
        }
    }
}

ResourceURIFactory *ResourceURIFactory::getInstance()
{
    static ResourceURIFactory s_instance;
    return &s_instance;
}

ResourceURIFactory::ResourceURIFactory()
    : m_id(0) {}

std::string ResourceURIFactory::makeUniqueURI(const std::string &uri)
{
    std::lock_guard<std::mutex> lock(m_lock);
    if (isUnique(uri))
    {
        updateUri(uri);
        return uri;
    }
    std::ostringstream os;
    os << uri;
    if (!uri.empty() && '/' != uri[uri.length() - 1])
        os << '/';
    os << m_id++;
    updateUri(os.str());
    return os.str();
}

void ResourceURIFactory::updateUri(const std::string &uri)
{
    m_uriList.insert(std::pair<std::string, bool>(uri, true));
}

bool ResourceURIFactory::isUnique(const std::string &uri)
{
    if (m_uriList.end() == m_uriList.find(uri))
        return true;
    else
        return false;
}

