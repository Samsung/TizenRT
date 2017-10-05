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

#ifndef _ES_MEDIATOR_SIMULATOR_H_
#define _ES_MEDIATOR_SIMULATOR_H_

#include <iostream>

#include "OCPlatform.h"
#include "OCApi.h"
#include "oic_malloc.h"

#include "escommon.h"
#include "ESEnrolleeCommon.h"
#include "easysetup.h"

class ESEnrolleeSimulator
{
public:
    ESEnrolleeSimulator()
    {
    };
    ~ESEnrolleeSimulator() = default;

    ESEnrolleeSimulator(const ESEnrolleeSimulator &) = delete;
    ESEnrolleeSimulator & operator = (const ESEnrolleeSimulator &) = delete;

    ESEnrolleeSimulator(ESEnrolleeSimulator &&) = delete;
    ESEnrolleeSimulator & operator = (ESEnrolleeSimulator &&) = delete;

    ESResult initEnrollee()
    {
        ESResourceMask resourcemMask = (ESResourceMask)(ES_WIFICONF_RESOURCE |
                                                    ES_COAPCLOUDCONF_RESOURCE |
                                                    ES_DEVCONF_RESOURCE);
        ESProvisioningCallbacks callbacks;

        callbacks.WiFiConfProvCb = &onWiFiProvisioning;
        callbacks.DevConfProvCb = &onDevConfProvisioning;
        callbacks.CoapCloudConfProvCb = &onCloudProvisioning;

        return ESInitEnrollee(false, resourcemMask, callbacks);
    }

    ESResult setDeviceProperty()
    {
        ESDeviceProperty deviceProperty = {
            {{WIFI_11G, WiFi_EOF}, WIFI_5G}, {"Test Device", "Test Model Number"}
        };

        return ESSetDeviceProperty(&deviceProperty);
    }

    ESResult setESState()
    {
        return ESSetState(ES_STATE_CONNECTED_TO_ENROLLER);
    }

    ESResult setESErrorCode()
    {
        return ESSetErrorCode(ES_ERRCODE_NO_INTERNETCONNECTION);
    }

private:
    static void onWiFiProvisioning(ESWiFiConfData* /*data*/)
    {
    }

    static void onDevConfProvisioning(ESDevConfData* /*data*/)
    {
    }

    static void onCloudProvisioning(ESCoapCloudConfData* /*data*/)
    {
    }
};

#endif //_NS_CONSUMER_SIMULATOR_H_
