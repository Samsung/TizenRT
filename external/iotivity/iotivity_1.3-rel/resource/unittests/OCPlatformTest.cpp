//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#include <OCPlatform.h>
#include <OCApi.h>
#include <oic_malloc.h>
#include <iotivity_debug.h>
#include <gtest/gtest.h>

namespace OCPlatformTest
{
    using namespace OC;

    static const char* SVR_DB_FILE_NAME = "./oic_svr_db_server.dat";
    const OCResourceHandle HANDLE_ZERO = 0;
    const std::string gResourceTypeName = "core.res";
    const std::string gResourceInterface = DEFAULT_INTERFACE;
    const uint8_t gResourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
    OCResourceHandle gResourceHandle;

    //OCPersistent Storage Handlers
    static FILE* client_open(const char *path, const char *mode)
    {
        if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
        {
            std::cout << "<===Opening SVR DB file = './oic_svr_db_client.dat' with mode = '" << mode
                << "' " << std::endl;
            return fopen(SVR_DB_FILE_NAME, mode);
        }
        else
        {
            return fopen(path, mode);
        }
    }
    OCPersistentStorage gps {client_open, fread, fwrite, fclose, unlink };

    // Callbacks
    OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> /*request*/)
    {
        return OC_EH_OK;
    }

    void foundResource(std::shared_ptr<OCResource> /*resource*/)
    {
    }

    void receivedDeviceInfo(const OCRepresentation& /*rep*/)
    {
    }

    void presenceHandler(OCStackResult /*result*/,
            const unsigned int /*nonce*/, const std::string& /*hostAddress*/)
    {
    }

#ifdef WITH_CLOUD
    void onObserve(const HeaderOptions, const OCRepresentation&, const int&, const int&)
    {
    }
#endif

    //Helper methods
    void DeleteStringLL(OCStringLL* ll)
    {
        if (!ll)
        {
            return;
        }

        DeleteStringLL(ll->next);
        delete[] ll->value;
        OICFree(ll);
    }

    void DeleteDeviceInfo(OCDeviceInfo deviceInfo)
    {
        delete[] deviceInfo.deviceName;
        DeleteStringLL(deviceInfo.types);
        delete[] deviceInfo.specVersion;
        DeleteStringLL(deviceInfo.dataModelVersions);
    }

    void DuplicateString(char ** targetString, std::string sourceString)
    {
        *targetString = new char[sourceString.length() + 1];
        strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
    }

    bool OCResourcePayloadAddStringLL(OCStringLL **stringLL, std::string value)
    {
        if (!stringLL)
        {
            return false;
        }

        char *dup = NULL;
        DuplicateString(&dup, value);
        if (!*stringLL)
        {
            *stringLL = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
            (*stringLL)->value = dup;
            return true;
        }
        else
        {
            OCStringLL *temp = *stringLL;
            while(temp->next)
            {
                temp = temp->next;
            }
            temp->next = (OCStringLL *)OICCalloc(1, sizeof(OCStringLL));
            temp->next->value = dup;
            return true;
        }
    }

    class Framework
    {
    public:
        Framework(ServiceType serviceType = OC::ServiceType::InProc,
                  ModeType mode = OC::ModeType::Server,
                  OCPersistentStorage *ps = nullptr)
                  : m_started(false)
        {
            PlatformConfig cfg(serviceType, mode, ps);
            OCPlatform::Configure(cfg);
        }
        ~Framework()
        {
            if (m_started)
            {
                OC_VERIFY(OC_STACK_OK == OCPlatform::stop());
                m_started = false;
            }
        }
        OCStackResult start()
        {
            OCStackResult result = OCPlatform::start();
            if (OC_STACK_OK == result)
            {
                m_started = true;
            }
            return result;
        }

    private:
        bool m_started;
    };

    OCResourceHandle RegisterResource(std::string uri, std::string type, std::string iface)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        gResourceHandle, uri, type,
                                        iface, entityHandler, gResourceProperty));
        return gResourceHandle;
    }

    OCResourceHandle RegisterResource(std::string uri, std::string type)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        gResourceHandle, uri, type,
                                        gResourceInterface, entityHandler, gResourceProperty));
        return gResourceHandle;
    }

    OCResourceHandle RegisterResource(std::string uri)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        gResourceHandle, uri, gResourceTypeName,
                                        gResourceInterface, entityHandler, gResourceProperty));
        return gResourceHandle;
    }

    OCResourceHandle RegisterResource(std::string uri, OCTpsSchemeFlags resourceTpsTypes)
    {
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                                        gResourceHandle, uri, gResourceTypeName,
                                        gResourceInterface, entityHandler, gResourceProperty,
                                        resourceTpsTypes));
        return gResourceHandle;
    }

    TEST(ConfigureTest, ConfigureInvalidModeType)
    {
        Framework framework(OC::ServiceType::InProc, (OC::ModeType)99);
        EXPECT_EQ(OC_STACK_NOTIMPL, framework.start());
    }

    TEST(ConfigureTest, ConfigureInvalidServiceType)
    {
        Framework framework((OC::ServiceType)99, OC::ModeType::Client);
        EXPECT_EQ(OC_STACK_NOTIMPL, framework.start());
    }

    // Enable it when the stack throw an exception
    // https://jira.iotivity.org/browse/IOT-428
    TEST(ConfigureTest, DISABLED_ConfigureClientOutProc)
    {
        Framework framework(OC::ServiceType::OutOfProc, OC::ModeType::Client, &gps);
        EXPECT_EQ(OC_STACK_NOTIMPL, framework.start());
    }

    TEST(ConfigureTest, ConfigureServerOutProc)
    {
        Framework framework(OC::ServiceType::OutOfProc, OC::ModeType::Server);
        EXPECT_EQ(OC_STACK_NOTIMPL, framework.start());
    }

    TEST(ConfigureTest, ConfigureServer)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Server, &gps);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/light69";
        std::string type = "core.light";
        uint8_t resourceProperty = 0;
        EXPECT_NO_THROW(OCPlatform::registerResource(
                 gResourceHandle, uri, type,
                 gResourceInterface, entityHandler, resourceProperty));
    }

    TEST(ConfigureTest, ConfigureClient)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Client, &gps);
        ASSERT_TRUE(OC_STACK_OK == framework.start());
    }

    //PersistentStorageTest
    TEST(ConfigureTest, ConfigureNULLPersistentStorage)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
    }

    //PersistentStorageTest
    TEST(ConfigureTest, ConfigureNULLHandlersPersistentStorage)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both, &gps);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_NO_THROW(OCPlatform::setDefaultDeviceEntityHandler(nullptr));
    }

    //RegisterResourceTest
    TEST(RegisterResourceTest, RegisterSingleResource)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/res2";
        EXPECT_NE(HANDLE_ZERO, RegisterResource(uri));
    }

    TEST(RegisterResourceTest, RegisterMultipleResources)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/multi";
        //Good enough for 5 resources.
        for(int i=0; i< 5; i++)
        {
            uri +=std::to_string(i);
            EXPECT_NE(HANDLE_ZERO, RegisterResource(uri));
        }
    }

    TEST(RegisterResourceTest, ReregisterResource)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light5"),
            std::string("core.light"));
        EXPECT_EQ(OC_STACK_OK, OC::OCPlatform::unregisterResource(resourceHandle));

        EXPECT_NE(HANDLE_ZERO, RegisterResource(std::string("/a/light5"),
            std::string("core.light")));

    }

    TEST(RegisterResourceTest, RegisterEmptyResource)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        // We should not allow empty URI.
        std::string emptyStr = "";
        EXPECT_ANY_THROW(OCPlatform::registerResource(gResourceHandle, emptyStr, emptyStr,
                                        emptyStr, entityHandler, gResourceProperty));
    }

    TEST(RegisterResourceTest, RegisterZeroResourceProperty)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/light6";
        std::string type = "core.light";
        uint8_t resourceProperty = 0;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                gResourceHandle, uri, type,
                gResourceInterface, entityHandler, resourceProperty));
    }

    TEST(RegisterResourceTest, RegisterWithTpsType)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/light7";
        std::string type = "core.light";
        uint8_t resourceProperty = 0;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                gResourceHandle, uri, type,
                gResourceInterface, entityHandler, resourceProperty, OC_COAP));
    }

    TEST(RegisterResourceTest, RegisterWithTpsTypeAll)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/light8";
        std::string type = "core.light";
        uint8_t resourceProperty = 0;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                gResourceHandle, uri, type,
                gResourceInterface, entityHandler, resourceProperty, OC_ALL));
    }
#ifdef TCP_ADAPTER
    TEST(RegisterResourceTest, RegisterWithTpsTypeBitComb)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string uri = "/a/light9";
        std::string type = "core.light";
        uint8_t resourceProperty = 0;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerResource(
                gResourceHandle, uri, type,
                gResourceInterface, entityHandler, resourceProperty, (OCTpsSchemeFlags)(OC_COAP || OC_COAP_TCP)));
    }
#endif

    //UnregisterTest
    TEST(UnregisterTest, UnregisterZeroHandleValue)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_ANY_THROW(OC::OCPlatform::unregisterResource(HANDLE_ZERO));
    }

    //UnbindResourcesTest
    TEST(UnbindResourcesTest, UnbindResources)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("a/home"),
            std::string("core.home"));
        OCResourceHandle resourceKitchen = RegisterResource(std::string("a/kitchen"),
            std::string("core.kitchen"), LINK_INTERFACE);
        OCResourceHandle resourceRoom = RegisterResource(std::string("a/office"),
            std::string("core.office"), LINK_INTERFACE);

        std::vector<OCResourceHandle> rList;
        rList.push_back(resourceKitchen);
        rList.push_back(resourceRoom);
        EXPECT_EQ(OC_STACK_OK, OCPlatform::bindResources(resourceHome, rList));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unbindResources(resourceHome, rList));
    }

    TEST(UnbindResourcesTest, UnbindResourcesWithZero)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle1 = 0;
        OCResourceHandle resourceHandle2 = 0;
        OCResourceHandle resourceHandle3 = 0;

        std::vector<OCResourceHandle> rList;

        rList.push_back(resourceHandle2);
        rList.push_back(resourceHandle3);

        EXPECT_ANY_THROW(OCPlatform::unbindResources(resourceHandle1, rList));
    }

    //BindInterfaceToResourceTest
    TEST(BindInterfaceToResourceTest, BindResourceInterface)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light"),
            std::string("core.light"));
        OCStackResult result = OC::OCPlatform::bindInterfaceToResource(resourceHandle,
            BATCH_INTERFACE);
        EXPECT_EQ(OC_STACK_OK, result);
    }

#if defined (_MSC_VER)
    TEST(BindInterfaceToResourceTest, DISABLED_BindZeroResourceInterface)
#else
    TEST(BindInterfaceToResourceTest, BindZeroResourceInterface)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light1"),
            std::string("core.light"));
        EXPECT_ANY_THROW(OC::OCPlatform::bindInterfaceToResource(resourceHandle, 0));
    }

    //BindTypeToResourceTest
    TEST(BindTypeToResourceTest, BindResourceType)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light3"),
            std::string("core.light"));
        OCStackResult result = OC::OCPlatform::bindTypeToResource(resourceHandle,
            "core.brightlight");
        EXPECT_EQ(OC_STACK_OK, result);
    }

#if defined (_MSC_VER)
    TEST(BindTypeToResourceTest, DISABLED_BindZeroResourceType)
#else
    TEST(BindTypeToResourceTest, BindZeroResourceType)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle = RegisterResource(std::string("/a/light4"),
            std::string("core.light"));
        EXPECT_ANY_THROW(OC::OCPlatform::bindTypeToResource(resourceHandle, 0));
    }

    //UnbindResourceTest
    TEST(UnbindResourceTest, BindAndUnbindResource)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHandle1 = RegisterResource(std::string("a/unres"),
            std::string("core.unres"));
        OCResourceHandle resourceHandle2 = RegisterResource(std::string("a/unres2"),
            std::string("core.unres"), LINK_INTERFACE);

        EXPECT_EQ(OC_STACK_OK, OCPlatform::bindResource(resourceHandle1, resourceHandle2));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unbindResource(resourceHandle1, resourceHandle2));
    }

    //PresenceTest
    TEST(PresenceTest, DISABLED_StartAndStopPresence)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_EQ(OC_STACK_OK, OCPlatform::startPresence(30));
        EXPECT_NE(HANDLE_ZERO, RegisterResource( std::string("/a/Presence"),
            std::string("core.Presence")));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::stopPresence());
    }

    TEST(OCPlatformTest, UnbindZeroRsourceHandleValue)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_ANY_THROW(OCPlatform::unbindResource(HANDLE_ZERO, HANDLE_ZERO));
    }

    //NotifyAllObserverTest
    TEST(NotifyAllObserverTest, NotifyAllObservers)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs1"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithLowQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs2"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::LowQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithMidQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs3"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::MidQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithNaQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs4"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::NaQos));
    }

    TEST(NotifyAllObserverTest, NotifyAllObserversWithHighQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs5"),
            std::string("core.obs"));
        EXPECT_EQ(OC_STACK_NO_OBSERVERS, OCPlatform::notifyAllObservers(resourceHome,
                OC::QualityOfService::HighQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObservers)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObservers)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs6"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithLowQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithLowQos)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs7"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::LowQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithMidQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithMidQos)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs8"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::MidQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithNaQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithNaQos)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs9"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::NaQos));
    }

#if defined (_MSC_VER)
    TEST(NotifyAllObserverTest, DISABLED_NotifyListOfObserversWithHighQos)
#else
    TEST(NotifyAllObserverTest, NotifyListOfObserversWithHighQos)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCResourceHandle resourceHome = RegisterResource(std::string("/a/obs10"),
            std::string("core.obs"));

        std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
        ObservationIds interestedObservers;
        EXPECT_ANY_THROW(OCPlatform::notifyListOfObservers(resourceHome,
            interestedObservers, resourceResponse,OC::QualityOfService::HighQos));
    }

    //DeviceEntityHandlerTest
    TEST(DeviceEntityHandlerTest, SetDefaultDeviceEntityHandler)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_EQ(OC_STACK_OK, OCPlatform::setDefaultDeviceEntityHandler(entityHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setDefaultDeviceEntityHandler(nullptr));
    }


    //FindResource test
    TEST(FindResourceTest, DISABLED_FindResourceValid)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK, OCPlatform::findResource("", requestURI.str(),
                CT_DEFAULT, &foundResource));
    }

#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullResourceURI)
#else
    TEST(FindResourceTest, FindResourceNullResourceURI)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_ANY_THROW(OCPlatform::findResource("", nullptr,
                CT_DEFAULT, &foundResource));
    }


#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullResourceURI1)
#else
    TEST(FindResourceTest, FindResourceNullResourceURI1)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_ANY_THROW(OCPlatform::findResource(nullptr, requestURI.str(),
                CT_DEFAULT, &foundResource));
    }

#if defined (_MSC_VER)
    TEST(FindResourceTest, DISABLED_FindResourceNullHost)
#else
    TEST(FindResourceTest, FindResourceNullHost)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_ANY_THROW(OCPlatform::findResource(nullptr, requestURI.str(),
                CT_DEFAULT, &foundResource));
    }

    TEST(FindResourceTest, FindResourceNullresourceHandler)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_THROW(OCPlatform::findResource("", requestURI.str(),
                CT_DEFAULT, nullptr), OC::OCException);
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithLowQoS)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::LowQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithMidQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::MidQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithHighQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::HighQos));
    }

    TEST(FindResourceTest, DISABLED_FindResourceWithNaQos)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=core.light";
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource,
                        OC::QualityOfService::NaQos));
    }

    //GetDeviceInfo Test
    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithValidParameters)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo));
    }

#if defined (_MSC_VER)
    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoNullDeviceURI)
#else
    TEST(GetDeviceInfoTest, GetDeviceInfoNullDeviceURI)
#endif
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_ANY_THROW(
                OCPlatform::getDeviceInfo("", nullptr, CT_DEFAULT, &receivedDeviceInfo));
    }

    TEST(GetDeviceInfoTest, GetDeviceInfoWithNullDeviceInfoHandler)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_THROW(
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, nullptr),
                OC::OCException);
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithLowQos)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::LowQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithMidQos)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::MidQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithHighQos)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::HighQos));
    }

    TEST(GetDeviceInfoTest, DISABLED_GetDeviceInfoWithNaQos)
    {
        Framework framework(OC::ServiceType::InProc, OC::ModeType::Both);
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceDiscoveryURI = "/oic/d";
        std::ostringstream requestURI;
        requestURI << OC_MULTICAST_PREFIX << deviceDiscoveryURI;
        EXPECT_EQ(OC_STACK_OK,
                OCPlatform::getDeviceInfo("", requestURI.str(), CT_DEFAULT, &receivedDeviceInfo,
                        OC::QualityOfService::NaQos));
    }

    TEST(GetSupportedTransportsInfoTest, getSupportedTransportsInfoWithValidParm)
    {
        OCTpsSchemeFlags input = OC_NO_TPS;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::getSupportedTransportsInfo(input));
    }

    //RegisterDeviceInfo test
    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithValidParameters)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCDeviceInfo deviceInfo;
        DuplicateString(&deviceInfo.deviceName, "myDeviceName");
        deviceInfo.types = NULL;
        OCResourcePayloadAddStringLL(&deviceInfo.types, OC_RSRVD_RESOURCE_TYPE_DEVICE);
        OCResourcePayloadAddStringLL(&deviceInfo.types, "oic.d.tv");
        DuplicateString(&deviceInfo.specVersion, "mySpecVersion");
        deviceInfo.dataModelVersions = nullptr;
        OCResourcePayloadAddStringLL(&deviceInfo.dataModelVersions, "myDataModelVersions");
        EXPECT_EQ(OC_STACK_OK, OCPlatform::registerDeviceInfo(deviceInfo));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(
            PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
            "bda0e016-fe64-41dc-871e-c7e94cc143b9"));
        EXPECT_NO_THROW(DeleteDeviceInfo(deviceInfo));
    }

    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithEmptyObject)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCDeviceInfo di = {0, 0, 0, 0};
        EXPECT_ANY_THROW(OCPlatform::registerDeviceInfo(di));
    }

    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithSetPropertyValue)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string deviceName = "myDeviceName";
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
            deviceName));
        std::string specVersion = "mySpecVersion";
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION,
            specVersion));
        std::vector<std::string> dmv;
        dmv.push_back("myDataModelVersions");
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
            dmv));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
            "99a74220-73d3-426f-8397-3c06d586a865"));
        OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
        ASSERT_TRUE(NULL != handle);
        EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "oic.wk.tv"));
    }


    TEST(RegisterDeviceInfoTest, RegisterDeviceInfoWithGetPropertyValue)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
            "myDeviceName"));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION,
            "mySpecVersion"));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
            "myDataModelVersions"));
        OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
        ASSERT_TRUE(NULL != handle);
        EXPECT_EQ(OC_STACK_OK, OCBindResourceTypeToResource(handle, "oic.wk.tv"));

        std::string value;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::getPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
            value));
        EXPECT_STREQ("myDeviceName", value.c_str());
        EXPECT_EQ(OC_STACK_OK, OCPlatform::getPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION,
            value));
        EXPECT_STREQ("mySpecVersion", value.c_str());
        std::vector<std::string> dmv;
        EXPECT_EQ(OC_STACK_OK, OCPlatform::getPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
            dmv));
        EXPECT_STREQ("myDataModelVersions", dmv[0].c_str());

        EXPECT_STREQ("oic.wk.d", OCGetResourceTypeName(handle, 0));
        EXPECT_STREQ("oic.wk.tv", OCGetResourceTypeName(handle, 1));
    }
    //SubscribePresence Test
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithValidParameters)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle, hostAddress,
                 CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullHost)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullHost)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle, nullptr,
                 CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullPresenceHandler)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullPresenceHandler)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle, nullptr,
                 CT_DEFAULT, nullptr));
    }

    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithResourceType)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, "core.light", CT_DEFAULT, &presenceHandler));
    }

#if defined (_MSC_VER)
    TEST(SubscribePresenceTest, DISABLED_SubscribePresenceWithNullResourceType)
#else
    TEST(SubscribePresenceTest, SubscribePresenceWithNullResourceType)
#endif
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_ANY_THROW(OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, nullptr, CT_DEFAULT, &presenceHandler));
    }

    TEST(SubscribePresenceTest, DISABLED_UnsubscribePresenceWithValidHandleAndRT)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, "core.light", CT_DEFAULT, &presenceHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }

    TEST(SubscribePresenceTest, UnsubscribePresenceWithNullHandle)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        EXPECT_ANY_THROW(OCPlatform::unsubscribePresence(presenceHandle));
    }

    TEST(SubscribePresenceTest, DISABLED_UnsubscribePresenceWithValidHandle)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribePresence(presenceHandle,
                OC_MULTICAST_IP, CT_DEFAULT, &presenceHandler));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }

#ifdef WITH_CLOUD
    // SubscribeDevicePresence Test
    TEST(SubscribeDevicePresenceTest, DISABLED_SubscribeDevicePresenceWithValidParameters)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribeDevicePresence(presenceHandle,
                hostAddress, di, CT_DEFAULT, &onObserve));
    }

    TEST(SubscribeDevicePresenceTest, SubscribeDevicePresenceWithNullHost)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_ANY_THROW(OCPlatform::subscribeDevicePresence(presenceHandle,
                        nullptr, di, CT_DEFAULT, &onObserve));
    }

    TEST(SubscribeDevicePresenceTest, SubscribeDevicePresenceWithNullOnObserve)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_ANY_THROW(OCPlatform::subscribeDevicePresence(presenceHandle,
                        hostAddress, di, CT_DEFAULT, nullptr));
    }

    TEST(SubscribeDevicePresenceTest, DISABLED_UnsubscribePresenceWithValidHandle)
    {
        Framework framework;
        ASSERT_TRUE(OC_STACK_OK == framework.start());

        std::string hostAddress = "192.168.1.2:5000";
        OCPlatform::OCPresenceHandle presenceHandle = nullptr;
        std::vector<std::string> di;

        EXPECT_EQ(OC_STACK_OK, OCPlatform::subscribeDevicePresence(presenceHandle,
                hostAddress, di, CT_DEFAULT, &onObserve));
        EXPECT_EQ(OC_STACK_OK, OCPlatform::unsubscribePresence(presenceHandle));
    }
#endif
}
