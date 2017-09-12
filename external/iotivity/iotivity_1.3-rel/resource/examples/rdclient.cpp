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

#include "OCPlatform.h"
#include "OCApi.h"
#include "RDClient.h"

#include <functional>
#include <iostream>

using namespace std;
using namespace OC;

static const char* SVR_DB_FILE_NAME = "./oic_svr_db_server.dat";

string rdAddress;
OCConnectivityType connectivityType = CT_ADAPTER_TCP;

ResourceHandles m_publishedResourceHandles;

static void onDelete(const int& eCode);
static void onPublish(const OCRepresentation& rep, const int& eCode);

class Resource
{

public:
    std::string m_resourceUri;
    OCResourceHandle m_resourceHandle;

public:
    /// This function internally calls registerResource API.
    void registerResource()
    {
        string resourceURI;
        string resourceTypeName;
        string resourceInterface;

        std::cout <<"   resourceURI: ";
        std::cin >> resourceURI;
        std::cout <<"   resourceTypeName: ";
        std::cin >> resourceTypeName;
        std::cout <<"   resourceInterface: ";
        std::cin >> resourceInterface;

        m_resourceUri = resourceURI;

        OCStackResult ret = OCPlatform::registerResource(m_resourceHandle,
                                                         resourceURI,
                                                         resourceTypeName,
                                                         resourceInterface,
                                                         nullptr,
                                                         OC_DISCOVERABLE);

        if (OC_STACK_OK != ret)
        {
            cout << "Resource creation was unsuccessful\n";
            return;
        }

        m_publishedResourceHandles.push_back(m_resourceHandle);
        cout << "registerResource is called." << endl;
    }

    void updateResource()
    {
        cout << "   1:: add resource type\n";
        cout << "   2:: add resource interface\n";

        int selectedMenu;
        std::cin >> selectedMenu;

        string inputString;
        switch (selectedMenu)
        {
        case 1:
            std::cout << "Add Resource Type" << std::endl;
            std::cout <<"   resourceTypeName: ";
            std::cin >> inputString;
            OCPlatform::bindTypeToResource(m_resourceHandle, inputString);
            break;
        case 2:
            std::cout << "Add Resource Interface" << std::endl;
            std::cout <<"   resourceInterface: ";
            std::cin >> inputString;
            OCPlatform::bindInterfaceToResource(m_resourceHandle, inputString);
            break;
        default:
            cout << "Invalid option" << endl;
            return;
        }
    }

    void publishResource()
    {
        /*
         * Publish Resource of Resource-Server to RD.
         */

        OCStackResult result = RDClient::Instance().publishResourceToRD(rdAddress, connectivityType,
                                                                         m_publishedResourceHandles, &onPublish);
        if (OC_STACK_OK != result)
        {
            cout << "Resource publish was unsuccessful\n";
        }
    }

    void deleteResource()
    {
        /*
         * Delete Resource with Resource Handle.
         * Don't need to include resource handle,
         * if resource-server want to delete all resources from RD.
         * Ex.) OCPlatform::deleteResourceFromRD(rdAddress, connectivityType, &onDelete);
         */
        OCStackResult result = RDClient::Instance().deleteResourceFromRD(rdAddress, connectivityType,
                                                                          m_publishedResourceHandles, &onDelete);
        if (OC_STACK_OK != result)
        {
            cout << "Resource delete was unsuccessful\n";
        }
    }
};

void onDelete(const int& eCode)
{
    cout << "Received Delete Resource Response From RD\n";
    try
    {
        if (OC_STACK_RESOURCE_DELETED == eCode)
        {
            cout << "Resource delete was successful\n";
        }
        else
        {
            std::cout << "onDelete Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onDelete" << std::endl;
    }
}

void onPublish(const OCRepresentation& rep, const int& eCode)
{
    cout << endl <<"Received Publish Resource Response From RD\n";

    try
    {
        if (OC_STACK_RESOURCE_CHANGED == eCode)
        {
            cout << "=========== Published Resource ===========" << endl;
            if (rep.hasAttribute("di"))
            {
                std::cout << " di: " << rep.getValue<std::string>("di") << std::endl;
            }

            // Published Resource is included as the child resource.
            std::vector<OCRepresentation> children = rep.getChildren();

            for (auto oit = children.begin(); oit != children.end(); ++oit)
            {
                std::string m_href;
                oit->getValue("href", m_href);
                cout << "   href : " << m_href << "\n";

                cout << "   resource type : \n";
                for(const std::string& type : oit->getResourceTypes())
                {
                    cout << "       " <<  type << "\n";
                }

                cout << "   resource interface : \n";
                for(const std::string& type : oit->getResourceInterfaces())
                {
                    cout << "       " <<  type << "\n";
                }

                int m_ins;
                oit->getValue("ins", m_ins);
                cout << "   ins : " << m_ins << "\n";
            }
            cout << "=========================================" << endl;
        }
        else
        {
            std::cout << "onPublish Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onPublish" << std::endl;
    }
}

static void printUsage()
{
    std::cout<<"Usage: rdserver <coap+tcp://10.11.12.13:5683>\n";
}

static FILE* client_open(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen(SVR_DB_FILE_NAME, mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

int main(int argc, char* argv[])
{
    ostringstream requestURI;

    if (argc == 2)
    {
        rdAddress = argv[1];
    }
    else
    {
        printUsage();
        return -1;
    }

    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };
    PlatformConfig config
    { OC::ServiceType::InProc, ModeType::Both, &ps};

    try
    {
        OCPlatform::Configure(config);
        OC_VERIFY(OCPlatform::start() == OC_STACK_OK);

        int selectedMenu;
        bool isRun = true;

        std::cout << "Register resources" << std::endl;
        Resource myResource;
        myResource.registerResource();

        while (isRun)
        {
            cout << "================= MENU =================" << endl;
            cout << "0 :: Quit" << endl;
            cout << "1 :: UPDATE RESOURCE" << endl;
            cout << "2 :: PUBLISH RESOURCE TO RD" << endl;
            cout << "3 :: DELETE RESOURCE FROM RD" << endl;
            cout << "========================================" << endl;
            std::cin >> selectedMenu;

            switch (selectedMenu)
            {
            case 0:
                isRun = false;
                break;
            case 1:
                std::cout << "Update resources" << std::endl;
                myResource.updateResource();
                break;
            case 2:
                std::cout << "Publish resources to RD" << std::endl;
                myResource.publishResource();
                break;
            case 3:
                std::cout << "Delete resources from RD" << std::endl;
                myResource.deleteResource();
                break;
            default:
                cout << "Invalid option" << endl;
            }

        }
        OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);
    }
    catch (OCException& e)
    {
        cerr << "Exception in main: "<< e.what();
    }

    return 0;
}
