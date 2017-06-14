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

#include <iostream>
#include <vector>
#include <algorithm>
#include <condition_variable>

#include "OCPlatform.h"
#include "RCSDiscoveryManager.h"
#include "RCSRemoteResourceObject.h"
#include "RCSAddress.h"
#include "RemoteSceneList.h"

using namespace OC;
using namespace OIC::Service;

constexpr int CREATE_REMOTE_SCENE_LIST = 1;

constexpr int CREATE_REMOTE_SCENE_COLLECTION = 1;
constexpr int SHOW_REMOTE_SCENE_COLLECTION = 2;

constexpr int CREATE_REMOTE_SCENE = 1;
constexpr int CREATE_REMOTE_SCENE_ACTION = 1;

constexpr int EXECUTE_REMOTE_SCENE = 1;

constexpr int SCENE_RESULT_OK = 200;

constexpr int numCreatedSceneAction = 2;
static int numRecvSceneActionCreationResp = 0;

typedef std::function< void() > Run;
Run g_currentRun;

const std::string scene_name = "Night mode";
const std::string relativetUri = OC_RSRVD_WELL_KNOWN_URI;
const std::vector<std::string> resourceTypes{ "oic.wk.sceneList", "core.light", "core.fan" };

std::mutex g_mtx;
std::mutex g_discoverymtx;
std::condition_variable g_cond;

std::unique_ptr<RCSDiscoveryManager::DiscoveryTask> g_discoveryTask;

RCSRemoteResourceObject::Ptr g_foundListResource;
RCSRemoteResourceObject::Ptr g_foundLightResource;
RCSRemoteResourceObject::Ptr g_foundFanResource;

RemoteSceneList::Ptr g_sceneList;
RemoteSceneCollection::Ptr g_sceneCollection;
RemoteScene::Ptr g_scene;

void displaySceneList();
void runCreateRemoteSceneList();
void runRemoteSceneCollection();
void runCreateRemoteScene();
void runCreateRemoteSceneAction();
void runExecuteCreatedRemoteScene();
void runExecuteExistingRemoteScene();

// Scene Manager Remote API sample ---
void onRemoteSceneListCreated(RemoteSceneList::Ptr remoteSceneList, int eCode)
{
    std::cout << __func__ << " - error code : " << eCode << std::endl;

    if (eCode == SCENE_RESULT_OK)
    {
        g_sceneList = std::move(remoteSceneList);
        g_currentRun = runRemoteSceneCollection;
    }
    else
    {
        std::cout << "Create Remote scene list failed." << std::endl;
        g_currentRun = runCreateRemoteSceneList;
    }
    g_currentRun();
}

void onRemoteSceneCollectionCreated(RemoteSceneCollection::Ptr remoteSceneCol, int eCode)
{
    std::cout << __func__ << " - error code : " << eCode << std::endl;

    if (eCode == SCENE_RESULT_OK)
    {
        g_sceneCollection = remoteSceneCol;
        g_currentRun = runCreateRemoteScene;
    }
    else
    {
        std::cout << "Create Remote scene collection failed." << std::endl;
        g_currentRun = runRemoteSceneCollection;
    }

    g_currentRun();
}

void onRemoteSceneCreated(RemoteScene::Ptr remoteScene, int eCode)
{
    std::cout << __func__ << " - error code : " << eCode << std::endl;

    if (eCode == SCENE_RESULT_OK)
    {
        g_scene = remoteScene;

        g_currentRun = runCreateRemoteSceneAction;
    }
    else
    {
        std::cout << "Create Remote scene failed." << std::endl;
        g_currentRun = runCreateRemoteScene;
    }

    g_currentRun();
}

void onRemoteSceneActionCreated(RemoteSceneAction::Ptr, int eCode)
{
    std::cout << __func__ << " - error code : " << eCode << std::endl;

    if (eCode == SCENE_RESULT_OK)
    {
        g_currentRun = runExecuteCreatedRemoteScene;
    }
    else
    {
        std::cout << "Create Remote scene action failed." << std::endl;
        g_currentRun = runCreateRemoteSceneAction;
    }

    numRecvSceneActionCreationResp++;

    if(numCreatedSceneAction == numRecvSceneActionCreationResp)
        g_currentRun();
}

void onRemoteSceneExecuted(const std::string &sceneName, int eCode)
{
    std::cout << __func__ << " - scene name : " << sceneName
              <<  ", error code : " << eCode << std::endl;

    if (eCode != SCENE_RESULT_OK)
    {
        std::cout << "Execute scene failed." << std::endl;
    }

    g_currentRun();
}

// --- Scene Manager Remote API sample

void createRemoteSceneList()
{
    if (g_foundListResource)
    {
        RemoteSceneList::createInstance(g_foundListResource, onRemoteSceneListCreated);
    }
    else
    {
        std::cout << "Scene List Resource is not discovered." << std::endl;
        g_currentRun();
    }
}

void createRemoteSceneCollection()
{
    if (!g_sceneList) return;

    g_sceneList->addNewSceneCollection(onRemoteSceneCollectionCreated);
}

void showRemoteSceneCollection()
{
    if (!g_sceneList) return;

    if (g_sceneList->getRemoteSceneCollections().size() == 0) return;

    g_sceneCollection = g_sceneList->getRemoteSceneCollections().at(0);

    if( g_sceneCollection->getRemoteScenes().size() == 0) return;

    g_scene = g_sceneCollection->getRemoteScenes().begin()->second;
}

void createRemoteScene()
{
    if (!g_sceneCollection) return;

    g_sceneCollection->addNewScene(scene_name, onRemoteSceneCreated);
}

void createRemoteSceneAction(
    RemoteScene::Ptr scene, RCSRemoteResourceObject::Ptr member,
    const std::string &key, const std::string &value)
{
    if (scene && member)
    {
        g_scene->addNewSceneAction(member, key, RCSResourceAttributes::Value(value),
            onRemoteSceneActionCreated);
    }
}

void createRemoteSceneActions()
{
    createRemoteSceneAction(g_scene, g_foundLightResource, "power", "on");
    createRemoteSceneAction(g_scene, g_foundFanResource, "speed", "50");
}

void executeScene()
{
    displaySceneList();

    if (g_scene)
    {
        g_scene->execute(onRemoteSceneExecuted);
        std::cout << "\n\t'" << g_scene->getName() << "' is executed!\n" << std::endl;
    }
}

// --- Scene Manager Remote API sample
void configurePlatform()
{
    PlatformConfig config
    {
        OC::ServiceType::InProc, ModeType::Both, "0.0.0.0", 0, OC::QualityOfService::LowQos
    };
    OCPlatform::Configure(config);
}

int processUserInput(int min, int max)
{
    assert(min <= max);

    int input;

    std::cin >> input;

    if (!std::cin.fail())
    {
        if (input == max + 1)  exit(0);
        if (min <= input && input <= max) return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

void displaySceneList()
{
    if (!g_sceneList) return;

    std::cout << "\t" << g_sceneList->getName() << "(SceneList)" << std::endl;

    if (!g_sceneCollection) return;

    std::cout << "\t\t   |_ _ _ " << g_sceneCollection->getId() << " (SceneCollection)" << std::endl;

    for( const auto &it_scene : g_sceneCollection->getRemoteScenes() )
    {
        std::cout << "\t\t\t   |_ _ _ " << it_scene.first << " (Scene)" << std::endl;

        auto sceneActionList = it_scene.second->getRemoteSceneActions();
        for (const auto &it : sceneActionList)
        {
            auto attr = it->getExecutionParameter();
            for (const auto &att : attr)
            {
                std::cout << "\t\t\t      \t\t|_ _ _ ";
                std::cout << it->getRemoteResourceObject()->getUri() << " : ";
                std::cout << att.key() << " - " << att.value().toString() << std::endl;
            }
        }
    }
}

void displayClear(Run runFunc)
{
    auto ret = std::system("/usr/bin/clear");
    if(ret == -1)
    {
        std::cout << "clear error!" << std::endl;
    }
    g_currentRun = runFunc;
}

void displayCreateRemoteSceneListMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_REMOTE_SCENE_LIST  << ". Create a RemoteSceneList \n";
    std::cout << CREATE_REMOTE_SCENE_LIST + 1  << ". Quit                 \n";
    std::cout << "========================================================\n";
}

void displayRemoteSceneCollectionMenu()
{
    std::cout << "========================================================               \n";
    std::cout << CREATE_REMOTE_SCENE_COLLECTION  << ". Create a RemoteSceneCollection    \n";
    std::cout << SHOW_REMOTE_SCENE_COLLECTION  << ". Show existing RemoteSceneCollection \n";
    std::cout << SHOW_REMOTE_SCENE_COLLECTION + 1  << ". Quit                            \n";
    std::cout << "========================================================               \n";
}

void displayRemoteSceneCreationMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_REMOTE_SCENE  << ". Create a RemoteScene          \n";
    std::cout << CREATE_REMOTE_SCENE + 1  << ". Quit                      \n";
    std::cout << "========================================================\n";
}

void displayRemoteSceneActionCreationMenu()
{
    std::cout << "========================================================   \n";
    std::cout << CREATE_REMOTE_SCENE_ACTION  << ". Create RemoteSceneActions \n";
    std::cout << CREATE_REMOTE_SCENE_ACTION + 1  << ". Quit                  \n";
    std::cout << "========================================================   \n";
}

void displayExecuteCreatedRemoteSceneCreationMenu()
{
    std::cout << "========================================================\n";
    std::cout << EXECUTE_REMOTE_SCENE  << ". Execute RemoteScene          \n";
    std::cout << EXECUTE_REMOTE_SCENE + 1  << ". Quit                     \n";
    std::cout << "========================================================\n";
}

void displayExecuteExistingRemoteSceneCreationMenu()
{
    std::cout << "========================================================\n";
    std::cout << EXECUTE_REMOTE_SCENE  << ". Execute a first RemoteScene  \n";
    std::cout << EXECUTE_REMOTE_SCENE + 1  << ". Quit                     \n";
    std::cout << "========================================================\n";
}

void runExecuteExistingRemoteScene()
{
    displaySceneList();

    displayExecuteExistingRemoteSceneCreationMenu();

    try
    {
        int command = processUserInput(EXECUTE_REMOTE_SCENE, EXECUTE_REMOTE_SCENE);
        switch(command)
        {
            case EXECUTE_REMOTE_SCENE:
                executeScene();
                displayClear(runExecuteExistingRemoteScene);
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void runExecuteCreatedRemoteScene()
{
    displaySceneList();

    displayExecuteCreatedRemoteSceneCreationMenu();

    try
    {
        int command = processUserInput(EXECUTE_REMOTE_SCENE, EXECUTE_REMOTE_SCENE);
        switch(command)
        {
            case EXECUTE_REMOTE_SCENE:
                executeScene();
                displayClear(runExecuteCreatedRemoteScene);
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void runCreateRemoteSceneAction()
{
    displaySceneList();

    displayRemoteSceneActionCreationMenu();

    try
    {
        int command = processUserInput(CREATE_REMOTE_SCENE_ACTION, CREATE_REMOTE_SCENE_ACTION);
        switch(command)
        {
            case CREATE_REMOTE_SCENE_ACTION:
                createRemoteSceneActions();
                displayClear(runExecuteCreatedRemoteScene);
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void runCreateRemoteScene()
{
    displaySceneList();

    displayRemoteSceneCreationMenu();

    try
    {
        int command = processUserInput(CREATE_REMOTE_SCENE, CREATE_REMOTE_SCENE);
        switch(command)
        {
            case CREATE_REMOTE_SCENE:
                createRemoteScene();
                displayClear(runCreateRemoteSceneAction);
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void runRemoteSceneCollection()
{
    displaySceneList();

    displayRemoteSceneCollectionMenu();

    try
    {
        int command = processUserInput(CREATE_REMOTE_SCENE_COLLECTION, SHOW_REMOTE_SCENE_COLLECTION);
        switch(command)
        {
            case CREATE_REMOTE_SCENE_COLLECTION:
                createRemoteSceneCollection();
                displayClear(runCreateRemoteScene);
                break;
            case SHOW_REMOTE_SCENE_COLLECTION:
                showRemoteSceneCollection();
                displayClear(runExecuteExistingRemoteScene);
                g_currentRun();
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void runCreateRemoteSceneList()
{
    displayCreateRemoteSceneListMenu();

    try
    {
        int command = processUserInput(CREATE_REMOTE_SCENE_LIST, CREATE_REMOTE_SCENE_LIST);
        switch(command)
        {
            case CREATE_REMOTE_SCENE_LIST:
                createRemoteSceneList();
                displayClear(runRemoteSceneCollection);
                break;
        }
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        g_currentRun();
    }
}

void onResourceDiscovered(std::shared_ptr<RCSRemoteResourceObject> foundResource)
{
    std::lock_guard< std::mutex > lock(g_discoverymtx);
    std::cout << "onResourceDiscovered callback" << std::endl;

    std::string resourceURI = foundResource->getUri();
    std::string hostAddress = foundResource->getAddress();
    std::vector< std::string > vecRTs = foundResource->getTypes();

    std::cout << "\t\tResource URI : " << resourceURI << std::endl;
    std::cout << "\t\tResource Host : " << hostAddress << std::endl;

    // if the found resource is a scene list resource
    if (std::find(vecRTs.begin(), vecRTs.end(), "oic.wk.sceneList") != vecRTs.end())
        g_foundListResource = foundResource;

    // if the found resource is a light resource
    else if (std::find(vecRTs.begin(), vecRTs.end(), "core.light") != vecRTs.end())
    {
        g_foundLightResource = foundResource;
    }

    // if the found resource is a fan resource
    else if (std::find(vecRTs.begin(), vecRTs.end(), "core.fan") != vecRTs.end())
    {
        g_foundFanResource = foundResource;
    }

    if (g_foundListResource && g_foundLightResource && g_foundFanResource)
    {
        g_discoveryTask->cancel();
        return;
    }

    g_cond.notify_all();
}

void discoverResource()
{
    std::cout << "Wait 4 seconds until discovered." << std::endl;

    try
    {
        g_discoveryTask
            = RCSDiscoveryManager::getInstance()->discoverResourceByTypes(RCSAddress::multicast(),
                    relativetUri, resourceTypes, &onResourceDiscovered);
    }
    catch (const RCSPlatformException &e)
    {
        std::cout << e.what() << std::endl;
    }
    std::unique_lock<std::mutex> lck(g_mtx);

    g_cond.wait_for(lck, std::chrono::seconds(4));
    return;
}

int main()
{
    configurePlatform();

    try
    {
        discoverResource();

        g_currentRun = runCreateRemoteSceneList;
        g_currentRun();
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }

    while (true) { }

    std::cout << "Stopping the scene client" << std::endl;

    return 0;
}
