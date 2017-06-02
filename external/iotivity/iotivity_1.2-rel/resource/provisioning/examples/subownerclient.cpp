/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <map>
#include <cstdlib>
#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "OCPlatform.h"
#include "OCApi.h"
#include "OCProvisioningManager.hpp"
#include "oxmjustworks.h"
#include "oxmrandompin.h"
#include "aclresource.h"
#include "utlist.h"

#define MAX_PERMISSION_LENGTH (5)
#define ACL_RESRC_ARRAY_SIZE (3)
#define CREATE (1)
#define READ (2)
#define UPDATE (4)
#define DELETE (8)
#define NOTIFY (16)
#define DASH '-'
#define PREDEFINED_TIMEOUT (10)
#define MAX_OWNED_DEVICE (10)
#define TAG  "subownerclient"

#define JSON_DB_PATH "./oic_svr_db_subowner_client.json"
#define DAT_DB_PATH "./oic_svr_db_subowner_client.dat"
#define DEV_STATUS_ON "DEV_STATUS_ON"
#define DEV_STATUS_OFF "DEV_STATUS_OFF"

#define DISCOVERY_TIMEOUT 5

using namespace OC;

DeviceList_t pMOwnedDeviceList, pMOTEnabledDeviceList;
static int transferDevIdx, ask = 1;

static FILE* client_open(const char *UNUSED_PARAM, const char *mode)
{
    (void)UNUSED_PARAM;
    return fopen(DAT_DB_PATH, mode);
}

void printMenu()
{
    std::cout << "\nChoose an option:"<<std::endl;
    std::cout << "   1. Discover Multiple Ownership Transfer Enabled Devices"<<std::endl;
    std::cout << "   2. Discover Multiple Owned Devices"<<std::endl;
    std::cout << "   3. Perform the Multiple Ownership Transfer"<<std::endl;
    std::cout << "   4. Get LED resource"<<std::endl;
    std::cout << "   5. Get PUT resource"<<std::endl;
    std::cout << "  99. Exit loop"<<std::endl;
}

void moveTransferredDevice()
{
    pMOwnedDeviceList.push_back(pMOTEnabledDeviceList[transferDevIdx]);
    pMOTEnabledDeviceList.erase(pMOTEnabledDeviceList.begin() + transferDevIdx);
}

void InputPinCB(char* pinBuf, size_t bufSize)
{
    if(pinBuf)
    {
        std::cout <<"INPUT PIN : ";
        std::string ptr;
        std::cin >> ptr;
        OICStrcpy(pinBuf, bufSize, ptr.c_str());
        return;
    }
}

void printUuid(OicUuid_t uuid)
{
    for (int i = 0; i < UUID_LENGTH; i++)
    {
        std::cout <<std::hex << uuid.id[i] << " ";
    }
    std::cout<<std::endl;
}

void multipleOwnershipTransferCB(PMResultList_t *result, int hasError)
{
    if (hasError)
    {
        std::cout << "Error!!! in MultipleOwnershipTransfer"<<std::endl;
    }
    else
    {
        std::cout<< "\nTransferred MultipleOwnership successfuly for device : ";
        printUuid(result->at(0).deviceId);
        delete result;

        moveTransferredDevice();
    }
    ask = 1;
}

void printStatus(int status)
{
    static std::map<int, std::string> devStatus = {{1<<0, DEV_STATUS_ON}, {1<<1, DEV_STATUS_OFF}};

    std::cout <<devStatus[status] <<std::endl;
}

void printDevices(DeviceList_t &list)
{
   for (unsigned int i = 0; i < list.size(); i++ )
   {
      std::cout << "Device "<< i+1 <<" ID: ";
      std::cout << list[i]->getDeviceID() << " From IP: ";
      std::cout << list[i]->getDevAddr() << std::endl;
   }
}

static void getCallback(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
    (void)(headerOptions);
    try
    {
        if (OC_STACK_OK == eCode)
        {
            std::cout << "Callback Context for GET query recvd successfully" << std::endl;
            std::cout << "Resource URI: " << rep.getUri() << std::endl;

            bool state = false;
            int power = 0;
            rep.getValue("state", state);
            rep.getValue("power", power);

            std::cout << "\tstate: " << state << std::endl;
            std::cout << "\tpower: " << power << std::endl;
        }
        else
        {
            std::cout << "getCallback Response error: " << eCode << std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
    }
}

void putCallback(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
    if(eCode == 0)
    {
        std::cout << "PUT request was successful !!!!!!" << std::endl;
    }
    else
    {
        std::cout << "onPut Response error !!!!!: " << eCode << std::endl;
    }
}

int main(void)
{
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
            OC::ModeType::Both,
            "0.0.0.0",
            0,
            OC::QualityOfService::LowQos,
            &ps
    };

    OCPlatform::Configure(cfg);

    //set Input Pin callback
    OCSecure::setInputPinCallback(InputPinCB);

    try
    {
        int choice;
        OicSecAcl_t *acl1 = nullptr, *acl2 = nullptr;
        if (OCSecure::provisionInit("subowner_pdm.db") != OC_STACK_OK)
        {
            std::cout <<"PM Init failed"<< std::endl;
            return 1;
        }

        for (int out = 0; !out;)
        {
            while (!ask)
            {
                sleep(1);
            }

            printMenu();
            std::cin >> choice;
            switch(choice) {
                case 1:
                    {
                        pMOTEnabledDeviceList.clear();
                        std::cout << "Started MOT enabled device discovery..." <<std::endl;
                        OCStackResult result = OCSecure::discoverMultipleOwnerEnabledDevices(
                                DISCOVERY_TIMEOUT, pMOTEnabledDeviceList);
                        if (result != OC_STACK_OK)
                        {
                            std::cout<< "!!Error - MOT enabled dev Discovery failed."<<std::endl;
                        }
                        else if (pMOTEnabledDeviceList.size())
                        {
                            std::cout <<"Found MOT enabled devices, count = " <<
                                pMOTEnabledDeviceList.size() << std::endl;
                            printDevices(pMOTEnabledDeviceList);
                        }
                        else
                        {
                            std::cout <<"NO MOT enabled devices found"<<std::endl;
                        }
                        break;
                    }
                case 2:
                    {
                        pMOwnedDeviceList.clear();
                        std::cout << "Started Multiple Owned device discovery..." <<std::endl;
                        OCStackResult result = OCSecure::discoverMultipleOwnerEnabledDevices(
                                DISCOVERY_TIMEOUT, pMOwnedDeviceList);
                        if (result != OC_STACK_OK)
                        {
                            std::cout<< "!!Error - Multiple Owned dev Discovery failed."<<std::endl;
                        }
                        else if (pMOwnedDeviceList.size())
                        {
                            std::cout <<"Found Multiple Owned devices, count = " <<
                                pMOwnedDeviceList.size() << std::endl;
                            printDevices(pMOwnedDeviceList);
                        }
                        else
                        {
                            std::cout <<"NO Multiple Owned devices found"<<std::endl;
                        }
                        break;
                    }
                case 3:
                    {  //Multiple Ownrship Trasfer
                        unsigned int devNum;

                        if (!pMOTEnabledDeviceList.size())
                        {
                            std::cout <<"There are no MOT enabled devices"<<std::endl;
                            break;
                        }

                        for (unsigned int i = 0; i < pMOTEnabledDeviceList.size(); i++ )
                        {
                            std::cout << i+1 << ": "<< pMOTEnabledDeviceList[i]->getDeviceID();
                            std::cout << " From IP:" << pMOTEnabledDeviceList[i]->getDevAddr() <<std::endl;
                        }

                        std::cout <<"Select device number: "<<std::endl;
                        std::cin >> devNum;
                        if (devNum > pMOTEnabledDeviceList.size())
                        {
                            std::cout <<"Invalid device number"<<std::endl;
                            break;
                        }
                        transferDevIdx = devNum - 1;

                        ask = 0;
                        std::cout << "MOT for : "<<
                            pMOTEnabledDeviceList[devNum-1]->getDeviceID()<<std::endl;
                        if (pMOTEnabledDeviceList[devNum-1]->doMultipleOwnershipTransfer
                                (multipleOwnershipTransferCB)
                                != OC_STACK_OK)
                        {
                            std::cout<<"Multiple OwnershipTransferCallback is failed"<<std::endl;
                            ask = 1;
                        }
                        break;
                    }
                case 4:
                    {
                        std::cout << "- Send data(GET Request) to device(led server) -" << std::endl;
                        printDevices(pMOwnedDeviceList);
                        printMenu();
                        std::cout << "Enter device number to GET data: ";
                        std::cin >> choice;
                        choice--;

                        std::vector<std::string> ledTypes = {"core.led"};
                        std::vector<std::string> ifaces = {DEFAULT_INTERFACE};

                        OCProvisionDev_t *selDev = pMOwnedDeviceList[choice]->getDevPtr();
                        std::stringstream host;
                        host << "coaps:://";
                        host << pMOwnedDeviceList[choice]->getDevAddr();
                        host << selDev->securePort;

                        OCResource::Ptr led = OC::OCPlatform::constructResourceObject(
                                host.str(), "/a/led", selDev->connType, false, ledTypes, ifaces);

                        if(!led)
                        {
                            std::cout << "Error: Led Object construction returned null" << std::endl;
                            break;
                        }
                        OCStackResult res = led->get(QueryParamsMap(), getCallback);

                        if (OC_STACK_OK != res)
                        {
                            std::cout << "Error: get Failed for Led" << std::endl;
                        }
                        break;
                    }
                    break;
                case 5:
                    {
                        std::cout << "- Send data(Put Request) to device(led server) -" << std::endl;
                        printDevices(pMOwnedDeviceList);
                        printMenu();
                        std::cout << "Enter device number to GET data: ";
                        std::cin >> choice;
                        choice--;

                        std::vector<std::string> ledTypes = {"core.led"};
                        std::vector<std::string> ifaces = {DEFAULT_INTERFACE};


                        OCProvisionDev_t *selDev = pMOwnedDeviceList[choice]->getDevPtr();
                        std::stringstream host;
                        host << "coaps:://";
                        host << pMOwnedDeviceList[choice]->getDevAddr();
                        host << selDev->securePort;

                        OCResource::Ptr led = OC::OCPlatform::constructResourceObject(host.str(),
                                "/a/led", selDev->connType, false, ledTypes, ifaces);

                        if(!led)
                        {
                            std::cout << "Error: Led Object construction returned null" << std::endl;
                            break;
                        }
                        OCRepresentation rep;
                        bool state = true;
                        int power = 39;
                        rep.setValue("state", state);
                        rep.setValue("power", power);
                        OCStackResult res = led->put(rep, QueryParamsMap(), putCallback);

                        if (OC_STACK_OK != res)
                        {
                            std::cout << "Error: put Failed for Led" << std::endl;
                        }
                        break;
                    }
                    break;
                case 99:
                default:
                    out = 1;
                    break;
            }
        }
    }
    catch(OCException& e)
    {
        oclog() << "Exception in main: "<<e.what();
    }

    return 0;
}
