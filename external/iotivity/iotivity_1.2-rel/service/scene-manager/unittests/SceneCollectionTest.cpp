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
#include "OCPlatform.h"

using namespace std;
using namespace OIC::Service;
using namespace OC;

class SceneCollectionTest: public TestWithMock
{
protected:
    void SetUp()
    {
        TestWithMock::SetUp();
        pSceneList = SceneList::getInstance();
    }

public:
    SceneList* pSceneList;
    std::shared_ptr<SceneCollection> pSceneCollection;
};

TEST_F(SceneCollectionTest, createSceneCollectionInstanceAndSceneCollectionResource)
{
    bool isNullPtr = false;
    pSceneCollection = pSceneList->addNewSceneCollection();

    if(pSceneCollection->getId() == "")
    {
        isNullPtr = true;
    }

    EXPECT_FALSE(isNullPtr);
}

TEST_F(SceneCollectionTest, getSceneCollectionInstanceAndSceneCollectionResource)
{
    auto sceneCollections = pSceneList->getSceneCollections();
    bool isNullPtr = false;

    for(const auto &it : sceneCollections)
    {
        if(it->getId() == "")
        {
            isNullPtr = true;
        }
        ASSERT_FALSE(isNullPtr);
    }
}

TEST_F(SceneCollectionTest, setAndGetSceneCollectionResourceName)
{
    pSceneCollection = pSceneList->addNewSceneCollection();
    pSceneCollection->setName("Kitchen");
    auto sceneCollectionName = pSceneCollection->getName();

    EXPECT_EQ("Kitchen", sceneCollectionName);
}
