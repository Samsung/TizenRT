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

#include "oc_interface_details.h"

OCInterfaceDetails::OCInterfaceDetails()
{
    m_interfaces[OC::DEFAULT_INTERFACE] = {"GET", "PUT", "POST"};
    m_interfaces[OC::READ_INTERFACE] = {"GET"};
    m_interfaces[OC::READWRITE_INTERFACE] = {"GET", "PUT", "POST"};
    m_interfaces[OC::ACTUATOR_INTERFACE] = {"GET", "PUT", "POST"};
    m_interfaces[OC::SENSOR_INTERFACE] = {"GET"};
}

OCInterfaceDetails *OCInterfaceDetails::getInstance()
{
    static OCInterfaceDetails instance;
    return &instance;
}

bool OCInterfaceDetails::isInterface(const std::string &interfaceType)
{
    if (m_interfaces.end() != m_interfaces.find(interfaceType))
        return true;
    return false;
}

bool OCInterfaceDetails::isRequestSupported(const std::string &interfaceType,
        const std::string &requestType)
{
    if (m_interfaces.end() != m_interfaces.find(interfaceType))
    {
        std::vector<std::string> requests = m_interfaces[interfaceType];
        if (requests.end() != std::find(requests.begin(), requests.end(), requestType))
            return true;
    }

    return false;
}

