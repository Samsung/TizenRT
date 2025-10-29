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
 * Name: pm_cp1_psram_malloc_count_state_set
 *
 * Description:
 *   Set the PSRAM malloc count state for CP1 (Co-Processor 1).
 *   Updates the memory allocation counter used to track PSRAM usage
 *   and make power management decisions.
 *
 * Input Parameters:
 *   value - The malloc count value to set
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_cp1_psram_malloc_count_state_set(uint32_t value);

/****************************************************************************
 * Name: pm_cp1_psram_malloc_state_get
 *
 * Description:
 *   Get the current PSRAM malloc state for CP1 (Co-Processor 1).
 *   Retrieves information about PSRAM memory allocation status to
 *   determine if PSRAM can be safely powered down.
 *
 * Returns:
 *   BK_OK if PSRAM is idle and can be powered down, or error code
 *   if PSRAM is in use or cannot be powered down
 *
 ****************************************************************************/
bk_err_t pm_cp1_psram_malloc_state_get(void);

/****************************************************************************
 * Name: pm_psram_malloc_state_and_power_ctrl
 *
 * Description:
 *   Check PSRAM malloc state and control PSRAM power accordingly.
 *   This function monitors PSRAM usage and automatically manages PSRAM
 *   power state based on whether the memory is actively allocated or idle.
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t pm_psram_malloc_state_and_power_ctrl(void);
