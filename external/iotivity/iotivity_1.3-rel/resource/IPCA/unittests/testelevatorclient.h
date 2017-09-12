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
#ifndef _ELEVATOR_CLIENT_H
#define _ELEVATOR_CLIENT_H

#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;

class ElevatorClient
{
public:
    ElevatorClient();
    ~ElevatorClient();

    bool FindElevator(std::string targetElevatorName);
    bool IsElevatorFound();

    void SetTargetFloor(int targetFloor);

    bool GetTargetFloor(int* targetFlr);
    bool GetCurrentFloor(int* currFloor);
    bool GetDirection(int* dir);

    bool StartObservation();
    bool StopObservation();
    int GetObservedCurrentFloor();

private:
    // Mutex is used togeter with the conditional_variable for waiting async OnGet and OnPut
    // callbacks.
    std::mutex syncMutex;
    std::condition_variable syncCV;
    bool WaitForCallback(int waitingTime);  // return false if wait timeout.
    void SignalCompletion();

    std::shared_ptr<OCResource> m_elevatorResource;

    void OnObserveCallback(
            const HeaderOptions headerOptions,
            const OCRepresentation &rep,
            const int &eCode,
            const int &sequenceNumber);

    void OnResourceFound(std::shared_ptr<OCResource> resource);
    void OnDeviceInfoCallback(const OCRepresentation& rep);
    void OnPlatformInfoCallback(const OCRepresentation& rep);
    void OnGet(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
    void OnPut(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
    void ReadRepresentation(const OCRepresentation& rep);
    void PrintResource(std::shared_ptr<OCResource> resource);
    void PrintOCRep(const OCRepresentation& rep);

    // Target server has this device name.
    std::string m_targetElevatorName;

    // Last known property values from server.
    int m_targetFloor;
    int m_currentFloor;
    int m_direction;

    // Last known notified values from server.
    int m_notifiedTargetFloor;
    int m_notifiedCurrentFloor;
    int m_notifiedDirection;

};

#endif // _ELEVATOR_CLIENT_H