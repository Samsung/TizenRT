/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#ifndef SHEIPA_H
#define SHEIPA_H

#if CONFIG_GIC_VER <= 2
#define CONFIG_GIC_V2
#define GIC_DIST_BASE			0xA0101000
#define GIC_CPU_BASE			0xA0102000
#else
#define GIC_DIST_BASE			0x02100000
#define GIC_RDIST_BASE			0x02140000
#endif

#ifndef CONFIG_PLATFORM_TIZENRT_OS
#include "FreeRTOS.h"
#endif
#include "debug_printf.h"
#include "arm_gic.h"
#include "arch_timer.h"
#include "spinlock.h"
#include "psci.h"

#define IRQ_PRIO_DEFAULT		0xa0
#define GIC_MAX_NUM_INTR		(32+96)
#define IPI_IRQ					0
#define IPI_CPUHP_IRQ			1
#define MAX_IRQ_NESTING_DEPTH	16

#ifndef _ASMLANGUAGE

typedef uint32_t (*ISRCallback_t)(void *context);

typedef struct InterruptTable {
	ISRCallback_t pxISR;
	void *pvContext;
} InterruptTable_t;

void smp_init(void);
void vPortSecondaryOff(void);

#endif /* !_ASMLANGUAGE */

#endif
