/**
  ******************************************************************************
  * @file    ameba_mipi.c
  * @author
  * @version V1.0.0
  * @date    2021-05-27
  * @brief   This file contains all the functions prototypes for the MIPI firmware
  *             library, including the following functionalities of DSI/DPHY controller:

  *           - MIPI Initialization For DSI Vedio Mode
  *           - DSI Command Mode
  *           - Interrupts and flags management
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

static const char *TAG = "MIPI";

void MIPI_DPHY_Reset(MIPI_TypeDef *MIPIx)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_CLOCK_GEN;
	Value32 &= ~(MIPI_BIT_TXBYTECLKHS_CKINV | MIPI_BIT_TXCLKESC_CKINV | MIPI_MASK_TXCLKESC_SEL);
	Value32 &= ~(MIPI_BIT_TX_RST_N | MIPI_BIT_RX_RST_N | MIPI_BIT_CLK_LANE_EN | MIPI_BIT_LANE3_EN | MIPI_BIT_LANE2_EN | MIPI_BIT_LANE1_EN | MIPI_BIT_LANE0_EN);
	Value32 |= MIPI_BIT_MTX_EN;
	MIPIx->MIPI_CLOCK_GEN = Value32;

	/* set DPHY watchdog */
	Value32 = MIPIx->MIPI_WATCHDOG;
	Value32 &= ~(MIPI_BIT_RSTB | MIPI_BIT_EN_WDOG);
	Value32 &= ~(MIPI_MASK_TIME2_RST_WIDTH | MIPI_MASK_DFS_TIME_RDY_CKOUT | MIPI_MASK_TIME_RDY_CKOUT | MIPI_MASK_TIME0_CK | MIPI_MASK_F390K);
	Value32 |= MIPI_TIME2_RST_WIDTH(1) | MIPI_DFS_TIME_RDY_CKOUT(1) | MIPI_TIME_RDY_CKOUT(2) | MIPI_TIME0_CK(3) | MIPI_F390K(1);
	MIPIx->MIPI_WATCHDOG = Value32;

	/* PLL divider setting */
	Value32 = MIPIx->MIPI_DF;
	Value32 &= ~(MIPI_BIT_POSTDIV4 | MIPI_BIT_POSTDIV3 | MIPI_BIT_POSTDIV2 | MIPI_BIT_EN_ALL_DIV4 | MIPI_BIT_EN_ALL_DIV2);
	Value32 &= ~(MIPI_MASK_LPRX_VTH_SEL | MIPI_MASK_LPCD_LEVTH_SEL | MIPI_MASK_LPCD_HEVTH_SEL | MIPI_MASK_EN_LPCD);
	Value32 |= MIPI_BIT_MBIAS_EN | MIPI_LPRX_VTH_SEL(4) | MIPI_LPCD_LEVTH_SEL(4) | MIPI_LPCD_HEVTH_SEL(4) | MIPI_MASK_EN_LPRX | MIPI_EN_LPCD(1);
	Value32 &= ~(MIPI_BIT_PWCT | MIPI_BIT_MBIAS_EN);
	MIPIx->MIPI_DF = Value32;

	/* DPHY/APHY reset */
	MIPIx->MIPI_MAIN_CTRL |= MIPI_BIT_APHY_PWDB | MIPI_BIT_APHY_RSTB | MIPI_BIT_DPHY_TX_RSTN;
}

void MIPI_DPHY_Reset_Release(MIPI_TypeDef *MIPIx, u32 dataLane_freq, u8 lane_num)
{
	u32 Value32;
	u8 div4, div2;
	if (dataLane_freq <= 200) { //100M ~ 200M
		div4 = 1;
		div2 = 0;
	} else if (dataLane_freq <= 400) { //200M ~ 400M
		div4 = 0;
		div2 = 1;
	} else { //200M ~ 400M
		div4 = 0;
		div2 = 0;
	}

	/* PLL divider setting */
	Value32 = MIPIx->MIPI_DF;
	Value32 &= ~(MIPI_MASK_LPRX_VTH_SEL | MIPI_MASK_LPCD_LEVTH_SEL | MIPI_MASK_LPCD_HEVTH_SEL | MIPI_MASK_EN_LPCD);
	Value32 |= MIPI_BIT_PWCT | MIPI_BIT_MBIAS_EN | MIPI_LPRX_VTH_SEL(4) | MIPI_LPCD_LEVTH_SEL(4) | MIPI_LPCD_HEVTH_SEL(4) |
			   MIPI_MASK_EN_LPRX | MIPI_EN_LPCD(1) | MIPI_BIT_POSTDIV4 | MIPI_BIT_POSTDIV3 | MIPI_BIT_POSTDIV2;
	Value32 |= (div4 == 0) ? 0 : MIPI_BIT_EN_ALL_DIV4;
	Value32 |= (div2 == 0) ? 0 : MIPI_BIT_EN_ALL_DIV2;
	MIPIx->MIPI_DF = Value32;

	/* set DPHY watchdog */
	Value32 = MIPIx->MIPI_WATCHDOG;
	Value32 &= ~MIPI_BIT_EN_WDOG;
	Value32 &= ~(MIPI_MASK_TIME2_RST_WIDTH | MIPI_MASK_DFS_TIME_RDY_CKOUT | MIPI_MASK_TIME_RDY_CKOUT | MIPI_MASK_TIME0_CK | MIPI_MASK_F390K);
	/* set RSTB to 1 until PHY clock setting ready */
	Value32 |= MIPI_TIME2_RST_WIDTH(1) | MIPI_DFS_TIME_RDY_CKOUT(1) | MIPI_TIME_RDY_CKOUT(2) | MIPI_TIME0_CK(1) | MIPI_BIT_RSTB | MIPI_F390K(1);
	MIPIx->MIPI_WATCHDOG = Value32;

	/* select lanes for disable escape mode, enable pll for all lanes */
	Value32 = MIPIx->MIPI_CLOCK_GEN;
	Value32 &= ~(MIPI_BIT_TXBYTECLKHS_CKINV | MIPI_BIT_TXCLKESC_CKINV | MIPI_MASK_TXCLKESC_SEL);
	Value32 &= ~(MIPI_BIT_PLL_LANE3_ENABLE | MIPI_BIT_PLL_LANE2_ENABLE | MIPI_BIT_PLL_LANE1_ENABLE | MIPI_BIT_PLL_LANE0_ENABLE |
				 MIPI_BIT_LANE3_EN | MIPI_BIT_LANE2_EN | MIPI_BIT_LANE1_EN | MIPI_BIT_LANE0_EN);
	Value32 |= (MIPI_BIT_MTX_EN | MIPI_BIT_PLL_CK_LANE_ENABLE | MIPI_BIT_TX_RST_N | MIPI_BIT_RX_RST_N | MIPI_BIT_CLK_LANE_EN);
	if (lane_num == 1) {
		Value32 |= (MIPI_BIT_PLL_LANE0_ENABLE | MIPI_BIT_LANE0_EN);
	} else if (lane_num == 2) {
		Value32 |= (MIPI_BIT_PLL_LANE1_ENABLE | MIPI_BIT_PLL_LANE0_ENABLE | MIPI_BIT_LANE1_EN | MIPI_BIT_LANE0_EN);
	} else {
		RTK_LOGE(TAG, "MIPI not support %d lane\n", lane_num);
	}
	MIPIx->MIPI_CLOCK_GEN = Value32;

	/* wait clock */
	DelayUs(4);
}

void MIPI_DPHY_PLL_Set(MIPI_TypeDef *MIPIx, u32 dataLane_freq)
{
	u8 div_number, ncode, xtal_clk = 40;
	u16 fcode;
	u32 freq_div, Value32;

	if (dataLane_freq < 100 || dataLane_freq > 1000) {
		RTK_LOGE(TAG, "dataLane_freq %dM is not support\n", dataLane_freq);
		return;
	} else if (dataLane_freq <= 200) { //100Mbps ~ 200Mbps
		div_number = 4;
	} else if (dataLane_freq <= 400) { //200Mbps ~ 400Mbps
		div_number = 2;
	} else { // > 400Mbps
		div_number = 1;
	}

	/* !!!The VCO of D-PHY cannot less than 400M!!! */
	freq_div = dataLane_freq * div_number;

	ncode = freq_div / xtal_clk - 3;
	fcode = ((freq_div % xtal_clk) * 2048) / xtal_clk;

	/*  PLL F code */
	Value32 = MIPIx->MIPI_SSC2;
	Value32 &= ~(MIPI_MASK_FCODE_T_SCPU | MIPI_MASK_FCODE_SSC);
	Value32 |= MIPI_FCODE_T_SCPU(fcode) | MIPI_FCODE_SSC(fcode);
	MIPIx->MIPI_SSC2 = Value32;

	/* PLL N code */
	Value32 = MIPIx->MIPI_SSC3 ;
	Value32 &= ~(MIPI_BIT_OC_EN | MIPI_MASK_OC_STEP_SET | MIPI_MASK_NCODE_T_SCPU | MIPI_MASK_NCODE_SSC);
	Value32 |= MIPI_OC_STEP_SET(0x28) | MIPI_NCODE_T_SCPU(ncode) | MIPI_NCODE_SSC(ncode);
	MIPIx->MIPI_SSC3 = Value32;

	/* PLL setting */
	Value32 = MIPIx->MIPI_MPLL;
	Value32 &= ~(MIPI_BIT_BYPASS_8X | MIPI_BIT_TXTST_EN | MIPI_BIT_MPLL_TESTEN | MIPI_MASK_MPLL_PDIV);
	Value32 &= ~(MIPI_MASK_TXTST_SEL | MIPI_MASK_MPLL_PI_ISEL | MIPI_MASK_MPLL_LPF_SR | MIPI_MASK_MPLL_LDO_VREF | MIPI_MASK_MPLL_ICP);
	Value32 |= MIPI_TXTST_SEL(2) | MIPI_BIT_EN_TX_LDO | MIPI_BIT_CMU_POW | MIPI_MPLL_PI_ISEL(2) | MIPI_BIT_MPLL_PI_C | MIPI_BIT_MPLL_PI_BYPASS |
			   MIPI_MPLL_LPF_SR(4) | MIPI_MPLL_LDO_VREF(2) | MIPI_MPLL_ICP(2) | MIPI_BIT_MPLL_DIV_CYC | MIPI_BIT_MPLL_BIG_CCO;
	MIPIx->MIPI_MPLL = Value32;
}

void MIPI_DPHY_Timing_Set(MIPI_TypeDef *MIPIx, u32 dataLane_freq)
{
	u8 tmp1, tmp2, tmp3;
	u8 clk_hs_prpr, hs_trail_0, hs_trail_1, hs_trail, hs_prpr;
	u32 Value32;
	/* byteclk_period = 1000 / (dataLane_freq / 8). The unit is nanoseconds */
	float byteclk_period = (float)8000.0 / (float)dataLane_freq;

	tmp1 = (u8)(50 / byteclk_period);
	tmp2 = (u8)(100 / byteclk_period) + 1;
	/* Timer setting */
	Value32 = MIPIx->MIPI_ESCAPE_TX_DATA_1;
	Value32 &= ~(MIPI_MASK_TX_LPX_TIME | MIPI_MASK_HS_EXIT_TIME | MIPI_MASK_TX_INIT_TIME);
	Value32 |= MIPI_TX_LPX_TIME(tmp1) | MIPI_HS_EXIT_TIME(tmp2) | MIPI_TX_INIT_TIME(0x1100);
	MIPIx->MIPI_ESCAPE_TX_DATA_1 = Value32;

	if (byteclk_period > 40) {
		/* datalane freq shall not less than 100Mbps */
		clk_hs_prpr = (u8)(95 / byteclk_period);
		clk_hs_prpr = (clk_hs_prpr > 1) ? (clk_hs_prpr - 1) : 0;
	} else {
		clk_hs_prpr = (u8)(38 / byteclk_period) + 1;
	}
	tmp1 = (u8)(byteclk_period / 8 * 8) + 1;
	tmp2 = (u8)(300 / byteclk_period) + 1;
	tmp3 = (u8)((60 + 52 * byteclk_period / 8) / byteclk_period) + 1;

	/* clock lane timing setting */
	Value32 = MIPIx->MIPI_ESCAPE_TX_DATA_2;
	Value32 &= ~(MIPI_MASK_CLK_PRE_TIME | MIPI_MASK_CLK_ZERO_TIME | MIPI_MASK_CLK_PRPR_TIME | MIPI_MASK_CLK_POST_TIME);
	Value32 |= MIPI_CLK_PRE_TIME(tmp1) | MIPI_CLK_ZERO_TIME(tmp2) | MIPI_CLK_PRPR_TIME(clk_hs_prpr) | MIPI_CLK_POST_TIME(tmp3);
	MIPIx->MIPI_ESCAPE_TX_DATA_2 = Value32;

	if (byteclk_period > 40) {
		hs_prpr = (u8)((85 + 6 * byteclk_period / 8) / byteclk_period);
		hs_prpr = (hs_prpr > 1) ? (hs_prpr - 1) : 0;
	} else {
		hs_prpr = (u8)((40 + 4 * byteclk_period / 8) / byteclk_period) + 1;
	}
	hs_trail_0 = 1;
	hs_trail_1 = (u8)((60 + 4 * byteclk_period / 8) / byteclk_period) + 1;
	hs_trail = (hs_trail_0 < hs_trail_1) ? hs_trail_1 : hs_trail_0;
	tmp1 = (u8)((145 + 10 * byteclk_period / 8) / byteclk_period) + 1;

	/* data lane timing setting */
	Value32 = MIPIx->MIPI_ESCAPE_TX_DATA_3;
	Value32 &= ~(MIPI_MASK_TX_VLD_TIME | MIPI_MASK_TX_TAIL_TIME | MIPI_MASK_HS_ZERO_TIME | MIPI_MASK_HS_PRPR_TIME);
	Value32 |= MIPI_TX_VLD_TIME(5) | MIPI_TX_TAIL_TIME(hs_trail) | MIPI_HS_ZERO_TIME(tmp1) | MIPI_HS_PRPR_TIME(hs_prpr);
	MIPIx->MIPI_ESCAPE_TX_DATA_3 = Value32;

	Value32 = MIPIx->MIPI_ESCAPE_TX_CLK_0;
	Value32 &= ~MIPI_MASK_CK_ULPS_EXIT_TIME;
	Value32 |= MIPI_CK_ULPS_EXIT_TIME(1000000 / 800);	/*ulps exit time should lager than 1ms*/
	MIPIx->MIPI_ESCAPE_TX_CLK_0 = Value32;

	Value32 = MIPIx->MIPI_ESCAPE_TX_DATA_6;
	Value32 &= ~MIPI_MASK_ULPS_EXIT_TIME;
	Value32 |= MIPI_ULPS_EXIT_TIME(1000000 / 50);		/*ulps exit time should lager than 1ms*/
	MIPIx->MIPI_ESCAPE_TX_DATA_6 = Value32;
}

void MIPI_DPHY_init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct)
{
	MIPI_DPHY_Reset(MIPIx);
	MIPI_DPHY_PLL_Set(MIPIx, MIPI_InitStruct->MIPI_VideDataLaneFreq);
	MIPI_DPHY_Timing_Set(MIPIx, MIPI_InitStruct->MIPI_VideDataLaneFreq);
	MIPI_DPHY_Reset_Release(MIPIx, MIPI_InitStruct->MIPI_VideDataLaneFreq, MIPI_InitStruct->MIPI_LaneNum);
}

void MIPI_DSI_TC0_Set(MIPI_TypeDef *MIPIx, u16 HSA, u16 HACT, u8 VideoDataFormat)
{
	u32 Value32;

	/* set HSA/HACT(RGB888*3, RGB565*2, RGB666*2.25, RGB666loosely*3) */
	Value32 = MIPIx->MIPI_TC0;
	Value32 &= ~(MIPI_MASK_HSA | MIPI_MASK_HACT);
	Value32 |= MIPI_HSA(HSA);

	switch (VideoDataFormat) {
	case MIPI_VIDEO_DATA_FORMAT_RGB565:
		Value32 |= MIPI_HACT(HACT * 2);
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB666_PACKED:
		Value32 |= MIPI_HACT(HACT * 18 / 8);
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB888:
	case MIPI_VIDEO_DATA_FORMAT_RGB666_LOOSELY:
	default:
		Value32 |= MIPI_HACT(HACT * 3);
		break;
	}
	MIPIx->MIPI_TC0 = Value32;
}

void MIPI_DSI_TC1_Set(MIPI_TypeDef *MIPIx, u16 HBP, u16 HFP)
{
	u32 Value32;

	/* set HBP/HFP */
	Value32 = MIPIx->MIPI_TC1;
	Value32 &= ~(MIPI_MASK_HBP | MIPI_MASK_HFP);
	Value32 |= MIPI_HBP(HBP) | MIPI_HFP(HFP);
	MIPIx->MIPI_TC1 = Value32;
}

void MIPI_DSI_TC2_Set(MIPI_TypeDef *MIPIx, u16 VSA, u16 VACT)
{
	u32 Value32;

	/* set VSA/VACT */
	Value32 = MIPIx->MIPI_TC2;
	Value32 &= ~(MIPI_MASK_VSA | MIPI_MASK_VACT);
	Value32 |= MIPI_VSA(VSA) | MIPI_VACT(VACT);
	MIPIx->MIPI_TC2 = Value32;
}

void MIPI_DSI_TC3_Set(MIPI_TypeDef *MIPIx, u16 VBP, u16 VFP)
{
	u32 Value32;

	/* set VBP/VFP */
	Value32 = MIPIx->MIPI_TC3;
	Value32 &= ~(MIPI_MASK_VBP | MIPI_MASK_VFP);
	Value32 |= MIPI_VBP(VBP) | MIPI_VFP(VFP);
	MIPIx->MIPI_TC3 = Value32;
}

void MIPI_DSI_TC4_Set(MIPI_TypeDef *MIPIx, u8 VideoModeInterface, u16 BllpLen)
{
	u32 Value32;

	/*set video mode and clock req timing*/
	Value32 = MIPIx->MIPI_TC4;
	Value32 &= ~(MIPI_MASK_CRC_LEN | MIPI_MASK_HEADER_LEN | MIPI_BIT_ACTIVE_BURST_EN | MIPI_BIT_HSE_EN | MIPI_MASK_BLLP_LEN);
	Value32 |= MIPI_CRC_LEN(2) | MIPI_HEADER_LEN(4) | MIPI_BLLP_LEN(BllpLen);
	switch (VideoModeInterface) {
	case MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES:
		Value32 |= MIPI_BIT_HSE_EN;
		break;
	case MIPI_VIDEO_BURST_MODE:
		Value32 |= MIPI_BIT_ACTIVE_BURST_EN;
		break;
	case MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_EVENTS:
	default:
		break;
	}
	MIPIx->MIPI_TC4 = Value32;
}

void MIPI_DSI_TC5_Set(MIPI_TypeDef *MIPIx, u16 PixelNum, u16 LineTime)
{
	u32 Value32;

	/*set pixel number and line time */
	Value32 = MIPIx->MIPI_TC5;
	Value32 &= ~(MIPI_MASK_PIXEL_NUM | MIPI_MASK_LINE_TIME);
	Value32 |= MIPI_PIXEL_NUM(PixelNum) | MIPI_LINE_TIME(LineTime);
	MIPIx->MIPI_TC5 = Value32;
}

void MIPI_DSI_Pat_Gen(MIPI_TypeDef *MIPIx, u8 NewState, u8 pg_type, u32 user_color)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_PAT_GEN;
	Value32 &= ~(MIPI_BIT_PG_EN | MIPI_MASK_PG_TYPE | MIPI_MASK_UD_B | MIPI_MASK_UD_R | MIPI_MASK_UD_G);
	if (NewState) {
		Value32 |= MIPI_BIT_PG_EN | MIPI_PG_TYPE(pg_type);
		Value32 |= MIPI_UD_B(user_color);
		Value32 |= MIPI_UD_G(user_color >> 8);
		Value32 |= MIPI_UD_R(user_color >> 16);
	}
	MIPIx->MIPI_PAT_GEN = Value32;
}

/**
  * @brief  Enables or disables the specified DSI interrupts.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @param  AcpuEn: specifies the Video Mode Frame Done(The End of VFP) interrupt enabled or not.
  * @param  ScpuEn: specifies the Command Mode interrupt enabled or not.
  * @param  IntSelAnS: TRUE means AcpuEn is valid, FALSE means ScpuEn is valid
  * @retval None
  */
void MIPI_DSI_INT_Config(MIPI_TypeDef *MIPIx, u8 AcpuEn, u8 ScpuEN, u8 IntSelAnS)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_INTE;
	Value32 &= ~(MIPI_BIT_INTP_EN_ACPU | MIPI_BIT_INTP_EN | MIPI_BIT_SEL);
	if (AcpuEn) {
		Value32 |= MIPI_BIT_INTP_EN_ACPU;
	}
	if (ScpuEN) {
		Value32 |= MIPI_BIT_INTP_EN;
	}

	if (IntSelAnS) {
		Value32 |= MIPI_BIT_SEL;
	}
	MIPIx->MIPI_INTE = Value32;
}

/**
  * @brief  Get DSI Scpu(Command Mode interrupt) Raw Interrupt Status.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @retval raw interrupt status
  */
u32 MIPI_DSI_INTS_Get(MIPI_TypeDef *MIPIx)
{
	return MIPIx->MIPI_INTS;
}

/**
  * @brief  Clears all of the DSI Scpu(Command Mode interrupt) pending bit.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @retval None
  */
void MIPI_DSI_INTS_Clr(MIPI_TypeDef *MIPIx, u32 DSI_INTS)
{
	/*clear the specified interrupt*/
	MIPIx->MIPI_INTS = DSI_INTS;
}

/**
  * @brief  Get DSI Acpu(Video Mode Frame Done interrupt) Raw Interrupt Status.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @retval raw interrupt status
  */
u32 MIPI_DSI_INTS_ACPU_Get(MIPI_TypeDef *MIPIx)
{
	return MIPIx->MIPI_INTS_ACPU;
}

/**
  * @brief  Clears all of the DSI Acpu(Video Mode Frame Done interrupt) pending bit.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @retval None
  */
void MIPI_DSI_INTS_ACPU_Clr(MIPI_TypeDef *MIPIx, u32 DSI_INTS)
{
	/*clear the specified interrupt*/
	MIPIx->MIPI_INTS_ACPU = DSI_INTS;
}

void MIPI_DSI_TO1_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_TO1;
	Value32 &= ~(MIPI_BIT_HSTX_TO_EN | MIPI_MASK_HSTX_TO);
	/* HSTX Timeout */
	if (NewState != DISABLE) {
		Value32 |= MIPI_BIT_HSTX_TO_EN;
		Value32 |= MIPI_HSTX_TO(TimeoutValue);
	}
	MIPIx->MIPI_TO1 = Value32;
}

void MIPI_DSI_TO2_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_TO2;
	Value32 &= ~(MIPI_BIT_LPTX_TO_EN | MIPI_MASK_LPTX_TO);
	/* LPTX Timeout */
	if (NewState != DISABLE) {
		Value32 |= MIPI_BIT_LPTX_TO_EN;
		Value32 |= MIPI_LPTX_TO(TimeoutValue);
	}
	MIPIx->MIPI_TO2 = Value32;
}

void MIPI_DSI_TO3_Set(MIPI_TypeDef *MIPIx, u8 NewState, u32 TimeoutValue)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_TO3;
	Value32 &= ~(MIPI_BIT_LPRX_TO_EN | MIPI_MASK_LPRX_TO);
	/* LPRX Timeout */
	if (NewState != DISABLE) {
		Value32 |= MIPI_BIT_LPRX_TO_EN;
		Value32 |= MIPI_LPRX_TO(TimeoutValue);
	}
	MIPIx->MIPI_TO3 = Value32;
}

void MIPI_DSI_TO4_Set(MIPI_TypeDef *MIPIx, u32 FifoReDly)
{
	/* After FIFO_RE_DLY clock cycles delay, start to read FIFO to send HS data */
	MIPIx->MIPI_TO4 = (MIPIx->MIPI_TO4 & ~MIPI_MASK_FIFO_RE_DLY) | MIPI_FIFO_RE_DLY(FifoReDly);
}

void MIPI_DSI_CMD_Send(MIPI_TypeDef *MIPIx, u8 DataId, u8 Byte0, u8 Byte1)
{
	u32 Value32;

	Value32 = MIPIx->MIPI_CMD0;
	Value32 &= ~(MIPI_MASK_CMD_BYTE1 | MIPI_MASK_CMD_BYTE0 | MIPI_MASK_CMD_DATAID);
	Value32 |= MIPI_CMD_BYTE1(Byte1);
	Value32 |= MIPI_CMD_BYTE0(Byte0);
	Value32 |= MIPI_CMD_DATAID(DataId);
	MIPIx->MIPI_CMD0 = Value32;

	MIPIx->MIPI_CMD_GO |= MIPI_BIT_CMD_GO;
}

void MIPI_DSI_CMD_LongPkt_MemQWordRW(MIPI_TypeDef *MIPIx, u32 Addr, u32 *Word0, u32 *Word1, u8 IsRead)
{
	u32 Value32;
	Value32 = MIPIx->MIPI_IDMA0;
	Value32 &= ~(MIPI_BIT_MEM_WR | MIPI_MASK_MEM_ADDR);
	Value32 |= MIPI_BIT_MEM_EN | MIPI_MEM_ADDR(Addr);
	if (IsRead) {
		Value32 |= MIPI_BIT_MEM_WR;
		MIPIx->MIPI_IDMA0 = Value32;
		*Word0 = MIPIx->MIPI_IDMA1;
		*Word1 = MIPIx->MIPI_IDMA2;
	} else {
		MIPIx->MIPI_IDMA1 = *Word0;
		MIPIx->MIPI_IDMA2 = *Word1;
		MIPIx->MIPI_IDMA0 = Value32;
	}
}

u32 MIPI_DSI_CMD_Rxcv_CMD(MIPI_TypeDef *MIPIx, u8 rcmd_idx)
{
	return MIPIx->MIPI_RCMD_x[rcmd_idx];
}

void MIPI_DSI_Mode_Switch(MIPI_TypeDef *MIPIx, u32 MIPI_VideoNCmdMode)
{
	if (MIPI_VideoNCmdMode) {
		MIPIx->MIPI_MAIN_CTRL |= MIPI_BIT_DSI_MODE;
	} else {
		MIPIx->MIPI_MAIN_CTRL &= ~MIPI_BIT_DSI_MODE;
	}
}

void MIPI_DSI_Main_Ctrl(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct)
{
	u32 Value32;

	/* set MIPIDSI HS mode enable/setting data lane num */
	Value32 = MIPIx->MIPI_MAIN_CTRL;
	Value32 &= ~(MIPI_BIT_DPHY_ISO_CTRL | MIPI_BIT_APHY_PWDB | MIPI_BIT_APHY_RSTB | MIPI_BIT_DPHY_TX_RSTN | MIPI_BIT_RX_ECC_EN |
				 MIPI_BIT_RX_CRC_EN | MIPI_BIT_TURNDISABLE | MIPI_BIT_TX_CRC_EN | MIPI_BIT_EOTP_EN | MIPI_MASK_VID_FORMAT | MIPI_MASK_LANE_NUM |
				 MIPI_BIT_LPRX_RST | MIPI_BIT_LPTX_RST | MIPI_BIT_HSTX_RST | MIPI_BIT_DSI_MODE);
	MIPIx->MIPI_MAIN_CTRL = Value32;

	if (MIPI_InitStruct->MIPI_RxEccChkEn) {
		Value32 |= MIPI_BIT_RX_ECC_EN;
	}

	if (MIPI_InitStruct->MIPI_RxCrcChkEn) {
		Value32 |= MIPI_BIT_RX_CRC_EN;
	}

	if (MIPI_InitStruct->MIPI_TxCrcCalculationEn) {
		Value32 |= MIPI_BIT_TX_CRC_EN;
	}

	if (MIPI_InitStruct->MIPI_BTADis) {
		Value32 |= MIPI_BIT_TURNDISABLE;
	}

	if (MIPI_InitStruct->MIPI_EotpEn) {
		Value32 |= MIPI_BIT_EOTP_EN;
	}

	Value32 |= MIPI_VID_FORMAT(MIPI_InitStruct->MIPI_VideoDataFormat);
	Value32 |= MIPI_LANE_NUM(MIPI_InitStruct->MIPI_LaneNum - 1);

	if (MIPI_InitStruct->MIPI_VideoNCmdMode) {
		Value32 |= MIPI_BIT_DSI_MODE;
	} else {
		/* Command Mode */
	}

	Value32 |= MIPI_BIT_APHY_PWDB | MIPI_BIT_APHY_RSTB | MIPI_BIT_DPHY_TX_RSTN;

	MIPIx->MIPI_MAIN_CTRL = Value32;
}

void MIPI_DSI_Timing_Ctrl(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct)
{
	/* Video Timing Set */
	MIPI_DSI_TC0_Set(MIPIx, MIPI_InitStruct->MIPI_HSA, MIPI_InitStruct->MIPI_HACT, MIPI_InitStruct->MIPI_VideoDataFormat);
	MIPI_DSI_TC1_Set(MIPIx, MIPI_InitStruct->MIPI_HBP, MIPI_InitStruct->MIPI_HFP);
	MIPI_DSI_TC2_Set(MIPIx, MIPI_InitStruct->MIPI_VSA, MIPI_InitStruct->MIPI_VACT);
	MIPI_DSI_TC3_Set(MIPIx, MIPI_InitStruct->MIPI_VBP, MIPI_InitStruct->MIPI_VFP);
	MIPI_DSI_TC4_Set(MIPIx, MIPI_InitStruct->MIPI_VideoModeInterface, MIPI_InitStruct->MIPI_BllpLen);
	MIPI_DSI_TC5_Set(MIPIx, MIPI_InitStruct->MIPI_HACT, MIPI_InitStruct->MIPI_LineTime);
}

void MIPI_DSI_init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct)
{
	MIPI_DSI_Timing_Ctrl(MIPIx, MIPI_InitStruct);
	MIPI_DSI_Main_Ctrl(MIPIx, MIPI_InitStruct);
}

void MIPI_StructInit(MIPI_InitTypeDef *MIPI_InitStruct)
{
	MIPI_InitStruct->MIPI_VideoNCmdMode = FALSE;
	MIPI_InitStruct->MIPI_VideoDataFormat  = MIPI_VIDEO_DATA_FORMAT_RGB888;
	MIPI_InitStruct->MIPI_VideoModeInterface = MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES;

	MIPI_InitStruct->MIPI_TxCrcCalculationEn = ENABLE;
	MIPI_InitStruct->MIPI_RxEccChkEn = ENABLE;
	MIPI_InitStruct->MIPI_RxCrcChkEn = ENABLE;

	MIPI_InitStruct->MIPI_EotpEn = DISABLE;
	MIPI_InitStruct->MIPI_BTADis = DISABLE;

	MIPI_InitStruct->MIPI_LaneNum = 2;
	MIPI_InitStruct->MIPI_FrameRate = 60;

	/*DataLaneFreq * LaneNum = FrameRate * (VSA+VBP+VACT+VFP) * (HSA+HBP+HACT+HFP) * PixelFromat*/
	MIPI_InitStruct->MIPI_VideDataLaneFreq = 775;

	/* HSA(Byte) = tHSA * LaneNum / UI*/
	MIPI_InitStruct->MIPI_HSA = 33;
	MIPI_InitStruct->MIPI_HBP = 100;
	MIPI_InitStruct->MIPI_HACT = 1024; /* pixel */
	MIPI_InitStruct->MIPI_HFP = 100;

	MIPI_InitStruct->MIPI_VSA = 2;
	MIPI_InitStruct->MIPI_VBP = 5;
	MIPI_InitStruct->MIPI_VACT = 768;
	MIPI_InitStruct->MIPI_VFP = 10;

	MIPI_InitStruct->MIPI_LineTime = 1250 * 3 / MIPI_InitStruct->MIPI_LaneNum;
	MIPI_InitStruct->MIPI_BllpLen = MIPI_InitStruct->MIPI_LineTime / 2;

}

void MIPI_BG_CMD(u32 NewStatus)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);

	if (NewStatus) {
		Temp |= LSYS_BIT_BG_PWR | LSYS_BIT_BG_ON_MIPI;
	} else {
		Temp &= ~LSYS_BIT_BG_ON_MIPI; //Do not close LSYS_BIT_BG_PWR for usb/ddr may use it
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, Temp);
}

/**
  * @brief  Initializes the MIPIx DPHY and DSI module according to the specified parameters in the MIPI_InitStruct.
  * @param  MIPIx: where MIPIx can be MIPI.
  * @param  MIPI_InitStruct: pointer to a MIPI_InitTypeDef structure that contains
  *              the configuration information for MIPIx DPHY and DSI module.
  * @retval None
  */
void MIPI_Init(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct)
{
	/* Enable BG */
	MIPI_BG_CMD(ENABLE);

	MIPI_DPHY_init(MIPIx, MIPI_InitStruct);
	MIPI_DSI_init(MIPIx, MIPI_InitStruct);
}

