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

/* TYPEDEFS */
typedef struct {
	uint8_t addr;
	uint8_t val;
	unsigned int delay;
} t_codec_init_script_entry;

t_codec_init_script_entry codec_reset_script[] = {
	/* 0x0F for soft reset */
	{ 0x0F, 0x01, 0}
};
t_codec_init_script_entry codec_stop_script[] = {
	/* set volume 0x07 to zero on stop */
	{ 0x07, 0x00, 0}
};

t_codec_init_script_entry codec_initial_script[] = {
	/* TODO : unmute in case muted 0x06 */
	{ 0x06, 0x20, 0},
	/* set it to default volume */
	{ 0x07, SYU645B_HW_VOL_DEF, 0}
};

t_codec_init_script_entry codec_init_mute_on_script[] = {
	{ 0x06, 0x28, 0}
};

t_codec_init_script_entry codec_set_master_volume_script[] = {
	{ 0x07, 0x01, 0}
};

#endif	/* CONFIG_AUDIO_SYU645B */
#endif	/* __DRIVERS_AUDIO_SYU645BSCRIPTS_H */
