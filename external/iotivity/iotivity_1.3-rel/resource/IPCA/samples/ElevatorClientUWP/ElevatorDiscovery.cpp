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
#include "ElevatorDiscovery.h"

#include "iotivity_config.h"

#include <mutex>
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <climits>
#include <sstream>

#include "ipca.h"
#include "Elevator.h"
#include "ElevatorViewModel.h"

using namespace ElevatorClientUWP;

using namespace concurrency;

using namespace std;
using namespace Platform;
using namespace Platform::Collections;
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

#define TIMEOUT_MS  3000

String^ ElevatorDiscovery::guidStr = "{ab2593e4-2994-401e-a526-c92fd9b71414}";

static void IPCA_CALL DiscoverDevicesCallback(
    void* context,
    IPCADeviceStatus deviceStatus,
    const IPCADiscoveredDeviceInfo* deviceInfo)
{
    if (context)
    {
        ElevatorDiscovery^ elvDisc = 
            (reinterpret_cast<WeakReference*>(context))->Resolve<ElevatorDiscovery>();
        if (elvDisc)
        {
            elvDisc->DiscoverDevicesCallback(deviceStatus, deviceInfo);
        }
    }
}

static IPCAStatus IPCA_CALL PwdInputCallback(
    void* context,
    const IPCADeviceInfo* deviceInformation,
    const IPCAPlatformInfo* platformInformation,
    IPCAOwnershipTransferType type,
    char* passwordBuffer,
    size_t passwordBufferSize)
{
    if (context)
    {
        ElevatorDiscovery^ elvDisc =
            (reinterpret_cast<WeakReference*>(context))->Resolve<ElevatorDiscovery>();
        if (elvDisc)
        {
            return elvDisc->PwdInputCallback(deviceInformation, platformInformation, type,
                passwordBuffer, passwordBufferSize);
        }
    }

    // If reached here means context is either null or wrong type
    return IPCA_INVALID_ARGUMENT;
}

static IPCAStatus IPCA_CALL PwdDisplayCallback(
    void* context,
    const IPCADeviceInfo* deviceInformation,
    const IPCAPlatformInfo* platformInformation,
    IPCAOwnershipTransferType type,
    const char* password)
{
    if (context)
    {
        ElevatorDiscovery^ elvDisc =
            (reinterpret_cast<WeakReference*>(context))->Resolve<ElevatorDiscovery>();
        if (elvDisc)
        {
            return elvDisc->PwdDisplayCallback(deviceInformation, platformInformation, type, password);
        }
    }

    // If reached here means context is either null or wrong type
    return IPCA_INVALID_ARGUMENT;
}

ElevatorDiscovery::ElevatorDiscovery() :
    m_ipcaAppHandle(nullptr),
    m_deviceHandle(nullptr),
    m_discoverDeviceHandle(nullptr),
    m_deviceDiscoveryHandle(nullptr),
    m_targetElevatorDiscovered(false),
    m_currentFloor(0)
{
    m_thisWeakRef = new WeakReference(this);
    m_dispatcher = CoreApplication::MainView->Dispatcher;

    GUID guid;
    HRESULT hr = IIDFromString(guidStr->Data(), &guid);
    if (FAILED(hr))
    {
        if (hr == E_INVALIDARG)
        {
            throw ref new InvalidArgumentException("IIDFromString invalid arg");
        }
        else if (hr == E_OUTOFMEMORY)
        {
            throw ref new OutOfMemoryException("IIDFromString out of memory");
        }
        else
        {
            throw ref new FailureException("IIDFromString failed");
        }
    }

    IPCAUuid appId;
    byte* guidBytes = reinterpret_cast<byte*>(&guid);
    for (int i = 0; i < 16; i++)
    {
        appId.uuid[i] = guidBytes[i];
    }

    IPCAAppInfo ipcaAppInfo = { appId, "ElevatorClientUWP", "1.0.0", "Microsoft" };

    IPCAStatus status = IPCAOpen(&ipcaAppInfo, IPCA_VERSION_1, &m_ipcaAppHandle);
    if (status != IPCA_OK)
    {
        String^ err = "IPCAOpen() Failed. Status: " + static_cast<int>(status).ToString();
        throw ref new FailureException(err);
    }

    status = IPCASetPasswordCallbacks(m_ipcaAppHandle, &::PwdInputCallback,
        &::PwdDisplayCallback, m_thisWeakRef);
    if (status != IPCA_OK)
    {
        String^ err = "IPCASetPasswordCallbacks() Failed. Status: " + static_cast<int>(status).ToString();
        throw ref new FailureException(err);
    }

    m_ElevatorList = ref new Map<String^, Elevator^>();

    // Generate Password Input Content Dialog
    TextBlock^ tbPwdInput = ref new TextBlock();
    tbPwdInput->Text = "Please enter Elevator Password:";
    tbPwdInput->Margin = Thickness(0, 5, 0, 0);

    m_pbPwdInput = ref new PasswordBox();
    m_pbPwdInput->PasswordRevealMode = PasswordRevealMode::Peek;
    m_pbPwdInput->Margin = Thickness(10);
    m_pbPwdInput->MinWidth = 250;

    StackPanel^ spPwdInput = ref new StackPanel();
    spPwdInput->Orientation = Orientation::Vertical;
    spPwdInput->Children->Append(tbPwdInput);
    spPwdInput->Children->Append(m_pbPwdInput);

    m_passInputDiag = ref new ContentDialog();
    m_passInputDiag->Title = "Authentication Needed";
    m_passInputDiag->Content = spPwdInput;
    m_passInputDiag->PrimaryButtonText = "Authenticate";
    m_passInputDiag->SecondaryButtonText = "Cancel";

    // Generate Password Display Content Dialog
    TextBlock^ tbPwdDisplay = ref new TextBlock();
    tbPwdDisplay->Text = "Elevator Password is:";
    tbPwdDisplay->Margin = Thickness(0, 5, 0, 0);

    m_txtPwdDisplay = ref new TextBox();
    m_txtPwdDisplay->IsReadOnly = true;
    m_txtPwdDisplay->Margin = 10;
    m_txtPwdDisplay->BorderThickness = 0;

    StackPanel^ spPwdDisplay = ref new StackPanel();
    spPwdDisplay->Orientation = Orientation::Vertical;
    spPwdDisplay->Children->Append(tbPwdDisplay);
    spPwdDisplay->Children->Append(m_txtPwdDisplay);

    m_passDisplayDiag = ref new ContentDialog();
    m_passDisplayDiag->Title = "Authentication Info";
    m_passDisplayDiag->Content = spPwdDisplay;
    m_passDisplayDiag->PrimaryButtonText = "OK";
}

ElevatorDiscovery::~ElevatorDiscovery()
{
    StopElevatorDiscovery();

    std::lock_guard<std::recursive_mutex> lock(m_ElevatorListMutex);
    m_ElevatorList->Clear();

    if (m_ipcaAppHandle != nullptr)
    {
        IPCAClose(m_ipcaAppHandle);
    }

    delete m_thisWeakRef;
}

bool ElevatorDiscovery::StartElevatorDiscovery()
{
    std::unique_lock<std::mutex> lock(m_deviceDiscoveryMutex);

    if (!m_deviceDiscoveryHandle)
    {
        // Start the discovery if not already started

        const char* resourceTypes[] = {
            "x.org.iotivity.sample.elevator",
            "x.org.iotivity.sample.elevator2",
            "x.org.iotivity.sample.elevator3",
            "x.org.iotivity.sample.elevator4",
        };

        const int NUMBER_OF_RESOURCE_TYPES = sizeof(resourceTypes) / sizeof(char*);

        IPCAStatus status = IPCADiscoverDevices(
            m_ipcaAppHandle,
            &::DiscoverDevicesCallback,
            m_thisWeakRef,
            resourceTypes,
            NUMBER_OF_RESOURCE_TYPES,
            &m_deviceDiscoveryHandle);

        if (status != IPCA_OK)
        {
            return false;
        }
    }

    return true;
}

void ElevatorDiscovery::StopElevatorDiscovery()
{
    std::unique_lock<std::mutex> lock(m_deviceDiscoveryMutex);
    // Stop discovery.
    if (m_deviceDiscoveryHandle)
    {
        IPCACloseHandle(m_deviceDiscoveryHandle, nullptr, nullptr);
        m_deviceDiscoveryHandle = nullptr;
    }
}

void ElevatorDiscovery::DiscoverDevicesCallback(IPCADeviceStatus deviceStatus,
    const IPCADiscoveredDeviceInfo* deviceInfo)
{
    std::lock_guard<std::recursive_mutex> lock(m_ElevatorListMutex);

    std::string deviceIdStr = deviceInfo->deviceId;
    String^ deviceId = Util::ConvertStrtoPlatformStr(deviceInfo->deviceId);
    String^ deviceName = Util::ConvertStrtoPlatformStr(deviceInfo->deviceName);
    std::vector<std::string> deviceUris;
    for (size_t i = 0; i < deviceInfo->deviceUriCount; i++)
    {
        deviceUris.push_back(deviceInfo->deviceUris[i]);
    }

    Elevator^ elevator = nullptr;

    try
    {
        elevator = m_ElevatorList->Lookup(deviceId);
    }
    catch (OutOfBoundsException^ e)
    {
        if (deviceStatus != IPCA_DEVICE_DISCOVERED)
        {
            // Log Unexpected discovery status
        }
        // Device not in the Map
        elevator = ref new Elevator();
        elevator->Init(m_ipcaAppHandle, deviceName, deviceIdStr);
        m_ElevatorList->Insert(deviceId, elevator);
    }

    switch (deviceStatus)
    {
    case IPCA_DEVICE_DISCOVERED:
        OnElevatorDiscovered(elevator);
        break;
    case IPCA_DEVICE_UPDATED_INFO:
        elevator->OnUpdatedInfo();
        break;
    case IPCA_DEVICE_STOPPED_RESPONDING:
        OnElevatorDisconnected(elevator);
        m_ElevatorList->Remove(deviceId);
        break;
    default:
        break;
    }
}

IPCAStatus ElevatorDiscovery::PwdInputCallback(
    const IPCADeviceInfo* deviceInformation,
    const IPCAPlatformInfo* platformInformation,
    IPCAOwnershipTransferType type,
    char* passwordBuffer,
    size_t passwordBufferSize)
{
    IAsyncOperation<ContentDialogResult>^ showPwdDiagAsync;
    create_task(m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
        [this, &showPwdDiagAsync]
        {
            m_pbPwdInput->Password = nullptr;
            showPwdDiagAsync = m_passInputDiag->ShowAsync();
        }))
    ).get();

    ContentDialogResult result = create_task(showPwdDiagAsync).get();
    // Get the password
    if (result == ContentDialogResult::Primary)
    {
        String^ pwd = nullptr;
        create_task(m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
            [this, &pwd]
            {
                pwd = m_pbPwdInput->Password;
            }))
        ).get();

        std::string pwdStr = Util::ConvertWStrtoStr(pwd->Data());
        if (pwdStr.empty() || ((pwdStr.length() + 1) > passwordBufferSize)) // +1 for null char
        {
            return IPCA_INVALID_ARGUMENT;
        }

        strcpy_s(passwordBuffer, passwordBufferSize, pwdStr.c_str());
    }
    else
    {
        return IPCA_INVALID_ARGUMENT;
    }

    return IPCA_OK;
}

IPCAStatus ElevatorDiscovery::PwdDisplayCallback(
    const IPCADeviceInfo* deviceInformation,
    const IPCAPlatformInfo* platformInformation,
    IPCAOwnershipTransferType type,
    const char* password)
{
    String^ pwd = Util::ConvertStrtoPlatformStr(password);
    if (pwd == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
        [this, pwd]
        {
            m_txtPwdDisplay->Text = pwd;
            m_passDisplayDiag->ShowAsync();
        }
    ));

    return IPCA_OK;
}
