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

#include "sdkconfig.h"
#include "armstar.h"
#include "arch_interrupt.h"
#include "components/log.h"

#define INT_NUMBER_MAX              (64)
#define TO_NVIC_IRQ(irq)            ((uint32_t)(irq))

static int_group_isr_t s_irq_handler[INT_NUMBER_MAX];

void arch_int_enable_irq(uint32_t irq)
{
	NVIC_EnableIRQ(TO_NVIC_IRQ(irq));
}

void arch_int_disable_irq(uint32_t irq)
{
	NVIC_DisableIRQ(TO_NVIC_IRQ(irq));
}

uint32_t arch_int_get_enable_irq(uint32_t irq)
{
	return NVIC_GetEnableIRQ(TO_NVIC_IRQ(irq));
}

void arch_int_set_target_state(uint32_t irq)
{
	NVIC_SetTargetState(TO_NVIC_IRQ(irq));
}

void arch_int_clear_target_state(uint32_t irq)
{
	NVIC_ClearTargetState(TO_NVIC_IRQ(irq));
}

uint32_t arch_int_get_target_state(uint32_t irq)
{
	return NVIC_GetTargetState(TO_NVIC_IRQ(irq));
}

void arch_interrupt_set_priority(uint32_t int_number, uint32_t int_priority)
{
	if (int_number > 0 && int_number < INT_NUMBER_MAX) {
		NVIC_SetPriority(TO_NVIC_IRQ(int_number), int_priority);
	}

	return;
}

#if defined(CONFIG_SOC_BK7239XX)
__IRAM_SEC void arch_int_set_default_priority(void)
{
	/* group priority is depends on macro:__NVIC_PRIO_BITS.
	   please refer to: www.freertos.org/zh-cn-cmn-s/RTOS-Cortex-M3-M4.html*/
	NVIC_SetPriorityGrouping(PRI_GOURP_BITS_7_5);
	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_SetPriority(irq_type, IRQ_DEFAULT_PRIORITY);
	}
}
#endif

void arch_interrupt_register_int(uint32_t int_number, int_group_isr_t isr_callback)
{
	if ((int_number > (INT_NUMBER_MAX - 1)) || isr_callback == NULL) {
		return;
	}

	s_irq_handler[int_number] = isr_callback;
	NVIC_EnableIRQ(int_number);
}

void arch_interrupt_unregister_int(uint32_t int_number)
{
	if (int_number > (INT_NUMBER_MAX - 1)) {
		return;
	}
	NVIC_DisableIRQ(int_number);
	s_irq_handler[int_number] = NULL;
}

int_group_isr_t arch_interrupt_get_handler(uint32_t int_number)
{
	return s_irq_handler[int_number];
}

#if defined(CONFIG_SOC_BK7239XX)
void arch_int_init_all_irq(void)
{
	__disable_irq();
	__disable_fault_irq();

	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_SetPriority(irq_type, IRQ_DEFAULT_PRIORITY);
		NVIC_DisableIRQ(irq_type);
	}
}

void arch_int_enable_all_irq(void)
{
	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_SetPriority(irq_type, IRQ_DEFAULT_PRIORITY);
		NVIC_EnableIRQ(irq_type);
	}

	__enable_fault_irq();
	__enable_irq();
}

void arch_int_disable_all_irq(void)
{
	__disable_irq();
	__disable_fault_irq();

	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_DisableIRQ(irq_type);
	}
}

bk_err_t arch_isr_entry_init(void)
{
	/* group priority is depends on macro:__NVIC_PRIO_BITS.
	   please refer to: www.freertos.org/zh-cn-cmn-s/RTOS-Cortex-M3-M4.html*/
	NVIC_SetPriorityGrouping(PRI_GOURP_BITS_7_5);
	arch_int_init_all_irq();

	return BK_OK;
}
#endif

#if defined(CONFIG_SOC_BK7236_SMP_TEMP) || defined(CONFIG_SOC_BK7239_SMP_TEMP)
/*TODO: the object about the corresponding core shall has the action*/
bk_err_t arch_isr_entry_init2(void)
{
	/* group priority is depends on macro:__NVIC_PRIO_BITS.
	   please refer to: www.freertos.org/zh-cn-cmn-s/RTOS-Cortex-M3-M4.html*/
	NVIC_SetPriorityGrouping(PRI_GOURP_BITS_7_5);
	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_SetPriority(irq_type, IRQ_DEFAULT_PRIORITY);
		NVIC_EnableIRQ(irq_type);
	}

	return BK_OK;
}
#endif

#if defined(CONFIG_SOC_BK7239XX)
void dump_state(void)
{
	uint32_t bits = 0;

	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		if (irq_type == 32) {
			BK_LOGI("int", "bits=%x\r\n", bits);
			bits = 0;
		}

		if (NVIC_GetTargetState(irq_type)) {
			uint32_t bit = irq_type % 32;
			bits |= BIT(bit);
		}
	}

	BK_LOGI("int", "high bits=%x\r\n", bits);
}

void arch_int_set_target_state_all(void)
{
	dump_state();
	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		NVIC_SetTargetState(irq_type);
	}
	dump_state();
}

void arch_int_dump_statis(void)
{
#if defined(CONFIG_ARCH_INT_STATIS)
	for (uint32_t irq_type = 0; irq_type < INT_NUMBER_MAX; irq_type++) {
		BK_LOGI(TAG, "[%d] = %u\r\n", irq_type, s_int_statis[irq_type]);
	}
#endif
}
#endif
// eof

