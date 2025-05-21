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

#include <audio/SoundManager.h>
#include <audio_manager.h>
#include <debug.h>

bool getVolume(uint8_t *volume, stream_info_t *stream_info)
{
	medvdbg("SoundManager : getVolume. stream_policy: %d\n", stream_info->policy);
	audio_manager_result_t res = get_output_stream_volume(volume, stream_info->policy);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_stream_volume failed stream_policy : %d, ret : %d\n", stream_info->policy, res);
		return false;
	}
	return true;
}

bool setVolume(uint8_t volume, stream_info_t *stream_info)
{
	medvdbg("SoundManager : setVolume. volume: %d, stream_policy: %d\n", volume, stream_info->policy);
	audio_manager_result_t res = set_output_audio_volume(volume, stream_info->policy);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_output_audio_volume failed volume : %d, stream_policy: %d, ret : %d\n", volume, stream_info->policy, res);
		return false;
	}
	return true;
}

bool setEqualizer(uint32_t preset)
{
	medvdbg("SoundManager : setEqulizer. preset: %d\n", preset);
	audio_manager_result_t res = set_output_audio_equalizer(preset);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_output_audio_equalizer failed preset : %d, ret : %d\n", preset, res);
		return false;
	}
	return true;
}

bool setMicMute(void)
{
	medvdbg("SoundManager : setMicMute\n");
	audio_manager_result_t res = set_audio_stream_mute(STREAM_TYPE_VOICE_RECORD, true);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, mute : %d, ret : %d\n", STREAM_TYPE_VOICE_RECORD, true, res);
		return false;
	}
	return true;
}

bool setMicUnmute(void)
{
	medvdbg("SoundManager : setMicUnmute\n");
	audio_manager_result_t res = set_audio_stream_mute(STREAM_TYPE_VOICE_RECORD, false);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, mute : %d, ret : %d\n", STREAM_TYPE_VOICE_RECORD, false, res);
		return false;
	}
	return true;
}

bool setStreamMute(stream_policy_t stream_policy, bool mute)
{
	medvdbg("SoundManager : setStreamMute. stream_policy: %d, mute: %d\n", stream_policy, mute);
	audio_manager_result_t res = set_audio_stream_mute(stream_policy, mute);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, mute : %d, ret : %d\n", stream_policy, mute, res);
		return false;
	}
	return true;
}

bool getStreamMuteState(stream_policy_t stream_policy, bool *mute)
{
	medvdbg("SoundManager : getStreamMuteState. stream_policy: %d\n", stream_policy);
	audio_manager_result_t res = get_audio_stream_mute_state(stream_policy, mute);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, ret : %d\n", stream_policy, res);
		return false;
	}
	return true;
}

