/******************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
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

/**
 * @file oc_interface_details.h
 *
 * @brief This file provides OIC definded interfaces and thier supported method types.
 */

#ifndef SIMULATOR_INTERFACE_DETAILS_H__
#define SIMULATOR_INTERFACE_DETAILS_H__

#include <iostream>
#include <unordered_map>
#include <vector>

#include "OCApi.h"

namespace OC
{
    // Read-Only interface
    const std::string READ_INTERFACE = "oic.if.r";

    // Read-Write Interface
    const std::string READWRITE_INTERFACE = "oic.if.rw";

    // Actuator Interface
    const std::string ACTUATOR_INTERFACE = "oic.if.a";

    // Sensor Interface
    const std::string SENSOR_INTERFACE = "oic.if.s";
}

class OCInterfaceDetails
{
    public:
        static OCInterfaceDetails *getInstance();
        bool isInterface(const std::string &interfaceType);
        bool isRequestSupported(const std::string &interfaceType, const std::string &requestType);

    private:
        OCInterfaceDetails();
        ~OCInterfaceDetails() = default;

        std::unordered_map<std::string, std::vector<std::string>> m_interfaces;
};

#endif
