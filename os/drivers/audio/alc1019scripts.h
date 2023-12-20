/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#ifndef __DRIVERS_AUDIO_ALC1019SCRIPTS_H
#define __DRIVERS_AUDIO_ALC1019SCRIPTS_H

#include <tinyara/config.h>

#ifdef CONFIG_AUDIO_ALC1019

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "alc1019.h"

/* TYPEDEFS */
typedef struct {
	uint8_t addr_h;
	uint8_t addr_l;
	uint8_t val;
	unsigned int delay;
} t_codec_init_script_entry;

t_codec_init_script_entry codec_reset_script[] = {
	/* 0x0000 for soft reset */
	{ 0x00, 0x00, 0x01, 0}
};
t_codec_init_script_entry codec_stop_script[] = {
	/* set volume 0x0504 to zero on stop */
	{ 0x05, 0x04, 0x00, 0}
};

t_codec_init_script_entry codec_initial_script[] = {
	/* unmute in case muted 0x0505 */
	{ 0x05, 0x05, 0x24, 0},
	/* set it to default volume */
	{ 0x05, 0x04, ALC1019_HW_VOL_DEF, 0}
};

t_codec_init_script_entry codec_init_mute_on_script[] = {
	{ 0x05, 0x05, 0x2C, 0}	
};

t_codec_init_script_entry codec_set_master_volume_script[] = {
	{ 0x05, 0x04, 0x01, 0}
};

#endif	/* CONFIG_AUDIO_ALC1019 */
#endif	/* __DRIVERS_AUDIO_ALC1019SCRIPTS_H */
