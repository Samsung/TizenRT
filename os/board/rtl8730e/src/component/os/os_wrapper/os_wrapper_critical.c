/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba.h"
#include "os_wrapper.h"

int rtos_critical_is_in_interrupt(void)
{
	return up_interrupt_context();
}

void rtos_critical_enter(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
}

void rtos_critical_exit(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
}

uint32_t rtos_get_critical_state(void)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return 0;
}

irqstate_t tizenrt_critical_enter(void)
{
	return enter_critical_section();
}

void tizenrt_critical_exit(irqstate_t flags)
{
	leave_critical_section(flags);
}
