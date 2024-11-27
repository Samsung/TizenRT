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
	medvdbg("SoundManager : getVolume\n");
	audio_manager_result_t res = get_output_stream_volume(volume, stream_info);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("get_output_stream_volume() is failed, res = %d\n", res);
		return false;
	}
	return true;
}

bool setVolume(uint8_t volume, stream_info_t *stream_info)
{
	medvdbg("SoundManager : setVolume. volume: %d\n", volume);
	audio_manager_result_t res = set_output_audio_volume(volume, stream_info);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_output_audio_volume failed volume : %d ret : %d\n", volume, res);
		return false;
	}
	return true;
}

bool setEqualizer(uint32_t preset)
{
	medvdbg("SoundManager : setEqulizer. preset: %d\n", preset);
	audio_manager_result_t res = set_output_audio_equalizer(preset);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_output_audio_equalizer failed preset : %d ret : %d\n", preset, res);
		return false;
	}
	return true;
}

bool setMicMute(void)
{
	medvdbg("SoundManager : setMicMute\n");
	audio_manager_result_t res = set_mic_mute();
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_mic_mute failed ret : %d\n", res);
		return false;
	}
	return true;
}

bool setMicUnmute(void)
{
	medvdbg("SoundManager : setMicUnmute\n");
	audio_manager_result_t res = set_mic_unmute();
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_mic_unmute failed ret : %d\n", res);
		return false;
	}
	return true;
}

