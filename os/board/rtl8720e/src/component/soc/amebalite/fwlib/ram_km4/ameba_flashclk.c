/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

#define FLASH_CALIBRATION_DEBUG		0

static FlashInfo_TypeDef *current_IC;

/* Flag to check configuration register or not. Necessary for wide-range VCC MXIC flash */
static u8 check_config_reg = 0;

extern FlashInfo_TypeDef Flash_AVL[];
extern u16 Flash_ReadMode;
extern u16 Flash_Speed;

/* When OTF enabled, FLASH_CalibrationNew in ROM will Fail because Cache In RSIP is not Clean */
BOOT_RAM_TEXT_SECTION
u32 FLASH_CalibrationNew(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode, u8 new_calibration_end, u8 CalStep, u8 SpicClkDelay, u8 StartIdx)
{
	SPIC_TypeDef *spi_flash = SPIC;

	u32 pgolden_data[2];
	u8 data_idx = 0;
	u8 phase_int = 0;
	u8 phase_frac = 0;

	int window1_start = -1;
	int window1_size = 0;
	int window1_end = -1;

	/* set bit mode  */
	FLASH_SetSpiMode(&flash_init_para, SpicBitMode);

	/* set spic register before IC clock close */
	spi_flash->BAUDR = SCKDV(flash_init_para.FLASH_baud_rate);
	spi_flash->AUTO_LENGTH = (spi_flash->AUTO_LENGTH & ~MASK_IN_PHYSICAL_CYC) | IN_PHYSICAL_CYC(SpicClkDelay);

	/* calibration Enable */
	FLASH_CalibrationNewCmd(DISABLE);
	FLASH_CalibrationNewCmd(ENABLE);

	/* first phase sample data */
	data_idx = StartIdx;
	while (1) {
		/* shift unit is CalStep*PLLCLK/FLASH_PLLCLK_EQUAL_PAHSE */
		phase_int = FLASH_SHIFT_IDX_TO_PLLCLK(data_idx);
		phase_frac = FLASH_SHIFT_IDX_TO_PAHSE(data_idx);

		if (data_idx > new_calibration_end) {
			break;
		}

		FLASH_CalibrationPhaseIdx(data_idx);

		DCache_Invalidate(SPI_FLASH_BASE, 8);
		RSIP_MMU_Cache_Clean(); 	/* Clean Cache in RSIP when otf enabled */
		pgolden_data[0] = HAL_READ32(SPI_FLASH_BASE, 0x00);
		pgolden_data[1] = HAL_READ32(SPI_FLASH_BASE, 0x04);

		if (FLASH_InitStruct->debug) {
			DBG_8195A("FLASH_CalibrationNew %x:%x phase_int:%x phase_frac:%x\n", pgolden_data[0], pgolden_data[1], phase_int, phase_frac);
		}

		/* compare data */
		if (_memcmp(pgolden_data, SPIC_CALIB_PATTERN, 8) == 0) {
			if (window1_start < 0) {
				window1_size = 1;
				window1_start = data_idx;
			} else {
				window1_size += CalStep;
			}
			window1_end = data_idx;
		} else {
			if (window1_size > 0) {
				window1_end = data_idx - CalStep;
				break;
			}
		}

		data_idx = data_idx + CalStep;
	}

	if (FLASH_InitStruct->debug) {
		DBG_8195A("FLASH_CalibrationNew LineDelay:%x phase_int:%x phase_frac:%x\n", SpicClkDelay, phase_int, phase_frac);
		DBG_8195A("FLASH_CalibrationNew window1_start:%d window1_size:%d \n", window1_start, window1_size);
	}

	if (window1_size > 0) {
		/* get phase_shift_idx */
		FLASH_InitStruct->phase_shift_idx = window1_start + window1_size / 2;

		if (FLASH_InitStruct->debug) {
			DBG_8195A("FLASH_CalibrationNew phase_shift_idx:%d \n", FLASH_InitStruct->phase_shift_idx);
		}
	}

	/* should disable it, enbale it outside if needed */
	FLASH_CalibrationNewCmd(DISABLE);

	/* Recover  */
	spi_flash->BAUDR = SCKDV(flash_init_para.FLASH_baud_boot);
	FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);

	return (window1_size | (window1_start << 16) | (window1_end << 24));
}

BOOT_RAM_TEXT_SECTION
BOOL _flash_calibration_highspeed(u8 SpicBitMode, FlashDivInt_E Div)
{
	u32 window_size = 0;
	u32 phase_shift_idx = 0;
	u32 spic_cyc_dly = 0;
	u32 spic_cyc_dly_temp = 0;
	u32 window_temp = 0;
	u32 window_size_temp = 0;

	/* init last point before calibration, Minus 1 because 0-based */
	u8 new_calibration_end = FLASH_SPICCLK_TO_PLLCLK(Div) * FLASH_PLLCLK_EQUAL_PAHSE - 1;

	/* 4.808 ms take when enter this function */
	/* 511us will take for flash full calibration */
	for (spic_cyc_dly_temp = 0; spic_cyc_dly_temp <= 4; spic_cyc_dly_temp++) {
		window_temp = FLASH_CalibrationNew(&flash_init_para, SpicBitMode, new_calibration_end, 2, spic_cyc_dly_temp, 1);

		if (flash_init_para.phase_shift_idx != 0) {
			flash_init_para.phase_shift_idx |= BIT(0); /* odd is better */
		}

		window_size_temp = (window_temp & 0xFF);
#if FLASH_CALIBRATION_DEBUG
		u32 window_start_temp = ((window_temp >> 16) & 0xFF);
		u32 window_end_temp = ((window_temp >> 24) & 0xFF);

		DBG_8195A("calibration_result:[%d:%d:%d][0x%x:0x%x] \n", spic_cyc_dly_temp, window_size_temp, flash_init_para.phase_shift_idx,
				  window_start_temp, window_end_temp);
		BKUP_Write(BKUP_REG3, ((window_start_temp << 24) | (spic_cyc_dly_temp << 16) | (window_size_temp << 8) | flash_init_para.phase_shift_idx));
#endif
		if (window_size_temp > window_size) {
			window_size = window_size_temp;
			spic_cyc_dly = spic_cyc_dly_temp;
			phase_shift_idx = flash_init_para.phase_shift_idx;
		} else {
			/*if window_size == window_size_temp == 0, shall not break. if window_size == window_size_temp != 0, means window is already max size.*/
			if (window_size_temp != window_size) {
				break;
			}
		}

		flash_init_para.phase_shift_idx = 0;
	}
	if (window_size > 0) {
		DiagPrintf("calibration_ok:[%d:%d:%d] \n", spic_cyc_dly, window_size, phase_shift_idx);

		flash_init_para.phase_shift_idx = phase_shift_idx;
		flash_init_para.FLASH_rd_sample_dly_cycle_cal = spic_cyc_dly;
		flash_init_para.FLASH_rd_sample_dly_cycle = flash_init_para.FLASH_rd_sample_dly_cycle_cal;

		return _TRUE;
	} else {
		flash_init_para.phase_shift_idx = 0;
		flash_init_para.FLASH_rd_sample_dly_cycle = SPIC_LOWSPEED_SAMPLE_PHASE;
	}

	return _FALSE;
}

BOOT_RAM_TEXT_SECTION
void flash_clksrc_sel(FlashDivInt_E div, u8 isCPUPLL, u8 is_handshake)
{
	if (isCPUPLL == BIT_LSYS_CKSL_SPIC_CPUPLL) {
		/*HW need DSPPLL & CPUPLL open*/
		FLASH_CalibrationPLLSel(FALSE);
		FLASH_Calibration_PSPLL_Open();
		if (is_handshake) {
			/* clock gate for ps_pll */
			FLASH_CalibrationPLLPSCmd(DISABLE);
		}

		/* SPIC clock switch to PLLM */
		FLASH_CalibrationPLLSel(TRUE);
		FLASH_PLLInit_ClockDiv(div);
		if (is_handshake) {
			/* clock gate for ps_pll */
			FLASH_CalibrationPLLPSCmd(DISABLE);
		}
	} else {
		/*HW need DSPPLL & CPUPLL open*/
		FLASH_CalibrationPLLSel(TRUE);
		FLASH_Calibration_PSPLL_Open();
		if (is_handshake) {
			/* clock gate for ps_pll */
			FLASH_CalibrationPLLPSCmd(DISABLE);
		}

		/* SPIC clock switch to PLLD */
		FLASH_CalibrationPLLSel(FALSE);
		FLASH_PLLInit_ClockDiv(div);
		if (is_handshake) {
			/* clock gate for ps_pll */
			FLASH_CalibrationPLLPSCmd(DISABLE);
		}
	}
}

BOOT_RAM_TEXT_SECTION
u32 flash_calibration_highspeed(FlashDivInt_E div)
{
	u32 Ret = _SUCCESS;
	u8 spic_mode = flash_init_para.FLASH_cur_bitmode;

	flash_init_para.debug = FLASH_CALIBRATION_DEBUG;
	flash_clksrc_sel(div, BIT_LSYS_CKSL_SPIC_CPUPLL, FALSE);

	if (_flash_calibration_highspeed(spic_mode, div) == _TRUE) {
		/* we should open calibration new first, and then set phase index */
		FLASH_CalibrationNewCmd(ENABLE);
		FLASH_CalibrationPhaseIdx(flash_init_para.phase_shift_idx);

		/* this code is rom code, so it is safe */
		FLASH_Init(spic_mode);

		DBG_8195A("FLASH CALIB[0x%x OK]\n", div);
	} else {
		/* calibration fail, revert SPIC clock to XTAL */
		RCC_PeriphClockSource_SPIC(BIT_LSYS_CKSL_SPIC_LBUS);

		DBG_8195A("FLASH CALIB[0x%x FAIL]\n", div);
		Ret = _FAIL;
	}

	return Ret;
}

BOOT_RAM_TEXT_SECTION
u32 flash_handshake_highspeed(FlashDivInt_E div)
{
	u32 Ret = _SUCCESS;
	u8 spic_mode = flash_init_para.FLASH_cur_bitmode;
	u8 Dphy_Dly_Cnt = 3; /* DD recommend this value */

	flash_init_para.debug = FLASH_CALIBRATION_DEBUG;
	flash_clksrc_sel(div, BIT_LSYS_CKSL_SPIC_CPUPLL, TRUE);

	RSIP_MMU_Cache_Clean(); 	/* Clean Cache in RSIP when otf enabled */

	if (FLASH_Read_HandShake(&flash_init_para, spic_mode, Dphy_Dly_Cnt) == _TRUE) {
		FLASH_Read_HandShake_Cmd(Dphy_Dly_Cnt, ENABLE);
		flash_init_para.FLASH_rd_sample_dly_cycle_cal = Dphy_Dly_Cnt + 2;
		flash_init_para.FLASH_rd_sample_dly_cycle = flash_init_para.FLASH_rd_sample_dly_cycle_cal;

		/* this code is rom code, so it is safe */
		FLASH_Init(spic_mode);

		DBG_8195A("FLASH HandShake[0x%x OK]\n", div);
	} else {
		RCC_PeriphClockSource_SPIC(BIT_LSYS_CKSL_SPIC_LBUS);

		DBG_8195A("FLASH HandShake[0x%x FAIL]\n", div);
		Ret = _FAIL;
	}

#if FLASH_CALIBRATION_DEBUG
	BKUP_Write(BKUP_REG3, Ret);
#endif

	return Ret;
}

BOOT_RAM_TEXT_SECTION
static u8 flash_get_option(u32 sys_data, BOOL is_speed)
{
	u16 tmp = (u16)BIT15;
	u8 cnt = 0;

	while (tmp) {
		if ((sys_data & tmp) != 0) {
			break;
		} else {
			tmp = tmp >> 1;
			cnt++;
		}
	}

	/* cnt =0 --> return FLASH_CLK_DIV10
	 * cnt =1 --> return FLASH_CLK_DIV9
	 * ...
	 * cnt>=8 --> return FLASH_CLK_DIV2
	*/
	if (is_speed) {
		if (cnt > 8) {
			cnt = 8;
		}

		cnt = FLASH_CLK_DIV10 - cnt;
	}

	return cnt;
}

BOOT_RAM_TEXT_SECTION
static FlashInfo_TypeDef *flash_get_chip_info(u32 flash_id)
{
	u32 i = 0;
	u32 temp;
	FlashInfo_TypeDef *pAVL;
	pAVL = Flash_AVL;

	while (pAVL[i].flash_class != FlashClassNone) {
		temp = flash_id & pAVL[i].id_mask;
		if (pAVL[i].flash_id == temp) {
			return &pAVL[i];
		}

		i++;
	}

	return NULL;
}

BOOT_RAM_TEXT_SECTION
static void flash_get_vendor(void)
{
	u8 flash_ID[4];

	/* Read flash ID */
	FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_id, 3, flash_ID);
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Flash ID: %x-%x-%x\n", flash_ID[0], flash_ID[1], flash_ID[2]);

	/* Get flash chip information */
	current_IC = flash_get_chip_info((flash_ID[2] << 16) | (flash_ID[1] << 8) | flash_ID[0]);
	if (current_IC == NULL) {
		DBG_8195A("This flash type is not supported!\n");
		assert_param(0);
	}

	/* Re-initialize flash init structure according to classification */
	switch (current_IC->flash_class) {
	case FlashClass1:
		FLASH_StructInit(&flash_init_para);
		break;
	case FlashClass2:
		FLASH_StructInit_GD(&flash_init_para);
		if (flash_ID[2] > 0x15) { /* GD capacity more than 2MB */
			flash_init_para.FLASH_cmd_wr_status2 = 0x31;
		}
		break;
	case FlashClass3:
		FLASH_StructInit_MXIC(&flash_init_para);
		break;
	case FlashClass4:	/* EON without QE bit */
		FLASH_StructInit_MXIC(&flash_init_para);
		flash_init_para.FLASH_QuadEn_bit = 0;
		break;
	case FlashClass5:
		FLASH_StructInit_Micron(&flash_init_para);
		break;
	case FlashClass6:	/* MXIC wide-range VCC chip */
		FLASH_StructInit_MXIC(&flash_init_para);
		check_config_reg = 1;
		break;
	case FlashClassUser:
		assert_param(current_IC->FlashInitHandler != NULL);
		current_IC->FlashInitHandler();
		break;
	default:
		break;
	}

	if (SYSCFG_OTP_SPICAddr4ByteEn()) {
		flash_init_para.FLASH_addr_phase_len = ADDR_4_BYTE;
	}

}

BOOT_RAM_TEXT_SECTION
static void flash_set_status_register(void)
{
	u8 StatusLen = 1;
	u32 data = 0;
	u32 status;
	u32 mask = current_IC->sta_mask;

	if (flash_init_para.FLASH_QuadEn_bit != 0) {
		data |= flash_init_para.FLASH_QuadEn_bit;
	}

	/* read status1 register */
	FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status, 1, (u8 *)&status);

	/* check if status2 exist */
	if (flash_init_para.FLASH_Status2_exist) {
		StatusLen = 2;
		FLASH_RxCmd(flash_init_para.FLASH_cmd_rd_status2, 1, ((u8 *)&status) + 1);

	} else if (check_config_reg) {	/* for MXIC wide-range flash, 1 status register + 2 config register */
		/* Read configuration register */
		FLASH_RxCmd(0x15, 2, ((u8 *)&status) + 1);
		StatusLen = 3;

		/* L/H Switch */
		data |= (BIT(9) << 8);
	}

	status &= mask;
	if (_memcmp((void *)&status, (void *)&data, StatusLen)) {
		if (!flash_init_para.FLASH_cmd_wr_status2) {
			FLASH_SetStatus(flash_init_para.FLASH_cmd_wr_status, StatusLen, (u8 *)&data);
		} else {
			FLASH_SetStatus(flash_init_para.FLASH_cmd_wr_status, 1, (u8 *)&data);
			FLASH_SetStatus(flash_init_para.FLASH_cmd_wr_status2, 1, ((u8 *)&data) + 1);
		}
	}
}

BOOT_RAM_TEXT_SECTION
u32 flash_rx_mode_switch(u8 read_mode)
{
	u32 Ret = _SUCCESS;
	u8 status = 0, spic_mode = 0, i;
	u32 pdata[2];
	char *str[] = {"1IO", "2O", "2IO", "4O", "4IO"};

	for (i = read_mode; i < 5; i++) {
		/* Try sequentially: 4IO, 4O, 2IO, 2O, 1bit */
		spic_mode = 4 - i;

		/* 4IO or 2IO should enable dummy byte function */
		if ((spic_mode == SpicQuadIOBitMode) || (spic_mode == SpicDualIOBitMode)) {
			flash_init_para.FLASH_dum_en = 1;
		}

		if (flash_init_para.FLASH_Id == FLASH_ID_MICRON) {
			FLASH_RxCmd(0x85, 1, &status);

			status = (status & 0x0f) | (flash_init_para.FLASH_rd_dummy_cyle[spic_mode] << 4);
			FLASH_SetStatus(0x81, 1, &status);
		}

		flash_init_para.FLASH_rd_sample_dly_cycle = SPIC_LOWSPEED_SAMPLE_PHASE;
		FLASH_Init(spic_mode);

		DCache_Invalidate(SPI_FLASH_BASE, 8);
		RSIP_MMU_Cache_Clean();		/* Clean Cache in RSIP when otf enabled */
		pdata[0] = HAL_READ32(SPI_FLASH_BASE, 0x00);
		pdata[1] = HAL_READ32(SPI_FLASH_BASE, 0x04);

		if (_memcmp(pdata, SPIC_CALIB_PATTERN, 8) == 0) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Flash Read %s\n", str[spic_mode]);
			break;
		} else {
			if (flash_init_para.debug) {
				DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Flash Read %s, FAIL\n", str[spic_mode]);
			}
		}
	}

	if (i == 5) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "Flash Switch Read Mode FAIL\n");
		Ret = _FAIL;
	}

	return Ret;
}

BOOT_RAM_TEXT_SECTION
void flash_highspeed_setup(void)
{
	u8 read_mode, flash_speed;
	u8 spic_ckd;
	u32 pllm_clk = PLL_ClkGet(CLK_CPU_MPLL);

	read_mode = flash_get_option(Flash_ReadMode, _FALSE);
	flash_speed = flash_get_option(Flash_Speed, _TRUE);

	spic_ckd = CLKDIV_ROUND_UP(pllm_clk, SPIC_CLK_LIMIT) - 1;
	flash_speed = MAX(flash_speed, spic_ckd);

	__disable_irq();

	/* SPIC stay in BUSY state when there are more than 0x1_0000 cycles between two input data.
	 * Disable DREIR to avoid that interrupt hanler time is lager than 0x1_0000 SPIC cycles.
	 */
	SPIC_TypeDef *spi_flash = SPIC;
	spi_flash->CTRLR0 |= BIT_SPI_DREIR_R_DIS;

	/* if 8 auto rd wrap ever occur, and user rd + auto rd wrap is together, SPIC will return 0xFFFFFFFF only in Lite. */
	/* Set DIS_WRAP_ALIGN = 1, SPIC will chop wrap transation into two incremental transactions to avoid corner case.*/
	spi_flash->CTRLR2 |= BIT_DIS_WRAP_ALIGN;
	/* use uncacheable read (no wrap) to restore to normal function */
	DCache_Invalidate(SPI_FLASH_BASE, 8);
	RSIP_MMU_Cache_Clean(); 	/* Clean Cache in RSIP when otf enabled */
	HAL_READ32(SPI_FLASH_BASE, 0x00);

	/* Get flash ID to reinitialize FLASH_InitTypeDef structure */
	flash_get_vendor();

	/* Set flash status register: set QE, clear protection bits */
	flash_set_status_register();

	/* Set flash I/O mode and high-speed calibration */
	flash_rx_mode_switch(read_mode);

	if (SYSCFG_CHIPType_Get() != CHIP_TYPE_FPGA) {
		flash_handshake_highspeed(flash_speed);
	}

	__enable_irq();
}

