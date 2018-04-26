/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef __DRIVERS_AUDIO_ALC5658SCRIPTS_H
#define __DRIVERS_AUDIO_ALC5658SCRIPTS_H

#ifdef CONFIG_AUDIO_ALC5658

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "alc5658reg.h"

/* TYPEDEFS */
typedef struct {
	uint16_t addr;
	uint16_t val;
	unsigned int delay;
} t_codec_init_script_entry;

t_codec_init_script_entry codec_reset_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},	/* Reset */
};

t_codec_init_script_entry codec_stop_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},	/* Reset */
	{ALC5658_I2S1_PORT_CTRL, 0x8000, 0},	/* Switch into slave to stop data transfer */
};

t_codec_init_script_entry codec_init_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},	/* Reset */
	{ALC5658_006E, 0xFFFF, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_006F, 0xFFFF, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_CLOCK, 0x8000, 0},	/* RC CLK, No dividers */
	{ALC5658_MICBIAS2, 0x0280, 0},	/* Enable ALL int CLK, even more than in documentation */
	{ALC5658_0111, 0xA502, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_0125, 0x0430, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_RESET1, 0x3020, 0},	/* DAC1, DAC2 + alpha clock enable */
	{ALC5658_ADCDAC_CLOCK, 0x1770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALC5658_I2S1_PORT_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ALC5658_007B, 0x0003, 0},	/* Select 64*FS for BCLK in master mode, No Info in documentation  */
	{ALC5658_GENERAL_CONTROL_1, 0x0001, 0},	/* Enable Gate mode, Use(pass) Noise Gain Mode2 CTRL */
	{ALC5658_0091, 0x0C16, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_MANAGEMENT3, 0xA23E, 60},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT3, 0xF23E, 50},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT2, 0x0400, 50},	/* pow_dac_stereo1_filter ON */
	{ALC5658_MANAGEMENT1, 0x8080, 10},	/* en_i2s1, Pow_ldo_dacref ON */
	{ALC5658_ADC_2_MXR, 0x8080, 0},	/* Mu_stereo1_adc_mixer_l/r MUTE */
	{ALC5658_DAC_ST_MXR, 0xAAAA, 0},	/* Default, mute all */
	{ALC5658_DAC_SRC, 0x0000, 0},	/* Default, no mixers (direct) */
	{ALC5658_HP, 0x0009, 50},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ALC5658_MANAGEMENT1, 0x8C80, 50},	/* en_i2s1, pow_dac1_l/r, Pow_ldo_dacref - ON */
	{ALC5658_0091, 0x0E16, 50},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_0040, 0x0505, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_MANAGEMENT5, 0x0180, 0},	/* Does not match with documentation */
	{ALC5658_013C, 0x3C05, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DF, 0x02C1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DE, 0x5100, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01E4, 0x0014, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DE, 0xD100, 30},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DE, 0x4900, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01E4, 0x0016, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DE, 0xC900, 250},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_HPOUT, 0x0000, 0},	/* UNMUTE HP Output */
	{ALC5658_01DE, 0x4500, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01E4, 0x001F, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DE, 0xC500, 800},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_0040, 0x0808, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_MANAGEMENT5, 0x0000, 0},	/* Default, PLL, LDO2, Speaker VDD off */
	{ALC5658_013C, 0x2005, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01E4, 0x0000, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_01DF, 0x20C0, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALC5658_ADCDAC_CLOCK, 0x0770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALC5658_CLOCK, 0x0000, 0},	/* MCLK, No dividers */
	{ALC5658_NOISE_GATE, 0x8EC0, 0},	/* Stereo_noise_gate_mode2_en - ENABLE */
	{ALC5658_HP, 0x0019, 0},	/* en_out_hp - ON, pow_pump_hp - ON, pow_capless - ON  */
	{ALC5658_NOISE_GATE1, 0xC0F0, 0},	/* Noise_gate_mode1_en, Noise_gate_mode1_auto_en - EN, Noise_gate_mode1_threshold -78dB, DOES NOT MATCH WITH DOCUMENTATION !!! */
	{ALC5658_NOISE_GATE1, 0x87F9, 0},	/* Blah blah blah, eventually enable Noise gate function >:( */
	{ALC5658_MICBIAS2, 0x0180, 0},	/* Probably enable Pow_int_clk1/2, does not match with DOC !!! */
	{ALC5658_GENERAL_CONTROL_2, 0x3000, 0},	/* Noise_gate_mode1/2_hp enable, DOES NOT MATCH WITH DOCUMENTATION */
};

t_codec_init_script_entry codec_initial_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},
	{ALC5658_0111, 0xA502, 0},
	{ALC5658_RESET1, 0x3030, 0},	/* DAC1, DAC2 + alpha clock enable */
	{ALC5658_006E, 0xEF00, 0},
	{ALC5658_006F, 0xEFFC, 0},
	{ALC5658_MICBIAS2, 0x0280, 0},	/* Enable ALL int CLK, even more than in documentation */
	{ALC5658_CLOCK, 0x8000, 0},	/* RC CLK, No dividers */
	{ALC5658_I2S1_PORT_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ALC5658_ADCDAC_CLOCK, 0x1770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALC5658_0091, 0x0C16, 0},
	{ALC5658_MANAGEMENT3, 0xAA7E, 60},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT3, 0xFE7E, 50},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT5, 0x0004, 0},	/* LDO2 ON */
	{ALC5658_MANAGEMENT2, 0x0400, 50},	/* pow_dac_stereo1_filter ON */
	{ALC5658_MANAGEMENT1, 0x0080, 10},	/* Pow_ldo_dacref ON */
	{ALC5658_ADC_2_MXR, 0x8080, 0},	/* Mu_stereo1_adc_mixer_l/r MUTE */
	{ALC5658_DAC_ST_MXR, 0xAAAA, 0},	/* Default, mute all */
	{ALC5658_DAC_SRC, 0x0000, 0},	/* Default, no mixers (direct) */
	{ALC5658_HP, 0x0009, 50},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ALC5658_MANAGEMENT1, 0x0F80, 50},	/* pow_dac1_l/r, pow_dac2_l/r, Pow_ldo_dacref - ON */
	{ALC5658_0091, 0x0E16, 50},
	{ALC5658_0040, 0x0505, 0},
	{ALC5658_MANAGEMENT5, 0x0184, 0},	/* NonDescribed,  LDO2 ON */
	{ALC5658_013C, 0x3C05, 0},
	{ALC5658_01DF, 0x2cc1, 6},
	{ALC5658_01DE, 0x5100, 6},
	{ALC5658_01E4, 0x0014, 6},
	{ALC5658_01DE, 0xd100, 30},
	{ALC5658_01DF, 0x20C1, 0},
	{ALC5658_01DF, 0x2CC1, 0},
	{ALC5658_01DE, 0x4900, 0},
	{ALC5658_01E4, 0x0016, 0},
	{ALC5658_01DE, 0xC900, 250},
	{ALC5658_01DF, 0x2CC1, 0},
	{ALC5658_HPOUT, 0x0000, 0},	/* UNMUTE HP Output */
	{ALC5658_01DE, 0x4500, 0},
	{ALC5658_01E4, 0x001F, 0},
	{ALC5658_01DE, 0xC500, 600},
	{ALC5658_01E4, 0x0000, 0},
	{ALC5658_01DF, 0x20C0, 0},
	{ALC5658_0040, 0x0808, 0},
	{ALC5658_MANAGEMENT5, 0x0000, 0},	/* LDO2 OFF */
	{ALC5658_013C, 0x2005, 0},
	{ALC5658_MANAGEMENT6, 0x0000, 0},	/* MIXERs OFF */
	{ALC5658_MANAGEMENT7, 0x0000, 0},	/* VOL Out/In ctrl, MIC in det OFF */
	{ALC5658_MANAGEMENT1, 0x0000, 0},	/* I2S, DAC, ADC, SPDIFF, CLASS D OFF */
	{ALC5658_MANAGEMENT2, 0x0000, 0},	/* Filters, PDM I/F  OFF */
	{ALC5658_MANAGEMENT3, 0x003C, 0},	/* HP L/R ON, HPamp x5, LDO1 out 0.9V */
	{ALC5658_CLOCK, 0x0000, 0},	/* MCLK, No dividers */
	{ALC5658_ADCDAC_CLOCK, 0x0770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALC5658_MICBIAS2, 0x0080, 0},	/* Probably enable Pow_int_clk1/2, does not match with DOC !!! */
};

t_codec_init_script_entry codec_init_out_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},
	{ALC5658_006E, 0xFFFF, 0},
	{ALC5658_006F, 0xFFFF, 0},
	{ALC5658_GENERAL_CONTROL_1, 0x8001, 0},	/* Bypass_noise_gate_mode2 BYPASS, digital_gate_ctrl ENABLE */
	{ALC5658_RESET1, 0x3030, 0},	/* DAC1, DAC2 + alpha clock enable */
	{ALC5658_PLL1, 0x0302, 0},	/* k = 2, n = 8 */
	{ALC5658_PLL2, 0x0800, 0},	/* m - bypass */
	{ALC5658_ADCDAC_CLOCK, 0x1110, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 2, something reserved, dac/adc 128Fs */
	{ALC5658_0091, 0x0C16, 0},
	{ALC5658_MANAGEMENT3, 0xA23E, 20},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT3, 0xF23E, 0},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT5, 0x0040, 0},	/* LDO2 ON */
	{ALC5658_CLOCK, 0x4000, 0},	/* PLL1, No dividers */
	{ALC5658_ASRC4, 0x0100, 0},	/* sel_i2s2_asrc = ASRC2 */
	{ALC5658_ASRC1, 0x1300, 0},	/* En_i2s2_asrc - EN, Sel_mono_dac_l/r_mode EN */
	{ALC5658_ASRC2, 0x0220, 0},	/* sel_da_filter_monol_track = clk_i2s2_track, sel_da_filter_monor_track = clk_i2s2_track */
	{ALC5658_MANAGEMENT1, 0xC080, 0},	/* en_i2s1/en_i2s2, Pow_ldo_dacref ON */
	{ALC5658_HP, 0x0009, 0},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ALC5658_MANAGEMENT1, 0xCC80, 0},	/* en_i2s1/en_i2s2, pow_dac1_l/r, Pow_ldo_dacref ON */
	{ALC5658_0091, 0x0E16, 0},
	{ALC5658_MANAGEMENT2, 0x0700, 0},	/* pow_dac_stereo1_filter, pow_dac_monol/r_filter ON  */
	{ALC5658_HPOUT, 0x0000, 0},	/* OFF */
	{ALC5658_0091, 0x0E1E, 0},
	{ALC5658_006E, 0xFFFF, 0},
	{ALC5658_006F, 0xFFFF, 0},
	{ALC5658_I2S1_PORT_CTRL, 0x0000, 0},	//MASTER 32FS
	{ALC5658_DAC_ST_MXR, 0x2A8A, 0},	/* mu_stereo_dacl1_mixl, mu_stereo_dacr1_mixr unmute */
	{ALC5658_DAC_SRC, 0x000F, 0},	/* DAC1 stereo, DAC2 mono */
	{ALC5658_HP, 0x0019, 0},	/* en_out_hp - ON, pow_pump_hp - ON, pow_capless - ON  */
	{ALC5658_TDM2, 0x000C, 0},	//loopback
};

t_codec_init_script_entry codec_init_in_script[] = {
	{ALC5658_SW_RESET, 0x0000, 0},
	{ALC5658_RESET1, 0x3030, 0},	/* DAC1, DAC2 + alpha clock enable */
	{ALC5658_RESET2, 0x3030, 0},	/* ADC1, ADC2 + alpha clock enable */
	{ALC5658_006E, 0xEF00, 0},
	{ALC5658_006F, 0xEFFC, 0},
	{ALC5658_I2S1_PORT_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ALC5658_GENERAL_CONTROL_1, 0x8001, 0},	/* Bypass_noise_gate_mode2 BYPASS, digital_gate_ctrl ENABLE */
	{ALC5658_ADCDAC_CLOCK, 0x0000, 0},	/* I2Sprediv1 = 1, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 1, something reserved, dac/adc 128Fs */
	{ALC5658_MANAGEMENT3, 0xA2BE, 50},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap ON, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT3, 0xF2BE, 0},	/* VREF1 on, slow VREF1, VREF2 on, slow VREF2, MBIAS on, LOUT off, MBIAS bandgap ON, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ALC5658_MANAGEMENT1, 0x8098, 0},	/* en_i2s1, Pow_ldo_dacref, pow_adc1_l/r ON */
	{ALC5658_MANAGEMENT1, 0x8C80, 0},	/* en_i2s1, pow_dac1_l/r Pow_ldo_dacref, pow_adc1_l/r ON */
	{ALC5658_0091, 0x0E16, 0},
	{ALC5658_MANAGEMENT2, 0xB400, 0},	/* pow_adc_stereo1_filter, pow_adc_monol/r_filter,  pow_dac_stereo1_filter ON */
	{ALC5658_0010, 0x3040, 0},	//CRT Mbias1 path
	{ALC5658_MANAGEMENT4, 0xC860, 0},	//enable mbias1 /* pow_bst1, pow_bst2, pow_micbias1_digital, pow_bst1-2, pow_bst2-2 ON   */
	{ALC5658_MANAGEMENT1, 0x8C98, 0},	/* en_i2s1, pow_dac1_l/r, Pow_ldo_dacref, pow_adc1_l/r ON */
	{ALC5658_MANAGEMENT6, 0x0C00, 0},	/* pow_recmix1l/r ON */
	{ALC5658_MANAGEMENT7, 0x0300, 0},	/* ?extra bits set? pow_inl_vol ON */
	{ALC5658_IN1, 0x2000, 0},	/* Gain -12dB + 0.75dB*0x20  */
	{ALC5658_RECMIX1L_SEL, 0x005F, 0},	/* Default, all mute */
	{ALC5658_RECMIX1R_SEL, 0x005F, 0},	/* Default, all mute */
	{ALC5658_ADC_ST1_MXR, 0x6020, 0},	/* mu_stereo1_adcl1 unmute, sel_stereo1_adc1 Sel, mu_stereo1_adcr1 unmute */
	{ALC5658_ADC_2_MXR, 0x0000, 100},	/* Mu_stereo1_adc_mixer_l/r unmute */
	{ALC5658_DAC_MO_MXR, 0x2A8A, 0},	/* mu_mono_dacl1_mixl/r unmute */
	{ALC5658_ADC_MO_MXR, 0x4040, 0},	/* mu_mono_adcl1 UM, sel_mono_adcl1/2 - Mono_DAC_Mixer_L, mu_mono_adcr1 UM, Sel_mono_adcr1/2 - Mono_DAC_Mixer_R, sel_mono_adcr - ADC1_L, Sel_mono_dmic_r - DMIC1_R */
	{ALC5658_DAC_SRC, 0x000F, 0},	/* DAC1 stereo, DAC2 mono */
	{ALC5658_TDM1, 0x00F0, 0},	/* TDM IN/OUT 32bit LEN Applicable in master mode */
	{ALC5658_TDM2, 0x0000, 0},	/* rx_adc_data_sel - IF_ADC1 / IF_ADC2 / DAC_REF / Null */
};

t_codec_init_script_entry codec_init_inout_script1[] = {
	{ALC5658_SW_RESET, 0x0000, 0},
	{ALC5658_006E, 0xEF00, 0},
	{ALC5658_006F, 0xEFFC, 0},
	{ALC5658_GENERAL_CONTROL_1, 0x8001, 0},
	{ALC5658_RESET1, 0x3030, 0},
	{ALC5658_RESET2, 0x3030, 0},
};

t_codec_init_script_entry codec_init_pll_8K[] = {
	{ALC5658_PLL1, 0x0302, 0},
	{ALC5658_PLL2, 0x0800, 0},
	{ALC5658_ADCDAC_CLOCK, 0x6110, 0},
};

t_codec_init_script_entry codec_init_pll_11K[] = {
	{ALC5658_PLL1, 0x4883, 0},
	{ALC5658_PLL2, 0xE000, 0},
	{ALC5658_ADCDAC_CLOCK, 0x5110, 0},
};

t_codec_init_script_entry codec_init_pll_16K[] = {
	{ALC5658_PLL1, 0x0302, 0},
	{ALC5658_PLL2, 0x0800, 0},
	{ALC5658_ADCDAC_CLOCK, 0x4110, 0},
};

t_codec_init_script_entry codec_init_pll_22K[] = {
	{ALC5658_PLL1, 0x4883, 0},
	{ALC5658_PLL2, 0xE000, 0},
	{ALC5658_ADCDAC_CLOCK, 0x3110, 0},
};

t_codec_init_script_entry codec_init_pll_32K[] = {
	{ALC5658_PLL1, 0x0302, 0},
	{ALC5658_PLL2, 0x0800, 0},
	{ALC5658_ADCDAC_CLOCK, 0x2110, 0},
};

t_codec_init_script_entry codec_init_pll_44K[] = {
	{ALC5658_PLL1, 0x4883, 0},
	{ALC5658_PLL2, 0xE000, 0},
	{ALC5658_ADCDAC_CLOCK, 0x1110, 0},
};

t_codec_init_script_entry codec_init_pll_48K[] = {
	{ALC5658_PLL1, 0x0302, 0},
	{ALC5658_PLL2, 0x0800, 0},
	{ALC5658_ADCDAC_CLOCK, 0x1110, 0},
};

t_codec_init_script_entry codec_init_inout_script2[] = {
	{ALC5658_0091, 0x0C16, 0},
	{ALC5658_MANAGEMENT3, 0xA2BE, 20},
	{ALC5658_MANAGEMENT3, 0xF2BE, 0},
	{ALC5658_MANAGEMENT5, 0x0040, 0},
	{ALC5658_CLOCK, 0x4000, 0},
	{ALC5658_ASRC4, 0x0100, 0},
	{ALC5658_ASRC1, 0x1300, 0},
	{ALC5658_ASRC2, 0x0220, 0},
	{ALC5658_MANAGEMENT1, 0xC080, 0},
	{ALC5658_HP, 0x0009, 0},
	{ALC5658_MANAGEMENT1, 0xCC80, 0},
	{ALC5658_MANAGEMENT1, 0xCC98, 0},
	{ALC5658_0091, 0x0E16, 0},
	{ALC5658_MANAGEMENT2, 0xB700, 0},
	{ALC5658_MANAGEMENT4, 0xC860, 0},	//enable mbias1
	{ALC5658_MANAGEMENT6, 0x0C00, 0},
	{ALC5658_MANAGEMENT7, 0x0300, 0},
	{ALC5658_HPOUT, 0x8080, 0},	// MUTE OUTPUT
	{ALC5658_0091, 0x0E1E, 0},
	{ALC5658_I2S1_PORT_CTRL, 0x0000, 0},
	{ALC5658_DAC_ST_MXR, 0x2A8A, 0},
	{ALC5658_DAC_SRC, 0x000F, 0},
	{ALC5658_HP, 0x0019, 0},
	{ALC5658_0010, 0x3040, 0},	//CRT Mbias1 path
	{ALC5658_IN1, 0x2000, 0},	//BST1 gain (minimal)
	{ALC5658_RECMIX1L_SEL, 0x005F, 0},	//BST1
	{ALC5658_RECMIX1R_SEL, 0x005F, 0},	//BST1
	{ALC5658_ADC_ST1_MXR, 0x6020, 0},
	{ALC5658_ADC_2_MXR, 0x8080, 100},
	{ALC5658_DAC_MO_MXR, 0x2A8A, 0},
	{ALC5658_ADC_MO_MXR, 0x4040, 0},
	{ALC5658_DAC_SRC, 0x000F, 0},
	{ALC5658_TDM1, 0x00F0, 0},
	{ALC5658_TDM2, 0x0000, 0},
};

#endif							/* CONFIG_AUDIO_ALC5658 */
#endif							/* __DRIVERS_AUDIO_ALC5658SCRIPTS_H */
