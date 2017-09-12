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

#pragma once

#include "ipca.h"
#include "Elevator.h"

namespace ElevatorClientUWP
{
    public delegate void ElevatorEvent(Elevator^ elevator);

    public ref class ElevatorDiscovery sealed
    {
    public:
        ElevatorDiscovery();
        virtual ~ElevatorDiscovery();

        bool StartElevatorDiscovery();
        void StopElevatorDiscovery();

    public:
        event ElevatorEvent^ OnElevatorDiscovered;
        event ElevatorEvent^ OnElevatorDisconnected;

    internal:
        void DiscoverDevicesCallback(
            IPCADeviceStatus deviceStatus,
            const IPCADiscoveredDeviceInfo* deviceInfo);

        IPCAStatus PwdInputCallback(
            const IPCADeviceInfo* deviceInformation,
            const IPCAPlatformInfo* platformInformation,
            IPCAOwnershipTransferType type,
            char* passwordBuffer,
            size_t passwordBufferSize);

        IPCAStatus PwdDisplayCallback(
            const IPCADeviceInfo* deviceInformation,
            const IPCAPlatformInfo* platformInformation,
            IPCAOwnershipTransferType type,
            const char* password);

    private:
        static Platform::String^ guidStr;
        
        Platform::WeakReference* m_thisWeakRef;
        Windows::UI::Core::CoreDispatcher^ m_dispatcher;

        std::string m_targetDeviceId;

        IPCAAppHandle m_ipcaAppHandle;
        IPCADeviceHandle m_deviceHandle;
        IPCAHandle m_discoverDeviceHandle;
        IPCAHandle m_deviceDiscoveryHandle;

        bool m_targetElevatorDiscovered;

        Windows::UI::Xaml::Controls::PasswordBox^ m_pbPwdInput;
        Windows::UI::Xaml::Controls::TextBox^ m_txtPwdDisplay;
        Windows::UI::Xaml::Controls::ContentDialog^ m_passInputDiag;
        Windows::UI::Xaml::Controls::ContentDialog^ m_passDisplayDiag;

        // Key is device id.  Value is pointer to Elevator.
        Windows::Foundation::Collections::IMap<Platform::String^, Elevator^>^ m_ElevatorList;

        // Sync access to m_ElevatorList.
        std::recursive_mutex m_ElevatorListMutex;
        // Discovery mutex and cond var
        std::mutex m_deviceDiscoveredCbMutex;
        std::condition_variable m_deviceDiscoveredCV;
        std::mutex m_deviceDiscoveryMutex;
        std::condition_variable m_deviceDiscoveryCV;
        // Device mutex
        std::mutex m_deviceMutex;
        // Get properties mutex and cond var
        std::mutex m_getPropsMutex;
        std::condition_variable m_getPropsCV;

        int m_currentFloor;
    };
}