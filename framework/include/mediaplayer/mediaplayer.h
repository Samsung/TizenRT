#ifndef __MEDIAPLAYER_H__
#define __MEDIAPLAYER_H__

#include <queue.h>
#include <sys/types.h>

/***
 * Errors
 */
#define MEDIA_OK					0
#define MEDIA_ERROR					1
#define MEDIA_ERROR_THREAD_INIT		2
#define MEDIA_ERROR_PCM_WRITE		3
#define MEDIA_ERROR_WRITE_TO_FILE	4
#define MEDIA_ERROR_CREATE			5
#define MEDIA_ERROR_UNKNOWN_FILE	6

/***
 * Enumerations
 */
typedef enum {
	MEDIA_TYPE_PCM,
	MEDIA_TYPE_PCM_STREAM,
	MEDIA_TYPE_WAV,
	MEDIA_TYPE_WAV_STREAM
} media_type_e;

typedef enum {
	MEDIA_OP_RECORD,
	MEDIA_OP_PLAYBACK
} media_op_e;

typedef enum {
	MEDIA_STATE_CREATED,
	MEDIA_STATE_PLAYING,
	MEDIA_STATE_PAUSING,
	MEDIA_STATE_PAUSED,
	MEDIA_STATE_STOPPING,
	MEDIA_STATE_RECORDING,
	MEDIA_STATE_CLOSING
} media_state_e;

/***
 * Structures
 */
struct media_s;
typedef struct media_s media_t;

struct media_cb_s {
	void (*on_finished)(media_t *m);
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @brief media_init() Initialize mediaplayer.
 * @param[in] cbs A set of media callbacks
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_init(struct media_cb_s *cbs);

/**
 * @brief media_shutdown() Shutdown mediaplayer.
 * @since Tizen RT v2.0
 */

void media_shutdown(void);

/**
 * @brief media_open() Open a media from a filename or a URL.
 * @param[in] path Path can be a filename or a URL.
 * @param[in] op Media type. It can be playback mode or recording mode.
 * @param[in] type Media format. It can be WAV or something else.
 * @return On success, media_t structure pointer is returned.
 *         On failure, NULL pointer is returned.
 * @since Tizen RT v2.0
 */

media_t *media_open(char *path, media_op_e op, media_type_e type);

/**
 * @brief media_close() Close opened media.
 * @param[in] m media_t pointer
 * @since Tizen RT v2.0
 */

void media_close(media_t *m);

/**
 * @brief media_play() Play media.
 * @param[in] m media_t pointer
 * @param[in] loop If true, media will be repeated.
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_play(media_t *m, bool loop);

/**
 * @brief media_record() Start recording to the media.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_record(media_t *m);

/**
 * @brief media_stop() Stop media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_stop(media_t *m);

/**
 * @brief media_pause() Pause media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_pause(media_t *m);

/**
 * @brief media_resume() Resume media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_resume(media_t *m);

/**
 * @brief media_setvol() Set media playing or recording volume.
 * @param[in] m media_t pointer
 * @param[in] vol volume amount(0 ~ 255)
 * @since Tizen RT v2.0
 */

void media_setvol(media_t *m, unsigned char vol);

/**
 * @brief media_isplaying() Check the media is playing currently.
 * @param[in] m media_t pointer
 * @return If media is playing, true is returned.
 *         If not, false is returned.
 * @since Tizen RT v2.0
 */

bool media_isplaying(media_t *m);

/**
 * @brief media_getvol() Return current media volume.
 * @param[in] m media_t pointer
 * @return Volume amount(0 ~ 255)
 * @since Tizen RT v2.0
 */

unsigned char media_getvol(media_t *m);

/**
 * @brief media_seek() Seeking media playing position. This function is for only playing.
 * @param[in] m media_t pointer
 * @param[in] time_ms amount to move. Negative value means backward, positive means forward.
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_seek(media_t *m, int time_ms);

/**
 * @brief media_setposition() Set media playing position. This function is only for playing.
 * @param[in] m media_t pointer
 * @param[in] time_ms Time(ms) from start
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v2.0
 */

int media_setposition(media_t *m, int time_ms);

/**
 * @brief media_getposition() Get current playing position
 * @param[in] m media_t pointer
 * @return Media playing position
 * @since Tizen RT v2.0
 */

int media_getposition(media_t *m);

#ifdef __cplusplus
}
#endif

#endif
