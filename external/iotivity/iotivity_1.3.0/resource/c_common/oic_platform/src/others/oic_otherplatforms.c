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
#include "iotivity_config.h"
#include "iotivity_debug.h"
#include "logger.h"
#include "oic_platform.h"

#define TAG "OIC_PLATFORM"

OICPlatformResult_t OICGetLocalAppDataPath(char* buffer, size_t* bufferLen)
{
    OIC_LOG(INFO, TAG, "OICGetLocalAppDataPath: Unsupported platform.");
    OC_UNUSED(buffer);
    OC_UNUSED(bufferLen);
    return OIC_PLATFORM_NOTIMPL;
}

OICPlatformResult_t OICGetTempAppDataPath(char* buffer, size_t* bufferLen)
{
    OIC_LOG(INFO, TAG, "OICGetTempAppDataPath: Unsupported platform.");
    OC_UNUSED(buffer);
    OC_UNUSED(bufferLen);
    return OIC_PLATFORM_NOTIMPL;
}
