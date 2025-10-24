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

#include "armstar.h"

typedef struct _bit_cap_str_
{
    uint32_t bit_val;
    char *cap_desc;
}BIT_CAP_STR_T;

void dwt_enable_debug_monitor_exception(void);
void dwt_enable_debug_monitor_mode(void);
void dwt_disable_debug_monitor_mode(void);
void dwt_init_cycle_counter(void);
void dwt_reset_cycle_counter(void);
void dwt_enable_cycle_counter(void);
void dwt_disable_cycle_counter(void);
uint32_t dwt_get_cycle_counter_val(void);
void dwt_set_data_address_read(uint32_t data_address);
void dwt_set_data_address_write(uint32_t data_address);
void dwt_set_data_address_access(uint32_t data_address);
void dwt_set_instruction_address(uint32_t data_address);
void dwt_set_data_address_range(uint32_t start_addr, uint32_t end_addr, ACCESS_T type);
void dwt_set_instruction_address_range(uint32_t start_addr, uint32_t end_addr);
void dwt_conditional_data_watchpoint(uint32_t addr, uint32_t data);
void dwt_disable_watchpoint_comparator(void);

// eof

