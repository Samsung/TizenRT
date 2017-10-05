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

#include <condition_variable>
#include <iostream>
#include <vector>
#include <condition_variable>

#include "OCPlatform.h"
#include "RCSDiscoveryManager.h"
#include "RCSRemoteResourceObject.h"
#include "RCSResourceAttributes.h"
#include "RCSResourceObject.h"
#include "RCSAddress.h"
#include "SceneList.h"

using namespace OC;
using namespace OIC::Service;

constexpr int CREATE_SCENE_LIST = 1;

constexpr int CREATE_SCENE_COLLECTION = 1;

constexpr int CREATE_SCENE = 1;

constexpr int CREATE_SCENE_ACTION = 1;

constexpr int EXECUTE_SCENE_1 = 1;
constexpr int EXECUTE_SCENE_2 = 2;

typedef void (*DisplayControlMenuFunc)();
typedef std::function<void()> Run;

std::unique_ptr<RCSDiscoveryManager::DiscoveryTask> discoveryTask;
Run g_currentRun;

struct CloseApp {};

const std::vector<std::string> resourceTypes{"core.light", "core.fan"};
const std::string relativetUri = OC_RSRVD_WELL_KNOWN_URI;

std::mutex mtx;
std::condition_variable cond;

RCSRemoteResourceObject::Ptr g_selectedResource;
std::shared_ptr<RCSRemoteResourceObject> g_discoveredResources;

std::vector<RCSRemoteResourceObject::Ptr> g_foundResourceList;
std::vector<RCSResourceObject::Ptr> g_memberResourceList;

SceneCollection::Ptr g_sceneColObj;
Scene::Ptr g_scene;
Scene::Ptr g_scene_2;
SceneAction::Ptr g_sceneAction;

typedef std::function<void(std::shared_ptr<RCSRemoteResourceObject>)> DiscoveryCallback;
typedef std::function<void (int)> ExecuteCallback;

void onExecute(int /*Code*/)
{
    std::cout << __func__ << std::endl;
}

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
        if(input == max + 1) throw CloseApp();
        if(min <= input && input <= max) return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

void displayCreateSceneListMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_SCENE_LIST  << ". Create a SceneList                       \n";
    std::cout << CREATE_SCENE_LIST + 1  << ". Quit                       \n";
    std::cout << "========================================================\n";
}

void displayCreateSceneCollectionMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_SCENE_COLLECTION  << ". Create a SceneCollection                       \n";
    std::cout << CREATE_SCENE_COLLECTION + 1  << ". Quit                       \n";
    std::cout << "========================================================\n";
}

void displayCreateSceneMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_SCENE  << ". Create a Scene                       \n";
    std::cout << CREATE_SCENE + 1  << ". Quit                       \n";
    std::cout << "========================================================\n";
}

void displayCreateSceneActionMenu()
{
    std::cout << "========================================================\n";
    std::cout << CREATE_SCENE_ACTION  << ". Create a SceneAction                       \n";
    std::cout << CREATE_SCENE_ACTION + 1  << ". Quit                       \n";
    std::cout << "========================================================\n";
}

void displayExecuteSceneMenu()
{
    std::cout << "========================================================\n";
    std::cout << EXECUTE_SCENE_1  << ". Execute Scene1                       \n";
    std::cout << EXECUTE_SCENE_2  << ". Execute Scene2                       \n";
    std::cout << EXECUTE_SCENE_2 + 1  << ". Quit                       \n";
    std::cout << "========================================================\n";
}

void displaySceneList()
{
    std::cout << "\t" << SceneList::getInstance()->getName();
    std::cout << "(SceneList)" << std::endl;
}

void displaySceneCollection()
{
    std::cout << "\t\t   |_ _ _ " << g_sceneColObj->getName();
    std::cout << "(SceneCollection)" << std::endl;
}

void displayScene()
{
    std::cout << "\t\t\t      |_ _ _ " << g_scene->getName();
    std::cout << "(Scene)" << std::endl;
    std::cout << "\t\t\t      |_ _ _ " << g_scene_2->getName();
    std::cout << "(Scene)" << std::endl;
}

void displayClear(Run runFunc)
{
    std::cout << "\nPress Enter to Continue....." << std::endl;
    std::cin.ignore();
    if(std::cin.get() == '\n')
    {
        auto ret = std::system("/usr/bin/clear");
        if(ret == -1)
        {
            std::cout << "clear error!" << std::endl;
        }
        g_currentRun = runFunc;
    }
}

void displaySceneAction()
{
    std::cout << "\t\t\t      |_ _ _ " << g_scene->getName();
    std::cout << "(Scene)" << std::endl;
    auto sceneActionList = g_scene->getSceneActions();
    for(const auto &it : sceneActionList)
    {
        auto attr = it->getExecutionParameter();
        for(const auto &att : attr)
        {
            std::cout << "\t\t\t      |\t\t|_ _ _ ";
            std::cout << it->getRemoteResourceObject()->getUri() << ":";
            std::cout << att.key() << " - "  << att.value().toString() << std::endl;
        }
    }

    std::cout << "\t\t\t      |_ _ _ " << g_scene_2->getName();
    std::cout << "(Scene)" << std::endl;

    sceneActionList = g_scene_2->getSceneActions();
    for(const auto &it : sceneActionList)
    {
        auto attr = it->getExecutionParameter();
        for(const auto &att : attr)
        {
            std::cout << "\t\t\t       \t\t|_ _ _ ";
            std::cout << it->getRemoteResourceObject()->getUri() << ":";
            std::cout << att.key() << " - "  << att.value().toString() << std::endl;
        }
    }
}

void onResourceDiscovered(std::shared_ptr<RCSRemoteResourceObject> foundResource)
{
    std::cout << "onResourceDiscovered callback" << std::endl;

    std::string resourceURI = foundResource->getUri();
    std::string hostAddress = foundResource->getAddress();

    std::cout << "\t\tResource URI : " << resourceURI << std::endl;
    std::cout << "\t\tResource Host : " << hostAddress << std::endl;

    g_foundResourceList.push_back(foundResource);

    cond.notify_all();
}

bool discoverResource()
{
    std::cout << "Wait 2 seconds until discovered." << std::endl;

    try
    {
        discoveryTask = RCSDiscoveryManager::getInstance()->discoverResourceByTypes(
                RCSAddress::multicast(), relativetUri, resourceTypes, &onResourceDiscovered);
    }
    catch(const RCSPlatformException& e)
    {
         std::cout << e.what() << std::endl;
    }
    catch(const RCSException& e)
    {
        std::cout << e.what() << std::endl;
    }
    std::unique_lock<std::mutex> lck(mtx);

    cond.wait_for(lck, std::chrono::seconds(4));
    return g_discoveredResources != nullptr;
}

void createSceneList()
{
    SceneList::getInstance()->setName("Home");
    displaySceneList();
}

void createSceneCollection()
{
    g_sceneColObj = SceneList::getInstance()->addNewSceneCollection();
    g_sceneColObj->setName("Living Room");
    displaySceneList();
    displaySceneCollection();
}

void createScene()
{
    try
    {
        g_scene = g_sceneColObj->addNewScene("Going Out");
        g_scene_2 = g_sceneColObj->addNewScene("TV mode");
    }
    catch(const RCSException& e)
    {
        std::cout << e.what() << std::endl;
    }

    displaySceneList();
    displaySceneCollection();
    displayScene();
}

void createSceneAction()
{
    try
    {
        g_scene->addNewSceneAction(g_foundResourceList.at(0), "power", "off");
        g_scene->addNewSceneAction(g_foundResourceList.at(1), "speed", "0");

        g_scene_2->addNewSceneAction(g_foundResourceList.at(0), "power", "on");
        g_scene_2->addNewSceneAction(g_foundResourceList.at(1), "speed", "20");
    }
    catch(const RCSException& e)
    {
        std::cout << e.what() << std::endl;
    }

    displaySceneList();
    displaySceneCollection();
    displaySceneAction();
}

void executeScene(int sceneNum)
{
    displaySceneList();
    displaySceneCollection();
    displaySceneAction();

    switch(sceneNum)
    {
        case 1:
            try
            {
                g_scene->execute(onExecute);
                std::cout << "\t'" << g_scene->getName() << "' is executed!" << std::endl;
            }
            catch(const RCSException& e)
            {
                std::cout << e.what() <<std::endl;
            }
            break;
        case 2:
            try
            {
                g_scene_2->execute(onExecute);
                std::cout << "\t'" << g_scene_2->getName() << "' is executed!" << std::endl;
            }
            catch(const RCSException& e)
            {
                std::cout << e.what() <<std::endl;
            }
            break;
    }
}

void runExecuteScene()
{
    displayExecuteSceneMenu();

    int command = processUserInput(EXECUTE_SCENE_1, EXECUTE_SCENE_2);
    switch(command)
    {
        case EXECUTE_SCENE_1:
            executeScene(1);
            break;
        case EXECUTE_SCENE_2:
            executeScene(2);
            break;
    }
    displayClear(runExecuteScene);
}

void runCreateSceneAction()
{
    displayCreateSceneActionMenu();

    int command = processUserInput(CREATE_SCENE_ACTION, CREATE_SCENE_ACTION);
    switch(command)
    {
        case CREATE_SCENE_ACTION:
            createSceneAction();
            displayClear(runExecuteScene);
            break;
    }
}

void runCreateScene()
{
    displayCreateSceneMenu();

    int command = processUserInput(CREATE_SCENE, CREATE_SCENE);
    switch(command)
    {
        case CREATE_SCENE:
            createScene();
            displayClear(runCreateSceneAction);
            break;
    }
}

void runCreateSceneCollection()
{
    displayCreateSceneCollectionMenu();

    int command = processUserInput(CREATE_SCENE_COLLECTION, CREATE_SCENE_COLLECTION);
    switch(command)
    {
        case CREATE_SCENE_COLLECTION:
            createSceneCollection();
            displayClear(runCreateScene);
            break;
    }
}

void runCreateSceneList()
{
    displayCreateSceneListMenu();

    int command = processUserInput(CREATE_SCENE_LIST, CREATE_SCENE_LIST);
    switch(command)
    {
        case CREATE_SCENE_LIST:
            discoveryTask->cancel();
            createSceneList();
            displayClear(runCreateSceneCollection);
            break;
    }
}

int main()
{
    configurePlatform();

    discoverResource();

    g_currentRun = runCreateSceneList;

    while (true)
    {
        try
        {
            g_currentRun();
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const CloseApp&)
        {
            break;
        }
    }

    std::cout << "Stopping the client" << std::endl;

    return 0;
}
