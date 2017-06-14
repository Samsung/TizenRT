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

#include <mutex>
#include <condition_variable>

#include "RCSDiscoveryManager.h"
#include "RCSRemoteResourceObject.h"
#include "RCSResourceAttributes.h"
#include "RCSAddress.h"

#include "OCPlatform.h"

using namespace OC;
using namespace OIC::Service;

constexpr int CORRECT_INPUT = 1;
constexpr int INCORRECT_INPUT = 2;
constexpr int QUIT_INPUT = 3;

enum Menu
{
    GET_ATTRIBUTE = 1,
    SET_ATTRIBUTE,
    QUIT,
    END_OF_MENU
};

typedef void(*ClientMenuHandler)();
typedef int ReturnValue;

struct ClientMenu
{
    Menu m_menu;
    ClientMenuHandler m_handler;
    ReturnValue m_result;
};

void getAttributeFromRemoteServer();
void setAttributeToRemoteServer();

ClientMenu clientMenu[] =
{
    {Menu::GET_ATTRIBUTE, getAttributeFromRemoteServer, CORRECT_INPUT},
    {Menu::SET_ATTRIBUTE, setAttributeToRemoteServer, CORRECT_INPUT},
    {Menu::QUIT, [](){}, QUIT_INPUT},
    {Menu::END_OF_MENU, nullptr, INCORRECT_INPUT}
};

const std::string defaultKey = "deviceInfo";
const std::string resourceType = "core.ac";

std::mutex mtx;
std::condition_variable cond;

RCSRemoteResourceObject::Ptr g_resource;

void displayMenu()
{
    std::cout << std::endl;
    std::cout << "1 :: Get Attribute" << std::endl;
    std::cout << "2 :: Set Attribute" << std::endl;
}

void onResourceDiscovered(RCSRemoteResourceObject::Ptr foundResource)
{
    std::cout << "onResourceDiscovered callback" << std::endl;

    std::cout << "\t\tResource URI : " << foundResource->getUri() << std::endl;
    std::cout << "\t\tResource Host : " << foundResource->getAddress() << std::endl;

    g_resource = foundResource;

    cond.notify_all();
}

void onRemoteAttributesReceivedCallback(const RCSResourceAttributes &attributes, int /*eCode*/)
{
    std::cout << "onRemoteAttributesReceivedCallback callback\n" << std::endl;

    if (attributes.empty())
    {
        std::cout << "\tAttribute is Empty" << std::endl;
        return;
    }

    for (const auto & attr : attributes)
    {
        std::cout << "\tkey : " << attr.key() << "\n\tvalue : "
                  << attr.value().toString() << std::endl;
        std::cout << "=============================================\n" << std::endl;

        const auto& doubleVector = attr.value().
                get< std::vector< std::vector< RCSResourceAttributes > > >();

        for (const auto& vector : doubleVector)
        {
            for (const auto& attrs : vector)
            {
                for (const auto & kvPair : attrs)
                {
                    std::cout << "\t" << kvPair.key() << " : "  <<
                            kvPair.value().toString() << std::endl;
                }
            }
            std::cout << std::endl;
        }
        std::cout << "=============================================\n" << std::endl;
    }
    displayMenu();
}

std::vector< std::vector< RCSResourceAttributes > > createNestedAttribute(int speedValue,
        int aircValue)
{
    RCSResourceAttributes model;
    RCSResourceAttributes weight;
    RCSResourceAttributes dimensions;

    model["model"] = "SamsungAC";
    weight["weight"] = 3;
    dimensions["dimensions"] = "10x25x35";

    RCSResourceAttributes speed;
    RCSResourceAttributes airCirculation;

    speed["speed"] = speedValue;
    airCirculation["air"] = aircValue;

    RCSResourceAttributes temperature;
    RCSResourceAttributes humidity;

    temperature["temp"] = 30;
    humidity["humidity"] = 30;

    RCSResourceAttributes power;
    RCSResourceAttributes capacity;

    power["power"] = 1600;
    capacity["capacity"] = 1;

    RCSResourceAttributes red;
    RCSResourceAttributes green;

    red["red"] = 50;
    green["green"] = 60;

    std::vector< RCSResourceAttributes > generalInfo{ model, weight, dimensions };
    std::vector< RCSResourceAttributes > fan{ speed, airCirculation } ;
    std::vector< RCSResourceAttributes > tempSensor{ temperature, humidity } ;
    std::vector< RCSResourceAttributes > efficiency{ power, capacity };
    std::vector< RCSResourceAttributes > light{ red, green };

    std::vector< std::vector< RCSResourceAttributes > > acServer;

    acServer.push_back(generalInfo);
    acServer.push_back(fan);
    acServer.push_back(tempSensor);
    acServer.push_back(efficiency);
    acServer.push_back(light);

    return acServer;
}

int processUserInput()
{
    int userInput = 0;
    std::cin >> userInput;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return userInput;
}

void getAttributeFromRemoteServer()
{
    g_resource->getRemoteAttributes(&onRemoteAttributesReceivedCallback);
}

void setAttributeToRemoteServer()
{
    int speed = 0;
    int airc = 0;

    std::cout << "\tEnter the Fan Speed you want to set : ";
    std::cin >> speed;
    std::cout << "\tEnter the Air circulation value you want to set :";
    std::cin >> airc;

    RCSResourceAttributes setAttribute;
    setAttribute[defaultKey] = createNestedAttribute(speed, airc);

    g_resource->setRemoteAttributes(setAttribute,
                                  &onRemoteAttributesReceivedCallback);
}

int selectClientMenu(int selectedMenu)
{
    for (int i = 0; clientMenu[i].m_menu != Menu::END_OF_MENU; i++)
    {
        if (clientMenu[i].m_menu == selectedMenu)
        {
            clientMenu[i].m_handler();
            return clientMenu[i].m_result;
        }
    }

    std::cout << "Invalid input, please try again" << std::endl;

    return INCORRECT_INPUT;
}

void process()
{
    while (true)
    {
        displayMenu();

        if (selectClientMenu(processUserInput()) == QUIT_INPUT)
        {
            break;
        }
    }
}

void platFormConfigure()
{
    PlatformConfig config
    {
        OC::ServiceType::InProc, ModeType::Client, "0.0.0.0", 0, OC::QualityOfService::LowQos
    };
    OCPlatform::Configure(config);
}

bool discoverResource()
{
    std::cout << "Wait 2 seconds until discovered." << std::endl;

    try
    {
        RCSDiscoveryManager::getInstance()->discoverResourceByType(RCSAddress::multicast(),
                resourceType, &onResourceDiscovered);
    }
    catch (const RCSPlatformException& e)
    {
         std::cout << e.what() << std::endl;
    }
    catch (...)
    {
         std::cout << "unknown exception" << std::endl;
    }
    std::unique_lock<std::mutex> lck(mtx);
    cond.wait_for(lck, std::chrono::seconds(2));

    return g_resource != nullptr;
}

int main()
{
    platFormConfigure();

    if (!discoverResource())
    {
        std::cout << "Can't discovered Server... Exiting the Client." << std::endl;
        return -1;
    }

    try
    {
        process();
    }
    catch (const std::exception &e)
    {
        std::cout << "main exception : " << e.what() << std::endl;
    }

    std::cout << "Stopping the client" << std::endl;

    return 0;
}
