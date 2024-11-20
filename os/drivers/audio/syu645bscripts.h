/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#ifndef __DRIVERS_AUDIO_SYU645BSCRIPTS_H
#define __DRIVERS_AUDIO_SYU645BSCRIPTS_H

#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_SYU645B

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "syu645b.h"

typedef enum reg_data_type{
	SYU645B_REG_D_2BYTE = 2,
	SYU645B_REG_D_3BYTE = 3,
	SYU645B_REG_D_4BYTE = 4,
	SYU645B_REG_D_5BYTE = 5,
	SYU645B_REG_D_20BYTE = 20,
	SYU645B_REG_DATA_TYPE_MAX = 20,
} reg_data_type;

/* TYPEDEFS */
typedef struct {
	uint8_t addr;
	uint8_t val[20];
	unsigned int delay;
	enum reg_data_type type;
} t_codec_init_script_entry;

t_codec_init_script_entry codec_reset_script[] = {
	/* 0x0F for soft reset */
	{ 0x0F, {0x01,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_stop_script[] = {
	/* set volume 0x0007 to zero on stop */
	{ 0x07, {0x00,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_initial_script[] = {
	/* Initialize AMP sequence */
	{ 0x0F, {0x01,}, 0, SYU645B_REG_D_2BYTE}, // reset & wait
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x01, {0x00,}, 0, SYU645B_REG_D_2BYTE},

	/* Audio FS/Format Setting */
	{ 0x00, {0x1A,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x15, {0x10,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x06, {0x08,}, 0, SYU645B_REG_D_2BYTE},

	/* Audio Protection & Performance Setting */
	{ 0x1B, {0xBD,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x23, {0x1A,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x05, {0x02,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x76, {0x0F,}, 0, SYU645B_REG_D_2BYTE},

	{ 0x82, {0x00, 0x00, 0x00, 0xF0,}, 0, SYU645B_REG_D_5BYTE},
	{ 0x85, {0x00, 0x00, 0x00, 0x06,}, 0, SYU645B_REG_D_5BYTE},
	{ 0x86, {0x00, 0x00, 0x30, 0x10,}, 0, SYU645B_REG_D_5BYTE},
	{ 0x94, {0x00, 0x00, 0x00, 0x11,}, 0, SYU645B_REG_D_5BYTE},
	{ 0x95, {0x40, 0x00, 0x32, 0x10,}, 0, SYU645B_REG_D_5BYTE},
	{ 0x96, {0x10, 0x00, 0x5C, 0xAA,}, 0, SYU645B_REG_D_5BYTE},
	{ 0xB0, {0x00, 0x00, 0x07, 0xFF,}, 0, SYU645B_REG_D_5BYTE},
	
	{ 0x19, {0x15,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x10, {0x77,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x11, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x12, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x13, {0x06,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x14, {0x06,}, 0, SYU645B_REG_D_2BYTE},
	/* SDATA OUT for AEC */
	{ 0x17, {0x80,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x18, {0x00,}, 0, SYU645B_REG_D_2BYTE},

};

t_codec_init_script_entry codec_init_mute_on_script[] = {
	{ 0x06, {0x08,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x22, {0x30,}, 0, SYU645B_REG_D_2BYTE},
};

t_codec_init_script_entry codec_init_mute_off_script[] = {
	{ 0x22, {0x00,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x06, {0x00,}, 0, SYU645B_REG_D_2BYTE},
};

t_codec_init_script_entry codec_set_master_volume_script[] = {
	{ 0x07, {0x01,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_set_samprate_32k_script[] = {
	{ 0x00, {0x10,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_set_samprate_44k_script[] = {
	{ 0x00, {0x9A,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_set_samprate_48k_script[] = {
	{ 0x00, {0x1A,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry codec_set_samprate_96k_script[] = {
	{ 0x00, {0x15,}, 0, SYU645B_REG_D_2BYTE}
};

t_codec_init_script_entry t_codec_dq_preset_0_script[] = {
	{ 0x1E, {0x01,}, 0, SYU645B_REG_D_2BYTE},
	{ 0x07, {0xFF,}, 0, SYU645B_REG_D_2BYTE},
};
#endif	/* CONFIG_AUDIO_SYU645B */
#endif	/* __DRIVERS_AUDIO_SYU645BSCRIPTS_H */
