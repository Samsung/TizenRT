/*
 * Copyright (c) 2015-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_TIMER_H
#define SYS_TIMER_H

#define CNT_CONTROL_BASE	0xB0002000
#define CNT_READ_BASE		0xA0000000

#define SYSTIM_CNTCR		(0x0)
#define SYSTIM_CNTSR		(0x4)
#define SYSTIM_CNTCV_RW		(0x8)
#define SYSTIM_CNTFID0		(0x20)

#define SYSTIM_CNTCV_RO		(0x0)

#define SYSTIM_CNT_EN_BIT	(0x1)

void sys_timer_enable(unsigned char en);




#endif /* GICV2_H */
