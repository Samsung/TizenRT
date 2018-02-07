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

#include <media/media_player.h>
#include <pthread.h>

#define IS_SOCKET(fd) (fd > CONFIG_NFILE_DESCRIPTORS)
#define PLAY_LOCK() do { pthread_mutex_lock(&g_pc.mutex_play); } while (false)
#define PLAY_UNLOCK() do { pthread_mutex_unlock(&g_pc.mutex_play); } while (false)

#define ST_PROTO_PLAY	0x00
#define ST_PROTO_STOP	0x01
#define ST_PROTO_PAUSE	0x02
#define ST_PROTO_RESUME	0x03

enum play_state_e {
	PLAY_ERROR,
	PLAY_AVAILABLE,
	PLAY_RUNNING,
	PLAY_PAUSED,
	PLAY_STOPPING,
	PLAY_DRAINING
};
typedef enum play_state_e play_state_t;

struct media_play_context_s {
	pthread_t           pth;
	bool                thread_running;
	pthread_mutexattr_t mutex_attr_play;
	pthread_mutex_t     mutex_play;
	sem_t               sem_pause;
	struct pcm         *pcmout;
	unsigned int        buffer_size;
	int                 fd;
	int		    download_size;
	media_format_t      format;
	play_state_t        state;
};

static struct media_play_context_s g_pc;
int player_worker(void *args);

/**
 * Public Functions
 */
play_result_t media_play(int fd, media_format_t format)
{
	g_pc.fd = fd;

	struct pcm_config config;
	config.channels = 2;
	config.rate = 16000;
	config.period_size = CONFIG_AUDIO_BUFSIZE;
	config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	config.format = PCM_FORMAT_S16_LE;

	g_pc.pcmout = pcm_open(0, 0, PCM_OUT, &config);
	if (!g_pc.pcmout) {
		return PLAY_ERROR_DRIVER;
	}

	g_pc.buffer_size = pcm_frames_to_bytes(g_pc.pcmout, pcm_get_buffer_size(g_pc.pcmout));
	g_pc.download_size = 0;

	if (IS_SOCKET(fd)) {
		char instruction = ST_PROTO_PLAY;
		send(g_pc.fd, &instruction, 1, 0);
	}

	pthread_attr_t attr;
	struct sched_param sparam;

	PLAY_LOCK();

	if (g_pc.state == PLAY_STOPPING) {
		g_pc.thread_running = false;
		pthread_join(g_pc.pth, NULL);
		g_pc.state = PLAY_AVAILABLE;
	}

	if (g_pc.state == PLAY_AVAILABLE) {

		g_pc.state = PLAY_RUNNING;
		g_pc.thread_running = true;
		if (pthread_attr_init(&attr) != 0) {
			goto error_thread_init;
		}

		sparam.sched_priority = CONFIG_MEDIA_PRIORITY;
		if (pthread_attr_setschedparam(&attr, &sparam) != 0) {
			goto error_thread_init;
		}

		if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
			goto error_thread_init;
		}

		if (pthread_attr_setstacksize(&attr, CONFIG_MEDIA_STACK_SIZE) != 0) {
			goto error_thread_init;
		}

		if (pthread_create(&g_pc.pth, &attr, (pthread_startroutine_t)player_worker, NULL) != 0) {
			goto error_thread_init;
		}
	}

	PLAY_UNLOCK();
	return MEDIA_OK;

error_thread_init:
	PLAY_UNLOCK();
	return PLAY_ERROR_THREAD_INIT;
}

int media_get_duration(int fd)
{
	return -1;
}

play_result_t media_stop_play(void)
{
	PLAY_LOCK();

	if (g_pc.state == PLAY_PAUSED) {
		g_pc.state = PLAY_STOPPING;
		sem_post(&g_pc.sem_pause);
	} else if (g_pc.state != PLAY_RUNNING) {
		PLAY_UNLOCK();
		return PLAY_ERROR_INVALID_STATE;
	}

	if (IS_SOCKET(g_pc.fd)) {
		char instruction = ST_PROTO_STOP;
		send(g_pc.fd, &instruction, 1, 0);
	}

	g_pc.state = PLAY_STOPPING;

	PLAY_UNLOCK();

	pthread_join(g_pc.pth, NULL);	
	g_pc.pth = -1;

	return MEDIA_OK;
}

play_result_t media_pause_play(void)
{
	PLAY_LOCK();

	if (g_pc.state != PLAY_RUNNING) {
		PLAY_UNLOCK();
		return MEDIA_ERROR_INVALID_PARAMETER;
	}

	if (IS_SOCKET(g_pc.fd)) {
		char instruction = ST_PROTO_PAUSE;
		send(g_pc.fd, &instruction, 1, 0);
	}

	g_pc.state = PLAY_PAUSED;

	PLAY_UNLOCK();

	return MEDIA_OK;
}

play_result_t media_resume_play(void)
{
	if (g_pc.state != PLAY_PAUSED) {
		return MEDIA_ERROR_INVALID_PARAMETER;
	}

	if (IS_SOCKET(g_pc.fd)) {
		char instruction = ST_PROTO_RESUME;
		send(g_pc.fd, &instruction, 1, 0);
	}

	g_pc.state = PLAY_RUNNING;
	sem_post(&g_pc.sem_pause);

	return MEDIA_OK;
}

play_result_t media_set_vol(unsigned char vol)
{
	return vol;
}

unsigned char media_get_vol(void)
{
	return 0;
}

play_result_t media_set_position(int time_ms)
{
	return MEDIA_OK;
}

int media_get_position(void)
{
	return 0;
}

bool media_is_available(void)
{
	return true;
}

bool media_is_playing(void)
{
	return (g_pc.state == PLAY_RUNNING);
}

/**
 * Internal Functions
 */

int read_data_pcm_write(char *buffer)
{
	int readed = 0;
	int remain;
	int ret;

	while (readed < g_pc.buffer_size) {
		if (IS_SOCKET(g_pc.fd)) {
			ret = recv(g_pc.fd, buffer + readed, g_pc.buffer_size - readed, 0);
		} else {
			ret = read(g_pc.fd, buffer + readed, g_pc.buffer_size - readed);
		}
		printf("ret = %d\n", ret);
		readed += ret;
		if (ret == 0) {
			break;
		}
	}
	g_pc.download_size += readed;
	if (readed) {
		remain = pcm_bytes_to_frames(g_pc.pcmout, readed);
		while (remain > 0) {
			ret = pcm_writei(g_pc.pcmout, buffer + readed - pcm_frames_to_bytes(g_pc.pcmout, remain), remain);
			if (ret > 0) {
				remain -= ret;
			} else {
				return ret;
			}
		}
	}

	return readed;
}

int player_worker(void *args)
{
	int ret;
	char *buffer = NULL;
	buffer = (char *)malloc(g_pc.buffer_size);

	if (!buffer) {
		PLAY_LOCK();
		g_pc.state = PLAY_STOPPING;
		PLAY_UNLOCK();
		return PLAY_ERROR_MALLOC;
	}

	while (g_pc.thread_running) {
		PLAY_LOCK();
		switch (g_pc.state) {
		case PLAY_RUNNING:
			ret = read_data_pcm_write(buffer);
			if (ret == 0) {
				g_pc.state = PLAY_DRAINING;
			} else if (ret < 0) {
				g_pc.state = PLAY_STOPPING;
			}
			break;

		case PLAY_PAUSED:
			PLAY_UNLOCK();
			sem_wait(&g_pc.sem_pause);
			PLAY_LOCK();
			break;

		case PLAY_STOPPING:
			g_pc.thread_running = false;
			break;

		case PLAY_DRAINING:
			pcm_drain(g_pc.pcmout);
			g_pc.state = PLAY_STOPPING;

		default:
			g_pc.thread_running = false;
			g_pc.state = PLAY_STOPPING;
			break;
		}
		PLAY_UNLOCK();

		// For context-switch
		sched_yield();
	}

	free(buffer);

	pcm_close(g_pc.pcmout);
	g_pc.pcmout = NULL;

	PLAY_LOCK();
	g_pc.state = PLAY_ERROR;
	PLAY_UNLOCK();

	return 0;
}

play_result_t media_play_init(void)
{
	if (g_pc.state == PLAY_ERROR) {
		memset(&g_pc, 0, sizeof(g_pc));

		g_pc.fd = -1;
		g_pc.state = PLAY_AVAILABLE;

		pthread_mutexattr_init(&g_pc.mutex_attr_play);
		pthread_mutexattr_settype(&g_pc.mutex_attr_play, PTHREAD_MUTEX_NORMAL);
		pthread_mutex_init(&g_pc.mutex_play, &g_pc.mutex_attr_play);

		sem_init(&g_pc.sem_pause, 0, 0);

		return PLAY_OK;
	}

	return PLAY_ERROR;
}
