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

//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace ElevatorClientUWP;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Core;

MainPage::MainPage()
{
	InitializeComponent();
    ViewModel = ref new ElevatorViewModel();
    DataContext = ViewModel;
    this->Loaded += ref new RoutedEventHandler(this, &MainPage::OnLoaded);
}

void MainPage::setFloorBtn_Click(Object^ sender, RoutedEventArgs^ e)
{
    int floor = 0;
    try
    {
        floor = std::stoi(setFloorText->Text->Data());
    }
    catch (std::exception e)
    {
        Util::ShowErrorMsg(nullptr, "Invalid Floor Number");
    }
    ViewModel->SetFloor(floor);
}

void MainPage::Elevators_SelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{
    setFloorText->Text = "";
}

void ElevatorClientUWP::MainPage::OnLoaded(Object ^sender, RoutedEventArgs ^e)
{
    ViewModel->DiscoverElevators();
}
