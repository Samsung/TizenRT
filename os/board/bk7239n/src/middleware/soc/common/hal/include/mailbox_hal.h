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

#include "hal_config.h"
#include "mailbox_ll.h"

#define mailbox_hal_struct						mailbox_hal_t

bk_err_t mailbox_hal_addr_init(mailbox_hal_t *hal);
void mailbox_hal_set_identity(mailbox_hal_t *hal);
mailbox_box_num_t mailbox_hal_check_which_box_ready(mailbox_hal_t *hal);
mailbox_box_num_t mailbox_hal_check_which_box_trigger(mailbox_hal_t *hal);


#define mailbox_hal_box_init(hal) mailbox_ll_box_init((hal)->hw)

#define mailbox_hal_write_param0(hal, param0, box) mailbox_ll_write_param0((hal)->hw, param0, box)
#define mailbox_hal_write_param1(hal, param1, box) mailbox_ll_write_param1((hal)->hw, param1, box)
#define mailbox_hal_write_param2(hal, param2, box) mailbox_ll_write_param2((hal)->hw, param2, box)
#define mailbox_hal_write_param3(hal, param3, box) mailbox_ll_write_param3((hal)->hw, param3, box)

#define mailbox_hal_read_param0(hal, box) mailbox_ll_read_param0((hal)->hw, box)
#define mailbox_hal_read_param1(hal, box) mailbox_ll_read_param1((hal)->hw, box)
#define mailbox_hal_read_param2(hal, box) mailbox_ll_read_param2((hal)->hw, box)
#define mailbox_hal_read_param3(hal, box) mailbox_ll_read_param3((hal)->hw, box)

#define mailbox_hal_box_trigger(hal, box) mailbox_ll_box_trigger((hal)->hw, box)
#define mailbox_hal_box_trigger_ready(hal, box) mailbox_ll_box_trigger_ready((hal)->hw, box)
#define mailbox_hal_read_box_ready(hal, box) mailbox_ll_read_box_ready((hal)->hw, box)

#define mailbox_hal_box_clear(hal, box) mailbox_ll_box_clear((hal)->hw, box)
#define mailbox_hal_box_clear_ready(hal, box) mailbox_ll_box_clear_ready((hal)->hw, box)
#define mailbox_hal_read_box_erasure(hal, box) mailbox_ll_read_box_erasure((hal)->hw, box)

#ifdef __cplusplus
}
#endif
