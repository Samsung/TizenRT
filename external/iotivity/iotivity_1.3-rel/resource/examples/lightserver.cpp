//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
// Copyright 2014 Samsung Electronics All Rights Reserved.
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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <functional>

#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

int gObservation = 0;
void * ChangeLightRepresentation (void *param);
void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest);

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
std::string  deviceName = "IoTivity Light Server";
std::string  specVersion = "ocf.1.1.0";
std::vector<std::string> dataModelVersions = {"ocf.res.1.1.0"};
std::string  protocolIndependentID = "b0ed9259-ec95-4ac6-8f62-241d0da02683";

// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;

// Specifies secure or non-secure
// false: non-secure resource
// true: secure resource
bool isSecure = false;

/// Specifies whether Entity handler is going to do slow response or not
bool isSlowResponse = false;

// Forward declaring the entityHandler

/// This class represents a single resource named 'lightResource'. This resource has
/// one simple attribute, power

class LightResource
{

public:
    /// Access this property from a TB client
    std::string m_power;
    std::string m_lightUri;
    OCResourceHandle m_resourceHandle;
    OCRepresentation m_lightRep;

public:
    /// Constructor
    LightResource()
        :m_power(""), m_lightUri("/a/light") {
        // Initialize representation
        m_lightRep.setUri(m_lightUri);

        m_lightRep.setValue("power", m_power);
    }

    /* Note that this does not need to be a member function: for classes you do not have
    access to, you can accomplish this with a free function: */

    /// This function internally calls registerResource API.
    void createResource()
    {
        std::string resourceURI = m_lightUri; //URI of the resource
        std::string resourceTypeName = "core.light"; //resource type name. In this case, it is light
        std::string resourceInterface = DEFAULT_INTERFACE; // resource interface.

        EntityHandler cb = std::bind(&LightResource::entityHandler, this,PH::_1);

        // This will internally create and register the resource.
        OCStackResult result = OCPlatform::registerResource(
                                    m_resourceHandle, resourceURI, resourceTypeName,
                                    resourceInterface, cb, OC_DISCOVERABLE | OC_OBSERVABLE);

        if (OC_STACK_OK != result)
        {
            cout << "Resource creation was unsuccessful\n";
        }
    }

    OCResourceHandle getHandle()
    {
        return m_resourceHandle;
    }

    // Puts representation.
    // Gets values from the representation and
    // updates the internal state
    void put(OCRepresentation& rep)
    {
        try {
            if (rep.getValue("power", m_power))
            {
                cout << "\t\t\t\t" << "power: " << m_power << endl;
            }
            else
            {
                cout << "\t\t\t\t" << "power not found in the representation" << endl;
            }
        }
        catch (exception& e)
        {
            cout << e.what() << endl;
        }

    }

    // Post representation.
    // Post can create new resource or simply act like put.
    // Gets values from the representation and
    // updates the internal state
    OCRepresentation post(OCRepresentation& rep)
    {
        put(rep);
        return get();
    }


    // gets the updated representation.
    // Updates the representation with latest internal state before
    // sending out.
    OCRepresentation get()
    {
        m_lightRep.setValue("power", m_power);

        return m_lightRep;
    }

    void addType(const std::string& type) const
    {
        OCStackResult result = OCPlatform::bindTypeToResource(m_resourceHandle, type);
        if (OC_STACK_OK != result)
        {
            cout << "Binding TypeName to Resource was unsuccessful\n";
        }
    }

    void addInterface(const std::string& interface) const
    {
        OCStackResult result = OCPlatform::bindInterfaceToResource(m_resourceHandle, interface);
        if (OC_STACK_OK != result)
        {
            cout << "Binding TypeName to Resource was unsuccessful\n";
        }
    }

private:
// This is just a sample implementation of entity handler.
// Entity handler can be implemented in several ways by the manufacturer
OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request)
{
    cout << "\tIn Server CPP entity handler:\n";
    OCEntityHandlerResult ehResult = OC_EH_ERROR;
    if(request)
    {
        // Get the request type and request flag
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if(requestFlag & RequestHandlerFlag::RequestFlag)
        {
            cout << "\t\trequestFlag : Request\n";
            auto pResponse = std::make_shared<OC::OCResourceResponse>();
            pResponse->setRequestHandle(request->getRequestHandle());
            pResponse->setResourceHandle(request->getResourceHandle());

            // If the request type is GET
            if(requestType == "GET")
            {
                cout << "\t\t\trequestType : GET\n";
                if(isSlowResponse) // Slow response case
                {
                    static int startedThread = 0;
                    if(!startedThread)
                    {
                        std::thread t(handleSlowResponse, (void *)this, request);
                        startedThread = 1;
                        t.detach();
                    }
                    ehResult = OC_EH_SLOW;
                }
                else // normal response case.
                {

                    pResponse->setResponseResult(OC_EH_OK);
                    pResponse->setResourceRepresentation(get());
                    if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
            }
            else if(requestType == "PUT")
            {
                cout << "\t\t\trequestType : PUT\n";
                OCRepresentation rep = request->getResourceRepresentation();

                // Do related operations related to PUT request
                // Update the lightResource
                put(rep);

                pResponse->setResponseResult(OC_EH_OK);
                pResponse->setResourceRepresentation(get());
                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "POST")
            {
                cout << "\t\t\trequestType : POST\n";

                OCRepresentation rep = request->getResourceRepresentation();

                // Do related operations related to POST request
                OCRepresentation rep_post = post(rep);
                pResponse->setResourceRepresentation(rep_post);

                if(rep_post.hasAttribute("createduri"))
                {
                    pResponse->setResponseResult(OC_EH_RESOURCE_CREATED);
                    pResponse->setNewResourceUri(rep_post.getValue<std::string>("createduri"));
                }

                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "DELETE")
            {
                // DELETE request operations
            }
        }
    }
    else
    {
        std::cout << "Request invalid" << std::endl;
    }

    return ehResult;
}
};

void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest)
{
    // This function handles slow response case
    LightResource* lightPtr = (LightResource*) param;
    // Induce a case for slow response by using sleep
    std::cout << "SLOW response" << std::endl;
    sleep (10);

    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(pRequest->getRequestHandle());
    pResponse->setResourceHandle(pRequest->getResourceHandle());
    pResponse->setResourceRepresentation(lightPtr->get());

    pResponse->setResponseResult(OC_EH_OK);

    // Set the slow response flag back to false
    isSlowResponse = false;
    OCPlatform::sendResponse(pResponse);
    return NULL;
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

OCStackResult SetDeviceInfo()
{
    OCStackResult result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
                                                        deviceName);
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set device name" << endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DATA_MODEL_VERSION,
                                          dataModelVersions);
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set data model versions" << endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, specVersion);
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set spec version" << endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                          protocolIndependentID);
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set piid" << endl;
        return result;
    }

    return OC_STACK_OK;
}

FILE* server_fopen(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_INTROSPECTION_FILE_NAME))
    {
        return fopen("light_introspection.dat", mode);
    }
    else
    {
        return fopen(path, mode);
    }
}


int main(int /*argc*/, char** /*argv[]*/)
{
    // Create persistent storage handlers
    OCPersistentStorage ps{server_fopen, fread, fwrite, fclose, unlink};
    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        &ps
    };

    OCPlatform::Configure(cfg);
    std::cout << "Starting server & setting platform info\n";
    OC_VERIFY(OCPlatform::start() == OC_STACK_OK);

    OCStackResult result = SetPlatformInfo(platformId, manufacturerName, manufacturerLink,
            modelNumber, dateOfManufacture, platformVersion, operatingSystemVersion,
            hardwareVersion, firmwareVersion, supportLink, systemTime);

    result = OCPlatform::registerPlatformInfo(platformInfo);

    if (result != OC_STACK_OK)
    {
        std::cout << "Platform Registration failed\n";
        return -1;
    }

    result = SetDeviceInfo();

    if (result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
        return -1;
    }

    try
    {
        // Create the instance of the resource class
        // (in this case instance of class 'LightResource').
        LightResource myLight;

        // Invoke createResource function of class light.
        myLight.createResource();

        myLight.addType(std::string("core.brightlight"));
        myLight.addInterface(std::string(LINK_INTERFACE));

        DeletePlatformInfo();

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

    // Perform platform clean up.
    OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);
    return 0;
}

