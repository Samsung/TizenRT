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

#include <iostream>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "OCProvisioningManager.hpp"
#include "securevirtualresourcetypes.h"

#include "EasySetup.hpp"
#include "ESRichCommon.h"

#define ES_SAMPLE_APP_TAG "ES_SAMPLE_APP_TAG"
#define DECLARE_MENU(FUNC, ...) { #FUNC, FUNC }

#define JSON_DB_PATH "./oic_svr_db_client.dat"

using namespace OC;
using namespace OIC::Service;

static std::shared_ptr<RemoteEnrollee> remoteEnrollee = nullptr;
static std::shared_ptr<OC::OCResource> curResource = nullptr;

static std::mutex g_discoverymtx;
static std::condition_variable g_cond;

typedef void (*Runner)();

Runner g_currentRun;

int processUserInput(int min = std::numeric_limits<int>::min(),
        int max = std::numeric_limits<int>::max())
{
    assert(min <= max);

    int input = 0;

    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!std::cin.fail() && min <= input && input <= max)
    {
        return input;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::runtime_error("Invalid Input, please try again");
}

void printConfiguration(const EnrolleeConf& conf)
{
    cout << "===========================================" << endl;
    cout << "\tDevice Name : " << conf.getDeviceName() << endl;
    cout << "\tModel Number : " << conf.getModelNumber() << endl;

    for(auto it : conf.getWiFiModes())
    {
        cout << "\tSupported WiFi modes : " << it << endl;
    }

    cout << "\tSupported WiFi freq : " << static_cast<int>(conf.getWiFiFreq()) << endl;
    cout << "\tCloud accessibility: " << conf.isCloudAccessible() << endl;
    cout << "===========================================" << endl;
}

void printStatus(const EnrolleeStatus& status)
{
    cout << "===========================================" << endl;
    cout << "\tProvStatus : " << status.getProvStatus() << endl;
    cout << "\tLastErrCode : " << status.getLastErrCode() << endl;
    cout << "===========================================" << endl;
}

ESOwnershipTransferData provisionSecurityStatusCallback(std::shared_ptr<SecProvisioningStatus> secProvisioningStatus)
{
    cout << "provisionSecurityStatusCallback IN" << endl;
    cout << "ESResult : " << secProvisioningStatus->getESResult() << std::endl;
    cout << "Device ID : " << secProvisioningStatus->getDeviceUUID() << std::endl;

    if(secProvisioningStatus->getESResult() == ES_SECURE_RESOURCE_IS_DISCOVERED)
    {
#ifdef __WITH_DTLS__
        cout << "Owned Status : " << secProvisioningStatus->isOwnedDevice() << std::endl;
        if(secProvisioningStatus->isOwnedDevice())
        {
            cout << "Owner ID : " << secProvisioningStatus->getOwnerID() << std::endl;
        }
        cout << "OT Method : " << secProvisioningStatus->getSelectedOTMethod() << std::endl;
#ifdef MULTIPLE_OWNER
        cout << "MOT Enabled : " << secProvisioningStatus->isMOTEnabled() << std::endl;

        // TEST
        ESOwnershipTransferData OTData;
        OTData.setMOTMethod(OIC_PRECONFIG_PIN, "12345678");

        cout << "Enter!" << std::endl;
        getchar();

        return OTData;
#endif
#endif
    }
    else if(secProvisioningStatus->getESResult() == ES_OK)
    {
        cout << "provisionSecurity is success." << std::endl;
    }
    else
    {
        cout << "provisionSecurity is failed." << endl;
    }

    return {};
}

void provisionSecurity()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    try
    {
        remoteEnrollee->provisionSecurity((SecurityProvStatusCbWithOption)provisionSecurityStatusCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during provisionSecurity call" << e.reason();
        return;
    }
}

void getStatusCallback(std::shared_ptr< GetEnrolleeStatus > getEnrolleeStatus)
{
    if(getEnrolleeStatus->getESResult() != ES_OK)
    {
      cout << "getStatus is failed." << endl;
      return;
    }
    else
    {
      cout << "getStatus is success." << endl;
      printStatus(getEnrolleeStatus->getEnrolleeStatus());
    }
}


void getStatus()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    try
    {
        remoteEnrollee->getStatus(getStatusCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during getConfiguration call" << e.reason();
        return;
    }
}

void getConfigurationCallback(std::shared_ptr< GetConfigurationStatus > getConfigurationStatus)
{
    if(getConfigurationStatus->getESResult() != ES_OK)
    {
      cout << "GetConfigurationStatus is failed." << endl;
      return;
    }
    else
    {
      cout << "GetConfigurationStatus is success." << endl;
      printConfiguration(getConfigurationStatus->getEnrolleeConf());
    }
}

void getConfiguration()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    try
    {
        remoteEnrollee->getConfiguration(getConfigurationCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during getConfiguration call" << e.reason();
        return;
    }
}

void deviceProvisioningStatusCallback(std::shared_ptr< DevicePropProvisioningStatus > provStatus)
{
    if(provStatus->getESResult() != ES_OK)
    {
      cout << "Device Provisioning is failed." << endl;
      return;
    }
    else
    {
      cout << "Device Provisioning is success." << endl;
    }
}

void provisionDeviceProperty()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    DeviceProp devProp;
    devProp.setWiFiProp("Iotivity_SSID", "Iotivity_PWD", WPA2_PSK, TKIP_AES);
    devProp.setDevConfProp("korean", "Korea", "Location");

    try
    {
        remoteEnrollee->provisionDeviceProperties(devProp, deviceProvisioningStatusCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during provisionDeviceProperties call" << e.reason();
        return;
    }
}

void connectRequestStatusCallback(std::shared_ptr< ConnectRequestStatus > requestStatus)
{
    if(requestStatus->getESResult() != ES_OK)
    {
      cout << "Request to connection is failed." << endl;
      return;
    }
    else
    {
      cout << "Request to connection is success." << endl;
    }
}

void requestToConnect()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    try
    {
        std::vector<ES_CONNECT_TYPE> types;
        types.push_back(ES_CONNECT_WIFI);
        types.push_back(ES_CONNECT_COAPCLOUD);
        remoteEnrollee->requestToConnect(types, connectRequestStatusCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during provisionDeviceProperties call" << e.reason();
        return;
    }
}

void cloudProvisioningStatusCallback(std::shared_ptr< CloudPropProvisioningStatus > provStatus)
{
    switch (provStatus->getESResult())
    {
        case ES_OK:
            cout << "Cloud Provisioning is success." << endl;
            break;
        case ES_SECURE_RESOURCE_DISCOVERY_FAILURE:
            cout << "Enrollee is not found in a given network." << endl;
            break;
        case ES_ACL_PROVISIONING_FAILURE:
            cout << "ACL provisioning is failed." << endl;
            break;
        case ES_CERT_PROVISIONING_FAILURE:
            cout << "CERT provisioning is failed." << endl;
            break;
        default:
            cout << "Cloud Provisioning is failed." << endl;
            break;
    }
}

void provisionCloudProperty()
{
    if(!remoteEnrollee)
    {
        std::cout << "RemoteEnrollee is null, retry Discovery EnrolleeResource." << endl;
        return;
    }

    CloudProp cloudProp;
    cloudProp.setCloudPropWithAccessToken("accessToken", OAUTH_TOKENTYPE_BEARER, "authProvider", "ciServer");
    cloudProp.setCloudID("f002ae8b-c42c-40d3-8b8d-1927c17bd1b3");
    cloudProp.setCredID(1);

    try
    {
        remoteEnrollee->provisionCloudProperties(cloudProp, cloudProvisioningStatusCallback);
    }
    catch (OCException &e)
    {
        std::cout << "Exception during provisionCloudProperties call" << e.reason();
        return;
    }
}

// Callback to found resources
void foundResource(std::shared_ptr<OC::OCResource> resource)
{
    std::string resourceURI;
    std::string hostAddress;
    try
    {
        // Do some operations with resource object.
        if(resource &&
           !curResource &&
           resource->getResourceTypes().at(0) == OC_RSRVD_ES_RES_TYPE_EASYSETUP)
        {
            std::cout<<"DISCOVERED Resource:"<<std::endl;
            // Get the resource URI
            resourceURI = resource->uri();
            std::cout << "\tURI of the resource: " << resourceURI << std::endl;

            // Get the resource host address
            hostAddress = resource->host();
            std::cout << "\tHost address of the resource: " << hostAddress << std::endl;

            // Get the resource types
            std::cout << "\tList of resource types: " << std::endl;
            for(auto &resourceTypes : resource->getResourceTypes())
            {
                std::cout << "\t\t" << resourceTypes << std::endl;
            }

            // Get the resource interfaces
            std::cout << "\tList of resource interfaces: " << std::endl;
            for(auto &resourceInterfaces : resource->getResourceInterfaces())
            {
                std::cout << "\t\t" << resourceInterfaces << std::endl;
            }

            if(curResource == nullptr)
            {
                remoteEnrollee = EasySetup::getInstance()->createRemoteEnrollee(resource);
                if(!remoteEnrollee)
                {
                    std::cout << "RemoteEnrollee object is failed for some reasons!" << std::endl;
                }
                else
                {
                    curResource = resource;
                    std::cout << "RemoteEnrollee object is successfully created!" << std::endl;
                    g_cond.notify_all();
                }
            }
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception in foundResource: "<< e.what() << std::endl;
    }
}

void discoveryEnrolleeResource()
{
    try
    {
        std::ostringstream requestURI;
        requestURI << OC_RSRVD_WELL_KNOWN_URI << "?rt=" << OC_RSRVD_ES_RES_TYPE_EASYSETUP;
        OCPlatform::findResource("", requestURI.str(), CT_DEFAULT, &foundResource);
        std::cout<< "Finding Resource... " <<std::endl;

        std::unique_lock<std::mutex> lck(g_discoverymtx);
        g_cond.wait_for(lck, std::chrono::seconds(5));
    }
    catch (OCException &e)
    {
        std::cout << "Exception in discoveryEnrolleeResource: "<<e.what();
    }
}

void DisplayMenu()
{
    constexpr int DISCOVERY_ENROLLEE = 1;
    constexpr int PROVISION_SECURITY = 2;
    constexpr int GET_STATUS = 3;
    constexpr int GET_CONFIGURATION = 4;
    constexpr int PROVISION_DEVICE_PROPERTY = 5;
    constexpr int REQUEST_TO_CONNECT = 6;
    constexpr int PROVISION_CLOUD_PROPERTY = 7;

    std::cout << "========================================================\n";
    std::cout << DISCOVERY_ENROLLEE << ". Discovery Enrollee Resource \n";
    std::cout << PROVISION_SECURITY << ". Provision Security to Enrollee  \n";
    std::cout << GET_STATUS << ". Get Status from Enrollee  \n";
    std::cout << GET_CONFIGURATION << ". Get Configuration from Enrollee  \n";
    std::cout << PROVISION_DEVICE_PROPERTY << ". Provision Device Property\n";
    std::cout << REQUEST_TO_CONNECT << ". Request to Connect  \n";
    std::cout << PROVISION_CLOUD_PROPERTY << ". Provision Cloud Property  \n";
    std::cout << "========================================================\n";

    int selection = processUserInput(DISCOVERY_ENROLLEE, PROVISION_CLOUD_PROPERTY);

    switch (selection)
    {
        case DISCOVERY_ENROLLEE:
            discoveryEnrolleeResource();
            break;
        case PROVISION_SECURITY:
            provisionSecurity();
            break;
        case GET_STATUS:
            getStatus();
            break;
        case GET_CONFIGURATION:
            getConfiguration();
            break;
        case PROVISION_DEVICE_PROPERTY:
            provisionDeviceProperty();
            break;
        case REQUEST_TO_CONNECT:
            requestToConnect();
            break;
        case PROVISION_CLOUD_PROPERTY:
            provisionCloudProperty();
            break;
        default:
            break;
    };
}

static FILE* client_open(const char *UNUSED_PARAM, const char *mode)
{
    (void)UNUSED_PARAM;
    return fopen(JSON_DB_PATH, mode);
}

int main()
{
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    PlatformConfig config
    {
        OC::ServiceType::InProc, ModeType::Both, "0.0.0.0", 0, OC::QualityOfService::HighQos, &ps
    };

    OCPlatform::Configure(config);

    try
    {
#ifdef __WITH_DTLS__
        //Initializing the provisioning client stack using the db path provided by the application.
        OCStackResult result = OCSecure::provisionInit("");

        if (result != OC_STACK_OK)
        {
            return -1;
        }
#endif
    }catch (...)
    {
        std::cout << "Exception in main: " << std::endl;
    }

    while (true)
    {
        try
        {
            DisplayMenu();
        }
        catch (...)
        {
            std::cout << "Exception caught in main " << std::endl;
        }
    }

    std::cout << "Stopping the client" << std::endl;

    return 0;
}

