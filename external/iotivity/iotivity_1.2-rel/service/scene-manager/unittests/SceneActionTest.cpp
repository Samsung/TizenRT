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

#include <condition_variable>
#include "UnitTestHelper.h"

#include "SceneList.h"
#include "RCSResourceObject.h"
#include "RCSRemoteResourceObject.h"
#include "SceneCommons.h"
#include "OCPlatform.h"

using namespace std;
using namespace OIC::Service;
using namespace OC;

typedef std::function<void (int)> ExecuteCallback;

constexpr char RESOURCE_URI[]{ "/a/light" };
constexpr char RESOURCE_TYPE[]{ "core.light" };
constexpr char RESOURCE_URI2[]{ "/a/fan" };
constexpr char RESOURCE_TYPE2[]{ "core.fan" };
constexpr char KEY[]{ "power" };
constexpr char VALUE[]{ "on" };
constexpr char KEY_2[]{ "state" };
constexpr char VALUE_2[]{ "100" };

class SceneActionTest: public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();
        pSceneList = SceneList::getInstance();
    }

    void createServer(const std::string& resourceUri1, const std::string& resourceUri2)
    {
        auto pResource1 = RCSResourceObject::Builder(
                resourceUri1, RESOURCE_TYPE, DEFAULT_INTERFACE).build();
        pResource1->setAttribute(KEY, VALUE);

        auto ocResourcePtr = OC::OCPlatform::constructResourceObject(
                "coap://" + SceneUtils::getNetAddress(), resourceUri1,
                OCConnectivityType::CT_ADAPTER_IP, false,
                pResource1->getTypes(), pResource1->getInterfaces());
        pRemoteResource1 = RCSRemoteResourceObject::fromOCResource(ocResourcePtr);

        auto pResource2 = RCSResourceObject::Builder(
                resourceUri2, RESOURCE_TYPE2, DEFAULT_INTERFACE).build();
        pResource2->setAttribute(KEY_2, VALUE_2);

        ocResourcePtr = OC::OCPlatform::constructResourceObject(
                        "coap://" + SceneUtils::getNetAddress(), resourceUri2,
                        OCConnectivityType::CT_ADAPTER_IP, false,
                        pResource2->getTypes(), pResource2->getInterfaces());
        pRemoteResource2 = RCSRemoteResourceObject::fromOCResource(ocResourcePtr);
    }

    void createSceneCollectionAndScene()
    {
        auto pSceneCollection = pSceneList->addNewSceneCollection();
        pScene1 = pSceneCollection->addNewScene("SceneTestName_1");
    }

    void createSceneAction()
    {
        pSceneAction1 = pScene1->addNewSceneAction(pRemoteResource1, KEY, VALUE);
        pSceneAction2 = pScene1->addNewSceneAction(pRemoteResource2, KEY_2, VALUE_2);
    }

public:
    SceneList* pSceneList;
    shared_ptr<Scene> pScene1;
    shared_ptr<Scene> pScene2;
    shared_ptr<SceneAction> pSceneAction1;
    shared_ptr<SceneAction> pSceneAction2;
    RCSRemoteResourceObject::Ptr pRemoteResource1;
    RCSRemoteResourceObject::Ptr pRemoteResource2;

private:
    std::condition_variable cond;
    std::mutex mutex;
};

TEST_F(SceneActionTest, createSceneActionByEmptyRCSRemoteResourceObjectPtr)
{
    createServer("/a/testuri1_1", "/a/testuri1_2");
    createSceneCollectionAndScene();
    ASSERT_THROW(pScene1->addNewSceneAction(
            nullptr, KEY, VALUE), RCSInvalidParameterException);
}

TEST_F(SceneActionTest, createSceneActionByAlreadyExistedRCSRemoteResourceObjectPtr)
{
    createServer("/a/testuri2_1", "/a/testuri2_2");
    createSceneCollectionAndScene();
    createSceneAction();
    ASSERT_THROW(pScene1->addNewSceneAction(
            pRemoteResource1, KEY, "off"), RCSBadRequestException);
}

TEST_F(SceneActionTest, getSceneActionInstance)
{
    createServer("/a/testuri3_1", "/a/testuri3_2");
    createSceneCollectionAndScene();
    createSceneAction();

    for(const auto &it : pSceneAction1->getExecutionParameter())
    {
        ASSERT_EQ(it.key(), KEY);
        ASSERT_EQ(it.value(), VALUE);
    }

    for(const auto &it: pSceneAction2->getExecutionParameter())
    {
        ASSERT_EQ(it.key(), KEY_2);
        ASSERT_EQ(it.value(), VALUE_2);
    }
}

TEST_F(SceneActionTest, updateSceneAction)
{
    createServer("/a/testuri4_1", "/a/testuri4_2");
    createSceneCollectionAndScene();
    createSceneAction();

    pSceneAction1->resetExecutionParameter(KEY, "off");
    for(const auto &it : pSceneAction1->getExecutionParameter())
    {
        ASSERT_EQ(it.key(), KEY);
        ASSERT_EQ(it.value(), "off");
    }
}

TEST_F(SceneActionTest, getRemoteResourceObject)
{
    createServer("/a/testuri5_1", "/a/testuri5_2");
    createSceneCollectionAndScene();
    createSceneAction();

    ASSERT_EQ(pSceneAction1->getRemoteResourceObject(), pRemoteResource1);
}
