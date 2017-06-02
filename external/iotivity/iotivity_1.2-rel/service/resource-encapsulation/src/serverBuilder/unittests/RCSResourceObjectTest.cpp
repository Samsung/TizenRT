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

#include "RCSResourceObject.h"
#include "RCSRequest.h"
#include "RCSSeparateResponse.h"
#include "InterfaceHandler.h"
#include "ResourceAttributesConverter.h"

#include "OCPlatform.h"

using namespace std;
using namespace std::placeholders;

using namespace OIC::Service;
using namespace OC;

typedef OCStackResult (*registerResource)(OCResourceHandle&, string&, const string&, const string&,
                           EntityHandler, uint8_t );

typedef OCStackResult (*NotifyAllObservers)(OCResourceHandle);

constexpr char RESOURCE_URI[]{ "a/test" };
constexpr char RESOURCE_TYPE[]{ "resourcetype" };
constexpr char KEY[]{ "key" };
constexpr char CUSTOM_INTERFACE[]{ "oic.if.custom" };
constexpr int VALUE{ 100 };

TEST(ResourceObjectBuilderCreateTest, ThrowIfUriIsInvalid)
{
    ASSERT_THROW(RCSResourceObject::Builder("", "", "").build(), RCSPlatformException);
}

class ResourceObjectBuilderTest: public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();

        mocks.OnCallFuncOverload(static_cast< registerResource >(OCPlatform::registerResource))
                .Return(OC_STACK_OK);
    }
};

TEST_F(ResourceObjectBuilderTest, RegisterResourceWhenCallCreate)
{
    mocks.ExpectCallFuncOverload(static_cast< registerResource >(OCPlatform::registerResource))
            .Return(OC_STACK_OK);

    RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").build();
}

TEST_F(ResourceObjectBuilderTest, ResourceServerHasPropertiesSetByBuilder)
{
    auto serverResource = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").
            setDiscoverable(false).setObservable(true).build();

    EXPECT_FALSE(serverResource->isDiscoverable());
    EXPECT_TRUE(serverResource->isObservable());
}

TEST_F(ResourceObjectBuilderTest, ResourceServerHasAttrsSetByBuilder)
{
    RCSResourceAttributes attrs;
    attrs[KEY] = 100;

    auto serverResource = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").
            setAttributes(attrs).build();

    RCSResourceObject::LockGuard lock{ serverResource, RCSResourceObject::AutoNotifyPolicy::NEVER };
    EXPECT_EQ(attrs, serverResource->getAttributes());
}

TEST_F(ResourceObjectBuilderTest, TypesAddedInBuilderWillBeBound)
{
    int count = 0;
    mocks.OnCallFunc(OCPlatform::bindTypeToResource).Do(
            [&count](const OCResourceHandle&, const std::string&)
            {
                ++count;
                return OC_STACK_OK;
            }
    );

    auto serverResource = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").
            addType("1").addType("2").build();

    EXPECT_EQ(2, count);
}

TEST_F(ResourceObjectBuilderTest, InterfaceAddedInBuilderWillBeBound)
{
    int count = 0;
    mocks.OnCallFunc(OCPlatform::bindInterfaceToResource).Do(
            [&count](const OCResourceHandle&, const std::string&)
            {
                ++count;
                return OC_STACK_OK;
            }
    );

    auto serverResource = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").
            addInterface("1").addInterface("2").build();

    EXPECT_EQ(2, count);
}

class ResourceObjectTest: public TestWithMock
{
public:
    RCSResourceObject::Ptr server;

protected:
    void SetUp()
    {
        TestWithMock::SetUp();

        initMocks();

        server = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, "").build();

        initResourceObject();
    }

    virtual void initMocks()
    {
        mocks.OnCallFuncOverload(static_cast< registerResource >(OCPlatform::registerResource)).
                Return(OC_STACK_OK);

        mocks.OnCallFunc(OCPlatform::unregisterResource).Return(OC_STACK_OK);
    }

    virtual void initResourceObject() {
        server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::NEVER);
    }
};

TEST_F(ResourceObjectTest, AccessAttributesWithLock)
{
    {
        RCSResourceObject::LockGuard lock{ server };
        auto& attr = server->getAttributes();
        attr[KEY] = VALUE;
    }

    ASSERT_EQ(VALUE, server->getAttribute<int>(KEY));
}

TEST_F(ResourceObjectTest, ThrowIfTryToAccessAttributesWithoutGuard)
{
    ASSERT_THROW(server->getAttributes(), NoLockException);
}

TEST_F(ResourceObjectTest, SettingAttributesWithinGuardDoesntCauseDeadLock)
{
    {
        RCSResourceObject::LockGuard guard{ server };
        server->setAttribute(KEY, VALUE);
    }

    ASSERT_EQ(VALUE, server->getAttribute<int>(KEY));
}

TEST_F(ResourceObjectTest, SettingNestedAttributesIsSameToGettingNestedAttributes)
{
    RCSResourceAttributes lightAttributes;

    lightAttributes["red"]=50;
    lightAttributes["blue"]=100;
    lightAttributes["green"]=150;

    server->setAttribute(KEY, lightAttributes);

    ASSERT_EQ(lightAttributes, server->getAttribute<RCSResourceAttributes>(KEY));
}

TEST_F(ResourceObjectTest, SettingNestedVectorAttributesIsSameToGettingNestedVectorAttributes)
{
    vector<int> arr11 = {0,1}, arr12 = {4,5}, arr13 ={7,8};
    vector<vector<int>> arr21 = { arr11, arr12 }, arr22 = { arr12, arr13 };
    vector<vector<vector<int>>> arr31={ arr21, arr22 };

    server->setAttribute(KEY, arr31);

    ASSERT_EQ(arr31, server->getAttributeValue(KEY));
}

TEST_F(ResourceObjectTest, ThrowIfResourceToBindIsInvalid)
{
    ASSERT_THROW(server->bindResource(server), RCSInvalidParameterException);
}

TEST_F(ResourceObjectTest, ThrowIfBindResourceFailed)
{
    mocks.OnCallFunc(OCBindResource).Return(OC_STACK_ERROR);

    ASSERT_THROW(server->bindResource(
            RCSResourceObject::Builder("a/temp", RESOURCE_TYPE, "").build()), RCSPlatformException);
}

TEST_F(ResourceObjectTest, ThrowIfResourceToUnbindIsInvalid)
{
    ASSERT_THROW(server->unbindResource(server), RCSInvalidParameterException);
}

TEST_F(ResourceObjectTest, BoundResourceCanBeRetrieved)
{
    mocks.OnCallFunc(OCBindResource).Return(OC_STACK_OK);

    auto boundResource = RCSResourceObject::Builder("a/temp", RESOURCE_TYPE, "").build();
    server->bindResource(boundResource);

    ASSERT_EQ(boundResource, server->getBoundResources()[0]);
}


class AutoNotifyTest: public ResourceObjectTest
{
protected:
    void initMocks()
    {
        mocks.OnCallFuncOverload(static_cast< NotifyAllObservers >(
                OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);
    }

    virtual void initResourceObject() {
        // intended blank
    }
};

TEST_F(AutoNotifyTest, DefalutAutoNotifyPolicyIsUpdated)
{
    ASSERT_EQ(RCSResourceObject::AutoNotifyPolicy::UPDATED, server->getAutoNotifyPolicy());
}

TEST_F(AutoNotifyTest, AutoNotifyPolicyCanBeSet)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::NEVER);

    ASSERT_EQ(RCSResourceObject::AutoNotifyPolicy::NEVER, server->getAutoNotifyPolicy());
}

TEST_F(AutoNotifyTest, WithUpdatedPolicy_NeverBeNotifiedIfAttributeIsNotChanged)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    server->setAttribute(KEY, VALUE);

    mocks.NeverCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers));

    server->setAttribute(KEY, VALUE);
}

TEST_F(AutoNotifyTest, WithUpdatedPolicy_WillBeNotifiedIfAttributeIsChanged)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    server->setAttribute(KEY, VALUE);

    mocks.ExpectCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    server->setAttribute(KEY, VALUE + 1);
}

TEST_F(AutoNotifyTest, WithUpdatedPolicy_WillBeNotifiedIfValueIsAdded)
{
    constexpr char newKey[]{ "newKey" };
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);

    mocks.ExpectCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    server->setAttribute(newKey, VALUE);
}

TEST_F(AutoNotifyTest, WithNeverPolicy_NeverBeNotifiedEvenIfAttributeIsChanged)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::NEVER);

    mocks.NeverCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers));

    RCSResourceObject::LockGuard lock{ server };
    server->setAttribute(KEY, VALUE);
}

TEST_F(AutoNotifyTest, WithUpdatePolicy_WillBeNotifiedIfAttributeIsDeleted)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);
    server->setAttribute(KEY, VALUE);

    mocks.ExpectCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    server->removeAttribute(KEY);
}

class AutoNotifyWithGuardTest: public AutoNotifyTest
{
};

TEST_F(AutoNotifyWithGuardTest, GuardFollowsServerPolicyByDefault)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::UPDATED);

    mocks.ExpectCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    RCSResourceObject::LockGuard guard{ server };
    server->setAttribute(KEY, VALUE);
}

TEST_F(AutoNotifyWithGuardTest, GuardCanOverridePolicy)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::ALWAYS);

    mocks.NeverCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers));

    RCSResourceObject::LockGuard guard{ server, RCSResourceObject::AutoNotifyPolicy::NEVER };
    server->getAttributes()[KEY] = VALUE;
}

TEST_F(AutoNotifyWithGuardTest, GuardInvokesNotifyWhenDestroyed)
{
    server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::NEVER);

    mocks.ExpectCallFuncOverload(static_cast< NotifyAllObservers >(
            OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    {
        RCSResourceObject::LockGuard guard{ server, RCSResourceObject::AutoNotifyPolicy::ALWAYS };
        server->setAttribute(KEY, VALUE);
    }

    mocks.NeverCallFuncOverload(static_cast< NotifyAllObservers >(
               OC::OCPlatform::notifyAllObservers)).Return(OC_STACK_OK);

    server->setAttribute(KEY, VALUE);
}

class ResourceObjectHandlingRequestTest: public ResourceObjectTest
{
public:
    EntityHandler handler;

    static const OCRequestHandle fakeRequestHandle;
    static const OCResourceHandle fakeResourceHandle;

public:
    OCResourceRequest::Ptr createRequest(OCMethod method = OC_REST_GET, OCRepresentation ocRep =
            OCRepresentation{}, const string& interface="")
    {
        auto request = make_shared<OCResourceRequest>();

        OCEntityHandlerRequest ocEntityHandlerRequest;
        memset(&ocEntityHandlerRequest, 0, sizeof(OCEntityHandlerRequest));
        OC::MessageContainer mc;

        mc.addRepresentation(ocRep);

        ocEntityHandlerRequest.requestHandle = fakeRequestHandle;
        ocEntityHandlerRequest.resource = fakeResourceHandle;
        ocEntityHandlerRequest.method = method;
        ocEntityHandlerRequest.payload = reinterpret_cast<OCPayload*>(mc.getPayload());

        if(!interface.empty())
        {
            const string query = string("if=" + interface);
            ocEntityHandlerRequest.query = const_cast<char *> (query.c_str());
        }

        formResourceRequest(OC_REQUEST_FLAG, &ocEntityHandlerRequest, request);

        return request;
    }

protected:
    OCStackResult registerResourceFake(OCResourceHandle&, string&, const string&,
            const string&, EntityHandler handler, uint8_t)
    {
        this->handler = handler;
        return OC_STACK_OK;
    }

    void initMocks()
    {
        mocks.OnCallFuncOverload(
            static_cast<registerResource>(OCPlatform::registerResource)).Do(
                    bind(&ResourceObjectHandlingRequestTest::registerResourceFake,
                            this, _1, _2, _3, _4, _5, _6));
        mocks.OnCallFunc(OCPlatform::unregisterResource).Return(OC_STACK_OK);
    }
};

const OCRequestHandle ResourceObjectHandlingRequestTest::fakeRequestHandle = 0x1234;
const OCResourceHandle  ResourceObjectHandlingRequestTest::fakeResourceHandle =
    reinterpret_cast<OCResourceHandle>(0x4321);

TEST_F(ResourceObjectHandlingRequestTest, CallSendResponseWhenReceiveRequest)
{
    mocks.ExpectCallFunc(OCPlatform::sendResponse).Return(OC_STACK_OK);

    ASSERT_EQ(OC_EH_OK, handler(createRequest()));
}

TEST_F(ResourceObjectHandlingRequestTest, ReturnErrorCodeWhenSendResponseFailed)
{
    mocks.ExpectCallFunc(OCPlatform::sendResponse).Return(OC_STACK_ERROR);

    ASSERT_EQ(OC_EH_ERROR, handler(createRequest()));
}

TEST_F(ResourceObjectHandlingRequestTest, SendResponseWithSameHandlesPassedByRequest)
{
    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [](const shared_ptr<OCResourceResponse> response)
            {
                return response->getRequestHandle() == fakeRequestHandle &&
                        response->getResourceHandle() == fakeResourceHandle;
            }
    ).Return(OC_STACK_OK);

    ASSERT_EQ(OC_EH_OK, handler(createRequest()));
}

TEST_F(ResourceObjectHandlingRequestTest, SendResponseWithRCSResponseResults)
{
    constexpr int errorCode{ 1999 };

    server->setGetRequestHandler(
            [](const RCSRequest&, RCSResourceAttributes&) -> RCSGetResponse
            {
                return RCSGetResponse::create(errorCode);
            }
    );

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [](const shared_ptr<OCResourceResponse> response)
            {
                return response->getErrorCode() == errorCode;
            }
    ).Return(OC_STACK_OK);

    ASSERT_EQ(OC_EH_OK, handler(createRequest()));
}

TEST_F(ResourceObjectHandlingRequestTest, SendSetResponseWithCustomAttrs)
{
    constexpr int errorCode{ 1999 };
    constexpr char value[]{ "VALUE" };

    server->setSetRequestHandler(
            [](const RCSRequest&, RCSResourceAttributes&) -> RCSSetResponse
            {
                RCSResourceAttributes attrs;
                attrs[KEY] = value;
                return RCSSetResponse::create(attrs, errorCode);
            }
    );

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [](const shared_ptr<OCResourceResponse> response)
            {
                return value == response->getResourceRepresentation()[KEY].getValue<std::string>()
                        && response->getErrorCode() == errorCode;
            }
    ).Return(OC_STACK_OK);

    ASSERT_EQ(OC_EH_OK, handler(createRequest(OC_REST_POST)));
}

TEST_F(ResourceObjectHandlingRequestTest, SeparateResponseIsSlowResponse)
{
    server->setGetRequestHandler(
            [](const RCSRequest&, RCSResourceAttributes&) -> RCSGetResponse
            {
                return RCSGetResponse::separate();
            }
    );

    ASSERT_EQ(OC_EH_SLOW, handler(createRequest()));
}

TEST_F(ResourceObjectHandlingRequestTest, SetMethodOfSeparateResponseInvokesSendResponse)
{
    RCSRequest aRequest;
    server->setGetRequestHandler(
            [&aRequest](const RCSRequest& request, RCSResourceAttributes&) -> RCSGetResponse
            {
                aRequest = request;
                return RCSGetResponse::separate();
            }
    );
    handler(createRequest(OC_REST_GET));

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Return(OC_STACK_OK);

    RCSSeparateResponse(aRequest).set();
}

TEST_F(ResourceObjectHandlingRequestTest, SetMethodOfSeparateResponseThrowsIfTheResourceIsDestroyed)
{
    RCSRequest aRequest;
    server->setGetRequestHandler(
            [&aRequest](const RCSRequest& request, RCSResourceAttributes&) -> RCSGetResponse
            {
                aRequest = request;
                return RCSGetResponse::separate();
            }
    );
    handler(createRequest(OC_REST_GET));

    RCSSeparateResponse resp(aRequest);

    server.reset();

    EXPECT_THROW(resp.set(), RCSBadRequestException);
}

static bool checkResponse(const OCRepresentation& ocRep, const RCSResourceAttributes& rcsAttr,
            const std::vector<std::string>& interfaces,
            const std::vector<std::string>& resourceTypes, const std::string& resourceUri)
{
    return resourceUri == ocRep.getUri() &&
           interfaces == ocRep.getResourceInterfaces() &&
           resourceTypes == ocRep.getResourceTypes() &&
           rcsAttr == ResourceAttributesConverter::fromOCRepresentation(ocRep);
}

static bool compareResponse(const OCRepresentation& ocRep1, const OCRepresentation& ocRep2)
{
    return ocRep1.getUri() == ocRep2.getUri() &&
           ocRep1.getResourceInterfaces() == ocRep2.getResourceInterfaces() &&
           ocRep1.getResourceTypes() == ocRep2.getResourceTypes() &&
           ResourceAttributesConverter::fromOCRepresentation(ocRep1) ==
                   ResourceAttributesConverter::fromOCRepresentation(ocRep2);
}

class ResourceObjectInterfaceHandlerTest: public ResourceObjectHandlingRequestTest
{
public:
    void initServer(vector<string> interfaces,
                const std::string& defaultInterface = BASELINE_INTERFACE)
    {
        auto initBuilder = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE,
                BASELINE_INTERFACE);

        for(const auto& itf : interfaces)
        {
            initBuilder.addInterface(itf);
        }

        RCSResourceAttributes rcsAttr;
        rcsAttr[KEY] = 2;
        initBuilder.setAttributes(rcsAttr);
        initBuilder.setDefaultInterface(defaultInterface);

        server = initBuilder.build();
        server->setAutoNotifyPolicy(RCSResourceObject::AutoNotifyPolicy::NEVER);
        server->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::ACCEPTANCE);
    }

protected:

    void SetUp()
    {
        TestWithMock::SetUp();

        initMocks();
    }

    void initMocks()
    {
        ResourceObjectHandlingRequestTest::initMocks();

        mocks.OnCallFunc(OCPlatform::bindInterfaceToResource).Return(OC_STACK_OK);
    }
};

TEST_F(ResourceObjectInterfaceHandlerTest, GetResponseForBaselineContainsAllPropertiesOfServer)
{
    initServer({BASELINE_INTERFACE});

    OCRepresentation ocRep;

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [=](const shared_ptr<OCResourceResponse> response)
            {
                RCSResourceObject::LockGuard guard{ server };

                return checkResponse(response->getResourceRepresentation(),
                        server->getAttributes(), server->getInterfaces(), server->getTypes(),
                        server->getUri());

            }
    ).Return(OC_STACK_OK);

    handler(createRequest(OC_REST_GET, ocRep, BASELINE_INTERFACE));
}

TEST_F(ResourceObjectInterfaceHandlerTest, SetResponseForActuatorContainsOnlyRequestedAttributes)
{
    initServer({ACTUATOR_INTERFACE});

    OCRepresentation ocRep;
    ocRep[KEY] = VALUE;

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [&ocRep](const shared_ptr<OCResourceResponse> response)
            {
                return checkResponse(response->getResourceRepresentation(),
                        ResourceAttributesConverter::fromOCRepresentation(ocRep), {}, {}, "");
            }
    ).Return(OC_STACK_OK);

    handler(createRequest(OC_REST_POST, ocRep, ACTUATOR_INTERFACE));
}

TEST_F(ResourceObjectInterfaceHandlerTest, SetResponseForBaselineContainsAppliedAttributes)
{
    initServer({BASELINE_INTERFACE});

    OCRepresentation ocRep;
    ocRep["NEWKEY"] = std::string("NEWVALUE");

    mocks.ExpectCallFunc(OCPlatform::sendResponse).Match(
            [=](const shared_ptr<OCResourceResponse> response)
            {
                RCSResourceObject::LockGuard guard{ server };

                return checkResponse(response->getResourceRepresentation(),
                        server->getAttributes(), server->getInterfaces(), server->getTypes(),
                        server->getUri());
            }
    ).Return(OC_STACK_OK);

    handler(createRequest(OC_REST_POST, ocRep, BASELINE_INTERFACE));
}

TEST_F(ResourceObjectInterfaceHandlerTest, GetResponseForCustomEqualsResponseForDefault)
{
    initServer({CUSTOM_INTERFACE});

    OCRepresentation ocRep;
    OCRepresentation repArray[2];
    int cnt = 0;

    mocks.OnCallFunc(OCPlatform::sendResponse).Do(
            [&repArray, &cnt](const shared_ptr<OCResourceResponse> response)
            {
                repArray[cnt++] = response->getResourceRepresentation();
                return OC_STACK_OK;
            }
    );

    handler(createRequest(OC_REST_GET, ocRep, CUSTOM_INTERFACE));
    handler(createRequest(OC_REST_GET, ocRep, server->getDefaultInterface()));

    EXPECT_EQ(cnt, 2);
    EXPECT_TRUE(compareResponse(repArray[0], repArray[1]));
}

TEST_F(ResourceObjectInterfaceHandlerTest, SetRequestForSensorGotNoHandler)
{
    initServer({SENSOR_INTERFACE});

    OCRepresentation ocRep;
    ocRep[KEY] = VALUE;

    EXPECT_EQ(OC_EH_ERROR, handler(createRequest(OC_REST_POST, ocRep, SENSOR_INTERFACE)));
}

TEST_F(ResourceObjectInterfaceHandlerTest, ThrowIfDefaultInterfaceIsInvalid)
{
    auto builder = RCSResourceObject::Builder(RESOURCE_URI, RESOURCE_TYPE, BASELINE_INTERFACE);

    ASSERT_THROW(builder.setDefaultInterface(ACTUATOR_INTERFACE), RCSBadRequestException);
}

TEST_F(ResourceObjectInterfaceHandlerTest, SettingDefaultInterfaceEqualsGetDefaultInterface)
{
    initServer({ SENSOR_INTERFACE }, BASELINE_INTERFACE);

    EXPECT_EQ(BASELINE_INTERFACE, server->getDefaultInterface());
}



class SetRequestHandlerPolicyTest: public ResourceObjectHandlingRequestTest
{
public:
    typedef OCStackResult (*SendResponse)(std::shared_ptr<OCResourceResponse>);

public:
    OCRepresentation createOCRepresentation()
    {
        OCRepresentation ocRep;
        ocRep[KEY] = VALUE;
        return ocRep;
    }

    void initMocks()
    {
        ResourceObjectHandlingRequestTest::initMocks();
        mocks.OnCallFunc(OCPlatform::sendResponse).Return(OC_STACK_OK);
    }
};

TEST_F(SetRequestHandlerPolicyTest, DefalutSetRequestHandlerPolicyIsNever)
{
    ASSERT_EQ(RCSResourceObject::SetRequestHandlerPolicy::NEVER,
                server->getSetRequestHandlerPolicy());
}

TEST_F(SetRequestHandlerPolicyTest, SetRequestHandlerPolicyCanBeSet)
{
    server->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::ACCEPTANCE);

    ASSERT_EQ(RCSResourceObject::SetRequestHandlerPolicy::ACCEPTANCE,
                server->getSetRequestHandlerPolicy());
}

TEST_F(SetRequestHandlerPolicyTest, WithNeverPolicy_DeniedIfKeyIsNew)
{
    server->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::NEVER);

    handler(createRequest(OC_REST_POST, createOCRepresentation()));

    RCSResourceObject::LockGuard guard{ server };
    ASSERT_FALSE(server->getAttributes().contains(KEY));
}

TEST_F(SetRequestHandlerPolicyTest, WithAcceptancePolicy_AcceptedEvenIfKeyIsNew)
{
    server->setSetRequestHandlerPolicy(RCSResourceObject::SetRequestHandlerPolicy::ACCEPTANCE);

    handler(createRequest(OC_REST_POST, createOCRepresentation()));

    RCSResourceObject::LockGuard guard{ server };
    ASSERT_TRUE(server->getAttributes().contains(KEY));
}



class ResourceObjectSynchronizationTest: public ResourceObjectHandlingRequestTest
{
public:

    static void withLock(RCSResourceObject::Ptr serverResource, int count)
    {
        for (int i=0; i<count; ++i)
        {
            RCSResourceObject::LockGuard lock{ serverResource };

            auto& attrs = serverResource->getAttributes();

            attrs[KEY] = attrs[KEY].get<int>() + 1;
        }
    }

    static void withSetter(RCSResourceObject::Ptr serverResource, int count)
    {
        for (int i=0; i<count; ++i)
        {
            RCSResourceObject::LockGuard lock{ serverResource };

            serverResource->setAttribute(KEY, serverResource->getAttribute<int>(KEY) + 1);
        }
    }
};

TEST_F(ResourceObjectSynchronizationTest, MultipleAccessToServerResource)
{
    int expected { 0 };
    vector<thread> threads;

    server->setAttribute(KEY, 0);

    for (int i = 20; i >= 0; --i) {
        int count = 5000 + i * 100;
        threads.push_back(thread { withLock, server, count });
        expected += count;
    }

    for (int i = 20; i >= 0; --i) {
        int count = 5000 + i * 100;
        threads.push_back(thread { withSetter, server, count });
        expected +=count;
    }

    for (auto& t : threads)
    {
        t.join();
    }

    ASSERT_EQ(expected, server->getAttribute<int>(KEY));
}

TEST_F(ResourceObjectSynchronizationTest, MultipleAccessToServerResourceWithRequests)
{
    int expected { 0 };
    vector<thread> threads;

    mocks.OnCallFunc(OCPlatform::sendResponse).Return(OC_STACK_OK);

    server->setAttribute(KEY, 0);

    for (int i = 20; i >= 0; --i) {
        int count = 5000 + i * 100;
        threads.push_back(thread{ withLock, server, count });
        expected += count;
    }

    for (int i = 20; i >= 0; --i) {
        int count = 5000 + i * 100;
        threads.push_back(thread{ withSetter, server, count });
        expected +=count;
    }

    threads.push_back(thread{
        [this]()
        {
            for (int i=0; i<10000; ++i)
            {
                if (i % 5 == 0) handler(createRequest(OC_REST_OBSERVE));
                handler(createRequest((i & 1) ? OC_REST_GET : OC_REST_POST));
            }
        }
    });

    for (auto& t : threads)
    {
        t.join();
    }

    ASSERT_EQ(expected, server->getAttribute<int>(KEY));
}



class AttributeUpdatedListenerTest: public ResourceObjectHandlingRequestTest
{
public:
    typedef OCStackResult (*SendResponse)(std::shared_ptr<OCResourceResponse>);

public:
    OCRepresentation createOCRepresentation(void)
    {
        OCRepresentation ocRep;
        ocRep[KEY] = VALUE;
        return ocRep;
    }

protected:
    void SetUp()
    {
        ResourceObjectHandlingRequestTest::SetUp();
        mocks.OnCallFunc(OCPlatform::sendResponse).Return(OC_STACK_OK);

        server->setAttribute(KEY, 0);
    }
};

class AttributeUpdatedListener
{
public:
    virtual void onUpdated(const OIC::Service::RCSResourceAttributes::Value&,
        const OIC::Service::RCSResourceAttributes::Value&)=0;

    virtual ~AttributeUpdatedListener() {}
};


TEST_F(AttributeUpdatedListenerTest, RemoveListenerReturnsFalseIfListenerIsNotAdded)
{
    ASSERT_FALSE(server->removeAttributeUpdatedListener(KEY));
}

TEST_F(AttributeUpdatedListenerTest, RemoveListenerReturnsTrueIfListenerIsAdded)
{
    auto listener = mocks.Mock< AttributeUpdatedListener >();

    server->addAttributeUpdatedListener(KEY,
            std::bind(&AttributeUpdatedListener::onUpdated, listener, _1, _2));

    ASSERT_TRUE(server->removeAttributeUpdatedListener(KEY));
}

TEST_F(AttributeUpdatedListenerTest, AddListenerRunsAddedFunction)
{
    auto listener = mocks.Mock< AttributeUpdatedListener >();

    server->addAttributeUpdatedListener(KEY,
            std::bind(&AttributeUpdatedListener::onUpdated, listener, _1, _2));

    mocks.ExpectCall(listener, AttributeUpdatedListener::onUpdated);

    handler(createRequest(OC_REST_POST, createOCRepresentation()));
}

TEST_F(AttributeUpdatedListenerTest, ListenerWithSameKeyOverridesPreviousOne)
{
    auto first = mocks.Mock< AttributeUpdatedListener >();
    auto second = mocks.Mock< AttributeUpdatedListener >();

    mocks.NeverCall(first, AttributeUpdatedListener::onUpdated);
    mocks.ExpectCall(second, AttributeUpdatedListener::onUpdated);

    server->addAttributeUpdatedListener(KEY,
            std::bind(&AttributeUpdatedListener::onUpdated, first, _1, _2));
    server->addAttributeUpdatedListener(KEY,
            std::bind(&AttributeUpdatedListener::onUpdated, second, _1, _2));

    handler(createRequest(OC_REST_POST, createOCRepresentation()));
}

TEST_F(AttributeUpdatedListenerTest, RemovedListenerNotBeInvoked)
{
    auto listener = mocks.Mock< AttributeUpdatedListener >();
    server->addAttributeUpdatedListener(KEY,
            std::bind(&AttributeUpdatedListener::onUpdated, listener, _1, _2));

    mocks.NeverCall(listener, AttributeUpdatedListener::onUpdated);

    server->removeAttributeUpdatedListener(KEY);

    handler(createRequest(OC_REST_POST, createOCRepresentation()));
}
