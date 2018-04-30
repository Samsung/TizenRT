/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

/**
 * @file audio_manager.h
 * @brief All macros, structures and functions to manager audio devices.
 */

#ifndef __AUDIO_MANAGER_H
#define __AUDIO_MANAGER_H

#include <sys/time.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/
/**
 * @brief Result types of Audio Manager APIs such as FAIL, SUCCESS, or INVALID ARGS
 */
enum audio_manager_result_e {
	AUDIO_MANAGER_RESAMPLE_FAIL = -9,
	AUDIO_MANAGER_DEVICE_FAIL = -8,
	AUDIO_MANAGER_CARD_NOT_READY = -7,
	AUDIO_MANAGER_CARD_NOT_FOUND = -6,
	AUDIO_MANAGER_XRUN_STATE = -5,
	AUDIO_MANAGER_INVALID_PARAM = -4,
	AUDIO_MANAGER_INVALID_DEVICE_NAME = -3,
	AUDIO_MANAGER_NO_AVAIL_CARD = -2,
	AUDIO_MANAGER_FAIL = -1,
	AUDIO_MANAGER_SUCCESS = 0
};

typedef enum audio_manager_result_e audio_manager_result_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: set_input_audio_volume
 *
 * Description:
 *   Adjust the volume level of the active input audio device.
 *
 * Input parameters:
 *   volume:   Volume level, 0(Min) ~ 10(Max)
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_input_audio_volume(uint8_t volume);

/****************************************************************************
 * Name: set_output_audio_volume
 *
 * Description:
 *   Adjust the volume level of the active output audio device.
 *
 * Input parameters:
 *   volume:   Volume level, 0(Min) ~ 10(Max)
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_output_audio_volume(uint8_t volume);

/****************************************************************************
 * Name: get_input_audio_volume
 *
 * Description:
 *   Get the current volume level of the active input audio device.
 *
 * Returned Value:
 *   On success, the current input volume level. Otherwise, a negative value.
 ****************************************************************************/
int get_input_audio_volume(void);

/****************************************************************************
 * Name: get_output_audio_volume
 *
 * Description:
 *   Get the current volume level of the active output audio device.
 *
 * Returned Value:
 *   On success, the current output volume level. Otherwise, a negative value.
 ****************************************************************************/
int get_output_audio_volume(void);

/****************************************************************************
 * Name: get_audio_volume
 *
 * Description:
 *   Get the maximum volume level of an audio device.
 *
 * Returned Value: The maximum volume level.
 ****************************************************************************/
uint16_t get_max_audio_volume(void);

/****************************************************************************
 * Name: set_audio_stream_in
 *
 * Description:
 *   Find an available audio card for input stream and setup the configuration
 *   parameters and status of the card together with opening the pcm for the
 *   input stream.
 *
 * Input parameters:
 *   channels: number of channels
 *   sample_rate: sample rate with which the stream is operated
 *   format: audio file format to be streamed in
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_in(uint8_t channels, uint32_t sample_rate, uint8_t format);

/****************************************************************************
 * Name: set_audio_stream_out
 *
 * Description:
 *   Find an available audio card for output stream and setup the configuration
 *   parameters and status of the card together with opening the pcm for the
 *   output stream.
 *
 * Input parameters:
 *   channels: number of channels
 *   sample_rate: sample rate with which the stream is operated
 *   format: audio file format to be streamed out
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_out(uint8_t channels, uint32_t sample_rate, uint8_t format);

int get_input_frame_count(void);
int get_input_frames_byte_size(uint32_t frames);
int get_input_bytes_frame_count(uint32_t bytes);
int get_output_frame_count(void);
int get_output_frames_byte_size(uint32_t frames);
int get_output_bytes_frame_count(uint32_t bytes);

/****************************************************************************
 * Name: start_audio_stream_in
 *
 * Description:
 *   Read the specified number of frames from the input stream.
 *   If the input audio device have been paused, resume and proceed the reading.
 *
 * Input parameters:
 *   data: buffer to get the frame data
 *   frames: number of frames to be read
 *
 * Returned Value:
 *   On success, the number of frames read. Otherwise, a negative value.
 ****************************************************************************/
int start_audio_stream_in(void *data, uint32_t frames);

/****************************************************************************
 * Name: start_audio_stream_out
 *
 * Description:
 *   Write the specified frame data to the output stream.
 *   If the output audio device have been paused, resume and proceed the writing.
 *
 * Input parameters:
 *   data: buffer to transfer the frame data
 *   frames: number of frames to be written
 *
 * Returned Value:
 *   On success, the number of frames written. Otherwise, a negative value.
 ****************************************************************************/
int start_audio_stream_out(void *data, uint32_t frames);

/****************************************************************************
 * Name: pause_audio_stream_in
 *
 * Description:
 *   Pause the active input audio device.
 *   Note that only the active audio device currently running can be paused.
 *   If the device is resumed, the paused stream is continued.
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t pause_audio_stream_in(void);

/****************************************************************************
 * Name: pause_audio_stream_out
 *
 * Description:
 *   Pause the active output audio device.
 *	 Note that only the active audio device currently running can be paused.
 *	 If the device is resumed, the paused stream is continued.
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t pause_audio_stream_out(void);

/****************************************************************************
 * Name: stop_audio_stream_in
 *
 * Description:
 *   Stop the active input audio device.
 *   Note that only the active audio device currently running can be stopped.
 *	 Once the device is stopped, the stream should be restarted from the beginning
 *	 with calling set_audio_stream_in().
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t stop_audio_stream_in(void);

/****************************************************************************
 * Name: stop_audio_stream_out
 *
 * Description:
 *   Stop the active output audio device.
 *   Note that only the active audio device currently running can be stopped.
 *	 Once the device is stopped, the stream should be restarted from the beginning
 *	 with calling set_audio_stream_out().
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t stop_audio_stream_out(void);

/****************************************************************************
 * Name: reset_audio_stream_in
 *
 * Description:
 *   Close the active input audio stream.
 *   After the reset, the stream should be restarted from the beginning with
 *   calling set_audio_stream_in().
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t reset_audio_stream_in(void);

/****************************************************************************
 * Name: reset_audio_stream_out
 *
 * Description:
 *   Close the active output audio stream.
 *   After the reset, the stream should be restarted from the beginning with
 *   calling set_audio_stream_out().
 *
 * Returned Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t reset_audio_stream_out(void);

#if defined(__cplusplus)
}								/* extern "C" */
#endif
#endif
