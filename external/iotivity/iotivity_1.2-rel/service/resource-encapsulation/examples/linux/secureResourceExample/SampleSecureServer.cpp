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

#include "RCSResourceObject.h"
#include "OCPlatform.h"

using namespace OC::OCPlatform;
using namespace OIC::Service;

struct CloseApp {};

constexpr int RESOURCE_LIGHT = 1;

constexpr int DEFAULT_SERVER = 1;
constexpr int CUSTOM_SERVER = 2;

constexpr int INCREASE = 1;
constexpr int DECREASE = 2;

typedef void (*DisplayControlMenuFunc)();
typedef std::function<void()> Run;

Run g_currentRun;
bool g_isPresenceStarted = false;

RCSResourceObject::Ptr g_resource;

int processUserInput(int min, int max)
{
    assert(min <= max);

    int input;

    std::cin >> input;

    if (!std::cin.fail())
    {
        if (input == max + 1) throw CloseApp();
        if (min <= input) return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

void displayControlLightMenu()
{
    std::cout << "========================================================\n";
    std::cout << INCREASE << ". Increase Brightness by 1 stage            \n";
    std::cout << DECREASE << ". Decrease Brightness by 1 stage            \n";
    std::cout << DECREASE + 1 << ". Quit                                  \n";
    std::cout << "========================================================\n";
}

void printAttributes(const RCSResourceAttributes &attrs)
{
    for (const auto &attr : attrs)
    {
        std::cout << "\tkey : " << attr.key() << "\n\tvalue : "
                  << attr.value().toString() << std::endl;
    }
}

RCSGetResponse requestHandlerForGet(const RCSRequest &, RCSResourceAttributes &attrs)
{
    std::cout << "Received a Get request from Client" << std::endl;
    printAttributes(attrs);

    {
        RCSResourceObject::LockGuard lock(g_resource);
        std::cout << "\nSending response to Client : " << std::endl;
        printAttributes(g_resource->getAttributes());
    }

    return RCSGetResponse::defaultAction();
}

RCSSetResponse requestHandlerForSet(const RCSRequest &, RCSResourceAttributes &attrs)
{
    std::cout << "Received a Set request from Client" << std::endl;
    printAttributes(attrs);

    return RCSSetResponse::defaultAction();
}

void initServer(const std::string &resourceUri, const std::string &resourceType,
                const std::string &attrKey)
{

    g_resource = RCSResourceObject::Builder(resourceUri, resourceType, "oic.if.baseline").
                 setDiscoverable(true).setObservable(true).setSecureFlag(true).build();

    g_resource->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    g_resource->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::NEVER);
    g_resource->setAttribute(attrKey, 0);
}

void updateAttribute(const std::string &attrKey, int control)
{
    const int diff = control == INCREASE ? 1 : - 1;

    {
        RCSResourceObject::LockGuard lock(g_resource);
        auto &attrs = g_resource->getAttributes();
        attrs[attrKey] = attrs[attrKey].get<int>() + diff;
    }

    if (control == INCREASE)
    {
        std::cout << attrKey << " increased." << std::endl;
    }
    else
    {
        std::cout << attrKey << " decreased." << std::endl;
    }
    std::cout << "\nCurrent " << attrKey << ": "
              << g_resource->getAttributeValue(attrKey).get<int>() << std::endl;
}

void runResourceControl(DisplayControlMenuFunc displayMenuFunc, const std::string &attrKey)
{
    displayMenuFunc();
    updateAttribute(attrKey, processUserInput(INCREASE, DECREASE));
}

void runResourceTypeSelection(int resourceMode)
{
    std::cout << "========================================================\n";
    std::cout << RESOURCE_LIGHT << ". Create a Secure Light Resource                              \n";
    std::cout << RESOURCE_LIGHT + 1 << ". Quit                            \n";
    std::cout << "========================================================\n";

    int resourceType = processUserInput(RESOURCE_LIGHT, RESOURCE_LIGHT);
    DisplayControlMenuFunc displayMenuFunc;
    std::string attrKey;

    switch (resourceType)
    {
        case RESOURCE_LIGHT:
            attrKey = "Brightness";
            initServer("/a/light", "oic.r.light", attrKey);

            displayMenuFunc = displayControlLightMenu;
            break;
    }

    if (resourceMode == CUSTOM_SERVER)
    {
        g_resource->setGetRequestHandler(requestHandlerForGet);
        g_resource->setSetRequestHandler(requestHandlerForSet);
    }

    g_currentRun = std::bind(runResourceControl, displayMenuFunc, std::move(attrKey));
}

void runResourceModeSelection()
{
    std::cout << "========================================================          \n";
    std::cout << DEFAULT_SERVER << ". Creation of Secure Resource Without Handlers  \n";
    std::cout << CUSTOM_SERVER << ". Creation of Secure Resource With Set and Get Handlers \n";
    std::cout << CUSTOM_SERVER + 1 << ". Quit                                       \n";
    std::cout << "========================================================          \n";

    g_currentRun = std::bind(runResourceTypeSelection,
                             processUserInput(DEFAULT_SERVER, CUSTOM_SERVER));
}

void runPresenceSelection()
{
    constexpr int PRESENCE_ON = 1;
    constexpr int PRESENCE_OFF = 2;

    std::cout << "========================================================\n";
    std::cout << PRESENCE_ON << ". Presence On                            \n";
    std::cout << PRESENCE_OFF << ". Presence Off                          \n";
    std::cout << PRESENCE_OFF + 1 << ". Quit                              \n";
    std::cout << "========================================================\n";

    if (processUserInput(PRESENCE_ON, PRESENCE_OFF) == PRESENCE_ON)
    {
        g_isPresenceStarted = true;
        startPresence(3);
    }

    g_currentRun = runResourceModeSelection;
}

static FILE *client_open(const char * /*path*/, const char *mode)
{
    return fopen("./oic_svr_db_server.json", mode);
}

int main(void)
{
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    OC::PlatformConfig cfg
    {
        OC::ServiceType::InProc, OC::ModeType::Server, "0.0.0.0", 0,
        OC::QualityOfService::LowQos, &ps
    };

    OC::OCPlatform::Configure(cfg);

    g_currentRun = runPresenceSelection;

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
    std::cout << "Stopping the server" << std::endl;

    g_resource.reset();

    if (g_isPresenceStarted)
    {
        stopPresence();
    }

}

