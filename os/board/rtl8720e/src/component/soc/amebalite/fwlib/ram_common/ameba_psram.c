/**
  ******************************************************************************
  * @file    rtl8721dhp_psram.c
  * @author
  * @version V1.0.0
  * @date    2017-09-17
  * @brief   This file contains all the functions prototypes for the psram firmware
  *             library, including the following functionalities of the PSRAM CTRL:
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

u8 APM_WR_INIT_LATENCY_SPEC[6] = {
	APM_WR_INIT_LATENCY_3CLK,
	APM_WR_INIT_LATENCY_4CLK,
	APM_WR_INIT_LATENCY_5CLK,
	APM_WR_INIT_LATENCY_6CLK,
	APM_WR_INIT_LATENCY_7CLK,
	APM_WR_INIT_LATENCY_8CLK,
};

u8 WB_WR_INIT_LATENCY_SPEC[8] = {
	WB_WR_INIT_LATENCY_3CLK,
	WB_WR_INIT_LATENCY_4CLK,
	WB_WR_INIT_LATENCY_5CLK,
	WB_WR_INIT_LATENCY_6CLK,
	WB_WR_INIT_LATENCY_7CLK,
	(int)NULL,
	(int)NULL,
	WB_WR_INIT_LATENCY_10CLK
};

static u32 PSRAM_CALIB_PATTERN[6] = {
	0x11223344,
	0xA55AA55A,
	0x5AA55AA5,
	0x44332211,
	0x96696996,
	0x69969669,
};

const PSRAM_Latency Psram_latency[] = {
	//Clk limit	  latency	   psram type
	{104,			4,			(int)NULL},
	{133,			5,			(int)NULL},
	{166,			6,			(int)NULL},
	{200,			7,			(int)NULL},
	{250,			7,			PSRAM_VENDOR_WB | PSRAM_SIZE_256Mb},
	{250,			8, 			PSRAM_VENDOR_APM},
	{250,			10,			PSRAM_VENDOR_WB | PSRAM_SIZE_128Mb},
	{0xFF,			0xFF,		0xFFFF},
};

PSRAMINFO_TypeDef PsramInfo;

u16 psram_type = 0;

/**
  * @brief get psram info
  * @param  none
  * @retval PSRAM Type
  */
void PSRAM_INFO_Update(void)
{
	u32 chipinfo = ChipInfo_PSRAMType();
	u32 PsramClk = 0;
	u32 PLLCLk, Div;
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
	u32 ClkSel = LSYS_GET_CKSL_PSRAM(Temp);
	u32 LBusSrc = LSYS_GET_CKSL_LBUS(Temp);
	RRAM_TypeDef *rram = RRAM_DEV;

	PsramInfo.Psram_Vendor = PSRAM_VENDOR_GET(chipinfo);

	// return if mcm flash
	if (PsramInfo.Psram_Vendor == PSRAM_VENDOR_NONE) {
		rram->PSRAM_TYPE = PSRAM_TYPE_NONE;
		return;
	}

	if (ClkSel == BIT_LSYS_CKSL_PSRAM_LBUS) {
		if (LBusSrc) {
			//current lbus clk is xtal
			PsramClk = XTAL_ClkGet();
		} else {
			//current lbus clk is osc4m
			PsramClk = 4000000;
		}
	} else {
		Div = LSYS_GET_CKD_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP0));
		Div += 1;

		if (ClkSel == BIT_LSYS_CKSL_PSRAM_CPUPLL) {
			PLLCLk = PLL_ClkGet(CLK_CPU_MPLL);
			PsramClk = PLLCLk / Div;
		} else if (ClkSel == BIT_LSYS_CKSL_PSRAM_DSPPLL) {
			PLLCLk = PLL_ClkGet(CLK_CPU_DPLL);
			PsramClk = PLLCLk / Div;
		}
	}

	PsramInfo.Psram_Clk_Set = PsramClk / 2;
	PsramInfo.Psram_Clk_Limit = PSRAM_CLK_LIMIT_GET(chipinfo);
	PsramInfo.Psram_Size = PSRAM_SIZE_GET(chipinfo);
	PsramInfo.Psram_Page_size = PSRAM_PAGE_SIZE_GET(chipinfo);
	PsramInfo.PSRAMC_Clk_Unit = 1000000000 / PsramClk;
	PsramInfo.Psram_Resume_Cnt = Psram_RESUME_TIME / (PsramInfo.PSRAMC_Clk_Unit);

	rram->PSRAM_RESUMECNT_BOOT = PsramInfo.Psram_Resume_Cnt;		//for nonsecure world

	if (PsramInfo.Psram_Vendor == PSRAM_VENDOR_WB) {
		rram->PSRAM_TYPE = PSRAM_TYPE_WB;
		if (PsramInfo.Psram_Clk_Set <= 104000000) {
			PsramInfo.Psram_Latency_Set = 4;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI133;
		} else if (PsramInfo.Psram_Clk_Set <= 133000000) {
			PsramInfo.Psram_Latency_Set = 5;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI133;
		} else if (PsramInfo.Psram_Clk_Set <= 166000000) {
			PsramInfo.Psram_Latency_Set = 6;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		} else if (PsramInfo.Psram_Clk_Set <= 200000000) {
			PsramInfo.Psram_Latency_Set = 7;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		} else if ((PsramInfo.Psram_Clk_Set <= 250000000) && (PsramInfo.Psram_Size == PSRAM_SIZE_256Mb)) {
			PsramInfo.Psram_Latency_Set = 7;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		} else if ((PsramInfo.Psram_Clk_Set <= 250000000) && (PsramInfo.Psram_Size == PSRAM_SIZE_128Mb)) {
			PsramInfo.Psram_Latency_Set = 10;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		}

	} else {
		rram->PSRAM_TYPE = PSRAM_TYPE_APM;
		if (PsramInfo.Psram_Clk_Set <= 104000000) {
			PsramInfo.Psram_Latency_Set = 4;
			PsramInfo.Psram_CSHI = Psram_APM_CSHI133;
		} else if (PsramInfo.Psram_Clk_Set <= 133000000) {
			PsramInfo.Psram_Latency_Set = 5;
			PsramInfo.Psram_CSHI = Psram_APM_CSHI133;
		} else if (PsramInfo.Psram_Clk_Set <= 166000000) {
			PsramInfo.Psram_Latency_Set = 6;
			PsramInfo.Psram_CSHI = Psram_APM_CSHI166;
		} else if (PsramInfo.Psram_Clk_Set <= 200000000) {
			PsramInfo.Psram_Latency_Set = 7;
			PsramInfo.Psram_CSHI = Psram_APM_CSHI200;
		} else if (PsramInfo.Psram_Clk_Set <= 250000000) {
			PsramInfo.Psram_Latency_Set = 8;
			PsramInfo.Psram_CSHI = Psram_APM_CSHI250;
		}
	}

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "PSRAM Ctrl CLK: %d Hz \n", PsramClk);
}
/**
  * @brief  Fills each PSPHY_InitStruct member with its default value.
  * @param  PSPHY_InitStruct: pointer to an PSPHY_InitStruct structure which will be initialized.
  * @retval None
  */
void PSRAM_PHY_StructInit(PSPHY_InitTypeDef *PSPHY_InitStruct)
{
	// 0x04
	PSPHY_InitStruct->PSRAMP_PRE_CAL_PHASE = 0x2; /* only for rtl now, default choose 1 on asic */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_JMIN = Psram_phy_Jmin;
	PSPHY_InitStruct->PSRAMP_CFG_CAL_JMAX = 0x3;
	PSPHY_InitStruct->PSRAMP_CFG_CAL_J = 0x3;
	PSPHY_InitStruct->PSRAMP_CFG_CAL_N = 0xA;

	//0x18
	PSPHY_InitStruct->PSRAMP_SW_RST_N = 0x1;
	PSPHY_InitStruct->PSRAMP_LATENCY_TYPE = PSRSAM_FIX_LATENCY;	//0:variable latency 1:fix leatency
	PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE = 0x1;
	if (PSPHY_InitStruct->PSRAMP_LATENCY_TYPE) {
		PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY = PSPHY_RFIFO_READY_DELAY_FIX;
	} else {
		PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY = PSPHY_RFIFO_READY_DELAY_VL;

	}

	PSPHY_InitStruct-> DDR_PAD_CTRL1 = PAD_DDR_PTB(PSRAM_PHY_PAD_DRIVING) | PAD_DDR_PT(PSRAM_PHY_PAD_DRIVING) | PAD_DDR_NT(PSRAM_PHY_PAD_DRIVING) | \
									   PAD_DDR_NTB(PSRAM_PHY_PAD_DRIVING) | PAD_BIT_DDR_PWDPADN_2REGU | PAD_BIT_DDR_VREF_RANGE;
	PSPHY_InitStruct-> DDR_PAD_CTRL2 = PAD_DDR_S(PSRAM_PHY_VOL_TUNE);
}

/**
  * @brief Initializes the PSRAM phy according to the specified
  *   parameters in the PSPHY_InitStruct
  * @param  PSPHY_InitStruct: pointer to a PSPHY_InitStruct structure that
  *   contains the configuration information for the specified PSRAM phy
  * @retval None
  */
void PSRAM_PHY_Init(PSPHY_InitTypeDef *PSPHY_InitStruct)
{

	PSPHY_TypeDef *psram_phy = PSRAMPHY_DEV;

	//enable phy pad and set ddr phy pad driving strength
	HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, PSPHY_InitStruct->DDR_PAD_CTRL1);

	//set REF voltage fine-tune
	HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL2, PSPHY_InitStruct->DDR_PAD_CTRL2);

	//0x04
	psram_phy->PSPHY_CAL_PAR = PSPHY_PRE_CAL_PHASE(PSPHY_InitStruct->PSRAMP_PRE_CAL_PHASE) | \
							   PSPHY_CFG_CAL_JMIN(PSPHY_InitStruct->PSRAMP_CFG_CAL_JMIN) | \
							   PSPHY_CFG_CAL_JMAX(PSPHY_InitStruct->PSRAMP_CFG_CAL_JMAX) | \
							   PSPHY_CFG_CAL_J(PSPHY_InitStruct->PSRAMP_CFG_CAL_J) | \
							   PSPHY_CFG_CAL_N(PSPHY_InitStruct->PSRAMP_CFG_CAL_N);
	//0x18
	psram_phy->PSPHY_PHY_CTRL = PSPHY_PSRAM_SW_RST_N(PSPHY_InitStruct->PSRAMP_SW_RST_N) | \
								PSPHY_LATENCY_TYPE(PSPHY_InitStruct->PSRAMP_LATENCY_TYPE) | \
								PSPHY_CAL_RWDS_PHASE(PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE) | \
								PSPHY_RFIFO_RDY_DLY(PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY);

}

/* boot_finish time = (2^16)/40 = 1.6ms when no speedup, when speed up 256 cycles will be used */
/* 16 is defined in SPIC config form, 40 is SPIC IP clock */
/* after SPIC clock & function enable, auto mode read */
/* will be stucked when boot_finish not ready, */
/* but user mode will hang if boot_finish not ready */
VOID PSRAM_CTRL_SPU(u32 state)
{
	u32 temp = 0;
	/* Should decide to speedup or not before spic clk&func enable */
	/* this register will write speed_up signal in spic to change BOOT_DELAY (speed_up = 1: boot_delay = 8; speed_up = 0: boot_delay = by config form) */
	temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL);

	if (state == DISABLE) {
		temp &= ~LSYS_BIT_PSRAM_SPDUPSIM;
	} else {
		temp |= LSYS_BIT_PSRAM_SPDUPSIM;
	}
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_PLAT_CTRL, temp);
}

/**
  * @brief Initializes the PSRAM controller according to the specified
  *   parameters in the PSRAM_InitStruct
  * @param  PSRAM_InitStruct: pointer to a PSRAM_InitTypeDef structure that
  *   contains the configuration information for the specified PSRAM controller
  * @retval None
  */
void PSRAM_CTRL_Init(void)
{
	SPIC_TypeDef *psram_ctrl = PSRAMC_DEV;

	/* Disable psramc */
	psram_ctrl->SSIENR = 0;

	//0x00
	psram_ctrl->CTRLR0 = MASK_DDR_EN | MASK_ADDR_CH | CK_MTIMES(0x1) | MASK_DATA_CH | MASK_CMD_CH;

	//0x14
	psram_ctrl->BAUDR = SCKDV(Psram_ClkDiv2);

	//0x120
	psram_ctrl->VALID_CMD = BIT_SEQ_WR_EN | BIT_SEQ_RD_EN | BIT_FRD_SINGLE | BIT_CTRLR0_CH;

	//psram_ctrl->CTRLR2 &= ~BIT_FULL_WR;  //bit 13 = 0 need to disable,default = 0

	if (PsramInfo.Psram_Vendor == PSRAM_VENDOR_WB) {
		//0x110
		psram_ctrl->CTRLR2 = BIT_SO_DNUM | TX_FIFO_ENTRY(0x5) | RX_FIFO_ENTRY(0x5) | BIT_DM_ACT | BIT_DIS_DM_CA | \
							 WR_VL_EN(!PSRSAM_FIX_LATENCY) | RD_VL_EN(!PSRSAM_FIX_LATENCY) | RD_WEIGHT(0x2);
		//0x134 waiting update typedefine
		psram_ctrl->DEVICE_INFO = BIT_DATA_UNIT_2B | BIT_JEDEC_P2CMF | BIT_PSRAM | ATOM_SIZE(0x1) | BIT_RD_PAGE_EN | \
								  BIT_WR_PAGE_EN | PAGE_SIZE(PsramInfo.Psram_Page_size);
		psram_ctrl->TPR0 = (CS_TCEM(Psram_Tcem_T25 / PsramInfo.PSRAMC_Clk_Unit / 32)) | \
						   (CS_SEQ_TIMEOUT(Psram_Seq_timeout)) | \
						   (CS_ACTIVE_HOLD(Psram_WB_CSH)) | \
						   (CS_H_WR_DUM_LEN(PsramInfo.Psram_CSHI / PsramInfo.PSRAMC_Clk_Unit)) | \
						   (CS_H_RD_DUM_LEN(PsramInfo.Psram_CSHI / PsramInfo.PSRAMC_Clk_Unit));
		//0xe0
		psram_ctrl->READ_FAST_SINGLE = FRD_CMD(WB_RD_CMD);
		//0xf4
		psram_ctrl->WRITE_SIGNLE = WR_CMD(WB_WR_CMD);
		//0x11c
		psram_ctrl->AUTO_LENGTH = AUTO_ADDR_LENGTH(0x5);
		//0x118
		psram_ctrl->USER_LENGTH = USER_ADDR_LENGTH(0x5) | USER_CMD_LENGHT(0x1);
	} else {
		//0x110
		psram_ctrl->CTRLR2 = BIT_SO_DNUM | TX_FIFO_ENTRY(0x5) | RX_FIFO_ENTRY(0x5) | BIT_DM_ACT | \
							 WR_VL_EN(!PSRSAM_FIX_LATENCY) | RD_VL_EN(!PSRSAM_FIX_LATENCY) | RD_WEIGHT(0x2);
		//0x134 waiting update typedefine
		psram_ctrl->DEVICE_INFO = BIT_PSRAM | ATOM_SIZE(0x1) | BIT_RD_PAGE_EN | \
								  BIT_WR_PAGE_EN | PAGE_SIZE(PsramInfo.Psram_Page_size);
		//0x138 waiting update typedefine
		//CS_TCEM = 0 for seq performance
		psram_ctrl->TPR0 = (CS_TCEM(Psram_Tcem_T85 / PsramInfo.PSRAMC_Clk_Unit / 32)) | \
						   (CS_SEQ_TIMEOUT(Psram_Seq_timeout)) | \
						   (CS_ACTIVE_HOLD(Psram_APM_CSH)) | \
						   (CS_H_WR_DUM_LEN(PsramInfo.Psram_CSHI / PsramInfo.PSRAMC_Clk_Unit)) | \
						   (CS_H_RD_DUM_LEN(PsramInfo.Psram_CSHI / PsramInfo.PSRAMC_Clk_Unit));
		psram_ctrl->READ_FAST_SINGLE = FRD_CMD(APM_RD_CMD);
		psram_ctrl->WRITE_SIGNLE = WR_CMD(APM_WR_CMD);
		//0x11c
		psram_ctrl->AUTO_LENGTH = AUTO_ADDR_LENGTH(0x4);
		//0x118
		psram_ctrl->USER_LENGTH = USER_ADDR_LENGTH(0x4) | USER_CMD_LENGHT(0x2);
	}

	//auto rd latency
	if (PSRSAM_FIX_LATENCY) {
		psram_ctrl->AUTO_LENGTH |= RD_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 4) | \
								   IN_PHYSICAL_CYC(PSPHY_RFIFO_READY_DELAY_FIX + 4);
		if (PsramInfo.Psram_Vendor == PSRAM_VENDOR_WB) {
			psram_ctrl->AUTO_LENGTH2 = WR_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 2);
		} else {
			psram_ctrl->AUTO_LENGTH2 = WR_DUMMY_LENGTH(2 * PsramInfo.Psram_Latency_Set - 2);

		}
		psram_ctrl->USER_LENGTH |= USER_RD_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 4);
	} else {
		psram_ctrl->AUTO_LENGTH |= RD_DUMMY_LENGTH(2 * PsramInfo.Psram_Latency_Set - 4);
		psram_ctrl->AUTO_LENGTH2 = WR_DUMMY_LENGTH(2 * PsramInfo.Psram_Latency_Set - 2);
		psram_ctrl->USER_LENGTH |= USER_RD_DUMMY_LENGTH(2 * PsramInfo.Psram_Latency_Set - 4);

	}

	while ((psram_ctrl->SR & BIT_BOOT_FIN) == 0);
}

void PSRAM_APM_DEVIC_Init(void)
{
	u8 mr0[2];
	u8 mr4[2];
	mr0[0] = PSRAM_READ_LATENCY_CODE(PsramInfo.Psram_Latency_Set - 3) | \
			 PSRAM_LT_SELECT(PSRSAM_FIX_LATENCY); //0:variable latency 1:fix latency

	mr4[0] = PSRAM_WRITE_LATENCY_CODE(APM_WR_INIT_LATENCY_SPEC[PsramInfo.Psram_Latency_Set - 3]);

	mr0[1] = mr0[0];
	mr4[1] = mr4[0];

	PSRAM_REG_Write(0, 0x0, 2, mr0);
	PSRAM_REG_Write(0, 0x4, 2, mr4);
}

void PSRAM_WB_DEVIC_Init(void)
{
	u8 data[2];
	data[0] = PSRAM_WB_RESERVER | \
			  PSRAM_WB_DRIVE_STRENGTH(0x0) | \
			  PSRAM_WB_DPD_EN(1);	 // keep default driving stength

	data[1] = PSRAM_WB_BURST_LENGTH(0x0) | \
			  PSRAM_WB_HyBURSE_EN | \
			  PSRAM_WB_FIX_LATENCY_EN(PSRSAM_FIX_LATENCY) | \
			  PSRAM_WB_INIT_LATENCY(WB_WR_INIT_LATENCY_SPEC[PsramInfo.Psram_Latency_Set - 3]);		// 1 for 6T 150M, 2 for 7T 200M

	PSRAM_REG_Write(1, PSRAM_WB_CR0, 2, data);
}


/**
  * @brief Get APM/Winbond PSRAM register value
  * @param  type: APM :0 Winbond:1
  * @param  addr: APM/Winbond PSRAM register addr
  * @param  read_len: read data length
  * @param  read_data: read data output buf
  * @retval None
  */
NON_DRAM_TEXT_SECTION
void PSRAM_REG_Read(u32 type, u32 addr, u32 read_len, u8 *read_data, u32 CR)
{
	SPIC_TypeDef *psram_ctrl = PSRAMC_DEV;
	u32 rx_num = 0;

	/* Wait spic busy done before switch to user mode */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Enter User Mode */
	psram_ctrl->CTRLR0 |= BIT_USER_MODE;

	/* Disable SSIENR to program control registers*/
	psram_ctrl->SSIENR = 0;

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	psram_ctrl->RX_NDF = RX_NDF(read_len);
	psram_ctrl->TX_NDF = 0;
	//DBG_8195A("RX_NDF = %x %x\n", &(psram_ctrl->RX_NDF), psram_ctrl->RX_NDF);

	//set rx mode
	psram_ctrl->CTRLR0 |= TMOD(3);
	//DBG_8195A("CTRLR0 = %x %x\n", &(psram_ctrl->CTRLR0), psram_ctrl->CTRLR0);

	/* set flash_cmd: write cmd to fifo */
	if (type == PSRAM_TYPE_APM) {
		psram_ctrl->DR[0].BYTE = 0x40;
		psram_ctrl->DR[0].BYTE = 0x40;
	} else {
		psram_ctrl->DR[0].BYTE = 0xe0;
		psram_ctrl->DR[0].BYTE = 0x00;
	}


	if (CR) {
		psram_ctrl->DR[0].BYTE = 0x01;
	} else {
		psram_ctrl->DR[0].BYTE = 0x00;    // IR reg read only
	}

	psram_ctrl->DR[0].BYTE = 0x00;
	psram_ctrl->DR[0].BYTE = 0x00;

	psram_ctrl->DR[0].BYTE = (u8)(addr & 0xFF);//addr


	/* Enable SPI_FLASH  User Mode */
	psram_ctrl->SSIENR = BIT_SPIC_EN;
	while (psram_ctrl->SSIENR & BIT_SPIC_EN) {
	};

	/* read data */
	rx_num = 0;

	while (rx_num < read_len) {
		read_data[rx_num] = psram_ctrl->DR[0].BYTE;
		rx_num += 1;
	}

	/* Wait transfer complete. When complete, SSIENR[0] and CTRLR0[31] is cleared by HW automatically. */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Restore to auto mode */
	psram_ctrl->CTRLR0 &= ~BIT_USER_MODE;

}

/**
  * @brief set APM/Winbond PSRAM register value
  * @param  type: APM :0 Winbond:1
  * @param  addr: PSRAM register addr
  * @param  write_len: write data length
  * @param  write_data: write data buf
  * @retval None
  */
NON_DRAM_TEXT_SECTION
void PSRAM_REG_Write(u32 type, u32 addr, u32 write_len, u8 *write_data)
{
	SPIC_TypeDef *psram_ctrl = PSRAMC_DEV;
	u32 tx_num = 0;

	/* Wait spic busy done before switch to user mode */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Enter User Mode */
	psram_ctrl->CTRLR0 |= BIT_USER_MODE;

	/* Disable SSIENR to program control registers*/
	psram_ctrl->SSIENR = 0;

	//set tx mode
	psram_ctrl->CTRLR0 &= ~(TMOD(3));

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	psram_ctrl->RX_NDF = 0;
	psram_ctrl->TX_NDF = TX_NDF(write_len);

	/* set flash_cmd: write cmd to fifo */
	if (type == PSRAM_TYPE_APM) {

		psram_ctrl->DR[0].BYTE = 0xc0;
		psram_ctrl->DR[0].BYTE = 0xc0;
	} else {

		psram_ctrl->DR[0].BYTE = 0x60;
		psram_ctrl->DR[0].BYTE = 0x00;
	}

	psram_ctrl->DR[0].BYTE = 0x01;
	psram_ctrl->DR[0].BYTE = 0x00;
	psram_ctrl->DR[0].BYTE = 0x00;

	psram_ctrl->DR[0].BYTE = (u8)(addr & 0xFF);//addr

	/* fill data */
	for (tx_num = 0; tx_num < write_len; tx_num++) {
		psram_ctrl->DR[0].BYTE = (u8) * (write_data + tx_num);
	}

	/* Enable SPI_FLASH  User Mode */
	psram_ctrl->SSIENR = BIT_SPIC_EN;
	while (psram_ctrl->SSIENR & BIT_SPIC_EN) {
	};

	/* Wait transfer complete. When complete, SSIENR[0] and CTRLR0[31] is cleared by HW automatically. */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Restore to auto mode */
	psram_ctrl->CTRLR0 &= ~BIT_USER_MODE;

}

/**
  * @brief set APM PSRAM register value
  * @param  cmd: APM PSRAM write mem cmd: 0x80 or 0xa0
  * @param  addr: APM PSRAM register addr
  * @param  write_len: write data length
  * @param  write_data: write data buf
  * @retval None
  */

void PSRAM_MEM_Write(u8 cmd, u32 addr, u32 write_len, u8 *write_data)
{
	SPIC_TypeDef *psram_ctrl = PSRAMC_DEV;
	u32 tx_num = 0;
	u32 txlen = write_len + (PsramInfo.Psram_Latency_Set - 1) * 2;

	/* Wait spic busy done before switch to user mode */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Enter User Mode */
	psram_ctrl->CTRLR0 |= BIT_USER_MODE;

	/* Disable SSIENR to program control registers*/
	psram_ctrl->SSIENR = 0;

	//set tx mode
	psram_ctrl->CTRLR0 &= ~(TMOD(3));
	DBG_8195A("CTRLR0 = %x %x\n", &(psram_ctrl->CTRLR0), psram_ctrl->CTRLR0);

	/* Set RX_NDF: frame number of receiving data. TX_NDF should be set in both transmit mode and receive mode.
		TX_NDF should be set to zero in receive mode to skip the TX_DATA phase. */
	psram_ctrl->RX_NDF = 0;
	psram_ctrl->TX_NDF = TX_NDF(txlen);

	addr = addr - PSRAM_BASE;

	/* set flash_cmd: write cmd to fifo */
	psram_ctrl->DR[0].BYTE = cmd;
	psram_ctrl->DR[0].BYTE = cmd;
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF000000) >> 24);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF0000) >> 16);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF00) >> 8);
	psram_ctrl->DR[0].BYTE = (u8)(addr & 0xFF);

	/* fill data */
	for (tx_num = 0; tx_num < (PsramInfo.Psram_Latency_Set - 1) * 2; tx_num++) {
		psram_ctrl->DR[0].BYTE = 0x0;
	}

	/* fill data */
	for (tx_num = 0; tx_num < write_len; tx_num++) {
		psram_ctrl->DR[0].BYTE = (u8) * (write_data + tx_num);
	}

	/* Enable SPI_FLASH  User Mode */
	psram_ctrl->SSIENR = BIT_SPIC_EN;

	while (psram_ctrl->SSIENR & BIT_SPIC_EN) {
	};

	/* Wait transfer complete. When complete, SSIENR[0] and CTRLR0[31] is cleared by HW automatically. */
	while (psram_ctrl->SR & BIT_BUSY);

	/* Restore to auto mode */
	psram_ctrl->CTRLR0 &= ~BIT_USER_MODE;

}

/**
  * @brief PSRAM FW calibration function
  * @retval None
  * @note cache will be disable during calibration
  */
BOOL PSRAM_calibration(VOID)
{
	PSPHY_TypeDef *psram_phy = PSRAMPHY_DEV;

	u32 caltempN = 0;
	u32 phase = 0;
	u32 tempPHYPara = psram_phy->PSPHY_CAL_PAR & (~PSPHY_MASK_CFG_CAL_N) & (~PSPHY_MASK_PRE_CAL_PHASE);
	u32 *tempdatawr = PSRAM_CALIB_PATTERN;
	u32 tempdatard[6] = {0, 0, 0, 0, 0, 0};
	int window_start = -1;
	int window_end = -1;
	int window_size = 0;

	int windowt_start = -1;
	int windowt_size = 0;
	int windowt_end = -1;

	int phase_cnt = -1;

	DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);
	DCache_Disable();
	/*Disable HW calibration*/
	psram_phy->PSPHY_CAL_CTRL &= (~PSPHY_BIT_CFG_CAL_EN);
	for (phase = 0x1; phase <= 0x8; phase = phase * 2) {
		windowt_size = 0;
		windowt_start = -1;
		windowt_end = -1;
		//DBG_8195A("===phase %x =====\n",phase);

		for (caltempN = 0; caltempN < 32; caltempN++) {
			psram_phy->PSPHY_CAL_PAR = tempPHYPara | caltempN | PSPHY_PRE_CAL_PHASE(phase);
			HAL_WRITE32(PSRAM_BASE, 0x0, tempdatawr[0]);
			HAL_WRITE32(PSRAM_BASE, 0x50000, tempdatawr[1]);
			HAL_WRITE32(PSRAM_BASE, 0x100000, tempdatawr[2]);
			HAL_WRITE32(PSRAM_BASE, 0x150000, tempdatawr[3]);
			HAL_WRITE32(PSRAM_BASE, 0x200000, tempdatawr[4]);
			HAL_WRITE32(PSRAM_BASE, 0x250000, tempdatawr[5]);

			tempdatard[0] = HAL_READ32(PSRAM_BASE, 0x0);
			tempdatard[1] = HAL_READ32(PSRAM_BASE, 0x50000);
			tempdatard[2] = HAL_READ32(PSRAM_BASE, 0x100000);
			tempdatard[3] = HAL_READ32(PSRAM_BASE, 0x150000);
			tempdatard[4] = HAL_READ32(PSRAM_BASE, 0x200000);
			tempdatard[5] = HAL_READ32(PSRAM_BASE, 0x250000);

			if (_memcmp(tempdatard, PSRAM_CALIB_PATTERN, 24) == 0) {
				//DBG_8195A("ok %x %x %x %x %x %x %x\n",caltempN, tempdatard[0],tempdatard[1],tempdatard[2],tempdatard[3],tempdatard[4],tempdatard[5]);
				if (windowt_start < 0) {
					windowt_start = caltempN;
				}
				windowt_end = windowt_start + windowt_size;
				windowt_size ++;

				if (caltempN == (32 - 1)) {
					if (windowt_size > window_size) {
						window_start = windowt_start;
						window_end = windowt_end;
						window_size = windowt_size;
						phase_cnt = phase;
					}
				}
			} else {
				//DBG_8195A("fail %x %x %x %x %x %x %x\n",caltempN, tempdatard[0],tempdatard[1],tempdatard[2],tempdatard[3],tempdatard[4],tempdatard[5]);
				if (windowt_start >= 0) {
					if (windowt_size > window_size) {
						window_start = windowt_start;
						window_end = windowt_end;
						window_size = windowt_size;
						phase_cnt = phase;

					}
					windowt_start = -1;
					windowt_size = 0;
					windowt_end = -1;
				}
			}
		}
	}


	DBG_8195A("CalNmin = %x CalNmax = %x WindowSize = %x phase: %x \n", window_start, window_end, window_size, phase_cnt);

	DCache_Enable();

	if ((window_size) < 9) {
		return _FALSE;
	}
	//tempPHYPara &= (~0xfffff);
	tempPHYPara = PSPHY_CFG_CAL_JMAX((window_end - window_start) / 2 - 2) | \
				  PSPHY_CFG_CAL_JMIN(Psram_phy_Jmin) | \
				  PSPHY_CFG_CAL_J((window_end - window_start) / 2 - 2)	| \
				  PSPHY_CFG_CAL_N((window_end + window_start) / 2) | \
				  PSPHY_PRE_CAL_PHASE(phase_cnt);

	psram_phy->PSPHY_CAL_PAR = tempPHYPara;

	/*start HW calibration*/
	psram_phy->PSPHY_CAL_CTRL |= PSPHY_BIT_CFG_CAL_EN;

	return _TRUE;

}

/**
  * @brief  Enable/Disable Psram auto gating function for power saving.
  * @param  Enable: Enable or Disable autogating function.
  * @param  IDleCnt: idle counter setting, unit us.
  * @param  ResumeCnt: dq pads resume counter setting, unit us.
  * @retval None
  * @note only in auto mode, disacle before using user mode
  */
NON_DRAM_TEXT_SECTION
void PSRAM_AutoGating(u32 Enable, u32 IDleCnt, u32 ResumeCnt)
{
	u32 Rtemp = 0;
	if (Enable) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1);
		Rtemp |= LSYS_BIT_CKD_XTAL_2M_EN;
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1, Rtemp);

		Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0);
		Rtemp &= ~(PAD_MASK_PWDPAD_RESUME_VAL | PAD_MASK_PWDPAD_IDLE_VAL);
		Rtemp |= PAD_PWDPAD_RESUME_VAL(ResumeCnt);
		Rtemp |= PAD_PWDPAD_IDLE_VAL(IDleCnt);
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0, Rtemp);

		Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0);
		Rtemp |= PAD_BIT_PWDPAD_DQ_EN; //don't write 1 if user mode
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0, Rtemp);
	} else {

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1);
		Rtemp &= (~LSYS_BIT_CKD_XTAL_2M_EN);
		HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKD_GRP1, Rtemp);

		Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0);
		Rtemp &= (~PAD_BIT_PWDPAD_DQ_EN); //don't write 1 if user mode
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL0, Rtemp);

	}
}

/**
  * @brief  set psram into half sleep mode.
  * @param  type: 0:apm psram type 1: winbond psram type
  * @retval None
  * @note None
  */
NON_DRAM_TEXT_SECTION
void set_psram_sleep_mode(u32 type)
{
	u8 psram_halfsleep[2];
	u32 cur_src = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	u8 psraminfo[2];
	RRAM_TypeDef *rram = RRAM_DEV;

	PSRAM_AutoGating(DISABLE, (uint32_t)NULL, (uint32_t)NULL);

	// 50ns will be enough, check if need
	DelayUs(1);

	if (type == PSRAM_TYPE_APM) {
		psram_halfsleep[0] = 0xF0;
		psram_halfsleep[1] = 0xF0;

		PSRAM_REG_Write(0, 0x06, 2, psram_halfsleep);

	} else {

		PSRAM_REG_Read(1, 0x0, 2, psraminfo, 0);
		if (psraminfo[1] == 0x5F) {
			psram_halfsleep[0] = 0xFF;
			psram_halfsleep[1] = 0x20;
		} else {
			psram_halfsleep[0] = 0xFF;
			psram_halfsleep[1] = 0xE1;
		}
		PSRAM_REG_Write(1, 0x1, 2, psram_halfsleep);

	}

	if (cur_src == BIT_LSYS_CKSL_PSRAM_LBUS) {
		PSRAM_AutoGating(ENABLE, Psram_IDLETIME_XTAL, Psram_RESUMECNT_XTAL);
	} else if (cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL || cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL) {
		PSRAM_AutoGating(ENABLE, Psram_IDLETIME_PLL, rram->PSRAM_RESUMECNT_BOOT);
	}

}

/**
  * @brief  Wake PSRAM from half sleep mode.
  * @param  type: 0:apm psram type 1: winbond psram type
  * @retval None
  * @note None
  */
NON_DRAM_TEXT_SECTION
void set_psram_wakeup_mode(u32 type)
{
	u32 temp;
	u32 cur_src = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));
	RRAM_TypeDef *rram = RRAM_DEV;

	PSRAM_AutoGating(DISABLE, (u32)NULL, (u32)NULL);

	DCache_CleanInvalidate(0x60000100, 32);
	temp = HAL_READ32(0x60000100, 0);
	//DBG_8195A("t: %x\n", temp);

	PSPHY_TypeDef *psram_phy = PSRAMPHY_DEV;

	temp = psram_phy->PSPHY_PHY_INFO;
	psram_phy->PSPHY_PHY_INFO = temp;

	if (type == PSRAM_TYPE_APM) {
		DelayUs(150);
	} else  if (type == PSRAM_TYPE_WB) {
		DelayUs(100);
	}

	DCache_Invalidate(0x60000100, 32);

	if (cur_src == BIT_LSYS_CKSL_PSRAM_LBUS) {
		PSRAM_AutoGating(ENABLE, Psram_IDLETIME_XTAL, Psram_RESUMECNT_XTAL);
	} else if (cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL || cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL) {
		PSRAM_AutoGating(ENABLE, Psram_IDLETIME_PLL, rram->PSRAM_RESUMECNT_BOOT);
	}

}

NON_DRAM_TEXT_SECTION
void set_psram_suspend(void)
{
	RRAM_TypeDef *rram = RRAM_DEV;
	u32 Rtemp = 0;

	if (rram->PSRAM_TYPE == PSRAM_TYPE_APM || rram->PSRAM_TYPE == PSRAM_TYPE_WB) {
		/* psram shutdown */
		set_psram_sleep_mode(rram->PSRAM_TYPE);

		/* shutdown psram pad */
		Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1);
		Rtemp &= ~PAD_BIT_DDR_PWDPADN_2REGU;
		Rtemp |= PAD_BIT_DDR_PD_REF;
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, Rtemp);
	} else {
		/* no psram */
		return;
	}
}

NON_DRAM_TEXT_SECTION
void set_psram_resume(void)
{
	RRAM_TypeDef *rram = RRAM_DEV;
	u32 Rtemp = 0;

	if (rram->PSRAM_TYPE == PSRAM_TYPE_APM || rram->PSRAM_TYPE == PSRAM_TYPE_WB) {

		/* psram re-enable */
		//open psram pad
		Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1);
		Rtemp |= PAD_BIT_DDR_PWDPADN_2REGU;
		Rtemp &= ~PAD_BIT_DDR_PD_REF;
		HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, Rtemp);

		//psram device exit self refresh
		set_psram_wakeup_mode(rram->PSRAM_TYPE);

	} else {
		/* no psram */
		return;
	}
}



/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/
