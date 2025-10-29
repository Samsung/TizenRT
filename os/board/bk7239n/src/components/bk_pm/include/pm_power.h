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

/****************************************************************************
 * Name: bk_pm_module_power_on
 *
 * Description:
 *   Power on a specific power management module and update the module
 *   power state bitmaps. This function transitions a module from powered-off
 *   or sleep state to active (powered-on) state.
 *
 * Input Parameters:
 *   pm_off_modules    - Pointer to bitmap of powered-off modules (updated)
 *   pm_on_modules     - Pointer to bitmap of powered-on modules (updated)
 *   pm_sleeped_modules - Pointer to bitmap of sleeping modules (updated)
 *   module            - The specific module to power on
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t bk_pm_module_power_on(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules, pm_power_module_name_e module);

/****************************************************************************
 * Name: bk_pm_module_power_off
 *
 * Description:
 *   Power off a specific power management module and update the module
 *   power state bitmaps. This function transitions a module from active
 *   (powered-on) state to powered-off state.
 *
 * Input Parameters:
 *   pm_off_modules    - Pointer to bitmap of powered-off modules (updated)
 *   pm_on_modules     - Pointer to bitmap of powered-on modules (updated)
 *   pm_sleeped_modules - Pointer to bitmap of sleeping modules (updated)
 *   module            - The specific module to power off
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t bk_pm_module_power_off(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules, pm_power_module_name_e module);

/****************************************************************************
 * Name: pm_check_power_on_module
 *
 * Description:
 *   Check and verify modules that need to be powered on based on the
 *   current power state bitmaps. This function analyzes module dependencies
 *   and ensures proper power-on sequence.
 *
 * Input Parameters:
 *   pm_off_modules    - Pointer to bitmap of powered-off modules
 *   pm_on_modules     - Pointer to bitmap of powered-on modules
 *   pm_sleeped_modules - Pointer to bitmap of sleeping modules
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_check_power_on_module(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules);

/****************************************************************************
 * Name: pm_check_power_off_module
 *
 * Description:
 *   Check and verify modules that can be safely powered off based on the
 *   current power state bitmaps. This function analyzes module dependencies
 *   and ensures proper power-off sequence.
 *
 * Input Parameters:
 *   pm_off_modules    - Pointer to bitmap of powered-off modules
 *   pm_on_modules     - Pointer to bitmap of powered-on modules
 *   pm_sleeped_modules - Pointer to bitmap of sleeping modules
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_check_power_off_module(uint32_t *pm_off_modules, uint32_t *pm_on_modules, uint64_t *pm_sleeped_modules);
