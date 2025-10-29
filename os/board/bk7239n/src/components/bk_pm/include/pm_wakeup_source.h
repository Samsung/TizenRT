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
 * Name: bk_pm_wakeup_source_get
 *
 * Description:
 *   Get the current wakeup source that triggered system wakeup.
 *   This function identifies which peripheral or event caused the system
 *   to wake from any sleep mode.
 *
 * Returns:
 *   Wakeup source enumeration value
 *
 ****************************************************************************/
pm_wakeup_source_e bk_pm_wakeup_source_get(void);

/****************************************************************************
 * Name: bk_pm_deep_sleep_wakeup_source_get
 *
 * Description:
 *   Get the wakeup source that woke the system from deep sleep mode.
 *   Identifies the specific event or peripheral that triggered wakeup
 *   from deep sleep state.
 *
 * Returns:
 *   Deep sleep wakeup source enumeration value
 *
 ****************************************************************************/
pm_wakeup_source_e bk_pm_deep_sleep_wakeup_source_get(void);

/****************************************************************************
 * Name: bk_pm_exit_low_vol_wakeup_source_get
 *
 * Description:
 *   Get the wakeup source that caused exit from low voltage mode.
 *   Identifies the event or peripheral that triggered wakeup from
 *   low voltage sleep state.
 *
 * Returns:
 *   Low voltage mode wakeup source enumeration value
 *
 ****************************************************************************/
pm_wakeup_source_e bk_pm_exit_low_vol_wakeup_source_get(void);

/****************************************************************************
 * Name: pm_touched_wakeup_low_voltage
 *
 * Description:
 *   Handle touch event wakeup from low voltage mode.
 *   Processes the touch-triggered wakeup event when exiting from
 *   low voltage sleep state.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_touched_wakeup_low_voltage(void);

/****************************************************************************
 * Name: pm_rtc_wakeup_deep_sleep
 *
 * Description:
 *   Handle RTC (Real-Time Clock) timer wakeup from deep sleep.
 *   Processes the RTC-triggered wakeup event when exiting from
 *   deep sleep state.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_rtc_wakeup_deep_sleep(void);

/****************************************************************************
 * Name: pm_touched_wakeup_deep_sleep
 *
 * Description:
 *   Handle touch event wakeup from deep sleep mode.
 *   Processes the touch-triggered wakeup event when exiting from
 *   deep sleep state.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_touched_wakeup_deep_sleep(void);

/****************************************************************************
 * Name: pm_wakeup_from_deepsleep_handle
 *
 * Description:
 *   Handle system restoration after waking from deep sleep.
 *   Performs necessary system initialization and state restoration
 *   after exiting deep sleep mode.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_wakeup_from_deepsleep_handle(void);

/****************************************************************************
 * Name: pm_deep_sleep_wakeup_source_set
 *
 * Description:
 *   Configure and set the wakeup sources for deep sleep mode.
 *   Determines which events or peripherals are enabled to wake the
 *   system from deep sleep state.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_deep_sleep_wakeup_source_set(void);