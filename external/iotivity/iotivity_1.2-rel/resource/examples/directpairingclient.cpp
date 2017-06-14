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
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string>
#include <map>
#include <cstdlib>
#include <mutex>
#include <condition_variable>

#include "logger.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "OCPlatform.h"
#include "OCApi.h"

#define MAX_PERMISSION_LENGTH (5)
#define CREATE (1)
#define READ (2)
#define UPDATE (4)
#define DELETE (8)
#define NOTIFY (16)
#define DASH '-'
#define PREDEFINED_TIMEOUT (10)
#define MAX_OWNED_DEVICE (10)
#define TAG  "provisioningclient"

#define JSON_DB_PATH "./oic_svr_db_client.json"
#define DAT_DB_PATH "./oic_svr_db_client.dat"
#define DEV_STATUS_ON "DEV_STATUS_ON"
#define DEV_STATUS_OFF "DEV_STATUS_OFF"

#define DP_DISCOVERY_TIMEOUT 4
#define DP_PIN_LENGTH 8

#define BOLD_BEGIN    "\033[1m"
#define RED_BEGIN     "\033[1;31m"
#define GREEN_BEGIN   "\033[1;92m"
#define COLOR_END     "\033[0m"

using namespace OC;

static int ask = 1;
static PairedDevices discoveredDeviceList, pairedDeviceList;

static FILE* client_open(const char* /*fileName*/, const char *mode)
{
    return fopen(DAT_DB_PATH, mode);
}


static void printMenu()
{
    std::cout << GREEN_BEGIN "Choose an option:" COLOR_END<<std::endl;
    std::cout << GREEN_BEGIN "# 1 (DP device discovery) : discover Direct-Pairing devices"
        COLOR_END<<std::endl;
    std::cout << GREEN_BEGIN "# 2 (start Direct-Pairing) : negotiate DP method & start Direct-Pairin"
        COLOR_END<<std::endl;
    std::cout << GREEN_BEGIN "# 3 (list all device) : list all discovered/paired devices"
        COLOR_END<<std::endl;
    std::cout << GREEN_BEGIN "# 4 (send data) : send data to device" COLOR_END<<std::endl;
    std::cout << GREEN_BEGIN "# 9 (quit) : quit test " COLOR_END<<std::endl;
}

static void printPrompt()
{
    std::cout << BOLD_BEGIN "IoTivity-DP#" COLOR_END" ";
}

static void printDevices(PairedDevices& list)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        std::cout << "["<< i+1 << "]" << " ID: " << list[i]->getDeviceID() << std::endl;
    }
}

static void findCallback(PairedDevices discoveredDevList)
{

    if (0 == discoveredDevList.size())
    {
        std::cout<< "No Direct-pairing Support device Found" << std::endl;
    }
    else
    {
        std::cout << "Discovered Direct-Pairing Support Device"<< std::endl;
        discoveredDeviceList = discoveredDevList;
        printDevices(discoveredDevList);
    }

    printMenu();
    printPrompt();
    fflush(NULL);
}

static bool printPairingMethod(int choice)
{
    if (NULL == discoveredDeviceList[choice])// || false == discoveredDeviceList[choice]->edp)
    {
        std::cout<< "Invalid device or Not support direct-pairing..\n\n" << std::endl;
        return false;
    }

    auto prms = discoveredDeviceList[choice]->getPairingMethods();
    if (0 == prms.size())
    {
        std::cout << "Not exist any support method..\n\n" << std::endl;
        return false;
    }

    bool ret = true;
    std::cout << "\n* List of supported pairing methods *" << std::endl;

    for (unsigned int i = 0; i < prms.size(); i++)
    {
        std::cout<< "[" << i+1 << "]";
        switch (prms[i])
        {
            case DP_PRE_CONFIGURED:
                std::cout<<"Pre-Configured PIN"<<std::endl;;
                break;
            case DP_RANDOM_PIN:
                std::cout<<"Random PIN"<<std::endl;;
                break;
            default:
                std::cout<<"NOT Allowed ("<< prms[i]<<")"<<std::endl;
                ret = false;
                break;
        }
        std::cout<<std::endl;
    }

    return ret;
}

static void resultCallback(std::shared_ptr<OCDirectPairing> ptr, OCStackResult result)
{

    if (OC_STACK_OK == result)
    {
        std::cout << " Direct-Pairing SUCCESS" << std::endl;
        std::cout << "Taget Add info:" << ptr->getHost() << std::endl;
    }
    else {
        std::cout <<" Direct-Pairing FAILED" << std::endl;
    }

    printMenu();
    printPrompt();
    fflush(NULL);
}

static void pairedDevListCB(PairedDevices pairedDevList)
{

    if (0 == pairedDevList.size())
    {
        std::cout << "No Paired Devcie  Found" << std::endl;
    }
    else
    {
        pairedDeviceList = pairedDevList;
        printDevices(pairedDevList);
    }

    printMenu();
    printPrompt();
    fflush(NULL);
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

static bool InputPIN(std::string& pin)
{
    std::cout <<"   > Enter PIN Number for authentication (ex - '00000000' [8 digit] ):" ;

    std::cin >> pin;

    if (pin.size() != DP_PIN_LENGTH)
    {
        std::cout<<"Invalid PIN"<<std::endl;
        return false;
    }

    return true;
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

    try
    {
        unsigned int choice;
        for (int out = 0; !out;)
        {
            if (ask)
            {
                printMenu();
                printPrompt();
                fflush(NULL);
            }
            std::cin >> choice;

            switch(choice) {
                case 1:
                    {
                        OCStackResult result = OC::OCPlatform::findDirectPairingDevices(
                                DP_DISCOVERY_TIMEOUT, findCallback);

                        if (OC_STACK_NO_RESOURCE == result)
                        {
                            std::cout << "!! No Direct-Pairing Support Device found"<<std::endl;
                            break;
                        }

                        if (OC_STACK_OK != result)
                        {
                            std::cout << "!!Error - findDirectPairingDevices failed."<<std::endl;
                        }
                        ask = 0;
                        break;
                    }
                case 2:
                    {
                        unsigned int pMethodIDx = -1;
                        std::string pin("");

                        std::cout << "- Negotiate DP method & Start Direct-Pairing - ";
                        std::cout << "* List of  discovered device" << std::endl;
                        printDevices(discoveredDeviceList);
                        std::cout << "   > Enter Peer Device Number to initiate Direct-Pairing:" << std::endl;
                        printPrompt();
                        std::cin >> choice;
                        if (choice < 1 || choice > discoveredDeviceList.size())
                        {
                            std::cout << "!!Device Number is incorrect, Try Again" << std::endl;
                            break;
                        }
                        OCPrm_t pmSel = DP_NOT_ALLOWED;
                        choice--;
                        if (false == printPairingMethod(choice))
                        {
                            std::cout << "Target does not support the Direct-Pairing" << std::endl;
                            break;
                        }

                        std::cout << " > Enter pairing method: "<< std::endl;
                        printPrompt();
                        std::cin >> pMethodIDx;
                        auto prms = discoveredDeviceList[choice]->getPairingMethods();
                        if (0 >= pMethodIDx || prms.size() < pMethodIDx)
                        {
                            std::cout <<"Invalid mode selection" << std::endl;
                            break;
                        }

                        pmSel =  prms[pMethodIDx - 1];
                        if (false == InputPIN(pin))
                        {
                            break;
                        }

                        OCStackResult result = OC::OCPlatform::doDirectPairing(discoveredDeviceList[choice], pmSel, pin, resultCallback);

                        if (OC_STACK_OK != result)
                        {
                            std::cout << "!!Error - doDirectPairing failed." << std::endl;
                        }
                        ask = 0;
                        break;
                    }
                case 3:
                    {
                        std::cout << "- List all discovered and paired devices) -";
                        std::cout << "  > List of discovered devices" << std::endl;
                        printDevices(discoveredDeviceList);
                        std::cout << std::endl;

                        std::cout << "  > List of paired devices" << std::endl;
                        OCStackResult result = OC::OCPlatform::getDirectPairedDevices(pairedDevListCB);
                        printDevices(pairedDeviceList);
                        std::cout << std::endl;

                        if (OC_STACK_NO_RESOURCE == result)
                        {
                            std::cout << "!! No Paired device found"<<std::endl;
                            break;
                        }
                        if (OC_STACK_OK != result)
                        {
                            std::cout << "!!Error - getDirectPairedDevices failed."<<std::endl;
                        }
                        break;
                    }
                case 4:
                    {
                        std::cout << "- Send data(GET Request) to device(led server) -" << std::endl;
                        printDevices(pairedDeviceList);
                        pairedDeviceList = discoveredDeviceList;
                        printMenu();
                        std::cout << "Enter device number to GET data: ";
                        std::cin >> choice;
                        choice--;

                        std::vector<std::string> ledTypes = {"core.led"};
                        std::vector<std::string> ifaces = {DEFAULT_INTERFACE};

                        OCConnectivityType ct = pairedDeviceList[choice]->getConnType();

                        std::cout << "\n\n HOST address is : " << pairedDeviceList[choice]->getHost() << "\n\n";
                        OCResource::Ptr led = OC::OCPlatform::constructResourceObject(
                                pairedDeviceList[choice]->getHost(),
                                "/a/led", ct, false, ledTypes, ifaces);

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
                case 9:
                    {
                        out = 1;
                        break;
                    }
                default:
                    {
                        std::cout << GREEN_BEGIN "Wrong Option : Try Again" COLOR_END << std::endl;
                        printMenu();
                        printPrompt();
                        break;
                    }
            }
        }
    }
    catch(OCException& e)
    {
        oclog() << "Exception in main: "<< e.what();
    }

    return 0;
}
