#include <tinyara/audio/audio.h>
#include <tinyara/audio/pcm.h>
#include <tinyalsa/tinyalsa.h>
#include <tinyara/fs/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>

#include "mediaplayer.h"

/***
 * Static variables
 */
static pthread_t g_pth_playing;
static pthread_t g_pth_recording;
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
int threadfunc_audio_playing(void *args);
int threadfunc_audio_recording(void *args);

int media_init(struct media_ops_s *ops)
{
	pthread_attr_t attr;
	pthread_mutexattr_t mutexAttr;
	struct sched_param sparam;

	// Init queue
	dq_init(&g_playing_q);
	dq_init(&g_recording_q);

	// Init semaphores
	sem_init(&g_sem_playing, 0, 0);
	sem_init(&g_sem_recording, 0, 0);

	// Init mutices
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&g_mutex_playing_q, &mutexAttr);
	pthread_mutex_init(&g_mutex_recording_q, &mutexAttr);

	// Init threads
	if (pthread_attr_init(&attr) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	sparam.sched_priority = 150;
	if (pthread_attr_setschedparam(&attr, &sparam) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_attr_setstacksize(&attr, 8192) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_create(&g_pth_playing, &attr, (pthread_startroutine_t)threadfunc_audio_playing, (void *)NULL) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	if (pthread_create(&g_pth_recording, &attr, (pthread_startroutine_t)threadfunc_audio_recording, (void *)NULL) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
	}

	return MEDIA_OK;
}

void media_shutdown(void)
{
	g_playing_live = false;
	g_recording_live = false;

	sem_post(&g_sem_playing);
	sem_post(&g_sem_recording);

	pthread_join(g_pth_playing, NULL);
	pthread_join(g_pth_recording, NULL);

	pthread_mutex_destroy(&g_mutex_playing_q);
	pthread_mutex_destroy(&g_mutex_recording_q);

	sem_destroy(&g_sem_playing);
	sem_destroy(&g_sem_recording);
}

media_t *media_open(char *path, media_op_e op, media_type_e type)
{
	media_t *m = (media_t *)malloc(sizeof(media_t));
	if (m == NULL) {
		return NULL;
	}

	m->fd = -1;
	m->op = op;
	m->state = MEDIA_STATE_CREATED;
	m->type = type;

	if (type == MEDIA_TYPE_WAV || type == MEDIA_TYPE_PCM) {
		m->payload = (char *)malloc(strlen(path) + 1);
		strcpy(m->payload, path);
	} else if (type == MEDIA_TYPE_WAV_STREAM) {
		m->payload = NULL;
	}

	return m;
}

int media_close(media_t *m)
{
	if (m->fd != -1) {
		close(m->fd);
		return MEDIA_OK;
	}

	return -MEDIA_ERROR;
}

int media_play(media_t *m, bool loop)
{
	pthread_mutex_lock(&g_mutex_playing_q);
	dq_addlast((dq_entry_t *)m, &g_playing_q);
	pthread_mutex_unlock(&g_mutex_playing_q);
	sem_post(&g_sem_playing);
	return MEDIA_OK;
}

int media_record(media_t *m)
{
	pthread_mutex_lock(&g_mutex_recording_q);
	dq_addlast((dq_entry_t *)m, &g_recording_q);
	pthread_mutex_unlock(&g_mutex_recording_q);
	sem_post(&g_sem_recording);
	return MEDIA_OK;
}

int media_stop(media_t *m)
{
	if (m->state == MEDIA_STATE_PLAYING) {
		m->state = MEDIA_STATE_STOPPING;
		return MEDIA_OK;
	} else {
		return -MEDIA_ERROR;
	}
}

int media_pause(media_t *m)
{
	if (m->state == MEDIA_STATE_PLAYING) {
		m->state = MEDIA_STATE_PAUSING;
		return MEDIA_OK;
	} else {
		return -MEDIA_ERROR;
	}
}

int media_resume(media_t *m)
{
	if (m->state == MEDIA_STATE_PAUSED) {
		m->state = MEDIA_STATE_PLAYING;
		sem_post(&g_sem_playing);
		return MEDIA_OK;
	} else {
		return -MEDIA_ERROR;
	}
}

void media_setvol(media_t *m, unsigned char vol)
{
	return;
}

bool media_isplaying(media_t *m)
{
	return (m->state == MEDIA_STATE_PLAYING);
}

unsigned char media_getvol(media_t *m)
{
	return MEDIA_OK;
}

int media_seek(media_t *m, int time_ms)
{
	return MEDIA_OK;
}

int media_setposition(media_t *m, int time_ms)
{
	return MEDIA_OK;
}

int media_getposition(media_t *m)
{
	return MEDIA_OK;
}

/***
 * Functions run on threads
 */

static int read_wav_header(media_t *m)
{
	//struct pcm_config config;
	struct wav_header_s header;
	int ret;

	ret = read(m->fd, &header, sizeof(header));
	if (ret != sizeof(header)) {
		return -MEDIA_ERROR_UNKNOWN_FILE;
	}

	// Todo: Add header information to media_t

	return MEDIA_OK;
}

int on_media_state_created(struct pcm *pcm, media_t *m)
{
	if (m->type == MEDIA_TYPE_WAV || m->type == MEDIA_TYPE_PCM) {

		if (m->op == MEDIA_OP_PLAYBACK) {
			m->fd = open(m->payload, O_RDONLY);
			if (m->fd < 0) {
				return -MEDIA_ERROR_CREATE;
			}

			if (m->type == MEDIA_TYPE_WAV) {
				read_wav_header(m);
			}
		} else if (m->op == MEDIA_OP_RECORD) {
			m->fd = open(m->payload, O_RDWR | O_CREAT | O_TRUNC);
		}

		free(m->payload);
		m->payload = NULL;

		if (m->fd < 0) {
			return -MEDIA_ERROR;
		}
	} else if (m->type == MEDIA_TYPE_WAV_STREAM) {
		// Todo: Supports network streaming
	}

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
			if (ret <= 0) {
				return -MEDIA_ERROR_PCM_WRITE;
			} else {
				remain -= ret;
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
			if (ret < 0) {
				return -MEDIA_ERROR_WRITE_TO_FILE;
			} else {
				remain -= ret;
			}
		}
	}

	return readed;
}

/***
 * Thread routine
 */
int threadfunc_audio_playing(void *args)
{
	char *buffer;
	unsigned int buffer_size;
	struct pcm *pcmout;
	struct pcm_config config;
	dq_entry_t *node;
	int ret;

	pcmout = pcm_open(0, 0, PCM_OUT, NULL);
	buffer_size = pcm_get_buffer_size(pcmout);
	buffer = (char *)malloc(buffer_size);

	// Configuration
	config.channels = 2;
	config.rate = 16000;
	config.period_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
	config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	config.format = PCM_FORMAT_S16_LE;
	config.start_threshold = 0;
	config.stop_threshold = 0;
	config.silence_threshold = 0;
	pcm_set_config(pcmout, &config);

	printf("audio_playing thread started. Buffer size: %d\n", buffer_size);

	g_playing_live = true;

	sem_wait(&g_sem_playing);
	while (g_playing_live) {
		pthread_mutex_lock(&g_mutex_playing_q);
		for (node = (dq_entry_t *)dq_peek(&g_playing_q); node && g_playing_live;) {
			switch (((media_t *)node)->state) {
			case MEDIA_STATE_CREATED:
				if (on_media_state_created(pcmout, (media_t *)node) == MEDIA_OK) {
					((media_t *)node)->state = MEDIA_STATE_PLAYING;
				} else {
					printf("Media creation error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				}
				break;

			case MEDIA_STATE_PLAYING:
				ret = on_media_state_playing(pcmout, (media_t *)node, buffer, buffer_size);
				if (ret == 0) {
					// If playing is done,
					dq_rem(node, &g_playing_q);
					sem_wait(&g_sem_playing);
					continue;
				} else if (ret < 0) {
					// Todo: Error handling
					printf("Playing error!\n");
				}
				break;

			case MEDIA_STATE_PAUSING:
				((media_t *)node)->state = MEDIA_STATE_PAUSED;
				sem_wait(&g_sem_playing);
				break;

			case MEDIA_STATE_PAUSED:
				break;

			case MEDIA_STATE_STOPPING:
				dq_rem(node, &g_playing_q);
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
	pcm_close(pcmout);

	printf("audio_playing thread terminated.\n");
	return MEDIA_OK;
}

int threadfunc_audio_recording(void *args)
{
	char *buffer;
	unsigned int buffer_size;
	struct pcm *pcmin;
	struct pcm_config config;
	dq_entry_t *node;

	pcmin = pcm_open(0, 0, PCM_IN, NULL);
	buffer_size = pcm_get_buffer_size(pcmin);
	buffer = (char *)malloc(buffer_size);

	// Configuration
	config.channels = 2;
	config.rate = 16000;
	config.period_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
	config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	config.format = PCM_FORMAT_S16_LE;
	config.start_threshold = 0;
	config.stop_threshold = 0;
	config.silence_threshold = 0;
	pcm_set_config(pcmin, &config);

	printf("audio_recording thread started. Buffer size: %d\n", buffer_size);

	g_recording_live = true;

	sem_wait(&g_sem_recording);
	while (g_recording_live) {
		pthread_mutex_lock(&g_mutex_recording_q);
		for (node = (dq_entry_t *)dq_peek(&g_recording_q); node && g_recording_live;) {
			switch (((media_t *)node)->state) {
			case MEDIA_STATE_CREATED:
				if (on_media_state_created(pcmin, (media_t *)node) == MEDIA_OK) {
					((media_t *)node)->state = MEDIA_STATE_RECORDING;
				} else {
					printf("Media creation error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				}
				break;

			case MEDIA_STATE_RECORDING:
				if (on_media_state_recording(pcmin, (media_t *)node, buffer, buffer_size) < 0) {
					// Todo: Error handling
					printf("Recording error!\n");
				}
				break;

			case MEDIA_STATE_PAUSING:
				((media_t *)node)->state = MEDIA_STATE_PAUSED;
				sem_wait(&g_sem_recording);
				break;

			case MEDIA_STATE_PAUSED:
				break;

			case MEDIA_STATE_STOPPING:
				dq_rem(node, &g_playing_q);
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
	pcm_close(pcmin);

	printf("audio_recording thread terminated.\n");
	return MEDIA_OK;
}
