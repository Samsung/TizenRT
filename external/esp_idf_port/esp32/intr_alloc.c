/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/******************************************************************
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
******************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "esp_attr.h"
#include "esp_system.h"

#define portNUM_PROCESSORS 1

//This bitmask has an 1 if the int should be disabled when the flash is disabled.
static uint32_t non_iram_int_mask[portNUM_PROCESSORS];
//This bitmask has 1 in it if the int was disabled using esp_intr_noniram_disable.
static uint32_t non_iram_int_disabled[portNUM_PROCESSORS];
static bool non_iram_int_disabled_flag[portNUM_PROCESSORS];

void IRAM_ATTR esp_intr_noniram_disable(void)
{
	int oldint;
	int cpu = smp_processor_id();
	int intmask = ~non_iram_int_mask[cpu];
	if (non_iram_int_disabled_flag[cpu]) {
		abort();
	}
	non_iram_int_disabled_flag[cpu] = true;
	asm volatile("movi %0,0\n" "xsr %0,INTENABLE\n"	//disable all ints first
				 "rsync\n" "and a3,%0,%1\n"	//mask ints that need disabling
				 "wsr a3,INTENABLE\n"	//write back
				 "rsync\n":"=&r"(oldint):"r"(intmask):"a3");
	//Save which ints we did disable
	non_iram_int_disabled[cpu] = oldint & non_iram_int_mask[cpu];
}

void IRAM_ATTR esp_intr_noniram_enable(void)
{
	int cpu = smp_processor_id();
	int intmask = non_iram_int_disabled[cpu];
	if (!non_iram_int_disabled_flag[cpu]) {
		abort();
	}
	non_iram_int_disabled_flag[cpu] = false;
    asm volatile (
        "movi a3,0\n"
        "xsr a3,INTENABLE\n"
        "rsync\n"
        "or a3,a3,%0\n"
        "wsr a3,INTENABLE\n"
        "rsync\n"
        ::"r"(intmask):"a3");
}
