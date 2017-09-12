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

#ifndef ELEVATOR_SERVER_H_
#define ELEVATOR_SERVER_H_

#include <string>
#include <mutex>
#include "OCPlatform.h"
#include "OCApi.h"
#include <OCProvisioningManager.hpp>

using namespace OC;

typedef enum
{
    Stopped = 0,
    Up,
    Down
} ElevatorDirection;

class ElevatorServer
{
public:
    ElevatorServer();
    ~ElevatorServer();

    // Start stop the thread processing the elevator movement.  Also register/unregister the
    // elevator from IoTivity.
    bool Start(const std::string& elevatorName);
    void Stop();

    // Target floor is set by caller.
    void SetTargetFloor(int floor);
    int GetTargetFloor();

    // Current floor is set by elevator.
    int GetCurrentFloor();
    ElevatorDirection GetElevatorDirection();

private:
    std::mutex m_elevatorMutex;

    // List of observers, when client app calls resource->Observer().
    ObservationIds m_observers;

    // Send notification to observers.
    void NotifyObservers();

    // Elevator resources
    OCResourceHandle m_elevatorResourceHandle;
    OCResourceHandle m_elevatorResourceHandle2;
    OCResourceHandle m_elevatorResourceHandle3;
    OCResourceHandle m_elevatorResourceHandle4;

    int m_targetFloor;    // where elevator needs to be.
    int m_currentFloor;   // where elevator is.
    ElevatorDirection m_direction;    // current direction of the elevator.

    // Thread moving the elevator.
    std::thread m_engineThread;
    bool m_isRunning;
    static void Engine(ElevatorServer* elevator);

    // Move current floor to target floor.
    void MoveElevator();

    // Helper function to send response for a request.
    OCStackResult SendResponse(std::shared_ptr<OCResourceRequest> request);

    // OCF callback for this elevator.
    OCEntityHandlerResult ElevatorEntityHandler(std::shared_ptr<OCResourceRequest> request);

    // OCF callback for a generated pin
    void PinDisplayCallback(char* pinData, size_t pinLength);

    // Elevator device details.
    std::string     m_name;

    // Elevator platform details.
    std::string     m_platformID;
    std::string     m_modelNumber;
    std::string     m_platformVersion;
    std::string     m_serialNumber;
    std::string     m_specVersion;
    std::string     m_defaultLanguage;
    std::string     m_manufacturerName;
    std::string     m_manufacturerUrl;
    std::string     m_dateOfManufacture;
    std::string     m_operatingSystemVersion;
    std::string     m_hardwareVersion;
    std::string     m_firmwareVersion;
    std::string     m_supportUrl;
    std::string     m_systemTime;

#ifdef SECURED
    DisplayPinCallbackHandle m_displayPasswordCallbackHandle;
#endif
};
#endif // ELEVATOR_SERVER_H_
