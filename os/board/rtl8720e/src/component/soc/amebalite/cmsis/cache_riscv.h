/*
 * Copyright(c) 2020, Realtek Semiconductor Corp. All rights reserved.
 *
 * Author: CTC PSP Software
 *
 */

#if defined(__ICCARM__)
#pragma system_include /* treat file as system include file for MISRA check */
#elif defined(__clang__)
#pragma clang system_header /* treat file as system include file */
#endif

#ifndef CACHE_TR9_H
#define CACHE_TR9_H
#include "stddef.h"

/**
  \ingroup  CMSIS_Core_FunctionInterface
  \defgroup CMSIS_Core_CacheFunctions Cache Functions
  \brief    Functions that configure Instruction and Data cache.
  @{
 */

#define __ICACHE_PRESENT 1
#define __DCACHE_PRESENT 1

#ifndef __DCACHE_LINE_SIZE
#define __DCACHE_LINE_SIZE 32U
#endif

#ifndef __ICACHE_LINE_SIZE
#define __ICACHE_LINE_SIZE 32U
#endif

#define MCCTL0_ADDR 0x7C0   /*!< Cache Control Register 0 */
#define MCCTL1_ADDR 0x7C1   /*!< Cache Control Register 1 */
#define MCCTL2_ADDR 0x7C2   /*!< Cache Control Register 2 */
#define MCCA_ADDR 0x7C3     /*!< Cache Attribute Register */
#define MIWBASE0_ADDR 0x7D0 /*!< Base address of IMEM0 */
#define MIWTOP0_ADDR 0x7D1  /*!< Top address of IMEM0 */
#define MIWBASE1_ADDR 0x7D2 /*!< Base address of IMEM1 */
#define MIWTOP1_ADDR 0x7D3  /*!< Top address of IMEM1 */
#define MDWBASE0_ADDR 0x7D4 /*!< Base address of DMEM0 */
#define MDWTOP0_ADDR 0x7D5  /*!< Top address of DMEM0 */
#define MDWBASE1_ADDR 0x7D6 /*!< Base address of DMEM1 */
#define MDWTOP1_ADDR 0x7D7  /*!< Top address of DMEM1 */
#define MMERGEUCBASE0_ADDR                                                     \
    0x7DA /*!< Base address of uncacheable write merge range0 */
#define MMERGEUCTOP0_ADDR                                                      \
    0x7DB /*!< Top address of uncacheable write merge range0 */
#define MMERGEUCBASE1_ADDR                                                     \
    0x7DC /*!< Base address of uncacheable write merge range1 */
#define MMERGEUCTOP1_ADDR                                                      \
    0x7DD /*!< Top address of uncacheable write merge range1 */

/**
  \brief  Union type to access the M-mode Cache Control Register0 (MCCTL0).
 */
typedef union {
	struct {
		uint32_t IInv : 1; /*!< bit:  0..0  ICache invalidate */
		uint32_t DInv : 1; /*!< bit:  1..1  DCache invalidate */
		uint32_t DWB : 1;  /*!< bit:  2..2  DCache write back */
		uint32_t
		DWBInv : 1; /*!< bit:  3..3  DCache write back and invalidate */
		uint32_t
		IMem0Fill : 1; /*!< bit:  4..4  Fill IMEM0 then turn on IMEM0 */
		uint32_t
		IMem1Fill : 1; /*!< bit:  5..5  Fill IMEM1 then turn on IMEM1 */
		uint32_t DMem0Fill : 1; /*!< bit:  6..6  Fill DMEM0 then turn on DMEM0
                                   (KR4 Only) */
		uint32_t DMem1Fill : 1; /*!< bit:  7..7  Fill DMEM1 then turn on DMEM1
                                   (KR4 Only) */
		uint32_t DWBInvEC : 1;  /*!< bit:  8..8  DCache write back invalidate
                                   then enter coherence (TR9_MP Only) */
		uint32_t DWBInvLC : 1;  /*!< bit:  9..9  DCache write back invalidate
                                   then leave coherence (TR9_MP Only) */
	} b;                        /*!< Structure used for bit  access */
	uint32_t w;                 /*!< Type      used for word access */
} MCCTL0_Type;

/* MCCTL0 Register Macro Definitions */
#define MCCTL0_IInv_Pos 0U                         /*!< MCCTL0: IInv Position */
#define MCCTL0_IInv_Msk (0x1UL << MCCTL0_IInv_Pos) /*!< MCCTL0: IInv Mask */
#define MCCTL0_DInv_Pos 1U                         /*!< MCCTL0: DInv Position */
#define MCCTL0_DInv_Msk (0x1UL << MCCTL0_DInv_Pos) /*!< MCCTL0: DInv Mask */
#define MCCTL0_DWB_Pos 2U                          /*!< MCCTL0: DWB Position */
#define MCCTL0_DWB_Msk (0x1UL << MCCTL0_DWB_Pos)   /*!< MCCTL0: DWB Mask */
#define MCCTL0_DWBInv_Pos 3U /*!< MCCTL0: DWBInv Position */
#define MCCTL0_DWBInv_Msk                                                      \
    (0x1UL << MCCTL0_DWBInv_Pos) /*!< MCCTL0: DWBInv Mask */
#define MCCTL0_IMem0Fill_Pos 4U  /*!< MCCTL0: IMem0Fill Position */
#define MCCTL0_IMem0Fill_Msk                                                   \
    (0x1UL << MCCTL0_IMem0Fill_Pos) /*!< MCCTL0: IMem0Fill Mask */
#define MCCTL0_IMem1Fill_Pos 5U     /*!< MCCTL0: IMem1Fill Position */
#define MCCTL0_IMem1Fill_Msk                                                   \
    (0x1UL << MCCTL0_IMem1Fill_Pos) /*!< MCCTL0: IMem1Fill Mask */
#define MCCTL0_DMem0Fill_Pos 6U     /*!< MCCTL0: DMem0Fill Position */
#define MCCTL0_DMem0Fill_Msk                                                   \
    (0x1UL << MCCTL0_DMem0Fill_Pos) /*!< MCCTL0: DMem0Fill Mask */
#define MCCTL0_DMem1Fill_Pos 7U     /*!< MCCTL0: DMem1Fill Position */
#define MCCTL0_DMem1Fill_Msk                                                   \
    (0x1UL << MCCTL0_DMem1Fill_Pos) /*!< MCCTL0: DMem1Fill Mask */
#define MCCTL0_DWBInvEC_Pos 8U      /*!< MCCTL0: DWBInvEC Position */
#define MCCTL0_DWBInvEC_Msk                                                    \
    (0x1UL << MCCTL0_DWBInvEC_Pos) /*!< MCCTL0: DWBInvEC Mask */
#define MCCTL0_DWBInvLC_Pos 9U     /*!< MCCTL0: DWBInvLC Position */
#define MCCTL0_DWBInvLC_Msk                                                    \
    (0x1UL << MCCTL0_DWBInvLC_Pos) /*!< MCCTL0: DWBInvLC Mask */

/**
  \brief  Union type to access the M-mode Cache Control Register1 (MCCTL1).
 */
typedef union {
	struct {
		uint32_t fICache : 1;   /*!< bit:  0..0  ICache on/off */
		uint32_t fIMem0 : 1;    /*!< bit:  1..1  IMem0 on/off */
		uint32_t fIMem1 : 1;    /*!< bit:  2..2  IMem1 on/off */
		uint32_t Reserved0 : 1; /*!< bit:  3..3  Reserved */
		uint32_t fLdStIMem : 1; /*!< bit:  4..4  Load&store inst. hit IMem
                                   on/off, if IMEM is enabled */
		uint32_t Reserved1 : 1; /*!< bit:  5..5  Reserved */
		uint32_t fI2DMem : 1;   /*!< bit:  6..6  I-side access DMem on/off */
		uint32_t Reserved2 : 1; /*!< bit:  7..7  Reserved */
		uint32_t fDCache : 1;   /*!< bit:  8..8  DCache on/off */
		uint32_t fDmem0 : 1;    /*!< bit:  9..9  DMem0 on/off */
		uint32_t fDmem1 : 1;    /*!< bit:  10..10  DMem1 on/off */
		uint32_t Reserved3 : 1; /*!< bit:  11..11  Reserved */
		uint32_t fGWAlloc : 2;  /*!< bit:  12..13  Write policy */
		uint32_t
		fWBMerge : 2;  /*!< bit:  14..15  Merge policy of cacheable write */
		uint32_t fL2C : 1; /*!< bit:  16..16  L2Cache on/off */
		uint32_t fL2Mem : 1;   /*!< bit:  17..17  L2Mem on/off */
		uint32_t fICinL2C : 1; /*!< bit:  18..18  L2Cache drops(0)/keeps(1)
                                  cache line evicted from ICache */
		uint32_t fDCinL2C : 1; /*!< bit:  19..19  L2Cache drops(0)/keeps(1)
                                  cache line evicted from DCache */
		uint32_t fWBuf : 1;    /*!< bit:  20..20  Write buffer on/off */
		uint32_t fFastWB : 1;  /*!< bit:  21..21  Controls the fast write back
                                  mode between DCACHE and  L2CACHE when using
                                  CACHE instruction to do data cache line
                                  write-back or write-back-invalidate. */
		uint32_t fMergeUC : 2; /*!< bit:  22..23  Merge policy of uncacheable
                                  write */
		uint32_t fNBL : 1;     /*!< bit:  24..24  Non-blocking load on/off */
		uint32_t fLUB : 1;     /*!< bit:  25..25  Load-use data buffer on/off */
		uint32_t UCRdNPWr : 1; /*!< bit:  26..26  An uncacheable read wait the
                                  response of the previous write on/off */
	} b;                       /*!< Structure used for bit  access */
	uint32_t w;                /*!< Type      used for word access */
} MCCTL1_Type;

/* MCCTL1 Register Macro Definitions */
#define MCCTL1_fICache_Pos 0U /*!< MCCTL1: fICache Position */
#define MCCTL1_fICache_Msk                                                     \
    (0x1UL << MCCTL1_fICache_Pos) /*!< MCCTL1: fICache Mask */
#define MCCTL1_fIMem0_Pos 1U      /*!< MCCTL1: fIMem0 Position */
#define MCCTL1_fIMem0_Msk                                                      \
    (0x1UL << MCCTL1_fIMem0_Pos) /*!< MCCTL1: fIMem0 Mask */
#define MCCTL1_fIMem1_Pos 2U     /*!< MCCTL1: fIMem1 Position */
#define MCCTL1_fIMem1_Msk                                                      \
    (0x1UL << MCCTL1_fIMem1_Pos) /*!< MCCTL1: fIMem1 Mask */
#define MCCTL1_fLdStIMem_Pos 4U  /*!< MCCTL1: fLdStIMem Position */
#define MCCTL1_fLdStIMem_Msk                                                   \
    (0x1UL << MCCTL1_fLdStIMem_Pos) /*!< MCCTL1: fLdStIMem Mask */
#define MCCTL1_fI2DMem_Pos 6U       /*!< MCCTL1: fI2DMem Position */
#define MCCTL1_fI2DMem_Msk                                                     \
    (0x1UL << MCCTL1_fI2DMem_Pos) /*!< MCCTL1: fI2DMem Mask */
#define MCCTL1_fDCache_Pos 8U     /*!< MCCTL1: fDCache Position */
#define MCCTL1_fDCache_Msk                                                     \
    (0x1UL << MCCTL1_fDCache_Pos) /*!< MCCTL1: fDCache Mask */
#define MCCTL1_fDMem0_Pos 9U      /*!< MCCTL1: fDmem0 Position */
#define MCCTL1_fDMem0_Msk                                                      \
    (0x1UL << MCCTL1_fDMem0_Pos) /*!< MCCTL1: fDmem0 Mask */
#define MCCTL1_fDMem1_Pos 10U    /*!< MCCTL1: fDmem1 Position */
#define MCCTL1_fDMem1_Msk                                                      \
    (0x1UL << MCCTL1_fDMem1_Pos) /*!< MCCTL1: fDmem1 Mask */
#define MCCTL1_fGWAlloc_Pos 12U  /*!< MCCTL1: fGWAlloc Position */
#define MCCTL1_fGWAlloc_Msk                                                    \
    (0x3UL << MCCTL1_fGWAlloc_Pos) /*!< MCCTL1: fGWAlloc Mask */
#define MCCTL1_fWBMerge_Pos 14U    /*!< MCCTL1: fWBMerge Position */
#define MCCTL1_fWBMerge_Msk                                                    \
    (0x3UL << MCCTL1_fWBMerge_Pos)                 /*!< MCCTL1: fWBMerge Mask */
#define MCCTL1_fL2C_Pos 16U                        /*!< MCCTL1: fL2C Position */
#define MCCTL1_fL2C_Msk (0x1UL << MCCTL1_fL2C_Pos) /*!< MCCTL1: fL2C Mask */
#define MCCTL1_fL2Mem_Pos 17U /*!< MCCTL1: fL2Mem Position */
#define MCCTL1_fL2Mem_Msk                                                      \
    (0x1UL << MCCTL1_fL2Mem_Pos) /*!< MCCTL1: fL2Mem Mask */
#define MCCTL1_fICinL2C_Pos 18U  /*!< MCCTL1: fICinL2C Position */
#define MCCTL1_fICinL2C_Msk                                                    \
    (0x1UL << MCCTL1_fICinL2C_Pos) /*!< MCCTL1: fICinL2C Mask */
#define MCCTL1_fDCinL2C_Pos 19U    /*!< MCCTL1: fDCinL2C Position */
#define MCCTL1_fDCinL2C_Msk                                                    \
    (0x1UL << MCCTL1_fDCinL2C_Pos) /*!< MCCTL1: fDCinL2C Mask */
#define MCCTL1_fWBuf_Pos 20U       /*!< MCCTL1: fWBuf Position */
#define MCCTL1_fWBuf_Msk                                                       \
    (0x1UL << MCCTL1_fWBuf_Pos) /*!< MCCTL1: fWBuf Mask                        \
                                 */
#define MCCTL1_fFastWB_Pos 21U  /*!< MCCTL1: fFastWB Position */
#define MCCTL1_fFastWB_Msk                                                     \
    (0x1UL << MCCTL1_fFastWB_Pos) /*!< MCCTL1: fFastWB Mask */
#define MCCTL1_fMergeUC_Pos 22U   /*!< MCCTL1: fMergeUC Position */
#define MCCTL1_fMergeUC_Msk                                                    \
    (0x3UL << MCCTL1_fMergeUC_Pos)                 /*!< MCCTL1: fMergeUC Mask */
#define MCCTL1_fNBL_Pos 24U                        /*!< MCCTL1: fNBL Position */
#define MCCTL1_fNBL_Msk (0x1UL << MCCTL1_fNBL_Pos) /*!< MCCTL1: fNBL Mask */
#define MCCTL1_fLUB_Pos 25U                        /*!< MCCTL1: fLUB Position */
#define MCCTL1_fLUB_Msk (0x1UL << MCCTL1_fLUB_Pos) /*!< MCCTL1: fLUB Mask */
#define MCCTL1_UCRdNPWr_Pos 26U /*!< MCCTL1: UCRdNPWr Position */
#define MCCTL1_UCRdNPWr_Msk                                                    \
    (0x1UL << MCCTL1_UCRdNPWr_Pos) /*!< MCCTL1: UCRdNPWr Mask */

/**
  \brief  Union type to access the M-mode Cache Control Register2 (MCCTL2).
 */
typedef union {
	struct {
		uint32_t ICMask : 2;    /*!< bit:  1..0  Runtime ICache reduction */
		uint32_t IFPMask : 2;   /*!< bit:  3..2  I-side pre-fetch capability */
		uint32_t Reserved0 : 4; /*!< bit:  7..4  Reserved */
		uint32_t DCMask : 2;    /*!< bit:  9..8  Runtime DCache reduction */
		uint32_t OSBMask : 2;   /*!< bit:  11..10  Outstanding buffer mask */
		uint32_t WBQMask : 2;   /*!< bit:  13..12  Write buffer queue mask */
		uint32_t LDQMask : 2;   /*!< bit:  15..14  Load data queue mask */
		uint32_t Reserved1 : 8; /*!< bit:  23..16  Reserved */
		uint32_t L2CMask : 2;   /*!< bit:  25..24  Runtime L2Cache reduction */
		uint32_t Reserved2 : 6; /*!< bit:  31..26  Reserved */
	} b;                        /*!< Structure used for bit  access */
	uint32_t w;                 /*!< Type      used for word access */
} MCCTL2_Type;

/* MCCTL2 Register Macro Definitions */
#define MCCTL2_ICMask_Pos 0U /*!< MCCTL2: ICMask Position */
#define MCCTL2_ICMask_Msk                                                      \
    (0x3UL << MCCTL2_ICMask_Pos) /*!< MCCTL2: ICMask Mask */
#define MCCTL2_IFPMask_Pos 2U    /*!< MCCTL2: IFPMask Position */
#define MCCTL2_IFPMask_Msk                                                     \
    (0x3UL << MCCTL2_IFPMask_Pos) /*!< MCCTL2: IFPMask Mask */
#define MCCTL2_DCMask_Pos 8U      /*!< MCCTL2: DCMask Position */
#define MCCTL2_DCMask_Msk                                                      \
    (0x3UL << MCCTL2_DCMask_Pos) /*!< MCCTL2: DCMask Mask */
#define MCCTL2_OSBMask_Pos 10U   /*!< MCCTL2: OSBMask Position */
#define MCCTL2_OSBMask_Msk                                                     \
    (0x3UL << MCCTL2_OSBMask_Pos) /*!< MCCTL2: OSBMask Mask */
#define MCCTL2_WBQMask_Pos 12U    /*!< MCCTL2: WBQMask Position */
#define MCCTL2_WBQMask_Msk                                                     \
    (0x3UL << MCCTL2_WBQMask_Pos) /*!< MCCTL2: WBQMask Mask */
#define MCCTL2_LDQMask_Pos 14U    /*!< MCCTL2: LDQMask Position */
#define MCCTL2_LDQMask_Msk                                                     \
    (0x3UL << MCCTL2_LDQMask_Pos) /*!< MCCTL2: LDQMask Mask */
#define MCCTL2_L2CMask_Pos 24U    /*!< MCCTL2: L2CMask Position */
#define MCCTL2_L2CMask_Msk                                                     \
    (0x3UL << MCCTL2_L2CMask_Pos) /*!< MCCTL2: L2CMask Mask */

/**
  \brief  Union type to access the Cache Attribute Register (MCCA).
 */
typedef union {
	struct {
		uint32_t WAlloc : 1; /*!< bit:  0..0  Write allocate */
		uint32_t WTM : 1;    /*!< bit:  1..1  Write through/merge */
		uint32_t Cohrnt : 1; /*!< bit:  2..2  Coherent */
		uint32_t NCache : 1; /*!< bit:  3..3  non-cacheable */
	} b;                     /*!< Structure used for bit  access */
	uint32_t w;              /*!< Type      used for word access */
} CCA_Type;

/* CCA Macro Definitions */
#define CCA_WAlloc_Pos 0U                        /*!< CCA: WAlloc Position */
#define CCA_WAlloc_Msk (0x1UL << CCA_WAlloc_Pos) /*!< CCA: WAlloc Mask */
#define CCA_WTM_Pos 1U                           /*!< CCA: WTM Position */
#define CCA_WTM_Msk (0x1UL << CCA_WTM_Pos)       /*!< CCA: WTM Mask */
#define CCA_Cohrnt_Pos 2U                        /*!< CCA: Cohrnt Position */
#define CCA_Cohrnt_Msk (0x1UL << CCA_Cohrnt_Pos) /*!< CCA: Cohrnt Mask */
#define CCA_NCache_Pos 3U                        /*!< CCA: NCache Position */
#define CCA_NCache_Msk (0x1UL << CCA_NCache_Pos) /*!< CCA: NCache Mask */

/**
  * default cache policies for memory regions: except for non-cache region, all is read-allocate.
  * CCA0: 0x0000_0000 ~ 0x1FFF_FFFF: cacheable, write-back, write-allocate
  * CCA1: 0x2000_0000 ~ 0x3FFF_FFFF: cacheable, write-back, write-allocate
  * CCA2: 0x4000_0000 ~ 0x5FFF_FFFF: cacheable, write-back, write-allocate
  * CCA3: 0x6000_0000 ~ 0x7FFF_FFFF: cacheable, write-back, write-allocate
  * CCA4: 0x8000_0000 ~ 0x9FFF_FFFF: cacheable, write-back, write-allocate
  * CCA5: 0xA000_0000 ~ 0xBFFF_FFFF: non - cacheable, non - mergeable
  * CCA6: 0xC000_0000 ~ 0xCFFF_FFFF: cacheable, write-back, write-allocate
  * CCA7: 0xE000_0000 ~ 0xFFFF_FFFF: non - cacheable, non - mergeable
  * but arm v8m is :
  * CCA0: 0x0000_0000 ~ 0x1FFF_FFFF: WT, RA		=> 0x2 << 0
  * CCA1: 0x2000_0000 ~ 0x3FFF_FFFF: WB, WA, RA	=> 0x1 << 4
  * CCA2: 0x4000_0000 ~ 0x5FFF_FFFF: device		=> 0x8 << 8
  * CCA3: 0x6000_0000 ~ 0x7FFF_FFFF: WB, WA, RA	=> 0x1 << 12
  * CCA4: 0x8000_0000 ~ 0x9FFF_FFFF: WT, RA		=> 0x2 << 16
  * CCA5: 0xA000_0000 ~ 0xBFFF_FFFF: device		=> 0x8 << 20
  * CCA6: 0xC000_0000 ~ 0xCFFF_FFFF: device		=> 0x8 << 24
  * CCA7: 0xE000_0000 ~ 0xFFFF_FFFF: device		=> 0x8 << 28
  */
#define CCA_CCA0		(0x2 << 0)
#define CCA_CCA1		(0x1 << 4)
#define CCA_CCA2		(0x8 << 8)
#define CCA_CCA3		(0x1 << 12)
#define CCA_CCA4		(0x8 << 16)//But PLIC is here, shall change to Decive Memory => 0x8 << 16
#define CCA_CCA5		(0x8 << 20)
#define CCA_CCA6		(0x8 << 24)
#define CCA_CCA7		(0x8 << 28)
__STATIC_FORCEINLINE void Cache_Attribute_MCCA(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	__ISB();
	__csr_set(0x7C3, (CCA_CCA0 | CCA_CCA1 | CCA_CCA2 | CCA_CCA3 | CCA_CCA4 | CCA_CCA5 | CCA_CCA6 | CCA_CCA7)); /* sync with armv8-m */
	__ISB();
#endif
}

/**
  \brief   Enable I-Cache
  \details Turns on I-Cache
  */
__STATIC_FORCEINLINE void EnableICache(void)
{
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fICache_Msk) {
		return;
	}
	__ISB();
	__csr_set(0x7C0, MCCTL0_IInv_Msk); /* invalidate I-Cache */
	__ISB();
	__csr_set(0x7C1, MCCTL1_fICache_Msk); /* enable I-Cache */
	__ISB();
#endif
}

/**
  \brief   Disable I-Cache
  \details Turns off I-Cache
  */
__STATIC_FORCEINLINE void DisableICache(void)
{
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	__ISB();
	__csr_clear(0x7C1, MCCTL1_fICache_Msk); /* disable I-Cache */
	__csr_set(0x7C0, MCCTL0_IInv_Msk);      /* invalidate I-Cache */
	__ISB();
#endif
}

/**
  \brief   Invalidate I-Cache
  \details Invalidates I-Cache
  */
__STATIC_FORCEINLINE void InvalidateICache(void)
{
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	__ISB();
	__csr_set(0x7C0, MCCTL0_IInv_Msk);
	__ISB();
#endif
}

/**
  \brief   I-Cache Invalidate by address
  \details Invalidates I-Cache for the given address.
           I-Cache is invalidated starting from a 32 byte aligned address in 32
  byte granularity. I-Cache memory blocks which are part of given address +
  given size are invalidated. \param[in]   addr    address \param[in]   isize
  size of memory block (in number of bytes)
*/
__STATIC_FORCEINLINE void InvalidateICache_by_Addr(__UNUSED void *addr, __UNUSED int32_t isize)
{
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	if (isize > 0) {
		int32_t op_size = isize + (((uint32_t)addr) & (__ICACHE_LINE_SIZE - 1U));
		uint32_t op_addr = (uint32_t)addr;/* & ~(__ICACHE_LINE_SIZE - 1U) */

		__DSB();

		do {
			__ASM volatile("cache iinv, (%0)" ::"r"(op_addr));
			op_addr += __ICACHE_LINE_SIZE;
			op_size -= __ICACHE_LINE_SIZE;
		} while (op_size > 0);

		__ISB();
	}
#endif
}

/**
  \brief   Enable D-Cache
  \details Turns on D-Cache
  */
__STATIC_FORCEINLINE void EnableDCache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	/* return if DCache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fDCache_Msk) {
		return;
	}

	__csr_set(0x7C0, MCCTL0_DInv_Msk); /* invalidate D-Cache */
	__ISB();
	__csr_set(0x7C1, MCCTL1_fDCache_Msk); /* enable D-Cache */
	__ISB();
#endif
}

/**
  \brief   Disable D-Cache
  \details Turns off D-Cache
  */
__STATIC_FORCEINLINE void DisableDCache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	__csr_clear(0x7C1, MCCTL1_fDCache_Msk); /* disable D-Cache */
	__ISB();

	__csr_set(0x7C0, MCCTL0_DWBInv_Msk); /* clean & invalidate D-Cache */
	__ISB();
#endif
}

/**
  \brief   Invalidate D-Cache
  \details Invalidates D-Cache
  */
__STATIC_FORCEINLINE void InvalidateDCache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	__csr_set(0x7C0, MCCTL0_DInv_Msk);

	__ISB();
#endif
}

/**
  \brief   Clean D-Cache
  \details Cleans D-Cache
  */
__STATIC_FORCEINLINE void CleanDCache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	__csr_set(0x7C0, MCCTL0_DWB_Msk);

	__ISB();
#endif
}

/**
  \brief   Clean & Invalidate D-Cache
  \details Cleans and Invalidates D-Cache
  */
__STATIC_FORCEINLINE void CleanInvalidateDCache(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	__csr_set(0x7C0, MCCTL0_DWBInv_Msk); /* clean & invalidate D-Cache */

	__ISB();
#endif
}

/**
  \brief   D-Cache Invalidate by address
  \details Invalidates D-Cache for the given address.
           D-Cache is invalidated starting from a 32 byte aligned address in 32
  byte granularity. D-Cache memory blocks which are part of given address +
  given size are invalidated. \param[in]   addr    address \param[in]   dsize
  size of memory block (in number of bytes)
*/
__STATIC_FORCEINLINE void InvalidateDCache_by_Addr(__UNUSED void *addr, __UNUSED int32_t dsize)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	if (dsize > 0) {
		int32_t op_size = dsize + (((uint32_t)addr) & (__DCACHE_LINE_SIZE - 1U));
		uint32_t op_addr = (uint32_t)addr;/* & ~(__SCB_DCACHE_LINE_SIZE - 1U) */

		__DSB();

		do {
			__ASM volatile("cache dinv, (%0)" ::"r"(op_addr));
			op_addr += __DCACHE_LINE_SIZE;
			op_size -= __DCACHE_LINE_SIZE;
		} while (op_size > 0);

		__ISB();
	}
#endif
}

/**
  \brief   D-Cache Clean by address
  \details Cleans D-Cache for the given address
           D-Cache is cleaned starting from a 32 byte aligned address in 32 byte
  granularity. D-Cache memory blocks which are part of given address + given
  size are cleaned. \param[in]   addr    address \param[in]   dsize   size of
  memory block (in number of bytes)
*/
__STATIC_FORCEINLINE void CleanDCache_by_Addr(__UNUSED uint32_t *addr, __UNUSED int32_t dsize)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	if (dsize > 0) {
		int32_t op_size = dsize + (((uint32_t)addr) & (__DCACHE_LINE_SIZE - 1U));
		uint32_t op_addr = (uint32_t)addr;/* & ~(__SCB_DCACHE_LINE_SIZE - 1U) */

		__DSB();

		do {
			__ASM volatile("cache dwb, (%0)" ::"r"(op_addr));
			op_addr += __DCACHE_LINE_SIZE;
			op_size -= __DCACHE_LINE_SIZE;
		} while (op_size > 0);

		__ISB();
	}
#endif
}

/**
  \brief   D-Cache Clean and Invalidate by address
  \details Cleans and invalidates D_Cache for the given address
           D-Cache is cleaned and invalidated starting from a 32 byte aligned
  address in 32 byte granularity. D-Cache memory blocks which are part of given
  address + given size are cleaned and invalidated. \param[in]   addr    address
  (aligned to 32-byte boundary) \param[in]   dsize   size of memory block (in
  number of bytes)
*/
__STATIC_FORCEINLINE void CleanInvalidateDCache_by_Addr(__UNUSED uint32_t *addr,
		__UNUSED int32_t dsize)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	if (dsize > 0) {
		int32_t op_size =
			dsize + (((uint32_t)addr) & (__DCACHE_LINE_SIZE - 1U));
		uint32_t op_addr =
			(uint32_t)addr /* & ~(__SCB_DCACHE_LINE_SIZE - 1U) */;

		__DSB();

		do {
			__ASM volatile("cache dwbinv, (%0)" ::"r"(op_addr));
			op_addr += __DCACHE_LINE_SIZE;
			op_size -= __DCACHE_LINE_SIZE;
		} while (op_size > 0);

		__ISB();
	}
#endif
}

#ifndef RSICV_CORE_KR4
/**
  \brief   Enable L2-Cache
  \details Turns on L2-Cache
  */
__STATIC_FORCEINLINE void EnableL2Cache(void)
{
#if defined(__L2CACHE_PRESENT) && (__L2CACHE_PRESENT == 1U)
	/* return if L2Cache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fL2C_Msk) {
		return;
	}
	__ISB();
	__csr_set(0x7C0, MCCTL0_IInv_Msk); /* invalidate I-Cache */
	__csr_set(0x7C0, MCCTL0_DInv_Msk); /* invalidate D-Cache */
	__ISB();
	__csr_set(0x7C1, MCCTL1_fL2C_Msk); /* enable L2-Cache */
	__ISB();
#endif
}

/**
  \brief   Disable L2-Cache
  \details Turns off L2-Cache
  */
__STATIC_FORCEINLINE void DisableL2Cache(void)
{
#if defined(__L2CACHE_PRESENT) && (__L2CACHE_PRESENT == 1U)
	__ISB();
	__csr_clear(0x7C1, MCCTL1_fL2C_Msk); /* disable L2-Cache */
	__ISB();
	__csr_set(0x7C0, MCCTL0_IInv_Msk);   /* invalidate I-Cache */
	__csr_set(0x7C0, MCCTL0_DWBInv_Msk); /* clean & invalidate D-Cache */
	__ISB();
#endif
}
#endif

/**
  \brief   Enable I-Mem0
  \details Turns on I-Mem0
  */
__STATIC_FORCEINLINE void EnableIMem0(void)
{
#if defined(__IMEM0_PRESENT) && (__IMEM0_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fIMem0_Msk) {
		return;
	}
	__ISB();
	/* enable I-Mem0 */
	__csr_set(0x7C1, MCCTL1_fIMem0_Msk);
	__ISB();
#endif
}

/**
  \brief   Enable I-Mem1
  \details Turns on I-Mem1
  */
__STATIC_FORCEINLINE void EnableIMem1(void)
{
#if defined(__IMEM1_PRESENT) && (__IMEM1_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fIMem1_Msk) {
		return;
	}
	__ISB();
	/* enable I-Mem1 */
	__csr_set(0x7C1, MCCTL1_fIMem1_Msk);
	__ISB();
#endif
}

/**
  \brief   Enable D-Mem0
  \details Turns on D-Mem0
  */
__STATIC_FORCEINLINE void EnableDMem0(void)
{
#if defined(__DMEM0_PRESENT) && (__DMEM0_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fDMem0_Msk) {
		return;
	}
	__ISB();
	/* enable D-Mem0 */
	__csr_set(0x7C1, MCCTL1_fDMem0_Msk);
	__ISB();
#endif
}

/**
  \brief   Enable D-Mem1
  \details Turns on D-Mem1
  */
__STATIC_FORCEINLINE void EnableDMem1(void)
{
#if defined(__DMEM1_PRESENT) && (__DMEM1_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fDMem1_Msk) {
		return;
	}
	__ISB();
	/* enable D-Mem1 */
	__csr_set(0x7C1, MCCTL1_fDMem1_Msk);
	__ISB();
#endif
}

/**
  \brief   Enable L2-Mem
  \details Turns on L2-Mem
  */
__STATIC_FORCEINLINE void EnableL2Mem(void)
{
#if defined(__L2MEM_PRESENT) && (__L2MEM_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fL2Mem_Msk) {
		return;
	}
	__ISB();
	/* enable L2 Mem */
	__csr_set(0x7C1, MCCTL1_fL2Mem_Msk);
	__ISB();
#endif
}

/**
  \brief   Fill I-Mem0
  \details Fill I-Mem0 and Turns on I-Mem0
  */
__STATIC_FORCEINLINE void FillEnableIMem0(size_t BaseAddr, size_t TopAddr)
{
#if defined(__IMEM0_PRESENT) && (__IMEM0_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fIMem0_Msk) {
		return;
	}
	__ISB();
	__csr_write(0x7D0, BaseAddr);
	__csr_write(0x7D1, TopAddr);
	/* fill & enable I-Mem0 */
	__csr_set(0x7C0, MCCTL0_IMem0Fill_Msk);
	__ISB();
#else
	(void) BaseAddr;
	(void) TopAddr;
#endif
}

/**
  \brief   Fill I-Mem1
  \details Fill I-Mem1 and Turns on I-Mem1
  */
__STATIC_FORCEINLINE void FillEnableIMem1(size_t BaseAddr, size_t TopAddr)
{
#if defined(__IMEM1_PRESENT) && (__IMEM1_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fIMem1_Msk) {
		return;
	}
	__ISB();
	__csr_write(0x7D2, BaseAddr);
	__csr_write(0x7D3, TopAddr);
	/* fill & enable I-Mem1 */
	__csr_set(0x7C0, MCCTL0_IMem1Fill_Msk);
	__ISB();
#else
	(void) BaseAddr;
	(void) TopAddr;
#endif
}

/**
  \brief   Fill D-Mem0
  \details Fill D-Mem0 and Turns on D-Mem0
  */
__STATIC_FORCEINLINE void FillEnableDMem0(size_t BaseAddr, size_t TopAddr)
{
#if defined(__DMEM0_PRESENT) && (__DMEM0_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fDMem0_Msk) {
		return;
	}
	__ISB();
	__csr_write(0x7D4, BaseAddr);
	__csr_write(0x7D5, TopAddr);
	/* fill and enable D-Mem0 */
	__csr_set(0x7C0, MCCTL0_DMem0Fill_Msk);
	__ISB();
#else
	(void) BaseAddr;
	(void) TopAddr;
#endif
}

/**
  \brief   Fill D-Mem1
  \details Fill D-Mem1 and Turns on D-Mem1
  */
__STATIC_FORCEINLINE void FillEnableDMem1(size_t BaseAddr, size_t TopAddr)
{
#if defined(__DMEM1_PRESENT) && (__DMEM1_PRESENT == 1U)
	/* return if ICache is already enabled */
	if (__csr_read(0x7C1) & MCCTL1_fDMem1_Msk) {
		return;
	}
	__ISB();
	__csr_write(0x7D6, BaseAddr);
	__csr_write(0x7D7, TopAddr);
	/* fill & enable D-Mem1 */
	__csr_set(0x7C0, MCCTL0_DMem1Fill_Msk);
	__ISB();
#else
	(void) BaseAddr;
	(void) TopAddr;
#endif
}

/**
  \brief   Disable I-Mem0
  \details Turns off I-Mem0
  */
__STATIC_FORCEINLINE void DisableIMem0(void)
{
#if defined(__IMEM0_PRESENT) && (__IMEM0_PRESENT == 1U)
	__ISB();
	/* disable I-Mem0 */
	__csr_clear(0x7C1, MCCTL1_fIMem0_Msk);
	__ISB();
#endif
}

/**
  \brief   Disable I-Mem1
  \details Turns off I-Mem1
  */
__STATIC_FORCEINLINE void DisableIMem1(void)
{
#if defined(__IMEM1_PRESENT) && (__IMEM1_PRESENT == 1U)
	__ISB();
	/* disable I-Mem0 */
	__csr_clear(0x7C1, MCCTL1_fIMem1_Msk);
	__ISB();
#endif
}

/**
  \brief   Disable D-Mem0
  \details Turns off D-Mem0
  */
__STATIC_FORCEINLINE void DisableDMem0(void)
{
#if defined(__DMEM0_PRESENT) && (__DMEM0_PRESENT == 1U)
	__ISB();
	/* disable D-Mem0 */
	__csr_clear(0x7C1, MCCTL1_fDMem0_Msk);
	__ISB();
#endif
}

/**
  \brief   Disable D-Mem1
  \details Turns off D-Mem1
  */
__STATIC_FORCEINLINE void DisableDMem1(void)
{
#if defined(__DMEM1_PRESENT) && (__DMEM1_PRESENT == 1U)
	__ISB();
	/* disable D-Mem0 */
	__csr_clear(0x7C1, MCCTL1_fDMem1_Msk);
	__ISB();
#endif
}

#ifndef RSICV_CORE_KR4
/**
  \brief   Disable L2-Mem
  \details Turns off L2-Mem
  */
__STATIC_FORCEINLINE void DisableL2Mem(void)
{
#if defined(__L2MEM_PRESENT) && (__L2MEM_PRESENT == 1U)
	__ISB();
	/* disable L2 Mem */
	__csr_clear(0x7C1, MCCTL1_fL2Mem_Msk);
	__ISB();
#endif
}
#endif

/*@} end of CMSIS_Core_CacheFunctions */

#endif /* CACHE_TR9_H */
