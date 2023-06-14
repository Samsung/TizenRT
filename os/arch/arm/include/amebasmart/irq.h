/****************************************************************************
 * arch/arm/include/amebasmart/irq.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* This file should never be included directly but, rather,
 * only indirectly through tinyara/irq.h
 */

#ifndef __ARCH_ARM_INCLUDE_AMEBASMART_IRQ_H
#define __ARCH_ARM_INCLUDE_AMEBASMART_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* The Global Interrupt Controller (GIC) collects up to 128 interrupt
 * requests from all i.MX 6Dual/6Quad sources and provides an interface to
 * each of the CPU cores.
 *
 * The first 32 interrupts are used for interrupts that are private to the
 * CPUs interface. interrupts besides the private CPU are also hooked up to
 * the GIC in the same order.
 *
 * Each interrupt can be configured as a normal or a secure interrupt.
 * Software force registers and software priority masking are also
 * supported. The following table describes the ARM interrupt sources.
 */

/* Private Peripheral Interrupts (PPI) **************************************/

/* Each Cortex-A9 processor has private interrupts, ID0-ID15, that can only
 * be triggered by software. These interrupts are aliased so that there is
 * no requirement for a requesting Cortex-A9 processor to determine its own
 * CPU ID when it deals with SGIs. The priority of an SGI depends on the
 * value set by the receiving Cortex-A9 processor in the banked SGI priority
 * registers, not the priority set by the sending Cortex-A9 processor.
 */


/******  Processor Exceptions Numbers ********/

#define	AMEBASMART_IRQ_SGI0				(0)	/* software generated interrupt 0 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI1				(1)	/* software generated interrupt 1 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI2				(2)	/* software generated interrupt 2 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI3				(3)	/* software generated interrupt 3 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI4				(4)	/* software generated interrupt 4 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI5				(5)	/* software generated interrupt 5 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI6				(6)	/* software generated interrupt 6 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI7				(7)	/* software generated interrupt 7 (recommended non-secure) */
#define	AMEBASMART_IRQ_SGI8				(8)	/* software generated interrupt 8 (recommended secure) */
#define	AMEBASMART_IRQ_SGI9				(9)	/* software generated interrupt 9 (recommended secure) */
#define	AMEBASMART_IRQ_SGI10			(10)	/* software generated interrupt 10 (recommended secure) */
#define	AMEBASMART_IRQ_SGI11			(11)	/* software generated interrupt 11 (recommended secure) */
#define	AMEBASMART_IRQ_SGI12			(12)	/* software generated interrupt 12 (recommended secure) */
#define	AMEBASMART_IRQ_SGI13			(13)	/* software generated interrupt 13 (recommended secure) */
#define	AMEBASMART_IRQ_SGI14			(14)	/* software generated interrupt 14 (recommended secure) */
#define	AMEBASMART_IRQ_SGI15			(15)	/* software generated interrupt 15 (recommended secure) */

#define	AMEBASMART_IRQ_PPI6				(25)	/* Virtual maintenance interrupt. */
#define	AMEBASMART_IRQ_PPI5				(26)	/* Hypervisor timer event. */
#define	AMEBASMART_IRQ_PPI4				(27)	/* Virtual timer event. */
#define	AMEBASMART_IRQ_PPI0				(28)	/* Legacy nFIQ signal. Not used. */
#define	AMEBASMART_IRQ_PPI1				(29)	/* Secure physical timer event. */
#define	AMEBASMART_IRQ_PPI2				(30)	/* Non-secure physical timer event. */
#define	AMEBASMART_IRQ_PPI3				(31)	/* Legacy nIRQ signal. Not used. */

/* External interrupts (vectors >= 32).  These definitions are chip-specific */

#define AMEBASMART_IRQ_FIRST         (32) /* Vector number of the first external interrupt */

/******  Specific Interrupt Numbers ************/
#define	AMEBASMART_IRQ_AON_TIM			(AMEBASMART_IRQ_FIRST +  95)
#define	AMEBASMART_IRQ_TIMER0			(AMEBASMART_IRQ_FIRST +  0)
#define	AMEBASMART_IRQ_TIMER1			(AMEBASMART_IRQ_FIRST +  1)
#define	AMEBASMART_IRQ_TIMER2			(AMEBASMART_IRQ_FIRST +  2)
#define	AMEBASMART_IRQ_TIMER3			(AMEBASMART_IRQ_FIRST +  3)
#define	AMEBASMART_IRQ_TIMER4			(AMEBASMART_IRQ_FIRST +  4)
#define	AMEBASMART_IRQ_TIMER5			(AMEBASMART_IRQ_FIRST +  5)
#define	AMEBASMART_IRQ_TIMER6			(AMEBASMART_IRQ_FIRST +  6)
#define	AMEBASMART_IRQ_TIMER7			(AMEBASMART_IRQ_FIRST +  7)
#define	AMEBASMART_IRQ_UART_LOG			(AMEBASMART_IRQ_FIRST +  8)
#define	AMEBASMART_IRQ_GPIOA			(AMEBASMART_IRQ_FIRST +  9)
#define	AMEBASMART_IRQ_GPIOB			(AMEBASMART_IRQ_FIRST +  10)
#define	AMEBASMART_IRQ_GPIOC			(AMEBASMART_IRQ_FIRST +  11)
#define	AMEBASMART_IRQ_RTC				(AMEBASMART_IRQ_FIRST +  12)
#define	AMEBASMART_IRQ_CTOUCH			(AMEBASMART_IRQ_FIRST +  13)
#define	AMEBASMART_IRQ_ADC				(AMEBASMART_IRQ_FIRST +  14)
#define	AMEBASMART_IRQ_ADC_COMP			(AMEBASMART_IRQ_FIRST +  15)
#define	AMEBASMART_IRQ_BOR				(AMEBASMART_IRQ_FIRST +  16)
#define	AMEBASMART_IRQ_PWR_DOWN			(AMEBASMART_IRQ_FIRST +  17)
#define	AMEBASMART_IRQ_VADBT_OR_VADPC	(AMEBASMART_IRQ_FIRST +  18)
#define	AMEBASMART_IRQ_SPI_FLASH		(AMEBASMART_IRQ_FIRST +  19)
#define	AMEBASMART_IRQ_DEBUG_TIMER		(AMEBASMART_IRQ_FIRST +  20)
#define	AMEBASMART_IRQ_THERMAL			(AMEBASMART_IRQ_FIRST +  21)
#define	AMEBASMART_IRQ_I2C0				(AMEBASMART_IRQ_FIRST +  22)
#define	AMEBASMART_IRQ_RSIP				(AMEBASMART_IRQ_FIRST +  23)
#define	AMEBASMART_IRQ_IPC_AP			(AMEBASMART_IRQ_FIRST +  24)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL0	(AMEBASMART_IRQ_FIRST +  25)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL1	(AMEBASMART_IRQ_FIRST +  26)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL2	(AMEBASMART_IRQ_FIRST +  27)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL3	(AMEBASMART_IRQ_FIRST +  28)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL4	(AMEBASMART_IRQ_FIRST +  29)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL5	(AMEBASMART_IRQ_FIRST +  30)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL6	(AMEBASMART_IRQ_FIRST +  31)
#define	AMEBASMART_IRQ_GDMA0_CHANNEL7	(AMEBASMART_IRQ_FIRST +  32)
#define	AMEBASMART_IRQ_LCDC				(AMEBASMART_IRQ_FIRST +  33)
#define	AMEBASMART_IRQ_WL_DMA			(AMEBASMART_IRQ_FIRST +  34)
#define	AMEBASMART_IRQ_WL_PROTOCOL		(AMEBASMART_IRQ_FIRST +  35)
#define	AMEBASMART_IRQ_AES				(AMEBASMART_IRQ_FIRST +  36)
#define	AMEBASMART_IRQ_SHA				(AMEBASMART_IRQ_FIRST +  37)
#define	AMEBASMART_IRQ_SDIO_HOST		(AMEBASMART_IRQ_FIRST +  38)
#define	AMEBASMART_IRQ_USB_OTG			(AMEBASMART_IRQ_FIRST +  39)
#define	AMEBASMART_IRQ_SPI0				(AMEBASMART_IRQ_FIRST +  40)
#define	AMEBASMART_IRQ_SPI1				(AMEBASMART_IRQ_FIRST +  41)
#define	AMEBASMART_IRQ_RSVD				(AMEBASMART_IRQ_FIRST +  42)
#define	AMEBASMART_IRQ_RSA				(AMEBASMART_IRQ_FIRST +  43)
#define	AMEBASMART_IRQ_ECDSA			(AMEBASMART_IRQ_FIRST +  44)
#define	AMEBASMART_IRQ_ED25519			(AMEBASMART_IRQ_FIRST +  45)
#define	AMEBASMART_IRQ_PSRAMC			(AMEBASMART_IRQ_FIRST +  46)
#define	AMEBASMART_IRQ_DDR				(AMEBASMART_IRQ_FIRST +  47)
#define	AMEBASMART_IRQ_RXI300_HP		(AMEBASMART_IRQ_FIRST +  48)
#define	AMEBASMART_IRQ_IR				(AMEBASMART_IRQ_FIRST +  49)
#define	AMEBASMART_IRQ_UART0			(AMEBASMART_IRQ_FIRST +  50)
#define	AMEBASMART_IRQ_UART1			(AMEBASMART_IRQ_FIRST +  51)
#define	AMEBASMART_IRQ_UART2			(AMEBASMART_IRQ_FIRST +  52)
#define	AMEBASMART_IRQ_UART3_BT			(AMEBASMART_IRQ_FIRST +  53)
#define	AMEBASMART_IRQ_TRNG				(AMEBASMART_IRQ_FIRST +  54)
#define	AMEBASMART_IRQ_I2C1				(AMEBASMART_IRQ_FIRST +  55)
#define	AMEBASMART_IRQ_I2C2				(AMEBASMART_IRQ_FIRST +  56)
#define	AMEBASMART_IRQ_TIMER8			(AMEBASMART_IRQ_FIRST +  57)
#define	AMEBASMART_IRQ_TIMER9			(AMEBASMART_IRQ_FIRST +  58)
#define	AMEBASMART_IRQ_WDG1				(AMEBASMART_IRQ_FIRST +  59)
#define	AMEBASMART_IRQ_WDG2				(AMEBASMART_IRQ_FIRST +  60)
#define	AMEBASMART_IRQ_WDG3				(AMEBASMART_IRQ_FIRST +  61)
#define	AMEBASMART_IRQ_WDG4				(AMEBASMART_IRQ_FIRST +  62)
#define	AMEBASMART_IRQ_SPORT0			(AMEBASMART_IRQ_FIRST +  63)
#define	AMEBASMART_IRQ_SPORT1			(AMEBASMART_IRQ_FIRST +  64)
#define	AMEBASMART_IRQ_SPORT2			(AMEBASMART_IRQ_FIRST +  65)
#define	AMEBASMART_IRQ_SPORT3			(AMEBASMART_IRQ_FIRST +  66)
#define	AMEBASMART_IRQ_BT_SCB			(AMEBASMART_IRQ_FIRST +  67)
#define	AMEBASMART_IRQ_LEDC				(AMEBASMART_IRQ_FIRST +  68)
#define	AMEBASMART_IRQ_PMUIRQ0			(AMEBASMART_IRQ_FIRST +  69)
#define	AMEBASMART_IRQ_MIPI_DSI			(AMEBASMART_IRQ_FIRST +  70)
#define	AMEBASMART_IRQ_AXIERIRQ			(AMEBASMART_IRQ_FIRST +  71)
#define	AMEBASMART_IRQ_AES_S			(AMEBASMART_IRQ_FIRST +  72)
#define	AMEBASMART_IRQ_SHA_S			(AMEBASMART_IRQ_FIRST +  73)
#define	AMEBASMART_IRQ_AON_WAKEPIN		(AMEBASMART_IRQ_FIRST +  74)
#define	AMEBASMART_IRQ_ZIGBEE			(AMEBASMART_IRQ_FIRST +  75)
#define	AMEBASMART_IRQ_BT_WAKE_HOST		(AMEBASMART_IRQ_FIRST +  76)
#define	AMEBASMART_IRQ_CTIIRQ			(AMEBASMART_IRQ_FIRST +  77)
#define	AMEBASMART_IRQ_TIMER10			(AMEBASMART_IRQ_FIRST +  78)
#define	AMEBASMART_IRQ_TIMER11			(AMEBASMART_IRQ_FIRST +  79)
#define	AMEBASMART_IRQ_TIMER12			(AMEBASMART_IRQ_FIRST +  80)
#define	AMEBASMART_IRQ_TIMER13			(AMEBASMART_IRQ_FIRST +  81)

#define NR_IRQS                 130  /* Total number of interrupts */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline functions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_AMEBASMART_IRQ_H */