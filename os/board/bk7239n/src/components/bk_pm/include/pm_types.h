// Copyright 2021-2025 Beken
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
#pragma once

#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PM_POWER_CTRL_CMD          			 (0x1)
#define PM_CLK_CTRL_CMD            			 (0x2)
#define PM_SLEEP_CTRL_CMD          			 (0x3)
#define PM_CPU_FREQ_CTRL_CMD       			 (0x4)
#define PM_CPU1_BOOT_READY_CMD     			 (0x5)
#define PM_CTRL_EXTERNAL_LDO_CMD   			 (0x6)
#define PM_CTRL_PSRAM_POWER_CMD    			 (0x7)
#define PM_CP1_PSRAM_MALLOC_STATE_CMD        (0x8)
#define PM_CP1_DUMP_PSRAM_MALLOC_INFO_CMD    (0x9)
#define PM_CP1_RECOVERY_CMD                  (0xa)


#define PM_AON_RTC_DEFAULT_TICK_COUNT        (32)//only for cp1 using aon rtc
typedef enum
{
	PM_BOOT_CP1_MODULE_NAME_FFT          = 0,
	PM_BOOT_CP1_MODULE_NAME_AUDP_SBC        ,// 1
	PM_BOOT_CP1_MODULE_NAME_AUDP_AUDIO      ,// 2
	PM_BOOT_CP1_MODULE_NAME_AUDP_I2S        ,// 3
	PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_EN    ,// 4
	PM_BOOT_CP1_MODULE_NAME_VIDP_JPEG_DE    ,// 5
	PM_BOOT_CP1_MODULE_NAME_VIDP_DMA2D      ,// 6
	PM_BOOT_CP1_MODULE_NAME_VIDP_LCD        ,// 7
	PM_BOOT_CP1_MODULE_NAME_MULTIMEDIA      ,// 8
	PM_BOOT_CP1_MODULE_NAME_APP             ,// 9
	PM_BOOT_CP1_MODULE_NAME_VIDP_ROTATE     ,// 10
	PM_BOOT_CP1_MODULE_NAME_VIDP_SCALE      ,// 11
	PM_BOOT_CP1_MODULE_NAME_GET_MEDIA_MSG   ,// 12
	PM_BOOT_CP1_MODULE_NAME_LVGL            ,// 13
	PM_BOOT_CP1_MODULE_NAME_USER            ,// 14
	PM_BOOT_CP1_MODULE_NAME_MAX             ,// attention: MAX value can not exceed 31.
}pm_boot_cp1_module_name_e;

typedef enum
{
	PM_BOOT_CP2_MODULE_NAME_FFT          = 0,
	PM_BOOT_CP2_MODULE_NAME_AUDP_SBC        ,// 1
	PM_BOOT_CP2_MODULE_NAME_AUDP_AUDIO      ,// 2
	PM_BOOT_CP2_MODULE_NAME_AUDP_I2S        ,// 3
	PM_BOOT_CP2_MODULE_NAME_VIDP_JPEG_EN    ,// 4
	PM_BOOT_CP2_MODULE_NAME_VIDP_JPEG_DE    ,// 5
	PM_BOOT_CP2_MODULE_NAME_VIDP_DMA2D      ,// 6
	PM_BOOT_CP2_MODULE_NAME_VIDP_LCD        ,// 7
	PM_BOOT_CP2_MODULE_NAME_APP             ,// 8
	PM_BOOT_CP2_MODULE_NAME_MAX             ,// attention: MAX value can not exceed 31.
}pm_boot_cp2_module_name_e;

typedef enum
{
	PM_POWER_PSRAM_MODULE_NAME_FFT       = 0,
	PM_POWER_PSRAM_MODULE_NAME_AUDP_SBC     ,// 1
	PM_POWER_PSRAM_MODULE_NAME_AUDP_AUDIO   ,// 2
	PM_POWER_PSRAM_MODULE_NAME_AUDP_I2S     ,// 3
	PM_POWER_PSRAM_MODULE_NAME_VIDP_JPEG_EN ,// 4
	PM_POWER_PSRAM_MODULE_NAME_VIDP_H264_EN ,// 5
	PM_POWER_PSRAM_MODULE_NAME_VIDP_JPEG_DE ,// 6
	PM_POWER_PSRAM_MODULE_NAME_VIDP_DMA2D   ,// 7
	PM_POWER_PSRAM_MODULE_NAME_VIDP_LCD     ,// 8
	PM_POWER_PSRAM_MODULE_NAME_APP          ,// 9
	PM_POWER_PSRAM_MODULE_NAME_AS_MEM       ,// 10
	PM_POWER_PSRAM_MODULE_NAME_CPU1         ,// 11
	PM_POWER_PSRAM_MODULE_NAME_MEDIA        ,// 12
	PM_POWER_PSRAM_MODULE_NAME_LVGL_CODE_RUN,// 13
	PM_POWER_PSRAM_MODULE_NAME_MAX          ,// attention: MAX value can not exceed 31.
}pm_power_psram_module_name_e;

typedef enum
{
	PM_MAILBOX_COMMUNICATION_INIT      = 0,
	PM_MAILBOX_COMMUNICATION_FINISH    = 1,
}pm_mailbox_communication_state_e;

typedef enum
{
	PM_CP1_MODULE_RECOVERY_STATE_INIT      = 0,
	PM_CP1_MODULE_RECOVERY_STATE_FINISH    = 1,
}pm_cp1_module_recovery_state_e;
typedef enum
{
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_FFT       = 0,
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_AUDP_SBC     ,// 1
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_AUDP_AUDIO   ,// 2
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_AUDP_I2S     ,// 3
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_VIDP_JPEG_EN ,// 4
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_VIDP_H264_EN ,// 5
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_VIDP_JPEG_DE ,// 6
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_VIDP_DMA2D   ,// 7
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_VIDP_LCD     ,// 8
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_APP          ,// 9
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_AS_MEM       ,// 10
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_CPU1         ,// 11
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_MEDIA        ,// 12
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_LVGL_CODE_RUN,// 13
	PM_CP1_PREPARE_CLOSE_MODULE_NAME_MAX          ,// attention: MAX value can not exceed 31.
}pm_cp1_prepare_close_module_name_e;

#ifdef __cplusplus
}
#endif
