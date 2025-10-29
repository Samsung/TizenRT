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

#ifndef _BK_PM_INTERNAL_API_H_
#define _BK_PM_INTERNAL_API_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <modules/pm.h>
#if CONFIG_PM_LV_WDT_PROTECTION
#include <components/event.h>
#include "modules/wifi_types.h"
#endif

typedef enum
{
	PM_LV_WAKEUP_STEP_0 = 0,
	PM_LV_WAKEUP_STEP_1,
	PM_LV_WAKEUP_STEP_2,
	PM_LV_WAKEUP_STEP_3,
	PM_LV_WAKEUP_STEP_4,
	PM_LV_WAKEUP_STEP_5,
	PM_LV_ENTER_STEP_0,
	PM_LV_ENTER_STEP_1,
	PM_LV_ENTER_STEP_2,
	PM_LV_ENTER_STEP_3,
	PM_LV_ENTER_STEP_4,
	PM_LV_ENTER_STEP_5,
	PM_LV_STEP_MAX
}pm_lv_step_e;

typedef enum
{
	PM_LV_MODE_ENTER = 0,
	PM_LV_MODE_WAKEUP,
	PM_LV_MODE_MAX
}pm_lv_mode_e;

#define PM_DEBUG_CTRL_STATE                    (8)

/****************************************************************************
 * Name: pm_disable_int
 *
 * Description:
 *   Disable all interrupts globally by setting the PRIMASK register.
 *   This function is typically used in power management to create a
 *   critical section before entering low power modes.
 *
 * Returns:
 *   The previous interrupt status (PRIMASK value) before disabling.
 *   This value should be saved and passed to pm_enable_int() to restore
 *   the original interrupt state.
 *
 ****************************************************************************/
uint32_t pm_disable_int(void);

/****************************************************************************
 * Name: pm_get_int_status
 *
 * Description:
 *   Get the current interrupt status without changing it.
 *   Returns the current value of PRIMASK register.
 *   - 0: interrupts are enabled
 *   - 1: interrupts are disabled
 *
 * Returns:
 *   Current interrupt status (0=enabled, 1=disabled)
 *
 ****************************************************************************/
uint32_t pm_get_int_status(void);

/****************************************************************************
 * Name: pm_enable_int
 *
 * Description:
 *   Restore the interrupt state by setting the PRIMASK register to the
 *   specified level. This function is typically used in power management
 *   to exit a critical section after waking from low power modes.
 *
 * Input Parameters:
 *   int_level - The interrupt level to restore, typically the value
 *               returned by a previous call to pm_disable_int().
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_enable_int(uint32_t int_level);

/****************************************************************************
 * Name: pm_hardware_init
 *
 * Description:
 *   Initialize power management hardware components and peripherals.
 *   This function sets up the hardware required for PM operations.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_hardware_init(void);

/****************************************************************************
 * Name: pm_core_bus_clock_ctrl
 *
 * Description:
 *   Control the core and bus clock frequencies by configuring clock
 *   selection and division parameters for the system.
 *
 * Input Parameters:
 *   cksel_core - Core clock selection
 *   ckdiv_core - Core clock divider value
 *   ckdiv_bus  - Bus clock divider value
 *   ckdiv_cpu0 - CPU0 clock divider value
 *   ckdiv_cpu1 - CPU1 clock divider value
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_core_bus_clock_ctrl(uint32_t cksel_core, uint32_t ckdiv_core,uint32_t ckdiv_bus, uint32_t ckdiv_cpu0,uint32_t ckdiv_cpu1);

/****************************************************************************
 * Name: pm_debug_ctrl
 *
 * Description:
 *   Enable or disable power management debug functionality.
 *
 * Input Parameters:
 *   debug_en - Debug enable flag (1=enable, 0=disable)
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_ctrl(uint32_t debug_en);

/****************************************************************************
 * Name: pm_debug_pwr_clk_state
 *
 * Description:
 *   Display debug information about current power and clock states.
 *   This function prints the status of power domains and clock settings.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_debug_pwr_clk_state(void);

/****************************************************************************
 * Name: pm_debug_lv_state
 *
 * Description:
 *   Display debug information about current low voltage (LV) mode state.
 *   Shows the status of low power mode operations.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_debug_lv_state(void);

/****************************************************************************
 * Name: bk_pm_low_vol_vote_state_get
 *
 * Description:
 *   Get the current voting state for low voltage mode.
 *   Returns a bitmap of modules that have voted for low voltage operation.
 *
 * Returns:
 *   Voting state bitmap
 *
 ****************************************************************************/
uint32_t bk_pm_low_vol_vote_state_get(void);

/****************************************************************************
 * Name: pm_debug_module_state
 *
 * Description:
 *   Display debug information about the state of all PM-related modules.
 *   Shows which modules are active, suspended, or in low power mode.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_debug_module_state(void);

/****************************************************************************
 * Name: pm_cp1_psram_malloc_count_state_set
 *
 * Description:
 *   Set the PSRAM malloc count state for CP1 (Co-Processor 1).
 *   Used to track memory allocation state for power management decisions.
 *
 * Input Parameters:
 *   value - The count value to set
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_cp1_psram_malloc_count_state_set(uint32_t value);

/****************************************************************************
 * Name: bk_pm_module_check_cp1_shutdown
 *
 * Description:
 *   Check if CP1 (Co-Processor 1) can be safely shut down.
 *   Verifies that all modules are ready for CP1 power-down.
 *
 * Returns:
 *   BK_OK if CP1 can be shut down, or error code if not ready
 *
 ****************************************************************************/
bk_err_t bk_pm_module_check_cp1_shutdown(void);

/****************************************************************************
 * Name: pm_printf_current_temperature
 *
 * Description:
 *   Print the current system temperature reading.
 *   Used for thermal monitoring and power management decisions.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_printf_current_temperature(void);

/****************************************************************************
 * Name: pm_cp1_aon_rtc_counter_get
 *
 * Description:
 *   Get the current RTC counter value from CP1's Always-On (AON) domain.
 *   The AON RTC continues running even in deep sleep modes.
 *
 * Returns:
 *   Current RTC counter value (64-bit)
 *
 ****************************************************************************/
uint64_t pm_cp1_aon_rtc_counter_get(void);

/****************************************************************************
 * Name: bk_pm_cpu_freq_dump
 *
 * Description:
 *   Dump (display) the current CPU frequency settings and configuration.
 *   Shows clock source, dividers, and actual operating frequencies.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t bk_pm_cpu_freq_dump(void);

#if CONFIG_PM_LV_TIME_COST_DEBUG
/****************************************************************************
 * Name: pm_lv_rtc_interval_get
 *
 * Description:
 *   Get the RTC time interval for a specific low voltage step.
 *   Used for measuring time costs during low power mode transitions.
 *
 * Input Parameters:
 *   step - The low voltage step to query
 *
 * Returns:
 *   RTC interval in ticks (64-bit)
 *
 ****************************************************************************/
__attribute__((section(".iram")))  uint64_t pm_lv_rtc_interval_get(pm_lv_step_e step);

/****************************************************************************
 * Name: pm_lv_rtc_tick_get
 *
 * Description:
 *   Get the RTC tick value recorded at a specific low voltage step.
 *   Used for debugging and profiling low power mode timing.
 *
 * Input Parameters:
 *   step - The low voltage step to query
 *
 * Returns:
 *   RTC tick value (64-bit)
 *
 ****************************************************************************/
__attribute__((section(".iram")))  uint64_t pm_lv_rtc_tick_get(pm_lv_step_e step);

/****************************************************************************
 * Name: pm_lv_rtc_tick_set
 *
 * Description:
 *   Set the RTC tick value for a specific low voltage step.
 *   Records timing information for performance analysis.
 *
 * Input Parameters:
 *   step - The low voltage step to set
 *   tick - The RTC tick value to record
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
__attribute__((section(".itcm_sec_code")))  bk_err_t pm_lv_rtc_tick_set(pm_lv_step_e step,uint64_t tick);

/****************************************************************************
 * Name: pm_enter_lv_rtc_tick_clear
 *
 * Description:
 *   Clear all RTC tick records for low voltage entry steps.
 *   Resets timing measurements before entering low power mode.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
__attribute__((section(".iram")))  bk_err_t pm_enter_lv_rtc_tick_clear();

/****************************************************************************
 * Name: pm_wakeup_lv_rtc_tick_clear
 *
 * Description:
 *   Clear all RTC tick records for low voltage wakeup steps.
 *   Resets timing measurements after waking from low power mode.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
__attribute__((section(".iram")))  bk_err_t pm_wakeup_lv_rtc_tick_clear();

/****************************************************************************
 * Name: pm_rtc_cur_tick_get
 *
 * Description:
 *   Get the current RTC tick value.
 *   Provides high-resolution timestamp for timing measurements.
 *
 * Returns:
 *   Current RTC tick value (64-bit)
 *
 ****************************************************************************/
__attribute__((section(".iram")))  uint64_t pm_rtc_cur_tick_get();
#endif

#if CONFIG_PM_LV_WDT_PROTECTION
/****************************************************************************
 * Name: pm_wifi_event_init
 *
 * Description:
 *   Initialize WiFi event monitoring for power management watchdog protection.
 *   Sets up event handlers to prevent system hang during low power transitions.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_wifi_event_init();

/****************************************************************************
 * Name: bk_pm_wifi_event_state
 *
 * Description:
 *   Get the current WiFi event state for power management monitoring.
 *   Used by watchdog protection to track WiFi activity.
 *
 * Returns:
 *   Current WiFi event state
 *
 ****************************************************************************/
wifi_event_t bk_pm_wifi_event_state();
#endif

#ifdef __cplusplus
}
#endif

#endif
