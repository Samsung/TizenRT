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

#include "PrimitiveResourceImpl.h"
#include "AssertUtils.h"

#include "OCResource.h"
#include "OCPlatform.h"

using namespace OIC::Service;

const std::string KEY{ "key" };

class FakeOCResource
{
public:
    virtual ~FakeOCResource() {};

    virtual OCStackResult get(const std::string&, const std::string&,
            const OC::QueryParamsMap&, OC::GetCallback) = 0;

    virtual OCStackResult put(
            const OC::OCRepresentation&, const OC::QueryParamsMap&, OC::PutCallback) = 0;

    virtual OCStackResult post(const std::string&, const std::string&,
            const OC::OCRepresentation&, const OC::QueryParamsMap&, OC::PostCallback) = 0;

    virtual OCStackResult observe(
            OC::ObserveType, const OC::QueryParamsMap&, OC::ObserveCallback) = 0;

    virtual OCStackResult cancelObserve() = 0;

    virtual std::string sid() const = 0;
    virtual std::string uri() const = 0;
    virtual std::string host() const = 0;
    virtual std::vector<std::string> getResourceTypes() const = 0;
    virtual std::vector<std::string> getResourceInterfaces() const = 0;
    virtual OCConnectivityType connectivityType() const = 0;

    virtual bool isObservable() const = 0;
};

class PrimitiveResourceTest: public TestWithMock
{
public:
    PrimitiveResource::Ptr resource;
    FakeOCResource* fakeResource;

protected:
    void SetUp() {
        TestWithMock::SetUp();

        fakeResource = mocks.Mock< FakeOCResource >();

        resource.reset(new PrimitiveResourceImpl< FakeOCResource >{
            std::shared_ptr< FakeOCResource >(fakeResource, [](FakeOCResource*) {}) });
    }
};

TEST_F(PrimitiveResourceTest, RequestGetInvokesOCResourceGet)
{
    mocks.ExpectCall(fakeResource, FakeOCResource::get).Return(OC_STACK_OK);

    resource->requestGet(PrimitiveResource::GetCallback());
}

TEST_F(PrimitiveResourceTest, RequestGetThrowsOCResourceGetReturnsNotOK)
{
    mocks.OnCall(fakeResource, FakeOCResource::get).Return(OC_STACK_ERROR);

    ASSERT_THROW(resource->requestGet(PrimitiveResource::GetCallback()), RCSPlatformException);
}

TEST_F(PrimitiveResourceTest, RequestSetInvokesOCResourcePost)
{
    mocks.ExpectCall(fakeResource, FakeOCResource::post).Return(OC_STACK_OK);

    resource->requestSet(RCSResourceAttributes{ }, PrimitiveResource::SetCallback());
}

TEST_F(PrimitiveResourceTest, RequestSetThrowsOCResourcePostReturnsNotOK)
{
    mocks.OnCall(fakeResource, FakeOCResource::post).Return(OC_STACK_ERROR);

    ASSERT_THROW(resource->requestSet(RCSResourceAttributes{ }, PrimitiveResource::SetCallback()),
            RCSPlatformException);
}

TEST_F(PrimitiveResourceTest, RequestSetPassResourceAttributesToOCResourcePost)
{
    constexpr int value{ -200 };

    RCSResourceAttributes attrs;

    mocks.ExpectCall(fakeResource, FakeOCResource::post).Match(
            [](const std::string&, const std::string&, const OC::OCRepresentation& ocRep,
                    const OC::QueryParamsMap&, OC::PutCallback)
            {
                return ocRep.getValue<int>(KEY) == value;
            }
        ).Return(OC_STACK_OK);

    attrs[KEY] = value;

    resource->requestSet(attrs, PrimitiveResource::SetCallback());
}

TEST_F(PrimitiveResourceTest, RequestObserveInvokesOCResourceObserve)
{
    mocks.ExpectCall(fakeResource, FakeOCResource::observe).Return(OC_STACK_OK);

    resource->requestObserve(PrimitiveResource::ObserveCallback());
}

TEST_F(PrimitiveResourceTest, RequestObserveThrowsOCResourceObserveReturnsNotOK)
{
    mocks.OnCall(fakeResource, FakeOCResource::observe).Return(OC_STACK_ERROR);

    ASSERT_THROW(resource->requestObserve(PrimitiveResource::ObserveCallback()), RCSPlatformException);
}

TEST_F(PrimitiveResourceTest, DelegteGettersToOCResource)
{
    const std::string host{ "host_test_" };
    const std::string uri{ "uri/test/" };

    mocks.OnCall(fakeResource, FakeOCResource::isObservable).Return(false);
    mocks.OnCall(fakeResource, FakeOCResource::host).Return(host);
    mocks.OnCall(fakeResource, FakeOCResource::uri).Return(uri);

    ASSERT_FALSE(resource->isObservable());
    ASSERT_EQ(host, resource->getHost());
    ASSERT_EQ(uri, resource->getUri());
}


TEST_F(PrimitiveResourceTest, ResponseStatementHasSameValuesWithOCRepresentationReceived)
{
    constexpr int errorCode{ 202 };
    constexpr int value{ 1999 };

    mocks.OnCall(fakeResource, FakeOCResource::get).Do(
            [](const std::string&, const std::string&, const OC::QueryParamsMap&, OC::GetCallback cb)
            {
                OC::OCRepresentation ocRep;
                ocRep[KEY] = value;

                cb(OC::HeaderOptions(), ocRep, errorCode);
                return OC_STACK_OK;
            }
        ).Return(OC_STACK_OK);

    resource->requestGet(
            [&](const HeaderOptions&, const ResponseStatement& response, int e)
            {
                ASSERT_EQ(e, errorCode);
                ASSERT_EQ(response.getAttributes().at(KEY).get<int>(), value);
            }
        );
}


class DiscoverResourceTest: public TestWithMock
{
public:
    typedef OCStackResult (*FindResource)(const std::string&, const std::string&,
            OCConnectivityType, OC::FindCallback);

public:
    static void discovered(std::shared_ptr< PrimitiveResource >) {}
};

TEST_F(DiscoverResourceTest, CallbackIsInvokedWhenResourceIsDiscovered)
{
    mocks.ExpectCallFuncOverload(static_cast<FindResource>(OC::OCPlatform::findResource)).Do(
            [](const std::string&, const std::string&, OCConnectivityType,
                    OC::FindCallback callback) -> OCStackResult
            {
                callback(nullptr);
                return OC_STACK_OK;
            }
        ).Return(OC_STACK_OK);

    mocks.ExpectCallFunc(discovered);

    discoverResource("", "", OCConnectivityType{ }, discovered);
}

TEST_F(DiscoverResourceTest, ThrowsdWhenOCPlatformFindResourceReturnsNotOK)
{
    mocks.ExpectCallFuncOverload(static_cast<FindResource>(OC::OCPlatform::findResource)).
            Return(OC_STACK_ERROR);

    EXPECT_THROW(discoverResource("", "", OCConnectivityType{ }, discovered), RCSPlatformException);
}

