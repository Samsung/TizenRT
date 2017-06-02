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
#include "RCSSeparateResponse.h"

#include "OCPlatform.h"

using namespace OC::OCPlatform;
using namespace OIC::Service;

constexpr int RESOURCE_TEMP = 1;
constexpr int RESOURCE_LIGHT = 2;

RCSResourceObject::Ptr g_resource;

void handleRequestWithDelay(int delayInMillis, const RCSRequest& request,
        const RCSResourceAttributes& requestedAttrs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds{ delayInMillis });

    if (!requestedAttrs.empty())
    {
        auto resObject = request.getResourceObject().lock();

        RCSResourceObject::LockGuard lock{ resObject };
        auto& resAttrs = resObject->getAttributes();
        for (const auto& kv : requestedAttrs)
        {
            if (resAttrs.contains(kv.key()))
            {
                resAttrs[kv.key()] = kv.value();
            }
        }
    }

    RCSSeparateResponse(request).set();

    std::cout << "Separate response is set!" << std::endl;
}

void doSeparateResponse(const RCSRequest& request, const RCSResourceAttributes& requestedAttrs)
{
    int delay = std::rand() % 3000 + 500;

    std::cout << "Response will be sent in " << delay << "ms" << std::endl;

    std::thread(handleRequestWithDelay, delay, request, requestedAttrs).detach();
}

int processUserInput(int min, int max)
{
    assert(min <= max);

    int input = 0;

    while(true)
    {
        std::cin >> input;

        if (!std::cin.fail())
        {
            if(min <= input && input <= max)
            {
                return input;
            }
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid Input, please try again\n";
    }
}

void printAttributes(const RCSResourceAttributes& attrs)
{
    for(const auto& attr : attrs)
    {
        std::cout << "\tkey : " << attr.key() << "\n\tvalue : "
                  << attr.value().toString() << std::endl;
    }
}

RCSGetResponse requestHandlerForGet(const RCSRequest& request, RCSResourceAttributes&)
{
    std::cout << "Received a Get request from Client" << std::endl;

    doSeparateResponse(request, RCSResourceAttributes{ });

    return RCSGetResponse::separate();
}

RCSSetResponse requestHandlerForSet(const RCSRequest& request, RCSResourceAttributes& attrs)
{
    std::cout << "Received a Set request from Client" << std::endl;
    printAttributes(attrs);

    doSeparateResponse(request, attrs);

    return RCSSetResponse::separate();
}

void initServer(const std::string& resourceUri, const std::string& resourceType)
{
    g_resource = RCSResourceObject::Builder(resourceUri, resourceType, "oic.if.").build();
}

void selectResourceType()
{
    std::cout << "========================================================\n";
    std::cout << "Select Resource Type                                    \n";
    std::cout << RESOURCE_TEMP << ". Temperature                          \n";
    std::cout << RESOURCE_LIGHT << ". Light                               \n";
    std::cout << "========================================================\n";

    int resourceType = processUserInput(RESOURCE_TEMP, RESOURCE_LIGHT);

    switch (resourceType)
    {
        case RESOURCE_TEMP:
            initServer("/a/TempSensor", "oic.r.temperaturesensor");
            break;

        case RESOURCE_LIGHT:
            initServer("/a/light", "oic.r.light");
            break;
    }

    g_resource->setGetRequestHandler(requestHandlerForGet);
    g_resource->setSetRequestHandler(requestHandlerForSet);
}

int main(void)
{
    try
    {
        selectResourceType();
    }
    catch(...)
    {
        std::cout << "Can't select Resource Type" << std::endl;
        return -1;
    }

    std::cout << "Resource successfully created!" << std::endl;

    std::cout << "Press '1' to stop the process" << std::endl;

    while (std::cin.get() != '1');

    std::cout << "Stopping the server" << std::endl;

    g_resource.reset();
}

