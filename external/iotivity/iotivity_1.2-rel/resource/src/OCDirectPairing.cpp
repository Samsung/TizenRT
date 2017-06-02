//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "OCDirectPairing.h"
#include <iomanip>

namespace OC
{
    static const char COAP[] = "coap://";
    static const char COAPS[] = "coaps://";
    static const int UUID_LENGTH = (128/8); //UUID length

    OCDirectPairing::OCDirectPairing(OCDPDev_t *ptr):m_devPtr(ptr)
    {
    }

    std::string OCDirectPairing::getHost()
    {
        bool ipv6 = false;
        std::ostringstream host("");
        if (m_devPtr->connType & CT_IP_USE_V6)
        {
            ipv6 = true;
        }

        host << COAPS << (ipv6?"[":"") << m_devPtr->endpoint.addr;
        host << (ipv6?"]:":":") << m_devPtr->securePort;

        return host.str();
    }

    std::string OCDirectPairing::getDeviceID()
    {
        std::ostringstream deviceId("");

        for (int i = 0; i < UUID_LENGTH; i++)
        {
            if (i == 4 || i == 6 || i == 8 || i == 10)
            {
                deviceId << '-';
            }
            deviceId << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned>(m_devPtr->deviceID.id[i]);
        }

        return deviceId.str();
    }

    std::vector<OCPrm_t> OCDirectPairing::getPairingMethods()
    {
        std::vector<OCPrm_t> prms;

        for (size_t i = 0; i < m_devPtr->prmLen; i++)
        {
            prms.push_back(m_devPtr->prm[i]);
        }
        return prms;
    }

    OCConnectivityType OCDirectPairing::getConnType()
    {
        return m_devPtr->connType;
    }

    OCDPDev_t* OCDirectPairing::getDev()
    {
        return m_devPtr;
    }
}
