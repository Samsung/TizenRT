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

#include "logger.h"
#include "OCApi.h"
#include "testelevatorserver.h"
#include "ipcatestdata.h"

using namespace OC;
using namespace std::placeholders;

#define TAG     "TestElevatorServer.cpp"

// Initialize Persistent Storage for security database
FILE* elevatorServer_fopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen("IPCAUnitTest.dat", mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

OCPersistentStorage elevatorServerPS = {elevatorServer_fopen, fread, fwrite, fclose, unlink};

#define ELEVATOR_MAINTENANCE_PATH   "/oic/mnt"

//
// Class ElevatorServer implementation.
//
ElevatorServer::ElevatorServer() :
    m_elevatorResourceHandle(nullptr),
    m_elevatorCOResourceHandle(nullptr),
    m_elevatorMaintenanceHandle(nullptr),
    m_elevatorCreateRelativeResource(nullptr),
    m_elevatorDeleteResource(nullptr),
    m_engineThread()
{
    m_isRunning = false;
    m_targetFloor = m_currentFloor = 1;
    m_direction = ElevatorDirection::Stopped;
    m_relativePathResourceCreateCount = 0;
    m_explicitPathResourceCreateCount = 0;
    m_deleteResourceCount = 0;
}

ElevatorServer::~ElevatorServer()
{
}

// Return all properties in response.
OCStackResult ElevatorServer::SendResponse(std::shared_ptr<OCResourceRequest> request,
                                           OCEntityHandlerResult result)
{
    // Values to return.
    OCRepresentation responseRep;
    responseRep[ELEVATOR_PROPERTY_CURRENT_FLOOR] = GetCurrentFloor();
    responseRep[ELEVATOR_PROPERTY_TARGET_FLOOR] = GetTargetFloor();
    responseRep[ELEVATOR_PROPERTY_DIRECTION] = (int)GetElevatorDirection();

    // Prepare the response.
    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(request->getRequestHandle());
    pResponse->setResourceHandle(request->getResourceHandle());
    pResponse->setResourceRepresentation(responseRep);
    pResponse->setResponseResult(result);

    // Send the response.
    return OCPlatform::sendResponse(pResponse);
}

OCStackResult ElevatorServer::SendMaintenanceResponse(std::shared_ptr<OCResourceRequest> request)
{
    // Values to return.
    OCRepresentation responseRep;
    responseRep["fr"] = false;
    responseRep["rb"] = false;

    // Prepare the response.
    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(request->getRequestHandle());
    pResponse->setResourceHandle(request->getResourceHandle());
    pResponse->setResourceRepresentation(responseRep);
    pResponse->setResponseResult(OC_EH_OK);

    // Send the response.
    return OCPlatform::sendResponse(pResponse);
}

// Callback handler for elevator resource.
OCEntityHandlerResult ElevatorServer::ElevatorEntityHandler(
                            std::shared_ptr<OCResourceRequest> request)
{
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if (request->getResourceUri() == ELEVATOR_CO_RESOURCE_PATH)
    {
        return OC_EH_FORBIDDEN;
    }

    if (request)
    {
        std::string resourceUri = request->getResourceUri();

        // Get the request type and request flag
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if (requestFlag & RequestHandlerFlag::RequestFlag)
        {
            // If the request type is GET
            if (requestType == OC::PlatformCommands::GET)
            {
                if (resourceUri.compare(ELEVATOR_RESOURCE_PATH) == 0)
                {
                    if (SendResponse(request) == OC_STACK_OK)
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else
                if (resourceUri.compare(ELEVATOR_MAINTENANCE_PATH) == 0)
                {
                    if (SendMaintenanceResponse(request) == OC_STACK_OK)
                    {
                        ehResult = OC_EH_OK;
                    }
                }
            }
            else if (requestType == OC::PlatformCommands::POST)
            {
                OCRepresentation requestRep = request->getResourceRepresentation();

                if (resourceUri.compare(ELEVATOR_RESOURCE_PATH) == 0)
                {
                    QueryParamsMap queryMaps = request->getQueryParameters();
                    auto interfaceQuery = queryMaps.find("if");
                    if ((interfaceQuery != queryMaps.end()) &&
                        (interfaceQuery->second.compare(ELEVATOR_RESOURCE_MADE_UP_INTERFACE) == 0))
                    {
                        m_IncorrectInterfaceCount++;
                        if (OC_STACK_OK == SendResponse(request, OC_EH_ERROR))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else
                    {
                        // Target floor can be set.
                        int targetFloor;
                        if (requestRep.getValue(ELEVATOR_PROPERTY_TARGET_FLOOR, targetFloor))
                        {
                            SetTargetFloor((int)targetFloor);
                            if (OC_STACK_OK == SendResponse(request))
                            {
                                ehResult = OC_EH_OK;
                            }
                        }
                    }

                }
                else
                if (resourceUri.compare(ELEVATOR_MAINTENANCE_PATH) == 0)
                {
                    bool action;

                    if (requestRep.getValue("rb", action))
                    {
                        std::cout << "ElevatorServer: Reboot request received." << std::endl;
                    }

                    if (requestRep.getValue("fr", action))
                    {
                        std::cout << "ElevatorServer: Factory reset request received." << std::endl;
                    }

                    if (SendMaintenanceResponse(request) == OC_STACK_OK)
                    {
                        ehResult = OC_EH_OK;
                    }

                    ehResult = OC_EH_OK;
                }
                if (resourceUri.compare(ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH) == 0)
                {
                    m_relativePathResourceCreateCount++;

                    // Prepare the response.
                    auto pResponse = std::make_shared<OC::OCResourceResponse>();
                    pResponse->setRequestHandle(request->getRequestHandle());
                    pResponse->setResourceHandle(request->getResourceHandle());
                    pResponse->setNewResourceUri(ELEVATOR_RESOURCE_NEW_RESOURCE_PATH);
                    pResponse->setResponseResult(OC_EH_RESOURCE_CREATED);

                    // Send the response.
                    OCPlatform::sendResponse(pResponse);
                    ehResult = OC_EH_OK;
                }

            }
            else if (requestType == OC::PlatformCommands::PUT)
            {
                m_explicitPathResourceCreateCount++;
                ehResult = OC_EH_OK;
            }
            else if (requestType == OC::PlatformCommands::DELETE)
            {
                if (resourceUri.compare(ELEVATOR_RESOURCE_DELETE_PATH) == 0)
                {
                    m_deleteResourceCount++;

                    // Prepare the response.
                    auto pResponse = std::make_shared<OC::OCResourceResponse>();
                    pResponse->setRequestHandle(request->getRequestHandle());
                    pResponse->setResourceHandle(request->getResourceHandle());
                    pResponse->setResponseResult(OC_EH_RESOURCE_DELETED);

                    // Send the response.
                    OCPlatform::sendResponse(pResponse);
                    ehResult = OC_EH_OK;
                }
            }
        }

        if (requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            ObservationInfo observationInfo = request->getObservationInfo();
            if (ObserveAction::ObserveRegister == observationInfo.action)
            {
                OIC_LOG_V(INFO, TAG, "ElevatorEntityHandler(): new observer ID: %d",
                    observationInfo.obsId);
                m_observers.push_back(observationInfo.obsId);
            }
            else if (ObserveAction::ObserveUnregister == observationInfo.action)
            {
                OIC_LOG_V(INFO, TAG, "ElevatorEntityHandler(): removing observer ID: %d",
                    observationInfo.obsId);
                m_observers.erase(std::remove(
                                    m_observers.begin(),
                                    m_observers.end(),
                                    observationInfo.obsId),
                                    m_observers.end());
            }

            ehResult = OC_EH_OK;
        }
    }

    return ehResult;
}


// Copy from std::string to char array.  Return true if source is truncated at dest.
bool CopyStringToBuffer(std::string& source, char* dest, size_t destSize)
{
    bool isTruncated = false;
    size_t copied = source.copy(dest, destSize, 0);
    if (copied == destSize)
    {
        copied -= 1;    // make room for null
        isTruncated = true;
    }

    // std::string copy does not include null.
    dest[copied] = 0x00;
    return isTruncated;
}

bool ElevatorServer::Start(std::string& elevatorName)
{
    // OCPlatform needs only 1 time initialization.
    static bool OCFInitialized = false;
    if (false == OCFInitialized)
    {
        PlatformConfig Configuration {
                                ServiceType::InProc,
                                ModeType::Both,
                                "0.0.0.0", // By setting to "0.0.0.0", it binds to all available
                                           // interfaces
                                0,         // Uses randomly available port
                                QualityOfService::NaQos,
                                &elevatorServerPS
                            };

        OCPlatform::Configure(Configuration);
        OCFInitialized = true;
    }

    if (false == m_isRunning)
    {
        std::string resourceTypeName(ELEVATOR_RESOURCE_TYPE);
        std::string resourcePath(ELEVATOR_RESOURCE_PATH);

        m_name = elevatorName;

        // Start with known state.
        m_targetFloor = m_currentFloor = 1;
        m_direction = ElevatorDirection::Stopped;

        // Start the engine thread.
        m_isRunning = true;
        m_engineThread = std::thread(&ElevatorServer::Engine, this);

        // Device Info.
        char devName[256];
        char resTypeName[256];
        CopyStringToBuffer(m_name, devName, 256);
        CopyStringToBuffer(resourceTypeName, resTypeName, 256);
        OCStringLL types { nullptr, resTypeName };
        char specVersion[] = "0.0.1";
        OCDeviceInfo deviceInfo = { devName, &types, specVersion, nullptr };

        std::vector<std::string> dataModelVersions = {
                                    ELEVATOR_DATA_MODEL_VERSION_1,
                                    ELEVATOR_DATA_MODEL_VERSION_2,
                                    ELEVATOR_DATA_MODEL_VERSION_3};

        // Platform Info
        char platformId[] = ELEVATOR_PLATFORM_ID;
        char manufacturerName[] = "Elevator Manufacturer";
        char manufacturerUrl[] = "http://www.example.com/elevator";
        char modelNumber[] = "Elevator Model Number";
        char dateManufacture[] = "2017-02-28";
        char platformVersion[] = "Elevator Platform Version";
        char osVersion[] = "Elevator OS Version";
        char hardwareVersion[] = "Elevator Hardware Version";
        char firmwareVersion[] = "Elevator Firmware Version";
        char supportURL[] = "http://www.example.com/elevator/support";

        OCPlatformInfo platformInfo = {
            platformId,
            manufacturerName,
            manufacturerUrl,
            modelNumber,
            dateManufacture,
            platformVersion,
            osVersion,
            hardwareVersion,
            firmwareVersion,
            supportURL,
            nullptr};

        // Register elevator's platformInfo, deviceInfo, and resource.
        if (OC_STACK_OK != OCPlatform::registerPlatformInfo(platformInfo))
        {
            return false;
        }

        if (OC_STACK_OK != OCPlatform::registerDeviceInfo(deviceInfo))
        {
            return false;
        }

        // additional info for device info.
        if (OC_STACK_OK != OCPlatform::setPropertyValue(
                                PAYLOAD_TYPE_DEVICE,
                                OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                ELEVATOR_PLATFORM_INDEPENDENT_ID))
        {
            return false;
        }

        if (OC_STACK_OK != OCPlatform::setPropertyValue(
                                PAYLOAD_TYPE_DEVICE,
                                OC_RSRVD_DATA_MODEL_VERSION,
                                dataModelVersions))
        {
            return false;
        }

        OCStackResult result = OCPlatform::registerResource(
                                    m_elevatorResourceHandle,
                                    resourcePath,
                                    resourceTypeName,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
        if (result != OC_STACK_OK)
        {
            return false;
        }

        std::string maintenanceResourcePath(ELEVATOR_MAINTENANCE_PATH);
        std::string mainenanceResourceType(OC_RSRVD_RESOURCE_TYPE_MAINTENANCE);

        result = OCPlatform::registerResource(
                                    m_elevatorMaintenanceHandle,
                                    maintenanceResourcePath,
                                    mainenanceResourceType,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        std::string COresourceTypeName(ELEVATOR_CO_RESOURCE_TYPE);
        std::string COresourcePath(ELEVATOR_CO_RESOURCE_PATH);
        result = OCPlatform::registerResource(
                                    m_elevatorCOResourceHandle,
                                    COresourcePath,
                                    COresourceTypeName,
                                    ELEVATOR_CO_PRIVATE_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);

        if (result != OC_STACK_OK)
        {
            return false;
        }


        std::string createRelativeResourcePath(ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH);
        std::string createRelativeResourceType(ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_TYPE);
        result = OCPlatform::registerResource(
                                    m_elevatorCreateRelativeResource,
                                    createRelativeResourcePath,
                                    createRelativeResourceType,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        std::string deleteResourcePath(ELEVATOR_RESOURCE_DELETE_PATH);
        std::string deleteResourceType(ELEVATOR_RESOURCE_DELETE_TYPE);
        result = OCPlatform::registerResource(
                                    m_elevatorDeleteResource,
                                    deleteResourcePath,
                                    deleteResourceType,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        return true;
    }

    // It's already running.
    return true;
}

void ElevatorServer::Stop()
{
    if (true == m_isRunning)
    {
        //  Unregister OCF resources.
        OCPlatform::unregisterResource(m_elevatorResourceHandle);
        m_elevatorResourceHandle = nullptr;

        OCPlatform::unregisterResource(m_elevatorCOResourceHandle);
        m_elevatorCOResourceHandle = nullptr;

        OCPlatform::unregisterResource(m_elevatorMaintenanceHandle);
        m_elevatorMaintenanceHandle = nullptr;

        OCPlatform::unregisterResource(m_elevatorCreateRelativeResource);
        m_elevatorCreateRelativeResource = nullptr;

        OCPlatform::unregisterResource(m_elevatorDeleteResource);
        m_elevatorDeleteResource = nullptr;

        // Signal the m_engineThread to stop and wait for it to exit.
        m_isRunning = false;
        if (m_engineThread.joinable())
        {
            m_engineThread.join();
        }
    }
}

void ElevatorServer::SetTargetFloor(int floor)
{
    m_targetFloor = floor;
}

int ElevatorServer::GetTargetFloor()
{
    return m_targetFloor;
}

int ElevatorServer::GetCurrentFloor()
{
    return m_currentFloor;
}

ElevatorDirection ElevatorServer::GetElevatorDirection()
{
    return m_direction;
}

void ElevatorServer::Engine(ElevatorServer* elevator)
{
    while (elevator->m_isRunning == true)
    {
        elevator->MoveElevator();
        std::this_thread::sleep_for(std::chrono::seconds(0));
    }
}

void ElevatorServer::NotifyObservers(std::string propertyName, int value)
{
    if (m_observers.size() == 0)
    {
        return;
    }

    OIC_LOG_V(INFO, TAG, "NotifyObservers(): notifying observer of property:: %s",
        propertyName.c_str());

    OCRepresentation rep;
    rep[propertyName.c_str()] = value;

    // Prepare the response.
    auto response = std::make_shared<OC::OCResourceResponse>();
    response->setResourceRepresentation(rep, DEFAULT_INTERFACE);

    OCStackResult result = OCPlatform::notifyListOfObservers(
                                m_elevatorResourceHandle,
                                m_observers,
                                response);

    if (OC_STACK_NO_OBSERVERS == result)
    {
        std::cout << "ElevatorServer:: failed notifyListOfObservers: result = ";
        std::cout << result << std::endl;
    }

}

void ElevatorServer::MoveElevator()
{
    int dwTargetFloor = m_targetFloor;
    int incrementValue;

    if (m_currentFloor == dwTargetFloor)
    {
        return;
    }

    if (m_currentFloor < dwTargetFloor)
    {
        m_direction = ElevatorDirection::Up;
        incrementValue = 1;
    }
    else
    {
        m_direction = ElevatorDirection::Down;
        incrementValue = -1;
    }

    NotifyObservers(ELEVATOR_PROPERTY_DIRECTION, m_direction);

    while (m_currentFloor != dwTargetFloor)
    {
        const int delayBetweenFloorInSecond = 0;
        m_currentFloor += incrementValue;
        NotifyObservers(ELEVATOR_PROPERTY_CURRENT_FLOOR, m_currentFloor);
        std::this_thread::sleep_for(std::chrono::seconds(delayBetweenFloorInSecond));
    }

    m_direction = ElevatorDirection::Stopped;
    NotifyObservers(ELEVATOR_PROPERTY_DIRECTION, m_direction);
}
