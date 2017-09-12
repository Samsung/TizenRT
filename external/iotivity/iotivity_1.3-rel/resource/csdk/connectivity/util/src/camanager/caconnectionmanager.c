/* *****************************************************************
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caadapterutils.h"
#include "cainterface.h"
#include "camessagehandler.h"
#include "caremotehandler.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "octhread.h"
#include "logger.h"
#include "caadapterutils.h"

#include "caconnectionmanager.h"
#include "camessagearbiter.h"
#include "capolicymanager.h"
#include "camanagerutil.h"

#define TAG "OIC_CM"


CAResult_t CACMInitialize()
{
    OIC_LOG(DEBUG, TAG, "CACMInitialize");
    return CAMsgArbiterInitialize();
}

CAResult_t CACMTerminate()
{
    OIC_LOG(DEBUG, TAG, "CACMTerminate");

    return CAMsgArbiterTerminate();
}

CAResult_t CACMSetConnUserConfig(CAConnectUserPref_t connPrefer)
{
    OIC_LOG(DEBUG, TAG, "CACMSetConnUserConfig");

    return CAPolicyMgrSetConfiguration(connPrefer);
}

CAResult_t CACMGetConnUserConfig(CAConnectUserPref_t *connPrefer)
{
    OIC_LOG(DEBUG, TAG, "CACMGetConnUserConfig");
    return CAPolicyMgrGetConfigure(connPrefer);
}

CAResult_t CACMGetMessageData(CAData_t *data)
{
    OIC_LOG(DEBUG, TAG, "CACMGetMessageData");

    VERIFY_NON_NULL(data, TAG, "data is null");

    return CAMsgArbiterGetMessageData(data);
}

CAResult_t CACMUpdateRemoteDeviceInfo(const CAEndpoint_t *endpoint, bool isCloud)
{
    return CAMsgArbiterUpdateDeviceInfo(endpoint, isCloud);
}

CAResult_t CACMResetRemoteDeviceInfo()
{
    return CAMsgArbiterResetDeviceInfo();
}
