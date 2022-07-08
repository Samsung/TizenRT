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

/**
  * @brief  Fills each PSPHY_InitStruct member with its default value.
  * @param  PSPHY_InitStruct: pointer to an PSPHY_InitStruct structure which will be initialized.
  * @retval None
  */
void PSRAM_PHY_StructInit(PSPHY_InitTypeDef *PSPHY_InitStruct)
{
	// 0x04
	PSPHY_InitStruct->PSRAMP_PRE_CAL_PHASE = 0x2; /* only for rtl now, default choose 1 on asic */
	PSPHY_InitStruct->PSRAMP_CFG_CAL_JMIN = 0x2;
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
							   PSPHY_CFG_CAL_JMAX(PSPHY_InitStruct ->PSRAMP_CFG_CAL_JMAX) | \
							   PSPHY_CFG_CAL_J(PSPHY_InitStruct->PSRAMP_CFG_CAL_J) | \
							   PSPHY_CFG_CAL_N(PSPHY_InitStruct->PSRAMP_CFG_CAL_N);
	//0x18
	psram_phy->PSPHY_PHY_CTRL = PSPHY_PSRAM_SW_RST_N(PSPHY_InitStruct->PSRAMP_SW_RST_N) | \
								PSPHY_LATENCY_TYPE(PSPHY_InitStruct->PSRAMP_LATENCY_TYPE) | \
								PSPHY_CAL_RWDS_PHASE(PSPHY_InitStruct->PSRAMP_CAL_RWDS_PHASE) | \
								PSPHY_RFIFO_RDY_DLY(PSPHY_InitStruct->PSRAMP_RFIFO_RDY_DLY);

}

/**
  * @brief  Fills each PSRAM_InitStruct member with its default value, for APM.
  * @param  PSRAM_InitStruct: pointer to an PSRAM_InitTypeDef structure which will be initialized.
  * @retval None
  */
void PSRAM_CTRL_StructInit(PCTL_InitTypeDef *PCTL_InitStruct)
{
	// choose psram
	PCTL_InitStruct->PSRAMC_qspi_psram = 0x1;
	PCTL_InitStruct->PSRAMC_page_size = 0xa; // page 1024 bytes
	PCTL_InitStruct->PSRAMC_wr_page_en = 0x1;
	PCTL_InitStruct->PSRAMC_rd_page_en = 0x1;

	//clk setting
	PCTL_InitStruct->PSRAMC_clk_div = 0x1;

	//channel set
	PCTL_InitStruct->PSRAMC_ddr_en = 0x7; //cmd/data/addr all ddr
	PCTL_InitStruct->PSRAMC_ch = 0x3F; //cmd/data/addr all quad ch

	//timing
	PCTL_InitStruct->PSRAMC_ck_mtims = 0x1;
	PCTL_InitStruct->PSRAMC_cs_active_hold = 0x1;
	PCTL_InitStruct->PSRAMC_cs_seq_timeout = 0x10;
	PCTL_InitStruct->PSRAMC_cs_tcem = 0x0c;
	PCTL_InitStruct->PSRAMC_rd_dum_len = APM_WR_DUMMY_LATENCY;
	PCTL_InitStruct->PSRAMC_wr_dum_len = APM_WR_DUMMY_LATENCY;
	PCTL_InitStruct->WR_VL_EN = (!PSRSAM_FIX_LATENCY);	// 1:variable latency 0:fix latency
	PCTL_InitStruct->RD_VL_EN = (!PSRSAM_FIX_LATENCY);		// 1:variable latency 0:fix latency
	if (PCTL_InitStruct->RD_VL_EN) {
		PCTL_InitStruct->PSRAMC_auto_in_phy_cyc = 0;

	} else {
		PCTL_InitStruct->PSRAMC_auto_in_phy_cyc = PSPHY_RFIFO_READY_DELAY_FIX + 4;	// phy RFIFO_RDY_DLY + 4

	}
	//hw setting
	PCTL_InitStruct->PSRAMC_full_wr = 0x1;
	PCTL_InitStruct->PSRAMC_tx_fifo_entry = 0x5;
	PCTL_InitStruct->PSRAMC_rx_fifo_entry = 0x5;
	PCTL_InitStruct->PSRAMC_so_dnum = 0x1;
	PCTL_InitStruct->PSRAMC_jedec_p2cmf = 0x0;
	PCTL_InitStruct->PSRAMC_data_unit_2b = 0x0;
	PCTL_InitStruct->PSRAMC_dis_dm_ca = 0x0;
	PCTL_InitStruct->PSRAMC_atom_size = 0x1;
	PCTL_InitStruct->RD_WEIGHT = 0x2;

	//valid cmd
	PCTL_InitStruct->PSRAMC_frd_single = 0x1;
	PCTL_InitStruct->PSRAMC_seq_rd_en = 0x1;
	PCTL_InitStruct->PSRAMC_seq_wr_en = 0x1;

	//auto mode setting

	if (PCTL_InitStruct->RD_VL_EN) {

		PCTL_InitStruct->PSRAMC_auto_rd_latency = (2 * PSRAM_INIT_VL_RD_LATENCY_CLK - 4);
	} else {
		PCTL_InitStruct->PSRAMC_auto_rd_latency = (2 * PSRAM_INIT_FIX_RD_LATENCY_CLK - 4);
	}
	PCTL_InitStruct->PSRAMC_auto_wr_latency =  2 * (PSRAM_INIT_WR_LATENCY_CLK - 1);

	PCTL_InitStruct->PSRAMC_frd_cmd = 0x2020;
	PCTL_InitStruct->PSRAMC_wr_cmd = 0xa0a0;
	PCTL_InitStruct->PSRAMC_auto_addr_len = 0x4;

	//user mode setting
	PCTL_InitStruct->PSRAMC_user_addr_len = 0x4;
	PCTL_InitStruct->PSRAMC_user_cmd_len = 0x2;

	if (PCTL_InitStruct->RD_VL_EN) {

		PCTL_InitStruct->PSRAMC_user_rd_dummy_len = (2 * PSRAM_INIT_VL_RD_LATENCY_CLK - 4);
	} else {
		PCTL_InitStruct->PSRAMC_user_rd_dummy_len = (2 * PSRAM_INIT_FIX_RD_LATENCY_CLK - 4);
	}

}

/**
  * @brief  Fills each PSRAM_InitStruct member with its default value£¬ for winbond.
  * @param  PSRAM_InitStruct: pointer to an PSRAM_InitTypeDef structure which will be initialized.
  * @retval None
  */
void PSRAM_CTRL_WB_StructInit(PCTL_InitTypeDef *PCTL_InitStruct)
{
	u8 type;
	type = ChipInfo_PSRAMType();
	if (SYSCFG_CHIPType_Get() == CHIP_TYPE_PALADIUM) {
		type = PSRAM_B_256Mb;
	}

	// choose psram
	PCTL_InitStruct->PSRAMC_qspi_psram = 0x1;

	if (type == PSRAM_B_256Mb) {
		PCTL_InitStruct->PSRAMC_page_size = 0xa; // page 1024 bytes
	} else if (type == PSRAM_B_32Mb) {
		PCTL_InitStruct->PSRAMC_page_size = 0x7; // page 128 bytes
	}

	PCTL_InitStruct->PSRAMC_wr_page_en = 0x1;
	PCTL_InitStruct->PSRAMC_rd_page_en = 0x1;

	//clk setting
	PCTL_InitStruct->PSRAMC_clk_div = 0x1;

	//channel set
	PCTL_InitStruct->PSRAMC_ddr_en = 0x7; //cmd/data/addr all ddr
	PCTL_InitStruct->PSRAMC_ch = 0x3F; //cmd/data/addr all quad ch

	//timing
	PCTL_InitStruct->PSRAMC_ck_mtims = 0x1;
	PCTL_InitStruct->PSRAMC_cs_active_hold = 0x0;
	PCTL_InitStruct->PSRAMC_cs_seq_timeout = 0x10;
	PCTL_InitStruct->PSRAMC_cs_tcem = 0x0c;
	PCTL_InitStruct->PSRAMC_rd_dum_len = 0xc;
	PCTL_InitStruct->PSRAMC_wr_dum_len = 0xc;
	PCTL_InitStruct->WR_VL_EN = (!PSRSAM_FIX_LATENCY);	// 1:variable latency 0:fix latency
	PCTL_InitStruct->RD_VL_EN = (!PSRSAM_FIX_LATENCY);	// 1:variable latency 0:fix latency
	if (PCTL_InitStruct->RD_VL_EN) {
		PCTL_InitStruct->PSRAMC_auto_in_phy_cyc = 0x0;

	} else {
		PCTL_InitStruct->PSRAMC_auto_in_phy_cyc = PSPHY_RFIFO_READY_DELAY_FIX + 4;	// phy RFIFO_RDY_DLY + 4

	}

	//hw setting
	PCTL_InitStruct->PSRAMC_dis_dm_ca = 0x1;
	PCTL_InitStruct->PSRAMC_full_wr = 0x1;
	PCTL_InitStruct->PSRAMC_tx_fifo_entry = 0x5;
	PCTL_InitStruct->PSRAMC_rx_fifo_entry = 0x5;
	PCTL_InitStruct->PSRAMC_so_dnum = 0x1;
	PCTL_InitStruct->PSRAMC_jedec_p2cmf = 0x1;
	PCTL_InitStruct->PSRAMC_data_unit_2b = 0x1;
	PCTL_InitStruct->PSRAMC_atom_size = 0x1;
	PCTL_InitStruct->RD_WEIGHT = 0x2;

	//valid cmd
	PCTL_InitStruct->PSRAMC_frd_single = 0x1;
	PCTL_InitStruct->PSRAMC_seq_rd_en = 0x1;
	PCTL_InitStruct->PSRAMC_seq_wr_en = 0x1;

	//auto mode setting
	if (PCTL_InitStruct->RD_VL_EN) {
		PCTL_InitStruct->PSRAMC_auto_rd_latency = (2 * PSRAM_WB_INIT_VL_RD_LATENCY_CLK - 4);
		PCTL_InitStruct->PSRAMC_auto_wr_latency =  2 * (PSRAM_WB_INIT_VL_WR_LATENCY_CLK - 1);
	} else {
		PCTL_InitStruct->PSRAMC_auto_rd_latency = (2 * PSRAM_WB_INIT_FIX_RD_LATENCY_CLK - 4);
		PCTL_InitStruct->PSRAMC_auto_wr_latency =  2 * (PSRAM_WB_INIT_FIX_WR_LATENCY_CLK - 1);


	}
	PCTL_InitStruct->PSRAMC_frd_cmd = 0xa0;
	PCTL_InitStruct->PSRAMC_wr_cmd = 0x20;
	PCTL_InitStruct->PSRAMC_auto_addr_len = 0x5;

	//user mode setting
	PCTL_InitStruct->PSRAMC_user_addr_len = 0x5;
	PCTL_InitStruct->PSRAMC_user_cmd_len = 0x1;
	if (PCTL_InitStruct->RD_VL_EN) {
		PCTL_InitStruct->PSRAMC_user_rd_dummy_len = (2 * PSRAM_WB_INIT_VL_RD_LATENCY_CLK - 4);
	} else {
		PCTL_InitStruct->PSRAMC_user_rd_dummy_len = (2 * PSRAM_WB_INIT_FIX_RD_LATENCY_CLK - 4);


	}
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
void PSRAM_CTRL_Init(PCTL_InitTypeDef *PCTL_InitStruct)
{
	SPIC_TypeDef *psram_ctrl = PSRAMC_DEV;

	/* Disable psramc */
	psram_ctrl->SSIENR = 0;

	//0x00
	psram_ctrl->CTRLR0 = DDR_EN(PCTL_InitStruct->PSRAMC_ddr_en) | \
						 (PCTL_InitStruct->PSRAMC_ch << 16) | \
						 CK_MTIMES(PCTL_InitStruct->PSRAMC_ck_mtims);

	//0x14
	psram_ctrl->BAUDR = SCKDV(PCTL_InitStruct->PSRAMC_clk_div);

	//0x120
	psram_ctrl->VALID_CMD = (PCTL_InitStruct->PSRAMC_seq_wr_en << 15) | \
							(PCTL_InitStruct->PSRAMC_seq_rd_en << 14) | \
							(PCTL_InitStruct->PSRAMC_frd_single) | BIT_CTRLR0_CH;

	//0x138 waiting update typedefine
	psram_ctrl->TPR0 = (PCTL_InitStruct->PSRAMC_cs_tcem << 24) | \
					   (PCTL_InitStruct->PSRAMC_cs_seq_timeout << 16) | \
					   (PCTL_InitStruct->PSRAMC_cs_active_hold << 12) | \
					   (PCTL_InitStruct->PSRAMC_wr_dum_len << 6) | \
					   (PCTL_InitStruct->PSRAMC_rd_dum_len);

	//0x134 waiting update typedefine
	psram_ctrl->DEVICE_INFO = PAGE_SIZE(PCTL_InitStruct->PSRAMC_page_size) | \
							  (PCTL_InitStruct->PSRAMC_wr_page_en << 4) | \
							  (PCTL_InitStruct->PSRAMC_rd_page_en << 5) | \
							  (PCTL_InitStruct->PSRAMC_atom_size << 6) | \
							  (PCTL_InitStruct->PSRAMC_qspi_psram << 10) | \
							  (PCTL_InitStruct->PSRAMC_jedec_p2cmf << 11) | \
							  (PCTL_InitStruct->PSRAMC_data_unit_2b << 12);

	//0x110
	psram_ctrl->CTRLR2 = (PCTL_InitStruct->PSRAMC_so_dnum) | \
						 TX_FIFO_ENTRY(PCTL_InitStruct->PSRAMC_tx_fifo_entry) | \
						 RX_FIFO_ENTRY(PCTL_InitStruct->PSRAMC_rx_fifo_entry) | BIT_DM_ACT | \
						 (PCTL_InitStruct->PSRAMC_dis_dm_ca << 14) | \
						 (PCTL_InitStruct->RD_VL_EN << 16) | \
						 (PCTL_InitStruct->WR_VL_EN << 17) | \
						 (PCTL_InitStruct->RD_WEIGHT << 19);
	psram_ctrl->CTRLR2 &= ~BIT_FULL_WR;  //bit 13 = 0 need to disable


	//0x11c
	psram_ctrl->AUTO_LENGTH = RD_DUMMY_LENGTH(PCTL_InitStruct->PSRAMC_auto_rd_latency) | \
							  AUTO_ADDR_LENGTH(PCTL_InitStruct->PSRAMC_auto_addr_len) | \
							  IN_PHYSICAL_CYC(PCTL_InitStruct->PSRAMC_auto_in_phy_cyc);

	//0x13c waiting update
	psram_ctrl->AUTO_LENGTH2 = (PCTL_InitStruct->PSRAMC_auto_wr_latency << 0);

	//0xe0
	psram_ctrl->READ_FAST_SINGLE = FRD_CMD(PCTL_InitStruct->PSRAMC_frd_cmd);

	//0xf4
	psram_ctrl->WRITE_SIGNLE = WR_CMD(PCTL_InitStruct->PSRAMC_wr_cmd);

	//0x118
	psram_ctrl->USER_LENGTH = (PCTL_InitStruct->PSRAMC_user_addr_len << 16) | \
							  (PCTL_InitStruct->PSRAMC_user_cmd_len << 12) | \
							  (PCTL_InitStruct->PSRAMC_user_rd_dummy_len << 0);

	while ((psram_ctrl->SR & BIT_BOOT_FIN) == 0);
}

void PSRAM_APM_DEVIC_Init(void)
{
	u8 mr0[2];
	u8 mr4[2];
	mr0[0] = PSRAM_READ_LATENCY_CODE(PSRAM_INIT_FIX_RD_LATENCY_CLK / 2 - 3) | \
			 PSRAM_LT_SELECT(PSRSAM_FIX_LATENCY); //0:variable latency 1:fix latency

	mr4[0] = PSRAM_WRITE_LATENCY_CODE(APM_WR_INIT_LATENCY_SPEC[PSRAM_INIT_WR_LATENCY_CLK - 3]);

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
			  PSRAM_WB_INIT_LATENCY(0x1);		// 1 for 6T 150M, 2 for 7T 200M (need modifycation)
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
	u32 txlen = write_len + (PSRAM_INIT_WR_LATENCY_CLK - 1) * 2;

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
	for (tx_num = 0; tx_num < (PSRAM_INIT_WR_LATENCY_CLK - 1) * 2; tx_num++) {
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

void PSRAM_calwindow(int window_start, int window_end, PCAL_InitTypeDef *PCAL_InitStruct)
{
	u32 suggest_phase = 0;
	int window_size = 0;
	int windowt_size, windowt_start, windowt_end;
	int JMAX_val = 0;
	int J_val = 0;
	int N_val = 0;
	int i, head, tail;
	u32 phase;
	for (i = 0; i < 4; i++) {
		head = i * 16;
		tail = head + 31;
		phase = BIT(i);
		windowt_start = head > window_start ? head : window_start;
		windowt_end = tail < window_end ? tail : window_end;
		windowt_size = windowt_end - windowt_start + 1;
		if (windowt_size > window_size) {

			window_size = windowt_size;
			N_val = (windowt_start + windowt_end - head - head) / 2;
			JMAX_val = (windowt_end - windowt_start) / 2 - 2;
			J_val = (windowt_end - windowt_start) / 2 - 2;
			suggest_phase = phase;
		}
	}

	if (window_size > PCAL_InitStruct->window_size) {
		PCAL_InitStruct->window_size = window_size;
		PCAL_InitStruct->CAL_JMAX = JMAX_val;
		PCAL_InitStruct->CAL_J = J_val;
		PCAL_InitStruct->CAL_N = N_val;
		PCAL_InitStruct->phase = suggest_phase;
	}

}

/**
  * @brief PSRAM FW calibration function
  * @retval None
  * @note cache will be disable during calibration
  */
BOOL PSRAM_calibration(PCAL_InitTypeDef *PCAL_InitStruct)
{
	PSPHY_TypeDef *psram_phy = PSRAMPHY_DEV;

	u32 caltempN = 0;
	u32 phase = 0;
	u32 tempPHYPara = psram_phy->PSPHY_CAL_PAR & (~PSPHY_MASK_CFG_CAL_N) & (~PSPHY_MASK_PRE_CAL_PHASE);
	u32 *tempdatawr = PSRAM_CALIB_PATTERN;
	u32 tempdatard[6] = {0, 0, 0, 0, 0, 0};
	int window_start = -1;
	int window_end = -1;
	int windowt_start = -1;
	int windowt_size = 0;
	int windowt_end = -1;
	u32 window_rank = 16;
	PCAL_InitStruct->window_size = 0;

	DCache_CleanInvalidate(0xFFFFFFFF, 0xFFFFFFFF);
	DCache_Disable();
	/*Disable HW calibration*/
	psram_phy->PSPHY_CAL_CTRL &= (~PSPHY_BIT_CFG_CAL_EN);
	for (phase = 0x1; phase <= 0x8; phase = phase * 2) {

		if (phase == 0x8) {
			window_rank = 32;
		}

		for (caltempN = 0; caltempN < window_rank; caltempN++) {
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

				if (windowt_start < 0) {
					windowt_start = caltempN;
				}
				windowt_end = windowt_start + windowt_size;
				windowt_size ++;

				if (windowt_end == 79) {
					window_start = windowt_start;
					window_end = windowt_end;
					PSRAM_calwindow(window_start, window_end, PCAL_InitStruct);
				}
//				DBG_8195A("windowt_start = %x windowt_end = %x windowt_size = %x \n", windowt_start, windowt_end, windowt_size);
			} else {
//				DBG_8195A("fail %x %x %x %x %x %x\n", tempdatard[0], tempdatard[1], tempdatard[2], tempdatard[3], tempdatard[4], tempdatard[5]);
				if (windowt_start >= 0) {
					window_start = windowt_start;
					window_end = windowt_end;
					PSRAM_calwindow(window_start, window_end, PCAL_InitStruct);

					windowt_start = -1;
					windowt_size = 0;
					windowt_end = -1;
				}
			}
		}
	}

	DBG_8195A("N = 0x%x J = 0x%x JMAX = 0x%x Phase = 0x%x window_size=%d\n", (PCAL_InitStruct->CAL_N), (PCAL_InitStruct->CAL_J), (PCAL_InitStruct->CAL_JMAX),
			  (PCAL_InitStruct->phase), (PCAL_InitStruct->window_size));

	DCache_Enable();

	if ((PCAL_InitStruct->window_size) < 9) {
		return _FALSE;
	}
	tempPHYPara &= (~0xfffff);
	tempPHYPara |= PSPHY_CFG_CAL_JMAX(PCAL_InitStruct->CAL_JMAX) | \
				   PSPHY_CFG_CAL_J(PCAL_InitStruct->CAL_J)	| \
				   PSPHY_CFG_CAL_N(PCAL_InitStruct->CAL_N);
	/*set N/J/PHASE initial value HW calibration*/
	psram_phy->PSPHY_CAL_PAR = tempPHYPara | PSPHY_PRE_CAL_PHASE(PCAL_InitStruct->phase);


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
void set_psram_sleep_mode(u32 type)
{
	u8 psram_halfsleep[2];
	u32 cur_src = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));

	PSRAM_AutoGating(DISABLE, 10, 3);

	// 50ns will be enough, check if need
	DelayUs(1);

	if (type == PSRAM_TYPE_APM) {
		psram_halfsleep[0] = 0xF0;
		psram_halfsleep[1] = 0xF0;

		PSRAM_REG_Write(0, 0x06, 2, psram_halfsleep);

	} else {
		psram_halfsleep[0] = 0xFF;
		psram_halfsleep[1] = 0xE1;

		PSRAM_REG_Write(1, 0x1, 2, psram_halfsleep);

	}

	if (cur_src == BIT_LSYS_CKSL_PSRAM_LBUS) {
		PSRAM_AutoGating(ENABLE, 10, 3);
	} else if (cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL || cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL) {
		PSRAM_AutoGating(ENABLE, 1, 16);
	}

}

/**
  * @brief  Wake PSRAM from half sleep mode.
  * @param  type: 0:apm psram type 1: winbond psram type
  * @retval None
  * @note None
  */
void set_psram_wakeup_mode(u32 type)
{
	u32 temp;
	u32 cur_src = LSYS_GET_CKSL_PSRAM(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0));

	PSRAM_AutoGating(DISABLE, 10, 3);

	DCache_CleanInvalidate(0x60000100, 32);
	temp = HAL_READ32(0x60000100, 0);

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
		PSRAM_AutoGating(ENABLE, 10, 3);
	} else if (cur_src == BIT_LSYS_CKSL_PSRAM_CPUPLL || cur_src == BIT_LSYS_CKSL_PSRAM_DSPPLL) {
		PSRAM_AutoGating(ENABLE, 1, 16);
	}

}

void set_psram_suspend_and_restore(u8 restore)
{
	RRAM_TypeDef *rram = RRAM_DEV;
	u32 Rtemp = 0;

	if (rram->PSRAM_TYPE == PSRAM_TYPE_APM || rram->PSRAM_TYPE == PSRAM_TYPE_WB) {

		if (restore) {
			/* psram re-enable */
			//open psram pad
			Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1);
			Rtemp |= PAD_BIT_DDR_PWDPADN_2REGU;
			Rtemp &= ~PAD_BIT_DDR_PD_REF;
			HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, Rtemp);

			//psram device exit self refresh
			set_psram_wakeup_mode(rram->PSRAM_TYPE);

		} else {
			/* psram shutdown */
			set_psram_sleep_mode(rram->PSRAM_TYPE);

			/* shutdown psram pad */
			Rtemp = HAL_READ32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1);
			Rtemp &= ~PAD_BIT_DDR_PWDPADN_2REGU;
			Rtemp |= PAD_BIT_DDR_PD_REF;
			HAL_WRITE32(PINMUX_REG_BASE, REG_DDR_PAD_CTRL1, Rtemp);
		}
	} else {
		/* no psram */
		return;
	}
}



/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/
