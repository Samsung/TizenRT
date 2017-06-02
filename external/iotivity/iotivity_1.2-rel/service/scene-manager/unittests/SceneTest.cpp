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

#include "SceneList.h"

#include "RCSResourceObject.h"
#include "RCSRemoteResourceObject.h"
#include "SceneCommons.h"
#include "OCPlatform.h"

#include <condition_variable>
#include <mutex>
#include <iostream>

using namespace std;
using namespace OIC::Service;
using namespace OC;

typedef std::function<void (int)> ExecuteCallback;

constexpr char RESOURCE_URI[]{ "/a/light" };
constexpr char RESOURCE_TYPE[]{ "core.light" };
constexpr char RESOURCE_URI2[]{ "/a/fan" };
constexpr char RESOURCE_TYPE2[]{ "core.fan" };
constexpr char KEY[]{ "power" };
constexpr char VALUE[]{ "off" };
constexpr char KEY_2[]{ "state" };
constexpr char VALUE_2[]{ "100" };

class SceneTest: public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();
        pSceneList = SceneList::getInstance();
    }
    void waitForCb(int watingTime)
    {
        std::unique_lock< std::mutex > lock{ mutex };
        cond.wait_for(lock, std::chrono::milliseconds{ watingTime });
    }
    void proceed()
    {
        cond.notify_all();
    }
    void createSceneCollection()
    {
        pSceneCollection = pSceneList->addNewSceneCollection();
    }
    void createScene()
    {
        pScene1 = pSceneCollection->addNewScene("SceneTestName_1");
        pScene2 = pSceneCollection->addNewScene("SceneTestName_2");
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

public:
    SceneList* pSceneList;
    shared_ptr<SceneCollection> pSceneCollection;
    shared_ptr<Scene> pScene1;
    shared_ptr<Scene> pScene2;
    RCSRemoteResourceObject::Ptr pRemoteResource1;
    RCSRemoteResourceObject::Ptr pRemoteResource2;

private:
    std::condition_variable cond;
    std::mutex mutex;
};
void executeCallback(int /*code*/) {};

TEST_F(SceneTest, createSceneInstance)
{
    createSceneCollection();
    createScene();
    ASSERT_EQ("SceneTestName_1", pScene1->getName());
    ASSERT_EQ("SceneTestName_2", pScene2->getName());
}

TEST_F(SceneTest, createSceneInstanceByEmptyName)
{
    createSceneCollection();
    ASSERT_THROW(pSceneCollection->addNewScene(""), RCSInvalidParameterException);
}

TEST_F(SceneTest, getSceneInstanceBySceneName)
{
    createSceneCollection();
    createScene();
    auto scene = pSceneCollection->getScene("SceneTestName_2");
    EXPECT_EQ("SceneTestName_2", scene->getName());
}

TEST_F(SceneTest, getAllSceneInstance)
{
    createSceneCollection();
    createScene();

    vector<string> sceneNames{"SceneTestName_1", "SceneTestName_2"};
    auto scenes = pSceneCollection->getScenes();
    int count = 0;

    for(const auto &it : scenes)
    {
        for (const auto &name : sceneNames)
        {
                if(it.first == name)
                {
                        count++;
                        break;
                }
        }
    }
    ASSERT_EQ(count, 2);
}

TEST_F(SceneTest, getSceneActionUsingRemoteResource)
{
    createServer(RESOURCE_URI, RESOURCE_URI2);
    createSceneCollection();
    createScene();

    auto pSceneAction1 = pScene1->addNewSceneAction(pRemoteResource1, KEY, VALUE);
    pSceneAction1->resetExecutionParameter(KEY_2, VALUE_2);

    ASSERT_EQ(pScene1->getSceneAction(pRemoteResource1)->getExecutionParameter(),
            pSceneAction1->getExecutionParameter());
}

TEST_F(SceneTest, getSceneActions)
{
    createServer("/a/testuri1_1", "/a/testuri1_2");
    createSceneCollection();
    createScene();

    auto pSceneAction1 = pScene1->addNewSceneAction(pRemoteResource1, KEY, VALUE);
    auto pSceneAction2 = pScene1->addNewSceneAction(pRemoteResource2, KEY, VALUE);

    for(const auto & it : pScene1->getSceneActions())
    {
        ASSERT_EQ(it->getExecutionParameter(), pSceneAction1->getExecutionParameter());
    }
}

TEST_F(SceneTest, executeScene)
{
    mocks.ExpectCallFunc(executeCallback).Do([this](int){ proceed(); });

    createServer("/a/testuri2_1", "/a/testuri2_2");
    createSceneCollection();
    createScene();
    pScene1->addNewSceneAction(pRemoteResource1, KEY, "on");
    pScene1->addNewSceneAction(pRemoteResource2, KEY_2, VALUE_2);

    pScene1->execute(executeCallback);
    waitForCb(100);
}

TEST_F(SceneTest, executeSceneUsingEmptyCallback)
{
    createServer("/a/testuri3_1", "/a/testuri3_2");
    createSceneCollection();
    createScene();
    pScene1->addNewSceneAction(pRemoteResource1, KEY, "on");
    pScene1->addNewSceneAction(pRemoteResource2, KEY_2, VALUE_2);

    ASSERT_THROW(pScene1->execute(nullptr), RCSInvalidParameterException);
}
