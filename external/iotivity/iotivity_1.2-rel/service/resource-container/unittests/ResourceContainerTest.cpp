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

#if defined(__linux__)
#include <unistd.h>
#endif

#include <algorithm>

#include <UnitTestHelper.h>

#include <gtest/gtest.h>
#include <HippoMocks/hippomocks.h>

#include "RCSResourceContainer.h"
#include "ResourceContainerImpl.h"
#include "SoftSensorResource.h"

#include "RCSResourceObject.h"
#include "RCSRemoteResourceObject.h"

#include "ResourceContainerTestSimulator.h"

using namespace std;
using namespace testing;
using namespace OIC::Service;

#define MAX_PATH 2048

string CONFIG_FILE = "ResourceContainerTestConfig.xml";

void getCurrentPath(std::string *pPath)
{
    char buffer[MAX_PATH] = {0,};

#if defined(__linux__)
    char *strPath = NULL;
    int length = readlink("/proc/self/exe", buffer, MAX_PATH - 1);

    if (length != -1)
    {
        buffer[length] = '\0';
        strPath = strrchr(buffer, '/');

        if (strPath != NULL)
        {
            *strPath = '\0';
        }
    }
#endif
    pPath->append(buffer);
}

template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del>
static_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
{
    auto d = static_cast<Derived *>(p.release());
    return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
}

template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del>
dynamic_unique_ptr_cast( std::unique_ptr<Base, Del>&& p )
{
    if(Derived *result = dynamic_cast<Derived *>(p.get())) {
        p.release();
        return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
    }
    return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
}

/*Fake bundle resource class for testing*/
class TestBundleResource: public BundleResource
{
    public:
        virtual void initAttributes()
        {
        }

        virtual void handleSetAttributesRequest(const RCSResourceAttributes &attr,
                                                const std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            BundleResource::setAttributes(attr);
        }

        virtual RCSResourceAttributes handleGetAttributesRequest(const
                std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            return BundleResource::getAttributes();
        }
};

/*Fake bundle resource class for testing*/
class TestBundleResourceWithAttrs: public BundleResource
{
    public:
        virtual void initAttributes()
        {
            setAttribute("attrib1", RCSResourceAttributes::Value("test"));
            setAttribute("attrib2", RCSResourceAttributes::Value(1));
            setAttribute("attrib3", RCSResourceAttributes::Value(true));
        }

        virtual void handleSetAttributesRequest(const RCSResourceAttributes &attr,
                                                const std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            BundleResource::setAttributes(attr);
        }

        virtual RCSResourceAttributes handleGetAttributesRequest(const
                std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            return BundleResource::getAttributes();
        }
};


/*Fake bundle resource class for testing*/
class TestSoftSensorResource: public SoftSensorResource
{
    public:
        virtual void initAttributes()
        {
            SoftSensorResource::initAttributes();
        }

        virtual void handleSetAttributesRequest(const RCSResourceAttributes &attr,
                                                const std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            BundleResource::setAttributes(attr);
        }

        virtual RCSResourceAttributes handleGetAttributesRequest(const
                 std::map< std::string, std::string > &queryParams)
        {
            (void)queryParams;
            return BundleResource::getAttributes();
        }

        virtual void executeLogic()
        {
        }

        virtual void onUpdatedInputResource(
                std::string, std::vector<OIC::Service::RCSResourceAttributes::Value>)
        {
        }
};

class ResourceContainerTest: public TestWithMock
{

    public:
        RCSResourceContainer *m_pResourceContainer;
        std::string m_strConfigPath;

    protected:
        void SetUp()
        {
            TestWithMock::SetUp();
            m_pResourceContainer = RCSResourceContainer::getInstance();
            getCurrentPath(&m_strConfigPath);
            m_strConfigPath.append("/");
            m_strConfigPath.append(CONFIG_FILE);
        }
};

TEST_F(ResourceContainerTest, TestBundleResource)
{
    TestBundleResourceWithAttrs testResource;
    testResource.initAttributes();

    // check if initAttributes worked
    EXPECT_STREQ("\"test\"", testResource.getAttribute("attrib1").toString().c_str());

    std::list<string> attrNames = testResource.getAttributeNames();
    ASSERT_TRUE(std::find(attrNames.begin(), attrNames.end(), "attrib1") != attrNames.end());

    ASSERT_FALSE(testResource.getAttributes().contains("attrib4"));

    testResource.getAttributeNames();

    RCSResourceAttributes fullAttributes;
    const std::map< std::string, std::string > queryParams = {};

    fullAttributes["attrib1"] = "test";
    fullAttributes["attrib2"] = 1;
    fullAttributes["attrib3"] = true;

    testResource.setAttributes(fullAttributes);

    ASSERT_TRUE(testResource.getAttributes().contains("attrib1"));

    fullAttributes["attrib1"] = "test2";
    fullAttributes["attrib2"] = 2;
    fullAttributes["attrib3"] = false;

    testResource.handleSetAttributesRequest(fullAttributes, queryParams);

    EXPECT_EQ((unsigned int) 3, testResource.getAttributeNames().size());

    EXPECT_EQ((unsigned int) 3, testResource.handleGetAttributesRequest(queryParams).size());
    std::string testString = "test";
    testResource.setAttribute("attrib1", RCSResourceAttributes::Value(testString), false);

    testResource.setAttributes(fullAttributes, false);

    EXPECT_STREQ("\"test2\"", testResource.getAttribute("attrib1").toString().c_str());
    EXPECT_EQ(2, testResource.getAttribute("attrib2"));

    testResource.setAttribute("attrib1", RCSResourceAttributes::Value("test"));
    EXPECT_STREQ("\"test\"", testResource.getAttribute("attrib1").toString().c_str());
    EXPECT_EQ(2, testResource.getAttribute("attrib2"));
}

TEST_F(ResourceContainerTest, TestSoftSensorResource)
{
    TestSoftSensorResource softSensorResource;
    softSensorResource.initAttributes();
    EXPECT_EQ((unsigned int) 0, softSensorResource.getAttributeNames().size());
}


TEST_F(ResourceContainerTest, BundleRegisteredWhenContainerStartedWithValidConfigFile)
{
    m_pResourceContainer->startContainer(m_strConfigPath);
    EXPECT_GT(m_pResourceContainer->listBundles().size(), (unsigned int) 0);
    EXPECT_STREQ("oic.bundle.test",
                 (*m_pResourceContainer->listBundles().begin())->getID().c_str());
    EXPECT_STREQ("libTestBundle.so",
                 (*m_pResourceContainer->listBundles().begin())->getPath().c_str());
    EXPECT_STREQ("1.0.0", (*m_pResourceContainer->listBundles().begin())->getVersion().c_str());
    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, BundleLoadedWhenContainerStartedWithValidConfigFile)
{
    m_pResourceContainer->startContainer(m_strConfigPath);

    EXPECT_GT(m_pResourceContainer->listBundles().size(), (unsigned int) 0);
    unique_ptr<RCSBundleInfo> first = std::move(*m_pResourceContainer->listBundles().begin());
    unique_ptr<BundleInfoInternal> firstInternal((BundleInfoInternal*)first.release());
    EXPECT_TRUE( firstInternal->isLoaded() );
    EXPECT_NE(nullptr,
    		firstInternal->getBundleHandle());

    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, BundleActivatedWhenContainerStartedWithValidConfigFile)
{
    m_pResourceContainer->startContainer(m_strConfigPath);

    EXPECT_GT(m_pResourceContainer->listBundles().size(), (unsigned int) 0);
    unique_ptr<RCSBundleInfo> first = std::move(*m_pResourceContainer->listBundles().begin());
    unique_ptr<BundleInfoInternal> firstInternal((BundleInfoInternal*)first.release());
    EXPECT_TRUE(firstInternal->isActivated());
    EXPECT_NE(nullptr,firstInternal->getBundleActivator());

    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, BundleNotRegisteredWhenContainerStartedWithInvalidConfigFile)
{
    m_pResourceContainer->startContainer("invalidConfig");

    EXPECT_EQ((unsigned int) 0, m_pResourceContainer->listBundles().size());
}

TEST_F(ResourceContainerTest, BundleNotRegisteredWhenContainerStartedWithEmptyConfigFile)
{
    m_pResourceContainer->startContainer("");

    EXPECT_EQ((unsigned int) 0, m_pResourceContainer->listBundles().size());
}

TEST_F(ResourceContainerTest, BundleUnregisteredWhenContainerStopped)
{
    m_pResourceContainer->startContainer(m_strConfigPath);
    m_pResourceContainer->stopContainer();

    EXPECT_EQ((unsigned int) 0, m_pResourceContainer->listBundles().size());
}

TEST_F(ResourceContainerTest, BundleStoppedWithStartBundleAPI)
{
    m_pResourceContainer->startContainer(m_strConfigPath);
    m_pResourceContainer->stopBundle("oic.bundle.test");

    unique_ptr<RCSBundleInfo> first = std::move(*m_pResourceContainer->listBundles().begin());
    unique_ptr<BundleInfoInternal> firstInternal((BundleInfoInternal*)first.release());
    EXPECT_FALSE(firstInternal->isActivated());

    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, BundleStartedWithStartBundleAPI)
{
    m_pResourceContainer->startContainer(m_strConfigPath);
    m_pResourceContainer->stopBundle("oic.bundle.test");
    m_pResourceContainer->startBundle("oic.bundle.test");
    unique_ptr<RCSBundleInfo> first = std::move(*m_pResourceContainer->listBundles().begin());
    unique_ptr<BundleInfoInternal> firstInternal((BundleInfoInternal*)first.release());
    EXPECT_TRUE(firstInternal->isActivated());

    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, AddNewSoBundleToContainer)
{
    std::map<string, string> bundleParams;
    std::list<std::unique_ptr<RCSBundleInfo>> bundles;

    bundles = m_pResourceContainer->listBundles();
    m_pResourceContainer->addBundle("oic.bundle.test", "", "libTestBundle.so", "test", bundleParams);

    EXPECT_EQ(bundles.size() + 1, m_pResourceContainer->listBundles().size());
    unique_ptr<RCSBundleInfo> first = std::move(*m_pResourceContainer->listBundles().begin());
    unique_ptr<BundleInfoInternal> firstInternal((BundleInfoInternal*)first.release());
    EXPECT_TRUE(firstInternal->isLoaded());
}

TEST_F(ResourceContainerTest, RemoveSoBundleFromContainer)
{
    std::map<string, string> bundleParams;
    std::list<std::unique_ptr<RCSBundleInfo>> bundles;

    bundles = m_pResourceContainer->listBundles();
    m_pResourceContainer->removeBundle("oic.bundle.test");

    EXPECT_EQ(bundles.size() - 1, m_pResourceContainer->listBundles().size());
}

TEST_F(ResourceContainerTest, AddBundleAlreadyRegistered)
{
    std::map<string, string> bundleParams;
    std::list<std::unique_ptr<RCSBundleInfo> > bundles;

    m_pResourceContainer->addBundle("oic.bundle.test", "", "libTestBundle.so", "test", bundleParams);
    bundles = m_pResourceContainer->listBundles();
    m_pResourceContainer->addBundle("oic.bundle.test", "", "libTestBundle.so", "test",  bundleParams);

    EXPECT_EQ(bundles.size(), m_pResourceContainer->listBundles().size());
}

TEST_F(ResourceContainerTest, AddAndRemoveSoBundleResource)
{
    std::list<string> resources;
    std::map<string, string> resourceParams;
    resourceParams["resourceType"] = "container.test";

    m_pResourceContainer->startContainer(m_strConfigPath);
    resources = m_pResourceContainer->listBundleResources("oic.bundle.test");

    m_pResourceContainer->addResourceConfig("oic.bundle.test", "/test_resource", resourceParams);

    EXPECT_EQ(resources.size() + 1,
              m_pResourceContainer->listBundleResources("oic.bundle.test").size());

    m_pResourceContainer->removeResourceConfig("oic.bundle.test", "/test_resource");

    EXPECT_EQ(resources.size(), m_pResourceContainer->listBundleResources("oic.bundle.test").size());

    m_pResourceContainer->stopContainer();
}

TEST_F(ResourceContainerTest, TryAddingSoBundleResourceToNotRegisteredBundle)
{
    std::map<string, string> resourceParams;

    mocks.NeverCallFunc(ResourceContainerImpl::buildResourceObject);

    m_pResourceContainer->addResourceConfig("unvalidBundleId", "", resourceParams);
}

class ResourceContainerBundleAPITest: public TestWithMock
{

    public:
        RCSResourceObject *m_pResourceObject;
        ResourceContainerBundleAPI *m_pResourceContainer;
        BundleResource::Ptr m_pBundleResource;
        std::string m_strConfigPath;

    protected:
        void SetUp()
        {
            TestWithMock::SetUp();
            m_pResourceObject = mocks.Mock<RCSResourceObject>();
            m_pResourceContainer = ResourceContainerBundleAPI::getInstance();

            getCurrentPath(&m_strConfigPath);
            m_strConfigPath.append("/");
            m_strConfigPath.append(CONFIG_FILE);

            m_pBundleResource = std::make_shared< TestBundleResource >();
            m_pBundleResource->m_bundleId = "oic.bundle.test";
            m_pBundleResource->m_uri = "/test_resource";
            m_pBundleResource->m_resourceType = "container.test";
            m_pBundleResource->m_interface = "oic.if.baseline";
        }
};

TEST_F(ResourceContainerBundleAPITest, ResourceServerCreatedWhenRegisterResourceCalled)
{
    m_pBundleResource = std::make_shared< TestBundleResource >();
    m_pBundleResource->m_bundleId = "oic.bundle.test";
    m_pBundleResource->m_uri = "/test_resource/test";
    m_pBundleResource->m_resourceType = "container.test";
    m_pBundleResource->m_interface = "oic.if.baseline";

    mocks.ExpectCallFunc(ResourceContainerImpl::buildResourceObject).With(m_pBundleResource->m_uri,
            m_pBundleResource->m_resourceType, m_pBundleResource->m_interface).Return(nullptr);

    m_pResourceContainer->registerResource(m_pBundleResource);
}

TEST_F(ResourceContainerBundleAPITest, RequestHandlerForResourceServerSetWhenRegisterResourceCalled)
{
    mocks.OnCallFunc(ResourceContainerImpl::buildResourceObject).Return(
        RCSResourceObject::Ptr(m_pResourceObject, [](RCSResourceObject *)
        {
        }));

    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setGetRequestHandler);
    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setSetRequestHandler);

    m_pResourceContainer->registerResource(m_pBundleResource);

    m_pResourceContainer->unregisterResource(m_pBundleResource);
}

TEST_F(ResourceContainerBundleAPITest, BundleResourceUnregisteredWhenUnregisterResourceCalled)
{
    mocks.OnCallFunc(ResourceContainerImpl::buildResourceObject).Return(
        RCSResourceObject::Ptr(m_pResourceObject, [](RCSResourceObject *)
        {
        }));

    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setGetRequestHandler);
    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setSetRequestHandler);

    m_pResourceContainer->registerResource(m_pBundleResource);
    m_pResourceContainer->unregisterResource(m_pBundleResource);

    EXPECT_EQ((unsigned int) 0,
              ((ResourceContainerImpl *)m_pResourceContainer)->listBundleResources(
                  m_pBundleResource->m_bundleId).size());
}

TEST_F(ResourceContainerBundleAPITest,
       ServerNotifiesToObserversWhenNotificationReceivedFromResource)
{
    mocks.OnCallFunc(ResourceContainerImpl::buildResourceObject).Return(
        RCSResourceObject::Ptr(m_pResourceObject, [](RCSResourceObject *)
        {
        }));

    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setGetRequestHandler);
    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::setSetRequestHandler);

    m_pResourceContainer->registerResource(m_pBundleResource);

    mocks.ExpectCall(m_pResourceObject, RCSResourceObject::notify);

    m_pResourceContainer->onNotificationReceived(m_pBundleResource->m_uri);

    m_pResourceContainer->unregisterResource(m_pBundleResource);
}

TEST_F(ResourceContainerBundleAPITest, BundleConfigurationParsedWithValidBundleId)
{
    configInfo bundle;
    map< string, string > results;

    ((ResourceContainerImpl *)m_pResourceContainer)->startContainer(m_strConfigPath);
    m_pResourceContainer->getBundleConfiguration("oic.bundle.test", &bundle);

    results = *bundle.begin();

    EXPECT_STREQ("oic.bundle.test", results["id"].c_str());
    EXPECT_STREQ("libTestBundle.so", results["path"].c_str());
    EXPECT_STREQ("1.0.0", results["version"].c_str());

    ((ResourceContainerImpl *)m_pResourceContainer)->stopContainer();
}

TEST_F(ResourceContainerBundleAPITest, BundleResourceConfigurationListParsed)
{
    vector< resourceInfo > resourceConfig;
    resourceInfo result;

    ((ResourceContainerImpl *)m_pResourceContainer)->startContainer(m_strConfigPath);
    m_pResourceContainer->getResourceConfiguration("oic.bundle.test", &resourceConfig);

    result = *resourceConfig.begin();

    EXPECT_STREQ("test_resource", result.name.c_str());
    EXPECT_STREQ("container.test", result.resourceType.c_str());

    ((ResourceContainerImpl *)m_pResourceContainer)->stopContainer();
}

class ResourceContainerImplTest: public TestWithMock
{

    public:
        ResourceContainerImpl *m_pResourceContainer;
        shared_ptr<BundleInfoInternal> m_pBundleInfo;

    protected:
        void SetUp()
        {
            TestWithMock::SetUp();
            m_pResourceContainer = ResourceContainerImpl::getImplInstance();
            m_pBundleInfo = std::make_shared<BundleInfoInternal>();
        }

        void TearDown()
        {
            m_pBundleInfo.reset();
        }
};


/* Test for Configuration */
TEST(ConfigurationTest, ConfigFileLoadedWithValidPath)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    EXPECT_TRUE(config->isLoaded());

    delete config;
}

TEST(ConfigurationTest, ConfigFileNotLoadedWithInvalidPath)
{
    Configuration *config = new Configuration("InvalidPath");

    EXPECT_FALSE(config->isLoaded());

    delete config;
}

TEST(ConfigurationTest, BundleConfigurationListParsed)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    configInfo bundles;
    map< string, string > results;

    config->getConfiguredBundles(&bundles);

    results = *bundles.begin();

    EXPECT_STREQ("oic.bundle.test", results["id"].c_str());
    EXPECT_STREQ("libTestBundle.so", results["path"].c_str());
    EXPECT_STREQ("1.0.0", results["version"].c_str());

    delete config;
}

TEST(ConfigurationTest, BundleConfigurationParsedWithValidBundleId)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    configInfo bundle;
    map< string, string > results;

    config->getBundleConfiguration("oic.bundle.test", &bundle);

    results = *bundle.begin();

    EXPECT_STREQ("oic.bundle.test", results["id"].c_str());
    EXPECT_STREQ("libTestBundle.so", results["path"].c_str());
    EXPECT_STREQ("1.0.0", results["version"].c_str());

    delete config;
}

TEST(ConfigurationTest, BundleConfigurationNotParsedWithInvalidBundleId)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    configInfo bundles;
    config->getBundleConfiguration("test", &bundles);

    EXPECT_TRUE(bundles.empty());

    delete config;
}

TEST(ConfigurationTest, BundleResourceConfigurationListParsed)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    vector< resourceInfo > resourceConfig;
    resourceInfo result;

    config->getResourceConfiguration("oic.bundle.test", &resourceConfig);

    result = *resourceConfig.begin();

    EXPECT_STREQ("test_resource", result.name.c_str());
    EXPECT_STREQ("container.test", result.resourceType.c_str());

    delete config;
}

TEST(ConfigurationTest, BundleResourceConfigurationNotParsedWithInvalidBundleId)
{
    std::string strConfigPath;
    getCurrentPath(&strConfigPath);
    strConfigPath.append("/");
    strConfigPath.append(CONFIG_FILE);

    Configuration *config = new Configuration(strConfigPath);

    configInfo bundles;
    vector< resourceInfo > resourceConfig;
    config->getResourceConfiguration("test", &resourceConfig);

    EXPECT_TRUE(bundles.empty());

    delete config;
}

class DiscoverResourceUnitTest: public TestWithMock
{
    private:
        typedef std::function<void(const std::string attributeName,
                                   std::vector<RCSResourceAttributes::Value> values)>
        UpdatedCB;
    public:
        ResourceContainerTestSimulator::Ptr testObject;
        DiscoverResourceUnit::Ptr m_pDiscoverResourceUnit;
        std::string m_bundleId;
        UpdatedCB m_updatedCB;

    protected:
        void SetUp()
        {
            TestWithMock::SetUp();

            testObject = std::make_shared<ResourceContainerTestSimulator>();
            testObject->createResource();
            m_bundleId = "/a/TempHumSensor/Container";
            m_pDiscoverResourceUnit = std::make_shared< DiscoverResourceUnit >( m_bundleId );
            m_updatedCB = ([](const std::string, std::vector< RCSResourceAttributes::Value >)
                    {
                    });
        }

        void TearDown()
        {
            m_pDiscoverResourceUnit.reset();
            testObject.reset();
            TestWithMock::TearDown();
        }
};

TEST_F(DiscoverResourceUnitTest, startDiscover)
{
    std::string type = "resource.container";
    std::string attributeName = "TestResourceContainer";

    m_pDiscoverResourceUnit->startDiscover(
        DiscoverResourceUnit::DiscoverResourceInfo("", type, attributeName), m_updatedCB);

    std::chrono::milliseconds interval(ResourceContainerTestSimulator::DEFAULT_WAITTIME);
    std::this_thread::sleep_for(interval);
}

TEST_F(DiscoverResourceUnitTest, onUpdateCalled)
{
    std::string type = "resource.container";
    std::string attributeName = "TestResourceContainer";

    m_pDiscoverResourceUnit->startDiscover(
        DiscoverResourceUnit::DiscoverResourceInfo("", type, attributeName), m_updatedCB);

    std::chrono::milliseconds interval(ResourceContainerTestSimulator::DEFAULT_WAITTIME);
    std::this_thread::sleep_for(interval);

    testObject->ChangeAttributeValue();
}

namespace
{
    void onCacheCB(const RCSResourceAttributes &)
    {
    }
}

class RemoteResourceUnitTest: public TestWithMock
{
    private:
        typedef std::function<void(RemoteResourceUnit::UPDATE_MSG, RCSRemoteResourceObject::Ptr)>
        UpdatedCBFromServer;

    public:
        ResourceContainerTestSimulator::Ptr testObject;
        RemoteResourceUnit::Ptr m_pRemoteResourceUnit;
        RCSRemoteResourceObject::Ptr m_pRCSRemoteResourceObject;
        UpdatedCBFromServer m_updatedCBFromServer;

    protected:
        void SetUp()
        {
            TestWithMock::SetUp();
            testObject = std::make_shared<ResourceContainerTestSimulator>();
            testObject->defaultRunSimulator();
            m_pRCSRemoteResourceObject = testObject->getRemoteResource();
            m_updatedCBFromServer = ([](RemoteResourceUnit::UPDATE_MSG, RCSRemoteResourceObject::Ptr) 
                {
                });
        }

        void TearDown()
        {
            m_pRCSRemoteResourceObject.reset();
            testObject.reset();
            TestWithMock::TearDown();
        }
};

TEST_F(RemoteResourceUnitTest, createRemoteResourceInfo)
{
    EXPECT_NE(nullptr, m_pRemoteResourceUnit->createRemoteResourceInfo(m_pRCSRemoteResourceObject,
              m_updatedCBFromServer));
}

TEST_F(RemoteResourceUnitTest, getRemoteResourceObject)
{
    RemoteResourceUnit::Ptr ptr = m_pRemoteResourceUnit->createRemoteResourceInfo(
                                      m_pRCSRemoteResourceObject, m_updatedCBFromServer);
    EXPECT_EQ(m_pRCSRemoteResourceObject, ptr->getRemoteResourceObject());
}

TEST_F(RemoteResourceUnitTest, getRemoteResourceUri)
{
    RemoteResourceUnit::Ptr ptr = m_pRemoteResourceUnit->createRemoteResourceInfo(
                                      m_pRCSRemoteResourceObject, m_updatedCBFromServer);
    EXPECT_NE("", ptr->getRemoteResourceUri());
}

TEST_F(RemoteResourceUnitTest, startCaching)
{
    RemoteResourceUnit::Ptr ptr = m_pRemoteResourceUnit->createRemoteResourceInfo(
                                      m_pRCSRemoteResourceObject, m_updatedCBFromServer);
    ptr->startCaching();
}

TEST_F(RemoteResourceUnitTest, startMonitoring)
{
    RemoteResourceUnit::Ptr ptr = m_pRemoteResourceUnit->createRemoteResourceInfo(
                                      m_pRCSRemoteResourceObject, m_updatedCBFromServer);
    ptr->startMonitoring();
}

#ifdef SECURED
TEST_F(RemoteResourceUnitTest, DISABLED_onCacheCBCalled)
#else
TEST_F(RemoteResourceUnitTest, onCacheCBCalled)
#endif
{
    bool isCalled = false;
    mocks.ExpectCallFunc(onCacheCB).Do(
        [this, &isCalled](const RCSResourceAttributes &)
    {
        isCalled = true;
    });
    RemoteResourceUnit::Ptr ptr = m_pRemoteResourceUnit->createRemoteResourceInfoWithCacheCB(
                                      m_pRCSRemoteResourceObject, m_updatedCBFromServer, onCacheCB);
    ptr->startCaching();
    testObject->ChangeAttributeValue();
    EXPECT_TRUE(isCalled);
}
