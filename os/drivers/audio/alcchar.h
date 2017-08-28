/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * drivers/audio/alcchar.h
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
 *   Author:  Gregory Nutt <gnutt@nuttx.org>
 *
 * Reference:
 *   "ALCCHAR Ultra Low Power CODEC for Portable Audio Applications, Pre-
 *    Production", September 2012, Rev 3.3, Wolfson Microelectronics
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __DRIVERS_AUDIO_ALCCHAR_H
#define __DRIVERS_AUDIO_ALCCHAR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <pthread.h>
#include <mqueue.h>

#include <tinyara/wqueue.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_AUDIO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Registers Addresses ******************************************************/

typedef enum {
	ACL5658_RESET = 0x0000,
	ACL5658_SPO_VOL = 0x0001,
	ACL5658_HPOUT_MUTE = 0x0002,
	ACL5658_LOUT_CTRL1 = 0x0003,
	ACL5658_LOUT_CTRL2 = 0x0004,
	ACL5658_HPOUT_VLML = 0x0005,
	ACL5658_HPOUT_VLMR = 0x0006,
	ACL5658_SPDIF_CTRL1 = 0x0008,
	ACL5658_SPDIF_CTRL2 = 0x0009,
	ACL5658_SPDIF_CTRL3 = 0x0036,
	ACL5658_IN1_CTRL = 0x000C,
	ACL5658_INL_VLM = 0x000F,

	ACL5658_SIDETONE = 0x0018,

	/* DIGITAL Volume */
	ACL5658_DAC_L1R1_VLM = 0x0019,
	ACL5658_DAC_L2R2_VLM = 0x001A,
	ACL5658_DAC_L2R2_MUTE = 0x001B,

	/* DIGITAL Mixers */
	ALCCHAR_ADC_STR1_MXR = 0x0026,
	ALCCHAR_ADC_MONO_MXR = 0x0027,
	ACL5658_ADC_2_DAC_MXR = 0x0029,
	ACL5658_DAC_STR_MXR = 0x002A,
	ACL5658_DAC_MONO_MXR = 0x002B,
	ACL5658_DAC_LB_SDTONE = 0x002C,
	ACL5658_COPY_MODE = 0x002F,

	/* Analog DAC Source */
	ACL5658_DAC_SRC = 0x002D,

	ACL5658_RECMIX1L_CTRL_1 = 0x003B,
	ACL5658_RECMIX1L_CTRL_2 = 0x003C,
	ACL5658_RECMIX1R_CTRL_1 = 0x003D,
	ACL5658_RECMIX1R_CTRL_2 = 0x003E,

	/* ANALOG Mixers */
	ACL5658_SPKMIXL = 0x0046,
	ACL5658_SPKMIXR = 0x0047,
	ACL5658_SPOMIX = 0x0048,
	ACL5658_OUTMIXL1 = 0x004D,
	ACL5658_OUTMIXL2 = 0x004E,
	ACL5658_OUTMIXR1 = 0x004F,
	ACL5658_OUTMIXR2 = 0x0050,
	ACL5658_LOUTMIX = 0x0052,

	/* Power management */
	ACL5658_PWR_MNG1 = 0x0061,
	ACL5658_PWR_MNG2 = 0x0062,
	ACL5658_PWR_MNG3 = 0x0063,
	ACL5658_PWR_MNG4 = 0x0064,
	ACL5658_PWR_MNG5 = 0x0065,
	ACL5658_PWR_MNG6 = 0x0066,
	ACL5658_PWR_MNG7 = 0x0067,

	/* DIGITAL ports control */
	ACL5658_IF_DTCT = 0x006B,
	ALCCHAR_006E = 0x006E,
	ALCCHAR_006F = 0x006F,
	ACL5658_I2S1_CTRL = 0x0070,
	ACL5658_I2S2_CTRL = 0x0071,
	ACL5658_ADDA_CLK = 0x0073,
	ACL5658_ADDA_HPF = 0x0074,
	ALCCHAR_007B = 0x007B,

	/* TDM */
	ACL5658_TDM_CTRL1 = 0x0077,
	ACL5658_TDM_CTRL2 = 0x0078,
	ACL5658_TDM_CTRL3 = 0x0079,
	ACL5658_TDM_CTRL4 = 0x007A,

	/* Global Clock */
	ACL5658_GLBL_CLK = 0x0080,
	ACL5658_GLBL_PLL1 = 0x0081,
	ACL5658_GLBL_PLL2 = 0x0082,
	ACL5658_GLBL_ASRC1 = 0x0083,
	ACL5658_GLBL_ASRC2 = 0x0084,
	ACL5658_GLBL_ASRC3 = 0x0085,
	ACL5658_GLBL_ASRC4 = 0x008A,

	/* Amplifiers */
	ACL5658_HP_AMP = 0x008E,
	ACL5658_SPK_AMP = 0x00A0,

	ALCCHAR_0091 = 0x0091,

	ALCCHAR_INTCLK_CTRL = 0x0094,

	ALCCHAR_GNRL_CTRL = 0x00FA,
	ALCCHAR_GNRL_CTRL2 = 0x00FB,

	ALCCHAR_0111 = 0x0111,
	ALCCHAR_0125 = 0x0125,

	ALCCHAR_ADDA_RST1 = 0x013A,
	ALCCHAR_ADDA_RST2 = 0x013B,
	ALCCHAR_0x013C = 0x013C,

	ALCCHAR_NOISE_G_M1_CTRL1 = 0x0015,
	ALCCHAR_NOISE_G_M2_CTRL = 0x0160,

	ALCCHAR_0x01DE = 0x01DE,
	ALCCHAR_0x01DF = 0x01DF,
	ALCCHAR_0x01E4 = 0x01E4,

	ALCCHAR_0040 = 0x0040,
	ALCCHAR_0010 = 0x0010,

} ALCCHAR_REG;

typedef struct {
	ALCCHAR_REG addr;
	uint16_t val;
	unsigned int delay;
} t_codec_init_script_entry;

typedef struct {
	ALCCHAR_REG addr;
	char *name;
} t_codec_dump_entry;

t_codec_init_script_entry codec_reset_script[] = {
	{ACL5658_RESET, 0x0000, 0},	/* Reset */
};

t_codec_init_script_entry codec_stop_script[] = {
	{ACL5658_RESET, 0x0000, 0},	/* Reset */
	{ACL5658_I2S1_CTRL, 0x8000, 0},	/* Switch into slave to stop data transfer */
};

t_codec_init_script_entry codec_init_script[] = {
	{ACL5658_RESET, 0x0000, 0},	/* Reset */
	{ALCCHAR_006E, 0xFFFF, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_006F, 0xFFFF, 0},	/* NO INFO IN DOCUMENTATION!!! */

	{ACL5658_GLBL_CLK, 0x8000, 0},	/* RC CLK, No dividers */
	{ALCCHAR_INTCLK_CTRL, 0x0280, 0},	/* ??? Enable ALL int CLK, even more than in documentation */
	{ALCCHAR_0111, 0xA502, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0125, 0x0430, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_ADDA_RST1, 0x3020, 0},	/* ??? DAC1, DAC2 + alpha clock enable */
	{ACL5658_ADDA_CLK, 0x1770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */

	//{ACL5658_I2S1_CTRL,       0x8020, 0},     /* Slave, off/normal/I2S...  24bit */
	{ACL5658_I2S1_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ALCCHAR_007B, 0x0003, 0},	/* Select 64*FS for BCLK in master mode, No Info in documentation  */

	{ALCCHAR_GNRL_CTRL, 0x0001, 0},	/* Enable Gate mode, Use(pass) Noise Gain Mode2 CTRL */
	{ALCCHAR_0091, 0x0C16, 0},	/* NO INFO IN DOCUMENTATION!!! */

	{ACL5658_PWR_MNG3, 0xA23E, 60},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG3, 0xF23E, 50},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */

	{ACL5658_PWR_MNG2, 0x0400, 50},	/* pow_dac_stereo1_filter ON */
	{ACL5658_PWR_MNG1, 0x8080, 10},	/* en_i2s1, Pow_ldo_dacref ON */
	{ACL5658_ADC_2_DAC_MXR, 0x8080, 0},	/* Mu_stereo1_adc_mixer_l/r MUTE */
	{ACL5658_DAC_STR_MXR, 0xAAAA, 0},	/* Default, mute all */
	{ACL5658_DAC_SRC, 0x0000, 0},	/* Default, no mixers (direct) */

	{ACL5658_HP_AMP, 0x0009, 50},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */

	{ACL5658_PWR_MNG1, 0x8C80, 50},	/* en_i2s1, pow_dac1_l/r, Pow_ldo_dacref - ON */
	{ALCCHAR_0091, 0x0E16, 50},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0040, 0x0505, 0},	/* NO INFO IN DOCUMENTATION!!! */

	{ACL5658_PWR_MNG5, 0x0180, 0},	/* Does not match with documentation */
	{ALCCHAR_0x013C, 0x3C05, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DF, 0x02C1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DE, 0x5100, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01E4, 0x0014, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DE, 0xD100, 30},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DE, 0x4900, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01E4, 0x0016, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DE, 0xC900, 250},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DF, 0x2CC1, 0},	/* NO INFO IN DOCUMENTATION!!! */

	{ACL5658_HPOUT_MUTE, 0x0000, 0},	/* UNMUTE HP Output */
	{ALCCHAR_0x01DE, 0x4500, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01E4, 0x001F, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DE, 0xC500, 800},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0040, 0x0808, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ACL5658_PWR_MNG5, 0x0000, 0},	/* Default, PLL, LDO2, Speaker VDD off */
	{ALCCHAR_0x013C, 0x2005, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01E4, 0x0000, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ALCCHAR_0x01DF, 0x20C0, 0},	/* NO INFO IN DOCUMENTATION!!! */
	{ACL5658_ADDA_CLK, 0x0770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ACL5658_GLBL_CLK, 0x0000, 0},	/* MCLK, No dividers */
	{ALCCHAR_NOISE_G_M2_CTRL, 0x8EC0, 0},	/* Stereo_noise_gate_mode2_en - ENABLE */
	{ACL5658_HP_AMP, 0x0019, 0},	/* en_out_hp - ON, pow_pump_hp - ON, pow_capless - ON  */
	{ALCCHAR_NOISE_G_M1_CTRL1, 0xC0F0, 0},	/* Noise_gate_mode1_en, Noise_gate_mode1_auto_en - EN, Noise_gate_mode1_threshold -78dB, DOES NOT MATCH WITH DOCUMENTATION !!! */
	{ALCCHAR_NOISE_G_M1_CTRL1, 0x87F9, 0},	/* Blah blah blah, eventually enable Noise gate function >:( */
	{ALCCHAR_INTCLK_CTRL, 0x0180, 0},	/* Probably enable Pow_int_clk1/2, does not match with DOC !!! */
	{ALCCHAR_GNRL_CTRL2, 0x3000, 0},	/* Noise_gate_mode1/2_hp enable, DOES NOT MATCH WITH DOCUMENTATION */
};

t_codec_init_script_entry codec_initial_script[] = {

	{ACL5658_RESET, 0x0000, 0},

	{ALCCHAR_0111, 0xA502, 0},
	{ALCCHAR_ADDA_RST1, 0x3030, 0},	/* ??? DAC1, DAC2 + alpha clock enable */

	{ALCCHAR_006E, 0xEF00, 0},
	{ALCCHAR_006F, 0xEFFC, 0},
	{ALCCHAR_INTCLK_CTRL, 0x0280, 0},	/* ??? Enable ALL int CLK, even more than in documentation */
	{ACL5658_GLBL_CLK, 0x8000, 0},	/* RC CLK, No dividers */
//  {ACL5658_I2S1_CTRL,         0x0020, 0},     /* Master, off/normal/I2S...  24bit*/
	{ACL5658_I2S1_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ACL5658_ADDA_CLK, 0x1770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALCCHAR_0091, 0x0C16, 0},
	{ACL5658_PWR_MNG3, 0xAA7E, 60},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG3, 0xFE7E, 50},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG5, 0x0004, 0},	/* LDO2 ON */

	{ACL5658_PWR_MNG2, 0x0400, 50},	/* pow_dac_stereo1_filter ON */
	{ACL5658_PWR_MNG1, 0x0080, 10},	/* Pow_ldo_dacref ON */
	{ACL5658_ADC_2_DAC_MXR, 0x8080, 0},	/* Mu_stereo1_adc_mixer_l/r MUTE */
	{ACL5658_DAC_STR_MXR, 0xAAAA, 0},	/* Default, mute all */
	{ACL5658_DAC_SRC, 0x0000, 0},	/* Default, no mixers (direct) */
	{ACL5658_HP_AMP, 0x0009, 50},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ACL5658_PWR_MNG1, 0x0F80, 50},	/* pow_dac1_l/r, pow_dac2_l/r, Pow_ldo_dacref - ON */
	{ALCCHAR_0091, 0x0E16, 50},

	{ALCCHAR_0040, 0x0505, 0},
	{ACL5658_PWR_MNG5, 0x0184, 0},	/* ???NonDescribed,  LDO2 ON */
	{ALCCHAR_0x013C, 0x3C05, 0},

	{ALCCHAR_0x01DF, 0x2cc1, 6},
	{ALCCHAR_0x01DE, 0x5100, 6},
	{ALCCHAR_0x01E4, 0x0014, 6},
	{ALCCHAR_0x01DE, 0xd100, 30},

	{ALCCHAR_0x01DF, 0x20C1, 0},

	{ALCCHAR_0x01DF, 0x2CC1, 0},
	{ALCCHAR_0x01DE, 0x4900, 0},
	{ALCCHAR_0x01E4, 0x0016, 0},
	{ALCCHAR_0x01DE, 0xC900, 250},

	{ALCCHAR_0x01DF, 0x2CC1, 0},
	{ACL5658_HPOUT_MUTE, 0x0000, 0},	/* UNMUTE HP Output */
	{ALCCHAR_0x01DE, 0x4500, 0},
	{ALCCHAR_0x01E4, 0x001F, 0},
	{ALCCHAR_0x01DE, 0xC500, 600},

	{ALCCHAR_0x01E4, 0x0000, 0},
	{ALCCHAR_0x01DF, 0x20C0, 0},

	{ALCCHAR_0040, 0x0808, 0},
	{ACL5658_PWR_MNG5, 0x0000, 0},	/* LDO2 OFF */
	{ALCCHAR_0x013C, 0x2005, 0},

	{ACL5658_PWR_MNG6, 0x0000, 0},	/* MIXERs OFF */
	{ACL5658_PWR_MNG7, 0x0000, 0},	/* VOL Out/In ctrl, MIC in det OFF */
	{ACL5658_PWR_MNG1, 0x0000, 0},	/* I2S, DAC, ADC, SPDIFF, CLASS D OFF */
	{ACL5658_PWR_MNG2, 0x0000, 0},	/* Filters, PDM I/F  OFF */
	{ACL5658_PWR_MNG3, 0x003C, 0},	/* HP L/R ON, HPamp x5, LDO1 out 0.9V */
	{ACL5658_GLBL_CLK, 0x0000, 0},	/* MCLK, No dividers */
	{ACL5658_ADDA_CLK, 0x0770, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 7, something reserved, dac/adc 128Fs */
	{ALCCHAR_INTCLK_CTRL, 0x0080, 0},	/* Probably enable Pow_int_clk1/2, does not match with DOC !!! */
};

t_codec_init_script_entry codec_init_out_script[] = {
	{ACL5658_RESET, 0x0000, 0},
	{ALCCHAR_006E, 0xFFFF, 0},
	{ALCCHAR_006F, 0xFFFF, 0},
	{ALCCHAR_GNRL_CTRL, 0x8001, 0},	/* Bypass_noise_gate_mode2 BYPASS, digital_gate_ctrl ENABLE */
	{ALCCHAR_ADDA_RST1, 0x3030, 0},	/* ??? DAC1, DAC2 + alpha clock enable */
	{ACL5658_GLBL_PLL1, 0x0302, 0},	/* k = 2, n = 8 */
	{ACL5658_GLBL_PLL2, 0x0800, 0},	/* m - bypass */
	{ACL5658_ADDA_CLK, 0x1110, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 2, something reserved, dac/adc 128Fs */
	{ALCCHAR_0091, 0x0C16, 0},
	{ACL5658_PWR_MNG3, 0xA23E, 20},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG3, 0xF23E, 0},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG5, 0x0040, 0},	/* LDO2 ON */
	{ACL5658_GLBL_CLK, 0x4000, 0},	/* PLL1, No dividers */
	{ACL5658_GLBL_ASRC4, 0x0100, 0},	/* sel_i2s2_asrc = ASRC2 */
	{ACL5658_GLBL_ASRC1, 0x1300, 0},	/* En_i2s2_asrc - EN, Sel_mono_dac_l/r_mode EN */
	{ACL5658_GLBL_ASRC2, 0x0220, 0},	/* sel_da_filter_monol_track = clk_i2s2_track, sel_da_filter_monor_track = clk_i2s2_track */
	{ACL5658_PWR_MNG1, 0xC080, 0},	/* en_i2s1/en_i2s2, Pow_ldo_dacref ON */
	{ACL5658_HP_AMP, 0x0009, 0},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ACL5658_PWR_MNG1, 0xCC80, 0},	/* en_i2s1/en_i2s2, pow_dac1_l/r, Pow_ldo_dacref ON */
	{ALCCHAR_0091, 0x0E16, 0},
	{ACL5658_PWR_MNG2, 0x0700, 0},	/* pow_dac_stereo1_filter, pow_dac_monol/r_filter ON  */
	{ACL5658_HPOUT_MUTE, 0x0000, 0},	/* OFF */
	{ALCCHAR_0091, 0x0E1E, 0},
	{ALCCHAR_006E, 0xFFFF, 0},
	{ALCCHAR_006F, 0xFFFF, 0},
//  {ACL5658_I2S1_CTRL,         0x8000, 0}, //32FS
	{ACL5658_I2S1_CTRL, 0x0000, 0},	//MASTER 32FS
	{ACL5658_DAC_STR_MXR, 0x2A8A, 0},	/* mu_stereo_dacl1_mixl, mu_stereo_dacr1_mixr unmute */
	{ACL5658_DAC_SRC, 0x000F, 0},	/* DAC1 stereo, DAC2 mono */
	{ACL5658_HP_AMP, 0x0019, 0},	/* en_out_hp - ON, pow_pump_hp - ON, pow_capless - ON  */
	{ACL5658_TDM_CTRL2, 0x000C, 0},	//loopback
};

t_codec_init_script_entry codec_init_loop_script[] = {

	{ACL5658_RESET, 0x0000, 0},
	{ALCCHAR_006E, 0xFFFF, 0},
	{ALCCHAR_006F, 0xFFFF, 0},
	{ALCCHAR_GNRL_CTRL, 0x8001, 0},	/* Bypass_noise_gate_mode2 BYPASS, digital_gate_ctrl ENABLE */
	{ALCCHAR_ADDA_RST1, 0x3030, 0},	/* ??? DAC1, DAC2 + alpha clock enable */
	{ACL5658_GLBL_PLL1, 0x0302, 0},	/* k = 2, n = 8 */
	{ACL5658_GLBL_PLL2, 0x0800, 0},	/* m - bypass */
	{ACL5658_ADDA_CLK, 0x1110, 0},	/* I2Sprediv1 = 2, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 2, something reserved, dac/adc 128Fs */
	{ALCCHAR_0091, 0x0C16, 0},
	{ACL5658_PWR_MNG3, 0xA23E, 20},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG3, 0xF23E, 0},	/* VREF1 on, SLOW VREF1, VREF2 on, SLOW VREF2, MBIAS on, LOUT off, MBIAS bandgap off, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG5, 0x0040, 0},	/* LDO2 ON */
	{ACL5658_GLBL_CLK, 0x4000, 0},	/* PLL1, No dividers */
	{ACL5658_GLBL_ASRC4, 0x0100, 0},	/* sel_i2s2_asrc = ASRC2 */
	{ACL5658_GLBL_ASRC1, 0x1300, 0},	/* En_i2s2_asrc - EN, Sel_mono_dac_l/r_mode EN */
	{ACL5658_GLBL_ASRC2, 0x0220, 0},	/* sel_da_filter_monol_track = clk_i2s2_track, sel_da_filter_monor_track = clk_i2s2_track */
	{ACL5658_PWR_MNG1, 0xC080, 0},	/* en_i2s1/en_i2s2, Pow_ldo_dacref ON */
	{ACL5658_HP_AMP, 0x0009, 0},	/* en_out_hp - OFF, pow_pump_hp - ON, pow_capless - ON */
	{ACL5658_PWR_MNG1, 0xCC80, 0},	/* en_i2s1/en_i2s2, pow_dac1_l/r, Pow_ldo_dacref ON */
	{ALCCHAR_0091, 0x0E16, 0},
	{ACL5658_PWR_MNG2, 0x0700, 0},	/* pow_dac_stereo1_filter, pow_dac_monol/r_filter ON  */
	{ACL5658_HPOUT_MUTE, 0x0000, 0},	/* OFF */
	{ALCCHAR_0091, 0x0E1E, 0},
	{ALCCHAR_006E, 0xFFFF, 0},
	{ALCCHAR_006F, 0xFFFF, 0},
//  {ACL5658_I2S1_CTRL,         0x8000, 0}, //32FS
	{ACL5658_I2S1_CTRL, 0x0000, 0},	//MASTER 32FS
	{ACL5658_DAC_STR_MXR, 0x2A8A, 0},	/* mu_stereo_dacl1_mixl, mu_stereo_dacr1_mixr unmute */
	{ACL5658_DAC_SRC, 0x000F, 0},	/*DAC1 stereo, DAC2 mono */
	{ACL5658_HP_AMP, 0x0019, 0},	/* en_out_hp - ON, pow_pump_hp - ON, pow_capless - ON  */
	{ACL5658_TDM_CTRL2, 0x000C, 0},	//loopback
};

t_codec_init_script_entry codec_init_in_script[] = {
	{ACL5658_RESET, 0x0000, 0},
	{ALCCHAR_ADDA_RST1, 0x3030, 0},	/* ??? DAC1, DAC2 + alpha clock enable */
	{ALCCHAR_ADDA_RST2, 0x3030, 0},	/* ??? ADC1, ADC2 + alpha clock enable */
	{ALCCHAR_006E, 0xEF00, 0},
	{ALCCHAR_006F, 0xEFFC, 0},
	{ACL5658_I2S1_CTRL, 0x0000, 0},	/* Master, off/normal/I2S...  16bit */
	{ALCCHAR_GNRL_CTRL, 0x8001, 0},	/* Bypass_noise_gate_mode2 BYPASS, digital_gate_ctrl ENABLE */
	{ACL5658_ADDA_CLK, 0x0000, 0},	/* I2Sprediv1 = 1, bclk_ms2 = 16bits(32FS), I2Sprediv2 = 1, something reserved, dac/adc 128Fs */
	{ACL5658_PWR_MNG3, 0xA2BE, 50},	/* VREF1 on, fast VREF1, VREF2 on, fast VREF2, MBIAS on, LOUT off, MBIAS bandgap ON, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG3, 0xF2BE, 0},	/* VREF1 on, slow VREF1, VREF2 on, slow VREF2, MBIAS on, LOUT off, MBIAS bandgap ON, HP L/R ON, HPamp x5, LDO1 out 1.2V */
	{ACL5658_PWR_MNG1, 0x8098, 0},	/* en_i2s1, Pow_ldo_dacref, pow_adc1_l/r ON */
	{ACL5658_PWR_MNG1, 0x8C80, 0},	/* en_i2s1, pow_dac1_l/r Pow_ldo_dacref, pow_adc1_l/r ON */
	{ALCCHAR_0091, 0x0E16, 0},
	{ACL5658_PWR_MNG2, 0xB400, 0},	/* pow_adc_stereo1_filter, pow_adc_monol/r_filter,  pow_dac_stereo1_filter ON */
	{ALCCHAR_0010, 0x3040, 0},	//CRT Mbias1 path
	{ACL5658_PWR_MNG4, 0xC860, 0},	//enable mbias1 /* pow_bst1, pow_bst2, pow_micbias1_digital, pow_bst1-2, pow_bst2-2 ON   */
	{ACL5658_PWR_MNG1, 0x8C98, 0},	/* en_i2s1, pow_dac1_l/r, Pow_ldo_dacref, pow_adc1_l/r ON */
	{ACL5658_PWR_MNG6, 0x0C00, 0},	/* pow_recmix1l/r ON */
	{ACL5658_PWR_MNG7, 0x0300, 0},	/* ?extra bits set? pow_inl_vol ON */
	{ACL5658_IN1_CTRL, 0x3000, 0},	/* Gain -12dB + 0.75dB*0x30  *///BST1 gain
	{ACL5658_RECMIX1L_CTRL_2, 0x005F, 0},	/* Default, all mute *///BST1
	{ACL5658_RECMIX1R_CTRL_2, 0x005F, 0},	/* Default, all mute *///BST1
	{ALCCHAR_ADC_STR1_MXR, 0x6020, 0},	/* mu_stereo1_adcl1 unmute, sel_stereo1_adc1 Sel, mu_stereo1_adcr1 unmute */
	{ACL5658_ADC_2_DAC_MXR, 0x0000, 100},	/* Mu_stereo1_adc_mixer_l/r unmute */
	{ACL5658_DAC_MONO_MXR, 0x2A8A, 0},	/* mu_mono_dacl1_mixl/r unmute */
	{ALCCHAR_ADC_MONO_MXR, 0x4040, 0},	/* mu_mono_adcl1 UM, sel_mono_adcl1/2 - Mono_DAC_Mixer_L, mu_mono_adcr1 UM, Sel_mono_adcr1/2 - Mono_DAC_Mixer_R, sel_mono_adcr - ADC1_L, Sel_mono_dmic_r - DMIC1_R */
	{ACL5658_DAC_SRC, 0x000F, 0},	/* DAC1 stereo, DAC2 mono */
	{ACL5658_TDM_CTRL1, 0x00F0, 0},	/* TDM IN/OUT 32bit LEN Applicable in master mode ??? */
	{ACL5658_TDM_CTRL2, 0x0000, 0},	/* rx_adc_data_sel - IF_ADC1 / IF_ADC2 / DAC_REF / Null */
};

t_codec_init_script_entry codec_init_inout_script1[] = {
	{0x0000, 0x0000, 0},
	{0x006E, 0xEF00, 0},
	{0x006F, 0xEFFC, 0},
	{0x00FA, 0x8001, 0},
	{0x013A, 0x3030, 0},
	{0x013B, 0x3030, 0},
};

t_codec_init_script_entry codec_init_pll_8K[] = {
	{0x0081, 0x0302, 0},
	{0x0082, 0x0800, 0},
	{0x0073, 0x6110, 0},
};

t_codec_init_script_entry codec_init_pll_11K[] = {
	{0x0081, 0x4883, 0},
	{0x0082, 0xE000, 0},
	{0x0073, 0x5110, 0},
};

t_codec_init_script_entry codec_init_pll_16K[] = {
	{0x0081, 0x0302, 0},
	{0x0082, 0x0800, 0},
	{0x0073, 0x4110, 0},
};

t_codec_init_script_entry codec_init_pll_22K[] = {
	{0x0081, 0x4883, 0},
	{0x0082, 0xE000, 0},
	{0x0073, 0x3110, 0},
};

t_codec_init_script_entry codec_init_pll_32K[] = {
	{0x0081, 0x0302, 0},
	{0x0082, 0x0800, 0},
	{0x0073, 0x2110, 0},
};

t_codec_init_script_entry codec_init_pll_44K[] = {
	{0x0081, 0x4883, 0},
	{0x0082, 0xE000, 0},
	{0x0073, 0x1110, 0},
};

t_codec_init_script_entry codec_init_pll_48K[] = {
	{0x0081, 0x0302, 0},
	{0x0082, 0x0800, 0},
	{0x0073, 0x1110, 0},
};

t_codec_init_script_entry codec_init_inout_script2[] = {
	{0x0091, 0x0C16, 0},
	{0x0063, 0xA2BE, 20},
	{0x0063, 0xF2BE, 0},
	{0x0065, 0x0040, 0},
	{0x0080, 0x4000, 0},
	{0x008A, 0x0100, 0},
	{0x0083, 0x1300, 0},
	{0x0084, 0x0220, 0},
	{0x0061, 0xC080, 0},
	{0x008E, 0x0009, 0},
	{0x0061, 0xCC80, 0},
	{0x0061, 0xCC98, 0},
	{0x0091, 0x0E16, 0},
	{0x0062, 0xB700, 0},
	{0x0064, 0xC860, 0},		//enable mbias1
	{0x0066, 0x0C00, 0},
	{0x0067, 0x0300, 0},
	{0x0002, 0x8080, 0},		// MUTE OUTPUT
	{0x0091, 0x0E1E, 0},
	{0x0070, 0x0000, 0},
	{0x002A, 0x2A8A, 0},
	{0x002D, 0x000F, 0},
	{0x008E, 0x0019, 0},
	{0x0010, 0x3040, 0},		//CRT Mbias1 path
	{0x000C, 0x0000, 0},		//BST1 gain (minimal)
	{0x003C, 0x005F, 0},		//BST1
	{0x003E, 0x005F, 0},		//BST1
	{0x0026, 0x6020, 0},
	{0x0029, 0x8080, 100},
	{0x002B, 0x2A8A, 0},
	{0x0027, 0x4040, 0},
	{0x002D, 0x000F, 0},
	{0x0077, 0x00F0, 0},
	{0x0078, 0x0000, 0},

};

t_codec_dump_entry codec_dump_script[] = {

	{ACL5658_RESET, "ACL5658_RESET"},
	{ACL5658_SPO_VOL, "ACL5658_SPO_VOL"},
	{ACL5658_HPOUT_MUTE, "ACL5658_HPOUT_MUTE"},
	{ACL5658_LOUT_CTRL1, "ACL5658_LOUT_CTRL1"},
	{ACL5658_LOUT_CTRL2, "ACL5658_LOUT_CTRL2"},
	{ACL5658_HPOUT_VLML, "ACL5658_HPOUT_VLML"},
	{ACL5658_HPOUT_VLMR, "ACL5658_HPOUT_VLMR"},
	{ACL5658_SPDIF_CTRL1, "ACL5658_SPDIF_CTRL1"},
	{ACL5658_SPDIF_CTRL2, "ACL5658_SPDIF_CTRL2"},
	{ACL5658_SPDIF_CTRL3, "ACL5658_SPDIF_CTRL3"},
	{ACL5658_IN1_CTRL, "ACL5658_IN1_CTRL"},
	{ACL5658_INL_VLM, "ACL5658_INL_VLM"},

	{ACL5658_SIDETONE, "ACL5658_SIDETONE"},

	{ACL5658_DAC_L1R1_VLM, "ACL5658_DAC_L1R1_VLM"},
	{ACL5658_DAC_L2R2_VLM, "ACL5658_DAC_L2R2_VLM"},
	{ACL5658_DAC_L2R2_MUTE, "ACL5658_DAC_L2R2_MUTE"},

	{ALCCHAR_ADC_STR1_MXR, "ALCCHAR_ADC_STR1_MXR"},
	{ALCCHAR_ADC_MONO_MXR, "ALCCHAR_ADC_MONO_MXR"},
	{ACL5658_ADC_2_DAC_MXR, "ACL5658_ADC_2_DAC_MXR"},
	{ACL5658_DAC_STR_MXR, "ACL5658_DAC_STR_MXR"},
	{ACL5658_DAC_MONO_MXR, "ACL5658_DAC_MONO_MXR"},
	{ACL5658_DAC_LB_SDTONE, "ACL5658_DAC_LB_SDTONE"},
	{ACL5658_COPY_MODE, "ACL5658_COPY_MODE"},

	{ACL5658_DAC_SRC, "ACL5658_DAC_SRC"},

	{ACL5658_RECMIX1L_CTRL_1, "ACL5658_RECMIX1L_CTRL_1"},
	{ACL5658_RECMIX1L_CTRL_2, "ACL5658_RECMIX1L_CTRL_2"},
	{ACL5658_RECMIX1R_CTRL_1, "ACL5658_RECMIX1R_CTRL_1"},
	{ACL5658_RECMIX1R_CTRL_2, "ACL5658_RECMIX1R_CTRL_2"},

	{ACL5658_SPKMIXL, "ACL5658_SPKMIXL"},
	{ACL5658_SPKMIXR, "ACL5658_SPKMIXR"},
	{ACL5658_SPOMIX, "ACL5658_SPOMIX"},
	{ACL5658_OUTMIXL1, "ACL5658_OUTMIXL1"},
	{ACL5658_OUTMIXL2, "ACL5658_OUTMIXL2"},
	{ACL5658_OUTMIXR1, "ACL5658_OUTMIXR1"},
	{ACL5658_OUTMIXR2, "ACL5658_OUTMIXR2"},
	{ACL5658_LOUTMIX, "ACL5658_LOUTMIX"},

	{ACL5658_PWR_MNG1, "ACL5658_PWR_MNG1"},
	{ACL5658_PWR_MNG2, "ACL5658_PWR_MNG2"},
	{ACL5658_PWR_MNG3, "ACL5658_PWR_MNG3"},
	{ACL5658_PWR_MNG4, "ACL5658_PWR_MNG4"},
	{ACL5658_PWR_MNG5, "ACL5658_PWR_MNG5"},
	{ACL5658_PWR_MNG6, "ACL5658_PWR_MNG6"},
	{ACL5658_PWR_MNG7, "ACL5658_PWR_MNG7"},

	{ACL5658_IF_DTCT, "ACL5658_IF_DTCT"},
	{ALCCHAR_006E, "ALCCHAR_006E"},
	{ALCCHAR_006F, "ALCCHAR_006F"},
	{ACL5658_I2S1_CTRL, "ACL5658_I2S1_CTRL"},
	{ACL5658_I2S2_CTRL, "ACL5658_I2S2_CTRL"},
	{ACL5658_ADDA_CLK, "ACL5658_ADDA_CLK"},
	{ACL5658_ADDA_HPF, "ACL5658_ADDA_HPF"},
	{ALCCHAR_007B, "ALCCHAR_007B"},

	{ACL5658_TDM_CTRL1, "ACL5658_TDM_CTRL1"},
	{ACL5658_TDM_CTRL2, "ACL5658_TDM_CTRL2"},
	{ACL5658_TDM_CTRL3, "ACL5658_TDM_CTRL3"},
	{ACL5658_TDM_CTRL4, "ACL5658_TDM_CTRL4"},

	{ACL5658_GLBL_CLK, "ACL5658_GLBL_CLK"},
	{ACL5658_GLBL_PLL1, "ACL5658_GLBL_PLL1"},
	{ACL5658_GLBL_PLL2, "ACL5658_GLBL_PLL2"},
	{ACL5658_GLBL_ASRC1, "ACL5658_GLBL_ASRC1"},
	{ACL5658_GLBL_ASRC2, "ACL5658_GLBL_ASRC2"},
	{ACL5658_GLBL_ASRC3, "ACL5658_GLBL_ASRC3"},
	{ACL5658_GLBL_ASRC4, "ACL5658_GLBL_ASRC4"},

	{ACL5658_HP_AMP, "ACL5658_HP_AMP"},
	{ACL5658_SPK_AMP, "ACL5658_SPK_AMP"},

	{ALCCHAR_0091, "ALCCHAR_0091"},

	{ALCCHAR_INTCLK_CTRL, "ALCCHAR_INTCLK_CTRL"},

	{ALCCHAR_GNRL_CTRL, "ALCCHAR_GNRL_CTRL"},
	{ALCCHAR_GNRL_CTRL2, "ALCCHAR_GNRL_CTRL2"},

	{ALCCHAR_0111, "ALCCHAR_0111"},
	{ALCCHAR_0125, "ALCCHAR_0125"},

	{ALCCHAR_ADDA_RST1, "ALCCHAR_ADDA_RST1"},
	{ALCCHAR_ADDA_RST2, "ALCCHAR_ADDA_RST2"},
	{ALCCHAR_0x013C, "ALCCHAR_0x013C"},

	{ALCCHAR_NOISE_G_M1_CTRL1, "ALCCHAR_NOISE_G_M1_CTRL1"},
	{ALCCHAR_NOISE_G_M2_CTRL, "ALCCHAR_NOISE_G_M2_CTRL"},

	{ALCCHAR_0x01DE, "ALCCHAR_0x01DE"},
	{ALCCHAR_0x01DF, "ALCCHAR_0x01DF"},
	{ALCCHAR_0x01E4, "ALCCHAR_0x01E4"},

	{ALCCHAR_0040, "ALCCHAR_0040"},
	{ALCCHAR_0010, "ALCCHAR_0010"},
};

/* Commonly defined and redefined macros */

#ifndef MIN
#define MIN(a, b)                   (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)                   (((a) > (b)) ? (a) : (b))
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef CONFIG_ALCCHAR_CLKDEBUG
extern const uint8_t g_sysclk_scaleb1[ALCCHAR_BCLK_MAXDIV + 1];
extern const uint8_t g_fllratio[ALCCHAR_NFLLRATIO];
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: alcchar_readreg
 *
 * Description
 *    Read the specified 16-bit register from the ALCCHAR device.
 *
 ****************************************************************************/

#if defined(CONFIG_ALCCHAR_REGDUMP) || defined(CONFIG_ALCCHAR_CLKDEBUG)
struct alcchar_dev_s;
uint16_t alcchar_readreg(FAR struct alcchar_dev_s *priv, uint16_t regaddr);
#endif

#endif							/* CONFIG_AUDIO */
#endif							/* __DRIVERS_AUDIO_ALCCHAR_H */
