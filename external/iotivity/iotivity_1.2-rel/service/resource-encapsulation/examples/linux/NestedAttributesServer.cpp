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

#include <iostream>

#include "RCSResourceObject.h"

using namespace OIC::Service;

constexpr int DEFAULT_SPEED = 30;
constexpr int UP_SPEED = 50;
constexpr int DOWN_SPEED = 10;

constexpr int DEFAULT_SERVER = 1;
constexpr int CUSTOM_SERVER = 2;
constexpr int STOP = 3;

constexpr int PRINT_ATTRIBUTES = 1;
constexpr int INCREASE_SPEED_ATTRIBUTE = 2;
constexpr int DECREASE_SPEED_ATTRIBUTE = 3;
constexpr int STOP_SENSOR = 4;

constexpr int CORRECT_INPUT = 1;
constexpr int INCORRECT_INPUT = 2;
constexpr int QUIT = 3;

const std::string resourceUri = "/a/airConditioner";
const std::string resourceType = "core.ac";
const std::string resourceInterface = "oic.if.baseline";
const std::string attributeKey = "deviceInfo";

RCSResourceObject::Ptr g_server;

void displayMenu()
{
    std::cout << "====================================================================="
              << std::endl;
    std::cout << "   1 - Creation of Resource [Auto control for requests]" << std::endl;
    std::cout << "   2 - Creation of Resource [Developer control for Get and Set requests]"
              << std::endl;
    std::cout << "   3 - Quit" << std::endl;
    std::cout << "====================================================================="
              << std::endl;
}

void displayControlMenu()
{
    std::cout << "========================================================" << std::endl;
    std::cout << "1. Print Nested attributes" << std::endl;
    std::cout << "2. Increase Speed attributes" << std::endl;
    std::cout << "3. Decrease Speed attributes" << std::endl;
    std::cout << "4. Stop the Sensor" << std::endl;
    std::cout << "========================================================" << std::endl;
}

std::vector< std::vector< RCSResourceAttributes > > createNestedAttribute(int speedValue)
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
    airCirculation["air"] = 425;

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
    std::vector< RCSResourceAttributes > fan{ speed, airCirculation };
    std::vector< RCSResourceAttributes > tempSensor{ temperature, humidity };
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

void printAttribute(const RCSResourceAttributes &attrs)
{
    for (const auto & attr : attrs)
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
}

void printNestedAttribute()
{
    RCSResourceObject::LockGuard lock(*g_server);
    const auto& attributes = g_server->getAttributes();

    std::cout << "\nPrinting nested attributes" << std::endl;
    printAttribute(attributes);
    return;
}

void changeSpeedAttribute(int state)
{
    std::vector< std::vector< RCSResourceAttributes > > attr;

    if (INCREASE_SPEED_ATTRIBUTE == state)
    {
        std::cout << "Increasing speed  attribute to : " << UP_SPEED  <<  std::endl;
        attr = createNestedAttribute(UP_SPEED);
    }
    else if (DECREASE_SPEED_ATTRIBUTE == state)
    {
        std::cout << "Decreasing speed  attribute to : " << DOWN_SPEED << std::endl;
        attr = createNestedAttribute(DOWN_SPEED);
    }

    RCSResourceObject::LockGuard lock(*g_server);
    g_server->getAttributes()[attributeKey] = attr;
    printNestedAttribute();
}

//hander for get request (if developer choose second option for resource Creation)
RCSGetResponse requestHandlerForGet(const RCSRequest& /*request*/,
                                    RCSResourceAttributes& /*attrs*/)
{
    std::cout << "Recieved a Get request from Client" << std::endl;

    RCSResourceObject::LockGuard lock(*g_server);
    std::cout << "Sending response to Client : " << std::endl;
    printAttribute(g_server->getAttributes());

    return RCSGetResponse::defaultAction();
}

//hander for set request (if developer choose second option for resource Creation)
RCSSetResponse requestHandlerForSet(const RCSRequest& /*request*/,
                                    RCSResourceAttributes &attrs)
{
    std::cout << "Recieved a Set request from Client" << std::endl;

    std::cout << "Requested attributes : " << std::endl;
    printAttribute(attrs);
    return RCSSetResponse::defaultAction();
}

void initServer()
{
    try
    {
        g_server = RCSResourceObject::Builder(resourceUri, resourceType,
                                            resourceInterface).build();
    }
    catch (const RCSPlatformException &e)
    {
        std::cout << "Exception in initServer : " << e.what() << std::endl;
        return;
    }

    g_server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    g_server->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::NEVER);

    g_server->setAttribute(attributeKey, createNestedAttribute(DEFAULT_SPEED));
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

int selectControlMenu()
{
    switch (processUserInput())
    {
        case PRINT_ATTRIBUTES:
            printNestedAttribute();
            return CORRECT_INPUT;

        case INCREASE_SPEED_ATTRIBUTE:
            changeSpeedAttribute(INCREASE_SPEED_ATTRIBUTE);
            return CORRECT_INPUT;

        case DECREASE_SPEED_ATTRIBUTE:
            changeSpeedAttribute(DECREASE_SPEED_ATTRIBUTE);
            return CORRECT_INPUT;

        case STOP_SENSOR:
            return QUIT;

        default:
            std::cout << "Invalid input. Please try again." << std::endl;
            return INCORRECT_INPUT;
    }
}

int selectServerMenu()
{
    switch (processUserInput())
    {
        case DEFAULT_SERVER:
            // Creation of Resource & Auto control for all requests from Client.
            initServer();
            return CORRECT_INPUT;

        case CUSTOM_SERVER:
            // Creation of Resource & setting get and set handler for handling get and
            // set request from client in application.
            initServer();

            g_server->setGetRequestHandler(requestHandlerForGet);
            g_server->setSetRequestHandler(requestHandlerForSet);
            return CORRECT_INPUT;
        case STOP :
            return QUIT;

        default :
            std::cout << "Invalid input, please try again" << std::endl;
            return INCORRECT_INPUT;
    }
}

void process()
{
    while (true)
    {
        displayMenu();

        int ret = selectServerMenu();

        if (ret == QUIT)
        {
            return;
        }
        if (ret == CORRECT_INPUT)
        {
            break;
        }
    }

    while (true)
    {
        displayControlMenu();

        if (selectControlMenu() == QUIT)
        {
            return;
        }
    }
}

int main(void)
{
    try
    {
        process();
    }
    catch (const std::exception &e)
    {
        std::cout << "main exception  : " << e.what() << std::endl;
    }

    std::cout << "Stopping the server" << std::endl;
}
