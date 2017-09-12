/* *****************************************************************
 *
 * Copyright 2017 Microsoft
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

#include "testelevatorclient.h"
#include "ipcatestdata.h"

#define VERBOSE_INFO  0   // set to 1 for extra output.

using namespace OC;
using namespace std::placeholders;

ElevatorClient::ElevatorClient()
{
    m_elevatorResource = nullptr;
}

ElevatorClient::~ElevatorClient()
{
}

void ElevatorClient::OnObserveCallback(
                        const HeaderOptions headerOptions,
                        const OCRepresentation &rep,
                        const int &eCode,
                        const int &sequenceNumber)
{
    OC_UNUSED(headerOptions);
    OC_UNUSED(sequenceNumber);
    OC_UNUSED(eCode);

    int newCurrentFloor, newTargetFloor, newDirection;

    if (rep.getValue(ELEVATOR_PROPERTY_CURRENT_FLOOR, newCurrentFloor) == true)
    {
        m_notifiedCurrentFloor = newCurrentFloor;
    }

    if (rep.getValue(ELEVATOR_PROPERTY_TARGET_FLOOR, newTargetFloor) == true)
    {
        m_notifiedTargetFloor = newTargetFloor;
    }

    if (rep.getValue(ELEVATOR_PROPERTY_DIRECTION, newDirection) == true)
    {
        m_notifiedDirection = newDirection;
    }
}

int ElevatorClient::GetObservedCurrentFloor()
{
    return m_notifiedCurrentFloor;
}

bool ElevatorClient::StartObservation()
{
    OCStackResult result = m_elevatorResource->observe(
                                    ObserveType::Observe,
                                    QueryParamsMap(),
                                    std::bind(&ElevatorClient::OnObserveCallback,
                                            this, _1, _2, _3, _4));
    return result == OC_STACK_OK ? true : false;
}

bool ElevatorClient::StopObservation()
{
    OCStackResult result = m_elevatorResource->cancelObserve();
    return result == OC_STACK_OK ? true : false;
}

// Outstanding requests should time out in 247 seconds (EXCHANGE_LIFETIME) per rfc 7252.
const int DEFAULT_WAITTIME_MS = 247000;
bool ElevatorClient::WaitForCallback(int waitTimeMs = DEFAULT_WAITTIME_MS)
{
    std::unique_lock<std::mutex> lock { syncMutex };

    if (syncCV.wait_for(lock, std::chrono::milliseconds{ waitTimeMs }) != std::cv_status::timeout)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ElevatorClient::SignalCompletion()
{
    syncCV.notify_all();
}

// Callback handler on GET request
void ElevatorClient::OnGet(const HeaderOptions& headerOptions,
                           const OCRepresentation& rep,
                           const int eCode)
{
    OC_UNUSED(headerOptions);
    OC_UNUSED(eCode);

    if (eCode == OC_STACK_OK)
    {
        ReadRepresentation(rep);
    }
    else
    {
        printf("OnGet:: received error [0x%x]\r\n", eCode);
    }

    SignalCompletion();
}

// Callback handler on PUT request
void ElevatorClient::OnPut(const HeaderOptions& headerOptions,
                           const OCRepresentation& rep,
                           const int eCode)
{
    OC_UNUSED(headerOptions);

    if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
    {
        ReadRepresentation(rep);
    }
    else
    {
        printf ("PUT request failed, error: 0x%x\r\n", eCode);
    }

    SignalCompletion();
}

void ElevatorClient::ReadRepresentation(const OCRepresentation& rep)
{
    m_targetFloor = rep.getValue<int>(ELEVATOR_PROPERTY_TARGET_FLOOR);
    m_currentFloor = rep.getValue<int>(ELEVATOR_PROPERTY_CURRENT_FLOOR);
    m_direction = rep.getValue<int>(ELEVATOR_PROPERTY_DIRECTION);
}

std::recursive_mutex theMutex;

void ElevatorClient::PrintResource(std::shared_ptr<OCResource> resource)
{
#if (VERBOSE_INFO == 1)
    std::lock_guard<std::recursive_mutex> lock(theMutex);
    std::string resourcePath;
    std::string hostAddress;
    std::cout << "ElevatorClient::PrintResource:" << std::endl;
    std::cout << "Resource URI: " << resource->uri() << std::endl;
    std::cout << "Device URI: " << resource->host() << std::endl;
    std::cout << "Resource Types: " << std::endl;
    for(auto &rt : resource->getResourceTypes())
        std::cout << "   " << rt << std::endl;
    std::cout << "List of resource interfaces: " << std::endl;
    for(auto &ri : resource->getResourceInterfaces())
        std::cout << "   " << ri << std::endl;
    std::cout << std::endl;
#else
    OC_UNUSED(resource);
#endif
}

// Temporarily store the found resource pointers until their device name is found.
// Key is device URI (e.g., "coap://[fe80::5828:93a8:d53e:4222%7]:62744")
std::map<std::string, std::shared_ptr<OCResource>> OCFDevices;

void ElevatorClient::OnDeviceInfoCallback(const OCRepresentation& rep)
{
    std::lock_guard<std::recursive_mutex> lock(theMutex);
    std::string deviceName;
    rep.getValue("n", deviceName);

    if (m_elevatorResource == nullptr && deviceName.compare(m_targetElevatorName) == 0)
    {
        m_elevatorResource = OCFDevices[rep.getHost()];
        OCFDevices.clear(); // free rest of the resources.
        PrintResource(m_elevatorResource);
    }
}

void ElevatorClient::PrintOCRep(const OCRepresentation& rep)
{
#if (VERBOSE_INFO == 1)
    std::lock_guard<std::recursive_mutex> lock(theMutex);
    std::cout << "++++++++++ PrintOCRep() ++++++++++++" << std::endl;
    std::cout << "For device URI: " << rep.getHost() << std::endl;
    std::cout << "Resoure URI: " << rep.getUri() << std::endl;
    OCRepresentation::const_iterator itr= rep.begin();
    OCRepresentation::const_iterator endItr = rep.end();
    for(;itr!=endItr;++itr)
    {
        std::string value = (*itr).getValue<std::string>();
        std::cout << "   = ElevatorClient:: Attribute name:  " << itr->attrname() << std::endl;
        std::cout << "     ElevatorClient:: Attribute type:  " << itr->type() << std::endl;
        std::cout << "     ElevatorClient:: Attribute value: ";
        switch(itr->type())
        {
            case AttributeType::Null:
            {
                std::cout << "nullptr" << std::endl;
                break;
            }

            case AttributeType::String:
            {
                std::cout << ((*itr).getValue<std::string>()).c_str() << std::endl;
                break;
            }

            case AttributeType::Integer:
            {
                std::cout << (*itr).getValue<int>() << std::endl;
                break;
            }

            default:
            {
                std::cout << "unrecognized type" << std::endl;
                break;
            }
        }

    }
    std::cout << "---------- PrintOCRep() ------------" << std::endl << std::endl;
#else
    OC_UNUSED(rep);
#endif
}

void ElevatorClient::OnPlatformInfoCallback(const OCRepresentation& rep)
{
    OC_UNUSED(rep);
    std::lock_guard<std::recursive_mutex> lock(theMutex);
    PrintOCRep(rep);
}

// Callback to found resources
void ElevatorClient::OnResourceFound(std::shared_ptr<OCResource> resource)
{
    std::lock_guard<std::recursive_mutex> lock(theMutex);

    // Target elevator already found.
    if (m_elevatorResource != nullptr)
    {
        return;
    }

    std::vector<std::string> resourceTypes = resource->getResourceTypes();

    if (resourceTypes.size() != 1)
    {
        return;
    }

    bool matchTargetResourceType = false;

    for (auto const& rt : resourceTypes)
    {
        if (rt.compare(ELEVATOR_RESOURCE_TYPE) == 0)
        {
            matchTargetResourceType = true;
            break;
        }
    }

    if (matchTargetResourceType == false)
    {
        return;
    }

    if (OCFDevices.find(resource->host()) != OCFDevices.end())
    {
        return;  // have seen this resource from this host.
    }

    OCFDevices[resource->host()] = resource;

    // Get the device name for match with m_targetElevatorName.
    OCPlatform::getDeviceInfo(
                    resource->host(),
                    OC_RSRVD_DEVICE_URI,
                    CT_DEFAULT,
                    std::bind(&ElevatorClient::OnDeviceInfoCallback, this, _1));


    OCPlatform::getPlatformInfo(
                    resource->host(),
                    OC_RSRVD_PLATFORM_URI,
                    CT_DEFAULT,
                    std::bind(&ElevatorClient::OnPlatformInfoCallback, this, _1));
}

// each elevator in unit test has unique device name to differentiate it from the others.
bool ElevatorClient::FindElevator(std::string elevatorName)
{
    std::string defaultElevatorResourceType = ELEVATOR_RESOURCE_TYPE;
    std::ostringstream deviceUri;
    m_targetElevatorName = elevatorName;

    deviceUri << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << defaultElevatorResourceType.c_str();

    OCConnectivityType connectivityType = CT_ADAPTER_IP;

    OCStackResult result = OCPlatform::findResource(
                                "",
                                deviceUri.str(),
                                connectivityType,
                                std::bind(&ElevatorClient::OnResourceFound, this, _1));

    if (result == OC_STACK_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ElevatorClient::IsElevatorFound()
{
    return m_elevatorResource != nullptr;
}

void ElevatorClient::SetTargetFloor(int targetFloor)
{
    if(m_elevatorResource)
    {
        OCRepresentation rep;
        rep[ELEVATOR_PROPERTY_TARGET_FLOOR] = targetFloor;

        m_elevatorResource->post(
            rep,
            QueryParamsMap(),
            std::bind(&ElevatorClient::OnPut, this, _1, _2, _3));

        WaitForCallback();  // wait for completion.
    }
}

bool ElevatorClient::GetTargetFloor(int* targetFlr)
{
    m_elevatorResource->get(QueryParamsMap(), std::bind(&ElevatorClient::OnGet, this, _1, _2, _3));
    if (WaitForCallback())
    {
        *targetFlr = m_targetFloor;
        return true;
    }
    else
    {
        *targetFlr = -1;
        return false;
    }
}

bool ElevatorClient::GetCurrentFloor(int* currFloor)
{
    m_elevatorResource->get(QueryParamsMap(), std::bind(&ElevatorClient::OnGet, this, _1, _2, _3));
    if (WaitForCallback())
    {
        *currFloor = m_currentFloor;
        return true;
    }
    else
    {
        *currFloor = -1;
        return false;
    }
}

bool ElevatorClient::GetDirection(int* dir)
{
    m_elevatorResource->get(QueryParamsMap(), std::bind(&ElevatorClient::OnGet, this, _1, _2, _3));
    if (WaitForCallback())
    {
        *dir = m_direction;
        return true;
    }
    else
    {
        *dir = -1;
        return false;
    }
}
