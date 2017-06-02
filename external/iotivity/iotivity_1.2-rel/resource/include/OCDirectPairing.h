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

#ifndef OC_DIRECT_PAIRING_H_
#define OC_DIRECT_PAIRING_H_
#include <OCApi.h>

namespace OC
{
    class OCDirectPairing
    {
        public:
            OCDirectPairing(OCDPDev_t *ptr);
            /**
             * Function to get the host address of direct pairing device.
             *
             * @return Returns host address in the format
             *           <coaps>:IP:securePort
             */
            std::string getHost();

            /**
             * Function to get the device ID of the direct pairing device.
             *
             * @return Returns device ID (UUID)
             */
            std::string getDeviceID();

            /**
             * Function to get the pairing methods supported by direct pairing device.
             *
             * @return Returns vector of pairing methods supported.
             *   DP_NOT_ALLOWED
             *   DP_PRE_CONFIGURED
             *   DP_RANDOM_PIN
             */
            std::vector<OCPrm_t> getPairingMethods();

            /**
             * Function to get the connectivity Type.
             *
             * @return Returns connectivity Type
             */
            OCConnectivityType getConnType();

            OCDPDev_t* getDev();

        private:
            OCDPDev_t *m_devPtr;
    };
}
#endif //OC_DIRECT_PAIRING_H_
