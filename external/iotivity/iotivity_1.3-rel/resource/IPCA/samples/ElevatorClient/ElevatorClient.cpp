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

#include "iotivity_config.h"

#include <mutex>
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <climits>

#include "ipca.h"
#include "OCFDevice.h"

#define UNUSED_PARAMETER(P)       (void)(P)

// Forward decls.
extern bool RediscoverElevator();
extern void IPCA_CALL ResourceChangeNotificationCallback(
                                            IPCAStatus result,
                                            void* context,
                                            IPCAPropertyBagHandle propertyBagHandle);

// Key is device id.  Value is pointer to OCFDevice.
std::map<std::string, OCFDevice::Ptr> g_OCFDeviceList;
std::recursive_mutex g_globalMutex;   // Sync access to g_OCFDeviceList.

// Command line arguments
std::string g_targetDeviceId;
bool g_isList = false;
bool g_isId = false;
bool g_isSet = false;
bool g_isGet = false;
bool g_isObserve = false;
bool g_isAuth = false;
int g_targetFloor = 0;
bool g_isContinuous = false;

// Handles
IPCADeviceHandle g_deviceHandle = nullptr;
IPCAHandle g_observeHandle = nullptr;
IPCAAppHandle g_ipcaAppHandle = nullptr;

// Operation flags
bool g_targetElevatorDiscovered = false;  // Set to true when device matching g_targetDeviceId is
                                          // discovered.
bool g_rediscoverFailedQuitNow = false;

// Call IPCAObserveResource();
IPCAStatus RequestObserve()
{
    if (g_observeHandle != nullptr)
    {
        IPCACloseHandle(g_observeHandle, nullptr, 0);
        g_observeHandle = nullptr;
    }

    return IPCAObserveResource(
                g_deviceHandle,
                &ResourceChangeNotificationCallback,
                0,
                "/ipca/sample/elevator",
                nullptr,
                &g_observeHandle);
}

// Callback from IPCAObserveResource().
void IPCA_CALL ResourceChangeNotificationCallback(
                                        IPCAStatus result,
                                        void* context,
                                        IPCAPropertyBagHandle propertyBagHandle)
{
    UNUSED_PARAMETER(context);

    int observedCurrentFloor = -1, observedDirection = -1, observedTargetFloor = -1;

    if (result != IPCA_OK)
    {
        std::cout << "!!! ResourceChangeNotificationCallback callback !!! error: " << result
            << std::endl;

        RediscoverElevator();
        if (g_rediscoverFailedQuitNow != true)
        {
            // Restart the observe request.
            RequestObserve();
        }

        return;
    }

    if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.CurrentFloor",
                                &observedCurrentFloor))
    {
        std::cout << "ResourceChangeNotificationCallback(): failed get CurrentFloor." << std::endl;
    }

    if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.Direction",
                                &observedDirection))
    {
        std::cout << "ResourceChangeNotificationCallback(): failed get Direction." << std::endl;
    }

    if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.TargetFloor",
                                &observedTargetFloor))
    {
        std::cout << "ResourceChangeNotificationCallback(): failed get TargetFloor." << std::endl;
    }

    std::cout << "Resource Change Notification: " << std::endl;
    std::cout << "  CurrentFloor: " << observedCurrentFloor;
    std::cout << "  Direction: " << observedDirection;
    std::cout << "  TargetFloor: " << observedTargetFloor << std::endl;
}

// IPCAGetProperties() completion callback.
std::mutex g_getPropertiesCbMutex;
std::condition_variable g_getPropertiesCompleteCV;

void IPCA_CALL GetPropertiesCallback(
                            IPCAStatus result,
                            void* context,
                            IPCAPropertyBagHandle propertyBagHandle)
{
    UNUSED_PARAMETER(context);

    int currentFloor = 0, direction = 0, targetFloor = 0;

    if (result != IPCA_OK)
    {
        std::cout << "!!! GetProperties callback !!! error: " << result << std::endl;
        RediscoverElevator();
        g_getPropertiesCompleteCV.notify_all();
        return;
    }

    if (propertyBagHandle != nullptr)
    {
        if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.CurrentFloor",
                                &currentFloor))
        {
            std::cout << "GetPropertiesCallback(): failed get CurrentFloor." << std::endl;
        }

        if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.Direction",
                                &direction))
        {
            std::cout << "GetPropertiesCallback(): failed get Direction." << std::endl;
        }

        if (IPCA_OK != IPCAPropertyBagGetValueInt(propertyBagHandle,
                                "x.org.iotivity.TargetFloor",
                                &targetFloor))
        {
            std::cout << "GetPropertiesCallback(): failed get TargetFloor." << std::endl;
        }
    }

    std::cout << "*** GetProperties callback: successful ***" << std::endl;
    std::cout << "Current Floor   : " << currentFloor << std::endl;
    std::cout << "Direction       : " << direction << std::endl;
    std::cout << "TargetFloor     : " << targetFloor << std::endl;

    g_getPropertiesCompleteCV.notify_all();
}

// IPCASetProperties() completion callback.
std::mutex g_setPropertiesCbMutex;
std::condition_variable g_setPropertiesCompleteCV;
void IPCA_CALL SetPropertiesCallback(
                        IPCAStatus result,
                        void* context,
                        IPCAPropertyBagHandle propertyBagHandle)
{
    UNUSED_PARAMETER(context);
    UNUSED_PARAMETER(propertyBagHandle);

    if (result == IPCA_OK)
    {
        std::cout << "*** SetProperties callback: successful ***" << std::endl;
    }
    else
    {
        std::cout << "!!! SetProperties callback !!! error: " << result << std::endl;
        RediscoverElevator();
    }

    g_setPropertiesCompleteCV.notify_all();
}

// Callback when device is discovered.
std::mutex g_deviceDiscoveredCbMutex;
std::condition_variable g_deviceDiscoveredCV;
void IPCA_CALL DiscoverDevicesCallback(
                        void* context,
                        IPCADeviceStatus deviceStatus,
                        const IPCADiscoveredDeviceInfo* deviceInfo)
{
    UNUSED_PARAMETER(context);

    std::lock_guard<std::recursive_mutex> lock(g_globalMutex);

    std::string deviceId = deviceInfo->deviceId;
    std::string deviceName = deviceInfo->deviceName;
    std::vector<std::string> deviceUris;
    for (size_t i = 0; i < deviceInfo->deviceUriCount; i++)
    {
        deviceUris.push_back(deviceInfo->deviceUris[i]);
    }

    if (g_OCFDeviceList.find(deviceId) == g_OCFDeviceList.end())
    {
        OCFDevice::Ptr ocfDevice = std::shared_ptr<OCFDevice>(new
                                        OCFDevice(g_ipcaAppHandle, deviceId));
        if (ocfDevice == nullptr)
        {
            std::cout << "Out of memory" << std::endl;
            return;
        }
        g_OCFDeviceList[deviceId] = ocfDevice;
    }

    OCFDevice::Ptr ocfDevice = g_OCFDeviceList[deviceId];

    // Wake up any thread waiting for discovery of this device.
    if ((!g_targetElevatorDiscovered) &&
        (g_targetDeviceId.length() != 0) &&
        (g_targetDeviceId.compare(deviceId) == 0))
    {
        g_targetElevatorDiscovered = true;
        g_deviceDiscoveredCV.notify_all();
    }

    if (g_isList)
    {
        if (deviceStatus == IPCA_DEVICE_DISCOVERED)
        {
            std::cout << "*** New elevator: *** " << std::endl << std::endl;
        }
        else
        if (deviceStatus == IPCA_DEVICE_UPDATED_INFO)
        {
            std::cout << "+++ Elevator info updated for id: [" << deviceId << "] +++";
            std::cout << std::endl << std::endl;
        }
        else
        {
            std::cout << "--- Elevator is no longer discoverable. Device ID: [";
            std::cout << deviceId << "] ---" << std::endl << std::endl;
            g_OCFDeviceList.erase(deviceId);
            return;
        }

        IPCADeviceInfo* di = ocfDevice->GetDeviceInfo();

        if (di != nullptr)
        {
            std::cout << "Device Info: "  << std::endl;
            std::cout << std::endl;
            if (deviceUris.size() != 0)
            {
                int i = 0;
                for (auto& uri : deviceUris)
                {
                    if (i++ == 0)
                    {

                        std::cout << "   Device URI . . . . . . . :  " << uri << std::endl;
                    }
                    else
                    {
                        std::cout << "                               " << uri << std::endl;
                    }
                }
            }

            std::cout << "   Device ID  . . . . . . . :  " << (di->deviceId ? di->deviceId : "");
            std::cout << std::endl;

            std::cout << "   Device Name  . . . . . . :  ";
            std::cout << (di ->deviceName ? di->deviceName : "") << std::endl;

            std::cout << "   Device Software Version  :  ";
            std::cout << (di->deviceSoftwareVersion ? di->deviceSoftwareVersion : "") << std::endl;
            std::cout << std::endl;

        }
        else
        {
            std::cout << "Device Info: Not available."  << std::endl << std::endl;
        }

        IPCAPlatformInfo* pi = ocfDevice->GetPlatformInfo();

        if (pi != nullptr)
        {
            std::cout << "Platform Info:"  << std::endl;
            std::cout << std::endl;

            std::cout << "   Plaform ID . . . . . . . :  ";
            std::cout << (pi->platformId ? pi->platformId : "") << std::endl;

            std::cout << "   Manufacturer Name  . . . :  ";
            std::cout << (pi->manufacturerName ? pi->manufacturerName : "")  << std::endl;

            std::cout << "   Manufacturer URL . . . . :  ";
            std::cout << (pi->manufacturerURL ? pi->manufacturerURL : "")  << std::endl;

            std::cout << "   Model Number . . . . . . :  ";
            std::cout << (pi->modelNumber ? pi->modelNumber : "") << std::endl;

            std::cout << "   Manufacturing Date . . . :  ";
            std::cout << (pi->manufacturingDate ? pi->manufacturingDate : "") << std::endl;

            std::cout << "   Platform Version . . . . :  ";
            std::cout << (pi->platformVersion ? pi->platformVersion : "")  << std::endl;

            std::cout << "   OS Version . . . . . . . :  ";
            std::cout << (pi->osVersion ? pi->osVersion : "")  << std::endl;

            std::cout << "   Hardware Version . . . . :  ";
            std::cout << (pi->hardwareVersion ? pi->hardwareVersion : "")  << std::endl;

            std::cout << "   Firmware Version . . . . :  ";
            std::cout << (pi->firmwareVersion ? pi->firmwareVersion : "")  << std::endl;

            std::cout << "   Manufacturer Support URL :  ";
            std::cout << (pi->manufacturerSupportURL ? pi->manufacturerSupportURL : "");
            std::cout << std::endl;

            std::cout << "   Reference Time . . . . . :  ";
            std::cout << (pi->referenceTime ? pi->referenceTime : "") << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Platform info:  Not available. " << std::endl << std::endl;
        }

        std::cout << "Resource List:" << std::endl;
        ResourceList resourceList = ocfDevice->GetResourceInfo();
        for (auto const& resource : resourceList)
        {
            bool firstEntry = true;
            std::cout << "   Resource URI   . . . . . :  " << resource.first << std::endl;
            std::cout << "      Resource Types  . . . :  ";
            for (auto const& resourceType : resource.second)
            {
                if (firstEntry == true)
                {
                    std::cout << resourceType << std::endl;
                    firstEntry = false;
                }
                else
                {
                    std::cout << "                               " << resourceType << std::endl;
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "[Press enter to exit discovery]"<< std::endl << std::endl;
    }
}

void ShowUsage()
{
    std::cout << std::endl;
    std::cout << "USAGE: elevatorclient [-l] [-id elevator_id] [-s target_floor] [-g] [-o]";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Options" << std::endl;
    std::cout << "  -l               Discover and list elevators." << std::endl;
    std::cout << "  -id              Device ID of elevator, must be set for -s, -g, and -o arguments.";
    std::cout << std::endl;
    std::cout << "  -auth            Authenticate to device." << std::endl;
    std::cout << "  -s target_floor  Set target floor." << std::endl;
    std::cout << "  -g               Get current floor." << std::endl;
    std::cout << "  -o               Observe." << std::endl;
    std::cout << "  -cont            Run continuous -g or -s with increasing target floor";
    std::cout << std::endl;
}

bool ParseArgv(int argc, char* argv[])
{
    const std::string ListArgument = "-l";
    const std::string IdArgument = "-id";
    const std::string SetArgument = "-s";
    const std::string GetArgument = "-g";
    const std::string ObserveArgument = "-o";
    const std::string ContinuousArgument = "-cont";
    const std::string AuthenticateToDeviceArgument = "-auth";

    for (int i = 1 ; i < argc; i++)
    {
       if (ListArgument.compare(argv[i]) == 0)
       {
           g_isList = true;
           continue;
       }

       if (GetArgument.compare(argv[i]) == 0)
       {
           g_isGet = true;
           continue;
       }

       if (ObserveArgument.compare(argv[i]) == 0)
       {
           g_isObserve = true;
           continue;
       }

       if (ContinuousArgument.compare(argv[i]) == 0)
       {
           g_isContinuous = true;
           continue;
       }

       if (IdArgument.compare(argv[i]) == 0)
       {
           if (++i == argc)
           {
               return false;
           }

           g_targetDeviceId = argv[i];
           g_isId = true;
           continue;
       }

       if (SetArgument.compare(argv[i]) == 0)
       {
           if (++i == argc)
           {
               return false;
           }

           g_targetFloor = std::stoi(argv[i]);
           g_isSet = true;
           continue;
       }

       if (AuthenticateToDeviceArgument.compare(argv[i]) == 0)
       {
           g_isAuth = true;
           continue;
       }
    }

    return true;
}

void CloseDeviceTargetElevator()
{
    if (g_deviceHandle != nullptr)
    {
        IPCACloseDevice(g_deviceHandle);
        g_deviceHandle = nullptr;
    }
}

bool OpenDeviceTargetElevator()
{
    if (g_deviceHandle != nullptr)
    {
        CloseDeviceTargetElevator();
    }

    IPCAStatus status = IPCAOpenDevice(g_ipcaAppHandle, g_targetDeviceId.c_str(), &g_deviceHandle);
    if (status != IPCA_OK)
    {
        return false;
    }

    return true;
}

IPCAHandle g_discoverDeviceHandle;

// Synchronous call to discovery device.
// If not free run, Function waits for timeOut milliseconds and stops when function exits.
bool DiscoverElevator(bool freeRun, size_t timeOutMs)
{
    std::unique_lock<std::mutex> lock { g_deviceDiscoveredCbMutex };

    const char* resourceTypes[] = {
            "x.org.iotivity.sample.elevator",
            "x.org.iotivity.sample.elevator2",
            "x.org.iotivity.sample.elevator3",
            "x.org.iotivity.sample.elevator4",
        };

    const int NUMBER_OF_RESOURCE_TYPES = sizeof(resourceTypes) / sizeof(char*);

    IPCAStatus status = IPCADiscoverDevices(
                            g_ipcaAppHandle,
                            &DiscoverDevicesCallback,
                            nullptr,
                            resourceTypes,
                            NUMBER_OF_RESOURCE_TYPES,
                            &g_discoverDeviceHandle);

    if (status != IPCA_OK)
    {
        return false;
    }

    if (!freeRun)
    {
        g_deviceDiscoveredCV.wait_for(lock, std::chrono::milliseconds{ timeOutMs });

        // Stop discovery.
        IPCACloseHandle(g_discoverDeviceHandle, nullptr, 0);
    }

    return g_targetElevatorDiscovered;
}

bool RediscoverElevator()
{
    g_targetElevatorDiscovered = false;

    CloseDeviceTargetElevator();

    std::cout << "Rediscovering elevator..." << std::endl;

    if (false == DiscoverElevator(false, INT_MAX) || false == OpenDeviceTargetElevator())
    {
        std::cout << "Failed to rediscover elevator! " << std::endl;
        g_rediscoverFailedQuitNow = true;
        return false;
    }

    std::cout << "Elevator rediscovered." << std::endl;
    return true;
}

IPCAStatus IPCA_CALL PwdInputCallback(
                            void* context,
                            const IPCADeviceInfo* deviceInformation,
                            const IPCAPlatformInfo* platformInformation,
                            IPCAOwnershipTransferType type,
                            char* passwordBuffer,
                            size_t passwordBufferSize)
{
    UNUSED_PARAMETER(passwordBufferSize);
    UNUSED_PARAMETER(type);
    UNUSED_PARAMETER(platformInformation);
    UNUSED_PARAMETER(deviceInformation);
    UNUSED_PARAMETER(context);

    std::cout << "Received Password Input Callback" << std::endl;

    std::cout << "Input password: ";

    for (int ret = 0; 1 != ret; )
    {
        ret = scanf("%32s", passwordBuffer);
        for (; 0x20 <= getchar(); );  // for removing overflow garbages
                                      // '0x20<=code' is character region
    }

    std::cout << std::endl;

    return IPCA_OK;
}

IPCAStatus IPCA_CALL PwdDisplayCallback(
                            void* context,
                            const IPCADeviceInfo* deviceInformation,
                            const IPCAPlatformInfo* platformInformation,
                            IPCAOwnershipTransferType type,
                            const char* password)
{
    UNUSED_PARAMETER(context);
    UNUSED_PARAMETER(deviceInformation);
    UNUSED_PARAMETER(platformInformation);
    UNUSED_PARAMETER(type);


    std::cout << "==========================================" << std::endl;

    std::cout << "Password for device: " << password << std::endl;

    std::cout << "==========================================" << std::endl;

    return IPCA_OK;
}

void IPCA_CALL AuthCompletionCallback(IPCAStatus completionStatus, void* context)
{
    UNUSED_PARAMETER(context);
    std::cout << "AuthCompletionCallback().  Completion status is: " << completionStatus;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if ((argc == 1) || (ParseArgv(argc, argv) == false))
    {
        ShowUsage();
        return 0;
    }

    IPCAUuid appId = {
                       {0xb6, 0x12, 0x38, 0x0c, 0x8c, 0x4c, 0x11, 0xe6,
                        0xae, 0x22, 0x56, 0xb6, 0xb6, 0x49, 0x96, 0x11}
                     };

    IPCAAppInfo ipcaAppInfo = { appId, "ElevatorClient", "1.0.0", "Microsoft" };

    IPCAStatus status = IPCAOpen(&ipcaAppInfo, IPCA_VERSION_1, &g_ipcaAppHandle);
    if (status != IPCA_OK)
    {
        std::cout << "Failed IPCAOpen(). Status: " << status << std::endl;
        goto exit;
    }

    status = IPCASetPasswordCallbacks(g_ipcaAppHandle,
                        &PwdInputCallback,
                        &PwdDisplayCallback,
                        nullptr);

    if (status != IPCA_OK)
    {
        std::cout << "Failed IPCASetPasswordCallbacks(). Status: " << status << std::endl;
        goto exit;
    }

    // List discovered elevators.
    if (g_isList)
    {
        DiscoverElevator(true, 0);
        std::cin.get();
        goto exit;
    }

    // Other than "-l" option, device ID is required.
    if (!g_isId)
    {
        std::cout << "Error, device ID is required." << std::endl;
        ShowUsage();
        goto exit;
    }

    // Wait for discovery of target elevator.
    if (false == DiscoverElevator(false, 3000))
    {
        std::cout << "Target elevator was not discovered." << std::endl;
        goto exit;
    }

    // Open target elevator.
    if (false == OpenDeviceTargetElevator())
    {
        std:: cout << "Error opening target elevator" << std::endl;
        goto exit;
    }

    // Get target elevator data.
    if (g_isGet)
    {
        std::unique_lock<std::mutex> lock { g_getPropertiesCbMutex };
        int loopCount = (g_isContinuous == true) ? INT_MAX : 1;

        for (int i = 0; i < loopCount; i++)
        {
            std::cout << "Calling IPCAGetProperties().. (" << i << ")" << std::endl;
            IPCAStatus getStatus = IPCAGetProperties(g_deviceHandle,
                                        &GetPropertiesCallback,
                                        0,
                                        "/ipca/sample/elevator",
                                        nullptr,
                                        nullptr,
                                        nullptr);
            if (IPCA_OK == getStatus)
            {
                g_getPropertiesCompleteCV.wait_for(lock, std::chrono::milliseconds{ INT_MAX });
            }
            if (g_rediscoverFailedQuitNow)
            {
                break;
            }
        }
        goto exit;
    }

    // Set target elevator's TargetFloor.
    if (g_isSet)
    {
        std::unique_lock<std::mutex> lock { g_setPropertiesCbMutex };
        int loopCount = g_isContinuous == true ? INT_MAX : 1;
        int targetFloor = g_targetFloor;

        for (int i = 0; i < loopCount; i++)
        {
            std::cout << "Calling IPCASetProperties() to target floor: ";
            std::cout << targetFloor << " (" << i << ")" << std::endl;

            IPCAPropertyBagHandle propertyBagHandle;
            status = IPCAPropertyBagCreate(&propertyBagHandle);
            if (IPCA_OK == status)
            {
                status = IPCAPropertyBagSetValueInt(propertyBagHandle,
                                    "x.org.iotivity.TargetFloor",
                                    targetFloor++);

                if (IPCA_OK == status)
                {
                    status = IPCASetProperties(g_deviceHandle,
                                    &SetPropertiesCallback,
                                    0,
                                    "/ipca/sample/elevator",
                                    "x.org.iotivity.sample.elevator",
                                    nullptr,
                                    propertyBagHandle,
                                    nullptr);

                    if (IPCA_OK == status)
                    {
                        g_setPropertiesCompleteCV.wait_for(
                                lock, std::chrono::milliseconds{ INT_MAX });
                    }
                }
                IPCAPropertyBagDestroy(propertyBagHandle);
            }

            if (g_rediscoverFailedQuitNow)
            {
                break;
            }
        }
        goto exit;
    }

    // Register for observation.
    if (g_isObserve)
    {
        RequestObserve();
        std::cout << "any key and enter to exit.." << std::endl;
        std::cin.get();
        goto exit;
    }

    // Authenticate to a device (i.e. MOT).
    if (g_isAuth)
    {
        IPCARequestAccess(g_deviceHandle, nullptr, AuthCompletionCallback, nullptr, nullptr);
        std::cout << "Authenticating to device.  Use ctrl-c to quit when authentication is complete.";
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1000));
    }

exit:
    CloseDeviceTargetElevator();
    g_OCFDeviceList.clear();

    if (g_ipcaAppHandle != nullptr)
    {
        IPCAClose(g_ipcaAppHandle);
    }

    return 0;
}
