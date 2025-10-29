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
 * Name: pm_management
 *
 * Description:
 *   Main power management function that determines and executes the
 *   appropriate low power mode based on the expected sleep duration.
 *   This function analyzes system state and sleep ticks to select the
 *   optimal power saving mode.
 *
 * Input Parameters:
 *   sleep_ticks - Expected number of ticks the system will sleep
 *
 * Returns:
 *   uint64_t missed_ticks
 *
 ****************************************************************************/
uint64_t pm_management(uint32_t sleep_ticks);

/****************************************************************************
 * Name: pm_check_protect_time
 *
 * Description:
 *   Check and calculate the protected time interval between two tick values.
 *   This function ensures minimum timing constraints are met to prevent
 *   power mode transitions that are too frequent or too short.
 *
 * Input Parameters:
 *   current_tick  - Current RTC tick value
 *   previous_tick - Previous RTC tick value
 *
 * Returns:
 *   Protected time interval in ticks
 *
 ****************************************************************************/
uint32_t pm_check_protect_time(uint64_t current_tick, uint64_t previous_tick);
