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

#include "ipca.h"
#include "elevatorserver.h"

ElevatorServer g_testElevator;

int main()
{
    std::string elevatorName = "Standalone Test Elevator";
    g_testElevator.Start(elevatorName);

    std::cout << "enter any key and press enter to exit" << std::endl;
    int userInput;
    std::cin >> userInput;

    g_testElevator.Stop();
}
