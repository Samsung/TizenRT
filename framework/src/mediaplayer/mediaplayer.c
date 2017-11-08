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

#include <tinyara/config.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/pcm.h>
#include <tinyalsa/tinyalsa.h>
#include <tinyara/fs/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>
#include <string.h>
#include <mediaplayer/mediaplayer.h>
#include "mplayer_debug.h"

/***
 * Structure
 */
struct media_s {
	dq_entry_t q;
	int fd;
	media_op_t op;
	media_state_t state;
	media_type_t type;
	media_play_type_t play_type;
	struct pcm_config *config;
};

/***
 * Static variables
 */
static pthread_t g_pth_playing;
#ifdef CONFIG_AUDIO_MULTI_CARD
static pthread_t g_pth_recording;
#endif
static sem_t g_sem_playing;
static sem_t g_sem_recording;
static pthread_mutex_t g_mutex_playing_q;
static pthread_mutex_t g_mutex_recording_q;

static bool g_playing_live;
static bool g_recording_live;

static dq_queue_t g_playing_q;
static dq_queue_t g_recording_q;

/***
 * Pre-definitions
 */
media_result_t audio_playing(void *args);
media_result_t audio_recording(void *args);

media_result_t media_init(struct media_cb_s *cbs)
{
	pthread_attr_t attr;
	pthread_mutexattr_t mutexAttr;
	struct sched_param sparam;

	// Init dq and sem
	dq_init(&g_playing_q);
	sem_init(&g_sem_playing, 0, 0);

	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&g_mutex_playing_q, &mutexAttr);


#ifdef CONFIG_AUDIO_MULTI_CARD
	dq_init(&g_recording_q);
	sem_init(&g_sem_recording, 0, 0);
	pthread_mutex_init(&g_mutex_recording_q, &mutexAttr);
#endif
	// Init threads
	if (pthread_attr_init(&attr) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}

	sparam.sched_priority = 150;
	if (pthread_attr_setschedparam(&attr, &sparam) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_attr_setstacksize(&attr, 8192) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_create(&g_pth_playing, &attr, (pthread_startroutine_t)audio_playing, (void *)NULL) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}
#ifdef CONFIG_AUDIO_MULTI_CARD
	if (pthread_create(&g_pth_recording, &attr, (pthread_startroutine_t)audio_recording, (void *)NULL) != 0) {
		return MEDIA_ERROR_THREAD_INIT;
	}
#endif
	return MEDIA_OK;
}

void media_shutdown(void)
{
	// Shutdown playback thread
	g_playing_live = false;
	sem_post(&g_sem_playing);
	pthread_join(g_pth_playing, NULL);
	pthread_mutex_destroy(&g_mutex_playing_q);
	sem_destroy(&g_sem_playing);

	// Shutdown record thread
#ifdef CONFIG_AUDIO_MULTI_CARD
	g_recording_live = false;
	sem_post(&g_sem_recording);
	pthread_join(g_pth_recording, NULL);
	pthread_mutex_destroy(&g_mutex_recording_q);
	sem_destroy(&g_sem_recording);
#endif
}

media_result_t media_play(media_t *m, bool loop)
{
	pthread_mutex_lock(&g_mutex_playing_q);
	dq_addlast((dq_entry_t *)m, &g_playing_q);
	pthread_mutex_unlock(&g_mutex_playing_q);
	sem_post(&g_sem_playing);
	return MEDIA_OK;
}

media_result_t media_record(media_t *m)
{
#ifdef CONFIG_AUDIO_MULTI_CARD
	pthread_mutex_lock(&g_mutex_recording_q);
	dq_addlast((dq_entry_t *)m, &g_recording_q);
	pthread_mutex_unlock(&g_mutex_recording_q);
	sem_post(&g_sem_recording);
#else
	pthread_mutex_lock(&g_mutex_playing_q);
	dq_addlast((dq_entry_t *)m, &g_playing_q);
	pthread_mutex_unlock(&g_mutex_playing_q);
	sem_post(&g_sem_playing);
#endif
	return MEDIA_OK;
}

media_result_t media_stop(media_t *m)
{
	if (m->state == MEDIA_STATE_PLAYING || m->state == MEDIA_STATE_RECORDING) {
		m->state = MEDIA_STATE_STOPPING;
		return MEDIA_OK;
	}  
	return MEDIA_ERROR;
}

media_result_t media_pause(media_t *m)
{
	if (m->state == MEDIA_STATE_PLAYING) {
		m->state = MEDIA_STATE_PAUSING;
		return MEDIA_OK;
	}
	return MEDIA_ERROR;
}

media_result_t media_resume(media_t *m)
{
	if (m->state == MEDIA_STATE_PAUSED) {
		m->state = MEDIA_STATE_PLAYING;
		sem_post(&g_sem_playing);
		return MEDIA_OK;
	}
	return MEDIA_ERROR;
}

media_state_t media_get_state(media_t *m)
{
	return m->state;
}

static media_result_t read_wav_header(media_t *m)
{
	//struct pcm_config config;
	struct wav_header_s header;
	int ret;

	ret = read(m->fd, &header, sizeof(header));
	if (ret != sizeof(header)) {
		return MEDIA_ERROR_UNKNOWN_FILE;
	}

	// Todo: Add header information to media_t

	return MEDIA_OK;
}

media_type_t get_media_type(char *path, media_t * m)
{
	char *dot = strrchr(path, '.');
	if (dot != NULL) {
		if (strcmp(dot, ".wav") == 0) {
			return MEDIA_TYPE_WAV;
		}
	}
	return MEDIA_TYPE_PCM;
}

media_t *media_open(char *path, media_op_t op, media_play_type_t play_type, struct pcm_config *config)
{
	media_t *m = (media_t *)malloc(sizeof(media_t));
	if (m == NULL) {
		return NULL;
	}

	m->fd = -1;
	m->op = op;
	m->state = MEDIA_STATE_CREATED;
	m->play_type = play_type;
	m->config = config;
	if (play_type == MEDIA_PLAY_TYPE_FILE) {
		m->type = get_media_type(path, m);
		if (m->type == MEDIA_TYPE_WAV || m->type == MEDIA_TYPE_PCM) {
			if (op == MEDIA_OP_PLAYBACK) {
				m->fd = open(path, O_RDONLY);
				if (m->fd < 0) {
					goto error_out;
				}

				if (m->type == MEDIA_TYPE_WAV) {
					read_wav_header(m);
				}
				m->state = MEDIA_STATE_PLAYING;
				return m;
			} else if (op == MEDIA_OP_RECORD) {
				m->fd = open(path, O_RDWR | O_CREAT | O_TRUNC);
				if (m->fd < 0) {
					goto error_out;
				}
				m->state = MEDIA_STATE_RECORDING;
				return m;
			}
		}

	}
error_out:
	free(m);
	return NULL;
}

media_result_t media_close(media_t *m)
{
	int ret;
	if (m == NULL) {
		return MEDIA_OK;
	}
	if (m->fd >= 0) {
		ret = close(m->fd);
		if (ret < 0) {
			return MEDIA_ERROR;
		}
	}
	free(m);
	return MEDIA_OK;
}


int on_media_state_playing(struct pcm *pcmout, media_t *m, char *buffer, unsigned int buffer_size)
{
	int readed;
	int remain;
	int ret;
	readed = read(m->fd, buffer, buffer_size);
	if (readed) {
		remain = pcm_bytes_to_frames(pcmout, readed);
		while (remain > 0) {
			ret = pcm_writei(pcmout, buffer + readed - pcm_frames_to_bytes(pcmout, remain), remain);
			if (ret > 0) {
				remain -= ret;
			} else {
				return ret;
			}
		}
	}

	return readed;
}

int on_media_state_recording(struct pcm *pcmin, media_t *m, char *buffer, unsigned int buffer_size)
{
	int readed;
	int remain;
	int ret;

	readed = pcm_readi(pcmin, buffer, pcm_bytes_to_frames(pcmin, buffer_size));
	if (readed) {
		remain = pcm_frames_to_bytes(pcmin, readed);
		while (remain > 0) {
			ret = write(m->fd, buffer + pcm_frames_to_bytes(pcmin, readed) - remain, remain);
			if (ret > 0) {
				remain -= ret;
			} else {
				return ret;
			}
		}
	}
	return readed;
}

/***
 * Thread routine
 */
media_result_t audio_playing(void *args)
{
	char *buffer;
	unsigned int buffer_size;
	struct pcm *pcmout;
	dq_entry_t *node;
	dq_entry_t *tmp_node;
	int ret;
#ifndef CONFIG_AUDIO_MULTI_CARD
	struct pcm *pcmin;
	bool playing = false;
	bool recording = false;
	pcmin = NULL;
#endif
	buffer = NULL;
	buffer_size = 0;
	pcmout = NULL;

#ifdef CONFIG_AUDIO_MULTI_CARD
	MPLAYER_VERBOSE("audio_playing thread started. Buffer size: %d\n", buffer_size);
#else
	MPLAYER_VERBOSE("audio thread started.\n");
#endif

	g_playing_live = true;

	sem_wait(&g_sem_playing);
	while (g_playing_live) {
		pthread_mutex_lock(&g_mutex_playing_q);
		for (node = (dq_entry_t *)dq_peek(&g_playing_q); node && g_playing_live;) {
			switch (((media_t *)node)->state) {
			case MEDIA_STATE_PLAYING:
#ifndef CONFIG_AUDIO_MULTI_CARD
				if (recording) {
					node = dq_next(node);
					continue;
				}
				if (!playing) {
#endif
					pcmout = pcm_open(0, 0, PCM_OUT, ((media_t *)node)->config);
					buffer_size = pcm_frames_to_bytes(pcmout, pcm_get_buffer_size(pcmout));
					buffer = (char *)malloc(buffer_size);
					if (buffer == NULL) {
						((media_t *)node)->state = MEDIA_STATE_STOPPING;
						node = dq_next(node);
						pcm_close(pcmout);
						continue;
					}
#ifndef CONFIG_AUDIO_MULTI_CARD
					playing = true;

				}

#endif
				ret = on_media_state_playing(pcmout, (media_t *)node, buffer, buffer_size);
				if (ret == 0) {
					// If playing is done,
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				} else if (ret < 0) {
					// Todo: Error handling
					MPLAYER_ERROR("Playing error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				}
				// pcm_drain call needed to ensure that all buffers are played till this point.
				pcm_close(pcmout);
				pcmout = NULL;
				free(buffer);
				break;
#ifndef CONFIG_AUDIO_MULTI_CARD
			case MEDIA_STATE_RECORDING:
				if (playing) {
					node = dq_next(node);
					continue;
				}
				if (!recording) {
					pcmin = pcm_open(0, 0, PCM_IN, NULL);
					buffer_size = pcm_frames_to_bytes(pcmin, pcm_get_buffer_size(pcmin));
					buffer = (char *)malloc(buffer_size);
					if (buffer == NULL) {
						((media_t *)node)->state = MEDIA_STATE_STOPPING;
						node = dq_next(node);
						pcm_close(pcmin);
						continue;
					}
					recording = true;
				}
				if (on_media_state_recording(pcmin, (media_t *)node, buffer, buffer_size) < 0) {
					// Todo: Error handling
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
					MPLAYER_ERROR("Recording error!\n");
				}
				pcm_close(pcmin);
				pcmin = NULL;
				free(buffer);
				break;
#endif

			case MEDIA_STATE_PAUSING:
				((media_t *)node)->state = MEDIA_STATE_PAUSED;
				sem_wait(&g_sem_playing);
				break;

			case MEDIA_STATE_PAUSED:
				break;

			case MEDIA_STATE_STOPPING:
#ifndef CONFIG_AUDIO_MULTI_CARD
				if (playing) {
					playing = false;
				}
				if (recording) {
					recording = false;
				}
#endif
				tmp_node = node;
				node = dq_next(node);
				dq_rem(tmp_node, &g_playing_q);
				sem_wait(&g_sem_playing);
				continue;
			default:
				break;
			}

			node = dq_next(node);
		}
		pthread_mutex_unlock(&g_mutex_playing_q);

		usleep(1);
	}

	sem_post(&g_sem_playing);

	MPLAYER_VERBOSE("audio_playing thread terminated.\n");
	return MEDIA_OK;
}

media_result_t audio_recording(void *args)
{
	char *buffer;
	unsigned int buffer_size;
	struct pcm *pcmin;
	dq_entry_t *node;
	dq_entry_t *tmp_node;
	buffer_size = 0;
	MPLAYER_VERBOSE("audio_recording thread started. Buffer size: %d\n", buffer_size);

	g_recording_live = true;

	sem_wait(&g_sem_recording);
	while (g_recording_live) {
		pthread_mutex_lock(&g_mutex_recording_q);
		for (node = (dq_entry_t *)dq_peek(&g_recording_q); node && g_recording_live;) {
			switch (((media_t *)node)->state) {
			case MEDIA_STATE_RECORDING:
				pcmin = pcm_open(0, 0, PCM_IN, ((media_t *)node)->config);
				buffer_size = pcm_frames_to_bytes(pcmin, pcm_get_buffer_size(pcmin));
				buffer = (char *)malloc(buffer_size);
				if (buffer == NULL) {
					pcm_close(pcmin);
					pcmin = NULL;
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
					continue;
				}
				if (on_media_state_recording(pcmin, (media_t *)node, buffer, buffer_size) < 0) {
					// Todo: Error handling
					MPLAYER_ERROR("Recording error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				}
				free(buffer);
				pcm_close(pcmin);
				pcmin = NULL;
				break;

			case MEDIA_STATE_PAUSING:
				((media_t *)node)->state = MEDIA_STATE_PAUSED;
				sem_wait(&g_sem_recording);
				break;

			case MEDIA_STATE_PAUSED:
				break;

			case MEDIA_STATE_STOPPING:
				tmp_node = dq_next(node);
				dq_rem(node, &g_recording_q);
				node = tmp_node;
				sem_wait(&g_sem_recording);
				continue;
			default:
				break;
			}

			node = dq_next(node);
		}
		pthread_mutex_unlock(&g_mutex_recording_q);

		usleep(1);
	}

	sem_post(&g_sem_recording);
	MPLAYER_VERBOSE("audio_recording thread terminated.\n");
	return MEDIA_OK;
}
