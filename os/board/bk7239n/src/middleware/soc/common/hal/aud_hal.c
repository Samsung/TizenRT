// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <common/bk_include.h>
#include "aud_hal.h"
//#include "aud_ll_macro_def.h"
//#include <driver/aud_types.h>


//#include <driver/aud_adc_types.h>
//#include <driver/aud_dac_types.h>



bk_err_t aud_hal_adc_hpf_config(aud_adc_hpf_config_t *config)
{
	aud_hal_set_adc_config1_adc_hpf2_coef_b0(config->adc_hpf2_coef_B0);
	aud_hal_set_adc_config1_adc_hpf2_coef_b1(config->adc_hpf2_coef_B1);
	aud_hal_set_adc_config0_adc_hpf2_coef_b2(config->adc_hpf2_coef_B2);

	aud_hal_set_adc_config2_adc_hpf2_coef_a0(config->adc_hpf2_coef_A0);
	aud_hal_set_adc_config2_adc_hpf2_coef_a1(config->adc_hpf2_coef_A1);

	aud_hal_set_adc_config0_adc_hpf2_bypass(config->adc_hpf2_bypass_enable);
	aud_hal_set_adc_config0_adc_hpf1_bypass(config->adc_hpf1_bypass_enable);

	return BK_OK;
}

bk_err_t aud_hal_adc_agc_config(aud_adc_agc_config_t *config)
{
	aud_hal_set_agc_config0_agc_noise_thrd(config->agc_noise_thrd);
	aud_hal_set_agc_config0_agc_noise_high(config->agc_noise_high);
	aud_hal_set_agc_config0_agc_noise_low(config->agc_noise_low);

	aud_hal_set_agc_config1_agc_noise_min(config->agc_noise_min);
	aud_hal_set_agc_config1_agc_noise_tout(config->agc_noise_tout);
	aud_hal_set_agc_config1_agc_high_dur(config->agc_high_dur);
	aud_hal_set_agc_config1_agc_low_dur(config->agc_low_dur);
	aud_hal_set_agc_config1_agc_min(config->agc_min);
	aud_hal_set_agc_config1_agc_max(config->agc_max);
	aud_hal_set_agc_config1_agc_ng_method(config->agc_ng_method);
	aud_hal_set_agc_config1_agc_ng_enable(config->agc_ng_enable);

	aud_hal_set_agc_config2_agc_decay_time(config->agc_decay_time);
	aud_hal_set_agc_config2_agc_attack_time(config->agc_attack_time);
	aud_hal_set_agc_config2_agc_high_thrd(config->agc_high_thrd);
	aud_hal_set_agc_config2_agc_low_thrd(config->agc_low_thrd);
	aud_hal_set_agc_config2_agc_iir_coef(config->agc_iir_coef);
	aud_hal_set_agc_config2_agc_enable(config->agc_enable);
	aud_hal_set_agc_config2_manual_pga_value(config->manual_pga_value);
	aud_hal_set_agc_config2_manual_pga(config->manual_pga_enable);

	return BK_OK;
}


bk_err_t aud_hal_dac_hpf_config(aud_dac_hpf_config_t *config)
{
	aud_hal_set_dac_config1_dac_hpf2_coef_b0(config->dac_hpf2_coef_B0);
	aud_hal_set_dac_config1_dac_hpf2_coef_b1(config->dac_hpf2_coef_B1);
	aud_hal_set_dac_config0_dac_hpf2_coef_b2(config->dac_hpf2_coef_B2);

	aud_hal_set_dac_config2_dac_hpf2_coef_a1(config->dac_hpf2_coef_B0);
	aud_hal_set_dac_config2_dac_hpf2_coef_a2(config->dac_hpf2_coef_B0);

	if (config->dac_hpf2_bypass_enable == AUD_DAC_HPF_BYPASS_ENABLE) {
		aud_hal_set_dac_config0_dac_hpf2_bypass(1);
	} else {
		aud_hal_set_dac_config0_dac_hpf2_bypass(0);
	}

	if (config->dac_hpf1_bypass_enable == AUD_DAC_HPF_BYPASS_ENABLE) {
		aud_hal_set_dac_config0_dac_hpf1_bypass(1);
	} else {
		aud_hal_set_dac_config0_dac_hpf1_bypass(0);
	}

	return BK_OK;
}

bk_err_t aud_hal_dac_filt_config(aud_dac_eq_config_t *config)
{
	aud_hal_set_flt0_coef_a1a2_flt0_a1((config->flt0_A1 >> 6) & 0xFFFF);
	aud_hal_set_flt0_ext_coef_flt0_a1_l6bit((config->flt0_A1) & 0x3F);
	aud_hal_set_flt0_coef_a1a2_flt0_a2((config->flt0_A2 >> 6) & 0xFFFF);
	aud_hal_set_flt0_ext_coef_flt0_a2_l6bit((config->flt0_A2) & 0x3F);
	aud_hal_set_flt0_coef_b0b1_flt0_b0((config->flt0_B0 >> 6) & 0xFFFF);
	aud_hal_set_flt0_ext_coef_flt0_b0_l6bit((config->flt0_B0) & 0x3F);
	aud_hal_set_flt0_coef_b0b1_flt0_b1((config->flt0_B1 >> 6) & 0xFFFF);
	aud_hal_set_flt0_ext_coef_flt0_b1_l6bit((config->flt0_B1) & 0x3F);
	aud_hal_set_flt0_coef_b2_flt0_b2((config->flt0_B2 >> 6) & 0xFFFF);
	aud_hal_set_flt0_ext_coef_flt0_b2_l6bit((config->flt0_B2) & 0x3F);

	aud_hal_set_flt1_coef_a1a2_flt1_a1((config->flt1_A1 >> 6) & 0xFFFF);
	aud_hal_set_flt1_ext_coef_flt1_a1_l6bit((config->flt1_A1) & 0x3F);
	aud_hal_set_flt1_coef_a1a2_flt1_a2((config->flt1_A2 >> 6) & 0xFFFF);
	aud_hal_set_flt1_ext_coef_flt1_a2_l6bit((config->flt1_A2) & 0x3F);
	aud_hal_set_flt1_coef_b0b1_flt1_b0((config->flt1_B0 >> 6) & 0xFFFF);
	aud_hal_set_flt1_ext_coef_flt1_b0_l6bit((config->flt1_B0) & 0x3F);
	aud_hal_set_flt1_coef_b0b1_flt1_b1((config->flt1_B1 >> 6) & 0xFFFF);
	aud_hal_set_flt1_ext_coef_flt1_b1_l6bit((config->flt1_B1) & 0x3F);
	aud_hal_set_flt1_coef_b2_flt1_b2((config->flt1_B2 >> 6) & 0xFFFF);
	aud_hal_set_flt1_ext_coef_flt1_b2_l6bit((config->flt1_B2) & 0x3F);

	aud_hal_set_flt2_coef_a1a2_flt2_a1((config->flt2_A1 >> 6) & 0xFFFF);
	aud_hal_set_flt2_ext_coef_flt2_a1_l6bit((config->flt2_A1) & 0x3F);
	aud_hal_set_flt2_coef_a1a2_flt2_a2((config->flt2_A2 >> 6) & 0xFFFF);
	aud_hal_set_flt2_ext_coef_flt2_a2_l6bit((config->flt2_A2) & 0x3F);
	aud_hal_set_flt2_coef_b0b1_flt2_b0((config->flt2_B0 >> 6) & 0xFFFF);
	aud_hal_set_flt2_ext_coef_flt2_b0_l6bit((config->flt2_B0) & 0x3F);
	aud_hal_set_flt2_coef_b0b1_flt2_b1((config->flt2_B1 >> 6) & 0xFFFF);
	aud_hal_set_flt2_ext_coef_flt2_b1_l6bit((config->flt2_B1) & 0x3F);
	aud_hal_set_flt2_coef_b2_flt2_b2((config->flt2_B2 >> 6) & 0xFFFF);
	aud_hal_set_flt2_ext_coef_flt2_b2_l6bit((config->flt2_B2) & 0x3F);

	aud_hal_set_flt3_coef_a1a2_flt3_a1((config->flt3_A1 >> 6) & 0xFFFF);
	aud_hal_set_flt3_ext_coef_flt3_a1_l6bit((config->flt3_A1) & 0x3F);
	aud_hal_set_flt3_coef_a1a2_flt3_a2((config->flt3_A2 >> 6) & 0xFFFF);
	aud_hal_set_flt3_ext_coef_flt3_a2_l6bit((config->flt3_A2) & 0x3F);
	aud_hal_set_flt3_coef_b0b1_flt3_b0((config->flt3_B0 >> 6) & 0xFFFF);
	aud_hal_set_flt3_ext_coef_flt3_b0_l6bit((config->flt3_B0) & 0x3F);
	aud_hal_set_flt3_coef_b0b1_flt3_b1((config->flt3_B1 >> 6) & 0xFFFF);
	aud_hal_set_flt3_ext_coef_flt3_b1_l6bit((config->flt3_B1) & 0x3F);
	aud_hal_set_flt3_coef_b2_flt3_b2((config->flt3_B2 >> 6) & 0xFFFF);
	aud_hal_set_flt3_ext_coef_flt3_b2_l6bit((config->flt3_B2) & 0x3F);

	return BK_OK;
}

bk_err_t aud_hal_dtmf_config(aud_dtmf_config_t *config)
{
	/* dtmf_config0 */
	aud_hal_set_dtmf_config0_tone_pattern(config->tone_pattern);
	aud_hal_set_dtmf_config0_tone_mode(config->tone_mode);
	aud_hal_set_dtmf_config0_tone_pause_time(config->tone_pause_time);
	aud_hal_set_dtmf_config0_tone_active_time(config->tone_active_time);

	/* dtmf_config1 */
	aud_hal_set_dtmf_config1_tone1_step(config->tone1_step);
	aud_hal_set_dtmf_config1_tone1_attu(config->tone1_attu);

	/* dtmf_config2 */
	aud_hal_set_dtmf_config2_tone2_step(config->tone2_step);
	aud_hal_set_dtmf_config2_tone2_attu(config->tone2_attu);

	return BK_OK;
}

/* get adc fifo port address */
bk_err_t aud_hal_adc_get_fifo_addr(uint32_t *adc_fifo_addr)
{
	//*adc_fifo_addr = aud_ll_get_adc_fifo_addr();
	*adc_fifo_addr = AUD_ADC_FPORT_ADDR;
	return BK_OK;
}

/* get dac fifo port address */
bk_err_t aud_hal_dac_get_fifo_addr(uint32_t *dac_fifo_addr)
{
	//*dac_fifo_addr = aud_ll_get_dac_fifo_addr();
	*dac_fifo_addr = AUD_DAC_FPORT_ADDR;
	return BK_OK;
}

/* get dtmf fifo port address */
bk_err_t aud_hal_dtmf_get_fifo_addr(uint32_t *dtmf_fifo_addr)
{
	//*dtmf_fifo_addr = aud_ll_get_dtmf_fifo_addr();
	*dtmf_fifo_addr = AUD_DTMF_FPORT_ADDR;
	return BK_OK;
}

/* get dmic fifo port address */
bk_err_t aud_hal_dmic_get_fifo_addr(uint32_t *dmic_fifo_addr)
{
	//*dmic_fifo_addr = aud_ll_get_dmic_fifo_addr();
	*dmic_fifo_addr = AUD_DMIC_FPORT_ADDR;
	return BK_OK;
}


