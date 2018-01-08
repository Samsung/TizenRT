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

#include <media/media_recorder.h>
#include <media/media_utils.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define IS_SOCKET(fd) (fd > CONFIG_NFILE_DESCRIPTORS)
#define RECORD_LOCK() do { pthread_mutex_lock(&g_rc.mutex_record); } while (false)
#define RECORD_UNLOCK() do { pthread_mutex_unlock(&g_rc.mutex_record); } while (false)

#define ST_PROTO_RECORD		0x04
#define ST_PROTO_RECORD_DATA	0x05
#define ST_PROTO_RECORD_PAUSE	0x02
#define ST_PROTO_RECORD_STOP	0x01

enum record_state_e {
	RECORD_ERROR,
	RECORD_AVAILABLE,
	RECORD_RUNNING,
	RECORD_PAUSED,
	RECORD_STOPPING,
	RECORD_DROPPING	
};
typedef enum record_state_e record_state_t;

struct media_record_context_s {
	pthread_t           pth;
	bool                thread_running;
	pthread_mutexattr_t mutex_attr_record;
	pthread_mutex_t     mutex_record;
	sem_t               sem_pause;
	struct pcm         *pcmin;
	struct pcm_config  *rec_config;
	unsigned int        buffer_size;
	int                 fd;
	media_format_t      format;
	record_state_t      state;
};

static struct media_record_context_s g_rc;
static bool b_started = false;

int recorder_worker(void *args);

/**
 * Public Functions
 */
record_result_t media_record(int fd)
{
	pthread_attr_t attr;
	struct sched_param sparam;

	g_rc.fd = fd;

	if (IS_SOCKET(g_rc.fd)) {
		char instruction = ST_PROTO_RECORD;
		send(g_rc.fd, &instruction, 1, 0);
	}

	RECORD_LOCK();

	if (g_rc.state != RECORD_STOPPING && g_rc.state != RECORD_AVAILABLE) {
		RECORD_UNLOCK();
		return RECORD_ALREADY_INITIALIZED;
	}

	if (g_rc.state == RECORD_STOPPING) {
		pthread_join(g_rc.pth, NULL);
		g_rc.state = RECORD_AVAILABLE;
	}

	if (g_rc.state == RECORD_AVAILABLE) {
		g_rc.fd = fd;
		g_rc.thread_running = true;

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
		if (pthread_create(&g_rc.pth, &attr, (pthread_startroutine_t)recorder_worker, NULL) != 0) {
			goto error_thread_init;
		}

		g_rc.state = RECORD_RUNNING;
	}

	RECORD_UNLOCK();
	return RECORD_OK;

error_thread_init:
	g_rc.thread_running = false;
	RECORD_UNLOCK();
	return RECORD_ERROR_THREAD_INIT;
}

record_result_t media_record_set_config(int channel, int sample_rate, int pcm_format, media_format_t format)
{
	RECORD_LOCK();

	if (g_rc.rec_config) {
		RECORD_UNLOCK();
		return RECORD_ALREADY_INITIALIZED;
	}

	g_rc.rec_config = (struct pcm_config *)malloc(sizeof(struct pcm_config));
	if (!g_rc.rec_config) {
		RECORD_UNLOCK();
		return RECORD_ERROR_MALLOC;
	}

	g_rc.rec_config->channels = channel;
	g_rc.rec_config->rate = sample_rate;
	g_rc.rec_config->format = pcm_format;
	//TO-DO we should discussion for default value or user value and then will change.
	g_rc.rec_config->period_size = 0;
	g_rc.rec_config->period_count = 0;
	g_rc.format = format;

	RECORD_UNLOCK();

	return RECORD_OK;
}

record_result_t media_record_prepare(void)
{
	RECORD_LOCK();

	if (g_rc.pcmin) {
		RECORD_UNLOCK();
		return RECORD_ALREADY_INITIALIZED;
	}

	g_rc.pcmin = pcm_open(0, 0, PCM_IN, g_rc.rec_config);
	if (!g_rc.pcmin) {
		RECORD_UNLOCK();
		return RECORD_ERROR_DRIVER;
	}

	pcm_prepare(g_rc.pcmin);
	g_rc.buffer_size = pcm_frames_to_bytes(g_rc.pcmin, pcm_get_buffer_size(g_rc.pcmin));

	RECORD_UNLOCK();

	return RECORD_OK;
}

bool media_is_recording(void)
{
	return (g_rc.state == RECORD_RUNNING);
}

record_result_t media_stop_record(void)
{
	RECORD_LOCK();

	if (g_rc.state == RECORD_PAUSED) {
		g_rc.state = RECORD_STOPPING;
		sem_post(&g_rc.sem_pause);
	} else if (g_rc.state != RECORD_RUNNING) {
		RECORD_UNLOCK();
		return RECORD_ERROR_INVALID_STATE;
	}

	g_rc.state = RECORD_STOPPING;
	RECORD_UNLOCK();
	
	pthread_join(g_rc.pth, NULL);

	return RECORD_OK;
}

record_result_t media_pause_record(void)
{
	RECORD_LOCK();

	if (g_rc.state != RECORD_RUNNING) {
		RECORD_UNLOCK();
		return RECORD_ERROR_INVALID_STATE;
	}

	g_rc.state = RECORD_PAUSED;
	RECORD_UNLOCK();

	return RECORD_OK;
}

record_result_t media_resume_record(void)
{
	RECORD_LOCK();

	if (g_rc.state != RECORD_PAUSED) {
		return RECORD_ERROR_INVALID_STATE;
	}

	g_rc.state = RECORD_RUNNING;
	media_record_prepare();
	sem_post(&g_rc.sem_pause);

	RECORD_UNLOCK();

	return RECORD_OK;
}

record_result_t media_record_init(void)
{
	if (!b_started) {
		b_started = true;
		memset(&g_rc, 0, sizeof(g_rc));

		pthread_mutexattr_init(&g_rc.mutex_attr_record);
		pthread_mutexattr_settype(&g_rc.mutex_attr_record, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&g_rc.mutex_record, &g_rc.mutex_attr_record);

		g_rc.fd = -1;
		g_rc.state = RECORD_AVAILABLE;
		g_rc.pcmin = NULL;
		g_rc.rec_config = NULL;

		sem_init(&g_rc.sem_pause, 0, 0);
	}
	return RECORD_OK;
}

static bool write_full(void *ptr, size_t size)
{
	int ret;
	size_t written = 0;
	while (written < size) {
		ret = write(g_rc.fd, (char *)ptr + written, size - written);
		if (ret < 0) {
			return false;
		}
		written += ret;
	}
	return true;
}

int record_and_write(char *buffer)
{
	int frames;
	int size;
	char inst = ST_PROTO_RECORD_DATA;

	frames = pcm_readi(g_rc.pcmin, buffer, pcm_bytes_to_frames(g_rc.pcmin, g_rc.buffer_size));

	if (frames > 0) {
		size = pcm_frames_to_bytes(g_rc.pcmin, frames);
		if (IS_SOCKET(g_rc.fd))	{
			write_full(&inst, 1);
			write_full(&size, sizeof(int));
		}
		write_full(buffer, size);
	}
	return frames;
}

int recorder_worker(void *args)
{
	int ret;
	char *buffer = NULL;
	char inst = 0;
	buffer = (char *)malloc(g_rc.buffer_size);

	if (!buffer) {
		RECORD_LOCK();
		g_rc.state = RECORD_AVAILABLE;
		RECORD_UNLOCK();
		return RECORD_ERROR_MALLOC;
	}

	while (g_rc.thread_running) {
		RECORD_LOCK();
		switch (g_rc.state) {
		case RECORD_RUNNING:
			ret = record_and_write(buffer);
			if (ret == 0) {
				g_rc.state = RECORD_DROPPING;
			} else if (ret < 0) {
				g_rc.state = RECORD_STOPPING;
			}
			break;

		case RECORD_PAUSED: {
			RECORD_UNLOCK();
			pcm_close(g_rc.pcmin);
			g_rc.pcmin = NULL;
			if (IS_SOCKET(g_rc.fd)) {
				inst = ST_PROTO_RECORD_PAUSE;
				write_full(&inst, 1);
			}
			sem_wait(&g_rc.sem_pause);
			RECORD_LOCK();
		}
		break;

		case RECORD_STOPPING: {
			if (IS_SOCKET(g_rc.fd)) {
				inst = ST_PROTO_RECORD_STOP;
				write_full(&inst, 1);
			}
			close(g_rc.fd);
			g_rc.fd = -1;
			g_rc.thread_running = false;
		}
		break;

		case RECORD_DROPPING:
			pcm_drop(g_rc.pcmin);
		default:
			g_rc.thread_running = false;
			g_rc.state = RECORD_STOPPING;
			break;
		}
		RECORD_UNLOCK();

		// For context-switch
		sched_yield();
	}

	RECORD_LOCK();

	free(buffer);
	free(g_rc.rec_config);
	pcm_close(g_rc.pcmin);

	g_rc.state = RECORD_STOPPING;
	b_started = false;

	RECORD_UNLOCK();

	return 0;
}
