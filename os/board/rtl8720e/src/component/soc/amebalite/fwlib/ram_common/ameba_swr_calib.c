/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static u8 aon_ldo_09_global = 0;
static u8 swr_pwm_135_global = 0;
static u8 core_ldo_10_global = 0;
static u8 swr_pwm_10_global = 0;

static u32 SWR_IDX_FLAG[7] = {
	VOL_SWR_PFM_125,	/*SWR PFM 1.25 in 0x7E0*/
	VOL_SWR_PWM_135,	/*SWR PWM 1.35 in 0x7E1*/
	VOL_SWR_PFM_09,	/*SWR PFM 0.9 in 0x7E2*/
	VOL_SWR_PWM_10,	/*SWR PWM 1.0 in 0x7E3*/
	VOL_AON_LDO_09,		/*AON LDO 0.9 in 0x7E4*/
	VOL_CORE_LDO_10,	/*CORE LDO 1.0 in 0x7E5*/
	VOL_ANA_LDO_180,	/*ANA LDO 1.8 in 0x7E6*/

};

/**
  * @brief Get actual calibration value.
  * @param swr_efuse_value: data read from otp
  * @param swr_type: swr type, this parameter can be one parameter from IS_SWR_Type.
  * @param width: width, this parameter can be one parameter from IS_SWR_CALIB_BitWith.
  * @retval actual calibration value.
  */
static u8 SWR_Calib_Get_Value(u8 swr_idx)
{
	u32 idx = 0;
	u32 Check_Addr = 0;
	u8 swr_data = 0;

	assert_param(IS_SWR_CALIB_VOLIDX(swr_idx));

	idx = swr_idx;
	Check_Addr = SWR_IDX_FLAG[idx];

	EFUSE_PMAP_READ8(0, Check_Addr, &swr_data, L25EOUTVOLTAGE);

	if ((swr_data & 0xF0) != 0xF0) {
		return (u8)(SWR_Calib_Twocalib_4Bit_Value(swr_data)); //calibration two times
	} else if ((swr_data & 0x0F) != 0xF) {
		return (u8)(SWR_Calib_Onecalib_4Bit_Value(swr_data)); //calibration only one time
	} else {
		return (u8)Swr_Calib_Nocalib; // no calibration yet
	}
}

u8 SWR_Calib_Get_Value_AUD(void)
{
	u8 audio_vref_f_data = 0;
	u8 audio_vref_s_data = 0;

	EFUSE_PMAP_READ8(0, VOL_AUDIO_VREF_1V8_F, &audio_vref_f_data, L25EOUTVOLTAGE);
	EFUSE_PMAP_READ8(0, VOL_AUDIO_VREF_1V8_S, &audio_vref_s_data, L25EOUTVOLTAGE);

	if ((audio_vref_s_data & 0x1F) != 0x1F) {
		return (u8)(AUDIO_Calib_5Bit_Value(audio_vref_s_data)); //calibration two times
	} else if ((audio_vref_f_data & 0x1F) != 0x1F) {
		return (u8)(AUDIO_Calib_5Bit_Value(audio_vref_f_data)); //calibration only one times
	} else {
		return (u8)Swr_Calib_Nocalib; // no calibration yet;
	}
}

NON_DRAM_TEXT_SECTION
void SWR_Calib_Load(void)
{
	u32 temp, Rtemp;
	u8 Value;
	REGU_TypeDef *regu = REGU_BASE;

	temp = regu->REGU_POWER_CTRL;

	if (temp | REGU_BIT_SWR_BOOT_SET_SEL) {
		/* power supply mode 1 */

		/* zcdc = 2'b11*/
		Rtemp = regu->REGU_SWR_ON_CTRL0;
		Rtemp &= ~(REGU_MASK_REG_ZCDC_H);
		Rtemp |= REGU_REG_ZCDC_H(0x3);
		regu->REGU_SWR_ON_CTRL0 = Rtemp;

		/* PFM125 load if OTP programmed */
		Value = SWR_Calib_Get_Value(SWR_PFM_125);
		if (Value != Swr_Calib_Nocalib) {
			Rtemp = regu->REGU_SWR_ON_CTRL0;
			Rtemp &= ~(REGU_MASK_VREFPFM_L);
			Rtemp |= REGU_VREFPFM_L(Value);
			regu->REGU_SWR_ON_CTRL0 = Rtemp;
		}
	} else {
		/* power supply mode 0 */

		Value = SWR_Calib_Get_Value(SWR_PFM_09);
		if (Value != Swr_Calib_Nocalib) {
			Rtemp = regu->REGU_SWR_ON_CTRL0;
			Rtemp &= ~(REGU_MASK_VREFPFM_L);
			Rtemp |= REGU_VREFPFM_L(Value);
			regu->REGU_SWR_ON_CTRL0 = Rtemp;
		}
	}
}

NON_DRAM_TEXT_SECTION
void SWR_Calib_RaiseVoltage(void)
{
	u32 temp, Rtemp;
	u8 Value;
	REGU_TypeDef *regu = REGU_BASE;

	/* aon_ldo 0.9 */
	aon_ldo_09_global = REGU_GET_AONLDO_VOSEL_H(regu->REGU_AON_LDO_CTRL);
	Value = SWR_Calib_Get_Value(AON_LDO_09);

	Rtemp = regu->REGU_AON_LDO_CTRL;
	Rtemp &= ~(REGU_MASK_AONLDO_VOSEL_H);
	if (Value != Swr_Calib_Nocalib) {
		Rtemp |= REGU_AONLDO_VOSEL_H(Value);
	} else {
		Rtemp |= REGU_AONLDO_VOSEL_H(aon_ldo_09_global + 4);
	}
	regu->REGU_AON_LDO_CTRL = Rtemp;

	/* check power mode */
	temp = regu->REGU_POWER_CTRL;
	if (temp | REGU_BIT_SWR_BOOT_SET_SEL) {
		/* power supply mode 1 */

		/* swr pwm 1.35 */
		swr_pwm_135_global = REGU_GET_VOL_L1_SET1(regu->REGU_SWR_NEW);
		Value = SWR_Calib_Get_Value(SWR_PWM_135);

		Rtemp = regu->REGU_SWR_NEW;
		Rtemp &= ~(REGU_MASK_VOL_L1_SET1);
		if (Value != Swr_Calib_Nocalib) {
			Rtemp |= REGU_VOL_L1_SET1(Value);
		} else {
			Rtemp |= REGU_VOL_L1_SET1(0xB);
		}
		regu->REGU_SWR_NEW = Rtemp;

		/* core ldo 1.0 */
		core_ldo_10_global = REGU_GET_VOADJ_L_1209(regu->REGU_LDO_1209_CTRL);
		Value = SWR_Calib_Get_Value(SWR_PWM_135);

		Rtemp = regu->REGU_LDO_1209_CTRL;
		Rtemp &= ~(REGU_MASK_VOADJ_L_1209);
		if (Value != Swr_Calib_Nocalib) {
			Rtemp |= REGU_VOADJ_L_1209(Value);
		} else {
			Rtemp |= REGU_VOADJ_L_1209(0xB);
		}
		regu->REGU_LDO_1209_CTRL = Rtemp;

	} else {
		/* power supply mode 0 */

		/* swr pwm 1.0 */
		swr_pwm_10_global = REGU_GET_VOL_L1(regu->REGU_SWR_ON_CTRL0);
		Value = SWR_Calib_Get_Value(SWR_PWM_10);

		Rtemp = regu->REGU_SWR_ON_CTRL0;
		Rtemp &= ~(REGU_MASK_VOL_L1);
		if (Value != Swr_Calib_Nocalib) {
			Rtemp |= REGU_VOL_L1(Value);
		} else {
			Rtemp |= REGU_VOL_L1(0xC);
		}
		regu->REGU_SWR_ON_CTRL0 = Rtemp;

	}

	/*zcd ioffset*/
	Rtemp = regu->REGU_SWR_ON_CTRL2;
	Rtemp &= ~(REGU_MASK_ZCD_IOFFSET_B);
	Rtemp |= REGU_ZCD_IOFFSET_B(0x3);
	regu->REGU_SWR_ON_CTRL2 = Rtemp;

}

NON_DRAM_TEXT_SECTION
void SWR_Calib_ReduceVoltage(void)
{
	u32 temp, Rtemp;
	REGU_TypeDef *regu = REGU_BASE;

	/* check power mode */
	temp = regu->REGU_POWER_CTRL;
	if (temp | REGU_BIT_SWR_BOOT_SET_SEL) {
		/* power supply mode 1 */
		/* core ldo */
		Rtemp = regu->REGU_LDO_1209_CTRL;
		Rtemp &= ~(REGU_MASK_VOADJ_L_1209);
		Rtemp |= REGU_VOADJ_L_1209(core_ldo_10_global);
		regu->REGU_LDO_1209_CTRL = Rtemp;

		/* swr pwm 1.35 */
		Rtemp = regu->REGU_SWR_NEW;
		Rtemp &= ~(REGU_MASK_VOL_L1_SET1);
		Rtemp |= REGU_VOL_L1_SET1(swr_pwm_135_global);
		regu->REGU_SWR_NEW = Rtemp;
	} else {
		/* power supply mode 0 */

		/* swr pwm 1.0 */
		Rtemp = regu->REGU_SWR_ON_CTRL0;
		Rtemp &= ~(REGU_MASK_VOL_L1);
		Rtemp |= REGU_VOL_L1(swr_pwm_10_global);
		regu->REGU_SWR_ON_CTRL0 = Rtemp;
	}

	/* aon_ldo 0.9 */
	Rtemp = regu->REGU_AON_LDO_CTRL;
	Rtemp &= ~(REGU_MASK_AONLDO_VOSEL_H);
	Rtemp |= REGU_AONLDO_VOSEL_H(aon_ldo_09_global);
	regu->REGU_AON_LDO_CTRL = Rtemp;

	/*zcd ioffset*/
	Rtemp = regu->REGU_SWR_ON_CTRL2;
	Rtemp &= ~(REGU_MASK_ZCD_IOFFSET_B);
	Rtemp |= REGU_ZCD_IOFFSET_B(0x2);
	regu->REGU_SWR_ON_CTRL2 = Rtemp;

}