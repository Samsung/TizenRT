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
#include "ipca.h"
#include "testelevatorserver.h"
#include "testelevatorclient.h"
#include "ipcatestdata.h"
#include "IPCAElevatorClient.h"

using namespace std;
using namespace std::placeholders;

// Implemented in ipca.dll.
void IPCA_CALL IPCASetUnitTestMode();

// IPCA test app info.
IPCAUuid IPCATestAppUuid = {
                              {0x84, 0x71, 0x88, 0x78, 0xe6, 0x91, 0x4b, 0xf4,
                               0xa9, 0x57, 0x04, 0xe0, 0x1b, 0x9b, 0x59, 0xd1}
                           };
char IPCATestAppName[] = "IPCA Unittests";

// IoTivity supports 1 server per instance.
ElevatorServer g_testElevator1;

// The elevator's name is suffixed with random number to differentiate it from elevators from
// other unit tests.
std::string g_elevator1Name;

/**
 * Start elevator 1.
 */
bool StartElevator1()
{
    // One time generation of the elevator's device name.
    if (g_elevator1Name.length() == 0)
    {
        char uuidString[37] = {};
        uint8_t uuid[UUID_SIZE] = {};
        OCGenerateUuid(uuid);
        OCConvertUuidToString(uuid, uuidString);

        g_elevator1Name = "IPCA Test Elevator ";

        // e.g., IPCA Test Elevator 2923be84-e16c-d6ae-5290-49f1f1bbe9eb
        g_elevator1Name.append(uuidString);
    }

    bool result = g_testElevator1.Start(g_elevator1Name);
    return result;
}

/**
 * Stop elevator 1.
 */
void StopElevator1()
{
    g_testElevator1.Stop();
}

// Start the test elevator once and use it through out the rest of the tests.
TEST(ElevatorServerStart, start)
{
    IPCASetUnitTestMode();
    ASSERT_TRUE(StartElevator1());
}

// Test IoTivity api directly before subsequent tests which use IPCA apis.
TEST(IoTivityDirect, IsIoTivityWorking)
{
    ElevatorClient elevatorClient;
    int loopCount;

    // Able to find the elevator server.
    elevatorClient.FindElevator(g_elevator1Name);
    loopCount = 0;
    while ((loopCount++ < 100) && (elevatorClient.IsElevatorFound() == false))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ASSERT_EQ(true, elevatorClient.IsElevatorFound());

    // Confirm able to set target floor using IoTivity client API and read the value back from the
    // elevator server.
    loopCount = 0;
    const int TARGET_FLOOR = 3;
    elevatorClient.SetTargetFloor(TARGET_FLOOR);    // SetTargetFloor() is synchronous.
    EXPECT_EQ(TARGET_FLOOR, g_testElevator1.GetCurrentFloor());

    // Confirm able to get current floor using IoTivity client API. The current floor should be
    // TARGET_FLOOR.
    int m_currentFloor;
    elevatorClient.GetCurrentFloor(&m_currentFloor);
    EXPECT_EQ(g_testElevator1.GetCurrentFloor(), m_currentFloor);

    // Confirm observation by setting the target floor to NEW_TARGET_FLOOR.
    // The client should receive current floor notification that it reaches the NEW_TARGET_FLOOR.
    const int NEW_TARGET_FLOOR = 8;
    bool returnValue = elevatorClient.StartObservation();
    EXPECT_TRUE(returnValue);
    elevatorClient.SetTargetFloor(NEW_TARGET_FLOOR);
    while ((loopCount++ < 20) && (elevatorClient.GetObservedCurrentFloor() != NEW_TARGET_FLOOR))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    EXPECT_EQ(NEW_TARGET_FLOOR, elevatorClient.GetObservedCurrentFloor());
    returnValue = elevatorClient.StopObservation();
    EXPECT_TRUE(returnValue);
}

/*
 * IPCA property bag tests.
 */
class IPCAPropertyBagTest : public testing::Test
{
    public:
        IPCAPropertyBagHandle m_propertyBagHandle;

    // Create a property bag containing an integar value.
    void CreateIntPropertyBag(IPCAPropertyBagHandle& propertyBagHandle, const char* key, int value)
    {
        ASSERT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle));
        ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle, key, value));
    }

    public:
        int m_intValue;
        double m_doubleValue;
        char* m_charPointerValue;

    protected:
        virtual void SetUp()
        {
            ASSERT_EQ(IPCA_OK, IPCAPropertyBagCreate(&m_propertyBagHandle));
        }
        virtual void TearDown()
        {
            IPCAPropertyBagDestroy(m_propertyBagHandle);
        }
};

TEST_F(IPCAPropertyBagTest, PropertyBagNonExistenceAttribute)
{
    EXPECT_EQ(IPCA_FAIL, IPCAPropertyBagGetValueInt(m_propertyBagHandle,
                                "NonexistenceAttribute", &m_intValue));
}

TEST_F(IPCAPropertyBagTest, PropertyBagInt)
{
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(m_propertyBagHandle, "IntValue", 3));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(m_propertyBagHandle, "IntValue", &m_intValue));
    EXPECT_EQ(3, m_intValue);
}

TEST_F(IPCAPropertyBagTest, PropertyBagDouble)
{
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueDouble(m_propertyBagHandle,
                                "DoubleValue", 12345678));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagGetValueDouble(m_propertyBagHandle,
                                "DoubleValue", &m_doubleValue));
    EXPECT_EQ(12345678, m_doubleValue);

    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueDouble(m_propertyBagHandle,
                                "DoubleValue", 1234.5678));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagGetValueDouble(m_propertyBagHandle,
                                "DoubleValue", &m_doubleValue));
    EXPECT_EQ(1234.5678, m_doubleValue);

    EXPECT_EQ(IPCA_FAIL, IPCAPropertyBagGetValueDouble(m_propertyBagHandle,
                                "doubleValue", &m_doubleValue)); // incorrect capital case
}

TEST_F(IPCAPropertyBagTest, PropertyBagBool)
{
    bool trueBool, falseBool;

    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueBool(m_propertyBagHandle,
                                "TrueBoolValue", true));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagGetValueBool(m_propertyBagHandle,
                                "TrueBoolValue", &trueBool));
    EXPECT_TRUE(trueBool);

    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueBool(m_propertyBagHandle,
                                "FalseBoolValue", false));
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagGetValueBool(m_propertyBagHandle,
                                "FalseBoolValue", &falseBool));
    EXPECT_FALSE(falseBool);
}

TEST_F(IPCAPropertyBagTest, PropertyBagString)
{
    ASSERT_EQ(IPCA_OK, IPCAPropertyBagSetValueString(m_propertyBagHandle,
                                "MyString", "Hello World"));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueString(m_propertyBagHandle,
                                "MyString", &m_charPointerValue));
    EXPECT_STREQ("Hello World", m_charPointerValue);
    IPCAPropertyBagFreeString(m_charPointerValue);
}

TEST_F(IPCAPropertyBagTest, PropertyBagPropertyBagType)
{
    IPCAPropertyBagHandle propertyBagHandle1, propertyBagHandle2, propertyBagHandle3;
    CreateIntPropertyBag(propertyBagHandle1, "IntValue1", 1);
    CreateIntPropertyBag(propertyBagHandle2, "IntValue2", 2);
    CreateIntPropertyBag(propertyBagHandle3, "IntValue3", 3);

    // Add propertyBagHandle1 to 3 to propertyBagHandle.
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag1", propertyBagHandle1));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag2", propertyBagHandle2));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag3", propertyBagHandle3));

    // The original is no longer needed.
    IPCAPropertyBagDestroy(propertyBagHandle1);
    IPCAPropertyBagDestroy(propertyBagHandle2);
    IPCAPropertyBagDestroy(propertyBagHandle3);

    // Read back each property bag object and compare with the original values.
    IPCAPropertyBagHandle propertyBag1;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag1", &propertyBag1));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag1,
                            "IntValue1", &m_intValue));
    EXPECT_EQ(1, m_intValue);
    IPCAPropertyBagDestroy(propertyBag1);

    IPCAPropertyBagHandle propertyBag2;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag2", &propertyBag2));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag2,
                            "IntValue2", &m_intValue));
    EXPECT_EQ(2, m_intValue);
    IPCAPropertyBagDestroy(propertyBag2);

    IPCAPropertyBagHandle propertyBag3;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(m_propertyBagHandle,
                            "PropertyBag3", &propertyBag3));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag3,
                            "IntValue3", &m_intValue));
    EXPECT_EQ(3, m_intValue);

    IPCAPropertyBagDestroy(propertyBag3);
}

TEST_F(IPCAPropertyBagTest, PropertyBagIntArrayType)
{
    // array of int
    int intArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueIntArray(m_propertyBagHandle,
                            "IntArray", intArray, 10));

    int* readBackIntArray;
    size_t readBackIntArraySize;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueIntArray(m_propertyBagHandle,
                            "IntArray", &readBackIntArray, &readBackIntArraySize));
    EXPECT_EQ(static_cast<size_t>(10), readBackIntArraySize);
    EXPECT_EQ(0, memcmp(intArray, readBackIntArray, readBackIntArraySize));

    IPCAPropertyBagFreeIntArray(readBackIntArray);
}

TEST_F(IPCAPropertyBagTest, PropertyBagDoubleArrayType)
{
    // array of double
    double doubleArray[] = {1.01, 2.02, 3.03, 4.04, 5.05, 6.06, 7.07, 8.08, 9.09, 10.010};
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueDoubleArray(m_propertyBagHandle,
                            "DoubleArray", doubleArray, 10));

    double* readBackDoubleArray;
    size_t readBackDoubleArraySize;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueDoubleArray(m_propertyBagHandle,
                            "DoubleArray", &readBackDoubleArray, &readBackDoubleArraySize));
    EXPECT_EQ(static_cast<size_t>(10), readBackDoubleArraySize);
    EXPECT_EQ(0, memcmp(doubleArray, readBackDoubleArray, readBackDoubleArraySize));

    IPCAPropertyBagFreeDoubleArray(readBackDoubleArray);
}

TEST_F(IPCAPropertyBagTest, PropertyBagBoolArrayType)
{
    // array of bool
    bool boolArray[] = {true, true, false, false, true, false, true, false, false, false};
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueBoolArray(m_propertyBagHandle,
                            "boolArray", boolArray, 10));

    bool* readBackBoolArray;
    size_t readBackBoolArraySize;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueBoolArray(m_propertyBagHandle,
                            "boolArray", &readBackBoolArray, &readBackBoolArraySize));
    EXPECT_EQ(static_cast<size_t>(10), readBackBoolArraySize);
    EXPECT_EQ(0, memcmp(boolArray, readBackBoolArray, readBackBoolArraySize));

    IPCAPropertyBagFreeBoolArray(readBackBoolArray);
}

TEST_F(IPCAPropertyBagTest, PropertyBagStringArrayType)
{
    // array of string
    const char* stringArray[] = {"hello world 1", "hello world 2", "hello world 3"};
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueStringArray(m_propertyBagHandle,
                            "stringArray", (const char**)stringArray, 3));

    char** readBackString;
    size_t readbackStringSize;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueStringArray(m_propertyBagHandle,
                            "stringArray", &readBackString, &readbackStringSize));
    ASSERT_EQ(static_cast<size_t>(3), readbackStringSize);
    EXPECT_STREQ(stringArray[0], readBackString[0]);
    EXPECT_STREQ(stringArray[1], readBackString[1]);
    EXPECT_STREQ(stringArray[2], readBackString[2]);
    IPCAPropertyBagFreeStringArray(readBackString, readbackStringSize);
}

TEST_F(IPCAPropertyBagTest, PropertyBagPropertyBagArrayType)
{
    IPCAPropertyBagHandle propertyBagHandle1, propertyBagHandle1A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle1));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle1A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle1A, "IntKey1", 1));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueString(propertyBagHandle1A, "StringKey1", "One"));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(propertyBagHandle1,
                                "values", propertyBagHandle1A));

    IPCAPropertyBagHandle propertyBagHandle2, propertyBagHandle2A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle2));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle2A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle2A, "IntKey2", 2));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueString(propertyBagHandle2A, "StringKey2", "Two"));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(propertyBagHandle2,
                                "values", propertyBagHandle2A));

    IPCAPropertyBagHandle propertyBagHandle3, propertyBagHandle3A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle3));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagCreate(&propertyBagHandle3A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueInt(propertyBagHandle3A, "IntKey3", 3));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValueString(propertyBagHandle3A, "StringKey3", "Three"));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBag(propertyBagHandle3,
                                "values", propertyBagHandle3A));

    IPCAPropertyBagHandle propertyBagArray[] = {
                propertyBagHandle1, propertyBagHandle2, propertyBagHandle3 };
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagSetValuePropertyBagArray(m_propertyBagHandle,
                                "PropertyBagArray", propertyBagArray, 3));

    // the original is no longer needed.
    IPCAPropertyBagDestroy(propertyBagHandle3);
    IPCAPropertyBagDestroy(propertyBagHandle3A);
    IPCAPropertyBagDestroy(propertyBagHandle2);
    IPCAPropertyBagDestroy(propertyBagHandle2A);
    IPCAPropertyBagDestroy(propertyBagHandle1);
    IPCAPropertyBagDestroy(propertyBagHandle1A);

    // Read back each property bag object and compare with the original values.
    IPCAPropertyBagHandle* propertyBagReadBackArray;
    size_t arrayCount;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBagArray(m_propertyBagHandle,
                                "PropertyBagArray", &propertyBagReadBackArray, &arrayCount));
    ASSERT_EQ(static_cast<size_t>(3), arrayCount);

    IPCAPropertyBagHandle propertyBag1 = propertyBagReadBackArray[0], propertyBag1A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(propertyBag1, "values", &propertyBag1A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag1A, "IntKey1", &m_intValue));
    EXPECT_EQ(1, m_intValue);
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueString(propertyBag1A,
                                "StringKey1", &m_charPointerValue));
    EXPECT_STREQ("One", m_charPointerValue);

    IPCAPropertyBagHandle propertyBag2 = propertyBagReadBackArray[1], propertyBag2A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(propertyBag2, "values", &propertyBag2A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag2A, "IntKey2", &m_intValue));
    EXPECT_EQ(2, m_intValue);
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueString(propertyBag2A,
                                "StringKey2", &m_charPointerValue));
    EXPECT_STREQ("Two", m_charPointerValue);

    IPCAPropertyBagHandle propertyBag3 = propertyBagReadBackArray[2], propertyBag3A;
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValuePropertyBag(propertyBag3, "values", &propertyBag3A));
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueInt(propertyBag3A, "IntKey3", &m_intValue));
    EXPECT_EQ(3, m_intValue);
    EXPECT_EQ(IPCA_OK, IPCAPropertyBagGetValueString(propertyBag3A,
                                "StringKey3", &m_charPointerValue));
    EXPECT_STREQ("Three", m_charPointerValue);

    IPCAPropertyBagDestroy(propertyBag1A);
    IPCAPropertyBagDestroy(propertyBag2A);
    IPCAPropertyBagDestroy(propertyBag3A);
}

/*
 * Test multiple IPCAOpen().
 */

class IPCAMiscTest : public testing::Test
{
    public:
        IPCAAppHandle m_ipcaAppHandle;
        IPCAAppHandle m_anotherIPCAAppHandle;
        IPCAAppInfo m_ipcaAppInfo;

        IPCAStatus DoAnotherIPCAOpenWithSameAppId()
        {
            return IPCAOpen(&m_ipcaAppInfo, IPCA_VERSION_1, &m_anotherIPCAAppHandle);
        }

        IPCAStatus DoAnotherIPCAOpenWithDifferentAppId()
        {
            IPCAAppInfo ipcaAppInfoExtra;
            IPCAAppHandle ipcaAppHandleExtra;
            IPCAUuid extraTestAppUuid = {
                                            {0x63, 0x72, 0xc0, 0xea, 0x1f, 0x12, 0x11, 0xe7,
                                             0x93, 0xae, 0x92, 0x36, 0x1f, 0x00, 0x26, 0x71}
                                        };

            ipcaAppInfoExtra = { extraTestAppUuid, IPCATestAppName, "1.0.0", "Microsoft" };
            return IPCAOpen(&ipcaAppInfoExtra, IPCA_VERSION_1, &ipcaAppHandleExtra);
        }

    protected:
        virtual void SetUp()
        {
            m_anotherIPCAAppHandle = NULL;
            m_ipcaAppInfo = { IPCATestAppUuid, IPCATestAppName, "1.0.0", "Microsoft" };
            ASSERT_EQ(IPCA_OK, IPCAOpen(&m_ipcaAppInfo, IPCA_VERSION_1, &m_ipcaAppHandle));
        }
        virtual void TearDown()
        {
            if (m_ipcaAppHandle != NULL)
            {
                IPCAClose(m_ipcaAppHandle);
                m_ipcaAppHandle = NULL;
            }

            if (m_anotherIPCAAppHandle != NULL)
            {
                IPCAClose(m_anotherIPCAAppHandle);
                m_anotherIPCAAppHandle = NULL;
            }
        }
};

TEST_F(IPCAMiscTest, MultipleIPCAOpen)
{
    EXPECT_EQ(IPCA_OK, DoAnotherIPCAOpenWithSameAppId());
    EXPECT_EQ(IPCA_ALREADY_OPENED, DoAnotherIPCAOpenWithDifferentAppId());
}

TEST_F(IPCAMiscTest, IPCAOpenShouldBeAllowedAfterIPCAClose)
{
    IPCAClose(m_ipcaAppHandle);
    m_ipcaAppHandle = NULL;

    ASSERT_EQ(IPCA_OK, DoAnotherIPCAOpenWithSameAppId());
}

/*
 * IPCADiscoverDevices() must be called before IPCAOpenDevice().
 */
TEST_F(IPCAMiscTest, ShouldFailOpenDeviceWithoutDiscoveryFirst)
{
    IPCADeviceHandle deviceHandle;

    // UUID is just random.
    EXPECT_EQ(IPCA_DEVICE_NOT_DISCOVERED, IPCAOpenDevice(m_ipcaAppHandle,
                                                "eee01972-8a91-11e6-ae22-56b6b6499611",
                                                &deviceHandle));
}

TEST_F(IPCAElevatorClient, DiscoveryShouldFindElevatorServer)
{
    EXPECT_TRUE(IsElevator1Discovered());
}

TEST_F(IPCAElevatorClient, DiscoveryShouldFindDeviceAndPlatformInfo)
{
    EXPECT_EQ(IPCA_OK, ConfirmDeviceAndPlatformInfo());
}

TEST_F(IPCAElevatorClient, DiscoveryShouldFindElevatorResources)
{
    EXPECT_EQ(IPCA_OK, ConfirmResources());
}

TEST_F(IPCAElevatorClient, DiscoveryShouldFindTargetedResourceTypes)
{
    EXPECT_EQ(IPCA_OK, ConfirmResourceTypes());
}

TEST_F(IPCAElevatorClient, ShouldBeAbleToFilterOnResourceInterface)
{
    EXPECT_EQ(IPCA_OK, ConfirmResourceInterfaces());
}

TEST_F(IPCAElevatorClient, SuccessfullyGetDataFromElevatorServer)
{
    // Directly set target floor of the elevator to 8.
    g_testElevator1.SetTargetFloor(8);

    int elevatorTargetFloor;
    bool result;

    GetTargetFloor(&elevatorTargetFloor, &result);
    ASSERT_TRUE(result);
    EXPECT_EQ(8, elevatorTargetFloor);
}

TEST_F(IPCAElevatorClient, ShouldFailGetUnknownResource)
{
    EXPECT_EQ(IPCA_RESOURCE_NOT_FOUND, GetUnknownResource());
}

TEST_F(IPCAElevatorClient, ShouldFailSetUnknownResource)
{
    EXPECT_EQ(IPCA_RESOURCE_NOT_FOUND, SetUnknownResource());
}

TEST_F(IPCAElevatorClient, ShouldFailSetWithUnknownInterface)
{
    size_t incorrectInterfaceCount = g_testElevator1.GetIncorrectInterfaceCount();
    EXPECT_EQ(true, SetUnknoownInterface());
    EXPECT_EQ((incorrectInterfaceCount + 1), g_testElevator1.GetIncorrectInterfaceCount());
}

TEST_F(IPCAElevatorClient, SuccessfullySetElevatorServerProperties)
{
    g_testElevator1.SetTargetFloor(1); // Set to known target floor.

    bool result;
    SetTargetFloor(8, &result); // Set it to 8th floor.
    ASSERT_EQ(true, result);

    // Now read back directly from the elevator.
    int newTargetFloor = g_testElevator1.GetTargetFloor();
    EXPECT_EQ(8, newTargetFloor);
}

TEST_F(IPCAElevatorClient, SuccessfullyCreateAndDeleteResources)
{
    // Do a few rounds of create resource with relative path.
    size_t beforeCreateCount, afterCreateCount;
    for (int i = 0 ; i < 3 ; i++)
    {
        beforeCreateCount = g_testElevator1.GetRelativePathResourceCreateCount();
        CreateResourceRelativePath();
        afterCreateCount = g_testElevator1.GetRelativePathResourceCreateCount();
        ASSERT_EQ(beforeCreateCount + 1, afterCreateCount);
        EXPECT_STREQ(ELEVATOR_RESOURCE_NEW_RESOURCE_PATH, GetNewResourcePath());
    }

    // Create new resource that results in resource with long resource path.
    beforeCreateCount = g_testElevator1.GetRelativePathResourceCreateCount();
    CreateResourceLongRelativePath();
    afterCreateCount = g_testElevator1.GetRelativePathResourceCreateCount();
    ASSERT_EQ(beforeCreateCount, afterCreateCount); // resource should not be created.

    // Delete resource
    size_t beforeDeleteCount, afterDeleteCount;
    for (int i = 0 ; i < 3 ; i++)
    {
        beforeDeleteCount = g_testElevator1.GetDeleteResourceCount();
        DeleteResource();  // the test elevator simply increments count,
                           // therefore can be deleted multiple times
        afterDeleteCount = g_testElevator1.GetDeleteResourceCount();
        ASSERT_EQ(beforeDeleteCount + 1, afterDeleteCount);
    }
}

TEST_F(IPCAElevatorClient, SuccessfullyReceiveResourceChangeNotifications)
{
    // Set to known target floor.
    g_testElevator1.SetTargetFloor(1);

    // Start observing
    ASSERT_EQ(true, StartObserve());

    // Use IPCA to set the target floor.
    bool result;
    SetTargetFloor(10, &result);    // Set to floor 10
    ASSERT_EQ(true, result);

    // Wait until observed current floor is set to targeted floor.
    // Outstanding requests should time out in 247 seconds (EXCHANGE_LIFETIME) per rfc 7252.
    std::unique_lock<std::mutex> lock(m_resourceChangeCbMutex);
    m_resourceChangeCbCV.wait_for(
            lock,
            std::chrono::seconds(247),
            [this] { return GetObservedCurrentFloor() == 10; });

    EXPECT_EQ(10, GetObservedCurrentFloor());   // check it is at floor 10.

    StopObserve();
}

TEST_F(IPCAElevatorClient, SuccessfulFactoryReset)
{
    EXPECT_EQ(IPCA_OK, FactoryResetElevator());
}

TEST_F(IPCAElevatorClient, SuccessfulReboot)
{
    EXPECT_EQ(IPCA_OK, RebootElevator());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForGet)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForGet());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForSet)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForSet());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForCreate)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForCreate());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForDelete)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForDelete());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForObserve)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForObserve());
}

TEST_F(IPCAElevatorClient, TestCloseHandleTimingForDiscover)
{
    EXPECT_EQ(IPCA_OK, TestCloseHandleForDiscover());
}

TEST_F(IPCAElevatorClient, TestMultipleCallsToCloseHandle)
{
    EXPECT_EQ(IPCA_OK, TestMultipleCallsToCloseSameHandle());
}

TEST(ElevatorServerStop, Stop)
{
    StopElevator1();
}
