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

#include <mutex>
#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>

#include "ipca.h"

namespace ElevatorClientUWP
{
    public delegate void AuthFailure();
    // Key of map is resource path. Value is array of resource types.
    typedef std::map<std::string, std::vector<std::string>> ResourceList;

    /**
     * Each Elevator object represents a device discovered by IPCA.
     */
    [Windows::UI::Xaml::Data::Bindable]
    public ref class Elevator sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
    {
    public:
        Elevator();
        virtual ~Elevator();
    internal:
        bool Init(IPCAAppHandle appHandle, Platform::String^ name, std::string id);
        void StartObservation();
        void StopObservation();
        void ResourceChangeNotificationCallback(IPCAStatus result, IPCAPropertyBagHandle propertyBagHandle);
        void SetPropertiesCallback(IPCAStatus result, IPCAPropertyBagHandle propertyBagHandle);
        void AuthCompletionCallback(IPCAStatus completionStatus);
        void OnUpdatedInfo();

        IPCADeviceInfo* GetDeviceInfo();
        IPCAPlatformInfo* GetPlatformInfo();
        ResourceList GetResourceInfo();

        bool SetTargetFloor(int floor);

    public:
        virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
        event AuthFailure^ OnAuthFailure;

        property Platform::String^ Name
        {
            Platform::String^ get();
            void set(Platform::String^ value);
        }

        property Platform::String^ CurrentFloor
        {
            Platform::String^ get();
            void set(Platform::String^ value);
        }

    private:
        void OnPropertyChanged(Platform::String^ propertyName);
        IPCAStatus OpenDevice();
        void UpdateDeviceInfo();
        void UpdatePlatformInfo();
        void UpdateResourceInfo();

    private:
        Platform::WeakReference* m_thisWeakRef;
        Windows::UI::Core::CoreDispatcher^ m_dispatcher;
        bool m_initialized;
        Platform::String^ m_deviceName;
        Platform::String^ m_curFloor;
        IPCAAppHandle m_ipcaAppHandle;
        IPCAHandle m_observeHandle;
        IPCAHandle m_requestAccessHandle;
        std::string m_deviceId;
        std::string m_hostAddress;
        IPCADeviceHandle m_deviceHandle;      // from IPCAOpenDevice();
        IPCADeviceInfo* m_deviceInfo;         // valid between IPCAOpenDevice() and IPCACloseDevice().
        IPCAPlatformInfo* m_platformInfo;     // valid between IPCAOpenDevice() and IPCACloseDevice().
        ResourceList m_resourceList;
        std::mutex m_deviceMutex;
        bool m_requestedAccess;
        std::mutex m_requestAccessMutex;
    };
}
