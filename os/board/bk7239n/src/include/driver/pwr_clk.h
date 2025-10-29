// Copyright 2020-2025 Beken
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

#include <modules/pm.h>
#include <driver/hal/hal_gpio_types.h>
#include <driver/gpio.h>
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
/**
 * @brief boot cpu1 ok response 
 *
 * boot cpu1 ok response 
 *
 * @attention
 * - This API is used to boot cpu1 ok response 
 *
 * @param
 * - void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_boot_ok_response_set(void);
/**
 * @brief get the cp1 revovery all finish
 *
 * get the cp1 revovery all finish
 *
 * @attention
 * - This API is used to get the cp1 revovery all finish
 *
 * @param
 * - void
 * @return
 * - all module recovery finish
 *
 *
 */
bool bk_pm_cp1_recovery_all_state_get(void);
/**
 * @brief cp1 prepare close module response message
 *
 * cp1 prepare close module response message
 *
 * @attention
 * - This API is used to cp1 prepare close module response message
 *
 * @param
 * - cmd: reponse command
 * - module_name: module name
 * - state: init or finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_recovery_response(uint32_t cmd, pm_cp1_prepare_close_module_name_e module_name,pm_cp1_module_recovery_state_e state);
/**
 * @brief get the work state of cpu1
 *
 * get the work state of cpu1
 *
 * @attention
 * - This API is used to get the work state of cpu1
 *
 * @param
 * - void
 * @return
 * - PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init(cp1 not bootup) ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish(cp1 bootup ok)
 *
 *
 */
pm_mailbox_communication_state_e bk_pm_cp1_work_state_get(void);
/**
 * @brief set the work state of cpu1
 *
 * set the work state of cpu1
 *
 * @attention
 * - This API is used to set the work state of cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_work_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief pm dump the cp1 psram malloc info
 *
 * pm dump the cp1 psram malloc info
 *
 * @attention
 * - This API is used to dump the cp1 psram malloc info
 *
 * @param
 * void
 * @return
 * - cp1 psram malloc count
 * 
 *
 */
bk_err_t bk_pm_dump_cp1_psram_malloc_info(void);
/**
 * @brief pm get the cp1 psram malloc count
 *
 * pm get the cp1 psram malloc count
 *
 * @attention
 * - This API is used to  get the cp1 psram malloc count
 *
 * @param
 * void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
uint32_t bk_pm_get_cp1_psram_malloc_count(void);
/**
 * @brief pm vote gpio ctrl external ldo
 *
 * pm vote power/clk on psram ctrl
 *
 * @attention
 * - This API is used to used to pm vote power/clk on psram ctrl
 *
 * @param
 * -module:gpio ctrl ldo module name;gpio_id:gpio id; value: 0x0:GPIO_OUTPUT_STATE_LOW;0x1:GPIO_OUTPUT_STATE_HIGH
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_module_vote_ctrl_external_ldo(gpio_ctrl_ldo_module_e module,gpio_id_t gpio_id,gpio_output_state_e value);
/**
 * @brief get cpu1 power ctrl state
 *
 * get cpu1 power ctrl state
 *
 * @attention
 * - This API is used to get cpu1 power ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_pwr_ctrl_state_get(void);
/**
 * @brief set power control state in cpu1
 *
 * set power control state in cpu1
 *
 * @attention
 * - This API is used to set power control state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_pwr_ctrl_state_set(pm_mailbox_communication_state_e state);

/**
 * @brief get cpu1 clock ctrl state
 *
 * get cpu1 clock ctrl state
 *
 * @attention
 * - This API is used to get cpu1 clock ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_clk_ctrl_state_get(void);

/**
 * @brief set clock control state in cpu1
 *
 * set clock control state in cpu1
 *
 * @attention
 * - This API is used to set clock control state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_clk_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 vote sleep ctrl state
 *
 * get cpu1 vote sleep ctrl state
 *
 * @attention
 * - This API is used to get cpu1 vote sleep ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_sleep_ctrl_state_get(void);
/**
 * @brief set vote sleep state in cpu1
 *
 * set vote sleep state in cpu1
 *
 * @attention
 * - This API is used to set vote sleep state in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_sleep_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 vote cpu freq ctrl state
 *
 * get cpu1 vote cpu freq  ctrl state
 *
 * @attention
 * - This API is used to get cpu1 vote cpu freq  ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_cpu_freq_ctrl_state_get(void);
/**
 * @brief set vote cpu frequency in cpu1
 *
 * set vote cpu frequency in cpu1
 *
 * @attention
 * - This API is used to set vote cpu frequency in cpu1
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_cpu_freq_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief get cpu1 boot ctrl state
 *
 * get cpu1 boot ctrl state
 *
 * @attention
 * - This API is used to get cpu1 boot ctrl state
 *
 * @param
 * -void
 * @return
 * - cpu1 power ctrl state(PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish)
 */
pm_mailbox_communication_state_e bk_pm_cp1_ctrl_state_get(void);
/**
 * @brief set bootup cpu1 state
 *
 * set bootup cpu1 state
 *
 * @attention
 * - This API is used to set bootup cpu1 state
 *
 * @param
 * -PM_MAILBOX_COMMUNICATION_INIT:mailbox communication init ;PM_MAILBOX_COMMUNICATION_FINISH:mailbox communication finish
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_cp1_ctrl_state_set(pm_mailbox_communication_state_e state);
/**
 * @brief cpu1 response info through mailbox
 *
 * cpu1 response info through mailbox
 *
 * @attention
 * - This API is used cpu1 response info through mailbox
 *
 * @param
 * -cmd: command ;ret:return value
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t pm_cp1_mailbox_response(uint32_t cmd, int ret);
/**
 * @brief pm vote power/clk on psram ctrl
 *
 * pm vote power/clk on psram ctrl
 *
 * @attention
 * - This API is used to used to pm vote power/clk on psram ctrl
 *
 * @param
 * -module:power/clk on psram module name;power_state:PM_POWER_MODULE_STATE_ON;PM_POWER_MODULE_STATE_OFF
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_module_vote_psram_ctrl(pm_power_psram_module_name_e module,pm_power_module_state_e power_state);
/**
 * @brief pm vote boot cp1 ctrl
 *
 * pm vote boot cp1 ctrl
 *
 * @attention
 * - This API is used to used to pm vote boot cp1 ctrl
 *
 * @param
 * -module:boot cp1 module name;power_state:PM_POWER_MODULE_STATE_ON;PM_POWER_MODULE_STATE_OFF
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_module_vote_boot_cp1_ctrl(pm_boot_cp1_module_name_e module,pm_power_module_state_e power_state);
/**
 * @brief pm vote boot cp2 ctrl
 *
 * pm vote boot cp2 ctrl
 *
 * @attention
 * - This API is used to used to pm vote boot cp2 ctrl
 *
 * @param
 * -module:boot cp2 module name;power_state:PM_POWER_MODULE_STATE_ON;PM_POWER_MODULE_STATE_OFF
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_module_vote_boot_cp2_ctrl(pm_boot_cp2_module_name_e module,pm_power_module_state_e power_state);

/**
 * @brief pm mailbox init
 *
 * pm mailbox init
 *
 * @attention
 * - This API is used to init pm mailbox init
 *
 * @param
 * -void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 *
 */
bk_err_t bk_pm_mailbox_init(void);

/**
 * @brief   get the 32k clock source configured by the customer
 *
 * This API get the 32k clock source configured by the customer
 *
 * @return
 *    - PM_LPO_SRC_X32K: support the external 32k clock
 *    - PM_LPO_SRC_ROSC: default support the ROSC
 */
pm_lpo_src_e bk_clk_32k_customer_config_get(void);


#ifdef __cplusplus
}
#endif


