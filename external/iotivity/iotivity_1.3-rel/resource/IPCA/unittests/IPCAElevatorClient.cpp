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

#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <gtest/gtest.h>
#include "ocrandom.h"
#include "octypes.h"
#include "oic_time.h"
#include "ipca.h"
#include "ipcatestdata.h"
#include "IPCAElevatorClient.h"

extern IPCAUuid IPCATestAppUuid;
extern char IPCATestAppName[];
extern std::string g_elevator1Name;

IPCAStatus IPCAElevatorClient::FactoryResetElevator()
{
    IPCAStatus status = IPCAFactoryReset(m_deviceHandle);

    if (status == IPCA_RESOURCE_NOT_FOUND)
    {
        // Give it some time to discover the maintenance resource.
        std::this_thread::sleep_for(std::chrono::seconds(2));
        status = IPCAFactoryReset(m_deviceHandle);
    }

    return status;
}

IPCAStatus IPCAElevatorClient::RebootElevator()
{
    IPCAStatus status = IPCAReboot(m_deviceHandle);

    if (status == IPCA_RESOURCE_NOT_FOUND)
    {
        // Give it some time to discover the maintenance resource.
        std::this_thread::sleep_for(std::chrono::seconds(2));
        status = IPCAReboot(m_deviceHandle);
    }

    return status;
}

void IPCA_CALL C_GetPropertiesCb(
                        IPCAStatus result,
                        void* context,
                        IPCAPropertyBagHandle propertyBagHandle)
{
    IPCAElevatorClient* ipcaElevatorClient = (IPCAElevatorClient*)context;
    ipcaElevatorClient->GetPropertiesCallback(result, context, propertyBagHandle);
}

bool IPCAElevatorClient::GetElevatorProperties()
{
    m_getPropertiesCallbackCalled = false;

    // Get the data.
    EXPECT_EQ(IPCA_OK, IPCAGetProperties(
                            m_deviceHandle,
                            &C_GetPropertiesCb,
                            (void*)this,
                            ELEVATOR_RESOURCE_PATH,
                            nullptr,
                            nullptr,
                            nullptr));

    // Wait for get data completion.
    std::unique_lock<std::mutex> lock(m_getDataCompleteCbMutex);
    m_getDataCompleteCbCV.wait_for(
        lock,
        std::chrono::seconds(10),
        [this] { return m_getPropertiesCallbackCalled; });

    return m_getPropertiesCallbackCalled;
}

void IPCA_CALL C_SetPropertiesCb(
                        IPCAStatus result,
                        void* context,
                        IPCAPropertyBagHandle propertyBagHandle)
{
    IPCAElevatorClient* ipcaTest = (IPCAElevatorClient*)context;
    ipcaTest->SetPropertiesCallback(result, context, propertyBagHandle);
}

bool IPCAElevatorClient::SetElevatorProperties(IPCAPropertyBagHandle propertyBagHandle)
{
    m_setPropertiesCallbackCalled = false;

    // Set properties.
    EXPECT_EQ(IPCA_OK, IPCASetProperties(
                            m_deviceHandle,
                            &C_SetPropertiesCb,
                            this,
                            (const char*)ELEVATOR_RESOURCE_PATH,
                            nullptr,
                            (const char*)ELEVATOR_RESOURCE_TYPE,
                            propertyBagHandle,
                            nullptr));


    std::unique_lock<std::mutex> lock(m_setPropertiesCompleteCbMutex);

    m_setPropertiesCompleteCV.wait_for(
            lock,
            std::chrono::seconds(10),
            [this] { return m_setPropertiesCallbackCalled; });

    return m_setPropertiesCallbackCalled;
}

void IPCA_CALL C_CreateResourceCb(
                        IPCAStatus result,
                        void* context,
                        const char* newResourcePath,
                        IPCAPropertyBagHandle propertyBagHandle)
{
    IPCAElevatorClient* ipcaTest = (IPCAElevatorClient*)context;
    ipcaTest->CreateResourceCallback(result, newResourcePath, propertyBagHandle);
}

bool IPCAElevatorClient::CreateElevatorResource(
                                    std::string resourcePath,
                                    IPCAPropertyBagHandle propertyBagHandle)
{
    m_createResourceCallbackCalled = false;

    // Set properties.
    EXPECT_EQ(IPCA_OK, IPCACreateResource(
                            m_deviceHandle,
                            &C_CreateResourceCb,
                            this,
                            resourcePath.c_str(),
                            nullptr,
                            nullptr,
                            propertyBagHandle,
                            nullptr));


    std::unique_lock<std::mutex> lock(m_createResourceCompleteCbMutex);

    m_createResourceCompleteCV.wait_for(
            lock,
            std::chrono::seconds(10),
            [this] { return m_createResourceCallbackCalled; });

    return m_createResourceCallbackCalled;
}

void IPCA_CALL C_DeleteResourceCb(IPCAStatus result, void* context)
{
    IPCAElevatorClient* ipcaTest = (IPCAElevatorClient*)context;
    ipcaTest->DeleteResourceCallback(result);
}

bool IPCAElevatorClient::DeleteElevatorResource()
{
    m_deleteResourceCallbackCalled = false;

    // Delete resource.
    EXPECT_EQ(IPCA_OK, IPCADeleteResource(
                            m_deviceHandle,
                            &C_DeleteResourceCb,
                            this,
                            (const char*)ELEVATOR_RESOURCE_DELETE_PATH,
                            nullptr));


    std::unique_lock<std::mutex> lock(m_deleteResourceCompleteCbMutex);

    m_deleteResourceCompleteCV.wait_for(
            lock,
            std::chrono::seconds(10),
            [this] { return m_deleteResourceCallbackCalled; });

    return m_deleteResourceCallbackCalled;
}

void IPCAElevatorClient::GetCurrentFloor(int* currentFloor, bool* result)
{
    *result = false;
    if (GetElevatorProperties() == true)
    {
        *currentFloor = m_currentFloor;
        *result = true;
    }
}

void IPCAElevatorClient::GetTargetFloor(int* targetFloor, bool* result)
{
    *result = false;
    if (GetElevatorProperties() == true)
    {
        *targetFloor = m_targetFloor;
        *result = true;
    }
}

void IPCAElevatorClient::SetTargetFloor(int targetFloor, bool* result)
{
    *result = false;

    IPCAPropertyBagHandle propertyBagHandle;
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(
                            propertyBagHandle, ELEVATOR_PROPERTY_TARGET_FLOOR, targetFloor));

    *result = SetElevatorProperties(propertyBagHandle);
    IPCAPropertyBagDestroy(propertyBagHandle);
}

void IPCAElevatorClient::CreateResourceRelativePath()
{
    IPCAPropertyBagHandle propertyBagHandle;
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));

    CreateElevatorResource(ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH, propertyBagHandle);

    IPCAPropertyBagDestroy(propertyBagHandle);
}

void IPCAElevatorClient::CreateResourceLongRelativePath()
{
    IPCAPropertyBagHandle propertyBagHandle;
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));

    CreateElevatorResource(ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_LONG, propertyBagHandle);

    IPCAPropertyBagDestroy(propertyBagHandle);
}

void IPCAElevatorClient::DeleteResource()
{
    DeleteElevatorResource();
}

void IPCAElevatorClient::CreateResouceExplicitPath()
{
}

void IPCA_CALL C_ResourceChangeNotificationCb(
                                    IPCAStatus result,
                                    void* context,
                                    IPCAPropertyBagHandle propertyBagHandle)
{
    IPCAElevatorClient* ipcaTest = (IPCAElevatorClient*)context;
    ipcaTest->ResourceChangeNotificationCallback(result, propertyBagHandle);
}

bool IPCAElevatorClient::StartObserve()
{
    m_observedCurrentFloor = 0;
    m_observedDirection = 0;
    m_observedTargetFloor = 0;

    bool isObservable;
    IPCAIsResourceObservable(m_deviceHandle, "fakePath", &isObservable);
    EXPECT_FALSE(isObservable);

    IPCAIsResourceObservable(m_deviceHandle, ELEVATOR_RESOURCE_PATH, &isObservable);
    EXPECT_TRUE(isObservable);

    IPCAStatus status = IPCAObserveResource(
                                m_deviceHandle,
                                &C_ResourceChangeNotificationCb,
                                (void*)this,
                                ELEVATOR_RESOURCE_PATH,
                                nullptr,
                                &m_observeHandle);

    return (status == IPCA_OK ? true : false);
}

void IPCAElevatorClient::StopObserve()
{
    if (m_observeHandle)
    {
        IPCACloseHandle(m_observeHandle, nullptr, 0);
        m_observeHandle = nullptr;
    }
}

IPCAStatus IPCAElevatorClient::GetUnknownResource()
{
    // Get the data.
    IPCAStatus status = IPCAGetProperties(
                                m_deviceHandle,
                                &C_GetPropertiesCb,
                                (void*)this,
                                "/ipca/test/unknownresource",
                                nullptr,
                                nullptr,
                                nullptr);

    return status;
}

IPCAStatus IPCAElevatorClient::SetUnknownResource()
{
    IPCAPropertyBagHandle propertyBagHandle;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle,
                                ELEVATOR_PROPERTY_TARGET_FLOOR, 1)); /* 1 is randomly chosen */

    IPCAStatus status = IPCASetProperties(
                            m_deviceHandle,
                            &C_SetPropertiesCb,
                            this,
                            "/ipca/test/unknownresource",
                            nullptr,
                            nullptr,
                            nullptr,
                            nullptr);

    IPCAPropertyBagDestroy(propertyBagHandle);

    return status;
}

bool IPCAElevatorClient::SetUnknoownInterface()
{
    m_setPropertiesCallbackCalled = false;

    IPCAPropertyBagHandle propertyBagHandle;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle,
                            ELEVATOR_PROPERTY_TARGET_FLOOR, 1)); /* 1 is randomly chosen */

    IPCAStatus status = IPCASetProperties(
                            m_deviceHandle,
                            &C_SetPropertiesCb,
                            this,
                            ELEVATOR_RESOURCE_PATH,
                            ELEVATOR_RESOURCE_MADE_UP_INTERFACE,
                            nullptr,
                            propertyBagHandle,
                            nullptr);

    EXPECT_EQ(IPCA_OK, status);

    std::unique_lock<std::mutex> lock(m_setPropertiesCompleteCbMutex);

    m_setPropertiesCompleteCV.wait_for(
            lock,
            std::chrono::seconds(10),
            [this] { return m_setPropertiesCallbackCalled; });

    return m_setPropertiesCallbackCalled;
}

void IPCAElevatorClient::SetUp()
{
    m_elevator1Discovered = false;
    m_discoveredElevator1DeviceId.clear();
    m_discoveredElevator1DeviceName.clear();

    m_ipcaAppHandle = nullptr;
    m_deviceDiscoveryHandle = nullptr;
    m_deviceHandle = nullptr;
    m_observeHandle = nullptr;
    m_newResourcePath = "";

    IPCAAppInfo ipcaAppInfo = { IPCATestAppUuid, IPCATestAppName, "1.0.0", "Microsoft" };

    IPCAStatus status = IPCAOpen(&ipcaAppInfo, IPCA_VERSION_1, &m_ipcaAppHandle);

    ASSERT_EQ(IPCA_OK, status);

    DiscoverElevator1();
}

void IPCAElevatorClient::TearDown()
{
    if (m_observeHandle != nullptr)
    {
        IPCACloseHandle(m_observeHandle, nullptr, 0);
        m_observeHandle = nullptr;
    }

    if (m_deviceHandle != nullptr)
    {
        IPCACloseDevice(m_deviceHandle);
        m_deviceHandle = nullptr;
    }

    if (m_deviceDiscoveryHandle != nullptr)
    {
        IPCACloseHandle(m_deviceDiscoveryHandle, nullptr, 0);
        m_deviceDiscoveryHandle = nullptr;
    }

    if (m_ipcaAppHandle != nullptr)
    {
        IPCAClose(m_ipcaAppHandle);
        m_ipcaAppHandle = nullptr;
    }
}

IPCAStatus IPCAElevatorClient::ConfirmDeviceAndPlatformInfo()
{
    IPCAStatus status;

    // Device info.
    IPCADeviceInfo *deviceInfo = nullptr;
    int loopCount = 0;
    while (loopCount++ < 10)
    {
        status = IPCAGetDeviceInfo(m_deviceHandle, &deviceInfo);
        if (status == IPCA_OK)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    EXPECT_NE(nullptr, deviceInfo);
    if (deviceInfo)
    {
        EXPECT_STREQ(g_elevator1Name.c_str(), deviceInfo->deviceName);

        // See: ipcatestdata.h ELEVATOR_DATA_MODEL_VERSION_1 to 3.
        EXPECT_EQ(static_cast<size_t>(3), deviceInfo->dataModelVersionCount);

        bool modelFound = false;
        std::string Model1(ELEVATOR_DATA_MODEL_VERSION_1);
        for (size_t i = 0; i < deviceInfo->dataModelVersionCount; i++)
        {
            if (Model1.compare(deviceInfo->dataModelVersions[i]) == 0)
            {
                modelFound = true;
                break;
            }
        }

        EXPECT_TRUE(modelFound);
        EXPECT_STREQ(ELEVATOR_PLATFORM_INDEPENDENT_ID, deviceInfo->protocolIndependentId);
        IPCAFreeDeviceInfo(deviceInfo);
    }

    // Platform info.
    IPCAPlatformInfo *platformInfo;
    loopCount = 0;
    while (loopCount++ < 10)
    {
        status = IPCAGetPlatformInfo(m_deviceHandle, &platformInfo);
        if (status == IPCA_OK)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    EXPECT_EQ(IPCA_OK, IPCAGetPlatformInfo(m_deviceHandle, &platformInfo));
    EXPECT_STREQ(ELEVATOR_PLATFORM_ID, platformInfo->platformId);
    IPCAFreePlatformInfo(platformInfo);

    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::ConfirmResources()
{
    // Resource list in array of char*, expect elevator resource is one of them.
    char** resourcePathList;
    size_t resourcePathCount;
    EXPECT_EQ(IPCA_OK, IPCAGetResources(m_deviceHandle,
                            nullptr, nullptr, &resourcePathList, &resourcePathCount));
    EXPECT_LT(0U, resourcePathCount); // At least there must be elevator resource.
    bool elevatorResourceFound = false;
    for (size_t i = 0; i < resourcePathCount; i++)
    {
        std::string resourcePath = resourcePathList[i];
        if (resourcePath.find(ELEVATOR_KEYWORD) != std::string::npos)
        {
            elevatorResourceFound = true;
            break;
        }
    }
    EXPECT_TRUE(elevatorResourceFound);
    IPCAFreeStringArray(resourcePathList, resourcePathCount);
    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::ConfirmResourceTypes()
{
    // Function list in array of char*, expect elevator function is one of them.
    char** resourceTypes;
    size_t resourceTypeCount;
    EXPECT_EQ(IPCA_OK, IPCAGetResourceTypes(m_deviceHandle,
                            nullptr, &resourceTypes, &resourceTypeCount));
    EXPECT_LT(0U, resourceTypeCount);  // At least 1 resource type.
    bool elevatorResourceTypeFound = false;
    for (size_t i = 0; i < resourceTypeCount; i++)
    {
        std::string rt = resourceTypes[i];
        if (rt.find(ELEVATOR_KEYWORD) != std::string::npos)
        {
            elevatorResourceTypeFound = true;
            break;
        }
    }
    EXPECT_TRUE(elevatorResourceTypeFound);
    IPCAFreeStringArray(resourceTypes, resourceTypeCount);     // free the IPCA allocated memory.

    // Function list for a given resource.
    EXPECT_EQ(IPCA_RESOURCE_NOT_FOUND, IPCAGetResourceTypes(m_deviceHandle,
                                                "oic/fake", &resourceTypes, &resourceTypeCount));
    EXPECT_EQ(0U, resourceTypeCount);
    EXPECT_EQ(nullptr, resourceTypes);

    EXPECT_EQ(IPCA_OK, IPCAGetResourceTypes(m_deviceHandle,
                                    ELEVATOR_RESOURCE_PATH, &resourceTypes, &resourceTypeCount));
    EXPECT_LT(0U, resourceTypeCount);  // At least 1 function.
    elevatorResourceTypeFound = false;
    for (size_t i = 0; i < resourceTypeCount; i++)
    {
        std::string rt = resourceTypes[i];
        if (rt.find(ELEVATOR_KEYWORD) != std::string::npos)
        {
            elevatorResourceTypeFound = true;
            break;
        }
    }
    EXPECT_TRUE(elevatorResourceTypeFound);
    IPCAFreeStringArray(resourceTypes, resourceTypeCount);    // free the IPCA allocated memory.

    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::ConfirmResourceInterfaces()
{
    // There should be at least 2 interfaces (ELEVATOR_CO_PRIVATE_INTERFACE and DEFAULT_INTERFACE).
    char** resourceInterfaces;
    size_t resourceInterfaceCount;
    EXPECT_EQ(IPCA_OK, IPCAGetResourceInterfaces(m_deviceHandle,
                            nullptr, &resourceInterfaces, &resourceInterfaceCount));
    EXPECT_LT(1U, resourceInterfaceCount);

    // Should return only 1 resource with ELEVATOR_CO_PRIVATE_INTERFACE interface.
    char** resourcePathList;
    size_t resourcePathCount;
    EXPECT_EQ(IPCA_OK, IPCAGetResources(m_deviceHandle,
                            ELEVATOR_CO_PRIVATE_INTERFACE,
                            nullptr,
                            &resourcePathList,
                            &resourcePathCount));
    EXPECT_EQ(static_cast<size_t>(1), resourcePathCount);
    EXPECT_STREQ(ELEVATOR_CO_RESOURCE_PATH, resourcePathList[0]);
    IPCAFreeStringArray(resourcePathList, resourcePathCount);

    // Should return 0 resource with non existence interface.
    EXPECT_EQ(IPCA_OK, IPCAGetResources(m_deviceHandle,
                            ELEVATOR_RESOURCE_MADE_UP_INTERFACE,
                            nullptr,
                            &resourcePathList,
                            &resourcePathCount));
    EXPECT_EQ(0U, resourcePathCount);
    IPCAFreeStringArray(resourcePathList, resourcePathCount);

    // Should be at least 4 resouces with DEFAULT_INTERFACE created in ElevatorServer::Start().
    EXPECT_EQ(IPCA_OK, IPCAGetResources(m_deviceHandle,
                            OC_RSRVD_INTERFACE_DEFAULT,
                            nullptr,
                            &resourcePathList,
                            &resourcePathCount));
    EXPECT_LT(3U, resourcePathCount);
    IPCAFreeStringArray(resourcePathList, resourcePathCount);

    return IPCA_OK;
}

void IPCAElevatorClient::DiscoverElevator1Callback(
                            void* context,
                            IPCADeviceStatus deviceStatus,
                            const IPCADiscoveredDeviceInfo* discoveredDeviceInfo)
{
    OC_UNUSED(deviceStatus);
    OC_UNUSED(context);

    if (g_elevator1Name.compare(discoveredDeviceInfo->deviceName) == 0)
    {
        m_discoveredElevator1DeviceUris.clear();
        for (size_t i = 0; i < discoveredDeviceInfo->deviceUriCount; i++)
        {
            m_discoveredElevator1DeviceUris.push_back(discoveredDeviceInfo->deviceUris[i]);
        }

        m_discoveredElevator1DeviceId = discoveredDeviceInfo->deviceId;
        m_discoveredElevator1DeviceName = discoveredDeviceInfo->deviceId;
        m_elevator1Discovered = true;
        m_deviceDiscoveredCV.notify_all();
    }
}

void IPCA_CALL C_DiscoverElevator1Cb(
                            void* context,
                            IPCADeviceStatus deviceStatus,
                            const IPCADiscoveredDeviceInfo* discoveredDeviceInfo)
{
    IPCAElevatorClient* ipcaTest = (IPCAElevatorClient*)context;
    ipcaTest->DiscoverElevator1Callback(context, deviceStatus, discoveredDeviceInfo);
}

void IPCAElevatorClient::DiscoverElevator1()
{
    // Resource types to work with, an elevator device with vertical function (ipca.test.elevator)
    // and standard OCF resource type (carbon monoxide detection).
    const char* RequiredResourceTypes[] = {
        ELEVATOR_RESOURCE_TYPE,
        ELEVATOR_CO_RESOURCE_TYPE,
        ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_TYPE,
        ELEVATOR_RESOURCE_DELETE_TYPE
    };

   const int ResourceTypeCount = sizeof(RequiredResourceTypes) / sizeof(char*);

   // Start discovery.
   IPCAStatus status = IPCADiscoverDevices(
                               m_ipcaAppHandle,
                               &C_DiscoverElevator1Cb,
                               (void*)this,
                               RequiredResourceTypes,
                               ResourceTypeCount,
                               &m_deviceDiscoveryHandle);

   ASSERT_EQ(IPCA_OK, status);

   // Wait for the callback.
   std::unique_lock<std::mutex> lock(m_deviceDiscoveredCVMutex);
   m_deviceDiscoveredCV.wait_for(
       lock,
       std::chrono::seconds(10),
       [this]{ return IsElevator1Discovered(); });

   ASSERT_EQ(true, IsElevator1Discovered());
   ASSERT_EQ(IPCA_OK, IPCAOpenDevice(m_ipcaAppHandle,
                            m_discoveredElevator1DeviceId.c_str(), &m_deviceHandle));
}

void IPCAElevatorClient::SetPropertiesCallback(
                                IPCAStatus result,
                                void* context,
                                IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(propertyBagHandle);
    OC_UNUSED(context);
    OC_UNUSED(result);

    m_setPropertiesCallbackCalled = true;
    m_setPropertiesCompleteCV.notify_all();
}

void IPCAElevatorClient::GetPropertiesCallback(
                                IPCAStatus result,
                                void* context,
                                IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(context);

    m_getPropertiesCallbackCalled = true;
    m_statusOfLastCallback = result;

    if (propertyBagHandle != nullptr)
    {
        EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBagHandle,
                                ELEVATOR_PROPERTY_CURRENT_FLOOR, &m_currentFloor));
        EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBagHandle,
                                ELEVATOR_PROPERTY_DIRECTION, &m_direction));
        EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBagHandle,
                                ELEVATOR_PROPERTY_TARGET_FLOOR, &m_targetFloor));
    }

    m_getDataCompleteCbCV.notify_all();
}

void IPCAElevatorClient::CreateResourceCallback(
                                IPCAStatus result,
                                const char* newResourcePath,
                                IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(result);
    OC_UNUSED(propertyBagHandle);

    m_newResourcePath = (newResourcePath != nullptr) ? newResourcePath : "";
    m_createResourceCallbackCalled = true;
    m_createResourceCompleteCV.notify_all();
}

void IPCAElevatorClient::DeleteResourceCallback(IPCAStatus result)
{
    OC_UNUSED(result);

    m_deleteResourceCallbackCalled = true;
    m_deleteResourceCompleteCV.notify_all();
}

void IPCAElevatorClient::ResourceChangeNotificationCallback(
                                            IPCAStatus result,
                                            IPCAPropertyBagHandle propertyBagHandle)
{
    EXPECT_EQ(IPCA_OK, result);

    IPCAPropertyBagGetValueInt(propertyBagHandle,
            ELEVATOR_PROPERTY_CURRENT_FLOOR, &m_observedCurrentFloor);
    IPCAPropertyBagGetValueInt(propertyBagHandle,
            ELEVATOR_PROPERTY_DIRECTION, &m_observedDirection);
    IPCAPropertyBagGetValueInt(propertyBagHandle,
            ELEVATOR_PROPERTY_TARGET_FLOOR, &m_observedTargetFloor);

    m_resourceChangeCbCV.notify_all();
}

// This data structure is used to coordinate between C_ControlledRequestCompleteCallback() and
// RunCloseHandleTest().
typedef struct
{
    bool isInCallback;  // Set to true when callback for request is called.
    bool isTimeToCompleteCallback;  // Set to true when callback should complete.
    uint64_t completeCallbackTimestamp;  // Timestamp when isTimeToCompleteCallback is set.
    uint64_t closeHandleCompleteTimestamp;  // When IPCACloseHandleComplete() is called.
} ContextForCloseHandleTest;

// This callback coordinates with RunCloseHandleTest() on when to complete the callback.
void IPCA_CALL C_ControlledRequestCompleteCallback(
                            IPCAStatus result,
                            void* context,
                            IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(propertyBagHandle);

    ContextForCloseHandleTest* testContext = reinterpret_cast<ContextForCloseHandleTest*>(context);
    testContext->isInCallback = true;

    if ((result != IPCA_OK) && (result != IPCA_RESOURCE_CREATED) && (result != IPCA_RESOURCE_DELETED))
    {
        std::cout << "C_ControlledRequestCompleteCallback(): unsuccessful. result = " << result;
        std::cout << std::endl;
        return;
    }

    // Hang on here until the RunCloseHandleTest() indicates that it's time to complete
    // this callback.
    while (testContext->isTimeToCompleteCallback == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void IPCA_CALL C_ControlledCreateResourceCallback(
                        IPCAStatus result,
                        void* context,
                        const char* newResourcePath,
                        IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(newResourcePath);
    C_ControlledRequestCompleteCallback(result, context, propertyBagHandle);
}

void IPCA_CALL C_ControlledDeleteResourceCallback(IPCAStatus result, void* context)
{
    C_ControlledRequestCompleteCallback(result, context, NULL);
}

void IPCA_CALL C_ControlledDiscoverCallback(
                            void* context,
                            IPCADeviceStatus deviceStatus,
                            const IPCADiscoveredDeviceInfo* discoveredDeviceInfo)
{
    OC_UNUSED(deviceStatus);
    OC_UNUSED(discoveredDeviceInfo);
    C_ControlledRequestCompleteCallback(IPCA_OK, context, NULL);
}

void IPCA_CALL C_CloseHandleCallback(void* context)
{
    ContextForCloseHandleTest* testContext = reinterpret_cast<ContextForCloseHandleTest*>(context);
    testContext->closeHandleCompleteTimestamp = OICGetCurrentTime(TIME_IN_MS);
}

// This function coordinates with C_ControlledRequestCompleteCallback() to test behavior
// of IPCACloseHandle() when callback is in progress.
void IPCAElevatorClient::RunCloseHandleTest(IPCAHandle ipcaHandle, void* context)
{
    ContextForCloseHandleTest* testContext = reinterpret_cast<ContextForCloseHandleTest*>(context);

    // Wait for the callback. If the server did not respond, rely on IPCA to time out the pending
    // request.
    while (testContext->isInCallback == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the handle.  At this time C_ControlledRequestCompleteCallback() is looping
    // waiting for isTimeToCompleteCallback to turn to true.
    if (IPCA_OK != IPCACloseHandle(
                            ipcaHandle,
                            &C_CloseHandleCallback,
                            static_cast<void*>(testContext)))
    {
        std::cout << "RunCloseHandleTest(): IPCACloseHandle() failed. Request may have timed out.";
        std::cout << std::endl;
        return;
    }

    // Another call to IPCAClose() on closing handle should not succeed.
    ASSERT_NE(IPCA_OK, IPCACloseHandle(
                            ipcaHandle,
                            &C_CloseHandleCallback,
                            static_cast<void*>(testContext)));

    // Sleep an extra 100 milliseconds to make sure C_CloseHandleCallback is not called.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testContext->completeCallbackTimestamp = OICGetCurrentTime(TIME_IN_MS);
    testContext->isTimeToCompleteCallback = true;

    // Now wait for C_CloseHandleCallback() to complete.
    while (testContext->closeHandleCompleteTimestamp == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // The closeHandleCompleteTimestamp value must be equal to or greater than when
    // isTimeToCompleteCallback is set. It could be equal if C_ControlledRequestCompleteCallback()
    // processes the isTimeToCompleteCallbackas as soon as it's set.
    ASSERT_TRUE(testContext->closeHandleCompleteTimestamp >=
                testContext->completeCallbackTimestamp);
}

IPCAStatus IPCAElevatorClient::TestCloseHandleForGet()
{
    IPCAHandle getHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    // Call Get api.
    IPCAStatus status = IPCAGetProperties(
                                m_deviceHandle,
                                &C_ControlledRequestCompleteCallback,
                                reinterpret_cast<void*>(&testContext),
                                ELEVATOR_RESOURCE_PATH,
                                nullptr,
                                nullptr,
                                &getHandle);

    if (status != IPCA_OK)
    {
        return status;
    }

    RunCloseHandleTest(getHandle, &testContext);
    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::TestCloseHandleForSet()
{
    IPCAStatus status;
    IPCAHandle setHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    IPCAPropertyBagHandle propertyBagHandle;
    status = IPCAPropertyBagCreate(&propertyBagHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    status = IPCAPropertyBagSetValueInt(
                    propertyBagHandle,
                    ELEVATOR_PROPERTY_TARGET_FLOOR,
                    3 /* target floor, value is random */);

    if (status != IPCA_OK)
    {
        return status;
    }

    // Set properties.
    status = IPCASetProperties(
                    m_deviceHandle,
                    &C_ControlledRequestCompleteCallback,
                    reinterpret_cast<void*>(&testContext),
                    (const char*)ELEVATOR_RESOURCE_PATH,
                    nullptr,
                    (const char*)ELEVATOR_RESOURCE_TYPE,
                    propertyBagHandle,
                    &setHandle);

    if (status != IPCA_OK)
    {
        return status;
    }

    RunCloseHandleTest(setHandle, &testContext);
    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::TestCloseHandleForObserve()
{
    IPCAHandle observeHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    // Call Get api.
    IPCAStatus status = IPCAObserveResource(
                                   m_deviceHandle,
                                   &C_ControlledRequestCompleteCallback,
                                   reinterpret_cast<void*>(&testContext),
                                   ELEVATOR_RESOURCE_PATH,
                                   nullptr,
                                   &observeHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    // Set target floor so the server sends notifications.
    bool result;
    SetTargetFloor(10, &result);    // Set to floor 10
    if (result != true)
    {
        return IPCA_FAIL;
    }

    RunCloseHandleTest(observeHandle, &testContext);
    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::TestCloseHandleForCreate()
{
    IPCAStatus status;
    IPCAHandle createHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    IPCAPropertyBagHandle propertyBagHandle;
    status = IPCAPropertyBagCreate(&propertyBagHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    // Call Create api.
    status = IPCACreateResource(
                        m_deviceHandle,
                        &C_ControlledCreateResourceCallback,
                        reinterpret_cast<void*>(&testContext),
                        ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH,
                        nullptr,
                        nullptr,
                        propertyBagHandle,
                        &createHandle);

    if (status != IPCA_OK)
    {
        return status;
    }

    RunCloseHandleTest(createHandle, &testContext);
    return IPCA_OK;
}


IPCAStatus IPCAElevatorClient::TestCloseHandleForDelete()
{
    IPCAHandle deleteHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    // Call Delete api.
    IPCAStatus status = IPCADeleteResource(
                            m_deviceHandle,
                            &C_ControlledDeleteResourceCallback,
                            reinterpret_cast<void*>(&testContext),
                            (const char*)ELEVATOR_RESOURCE_DELETE_PATH,
                            &deleteHandle);

    if (status != IPCA_OK)
    {
        return status;
    }

    RunCloseHandleTest(deleteHandle, &testContext);
    return IPCA_OK;
}

IPCAStatus IPCAElevatorClient::TestCloseHandleForDiscover()
{
    IPCAHandle discoverDeviceHandle;
    ContextForCloseHandleTest testContext;
    memset(&testContext, 0, sizeof(ContextForCloseHandleTest));

    const char* RequiredResourceTypes[] = {
        ELEVATOR_RESOURCE_TYPE,
        ELEVATOR_CO_RESOURCE_TYPE,
        ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_TYPE,
        ELEVATOR_RESOURCE_DELETE_TYPE
    };

    const int ResourceTypeCount = sizeof(RequiredResourceTypes) / sizeof(char*);

   // Start discovery.
   IPCAStatus status = IPCADiscoverDevices(
                               m_ipcaAppHandle,
                               &C_ControlledDiscoverCallback,
                               reinterpret_cast<void*>(&testContext),
                               RequiredResourceTypes,
                               ResourceTypeCount,
                               &discoverDeviceHandle);

    if (status != IPCA_OK)
    {
        return status;
    }

    RunCloseHandleTest(discoverDeviceHandle, &testContext);
    return IPCA_OK;
}

// Callback from IPCA for IPCAGetProperties() call in TestMultipleCallsToCloseSameHandle().
void IPCA_CALL C_RequestComplete(
                    IPCAStatus result,
                    void* context,
                    IPCAPropertyBagHandle propertyBagHandle)
{
    OC_UNUSED(result);
    OC_UNUSED(context);
    OC_UNUSED(propertyBagHandle);
}

// IPCACloseHandleComplete() in TestMultipleCallsToCloseSameHandle().
void IPCA_CALL C_CloseHandleComplete(void* context)
{
    size_t* testContext = reinterpret_cast<size_t*>(context);
    (*testContext)++;
}

// Wait for *value to reach target or timeout. Return *value.
size_t C_WaitNumber(size_t* value, size_t target)
{
    const uint64_t MAX_WAIT_TIME_MS = 100;
    uint64_t beginTime = OICGetCurrentTime(TIME_IN_MS);
    while ((OICGetCurrentTime(TIME_IN_MS) - beginTime < MAX_WAIT_TIME_MS) && (*value != target))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return *value;
}

// Observe the right behavior of IPCACloseHandle() when handle is already closed.
IPCAStatus IPCAElevatorClient::TestMultipleCallsToCloseSameHandle()
{
    IPCAHandle getHandle;
    IPCAStatus status = IPCAGetProperties(
                                m_deviceHandle,
                                &C_RequestComplete,
                                NULL,
                                ELEVATOR_RESOURCE_PATH,
                                nullptr,
                                nullptr,
                                &getHandle);
    if (status != IPCA_OK)
    {
        return status;
    }

    size_t count = 0;

    // First IPCACloseHandle() should be succesful.
    EXPECT_EQ(IPCA_OK, IPCACloseHandle(getHandle, &C_CloseHandleComplete, static_cast<void*>(&count)));
    EXPECT_EQ(static_cast<size_t>(1), C_WaitNumber(&count, 1));

    // Subsequent IPCACloseHandle() on the same handle is expected to fail.
    // And the C_CloseHandleMultiple() is not called, i.e. count should not increase.
    EXPECT_EQ(IPCA_FAIL, IPCACloseHandle(getHandle, &C_CloseHandleComplete, static_cast<void*>(&count)));
    EXPECT_EQ(static_cast<size_t>(1), C_WaitNumber(&count, 2));

    EXPECT_EQ(IPCA_FAIL, IPCACloseHandle(getHandle, &C_CloseHandleComplete, static_cast<void*>(&count)));
    EXPECT_EQ(static_cast<size_t>(1), C_WaitNumber(&count, 3));

    return IPCA_OK;
}
