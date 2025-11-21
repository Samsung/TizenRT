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
// #include <common/bk_compiler.h>
#include <os/mem.h>
#include "icu_driver.h"
#include "interrupt_base.h"
#include "interrupt.h"
#include <driver/int_types.h>
#include <driver/int.h>
#include <common/bk_assert.h>
#include <tinyara/irq.h>

#define ICU_RETURN_ON_INVALID_DEVS(dev) do {\
				if ((dev) >= INT_SRC_NONE) {\
					return BK_ERR_INT_DEVICE_NONE;\
				}\
			} while(0)

const icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;


bk_err_t bk_int_isr_register(icu_int_src_t src, int_group_isr_t isr_callback, void*arg)
{
	ICU_RETURN_ON_INVALID_DEVS(src);

	// icu_int_map_t icu_int_map_table[] = ICU_DEV_MAP;
	const icu_int_map_t *icu_int_map = &icu_int_map_table[src];

	uint8_t int_num = icu_int_map->int_bit + ARMINO_IRQ_FIRST;

    irq_attach(int_num, (xcpt_t)isr_callback, arg);
    up_enable_irq(int_num);

	return BK_OK;
}
void interrupt_init(void)
{
	// soc_isr_init();
}

void interrupt_deinit(void)
{
	// soc_isr_deinit();
}

bk_err_t bk_int_set_priority(icu_int_src_t int_src, uint32_t int_priority)
{
#ifdef CONFIG_ARCH_IRQPRIO
	const icu_int_map_t *icu_int_map = &icu_int_map_table[int_src];
	uint8_t int_num = icu_int_map->int_bit + ARMINO_IRQ_FIRST;

	up_prioritize_irq(int_num, int_priority);
#endif
	return BK_OK;
}

bk_err_t bk_int_isr_unregister(icu_int_src_t src)
{
	ICU_RETURN_ON_INVALID_DEVS(src);

	const icu_int_map_t *icu_int_map = &icu_int_map_table[src];

	uint8_t int_num = icu_int_map->int_bit  + ARMINO_IRQ_FIRST;

	up_disable_irq(int_num);
    irq_detach(int_num);

	return BK_OK;
}
