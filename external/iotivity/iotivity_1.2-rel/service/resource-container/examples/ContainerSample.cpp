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
#include <string.h>
#endif

#include "RCSResourceContainer.h"
#include "RCSBundleInfo.h"

#include <iostream>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>

using namespace OIC::Service;

namespace
{
    typedef enum
    {
        START_RC = 1, STOP_RC,
        ADD_SAMPLE_BUNDLE, START_SAMPLE_BUNDLE,
        STOP_SAMPLE_BUNDLE, REMOVE_SAMPLE_BUNDLE,
        ADD_SAMPLE_RESOURCE, REMOVE_SAMPLE_RESOURCE,
        LIST_BUNDLES, LIST_RESOURCES,
        EXIT = 11
    } APPMenu;

    struct CloseApp
    {
    };

    const int MAX_PATH = 2048;

    const std::string EXAMPLE_CONFIG_PATH = "/examples/ResourceContainerConfig.xml";
    const std::string EXAMPLE_BUNDLE_ID = "oic.bundle.hueSample";
    const std::string EXAMPLE_BUNDLE_URI = "/hueSample";
    const std::string EXAMPLE_BUNDLE_PATH = "libHueBundle.so";
    const std::string EXAMPLE_BUNDLE_ACTIVATOR = "huesample";
    const std::string EXAMPLE_RESOURCE_URI = "/hue/light/sample";
    const std::string EXAMPLE_RESOURCE_TYPE = "oic.r.light";
    const std::string EXAMPLE_ADDRESS = "http://192.168.0.2/api/newdeveloper/lights/1";
};

bool g_bContainerStarted = false;
bool g_bSampleBundleStarted = false;
RCSResourceContainer *g_pResourceContainer = nullptr;

void getCurrentPath(std::string *pPath)
{
    char buffer[MAX_PATH] = {0,};

    if (!pPath->empty())
    {
        pPath->clear();
    }

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

int processUserInput(int min, int max)
{
    int input = 0;

    std::cin >> input;

    if (!std::cin.fail() && min <= input && input <= max)
    {
        return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    throw std::runtime_error("Invalid Input, please try again");
}

std::string processUserStringInput()
{
    std::string input;

    std::cin >> input;

    return input;
}

void displayMenu()
{
    std::cout << "================================================================" << std::endl;
    std::cout << "          IoTivity Resource Container Test Application	      " << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "   1.  Start Resource Container                                 " << std::endl;
    std::cout << "   2.  Stop Resource Container                                  " << std::endl;
    std::cout << "   3.  Add Sample Bundle                                        " << std::endl;
    std::cout << "   4.  Start Sample Bundle                                      " << std::endl;
    std::cout << "   5.  Stop Sample Bundle                                       " << std::endl;
    std::cout << "   6.  Remove Sample Bundle                                     " << std::endl;
    std::cout << "   7.  Add Sample Resource                                      " << std::endl;
    std::cout << "   8.  Remove Sample Resource                                   " << std::endl;
    std::cout << "   9.  List Bundles                                             " << std::endl;
    std::cout << "   10. List Resources                                           " << std::endl;
    std::cout << "   11. Exit                                                     " << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "   Please Enter the NO: ";
}

bool checkBundleRegistered(std::string bundleId)
{
    std::list< std::unique_ptr< RCSBundleInfo > > bundleList;

    if (g_bContainerStarted)
    {
        bundleList = g_pResourceContainer->listBundles();

        for (auto &bundle : bundleList)
        {
            if (bundle->getID().compare(bundleId) == 0)
            {
                return true;
            }
        }
    }

    std::cout << "Bundle \'" << bundleId << "\' is not registered." << std::endl;
    return false;
}

bool checkResourceRegistered(std::string bundleId, std::string resourceUri)
{
    std::list< std::string > resourceList;

    if (g_bContainerStarted && checkBundleRegistered(bundleId))
    {
        resourceList = g_pResourceContainer->listBundleResources(bundleId);

        for (auto &resource : resourceList)
        {
            if (resource.compare(resourceUri) == 0)
            {
                return true;
            }
        }
    }

    std::cout << "Resource \'" << resourceUri << "\' is not registered." << std::endl;
    return false;
}

void StartContainer(std::string configPath)
{
    if (!g_bContainerStarted)
    {
        g_pResourceContainer->startContainer(configPath);
        g_bContainerStarted = true;
        std::cout << "Container started." << std::endl;
    }
    else
    {
        std::cout << "Container is already started." << std::endl;
    }
}

void StopContainer()
{
    if (g_pResourceContainer && g_bContainerStarted)
    {
        g_pResourceContainer->stopContainer();
        g_bContainerStarted = false;
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void AddSampleBundle()
{
    std::map< std::string, std::string > bundleParams;

    if (g_pResourceContainer && g_bContainerStarted)
    {
        g_pResourceContainer->addBundle(EXAMPLE_BUNDLE_ID, EXAMPLE_BUNDLE_URI,
                                        EXAMPLE_BUNDLE_PATH, EXAMPLE_BUNDLE_ACTIVATOR,
                                        bundleParams);
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void StartSampleBundle()
{
    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkBundleRegistered(EXAMPLE_BUNDLE_ID))
        {
            g_pResourceContainer->startBundle(EXAMPLE_BUNDLE_ID);
            g_bSampleBundleStarted = true;
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void StopSampleBundle()
{
    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkBundleRegistered(EXAMPLE_BUNDLE_ID) && g_bSampleBundleStarted)
        {
            g_pResourceContainer->stopBundle(EXAMPLE_BUNDLE_ID);
            g_bSampleBundleStarted = false;
        }
        else
        {
            std::cout << "Sample Bundle is not started." << std::endl;
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void RemoveSampleBundle()
{
    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkBundleRegistered(EXAMPLE_BUNDLE_ID))
        {
            g_pResourceContainer->removeBundle(EXAMPLE_BUNDLE_ID);
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void AddSampleBundleResource()
{
    std::map< std::string, std::string > resourceParams;

    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkBundleRegistered(EXAMPLE_BUNDLE_ID) && g_bSampleBundleStarted)
        {
            resourceParams.insert(std::make_pair("resourceType", EXAMPLE_RESOURCE_TYPE));
            resourceParams.insert(std::make_pair("address", EXAMPLE_ADDRESS));

            g_pResourceContainer->addResourceConfig(EXAMPLE_BUNDLE_ID, EXAMPLE_RESOURCE_URI,
                                                    resourceParams);
        }
        else
        {
            std::cout << "Sample Bundle is not started." << std::endl;
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void RemoveSampleBundleResource()
{
    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkResourceRegistered(EXAMPLE_BUNDLE_ID, EXAMPLE_RESOURCE_URI)
            && g_bSampleBundleStarted)
        {
            g_pResourceContainer->removeResourceConfig(EXAMPLE_BUNDLE_ID, EXAMPLE_RESOURCE_URI);
        }
        else
        {
            std::cout << "Sample Bundle is not started." << std::endl;
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void printBundleList(std::list< std::unique_ptr< RCSBundleInfo > > &list)
{
    std::cout << std::endl;
    for (auto &bundleinfo : list)
    {
        std::cout << "-- " << bundleinfo->getID() << std::endl;
    }
    std::cout << std::endl;
}

void ListBundles()
{
    std::list< std::unique_ptr< RCSBundleInfo > > bundles;

    if (g_pResourceContainer && g_bContainerStarted)
    {
        bundles = g_pResourceContainer->listBundles();
        printBundleList(bundles);
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void printResourceList(std::list< std::string > &list)
{
    std::cout << std::endl;
    for (auto &bundleResource : list)
    {
        std::cout << "-- " << bundleResource << std::endl;
    }
    std::cout << std::endl;
}

void ListResources(std::string bundleId)
{
    std::list< std::string > resources;

    if (g_pResourceContainer && g_bContainerStarted)
    {
        if (checkBundleRegistered(bundleId))
        {
            resources = g_pResourceContainer->listBundleResources(bundleId);
            printResourceList(resources);
        }
    }
    else
    {
        std::cout << "Container is not started." << std::endl;
    }
}

void ExecuteCommand(APPMenu menu)
{
    switch (menu)
    {
        case APPMenu::START_RC:
            {
                std::string filePath;
                std::cout << "Type Configuration File Path (Press \'0\' to start with example): ";

                if ((filePath = processUserStringInput()).compare("0") == 0)
                {
                    getCurrentPath(&filePath);
                    filePath.append(EXAMPLE_CONFIG_PATH);
                }

                StartContainer(filePath);
            }
            break;
        case APPMenu::STOP_RC:
            StopContainer();
            break;
        case APPMenu::ADD_SAMPLE_BUNDLE:
            AddSampleBundle();
            break;
        case APPMenu::START_SAMPLE_BUNDLE:
            StartSampleBundle();
            break;
        case APPMenu::STOP_SAMPLE_BUNDLE:
            StopSampleBundle();
            break;
        case APPMenu::REMOVE_SAMPLE_BUNDLE:
            RemoveSampleBundle();
            break;
        case APPMenu::ADD_SAMPLE_RESOURCE:
            AddSampleBundleResource();
            break;
        case APPMenu::REMOVE_SAMPLE_RESOURCE:
            RemoveSampleBundleResource();
            break;
        case APPMenu::LIST_BUNDLES:
            ListBundles();
            break;
        case APPMenu::LIST_RESOURCES:
            {
                std::string bundleId;
                std::cout <<
                          "Type Bundle Id to get Resource List (Press \'0\' to get example resource list): ";

                if ((bundleId = processUserStringInput()).compare("0") == 0)
                {
                    bundleId = EXAMPLE_BUNDLE_ID;
                }

                ListResources(bundleId);
            }
            break;
        case APPMenu::EXIT:
            throw CloseApp();
            break;
    }
}

int main()
{
    g_pResourceContainer = RCSResourceContainer::getInstance();

    while (true)
    {
        try
        {
            displayMenu();
            ExecuteCommand((APPMenu)processUserInput(APPMenu::START_RC, APPMenu::EXIT));
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch (const CloseApp &)
        {
            break;
        }
    }

    if (g_bContainerStarted)
    {
        g_pResourceContainer->stopContainer();
    }

    g_pResourceContainer = nullptr;

    return 0;
}
