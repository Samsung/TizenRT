/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"

static u32 SWR_IDX_FLAG[7] = {
	SEC_VOL_SWR_PFM_125,	/*SWR PFM 1.25 in 0x7E0*/
	SEC_VOL_SWR_PWM_135,	/*SWR PWM 1.35 in 0x7E1*/
	SEC_VOL_SWR_PFM_09,	/*SWR PFM 0.9 in 0x7E2*/
	SEC_VOL_SWR_PWM_10,	/*SWR PWM 1.0 in 0x7E3*/
	SEC_VOL_AON_LDO_09,		/*AON LDO 0.9 in 0x7E4*/
	SEC_VOL_CORE_LDO_10,	/*CORE LDO 1.0 in 0x7E5*/
	SEC_VOL_ANA_LDO_180,	/*ANA LDO 1.8 in 0x7E6*/

};

/**
  * @brief Get actual calibration value.
  * @param swr_efuse_value: data read from otp
  * @param swr_type: swr type, this parameter can be one parameter from IS_SWR_Type.
  * @param width: width, this parameter can be one parameter from IS_SWR_CALIB_BitWith.
  * @retval actual calibration value.
  */
u8 SWR_Calib_Get_Value(u8 swr_idx)
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

/**
  * @brief Set calib value step by step.
  * @param swr_idx: parameter from SWR_IDX_FLAG
  * @param vol_initial: initial value.
  * @param vol_end: end value.
  * @param dir: ReduceVol or RaiseVol
  */
static void SWR_Calib_Vol_Set(u8 swr_idx, u8 vol_initial, u8 vol_end, u8 dir)
{
	u32 RTemp;
	u8 i;
	REGU_TypeDef *regu = REGU_BASE;

	if (dir == RaiseVol) {
		/* raise voltage step by step*/
		for (i = vol_initial + 1; i <= vol_end; i++) {
			if (swr_idx == SWR_PWM_135) {
				RTemp = regu->REGU_SWR_NEW;
				RTemp &= ~(REGU_MASK_VOL_L1_SET1);
				RTemp |= REGU_VOL_L1_SET1(i);
				regu->REGU_SWR_NEW = RTemp;
			} else if (swr_idx == CORE_LDO_10) {
				RTemp = regu->REGU_LDO_1209_CTRL;
				RTemp &= ~(REGU_MASK_VOADJ_L_1209);
				RTemp |= REGU_VOADJ_L_1209(i);
				regu->REGU_LDO_1209_CTRL = RTemp;
			} else if (swr_idx == SWR_PWM_10) {
				RTemp = regu->REGU_SWR_ON_CTRL0;
				RTemp &= ~(REGU_MASK_VOL_L1);
				RTemp |= REGU_VOL_L1(i);
				regu->REGU_SWR_ON_CTRL0 = RTemp;
			} else {
				return;
			}
		}

	} else {
		/* reduce voltage step by step*/
		for (i = vol_initial; i >= vol_end; i--) {
			if (swr_idx == CORE_LDO_10) {
				RTemp = regu->REGU_LDO_1209_CTRL;
				RTemp &= ~(REGU_MASK_VOADJ_L_1209);
				RTemp |= REGU_VOADJ_L_1209(i);
				regu->REGU_LDO_1209_CTRL = RTemp;
			} else if (swr_idx == SWR_PWM_135) {
				RTemp = regu->REGU_SWR_NEW;
				RTemp &= ~(REGU_MASK_VOL_L1_SET1);
				RTemp |= REGU_VOL_L1_SET1(i);
				regu->REGU_SWR_NEW = RTemp;
			} else if (swr_idx == SWR_PWM_10) {
				RTemp = regu->REGU_SWR_ON_CTRL0;
				RTemp &= ~(REGU_MASK_VOL_L1);
				RTemp |= REGU_VOL_L1(i);
				regu->REGU_SWR_ON_CTRL0 = RTemp;
			} else {
				return;
			}
		}
	}
}

u8 SWR_Calib_Get_Value_AUD(void)
{
	u8 audio_vref_f_data = 0;
	u8 audio_vref_s_data = 0;

	EFUSE_PMAP_READ8(0, SEC_VOL_AUDIO_VREF_1V8_F, &audio_vref_f_data, L25EOUTVOLTAGE);
	EFUSE_PMAP_READ8(0, SEC_VOL_AUDIO_VREF_1V8_S, &audio_vref_s_data, L25EOUTVOLTAGE);

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

	if (temp & REGU_BIT_SWR_BOOT_SET_SEL) {
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

		if (regu->REGU_LDO_1209_CTRL & BIT(5)) {
			regu->REGU_DUMMY |= (BIT(9));
		} else {
			regu->REGU_DUMMY &= ~(BIT(9));
		}

		if (regu->REGU_LDO_1209_CTRL & BIT(4)) {
			regu->REGU_DUMMY |= (BIT(1));
		} else {
			regu->REGU_DUMMY &= ~(BIT(1));
		}

		if (regu->REGU_LDO_1209_CTRL & BIT(3)) {
			regu->REGU_DUMMY |= (BIT(8));
		} else {
			regu->REGU_DUMMY &= ~(BIT(8));
		}

		if (regu->REGU_LDO_1209_CTRL & BIT(2)) {
			regu->REGU_DUMMY |= (BIT(0));
		} else {
			regu->REGU_DUMMY &= ~(BIT(0));
		}

	} else {
		/* power supply mode 0 */

		Value = SWR_Calib_Get_Value(SWR_PFM_09);
		Rtemp = regu->REGU_SWR_ON_CTRL0;
		Rtemp &= ~(REGU_MASK_VREFPFM_L);

		if (Value != Swr_Calib_Nocalib) {
			Rtemp |= REGU_VREFPFM_L(Value);
		} else {
			Rtemp |= REGU_VREFPFM_L(0x3);
		}
		regu->REGU_SWR_ON_CTRL0 = Rtemp;

		regu->REGU_DUMMY &= ~(BIT(6));
	}
}

NON_DRAM_TEXT_SECTION
void SWR_Calib_RaiseVoltage(void)
{
	u32 temp, Rtemp, VolTemp;
	u8 RRamTemp;
	REGU_TypeDef *regu = REGU_BASE;
	RRAM_TypeDef *rram = RRAM_DEV;

	/* check power mode */
	temp = regu->REGU_POWER_CTRL;
	if (temp & REGU_BIT_SWR_BOOT_SET_SEL) {
		/* power supply mode 1 */

		/* store pwm 1.25 in RRAM for Reduce Voltage */
		VolTemp = REGU_GET_VOL_L1_SET1(regu->REGU_SWR_NEW);
		rram->SWRCALIB_PWM_125 = VolTemp;

		if (rram->SWRCALIB_PWM_135 == 0) {
			/* pwm 1.35 uninitialised, fetch from otp and store in RRAM*/
			RRamTemp = SWR_Calib_Get_Value(SWR_PWM_135);
			if (RRamTemp != Swr_Calib_Nocalib) {
				rram->SWRCALIB_PWM_135 = RRamTemp;
			} else {
				rram->SWRCALIB_PWM_135 = 0xB;
			}
		}
		/* switch to pwm 1.35 */
		SWR_Calib_Vol_Set(SWR_PWM_135, VolTemp, rram->SWRCALIB_PWM_135, RaiseVol);

		/* store core ldo 0.9 in RRAM for Reduce Voltage */
		VolTemp = REGU_GET_VOADJ_L_1209(regu->REGU_LDO_1209_CTRL);
		rram->SWRCALIB_CORE_LDO_09 = VolTemp;

		if (rram->SWRCALIB_CORE_LDO_10 == 0) {
			/* core_ldo 1.0 uninitialised, fetch from otp and store in RRAM*/
			RRamTemp = SWR_Calib_Get_Value(CORE_LDO_10);
			if (RRamTemp != Swr_Calib_Nocalib) {
				rram->SWRCALIB_CORE_LDO_10 = RRamTemp;
			} else {
				rram->SWRCALIB_CORE_LDO_10 = 0xB;
			}
		}
		/* switch to core ldo 1.0 */
		SWR_Calib_Vol_Set(CORE_LDO_10, VolTemp, rram->SWRCALIB_CORE_LDO_10, RaiseVol);

	} else {
		/* power supply mode 0 */

		/* store pwm 0.9 in RRAM for Reduce Voltage */
		VolTemp = REGU_GET_VOL_L1(regu->REGU_SWR_ON_CTRL0);
		rram->SWRCALIB_PWM_09 = VolTemp;

		if (rram->SWRCALIB_PWM_10 == 0) {
			/* pwm 1.0 uninitialised, fetch from otp and store in RRAM*/
			RRamTemp = SWR_Calib_Get_Value(SWR_PWM_10);
			if (RRamTemp != Swr_Calib_Nocalib) {
				rram->SWRCALIB_PWM_10 = RRamTemp;
			} else {
				rram->SWRCALIB_PWM_10 = 0xC;
			}
		}
		SWR_Calib_Vol_Set(SWR_PWM_10, VolTemp, rram->SWRCALIB_PWM_10, RaiseVol);

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
	u32 temp, Rtemp, VolTemp;
	REGU_TypeDef *regu = REGU_BASE;
	RRAM_TypeDef *rram = RRAM_DEV;

	/* check power mode */
	temp = regu->REGU_POWER_CTRL;
	if (temp & REGU_BIT_SWR_BOOT_SET_SEL) {
		/* power supply mode 1 */

		/* restore core ldo 0.9 from RRAM */
		VolTemp = REGU_GET_VOADJ_L_1209(regu->REGU_LDO_1209_CTRL);
		SWR_Calib_Vol_Set(CORE_LDO_10, VolTemp, rram->SWRCALIB_CORE_LDO_09, ReduceVol);

		/* restore swr pwm 1.25 from RRAM */
		VolTemp = REGU_GET_VOL_L1_SET1(regu->REGU_SWR_NEW);
		SWR_Calib_Vol_Set(SWR_PWM_135, VolTemp, rram->SWRCALIB_PWM_125, ReduceVol);
	} else {
		/* power supply mode 0 */

		/* restore swr pwm 0.9 from RRAM */
		VolTemp = REGU_GET_VOL_L1(regu->REGU_SWR_ON_CTRL0);
		SWR_Calib_Vol_Set(SWR_PWM_10, VolTemp, rram->SWRCALIB_PWM_09, ReduceVol);
	}

	/*zcd ioffset*/
	Rtemp = regu->REGU_SWR_ON_CTRL2;
	Rtemp &= ~(REGU_MASK_ZCD_IOFFSET_B);
	Rtemp |= REGU_ZCD_IOFFSET_B(0x2);
	regu->REGU_SWR_ON_CTRL2 = Rtemp;

}
/**
  * @brief  Set Core voltage Type.
  * @param  Source: This parameter can be one of the following values:
  *		 @arg CORE_VOL_0P9
  *		 @arg CORE_VOL_1P0
  */
NON_DRAM_TEXT_SECTION
void SWR_CORE_Vol_Set(u8 vol_type)
{
	RRAM_TypeDef *rram = RRAM_DEV;
	rram->CORE_Vol_Flag = vol_type;

	SWR_Calib_Load();

	if (CORE_VOL_1P0 == vol_type) {
		SWR_Calib_RaiseVoltage();
	}
}

NON_DRAM_TEXT_SECTION
u8 SWR_CORE_Vol_Get(void)
{
	RRAM_TypeDef *rram = RRAM_DEV;
	return rram->CORE_Vol_Flag;
}

