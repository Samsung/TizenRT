//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <gtest/gtest.h>
#include <HippoMocks/hippomocks.h>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <chrono>

#include "ESEnrolleeSimulator.h"
#include "escommon.h"
#include "ESRichCommon.h"
#include "EasySetup.hpp"
#include "RemoteEnrollee.h"

#include "ESEnrolleeCommon.h"
#include "easysetup.h"

#include "ocrandom.h"
#include "cainterface.h"
#include "OCPlatform.h"

using namespace OC;
using namespace OIC::Service;

namespace
{
    std::atomic_bool g_isStartedStack(false);

    std::chrono::milliseconds g_waitForResponse(1000);

    std::condition_variable responseCon;
    std::mutex mutexForCondition;

    ESEnrolleeSimulator g_enrolleeSimul;
    std::shared_ptr<RemoteEnrollee> g_remoteEnrollee;
}

class TestWithMock: public testing::Test
{
public:
    MockRepository mocks;

protected:
    virtual ~TestWithMock() noexcept(noexcept(std::declval<Test>().~Test()))
    {
    }

    virtual void TearDown()
    {
        try
        {
            mocks.VerifyAll();
        }
        catch (...)
        {
            mocks.reset();
            throw;
        }
    }
};

class EasysetupMediatorTest : public TestWithMock
{
public:
    std::shared_ptr<OC::OCResource> m_enrolleeResource;

public:
    EasysetupMediatorTest() = default;
    ~EasysetupMediatorTest() = default;

    std::shared_ptr<OC::OCResource> CreateNotEasySetupResource()
    {
        OCConnectivityType connectivityType = CT_DEFAULT;
        std::vector<std::string> types = {"oic.r.noteasysetup"};
        std::vector<std::string> ifaces = {DEFAULT_INTERFACE};

        return OCPlatform::constructResourceObject("coap://192.168.1.2:5000",
                                                   "/NotEasySetupResURI",
                                                   connectivityType,
                                                   false,
                                                   types,
                                                   ifaces);
    }

    void discoverRemoteEnrollee()
    {
        std::string uri = std::string("/oic/res?rt=") + OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OC::OCPlatform::findResource("", uri,
                OCConnectivityType::CT_DEFAULT,
                std::bind(&EasysetupMediatorTest::discoverRemoteEnrolleeCb,
                            this, std::placeholders::_1));

        std::unique_lock< std::mutex > lock{ mutexForCondition };
        responseCon.wait_for(lock, g_waitForResponse);
    }

protected:
    void SetUp()
    {
        TestWithMock::SetUp();
        if (g_isStartedStack == false)
        {
            if (OCInit(NULL, 0, OC_CLIENT_SERVER) != OC_STACK_OK)
            {
                printf("OCStack init error!!\n");
                return;
            }
            g_enrolleeSimul.initEnrollee();
            g_isStartedStack = true;
        }
    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }

private:
    bool isValidResourceToTest(std::shared_ptr<OC::OCResource> resource)
    {
        if((resource->connectivityType() & CT_ADAPTER_TCP) == CT_ADAPTER_TCP)
        {
            return false;
        }

        CAEndpoint_t *tempInfo = NULL;
        uint32_t tempSize = 0;

        CAResult_t res = CAGetNetworkInformation(&tempInfo, &tempSize);
        if (CA_STATUS_OK != res || NULL == tempInfo || 0 >= tempSize)
        {
            free(tempInfo);
            return false;
        }

        for (uint32_t index = 0; index  < tempSize; index++)
        {
            if (CA_ADAPTER_IP == tempInfo[index].adapter)
            {
                if(resource->host().find(tempInfo[index].addr) != std::string::npos &&
                    resource->host().find(std::to_string(tempInfo[index].port).c_str()) != std::string::npos)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void discoverRemoteEnrolleeCb(std::shared_ptr<OC::OCResource> resource)
    {
        if(!isValidResourceToTest(resource))
        {
            return ;
        }

        if(!resource->getResourceTypes().at(0).compare(OC_RSRVD_ES_RES_TYPE_EASYSETUP))
        {
            m_enrolleeResource = resource;
        }
    }
};

TEST_F(EasysetupMediatorTest, createremoteenrolleeFailedWithNotEasySetupResource)
{
    auto remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(CreateNotEasySetupResource());

    EXPECT_EQ(nullptr, remoteEnrollee);
}

TEST_F(EasysetupMediatorTest, createremoteenrolleeSucceedWithEasySetupResource)
{
    discoverRemoteEnrollee();
    g_remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(m_enrolleeResource);

    ASSERT_NE(nullptr, g_remoteEnrollee);
}

class GetConfigurationTest : public EasysetupMediatorTest
{
public:
    GetConfigurationTest() = default;
    ~GetConfigurationTest() = default;

    static void onGetConfigurationCb(shared_ptr< GetConfigurationStatus > /*status*/)
    {
    }

protected:
    void SetUp()
    {
        TestWithMock::SetUp();
    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }
};

TEST_F(GetConfigurationTest, ThrowExceptionWhenGetConfigurationFailedByCallbackIsNull)
{
    EXPECT_ANY_THROW(g_remoteEnrollee->getConfiguration(nullptr));
}

TEST_F(GetConfigurationTest, GetConfigurationSucceed)
{
    bool isWellConstructed = false;

    g_enrolleeSimul.setDeviceProperty();

    mocks.ExpectCallFunc(onGetConfigurationCb).Do(
        [&isWellConstructed](std::shared_ptr< GetConfigurationStatus > status)
        {
            if(status->getESResult() == ES_OK)
            {
                EnrolleeConf conf = status->getEnrolleeConf();
                if(!conf.getWiFiModes().empty())
                {
                    if(conf.getWiFiModes().at(0) == WIFI_11G &&
                        conf.getWiFiFreq() == WIFI_5G &&
                        !strcmp(conf.getDeviceName().c_str(), "Test Device") &&
                        !strcmp(conf.getModelNumber().c_str(), "Test Model Number"))
                    {
                        isWellConstructed = true;
                    }
                    cout << "getDeviceName : " << conf.getDeviceName().c_str() << endl;
                    cout << "getModelNumber : " << conf.getModelNumber().c_str() << endl;
                }
            }
        });

    g_remoteEnrollee->getConfiguration(onGetConfigurationCb);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_TRUE(isWellConstructed);
}

class GetStatusTest : public EasysetupMediatorTest
{

public:
    GetStatusTest() = default;
    ~GetStatusTest() = default;

    static void onGetStatusCb(shared_ptr< GetEnrolleeStatus > /*status*/)
    {
    }

protected:
    void SetUp()
    {
        TestWithMock::SetUp();
    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }
};

TEST_F(GetStatusTest, ThrowExceptionWhenGetStatusFailedByCallbackIsNull)
{
    EXPECT_ANY_THROW(g_remoteEnrollee->getStatus(nullptr));
}

TEST_F(GetStatusTest, GetStatusSucceed)
{
    g_enrolleeSimul.setESState();
    g_enrolleeSimul.setESErrorCode();

    bool isWellConstructed = false;
    mocks.ExpectCallFunc(onGetStatusCb).Do(
        [&isWellConstructed](std::shared_ptr< GetEnrolleeStatus > status)
        {
            if(status->getESResult() == ES_OK)
            {
                EnrolleeStatus enrolleeStatus = status->getEnrolleeStatus();

                if(enrolleeStatus.getProvStatus() == ES_STATE_CONNECTED_TO_ENROLLER &&
                   enrolleeStatus.getLastErrCode() == ES_ERRCODE_NO_INTERNETCONNECTION)
                {
                    isWellConstructed = true;
                }
            }
        });

    g_remoteEnrollee->getStatus(onGetStatusCb);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_TRUE(isWellConstructed);
}

class ProvisionDevicePropertiesTest : public EasysetupMediatorTest
{

public:
    ProvisionDevicePropertiesTest() = default;
    ~ProvisionDevicePropertiesTest() = default;

    static void deviceProvisioningStatusCb(
                                            shared_ptr< DevicePropProvisioningStatus > /*status*/)
    {
    }

protected:
    void SetUp()
    {
        TestWithMock::SetUp();
    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }
};

TEST_F(ProvisionDevicePropertiesTest,
          ThrowExceptionWhenProvisionDeviceProperiesFailedByCallbackIsNull)
{
    DeviceProp devProp;
    devProp.setWiFiProp("Iotivity_SSID", "Iotivity_PWD", WPA2_PSK, TKIP_AES);
    devProp.setDevConfProp("korean", "Korea", "Location");

    EXPECT_ANY_THROW(g_remoteEnrollee->provisionDeviceProperties(devProp, nullptr));
}

TEST_F(ProvisionDevicePropertiesTest,
          ThrowExceptionWhenProvisionDeviceProperiesFailedWithoutSSID)
{
    DeviceProp devProp;
    devProp.setWiFiProp("", "Iotivity_PWD", WPA2_PSK, TKIP_AES);
    devProp.setDevConfProp("korean", "Korea", "Location");
    EXPECT_ANY_THROW(g_remoteEnrollee->provisionDeviceProperties(devProp,
                                                                 deviceProvisioningStatusCb));
}

TEST_F(ProvisionDevicePropertiesTest,
          ProvisionDeviceProperiesSucceed)
{
    DeviceProp devProp;
    devProp.setWiFiProp("Iotivity_SSID", "Iotivity_PWD", WPA2_PSK, TKIP_AES);
    devProp.setDevConfProp("korean", "Korea", "Location");

    int cntForReceivedCallbackWithSuccess = 0;

    mocks.OnCallFunc(deviceProvisioningStatusCb).Do(
        [&cntForReceivedCallbackWithSuccess]
        (std::shared_ptr< DevicePropProvisioningStatus > status)
        {
            if(status->getESResult() == ES_OK)
            {
               cntForReceivedCallbackWithSuccess++;
            }

        });

    g_remoteEnrollee->provisionDeviceProperties(devProp, deviceProvisioningStatusCb);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(cntForReceivedCallbackWithSuccess, 1);
}

class ProvisionCloudPropertiesTest : public EasysetupMediatorTest
{

public:
    ProvisionCloudPropertiesTest() = default;
    ~ProvisionCloudPropertiesTest() = default;

    static void cloudPropProvStatusCb(shared_ptr< CloudPropProvisioningStatus > /*status*/)
    {
    }

protected:
    void SetUp()
    {
        TestWithMock::SetUp();
    }

    void TearDown()
    {
        TestWithMock::TearDown();
    }
};

TEST_F(ProvisionCloudPropertiesTest,
          ThrowExceptionWhenProvisionCloudPropertiesFailedByCallbackIsNull)
{
    CloudProp cloudProp;
    cloudProp.setCloudProp("authCode", "authProvider", "ciServer");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");

    EXPECT_ANY_THROW(g_remoteEnrollee->provisionCloudProperties(cloudProp, nullptr));
}

TEST_F(ProvisionCloudPropertiesTest,
          ThrowExceptionWhenProvisionCloudPropertiesFailedWithoutAuthCode)
{
    CloudProp cloudProp;
    cloudProp.setCloudProp("", "authProvider", "ciServer");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");
    EXPECT_ANY_THROW(g_remoteEnrollee->provisionCloudProperties(cloudProp,
                                                                cloudPropProvStatusCb));
}

TEST_F(ProvisionCloudPropertiesTest,
          ThrowExceptionWhenProvisionCloudPropertiesFailedWithoutAuthProvider)
{
    CloudProp cloudProp;
    cloudProp.setCloudProp("authCode", "", "ciServer");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");
    EXPECT_ANY_THROW(g_remoteEnrollee->provisionCloudProperties(cloudProp,
                                                                cloudPropProvStatusCb));
}

TEST_F(ProvisionCloudPropertiesTest,
          ThrowExceptionWhenProvisionCloudPropertiesFailedWithoutCIServer)
{
    CloudProp cloudProp;
    cloudProp.setCloudProp("authCode", "authProvider", "");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");
    EXPECT_ANY_THROW(g_remoteEnrollee->provisionCloudProperties(cloudProp,
                                                                cloudPropProvStatusCb));
}

TEST_F(ProvisionCloudPropertiesTest, ProvisionCloudPropertiesSucceed)
{
    CloudProp cloudProp;
    cloudProp.setCloudProp("authCode", "authProvider", "ciServer");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");

    int cntForReceivedCallbackWithSuccess = 0;

    mocks.OnCallFunc(cloudPropProvStatusCb).Do(
        [& cntForReceivedCallbackWithSuccess](std::shared_ptr< CloudPropProvisioningStatus > status)
        {
            if(status->getESResult() == ES_OK)
            {
               cntForReceivedCallbackWithSuccess++;
            }
        });

    g_remoteEnrollee->provisionCloudProperties(cloudProp, cloudPropProvStatusCb);

    std::unique_lock< std::mutex > lock{ mutexForCondition };
    responseCon.wait_for(lock, g_waitForResponse);

    EXPECT_EQ(cntForReceivedCallbackWithSuccess, 1);

    ESTerminateEnrollee();
}
