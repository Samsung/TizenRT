// Copyright 2020-2021 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

#define BT_SUCCESS          (0)
#define BT_FAILURE          (1)
#define BT_DEV_NAME        "bt"

void ceva_main(void *arg);
void bt_activate(char *bt_name);
void bt_host_to_controller(uint8_t *msg, uint32_t type);
void bt_init(void);
void bt_exit(void);
void ble_host_to_controller(uint32_t cmd, uint8_t *buff,uint32_t len, uint32_t param);

#ifdef __cplusplus
}
#endif
