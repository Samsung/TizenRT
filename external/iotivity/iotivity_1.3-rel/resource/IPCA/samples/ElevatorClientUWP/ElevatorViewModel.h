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

#include "Elevator.h"

namespace ElevatorClientUWP
{
    ref class ElevatorDiscovery;

    [Windows::UI::Xaml::Data::Bindable]
    public ref class ElevatorViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
    {
    public:
        ElevatorViewModel();
        virtual ~ElevatorViewModel();
        void DiscoverElevators();
        void SetFloor(int floor);

    public:
        virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

        property Windows::Foundation::Collections::IVector<Elevator^>^ DeviceList
        {
            Windows::Foundation::Collections::IVector<Elevator^>^ get();
        }

        property Elevator^ SelectedDevice
        {
            Elevator^ get();
            void set(Elevator^ value);
        }

    private:
        void OnPropertyChanged(Platform::String^ propertyName);
        void ElevatorDiscovered(Elevator^ elevator);
        void ElevatorDisconnected(Elevator ^elevator);
        void AuthFailure();

    private:
        Windows::UI::Core::CoreDispatcher^ m_dispatcher;
        ElevatorDiscovery^ m_elevatorDisc;
        Platform::String^ m_targetDeviceId;
        Platform::Collections::Vector<Elevator^>^ m_deviceList;
        Elevator^ m_selectedDevice;
    };
}
