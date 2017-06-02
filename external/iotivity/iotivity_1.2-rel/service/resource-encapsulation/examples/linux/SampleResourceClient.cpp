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

#include <iostream>

#include "RCSDiscoveryManager.h"
#include "RCSRepresentation.h"
#include "RCSRemoteResourceObject.h"
#include "RCSResourceAttributes.h"
#include "RCSAddress.h"

#include "OCPlatform.h"

#define DECLARE_MENU(FUNC, ...) { #FUNC, FUNC }

using namespace OC;
using namespace OIC::Service;

struct CloseApp {};

struct MenuItem
{
private:
    typedef void(*Handler)();

public:
    const std::string title;
    const Handler handler;
};

typedef void(*Runner)();

constexpr int RESOURCE_TEMP = 1;
constexpr int RESOURCE_LIGHT = 2;

const std::string RESOURCE_TYPE_TEMP = "oic.r.temperaturesensor";
const std::string RESOURCE_TYPE_LIGHT = "oic.r.light";

RCSRemoteResourceObject::Ptr g_selectedResource;
std::vector<RCSRemoteResourceObject::Ptr> g_discoveredResources;

std::string g_attrKey;

Runner g_currentRun;

std::ostream& operator<<(std::ostream& os, const RCSRemoteResourceObject::Ptr& object)
{
    return os << "\turi : " << object->getUri() << std::endl <<
            "\thost address : " << object->getAddress();
}

std::ostream& operator<<(std::ostream& os, const MenuItem& item)
{
    return os << item.title;
}

void onSelected(const RCSRemoteResourceObject::Ptr& object)
{
    g_selectedResource = object;
}

void onSelected(const MenuItem& item)
{
    std::cout << item.title << " start.." << std::endl;
    item.handler();
}

int processUserInput(int min = std::numeric_limits<int>::min(),
        int max = std::numeric_limits<int>::max())
{
    assert(min <= max);

    int input = 0;

    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!std::cin.fail() && min <= input && input <= max)
    {
        return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

template<typename D>
void displayItem(int width, int index, const D& data)
{
    std::cout.width(width);
    std::cout << std::right << index << ". ";
    std::cout << data << std::endl;
}

template<typename T>
void displayItems(const std::vector<T>& items)
{
    std::cout << std::endl;

    const auto width = (items.size() + 1) / 10 + 1;

    for (size_t i = 0; i < items.size(); ++i)
    {
        displayItem(width, i + 1, items[i]);
    }
    displayItem(width, items.size() + 1, "quit");
}

template<typename T>
void selectItem(const std::vector<T>& items)
{
    int selected = processUserInput(1, items.size() + 1) - 1;

    if (selected == static_cast<int>(items.size()))
    {
        throw CloseApp();
    }

    onSelected(items[selected]);
}

template<typename T>
void handleItems(const std::vector<T>& items)
{
    displayItems(items);
    selectItem(items);
}

std::string inputInterface()
{
    std::string interfaceName;

    std::cout << "\tInput the Interface you want to set : ";
    std::cin >> interfaceName;

    return interfaceName;
}

RCSResourceAttributes inputKeyValue()
{
    std::string key;

    std::cout << "\tEnter the Key you want to set : ";
    std::cin >> key;

    std::cout << "\tEnter the value(INT) you want to set :";
    RCSResourceAttributes attrs;
    attrs[key] = processUserInput();

    return attrs;
}

void printAttribute(const std::string& key, const RCSResourceAttributes::Value& value)
{
    std::cout << "\tkey : " << key << std::endl
              << "\tvalue : " << value.toString() << std::endl;
}

void printAttributes(const RCSResourceAttributes& attributes)
{
    if (attributes.empty())
    {
       std::cout << "\tattributes is empty" << std::endl;
    }

    for (const auto& attr : attributes)
    {
        printAttribute(attr.key(), attr.value());
    }
}

void print(const std::string& name, const std::vector< std::string >& elements)
{
    if (elements.empty())
    {
       std::cout << "\t" << name << " is empty" << std::endl;
       return;
    }

    std::cout << "\t" << name << " : " << std::endl;
    for (const auto& item : elements)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void printUri(const std::string& uri)
{
    if (uri.empty())
    {
        std::cout << "\turi is empty" << std::endl;
    }
    else
    {
        std::cout << "\turi : " << uri << std::endl;
    }
}

void printRepresentation(const RCSRepresentation& rep)
{
    printUri(rep.getUri());
    print("interfaces", rep.getInterfaces());
    print("resourceTypes", rep.getResourceTypes());
    printAttributes(rep.getAttributes());

    const auto& children = rep.getChildren();

    if (children.empty())
    {
        std::cout << "\tchildren is empty" << std::endl;
    }
    else
    {
        int cnt = 0;
        for (const auto& child : children)
        {
            std::cout << "========================================================" << std::endl;
            std::cout << ++cnt << " chlid" << std::endl;
            printRepresentation(child);
            std::cout << "========================================================" << std::endl;
        }
    }
}

void onResourceStateChanged(ResourceState resourceState)
{
    std::cout << "onResourceStateChanged callback" << std::endl;

    switch (resourceState)
    {
        case ResourceState::NONE:
            std::cout << "\tState changed to : NOT_MONITORING" << std::endl;
            break;

        case ResourceState::ALIVE:
            std::cout << "\tState changed to : ALIVE" << std::endl;
            break;

        case ResourceState::REQUESTED:
            std::cout << "\tState changed to : REQUESTED" << std::endl;
            break;

        case ResourceState::LOST_SIGNAL:
            std::cout << "\tState changed to : LOST_SIGNAL" << std::endl;
            break;

        case ResourceState::DESTROYED:
            std::cout << "\tState changed to : DESTROYED" << std::endl;
            break;
    }
}

void onCacheUpdated(const RCSResourceAttributes& attributes)
{
    std::cout << "onCacheUpdated callback" << std::endl;

    printAttributes(attributes);
}

void onRemoteAttributesReceived(const RCSResourceAttributes& attributes, int)
{
    std::cout << "onRemoteAttributesReceived callback" << std::endl;

    printAttributes(attributes);
}

void onRemoteGetReceived(const HeaderOpts&, const RCSRepresentation& rep, int)
{
    std::cout << "onRemoteGetReceived callback" << std::endl;

    printRepresentation(rep);
}

void onRemoteSetReceived(const HeaderOpts&, const RCSRepresentation& rep, int)
{
    std::cout << "onRemoteSetReceived callback" << std::endl;

    printRepresentation(rep);
}

void startMonitoring()
{
    if (g_selectedResource->isMonitoring())
    {
        std::cout << "\tAlready Started..." << std::endl;
        return;
    }

    g_selectedResource->startMonitoring(&onResourceStateChanged);
    std::cout << "\tMonitoring Started..." << std::endl;
}

void stopMonitoring()
{
    if (!g_selectedResource->isMonitoring())
    {
        std::cout << "\tMonitoring not started..." << std::endl;
        return;
    }

    g_selectedResource->stopMonitoring();
    std::cout << "\tMonitoring stopped..." << std::endl;
}

void getRemoteAttributes()
{
    g_selectedResource->getRemoteAttributes(onRemoteAttributesReceived);
}

void setRemoteAttributes()
{
    g_selectedResource->setRemoteAttributes(inputKeyValue(), onRemoteAttributesReceived);
}

void getWithInterface()
{
    RCSQueryParams queryParams;
    queryParams.setResourceInterface(inputInterface());

    g_selectedResource->get(queryParams, onRemoteGetReceived);
}

void setWithInterface()
{
    RCSQueryParams queryParams;
    queryParams.setResourceInterface(inputInterface());

    g_selectedResource->set(queryParams, inputKeyValue(), onRemoteSetReceived);
}

void startCaching(RCSRemoteResourceObject::CacheUpdatedCallback cb)
{
    if (g_selectedResource->isCaching())
    {
        std::cout << "\tAlready Started Caching..." << std::endl;
        return;
    }

    g_selectedResource->startCaching(std::move(cb));
    std::cout << "\tCaching Started..." << std::endl;
}

void startCachingWithoutCallback()
{
    startCaching(nullptr);
}

void startCachingWithCallback()
{
    startCaching(onCacheUpdated);
}

void getResourceCacheState()
{
    switch(g_selectedResource->getCacheState())
    {
        case CacheState::READY:
            std::cout << "\tCurrent Cache State : CACHE_STATE::READY" << std::endl;
            break;

        case CacheState::UNREADY:
            std::cout << "\tCurrent Cache State : CACHE_STATE::UNREADY" << std::endl;
            break;

        case CacheState::LOST_SIGNAL:
            std::cout << "\tCurrent Cache State : CACHE_STATE::LOST_SIGNAL" << std::endl;
            break;

        case CacheState::NONE:
            std::cout << "\tCurrent Cache State : CACHE_STATE::NONE" << std::endl;
            break;
    }
}

void getCachedAttributes()
{
    printAttributes(g_selectedResource->getCachedAttributes());
}

void getCachedAttribute()
{
    printAttribute(g_attrKey, g_selectedResource->getCachedAttribute(g_attrKey));
}

void stopCaching()
{
    if (!g_selectedResource->isCaching())
    {
        std::cout << "\tCaching not started..." << std::endl;
        return;
    }

    g_selectedResource->stopCaching();
    std::cout << "\tCaching stopped..." << std::endl;
}

std::string selectResourceType()
{
    std::cout << "========================================================" << std::endl;
    std::cout << "1. Temperature Resource Discovery" << std::endl;
    std::cout << "2. Light Resource Discovery" << std::endl;
    std::cout << "========================================================" << std::endl;

    switch (processUserInput(RESOURCE_TEMP, RESOURCE_LIGHT))
    {
        case RESOURCE_TEMP:
        {
            g_attrKey = "Temperature";
            return RESOURCE_TYPE_TEMP;
        }
        case RESOURCE_LIGHT:
        {
            g_attrKey = "Brightness";
            return RESOURCE_TYPE_LIGHT;
        }
    }

    throw std::logic_error("unreachable");
}

RCSAddress inputAddress()
{
    std::cout << "========================================================" << std::endl;
    std::cout << "Please input address (empty for multicast)" << std::endl;
    std::cout << "========================================================" << std::endl;

    std::string address;

    if (std::cin.peek() != '\n')
    {
        std::cin >> address;
    }

    return address.empty() ? RCSAddress::multicast() : RCSAddress::unicast(address);
}

void printDiscoveryInProgress()
{
    std::cout << "Discovery in progress, press '1' to stop." << std::endl;
}

void discoverResource()
{
    auto onResourceDiscovered = [](
            const RCSRemoteResourceObject::Ptr& discoveredResource)
    {
        std::cout << "onResourceDiscovered callback :: " << std::endl;

        std::cout << "uri : " << discoveredResource->getUri() << std::endl;
        std::cout << "host address : " << discoveredResource->getAddress() << std::endl;

        g_discoveredResources.push_back(discoveredResource);

        printDiscoveryInProgress();
    };

    auto resourceType = selectResourceType();
    auto address = inputAddress();

    printDiscoveryInProgress();

    auto discoveryTask = RCSDiscoveryManager::getInstance()->discoverResourceByType(address,
            resourceType, onResourceDiscovered);

    while (processUserInput() != 1);

    discoveryTask->cancel();
}

void runResourceControl()
{
    static std::vector<MenuItem> resourceMenuItems {
        DECLARE_MENU(startMonitoring),
        DECLARE_MENU(stopMonitoring),
        DECLARE_MENU(getRemoteAttributes),
        DECLARE_MENU(setRemoteAttributes),
        DECLARE_MENU(getWithInterface),
        DECLARE_MENU(setWithInterface),
        DECLARE_MENU(startCachingWithoutCallback),
        DECLARE_MENU(startCachingWithCallback),
        DECLARE_MENU(getResourceCacheState),
        DECLARE_MENU(getCachedAttributes),
        DECLARE_MENU(getCachedAttribute),
        DECLARE_MENU(stopCaching),
    };

    handleItems(resourceMenuItems);
}

void runResourceSelection()
{
    handleItems(g_discoveredResources);

    g_currentRun = runResourceControl;
}

void runDiscovery()
{
    static std::vector<MenuItem> discoveryMenuItems {
        DECLARE_MENU(discoverResource),
    };

    handleItems(discoveryMenuItems);

    if (g_discoveredResources.empty())
    {
        throw std::runtime_error("No resource found!");
    }

    g_currentRun = runResourceSelection;
}

void configurePlatform()
{
    PlatformConfig config
    {
        OC::ServiceType::InProc, ModeType::Client, "0.0.0.0", 0, OC::QualityOfService::LowQos
    };
    OCPlatform::Configure(config);
}

int main()
{
    configurePlatform();

    g_currentRun = runDiscovery;

    while (true)
    {
        try
        {
            g_currentRun();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const CloseApp&)
        {
            break;
        }
    }

    std::cout << "Stopping the client" << std::endl;

    return 0;
}
