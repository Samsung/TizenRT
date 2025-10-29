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

#include <common/bk_include.h>
#include <driver/mailbox_channel.h>

/****************************************************************************
 * Name: bk_pm_cp1_psram_power_state_get
 *
 * Description:
 *   Get the PSRAM power state for CP1 (Co-Processor 1) via mailbox
 *   communication. Queries the current power management state of PSRAM
 *   connected to CP1.
 *
 * Returns:
 *   Current PSRAM power state (mailbox communication state enumeration)
 *
 ****************************************************************************/
pm_mailbox_communication_state_e bk_pm_cp1_psram_power_state_get(void);

/****************************************************************************
 * Name: bk_pm_cp1_psram_power_state_set
 *
 * Description:
 *   Set the PSRAM power state for CP1 (Co-Processor 1) via mailbox
 *   communication. Controls the power management state of PSRAM
 *   connected to CP1.
 *
 * Input Parameters:
 *   state - Desired PSRAM power state to set
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t bk_pm_cp1_psram_power_state_set(pm_mailbox_communication_state_e state);

/****************************************************************************
 * Name: bk_pm_cp1_external_ldo_ctrl_state_get
 *
 * Description:
 *   Get the external LDO (Low Drop-Out regulator) control state for CP1
 *   via mailbox communication. Retrieves the current control state of
 *   external power regulator for CP1.
 *
 * Returns:
 *   Current external LDO control state (mailbox communication state enumeration)
 *
 ****************************************************************************/
pm_mailbox_communication_state_e bk_pm_cp1_external_ldo_ctrl_state_get(void);

/****************************************************************************
 * Name: bk_pm_cp1_external_ldo_ctrl_state_set
 *
 * Description:
 *   Set the external LDO (Low Drop-Out regulator) control state for CP1
 *   via mailbox communication. Controls the external power regulator state
 *   for CP1 power management.
 *
 * Input Parameters:
 *   state - Desired external LDO control state to set
 *
 * Returns:
 *   BK_OK on success, or error code on failure
 *
 ****************************************************************************/
bk_err_t bk_pm_cp1_external_ldo_ctrl_state_set(pm_mailbox_communication_state_e state);