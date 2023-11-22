/**
  ******************************************************************************
  * @file    rtl8721dhp_ddrphy.c
  * @author
  * @version V1.0.0
  * @date    2020-08-20
  * @brief   This file contains all the functions prototypes for the ddrphy firmware
  *             library, including the following functionalities of the DDR PHY:
  *           -Initialization
  *
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */
#include "ameba_soc.h"
#include "ameba_ddrphy_scanpara.h"
#define DDRPHY_CKREF 40

#define DDRPHY_SSC_EN	DISABLE
#define DDRPHY_LOOPBACK	DISABLE

#if DDR_SCAN_PARA || DDRPHY_LOOPBACK
#define ChipInfo_Get()				0xFF

#define SUPPORT_DYNAMIC_POWEROFF	DISABLE
#else
/*if cmd_ex_rd_str_num = cmd_rd_str_num-1, let cke_ph_sel-1 and cs_ph_sel-1 of CMD_ADR_PH, then cmd_ex_rd_str_num can equal to cmd_rd_str_num*/
#define SUPPORT_DYNAMIC_POWEROFF	DISABLE//ENABLE
#endif

static const char *TAG = "DDRPHY";
typedef struct {
	u32 ODT_TTCP0_SET0;
	u32 ODT_TTCN0_SET0;
	u32 OCDP0_SET0;
	u32 OCDN0_SET0;
	u8 RZQ_480CODE;
	u8 READ_CTRL_0_SET0;
	u8 READ_CTRL_0_SET1;
	u8 READ_CTRL_1;
} DDRPHY_ZQ_Rx_Scan_Def;

typedef struct {
	u32 PLL_PI0;
	u32 PLL_PI1;
	u32 PLL_PI2;
	u32 PLL_PI3;
	u32 PLL_CTL1;
	u32 AFIFO_STR_0;
	u32 AFIFO_STR_1;
	u32 AFIFO_STR_SEL;
} DDRPHY_Tx_Scan_Def;

/*
static const unsigned short rl6678_pll_sel_table[7][13] = {
	//v11_ldo_vsel, post_pi_rs, post_pi_rl, post_pi_bias, pll_sel_cpmode, pll_ldo_vsel, dpi_pdiv, lpf_sr, lpf_cp, loop_pi_isel, dpi_icp, cco_kvco, cco_band
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //1440 MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //1280MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //1120MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //960MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //800MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //640MHz
	{3		, 1		, 5		, 2		, 2		, 1		, 2		, 2		, 0		, 1		, 11		, 1		, 2}, //320MHz
};
*/

static const DDRPHY_ZQ_Rx_Scan_Def ddrphy_zq_rx_scan[] = {
	//ODT_TTCP0_SET0,	ODT_TTCN0_SET0,	OCDP0_SET0,		OCDN0_SET0,		RZQ_480CODE,	READ_CTRL_0_SET0,	READ_CTRL_0_SET1,	READ_CTRL_1,
	{0x00000000,		0x00000000,		0x26262626,		0x26262626,		0xF,			0x3,				0x3,				0x3},//Default
	{0x0E141414,		0x140E0E0E,		0x261F362E,		0x261C3029,		0xF,			0x2,				0x2,				0x3},//QFN228_DDR3L(0xE0)
	{0x0E141414,		0x140E0E0E,		0x261F362E,		0x261C3029,		0xF,			0x2,				0x2,				0x3},//QFN144_DDR3L(0x02)
	{0x0a0c0b0b,		0x0b0b0a0a,		0x26172721,		0x2615231e,		0x6,			0x2,				0x2,				0x4},//QFN144_DDR2(0x03)
	{0x0a0e0c0c,		0x0c0b0a0a,		0x261e362c,		0x26162620,		0x3,			0x2,				0x2,				0x4},//QFN100_DDR2(0x05)

	{0x0E141414,		0x140E0E0E,		0x26243c34,		0x261c3029,		0xF,			0x3,				0x3,				0x3},//QFN144_DDR3L(0x09)
	{0x090b0a0a,		0x0a0a0909,		0x2615231e,		0x2613201b,		0x4,			0x2,				0x2,				0x4},//QFN144_DDR2(0x0A)
	{0x0b0f0c0c,		0x0c0b0b0b,		0x26233A30,		0x26182722,		0x5,			0x2,				0x2,				0x4},//QFN100_DDR2(0x07)

	{0x0e141414,		0x140e0e0e,		0x26243c34,		0x261b2f28,		0xF,			0x3,				0x3,				0x3},//QFN144_DDR3L(0x0D)
	{0x0a0c0c0c,		0x0c0a0a0a,		0x2616251f,		0x2614221d,		0x5,			0x2,				0x2,				0x4},//QFN144_DDR2(0x0E)
};

static const DDRPHY_Tx_Scan_Def ddrphy_tx_scan[] = {
	//PLL_PI0,		PLL_PI1,	PLL_PI2,	PLL_PI3,	PLL_CTL1,	AFIFO_STR_0,	AFIFO_STR_1,	AFIFO_STR_SEL
	{0x00000000,	0x00080000,	0x00080808,	0x00000000,	0x20000000,	0x33333333,		0x000c0022,		0x00000000},//Default
	{0x07060012,	0x000f0707,	0x080f0f0f,	0x00000008,	0x20000001,	0x22222222,		0x000c0012,		0x34000000},//QFN228_DDR3L(0xE0)
	{0x0b0b0016,	0x00130b0b,	0x0e131313,	0x0000000e,	0x200003c1,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR3L(0x02)
	{0x0e0e0018,	0x00170e0e,	0x0e171717,	0x0000000e,	0x200003c1,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR2(0x03)
	{0x0a0a0018,	0x00130a0a,	0x0e131313,	0x0000000e,	0x200003c1,	0x22222222,		0x000c0012,		0x34000000},//QFN100_DDR2(0x05)

	{0x0e0e001a,	0x00170e0e,	0x12171717,	0x00000012,	0x20000fc1,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR3L(0x09)
	{0x0f0f0016,	0x00180f0f,	0x0e181818,	0x0000000e,	0x200003c1,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR2(0x0A)
	{0x0f0f001b,	0x00180f0f,	0x13181818,	0x00000013,	0x20000fc1,	0x22222222,		0x000c0012,		0x34000000},//QFN100_DDR2(0x07)

	{0x1110001b,	0x00191111,	0x13191919,	0x00000013,	0x20000ffd,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR3L(0x0D)
	{0x1313001c,	0x001c1313,	0x121c1c1c,	0x00000012,	0x20000ffd,	0x22222222,		0x000c0012,		0x34000000},//QFN144_DDR2(0x0E)
};

static u8 DDR_PHY_ChipInfo(void)
{
	static u8 s_chipinfo_ddr = 0xFF; //0xFF means None

	if (s_chipinfo_ddr != 0xFF) {
		return s_chipinfo_ddr;
	}

	switch (ChipInfo_Get()) {
	case 0xE0:
		s_chipinfo_ddr = 1;
		break;
	case 0x02:
		s_chipinfo_ddr = 2;
		break;
	case 0x03:
		s_chipinfo_ddr = 3;
		break;
	case 0x05:
		s_chipinfo_ddr = 4;
		break;
	case 0x09:
		s_chipinfo_ddr = 5;
		break;
	case 0x0A:
		s_chipinfo_ddr = 6;
		break;
	case 0x07:
		s_chipinfo_ddr = 7;
		break;
	case 0x0D:
		s_chipinfo_ddr = 8;
		break;
	case 0x0E:
		s_chipinfo_ddr = 9;
		break;
	default:
		RTK_LOGE(TAG, "DRAM is not Calibraion\r\n");
		s_chipinfo_ddr = 0;
		break;
	}

	return s_chipinfo_ddr;
}

u8 DDR_PHY_ChipInfo_ddrtype(void)
{
	static u8 s_chipinfo_ddrtpye = 0;

	if (s_chipinfo_ddrtpye == 0) {
		s_chipinfo_ddrtpye = ChipInfo_DDRType();
	}
	return s_chipinfo_ddrtpye;
}

/**
  * @brief  Fills each PSRAM_InitStruct member with its default value.
  * @param  PSRAM_InitStruct: pointer to an PSRAM_InitTypeDef structure which will be initialized.
  * @retval None
  */
void DDR_PHY_StructInit(DDRPHY_InitTypeDef *DDRPHY_InitStruct)
{
	DDRPHY_InitStruct->ddr_freq = 533;
}

void DDR_PHY_PLL_SEL(const unsigned short *pll_table)
{

	u32 DDRPHY_pll_ctl3;

	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	//DDRPHY_BIT_DPI_CKREF_SEL & dpi_pll_dbug_en
	DDRPHY_pll_ctl3 = DDRPHY_DPI_V11_LDO_VSEL(pll_table[0]) | DDRPHY_DPI_POST_PI_RS(pll_table[1]) | \
					  DDRPHY_DPI_POST_PI_RL(pll_table[2]) | DDRPHY_DPI_POST_PI_BIAS(pll_table[3]) | \
					  DDRPHY_DPI_PLL_SEL_CPMODE(pll_table[4]) | DDRPHY_DPI_PLL_LDO_VSEL(pll_table[5]) | \
					  DDRPHY_DPI_PDIV(pll_table[6]) | DDRPHY_DPI_LPF_SR(pll_table[7]) | \
					  DDRPHY_DPI_LPF_CP(pll_table[8]) | DDRPHY_DPI_LOOP_PI_ISEL(pll_table[9]) | \
					  DDRPHY_DPI_ICP(pll_table[10]) | DDRPHY_DPI_CCO_KVCO(pll_table[11]) | \
					  DDRPHY_DPI_CCO_BAND(pll_table[12]);

	ddr_phy->DDRPHY_PLL_CTL3 = DDRPHY_pll_ctl3;
}

void DDR_PHY_DLL_CLK_DIV(u32 DDR_Freq)
{
	u32 DDRPHY_Div_n_t, DDRPHY_Div_f_t, DDRPHY_Div_n, DDRPHY_Div_f;

	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	DDRPHY_Div_n_t = (DDR_Freq / DDRPHY_CKREF) - 3;
	DDRPHY_Div_f_t = ((DDR_Freq * 2048) - (DDRPHY_CKREF * (DDRPHY_Div_n_t + 3)) * 2048) / DDRPHY_CKREF;
	//RTK_LOGD(TAG, "DDRPHY DLL Div %d %d \n",DDRPHY_Div_n, DDRPHY_Div_f);

	/* Set DDR PHY DLL divide parameters */
	ddr_phy->DDRPHY_SSC2 = ((ddr_phy->DDRPHY_SSC2 & (~DDRPHY_MASK_DPI_F_CODE_T)) | DDRPHY_DPI_F_CODE_T(DDRPHY_Div_f_t));
	ddr_phy->DDRPHY_SSC3 = ((ddr_phy->DDRPHY_SSC3 & (~DDRPHY_MASK_DPI_N_CODE_T)) | DDRPHY_DPI_N_CODE_T(DDRPHY_Div_n_t));

#if DDRPHY_SSC_EN
	DDR_Freq = DDR_Freq * 99 / 100;
	DDRPHY_Div_n = (DDR_Freq / DDRPHY_CKREF) - 3;
	DDRPHY_Div_f = ((DDR_Freq * 2048) - (DDRPHY_CKREF * (DDRPHY_Div_n + 3)) * 2048) / DDRPHY_CKREF;
	ddr_phy->DDRPHY_SSC2 = ((ddr_phy->DDRPHY_SSC2 & (~DDRPHY_MASK_DPI_F_CODE)) | DDRPHY_DPI_F_CODE(DDRPHY_Div_f));
	ddr_phy->DDRPHY_SSC3 = ((ddr_phy->DDRPHY_SSC3 & (~DDRPHY_MASK_DPI_N_CODE)) | DDRPHY_DPI_N_CODE(DDRPHY_Div_n));

	//((Ncode_T * 2048 + Fcode_T ¨C Ncode_ssc * 2048 - Fcode_ssc) * 33K * 2 ^ 11) / ((Frequency of CKREF) * 0.5)
	u32 temp = (DDRPHY_Div_n_t * 2048 + DDRPHY_Div_f_t - DDRPHY_Div_n * 2048 - DDRPHY_Div_f) * 33 * 2048 / (DDRPHY_CKREF * 1000 / 2);
	ddr_phy->DDRPHY_SSC1 = ((ddr_phy->DDRPHY_SSC1 & (~DDRPHY_MASK_DPI_GRAN_SET)) | DDRPHY_DPI_GRAN_SET(temp));
#else
	DDRPHY_Div_n = DDRPHY_Div_n_t;
	DDRPHY_Div_f = DDRPHY_Div_f_t;
	ddr_phy->DDRPHY_SSC2 = ((ddr_phy->DDRPHY_SSC2 & (~DDRPHY_MASK_DPI_F_CODE)) | DDRPHY_DPI_F_CODE(DDRPHY_Div_f));
	ddr_phy->DDRPHY_SSC3 = ((ddr_phy->DDRPHY_SSC3 & (~DDRPHY_MASK_DPI_N_CODE)) | DDRPHY_DPI_N_CODE(DDRPHY_Div_n));
#endif

#if DDR_AUTOGATING
	//Bit-0 is used to select clock source of mck_mc in RL6678. 1'b0: select mck_cmd 1'b1: select PLL internal clock
	ddr_phy->DDRPHY_CRT_DMY = 0x1;
#endif

	u32 para_idx = DDR_PHY_ChipInfo();
	/*PI Setting*/
	ddr_phy->DDRPHY_PLL_PI0 = ddrphy_tx_scan[para_idx].PLL_PI0;
	ddr_phy->DDRPHY_PLL_PI1 = ddrphy_tx_scan[para_idx].PLL_PI1;
	ddr_phy->DDRPHY_PLL_PI2 = ddrphy_tx_scan[para_idx].PLL_PI2;
	ddr_phy->DDRPHY_PLL_PI3 = ddrphy_tx_scan[para_idx].PLL_PI3;

	/*output clock enable*/
	ddr_phy->DDRPHY_PLL_CTL1 = ddrphy_tx_scan[para_idx].PLL_CTL1; //DDRPHY_DPI_OESYNC_OP_SEL(0x3c1)
	ddr_phy->DDRPHY_PLL_CTL4 = ddr_phy->DDRPHY_PLL_CTL4 & (~DDRPHY_MASK_DPI_DLY_SEL); // ALL 0 or 0x10

	/*TX/RX FIFO threshold setting*/
	if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
		ddr_phy->DDRPHY_AFIFO_STR_2 = 0x00000033; // for DDR2
	} else {
		ddr_phy->DDRPHY_AFIFO_STR_2 = 0x00000022; // for real DDR3
	}

	ddr_phy->DDRPHY_AFIFO_STR_0 = ddrphy_tx_scan[para_idx].AFIFO_STR_0;
	ddr_phy->DDRPHY_AFIFO_STR_1 = ddrphy_tx_scan[para_idx].AFIFO_STR_1; //DDRPHY_CMD_EX_RD_STR_NUM(0x1) | DDRPHY_CMD_RD_STR_NUM(0x2)
	ddr_phy->DDRPHY_AFIFO_STR_SEL = ddrphy_tx_scan[para_idx].AFIFO_STR_SEL;

#if DDR_SCAN_PARA
	DDR_PHY_Scan_Result_Set();
#else
#if SUPPORT_DYNAMIC_POWEROFF
	if (ddr_phy->DDRPHY_AFIFO_STR_SEL != 0x0) {
		ddr_phy->DDRPHY_AFIFO_STR_1 = (ddr_phy->DDRPHY_AFIFO_STR_1 & ~DDRPHY_MASK_CMD_EX_RD_STR_NUM) | \
									  DDRPHY_CMD_EX_RD_STR_NUM(DDRPHY_GET_CMD_EX_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1) + 1);
	}
	if (DDRPHY_GET_CMD_EX_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1) != DDRPHY_GET_CMD_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1)) {
		RTK_LOGW(TAG, "CMD_EX_RD_STR_NUM != CMD_RD_STR_NUM\n");
	}

#endif
#endif

	ddr_phy->DDRPHY_AFIFO_STR_1 = ddr_phy->DDRPHY_AFIFO_STR_1 & (~DDRPHY_MASK_CMD_RD_STR_NUM_SEL_H);

	/* force rst to 1 and tcke to low before controller ready */
	if (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG) & LSYS_BIT_BOOT_WAKE_FROM_PS_HS) {
		if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
			ddr_phy->DDRPHY_CRT_CTL = 0x4C000903; //according to ddr2 type
		} else {
			ddr_phy->DDRPHY_CRT_CTL = 0x4C000821;
		}
	} else {
		if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
			ddr_phy->DDRPHY_CRT_CTL = 0x40000103; //according to ddr2 type
		} else {
			ddr_phy->DDRPHY_CRT_CTL = 0x40000021; //according to ddr3 type
		}
	}
#if DDR_AUTOGATING
	ddr_phy->DDRPHY_AFIFO_STR_1 = ((ddr_phy->DDRPHY_AFIFO_STR_1 & (~(DDRPHY_BIT_REF_RST_EN_TX | DDRPHY_BIT_REF_RST_EN_RX))) | \
								   DDRPHY_BIT_FORCE_RST_N_TX | DDRPHY_BIT_FORCE_RST_N_RX);
#else
	ddr_phy->DDRPHY_AFIFO_STR_1 = ((ddr_phy->DDRPHY_AFIFO_STR_1 & (~(DDRPHY_BIT_REF_RST_EN_TX | DDRPHY_BIT_REF_RST_EN_RX))) | \
								   DDRPHY_BIT_FORCE_RST_N_TX | DDRPHY_BIT_FORCE_RST_N_RX | \
								   DDRPHY_BIT_BYPASS_EN_TX | DDRPHY_BIT_BYPASS_EN_RX);
#endif

	//DDR_PHY_PLL_SEL(&(rl6678_pll_sel_table[DDR_Freq_idx][4]));
	ddr_phy->DDRPHY_PLL_CTL3 = (ddr_phy->DDRPHY_PLL_CTL3 & ~DDRPHY_MASK_DPI_PLL_LDO_VSEL) | DDRPHY_DPI_PLL_LDO_VSEL(0);
	ddr_phy->DDRPHY_PLL_CTL3 |= DDRPHY_BIT_DPI_CKREF_SEL;
#ifdef AMEBAD2_TODO
	ddr_phy->DDRPHY_PLL_CTL3 = 0x10685256;
#endif
}

void DDR_PHY_DUTY_CYCLE_SET(u32 duty_value)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_DCC_CSx[0] = duty_value;
	ddr_phy->DDRPHY_DCC_CMD = duty_value;
	ddr_phy->DDRPHY_DCC_DQS_SETx[0] = duty_value;
	ddr_phy->DDRPHY_DCC_DQS_SETx[1] = duty_value;
	ddr_phy->DDRPHY_DCC_DQ_SETx[0] = duty_value;
	ddr_phy->DDRPHY_DCC_DQ_SETx[1] = duty_value;
	ddr_phy->DDRPHY_DCC_CK_SETx[0] = duty_value;
}

static void  DDR_PHY_BG_CMD(u32 NewStatus)
{
	u32 Temp;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);

	if (NewStatus) {
		Temp |= LSYS_BIT_BG_PWR;
	} else {
		Temp &= ~LSYS_BIT_BG_PWR;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, Temp);
}

void DDR_PHY_CRT_Init(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	/*release DDRPLL bandgap*/
	DDR_PHY_BG_CMD(ENABLE);

	/*release PLL LDO reset*/
	ddr_phy->DDRPHY_CRT_RST_CTL |= DDRPHY_BIT_PLL_LDO_RST_N;	//release PLL LDO reset

	/*release PLL LDO reset*/
	ddr_phy->DDRPHY_CRT_RST_CTL |= (DDRPHY_BIT_POW_LDO_PLL | DDRPHY_BIT_POW_LDO_V11);	//PLL internal LDO

	//rtd_maskl(0x180be08c,0xffffffcf,0x00000030);	//PLL internal LDO ????
	ddr_phy->DDRPHY_PLL_CTL2 &= (~DDRPHY_MASK_DPI_PLL_DUM);	//ck open

	/*Release SSC NFCODE sync_reset*/
	ddr_phy->DDRPHY_CRT_RST_CTL |= DDRPHY_BIT_PLL_SSC_DIG_RST_N;	//Release SSC NFCODE sync_reset

	/*PLL and DPI enable*/
	ddr_phy->DDRPHY_CRT_RST_CTL &= (~DDRPHY_BIT_PTR_RST_N);	//PLL and DPI enable (ptr_rst_n = 0x0)
	DelayUs(5);
	ddr_phy->DDRPHY_CRT_RST_CTL |= DDRPHY_BIT_RST_N;	//PLL and DPI enable (rst_n = 0x1)
	ddr_phy->DDRPHY_CRT_RST_CTL |= DDRPHY_BIT_PTR_RST_N;	//PLL and DPI enable (ptr_rst_n = 0x0)

	/*SSC NFCODE fetch*/
	ddr_phy->DDRPHY_SSC3 |= DDRPHY_BIT_DPI_OC_EN;	//OC_EN toggle, set dpi_oc_en

#if DDRPHY_SSC_EN
	DelayUs(500); //wait ssc_oc_done = 1(in 0x1800_0260[12])
	ddr_phy->DDRPHY_SSC3 &= (~DDRPHY_BIT_DPI_OC_EN);	//OC_EN toggle, clear dpi_oc_en
	ddr_phy->DDRPHY_SSC0 |=	DDRPHY_BIT_DPI_EN_SSC;
#else
	ddr_phy->DDRPHY_SSC3 &= (~DDRPHY_BIT_DPI_OC_EN);	//OC_EN toggle, clear dpi_oc_en
#endif
	ddr_phy->DDRPHY_SSC0 |= DDRPHY_BIT_DPI_SSC_FLAG_INIT;	//Initial SSC, set ssc_flag_init

	/*Enable mck_mc(clock for controller)*/
	ddr_phy->DDRPHY_PLL_CTL1 |= DDRPHY_BIT_MCK_MC_EN;	//enable mck_mc(clock for controller) // R2489, change order

	/*turn on clock flow*/
	ddr_phy->DDRPHY_PLL_CTL0 |= DDRPHY_MASK_DPI_MCK_CLK_EN;	//set mck_clk_en ????

	/*dpi_oesync_op_sel (crt_spec) = 0x? (if value of dpi_post_pi* > 15 (now set 0), set oesync_op_sel[*] = 1)*/
	//ddr_phy->DDRPHY_PLL_CTL1 &= (~DDRPHY_MASK_DPI_OESYNC_OP_SEL); //DDR_PHY_DLL_CLK_DIV() have done

	ddr_phy->DDRPHY_PLL_CTL1 |= DDRPHY_MASK_DPI_CLK_OE;	//set dpi_clk_oe

	ddr_phy->DDRPHY_DPI_CTRL_2 = 0x90161F00;
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_FW_SET_MODE)) | DDRPHY_FW_SET_MODE(0x2);
	//ddr_phy->DDRPHY_DPI_CTRL_0 = 0x00262006; //disable dqsen calibration
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~(DDRPHY_MASK_CAL_DQSEN_MODE)) | DDRPHY_CAL_DQSEN_MODE(0x3); //disable dqsen calibration
#if SUPPORT_DYNAMIC_POWEROFF
	if (DDRPHY_GET_CMD_EX_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1) >= DDRPHY_GET_CMD_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1)) {
		/* ddrphy auto clock gating for power saving */
		/* reg_rw_pwroff_mode should be 0x1 because of small CL */
		ddr_phy->DDRPHY_PLL_CTL5 = (ddr_phy->DDRPHY_PLL_CTL5 & ~(DDRPHY_MASK_DPI_CA_PWROFF_MODE | DDRPHY_MASK_DPI_RW_PWROFF_MODE)) |
								   DDRPHY_DPI_RW_PWROFF_MODE(0x1) | DDRPHY_DPI_CA_PWROFF_MODE(0x2);
	}
#endif
}

void DDR_PHY_R240_ZQ_CAL(VOID)
{
	u32 para_idx = DDR_PHY_ChipInfo();
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	/*R240 Calibration==>PAD_RZCTRL_STATUS[5:1]*/
	//DDR3L use external 240ohm, no need to K R480
	ddr_phy->DDRPHY_PAD_RZCTRL_STATUS = (ddr_phy->DDRPHY_PAD_RZCTRL_STATUS & ~DDRPHY_MASK_RZQ_480CODE) | DDRPHY_RZQ_480CODE(
											ddrphy_zq_rx_scan[para_idx].RZQ_480CODE);

	/*ZQ Calibration*/
	/*ZQ K shall not set PAD_ZCTRL_RESULT, Because OCD/ODT will update to other register*/
	/*ODT_SEL set3 is the inverse of set1, OCD set7 keep closed by set to 00*/
	ddr_phy->DDRPHY_ODT_TTCP0_SET0 = ddrphy_zq_rx_scan[para_idx].ODT_TTCP0_SET0; //set3 ~ set0
	ddr_phy->DDRPHY_ODT_TTCP1_SET0 = 0x00000000;							  //set7 ~ set4
	ddr_phy->DDRPHY_ODT_TTCN0_SET0 = ddrphy_zq_rx_scan[para_idx].ODT_TTCN0_SET0;
	ddr_phy->DDRPHY_ODT_TTCN1_SET0 = 0x00000000;							  //00 means closed by default
	ddr_phy->DDRPHY_OCDP0_SET0 = ddrphy_zq_rx_scan[para_idx].OCDP0_SET0;		  //set3 ~ set0
	ddr_phy->DDRPHY_OCDP1_SET0 = 0x00262626;								  //set7 ~ set4
	ddr_phy->DDRPHY_OCDN0_SET0 = ddrphy_zq_rx_scan[para_idx].OCDN0_SET0;
	ddr_phy->DDRPHY_OCDN1_SET0 = 0x00262626;

	/*toggle DDRPHY_PAD_CTRL_PROG[27](dzq_auto_up), this will trigger *_SET0 to *_SET1*/
	ddr_phy->DDRPHY_PAD_CTRL_PROG |= DDRPHY_BIT_DZQ_AUTO_UP;
	ddr_phy->DDRPHY_PAD_CTRL_PROG &= ~DDRPHY_BIT_DZQ_AUTO_UP;
}

void DDR_PHY_ZQ_SET_SEL(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	/*Set OCD_SEL, ODT_SEL*/
	ddr_phy->DDRPHY_DQ_ODT_SEL_SETx[0] = 0xffff1111;  //dq1n/0n/1p/0p odt set sel
	ddr_phy->DDRPHY_DQ_ODT_SEL_SETx[1] = 0xffff1111;  //dq3n/2n/3p/2p odt set sel
	ddr_phy->DDRPHY_DQ_OCD_SEL_SETx[0] = 0xffff1111;  //dq1n/0n/1p/0p ocd set sel
	ddr_phy->DDRPHY_DQ_OCD_SEL_SETx[1] = 0xffff1111;  //dq3n/2n/3p/2p ocd set sel

	/* Dqs P shall be the opposite of Dqs N	*/
	ddr_phy->DDRPHY_DQS_P_ODT_SEL_SETx[0] = 0xffff3333;  //dqs1n/0n/1p/0p odt set sel
	ddr_phy->DDRPHY_DQS_P_ODT_SEL_SETx[1] = 0xffff3333;  //dqs3n/2n/3p/2p odt set sel
	ddr_phy->DDRPHY_DQS_N_ODT_SEL_SETx[0] = 0xffff1111;  //dqs1#n/0#n/1#p/0#p odt set sel
	ddr_phy->DDRPHY_DQS_N_ODT_SEL_SETx[1] = 0xffff1111;  //dqs3#n/2#n/3#p/2#p odt set sel
	ddr_phy->DDRPHY_DQS_OCD_SEL_SETx[0] = 0x11111111;  //dqs1#n/0#n/1#p/0#p/dqs1n/0n/1p/0p ocd set sel
	ddr_phy->DDRPHY_DQS_OCD_SEL_SETx[1] = 0x11111111;  //dqs3#n/2#n/3#p/2#p/dqs3n/2n/3p/2p ocd set sel

	ddr_phy->DDRPHY_CKE_OCD_SEL = 0xffff0000;  //cke1n/cke1p/cken/ckep ocd set sel
	ddr_phy->DDRPHY_ADR_OCD_SEL = 0xffff0000;  //cs1n/cs1p/csn/csp ocd set sel
	ddr_phy->DDRPHY_CK_OCD_SEL = 0x00000000;  //ck1#n/ck#n/ck1#p/ck#p/ck1n/ckn/ck1p/ckp ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_0 = 0x00000000;  //addr0n/0p/1n/1p/2n/2p/3n/3p ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_1 = 0x00000000;  //addr4n/4p/5n/5p/6n/6p/7n/7p ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_2 = 0x00000000;  //addr8n/8p/9n/9p/10n/10p/11n/11p ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_3 = 0x00000000;  //addr12n/12p/13n/13p/14n/14p/15n/15p ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_4 = 0x00000000;  //addr16n/16p/4exn/4exp/5exn/5exp/ba0n/ba0p ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_5 = 0x00000000;  //ba1n/1p/2n/2p/rasn/rasp/casn/casp ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_6 = 0x00000000;  //wen/wep/odtn/odtp/rstn/rstp/parn/parp ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_7 = 0x00000000;  //par1n/par1p/actn/actp/bg0n/bg0p/tenn/tenp ocd set sel
	ddr_phy->DDRPHY_OCD_SEL_8 = 0x0000ffff;  //alertn/alertp/alert1n/alert1p ocd set sel

	////Pad////=====IRT sel=====//CMD ODT is used in >2133MHz
	ddr_phy->DDRPHY_ODT_SEL_0 = 0xffffffff;  //can/cap irt set sel
	ddr_phy->DDRPHY_CKE_ODT_SEL = 0xffffffff;  //cke1n/cke1p/cken/ckep irt set sel
	ddr_phy->DDRPHY_ADR_ODT_SEL = 0xffffffff;  //cs1n/cs1p/csn/csp/rstn/rstp irt set sel
	ddr_phy->DDRPHY_CLK_ODT_SEL = 0xffffffff;  //ck1n/ck1p/ckn/ckp irt set sel

	////Pad////=====dq/dqs en_emph=====//DQ/DQS en OCD is used in >2133MHz
	ddr_phy->DDRPHY_DQ_OCD_SEL_2_SETx[0] = 0xffffffff;  //dq3n/2n/3p/2p/1n/0n/1p/0p nt set sel
	ddr_phy->DDRPHY_DQ_OCD_SEL_2_SETx[1] = 0xffffffff;  //dq3n/2n/3p/2p/1n/0n/1p/0p en_emph set sel
	ddr_phy->DDRPHY_DQS_OCD_SEL_2_SETx[0] = 0xffffffff;  //dqs3n/2n/3p/2p/1n/0n/1p/0p nt set sel
	ddr_phy->DDRPHY_DQS_OCD_SEL_2_SETx[1] = 0xffffffff;  //dqs3n/2n/3p/2p/1n/0n/1p/0p en_emph set sel
}

void DDR_PHY_DQ_DQS_SEL_EMPH(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	ddr_phy->DDRPHY_RW_SEL_EMPH_CTL_SETx[0] = 0x00110011;
	ddr_phy->DDRPHY_RW_SEL_EMPH_CTL_SETx[1] = 0x00110011;
	ddr_phy->DDRPHY_RW_EMPH_CTL_SETx[0] = 0x00000000;
	ddr_phy->DDRPHY_RW_EMPH_CTL_SETx[1] = 0x00000000;

	ddr_phy->DDRPHY_DT_SET0 = 0x00045500;
	ddr_phy->DDRPHY_CK_EMPH_CTL = 0x00001000;
}

void DDR_PHY_DELAY_TAP_SET(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	/*CA delay tap*/
	ddr_phy->DDRPHY_ADR_DLY_0 = 0x88888888; /*A0~A7*/
	ddr_phy->DDRPHY_ADR_DLY_1 = 0x88888888; /*A8~A15*/
	ddr_phy->DDRPHY_ADR_DLY_2 = 0x88888888; /*Ext Addr/BANK*/
	ddr_phy->DDRPHY_CMD_DLY_0 = 0x20888888; /*RAS/CAS/WE*/
	ddr_phy->DDRPHY_CMD_DLY_1 = 0x88888888;

	/*geardown*/
	if (DDR_PHY_ChipInfo_ddrtype() == DDR_Type_DDR2) {
#if SUPPORT_DYNAMIC_POWEROFF
		if (ddr_phy->DDRPHY_AFIFO_STR_SEL != 0x0) {
			ddr_phy->DDRPHY_CMD_ADR_PH = 0x7c032323;
		}
#else
		ddr_phy->DDRPHY_CMD_ADR_PH = 0x7C033333;
#endif
	} else {
		ddr_phy->DDRPHY_CMD_ADR_PH = 0x7C011111;
	}
	/*write delay tap*/
	ddr_phy->DDRPHY_DQ_DLY_0_SETx[0] = 0x88888888;
	ddr_phy->DDRPHY_DQ_DLY_0_SETx[1] = 0x88888888;
	ddr_phy->DDRPHY_DQ_DLY_1_SETx[0] = 0x00000888;
	ddr_phy->DDRPHY_DQ_DLY_1_SETx[1] = 0x00000888;
}

void DDR_PHY_READ_CTRL(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	u32 para_idx = DDR_PHY_ChipInfo();

	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] = ddrphy_zq_rx_scan[para_idx].READ_CTRL_0_SET0; //byte0:dqs_en_dly[13:8],dqs_en[6:0]
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = ddrphy_zq_rx_scan[para_idx].READ_CTRL_0_SET1; //byte1:dqs_en_dly[13:8],dqs_en[6:0]
	ddr_phy->DDRPHY_READ_CTRL_1 = ddrphy_zq_rx_scan[para_idx].READ_CTRL_1; //tm_rd_fifo[5:0]

	ddr_phy->DDRPHY_READ_CTRL_2_SETx[0] = 0x00000008;	//byte0:force_odt[23:22],odt_en_odd[3],odt_en_sel[2:1],odt_en_dly[0]
	ddr_phy->DDRPHY_READ_CTRL_2_SETx[1] = 0x00000008;	//byte1:force_odt[23:22],odt_en_odd[3],odt_en_sel[2:1],odt_en_dly[0]
	ddr_phy->DDRPHY_READ_CTRL_6_SETx[0] = 0x000000FE;  //byte0:odt_en
	ddr_phy->DDRPHY_READ_CTRL_6_SETx[1] = 0x000000FE;  //byte1:odt_en
}

void DDR_PHY_3_POINT_CAL_RANGE(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_CAL_SHIFT_CTRL = 0x11117777;	//Central point shift right[30:16],Central point shift left[15:0]
#if DDRPHY_LOOPBACK
	ddr_phy->DDRPHY_DEL_POS_SEL_SETx[0] = 0x00000000;
	ddr_phy->DDRPHY_DEL_POS_SEL_SETx[1] = 0x00000000;
	ddr_phy->DDRPHY_DEL_POS_SEL_DBI_SETx[0] = 0x00000000;
	ddr_phy->DDRPHY_DEL_POS_SEL_DBI_SETx[1] = 0x00000000;
	ddr_phy->DDRPHY_DEL_NEG_SEL_SETx[0] = 0x00000000;
	ddr_phy->DDRPHY_DEL_NEG_SEL_SETx[1] = 0x00000000;
	ddr_phy->DDRPHY_DEL_NEG_SEL_DBI_SETx[0] = 0x00000000;
	ddr_phy->DDRPHY_DEL_NEG_SEL_DBI_SETx[1] = 0x00000000;
#else
	ddr_phy->DDRPHY_DEL_POS_SEL_SETx[0] = 0x22222222;	//byte0:dqs0(7:0) rd_cal_right_range
	ddr_phy->DDRPHY_DEL_POS_SEL_SETx[1] = 0x22222222;	//byte1:dqs1(7:0) rd_cal_right_range
	ddr_phy->DDRPHY_DEL_NEG_SEL_SETx[0] = 0x22222222;	//byte0:dqs0(7:0) rd_cal_left_range
	ddr_phy->DDRPHY_DEL_NEG_SEL_SETx[1] = 0x22222222;	//byte1:dqs1(7:0) rd_cal_left_range
#endif
}

void DDR_PHY_WRITE_LEVELING_CAL(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;
	u32 temp;

	/*write leveling enable*/
	/*enable DQ input mode*/
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[0] |= (DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG);  // for slite0
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[1] |= (DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG);  // for slite1
	/*enable DQS output*/
	ddr_phy->DDRPHY_DQ_DQS_CTRL  |= (DDRPHY_BIT_DQS_OE_FRC_EN | DDRPHY_BIT_DQ_LATCH_EN | DDRPHY_BIT_DQS_OE_SIG);

	for (temp = 0; temp < 32; temp++) {
		/*Adjust DQS PI value*/
		ddr_phy->DDRPHY_PLL_PI0 = (ddr_phy->DDRPHY_PLL_PI0 & (~(DDRPHY_MASK_DPI_POST_PI_SEL3 | DDRPHY_MASK_DPI_POST_PI_SEL2))) | \
								  DDRPHY_DPI_POST_PI_SEL3(temp)  | DDRPHY_DPI_POST_PI_SEL2(temp);
		/*Trigger one pulse*/
		ddr_phy->DDRPHY_DQ_DQS_CTRL |= DDRPHY_BIT_TRIG_DQS;

		/*check DQ value*/
		if (ddr_phy->DDRPHY_DQ_PAT_IN0) {
			DiagPrintf("WRITE_LEVELING_CAL dqs_pi = %d DQ_PAT_IN0 = %x\n", temp, ddr_phy->DDRPHY_DQ_PAT_IN0);
			break;
		}
	}

	/*write leveling disable*/
	/*disable DQ input mode*/
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[0] &= (~(DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG));  // for slite0
	ddr_phy->DDRPHY_DQ_IO_CTL_0_SETx[1] &= (~(DDRPHY_MASK_DQ_IE_FRC_EN | DDRPHY_MASK_DQ_IE_SIG));  // for slite1
	/*disable DQS output*/
	ddr_phy->DDRPHY_DQ_DQS_CTRL  &= (~(DDRPHY_BIT_DQS_OE_FRC_EN | DDRPHY_BIT_DQ_LATCH_EN));
}

void DDR_PHY_READ_LEVELING(VOID)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	if (DDR_PHY_ChipInfo()) {
		ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = 0x12121212;
		ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = 0x12121212;
		ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = 0x11111111;
		ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = 0x11111111;
		ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = 0x11111111;
		ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = 0x11111111;
		ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = 0x12121212;
		ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = 0x12121212;
		ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = 0x0000000f;
		ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = 0x0000000f;
		ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = 0x0000000f;
		ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = 0x0000000f;
	} else {
		ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = 0x10101010;	//byte0:dqs0p(3:0) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[0] = 0x10101010;	//byte0:dqs0p(7:4) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = 0x10101010;	//byte0:dqs0n(3:0) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[0] = 0x10101010;	//byte0:dqs0n(7:4) dly

		ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[1] = 0x10101010;	//byte1:dqs1p(3:0) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_1_SETx[1] = 0x10101010;	//byte1:dqs1p(7:4) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[1] = 0x10101010;	//byte1:dqs1n(3:0) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_3_SETx[1] = 0x10101010;	//byte1:dqs1n(7:4) dly

		ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[0] = 0x00000010;	//byte0:dqs0 dbi(p) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_1_DBI_SETx[1] = 0x00000010;	//byte1:dqs1 dbi(p) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[0] = 0x00000010;	//byte0:dqs0 dbi(n) dly
		ddr_phy->DDRPHY_DQS_IN_DLY_3_DBI_SETx[1] = 0x00000010;	//byte1:dqs1 dbi(n) dly
	}
}

#if DDRPHY_LOOPBACK
void DDR_PHY_LoopBack(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	u32 temp;

	ddr_phy->DDRPHY_AFIFO_STR_1 = (ddr_phy->DDRPHY_AFIFO_STR_1 & 0xFFFF) | (0x8 << 16); //0x41011084[23:16] = 8'h8, force_rst_n_tx = 0
	ddr_phy->DDRPHY_PLL_CTL5 &= ~(DDRPHY_MASK_DPI_CA_PWROFF_MODE | DDRPHY_MASK_DPI_RW_PWROFF_MODE); //0x410110ac[5:2] = 4'h0, disable clock gating

	ddr_phy->DDRPHY_DPI_CTRL_0 &= ~DDRPHY_BIT_DATA_PRE; //DPI_CTRL_0[21] = 0, data_pre = 0
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_RST_FIFO_MODE) | DDRPHY_RST_FIFO_MODE(
									 2); //DPI_CTRL_0[3:2] = 2'b10, rst_fifo_mode = disable
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & ~DDRPHY_MASK_FW_SET_MODE) | DDRPHY_FW_SET_MODE(
									 2); //DPI_CTRL_0[1:0] = 2'b10, fw_set_mode = immidiately

	//0x4101_1440[4:0] = 5'h1, rd_dly_pos_sel = 1
	ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_0_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_POS_SEL_0) | DDRPHY_FW_RD_DLY_POS_SEL_0(1);
	//0x4101_1458[4:0] = 5'h1, rd_dly_neg_sel = 1
	ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] = (ddr_phy->DDRPHY_DQS_IN_DLY_2_SETx[0] & ~DDRPHY_MASK_FW_RD_DLY_NEG_SEL_0) | DDRPHY_FW_RD_DLY_NEG_SEL_0(1);

	ddr_phy->DDRPHY_READ_CTRL_1 |= DDRPHY_BIT_RD_DLY_FOLLOW_DQ0; //0x4101_1170[10] = 1'b1, rd_dly_follow_dq0 = 1

	ddr_phy->DDRPHY_READ_CTRL_0_SETx[0] &= ~DDRPHY_MASK_TM_DQS_EN; //0x4101_15F8[6:2] = 5'h0, tm_dqs_en= 0
	ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] = (ddr_phy->DDRPHY_READ_CTRL_0_SETx[1] & ~DDRPHY_MASK_TM_DQS_EN_FTUN) | DDRPHY_TM_DQS_EN_FTUN(
			1); //0x4101_15FC[1:0] = 2'b01, tm_dqs_en_ftun= 1

	ddr_phy->DDRPHY_READ_CTRL_1 = (ddr_phy->DDRPHY_READ_CTRL_1 & ~DDRPHY_MASK_TM_RD_FIFO) | DDRPHY_TM_RD_FIFO(3); //0x4101_1170[5:0] = 6'h3, tm_rd_fifo = 3
	ddr_phy->DDRPHY_DPI_CTRL_1 |= DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1; //0x4101_123c[3:2] = 2'b11, fw_set_rd_dly = 1

	ddr_phy->DDRPHY_TEST_CTRL0 = (ddr_phy->DDRPHY_TEST_CTRL0 & ~DDRPHY_MASK_LBK_MODE) | DDRPHY_LBK_MODE(1); //0x4101_1164[31:30] = 2'b01, lbk_mode = bypass pad

	ddr_phy->DDRPHY_BIST_2TO1_0 &= ~DDRPHY_MASK_BIST_MODE; //0x4101_1244[22:20] = 3'b0, bist_mode = PRBS11
	ddr_phy->DDRPHY_BIST_PT = DDRPHY_BIST_PATTERN(0x87654321); //0x4101_124c[31:0] = 32'h8765_4321
	ddr_phy->DDRPHY_BIST_2TO1_0 |= DDRPHY_BIT_BIST_EN; //0x4101_1244[23] = 1'b1	WR, bist_en = 1

	DelayMs(25);
	if ((ddr_phy->DDRPHY_BIST_2TO1_1 & DDRPHY_BIT_BIST_RDY) == ddr_phy->DDRPHY_BIST_2TO1_1) {
		RTK_LOGI(TAG, "BIST_2TO1_1 = BIT[5], pass\n");
	}
	RTK_LOGI(TAG, "BIST_2TO1_1 is 0x%x\n", ddr_phy->DDRPHY_BIST_2TO1_1);
}
#endif

/**
  * @brief Initializes the PSRAM controller according to the specified
  *   parameters in the PSRAM_InitStruct
  * @param  PSRAM_InitStruct: pointer to a PSRAM_InitTypeDef structure that
  *   contains the configuration information for the specified PSRAM controller
  * @retval None
  */
void DDR_PHY_Init(DDRPHY_InitTypeDef *DDRPHY_InitStruct)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	DDR_PHY_DLL_CLK_DIV(DDRPHY_InitStruct->ddr_freq);

	DDR_PHY_CRT_Init();

#if !DDRPHY_LOOPBACK
#if DDR_SCAN_PARA
	/*pad control*/
	DDR_PHY_R480_CAL();
	DDR_PHY_ZQ_CAL();
#else
	DDR_PHY_R240_ZQ_CAL();
#endif
#endif

	DDR_PHY_ZQ_SET_SEL();

	DDR_PHY_DQ_DQS_SEL_EMPH();

	DDR_PHY_DELAY_TAP_SET();

	DDR_PHY_READ_CTRL();

	DDR_PHY_3_POINT_CAL_RANGE();
	DDR_PHY_READ_LEVELING();
	//DDR_PHY_WRITE_LEVELING_CAL();

	/*=====soc vref====*/
	ddr_phy->DDRPHY_PAD_BUS_1 = ((ddr_phy->DDRPHY_PAD_BUS_1 & (~DDRPHY_MASK_VREF_S_4)) | \
								 DDRPHY_VREF_S_4(0x32) | DDRPHY_BIT_VREF_RANGE_4);	//CA Vref Vref:50%
	ddr_phy->DDRPHY_PAD_BUS_2 = 0xd8d8d8d8;	//byte3/byte2/byte1/byte0 Vref:44.5%
	ddr_phy->DDRPHY_RX_CAL_CTRL_0 |= (DDRPHY_BIT_DELTA_FORCE | DDRPHY_BIT_VR_UPD_3 | DDRPHY_BIT_VR_UPD_2 | \
									  DDRPHY_BIT_VR_UPD_1 | DDRPHY_BIT_VR_UPD_0);	//byte3/byte2/byte1/byte0_Vref_cal_off
	//rtd_maskl(0x180be2d0,0xfffff0ff,0x00000000);
	ddr_phy->DDRPHY_PAD_BUS_2 = 0x8C8C8C8C;	//byte3/byte2/byte1/byte0 VREF_DQ:50%
	//VREF_S is Not exist in AmebaD2
	//ddr_phy->DDRPHY_PAD_BUS_0 = (ddr_phy->DDRPHY_PAD_BUS_0 & (~DDRPHY_MASK_VREF_S)) | DDRPHY_VREF_S(6) | DDRPHY_BIT_VREF_RANGE; //VREF_CA:50%

	/*=====Delay Update Mode=====*/
	ddr_phy->DDRPHY_DPI_CTRL_1 |= (DDRPHY_BIT_FW_SET_WR_DLY | DDRPHY_BIT_WRITE_EN_0);  //Update Write Delay Tap
	ddr_phy->DDRPHY_DPI_CTRL_1 |= (DDRPHY_BIT_FW_SET_RD_DLY | DDRPHY_BIT_WRITE_EN_1);  //Update Read Delay Tap
	ddr_phy->DDRPHY_DPI_CTRL_0 &= (~DDRPHY_MASK_FW_SET_MODE);	//fw_set[1:0]=no read/refresh

	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & (~DDRPHY_MASK_RST_FIFO_MODE)) |
	// DDRPHY_RST_FIFO_MODE(0x3); //rst_fifo[1:0]=0x3 force reset FIFO pointer
	//ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & (~DDRPHY_MASK_RST_FIFO_MODE)) |
	// DDRPHY_RST_FIFO_MODE(0x2);  //rst_fifo[1:0]=0x2 disable reset FIFO pointer
	ddr_phy->DDRPHY_DPI_CTRL_0 = (ddr_phy->DDRPHY_DPI_CTRL_0 & (~DDRPHY_MASK_REF_UPDATE_DLY)) |
								 DDRPHY_REF_UPDATE_DLY(0x10);  //adapt refresh delay

#if SUPPORT_DYNAMIC_POWEROFF
	if (DDRPHY_GET_CMD_EX_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1) >= DDRPHY_GET_CMD_RD_STR_NUM(ddr_phy->DDRPHY_AFIFO_STR_1)) {
		/* ddrphy auto clock gating for power saving */
		u32 temp = ddr_phy->DDRPHY_PLL_CTL5;
		temp &= ~(DDRPHY_MASK_DPI_CA_PWROFF_MODE | DDRPHY_MASK_DPI_RW_PWROFF_MODE);

		/* reg_rw_pwroff_mode should be 0x1 because of small CL */
		temp |= DDRPHY_DPI_RW_PWROFF_MODE(0x1) | DDRPHY_DPI_CA_PWROFF_MODE(0x2);
		ddr_phy->DDRPHY_PLL_CTL5 = temp;
	}
#endif

#if DDRPHY_LOOPBACK
	DDR_PHY_LoopBack();
#endif
}

void DDR_PHY_CAL(void)
{
#if (DDR_SCAN_PARA && DDR_AUTOGATING)
	assert_param(0);//DDR_AUTOGATING shall be 0 when Scan parameter
#endif
	DDR_PHY_Scan_param();
}

void DDR_PHY_AutoGating(void)
{
	u32 Rtemp;
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	//CTL1
	ddr_phy->DDRPHY_PLL_CTL1 &= (~DDRPHY_BIT_DPI_REG_CRT_PWROFF_DIS);

	//CTL5
	Rtemp = ddr_phy->DDRPHY_PLL_CTL5;
	Rtemp &= ~(0x3c);
	Rtemp |= DDRPHY_DPI_RW_PWROFF_MODE(0x1) | DDRPHY_DPI_CA_PWROFF_MODE(0x1);
	ddr_phy->DDRPHY_PLL_CTL5 = Rtemp;
	DelayUs(50);

	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
	Rtemp |= HSYS_BIT_PWDPAD_DQ_EN | HSYS_BIT_PI_PWROFF_EN ; //don't write 1 if user mode  HSYS_BIT_PWDPAD_DQ_EN HSYS_BIT_PI_PWROFF_EN
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, Rtemp);
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/
