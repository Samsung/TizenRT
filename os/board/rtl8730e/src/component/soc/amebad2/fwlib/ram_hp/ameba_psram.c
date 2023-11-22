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

static const char *TAG = "PSRAM";
u8 APM_WR_INIT_LATENCY_SPEC[6] = {
	APM_WR_INIT_LATENCY_3CLK,
	APM_WR_INIT_LATENCY_4CLK,
	APM_WR_INIT_LATENCY_5CLK,
	APM_WR_INIT_LATENCY_6CLK,
	APM_WR_INIT_LATENCY_7CLK,
	APM_WR_INIT_LATENCY_8CLK,
};

u8 WB_WR_INIT_LATENCY_SPEC[5] = {
	WB_WR_INIT_LATENCY_3CLK,
	WB_WR_INIT_LATENCY_4CLK,
	WB_WR_INIT_LATENCY_5CLK,
	WB_WR_INIT_LATENCY_6CLK,
	WB_WR_INIT_LATENCY_7CLK,
};

static u32 PSRAM_CALIB_PATTERN[6] = {
	0x11223344,
	0xA55AA55A,
	0x5AA55AA5,
	0x44332211,
	0x96696996,
	0x69969669,
};

PSRAMINFO_TypeDef PsramInfo;

/**
  * @brief get psram info
  * @param  none
  * @retval PSRAM Type
  */
void PSRAM_INFO_Update(void)
{
	u32 PsramClk = 0;
	u32 Div;

	Div = LSYS_GET_CKD_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0));
	Div += 1;

	PsramClk = SocClk_Info->NPPLL_CLK * 1000000 / Div;

	PsramInfo.Psram_Clk_Set = PsramClk / 2;
	PsramInfo.Psram_Vendor =  ChipInfo_MemoryVendor() ;
	//PsramInfo.Psram_Clk_Limit = PSRAM_CLK_LIMIT_GET(chipinfo);
	PsramInfo.Psram_Size = ChipInfo_MemorySize();
	PsramInfo.Psram_Page_size = PSRAM_PAGE1024;
	PsramInfo.PSRAMC_Clk_Unit = 1000000000 / PsramClk;
	PsramInfo.Psram_Resume_Cnt = Psram_RESUME_TIME / (PsramInfo.PSRAMC_Clk_Unit);

	//rram->PSRAM_RESUMECNT_BOOT = PsramInfo.Psram_Resume_Cnt;		//for nonsecure world

	if (PsramInfo.Psram_Vendor == Vendor_PSRAM_A) {
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

	} else {
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
		} else if ((PsramInfo.Psram_Clk_Set <= 250000000) && (PsramInfo.Psram_Size == PSRAM_Size_256M)) {
			PsramInfo.Psram_Latency_Set = 7;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		} else if ((PsramInfo.Psram_Clk_Set <= 250000000) && (PsramInfo.Psram_Size == PSRAM_Size_128M)) {
			PsramInfo.Psram_Latency_Set = 10;
			PsramInfo.Psram_CSHI = Psram_WB_CSHI166;
		}
		/* backup latency code for KM0 for sleep */
		RRAM->PSRAM_LATENCY = PsramInfo.Psram_Latency_Set;
	}

	RTK_LOGI(TAG, "PSRAM Ctrl CLK: %d Hz \n", PsramClk);
}

void PSRAM_APM_DEVIC_Init(void)
{
	u8 mr0[2];
	u8 mr4[2];
	mr0[0] = PSRAM_READ_LATENCY_CODE(PsramInfo.Psram_Latency_Set - 3) | \
			 PSRAM_LT_SELECT(1); //0:variable latency 1:fix latency, fix only

	mr4[0] = PSRAM_WRITE_LATENCY_CODE(APM_WR_INIT_LATENCY_SPEC[PsramInfo.Psram_Latency_Set - 3]);

	mr0[1] = mr0[0];
	mr4[1] = mr4[0];

	PSRAM_REG_Write(0x0, 2, mr0);
	PSRAM_REG_Write(0x4, 2, mr4);
}

void PSRAM_WB_DEVIC_Init(void)
{
	u8 data[2];
	data[0] = PSRAM_WB_RESERVER | \
			  PSRAM_WB_DRIVE_STRENGTH(0x0) | \
			  PSRAM_WB_DPD_EN(1);	 // keep default driving stength

	data[1] = PSRAM_WB_BURST_LENGTH(0x0) | \
			  PSRAM_WB_HyBURSE_EN | \
			  PSRAM_WB_FIX_LATENCY_EN(1) | \
			  PSRAM_WB_INIT_LATENCY(WB_WR_INIT_LATENCY_SPEC[PsramInfo.Psram_Latency_Set - 3]);		// 1 for 6T 150M, 2 for 7T 200M

	PSRAM_WB_REG_Write(PSRAM_WB_CR0, 2, data);
}

/**
  * @brief  Fills each PSPHY_InitStruct member with its default value.
  * @param  PSPHY_InitStruct: pointer to an PSPHY_InitStruct structure which will be initialized.
  * @retval None
  */
void PSRAM_PHY_StructInit(PSPHY_InitTypeDef *PSPHY_InitStruct)
{
	// 0x04
	PSPHY_InitStruct->PSRAMP_PRE_CAL_PHASE = 0x1; /* only for device init, update in calibration */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_JMIN = 0x2; /* suggested by HW */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_JMAX = 0x3; /* only for device init, update in calibration */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_J = 0x3; /* only for device init, update in calibration */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_N = 0xA; /* 0~31, middle at first */

	//0x18
	PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE = 0x1;
	PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY = PSPHY_RFIFO_READY_DELAY;
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

	//0x04
	psram_phy->PSPHY_CAL_PAR = PSPHY_PRE_CAL_PHASE(PSPHY_InitStruct->PSRAMP_PRE_CAL_PHASE) | \
							   PSPHY_CFG_CAL_JMIN(PSPHY_InitStruct->PSRAMP_CFG_CAL_JMIN) | \
							   PSPHY_CFG_CAL_JMAX(PSPHY_InitStruct ->PSRAMP_CFG_CAL_JMAX) | \
							   PSPHY_CFG_CAL_J(PSPHY_InitStruct->PSRAMP_CFG_CAL_J) | \
							   PSPHY_CFG_CAL_N(PSPHY_InitStruct->PSRAMP_CFG_CAL_N);

	if (ChipInfo_MemoryVendor() == Vendor_PSRAM_A) {
		//0x18
		psram_phy->PSPHY_PHY_CTRL = PSPHY_BIT_PSRAM_SW_RST_N | \
									PSPHY_CMD_END_DLY(PSPHY_CMD_END_DELAY) | \
									PSPHY_CAL_RWDS_PHASE(PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE) | \
									PSPHY_RFIFO_RDY_DLY(PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY);
	} else {
		psram_phy->PSPHY_PHY_CTRL = PSPHY_BIT_PSRAM_SW_RST_N | \
									PSPHY_CMD_END_DLY(PSPHY_CMD_END_DELAY) | \
									PSPHY_BIT_CMD_RW_POL | \
									PSPHY_CAL_RWDS_PHASE(PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE) | \
									PSPHY_RFIFO_RDY_DLY(PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY);
	}

}

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

	if (PsramInfo.Psram_Vendor == Vendor_PSRAM_B) {
		//0x110
		psram_ctrl->CTRLR2 = BIT_SO_DNUM | TX_FIFO_ENTRY(0x5) | RX_FIFO_ENTRY(0x5) | BIT_DM_ACT | BIT_DIS_DM_CA;
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
		psram_ctrl->USER_LENGTH = USER_ADDR_LENGTH(0x5) | USER_CMD_LENGHT(0x1) | USER_RD_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 1);
	} else {
		//0x110
		psram_ctrl->CTRLR2 = BIT_SO_DNUM | TX_FIFO_ENTRY(0x5) | RX_FIFO_ENTRY(0x5) | BIT_DM_ACT;
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
		psram_ctrl->USER_LENGTH = USER_ADDR_LENGTH(0x4) | USER_CMD_LENGHT(0x2) | USER_RD_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 1);
	}

	//auto rd latency
	psram_ctrl->AUTO_LENGTH |= RD_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 1) | \
							   IN_PHYSICAL_CYC(PSPHY_RFIFO_READY_DELAY_FIX + 4);
	if (PsramInfo.Psram_Vendor == Vendor_PSRAM_B) {
		psram_ctrl->AUTO_LENGTH2 = WR_DUMMY_LENGTH(2 * 2 * PsramInfo.Psram_Latency_Set - 2);
	} else {
		psram_ctrl->AUTO_LENGTH2 = WR_DUMMY_LENGTH(2 * PsramInfo.Psram_Latency_Set - 2);

	}

	while ((psram_ctrl->SR & BIT_BOOT_FIN) == 0);
}

/**
  * @brief Generate PSRAM command address value for dpin mode
  * @param  PSRAM_CA: pointer to the CA array to be generated
  * @param  StartAddr: Specify the target address
  * @param  BurstType: Indicate the burst transmission mode
  *			@arg PSRAM_WRAPPED_TYPE: indicate wrapper burst.
  *			@arg PSRAM_LINEAR_TYPE: indicate linear burst.
  * @param  AddSpace: Indicate access the memory or register space
  *			@arg PSRAM_MEM_SPACE: indicate memory space.
  *			@arg PSRAM_REG_SPACE: indicate register space.
  * @param  RW: Indicate the transaction mode
  *			@arg PSRAM_WRITE_TRANSACTION: indicate a write transaction.
  *			@arg PSRAM_READ_TRANSACTION: indicate a read transaction.
  * @retval None
  */
void PSRAM_CTRL_CA_Gen(u8 *PSRAM_CA, u32 StartAddr, u8 BurstType, u8 AddSpace, u8 RW)
{
	PSRAM_CA[0] = StartAddr & 0x7;
	PSRAM_CA[1] = 0;
	PSRAM_CA[2] = (StartAddr >> 3) & 0xf;
	PSRAM_CA[3] = (StartAddr >> 11) & 0xf;
	PSRAM_CA[4] = (StartAddr >> 19) & 0xf;
	PSRAM_CA[5] = ((StartAddr >> 27) & 0xf) | (BurstType << 5) | (AddSpace << 6) | (RW << 7);
}

/**
  * @brief Get APM PSRAM register value
  * @param  addr: APM PSRAM register addr
  * @param  read_len: read data length
  * @param  read_data: read data output buf
  * @retval None
  */
void PSRAM_REG_Read(u32 addr, u32 read_len, u8 *read_data)
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
	RTK_LOGI(TAG, "RX_NDF = %p %x\n", &(psram_ctrl->RX_NDF), psram_ctrl->RX_NDF);

	//set rx mode
	psram_ctrl->CTRLR0 |= TMOD(3);
	RTK_LOGI(TAG, "CTRLR0 = %p %x\n", &(psram_ctrl->CTRLR0), psram_ctrl->CTRLR0);

	/* set flash_cmd: write cmd to fifo */
	psram_ctrl->DR[0].BYTE = 0x40;
	psram_ctrl->DR[0].BYTE = 0x40;

	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF000000) >> 24);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF0000) >> 16);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF00) >> 8);

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
  * @brief set APM PSRAM register value
  * @param  addr: APM PSRAM register addr
  * @param  write_len: write data length
  * @param  write_data: write data buf
  * @retval None
  */
void PSRAM_REG_Write(u32 addr, u32 write_len, u8 *write_data)
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
	psram_ctrl->DR[0].BYTE = 0xc0;
	psram_ctrl->DR[0].BYTE = 0xc0;

	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF000000) >> 24);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF0000) >> 16);
	psram_ctrl->DR[0].BYTE = (u8)((addr & 0xFF00) >> 8);

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
	RTK_LOGI(TAG, "CTRLR0 = %p %x\n", &(psram_ctrl->CTRLR0), psram_ctrl->CTRLR0);

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
  * @brief Get Winbond PSRAM register value
  * @param  regnum: Winbond PSRAM register num: PSRAM_WB_IR0/PSRAM_WB_IR1/PSRAM_WB_CR0/PSRAM_WB_CR1
  * @param  read_len: read data length
  * @param  read_data: read data output buf
  * @param  CR: CR reg or not
  * @retval None
  */
void PSRAM_WB_REG_Read(u32 regnum, u32 read_len, u8 *read_data, u32 CR)
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
	RTK_LOGI(TAG, "RX_NDF = %p %x\n", &(psram_ctrl->RX_NDF), psram_ctrl->RX_NDF);

	//set rx mode
	psram_ctrl->CTRLR0 |= TMOD(3);
	RTK_LOGI(TAG, "CTRLR0 = %p %x\n", &(psram_ctrl->CTRLR0), psram_ctrl->CTRLR0);

	/* set flash_cmd: write cmd to fifo */
	psram_ctrl->DR[0].BYTE = 0xe0;
	psram_ctrl->DR[0].BYTE = 0x00;

	if (CR) {
		psram_ctrl->DR[0].BYTE = 0x01;
	} else {
		psram_ctrl->DR[0].BYTE = 0x00;    // IR reg read only
	}

	psram_ctrl->DR[0].BYTE = 0x00;
	psram_ctrl->DR[0].BYTE = 0x00;

	psram_ctrl->DR[0].BYTE = regnum;//addr

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
  * @brief Set Winbond PSRAM register value
  * @param  regnum: Winbond PSRAM register num: PSRAM_WB_CR0/PSRAM_WB_CR1
  * @param  write_len: write data length
  * @param  write_data: write data buf
  * @retval None
  */
void PSRAM_WB_REG_Write(u32 regnum, u32 write_len, u8 *write_data)
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

	//need modify
	/* set flash_cmd: write cmd to fifo */
	psram_ctrl->DR[0].BYTE = 0x60;
	psram_ctrl->DR[0].BYTE = 0x00;

	psram_ctrl->DR[0].BYTE = 0x01;
	psram_ctrl->DR[0].BYTE = 0x00;
	psram_ctrl->DR[0].BYTE = 0x00;

	psram_ctrl->DR[0].BYTE = regnum;//addr

	/* fill data */
	for (tx_num = 0; tx_num < write_len; tx_num++) {
		psram_ctrl->DR[0].BYTE = (u8) * (write_data + tx_num);
	}

	/* Enable SPI_FLASH User Mode */
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
BOOL PSRAM_calibration(void)
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
		//RTK_LOGD(TAG, "===phase %x =====\n",phase);

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
				//RTK_LOGD(TAG, "ok %x %x %x %x %x %x %x\n",caltempN, tempdatard[0],tempdatard[1],tempdatard[2],tempdatard[3],tempdatard[4],tempdatard[5]);
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
				//RTK_LOGD(TAG, "fail %x %x %x %x %x %x %x\n",caltempN, tempdatard[0],tempdatard[1],tempdatard[2],tempdatard[3],tempdatard[4],tempdatard[5]);
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

	RTK_LOGI(TAG, "CalNmin = %x CalNmax = %x WindowSize = %x phase: %x \n", window_start, window_end, window_size, phase_cnt);

	DCache_Enable();

	if ((window_size) < 9) {
		return _FALSE;
	}
	tempPHYPara &= (~0xfffff);
	tempPHYPara |= PSPHY_CFG_CAL_JMAX((window_end - window_start) / 2 - 2) | \
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
  * @param  ResumeCnt: dq pads resume counter setting, unit psramc clk.
  * @retval None
  * @note only in auto mode, disacle before using user mode
  */
void PSRAM_AutoGating(u32 Enable, u32 IDleCnt, u32 ResumeCnt)
{
	u32 Rtemp = 0;
	if (Enable) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E4);
		Rtemp &= ~(HSYS_MASK_PWDPAD_RESUME_VAL | HSYS_MASK_PWDPAD_IDLE_VAL);

		Rtemp |= HSYS_PWDPAD_RESUME_VAL(ResumeCnt);
		Rtemp |= HSYS_PWDPAD_IDLE_VAL(IDleCnt);
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E4, Rtemp);

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
		Rtemp |= HSYS_BIT_PWDPAD_DQ_EN; //don't write 1 if user mode
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, Rtemp);

	} else {

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
		Rtemp &= (~HSYS_BIT_PWDPAD_DQ_EN); //don't write 1 if user mode
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, Rtemp);
	}
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/
