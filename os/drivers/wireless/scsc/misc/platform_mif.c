/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include <tinyara/irq.h>
#include <tinyara/wdog.h>
#include <tinyara/kmalloc.h>

#include <arch/chip/irq.h>

#include "platform_mif.h"
#include "mif_reg.h"
#include "platform_mif_module.h"
#include "utils_misc.h"

enum irqreturn {
	IRQ_NONE = (0 << 0),
	IRQ_HANDLED = (1 << 0),
	IRQ_WAKE_THREAD = (1 << 1),
};
typedef enum irqreturn irqreturn_t;

#define __iomem

#define putreg32(v, c)		(*(volatile unsigned int*)(c) = (v))
#define getreg32(c)		(*(volatile unsigned int*)(c))

/* Enables WIFI ARM cores reset */
#ifdef SCSC_DISABLE_WLAN_RESET
static bool enable_platform_mif_arm_reset = false;
#else
static bool enable_platform_mif_arm_reset = true;
#endif

struct platform_mif *platform_isr;

struct platform_mif {
	struct scsc_mif_abs interface;
	struct scsc_mbox_s *mbox;
	struct {
		int irq_num;
		int flags;
	} wlbt_irq[3];

	/* register MBOX memory space */
	size_t reg_size;
	void __iomem *base;

	/* Shared memory space - reserved memory */
	unsigned long mem_start;
	size_t mem_size;
	void __iomem *mem;

	/* Callback function and dev pointer mif_intr manager handler */
	void (*r4_handler)(int irq, void *data);
	void *irq_dev;
	/* spinlock to serialize driver access */
	spinlock_t mif_spinlock;
	void (*reset_request_handler)(int irq, void *data);
	void *irq_reset_request_dev;

	/* Suspend/resume handlers */
	int (*suspend_handler)(struct scsc_mif_abs *abs, void *data);
	void (*resume_handler)(struct scsc_mif_abs *abs, void *data);
	void *suspendresume_data;
};

#define platform_mif_from_mif_abs(MIF_ABS_PTR) container_of(MIF_ABS_PTR, struct platform_mif, interface)

static inline void platform_mif_reg_write(struct platform_mif *platform, u16 offset, u32 value)
{
	putreg32(value, platform->base + offset);
	//dev_dbg(NULL, "INTGR1 value: bit %d %p\n", getreg32(platform->base + offset), platform->base + offset);
}

static inline u32 platform_mif_reg_read(struct platform_mif *platform, u16 offset)
{
	return getreg32(platform->base + offset);
}

static void platform_mif_irq_default_handler(int irq, void *data)
{
	/* Avoid unused parameter error */
	(void)irq;
	(void)data;

	/* int handler not registered */
	dev_info(NULL, "%s INT handler not registered\n", __func__);
}

static void platform_mif_irq_reset_request_default_handler(int irq, void *data)
{
	/* Avoid unused parameter error */
	(void)irq;
	(void)data;

	/* int handler not registered */
	dev_info(NULL, "%s INT reset_request handler not registered\n", __func__);
}

int platform_mif_isr(int irq, FAR void *data, void *arg)
{
	//struct platform_mif *platform = (struct platform_mif *)data;
	struct platform_mif *platform = platform_isr;

	if (platform->r4_handler != platform_mif_irq_default_handler) {
		platform->r4_handler(irq, platform->irq_dev);
	} else {
		dev_info(NULL, "MIF Interrupt Handler not registered\n");
	}

	return IRQ_HANDLED;
}

#ifdef CONFIG_SCSC_ENABLE_ALIVE_IRQ
int platform_alive_isr(int irq, void *data, void *arg)
{
	struct platform_mif *platform = (struct platform_mif *)data;

	dev_info(NULL, "%s INT received\n", __func__);

	return IRQ_HANDLED;
}
#endif

int platform_wdog_isr(int irq, void *data, void *arg)
{
	//struct platform_mif *platform = (struct platform_mif *)data;
	struct platform_mif *platform = platform_isr;

	dev_info(NULL, "%s WDOG INT received\n", __func__);
	if (platform->reset_request_handler != platform_mif_irq_reset_request_default_handler) {
		up_disable_irq(platform->wlbt_irq[PLATFORM_MIF_WDOG].irq_num);
		platform->reset_request_handler(irq, platform->irq_reset_request_dev);
	} else {
		dev_info(NULL, "MIF Interrupt reset_request_handler not registered\n");
	}

	return IRQ_HANDLED;
}

static void platform_mif_unregister_irq(struct platform_mif *platform)
{
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Unregistering IRQs\n");
#endif
	up_disable_irq(IRQ_MAILBOX_WIFI);
	up_disable_irq(IRQ_WIFI_WB2AP_WDOG_RESET);
#ifdef CONFIG_SCSC_ENABLE_ALIVE_IRQ
	/* if ALIVE irq is required  */
	devm_free_irq(NULL, platform->wlbt_irq[PLATFORM_MIF_ALIVE].irq_num, platform);
#endif
}

static int platform_mif_register_irq(struct platform_mif *platform)
{
	int err;

	/* Register MBOX irq */
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Registering MBOX irq: %d flag 0x%x\n", platform->wlbt_irq[PLATFORM_MIF_MBOX].irq_num, platform->wlbt_irq[PLATFORM_MIF_MBOX].flags);
#endif

	err = irq_attach(platform->wlbt_irq[PLATFORM_MIF_MBOX].irq_num, platform_mif_isr, NULL);
	up_enable_irq(IRQ_MAILBOX_WIFI);

	if (err) {
		dev_err(NULL, "Failed to register MBOX handler: %d. Aborting.\n", err);
		err = -ENODEV;
		return err;
	}

	/* Register WDOG irq */
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Registering WDOG irq: %d flag 0x%x\n", platform->wlbt_irq[PLATFORM_MIF_WDOG].irq_num, platform->wlbt_irq[PLATFORM_MIF_WDOG].flags);
#endif

	err = irq_attach(platform->wlbt_irq[PLATFORM_MIF_WDOG].irq_num, platform_wdog_isr, NULL);
	up_enable_irq(IRQ_WIFI_WB2AP_WDOG_RESET);

	if (err) {
		dev_err(NULL, "Failed to register WDOG handler: %d. Aborting.\n", err);
		err = -ENODEV;
		return err;
	}
#ifdef CONFIG_SCSC_ENABLE_ALIVE_IRQ
	/* Register ALIVE irq */
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Registering ALIVE irq: %d flag 0x%x\n", platform->wlbt_irq[PLATFORM_MIF_ALIVE].irq_num, platform->wlbt_irq[PLATFORM_MIF_ALIVE].flags);
#endif

	err = irq_attach(platform->wlbt_irq[PLATFORM_MIF_ALIVE].irq_num, platform_alive_isr, NULL);
	if (err) {
		dev_err(NULL, "Failed to register ALIVE handler: %d. Aborting.\n", err);
		err = -ENODEV;
		return err;
	}
#endif
	return 0;
}

static void platform_mif_destroy(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	platform_mif_unregister_irq(platform);
}

static char *platform_mif_get_uid(struct scsc_mif_abs *interface)
{
	/* Avoid unused parameter error */
	(void)interface;
	return "0";
}

/* Wi-Fi Power domain */
static int platform_mif_power(bool power)
{
	u32 val = 0x0;

	/* WIFI power on/off control  If WIFI_PWRON = 1
	 * and WIFI_START=1, WIFI enters to UP state.
	 * This bit is 0 as default value because WIFI
	 * should be reset at AP boot mode after Power-on Reset.  This bits reset is */
	if (power) {
		val = 0x1;
	}

	modifyreg32((uint32_t)WIFI_CTRL_NS, 0x1 << 1, val << 1);	//WIFI_PWRON
	return 0;
}

/* Wi-Fi RESET */
static int platform_mif_hold_reset(bool reset)
{
	u32 val = 0x0;

	if (reset) {
		val = 0x1;
	}

#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	pr_debug("Setting WIFI_CTRL_NS to %d\n", val);
#endif
	modifyreg32((uint32_t)WIFI_CTRL_NS, 0x1 << 2, val << 2);	//WIFI_RESET_SET

	return 0;
}

/* Wi-Fi START */
static int platform_mif_start(void)
{
	modifyreg32((uint32_t)WIFI_CTRL_S, 0x1 << 3, 0x1 << 3);	//WIFI_START

	return 0;
}

static int platform_mif_pmu_reset_release(struct scsc_mif_abs *interface)
{
	int ret = 0;

	ret = platform_mif_power(true);
	if (ret) {
		return ret;
	}
	ret = platform_mif_hold_reset(false);
	if (ret) {
		return ret;
	}
	ret = platform_mif_start();
	if (ret) {
		return ret;
	}

	return ret;
}

static int platform_mif_pmu_reset(struct scsc_mif_abs *interface, u8 rst_case)
{
	int ret;

	if (rst_case == 0 || rst_case > 2) {
		dev_err(NULL, "Incorrect pmu reset case %d\n", rst_case);
		return -EIO;
	}

	modifyreg32((uint32_t)RESET_ASB_WIFI_SYS_PWR_REG, 0x3, 0x0);
	modifyreg32((uint32_t)TCXO_GATE_WIFI_SYS_PWR_REG, 0x1, 0x0);
	modifyreg32((uint32_t)LOGIC_RESET_WIFI_SYS_PWR_REG, 0x3, 0x0);
	modifyreg32((uint32_t)CLEANY_BUS_WIFI_SYS_PWR_REG, 0x1, 0x0);

	modifyreg32((uint32_t)CENTRAL_SEQ_WIFI_CONFIGURATION, 0x1 << 16, 0x0);

	if (rst_case == 1) {
		ret = platform_mif_power(false);
	} else {
		ret = platform_mif_hold_reset(true);
	}

	if (ret) {
		return ret;
	}

	while (((getreg32(CENTRAL_SEQ_WIFI_STATUS) >> 16) & 0xFF) != 0x80) ;

	return 0;
}

/* reset=0 - release from reset */
/* reset=1 - hold reset */
static int platform_mif_reset(struct scsc_mif_abs *interface, bool reset)
{
	u32 ret = 0;
	u32 val;

	if (enable_platform_mif_arm_reset || !reset) {
		if (!reset) {			/* Release from reset */
			/* Set WIFI_MEM_BA0 */
			val = up_wlan_get_firmware() >> 12;
			putreg32(val, WIFI2AP_MEM_CONFIG1);

			/* Set WIFI_MEM_BA1 */
			val = WIFI_IRAM_MEM_BASE;
			putreg32(val, WIFI2AP_MEM_CONFIG2);

			/* Set WIFI_MEM_SIZE */
			val = WIFI_IMEM_SIZE;
			putreg32(val, WIFI2AP_MEM_CONFIG0);

			/* Set bit 0 of PMU_SPARE0 to 1 */
			modifyreg32((uint32_t)WIFI2AP_PMU_SPARE0, 0x1, 0x1);
			ret = platform_mif_pmu_reset_release(interface);
		} else {
			ret = platform_mif_pmu_reset(interface, 2);
			/* WLBT should be stopped/powered-down at this point */
			putreg32(0x00000, WIFI2AP_MEM_CONFIG1);
			putreg32(0x00000, WIFI2AP_MEM_CONFIG0);
		}
	} else {
		dev_info(NULL, "Not resetting ARM Cores - enable_platform_mif_arm_reset: %d\n", enable_platform_mif_arm_reset);
	}
	return ret;
}

static void __iomem *platform_mif_map_region(unsigned long phys_addr, size_t size)
{
	void *vmem = NULL;

	return (void *)vmem;
}

static void platform_mif_unmap_region(void *vmem)
{
	//vunmap(vmem);
}

static void *platform_mif_map(struct scsc_mif_abs *interface, size_t *allocated)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u8 i;

	if (allocated) {
		*allocated = 0;
	}

	platform->mem = (void *)platform->mem_start;

	if (!platform->mem) {
		dev_err(NULL, "Error remaping shared memory\n");
		return NULL;
	}
	/* Initialise MIF registers with documented defaults */
	/* MBOXes */
	for (i = 0; i < NUM_MBOX_PLAT; i++) {
		//dev_info(NULL, "platform->base %p MAILBOX %lx\n", platform->base, MAILBOX_WLBT_REG(ISSR(i)));
		platform_mif_reg_write(platform, MAILBOX_WLBT_REG(ISSR(i)), 0x00000000);
	}
	/* Clear all the interrupts R0/R1/R2 */
	/* CRs *//* 1's */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR0), 0xffff0000);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR1), 0x0000ffff);
	/* MRs *//*0's */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR0), 0x00000000);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR1), 0x00000000);
	/*Set WLBT_BOOT_TEST_RST_CFG to 0 to boot from external DRAM */
	putreg32(0x00000, WIFI_BOOT_TEST_RST_CONFIG);

	/* Add more requrired initialization here: */

	/* register interrupts */
	if (platform_mif_register_irq(platform)) {
		dev_info(NULL, "Unmap: virt %p phys %lx\n", platform->mem, (uintptr_t)platform->mem_start);
		platform_mif_unmap_region(platform->mem);
		return NULL;
	}

	if (allocated) {
		*allocated = platform->mem_size;
	}
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Map: platform->mem %p\n", platform->mem);
#endif

	return platform->mem;
}

/* HERE: Not sure why mem is passed in - its stored in platform - as it should be */
static void platform_mif_unmap(struct scsc_mif_abs *interface, void *mem)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	/* Avoid unused parameter error */
	(void)mem;

	/* MRs *//*1's - set all as Masked */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR0), 0xffff0000);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR1), 0x0000ffff);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR2), 0x0000ffff);

	platform_mif_unregister_irq(platform);

	/* CRs *//* 1's - clear all the interrupts */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR0), 0xffff0000);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR1), 0x0000ffff);
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR2), 0x0000ffff);
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Unmap: virt %p phys %lx\n", platform->mem, (uintptr_t)platform->mem_start);
#endif
	platform_mif_unmap_region(platform->mem);
	platform->mem = NULL;
}

static u32 platform_mif_irq_bit_mask_status_get(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 val;

	val = platform_mif_reg_read(platform, MAILBOX_WLBT_REG(INTMSR0)) >> 16;
	dev_dbg(NULL, "Getting INTMSR0: 0x%x\n", val);
	return val;
}

static u32 platform_mif_irq_get(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 val;

	val = platform_mif_reg_read(platform, MAILBOX_WLBT_REG(INTMSR0)) >> 16;

	return val;
}

static void platform_mif_irq_bit_set(struct scsc_mif_abs *interface, int bit_num, enum scsc_mif_abs_target target)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 reg;

	if (bit_num >= 16) {
		dev_err(NULL, "Incorrect INT number: %d\n", bit_num);
		return;
	}
	/* Generate INT to R4/M4 - VIC */
	if (target == SCSC_MIF_ABS_TARGET_R4) {
		reg = INTGR1;
	} else {
		dev_err(NULL, "Incorrect Target %d\n", target);
		return;
	}
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(reg), (1 << bit_num));
}

static void platform_mif_irq_bit_clear(struct scsc_mif_abs *interface, int bit_num)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	if (bit_num >= 16) {
		dev_err(NULL, "Incorrect INT number: %d\n", bit_num);
		return;
	}
	/* WRITE : 1 = Clears Interrupt */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTCR0), ((1 << bit_num) << 16));
}

static void platform_mif_irq_bit_mask(struct scsc_mif_abs *interface, int bit_num)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 val;
	unsigned long flags;

	if (bit_num >= 16) {
		dev_err(NULL, "Incorrect INT number: %d\n", bit_num);
		return;
	}
	spin_lock_enter_critical_section(&platform->mif_spinlock, flags);
	val = platform_mif_reg_read(platform, MAILBOX_WLBT_REG(INTMR0));
	/* WRITE : 1 = Mask Interrupt */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR0), val | ((1 << bit_num) << 16));
	spin_unlock_leave_critical_section(&platform->mif_spinlock, flags);
}

static void platform_mif_irq_bit_unmask(struct scsc_mif_abs *interface, int bit_num)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 val;
	unsigned long flags;

	if (bit_num >= 16) {
		dev_err(NULL, "Incorrect INT number: %d\n", bit_num);
		return;
	}
	spin_lock_enter_critical_section(&platform->mif_spinlock, flags);
	val = platform_mif_reg_read(platform, MAILBOX_WLBT_REG(INTMR0));
	/* WRITE : 0 = Unmask Interrupt */
	platform_mif_reg_write(platform, MAILBOX_WLBT_REG(INTMR0), val & ~((1 << bit_num) << 16));
	spin_unlock_leave_critical_section(&platform->mif_spinlock, flags);
}

static void platform_mif_irq_reg_handler(struct scsc_mif_abs *interface, void (*handler)(int irq, void *data), void *dev)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	unsigned long flags;

	spin_lock_enter_critical_section(&platform->mif_spinlock, flags);
	platform->r4_handler = handler;
	platform->irq_dev = dev;
	spin_unlock_leave_critical_section(&platform->mif_spinlock, flags);
}

static void platform_mif_irq_unreg_handler(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	unsigned long flags;
#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "Unregistering mif int handler %pS\n", interface);
#endif
	spin_lock_enter_critical_section(&platform->mif_spinlock, flags);
	platform->r4_handler = platform_mif_irq_default_handler;
	platform->irq_dev = NULL;
	spin_unlock_leave_critical_section(&platform->mif_spinlock, flags);
}

static void platform_mif_irq_reg_reset_request_handler(struct scsc_mif_abs *interface, void (*handler)(int irq, void *data), void *dev)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	platform->reset_request_handler = handler;
	platform->irq_reset_request_dev = dev;
}

static void platform_mif_irq_unreg_reset_request_handler(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	dev_info(NULL, "Registering mif reset_request int handler %pS\n", interface);
	platform->reset_request_handler = platform_mif_irq_reset_request_default_handler;
	platform->irq_reset_request_dev = NULL;
}

static void platform_mif_suspend_reg_handler(struct scsc_mif_abs *interface, int (*suspend)(struct scsc_mif_abs *abs, void *data), void (*resume)(struct scsc_mif_abs *abs, void *data), void *data)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	platform->suspend_handler = suspend;
	platform->resume_handler = resume;
	platform->suspendresume_data = data;
}

static void platform_mif_suspend_unreg_handler(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	dev_info(NULL, "Unregistering mif suspend/resume handlers in %p %p\n", platform, interface);
	platform->suspend_handler = NULL;
	platform->resume_handler = NULL;
	platform->suspendresume_data = NULL;
}

static u32 *platform_mif_get_mbox_ptr(struct scsc_mif_abs *interface, u32 mbox_index)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);
	u32 *addr;

	addr = platform->base + MAILBOX_WLBT_REG(ISSR(mbox_index));

	return addr;
}

static int platform_mif_get_mifram_ref(struct scsc_mif_abs *interface, void *ptr, scsc_mifram_ref *ref)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	if (!platform->mem) {
		dev_err(NULL, "Memory unmmaped\n");
		return -ENOMEM;
	}

	/* Check limits! */
	if (ptr >= (platform->mem + platform->mem_size)) {
		dev_err(NULL, "Unable to get pointer reference\n");
		return -ENOMEM;
	}

	*ref = (scsc_mifram_ref)((uintptr_t)ptr - (uintptr_t)platform->mem);

	return 0;
}

static void *platform_mif_get_mifram_ptr(struct scsc_mif_abs *interface, scsc_mifram_ref ref)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	//dev_dbg(NULL, "%s\n", __func__);

	if (!platform->mem) {
		dev_err(NULL, "Memory unmmaped\n");
		return NULL;
	}

	/* Check limits */
	if (ref >= 0 && ref < platform->mem_size) {
		return (void *)(platform->mem + (uintptr_t)ref);
	} else {
		return NULL;
	}
}

static uintptr_t platform_mif_get_mif_pfn(struct scsc_mif_abs *interface)
{
	return 0;
}

static struct device *platform_mif_get_mif_device(struct scsc_mif_abs *interface)
{
	dev_dbg(NULL, "%s\n", __func__);

	return NULL;
}

static void platform_mif_irq_clear(void)
{
	/* Implement if required */
}

//struct scsc_mif_abs *platform_mif_create(struct platform_device *pdev)
struct scsc_mif_abs *platform_mif_create()
{
	struct scsc_mif_abs *platform_if;
	struct platform_mif *platform = (struct platform_mif *)kmm_zalloc(sizeof(struct platform_mif));
	int err = 0;

	if (!platform) {
		return NULL;
	}

	platform_if = &platform->interface;

	platform_isr = platform;

	/* initialise interface structure */
	platform_if->destroy = platform_mif_destroy;
	platform_if->get_uid = platform_mif_get_uid;
	platform_if->reset = platform_mif_reset;
	platform_if->map = platform_mif_map;
	platform_if->unmap = platform_mif_unmap;
	platform_if->irq_bit_set = platform_mif_irq_bit_set;
	platform_if->irq_get = platform_mif_irq_get;
	platform_if->irq_bit_mask_status_get = platform_mif_irq_bit_mask_status_get;
	platform_if->irq_bit_clear = platform_mif_irq_bit_clear;
	platform_if->irq_bit_mask = platform_mif_irq_bit_mask;
	platform_if->irq_bit_unmask = platform_mif_irq_bit_unmask;
	platform_if->irq_reg_handler = platform_mif_irq_reg_handler;
	platform_if->irq_unreg_handler = platform_mif_irq_unreg_handler;
	platform_if->irq_reg_reset_request_handler = platform_mif_irq_reg_reset_request_handler;
	platform_if->irq_unreg_reset_request_handler = platform_mif_irq_unreg_reset_request_handler;
	platform_if->suspend_reg_handler = platform_mif_suspend_reg_handler;
	platform_if->suspend_unreg_handler = platform_mif_suspend_unreg_handler;
	platform_if->get_mbox_ptr = platform_mif_get_mbox_ptr;
	platform_if->get_mifram_ptr = platform_mif_get_mifram_ptr;
	platform_if->get_mifram_ref = platform_mif_get_mifram_ref;
	platform_if->get_mifram_pfn = platform_mif_get_mif_pfn;
	platform_if->get_mif_device = platform_mif_get_mif_device;
	platform_if->irq_clear = platform_mif_irq_clear;

	platform->r4_handler = platform_mif_irq_default_handler;
	platform->irq_dev = NULL;
	platform->reset_request_handler = platform_mif_irq_reset_request_default_handler;
	platform->irq_reset_request_dev = NULL;
	platform->suspend_handler = NULL;
	platform->resume_handler = NULL;
	platform->suspendresume_data = NULL;

	platform->mem_start = WIFI_IRAM_MEM_BASE << 12;
	platform->mem_size = WIFI_IMEM_SIZE << 12;

#ifdef CONFIG_SCSC_WLAN_LOGLVL_ALL
	dev_info(NULL, "platform->mem_start 0x%x platform->mem_size 0x%x\n", (u32)platform->mem_start, (u32)platform->mem_size);
#endif
	if (platform->mem_start == 0) {
		dev_warn(NULL, "platform->mem_start is 0");
	}

	if (platform->mem_size == 0) {
		/* We return return if mem_size is 0 as it does not make any sense.
		 * This may be an indication of an incorrect platform device binding. */
		dev_err(NULL, "platform->mem_size is 0");
		err = -EINVAL;
		goto error_exit;
	}

	platform->base = (void *)0x80050000;
	platform->reg_size = 0x180;
	platform->wlbt_irq[PLATFORM_MIF_MBOX].irq_num = IRQ_MAILBOX_WIFI;
	platform->wlbt_irq[PLATFORM_MIF_WDOG].irq_num = IRQ_WIFI_WB2AP_WDOG_RESET;

	/* Initialize spinlock */
	spin_lock_init(&platform->mif_spinlock);
	return platform_if;
error_exit:
	kmm_free(platform);
	return NULL;
}

int platform_mif_suspend(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	if (platform->suspend_handler) {
		return platform->suspend_handler(interface, platform->suspendresume_data);
	}

	return 0;
}

void platform_mif_resume(struct scsc_mif_abs *interface)
{
	struct platform_mif *platform = platform_mif_from_mif_abs(interface);

	if (platform->resume_handler) {
		platform->resume_handler(interface, platform->suspendresume_data);
	}
}
