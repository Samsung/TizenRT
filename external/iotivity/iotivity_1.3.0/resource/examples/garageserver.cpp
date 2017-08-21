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
/// This sample provides using varous json types in the representation.
///

#include <functional>

#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

using namespace OC;
using namespace std;

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
std::string  deviceName = "IoTivity Garage Server";
std::string  specVersion = "ocf.1.1.0";
std::vector<std::string> dataModelVersions = {"ocf.res.1.1.0"};
std::string  protocolIndependentID = "1cbab786-ca5f-4eb9-89b2-e90eb4820145";

// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;

// Forward declaring the entityHandler
OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);

/// This class represents a single resource named 'GarageResource'.
class GarageResource
{
public:
    /// Access this property from a TB client
    std::string m_name;
    bool m_state;
    std::string m_garageUri;
    OCResourceHandle m_resourceHandle;
    OCRepresentation m_garageRep;
    ObservationIds m_interestedObservers;

    // array of lights representation with in GarageResource
    OCRepresentation m_lightRep;
    std::vector<OCRepresentation> m_reps;
    std::vector<std::vector<int>> m_hingeStates;

public:
    /// Constructor
    GarageResource(): m_name("John's Garage"), m_state(false), m_garageUri("/a/garage"),
        m_hingeStates{{1,2,3},{4,5,6}}
    {
        // Initialize representation
        m_garageRep.setUri(m_garageUri);

        m_garageRep["state"] = m_state;
        m_garageRep["name"] = m_name;

        // For demonstration purpose we are setting x to nullptr here.
        // In reality it may happen else where.
        m_garageRep["nullAttribute"] = nullptr;

        std::vector<bool> lightStates;
        std::vector<int>  lightPowers;

        for(int i = 0; i <= 9; i++)
        {
            lightStates.push_back(i % 2 == 0);
            lightPowers.push_back(i);
        }

        m_lightRep["states"] = lightStates;
        m_lightRep["powers"] = lightPowers;

        // Storing another representation within a representation
        m_garageRep["light"] = m_lightRep;

        OCRepresentation rep1;
        int value1 = 5;
        rep1["key1"] = value1;
        OCRepresentation rep2;
        int value2 = 10;
        rep2["key2"] = value2;

        m_reps.push_back(rep1);
        m_reps.push_back(rep2);

        // storing array of representations
        m_garageRep["reps"] =  m_reps;


        // setting json string
        std::string json = "{\"num\":10,\"rno\":23.5,\"aoa\":[[1,2],[3]],\"str\":\"john\",\
\"object\":{\"bl1\":false,\"ar\":[2,3]}, \"objects\":[{\"bl2\":true,\"nl\":null},{\"ar1\":[1,2]}]}";
        m_garageRep["json"] = json;

        m_garageRep["hinges"] = m_hingeStates;
    }

    /* Note that this does not need to be a member function: for classes you do not have
    access to, you can accomplish this with a free function: */

    /// This function internally calls registerResource API.
    void createResource()
    {
        std::string resourceURI = m_garageUri; // URI of the resource
        std::string resourceTypeName = "core.garage"; // resource type name.
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
            if (rep.getValue("state", m_state))
            {
                cout << "\t\t\t\t" << "state: " << m_state << endl;
            }
            else
            {
                cout << "\t\t\t\t" << "state not found in the representation" << endl;
            }
        }
        catch (exception& e)
        {
            cout << e.what() << endl;
        }

    }

    // gets the updated representation.
    // Updates the representation with latest internal state before
    // sending out.
    OCRepresentation get()
    {
        m_garageRep["state"] = m_state;

        return m_garageRep;
    }

};

// Create the instance of the resource class (in this case instance of class 'GarageResource').
GarageResource myGarage;

OCStackResult sendResponse(std::shared_ptr<OCResourceRequest> pRequest)
{
    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(pRequest->getRequestHandle());
    pResponse->setResourceHandle(pRequest->getResourceHandle());
    pResponse->setResourceRepresentation(myGarage.get());
    pResponse->setResponseResult(OC_EH_OK);

    return OCPlatform::sendResponse(pResponse);
}

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

            // If the request type is GET
            if(requestType == "GET")
            {
                cout << "\t\t\trequestType : GET\n";
                if(OC_STACK_OK == sendResponse(request))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "PUT")
            {
                cout << "\t\t\trequestType : PUT\n";
                OCRepresentation rep = request->getResourceRepresentation();
                // Do related operations related to PUT request
                myGarage.put(rep);
                if(OC_STACK_OK == sendResponse(request))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "POST")
            {
                // POST request operations
            }
            else if(requestType == "DELETE")
            {
                // DELETE request operations
            }
        }
        if(requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            // OBSERVE operations
        }
    }
    else
    {
        std::cout << "Request invalid" << std::endl;
    }

    return ehResult;
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
    std::string platformVersion, std::string operatingSystemVersion, std::string hardwareVersion,
    std::string firmwareVersion, std::string supportUrl, std::string systemTime)
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

int main(int /*argc*/, char** /*argv[1]*/)
{
    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        nullptr
    };

    OCPlatform::Configure(cfg);
    OC_VERIFY(OCPlatform::start() == OC_STACK_OK);
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

    result = SetDeviceInfo();

    if (result != OC_STACK_OK)
    {
        std::cout << "Device Registration failed\n";
        return -1;
    }

    try
    {
        // Invoke createResource function of class light.
        myGarage.createResource();

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
    catch(OCException e)
    {
        oclog() << e.what();
    }

    OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);

    return 0;
}

