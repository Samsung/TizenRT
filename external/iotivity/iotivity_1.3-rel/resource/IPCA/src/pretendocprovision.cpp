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

#include "ipcainternal.h"

#include "ocstack.h"
#include "srmutility.h"
#include "base64.h"

#include <thread>

#include "pinoxmcommon.h"
#include "srmutility.h"
#include "common.h"

// These APIs are used when building with SECURED=0.
OCStackResult OCSecure::provisionInit(const std::string& dbPath)
{
    OC_UNUSED(dbPath);
    return OC_STACK_OK;
}

OCStackResult OCSecure::provisionClose()
{
    return OC_STACK_OK;
}

OCStackResult OCSecure::registerInputPinCallback(InputPinCB inputPinCB,
                            InputPinCallbackHandle* inputPinCallbackHandle)
{
    OC_UNUSED(inputPinCB);
    OC_UNUSED(inputPinCallbackHandle);
    return OC_STACK_OK;
}

OCStackResult OCSecure::deregisterInputPinCallback(InputPinCallbackHandle inputPinCallbackHandle)
{
    OC_UNUSED(inputPinCallbackHandle);
    return OC_STACK_OK;
}

OCStackResult OCSecure::registerDisplayPinCallback(DisplayPinCB displayPinCB,
                            DisplayPinCallbackHandle* displayPinCallbackHandle)
{
    OC_UNUSED(displayPinCB);
    OC_UNUSED(displayPinCallbackHandle);
    return OC_STACK_OK;
}

OCStackResult OCSecure::deregisterDisplayPinCallback(
                DisplayPinCallbackHandle displayPinCallbackHandle)
{
    OC_UNUSED(displayPinCallbackHandle);
    return OC_STACK_OK;
}

OicSecOxm_t OCSecureResource::getSelectedOwnershipTransferMethod()
{
    return OIC_JUST_WORKS;
}

#ifdef MULTIPLE_OWNER
OCStackResult OCSecure::discoverMultipleOwnerEnabledDevices(unsigned short timeout,
                                                            DeviceList_t &list)
{
    OC_UNUSED(timeout);
    OC_UNUSED(list);
    return OC_STACK_OK;
}

OCStackResult OCSecure::discoverMultipleOwnerEnabledDevice(unsigned short timeout,
                            const OicUuid_t* deviceID,
                            std::shared_ptr<OCSecureResource> &foundDevice)
{
    OC_UNUSED(timeout);
    OC_UNUSED(deviceID);
    OC_UNUSED(foundDevice);
    return OC_STACK_OK;
}

OCStackResult OCSecureResource::addPreconfigPIN(const char* preconfPIN, size_t preconfPINLength)
{
    OC_UNUSED(preconfPIN);
    OC_UNUSED(preconfPINLength);
    return OC_STACK_OK;
}

OCStackResult OCSecureResource::doMultipleOwnershipTransfer(ResultCallBack resultCallback)
{
    OC_UNUSED(resultCallback);
    return OC_STACK_OK;
}

OCStackResult OCSecureResource::isSubownerOfDevice(bool* subowner)
{
    OC_UNUSED(subowner);
    return OC_STACK_OK;
}
#endif
