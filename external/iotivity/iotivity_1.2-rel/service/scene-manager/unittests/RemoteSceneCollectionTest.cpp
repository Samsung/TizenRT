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
#include "RCSResourceObject.h"
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
RemoteSceneCollection::Ptr pSceneCollection = nullptr;
RemoteScene::Ptr pScene = nullptr;

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

class RemoteSceneCollectionTest : public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();

        g_sceneList->getName();
        discoverSceneListServer();

        if(pSceneList == nullptr){
            RemoteSceneList::createInstance(pListResource, std::bind(
                &RemoteSceneCollectionTest::onRemoteSceneListCreated, this,
                placeholders::_1, placeholders::_2));
            waitForCallback();
        }
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

    void onRemoteSceneCollectionCreated(RemoteSceneCollection::Ptr remoteSceneCol, int)
    {
        pSceneCollection = remoteSceneCol;
        cond.notify_all();
    }

    void onRemoteSceneCreated(RemoteScene::Ptr remoteScene, int)
    {
        pScene = remoteScene;
        cond.notify_all();
    }

    void onSetName(int)
    {
        cond.notify_all();
    }
};

TEST_F(RemoteSceneCollectionTest, addNewRemoteSceneCollection)
{
    if(pSceneCollection == nullptr)
    {
        pSceneList->addNewSceneCollection(std::bind(
            &RemoteSceneCollectionTest::onRemoteSceneCollectionCreated, this,
            placeholders::_1, placeholders::_2));
        waitForCallback();
    }

    if(pScene == nullptr)
    {
        pSceneCollection->addNewScene("Default", std::bind(
            &RemoteSceneCollectionTest::onRemoteSceneCreated, this,
            placeholders::_1, placeholders::_2));
        waitForCallback();
    }

    EXPECT_NE(nullptr, pSceneCollection);
}

TEST_F(RemoteSceneCollectionTest, getRemoteSceneCollectionList)
{
    std::vector< RemoteSceneCollection::Ptr > sceneCollections
        = pSceneList->getRemoteSceneCollections();

    bool getCollectionsOK = true;

    if (!sceneCollections.empty())
    {
        for (const auto &it : sceneCollections)
        {
            if (it->getId() == "")
            {
                getCollectionsOK = false;
                break;
            }
        }
    }
    else
    {
        getCollectionsOK = false;
    }

    ASSERT_TRUE(getCollectionsOK);
}

TEST_F(RemoteSceneCollectionTest, setAndGetSceneCollectionName)
{
    if(pSceneCollection == nullptr)
    {
        pSceneList->addNewSceneCollection(std::bind(
            &RemoteSceneCollectionTest::onRemoteSceneCollectionCreated, this,
            placeholders::_1, placeholders::_2));
        waitForCallback();
    }

    if(pScene == nullptr)
    {
        pSceneCollection->addNewScene("Default", std::bind(
            &RemoteSceneCollectionTest::onRemoteSceneCreated, this,
            placeholders::_1, placeholders::_2));
        waitForCallback();
    }

    pSceneCollection->setName("Kitchen", std::bind(
        &RemoteSceneCollectionTest::onSetName, this, placeholders::_1));

    waitForCallback();

    EXPECT_EQ("Kitchen", pSceneCollection->getName());
}