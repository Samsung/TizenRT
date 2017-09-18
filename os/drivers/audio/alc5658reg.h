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
#ifndef __DRIVERS_AUDIO_ALC5658REG_H
#define __DRIVERS_AUDIO_ALC5658REG_H

#ifdef CONFIG_AUDIO_ALC5658

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/compiler.h>


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Registers Addresses ******************************************************/
typedef enum {
	ALC5658_RESET = 0x0000,
	ALC5658_SPO_VOL = 0x0001,
	ALC5658_HPOUT_MUTE = 0x0002,
	ALC5658_LOUT_CTRL1 = 0x0003,
	ALC5658_LOUT_CTRL2 = 0x0004,
	ALC5658_HPOUT_VLML = 0x0005,
	ALC5658_HPOUT_VLMR = 0x0006,
	ALC5658_SPDIF_CTRL1 = 0x0008,
	ALC5658_SPDIF_CTRL2 = 0x0009,
	ALC5658_SPDIF_CTRL3 = 0x0036,
	ALC5658_IN1_CTRL = 0x000C,
	ALC5658_INL_VLM = 0x000F,
	ALC5658_SIDETONE = 0x0018,
	/* DIGITAL Volume */
	ALC5658_DAC_L1R1_VLM = 0x0019,
	ALC5658_DAC_L2R2_VLM = 0x001A,
	ALC5658_DAC_L2R2_MUTE = 0x001B,
	/* DIGITAL Mixers */
	ALC5658_ADC_STR1_MXR = 0x0026,
	ALC5658_ADC_MONO_MXR = 0x0027,
	ALC5658_ADC_2_DAC_MXR = 0x0029,
	ALC5658_DAC_STR_MXR = 0x002A,
	ALC5658_DAC_MONO_MXR = 0x002B,
	ALC5658_DAC_LB_SDTONE = 0x002C,
	ALC5658_COPY_MODE = 0x002F,
	/* Analog DAC Source */
	ALC5658_DAC_SRC = 0x002D,
	ALC5658_RECMIX1L_CTRL_1 = 0x003B,
	ALC5658_RECMIX1L_CTRL_2 = 0x003C,
	ALC5658_RECMIX1R_CTRL_1 = 0x003D,
	ALC5658_RECMIX1R_CTRL_2 = 0x003E,
	/* ANALOG Mixers */
	ALC5658_SPKMIXL = 0x0046,
	ALC5658_SPKMIXR = 0x0047,
	ALC5658_SPOMIX = 0x0048,
	ALC5658_OUTMIXL1 = 0x004D,
	ALC5658_OUTMIXL2 = 0x004E,
	ALC5658_OUTMIXR1 = 0x004F,
	ALC5658_OUTMIXR2 = 0x0050,
	ALC5658_LOUTMIX = 0x0052,
	/* Power management */
	ALC5658_PWR_MNG1 = 0x0061,
	ALC5658_PWR_MNG2 = 0x0062,
	ALC5658_PWR_MNG3 = 0x0063,
	ALC5658_PWR_MNG4 = 0x0064,
	ALC5658_PWR_MNG5 = 0x0065,
	ALC5658_PWR_MNG6 = 0x0066,
	ALC5658_PWR_MNG7 = 0x0067,
	/* DIGITAL ports control */
	ALC5658_IF_DTCT = 0x006B,
	ALC5658_006E = 0x006E,
	ALC5658_006F = 0x006F,
	ALC5658_I2S1_CTRL = 0x0070,
	ALC5658_I2S2_CTRL = 0x0071,
	ALC5658_ADDA_CLK = 0x0073,
	ALC5658_ADDA_HPF = 0x0074,
	ALC5658_007B = 0x007B,
	/* TDM */
	ALC5658_TDM_CTRL1 = 0x0077,
	ALC5658_TDM_CTRL2 = 0x0078,
	ALC5658_TDM_CTRL3 = 0x0079,
	ALC5658_TDM_CTRL4 = 0x007A,
	/* Global Clock */
	ALC5658_GLBL_CLK = 0x0080,
	ALC5658_GLBL_PLL1 = 0x0081,
	ALC5658_GLBL_PLL2 = 0x0082,
	ALC5658_GLBL_ASRC1 = 0x0083,
	ALC5658_GLBL_ASRC2 = 0x0084,
	ALC5658_GLBL_ASRC3 = 0x0085,
	ALC5658_GLBL_ASRC4 = 0x008A,
	/* Amplifiers */
	ALC5658_HP_AMP = 0x008E,
	ALC5658_SPK_AMP = 0x00A0,
	ALC5658_0091 = 0x0091,
	ALC5658_INTCLK_CTRL = 0x0094,
	ALC5658_GNRL_CTRL = 0x00FA,
	ALC5658_GNRL_CTRL2 = 0x00FB,
	ALC5658_0111 = 0x0111,
	ALC5658_0125 = 0x0125,
	ALC5658_ADDA_RST1 = 0x013A,
	ALC5658_ADDA_RST2 = 0x013B,
	ALC5658_0x013C = 0x013C,
	ALC5658_NOISE_G_M1_CTRL1 = 0x0015,
	ALC5658_NOISE_G_M2_CTRL = 0x0160,
	ALC5658_0x01DE = 0x01DE,
	ALC5658_0x01DF = 0x01DF,
	ALC5658_0x01E4 = 0x01E4,
	ALC5658_0040 = 0x0040,
	ALC5658_0010 = 0x0010,
} ALC5658_REG;

#endif							/* CONFIG_AUDIO_ALC5658 */
#endif							/* __DRIVERS_AUDIO_ALC5658REG_H */
