/******************************************************************
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

#ifndef IPCA_ELEVATOR_CLIENT_H
#define IPCA_ELEVATOR_CLIENT_H

/**
 * Class that asssists testing IPCA apis.
 */
class IPCAElevatorClient : public testing::Test
{
public:
    // Discovery functionalities
    bool IsElevator1Discovered() { return m_elevator1Discovered; }
    IPCAStatus ConfirmDeviceAndPlatformInfo();
    IPCAStatus ConfirmResources();
    IPCAStatus ConfirmResourceTypes();
    IPCAStatus ConfirmResourceInterfaces();

    // Use IPCA to get the elevator's current floor.
    void GetCurrentFloor(int* currentFloor, bool* result);
    void GetTargetFloor(int* targetFloor, bool* result);
    void SetTargetFloor(int targetFloor, bool* result);
    void CreateResourceRelativePath();
    void CreateResourceLongRelativePath();
    const char* GetNewResourcePath() { return m_newResourcePath.c_str(); }
    void CreateResouceExplicitPath();
    void DeleteResource();

    // These are values updated by notifications.
    int GetObservedCurrentFloor() { return m_observedCurrentFloor; }
    int GetObservedTargetFloor() { return m_observedTargetFloor; }
    int GetObservedDirection() { return m_observedDirection; }

    // Use IPCA to start/stop observing elevator resource.
    IPCAHandle m_observeHandle;
    std::mutex m_resourceChangeCbMutex;
    std::condition_variable m_resourceChangeCbCV;
    bool StartObserve();
    void StopObserve();

    // Helper functions
    IPCAStatus FactoryResetElevator();
    IPCAStatus RebootElevator();

    void DiscoverElevator1();
    void DiscoverElevator1Callback(
                void* context,
                IPCADeviceStatus deviceStatus,
                const IPCADiscoveredDeviceInfo* discoveredDeviceInfo);

    void GetPropertiesCallback(
                IPCAStatus result,
                void* context,
                IPCAPropertyBagHandle propertyBagHandle);

    void SetPropertiesCallback(
            IPCAStatus result,
            void* context,
            IPCAPropertyBagHandle propertyBagHandle);

    void CreateResourceCallback(
            IPCAStatus result,
            const char* newResourcePath,
            IPCAPropertyBagHandle propertyBagHandle);

    void DeleteResourceCallback(IPCAStatus result);

    void ResourceChangeNotificationCallback(
        IPCAStatus result,
        IPCAPropertyBagHandle propertyBagHandle);

    // IPCACloseHandle() tests.
    IPCAStatus TestCloseHandleForGet();
    IPCAStatus TestCloseHandleForSet();
    IPCAStatus TestCloseHandleForObserve();
    IPCAStatus TestCloseHandleForCreate();
    IPCAStatus TestCloseHandleForDelete();
    IPCAStatus TestCloseHandleForDiscover();
    IPCAStatus TestMultipleCallsToCloseSameHandle();

    // Failure cases.
    IPCAStatus GetUnknownResource();
    IPCAStatus SetUnknownResource();
    bool SetUnknoownInterface();

private:
    IPCAAppHandle m_ipcaAppHandle;
    IPCAHandle m_deviceDiscoveryHandle;
    IPCADeviceHandle m_deviceHandle;

    int m_currentFloor;
    int m_targetFloor;
    int m_direction;

    int m_observedCurrentFloor;
    int m_observedDirection;
    int m_observedTargetFloor;

    // Used by IPCADiscoverDevices() tests.
    bool m_elevator1Discovered;
    std::string m_discoveredElevator1DeviceName;
    std::string m_discoveredElevator1DeviceId;
    std::vector<std::string> m_discoveredElevator1DeviceUris;
    std::mutex m_deviceDiscoveredCVMutex;
    std::condition_variable m_deviceDiscoveredCV;  // conditional variable to wake up thread waiting
                                                   // for device discovered callback.

    // Used by IPCAGetProperties() tests.
    bool m_getPropertiesCallbackCalled;
    IPCAStatus m_statusOfLastCallback;    // the IPCAStatus of the last callback.
    std::mutex m_getDataCompleteCbMutex;
    std::condition_variable m_getDataCompleteCbCV;

    // Used by IPCASetProperties() tests.
    bool m_setPropertiesCallbackCalled;
    std::mutex m_setPropertiesCompleteCbMutex;
    std::condition_variable m_setPropertiesCompleteCV;

    // Used by IPCACreateResource() tests
    std::string m_newResourcePath;
    bool m_createResourceCallbackCalled;
    std::mutex m_createResourceCompleteCbMutex;
    std::condition_variable m_createResourceCompleteCV;

    // Used by IPCADeleteResource() tests
    bool m_deleteResourceCallbackCalled;
    std::mutex m_deleteResourceCompleteCbMutex;
    std::condition_variable m_deleteResourceCompleteCV;

    bool GetElevatorProperties();
    bool SetElevatorProperties(IPCAPropertyBagHandle propertyBagHandle);
    bool CreateElevatorResource(
            std::string resourcePath, IPCAPropertyBagHandle propertyBagHandle);
    bool DeleteElevatorResource();

    // Test timing of IPCACloseHandle() for the ipcaHandle.
    void RunCloseHandleTest(IPCAHandle ipcaHandle, void* testContext);

protected:
    virtual void SetUp();
    virtual void TearDown();
};

#endif // IPCA_ELEVATOR_CLIENT_H
