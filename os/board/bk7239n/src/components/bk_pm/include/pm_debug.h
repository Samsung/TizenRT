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

#include <common/bk_include.h>

#define TAG "pm"
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

/****************************************************************************
 * Name: pm_debug_dump
 *
 * Description:
 *   Dump all power management debug information.
 *   Displays comprehensive PM status including power, clock, and module states.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_dump(void);

/****************************************************************************
 * Name: pm_core_dump
 *
 * Description:
 *   Dump core power management information.
 *   Displays CPU core states, clock frequencies, and voltage levels.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_core_dump(void);

/****************************************************************************
 * Name: pm_power_dump
 *
 * Description:
 *   Dump power domain status information.
 *   Shows which power domains are active, idle, or powered down.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_power_dump(void);

/****************************************************************************
 * Name: pm_power_modules_dump
 *
 * Description:
 *   Dump power status of all modules in the system.
 *   Lists each module's power state and activity status.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_power_modules_dump(void);

/****************************************************************************
 * Name: pm_debug_lowvol_sleep_hook
 *
 * Description:
 *   Debug hook function called before entering low voltage sleep mode.
 *   Used for debugging and logging sleep entry process.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_lowvol_sleep_hook(void);

/****************************************************************************
 * Name: pm_debug_low_vol_wakeup_hook
 *
 * Description:
 *   Debug hook function called after waking up from low voltage mode.
 *   Used for debugging and logging wakeup process.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_low_vol_wakeup_hook(void);

/****************************************************************************
 * Name: pm_debug_mode
 *
 * Description:
 *   Get the current power management debug mode status.
 *
 * Returns:
 *   Debug mode value (1=enabled, 0=disabled)
 *
 ****************************************************************************/
uint32_t pm_debug_mode(void);

/****************************************************************************
 * Name: pm_debug_ctrl
 *
 * Description:
 *   Control (enable/disable) power management debug mode.
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
 * Name: pm_power_modules_dump_with_sleep_mode
 *
 * Description:
 *   Dump power module states for a specific sleep mode.
 *   Shows which modules will be active/suspended in the given sleep mode.
 *
 * Input Parameters:
 *   sleep_mode - The sleep mode to query
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_power_modules_dump_with_sleep_mode(pm_sleep_mode_e sleep_mode);

// TODO: rename or remove it
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
 * Name: pm_debug_psram
 *
 * Description:
 *   Dump PSRAM (Pseudo-Static RAM) debug information.
 *   Displays PSRAM configuration, usage, and power state.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_psram(void);

/****************************************************************************
 * Name: pm_debug_psram_state
 *
 * Description:
 *   Dump PSRAM current state information.
 *   Shows PSRAM power state, access mode, and operational status.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_debug_psram_state(void);

/****************************************************************************
 * Name: pm_sleep_mode_to_string
 *
 * Description:
 *   Convert sleep mode enumeration value to human-readable string.
 *   Useful for debug logging and display purposes.
 *
 * Input Parameters:
 *   sleep_mode - The sleep mode to convert
 *
 * Returns:
 *   Pointer to constant string representing the sleep mode name
 *
 ****************************************************************************/
const char *pm_sleep_mode_to_string(pm_sleep_mode_e sleep_mode);