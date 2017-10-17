#include <tinyara/config.h>
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
 * Structure
 */
struct media_s {
	dq_entry_t q;

	int fd;
	char *payload;
	uint32_t size;
	media_op_e op;
	media_state_e state;
	media_type_e type;
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
int threadfunc_audio_playing(void *args);
int threadfunc_audio_recording(void *args);

int media_init(struct media_cb_s *cbs)
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
#ifdef CONFIG_AUDIO_MULTI_CARD
	if (pthread_create(&g_pth_recording, &attr, (pthread_startroutine_t)threadfunc_audio_recording, (void *)NULL) != 0) {
		return -MEDIA_ERROR_THREAD_INIT;
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

int media_stop(media_t *m)
{
	if (m->state == MEDIA_STATE_PLAYING || m->state == MEDIA_STATE_RECORDING) {
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
	m->payload = NULL;

	if (type == MEDIA_TYPE_WAV || type == MEDIA_TYPE_PCM) {
		if (op == MEDIA_OP_PLAYBACK) {
			m->fd = open(path, O_RDONLY);
			if (m->fd < 0) {
				return NULL;
			}

			if (m->type == MEDIA_TYPE_WAV) {
				read_wav_header(m);
			}
			m->state = MEDIA_STATE_PLAYING;

		} else if (op == MEDIA_OP_RECORD) {
			m->fd = open(path, O_RDWR | O_CREAT | O_TRUNC);
			if (m->fd < 0) {
				return NULL;
			}
			m->state = MEDIA_STATE_RECORDING;
		}
	} else if (type == MEDIA_TYPE_WAV_STREAM) {
		// Todo: Supports network streaming
		return NULL;
	}

	return m;
}

void media_close(media_t *m)
{
	if (m == NULL) {
		return;
	}
	if (m->fd >= 0) {
		close(m->fd);
	}
	free(m);
	return;
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
#ifndef CONFIG_AUDIO_MULTI_CARD
	struct pcm *pcmin;
	int playing = 0;
	int recording = 0;
	buffer = NULL;
	buffer_size = 0;
	pcmout = NULL;
	pcmin = NULL;
#endif
	// Configuration
	config.channels = 2;
	config.rate = 16000;
	config.period_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
	config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	config.format = PCM_FORMAT_S16_LE;
	config.start_threshold = 0;
	config.stop_threshold = 0;
	config.silence_threshold = 0;

#ifdef CONFIG_AUDIO_MULTI_CARD
	pcmout = pcm_open(0, 0, PCM_OUT, &config);
	buffer_size = pcm_get_buffer_size(pcmout);
	buffer = (char *)malloc(buffer_size);
#endif

#ifdef CONFIG_AUDIO_MULTI_CARD
	printf("audio_playing thread started. Buffer size: %d\n", buffer_size);
#else
	printf("audio thread started.\n");
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
					pcmout = pcm_open(0, 0, PCM_OUT, &config);
					buffer_size = pcm_get_buffer_size(pcmout);
					buffer = (char *)malloc(buffer_size);
					playing = 1;
				}

#endif
				ret = on_media_state_playing(pcmout, (media_t *)node, buffer, buffer_size);
				if (ret == 0) {
					// If playing is done,
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				} else if (ret < 0) {
					// Todo: Error handling
					printf("Playing error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
				}
				break;
#ifndef CONFIG_AUDIO_MULTI_CARD
			case MEDIA_STATE_RECORDING:
				if (playing) {
					node = dq_next(node);
					continue;
				}
				if (!recording) {
					pcmin = pcm_open(0, 0, PCM_IN, &config);
					buffer_size = pcm_get_buffer_size(pcmin);
					buffer = (char *)malloc(buffer_size);
					recording = 1;
				}
				if (on_media_state_recording(pcmin, (media_t *)node, buffer, buffer_size) < 0) {
					// Todo: Error handling
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
					printf("Recording error!\n");
				}

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
					pcm_close(pcmout);
					pcmout = NULL;
					playing = 0;
				}
				if (recording) {
					pcm_close(pcmin);
					pcmin = NULL;
					recording = 0;
				}
				if (buffer != NULL) {
					free(buffer);
				}
#endif
				dq_rem(node, &g_playing_q);
				sem_wait(&g_sem_playing);
				break;
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
	pcmout = NULL;
#ifndef CONFIG_AUDIO_MULTI_CARD
	pcm_close(pcmin);
	pcmin = NULL;
#endif

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

	// Configuration
	config.channels = 2;
	config.rate = 16000;
	config.period_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
	config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	config.format = PCM_FORMAT_S16_LE;
	config.start_threshold = 0;
	config.stop_threshold = 0;
	config.silence_threshold = 0;

	pcmin = pcm_open(0, 0, PCM_IN, &config);
	buffer_size = pcm_get_buffer_size(pcmin);
	buffer = (char *)malloc(buffer_size);


	printf("audio_recording thread started. Buffer size: %d\n", buffer_size);

	g_recording_live = true;

	sem_wait(&g_sem_recording);
	while (g_recording_live) {
		pthread_mutex_lock(&g_mutex_recording_q);
		for (node = (dq_entry_t *)dq_peek(&g_recording_q); node && g_recording_live;) {
			switch (((media_t *)node)->state) {
			case MEDIA_STATE_RECORDING:
				if (on_media_state_recording(pcmin, (media_t *)node, buffer, buffer_size) < 0) {
					// Todo: Error handling
					printf("Recording error!\n");
					((media_t *)node)->state = MEDIA_STATE_STOPPING;
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
				break;
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
	pcmin = NULL;
	printf("audio_recording thread terminated.\n");
	return MEDIA_OK;
}
