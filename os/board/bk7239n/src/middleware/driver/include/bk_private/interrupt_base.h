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

#include <common/bk_include.h>
#include <common/bk_compiler.h>
#include "bk_list.h"
#include <common/bk_generic.h>
#include "icu_map.h"
#include "arch_interrupt.h"
#include <common/bk_err.h>

typedef struct {
	LIST_HEADER_T list;
	int_group_isr_t isr_func;
	uint8_t   pri;	// priority
	uint8_t  int_num;
	uint16_t  resv;
} isr_t;

typedef struct {
	LIST_HEADER_T isr;
	uint8_t max;     // maximum interrupt number
	uint8_t min;     // minum interrupt number
} isr_list_t;

typedef struct {
	icu_int_src_t src;
	uint8_t int_bit;
	uint8_t int_prio;
	uint8_t group;
} icu_int_map_t;


void interrupt_init(void);
void interrupt_deinit(void);
void interrupt_spurious(void);
void group_isr(uint32_t group_id, uint32_t int_status);



