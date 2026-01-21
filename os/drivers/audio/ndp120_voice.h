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

#ifndef __DRIVERS_AUDIO_NDP120_H
#define __DRIVERS_AUDIO_NDP120_H

#include <tinyara/audio/audio.h>
#include <tinyara/audio/ndp120.h>
#include <semaphore.h>
#include <queue.h>
#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_evb_io.h>
#include <syntiant_ilib/syntiant_ndp_error.h>

#include <tinyara/pm/pm.h>

#define MAX_LABELS 32

#define NDP120_INIT_RETRY_COUNT 3

#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
enum speech_state_e {
	NDP120_SPI_SPEECH_STATE_NONE = 0,
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	/* ndp120 only supports keyword detect, no epd detection */
	NDP120_SPI_SPEECH_STATE_KD = 1,
#endif
};
typedef enum speech_state_e speech_state_t;
#endif

struct ndp120_dev_s {
	/* common parts */
	struct audio_lowerhalf_s dev; /* ndp120 audio lower half (this device) */
#ifndef CONFIG_AUDIO_EXCLUDE_GAIN
	uint16_t mic_gain;
#endif
	bool mute;
	struct sq_queue_s pendq;	/* Queue of pending buffers to be sent */
	sem_t devsem;			/* Protection for both pendq & dev */
	bool running;			/* True: Worker thread is running */
	bool reserved;			/* True: Device is reserved */

	/* spi parts */
	FAR struct spi_dev_s *spi;

	bool wakeuped; /* True: wakeup was triggered */
	bool reseted;  /* True: spi data was reseted */
	bool compressed;

	/* ndp120 specific data */
	syntiant_evb_io_handle_t io_handle;
	struct syntiant_ndp_device_s *ndp;
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	uint32_t keyword_bytes; 
	uint32_t keyword_bytes_left;
	bool kd_enabled;
#endif
	struct ndp120_lower_s *lower;
	volatile bool fw_loaded;
	bool recording; /* during recording we ignore main keyword detections */
	bool ndp_interrupts_enabled; /* used by mbwait to determine whether wait for sem or poll */
	char *labels_per_network[MAX_NNETWORKS][MAX_LABELS];
	uint32_t sample_size;
	uint32_t sample_size_orig_annot;
	uint8_t *keyword_buffer;
	uint32_t extract_size;
	uint32_t total_size;
	bool extclk_inuse;
	bool keyword_correction;
	struct pm_domain_s *pm_domain;
	uint32_t sample_ready_cnt;

	/* moved to using pthread cond variable for parity with reference implementation in ilib examples */
	pthread_mutex_t ndp_mutex_mbsync;

	pthread_mutex_t ndp_mutex_mcu_mb_in;
	pthread_cond_t ndp_cond_mcu_mb_in;

	pthread_mutex_t ndp_mutex_notification_sample;
	pthread_cond_t ndp_cond_notification_sample;
	int8_t kd_num; // -1 is none, 0 is hi bixby, 1 is bixby
	sem_t reset_sem;
};

#endif				/* __DRIVERS_AUDIO_NDP120_H */
