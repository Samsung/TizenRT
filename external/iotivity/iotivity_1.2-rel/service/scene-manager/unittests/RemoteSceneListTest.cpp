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

#include <mutex>
#include <condition_variable>

#include "RemoteSceneList.h"
#include "UnitTestHelper.h"
#include "SceneCommons.h"
#include "SceneList.h"
#include "RCSRemoteResourceObject.h"
#include "OCPlatform.h"
#include "RCSDiscoveryManager.h"
#include "RCSAddress.h"

using namespace std;
using namespace OIC::Service;
using namespace OC;

constexpr int DEFAULT_WAITTIME = 2000;

SceneList* g_sceneList = SceneList::getInstance();
RCSRemoteResourceObject::Ptr pListResource = nullptr;
RemoteSceneList::Ptr pSceneList = nullptr;

void discoverSceneListServer()
{
    if(pListResource == nullptr)
    {
        std::vector< std::string > vecRT{ SCENE_LIST_RT };
        std::vector< std::string > vecIF{ OC_RSRVD_INTERFACE_DEFAULT, OC::BATCH_INTERFACE };

        pListResource = SceneUtils::createRCSResourceObject(
                            "coap://" + SceneUtils::getNetAddress() + SCENE_LIST_URI,
                            SCENE_CONNECTIVITY, vecRT, vecIF);
    }
}

class RemoteSceneListTest : public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();
        g_sceneList->getName();
    }

    void waitForCallback(int waitingTime = DEFAULT_WAITTIME)
    {
        std::unique_lock< std::mutex > lock{ mutex };
        cond.wait_for(lock, std::chrono::milliseconds{ waitingTime });
    }

public:
    std::condition_variable cond;
    std::mutex mutex;

    void onRemoteSceneListCreated(RemoteSceneList::Ptr remoteSceneList, int)
    {
        pSceneList = std::move(remoteSceneList);
        cond.notify_all();
    }

    void onSetName(int)
    {
        cond.notify_all();
    }
};

TEST_F(RemoteSceneListTest, createRemoteSceneListInstance)
{
    discoverSceneListServer();
    if(pSceneList == nullptr)
    {
        RemoteSceneList::createInstance(pListResource, std::bind(
            &RemoteSceneListTest::onRemoteSceneListCreated, this,
            placeholders::_1, placeholders::_2));

        waitForCallback();
    }

    EXPECT_NE(nullptr, pSceneList);
}

TEST_F(RemoteSceneListTest, setAndGetRemoteSceneListName)
{
    discoverSceneListServer();
    if(pSceneList == nullptr)
    {
        RemoteSceneList::createInstance(pListResource, std::bind(
            &RemoteSceneListTest::onRemoteSceneListCreated, this,
            placeholders::_1, placeholders::_2));

        waitForCallback();
    }

    pSceneList->setName("Test Scene List", std::bind(
        &RemoteSceneListTest::onSetName, this, placeholders::_1));

    waitForCallback();

    EXPECT_EQ("Test Scene List", pSceneList->getName());
}