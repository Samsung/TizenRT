/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#ifndef ARM_GIC_H
#define ARM_GIC_H

//FENG Define to verision 2
#define CONFIG_GIC_VER 2

#include "sheipa.h"
#if CONFIG_GIC_VER > 2
#include "arm_gicv3_priv.h"
#endif

/*
 * GIC Distributor Interface
 */
#if !defined(GIC_DIST_BASE)
#error "GIC_DIST_BASE is not defined!\n"
#endif

#if !defined(GIC_CPU_BASE) && (CONFIG_GIC_VER <= 2)
#error "GIC_CPU_BASE is not defined!\n"
#endif

#define	IRQ_TYPE_LEVEL		0x0
#define	IRQ_TYPE_EDGE		0x1

/*
 * 0x000  Distributor Control Register
 * v1		ICDDCR
 * v2/v3	GICD_CTLR
 */
#define	GICD_CTLR			(GIC_DIST_BASE +   0x0)

/*
 * 0x004  Interrupt Controller Type Register
 * v1		ICDICTR
 * v2/v3	GICD_TYPER
 */
#define	GICD_TYPER			(GIC_DIST_BASE +   0x4)

/*
 * 0x008  Distributor Implementer Identification Register
 * v1		ICDIIDR
 * v2/v3	GICD_IIDR
 */
#define	GICD_IIDR			(GIC_DIST_BASE +   0x8)

/*
 * 0x080  Interrupt Group Registers
 * v1		ICDISRn
 * v2/v3	GICD_IGROUPRn
 */
#define	GICD_IGROUPRn		(GIC_DIST_BASE +  0x80)

/*
 * 0x100  Interrupt Set-Enable Reigsters
 * v1		ICDISERn
 * v2/v3	GICD_ISENABLERn
 */
#define	GICD_ISENABLERn		(GIC_DIST_BASE + 0x100)

/*
 * 0x180  Interrupt Clear-Enable Registers
 * v1		ICDICERn
 * v2/v3	GICD_ICENABLERn
 */
#define	GICD_ICENABLERn		(GIC_DIST_BASE + 0x180)

/*
 * 0x200  Interrupt Set-Pending Registers
 * v1		ICDISPRn
 * v2/v3	GICD_ISPENDRn
 */
#define	GICD_ISPENDRn		(GIC_DIST_BASE + 0x200)

/*
 * 0x280  Interrupt Clear-Pending Registers
 * v1		ICDICPRn
 * v2/v3	GICD_ICPENDRn
 */
#define	GICD_ICPENDRn		(GIC_DIST_BASE + 0x280)

/*
 * 0x300  Interrupt Set-Active Registers
 * v1		ICDABRn
 * v2/v3	GICD_ISACTIVERn
 */
#define	GICD_ISACTIVERn		(GIC_DIST_BASE + 0x300)

#if CONFIG_GIC_VER >= 2
/*
 * 0x380  Interrupt Clear-Active Registers
 * v2/v3	GICD_ICACTIVERn
 */
#define	GICD_ICACTIVERn		(GIC_DIST_BASE + 0x380)
#endif

/*
 * 0x400  Interrupt Priority Registers
 * v1		ICDIPRn
 * v2/v3	GICD_IPRIORITYRn
 */
#define	GICD_IPRIORITYRn	(GIC_DIST_BASE + 0x400)

/*
 * 0x800  Interrupt Processor Targets Registers
 * v1		ICDIPTRn
 * v2/v3	GICD_ITARGETSRn
 */
#define	GICD_ITARGETSRn		(GIC_DIST_BASE + 0x800)

/*
 * 0xC00  Interrupt Configuration Registers
 * v1		ICDICRn
 * v2/v3	GICD_ICFGRn
 */
#define	GICD_ICFGRn			(GIC_DIST_BASE + 0xc00)

/*
 * 0xF00  Software Generated Interrupt Register
 * v1		ICDSGIR
 * v2/v3	GICD_SGIR
 */
#define	GICD_SGIR			(GIC_DIST_BASE + 0xf00)

/*
 * GIC CPU Interface
 */

#if CONFIG_GIC_VER <= 2

/*
 * 0x0000  CPU Interface Control Register
 * v1		ICCICR
 * v2/v3	GICC_CTLR
 */
#define GICC_CTLR			(GIC_CPU_BASE +    0x0)

/*
 * 0x0004  Interrupt Priority Mask Register
 * v1		ICCPMR
 * v2/v3	GICC_PMR
 */
#define GICC_PMR			(GIC_CPU_BASE +    0x4)

/*
 * 0x0008  Binary Point Register
 * v1		ICCBPR
 * v2/v3	GICC_BPR
 */
#define GICC_BPR			(GIC_CPU_BASE +    0x8)

/*
 * 0x000C  Interrupt Acknowledge Register
 * v1		ICCIAR
 * v2/v3	GICC_IAR
 */
#define GICC_IAR			(GIC_CPU_BASE +    0xc)

/*
 * 0x0010  End of Interrupt Register
 * v1		ICCEOIR
 * v2/v3	GICC_EOIR
 */
#define GICC_EOIR			(GIC_CPU_BASE +   0x10)

/*
 * 0x1000  Deactive Interrupt Register
 * v1		ICCDIR
 * v2/v3	GICC_DIR
 */
#define GICC_DIR			(GIC_CPU_BASE +   0x1000)

/*
 * Helper Constants
 */
/* GICD_CTLR */
#define GICD_CTLR_ENABLEGRP0		(1 << 0)
#define GICD_CTLR_ENABLEGRP1		(1 << 1)
#define GICD_CTLR_ENABLE_MASK		(GICD_CTLR_ENABLEGRP0 | GICD_CTLR_ENABLEGRP1)

/* GICC_CTLR */
#define GICC_CTLR_ENABLEGRP0		(1 << 0)
#define GICC_CTLR_ENABLEGRP1		(1 << 1)
#define GICC_CTLR_ACKCTL			(1 << 2)

#define GICC_CTLR_ENABLE_MASK		(GICC_CTLR_ENABLEGRP0 | GICC_CTLR_ENABLEGRP1)

#if defined(CONFIG_GIC_V2)

#define GICC_CTLR_FIQBYPDISGRP0		(1 << 5)
#define GICC_CTLR_IRQBYPDISGRP0		(1 << 6)
#define GICC_CTLR_FIQBYPDISGRP1		(1 << 7)
#define GICC_CTLR_IRQBYPDISGRP1		(1 << 8)

#define GICC_CTLR_BYPASS_MASK		(GICC_CTLR_FIQBYPDISGRP0 | \
									 GICC_CTLR_IRQBYPDISGRP0 | \
									 GICC_CTLR_FIQBYPDISGRP1 | \
									 GICC_CTLR_IRQBYPDISGRP1)

#endif /* CONFIG_GIC_V2 */

/* GICD_SGIR */
#define GICD_SGIR_TGTFILT(x)		(x << 24)
#define GICD_SGIR_TGTFILT_CPULIST	GICD_SGIR_TGTFILT(0b00)
#define GICD_SGIR_TGTFILT_ALLBUTREQ	GICD_SGIR_TGTFILT(0b01)
#define GICD_SGIR_TGTFILT_REQONLY	GICD_SGIR_TGTFILT(0b10)

#define GICD_SGIR_CPULIST(x)		(x << 16)
#define GICD_SGIR_CPULIST_CPU(n)	GICD_SGIR_CPULIST(BIT(n))

#define GICD_SGIR_NSATT				(1 << 15)

#define GICD_SGIR_SGIINTID(x)		(x)

#endif /* CONFIG_GIC_VER <= 2 */

/* GICD_ICFGR */
#define GICD_ICFGR_MASK				((1 << 2) - 1)
#define GICD_ICFGR_TYPE				(1 << 1)

/* GICD_TYPER.ITLinesNumber 0:4 */
#define GICD_TYPER_ITLINESNUM_MASK	0x1f

/*
 * Common Helper Constants
 */

#define GIC_SPI_INT_BASE			32

#define GIC_NUM_INTR_PER_REG		32

#define GIC_NUM_CFG_PER_REG			16

#define GIC_NUM_PRI_PER_REG			4

/* GIC idle priority : value '0xff' will allow all interrupts */
#define GIC_IDLE_PRIO				0xff

/* Priority levels 0:255 */
#define GIC_PRI_MASK				0xff

/*
 * '0xa0'is used to initialize each interrtupt default priority.
 * This is an arbitrary value in current context.
 * Any value '0x80' to '0xff' will work for both NS and S state.
 * The values of individual interrupt and default has to be chosen
 * carefully if PMR and BPR based nesting and preemption has to be done.
 */
#define GIC_INT_DEF_PRI_X4			0xa0a0a0a0

/* GIC special interrupt id */
#define GIC_INTID_SPURIOUS			1023

/* Fixme: update from platform specific define or dt */
#define GIC_NUM_CPU_IF				1
/* Fixme: arch support need to provide api/macro in SMP implementation */
#define GET_CPUID					MPIDR_TO_CORE(GET_MPIDR())
#ifndef _ASMLANGUAGE

/*
 * GIC Driver Interface Functions
 */

/**
 * @brief Initialise ARM GIC driver
 *
 * @return 0 if successful
 */
int arm_gic_init(void);
int arm_gic_init_secondary(void);

/**
 * @brief Enable interrupt
 *
 * @param irq interrupt ID
 */
void arm_gic_irq_enable(uint32_t irq);

/**
 * @brief Disable interrupt
 *
 * @param irq interrupt ID
 */
void arm_gic_irq_disable(uint32_t irq);

/**
 * @brief Check if an interrupt is enabled
 *
 * @param irq interrupt ID
 * @return Returns true if interrupt is enabled, false otherwise
 */
int arm_gic_irq_is_enabled(uint32_t irq);

/**
 * @brief Set interrupt priority
 *
 * @param irq interrupt ID
 * @param prio interrupt priority
 * @param flags interrupt flags
 */
void arm_gic_irq_set_priority(
	uint32_t irq, uint32_t prio, uint32_t flags);

/**
 * @brief Deactive interrupt ID
 *
 * @param irq interrupt ID
 */
void arm_gic_deactive_irq(uint32_t irq);

/**
 * @brief Get active interrupt ID
 *
 * @return Returns the ID of an active interrupt
 */
uint32_t arm_gic_get_active(void);

/**
 * @brief Signal end-of-interrupt
 *
 * @param irq interrupt ID
 */
void arm_gic_eoi(uint32_t irq);

/**
 * @brief Raise software interrupt
 *
 * @param target cpu
 * @param irq interrupt ID
 */
void arm_gic_raise_softirq(uint32_t cpu, uint32_t irq);

/**
 * @brief Clear software interrupt
 *
 * @param irq interrupt ID
 */
void arm_gic_clear_softirq(uint32_t irq);
void arm_gic_set_pending_irq(uint32_t irq);
int arm_gic_irq_is_pending(uint32_t irq);
void arm_gic_clear_pending_irq(uint32_t irq);

void arm_gic_freq_switch(void);
void arm_gic_freq_restore(void);
uint32_t arm_gic_get_freq_flag(void);
#endif /* !_ASMLANGUAGE */

#endif
