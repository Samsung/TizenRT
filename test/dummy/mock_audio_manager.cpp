/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <stdbool.h>
#include "audio/audio_manager.h"
#include <stdio.h>
#include <string.h>

enum audio_io_direction_e {
	INPUT = 0,
	OUTPUT = 1
};

typedef enum audio_io_direction_e audio_io_direction_t;

int card_number = 0;
int device_number = 0;

bool SetAudioStreamInRetrunFail = false;
bool ResetAudioStreamInRetrunFail = false;
bool GetInputAudioGainReturnFail = false;
bool GetMaxAudioVolumeReturnFail = false;
bool StopAudioStreaInReturnFail = false;
bool PauseAudioStreamInReturnFail = false;
int GetUserInputFramesToByteRetrun = 64;
int SetInputAudioGainReturn = 0;
int StartAudioStreamInReturn = 64;
int GetInputFrameCountReturn = 64;
bool GetOutputStreamVolumeReturnFail = false;
bool SetOutputAudioVolumeReturnFail_NOT_SUPPORT = false;
bool SetOutputAudioVolumeReturnFail_INVALID_PARAM = false;
bool SetOutputAudioEqualizerReturnFail = false;
bool SetAudioStreamMuteReturnFail = false;
bool GetAudioStreamMuteStateReturnFail = false;
bool setDmicReturnFail = false;
bool SetStreamPolicyReturnFail = false;
bool SetAudioStreamOutReturnFail = false;
int GetOutputCardBufferSizeReturn = 64;
bool ResetAudioStreamOutReturnFail = false;
bool SetOutputStreamVolumeReturnFail = false;
bool SetAudioStreamMuteFromJsonReturnFail = false;
bool StopAudioStreamOutReturnFail = false;
bool PauseAudioStreamOutReturnFail = false;
int StartAudioStreamOutReturn = 0;
bool RegisterStreamInDeviceProcessTypeKDReturnFail = false;
bool RegisterStreamInDeviceProcessTypeLocalReturnFail = false;
bool RegisterStreamInDeviceProcessHandlerReturnFail = false;
bool UnregisterStreamInDeviceProcessReturnFail = false;
bool ChangeStreamInDeviceReturnFail = false;
bool StartStreamInDeviceProcessTypeReturnFail = false;
bool StopStreamInDeviceProcessTypeReturnFail = false;
bool GetKeywordBufferSizeReturnFail = false;
bool GetKeywordDataReturnFail = false;
bool SetKeywordModelReturnFail = false;
bool GetDeviceProcessHandlerMessageReturnFail = false;
bool ChangeInputDspFlowReturnFail = false;
audio_device_process_unit_subtype_e AUDIO_DEVICE_SPEECH_DETECT = AUDIO_DEVICE_SPEECH_DETECT_KD;
uint32_t KeyWordBufferSize = 4;

AudioEventListener mRecorderMuteCallback = nullptr;

void registerRecorderMuteListener(AudioEventListener listener)
{
	mRecorderMuteCallback = listener;
}

void unregisterRecorderMuteListener()
{
	mRecorderMuteCallback = nullptr;
}

audio_manager_result_t audio_manager_init(void)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_in(unsigned int channels, unsigned int sample_rate, int format)
{
	if (SetAudioStreamInRetrunFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_out(unsigned int channels, unsigned int sample_rate, int format, stream_info_id_t stream_id)
{
	if (SetAudioStreamOutReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

int start_audio_stream_in(void *data, unsigned int frames)
{
	return StartAudioStreamInReturn;
}

int start_audio_stream_out(void *data, unsigned int frames, uint8_t playback_idx, stream_info_id_t stream_id)
{
	return StartAudioStreamOutReturn;
}

audio_manager_result_t pause_audio_stream_in(void)
{
	if (PauseAudioStreamInReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t pause_audio_stream_out(stream_info_id_t stream_id)
{
	if (PauseAudioStreamOutReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_in(void)
{
	if (StopAudioStreaInReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_audio_stream_out(stream_info_id_t stream_id, bool drain)
{
	if(StopAudioStreamOutReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_in(void)
{
	if (ResetAudioStreamInRetrunFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t reset_audio_stream_out(stream_info_id_t stream_id)
{
	if (ResetAudioStreamOutReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

unsigned int get_input_frame_count(void)
{
	return 64;
}

unsigned int get_card_input_frames_to_byte(unsigned int frames)
{
	return 64;
}

unsigned int get_card_input_bytes_to_frame(unsigned int bytes)
{
	return 64;
}

unsigned int get_user_input_frames_to_byte(unsigned int frames)
{
	return GetUserInputFramesToByteRetrun;
}

unsigned int get_user_input_bytes_to_frame(unsigned int bytes)
{
	return 64;
}

unsigned int get_output_frame_count(stream_info_id_t stream_id)
{
	return 64;
}

unsigned int get_card_output_frames_to_byte(unsigned int frames)
{
	return 64;
}

unsigned int get_card_output_bytes_to_frame(unsigned int bytes)
{
	return 64;
}

unsigned int get_user_output_frames_to_byte(unsigned int frames, stream_info_id_t stream_id)
{
	return 64;
}

unsigned int get_user_output_bytes_to_frame(unsigned int bytes, stream_info_id_t stream_id)
{
	return 64;
}

float get_output_sample_rate_ratio(stream_info_id_t stream_id)
{
	return 1.0f;
}

unsigned int get_input_card_buffer_size(void)
{
	return 64;
}

unsigned int get_output_card_buffer_size(void)
{
	return GetOutputCardBufferSizeReturn;
}

audio_manager_result_t get_max_audio_volume(uint8_t *volume)
{
	if (GetMaxAudioVolumeReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	*volume = 10;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_input_audio_gain(uint8_t *gain)
{
	if (GetInputAudioGainReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	*gain = 5;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_output_audio_volume(uint8_t *volume)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_input_audio_gain(uint8_t gain)
{
	if (SetInputAudioGainReturn != 0) {
		return static_cast<audio_manager_result_t>(SetInputAudioGainReturn);
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_output_audio_volume(uint8_t volume, stream_policy_t stream_policy)
{
	if (SetOutputAudioVolumeReturnFail_INVALID_PARAM) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	if (SetOutputAudioVolumeReturnFail_NOT_SUPPORT) {
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t find_stream_in_device_with_process_type(device_process_type_t type, device_process_subtype_t subtype, int *card_id, int *device_id)
{
	*card_id = card_number;
	*device_id = device_number;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t request_stream_in_device_process_type(int card_id, int device_id, int cmd, device_process_subtype_t subtype)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t start_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype)
{
	if (StartStreamInDeviceProcessTypeReturnFail) {
		return AUDIO_MANAGER_CARD_NOT_READY;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t stop_stream_in_device_process_type(int card_id, int device_id, device_process_subtype_t subtype)
{
	if (StopStreamInDeviceProcessTypeReturnFail) {
		return AUDIO_MANAGER_CARD_NOT_READY;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t register_stream_in_device_process_type(int card_id, int device_id, device_process_type_t type, device_process_subtype_t subtype)
{
	if (subtype == AUDIO_DEVICE_SPEECH_DETECT_KD && RegisterStreamInDeviceProcessTypeKDReturnFail) {
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	if (subtype == AUDIO_DEVICE_SPEECH_DETECT_LOCAL && RegisterStreamInDeviceProcessTypeLocalReturnFail) {
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t register_stream_in_device_process_handler(int card_id, int device_id, device_process_type_t type)
{
	if (RegisterStreamInDeviceProcessHandlerReturnFail) {
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t unregister_stream_in_device_process(int card_id, int device_id)
{
	if (UnregisterStreamInDeviceProcessReturnFail) {
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_device_process_handler_message(int card_id, int device_id, uint16_t *msgId)
{
	if (GetDeviceProcessHandlerMessageReturnFail) {
		return AUDIO_MANAGER_INVALID_DEVICE;
	}
	*msgId = AUDIO_DEVICE_SPEECH_DETECT;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t change_stream_device(int card_id, int device_id, audio_io_direction_t direct)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t change_stream_in_device(int card_id, int device_id)
{
	if (card_id != card_number || device_id != device_number) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}

	if (ChangeStreamInDeviceReturnFail) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t change_stream_out_device(int card_id, int device_id)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_stream_policy(stream_policy_t policy, audio_io_direction_t direct)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_stream_policy(stream_policy_t *policy, audio_io_direction_t direct)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_stream_in_policy(stream_policy_t policy)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_stream_out_policy(stream_policy_t policy, stream_info_id_t stream_id)
{
	if (SetStreamPolicyReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_stream_in_policy(stream_policy_t *policy)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_stream_out_policy(stream_policy_t *policy)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_keyword_model(uint8_t model)
{
	if (SetKeywordModelReturnFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_keyword_data(uint8_t *buffer)
{
	if (GetKeywordDataReturnFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_keyword_buffer_size(uint32_t *keywordBufferSize)
{
	if (GetKeywordBufferSizeReturnFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	*keywordBufferSize = KeyWordBufferSize;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_output_stream_volume(stream_policy_t stream_policy)
{
	if (SetOutputStreamVolumeReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_output_stream_volume(uint8_t *volume, stream_policy_t stream_policy)
{
	if (GetOutputStreamVolumeReturnFail) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_dmic(bool enable)
{
	if (setDmicReturnFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_output_audio_equalizer(uint32_t preset)
{
	if (SetOutputAudioEqualizerReturnFail) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_mute(stream_policy_t stream_policy, bool mute)
{
	if (SetAudioStreamMuteReturnFail) {
		return AUDIO_MANAGER_DEVICE_NOT_SUPPORT;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_audio_stream_mute_state(stream_policy_t stream_policy, bool *mute)
{
	if (GetAudioStreamMuteStateReturnFail) {
		return AUDIO_MANAGER_INVALID_PARAM;
	}
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_input_audio_equalizer(uint32_t preset)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_audio_stream_mute_from_json(stream_policy_t stream_policy)
{
	if (SetAudioStreamMuteFromJsonReturnFail) return AUDIO_MANAGER_OPERATION_FAIL;
	return AUDIO_MANAGER_SUCCESS;
}

std::chrono::milliseconds get_output_read_timeout(void)
{
	return std::chrono::milliseconds(0);
}

audio_manager_result_t set_output_audio_mixer(stream_info_id_t stream_id)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t set_kd_sensitivity(uint16_t sensitivity)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t get_kd_sensitivity(uint16_t *sensitivity)
{
	return AUDIO_MANAGER_SUCCESS;
}

audio_manager_result_t change_input_dsp_flow(uint8_t dsp_flow_num)
{
	if (ChangeInputDspFlowReturnFail) {
		return AUDIO_MANAGER_OPERATION_FAIL;
	}
	return AUDIO_MANAGER_SUCCESS;
}
