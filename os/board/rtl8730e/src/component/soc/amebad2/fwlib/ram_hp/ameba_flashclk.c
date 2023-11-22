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

static const char *TAG = "FLASHCLK";
static const FlashInfo_TypeDef *current_IC;

/* Flag to check configuration register or not. Necessary for wide-range VCC MXIC flash */
static u8 check_config_reg = 0;

/**
  * @brief  Set Flash Clock
  * @param  Div: clock Dividor for SPIC Clock.
  *   This parameter can be any one of the following values:
  *		 @arg FLASH_CLK_DIV4 => SPIC-1/4 nppll
  *		 @arg FLASH_CLK_DIV5 => SPIC-1/5 nppll
  *		 @arg FLASH_CLK_DIV6 => SPIC-1/6 nppll
  *		 @arg FLASH_CLK_DIV7 => SPIC-1/7 nppll
  *		 @arg FLASH_CLK_DIV8 => SPIC-1/8 nppll
  *		 @arg FLASH_CLK_DIV9 => SPIC-1/9 nppll
  *		 @arg FLASH_CLK_DIV10 => SPIC-1/10 nppll
  */
BOOT_RAM_TEXT_SECTION
u32 FLASH_ClockDiv(u8 Div)
{
	u32 temp = 0;
	int timeout = 10;
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;

	/* 1. Gate flash clock, dont reset SPIC */
	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_FLASH_CLOCK, DISABLE);

	/* disable clock div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp &= ~(HSYS_BIT_FLASH_DIV_EN | HSYS_BIT_FLASH_PS_DIV_EN);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 2. set clock div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp &= ~(HSYS_MASK_FLASH_DIV_INT);
	temp |= HSYS_FLASH_DIV_INT(Div);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 3. polling npll_pow_erc: 0x4200_8840h[31] */
	while (timeout > 0) {
		timeout--;
		temp = PLL->PLL_NPPLL_CTRL0;
		if (temp & PLL_BIT_NPLL_POW_ERC) {
			break;
		}
	}

	timeout = 10;
	/* 4. polling npll_pow_pll: 0x4200_8840h[29] */
	while (timeout > 0) {
		timeout--;
		temp = PLL->PLL_NPPLL_CTRL0;
		if (temp & PLL_BIT_NPLL_POW_PLL) {
			break;
		}
	}

	/* 5. enable clock div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp |= HSYS_BIT_FLASH_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 6. enable clock ps div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp |= HSYS_BIT_FLASH_PS_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	timeout = 10;
	/* 7. wait ready flag of Flash clock with phase shift, 65 ns cost */
	while (timeout > 0) {
		timeout--;
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
		temp &= HSYS_BIT_FLASH_DIV_RDY | HSYS_BIT_FLASH_PS_DIV_RDY;
		if (temp == (HSYS_BIT_FLASH_DIV_RDY | HSYS_BIT_FLASH_PS_DIV_RDY)) {
			break;
		}
	}

	/* 8. Release gating of flash clock after BIT_FLASH_DIV_RDY. */
	RCC_PeriphClockCmd(APBPeriph_FLASH, APBPeriph_FLASH_CLOCK, ENABLE);
	RCC_PeriphClockSource_SPIC(BIT_LSYS_CKSL_SPIC_PLL);
	return _TRUE;
}

/**
  * @brief  Set New Calibration Clock Phase shift step for calibration
  * @param  phase_idx: get from calibration to set FLASH_PS_PHASE
  */
BOOT_RAM_TEXT_SECTION
u32 FLASH_CalibrationPhaseIdx(u8 phase_idx)
{
	u32 temp = 0;
	int timeout = 10;

	/* 1. Gate flash clock */
	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_FLASH_CLOCK, DISABLE);

	/* 2.1. disable clock ps div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp &= ~HSYS_BIT_FLASH_PS_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 2.2. disable clock div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp &= ~HSYS_BIT_FLASH_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 2.3. wait ready flag disable for Flash clock with phase shift */
	while (timeout > 0) {
		timeout--;
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
		temp &= HSYS_BIT_FLASH_DIV_RDY | HSYS_BIT_FLASH_PS_DIV_RDY;
		if (temp == 0) {
			break;
		}
	}

	/* 3 set phase shift clock */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp &= ~HSYS_MASK_FLASH_PS_PHASE; /* set phase shift: 5bits : 1/2 np PLL clock cycles */
	temp |= HSYS_FLASH_PS_PHASE(phase_idx);
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 4.1. enable clock div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp |= HSYS_BIT_FLASH_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 4.2. enable clock ps div */
	temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
	temp |= HSYS_BIT_FLASH_PS_DIV_EN;
	HAL_WRITE32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL, temp);

	/* 5. wait ready flag of Flash clock with phase shift */
	timeout = 10;
	while (timeout > 0) {
		timeout--;
		temp = HAL_READ32(SYSTEM_CTRL_BASE_HP, REG_HSYS_SPIC_CTRL);
		temp &= HSYS_BIT_FLASH_DIV_RDY | HSYS_BIT_FLASH_PS_DIV_RDY;
		if (temp == (HSYS_BIT_FLASH_DIV_RDY | HSYS_BIT_FLASH_PS_DIV_RDY)) {
			break;
		}
	}

	/* 6. Release gating of flash clock */
	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_FLASH_CLOCK, ENABLE);

	return _TRUE;
}

/**
  * @brief  Set New Calibration Disable/Enable
  * @param  NewStatus: Disable/Enable
  */
BOOT_RAM_TEXT_SECTION
u32 FLASH_CalibrationNewCmd(u32 NewStatus)
{
	u32 temp = 0;
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

/**
  * @brief  Flash New calibration function with phase shift
  * @param  FLASH_InitStruct: pointer to a FLASH_InitTypeDef structure that contains
  *         the configuration information for the SPIC Clock.
  * @param  SpicBitMode: This parameter can be any combination of the following values
  *		 @arg SpicOneBitMode
  *		 @arg SpicDualBitMode
  *		 @arg SpicQuadBitMode
  * @param  Div: NPPLL clock Dividor for SPIC Clock.
  *   This parameter can be any combination of the following values:
  *		 @arg FLASH_CLK_DIV4 => SPIC-1/4 nppll
  *		 @arg FLASH_CLK_DIV5 => SPIC-1/5 nppll
  *		 @arg FLASH_CLK_DIV6 => SPIC-1/6 nppll
  *		 @arg FLASH_CLK_DIV7 => SPIC-1/7 nppll
  *		 @arg FLASH_CLK_DIV8 => SPIC-1/8 nppll
  *		 @arg FLASH_CLK_DIV9 => SPIC-1/9 nppll
  *		 @arg FLASH_CLK_DIV10 => SPIC-1/10 nppll
  * @param  CalStep: CalStep * 1/2 np PLL clock cycles.(If np PLL is 1GHz, the step is 0.5ns)
  * @param  LineDelay(13~21ns): include FlashDelay(winbond: 6ns)+LineDelay(1ns)+AmebaDelay(13ns)
  * @param  StartIdx: calibration start index of Flash clock phase shift in units of 1/2 np PLL clock cycles.
  * @retval status value: (window1_size | (window1_start << 16) | (window1_end << 24))
  * @note 100MHz baudrate read 32bit will cost 8(cmd)+6(addr)+6(dummy)+4(data) = 24cycles = 240ns
  * @note step=2 & 100MHz will take 56.5us for a LineDelay calibration
  * @note cache will be disable in this function, you should open cache after this function if needed
  */
BOOT_RAM_TEXT_SECTION
u32 FLASH_CalibrationNew(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode, u8 Div, u8 CalStep, u8 LineDelay,
						 u8 StartIdx)
{
	SPIC_TypeDef *spi_flash = SPIC;

	u32 pgolden_data[2];
	u8 shift_step = 0;
	u8 phase_shift_end = (Div + 1) * 2 - 1; /* FLASH_PS_PHASE range is [0, (FLASH_DIV_INT+1)*2-1] */

	int window1_start = -1;
	int window1_size = 0;
	int window1_end = -1;

	/* set bit mode  */
	if (SYSCFG_BootFromNor()) {
		FLASH_SetSpiMode(&flash_init_para, SpicBitMode);
	} else {
		NAND_SetSpiMode(&flash_init_para, SpicBitMode);
	}

	/* set spic register before IC clock close */
	spi_flash->BAUDR = SCKDV(flash_init_para.FLASH_baud_rate);
	spi_flash->AUTO_LENGTH = (spi_flash->AUTO_LENGTH & ~MASK_IN_PHYSICAL_CYC) | IN_PHYSICAL_CYC(LineDelay);

	/* calibration Enable */
	FLASH_CalibrationNewCmd(DISABLE);
	FLASH_CalibrationNewCmd(ENABLE);

	/* first phase sample data */
	shift_step = StartIdx;
	while (1) {
		if (shift_step > phase_shift_end) {
			break;
		}

		FLASH_CalibrationPhaseIdx(shift_step);

		DCache_Invalidate(SPI_FLASH_BASE, 8);
		RSIP_MMU_Cache_Clean();
		pgolden_data[0] = HAL_READ32(SPI_FLASH_BASE, 0x00);
		pgolden_data[1] = HAL_READ32(SPI_FLASH_BASE, 0x04);

		if (FLASH_InitStruct->debug) {
			RTK_LOGD(TAG, "FLASH_CalibrationNew %x:%x phase_shift:%x\n", pgolden_data[0], pgolden_data[1], shift_step);
		}

		/* compare data */
		if (_memcmp(pgolden_data, SPIC_CALIB_PATTERN, 8) == 0) {
			if (window1_start < 0) {
				window1_size = 1;
				window1_start = shift_step;
			} else {
				window1_size += CalStep;
			}
			window1_end = shift_step;
		} else {
			if (window1_size > 0) {
				break;
			}
		}

		shift_step = shift_step + CalStep;
	}

	if (FLASH_InitStruct->debug) {
		RTK_LOGD(TAG, "FLASH_CalibrationNew LineDelay:%x\n", LineDelay);
		RTK_LOGD(TAG, "FLASH_CalibrationNew window1_start:%d window1_size:%d \n", window1_start, window1_size);
	}

	if (window1_size > 0) {
		/* get phase_shift_idx */
		FLASH_InitStruct->phase_shift_idx = window1_start + window1_size / 2;

		if (FLASH_InitStruct->debug) {
			RTK_LOGD(TAG, "FLASH_CalibrationNew phase_shift_idx:%d \n", FLASH_InitStruct->phase_shift_idx);
		}
	}

	/* should disable it, enbale it outside if needed */
	FLASH_CalibrationNewCmd(DISABLE);

	/* Recover  */
	spi_flash->BAUDR = SCKDV(flash_init_para.FLASH_baud_boot);

	if (SYSCFG_BootFromNor()) {
		FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	} else {
		NAND_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	}

	return (window1_size | (window1_start << 16) | (window1_end << 24));
}

/**
  * @brief  This function is used to calibration for multi-channel communication.
  * @param FLASH_InitStruct: pointer to a FLASH_InitTypeDef
  *         structure that contains the configuration information for SPIC.
  * @param SpicBitMode: the bitmode to be calibrated
  * @param  LineDelay: include FlashDelay(winbond: 6ns)+LineDelay(1ns)+AmebaDelay(13ns)
  * @retval 1: calibration success
  *		0: calibration fail
  */
BOOT_RAM_TEXT_SECTION
u32 FLASH_Calibration(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode, u8 LineDelay)
{
	SPIC_TypeDef *spi_flash = SPIC;
	u32 rd_data;
	u32 baudr = FLASH_InitStruct->FLASH_baud_rate;
	u32 pass = 0;
	u32 tempBaudr = spi_flash->BAUDR;
	u32 tempAutolen = spi_flash->AUTO_LENGTH;
	u32 sample_phase = 0;
	int window_start = -1;
	int window_size = 0;

	/* in old calibration, we should close new calibration */
	FLASH_CalibrationNewCmd(DISABLE);

	/* set bit mode  */
	if (SYSCFG_BootFromNor()) {
		FLASH_SetSpiMode(&flash_init_para, SpicBitMode);
	} else {
		NAND_SetSpiMode(&flash_init_para, SpicBitMode);
	}

	/* start calibration */
	spi_flash->BAUDR = SCKDV(baudr);

	/* unit is spic IP clock */
	for (sample_phase = 0; sample_phase < baudr * 2 + LineDelay; sample_phase++) {
		spi_flash->AUTO_LENGTH = (spi_flash->AUTO_LENGTH & ~MASK_IN_PHYSICAL_CYC) | IN_PHYSICAL_CYC(sample_phase);

		DCache_Invalidate(SPI_FLASH_BASE, 8);
		RSIP_MMU_Cache_Clean();
		rd_data = HAL_READ32(SPI_FLASH_BASE,  0);

		if (FLASH_InitStruct->debug) {
			RTK_LOGD(TAG, "FLASH_Calibration %x sample_phase:%x rate:%x\n", rd_data, sample_phase, baudr);
		}

		/* compare data */
		if (rd_data == SPIC_CALIB_PATTERN[0]) {
			window_size++;
			if (window_start < 0) {
				window_start = sample_phase;
			}
			pass = 1;
		}
	}
	/* stop calibration */

	if (window_size > 0) {
		sample_phase = window_start + window_size / 2;
		FLASH_InitStruct->FLASH_rd_sample_phase = sample_phase;

		if (FLASH_InitStruct->debug) {
			RTK_LOGD(TAG, "FLASH_Calibration sample_phase:%x rate:%x\n", sample_phase, baudr);
		}
	}

	/* Recover SPI mode */
	if (SYSCFG_BootFromNor()) {
		FLASH_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	} else {
		NAND_SetSpiMode(&flash_init_para, flash_init_para.FLASH_cur_bitmode);
	}

	/* restore baudr & AUTO_LENGTH */
	spi_flash->BAUDR = tempBaudr ;
	spi_flash->AUTO_LENGTH = tempAutolen;

	if (pass) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}

BOOT_RAM_TEXT_SECTION
BOOL _flash_calibration_highspeed(u8 SpicBitMode, u8 div)
{
	u32 window_size = 0;
	u32 phase_shift_idx = 0;
	u32 line_delay = 0;
	u32 line_delay_temp = 0;
	u32 window_temp = 0;
	u32 window_size_temp = 0;

	/* 4.808 ms take when enter this function */
	/* 511us will take for flash full calibration */
	for (line_delay_temp = 0; line_delay_temp <= 4; line_delay_temp++) {
		window_temp = FLASH_CalibrationNew(&flash_init_para, SpicBitMode, div, 1, line_delay_temp, 1);
		window_size_temp = (window_temp & 0xFF);

		if (flash_init_para.phase_shift_idx != 0) {
			if (window_size_temp > 2) {
				flash_init_para.phase_shift_idx |= BIT(0);    /* odd is better */
			}
		}

#if FLASH_CALIBRATION_DEBUG
		u32 window_start_temp = ((window_temp >> 16) & 0xFF);
		u32 window_end_temp = ((window_temp >> 24) & 0xFF);

		RTK_LOGD(TAG, "calibration_result:[%d:%d:%d][%x:%x] \n", line_delay_temp, window_size_temp, flash_init_para.phase_shift_idx,
				 window_start_temp, window_end_temp);
		BKUP_Write(BKUP_REG3, ((window_start_temp << 24) | (line_delay_temp << 16) | (window_size_temp << 8) | flash_init_para.phase_shift_idx));
#endif

		if (window_size_temp > window_size) {
			window_size = window_size_temp;
			line_delay = line_delay_temp;
			phase_shift_idx = flash_init_para.phase_shift_idx;
		} else {
			if (window_size > 0 && window_size_temp == 0) {
				break;
			}
		}

		flash_init_para.phase_shift_idx = 0;
	}
	if (window_size > 0) {
		RTK_LOGI(TAG, "calibration_ok:[%d:%d:%d] \n", line_delay, window_size, phase_shift_idx);

		flash_init_para.phase_shift_idx = phase_shift_idx;
		flash_init_para.FLASH_rd_sample_phase_cal = line_delay;
		flash_init_para.FLASH_rd_sample_phase = flash_init_para.FLASH_rd_sample_phase_cal;

		return _TRUE;
	} else {
		flash_init_para.phase_shift_idx = 0;
		flash_init_para.FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
	}

	return _FALSE;
}

BOOT_RAM_TEXT_SECTION
u32 flash_calibration_highspeed(u8 div)
{
	u32 Ret = _SUCCESS;
	u8 spic_mode = flash_init_para.FLASH_cur_bitmode;


	flash_init_para.debug = FLASH_CALIBRATION_DEBUG;

	/* SPIC clock switch to PLL */
	FLASH_ClockDiv(div);

	if (_flash_calibration_highspeed(spic_mode, div) == _TRUE) {
		/* we should open calibration new first, and then set phase index */
		FLASH_CalibrationNewCmd(ENABLE);
		FLASH_CalibrationPhaseIdx(flash_init_para.phase_shift_idx);

		/* this code is rom code, so it is safe */
		if (SYSCFG_BootFromNor()) {
			FLASH_Init(spic_mode);
		} else {
			NAND_Init(spic_mode);
		}

		RTK_LOGI(TAG, "FLASH CALIB[0x%x OK]\n", div);
	} else {
		/* calibration fail, revert SPIC clock to XTAL */
		RCC_PeriphClockSource_SPIC(BIT_LSYS_CKSL_SPIC_XTAL);

		RTK_LOGE(TAG, "FLASH CALIB[0x%x FAIL]\n", div);

		Ret = _FAIL;
	}


	return Ret;
}

BOOT_RAM_TEXT_SECTION
static u8 flash_get_option(u32 sys_data, BOOL is_speed)
{
	u16 tmp = 0x8000;
	u8 cnt = 0;

	while (tmp) {
		if ((sys_data & tmp) != 0) {
			break;
		} else {
			tmp = tmp >> 1;
			cnt++;
		}
	}

	if (is_speed) {
		if (cnt > 6) {
			cnt = 6;
		}

		cnt = FLASH_CLK_DIV10 - cnt;
	}

	return cnt;
}

BOOT_RAM_TEXT_SECTION
static const FlashInfo_TypeDef *flash_get_chip_info(u32 flash_id)
{
	u32 i = 0;
	u32 temp;
	const FlashInfo_TypeDef *pAVL;
	if (SYSCFG_BootFromNor()) {
		pAVL = Flash_AVL;
	} else {
		pAVL = NAND_AVL;
	}

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
	RTK_LOGI(TAG, "Flash ID: %x-%x-%x\n", flash_ID[0], flash_ID[1], flash_ID[2]);

	/* Get flash chip information */
	current_IC = flash_get_chip_info((flash_ID[2] << 16) | (flash_ID[1] << 8) | flash_ID[0]);
	if (current_IC == NULL) {
		RTK_LOGW(TAG, "This flash type is not supported!\n");
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
static void nand_get_vendor(void)
{
	u8 flash_ID[4];

	/* Read flash ID */
	NAND_RxCmd(flash_init_para.FLASH_cmd_rd_id, 0, NULL, 2, flash_ID);
	RTK_LOGI(TAG, "NAND ID: %x-%x\n", flash_ID[0], flash_ID[1]);

	/* Get flash chip information */
	current_IC = flash_get_chip_info((flash_ID[2] << 16) | (flash_ID[1] << 8) | flash_ID[0]);
	if (current_IC == NULL) {
		RTK_LOGW(TAG, "This flash type is not supported!\n");
		assert_param(0);
	}

	/* Re-initialize flash init structure according to classification */
	switch (current_IC->flash_class) {
	case FlashClass1:
		NAND_StructInit(&flash_init_para);
		break;
	case FlashClass2:
		NAND_StructInit_GD(&flash_init_para);
		break;
	case FlashClass3:
		NAND_StructInit_MXIC(&flash_init_para);
		break;
	case FlashClass5:
		NAND_StructInit_Micron(&flash_init_para);
		break;
	case FlashClassUser:
		assert_param(current_IC->FlashInitHandler != NULL);
		current_IC->FlashInitHandler();
		break;
	default:
		break;
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

		if (SYSCFG_BootFromNor()) {
			/* 4IO or 2IO should enable dummy byte function */
			if ((spic_mode == SpicQuadIOBitMode) || (spic_mode == SpicDualIOBitMode)) {
				flash_init_para.FLASH_dum_en = 1;
			}

			if (flash_init_para.FLASH_Id == FLASH_ID_MICRON) {
				FLASH_RxCmd(0x85, 1, &status);

				status = (status & 0x0f) | (flash_init_para.FLASH_rd_dummy_cyle[spic_mode] << 4);
				FLASH_SetStatus(0x81, 1, &status);
			}

			flash_init_para.FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
			FLASH_Init(spic_mode);
		} else {
			flash_init_para.FLASH_rd_sample_phase = SPIC_LOWSPEED_SAMPLE_PHASE;
			NAND_Init(spic_mode);
		}

		DCache_Invalidate(SPI_FLASH_BASE, 8);
		RSIP_MMU_Cache_Clean();
		pdata[0] = HAL_READ32(SPI_FLASH_BASE, 0x00);
		pdata[1] = HAL_READ32(SPI_FLASH_BASE, 0x04);

		if (_memcmp(pdata, SPIC_CALIB_PATTERN, 8) == 0) {
			RTK_LOGI(TAG, "Flash Read %s\n", str[spic_mode]);
			break;
		} else {
			if (flash_init_para.debug) {
				RTK_LOGD(TAG, "Flash Read %s, FAIL\n", str[spic_mode]);
			}
		}
	}

	if (i == 5) {
		RTK_LOGE(TAG, "Flash Switch Read Mode FAIL\n");
		Ret = _FAIL;
	}

	return Ret;
}

BOOT_RAM_TEXT_SECTION
void flash_highspeed_setup(void)
{
	u8 read_mode;
	u8 flash_speed;

	read_mode = flash_get_option(Flash_ReadMode, _FALSE);
	flash_speed = flash_get_option(Flash_Speed, _TRUE);
	//RTK_LOGD(TAG, "flash_speed: %d\n", flash_speed);
	__asm volatile("cpsid i");

	/* SPIC stay in BUSY state when there are more than 0x1_0000 cycles between two input data.
	 * Disable DREIR to avoid that interrupt hanler time is lager than 0x1_0000 SPIC cycles.
	 */
	SPIC_TypeDef *spi_flash = SPIC;
	spi_flash->CTRLR0 |= BIT_SPI_DREIR_R_DIS;

	if (SYSCFG_BootFromNor()) {
		/* Get flash ID to reinitialize FLASH_InitTypeDef structure */
		flash_get_vendor();

		/* Set flash status register: set QE, clear protection bits */
		flash_set_status_register();
	} else {
		/* Get Nand ID and Set QE */
		nand_get_vendor();
		/* QuadEnable */
		if (flash_init_para.FLASH_QuadEn_bit != 0) {
			NAND_SetStatus(NAND_REG_CFG, NAND_GetStatus(NAND_REG_CFG) | NAND_CFG_QUAD_ENABLE);
		}
		/*After power-up, the chip is in protection state, set feature bits BPx to 0 to unlock all block. */
		NAND_SetStatus(NAND_REG_BLOCK_LOCK, NAND_BL_ALL_UNLOCKED);
	}

	/* Set flash I/O mode and high-speed calibration */
#ifdef CONFIG_LINUX_FW_EN
	/* A-cut need flash 1IO when running Linux */
	if (SYSCFG_CUT_VERSION_B == SYSCFG_RLVersion()) {
		flash_rx_mode_switch(read_mode);
	}
#else
	flash_rx_mode_switch(read_mode);
#endif

	if (SYSCFG_CHIPType_Get() != CHIP_TYPE_FPGA) {
		flash_calibration_highspeed(flash_speed);
	}

	__asm volatile("cpsie i");
}

