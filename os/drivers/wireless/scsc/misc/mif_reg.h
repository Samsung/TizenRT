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
#ifndef __MIF_REG_H
#define __MIF_REG_H

/*****************************/
/* PCIE register definitions */
/*****************************/
/* Maximum number of mbox register (issr) to allocate. Should match with FW configuraton */
#define MAX_NUM_MBOX 64

/* Number of emulated mailboxes
 * This number should match mif: mifmboxman.h but is up to the
 * the specific PCIe implementation  */
#define NUM_MBOX 4				/* FW write to hardware appears to have only 4 */

#if NUM_MBOX > MAX_NUM_MBOX
#error "NUM_MBOX >  MAX_NUM_MBOX"
#endif

/* Match platform driver register naming */
struct scsc_mbox_s {
	volatile u32 issr[NUM_MBOX];	/* MAILBOXES */
	volatile u32 padding[MAX_NUM_MBOX - NUM_MBOX];	/* MAILBOXES */
	volatile u32 intmr0;		/* R4 Int Mask  */
	volatile u32 intsr0;		/* R4 Int Status register */
	volatile u32 intmr1;		/* AP Int Mask */
	volatile u32 intsr1;		/* AP Int Status register */
	volatile u32 intmr2;		/* M4 Int Mask */
	volatile u32 intsr2;		/* M4 Int Status register  */
};

struct peterson_mutex {
	u32 flag[2];
	u32 turn;
};

#define PMU_ALIVE_BASE          0x80090000	/* From s5jt200_memorymap.h */

#define MBOX_OFFSET             0x3ffc00
/* Leave space to allocate emulated registers */
#define P_OFFSET_AP             0x3ffe00
#define P_OFFSET_R4             0x3fff00

/*********************************/
/* PLATFORM register definitions */
/*********************************/
#define NUM_MBOX_PLAT   8
#define NUM_SEMAPHORE   12

#define MAILBOX_WLBT_BASE       0x0000
#define MAILBOX_WLBT_REG(r)     (MAILBOX_WLBT_BASE + (r))
#define MCUCTRL                 0x000	/* MCU Controller Register */
/* R0 [31:16]  - Int FROM R4/M4 */
#define INTGR0                  0x008	/* Interrupt Generation Register 0 */
#define INTCR0                  0x00C	/* Interrupt Clear Register 0 */
#define INTMR0                  0x010	/* Interrupt Mask Register 0 */
#define INTSR0                  0x014	/* Interrupt Status Register 0 */
#define INTMSR0                 0x018	/* Interrupt Mask Status Register 0 */
/* R1 [15:0]  - Int TO R4 */
#define INTGR1                  0x01c	/* Interrupt Generation Register 1 */
#define INTCR1                  0x020	/* Interrupt Clear Register 1 */
#define INTMR1                  0x024	/* Interrupt Mask Register 1 */
#define INTSR1                  0x028	/* Interrupt Status Register 1 */
#define INTMSR1                 0x02c	/* Interrupt Mask Status Register 1 */
/* R2 [15:0]  - Int TO M4 */
#define INTGR2                  0x030	/* Interrupt Generation Register 2 */
#define INTCR2                  0x034	/* Interrupt Clear Register 2 */
#define INTMR2                  0x038	/* Interrupt Mask Register 2 */
#define INTSR2                  0x03c	/* Interrupt Status Register 2 */
#define INTMSR2                 0x040	/* Interrupt Mask Status Register 2 */
#define MIF_INIT                0x04c	/* MIF_init */
#define IS_VERSION              0x050	/* Version Information Register */
#define ISSR_BASE               0x080	/* IS_Shared_Register Base address */
#define ISSR(r)                 (ISSR_BASE + (4 * (r) * 2))	/* Factor of 2 correponds to NUM_MBOX 4 to skip alternate ones */
#define SEMAPHORE_BASE          0x180	/* IS_Shared_Register Base address */
#define SEMAPHORE(r)            (SEMAPHORE_BASE + (4 * (r)))
#define SEMA0CON                0x1c0
#define SEMA0STATE              0x1c8

/* POWER */
/* Page 594 datasheet */
/* Base Address - 0x11C8_0000 */
#define WIFI_CTRL_NS            ((void *)(PMU_ALIVE_BASE + 0x0140))	/* WIFI Control SFR non-secure */
#define WIFI_PWRON              BIT(1)
#define WIFI_RESET_SET          BIT(2)
#define WIFI_ACTIVE_EN          BIT(5)	/* Enable of WIFI_ACTIVE_REQ */
#define WIFI_ACTIVE_CLR         BIT(6)	/* WIFI_ACTIVE_REQ is clear internally on WAKEUP */
#define WIFI_RESET_REQ_EN       BIT(7)	/* 1:enable, 0:disable  Enable of WIFI_RESET_REQ */
#define WIFI_RESET_REQ_CLR      BIT(8)	/* WIFI_RESET_REQ is clear internally on WAKEUP */
#define MASK_WIFI_PWRDN_DONE    BIT(9)	/* 1:mask, 0 : pass RTC clock out enable to WIFI
										 * This mask WIFI_PWRDN_DONE come in from WIFI.
										 * If MASK_WIFI_PWRDN_DONE = 1, WIFI enter to DOWN
										 * state without checking WIFI_PWRDN_DONE*/

#define WIFI_CTRL_S             ((void *)(PMU_ALIVE_BASE + 0x0144))	/* WIFI Control SFR secure */
#define WIFI_START              BIT(3)	/* WIFI Reset release control  If WIFI_START = 1,
										 * WIFI exit from DOWN state and go to UP state.
										 * If this field is set to high (WIFI_START = 1)
										 * WIFI state can go to UP state. This signal can be
										 * auto-clear by DIRECTWR at UP */

#define WIFI_STAT               ((void *)(PMU_ALIVE_BASE + 0x0148))	/* Indicate whether WIFI uses MIF domain */
#define WIFI_DEBUG              ((void *)(PMU_ALIVE_BASE + 0x014c))	/* MIF sleep, wakeup debugging control */
/* Page 1574 datasheet */
//#define PMU_ALIVE_BASE          0x0000
//#define PMU_ALIVE_REG(r)        (PMU_ALIVE_BASE + (r))
#define WLBT2AP_MIF_ACCESS_WIN0 0x0154	/* ACCESS_CONTROL_PERI_IP */
#define WLBT2AP_MIF_ACCESS_WIN1 0x0158	/* ACCESS_CONTROL_PERI_IP */
#define WLBT2AP_MIF_ACCESS_WIN2 0x015a	/* ACCESS_CONTROL_PERI_IP */
#define WLBT2AP_MIF_ACCESS_WIN3 0x0160	/* ACCESS_CONTROL_PERI_IP */
//#define WLBT_BOOT_TEST_RST_CFG  0x0168 /* WLBT_IRAM_BOOT_OFFSET */
/* WLBT_IRAM_BOOT_TEST */
/* WLBT2AP_PERI_PROT2 */

#define WLBT2AP_PERI_ACCESS_WIN 0x016c	/* WLBT2AP_PERI_ACCESS_END - WLBT2AP_PERI_ACCESS_START */
#define WIFI2AP_MODAPIF_CONFIG  0x0170	/* WLBT2AP_PERI_ACCESS_END - WLBT2AP_PERI_ACCESS_START */
#define WIFI2AP_QOS             0x0170	/* RT */

#define SYS_REG_BASE                    0x800A0000
#define WIFI_BOOT_TEST_RST_CONFIG       ((void *)(SYS_REG_BASE + 0x434))
#define WIFI2AP_MEM_CONFIG0             ((void *)(SYS_REG_BASE + 0x0440))	/* Control WIFI_MEM_SIZE */
#define WIFI2AP_MEM_CONFIG1             ((void *)(SYS_REG_BASE + 0x0438))	/* Control WIFI_MEM_BA0 */
#define WIFI2AP_MEM_CONFIG2             ((void *)(SYS_REG_BASE + 0x043c))	/* Control WIFI_MEM_BA1 */

#define WIFI2AP_PMU_SPARE0      ((void *)(PMU_ALIVE_BASE + 0x0900))	/* Control PMU_SPARE0 */

/* Wi-Fi Firmware load address */
#define WIFI_IRAM_MEM_BASE              0x2020	/* Base address for iRAM mappings */
#define WIFI_IMEM_SIZE                  0x800	/* Mapped flash region up to the top of the iram_shared block (8 MB) */

/* Power down registers */
#define CLEANY_BUS_WIFI_SYS_PWR_REG ((void *)(PMU_ALIVE_BASE + 0x1324))	/* Control power state in LOWPWR mode 1 - on, 0 - down */
#define LOGIC_RESET_WIFI_SYS_PWR_REG ((void *)(PMU_ALIVE_BASE + 0x1328))	/* Control power state in LOWPWR mode 1 - on, 0 - down */
#define TCXO_GATE_WIFI_SYS_PWR_REG ((void *)(PMU_ALIVE_BASE + 0x132c))	/* Control power state in LOWPWR mode 1 - on, 0 - down */
#define RESET_ASB_WIFI_SYS_PWR_REG ((void *)(PMU_ALIVE_BASE + 0x1330))	/* Control power state in LOWPWR mode 1 - on, 0 - down */

#define CENTRAL_SEQ_WIFI_CONFIGURATION ((void *)(PMU_ALIVE_BASE + 0x0380))	/* bit 16. Decides whether system-level low-power mode */
// * is used HIGH: System-level Low-Power mode
// * disabled. LOW: System-level Low-Power mode
// * enabled. When system enters low-power mode,
// * this field is automatically cleared to HIGH.

#define CENTRAL_SEQ_WIFI_STATUS ((void *)(PMU_ALIVE_BASE + 0x0384))	/* 23:16  Check statemachine status */
#define STATES                  0xff0000

#define SYS_PWR_CFG             BIT(0)
#define SYS_PWR_CFG_2           (BIT(0) | BIT(1))
#define SYS_PWR_CFG_16          BIT(16)

/***** Interrupts ********
 *
 * - MBOX
 * - WIFI_ACTIVE (pag 553)
 *   comes from BLK_WIFI. Initial value is low and then this value becomes high after WIFI booting. If
 *   some problem occurs within WIFI, WIFI_ACTIVE can be low by WIFI CPU. AP CPU detects that WIFI_ACTIVE is
 *   low after WIFI_ACTIVE is high. And WIFI_ACTIVE detected goes to GIC's interrupt source. At ISR, AP CPU
 *   makes wake source and interrupt clear as setting WIFI_CTRL__WIFI_ACTIVE_CLR. WIFI_ACTIVE_CLR is auto
 *   clear by direct-write function.
 *
 * - WIFI_RESET_REQ (pag 554)
 *   WIFI can request WIFI reset only by WIFI_RESET_REQ. If WIFI_RESET_REQ is asserted, AP PMU detects it as
 *   wakeup source and interrupt source. At ISR, AP CPU makes wakeup source clear as setting
 *   WIFI_CTRL__CP_RESET_REQ_CLR. But, interrupt can be not clear because the interrupt goes to GIC directly
 *   from WIFI. (It use make function within GIC) WIFI_RESET_REQ_CLR is auto clear by direct-write function.
 */

#endif							/* __MIF_REG_H */
