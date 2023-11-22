/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static const char *TAG = "SWR";
/**
* @brif  swr calibation times.
*/
static u8 Swr_Calib_DCore_Times = Swr_Calib_Nocalib;
static u8 Swr_Calib_MEM_PWM_Times = Swr_Calib_Nocalib;
static u8 Swr_Calib_MEM_PFM_Times = Swr_Calib_Nocalib;
static u8 Swr_Calib_AUD_PWM_Times = Swr_Calib_Nocalib;
static u8 Swr_Calib_AUD_PFM_Times = Swr_Calib_Nocalib;


static u32 SWR_IDX_FLAG[5] = {
	VOL_SWR_DCORE_09_PWM,		/* DCore check 0x7E0*/
	VOL_SWR_MEM_PWM,		/* MEM check 0x7E4*/
	VOL_SWR_MEM_PFM,		/* MEM check 0x7E5*/
	VOL_SWR_AUD_PWM,		/* AUDIO PWM check 0x7E6*/
	VOL_SWR_AUD_PFM,		/* AUDIO PFM check 0x7E7*/
};

/**
  * @brief Check calibration timers.
  * @param swr_type: this parameter can be one parameter from IS_SWR_Type.
  * @retval this parameter can be one parameter from IS_SWR_CALIB_Info.
  */
static u32 SWR_Calib_Check(u32 swr_idx)
{
	u32 idx = 0;
	u32 Check_Addr = 0;
	u8 swr_data = 0;

	/*check the parameters*/
	assert_param(IS_SWR_Type(swr_idx));

	idx = swr_idx - 1;
	Check_Addr = SWR_IDX_FLAG[idx];

	OTP_Read8(Check_Addr, &swr_data);

	if ((swr_data & 0xF0) != 0xF0) {
		return Swr_Calib_Twocalib;//calibration two times
	} else if ((swr_data & 0x0F) != 0xF) {
		return Swr_Calib_Onecalib;//calibration only one time
	} else {
		return Swr_Calib_Nocalib; // no calibration yet
	}


}

/**
  * @brief Get actual calibration value.
  * @param swr_efuse_value: data read from otp
  * @param swr_type: swr type, this parameter can be one parameter from IS_SWR_Type.
  * @param width: width, this parameter can be one parameter from IS_SWR_CALIB_BitWith.
  * @retval actual calibration value.
  */
static u8 SWR_Calib_Get_Value(u8 swr_efuse_value, u32 swr_type, u8 width)
{
	u32 type = swr_type;
	u8 swr_calib_times = 0;

	/*check the parameters*/
	assert_param(IS_SWR_Type(type));
	assert_param(IS_SWR_CALIB_BitWith(width));

	switch (type) {
	case Swr_Core:
		swr_calib_times = Swr_Calib_DCore_Times;
		break;

	case Swr_MEM_PWM:
		swr_calib_times = Swr_Calib_MEM_PWM_Times;
		break;

	case Swr_MEM_PFM:
		swr_calib_times = Swr_Calib_MEM_PFM_Times;
		break;

	case Swr_AUD_PWM:
		swr_calib_times = Swr_Calib_AUD_PWM_Times;
		break;

	case Swr_AUD_PFM:
		swr_calib_times = Swr_Calib_AUD_PFM_Times;
		break;

	default:
		RTK_LOGE(TAG, "wrong swr calib type\n");
		return 0;
	}

	if (width == Width_4Bit) {
		if (swr_calib_times == Swr_Calib_Twocalib) {
			return (u8)(SWR_Calib_Twocalib_4Bit_Value(swr_efuse_value));
		} else if (swr_calib_times == Swr_Calib_Onecalib) {
			return (u8)(SWR_Calib_Onecalib_4Bit_Value(swr_efuse_value));
		}
	} else if (width == Width_2Bit) {
		if (swr_calib_times == Swr_Calib_Twocalib) {
			return (u8)(SWR_Calib_Twocalib_2Bit_Value(swr_efuse_value));
		} else if (swr_calib_times == Swr_Calib_Onecalib) {
			return (u8)(SWR_Calib_Onecalib_2Bit_Value(swr_efuse_value));
		}
	}

	return 0;
}


/**
  * @brief Load SWR_DCore parameters.
  * @retval none
  */
void SWR_Calib_DCore(void)
{
	REGU_TypeDef *regu = REGU_BASE;
	u32 reg_tmp;

	/* improve load transient */
	reg_tmp = regu->REGU_SWR_OFF_CTRL0;
	reg_tmp &= ~(REGU_MASK_C1_L1);
	reg_tmp |= REGU_C1_L1(0x1);
	reg_tmp &= ~(REGU_MASK_C3_L1);
	reg_tmp |= REGU_C3_L1(0x3);
	reg_tmp &= ~(REGU_MASK_R2_L1);
	reg_tmp |= REGU_R2_L1(0x3);
	regu->REGU_SWR_OFF_CTRL0 = reg_tmp;

	/* PWM ZCDC keep 2'b11 */
	reg_tmp = regu->REGU_SWR_ON_CTRL0;
	reg_tmp &= ~(REGU_MASK_REG_ZCDC_H);
	reg_tmp |= REGU_REG_ZCDC_H(0x3);
	regu->REGU_SWR_ON_CTRL0 = reg_tmp;

	/* Bypass C-cut ZCD ECO */
	reg_tmp = regu->REGU_SWR_DIG_ZCD;
	reg_tmp |= BIT0;
	regu->REGU_SWR_DIG_ZCD = reg_tmp;

	reg_tmp = regu->REGU_SWR_NEW;
	reg_tmp |= REGU_REG_SWR_ZCD_CTRL_PFM(0x1);
	regu->REGU_SWR_NEW = reg_tmp;

	/* set swr step time */
	reg_tmp = regu->REGU_SWR_ARB_CTRL0;
	reg_tmp &= ~(REGU_MASK_SWR_PSW_WAIT_CYC);

	/* 50us/step is recommanded by DD */
	reg_tmp |= 0x4;
	regu->REGU_SWR_ARB_CTRL0 = reg_tmp;
}


/**
  * @brief Load SWR_MEM parameters.
  * @retval none
  */
void SWR_Calib_MEM(void)
{
	REGU_TypeDef *regu = REGU_BASE;
	SWR_TypeDef *SWR = SWR2_BASE;
	u32 reg_tmp;
	u32 MEM_PWM_Addr = VOL_SWR_MEM_PWM;
	u32 MEM_PFM_Addr = VOL_SWR_MEM_PFM;
	u8 MEM_PWM, MEM_PFM, MEM_PWM_Value, MEM_PFM_Value;

	/* improve load transient */
	reg_tmp = SWR->SWR_OFF_CTRL0;
	reg_tmp &= ~(SWR_MASK_C1_L1);
	reg_tmp |= SWR_C1_L1(0x1);
	reg_tmp &= ~(SWR_MASK_C3_L1);
	reg_tmp |= SWR_C3_L1(0x3);
	reg_tmp &= ~(SWR_MASK_R2_L1);
	reg_tmp |= SWR_R2_L1(0x3);
	SWR->SWR_OFF_CTRL0 = reg_tmp;

	/* PWM ZCDC keep 2'b11 */
	reg_tmp = SWR->SWR_ON_CTRL0;
	reg_tmp &= ~(SWR_MASK_REG_ZCDC_H);
	reg_tmp |= SWR_REG_ZCDC_H(0x3);
	SWR->SWR_ON_CTRL0 = reg_tmp;

	/* Bypass C-cut ZCD ECO */
	reg_tmp = SWR->SWR_DIG_ZCD;
	reg_tmp |= BIT0;
	SWR->SWR_DIG_ZCD = reg_tmp;

	reg_tmp = SWR->SWR_NEW;
	reg_tmp |= SWR_REG_SWR_ZCD_CTRL_PFM(0x1);
	SWR->SWR_NEW = reg_tmp;

	/* load VOL_SWR_MEM_PWM */
	Swr_Calib_MEM_PWM_Times = SWR_Calib_Check(Swr_MEM_PWM);

	if (Swr_Calib_MEM_PWM_Times != Swr_Calib_Nocalib) {
		OTP_Read8(MEM_PWM_Addr, &MEM_PWM);
		MEM_PWM_Value = SWR_Calib_Get_Value(MEM_PWM, Swr_MEM_PWM, Width_4Bit);
		reg_tmp = SWR->SWR_ON_CTRL0;
		reg_tmp &= ~(SWR_MASK_VOL_L1);
		reg_tmp |= SWR_VOL_L1(MEM_PWM_Value);
		SWR->SWR_ON_CTRL0 = reg_tmp;

		if ((REGU_GET_BG_R1_TUNE(regu->REGU_SWR_ON_CTRL2) == 0x7) && (REGU_GET_BG_R2_TUEN(regu->REGU_SWR_ON_CTRL2) == 0x0)) {
			/* adjust BG R1&R2*/
			reg_tmp = SWR->SWR_OFF_CTRL1;
			reg_tmp &= ~(SWR_MASK_BG_R2_TUNE);
			reg_tmp |= SWR_MASK_BG_R1_TUNE;
			SWR->SWR_OFF_CTRL1 = reg_tmp;
		}
	}

	/* load VOL_SWR_MEM_PFM */
	Swr_Calib_MEM_PFM_Times = SWR_Calib_Check(Swr_MEM_PFM);

	if (Swr_Calib_MEM_PFM_Times != Swr_Calib_Nocalib) {
		OTP_Read8(MEM_PFM_Addr, &MEM_PFM);
		MEM_PFM_Value = SWR_Calib_Get_Value(MEM_PFM, Swr_MEM_PFM, Width_4Bit);
		reg_tmp = SWR->SWR_ON_CTRL0;
		reg_tmp &= ~(SWR_MASK_VREFPFM_L);
		reg_tmp |= SWR_VREFPFM_L(MEM_PFM_Value);
		SWR->SWR_ON_CTRL0 = reg_tmp;

	}
}


/**
  * @brief Load SWR_AUD parameters.
  * @retval none
  */
void SWR_Calib_AUD(void)
{
	SWR_TypeDef *SWR = SWR1_BASE;
	u32 reg_tmp;
	u32 Audio_PWM_Addr = VOL_SWR_AUD_PWM;
	u32 Audio_PFM_Addr = VOL_SWR_AUD_PFM;
	u8 Audio_PWM, Audio_PFM, Audio_PWM_Value, Audio_PFM_Value;

	/* improve load transient*/
	reg_tmp = SWR->SWR_OFF_CTRL0;
	reg_tmp &= ~(SWR_MASK_C1_L1);
	reg_tmp |= SWR_C1_L1(0x1);
	reg_tmp &= ~(SWR_MASK_C3_L1);
	reg_tmp |= SWR_C3_L1(0x3);
	reg_tmp &= ~(SWR_MASK_R2_L1);
	reg_tmp |= SWR_R2_L1(0x3);
	SWR->SWR_OFF_CTRL0 = reg_tmp;

	/*  PWM ZCDC keep 2'b11 */
	reg_tmp = SWR->SWR_ON_CTRL0;
	reg_tmp &= ~(SWR_MASK_REG_ZCDC_H);
	reg_tmp |= SWR_REG_ZCDC_H(0x3);
	SWR->SWR_ON_CTRL0 = reg_tmp;

	/* Bypass C-cut ZCD ECO */
	reg_tmp = SWR->SWR_DIG_ZCD;
	reg_tmp |= BIT0;
	SWR->SWR_DIG_ZCD = reg_tmp;

	reg_tmp = SWR->SWR_NEW;
	reg_tmp |= SWR_REG_SWR_ZCD_CTRL_PFM(0x1);
	SWR->SWR_NEW = reg_tmp;

	/* load VOL_SWR_AUD_PWM */
	Swr_Calib_AUD_PWM_Times = SWR_Calib_Check(Swr_AUD_PWM);

	if (Swr_Calib_AUD_PWM_Times != Swr_Calib_Nocalib) {
		OTP_Read8(Audio_PWM_Addr, &Audio_PWM);
		Audio_PWM_Value = SWR_Calib_Get_Value(Audio_PWM, Swr_AUD_PWM, Width_4Bit);
		reg_tmp = SWR->SWR_ON_CTRL0;
		reg_tmp &= ~(SWR_MASK_VOL_L1);
		reg_tmp |= SWR_VOL_L1(Audio_PWM_Value);
		SWR->SWR_ON_CTRL0 = reg_tmp;

	}

	/* load VOL_SWR_AUD_PFM */
	Swr_Calib_AUD_PFM_Times = SWR_Calib_Check(Swr_AUD_PFM);

	if (Swr_Calib_AUD_PFM_Times != Swr_Calib_Nocalib) {
		OTP_Read8(Audio_PFM_Addr, &Audio_PFM);
		Audio_PFM_Value = SWR_Calib_Get_Value(Audio_PFM, Swr_AUD_PFM, Width_4Bit);
		reg_tmp = SWR->SWR_ON_CTRL0;
		reg_tmp &= ~(SWR_MASK_VREFPFM_L);
		reg_tmp |= SWR_VREFPFM_L(Audio_PFM_Value);
		SWR->SWR_ON_CTRL0 = reg_tmp;
	}
}
