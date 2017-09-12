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
#include "ElevatorViewModel.h"
#include "ElevatorDiscovery.h"

using namespace ElevatorClientUWP;

using namespace concurrency;

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

ElevatorViewModel::ElevatorViewModel() : m_dispatcher(nullptr), m_elevatorDisc(nullptr)
{
    m_dispatcher = CoreApplication::MainView->Dispatcher;

    m_deviceList = ref new Vector<Elevator^>();

    try
    {
        ElevatorDiscovery^ elvDisc = ref new ElevatorDiscovery();
        elvDisc->OnElevatorDiscovered += ref new ElevatorEvent(this, &ElevatorViewModel::ElevatorDiscovered);
        elvDisc->OnElevatorDisconnected += ref new ElevatorEvent(this, &ElevatorViewModel::ElevatorDisconnected);
        m_elevatorDisc = elvDisc;
    }
    catch (Exception^ e)
    {
        Util::ShowErrorMsg(nullptr, e->Message);
    }
}

ElevatorViewModel::~ElevatorViewModel()
{
    if (m_elevatorDisc)
    {
        m_elevatorDisc->StopElevatorDiscovery();
    }
}

void ElevatorViewModel::DiscoverElevators()
{
    if (!m_elevatorDisc->StartElevatorDiscovery())
    {
        Util::ShowErrorMsg(nullptr, "Couldn't Start Elevator Discovery");
    }
}

void ElevatorViewModel::ElevatorDiscovered(Elevator^ elevator)
{
    elevator->OnAuthFailure += ref new ElevatorClientUWP::AuthFailure(this,
        &ElevatorViewModel::AuthFailure);

    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
        [this, elevator]
        {
            unsigned int index;
            if (!m_deviceList->IndexOf(elevator, &index))
            {
                m_deviceList->Append(elevator);
            }
        }
    ));
}

void ElevatorViewModel::ElevatorDisconnected(Elevator^ elevator)
{
    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler(
        [this, elevator]
        {
            unsigned int index;
            if (m_deviceList->IndexOf(elevator, &index))
            {
                m_deviceList->RemoveAt(index);
            }
        }
    ));
}

void ElevatorViewModel::AuthFailure()
{
    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]
    {
        SelectedDevice = nullptr;
    }));
}

void ElevatorViewModel::SetFloor(int floor)
{
    if (m_selectedDevice != nullptr)
    {
        if (!m_selectedDevice->SetTargetFloor(floor))
        {
            Util::ShowErrorMsg(nullptr, "Failed to set the target floor");
        }
    }
}

IVector<Elevator^>^ ElevatorViewModel::DeviceList::get()
{
    return m_deviceList;
}

Elevator^ ElevatorViewModel::SelectedDevice::get()
{
    return m_selectedDevice;
}

void ElevatorViewModel::SelectedDevice::set(Elevator^ value)
{
    if (m_selectedDevice != value)
    {
        // Stop Observation for the previously selected device
        if (m_selectedDevice)
        {
            m_selectedDevice->StopObservation();
        }

        // Set the new device
        m_selectedDevice = value;
        OnPropertyChanged("SelectedDevice");

        // Start Observation for the newly selected device
        if (m_selectedDevice)
        {
            try
            {
                m_selectedDevice->StartObservation();
            }
            catch (Exception^ e)
            {
                Util::ShowErrorMsg(nullptr, e->Message);
            }
        }
    }
}

void ElevatorViewModel::OnPropertyChanged(String^ propertyName)
{
    m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, propertyName]
    {
        PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
    }));
}
