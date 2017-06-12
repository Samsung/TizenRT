//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "UnitTestHelper.h"

#include "RCSRemoteResourceObject.h"
#include "RCSDiscoveryManager.h"
#include "RCSResourceObject.h"
#include "RCSAddress.h"

#include "OCPlatform.h"

using namespace OIC::Service;
using namespace OC;
using namespace OC::OCPlatform;

typedef std::unique_ptr< RCSDiscoveryManager::DiscoveryTask > DiscoveryTaskPtr;

typedef OCStackResult (*OCFindResource)(const std::string&, const std::string&,
            OCConnectivityType, FindCallback);

constexpr char RESOURCE_URI[]{ "/a/TemperatureSensor" };
constexpr char RESOURCE_TYPE[]{ "resource.type" };
constexpr char SECOND_RESOURCETYPE[]{ "resource.type.second" };

#ifdef SECURED
const char * SVR_DB_FILE_NAME = "./oic_svr_db_re_client.dat";
//OCPersistent Storage Handlers
static FILE* client_open(const char * /*path*/, const char *mode)
{
    std::string file_name = SVR_DB_FILE_NAME;
#ifndef LOCAL_RUNNING
    file_name = "./service/resource-encapsulation/unittests/oic_svr_db_re_client.dat";
#endif
    return fopen(file_name.c_str(), mode);
}
#endif

void onResourceDiscovered(RCSRemoteResourceObject::Ptr)
{

}

class ScopedTask
{
public:
    ScopedTask(DiscoveryTaskPtr&& task) :
        m_task{ std::move(task) }
    {
    }

    ~ScopedTask()
    {
        if (m_task)
        {
            m_task->cancel();
        }
    }

    RCSDiscoveryManager::DiscoveryTask* operator->()
    {
        return m_task.get();
    }

    ScopedTask(ScopedTask&&) = default;
    ScopedTask& operator=(ScopedTask&&) = default;

private:
    DiscoveryTaskPtr m_task;
};

TEST(DiscoveryManagerTest, ThrowIfDiscoverWithEmptyCallback)
{
#ifdef SECURED
    OCPersistentStorage gps {client_open, fread, fwrite, fclose, unlink };
    OC::PlatformConfig cfg
    { OC::ServiceType::InProc, OC::ModeType::Both, "0.0.0.0", 0,
            OC::QualityOfService::LowQos, &gps };
    OC::OCPlatform::Configure(cfg);
#endif

    EXPECT_THROW(RCSDiscoveryManager::getInstance()->discoverResource(RCSAddress::multicast(), { }),
            RCSInvalidParameterException);
}

TEST(DiscoveryManagerTest, ThrowIfDiscoverWithMultipleTypesThatContainEmptyString)
{
    EXPECT_THROW(RCSDiscoveryManager::getInstance()->discoverResourceByTypes(
            RCSAddress::multicast(), { "a", "" }, onResourceDiscovered), RCSBadRequestException);
}

TEST(DiscoveryManagerTest, DiscoverInvokesFindResource)
{
    MockRepository mocks;
    mocks.ExpectCallFuncOverload(static_cast<OCFindResource>(findResource)).Match(
        [](const std::string& host, const std::string& resourceURI, OCConnectivityType, FindCallback)
        {
            return host.empty() && resourceURI == (std::string(RESOURCE_URI) + "?rt=" + RESOURCE_TYPE);
        }
    ).Return(OC_STACK_OK);

    ScopedTask task {RCSDiscoveryManager::getInstance()->discoverResourceByType(
            RCSAddress::multicast(), RESOURCE_URI, RESOURCE_TYPE, onResourceDiscovered)};
}

TEST(DiscoveryManagerTest, DiscoverWithMultipleTypesInvokesFindResourceMultipleTimes)
{
    MockRepository mocks;
    const std::vector< std::string > resourceTypes{ RESOURCE_TYPE, SECOND_RESOURCETYPE };
    size_t counter = 0;

    mocks.OnCallFuncOverload(static_cast<OCFindResource>(findResource)).Do(
        [&counter](const std::string&, const std::string&, OCConnectivityType, FindCallback)
        {
            ++counter;
            return OC_STACK_OK;
        }
    ).Return(OC_STACK_OK);

    ScopedTask task {RCSDiscoveryManager::getInstance()->discoverResourceByTypes(
            RCSAddress::multicast(), resourceTypes, onResourceDiscovered)};

    EXPECT_EQ(counter, resourceTypes.size());
}

TEST(DiscoveryManagerTest, TaskCanBeCanceled)
{
    ScopedTask aTask {RCSDiscoveryManager::getInstance()->discoverResource(RCSAddress::multicast(),
            onResourceDiscovered)};
    ScopedTask aTaskToBeCanceled {RCSDiscoveryManager::getInstance()->discoverResource(
            RCSAddress::multicast(), onResourceDiscovered)};

    aTaskToBeCanceled->cancel();

    ASSERT_FALSE(aTask->isCanceled());
    ASSERT_TRUE(aTaskToBeCanceled->isCanceled());
}

TEST(DiscoveryManagerTest, CallbackWouldNotBeCalledForSameRemoteResource)
{
    FindCallback callback;

    MockRepository mocks;
    mocks.OnCallFuncOverload(static_cast<OCFindResource>(findResource)).Do(
       [&callback](const std::string&, const std::string&, OCConnectivityType, FindCallback cb)
       {
           callback = cb;
           return OC_STACK_OK;
       }
   ).Return(OC_STACK_OK);

    ScopedTask aTask {RCSDiscoveryManager::getInstance()->discoverResource(RCSAddress::multicast(),
            onResourceDiscovered)};

    std::vector< std::string > interfaces{ "interface" };
    std::vector< std::string > resourceTypes{ "resource.type" };
    constexpr char fakeHost[] { "coap://127.0.0.1:1" };

    mocks.ExpectCallFunc(onResourceDiscovered);
    callback(OCPlatform::constructResourceObject(fakeHost, "/uri", OCConnectivityType::CT_ADAPTER_IP,
            true, interfaces, resourceTypes));

    mocks.NeverCallFunc(onResourceDiscovered);
    callback(OCPlatform::constructResourceObject(fakeHost, "/uri", OCConnectivityType::CT_ADAPTER_IP,
            true, interfaces, resourceTypes));
}
