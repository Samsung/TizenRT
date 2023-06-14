/**
  ******************************************************************************
  * @file    ameba_clk_rom.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the NCO clock:
  *           - NCO32k
  *           - NCO8M
  *           - EXT32k Disable/Enable
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

HAL_ROM_DATA_SECTION static const u32 XTAL_CLK[] = {
	40000000, /* EFUSE 0000: default */
	25000000, /* EFUSE 0001: */
	13000000, /* EFUSE 0010: */
	19200000, /* EFUSE 0011: */
	20000000, /* EFUSE 0100: */
	26000000, /* EFUSE 0101: */
	38400000, /* EFUSE 0110: */
	17664000, /* EFUSE 0111: */
	16000000, /* EFUSE 1000: */
	14318000, /* EFUSE 1001: */
	12000000, /* EFUSE 1010: */
	52000000, /* EFUSE 1011: */
	48000000, /* EFUSE 1100: */
	27000000, /* EFUSE 1101: */
	24000000, /* EFUSE 1110: */
	40000000, /* EFUSE 1111: */
};

HAL_ROM_DATA_SECTION static const u32 CpkClkTbl_ASIC_HP[] = {
	40000000,	/* 0 XTAL clock */
	400000000,	/* 1 */
	266667000,	/* 2 */
	200000000,	/* 3 */
	160000000,	/* 4 */
	133333000,	/* 5 */
	114285700,	/* 6 */
	100000000,	/* 7 */
};


/**
  * @brief  Get XTAL clock.
  * @retval  can be one of the following values:
  *		 @arg 40000000
  *		 @arg 25000000
  *		 @arg 13000000
  *		 @arg 19200000
  *		 @arg 20000000
  *		 @arg 26000000
  *		 @arg 38400000
  *		 @arg 17664000
  *		 @arg 16000000
  *		 @arg 14318000
  *		 @arg 12000000
  *		 @arg 52000000
  *		 @arg 48000000
  *		 @arg 26000000
  *		 @arg 27000000
  *		 @arg 24000000
  */
__weak HAL_ROM_TEXT_SECTION
u32 XTAL_ClkGet(void)
{
	u32 clock_index = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);

	clock_index = LSYS_GET_CKSL_XTAL(clock_index);

	return XTAL_CLK[clock_index];
}

/**
  * @brief  Set CPU clock.
  * @param  CpuType: This parameter can be one of the following values:
  * For LP:
  *		 @arg CLK_KM0_XTAL_40M
  *		 @arg CLK_KM0_XTAL_26M
  *		 @arg CLK_KM0_ANA_4M
  *For HP
  *		 @arg CLK_KM4_100M
  *		 @arg CLK_KM4_114P3M
  *		 @arg CLK_KM4_133P3M
  *		 @arg CLK_KM4_160M
  *		 @arg CLK_KM4_200M
  *		 @arg CLK_KM4_266P7M
  *		 @arg CLK_KM4_400M
  *		 @arg CLK_KM4_XTAL
  * @note REG_LP_KM4_CTRL default is 25MHz
  */
__weak HAL_ROM_TEXT_SECTION
void CPU_ClkSet(u8  CpuClk)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	u32 CPUID = SYS_CPUID();

	if (CPUID == LP_CPU_ID) {
		Temp = Temp & ~(LSYS_MASK_CKSL_LSOC);
		Temp = Temp | LSYS_CKSL_LSOC(CLK_KM0_GET_LSOC_SEL(CpuClk));

		if (CpuClk != CLK_KM0_ANA_4M) {
			Temp = Temp & ~(LSYS_MASK_CKSL_XTAL);
			Temp = Temp | LSYS_CKSL_XTAL(CLK_KM0_GET_XTAL_SEL(CpuClk));
		}
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);

	} else if (CPUID == NP_CPU_ID) {
		if (CpuClk == CLK_KM4_XTAL) {
			Temp = Temp & ~(LSYS_BIT_CKSL_NP);
		} else {
			Temp = Temp | LSYS_BIT_CKSL_NP;
		}
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);

		if (CpuClk != CLK_KM4_XTAL) {
			Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0);
			Temp = Temp & ~(LSYS_MASK_CKD_NP);
			Temp = Temp | LSYS_CKD_NP(CpuClk);
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, Temp);
		}
	}
}
/**
  * @brief  Get CPU clock.
  * @retval  can be one of the following values:
  * For LP:
  *		 @arg 26000000
  *		 @arg 40000000
  *		 @arg 4000000
  * For HP:
  *		 @arg 400000000
  *		 @arg 266667000
  *		 @arg 200000000
  *		 @arg 160000000
  *		 @arg 133333000
  *		 @arg 114285700
  *		 @arg 100000000
  *		 @arg XTAL
  */
__weak HAL_ROM_TEXT_SECTION
u32 CPU_ClkGet(void)
{
	u32  CpuClk = 0;//, FreqDown = 0;
	u32  Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	u32 CPUID = SYS_CPUID();
	u32 ChipType = SYSCFG_CHIPType_Get();

	if (CPUID == LP_CPU_ID) {
		Temp = LSYS_GET_CKSL_LSOC(Temp);

		if (ChipType == CHIP_TYPE_FPGA) {
			CpuClk = 20000000;
		} else {
			if (Temp & 0x2) {
				CpuClk = XTAL_ClkGet();
			} else {
				CpuClk = 4000000;
			}
		}
	} else if (CPUID == NP_CPU_ID) {
		if (ChipType == CHIP_TYPE_FPGA) {
			Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FPGA_CLK_INFO);
			CpuClk = LSYS_GET_FPGA_NP_CLK(Temp);
			if (CpuClk == 0) {
				CpuClk = 50000000;
			} else {
				CpuClk = CpuClk * 1000000;
			}
		} else {
			Temp = LSYS_GET_CKSL_NP(Temp);

			if (Temp != CLK_KM4_XTAL) {
				Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0);
				Temp = LSYS_GET_CKD_NP(Temp);
			}

			CpuClk = CpkClkTbl_ASIC_HP[Temp];
		}
	}
	return CpuClk;
}


/**
  * @brief  Set HBUS clock.
  * @param  Source: This parameter can be one of the following values:
  *		 @arg CKSL_HBUS_XTAL
  *		 @arg CKSL_HBUS_PLL
  */
__weak HAL_ROM_TEXT_SECTION
void HBUS_ClkSet(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_HBUS);
	Temp |= LSYS_CKSL_HBUS(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
