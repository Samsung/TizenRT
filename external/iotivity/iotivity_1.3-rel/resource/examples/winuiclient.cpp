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

#include <string>
#include "winuiclient.h"
#include <Windows.h>
#include <Commctrl.h>
#include <functional>

extern int g_CurSliderVal;
extern HWND g_hwndVolumeSlider, g_hwndVolumeExpectedLabel;
using namespace WinUIClient;
void LabelPrintf (HWND hwndEdit, TCHAR * szFormat, ...);

WinUIClientApp::WinUIClientApp(OCPersistentStorage ps)
    : persistentStorage(ps),
      OBSERVE_TYPE_TO_USE(ObserveType::Observe),
      initialized(false)
{

}

WinUIClientApp::~WinUIClientApp()
{
    if (initialized)
    {
        OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);
    }
}

bool WinUIClientApp::Initialize()
{
    assert(!initialized);

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Both,
        &persistentStorage
    };

    OCPlatform::Configure(cfg);
    if (OCPlatform::start() == OC_STACK_OK)
    {
        initialized = true;
    }
    return initialized;
}

void WinUIClientApp::Run()
{
    try
    {
        // makes it so that all boolean values are printed as 'true/false' in this stream
        std::cout.setf(std::ios::boolalpha);
        // Find all resources
        std::ostringstream requestURI;// << "?rt=core.media";
        std::string s = OC_RSRVD_WELL_KNOWN_URI;
        requestURI << s;

        OCPlatform::findResource("", requestURI.str(),
                CT_DEFAULT, std::bind(&WinUIClientApp::foundResource, this, std::placeholders::_1));
        std::cout<< "Finding Resource... " <<std::endl;

        // Find resource is done twice so that we discover the original resources a second time.
        // These resources will have the same uniqueidentifier (yet be different objects), so that
        // we can verify/show the duplicate-checking code in foundResource(above);
        OCPlatform::findResource("", requestURI.str(),
                CT_DEFAULT, std::bind(&WinUIClientApp::foundResource, this, std::placeholders::_1));
        std::cout<< "Finding Resource for second time..." << std::endl;

    }catch(OCException& e)
    {
        std::cerr << "Exception in main: "<<e.what();
    }
}

void WinUIClientApp::FindResources()
{
    std::ostringstream requestURI;
    requestURI << OC_RSRVD_WELL_KNOWN_URI;// << "?rt=core.media";
    OCPlatform::findResource("", requestURI.str(),
                            CT_DEFAULT, std::bind(&WinUIClientApp::foundResource, this, std::placeholders::_1));
}


// Callback to found resources
void WinUIClientApp::foundResource(std::shared_ptr<OCResource> resource)
{
    std::cout << "In foundResource\n";
    std::string resourceURI;
    std::string hostAddress;
    try
    {
        {
            std::lock_guard<std::mutex> lock(curResourceLock);
            if (discoveredResources.find(resource->uniqueIdentifier()) == discoveredResources.end())
            {
                std::cout << "Found resource " << resource->uniqueIdentifier() <<
                    " for the first time on server with ID: "<< resource->sid()<<std::endl;
                discoveredResources[resource->uniqueIdentifier()] = resource;
            }
            else
            {
                std::cout<<"Found resource "<< resource->uniqueIdentifier() << " again!"<<std::endl;
            }

            if (curResource)
            {
                std::cout << "Found another resource, ignoring"<<std::endl;
                return;
            }
        }

        // Do some operations with resource object.
        if (resource)
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

            if (resourceURI == "/a/media")
            {
                curResource = resource;
                // Call a local function which will internally invoke get API on the resource pointer
                this->GetMediaRepresentation();
                this->BeginObserving();
            }
        }
        else
        {
            // Resource is invalid
            std::cout << "Resource is invalid" << std::endl;
        }

    }
    catch(std::exception& e)
    {
        std::cerr << "Exception in foundResource: "<< e.what() << std::endl;
    }
}

// Local function to get representation of media resource
void WinUIClientApp::GetMediaRepresentation()
{
    if (curResource)
    {
        std::cout << "Getting Media Representation..."<<std::endl;
        // Invoke resource's get API with the callback parameter

        QueryParamsMap test;
        curResource->get(test, std::bind(&WinUIClientApp::onGet, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    }
    else
    {
        std::cout << "No Current Resource to GetMediaRepresentation..."<<std::endl;
    }
}

// Callback handler on GET request
void WinUIClientApp::onGet(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
    try
    {
        if (OC_STACK_OK == eCode)
        {
            std::cout << "GET request was successful" << std::endl;
            std::cout << "Resource URI: " << rep.getUri() << std::endl;

            rep.getValue("state", mymedia.m_state);
            rep.getValue("volume", mymedia.m_volume);
            rep.getValue("name", mymedia.m_name);

            std::cout << "\tstate: " << mymedia.m_state << std::endl;
            std::cout << "\tvolume: " << mymedia.m_volume << std::endl;
            std::cout << "\tname: " << mymedia.m_name << std::endl;

            g_CurSliderVal = mymedia.m_volume;
            SendMessage(g_hwndVolumeSlider, TBM_SETPOS, TRUE, g_CurSliderVal);
            LabelPrintf(g_hwndVolumeExpectedLabel,"Volume: %i", g_CurSliderVal);

        }
        else
        {
            std::cout << "onGET Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onGet" << std::endl;
    }
}

// Local function to put a different state for this resource
void WinUIClientApp::PutMediaRepresentation()
{
    if (curResource)
    {
        OCRepresentation rep;

        std::cout << "Putting media representation..."<<std::endl;

        rep.setValue("state", mymedia.m_state);
        rep.setValue("volume", mymedia.m_volume);

        // Invoke resource's put API with rep, query map and the callback parameter
        curResource->put(rep, QueryParamsMap(), std::bind(&WinUIClientApp::onPut, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
}

// callback handler on PUT request
void WinUIClientApp::onPut(const HeaderOptions& /*headerOptions*/, const OCRepresentation& rep, const int eCode)
{
    try
    {
        if (OC_STACK_OK == eCode || OC_STACK_RESOURCE_CHANGED == eCode)
        {
            std::cout << "PUT request was successful" << std::endl;

            rep.getValue("state", mymedia.m_state);
            rep.getValue("volume", mymedia.m_volume);
            rep.getValue("name", mymedia.m_name);

            std::cout << "\tstate: " << mymedia.m_state << std::endl;
            std::cout << "\tvolume: " << mymedia.m_volume << std::endl;
            std::cout << "\tname: " << mymedia.m_name << std::endl;

        }
        else
        {
            std::cout << "onPut Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onPut" << std::endl;
    }
}

// Local function to put a different state for this resource
void WinUIClientApp::PostMediaRepresentation()
{
    if (curResource)
    {
        OCRepresentation rep;

        std::cout << "Posting media representation..."<<std::endl;

        rep.setValue("state", mymedia.m_state);
        rep.setValue("volume", mymedia.m_volume);

        // Invoke resource's post API with rep, query map and the callback parameter
        curResource->post(rep, QueryParamsMap(), std::bind(&WinUIClientApp::onPost, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
}

void WinUIClientApp::onPost(const HeaderOptions& /*headerOptions*/,
        const OCRepresentation& rep, const int eCode)
{
    try
    {
        if (OC_STACK_OK == eCode || (OC_STACK_RESOURCE_CREATED == eCode
                || OC_STACK_RESOURCE_CHANGED == eCode))
        {
            std::cout << "POST request was successful" << std::endl;

            if (rep.hasAttribute("createduri"))
            {
                std::cout << "\tUri of the created resource: "
                    << rep.getValue<std::string>("createduri") << std::endl;
            }
            else
            {
                rep.getValue("state", mymedia.m_state);
                rep.getValue("volume", mymedia.m_volume);
                rep.getValue("name", mymedia.m_name);

                std::cout << "\tstate: " << mymedia.m_state << std::endl;
                std::cout << "\tvolume: " << mymedia.m_volume << std::endl;
                std::cout << "\tname: " << mymedia.m_name << std::endl;
            }

        }
        else
        {
            std::cout << "onPost Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onPost" << std::endl;
    }
}

void WinUIClientApp::onPost2(const HeaderOptions& /*headerOptions*/,
        const OCRepresentation& rep, const int eCode)
{
    try
    {
        if (OC_STACK_OK == eCode || OC_STACK_RESOURCE_CREATED == eCode)
        {
            std::cout << "POST request was successful" << std::endl;

            if (rep.hasAttribute("createduri"))
            {
                std::cout << "\tUri of the created resource: "
                    << rep.getValue<std::string>("createduri") << std::endl;
            }
            else
            {
                rep.getValue("state", mymedia.m_state);
                rep.getValue("volume", mymedia.m_volume);
                rep.getValue("name", mymedia.m_name);

                std::cout << "\tstate: " << mymedia.m_state << std::endl;
                std::cout << "\tvolume: " << mymedia.m_volume << std::endl;
                std::cout << "\tname: " << mymedia.m_name << std::endl;
            }

        }
        else
        {
            std::cout << "onPost2 Response error: " << eCode << std::endl;
            std::exit(-1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onPost2" << std::endl;
    }

}

void WinUIClientApp::onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation& rep,
                    const int& eCode, const int& sequenceNumber)
{
    try
    {
        if (OC_STACK_OK == eCode && sequenceNumber <= MAX_SEQUENCE_NUMBER)
        {
            if (sequenceNumber == OC_OBSERVE_REGISTER)
            {
                std::cout << "Observe registration action is successful" << std::endl;
            }

            std::cout << "OBSERVE RESULT:"<<std::endl;
            std::cout << "\tSequenceNumber: "<< sequenceNumber << std::endl;
            rep.getValue("state", mymedia.m_state);
            rep.getValue("volume", mymedia.m_volume);
            rep.getValue("name", mymedia.m_name);

            std::cout << "\tstate: " << mymedia.m_state << std::endl;
            std::cout << "\tvolume: " << mymedia.m_volume << std::endl;
            std::cout << "\tname: " << mymedia.m_name << std::endl;

            g_CurSliderVal = mymedia.m_volume;
            SendMessage(g_hwndVolumeSlider, TBM_SETPOS, TRUE, g_CurSliderVal);
            LabelPrintf(g_hwndVolumeExpectedLabel,"Volume: %i", g_CurSliderVal);
        }
        else
        {
            if (OC_STACK_OK == eCode)
            {
                std::cout << "No observe option header is returned in the response." << std::endl;
                std::cout << "For a registration request, it means the registration failed"
                        << std::endl;
            }
            else
            {
                std::cout << "onObserve Response error: " << eCode << std::endl;
                std::exit(-1);
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << " in onObserve" << std::endl;
    }

}

void WinUIClientApp::BeginObserving()
{
    if (OBSERVE_TYPE_TO_USE == ObserveType::Observe)
        std::cout << std::endl << "Observe is used." << std::endl << std::endl;
    else if (OBSERVE_TYPE_TO_USE == ObserveType::ObserveAll)
        std::cout << std::endl << "ObserveAll is used." << std::endl << std::endl;

    curResource->observe(OBSERVE_TYPE_TO_USE, QueryParamsMap(), std::bind(&WinUIClientApp::onObserve, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void WinUIClientApp::CancelObserving()
{
    std::cout<<"Cancelling Observe..."<<std::endl;
    curResource->cancelObserve();
}

std::shared_ptr<OCResource> WinUIClientApp::GetResource()
{
    return curResource;
}

int WinUIClientApp::observe_count()
{
    static int oc = 0;
    return ++oc;
}

