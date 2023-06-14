/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#ifndef ARCH_TIMER_H
#define ARCH_TIMER_H

#define ARM_ARCH_TIMER_IRQ		(11 + 16)

#define CNTV_CTL_ENABLE		((1) << 0)
#define CNTV_CTL_IMASK		((1) << 1)
#define CNTV_CTL_ISTATUS	((1) << 2)

#ifndef _ASMLANGUAGE

#if defined( __ICCARM__ )
#define __asm__ __asm
#endif

static inline void arm_arch_timer_set_compare(uint64_t val) /*write CNTV_CVAL: virtual timer CompareValue register*/
{
	__asm__ volatile("mcrr p15, 3, %Q0, %R0, c14\n\t"
					 : : "r"(val) : "memory");
}

static inline uint64_t arm_arch_timer_compare(void)
{
	uint64_t cntvcval;

	__asm__ volatile("mrrc p15, 3, %Q0, %R0, c14\n\t"
					 : "=r"(cntvcval) : : "memory");

	return cntvcval;
}

static inline void arm_arch_timer_enable(unsigned char enable) /*write CNTV_CTL: virtual timer Control register*/
{
	uint32_t cntv_ctl = 0;

	//__asm__ volatile("mrc p15, 0, %0, c14, c3, 1\n\t"
	//				 : "=r"(cntv_ctl) :  : "memory");

	cntv_ctl &= ~CNTV_CTL_IMASK;
	if (enable) {
		cntv_ctl |= CNTV_CTL_ENABLE;
	} else {
		cntv_ctl &= ~CNTV_CTL_ENABLE;
	}

	__asm__ volatile("mcr p15, 0, %0, c14, c3, 1\n\t"
					 : : "r"(cntv_ctl) : "memory");
}

static inline uint64_t arm_arch_timer_count(void) /*read CNTVCT: virtual timer count register*/
{
	uint64_t cntvct;

	__asm__ volatile("mrrc p15, 1, %Q0, %R0, c14\n\t"
					 : "=r"(cntvct) : : "memory");

	return cntvct;
}

static inline uint32_t arm_arch_timer_condition_met(void)
{
	uint32_t cntv_ctl;

	__asm__ volatile("mrc p15, 0, %0, c14, c3, 1\n\t"
					 : "=r"(cntv_ctl) :  : "memory");

	return (cntv_ctl & CNTV_CTL_ISTATUS);
}

static inline char arm_arch_timer_check_enable(void) /*check CNTV_CTL: virtual timer Control register*/
{
	uint32_t cntv_ctl;

	__asm__ volatile("mrc p15, 0, %0, c14, c3, 1\n\t"
					 : "=r"(cntv_ctl) :  : "memory");

	if (cntv_ctl & CNTV_CTL_ENABLE) {
		return 1;
	} else {
		return 0;
	}
}
#endif /* !_ASMLANGUAGE */

#endif
