/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static const char *TAG = "LPCNP";
u32 NPSleepTick = 0;
u32 np_sleep_type;
u32 np_sleep_timeout = 0xffffffff;
u32 np_flash_dummy = 0;
u32 np_flash_phase_shift_idx = 0;
u32 np_flash_FLASH_rd_sample_phase_cal = 0;
u32 np_flash_FLASH_rd_sample_phase = 0;
//u32 mem_type = 0;

SRAM_ONLY_TEXT_SECTION
u32 FLASH_CalibrationNewCmd(u32 NewStatus)
{
	u32 temp = 0;
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;

	/* set PLL 600M phase shift enable */
	temp = PLL->PLL_PS;
	if (NewStatus == ENABLE) {
		temp |= PLL_BIT_EN_CK600M_PS;
	} else {
		temp &= ~PLL_BIT_EN_CK600M_PS;
	}
	PLL->PLL_PS = temp;

	/* set phase calibration enable */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);

	if (NewStatus == ENABLE) {
		temp |= HSYS_BIT_FLASH_CAL_EN;
	} else {
		temp &= ~HSYS_BIT_FLASH_CAL_EN;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	return _TRUE;
}

SRAM_ONLY_TEXT_SECTION
void FLASH_ClockSwitch(u32 Source, u32 Protection)
{
	/* To avoid gcc warnings */
	(void) Source;
	(void) Protection;
	u32 Temp = 0;
	u32 timeout = 20;
	SPIC_TypeDef *spi_flash = SPIC;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
	Temp &= (LSYS_MASK_CKSL_SPIC);

	if (Source == Temp) {
		return;
	}

	if (Protection) {
		asm volatile("cpsid i" : : : "memory");
		//asm volatile ("cpsid f" : : : "memory");
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	}

	if (Source == BIT_LSYS_CKSL_SPIC_XTAL) {
		/* 1. clock source switch to XTAL */
		Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Temp &= ~(LSYS_MASK_CKSL_SPIC);
		Temp |= LSYS_CKSL_SPIC(Source);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
		DelayUs(10);

		np_flash_dummy = GET_IN_PHYSICAL_CYC(spi_flash->AUTO_LENGTH);

		/* set phase calibration disable */
		FLASH_CalibrationNewCmd(DISABLE);

		/* user read need to switch to onebit mode */
		flash_init_para.phase_shift_idx = 0;
		flash_init_para.FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;

		spi_flash->AUTO_LENGTH &= (~MASK_IN_PHYSICAL_CYC);

	} else  if (Source == BIT_LSYS_CKSL_SPIC_PLL) {

		FLASH_CalibrationNewCmd(ENABLE);
		Temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
		Temp |= (HSYS_BIT_FLASH_PS_DIV_EN | HSYS_BIT_FLASH_DIV_EN); /* enable clock ps div & enable clock div*/
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, Temp);

		while (timeout > 0) {
			timeout--;
			Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_HSYS_SPIC_CTRL);
			if ((Temp & HSYS_BIT_FLASH_PS_DIV_RDY) && (Temp & HSYS_BIT_FLASH_DIV_RDY)) {
				break;
			}
		}

		/* 2. clock source switch */
		Temp =  HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Temp &= ~(LSYS_MASK_CKSL_SPIC);
		Temp |= LSYS_CKSL_SPIC(Source);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Temp);
		DelayUs(10);

		/* 3. SPIC Dummy to high speed dummy */
		Temp = spi_flash->AUTO_LENGTH;
		Temp &= (~MASK_IN_PHYSICAL_CYC);
		Temp |= IN_PHYSICAL_CYC(np_flash_dummy);
		spi_flash->AUTO_LENGTH = Temp;

		/* 4. refill flash para for user mode */
		flash_init_para.phase_shift_idx = np_flash_phase_shift_idx;
		flash_init_para.FLASH_rd_sample_phase = np_flash_FLASH_rd_sample_phase;
		flash_init_para.FLASH_rd_sample_phase_cal = np_flash_FLASH_rd_sample_phase_cal;

	}

	if (Protection) {
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
		asm volatile("cpsie i" : : : "memory");
		//asm volatile ("cpsie f" : : : "memory");
	}
}

void np_set_ddr_sre(void)
{
	DDRC_TypeDef *ddrc = DDRC_DEV;
	u32 temp;
	RTK_LOGI(TAG, "np_set_ddr_sre enter\n");

	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
	temp &= (~(HSYS_BIT_PWDPAD_DQ_EN | HSYS_BIT_PI_PWROFF_EN)); //don't write 1 if user mode
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, temp);
	DelayUs(1);

	ddrc->DDRC_IOCR &= (~DDRC_BIT_DYN_SRE);

	/* update IOCR reg */
	ddrc->DDRC_CCR = DDRC_BIT_CR_UPDATE;

	/*disable refresh function*/
	//ddrc->DDRC_DRR |= DDRC_REF_DIS(ENABLE);
	ddrc->DDRC_CSR = (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE);

	// polling idle status, for all cmds and data done in ddrc
	while (((ddrc->DDRC_CSR) & (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE)) != (DDRC_BIT_BSTC_IDLE | DDRC_BIT_TM_IDLE | DDRC_BIT_MEM_IDLE));

	/* set deselect better */
	temp = ddrc->DDRC_CSR;
	temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
	temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_PRECHARGE_ALL));
	ddrc->DDRC_CSR = temp;

	ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SHIGH) | DDRC_RAS_N_NDGE(DDR_SLOW) |
						   DDRC_CAS_N_NDGE(DDR_SHIGH) |  DDRC_WE_N_NDGE(DDR_SLOW) | \
						   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));

	ddrc->DDRC_TIE_DPIN |= DDRC_CKE(DDR_SHIGH);

	//disable SRE here for idle state may take some time, and should before enter Dpin mode
	ddrc->DDRC_DRR |= DDRC_REF_DIS(ENABLE);
	ddrc->DDRC_CCR = DDRC_BIT_CR_UPDATE;

	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);

	/*precharge all*/
	temp = ddrc->DDRC_CSR;
	temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
	temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_PRECHARGE_ALL));
	ddrc->DDRC_CSR = temp;

	ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
						   DDRC_CAS_N_NDGE(DDR_SHIGH) |  DDRC_WE_N_NDGE(DDR_SLOW) | \
						   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
	ddrc->DDRC_TIE_DPIN = (DDRC_ODT(DDR_SLOW) | DDRC_CKE(DDR_SHIGH) | DDRC_RST_N(DDR_SHIGH));

	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);

	/* refresh */
	temp = ddrc->DDRC_CSR;
	temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
	temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_RSVD));
	ddrc->DDRC_CSR = temp;

	ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
						   DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
						   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
	ddrc->DDRC_TIE_DPIN = (DDRC_ODT(DDR_SLOW) | DDRC_CKE(DDR_SHIGH) | DDRC_RST_N(DDR_SHIGH));

	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);

	/*self-refrest enter*/
	temp = ddrc->DDRC_CSR;
	temp &= ~(DDRC_MASK_DPIN_MOD | DDRC_MASK_DPIN_CMD_INFO);
	temp |= (DDRC_DPIN_MOD(DDR_DPIN_CMD) | DDRC_DPIN_CMD_INFO(DDR_DPIN_CMD_RSVD));
	ddrc->DDRC_CSR = temp;

	ddrc->DDRC_CMD_DPIN = (DDRC_CS_N_NDGE(DDR_SLOW) | DDRC_RAS_N_NDGE(DDR_SLOW) |
						   DDRC_CAS_N_NDGE(DDR_SLOW) |  DDRC_WE_N_NDGE(DDR_SHIGH) | \
						   DDRC_BA_NDGE(DDR_SLOW) |  DDRC_ADDR_NDGE(DDR_SLOW));
	ddrc->DDRC_TIE_DPIN = (DDRC_ODT(DDR_SLOW) | DDRC_CKE(DDR_SLOW) | DDRC_RST_N(DDR_SHIGH));

	ddrc->DDRC_CCR = DDRC_DPIT(ENABLE);
	while ((ddrc->DDRC_CCR & DDRC_BIT_DPIT) != DDRC_BIT_DPIT);
	RTK_LOGI(TAG, "np_set_ddr_sre exit\n");
}

void np_set_ddrphy_pll_off(void)
{
	DDRPHY_TypeDef *ddr_phy = DDRPHY_DEV;

	ddr_phy->DDRPHY_PLL_CTL0 &= (~DDRPHY_MASK_DPI_MCK_CLK_EN);
	ddr_phy->DDRPHY_PLL_CTL1 &= (~DDRPHY_MASK_DPI_CLK_OE);
	ddr_phy->DDRPHY_CRT_RST_CTL &= (~DDRPHY_BIT_PLL_SSC_DIG_RST_N);
	ddr_phy->DDRPHY_SSC0 &= (~DDRPHY_BIT_DPI_SSC_FLAG_INIT);
	ddr_phy->DDRPHY_CRT_RST_CTL &= (~DDRPHY_BIT_PLL_LDO_RST_N);
}

void np_set_psram_cmd(u8 cmd, u32 addr, u32 write_len, u8 *write_data)
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
	//waiting update typedef
	psram_ctrl->RX_NDF = 0;

	//waiting update typedef
	psram_ctrl->TX_NDF = TX_NDF(write_len);

	//need modify
	/* set flash_cmd: write cmd to fifo */
	psram_ctrl->DR[0].BYTE = cmd;
	psram_ctrl->DR[0].BYTE = cmd;

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

u8 APM_WR_INIT_LATENCY_SPEC[6] = {
	APM_WR_INIT_LATENCY_3CLK,
	APM_WR_INIT_LATENCY_4CLK,
	APM_WR_INIT_LATENCY_5CLK,
	APM_WR_INIT_LATENCY_6CLK,
	APM_WR_INIT_LATENCY_7CLK,
	APM_WR_INIT_LATENCY_8CLK,
};

_OPTIMIZE_NONE_
void np_set_psram_sleep_mode(u32 State)
{
	u32 Rtemp;
	u8 mr4[2];

	if (State) {
		// close auto gating
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
		Rtemp &= (~HSYS_BIT_PWDPAD_DQ_EN); //don't write 1 if user mode
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, Rtemp);

		// 50ns will be enough, check if need when without DBG
		DelayUs(1);
		//RTK_LOGD(TAG, "psram enter half sleep mode\r\n");

		mr4[0] = (APM_WR_INIT_LATENCY_SPEC[RRAM->PSRAM_LATENCY - 3]) << 5 | PSRAM_SLOW_REFRSH_ENABLE;
		mr4[1] = mr4[0];
		np_set_psram_cmd(0xc0, 0x4, 2, mr4);

		//u8 psram_halfsleep[2] = {0xC0, 0xC0};	// deep power down
		u8 psram_halfsleep[2] = {0xF0, 0xF0};

		np_set_psram_cmd(0xc0, 0x06, 2, psram_halfsleep);
	} else {
		DCache_Invalidate(0x60000000, 4);
		Rtemp = HAL_READ32(0x60000000, 0);
		//RTK_LOGD(TAG, "temp: %x\r\n", Rtemp);
		/* wait self refresh exit */
		DelayUs(150);

		/* disable slow refresh */
		mr4[0] = (APM_WR_INIT_LATENCY_SPEC[RRAM->PSRAM_LATENCY - 3]) << 5;
		mr4[1] = mr4[0];
		np_set_psram_cmd(0xc0, 0x4, 2, mr4);

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E4);
		Rtemp &= ~(HSYS_MASK_PWDPAD_RESUME_VAL | HSYS_MASK_PWDPAD_IDLE_VAL);

		Rtemp |= HSYS_PWDPAD_RESUME_VAL(0x10);
		Rtemp |= HSYS_PWDPAD_IDLE_VAL(1);
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E4, Rtemp);

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0);
		Rtemp |= HSYS_BIT_PWDPAD_DQ_EN; //don't write 1 if user mode
		HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_DUMMY_1E0, Rtemp);

	}
}

void np_ddr_reinit(void)
{
	u32 Rtemp;

	/* only in M0 wake M4 from clk gating, set LSYS_BIT_BOOT_WAKE_FROM_PS_HS for ddr reinit flow */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
	Rtemp |= LSYS_BIT_BOOT_WAKE_FROM_PS_HS;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);

	/*DDRPHY init*/
	DDRPHY_InitTypeDef DDRPHY_InitStruct;
	DDR_PHY_StructInit(&DDRPHY_InitStruct);
	DDR_PHY_Init(&DDRPHY_InitStruct);

	/*DDRC init*/
	ddr_init();

	/*DDRPHY Calibration Offline*/
	DDR_PHY_CAL();

#if DDR_AUTOGATING
	DDR_PHY_AutoGating();
	rxi316_DynSre_init(0x1FF, ENABLE);
#endif
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
	Rtemp &= ~LSYS_BIT_BOOT_WAKE_FROM_PS_HS;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);

}

SRAM_ONLY_TEXT_SECTION
void np_power_gate_ctrl(void)
{
	u32 Rtemp;
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	RRAM_TypeDef *rram = RRAM;

	/* for test only */
	//Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_CTRL);
	//Rtemp |= HSYS_BIT_SHARE_BT_MEM | HSYS_BIT_SHARE_WL_MEM;
	//HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_CTRL, Rtemp);

	if (rram->MEM_TYPE == Memory_Type_DDR) {
		np_set_ddr_sre();

		/* MP ECO, shutdown pad, immedately after SRE */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP,
					REG_LSYS_DUMMY_098)  & (~(LSYS_BIT_PWDPAD15N_DQ | LSYS_BIT_PWDPAD15N_CA))));

		/* gating clk first */
		RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_DDRC_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_DDRP_CLOCK, DISABLE);

		/* off phy pll */
		np_set_ddrphy_pll_off();

		RCC_PeriphClockCmd(APBPeriph_DDRC, APBPeriph_DDRC_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_DDRP, APBPeriph_DDRP_CLOCK, DISABLE);

		/* switch to PFM mode only ddr need */
		//switch swr_mem to PFM mode
		if (SWR_MEM_Mode_Set(SWR_PFM)) {
			RTK_LOGW(TAG, "MEM PFM fail\r\n");
		}

	} else if (rram->MEM_TYPE == Memory_Type_PSRAM) {

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);

		if ((Rtemp & (LSYS_BIT_BG_ON_MIPI | LSYS_BIT_BG_ON_USB2)) == 0) {
			/* off ddrphy BG for psram chip, open by USB AND MIPI when need */
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1) & (~LSYS_BIT_BG_PWR));
		}

		/* MP ECO */
		np_set_psram_sleep_mode(ENABLE);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  & (~LSYS_BIT_PWDPAD15N_DQ)));
	} else {
		/* MP ECO */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  & (~LSYS_BIT_PWDPAD15N_DQ)));

		/* to do */
	}

	RCC_PeriphClockCmd(APBPeriph_PSRAM, APBPeriph_PSRAM_CLOCK, DISABLE);
	RCC_PeriphClockCmd(APBPeriph_THM, APBPeriph_THM_CLOCK, DISABLE);
	RCC_PeriphClockCmd(APBPeriph_TRNG, APBPeriph_TRNG_CLOCK, DISABLE);

	if (ps_config.km0_pll_off == TRUE) {
		/*switch HIPC clk to  to LPON clk*/
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= (LSYS_BIT_CKSL_HIPC);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
			/* 1. clock source switch to XTAL */
			Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
			Rtemp &= ~(LSYS_MASK_CKSL_SPIC);
			Rtemp |= LSYS_CKSL_SPIC(BIT_LSYS_CKSL_SPIC_XTAL);
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
			DelayUs(10);
		} else {
			/* 0. backup flash para */
			np_flash_phase_shift_idx = flash_init_para.phase_shift_idx;
			np_flash_FLASH_rd_sample_phase_cal = flash_init_para.FLASH_rd_sample_phase_cal;
			np_flash_FLASH_rd_sample_phase = flash_init_para.FLASH_rd_sample_phase;

			/* switch clock to XTAL, disable dummy cycle and diable cal */
			FLASH_ClockSwitch(BIT_LSYS_CKSL_SPIC_XTAL, 1);

		}
	}

	if (ps_config.km0_audio_vad_on == TRUE) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= LSYS_BIT_CKSL_VADM;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
	}
	/* Disable KM4/HPlatform clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
	Rtemp &= (~(APBPeriph_NP_CLOCK | APBPeriph_HPLFM_CLOCK | APBPeriph_HPON_CLOCK));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);

	/* Disable KM4/HPlatform function */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
	Rtemp &= (~(APBPeriph_NP | APBPeriph_HPLFM));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0, Rtemp);

	if (ps_config.km0_pll_off == TRUE) {
		/* if NP PLL/ AP PLL/ADC  all power off, need to close BandGap */
		PLL->PLL_NPPLL_CTRL0 &= ~(PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);
		PLL->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_PLL;
		PLL->PLL_PS &= (~(PLL_BIT_EN_CK600M | PLL_BIT_EN_CK600M_PS));
		PLL->PLL_PS &= ~PLL_BIT_POW_CKGEN;

		PLL->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_ERC;
		PLL->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
		PLL->PLL_AUX_BG |= PLL_BIT_POW_I;
	}

	if (ps_config.km0_audio_vad_on == TRUE) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= LSYS_BIT_CKSL_VADM;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	/* close ddyphy & USB & MIPI BG */
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, 0);

	/* Reset HP Platform */
	Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR);
	Rtemp &= (~(LSYS_BIT_POR_HP_PLAT | LSYS_BIT_POR_HP_DDRPHY));
	HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR, Rtemp);

	/* Enable isolation of HP_SRAM & HP Platform & NP */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO);
	Rtemp |= (LSYS_BIT_ISO_HP_PLAT | LSYS_BIT_ISO_HP_DDRPHY);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO, Rtemp);

	/* Disable HP Platform */
	Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC);
	Rtemp &= (~(LSYS_BIT_PWC_HP_PLAT | LSYS_BIT_PWC_HP_DDRPHY));
	HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC, Rtemp);

	if (RRAM->VOL_TYPE == VOL_10) {
		/* step 1: first back to normal mode 0.9v, need step by step */
		SWR_BST_MODE_Set(DISABLE);
	}

	/* step 2: sys req pfm mode when only km0 */
	SWR_PFM_MODE_Set(ENABLE);
}

SRAM_ONLY_TEXT_SECTION
void np_power_wake_ctrl(void)
{
	u32 Rtemp;
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	RRAM_TypeDef *rram = RRAM;

	/* sys req pwm mode before power on km4 */
	SWR_PFM_MODE_Set(DISABLE);

	if (RRAM->VOL_TYPE == VOL_10) {
		/* step 1: first back to burst mode 1.0v, then enable pll to high speed directly */
		SWR_BST_MODE_Set(ENABLE);
	}
	if (rram->MEM_TYPE == Memory_Type_PSRAM) {

		// enable BG here to save time
		if (ps_config.km0_pll_off == TRUE) {
			//Check BandGap power on
			if ((PLL->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
				PLL->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
				DelayUs(20);
				PLL->PLL_AUX_BG &= (~PLL_BIT_POW_I);
				DelayUs(40);
			}
		}

		/* Enable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC);
		Rtemp |= (LSYS_BIT_PWC_HP_PLAT);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC, Rtemp);

		/* for power stable */
		DelayUs(100);
		/* Disable isolation of HP Platform & NP */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO);
		Rtemp &= (~(LSYS_BIT_ISO_HP_PLAT));
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO, Rtemp);

		/* Enable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR);
		Rtemp |= (LSYS_BIT_POR_HP_PLAT);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR, Rtemp);
	} else {
		if (ps_config.km0_pll_off == TRUE) {
			//Check BandGap power on
			if ((PLL->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
				PLL->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
				DelayUs(20);
				PLL->PLL_AUX_BG &= (~PLL_BIT_POW_I);
				DelayUs(40);
			}
		}

		//switch swr_mem to PWM mode
		if (SWR_MEM_Mode_Set(SWR_PWM)) {
			RTK_LOGW(TAG, "MEM PWM fail\r\n");
		}

		/* Enable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC);
		Rtemp |= (LSYS_BIT_PWC_HP_PLAT | LSYS_BIT_PWC_HP_DDRPHY);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC, Rtemp);

		/* for power stable */
		DelayUs(100);
		/* Disable isolation of HP Platform & NP */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO);
		Rtemp &= (~(LSYS_BIT_ISO_HP_PLAT | LSYS_BIT_ISO_HP_DDRPHY));
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO, Rtemp);

		/* Enable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR);
		Rtemp |= (LSYS_BIT_POR_HP_PLAT | LSYS_BIT_POR_HP_DDRPHY);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR, Rtemp);
	}

	/* Enable KM4/HPlatform function */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0);
	Rtemp |= (APBPeriph_NP | APBPeriph_HPLFM);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_FEN_GRP0, Rtemp);

	/* Enable HSOC/HPlatform clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
	Rtemp |= (APBPeriph_HPLFM_CLOCK | APBPeriph_HPON_CLOCK);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);

	if (ps_config.km0_pll_off == TRUE) {

//together with enable hplat, save about 100us+
#if 0
		//Check BandGap power on
		if ((PLL->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			PLL->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			PLL->PLL_AUX_BG &= (~PLL_BIT_POW_I);
			DelayUs(140);
		}
#endif
		// erc enable
		PLL->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_ERC;
		DelayUs(1);

		// PLL power on
		PLL->PLL_PS |= PLL_BIT_EN_CK600M | PLL_BIT_EN_CK600M_PS;
		PLL->PLL_PS |= PLL_BIT_POW_CKGEN;
		PLL->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_PLL;
		PLL->PLL_NPPLL_CTRL0 |= (PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);

		// Wait ready
		while (!(PLL->PLL_STATE & PLL_BIT_CKRDY_NP));

		/* switch HIPC clk to  to HAPB clk, or ipc isr can't be cleared */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp &= (~LSYS_BIT_CKSL_HIPC);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
			/* 2. clock source switch */
			Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
			Rtemp &= ~(LSYS_MASK_CKSL_SPIC);
			Rtemp |= LSYS_CKSL_SPIC(BIT_LSYS_CKSL_SPIC_PLL);
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
			DelayUs(10);
		} else {
			// keep in Xtal mode
			/* 2. clock source switch, enable dummy cycle and cal */
			//FLASH_ClockSwitch(BIT_LSYS_CKSL_SPIC_PLL, 1);
		}
	}

	if (ps_config.km0_audio_vad_on == TRUE) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp &= (~LSYS_BIT_CKSL_VADM);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	/* Enable KM4 clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
	Rtemp |= (APBPeriph_NP_CLOCK);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);
}

void np_clk_gate_ctrl(void)
{
	u32 Rtemp;
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	RRAM_TypeDef *rram = RRAM;

	if (rram->MEM_TYPE == Memory_Type_DDR) {

		np_set_ddr_sre();

		/* gating clk first */
		RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_DDRC_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_DDRP_CLOCK, DISABLE);

		/* off phy pll */
		np_set_ddrphy_pll_off();

		RCC_PeriphClockCmd(APBPeriph_DDRC, APBPeriph_DDRC_CLOCK, DISABLE);
		RCC_PeriphClockCmd(APBPeriph_DDRP, APBPeriph_DDRP_CLOCK, DISABLE);

		/* MP ECO, shutdown pad */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP,
					REG_LSYS_DUMMY_098) & (~(LSYS_BIT_PWDPAD15N_DQ | LSYS_BIT_PWDPAD15N_CA))));


		/* close ddyphy & USB & MIPI BG */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, 0);

		/* Reset HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR);
		Rtemp &= (~(LSYS_BIT_POR_HP_DDRPHY));
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR, Rtemp);

		/* Enable isolation of HP_SRAM & HP Platform & NP */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO);
		Rtemp |= (LSYS_BIT_ISO_HP_DDRPHY);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO, Rtemp);

		/* Disable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC);
		Rtemp &= (~(LSYS_BIT_PWC_HP_DDRPHY));
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC, Rtemp);


		/* switch to PFM mode only ddr need */
		//switch swr_mem to PFM mode
		if (SWR_MEM_Mode_Set(SWR_PFM)) {
			RTK_LOGW(TAG, "MEM PFM fail\r\n");
		}

	} else if (rram->MEM_TYPE == Memory_Type_PSRAM) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1);

		if ((Rtemp & (LSYS_BIT_BG_ON_MIPI | LSYS_BIT_BG_ON_USB2)) == 0) {
			/* off ddrphy BG for psram chip, open by USB AND MIPI when need */
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1, HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_AIP_CTRL1) & (~LSYS_BIT_BG_PWR));
		}
		/* MP ECO */
		if (ps_config.km0_config_psram) {
			np_set_psram_sleep_mode(ENABLE);
		}
		RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_PSRAM_CLOCK, DISABLE);

		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  & (~LSYS_BIT_PWDPAD15N_DQ)));
	} else {
		/* MP ECO */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  & (~LSYS_BIT_PWDPAD15N_DQ)));

		/* to do */
	}


	if (ps_config.km0_pll_off == TRUE) {
		/*switch HIPC clk to  to LPON clk*/
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= (LSYS_BIT_CKSL_HIPC);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		/* switch hplatform and np(for sram) clk to xtal */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp &= ~(LSYS_BIT_CKSL_NP);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		if (SYSCFG_CHIPType_Get() == CHIP_TYPE_FPGA) {
			/* 1. clock source switch to XTAL */
			Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
			Rtemp &= ~(LSYS_MASK_CKSL_SPIC);
			Rtemp |= LSYS_CKSL_SPIC(BIT_LSYS_CKSL_SPIC_XTAL);
			HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
			DelayUs(10);
		} else {
			/* 0. backup flash para */
			np_flash_phase_shift_idx = flash_init_para.phase_shift_idx;
			np_flash_FLASH_rd_sample_phase_cal = flash_init_para.FLASH_rd_sample_phase_cal;
			np_flash_FLASH_rd_sample_phase = flash_init_para.FLASH_rd_sample_phase;

			/* switch clock to XTAL, disable dummy cycle and diable cal */
			FLASH_ClockSwitch(BIT_LSYS_CKSL_SPIC_XTAL, 1);
		}
	}

	/* Disable KM4/HPlatform clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
#if defined(CONFIG_CLINTWOOD ) && CONFIG_CLINTWOOD
	Rtemp &= (~(APBPeriph_NP_CLOCK));
#else
	Rtemp &= (~(APBPeriph_NP_CLOCK | APBPeriph_HPLFM_CLOCK));
#endif
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);

	if (ps_config.km0_audio_vad_on == TRUE) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= LSYS_BIT_CKSL_VADM;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	if (ps_config.km0_pll_off == TRUE) {
		/* if NP PLL/ AP PLL/ADC  all power off, need to close BandGap */
		PLL->PLL_NPPLL_CTRL0 &= ~(PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);
		PLL->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_PLL;
		PLL->PLL_PS &= (~(PLL_BIT_EN_CK600M | PLL_BIT_EN_CK600M_PS));
		PLL->PLL_PS &= ~PLL_BIT_POW_CKGEN;

		PLL->PLL_NPPLL_CTRL0 &= ~PLL_BIT_NPLL_POW_ERC;
		PLL->PLL_AUX_BG &= ~(PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
		PLL->PLL_AUX_BG |= PLL_BIT_POW_I;
	}

	if (RRAM->VOL_TYPE == VOL_10) {
		/* step 1: first back to normal mode 0.9v, need step by step */
		SWR_BST_MODE_Set(DISABLE);
	}

#ifndef CONFIG_CLINTWOOD
	/* step 2: sys req pfm mode when only km0 */
	SWR_PFM_MODE_Set(ENABLE);
#endif
}

void np_clk_wake_ctrl(void)
{
	u32 Rtemp;

	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	RRAM_TypeDef *rram = RRAM;

	/* sys req pwm mode before power on km4 */
	SWR_PFM_MODE_Set(DISABLE);

	if (RRAM->VOL_TYPE == VOL_10) {
		/* step 1: first back to burst mode 1.0v, then enable pll to high speed directly */
		SWR_BST_MODE_Set(ENABLE);
	}

	/* Enable HPlatform clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
	Rtemp |= (APBPeriph_HPLFM_CLOCK);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);

	if (ps_config.km0_pll_off == TRUE) {
		//Check BandGap power on
		if ((PLL->PLL_AUX_BG & PLL_BG_POW_MASK) == 0) {
			PLL->PLL_AUX_BG |= (PLL_BIT_POW_BG | PLL_BIT_POW_MBIAS);
			DelayUs(20);
			PLL->PLL_AUX_BG &= (~PLL_BIT_POW_I);
			DelayUs(140);
		}

		// erc enable
		PLL->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_ERC;
		DelayUs(1);

		// PLL power on
		PLL->PLL_PS |= PLL_BIT_EN_CK600M | PLL_BIT_EN_CK600M_PS;
		PLL->PLL_PS |= PLL_BIT_POW_CKGEN;
		PLL->PLL_NPPLL_CTRL0 |= PLL_BIT_NPLL_POW_PLL;
		PLL->PLL_NPPLL_CTRL0 |= (PLL_BIT_NPLL_CK_EN | PLL_BIT_NPLL_CK_EN_D4);

		// Wait ready
		while (!(PLL->PLL_STATE & PLL_BIT_CKRDY_NP));

		/* switch HIPC clk to  to HAPB clk, or ipc isr can't be cleared */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp &= (~LSYS_BIT_CKSL_HIPC);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp |= (LSYS_BIT_CKSL_NP);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);

		/* 2. just keep in xtal mode */
		//FLASH_ClockSwitch(BIT_LSYS_CKSL_SPIC_PLL, 1);
	}

	if (ps_config.km0_audio_vad_on == TRUE) {
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0);
		Rtemp &= (~LSYS_BIT_CKSL_VADM);
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKSL_GRP0, Rtemp);
	}

	if (rram->MEM_TYPE == Memory_Type_PSRAM) {
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  | (LSYS_BIT_PWDPAD15N_DQ)));
		/* MP ECO */
		RCC_PeriphClockCmd(APBPeriph_PSRAM, APBPeriph_PSRAM_CLOCK, ENABLE);

		if (ps_config.km0_config_psram) {
			np_set_psram_sleep_mode(DISABLE);
		}

	} else if (rram->MEM_TYPE == Memory_Type_DDR) {
		//switch swr_mem to PWM mode
		if (SWR_MEM_Mode_Set(SWR_PWM)) {
			RTK_LOGW(TAG, "MEM PWM fail\r\n");
		}

		/* Enable DDRPHY PWC */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC);
		Rtemp |= (LSYS_BIT_PWC_HP_DDRPHY);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_PWC, Rtemp);

		/* for power stable */
		DelayUs(100);
		/* Disable isolation of HP Platform & NP */
		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO);
		Rtemp &= (~(LSYS_BIT_ISO_HP_DDRPHY));
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_ISO, Rtemp);

		/* Enable HP Platform */
		Rtemp = HAL_READ16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR);
		Rtemp |= (LSYS_BIT_POR_HP_DDRPHY);
		HAL_WRITE16(SYSTEM_CTRL_BASE_LP, REG_LSYS_POR, Rtemp);

		/* MP ECO, shutdown pad */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP,
					REG_LSYS_DUMMY_098)  | (LSYS_BIT_PWDPAD15N_DQ | LSYS_BIT_PWDPAD15N_CA)));

		/* enable func & clk first */
		RCC_PeriphClockCmd(APBPeriph_DDRP, APBPeriph_DDRP_CLOCK, ENABLE);
		RCC_PeriphClockCmd(APBPeriph_DDRC, APBPeriph_DDRC_CLOCK, ENABLE);

		np_ddr_reinit();

	} else {
		/* MP ECO */
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_DUMMY_098)  & (~LSYS_BIT_PWDPAD15N_DQ)));

		/* to do */
	}

	/* Enable KM4/HPlatform clock */
	Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);
	Rtemp |= (APBPeriph_NP_CLOCK);
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0, Rtemp);

}

u32 np_status_on(void)
{
	u32 Temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKE_GRP0);

	if (Temp & APBPeriph_NP_CLOCK) {
		return 1;
	} else {
		return 0;
	}
}
void np_power_gate(void)
{
	u32 temp = 0;

	if (!np_status_on()) {
		return;
	}

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "M4G\n");
	}
	/* poll KM4 clock gate */
	while (1) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_STATUS);	/*get KM4 sleep status*/
		if (temp & HSYS_BIT_KM4_SL_SLEEPSYS_R) {
			break;
		}
	}

	np_power_gate_ctrl();

	pmu_release_wakelock(PMU_KM4_RUN);
	pmu_release_wakelock(PMU_OS);

}

void np_power_wake(void)
{
	if (np_status_on()) {
		return;
	}
	u32 i = 0;

	/* move here, or KM4 may boot fail when mem in DS mode(when PG + PG)*/
	/* hs sram option init */
	for (i = 0;;) {
		/*  Check if search to end */
		if (sleep_sram_config[i].Module == 0xFFFFFFFF) {
			break;
		}

		SOCPS_ResMemMode(sleep_sram_config[i].Module);

		i++;
	}

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "M4W\n");
	}
	pmu_acquire_wakelock(PMU_KM4_RUN);

	np_power_wake_ctrl();
}

void np_clock_gate(void)
{
	u32 temp = 0;

	if (!np_status_on()) {
		return;
	}

	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "M4CG\n");
	}
	/* poll KM4 clock gate */
	while (1) {
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_STATUS);	/*get KM4 sleep status*/
		if (temp & HSYS_BIT_KM4_SL_SLEEPSYS_R) {
			break;
		}
	}

	np_clk_gate_ctrl();


	if (ps_config.km0_tickles_debug) {
		RTK_LOGD(TAG, "M4CG-\n");
	}

	pmu_release_wakelock(PMU_KM4_RUN);
	if (ps_config.km0_sleep_withM4) {
		pmu_release_wakelock(PMU_OS);
	}

}

void np_clock_on(void)
{
	if (ps_config.km0_sleep_withM4) {
		u32 i = 0;

		/* hs sram option init */
		for (i = 0;;) {
			/*  Check if search to end */
			if (sleep_sram_config[i].Module == 0xFFFFFFFF) {
				break;
			}

			SOCPS_ResMemMode(sleep_sram_config[i].Module);

			i++;
		}
	}

	pmu_acquire_wakelock(PMU_KM4_RUN);

	np_clk_wake_ctrl();

	/* tell KM4 wake */
	asm volatile("sev");
	RTK_LOGI(TAG, "M4CW-\n");
}

u32 ap_clk_status_on(void)
{

	if (HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_CKE) & HSYS_BIT_CKE_AP) {
		return 1;
	} else {
		return 0;
	}
}

u32 ap_status_on(void)
{
	if ((HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HP_PWC) & (HSYS_BIT_PSW_HP_AP_L2))) {
		return 1;
	} else {
		return 0;
	}
}

u32 np_suspend(u32 type)
{
	u32 Rtemp;
	u32 ret = _SUCCESS;

	HAL_WRITE8(SYSTEM_CTRL_BASE_LP, REG_LSYS_NP_STATUS_SW,
			   HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_NP_STATUS_SW) & (~LSYS_BIT_NP_RUNNING));

	if (type == SLEEP_CG) {
		np_clock_gate();
	} else {
		np_power_gate();

		Rtemp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG);
		Rtemp |= LSYS_BIT_BOOT_WAKE_FROM_PS_HS;
		HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_BOOT_CFG, Rtemp);
	}

	/*clean np wake pending interrupt, clear before Enable*/
	NVIC_ClearPendingIRQ(NP_WAKE_IRQ);
	InterruptEn(NP_WAKE_IRQ, 10);

#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	NVIC_ClearPendingIRQ(WL_PROTOCOL_IRQ);
	irq_enable(WL_PROTOCOL_IRQ);
#endif

	return ret;
}

void np_resume(void)
{
	if (np_status_on()) {
		return;
	}
#if defined (CONFIG_CLINTWOOD) && CONFIG_CLINTWOOD
	irq_disable(WL_PROTOCOL_IRQ);
#endif
	pmu_acquire_wakelock(PMU_KM4_RUN);
	pmu_acquire_deepwakelock(PMU_KM4_RUN);

	if (np_sleep_type == SLEEP_CG) {
		np_clock_on();
	} else {
		np_power_wake();
	}

	/*set wake up status*/
	//HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HS_WAKE_EVENT_STATUS1, km4_wake_event);
}

void np_tickless_ipc_int(UNUSED_WARN_DIS VOID *Data, UNUSED_WARN_DIS u32 IrqStatus, UNUSED_WARN_DIS u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);
	int temp = 0;
	SLEEP_ParamDef *psleep_param;
	DBG_INFO_MSG_ON(MODULE_KM4);

	psleep_param = (SLEEP_ParamDef *)ipc_get_message(IPC_NP_TO_LP, IPC_N2L_TICKLESS_INDICATION);
	DCache_Invalidate((u32)psleep_param, sizeof(SLEEP_ParamDef));

	if (psleep_param->dlps_enable) {
		pmu_release_deepwakelock(PMU_KM4_RUN);
		pmu_release_deepwakelock(PMU_OS);
	}

	//set dlps
	if (freertos_ready_to_dsleep()) {
		RCC_PeriphClockCmd(APBPeriph_ATIM, APBPeriph_ATIM_CLOCK, ENABLE);
		if (psleep_param->sleep_time) {
			SOCPS_AONTimerClearINT();
			SOCPS_AONTimer(psleep_param->sleep_time);
			SOCPS_AONTimerINT_EN(ENABLE);
		}
		SWR_MEM(DISABLE);
		SWR_AUDIO(DISABLE);
		SOCPS_DeepSleep_RAM();
	}

	np_sleep_type = psleep_param->sleep_type;

	if (np_sleep_type == SLEEP_PG) {
		if (ap_status_on() || (HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) & LSYS_BIT_AP_RUNNING)) {
			/* poll KM4 clock gate */
			while (1) {
				temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_STATUS);	/*get KM4 sleep status*/
				if (temp & HSYS_BIT_KM4_SL_SLEEPSYS_R) {
					break;
				}
			}
			asm volatile("sev");
			return;
		}
	} else {
		if (ap_clk_status_on() || (HAL_READ8(SYSTEM_CTRL_BASE_LP, REG_LSYS_AP_STATUS_SW) & LSYS_BIT_AP_RUNNING)) {
			/* poll KM4 clock gate */
			while (1) {
				temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_HPLAT_STATUS);	/*get KM4 sleep status*/
				if (temp & HSYS_BIT_KM4_SL_SLEEPSYS_R) {
					break;
				}
			}
			asm volatile("sev");
			return;
		}
	}

#if defined(CONFIG_CLINTWOOD ) && CONFIG_CLINTWOOD
	/* pfm req immediately when KM4 enter wfe */
	SWR_PFM_MODE_Set(ENABLE);
#endif

	switch (psleep_param->sleep_type) {
	case SLEEP_PG:
		if (_SUCCESS == np_suspend(SLEEP_PG)) {
			pmu_set_sleep_type(SLEEP_PG);
		}
		break;
	case SLEEP_CG:
		if (_SUCCESS == np_suspend(SLEEP_CG)) {
			pmu_set_sleep_type(SLEEP_CG);
			//pmu_set_sysactive_time(2);
		}
		break;

	default:
		RTK_LOGW(TAG, "unknow sleep type\n");
	}
	NPSleepTick = SYSTIMER_TickGet();
	//RTK_LOGD(TAG, ("T:%d, tms:%d\r\n",KM4SleepTick,(((KM4SleepTick & 0xFFFF8000)/32768) * 1000 + ((KM4SleepTick & 0x7FFF) * 1000) /32768));
}

/**
  * @brief  Get km4 sleep time.
  * @param  null.
  * @retval km4 sleep time in ms.
  */
uint32_t pmu_get_npsleeptime(void)
{
	u32 current_tick = 0, km4sleeptime = 0;

	current_tick = SYSTIMER_TickGet();

	if (current_tick >= NPSleepTick) {
		km4sleeptime = current_tick - NPSleepTick;
	} else {
		km4sleeptime = (0xFFFFFFFF - NPSleepTick) + current_tick;
	}

	km4sleeptime = ((km4sleeptime & 0xFFFF8000) / 32768) * 1000 + ((km4sleeptime & 0x7FFF) * 1000) / 32768;

	return km4sleeptime;
}

IPC_TABLE_DATA_SECTION

const IPC_INIT_TABLE ipc_nptickless_table = {
	.USER_MSG_TYPE = IPC_USER_DATA,
	.Rxfunc = np_tickless_ipc_int,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = IPC_TXHandler,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_NP_TO_LP,
	.IPC_Channel = IPC_N2L_TICKLESS_INDICATION
};
