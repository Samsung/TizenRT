/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
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

#include "pch.h"
#include <string>
#include <iostream>
#include "ipca.h"
#include "Elevator.h"
#include "ElevatorViewModel.h"

using namespace ElevatorClientUWP;

using namespace std;
using namespace Platform;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// Callback from IPCAObserveResource().
void IPCA_CALL ResourceChangeNotificationCallback(
    IPCAStatus result,
    void* context,
    IPCAPropertyBagHandle propertyBagHandle)
{
    if (context)
    {
        Elevator^ dev = (reinterpret_cast<WeakReference*>(context))->Resolve<Elevator>();
        if (dev)
        {
            dev->ResourceChangeNotificationCallback(result, propertyBagHandle);
        }
    }
}

void IPCA_CALL SetPropertiesCallback(
    IPCAStatus result,
    void* context,
    IPCAPropertyBagHandle propertyBagHandle)
{
    if (context)
    {
        Elevator^ dev = (reinterpret_cast<WeakReference*>(context))->Resolve<Elevator>();
        if (dev)
        {
            dev->SetPropertiesCallback(result, propertyBagHandle);
        }
    }
}

void IPCA_CALL AuthCompletionCallback(IPCAStatus completionStatus, void* context)
{
    if (context)
    {
        Elevator^ dev = (reinterpret_cast<WeakReference*>(context))->Resolve<Elevator>();
        if (dev)
        {
            dev->AuthCompletionCallback(completionStatus);
        }
    }
}

Elevator::Elevator() :
    m_initialized(false),
    m_dispatcher(nullptr),
    m_ipcaAppHandle(nullptr),
    m_observeHandle(nullptr),
    m_deviceName(nullptr),
    m_deviceHandle(nullptr),
    m_deviceInfo(nullptr),
    m_platformInfo(nullptr),
    m_requestedAccess(false),
    m_requestAccessHandle(nullptr)
{
    m_thisWeakRef = new WeakReference(this);
    m_dispatcher = CoreApplication::MainView->Dispatcher;
}

Elevator::~Elevator()
{
    StopObservation();

    if (m_requestAccessHandle != nullptr)
    {
        IPCACloseHandle(m_requestAccessHandle, nullptr, nullptr);
        m_requestAccessHandle = nullptr;
    }
    
    if (m_deviceHandle != nullptr)
    {
        IPCACloseDevice(m_deviceHandle);
        m_deviceHandle = nullptr;
    }

    if (m_deviceInfo != nullptr)
    {
        IPCAFreeDeviceInfo(m_deviceInfo);
        m_deviceInfo = nullptr;
    }

    if (m_platformInfo != nullptr)
    {
        IPCAFreePlatformInfo(m_platformInfo);
        m_platformInfo = nullptr;
    }
}

String^ Elevator::Name::get()
{
    String^ ret = m_deviceName;

    if ((ret == nullptr) || ret->IsEmpty())
    {
        ret = Util::ConvertStrtoPlatformStr(m_deviceId.c_str());
    }
    
    return ret;
}

void Elevator::Name::set(String^ value)
{
    if (m_deviceName != value)
    {
        m_deviceName = value;
        OnPropertyChanged("Name");
    }
}

String^ Elevator::CurrentFloor::get()
{
    return m_curFloor;
}

void Elevator::CurrentFloor::set(String^ value)
{
    if (m_curFloor != value)
    {
        m_curFloor = value;
        OnPropertyChanged("CurrentFloor");
    }
}

void Elevator::OnPropertyChanged(String^ propertyName)
{
    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
    [this, propertyName]
    {
        PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
    }));
}

IPCAStatus Elevator::OpenDevice()
{
    if (m_deviceHandle != nullptr)
    {
        return IPCA_OK;
    }
    else
    {
        return IPCAOpenDevice(m_ipcaAppHandle, m_deviceId.c_str(), &m_deviceHandle);
    }
}

void Elevator::UpdateDeviceInfo()
{
    IPCAStatus status;

    if (!m_initialized)
    {
        return;
    }

    if (OpenDevice() != IPCA_OK)
    {
        return;
    }

    if (m_deviceInfo != nullptr)
    {
        IPCAFreeDeviceInfo(m_deviceInfo);
        m_deviceInfo = nullptr;
    }

    status = IPCAGetDeviceInfo(m_deviceHandle, &m_deviceInfo);
    if (IPCA_OK != status)
    {
        return;
    }

    Name = Util::ConvertStrtoPlatformStr(m_deviceInfo->deviceName);
}

void Elevator::UpdatePlatformInfo()
{
    IPCAStatus status;

    if (!m_initialized)
    {
        return;
    }

    if (OpenDevice() != IPCA_OK)
    {
        return;
    }

    if (m_platformInfo != nullptr)
    {
        IPCAFreePlatformInfo(m_platformInfo);
        m_platformInfo = nullptr;
    }

    status = IPCAGetPlatformInfo(m_deviceHandle, &m_platformInfo);
    if (IPCA_OK != status)
    {
        return;
    }
}

void Elevator::UpdateResourceInfo()
{
    IPCAStatus status;

    ResourceList emptyResourceList;

    if (!m_initialized)
    {
        return;
    }

    if (OpenDevice() != IPCA_OK)
    {
        return;
    }

    char** resourcePathList;
    size_t resourceListCount;
    status = IPCAGetResources(m_deviceHandle,
        nullptr, nullptr, &resourcePathList, &resourceListCount);
    if (IPCA_OK != status)
    {
        return;
    }

    m_resourceList.clear();
    for (size_t i = 0; i < resourceListCount; i++)
    {
        char** resourceTypes;
        size_t resourceTypeCount;
        status = IPCAGetResourceTypes(m_deviceHandle,
            resourcePathList[i], &resourceTypes, &resourceTypeCount);
        if (IPCA_OK == status)
        {
            for (size_t j = 0; j < resourceTypeCount; j++)
            {
                m_resourceList[resourcePathList[i]].push_back(resourceTypes[j]);
            }
            IPCAFreeStringArray(resourceTypes, resourceTypeCount);
        }
        else
        {
            // Log failure
        }
    }

    IPCAFreeStringArray(resourcePathList, resourceListCount);
}

bool Elevator::Init(IPCAAppHandle appHandle, Platform::String^ name, std::string id)
{
    if (!m_initialized)
    {
        m_ipcaAppHandle = appHandle;
        m_deviceName = name;
        m_deviceId = id;
        m_initialized = true;
    }

    return m_initialized;
}

void Elevator::StartObservation()
{
    if (OpenDevice() != IPCA_OK)
    {
        throw ref new FailureException("Failed to open device");
    }

    StopObservation();

    if (IPCAObserveResource(
        m_deviceHandle,
        &::ResourceChangeNotificationCallback,
        m_thisWeakRef,
        "/ipca/sample/elevator",
        nullptr,
        &m_observeHandle) != IPCA_OK)
    {
        throw ref new FailureException("IPCAObserveResource failed.");
    }
}

void Elevator::StopObservation()
{
    if (m_observeHandle != nullptr)
    {
        IPCACloseHandle(m_observeHandle, nullptr, nullptr);
        m_observeHandle = nullptr;
    }
}

void Elevator::ResourceChangeNotificationCallback(IPCAStatus result,
    IPCAPropertyBagHandle propertyBagHandle)
{
    int observedCurrentFloor = -1;

    if (result == IPCA_ACCESS_DENIED)
    {
        std::lock_guard<std::mutex> lock(m_requestAccessMutex);
        if (!m_requestedAccess)
        {
            if (m_requestAccessHandle != nullptr)
            {
                IPCACloseHandle(m_requestAccessHandle, nullptr, nullptr);
                m_requestAccessHandle = nullptr;
            }

            IPCAStatus reqResult = IPCARequestAccess(
                m_deviceHandle,
                nullptr,
                &::AuthCompletionCallback,
                m_thisWeakRef,
                &m_requestAccessHandle);
            if (reqResult == IPCA_OK)
            {
                m_requestedAccess = true;
            }
        }
        return;
    }
    else if ((result != IPCA_OK) && (result != IPCA_DEVICE_APPEAR_OFFLINE))
    {
        String^ err = "Failure in Resource Change Notification. result=" +
            static_cast<int>(result).ToString();
        Util::ShowErrorMsg(m_dispatcher, err);
        return;
    }

    if (IPCAPropertyBagGetValueInt(propertyBagHandle, "x.org.iotivity.CurrentFloor",
        &observedCurrentFloor) == IPCA_OK)
    {
        CurrentFloor = observedCurrentFloor.ToString();
    }
}

void Elevator::AuthCompletionCallback(IPCAStatus completionStatus)
{
    if ((completionStatus != IPCA_SECURITY_UPDATE_REQUEST_FINISHED) &&
        (completionStatus != IPCA_DEVICE_APPEAR_OFFLINE))
    {
        std::lock_guard<std::mutex> lock(m_requestAccessMutex);
        // Reset requested access to enable request access again.
        m_requestedAccess = false;

        String^ err = "Failure in Auth Callback. result=" +
            static_cast<int>(completionStatus).ToString();
        Util::ShowErrorMsg(m_dispatcher, err);
        OnAuthFailure();
        return;
    }

    // Restart observation
    StartObservation();
}

void Elevator::SetPropertiesCallback(IPCAStatus result, IPCAPropertyBagHandle propertyBagHandle)
{
    if ((result != IPCA_OK) && (result != IPCA_DEVICE_APPEAR_OFFLINE))
    {
        String^ err = "Failure trying to Set Properties. result=" +
            static_cast<int>(result).ToString();
        Util::ShowErrorMsg(m_dispatcher, err);
    }
}

void Elevator::OnUpdatedInfo()
{
    std::lock_guard<std::mutex> lock(m_deviceMutex);
    UpdateDeviceInfo();
    UpdatePlatformInfo();
    UpdateResourceInfo();
}

IPCADeviceInfo* Elevator::GetDeviceInfo()
{
    std::lock_guard<std::mutex> lock(m_deviceMutex);
    if (!m_initialized)
    {
        return nullptr;
    }

    return m_deviceInfo;
}

IPCAPlatformInfo* Elevator::GetPlatformInfo()
{
    std::lock_guard<std::mutex> lock(m_deviceMutex);
    if (!m_initialized)
    {
        return nullptr;
    }

    return m_platformInfo;
}

ResourceList Elevator::GetResourceInfo()
{
    std::lock_guard<std::mutex> lock(m_deviceMutex);
    ResourceList emptyResourceList;

    if (!m_initialized)
    {
        return emptyResourceList;
    }

    return m_resourceList;
}

bool Elevator::SetTargetFloor(int floor)
{
    if (OpenDevice() != IPCA_OK)
    {
        return false;
    }

    bool ret = false;
    IPCAPropertyBagHandle propertyBagHandle;
    IPCAStatus status = IPCAPropertyBagCreate(&propertyBagHandle);
    if (IPCA_OK == status)
    {
        status = IPCAPropertyBagSetValueInt(propertyBagHandle,
            "x.org.iotivity.TargetFloor", floor);

        if (IPCA_OK == status)
        {
            status = IPCASetProperties(m_deviceHandle,
                &::SetPropertiesCallback,
                m_thisWeakRef,
                "/ipca/sample/elevator",
                "x.org.iotivity.sample.elevator",
                nullptr,
                propertyBagHandle,
                nullptr);

            if (IPCA_OK == status)
            {
                ret = true;
            }
        }
        IPCAPropertyBagDestroy(propertyBagHandle);
    }

    return ret;
}
