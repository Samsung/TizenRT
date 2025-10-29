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
 * Name: pm_cpu_wfi_process
 *
 * Description:
 *   Execute CPU WFI (Wait For Interrupt) sleep process.
 *   This is the lightest sleep mode where the CPU waits for an interrupt
 *   while keeping all peripherals and clocks active.
 *
 * Returns:
 *   uint64_t sleep_tick
 *
 ****************************************************************************/
uint64_t pm_cpu_wfi_process(void);

/****************************************************************************
 * Name: pm_normal_sleep_process
 *
 * Description:
 *   Execute normal sleep process.
 *   This sleep mode provides moderate power savings by reducing CPU
 *   clock frequency while maintaining system state and peripheral operation.
 *
 * Returns:
 *   uint64_t sleep_tick
 *
 ****************************************************************************/
uint64_t pm_normal_sleep_process(void);

/****************************************************************************
 * Name: pm_low_voltage_process
 *
 * Description:
 *   Execute low voltage sleep process.
 *   This mode achieves significant power savings by entering low voltage
 *   operation, reducing both voltage and frequency while preserving
 *   critical system state.
 *
 * Returns:
 *   uint64_t sleep_tick
 *
 ****************************************************************************/
uint64_t pm_low_voltage_process(void);

/****************************************************************************
 * Name: pm_deep_sleep_prepare
 *
 * Description:
 *   Prepare system for deep sleep mode.
 *   Performs necessary hardware and software preparations before entering
 *   deep sleep, including saving context and configuring wakeup sources.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_deep_sleep_prepare(void);

/****************************************************************************
 * Name: pm_deep_sleep_process
 *
 * Description:
 *   Execute deep sleep process.
 *   Enters deep sleep mode with maximum power savings by powering down
 *   most system components while maintaining minimal wakeup capability.
 *
 * Returns:
 *   uint64_t sleep_tick
 *
 ****************************************************************************/
uint64_t pm_deep_sleep_process(void);

#if CONFIG_PM_SUPER_DEEP_SLEEP
/****************************************************************************
 * Name: pm_super_deep_sleep_prepare
 *
 * Description:
 *   Prepare system for super deep sleep mode.
 *   Performs extensive preparations for the deepest sleep mode, including
 *   saving all necessary context and configuring minimal wakeup sources.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_super_deep_sleep_prepare(void);

/****************************************************************************
 * Name: pm_super_deep_sleep_process
 *
 * Description:
 *   Execute super deep sleep process.
 *   Enters the deepest sleep mode with absolute minimum power consumption
 *   by shutting down nearly all system components except essential wakeup
 *   circuitry.
 *
 * Returns:
 *   None
 *
 ****************************************************************************/
void pm_super_deep_sleep_process(void);
#endif

/****************************************************************************
 * Name: pm_lv_enter_time_out_clear
 *
 * Description:
 *   Clear the low voltage entry timeout flag.
 *   Resets timeout tracking when entering or recovering from low voltage
 *   mode to prevent false timeout detections.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_lv_enter_time_out_clear(void);