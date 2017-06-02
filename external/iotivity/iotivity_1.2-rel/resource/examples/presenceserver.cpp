//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

///
/// This sample provides steps to define an interface for a resource
/// (properties and methods) and host this resource on the server.
///

#include "iotivity_config.h"
#include <functional>

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <array>
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

using namespace OC;
using namespace std;

#define numPresenceResources (2)

// Set of strings for each of platform Info fields
std::string  platformId = "0A3E0D6F-DBF5-404E-8719-D6880042463A";
std::string  manufacturerName = "OCF";
std::string  manufacturerLink = "https://www.iotivity.org";
std::string  modelNumber = "myModelNumber";
std::string  dateOfManufacture = "2016-01-15";
std::string  platformVersion = "myPlatformVersion";
std::string  operatingSystemVersion = "myOS";
std::string  hardwareVersion = "myHardwareVersion";
std::string  firmwareVersion = "1.0";
std::string  supportLink = "https://www.iotivity.org";
std::string  systemTime = "2016-01-15T11.01";

// Set of strings for each of device info fields
std::string  deviceName = "IoTivity Presence Server";
std::string  specVersion = "core.1.1.0";
std::string  dataModelVersions = "res.1.1.0";

// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;

// OCDeviceInfo Contains all the device info to be stored
OCDeviceInfo deviceInfo;

// Forward declaring the entityHandler
OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);

/// This class represents a single resource named 'lightResource'. This resource has
/// two simple properties named 'state' and 'power'

class LightResource
{
public:
    /// Access this property from a TB client
    bool m_state;
    int m_power;
    std::string m_lightUri;
    std::string m_lightUri2;
    std::string m_lightUri3;
    OCResourceHandle m_resourceHandle;
    OCResourceHandle m_resourceHandle2;
    OCResourceHandle m_resourceHandle3;

public:
    /// Constructor
    LightResource(): m_state(false), m_power(0), m_lightUri("/a/light"),
                     m_lightUri2("/a/light2"),m_lightUri3("/a/light3") {}

    /* Note that this does not need to be a member function: for classes you do not have
    access to, you can accomplish this with a free function: */

    /// This function internally calls registerResource API.
    void createResource()
    {
        std::string resourceURI = m_lightUri; // URI of the resource
        std::string resourceTypeName = "core.light"; // resource type name.
        std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

        // OCResourceProperty is defined ocstack.h
        uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;

        // This will internally create and register the resource.
        OCStackResult result = OCPlatform::registerResource(
                                    m_resourceHandle, resourceURI, resourceTypeName,
                                    resourceInterface, &entityHandler, resourceProperty);

        if (OC_STACK_OK != result)
        {
            cout << "Resource creation was unsuccessful\n";
        }
    }

    /// This function internally calls registerResource API.
    void createResource2()
    {
        std::string resourceURI = m_lightUri2; // URI of the resource
        std::string resourceTypeName = "core.light"; // resource type name. In this case, it is light
        std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

        // OCResourceProperty is defined ocstack.h
        uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;

        // This will internally create and register the resource.
        OCStackResult result = OCPlatform::registerResource(
                                    m_resourceHandle2, resourceURI, resourceTypeName,
                                    resourceInterface, &entityHandler, resourceProperty);

        if (OC_STACK_OK != result)
        {
            cout << "Resource creation was unsuccessful\n";
        }
    }

    void createResource3()
    {
        std::string resourceURI = m_lightUri3; // URI of the resource
        std::string resourceTypeName = "core.light";
        std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

        // OCResourceProperty is defined ocstack.h
        uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;

        // This will internally create and register the resource.
        OCStackResult result = OCPlatform::registerResource(
                                    m_resourceHandle3, resourceURI, resourceTypeName,
                                    resourceInterface, &entityHandler, resourceProperty);

        if (OC_STACK_OK != result)
        {
            cout << "Resource creation was unsuccessful\n";
        }
    }

    OCResourceHandle getHandle()
    {
        return m_resourceHandle;
    }

    void addType(const std::string& type) const
    {
        OCStackResult result = OC::OCPlatform::bindTypeToResource(m_resourceHandle, type);
        if (OC_STACK_OK != result)
        {
            cout << "Binding TypeName to Resource was unsuccessful\n";
        }
    }

    void addInterface(const std::string& iface) const
    {
        OCStackResult result = OC::OCPlatform::bindInterfaceToResource(m_resourceHandle, iface);
        if (OC_STACK_OK != result)
        {
            cout << "Binding TypeName to Resource was unsuccessful\n";
        }
    }

};

void createPresenceResources()
{
    std::array<std::string, numPresenceResources> resourceURI { {
        "/a/fan",
        "/a/led" } };
    std::array<std::string, numPresenceResources> resourceTypeName { {
        "core.fan",
        "core.led" } };

    std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.
    OCResourceHandle handle;
    // OCResourceProperty is defined ocstack.h
    uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;

    // This will internally create and register the resource.
    OCStackResult result = OC_STACK_OK;
    for(int i=0; i<numPresenceResources; i++)
    {
        result = OCPlatform::registerResource(handle,
                resourceURI.at(i), resourceTypeName.at(i), resourceInterface,
                &entityHandler, resourceProperty);
        if (result != OC_STACK_OK)
        {
            cout << "Resource creation was unsuccessful with resource URI "
                    << resourceURI.at(i);
        }
    }
}

// Create the instance of the resource class (in this case instance of class 'LightResource').
LightResource myLightResource;

OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> /*request*/)
{
    cout << "\tIn Server CPP entity handler:\n";
    return OC_EH_OK;
}

void DeletePlatformInfo()
{
    delete[] platformInfo.platformID;
    delete[] platformInfo.manufacturerName;
    delete[] platformInfo.manufacturerUrl;
    delete[] platformInfo.modelNumber;
    delete[] platformInfo.dateOfManufacture;
    delete[] platformInfo.platformVersion;
    delete[] platformInfo.operatingSystemVersion;
    delete[] platformInfo.hardwareVersion;
    delete[] platformInfo.firmwareVersion;
    delete[] platformInfo.supportUrl;
    delete[] platformInfo.systemTime;
}

void DeleteDeviceInfo()
{
    delete[] deviceInfo.deviceName;
    delete[] deviceInfo.specVersion;
    OCFreeOCStringLL(deviceInfo.dataModelVersions);
}

void DuplicateString(char ** targetString, std::string sourceString)
{
    *targetString = new char[sourceString.length() + 1];
    strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
}

OCStackResult SetPlatformInfo(std::string platformID, std::string manufacturerName,
        std::string manufacturerUrl, std::string modelNumber, std::string dateOfManufacture,
        std::string platformVersion, std::string operatingSystemVersion,
        std::string hardwareVersion, std::string firmwareVersion, std::string supportUrl,
        std::string systemTime)
{
    DuplicateString(&platformInfo.platformID, platformID);
    DuplicateString(&platformInfo.manufacturerName, manufacturerName);
    DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl);
    DuplicateString(&platformInfo.modelNumber, modelNumber);
    DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture);
    DuplicateString(&platformInfo.platformVersion, platformVersion);
    DuplicateString(&platformInfo.operatingSystemVersion, operatingSystemVersion);
    DuplicateString(&platformInfo.hardwareVersion, hardwareVersion);
    DuplicateString(&platformInfo.firmwareVersion, firmwareVersion);
    DuplicateString(&platformInfo.supportUrl, supportUrl);
    DuplicateString(&platformInfo.systemTime, systemTime);

    return OC_STACK_OK;
}

OCStackResult SetDeviceInfo(std::string deviceName, std::string specVersion, std::string dataModelVersions)
{
    DuplicateString(&deviceInfo.deviceName, deviceName);

    if (!specVersion.empty())
    {
        DuplicateString(&deviceInfo.specVersion, specVersion);
    }

    if (!dataModelVersions.empty())
    {
        OCResourcePayloadAddStringLL(&deviceInfo.dataModelVersions, dataModelVersions.c_str());
    }

    return OC_STACK_OK;
}

int main()
{
    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        OC::QualityOfService::LowQos
    };

    OCPlatform::Configure(cfg);
    std::cout << "Starting server & setting platform info\n";

    OCStackResult result = SetPlatformInfo(platformId, manufacturerName, manufacturerLink,
            modelNumber, dateOfManufacture, platformVersion, operatingSystemVersion,
            hardwareVersion, firmwareVersion, supportLink, systemTime);

    result = OCPlatform::registerPlatformInfo(platformInfo);

    if (result != OC_STACK_OK)
    {
        std::cout << "Platform Registration failed\n";
        return -1;
    }

    result = SetDeviceInfo(deviceName, specVersion, dataModelVersions);
    OCResourcePayloadAddStringLL(&deviceInfo.types, "oic.wk.d");

    result = OCPlatform::registerDeviceInfo(deviceInfo);

    if (result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
        return -1;
    }
    try
    {
        using namespace OC::OCPlatform;
        // Time to Live is 30 seconds
        startPresence(30);

        // Invoke createResource function of class light.
        myLightResource.createResource();
        std :: cout << "Creating first resource of type \"core.light\"" << std :: endl;

        std :: cout << "Will start creating/deleting resources for presence in 10 seconds.\n";

        sleep(10);

        std :: cout << "\nCreating the second resource of type \"core.light\"" <<  std :: endl;
        sleep(1);

        myLightResource.createResource2();

        std :: cout << "Stopping presence\n" << std :: endl;
        sleep(1);
        stopPresence();

        std :: cout << "Restarting presence\n" << std :: endl;
        sleep(1);

        startPresence(30);

        std :: cout << "Creating a third resource of type \"core.light\"\n" << std :: endl;
        sleep(1);

        myLightResource.createResource3();

        std :: cout << "Creating two non-operational resources.\"\n" << std :: endl;
        sleep(1);

        createPresenceResources();

        DeletePlatformInfo();
        DeleteDeviceInfo();

        // A condition variable will free the mutex it is given, then do a non-
        // intensive block until 'notify' is called on it.  In this case, since we
        // don't ever call cv.notify, this should be a non-processor intensive version
        // of while(true);
        std::mutex blocker;
        std::condition_variable cv;
        std::unique_lock<std::mutex> lock(blocker);
        cv.wait(lock);
    }
    catch(OCException& e)
    {
        oclog() << "Exception in main: "<< e.what();
    }

    // No explicit call to stop the platform.
    // When OCPlatform destructor is invoked, internally we do platform cleanup

    return 0;
}

