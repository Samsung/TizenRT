// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


/* These APIs require the following menuconfig options enabled:
 *
 * CONFIG_PM_ENABLE=y
 * CONFIG_PM_LIGHT_SLEEP_CALLBACKS=y
 * CONFIG_FREERTOS_LEGACY_HOOKS=y
 * CONFIG_FREERTOS_LEGACY_IDLE_HOOK=y
 * CONFIG_FREERTOS_LEGACY_TICK_HOOK=y
 * CONFIG_FREERTOS_USE_TICKLESS_IDLE=y
 */

#include <stdlib.h>
#include <string.h>
#include <iot_reset.h>

#include <common/bk_include.h>
#include <components/system.h>
#include "reset_reason.h"




#define COMMON_RESET_TAG  "bk-iot-reset"

void iot_reset_reboot( IotResetBootFlag_t xResetBootFlag )
{
    bk_reboot();
}

int32_t iot_reset_shutdown( void )
{
    return IOT_RESET_FUNCTION_NOT_SUPPORTED;
}

int32_t iot_get_reset_reason( IotResetReason_t * xResetReason )
{
    if(xResetReason == NULL){
        BK_LOGE(COMMON_RESET_TAG, "iot_get_reset_reason err,invalid ptr\r\n");
        return IOT_RESET_INVALID_VALUE;
    }
    RESET_SOURCE_STATUS start_type = bk_misc_get_reset_reason();
    switch (start_type) {
    case RESET_SOURCE_POWERON:
        *xResetReason = eResetPowerOnBoot;
    case RESET_SOURCE_REBOOT:
        *xResetReason = eResetWarmBoot;
    case RESET_SOURCE_WATCHDOG:
        *xResetReason = eResetWatchdog;
    default:
        *xResetReason = eResetOther;
    }    
    
    return IOT_RESET_SUCCESS;
}

