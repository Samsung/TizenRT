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
#include "elevatorserver.h"

using namespace OC;
using namespace std::placeholders;

#define TAG                "ElevatorServer.cpp"

// Secure Virtual Resource database for Iotivity Server
// It contains Server's Identity and the PSK credentials
// of other devices which the server trusts
static char CredFile[] = "ElevatorServerSecurityDB.dat";

FILE* server_fopen(const char *path, const char *mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(CredFile, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

//
// class ElevatorServer implementation.
//
ElevatorServer::ElevatorServer() :
    m_elevatorResourceHandle(nullptr),
    m_elevatorResourceHandle2(nullptr),
    m_elevatorResourceHandle3(nullptr),
    m_elevatorResourceHandle4(nullptr),
    m_targetFloor(1),
    m_currentFloor(1),
    m_direction(ElevatorDirection::Stopped),
    m_engineThread()
{
#ifdef SECURED
    m_displayPasswordCallbackHandle = nullptr;
#endif
}

ElevatorServer::~ElevatorServer()
{
    Stop();
}

// Return all properties in response.
OCStackResult ElevatorServer::SendResponse(std::shared_ptr<OCResourceRequest> request)
{
    // Values to return.
    OCRepresentation responseRep;
    responseRep["x.org.iotivity.CurrentFloor"] = GetCurrentFloor();
    responseRep["x.org.iotivity.TargetFloor"] = GetTargetFloor();
    responseRep["x.org.iotivity.Direction"] = (int)GetElevatorDirection();

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

    if(request != nullptr)
    {
        // Get the request type and request flag
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if(requestFlag & RequestHandlerFlag::RequestFlag)
        {
            // If the request type is GET
            if(requestType == "GET")
            {
                if (SendResponse(request) == OC_STACK_OK)
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "POST")
            {
                OCRepresentation requestRep = request->getResourceRepresentation();

                // Target floor can be set.
                int targetFloor;
                if (requestRep.getValue("x.org.iotivity.TargetFloor", targetFloor))
                {
                    SetTargetFloor(static_cast<int>(targetFloor));
                }

                if(OC_STACK_OK == SendResponse(request))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "PUT")
            {
                // not supported.
            }
            else if(requestType == "DELETE")
            {
                // not supported.
            }
        }

        if(requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            // Hold the lock to make sure no iterator is in progress.
            std::lock_guard<std::mutex> lock(m_elevatorMutex);

            ObservationInfo observationInfo = request->getObservationInfo();
            if(ObserveAction::ObserveRegister == observationInfo.action)
            {
                OIC_LOG_V(INFO, TAG, "ElevatorEntityHandler(): new observer ID: %d",
                    observationInfo.obsId);
                m_observers.push_back(observationInfo.obsId);
            }
            else if(ObserveAction::ObserveUnregister == observationInfo.action)
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
bool CopyStringToBuffer(const std::string& source, char* dest, size_t destSize)
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

// Initialize Persistent Storage for security database
OCPersistentStorage ps = {server_fopen, fread, fwrite, fclose, unlink};

bool ElevatorServer::Start(const std::string& elevatorName)
{
    std::lock_guard<std::mutex> lock(m_elevatorMutex);

    // OCPlatform needs only 1 time initialization.
    static bool OCFInitialized = false;
    if (false == OCFInitialized)
    {
        PlatformConfig Configuration {
                                ServiceType::InProc,
                                ModeType::Server,
                                "0.0.0.0", // By setting to "0.0.0.0", it binds to all available
                                           // interfaces
                                0,         // Uses randomly available port
                                QualityOfService::NaQos,
                                &ps
                            };

        OCPlatform::Configure(Configuration);
        OCFInitialized = true;
    }

    if (false == m_isRunning)
    {
        std::string defaultResourceTypeName("x.org.iotivity.sample.elevator");
        std::string defaultResourceName("/ipca/sample/elevator");

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
        CopyStringToBuffer(defaultResourceTypeName, resTypeName, 256);
        OCStringLL types { nullptr, resTypeName };
        char specVersion[] = "0.0.1";
        OCDeviceInfo deviceInfo = { devName, &types, specVersion, nullptr };

        // Platform Info
        char platformId[] = "6cb6c994-8c4b-11e6-ae22-56b6b6499611";
        char manufacturerName[] = "E Manufacturer";
        char manufacturerUrl[] = "http://www.example.com/elevator";
        char modelNumber[] = "Elevator Model Number";
        char dateManufacture[] = "2017-02-28";
        char platformVersion[] = "Elevator Platform Version";
        char osVersion[] = "Elevator OS Version";
        char hardwareVersion[] = "Elevator HW Version";
        char firmwareVersion[] = "Elevator FW Version";
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

#ifdef SECURED
        if (OC_STACK_OK != OCSecure::registerDisplayPinCallback(
                            std::bind(&ElevatorServer::PinDisplayCallback, this, _1, _2),
                                      &m_displayPasswordCallbackHandle))
        {
            return false;
        }
#endif
        OCStackResult result = OCPlatform::registerResource(
                                        m_elevatorResourceHandle,
                                        defaultResourceName,
                                        defaultResourceTypeName,
                                        DEFAULT_INTERFACE,
                                        std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                        OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        // These extra resources are created but not implemented by the entity handler.
        // They better reflect real devices supporting multiple resources.

        std::string defaultResourceTypeName2("x.org.iotivity.sample.elevator2");
        std::string defaultResourceName2("/ipca/sample/elevator2");
        result = OCPlatform::registerResource(
                                    m_elevatorResourceHandle2,
                                    defaultResourceName2,
                                    defaultResourceTypeName2,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        std::string defaultResourceTypeName3("x.org.iotivity.sample.elevator3");
        std::string defaultResourceName3("/ipca/sample/elevator3");
        result = OCPlatform::registerResource(
                                    m_elevatorResourceHandle3,
                                    defaultResourceName3,
                                    defaultResourceTypeName3,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);

        if (result != OC_STACK_OK)
        {
            return false;
        }

        std::string defaultResourceTypeName4("x.org.iotivity.sample.elevator4");
        std::string defaultResourceName4("/ipca/sample/elevator4");
        result = OCPlatform::registerResource(
                                    m_elevatorResourceHandle4,
                                    defaultResourceName4,
                                    defaultResourceTypeName4,
                                    DEFAULT_INTERFACE,
                                    std::bind(&ElevatorServer::ElevatorEntityHandler, this, _1),
                                    OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);


        if (result == OC_STACK_OK)
        {
            const char* deviceID = OCGetServerInstanceIDString();
            std::cout << "Elevator server's device ID: ";
            if (deviceID != nullptr)
            {
                 std::cout << deviceID << std::endl;
            }
            else
            {
                std::cout << "Unknown" << std::endl;
            }
        }

        return (result == OC_STACK_OK);
    }

    // It's already running.
    return true;
}

void ElevatorServer::Stop()
{
    std::lock_guard<std::mutex> lock(m_elevatorMutex);

    if (true == m_isRunning)
    {
        // Unregister OCF resource.
        OCPlatform::unregisterResource(m_elevatorResourceHandle);
        OCPlatform::unregisterResource(m_elevatorResourceHandle2);
        OCPlatform::unregisterResource(m_elevatorResourceHandle3);
        OCPlatform::unregisterResource(m_elevatorResourceHandle4);
        m_elevatorResourceHandle = nullptr;
        m_elevatorResourceHandle2 = nullptr;
        m_elevatorResourceHandle3 = nullptr;
        m_elevatorResourceHandle4 = nullptr;

        // Signal the m_engineThread to stop and wait for it to exit.
        m_isRunning = false;
        if (m_engineThread.joinable())
        {
            m_engineThread.join();
        }

#ifdef SECURED
        // Unregister the password display callback
        OCSecure::deregisterDisplayPinCallback(m_displayPasswordCallbackHandle);
        m_displayPasswordCallbackHandle = nullptr;
#endif
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
    const size_t EngineSleepTimeSeconds = 2;
    std::chrono::seconds engineSleepTime(EngineSleepTimeSeconds);

    while (elevator->m_isRunning == true)
    {
        elevator->MoveElevator();
        std::this_thread::sleep_for(engineSleepTime);
    }
}

void ElevatorServer::NotifyObservers()
{
    // Local copy of observers so the code below doesn't need to hold the lock when
    // calling out to notifyListOfObservers.
    ObservationIds localCopyObservers;

    if (m_observers.size() == 0)
    {
        return;
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_elevatorMutex);
        localCopyObservers = m_observers;
    }

    OCRepresentation rep;
    rep["x.org.iotivity.CurrentFloor"] = GetCurrentFloor();
    rep["x.org.iotivity.Direction"] = (int)GetElevatorDirection();
    rep["x.org.iotivity.TargetFloor"] = GetTargetFloor();

    // Prepare the response.
    auto response = std::make_shared<OC::OCResourceResponse>();
    response->setResourceRepresentation(rep, DEFAULT_INTERFACE);

    OCStackResult result = OCPlatform::notifyListOfObservers(
                                m_elevatorResourceHandle,
                                localCopyObservers,
                                response);

    if(OC_STACK_NO_OBSERVERS == result)
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

    std::cout << "ElevatorServer::MoveElevator() new Direction: " << m_direction << std::endl;
    NotifyObservers();

    const int DelayBetweenFloorMilliseconds = 10;
    std::chrono::milliseconds delayBetweenFloor(DelayBetweenFloorMilliseconds);

    while (m_currentFloor != dwTargetFloor)
    {

        m_currentFloor += incrementValue;
        NotifyObservers();
        std::cout << "ElevatorServer::MoveElevator() new CurrentFloor: " << m_currentFloor;
        std::cout << std::endl;
        std::this_thread::sleep_for(delayBetweenFloor);
    }

    m_direction = ElevatorDirection::Stopped;
    NotifyObservers();
    std::cout << "ElevatorServer::MoveElevator() new Direction: " << m_direction << std::endl;
}

void ElevatorServer::PinDisplayCallback(char* pinData, size_t pinLength)
{
    if ((nullptr == pinData) || (pinLength == 0))
    {
        std::cout << "Invalid pin!" << std::endl;
        return;
    }

    std::cout << "============================" << std::endl;
    std::cout << "    PIN CODE: " << pinData << std::endl;
    std::cout << "============================" << std::endl;
}
