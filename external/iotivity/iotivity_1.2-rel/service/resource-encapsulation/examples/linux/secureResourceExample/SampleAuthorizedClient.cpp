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

const std::string RESOURCE_TYPE_LIGHT = "oic.r.light";

RCSRemoteResourceObject::Ptr g_selectedResource;
std::vector<RCSRemoteResourceObject::Ptr> g_discoveredResources;

std::string g_attrKey;
Runner g_currentRun;

std::ostream &operator<<(std::ostream &os, const RCSRemoteResourceObject::Ptr &object)
{
    return os << "\turi : " << object->getUri() << std::endl <<
           "\thost address : " << object->getAddress();
}

std::ostream &operator<<(std::ostream &os, const MenuItem &item)
{
    return os << item.title;
}

void onSelected(const RCSRemoteResourceObject::Ptr &object)
{
    g_selectedResource = object;
}

void onSelected(const MenuItem &item)
{
    std::cout << item.title << " start.." << std::endl;
    item.handler();
}

int processUserInput(int min = std::numeric_limits<int>::min(),
                     int max = std::numeric_limits<int>::max())
{
    assert(min <= max);

    int input;

    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!std::cin.fail() && min <= input && input <= max) return input;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

template<typename D>
void displayItem(int width, int index, const D &data)
{
    std::cout.width(width);
    std::cout << std::right << index << ". ";
    std::cout << data << std::endl;
}

template<typename T>
void displayItems(const std::vector<T> &items)
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
void selectItem(const std::vector<T> &items)
{
    int selected = processUserInput(1, items.size() + 1) - 1;

    if (selected == static_cast<int>(items.size())) throw CloseApp();

    onSelected(items[selected]);
}

template<typename T>
void handleItems(const std::vector<T> &items)
{
    displayItems(items);
    selectItem(items);
}

void printAttribute(const std::string &key, const RCSResourceAttributes::Value &value)
{
    std::cout << "\tkey : " << key << std::endl
              << "\tvalue : " << value.toString() << std::endl;
}

void printAttributes(const RCSResourceAttributes &attributes)
{
    if (attributes.empty())
    {
        std::cout << "\tattributes is empty" << std::endl;
    }

    for (const auto &attr : attributes)
    {
        printAttribute(attr.key(), attr.value());
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

void onRemoteAttributesReceived(const RCSResourceAttributes &attributes, int)
{
    std::cout << "onRemoteAttributesReceived callback" << std::endl;
    printAttributes(attributes);
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
    std::string key;

    std::cout << "\tEnter the Key you want to set : ";
    std::cin >> key;

    std::cout << "\tEnter the value(INT) you want to set :";
    RCSResourceAttributes attrs;
    attrs[key] = processUserInput();

    g_selectedResource->setRemoteAttributes(attrs, onRemoteAttributesReceived);
}

RCSAddress inputAddress()
{
    std::cout << "========================================================";
    std::cout << "==================" << std::endl;
    std::cout << "Please enter the address for Unicast Discovery or press enter for multicast" <<
              std::endl;
    std::cout << "========================================================";
    std::cout << "==================" << std::endl;

    std::string address;

    if (std::cin.peek() != '\n') std::cin >> address;

    return address.empty() ? RCSAddress::multicast() : RCSAddress::unicast(address);
}

void printDiscoveryInProgress()
{
    std::cout << "Discovery in progress, press '1' to stop." << std::endl;
}

void discoverResource()
{
    auto onResourceDiscovered = [](
                                    const RCSRemoteResourceObject::Ptr & discoveredResource)
    {
        std::cout << "onResourceDiscovered callback :: " << std::endl;

        std::cout << "\t uri : " << discoveredResource->getUri() << std::endl;
        std::cout << "\t host address : " << discoveredResource->getAddress() << std::endl;

        g_discoveredResources.push_back(discoveredResource);

        printDiscoveryInProgress();
    };

    auto resourceType = RESOURCE_TYPE_LIGHT;
    auto address = inputAddress();

    printDiscoveryInProgress();

    auto discoveryTask = RCSDiscoveryManager::getInstance()->discoverResourceByType(address,
                         resourceType, onResourceDiscovered);

    while (processUserInput() != 1);

    discoveryTask->cancel();
}

void runResourceControl()
{
    static std::vector<MenuItem> resourceMenuItems
    {
        DECLARE_MENU(startMonitoring),
        DECLARE_MENU(stopMonitoring),
        DECLARE_MENU(getRemoteAttributes),
        DECLARE_MENU(setRemoteAttributes),
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
    static std::vector<MenuItem> discoveryMenuItems
    {
        DECLARE_MENU(discoverResource),
    };

    handleItems(discoveryMenuItems);

    if (g_discoveredResources.empty()) throw std::runtime_error("No resource found!");

    g_currentRun = runResourceSelection;
}

static FILE *client_open(const char * /*path*/, const char *mode)
{
    return fopen("./oic_svr_db_client.json", mode);
}

int main()
{

    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    PlatformConfig cfg
    {
        OC::ServiceType::InProc, OC::ModeType::Both, "0.0.0.0", 0,
        OC::QualityOfService::LowQos, &ps
    };

    OCPlatform::Configure(cfg);

    g_currentRun = runDiscovery;

    while (true)
    {
        try
        {
            g_currentRun();
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const CloseApp &)
        {
            break;
        }
    }

    if ((g_selectedResource) && (g_selectedResource->isMonitoring()))
    {
        g_selectedResource->stopMonitoring();
    }

    std::cout << "Stopping the client" << std::endl;

    return 0;
}
