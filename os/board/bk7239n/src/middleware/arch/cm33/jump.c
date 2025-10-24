// Copyright 2020-2023 Beken
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

#include <sdkconfig.h>
#include <stdint.h>
#include <components/log.h>
#include "cmsis_gcc.h"
#include "armstar.h"

#define TAG "jump"

typedef void (*ns_reset_handler_t) (void) __attribute__((cmse_nonsecure_call));

void jump_to_ns(uint32_t addr)
{
	//Don't display any log because all uart maybe disabled by caller!
	ns_reset_handler_t fn;
	__TZ_set_MSP_NS(*(( uint32_t *)(addr)));
	fn = (ns_reset_handler_t)(*(( uint32_t * ) (( addr ) + 4U )));
	fn();
}

void jump_to_s(uint32_t addr)
{
	uint32_t msp = 0;
	uint32_t pc  = 0;

	BK_LOGI(TAG, "Next Image Addr: 0x%x\n", addr);
	msp = *(uint32_t *)addr;
	pc  = *(uint32_t *)(addr + 4);

	BK_LOGI(TAG, "Set MSP: 0x%x\n", (unsigned int)msp);
	BK_LOGI(TAG, "Jump to S: 0x%x\n", (unsigned int)pc);

	__set_MSP(msp);
	__DSB();
	__ISB();

	/* relocate vecotr table */
	SCB->VTOR = (addr);
	__DSB();
	__ISB();

	/* save next entry to r9 */
	__asm__ volatile("mov r9, %0" : : "r"(pc) : "memory");

	/* clear all general registers */
	__asm__ volatile(
		"mov r0, #0;\n\t"
		"mov r1, r0;\n\t"
		"mov r2, r0;\n\t"
		"mov r3, r0;\n\t"
		"mov r4, r0;\n\t"
		"mov r5, r0;\n\t"
		"mov r6, r0;\n\t"
		"mov r7, r0;\n\t"
		"mov r8, r0;\n\t"
		"mov r10, r0;\n\t"
		"mov r11, r0;\n\t"
		"mov r12, r0;\n\t"
		"dsb;\n\t"
		"isb;\n\t"
		"bx r9;\n\t"
		"b .;\n\t"	  /* should not be here */
		);
}
