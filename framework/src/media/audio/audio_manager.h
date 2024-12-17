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
#include <media/stream_info.h>

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
	AUDIO_MANAGER_DEVICE_DEAD = -14,
	AUDIO_MANAGER_DEVICE_SUSPENDED = -13,
	AUDIO_MANAGER_DEVICE_ALREADY_IN_USE = -12,
	AUDIO_MANAGER_SET_STREAM_POLICY_NOT_ALLOWED = -11,
	AUDIO_MANAGER_SET_STREAM_POLICY_FAIL = -10,
	AUDIO_MANAGER_DEVICE_NOT_SUPPORT = -9,
	AUDIO_MANAGER_RESAMPLE_FAIL = -8,
	AUDIO_MANAGER_DEVICE_FAIL = -7,
	AUDIO_MANAGER_CARD_NOT_READY = -6,
	AUDIO_MANAGER_XRUN_STATE = -5,
	AUDIO_MANAGER_INVALID_PARAM = -4,
	AUDIO_MANAGER_INVALID_DEVICE = -3,
	AUDIO_MANAGER_NO_AVAIL_CARD = -2,
	AUDIO_MANAGER_OPERATION_FAIL = -1,
	AUDIO_MANAGER_SUCCESS = 0
};

typedef enum audio_manager_result_e audio_manager_result_t;

/**
 * @brief Type of device
 */
enum audio_device_type_e {
	AUDIO_DEVICE_TYPE_SPEAKER = 0,
	AUDIO_DEVICE_TYPE_MIC = 1,
	AUDIO_DEVICE_TYPE_BT = 2,
	AUDIO_DEVICE_TYPE_AUDIO_JACK = 3
};

typedef enum audio_device_type_e audio_device_type_t;

/**
 * @brief Defined values for handling process of voice recognition on device, provides as a interface.
 */
enum audio_device_process_unit_type_e {
	AUDIO_DEVICE_PROCESS_TYPE_NONE = 0,
	AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR = 1,
};

typedef enum audio_device_process_unit_type_e device_process_type_t;

enum audio_device_process_unit_subtype_e {
	/* Nothing For Stream Out now */

	/* For Stream In */
	AUDIO_DEVICE_SPEECH_DETECT_NONE = 0,
	AUDIO_DEVICE_SPEECH_DETECT_EPD = 1,
	AUDIO_DEVICE_SPEECH_DETECT_SPD = 2,
	AUDIO_DEVICE_SPEECH_DETECT_KD = 3,
	/* For PU Control only */
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL = 4,
	/* All these value need to be aligned with value in product app */
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL0 = 5,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL1 = 6,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL2 = 7,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL3 = 8,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL4 = 9,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL5 = 10,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL6 = 11,
	AUDIO_DEVICE_SPEECH_DETECT_LOCAL7 = 12,
	AUDIO_DEVICE_SPEECH_DETECT_AEC = 13
};

typedef enum audio_device_process_unit_subtype_e device_process_subtype_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: audio_manager_init
 *
 * Description:
 *   Init audio_manager
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise a negative value.
 ****************************************************************************/
audio_manager_result_t audio_manager_init(void);

/****************************************************************************
 * Name: set_audio_stream_in
 *
 * Description:
 *   Opening the pcm for the input stream and setup the status of the active
 *   input card. If the target sample rate is out of range from the sample rates
 *   supported by the active input audio card, a resampling flag is set.
 *
 * Input parameters:
 *   channels: number of channels
 *   sample_rate: sample rate with which the stream is operated
 *   format: audio file format to be streamed in
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_in(unsigned int channels, unsigned int sample_rate, int format);

/****************************************************************************
 * Name: set_audio_stream_out
 *
 * Description:
 *   Opening the pcm for the output stream and setup the status of the active
 *   output card. If the target sample rate is out of range from the sample rates
 *   supported by the active output audio card, a resampling flag is set.
 *
 * Input parameters:
 *   channels: number of channels
 *   sample_rate: sample rate with which the stream is operated
 *   format: audio file format to be streamed out
 *   stream_id: stream info id to be used for stream out
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_out(unsigned int channels, unsigned int sample_rate, int format, stream_info_id_t stream_id);

/****************************************************************************
 * Name: start_audio_stream_in
 *
 * Description:
 *   Read the specified number of frames from the input stream.
 *   If the input audio device have been paused, resume and proceed the reading.
 *   If the resampling flag is set, resamplings are performed for all target frames.
 *
 * Input parameters:
 *   data: buffer to get the frame data
 *   frames: number of frames to be read
 *
 * Return Value:
 *   On success, the number of frames read. Otherwise, a negative value.
 ****************************************************************************/
int start_audio_stream_in(void *data, unsigned int frames);

/****************************************************************************
 * Name: start_audio_stream_out
 *
 * Description:
 *   Write the specified frame data to the output stream.
 *   If the output audio device have been paused, resume and proceed the writing.
 *   If the resampling flag is set, resamplings are performed for all target frames.
 *
 * Input parameters:
 *   data: buffer to transfer the frame data
 *   frames: number of frames to be written
 *
 * Return Value:
 *   On success, the number of frames written. Otherwise, a negative value.
 ****************************************************************************/
int start_audio_stream_out(void *data, unsigned int frames);

/****************************************************************************
 * Name: pause_audio_stream_in
 *
 * Description:
 *   Pause the active input audio device.
 *   Note that only the active audio device currently running can be paused.
 *   If the device is resumed, the paused stream is continued.
 *
 * Return Value:
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
 * Return Value:
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
 * Return Value:
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
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t stop_audio_stream_out(bool drain);

/****************************************************************************
 * Name: reset_audio_stream_in
 *
 * Description:
 *   Close the active input audio stream.
 *   After the reset, the stream should be restarted from the beginning with
 *   calling set_audio_stream_in().
 *
 * Input parameter:
 *   drain: If true Drain pcm data before stop, otherwise drop
 *
 * Return Value:
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
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t reset_audio_stream_out(stream_info_id_t stream_id);

/****************************************************************************
 * Name: get_input_frame_count
 *
 * Description:
 *   Get the frame size of the pcm buffer in the active input audio device.
 *
 * Return Value:
 *   On success, the size of the pcm buffer for input streams. Otherwise, 0.
 ****************************************************************************/
unsigned int get_input_frame_count(void);

/****************************************************************************
 * Name: get_card_input_frames_to_byte
 *
 * Description:
 *   Get the byte size of the given frame value with the channel value
 *   supported by the card for input stream.
 *
 * Input parameter:
 *   frames: the target of which byte size is returned.
 *
 * Return Value:
 *   On success, the byte size of the frame in input stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_card_input_frames_to_byte(unsigned int frames);

/****************************************************************************
 * Name: get_user_input_frames_to_byte
 *
 * Description:
 *   Get the byte size of the given frame value with the channel value
 *   specified by the user for input stream.
 *
 * Input parameter:
 *   frames: the target of which byte size is returned.
 *
 * Return Value:
 *   On success, the byte size of the frame in input stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_user_input_frames_to_byte(unsigned int frames);

/****************************************************************************
 * Name: get_user_input_bytes_to_frame
 *
 * Description:
 *   Get the number of frames for the given byte size with the channel value
 *   specified by the user for input stream.
 *
 * Input parameter:
 *   bytes: the target of which frame count is returned.
 *
 * Return Value:
 *   On success, the number of frames in input stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_user_input_bytes_to_frame(unsigned int bytes);

/****************************************************************************
 * Name: get_output_frame_count
 *
 * Description:
 *   Get the frame size of the pcm buffer in the active output audio device.
 *
 * Return Value:
 *   On success, the size of the pcm buffer for output streams. Otherwise, 0.
 ****************************************************************************/
unsigned int get_output_frame_count(void);

/****************************************************************************
 * Name: get_card_output_frames_to_byte
 *
 * Description:
 *   Get the byte size of the given frame value with the channel value
 *   supported by the card for output stream.
 *
 * Input parameter:
 *   frames: the target of which byte size is returned.
 *
 * Return Value:
 *   On success, the byte size of the frame in output stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_card_output_frames_to_byte(unsigned int frames);

/****************************************************************************
 * Name: get_user_output_frames_to_byte
 *
 * Description:
 *   Get the byte size of the given frame value with the channel value
 *   specified by the user for output stream.
 *
 * Input parameter:
 *   frames: the target of which byte size is returned.
 *
 * Return Value:
 *   On success, the byte size of the frame in output stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_user_output_frames_to_byte(unsigned int frames);

/****************************************************************************
 * Name: get_user_output_bytes_to_frame
 *
 * Description:
 *   Get the number of frames for the given byte size with the channel value
 *   specified by the user for output stream.
 *
 * Input parameter:
 *   bytes: the target of which frame count is returned.
 *
 * Return Value:
 *   On success, the number of frames in output stream. Otherwise, 0.
 ****************************************************************************/
unsigned int get_user_output_bytes_to_frame(unsigned int bytes);

/****************************************************************************
 * Name: get_output_card_buffer_size
 *
 * Description:
 *   Get byte size of record buffer of actual output card.
 *
 * Return Value:
 *   On success, the number of bytes in buffer size of output card. Otherwise, 0.
 ****************************************************************************/
unsigned int get_output_card_buffer_size(void);

/****************************************************************************
 * Name: get_input_card_buffer_size
 *
 * Description:
 *   Get byte size of record buffer of actual input card.
 *
 * Return Value:
 *   On success, the number of bytes in buffer size of input card. Otherwise, 0.
 ****************************************************************************/
unsigned int get_input_card_buffer_size(void);

/****************************************************************************
 * Name: get_max_audio_volume
 *
 * Description:
 *   Get the maximum volume of the active output audio device.
 *
 * Input parameter:
 *   volume: the pointer to get the maximum volume value
 *
 * Return Value:
 *   AUDIO_MANAGER_SUCCESS only if the maximum volume is obtained successfully
 *   from the current device. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_max_audio_volume(uint8_t *volume);

/****************************************************************************
 * Name: get_input_audio_gain
 *
 * Input parameter:
 *   gain: the pointer to get the current gain value
 *
 * Return Value:
 *   AUDIO_MANAGER_SUCCESS only if the gain is obtained successfully
 *   from the current device. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_input_audio_gain(uint8_t *gain);

/****************************************************************************
 * Name: get_output_audio_volume
 *
 * Input parameter:
 *   volume: the pointer to get the current volume value
 *
 * Return Value:
 *   AUDIO_MANAGER_SUCCESS only if the volume is obtained successfully
 *   from the current device. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_output_audio_volume(uint8_t *volume);

/****************************************************************************
 * Name: get_output_stream_volume
 *
 * Description:
 *   Get current volume level stored in active output audio card for the given stream policy.
 *
 * Input parameter:
 *   volume: the pointer to get the volume level
 *   stream_policy: policy of the stream
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_output_stream_volume(uint8_t *volume, stream_policy_t stream_policy);

/****************************************************************************
 * Name: set_input_audio_gain
 *
 * Description:
 *   Adjust the gain level of the active input audio device.
 *
 * Input parameter:
 *   gain: gain value to set, Min = 0, Max = get_max_audio_gain()
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_input_audio_gain(uint8_t gain);

/****************************************************************************
 * Name: set_output_audio_volume
 *
 * Description:
 *   Updates the volume level in json & in active output audio card volume array
 *   for the given stream policy type. If card policy matches with the given policy,
 *   it also adjusts the volume level of the active output audio device. Otherwise,
 *   adjust later when given policy stream is played.
 *
 * Input parameter:
 *   volume: volume value to set, Min = 0, Max = get_max_audio_volume()
 * 	 stream_policy: policy of the stream
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_output_audio_volume(uint8_t volume, stream_policy_t stream_policy);

/****************************************************************************
 * Name: set_output_stream_volume
 *
 * Description:
 *   Adjust the volume level of the active output audio device based on the volume level
 *   stored in json for the given stream policy type.
 *
 * Input parameter:
 *   stream_policy: policy of the stream
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_output_stream_volume(stream_policy_t stream_policy);

/****************************************************************************
 * Name: set_output_audio_equalizer
 *
 * Description:
 *   Sets the audio equalizer preset for the active output audio device.
 *
 * Input parameter:
 *   preset: The preset number to apply to the audio equalizer.
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_output_audio_equalizer(uint32_t preset);

/****************************************************************************
 * Name: set_input_audio_equalizer
 *
 * Description:
 *   Sets the audio equalizer preset for the active input audio device.
 *
 * Input parameter:
 *   preset: The preset number to apply to the audio equalizer.
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_input_audio_equalizer(uint32_t preset);

/****************************************************************************
 * Name: find_stream_in_device_with_process_type
 *
 * Description:
 *   Find stream in device that supports specific process on dsp
 *
 * Input parameter:
 *   type : supported type of processing, subtype : supported sub type of processing
 *   card_id : founded card id to be returned, device_id : founded device id to be returned
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t find_stream_in_device_with_process_type(device_process_type_t type, device_process_subtype_t subtype, int *card_id, int *device_id);

/****************************************************************************
 * Name: register_stream_in_device_process_handler
 *
 * Description:
 *   Regsiter process handler to handle event messages from the target stream of the device
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *   type : Process Type
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t register_stream_in_device_process_handler(int card_id, int device_id, device_process_type_t type);

/****************************************************************************
 * Name: register_stream_in_device_process_type
 *
 * Description:
 *   Regsiter processtype based on given type & subtype of process
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *   type : Process Type, subtype : Process Subtype
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t register_stream_in_device_process_type(int card_id, int device_id, device_process_type_t type, device_process_subtype_t subtype);

/****************************************************************************
 * Name: start_stream_in_device_process
 *
 * Description:
 *   Stop registered process
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t start_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype);

/****************************************************************************
 * Name: stop_stream_in_device_process
 *
 * Description:
 *   Stop registered process
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t stop_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype);

/****************************************************************************
 * Name: unregister_stream_in_device_process
 *
 * Description:
 *   Release device process and unregister it
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t unregister_stream_in_device_process(int card_id, int device_id);

/****************************************************************************
 * Name: get_device_process_handler_message
 *
 * Description:
 *   Get message from registered process handler
 *
 * Input parameter:
 *   card_id : Target card id , device_id : Target device id
 *   msgId : Message ID to be returned
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_device_process_handler_message(int card_id, int device_id, uint16_t *msgId);

/****************************************************************************
 * Name: set_stream_in_policy
 *
 * Description:
 *   Set policy to prevent the current stream in from being stopped by another operation
 *   when the current operation is more important.
 *   The policy follows priority based on stream_policy_t
 *
 * Input parameter:
 *   policy : policy to set as a current stream's policy
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_stream_in_policy(stream_policy_t policy);

/****************************************************************************
 * Name: set_stream_out_policy
 *
 * Description:
 *   Set policy to prevent the current stream out from being stopped by another operation
 *   when the current operation is more important.
 *   The policy follows priority based on stream_policy_t
 *
 * Input parameter:
 *   policy : policy to set as a current stream's policy
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_stream_out_policy(stream_policy_t policy);

/****************************************************************************
 * Name: get_stream_in_policy
 *
 * Description:
 *   Get policy of actual stream in
 *
 * Input parameter:
 *   policy : current policy to be returned
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_stream_in_policy(stream_policy_t *policy);

/****************************************************************************
 * Name: get_stream_out_policy
 *
 * Description:
 *   Get policy of actual stream out
 *
 * Input parameter:
 *   policy : current policy to be returned
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_stream_out_policy(stream_policy_t *policy);

/****************************************************************************
 * Name: change_stream_in_device
 *
 * Description:
 *   Change actual stream in device with given values
 *
 * Input parameter:
 *   card_id : id of sound card, device_id : id of device
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t change_stream_in_device(int card_id, int device_id);

/****************************************************************************
 * Name: change_stream_out_device
 *
 * Description:
 *   Change actual stream out device with given values
 *
 * Input parameter:
 *   card_id : id of sound card, device_id : id of device
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t change_stream_out_device(int card_id, int device_id);

/****************************************************************************
 * Name: get_stream_in_id
 *
 * Description:
 *   Get card & device id of actual stream in device
 *
 * Input parameter:
 *   card_id : id of sound card to be revealed , device_id : id of device to be revealed
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_stream_in_id(int *card_id, int *device_id);

/****************************************************************************
 * Name: get_stream_out_id
 *
 * Description:
 *   Get card & device id of actual stream out device
 *
 * Input parameter:
 *   card_id : id of sound card to be revealed , device_id : id of device to be revealed
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_stream_out_id(int *card_id, int *device_id);

/****************************************************************************
 * Name: get_keyword_buffer_size
 *
 * Description:
 *   Get the buffer size required to store keyword audio data
 *
 * Input parameter:
 *   keywordBufferSize : output parameter to get buffer size 
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_keyword_buffer_size(uint32_t *keywordBufferSize);

/****************************************************************************
 * Name: get_keyword_data
 *
 * Description:
 *   Stores the keyword data in the passed parameter.
 *
 * Input parameter:
 *   buffer : output parameter to get keyword data
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_keyword_data(uint8_t *buffer);

/****************************************************************************
 * Name: set_mic_mute
 *
 * Description:
 *   Sets the microphone to mute state. Recorder client(if any) will get notified through onRecordStoppped callback.
 *
 * Return Value:
 *   On successfully performing mic mute operation, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_mic_mute(void);

/****************************************************************************
 * Name: set_mic_unmute
 *
 * Description:
 *   Unmutes the microphone.
 *
 * Return Value:
 *   On successfully performing mic unmute operation, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_mic_unmute(void);

/****************************************************************************
 * Name: set_audio_stream_mute
 *
 * Description:
 *   Sets the mute state for given stream policy. Valid only for STREAM_TYPE_NOTIFY
 *   and STREAM_TYPE_VOICE_RECORD. For other stream policies, it will return error.
 *   First, it updates the mute state of given stream policy in json and then sets
 *   the mute state in active i/o audio device.
 *   In case of output stream, if card policy doesnot matches with the given stream
 *   policy, mute state will be set later in the active output audio device when given
 *   policy stream is played.
 *
 * Input parameter:
 * 	 stream_policy: policy of the stream
 *   mute: mute state to be set
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_mute(stream_policy_t stream_policy, bool mute);

/****************************************************************************
 * Name: set_audio_stream_mute_from_json
 *
 * Description:
 *   It fetches the mute state of given stream policy from json and then sets it in
 *   active i/o audio device.
 *
 * Input parameter:
 * 	 stream_policy: policy of the stream
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t set_audio_stream_mute_from_json(stream_policy_t stream_policy);

/****************************************************************************
 * Name: get_audio_stream_mute_state
 *
 * Description:
 *   It gets the mute state of given stream policy stored in json.
 *
 * Input parameter:
 * 	 stream_policy: policy of the stream
 *   mute: pointer to store the mute state of given stream policy
 *
 * Return Value:
 *   On success, AUDIO_MANAGER_SUCCESS. Otherwise, a negative value.
 ****************************************************************************/
audio_manager_result_t get_audio_stream_mute_state(stream_policy_t stream_policy, bool *mute);

#ifdef CONFIG_DEBUG_MEDIA_INFO
/****************************************************************************
 * Name: dump_audio_card_info
 *
 * Description:
 *   Print out information of all stream out devices . It will be properly executed only when media debug is enabled.
 *
 * Input parameter:
 *   None
 *
 * Return Value:
 *   None
 ****************************************************************************/
void dump_audio_card_info(void);
#endif

#if defined(__cplusplus)
}								/* extern "C" */
#endif
#endif
