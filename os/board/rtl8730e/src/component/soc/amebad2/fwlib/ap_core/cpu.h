/*
 * Copyright (c) 2021 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#ifndef CPU_H
#define CPU_H

#include "sys_io.h"

/* ICC_xxx */
#define __stringify_1(x...)		#x
#define __stringify(x...)		__stringify_1(x)

#define __read_sysreg(r, w, c, t) ({		\
	t __val;								\
	asm volatile(r " " c : "=r" (__val));	\
	__val;							\
})
#define read_sysreg(...)		__read_sysreg(__VA_ARGS__)

#define __write_sysreg(v, r, w, c, t)		\
	asm volatile(w " " c : : "r" ((t)(v)))
#define write_sysreg(v, ...)	__write_sysreg(v, __VA_ARGS__)

#define __ACCESS_CP15(CRn, Op1, CRm, Op2)	\
	"mrc", "mcr", __stringify(p15, Op1, %0, CRn, CRm, Op2), u32_t

#define __ACCESS_CP15_64(Op1, CRm)			\
	"mrrc", "mcrr", __stringify(p15, Op1, %Q0, %R0, CRm), u64_t

#define ICC_EOIR1				__ACCESS_CP15(c12, 0, c12, 1)
#define ICC_DIR					__ACCESS_CP15(c12, 0, c11, 1)
#define ICC_IAR1				__ACCESS_CP15(c12, 0, c12, 0)
#define ICC_SGI1R				__ACCESS_CP15_64(0, c12)
#define ICC_PMR					__ACCESS_CP15(c4,  0, c6,  0)
#define ICC_CTLR				__ACCESS_CP15(c12, 0, c12, 4)
#define ICC_SRE					__ACCESS_CP15(c12, 0, c12, 5)
#define ICC_IGRPEN1				__ACCESS_CP15(c12, 0, c12, 7)
#define ICC_BPR1				__ACCESS_CP15(c12, 0, c12, 3)

/* mpidr */
#define SYS_MPIDR				__ACCESS_CP15(c0, 0, c0, 5)

#define MPIDR_AFFLVL_MASK		0xff

#define MPIDR_AFF0_SHIFT		0
#define MPIDR_AFF1_SHIFT		8
#define MPIDR_AFF2_SHIFT		16
#define MPIDR_AFF3_SHIFT		32

#define MPIDR_AFFLVL(mpidr, aff_level)		\
		(((mpidr) >> MPIDR_AFF##aff_level##_SHIFT) & MPIDR_AFFLVL_MASK)

#define GET_MPIDR()				read_sysreg(SYS_MPIDR)
#define MPIDR_TO_CORE(mpidr)	MPIDR_AFFLVL(mpidr, 1)

#endif
