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

#define ALC5658_SW_RESET			0x0000	/* S/W Reset & Device ID */
#define ALC5658_SPKOUT				0x0001	/* Speaker Output Volume & Mute/Un-Mute */
#define ALC5658_HPOUT				0x0002	/* Headphone Mute/Un-Mute */
#define ALC5658_LOUT1				0x0003	/* Line Output Volume & Mute/Un-Mute */
#define ALC5658_LOUT2				0x0004	/* Line Output Control */
#define ALC5658_HPOUT_L				0x0005	/* Headphone L Channel Gain Control */
#define ALC5658_HPOUT_R				0x0006	/* Headphone R Channel Gain Control */
#define ALC5658_SPDIF_CTRL1			0x0008	/* S/PDIF Out Control */
#define ALC5658_SPDIF_CTRL2			0x0009	/* S/PDIF Out Control */
#define ALC5658_SPDIF_CTRL3			0x0036	/* S/PDIF Out Control */
#define ALC5658_IN1					0x000C	/* IN1 Port Control */
#define ALC5658_INL					0x000F	/* INLVolume Control */
#define ALC5658_0010				0x0010
#define ALC5658_JD					0x0011	/* Jack and Mic Detection Control 1 */
#define ALC5658_SIDETONE			0x0018	/* Sidetone Control */
#define ALC5658_NOISE_GATE1			0x0015	/* Noise Gate Mode1 Conrol 1 */
#define ALC5658_NOISE_GATE2			0x0016	/* Noise Gate Mode1 Conrol 2 */
#define ALC5658_DACL1R1				0x0019	/* DACL1/R1 Digital Volume Control */
#define ALC5658_DACL2R2				0x001A	/* DACL2/R2 Digital Volume Control */
#define ALC5658_DACL2R2_MUTE		0x001B	/* DACL2/R2 Digital Mute/Un-Mute Control */
#define ALC5658_ADC_ST1_VOL			0x001C	/* Stereo1 ADCL/R Digital Volume & Mute/Un-Mute Control */
#define ALC5658_ADC_MO_VOL			0x001D	/* Mono ADCL/R Digital Path Volume Control */
#define ALC5658_ADC_ST1_GAIN		0x001F	/* Stereo1 ADC Filter Boost Gain for DMIC */
#define ALC5658_ADC_MO_GAIN			0x0020	/* Mono ADC Filter Boost Gain for DMIC */
#define ALC5658_ADC_ST1_MXR			0x0026	/* Stereo1 ADC Digital Mixer Control */
#define ALC5658_ADC_MO_MXR			0x0027	/* Mono ADC Digital Mixer Control */
#define ALC5658_ADC_2_MXR			0x0029	/* ADC to DAC Digital Mixer Control */
#define ALC5658_DAC_ST_MXR			0x002A	/* DAC Stereo Digital Mixer Control */
#define ALC5658_DAC_MO_MXR			0x002B	/* DAC Mono Digital Mixer Control */
#define ALC5658_DAC_LOOPBACK		0x002C	/* Digital Loop Back and Sidetone Control Control */
#define ALC5658_COPY_MODE			0x002F	/* ADC/DAC Data Copy Mode Control */
#define ALC5658_DAC_SRC				0x002D	/* Analog DACL1/R1 and DACL2/R2 Input Source Control */
#define ALC5658_PDM					0x0031	/* PDM Interface Control */
#define ALC5658_RECMIX1L			0x003B	/* RECMIX1L Gain Control */
#define ALC5658_RECMIX1L_SEL		0x003C	/* RECMIX1L Gain & Selection Control */
#define ALC5658_RECMIX1R			0x003D	/* RECMIX1R Gain Control */
#define ALC5658_RECMIX1R_SEL		0x003E	/* RECMIX1R Gain & Selection Control */
#define ALC5658_RECMIX2L			0x009B	/* RECMIX2L Gain Control */
#define ALC5658_RECMIX2L_SEL		0x009C	/* RECMIX2L Gain & Selection Control */
#define ALC5658_RECMIX2R			0x009D	/* RECMIX2R Gain Control */
#define ALC5658_RECMIX2R_SEL		0x009E	/* RECMIX2R Gain & Selection Control */
#define ALC5658_0040				0x0040
#define ALC5658_SPKMIXL				0x0046	/* SPKMIXL Gain & Selection Control */
#define ALC5658_SPKMIXR				0x0047	/* SPKMIXR Gain & Selection Control */
#define ALC5658_SPOMIX				0x0048	/* Speaker Amp Source Selection Control */
#define ALC5658_OUTMIXL1			0x004D	/* OUTMIXL Gain & Selection Control */
#define ALC5658_OUTMIXL2			0x004E	/* OUTMIXL Gain & Selection Control */
#define ALC5658_OUTMIXR1			0x004F	/* OUTMIXR Gain & Selection Control */
#define ALC5658_OUTMIXR2			0x0050	/* OUTMIXR Gain & Selection Control */
#define ALC5658_LOUTMIX				0x0052	/* LOUTMIX Gain & Selection Control */
#define ALC5658_GAPTIC_CTLRL01		0x0053	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL02		0x0054	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL03		0x0055	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL04		0x0056	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL05		0x0057	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL06		0x0058	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL07		0x0059	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL08		0x005A	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL09		0x005B	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL10		0x005C	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL11		0x005D	/* Gaptic Generator Control */
#define ALC5658_GAPTIC_CTLRL12		0x005E	/* Gaptic Generator Control */
#define ALC5658_MANAGEMENT1			0x0061	/* I2S & DAC & ADC & Class-D Power Control */
#define ALC5658_MANAGEMENT2			0x0062	/* Digital Filter & PDM Power Control */
#define ALC5658_MANAGEMENT3			0x0063	/* VREF & MBias & LOUTMIX & HP Power Control */
#define ALC5658_MANAGEMENT4			0x0064	/* MICBST & MICBIAS & JD Power Control */
#define ALC5658_MANAGEMENT5			0x0065	/* PLL & LDO Power Control */
#define ALC5658_MANAGEMENT6			0x0066	/* OUTMIX & SPKMIX & RECMIX Power Control */
#define ALC5658_MANAGEMENT7			0x0067	/* SPOVOL & OUTVOL & INVOL & MIC_IN_DET Power Control */
#define ALC5658_IF_DETECTION		0x006B	/* Interface and System Clock Detection Control */
#define ALC5658_006E				0x006E
#define ALC5658_006F				0x006F
#define ALC5658_I2S1_PORT_CTRL		0x0070	/* I2S-1 Interface Control */
#define ALC5658_I2S2_PORT_CTRL		0x0071	/* I2S-2 Interface Control */
#define ALC5658_ADCDAC_CLOCK		0x0073	/* ADC/DAC Clock Control */
#define ALC5658_ADCDAC_HPF			0x0074	/* ADC/DAC HPF Control */
#define ALC5658_MIC_CTRL1			0x0075	/* Digital Microphone Control */
#define ALC5658_MIC_CTRL2			0x0076	/* Digital Microphone Control */
#define ALC5658_TDM1				0x0077	/* TDM Interface Control */
#define ALC5658_TDM2				0x0078	/* TDM Interface Control */
#define ALC5658_TDM3				0x0079	/* TDM Interface Control */
#define ALC5658_TDM4				0x007A	/* TDM Interface Control */
#define ALC5658_007B				0x007B
#define ALC5658_CLOCK				0x0080	/* Global Clock Control */
#define ALC5658_PLL1				0x0081	/* PLL Control 1 */
#define ALC5658_PLL2				0x0082	/* PLL Control 2 */
#define ALC5658_ASRC1				0x0083	/* ASRC Control 1 */
#define ALC5658_ASRC2				0x0084	/* ASRC Control 2 */
#define ALC5658_ASRC3				0x0085	/* ASRC Control 3 */
#define ALC5658_ASRC4				0x008A	/* ASRC Control 4 */
#define ALC5658_HP					0x008E	/* HP Output Control 1 */
#define ALC5658_0091				0x0091
#define ALC5658_MICBIAS1			0x0093	/* MICBIAS Control */
#define ALC5658_MICBIAS2			0x0094	/* MICBIAS Control */
#define ALC5658_SPK_AMP				0x00A0	/* Class-D Amp Control */
#define ALC5658_ADC_EQ1				0x00AE	/* ADC EQ Control 1 */
#define ALC5658_ADC_EQ2				0x00AF	/* ADC EQ Control 2 */
#define ALC5658_DAC_EQ1				0x00B0	/* DAC EQ Control 1 */
#define ALC5658_DAC_EQ2				0x00B1	/* DAC EQ Control 2 */
#define ALC5658_DAC_EQ3				0x00B2	/* DAC EQ Control 3 */
#define ALC5658_IRQ1				0x00B8	/* IRQ Control 1 */
#define ALC5658_IRQ2				0x00BA	/* IRQ Control 2 */
#define ALC5658_IRQ3				0x00BB	/* IRQ Control 3 */
#define ALC5658_INTERNAL_STATUS1	0x00BE	/* Internal Status 1 */
#define ALC5658_INTERNAL_STATUS2	0x00BF	/* Internal Status 2 */
#define ALC5658_GPIO1				0x00C0	/* GPIO Control 1 */
#define ALC5658_GPIO2				0x00C1	/* GPIO Control 2 */
#define ALC5658_GPIO3				0x00C2	/* GPIO Control 3 */
#define ALC5658_SVOL1				0x00D9	/* Soft Volume and ZCD Control 1 */
#define ALC5658_SVOL2				0x00DA	/* Soft Volume and ZCD Control 2 */
#define ALC5658_3BTN1				0x00DB	/* 3 Buttons Inline Command Control 1 */
#define ALC5658_3BTN2				0x00DC	/* 3 Buttons Inline Command Control 2 */
#define ALC5658_3BTN3				0x00DD	/* 3 Buttons Inline Command Control 3 */
#define ALC5658_3BTN4				0x00DE	/* 3 Buttons Inline Command Control 4 */
#define ALC5658_4BTN1				0x00DF	/* 4 Buttons Inline Command Control 1 */
#define ALC5658_4BTN2				0x00E0	/* 4 Buttons Inline Command Control 2 */
#define ALC5658_4BTN3				0x00E1	/* 4 Buttons Inline Command Control 3 */
#define ALC5658_PWR_SAVE1			0x00E4	/* Power Saving Inline Control 1 */
#define ALC5658_PWR_SAVE2			0x00E5	/* Power Saving Inline Control 2 */
#define ALC5658_STEREO1				0x00EA	/* Stereo1 ADC Wind Filter Control 1 */
#define ALC5658_STEREO2				0x00EB	/* Stereo1 ADC Wind Filter Control 2 */
#define ALC5658_MONO1				0x00EC	/* Mono ADC Wind Filter Control 1 */
#define ALC5658_MONO2				0x00ED	/* Mono ADC Wind Filter Control 2 */
#define ALC5658_GPIO_JD				0x00F6	/* GPIO Jack Detection Control 1 */
#define ALC5658_GENERAL_CONTROL_1	0x00FA	/* General Control 1 */
#define ALC5658_GENERAL_CONTROL_2	0x00FB	/* General Control 2 */
#define ALC5658_0111				0x0110
#define ALC5658_0125				0x0125
#define ALC5658_RESET1				0x013A	/* ADC/DAC RESET Control 1 */
#define ALC5658_RESET2				0x013B	/* ADC/DAC RESET Control 2 */
#define ALC5658_013C				0x013C
#define ALC5658_BASSBACK			0x0150	/* BassBack Control */
#define ALC5658_TRUTREBLE1			0x0151	/* TruTreble Control 1 */
#define ALC5658_TRUTREBLE2			0x0152	/* TruTreble Control 2 */
#define ALC5658_OMNI_SOUND			0x0157	/* Omni Sound Control */
#define ALC5658_NOISE_GATE			0x0160	/* Noise Gate Mode2 Conrol 1 */
#define ALC5658_LDO					0x0192	/* LDO Control */
#define ALC5658_SENSING_CTRL1		0x01C0	/* HP Impedance Sensing Control 1 */
#define ALC5658_SENSING_CTRL2		0x01C2	/* HP Impedance Sensing Control 2 */
#define ALC5658_SENSING_CTRL4		0x01DA	/* HP Impedance Sensing Control 4 */
#define ALC5658_01DE				0x01DE
#define ALC5658_01DF				0x01DF
#define ALC5658_01E4				0x01E4
#define ALC5658_PWR_LIMIT1			0x0200	/* Speaker Power Limiter Control 1 */
#define ALC5658_PWR_LIMIT2			0x0201	/* Speaker Power Limiter Control 2 */
#define ALC5658_PWR_LIMIT3			0x0202	/* Speaker Power Limiter Control 3 */
#define ALC5658_PWR_LIMIT4			0x0204	/* Speaker Power Limiter Control 4 */
#define ALC5658_PWR_LIMIT5			0x0205	/* Speaker Power Limiter Control 5 */
#define ALC5658_SPKVDD				0x0281	/* SPKVDD Voltage Detection Control */
#define ALC5658_SPK_DC_LV_DETECT1	0x0282	/* Speaker DC Level Detection Control 1 */
#define ALC5658_SPK_DC_LV_DETECT2	0x0283	/* Speaker DC Level Detection Control 2 */
#define ALC5658_SPK_DC_LV_DETECT3	0x0284	/* Speaker DC Level Detection Control 3 */
#define ALC5658_SPK_DC_DETECT1		0x0290	/* Speaker DC Detection Control 1 */
#define ALC5658_SPK_DC_DETECT2		0x0291	/* Speaker DC Detection Control 2 */
#define ALC5658_DRCAGC1				0x0300	/* DRC/AGC Control 1 */
#define ALC5658_DRCAGC2				0x0301	/* DRC/AGC Control 2 */
#define ALC5658_DRCAGC3				0x0302	/* DRC/AGC Control 3 */
#define ALC5658_DRCAGC4				0x0303	/* DRC/AGC Control 4 */
#define ALC5658_DRCAGC5				0x0304	/* DRC/AGC Control 5 */
#define ALC5658_DRCAGC_BASS1		0x0308	/* DRC/AGC Bass Control 1 */
#define ALC5658_DRCAGC_BASS2		0x0309	/* DRC/AGC Bass Control 2 */
#define ALC5658_DRCAGC_BASS3		0x030A	/* DRC/AGC Bass Control 3 */
#define ALC5658_DRCAGC_BASS4		0x030B	/* DRC/AGC Bass Control 4 */
#define ALC5658_DRCAGC_BASS5		0x030C	/* DRC/AGC Bass Control 5 */
#define ALC5658_MULTI_DRC			0x0320	/* Multi-Band DRC Control */
#define ALC5658_ALC_PGA1			0x0330	/* ALC PGA Control 1 */
#define ALC5658_ALC_PGA2			0x0331	/* ALC PGA Control 2 */
#define ALC5658_ALC_PGA3			0x0332	/* ALC PGA Control 3 */
#define ALC5658_ALC_PGA4			0x0333	/* ALC PGA Control 4 */
#define ALC5658_ALC_PGA5			0x0334	/* ALC PGA Control 5 */
#define ALC5658_ALC_PGA6			0x0335	/* ALC PGA Control 6 */
#define ALC5658_ALC_PGA7			0x0336	/* ALC PGA Control 7 */
#define ALC5658_ALC_PGA8			0x0337	/* ALC PGA Control 8 */

#endif							/* CONFIG_AUDIO_ALC5658 */
#endif							/* __DRIVERS_AUDIO_ALC5658REG_H */
