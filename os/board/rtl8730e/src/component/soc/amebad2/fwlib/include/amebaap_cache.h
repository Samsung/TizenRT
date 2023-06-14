/**
  ******************************************************************************
  * @file    amebaap_cache.h
  * @author
  * @version V1.0.0
  * @date    2020-12-07
  * @brief   This file contains all the functions prototypes for the flash cache firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _AMEBAD2_AP_CACHE_H_
#define _AMEBAD2_AP_CACHE_H_

/** @addtogroup AmebaD2_Platform
  * @{
  */

/** @defgroup CACHE
  * @brief CACHE modules
  * @{
  */

/** @addtogroup CACHE
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  *		-just support read cache.
  *		-32K bytes.
  *		-used for flash read and XIP.
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  *		Cache_Enable: enable/disable cache
  *		Cache_Flush: flush cache, you should Cache_Flush after flash write or flash erase
  *****************************************************************************************
  * @endverbatim
  */

/** @defgroup CACHE_Line_Aligned_define
  * @{
  */
#define CACHE_L1_DCACHE_LINE_SIZE					64
#define CACHE_L1_ICACHE_LINE_SIZE					32
#define CACHE_L2_CACHE_LINE_SIZE					64

#define CACHE_L1_DCACHE_LINE_ADDR_MSK					0xFFFFFFC0
#define CACHE_L1_ICACHE_LINE_ADDR_MSK					0xFFFFFFE0
#define CACHE_L2_CACHE_LINE_ADDR_MSK					0xFFFFFFC0

#define IS_L1_DCACHE_LINE_ALIGNED_SIZE(BYTES)		((BYTES & 0x3F) == 0)
#define IS_L1_DCACHE_LINE_ALIGNED_ADDR(ADDR)		((ADDR & 0x3F) == 0)
#define IS_L1_ICACHE_LINE_ALIGNED_SIZE(BYTES)		((BYTES & 0x1F) == 0)
#define IS_L1_ICACHE_LINE_ALIGNED_ADDR(ADDR)		((ADDR & 0x1F) == 0)
#define IS_L2_CACHE_LINE_ALIGNED_SIZE(BYTES)		((BYTES & 0x3F) == 0)
#define IS_L2_CACHE_LINE_ALIGNED_ADDR(ADDR)			((ADDR & 0x3F) == 0)

#define CACHE_LINE_ADDR_MSK CACHE_L1_DCACHE_LINE_ADDR_MSK
#define CACHE_LINE_SIZE     CACHE_L1_DCACHE_LINE_SIZE
#define IS_CACHE_LINE_ALIGNED_ADDR IS_L1_DCACHE_LINE_ALIGNED_ADDR
#define CACHE_LINE_ALIGMENT(x)					(((u32)(x) + CACHE_LINE_SIZE - 1) & CACHE_LINE_ADDR_MSK)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup CACHE_Exported_Functions FLash Cache Exported Functions
  * @{
  */
/**
  * @brief  Disable/Enable I/D cache.
  * @param  Enable
  *   This parameter can be any combination of the following values:
  *		 @arg ENABLE: L1 & L2 cache enable
  *		 @arg ENABLE: L1 & L2 cache disable
  */
__STATIC_INLINE
void Cache_Enable(u32 Enable)
{
	if (Enable) {
		L1C_EnableCaches();
	} else {
		L1C_DisableCaches();
	}
}

/**
  * @brief  invalidate all I/D cache.
  */
__STATIC_INLINE
void Cache_Flush(void)
{
	L1C_InvalidateICacheAll();
	L1C_InvalidateDCacheAll();
}

/**
  * @brief  Enable Icache.
  */
__STATIC_INLINE
void ICache_Enable(void)
{
	__set_SCTLR(__get_SCTLR() | SCTLR_I_Msk);
	__ISB();
}

/**
  * @brief  Disable Icache.
  */
__STATIC_INLINE
void ICache_Disable(void)
{
	__set_SCTLR(__get_SCTLR() & (~SCTLR_I_Msk));
	__ISB();

}

/**
  * @brief  Invalidate Icache.
  */
__STATIC_INLINE
void ICache_Invalidate(void)
{
	L1C_InvalidateICacheAll();
}

/**
  * @brief  Check DCache Enabled or not.
  */
__STATIC_INLINE
u32 DCache_IsEnabled(void)
{
	return ((__get_SCTLR() & (u32)SCTLR_C_Msk) ? 1 : 0);
}

/**
  * @brief  Enable Dcache.
  */
__STATIC_INLINE
void DCache_Enable(void)
{
	__set_SCTLR(__get_SCTLR() | SCTLR_C_Msk);
	__ISB();
}

/**
  * @brief  Disable Dcache.
  */
__STATIC_INLINE
void DCache_Disable(void)
{
	__set_SCTLR(__get_SCTLR() & (~SCTLR_C_Msk));
	__ISB();
}

/**
  * @brief   D-Cache Invalidate by address.
  * @details Invalidates D-Cache for the given address
  * @param  Address    address (aligned to 32-byte boundary)
  * @param  Bytes   size of memory block (in number of bytes)
  *
  * @note  Dcache will be restored from memory.
  * @note  This can be used after DMA Rx, and CPU read DMA data from DMA buffer.
  * @note  if Address is 0xFFFFFFFF, it means dont care, it was used when all Dcache be Invalidated.
  */
__STATIC_INLINE
void DCache_Invalidate(u32 Address, u32 Bytes)
{
	u32 addr = Address, len = Bytes;
	u32 mva_addr;

	if (DCache_IsEnabled() == 0) {
		return;
	}

	if ((Address == 0xFFFFFFFF) && (Bytes == 0xFFFFFFFF)) {
		L1C_InvalidateDCacheAll();
	} else {
		/* Align start to cache line boundary */
		addr &= ~(CACHE_L1_DCACHE_LINE_SIZE - 1);   //64-byte aligned
		len = ((((Address + Bytes - 1) >> 6) + 1) << 6) - addr; //next 64-byte aligned
		for (mva_addr = addr; mva_addr < (addr + len); mva_addr += CACHE_L1_DCACHE_LINE_SIZE) {
			/* DCIMVAC - Invalidate data cache by MVA to PoC */
			__set_DCIMVAC((uint32_t)mva_addr);
		}
		__DMB();	 //ensure the ordering of data cache maintenance operations and their effects
	}
}

/**
  * @brief   D-Cache Clean by address
  * @details Cleans D-Cache for the given address
  * @param  Address    address (aligned to 32-byte boundary)
  * @param  Bytes   size of memory block (in number of bytes)
  *
  * @note  Dcache will be write back to memory.
  * @note  This can be used before DMA Tx, after CPU write data to DMA buffer.
  * @note  if Address is 0xFFFFFFFF, it means dont care, it was used when all Dcache be cleaned.
  * @note  AmebaD cache is default read allocation and write through, so clean is not needed.
  */
__STATIC_INLINE
void DCache_Clean(u32 Address, u32 Bytes)
{
	u32 addr = Address, len = Bytes;
	u32 mva_addr;

	if (DCache_IsEnabled() == 0) {
		return;
	}

	if ((Address == 0xFFFFFFFF) && (Bytes == 0xFFFFFFFF)) {
		L1C_CleanDCacheAll();
	} else {
		/* Align start to cache line boundary */
		addr &= ~(CACHE_L1_DCACHE_LINE_SIZE - 1);   //64-byte aligned
		len = ((((Address + Bytes - 1) >> 6) + 1) << 6) - addr; //next 64-byte aligned
		for (mva_addr = addr; mva_addr < (addr + len); mva_addr += CACHE_L1_DCACHE_LINE_SIZE) {
			/* DCCMVAC - Clean data cache by MVA to PoC */
			__set_DCCMVAC((uint32_t)mva_addr);
		}
		__DMB();	 //ensure the ordering of data cache maintenance operations and their effects
	}
}


/**
  * @brief   D-Cache Clean and Invalidate by address
  * @details Cleans and invalidates D_Cache for the given address
  * @param  Address    address (aligned to 32-byte boundary)
  * @param  Bytes   size of memory block (in number of bytes)
  *
  * @note  This can be used when you want to write back cache data and then Invalidate cache.
  * @note  if Address is 0xFFFFFFFF, it means dont care, it was used when all Dcache be cleaned.
  */
__STATIC_INLINE
void DCache_CleanInvalidate(u32 Address, u32 Bytes)
{
	u32 addr = Address, len = Bytes;
	u32 mva_addr;

	if (DCache_IsEnabled() == 0) {
		return;
	}

	if ((Address == 0xFFFFFFFF) && (Bytes == 0xFFFFFFFF)) {
		L1C_CleanInvalidateDCacheAll();
	} else {
		/* Align start to cache line boundary */
		addr &= ~(CACHE_L1_DCACHE_LINE_SIZE - 1);   //64-byte aligned
		len = ((((Address + Bytes - 1) >> 6) + 1) << 6) - addr; //next 64-byte aligned
		for (mva_addr = addr; mva_addr < (addr + len); mva_addr += CACHE_L1_DCACHE_LINE_SIZE) {
			/* DCCIMVAC - Clean & Invalidate data cache by MVA to PoC */
			__set_DCCIMVAC((uint32_t)mva_addr);
		}
		__DMB();	 //ensure the ordering of data cache maintenance operations and their effects
	}
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif //_AMEBAD2_AP_CACHE_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
