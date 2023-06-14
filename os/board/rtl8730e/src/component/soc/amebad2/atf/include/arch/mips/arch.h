/*
 * Copyright (c) 2017, Realtek. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __ARCH_H__
#define __ARCH_H__

/*
 * Coprocessor 0 registers
 */
#define CP0_COUNT 		$9
#define CP0_COMPARE		$11
#define CP0_STATUS		$12
#define CP0_CAUSE		$13
#define CP0_EPC			$14
#define CP0_PRID		$15


#define CAUSEB_DC		27
#define CAUSEF_DC		(1UL << 27)

/*
 * RLX CP0 registers
 */
#define LXCP0_ESTATUS		$0
#define LXCP0_ECAUSE		$1
#define LXCP0_INTVEC		$2
#define LXCP0_CVSTAG		$3
#define LXCP0_BPCTL		$4
#define LXCP0_WMPCTL		$5
#define LXCP0_WMPSTATUS		$6
#define LXCP0_WMPVADDR		$7
#define LXCP0_TLPTR		$8
#define LXCP0_WMPEXTRAMASK	$19
#define LXCP0_CCTL		$20
#define LXCP0_TR1		$24
#define LXCP0_TR2		$25

#define CCTL_DInval		0x00000001
#define CCTL_IInval		0x00000002
#define CCTL_IMEM0FILL		0x00000010
#define CCTL_IMEM0OFF		0x00000020
#define CCTL_IMEM0ON		0x00000040
#define CCTL_DWB		0x00000100
#define CCTL_DWBInval		0x00000200
#define CCTL_DMEM0ON		0x00000400
#define CCTL_DMEM0OFF		0x00000800

/*
 * RLX status register bits
 */
#define EST0_IM			0x00ff0000
#define ESTATUSF_IP0		(1UL << 16)
#define ESTATUSF_IP1		(1UL << 17)
#define ESTATUSF_IP2		(1UL << 18)
#define ESTATUSF_IP3		(1UL << 19)
#define ESTATUSF_IP4		(1UL << 20)
#define ESTATUSF_IP5		(1UL << 21)
#define ESTATUSF_IP6		(1UL << 22)
#define ESTATUSF_IP7		(1UL << 23)

#define ECAUSEF_IP		(255UL << 16)
#define ECAUSEF_IP0		(1UL << 16)
#define ECAUSEF_IP1		(1UL << 17)
#define ECAUSEF_IP2		(1UL << 18)
#define ECAUSEF_IP3		(1UL << 19)
#define ECAUSEF_IP4		(1UL << 20)
#define ECAUSEF_IP5		(1UL << 21)
#define ECAUSEF_IP6		(1UL << 22)
#define ECAUSEF_IP7		(1UL << 23)

/*
 * WMPU CTRL register
 */
#define WMPCTLF_MS		(1UL << 0)
#define WMPCTLF_KE		(1UL << 1)

#define WMPCTLF_EE0		(1UL << 16)
#define WMPCTLF_EE1		(1UL << 17)
#define WMPCTLF_EE2		(1UL << 18)
#define WMPCTLF_EE3		(1UL << 19)
#define WMPCTLF_EE4		(1UL << 20)
#define WMPCTLF_EE5		(1UL << 21)
#define WMPCTLF_EE6		(1UL << 22)
#define WMPCTLF_EE7		(1UL << 23)

/*
 * WMPU status register
 */
#define WMPSTATUSF_II		(1UL << 0)
#define WMPSTATUSF_DR		(1UL << 1)
#define WMPSTATUSF_DW		(1UL << 2)

#define WMPSTATUSF_EM0		(1UL << 16)
#define WMPSTATUSF_EM1		(1UL << 17)
#define WMPSTATUSF_EM2		(1UL << 18)
#define WMPSTATUSF_EM3		(1UL << 19)
#define WMPSTATUSF_EM4		(1UL << 20)
#define WMPSTATUSF_EM5		(1UL << 21)
#define WMPSTATUSF_EM6		(1UL << 22)
#define WMPSTATUSF_EM7		(1UL << 23)

#ifndef __ASSEMBLY__
#if defined(__m4281) || defined(__m5281)
#define __sync()                                \
        __asm__ __volatile__(                   \
                ".set   push\n\t"               \
                ".set   noreorder\n\t"          \
                "sync\n\t"                      \
                ".set   pop"                    \
                : /* no output */               \
                : /* no input */                \
                : "memory")
#else
#define __sync()        __asm__ __volatile__("" : : : "memory")
#endif

/*
 * Read C0 coprocessor register.
 */
#define __read_32bit_c0_register(source, sel)                           \
({ unsigned int __res;                                                  \
        if (sel == 0)                                                   \
                __asm__ __volatile__(                                   \
                        "mfc0\t%0, " #source "\n\t"                     \
                        : "=r" (__res));                                \
        else                                                            \
                __asm__ __volatile__(                                   \
                        ".set\tmips32\n\t"                              \
                        "mfc0\t%0, " #source ", " #sel "\n\t"           \
                        ".set\tmips0\n\t"                               \
                        : "=r" (__res));                                \
        __res;                                                          \
})



#define __write_32bit_c0_register(register, sel, value)                 \
do {                                                                    \
        if (sel == 0)                                                   \
                __asm__ __volatile__(                                   \
                        "mtc0\t%z0, " #register "\n\t"                  \
                        : : "Jr" ((unsigned int)(value)));              \
        else                                                            \
                __asm__ __volatile__(                                   \
                        ".set\tmips32\n\t"                              \
                        "mtc0\t%z0, " #register ", " #sel "\n\t"        \
                        ".set\tmips0"                                   \
                        : : "Jr" ((unsigned int)(value)));              \
} while (0)


#define read_c0_count()     __read_32bit_c0_register($9, 0)
#define write_c0_count(val) __write_32bit_c0_register($9, 0, val)

#define read_c0_cause()     __read_32bit_c0_register($13, 0)
#define write_c0_cause(val) __write_32bit_c0_register($13, 0, val)

#define read_c0_cctl0()     __read_32bit_c0_register($20, 0)
#define write_c0_cctl0(val) __write_32bit_c0_register($20, 0, val)
#define read_c0_cctl1()     __read_32bit_c0_register($20, 1)
#define write_c0_cctl1(val) __write_32bit_c0_register($20, 1, val)
#define read_c0_cctl2()     __read_32bit_c0_register($20, 2)
#define write_c0_cctl2(val) __write_32bit_c0_register($20, 2, val)
#define read_c0_cctl3()     __read_32bit_c0_register($20, 3)
#define write_c0_cctl3(val) __write_32bit_c0_register($20, 3, val)

#endif /* ! __ASSEMBLY__ */

/* define all of the CCTL Select 0 bitfields */
#define CCTL0_DInv		0x00000001
#define CCTL0_IInv		0x00000002
#define CCTL0_LdStIMem0On	0x00000004
#define CCTL0_LdStIMem0Off	0x00000008
#define CCTL0_IMEM0Fill		0x00000010
#define CCTL0_IMEM0Off		0x00000020
#define CCTL0_IMEM0On		0x00000040
#define CCTL0_DWAllocOn		0x00000080
#define CCTL0_DWB		0x00000100
#define CCTL0_DWBInv		0x00000200
#define CCTL0_DMEM0On		0x00000400
#define CCTL0_DMEM0Off		0x00000800
#define CCTL0_DWBInvLC		0x00001000
#define CCTL0_DWBInvEC		0x00002000
/* placeholder - bit 14 is reserved */
#define CCTL0_DWAllocOff	0x00008000
/* placeholder - bit 16 is reserved */
/* placeholder - bit 17 is reserved */
/* placeholder - bit 18 is reserved */
/* placeholder - bit 19 is reserved */
#define CCTL0_WBMergeOn		0x00100000
#define CCTL0_WBMergeOff	0x00200000
#define CCTL0_WBufOn		0x00400000
#define CCTL0_WBufOff		0x00800000
#define CCTL0_FastWBOn		0x01000000
#define CCTL0_FastWBOff		0x02000000
#define CCTL0_LUBOn		0x04000000
#define CCTL0_LUBOff		0x08000000
#define CCTL0_NBLOn		0x10000000
#define CCTL0_NBLOff		0x20000000
/* placeholder - bit 30 is reserved */
/* placeholder - bit 31 is reserved */

/* define all of the CCTL Select 1 bitfields */
/* placeholder - bit 0 is reserved */
/* placeholder - bit 1 is reserved */
#define CCTL1_LdStIMem1On	0x00000004
#define CCTL1_LdStIMem1Off	0x00000008
#define CCTL1_IMEM1Fill		0x00000010
#define CCTL1_IMEM1Off		0x00000020
#define CCTL1_IMEM1On		0x00000040
/* placeholder - bit 7 is reserved */
#define CCTL1_DCacheOn		0x00000100
#define CCTL1_DCacheOff		0x00000200
#define CCTL1_DMEM1On		0x00000400
#define CCTL1_DMEM1Off		0x00000800
/* placeholder - bit 12 is reserved */
/* placeholder - bit 13 is reserved */
/* placeholder - bit 14 is reserved */
/* placeholder - bit 15 is reserved */
#define CCTL1_ICacheOn		0x00010000
#define CCTL1_ICacheOff		0x00020000
/* placeholder - bit 18 is reserved */
/* placeholder - bit 19 is reserved */
#define CCTL1_MergeUCOn		0x00100000
#define CCTL1_MergeUCOff	0x00200000
/* placeholder - bit 22 is reserved */
/* placeholder - bit 23 is reserved */
#define CCTL1_L2COn		0x01000000
#define CCTL1_L2COff		0x02000000
#define CCTL1_L2MEMOn		0x04000000
#define CCTL1_L2MEMOff		0x08000000
/* placeholder - bit 28 is reserved */
/* placeholder - bit 29 is reserved */
/* placeholder - bit 30 is reserved */
/* placeholder - bit 31 is reserved */

/* define all of the CCTL Select 2 bitfields */
#define CCTL2_ICMask0		0x00000001
#define CCTL2_ICMask1		0x00000002
#define CCTL2_IPFMask0		0x00000004
#define CCTL2_IPFMask1		0x00000008
/* placeholder - bit 4 is reserved */
/* placeholder - bit 5 is reserved */
/* placeholder - bit 6 is reserved */
/* placeholder - bit 7 is reserved */
#define CCTL2_DCMask0		0x00000100
#define CCTL2_DCMask1		0x00000200
#define CCTL2_OSBMAsk0		0x00000400
#define CCTL2_OSBMask1		0x00000800
#define CCTL2_WBQMask0		0x00001000
#define CCTL2_WBQMask1		0x00002000
#define CCTL2_LDQMask0		0x00004000
#define CCTL2_LDQMask1		0x00008000
/* placeholder - bit 16 is reserved */
/* placeholder - bit 17 is reserved */
/* placeholder - bit 18 is reserved */
/* placeholder - bit 19 is reserved */
/* placeholder - bit 20 is reserved */
/* placeholder - bit 21 is reserved */
/* placeholder - bit 22 is reserved */
/* placeholder - bit 23 is reserved */
#define CCTL2_L2CMask0		0x01000000
#define CCTL2_L2CMask1		0x02000000
/* placeholder - bit 26 is reserved */
/* placeholder - bit 27 is reserved */
/* placeholder - bit 28 is reserved */
/* placeholder - bit 29 is reserved */
/* placeholder - bit 30 is reserved */
/* placeholder - bit 31 is reserved */

/* define all of the CCTL Select 3 bitfields */
#define CCTL3_fIMem0		0x00000001
#define CCTL3_fIMem1		0x00000002
#define CCTL3_fICache		0x00000004
/* placeholder - bit 3 is reserved */
/* placeholder - bit 4 is reserved */
/* placeholder - bit 5 is reserved */
/* placeholder - bit 6 is reserved */
/* placeholder - bit 7 is reserved */
#define CCTL3_fDMem0		0x00000100
#define CCTL3_fDMem1		0x00000200
#define CCTL3_fDCache		0x00000400
/* placeholder - bit 11 is reserved */
#define CCTL3_fAlloc		0x00001000
#define CCTL3_fLdStIMem0	0x00002000
#define CCTL3_fLdStIMem1	0x00004000
/* placeholder - bit 15 is reserved */
#define CCTL3_fL2Mem		0x00010000
#define CCTL3_fL2C		0x00020000
/* placeholder - bit 18 is reserved */
/* placeholder - bit 19 is reserved */
#define CCTL3_fWBuf		0x00100000
#define CCTL3_fFastWB		0x00200000
#define CCTL3_fWBMerge		0x00400000
#define CCTL3_fMergeUC		0x00800000
#define CCTL3_fNBL		0x01000000
#define CCTL3_fLUB		0x02000000
/* placeholder - bit 26 is reserved */
/* placeholder - bit 27 is reserved */
/* placeholder - bit 28 is reserved */
/* placeholder - bit 29 is reserved */
/* placeholder - bit 30 is reserved */
/* placeholder - bit 31 is reserved */

#endif /* __ARCH_H__ */
