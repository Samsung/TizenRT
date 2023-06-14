/*
 * Copyright (c) 2015-2019, Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/gic_common.h>
#include <platform_def.h>
#include <plat/common/platform.h>
#include "sheipa_private.h"

#if ARM_GIC_ARCH == 3
#include <drivers/arm/gicv3.h>
/* The GICv3 driver only needs to be initialized in EL3 */
static uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];

const gicv3_driver_data_t plat_gicv3_driver_data = {
	.gicd_base = GICD_BASE,
	.gicr_base = GICR_BASE,
	.interrupt_props = 0,
	.interrupt_props_num = 0,
	.rdistif_num = PLATFORM_CORE_COUNT,
	.rdistif_base_addrs = rdistif_base_addrs,
	.mpidr_to_core_pos = plat_sheipa_calc_core_pos
};
#else
#include <drivers/arm/gicv2.h>
static const struct gicv2_driver_data plat_gicv2_driver_data = {
	.gicd_base = GICD_BASE,
	.gicc_base = GICC_BASE,
	.interrupt_props = 0,
	.interrupt_props_num = 0,
};
#endif

void plat_sheipa_gic_driver_init(void)
{

#if ARM_GIC_ARCH == 3
	/*
	 * The GICv3 driver is initialized in EL3 and does not need
	 * to be initialized again in SEL1. This is because the S-EL1
	 * can use GIC system registers to manage interrupts and does
	 * not need GIC interface base addresses to be configured.
	 */
#if (!defined(__aarch64__) && defined(IMAGE_BL32)) || \
	(defined(__aarch64__) && defined(IMAGE_BL31))
	gicv3_driver_init(&plat_gicv3_driver_data);
#endif
#else
	gicv2_driver_init(&plat_gicv2_driver_data);
#endif
}

/******************************************************************************
 * ARM common helper to initialize the GIC. Only invoked by BL31
 *****************************************************************************/
void plat_sheipa_gic_init(void)
{
#if ARM_GIC_ARCH == 3
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
#else
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
#endif
}

/******************************************************************************
 * ARM common helper to enable the GIC CPU interface
 *****************************************************************************/
void plat_sheipa_gic_cpuif_enable(void)
{

#if ARM_GIC_ARCH == 3
	gicv3_cpuif_enable(plat_my_core_pos());
#else
	gicv2_cpuif_enable();
#endif
}

/******************************************************************************
 * ARM common helper to disable the GIC CPU interface
 *****************************************************************************/
void plat_sheipa_gic_cpuif_disable(void)
{
#if ARM_GIC_ARCH == 3
	gicv3_cpuif_disable(plat_my_core_pos());
#else
	gicv2_cpuif_disable();
#endif
}

/******************************************************************************
 * ARM common helper to initialize the per-cpu redistributor interface in GICv3
 *****************************************************************************/
void plat_sheipa_gic_pcpu_init(void)
{
#if ARM_GIC_ARCH == 3
	gicv3_rdistif_init(plat_my_core_pos());
#else
	gicv2_pcpu_distif_init();
#endif
}

#if ARM_GIC_ARCH == 3
/******************************************************************************
 * ARM common helpers to power GIC redistributor interface
 *****************************************************************************/
void plat_sheipa_gic_redistif_on(void)
{
	gicv3_rdistif_on(plat_my_core_pos());
}

void plat_sheipa_gic_redistif_off(void)
{
	gicv3_rdistif_off(plat_my_core_pos());
}
#endif
