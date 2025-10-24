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

#include <stdbool.h>
#include <stdint.h>
#include <common/sys_config.h>

void arch_init(void);
void arch_wait_for_interrupt(void);
void arch_parse_stack_backtrace(const char *str_type, uint32_t stack_top,
        uint32_t stack_bottom, uint32_t stack_size, bool thumb_mode);
void arch_sleep(void);
void arch_deep_sleep(void);
uint64_t check_IRQ_pending(void);
