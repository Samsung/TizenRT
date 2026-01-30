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

#include "components/bluetooth/bk_ble_types.h"

uint8_t get_tx_pwr_idx(void);
UINT32 ble_in_dut_mode(void);
void txpwr_max_set_bt_polar(void);

int bluetooth_controller_init(void);
int bluetooth_controller_deinit(void);
int bluetooth_host_init(void);
int bluetooth_host_deinit(void);
int bluetooth_get_mac(uint8_t *mac);
void bluetooth_rf_test_mode_retrig(void);
void bluetooth_lowvol_enter_cb_internal(void);
void bluetooth_lowvol_exit_cb_internal(void);

