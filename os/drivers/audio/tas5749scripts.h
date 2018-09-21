/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#ifndef __DRIVERS_AUDIO_TAS5749SCRIPTS_H
#define __DRIVERS_AUDIO_TAS5749SCRIPTS_H

#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_TAS5749

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "tas5749reg.h"

/* TYPEDEFS */
typedef struct {
	char /* TAS5749_REG_DATA_TYPE */ type;
	uint8_t addr;  /* TAS5749_REG_1BYTE_MODE addr or TAS5749_REG_4BYTE_MODE addr */
	uint8_t val[4];
	unsigned int delay;
} t_codec_init_script_entry;

typedef struct {
	char /* TAS5749_REG_DATA_TYPE */ type;
	uint8_t addr;  /* TAS5749_REG_1BYTE_MODE addr or TAS5749_REG_4BYTE_MODE addr */
	char *name;
} t_codec_dump_entry;

t_codec_init_script_entry codec_reset_script[] = {
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE, {0x00,}, 0}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_SOFT_RESET, {0x01,}, 30}, /* Soft Reset */
};
t_codec_init_script_entry codec_stop_script[] = {
	//on mute
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,      {0x00,}, 0}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_MUTE_TIME,     {0x01,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0x00,}, 0}
};

t_codec_init_script_entry codec_initial_script[] = { /* TAS5747 final setting with no EQ.TXT */
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,   {0x00,}, 0}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_SOFT_RESET, {0x01,}, 30}, /* Soft Reset */

	{TAS5749_REG_D_1BYTE, TAS5749_EFUSE_CMD,  {0x03,}, 0}, /* Clock resistor setting */
	{TAS5749_REG_D_1BYTE, TAS5749_CLOCK_CTRL, {0x00,}, 0}, /* LRCK range(Sampling rate)   0x00 setting    PLL Range
								* 8kHz, 44.1kHz              0x00            38~57kHz
								* 32kHz                       0x10            24~37kHz
								*/

	{TAS5749_REG_D_1BYTE, TAS5749_MODULATION_LIMIT, {0x02,}, 0}, /* MI = 98% */

	/* Scale value */
	{TAS5749_REG_D_1BYTE, TAS5749_AMP_SCALE,      {0x80,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_LEGACY_SYSCTL2, {0x48,}, 0},  // 0x48 sclk_source is MCLK
	/* Inter Channel Delay */
	{TAS5749_REG_D_1BYTE, TAS5749_IC_DELAY_CH1, {0x80,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_IC_DELAY_CH2, {0xD4,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_IC_DELAY_CH3, {0x80,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_IC_DELAY_CH4, {0xD4,}, 0},

	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE, {0x01,}, 0}, /* 4byte I2C Write */

	{TAS5749_REG_D_4BYTE, TAS5749_OUTPUT_PRESCALE,   {0x00, 0x01, 0x0F, 0x00}, 0}, /* Pre Scale setting */
	{TAS5749_REG_D_4BYTE, TAS5749_OUTPUT_POST_SCALE, {0x00, 0x30, 0x00, 0x00}, 0}, /* Post Scale setting */ //e.g.) 13V 6ohm 10W - 0x00 35 00 00 

	/* 2Band DRC setting (DRC attack/release/X-over setting) */
	{TAS5749_REG_D_4BYTE, TAS5749_AGL1_SOFTENING_ALPHA,         {0x00, 0x01, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL1_SOFTENING_OMEGA,         {0x00, 0x7F, 0xFE, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL1_ATTACK_RATE,             {0x00, 0x04, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL1_RELEASE_RATE,            {0xFF, 0xFC, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL2_SOFTENING_ALPHA,         {0x00, 0x00, 0x01, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL_SOFTENING_OMEGA,          {0x00, 0x7F, 0xFF, 0x7F}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL2_ATTACK_RATE,             {0x00, 0x0A, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL2_RELEASE_RATE,            {0xFF, 0xF8, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL1_THRESHOLD,               {0x09, 0x00, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL2_THRESHOLD,               {0x04, 0x00, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_2_CROSSOVER_BIQUAD_B0,   {0x00, 0x3F, 0xCA, 0x8E}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_2_CROSSOVER_BIQUAD_B1,   {0x03, 0xC0, 0x35, 0x72}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_2_CROSSOVER_BIQUAD_B2,   {0x00, 0x00, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_2_CROSSOVER_BIQUAD_A0,   {0x00, 0x3F, 0x95, 0x1D}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_2_CROSSOVER_BIQUAD_A1,   {0x00, 0x00, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL_CHECKSUM,                 {0x03, 0xC0, 0x6A, 0xE1}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_AGL_ADVANC_ED_MODE_GAIN,      {0x00, 0x02, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_OUTPUT_MIXER_LEFT,       {0x00, 0x40, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_1_OUTPUT_MIXER_RIGHT,      {0x00, 0x40, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_2_OUTPUT_MIXER_LEFT,       {0x00, 0x40, 0x00, 0x00}, 0},
	{TAS5749_REG_D_4BYTE, TAS5749_CH_2_OUTPUT_MIXER_RIGHT,      {0x00, 0x40, 0x00, 0x00}, 0},

	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE, {0x00,}, 0},   /* 1byte I2C Write */
	{TAS5749_REG_D_1BYTE, TAS5749_FUNCTION, {0x64,}, 0},   /* Speaker EQ On/Off setting
								* On  : 0x0A=0x64 (PEQ/DRC/CheckSum Enable)
								* Off : 0x0A=0x44 (PEQ  Disable)
								*/

	/* peq_on : 0x06 = Amp Vol(Factory Data) + peq_volume
	 *          0x07 = Amp Vol(Factory Data) + peq_volume
	 * peq_on : 0x06 = Amp Vol(Factory Data) 
	 *          0x07 = Amp Vol(Factory Data)
	 */
	{TAS5749_REG_D_1BYTE, TAS5749_VOLUME_CH1, {0xC5,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_VOLUME_CH2, {0xC5,}, 0},

	{TAS5749_REG_D_1BYTE, TAS5749_GENERAL_STATUS, {0x00,}, 0}, // Auto Recovery Initializing 

	/*Mute off*/
	{TAS5749_REG_D_1BYTE, TAS5749_MUTE_TIME,     {0x01,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0x03,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0xFF,}, 0},

	{TAS5749_REG_D_1BYTE, 0x13, {0x01,}, 0},
	{TAS5749_REG_D_1BYTE, 0x12, {0x92,}, 0},
	{TAS5749_REG_D_1BYTE, 0x23, {0x05,}, 0},  // Level meter on // TAS5749_PWM_LEVEL_METER_AND_SDOUT_MUX

	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,   {0x00,}, 0} /* 1byte I2C write */
};

t_codec_init_script_entry codec_init_mute_on_script[] = {
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,      {0x00,}, 0}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_MUTE_TIME,     {0x01,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0x00,}, 0}
};

t_codec_init_script_entry codec_set_master_volume_script[] = {
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,      {0x00,}, 0}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_MUTE_TIME,     {0x01,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0x03,}, 0},
	{TAS5749_REG_D_1BYTE, TAS5749_MASTER_VOLUME, {0xFF,}, 0}
};


t_codec_dump_entry codec_dump_script[] = {
	{TAS5749_REG_D_1BYTE, TAS5749_I2C_MODE,     "TAS5749_I2C_MODE"}, /* 1byte I2C write */
	{TAS5749_REG_D_1BYTE, TAS5749_TI_DEVICE_ID, "TAS5749_DEVICE_ID"} /* Device ID */
};

#endif	/* CONFIG_AUDIO_TAS5749 */
#endif	/* __DRIVERS_AUDIO_TAS5749SCRIPTS_H */
