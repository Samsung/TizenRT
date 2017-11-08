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

#ifndef __MEDIAPLAYER_H__
#define __MEDIAPLAYER_H__

/****************************************************************************
* Included Files
****************************************************************************/
#include <queue.h>
#include <sys/types.h>
enum media_result_e {
	MEDIA_OK,		
	MEDIA_ERROR,
	MEDIA_ERROR_THREAD_INIT,
	MEDIA_ERROR_PLAYBACK,
	MEDIA_ERROR_RECORD,
	MEDIA_ERROR_WRITE_TO_FILE,
	MEDIA_ERROR_CREATE,
	MEDIA_ERROR_UNKNOWN_FILE
};

typedef enum media_result_e media_result_t;


enum media_type_e {
	MEDIA_TYPE_PCM,
	MEDIA_TYPE_WAV
};

typedef enum media_type_e media_type_t;

enum media_play_type_e {
	MEDIA_PLAY_TYPE_FILE,
	MEDIA_PLAY_TYPE_NETWORK,
};

typedef enum media_play_type_e media_play_type_t;

enum media_op_e {
	MEDIA_OP_RECORD,
	MEDIA_OP_PLAYBACK
};

typedef enum media_op_e media_op_t;

enum media_state_e {
	MEDIA_STATE_CREATED,
	MEDIA_STATE_PLAYING,
	MEDIA_STATE_PAUSING,
	MEDIA_STATE_PAUSED,
	MEDIA_STATE_STOPPING,
	MEDIA_STATE_RECORDING,
	MEDIA_STATE_CLOSING
};

typedef enum media_state_e media_state_t;
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
 * @cond
 * @internal
 * @brief media_init() Initialize mediaplayer.
 * @param[in] cbs A set of media callbacks
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_init(struct media_cb_s *cbs);

/**
 * @internal
 * @brief media_shutdown() Shutdown mediaplayer.
 * @since Tizen RT v1.x
 */

void media_shutdown(void);

/**
 * @internal
 * @brief media_open() Open a media from a filename or a URL.
 * @param[in] path Path can be a filename or a URL.
 * @param[in] op Media type. It can be playback mode or recording mode.
 * @param[in] play_type Media play type. It can be file or network stream.
 * @param[in] config pcm_config. Contains audio parameters.
 * @return On success, media_t structure pointer is returned.
 *         On failure, NULL pointer is returned.
 * @since Tizen RT v1.x
 */

media_t *media_open(char *path, media_op_t op, media_play_type_t play_type, struct pcm_config *config);

/**
 * @internal
 * @brief media_close() Close opened media.
 * @param[in] m media_t pointer
 * @return On success, MEDIA_OK is returned.
	   On failure, MEDIA_ERROR is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_close(media_t *m);

/**
 * @internal
 * @brief media_play() Play media.
 * @param[in] m media_t pointer
 * @param[in] loop If true, media will be repeated.
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_play(media_t *m, bool loop);

/**
 * @internal
 * @brief media_record() Start recording to the media.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_record(media_t *m);

/**
 * @internal
 * @brief media_stop() Stop media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_stop(media_t *m);

/**
 * @internal
 * @brief media_pause() Pause media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_pause(media_t *m);

/**
 * @internal
 * @brief media_resume() Resume media playing or recording.
 * @param[in] m media_t pointer
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 * @since Tizen RT v1.x
 */

media_result_t media_resume(media_t *m);

/**
 * @internal
 * @brief media_get_state() Retrieve the state of media object.
 * @param[in] m media_t pointer
 * @return The current state of media object is returned.
 * @since Tizen RT v1.x
 */

media_state_t media_get_state(media_t *m);

/**
 * @endcond
 */

#ifdef __cplusplus
}
#endif

#endif
