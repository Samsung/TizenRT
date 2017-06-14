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
#include "RCSRequest.h"
#include "OCPlatform.h"

using namespace OC::OCPlatform;
using namespace OIC::Service;

struct CloseApp{};

constexpr int RESOURCE_TEMP = 1;
constexpr int RESOURCE_LIGHT = 2;

constexpr int DEFAULT_SERVER = 1;
constexpr int CUSTOM_SERVER = 2;

constexpr int INCREASE = 1;
constexpr int DECREASE = 2;

const std::string BASELINE_INTERFACE = "oic.if.baseline";
const std::string ACTUATOR_INTERFACE = "oic.if.a";
const std::string SENSOR_INTERFACE = "oic.if.s";
const std::string CUSTOM_INTERFACE = "test.custom";

typedef void (*DisplayControlMenuFunc)();
typedef std::function<void()> Run;

Run g_currentRun;

bool g_isPresenceStarted = false;

RCSResourceObject::Ptr g_resource;

int processUserInput(int min, int max)
{
    assert(min <= max);

    int input = 0;

    std::cin >> input;

    if (!std::cin.fail())
    {
        if (input == max + 1)
        {
            throw CloseApp();
        }
        if (min <= input && input <= max)
        {
            return input;
        }
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

void displayControlTemperatureMenu()
{
    std::cout << "========================================================\n";
    std::cout << INCREASE << ". Increase Temperature by 1 degree          \n";
    std::cout << DECREASE << ". Decrease Temperature by 1 degree          \n";
    std::cout << DECREASE + 1 << ". Quit                                  \n";
    std::cout << "========================================================\n";
}

void displayControlLightMenu()
{
    std::cout << "========================================================\n";
    std::cout << INCREASE << ". Increase Brightness by 1 stage            \n";
    std::cout << DECREASE << ". Decrease Brightness by 1 stage            \n";
    std::cout << DECREASE + 1 << ". Quit                                  \n";
    std::cout << "========================================================\n";
}

void printAttributes(const RCSResourceAttributes& attrs)
{
    for (const auto& attr : attrs)
    {
        std::cout << "\tkey : " << attr.key() << "\n\tvalue : "
                  << attr.value().toString() << std::endl;
    }
}

RCSGetResponse requestHandlerForGet(const RCSRequest & req, RCSResourceAttributes& attrs)
{
    std::cout << "Received a Get request from Client" << std::endl;
    printAttributes(attrs);

    {
        RCSResourceObject::LockGuard lock(g_resource);
        std::cout << "\nSending response to Client : " << std::endl;
        if (req.getInterface() == CUSTOM_INTERFACE)
        {
            auto attr = g_resource->getAttributes();
            static RCSByteString::DataType binval {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
                                                   0x9, 0x0, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
            attr["blob"] = RCSByteString {binval};
            printAttributes(attr);
            return RCSGetResponse::create(attr);
        }
        else
        {
            printAttributes(g_resource->getAttributes());
            return RCSGetResponse::defaultAction();
        }
    }
}

RCSSetResponse requestHandlerForSet(const RCSRequest&, RCSResourceAttributes& attrs)
{
    std::cout << "Received a Set request from Client" << std::endl;
    printAttributes(attrs);

    return RCSSetResponse::defaultAction();
}

void initServer(const std::string& resourceUri, const std::string& resourceType,
        const std::string& attrKey)
{
    g_resource = RCSResourceObject::Builder(resourceUri, resourceType, ACTUATOR_INTERFACE)
            .addInterface(CUSTOM_INTERFACE)
            .addInterface(SENSOR_INTERFACE)
            .setDefaultInterface(BASELINE_INTERFACE)
            .setDiscoverable(true)
            .setObservable(true)
            .build();

    g_resource->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    g_resource->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::NEVER);
    g_resource->setAttribute(attrKey, 0);
}

void updateAttribute(const std::string& attrKey, int control)
{
    const int diff = control == INCREASE ? 1 : - 1;

    {
        RCSResourceObject::LockGuard lock(g_resource);
        auto& attrs = g_resource->getAttributes();
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

void runResourceControl(DisplayControlMenuFunc displayMenuFunc, const std::string& attrKey)
{
    displayMenuFunc();
    updateAttribute(attrKey, processUserInput(INCREASE, DECREASE));
}

void runResourceTypeSelection(int resourceMode)
{
    std::cout << "========================================================\n";
    std::cout << "Select Resource Type                                    \n";
    std::cout << RESOURCE_TEMP << ". Temperature                          \n";
    std::cout << RESOURCE_LIGHT << ". Light                               \n";
    std::cout << RESOURCE_LIGHT + 1 << ". Quit                            \n";
    std::cout << "========================================================\n";

    int resourceType = processUserInput(RESOURCE_TEMP, RESOURCE_LIGHT);
    DisplayControlMenuFunc displayMenuFunc = nullptr;
    std::string attrKey;

    switch (resourceType)
    {
        case RESOURCE_TEMP:
            attrKey = "Temperature";
            initServer("/a/TempSensor", "oic.r.temperaturesensor", attrKey);

            displayMenuFunc = displayControlTemperatureMenu;
            break;

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
    std::cout << DEFAULT_SERVER << ". Creation of Simple Resource Without Handlers  \n";
    std::cout << CUSTOM_SERVER << ". Creation of Resource With Set and Get Handlers \n";
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

int main(void)
{
    g_currentRun = runPresenceSelection;

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
    std::cout << "Stopping the server" << std::endl;

    g_resource.reset();

    if (g_isPresenceStarted)
    {
        stopPresence();
    }
}

