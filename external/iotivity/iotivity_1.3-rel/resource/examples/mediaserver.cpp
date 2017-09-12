/* ****************************************************************
 *
 * Copyright 2016 Intel Corporation All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

///
/// This sample provides steps to define an interface for a resource
/// (properties and methods) and host this resource on the server.
///

#include <functional>
#include <mutex>
#include "platform_features.h"
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"
#include <windows.h>
#include <objbase.h>

#include <commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <stdio.h>
#include <math.h>       /* log */

#define SAFE_RELEASE(x)  \
              if ((x) != NULL)  \
                { (x)->Release(); (x) = NULL; }

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

int gObservation = 0;
void * ChangeMediaRepresentation (void *param);
void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest);
void playPause(void);
void setVolume(int vol);
int getVolume(void);

// Specifies where to notify all observers or list of observers
// false: notifies all observers
// true: notifies list of observers
bool isListOfObservers = false;

// Specifies secure or non-secure
// false: non-secure resource
// true: secure resource
bool isSecure = false;

/// Specifies whether Entity handler is going to do slow response or not
bool isSlowResponse = false;


// Forward declaring the entityHandler

/// This class represents a single resource named 'MediaResource'. This resource has
/// two simple properties named 'state' and 'volume'

class MediaResource
{

public:
    /// Access this property from a TB client
    std::string m_name;
    bool m_state;
    int m_volume;
    std::string m_mediaUri;
    OCResourceHandle m_resourceHandle;
    OCRepresentation m_mediaRep;
    ObservationIds m_interestedObservers;

public:
    /// Constructor
    MediaResource()
        :m_name("Media Player"), m_state(false), m_volume(0), m_mediaUri("/a/media"),
                m_resourceHandle(nullptr)
    {
        // Initialize representation
        m_mediaRep.setUri(m_mediaUri);

        m_mediaRep.setValue("state", m_state);
        m_mediaRep.setValue("volume", m_volume);
        m_mediaRep.setValue("name", m_name);
    }

    /* Note that this does not need to be a member function: for classes you do not have
    access to, you can accomplish this with a free function: */

    /// This function internally calls registerResource API.
    void createResource()
    {
        OCStackResult result = OC_STACK_OK;

        /* Resource Information */
        std::string resourceURI = m_mediaUri;
        std::string resourceTypeName = "core.media";
        std::string resourceInterface = DEFAULT_INTERFACE;

        /* Device Information */
        result = SetDeviceInfo();
        if (OC_STACK_OK != result)
        {
            cout << "Device information registration was unsuccessful\n";
            return;
        }

        /* Platform Info */
        char* platformId = "0A3E0D6F-DBF5-404E-8719-D6880042463A";
        char* manufacturerName = "OCF";
        char* manufacturerLink = "https://www.iotivity.org";
        char* modelNumber = "895";
        char* dateOfManufacture = "2016-01-15";
        char* platformVersion = "1.0";
        char* osVersion = "1.0";
        char* hardwareVersion = "1.0";
        char* firmwareVersion = "1.0";
        char* supportLink = "https://www.iotivity.org";
        OCPlatformInfo platformInfo = { platformId,
                                        manufacturerName,
                                        manufacturerLink,
                                        modelNumber,
                                        dateOfManufacture,
                                        platformVersion,
                                        osVersion,
                                        hardwareVersion,
                                        firmwareVersion,
                                        supportLink,
                                        nullptr
                                      };

        result = OCPlatform::registerPlatformInfo(platformInfo);
        if (OC_STACK_OK != result)
        {
            cout << "Platform information registration was unsuccessful\n";
            return;
        }

        // OCResourceProperty is defined ocstack.h
        uint8_t resourceProperty;
        if (isSecure)
        {
            resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE;
        }
        else
        {
            resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;
        }
        EntityHandler cb = std::bind(&MediaResource::entityHandler, this, PH::_1);

        // This will internally create and register the resource.
        result = OCPlatform::registerResource(m_resourceHandle, resourceURI, resourceTypeName,
                                              resourceInterface, cb, resourceProperty);
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
        try
        {
            if (rep.getValue("state", m_state))
            {
                cout << "\t\t\t\t" << "state: " << m_state << endl;
                if(m_state)
                {
                    playPause();
                }
            }
            else
            {
                cout << "\t\t\t\t" << "state not found in the representation" << endl;
            }

            if (rep.getValue("volume", m_volume))
            {
                cout << "\t\t\t\t" << "volume: " << m_volume << endl;
                if((0 <= m_volume) && (m_volume <= 100))
                {
                    setVolume(m_volume);
                }
            }
            else
            {
                cout << "\t\t\t\t" << "volume not found in the representation" << endl;
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
        m_mediaRep.setValue("state", m_state);
        m_mediaRep.setValue("volume", m_volume);

        return m_mediaRep;
    }

    void addType(const std::string& type) const
    {
        OCStackResult result = OCPlatform::bindTypeToResource(m_resourceHandle, type);
        if (OC_STACK_OK != result)
        {
            cout << "Binding TypeName to Resource was unsuccessful\n";
        }
    }

    void addInterface(const std::string& intf) const
    {
        OCStackResult result = OCPlatform::bindInterfaceToResource(m_resourceHandle, intf);
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

            // Check for query params (if any)
            QueryParamsMap queries = request->getQueryParameters();

            if (!queries.empty())
            {
                cout << "\nQuery processing upto entityHandler" << std::endl;
            }
            for (auto it : queries)
            {
                cout << "Query key: " << it.first << " value : " << it.second
                        << std::endl;
            }

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
                // Update the mediaResource
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
                else
                {
                    pResponse->setResponseResult(OC_EH_OK);
                }

                if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
                {
                    ehResult = OC_EH_OK;
                }
            }
            else if(requestType == "DELETE")
            {
                cout << "Delete request received" << endl;
            }
        }

        if(requestFlag & RequestHandlerFlag::ObserverFlag)
        {
            ObservationInfo observationInfo = request->getObservationInfo();
            if(ObserveAction::ObserveRegister == observationInfo.action)
            {
                m_interestedObservers.push_back(observationInfo.obsId);
            }
            else if(ObserveAction::ObserveUnregister == observationInfo.action)
            {
                m_interestedObservers.erase(std::remove(
                                                            m_interestedObservers.begin(),
                                                            m_interestedObservers.end(),
                                                            observationInfo.obsId),
                                                            m_interestedObservers.end());
            }

            cout << "\t\trequestFlag : Observer\n";
            gObservation = 1;
            static int startedThread = 0;

            // Observation happens on a different thread in ChangeMediaRepresentation function.
            // If we have not created the thread already, we will create one here.

            if(!startedThread)
            {
                std::thread t(ChangeMediaRepresentation, (void *)this);
                startedThread = 1;
                t.detach();
            }

            ehResult = OC_EH_OK;
        }
    }
    else
    {
        cout << "Request invalid" << std::endl;
    }

    return ehResult;
}

OCStackResult SetDeviceInfo()
{
    OCStackResult result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_DEVICE_NAME,
                                                        "IoTivity Media Server");
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set device name" << endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, "ocf.1.1.0");
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set spec version" << endl;
        return result;
    }

    result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_PROTOCOL_INDEPENDENT_ID,
                                          "9f9752ed-b4ab-4662-af22-7e541bbee2fb");
    if (result != OC_STACK_OK)
    {
        cout << "Failed to set piid" << endl;
        return result;
    }

    return OC_STACK_OK;
}

};

// ChangeMediaRepresentaion is an observation function,
// which notifies any changes to the resource to stack
// via notifyObservers
void * ChangeMediaRepresentation (void *param)
{
    int prevVolume = 0;
    MediaResource* mediaPtr = (MediaResource*) param;

    // This function continuously monitors for the changes
    while (1)
    {
        Sleep(100);

        if (gObservation)
        {
            prevVolume = mediaPtr->m_volume;
            mediaPtr->m_volume = getVolume();
            if (prevVolume == mediaPtr->m_volume)
                continue;

            cout << "Volume changed from " << prevVolume << "% to " << mediaPtr->m_volume << "%\n";

            // If under observation if there are any changes to the media resource
            // we call notifyObservors
            //
            // For demostration we are changing the volume value and notifying.

            cout << "\nVolume updated to : " << mediaPtr->m_volume << endl;
            cout << "Notifying observers with resource handle: " << mediaPtr->getHandle() << endl;

            OCStackResult result = OC_STACK_OK;

            if(isListOfObservers)
            {
                std::shared_ptr<OCResourceResponse> resourceResponse =
                            {std::make_shared<OCResourceResponse>()};

                resourceResponse->setResourceRepresentation(mediaPtr->get(), DEFAULT_INTERFACE);

                result = OCPlatform::notifyListOfObservers(  mediaPtr->getHandle(),
                                                             mediaPtr->m_interestedObservers,
                                                             resourceResponse);
            }
            else
            {
                result = OCPlatform::notifyAllObservers(mediaPtr->getHandle());
            }

            if(OC_STACK_NO_OBSERVERS == result)
            {
                cout << "No More observers, stopping notifications" << endl;
                gObservation = 0;
            }
        }
    }

    return NULL;
}

void * handleSlowResponse (void *param, std::shared_ptr<OCResourceRequest> pRequest)
{
    // This function handles slow response case
    MediaResource* mediaPtr = (MediaResource*) param;
    // Induce a case for slow response by using sleep
    cout << "SLOW response" << std::endl;
    sleep (10);

    auto pResponse = std::make_shared<OC::OCResourceResponse>();
    pResponse->setRequestHandle(pRequest->getRequestHandle());
    pResponse->setResourceHandle(pRequest->getResourceHandle());
    pResponse->setResourceRepresentation(mediaPtr->get());

    pResponse->setResponseResult(OC_EH_OK);

    // Set the slow response flag back to false
    isSlowResponse = false;
    OCPlatform::sendResponse(pResponse);
    return NULL;
}

void PrintUsage()
{
    cout << std::endl;
    cout << "Usage : mediaserver <value>\n";
    cout << "    Default - Non-secure resource and notify all observers\n";
    cout << "    1 - Non-secure resource and notify list of observers\n\n";
    cout << "    2 - Secure resource and notify all observers\n";
    cout << "    3 - Secure resource and notify list of observers\n\n";
    cout << "    4 - Non-secure resource, GET slow response, notify all observers\n";
}

static FILE* client_open(const char* path, const char* mode)
{
    if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
    {
        return fopen("./oic_svr_db_server.dat", mode);
    }
    else
    {
        return fopen(path, mode);
    }
}

void playPause()
{
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = VK_MEDIA_PLAY_PAUSE; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press

    SendInput(1, &ip, sizeof(INPUT));
    // Release the "Play/Pause" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

int getVolume()
{
    IAudioEndpointVolume *g_pEndptVol = NULL;
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;

    CoInitialize(NULL);

    // Get enumerator for audio endpoint devices.
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                          NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator),
                          (void**)&pEnumerator);

    // Get default audio-rendering device.
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),
                           CLSCTX_ALL, NULL, (void**)&g_pEndptVol);
    float currentVal;
    hr = g_pEndptVol->GetMasterVolumeLevelScalar(&currentVal);
    fflush(stdout); // just in case

    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(g_pEndptVol)
    CoUninitialize();
    return ((int) round(100 * currentVal));
}

void setVolume(int vol)
{
    IAudioEndpointVolume *g_pEndptVol = NULL;
    HRESULT hr = S_OK;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;

    CoInitialize(NULL);

    // Get enumerator for audio endpoint devices.
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
                          NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator),
                          (void**)&pEnumerator);

    // Get default audio-rendering device.
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),
                           CLSCTX_ALL, NULL, (void**)&g_pEndptVol);
    float got = (float)vol / 100.0f; // needs to be within 1.0 to 0.0
    hr = g_pEndptVol->SetMasterVolumeLevelScalar(got, NULL);
    fflush(stdout); // just in case

    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(g_pEndptVol)
    CoUninitialize();
}

int main(int argc, char* argv[])
{
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    if (argc == 1)
    {
        isListOfObservers = false;
        isSecure = false;
    }
    else if (argc == 2)
    {
        int value = atoi(argv[1]);
        switch (value)
        {
            case 1:
                isListOfObservers = true;
                isSecure = false;
                break;
            case 2:
                isListOfObservers = false;
                isSecure = true;
                break;
            case 3:
                isListOfObservers = true;
                isSecure = true;
                break;
            case 4:
                isSlowResponse = true;
                break;
            default:
                PrintUsage();
                break;
       }
     }
    else
    {
        PrintUsage();
        return -1;
    }

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Server,
        &ps
    };

    OCPlatform::Configure(cfg);
    try
    {
        OC_VERIFY(OCPlatform::start() == OC_STACK_OK);
        
        // Create the instance of the resource class
        // (in this case instance of class 'MediaResource').
        MediaResource myMedia;

        // Invoke createResource function of class media.
        myMedia.createResource();
        cout << "Created resource." << std::endl;

        cout <<"Waiting. Press \"Enter\" to quit." << std::endl;
#ifdef _MSC_VER
#undef max
#endif
        // Ignoring all input except for EOL.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Perform platform clean up.
        OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);
    }
    catch(OCException &e)
    {
        cout << "OCException in main : " << e.what() << endl;
    }

    return 0;
}

