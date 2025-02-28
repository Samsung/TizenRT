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

sq_queue_t gPlayerVolumeListenerList, gRecorderVolumeListenerList;// List of listeners for volume, mute changes.

void sound_manager_init(void)
{
	sq_init(((sq_queue_t *)&gPlayerVolumeListenerList));
	sq_init(((sq_queue_t *)&gRecorderVolumeListenerList));
}

void notifyListeners(uint16_t state, stream_policy_t stream_type, int8_t volume, sq_queue_t list)
{
	FAR struct VolumeStateChangedListenerListNode *itr;
	for (itr = (FAR struct VolumeStateChangedListenerListNode *)sq_peek(&list); itr; itr = sq_next(itr)) {
		if ((itr->state & state) && itr->stream_type == stream_type) {
			itr->listener(state, stream_type, volume);
		}
	}
}

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
	sq_queue_t list;
	int16_t state;
	if (stream_info->policy == STREAM_TYPE_VOICE_RECORD) {
		state = AUDIO_DEVICE_STATE_MIC_GAIN_LEVEL;
		list = gRecorderVolumeListenerList;
	} else {
		state = AUDIO_DEVICE_STATE_SPEAKER_GAIN_LEVEL;
		list = gPlayerVolumeListenerList;
	}
	notifyListeners(state, stream_info->policy, volume, list);
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

static bool addVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, OnRecorderVolumeStateChangedListener listener, sq_queue_t list)
{
	struct VolumeStateChangedListenerListNode *newNode = (struct VolumeStateChangedListenerListNode *)malloc(sizeof(struct VolumeStateChangedListenerListNode));
	if (newNode == NULL) {
		meddbg("memory allocation for listener has failed.\n");
		return false;
	}
	newNode->listener = listener;
	newNode->state = state;
	newNode->stream_type = stream_type;
	sq_addfirst((FAR sq_entry_t *)newNode, (FAR sq_queue_t *)&list);
	return true;
}

bool addRecorderVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, OnRecorderVolumeStateChangedListener listener)
{
	return addVolumeStateChangedListener(state,stream_type, listener, gRecorderVolumeListenerList);
}


bool addPlayerVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, OnPlayerVolumeStateChangedListener listener)
{
	return addVolumeStateChangedListener(state, stream_type, listener, gPlayerVolumeListenerList);
}

bool removeVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, VolumeStateChangedListener listener, sq_queue_t list)
{
	FAR struct VolumeStateChangedListenerListNode *prev = NULL;
	FAR struct VolumeStateChangedListenerListNode *curr;
	for (curr = (FAR struct VolumeStateChangedListenerListNode *)sq_peek(&list); curr; prev = curr, curr = sq_next(curr)) {
		if (curr->listener == listener) {
			if (prev == NULL) {
				sq_remfirst(&list);
			} else {
				sq_remafter((FAR sq_entry_t *)prev, &list);
			}
			medvdbg("found the listener to remove %x\n", listener);
			return true;
		}
	}
	meddbg("Listener not found\n");
	return false;
}

bool removeRecorderVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, OnRecorderVolumeStateChangedListener listener)
{
	return removeVolumeStateChangedListener(state, stream_type, listener, gRecorderVolumeListenerList);
}

bool removePlayerVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, OnPlayerVolumeStateChangedListener listener)
{
	return removeVolumeStateChangedListener(state, stream_type, listener, gPlayerVolumeListenerList);
}

bool setMicMute(void)
{
	medvdbg("SoundManager : setMicMute\n");
	audio_manager_result_t res = set_audio_stream_mute(STREAM_TYPE_VOICE_RECORD, true);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, mute : %d, ret : %d\n", STREAM_TYPE_VOICE_RECORD, true, res);
		return false;
	}
	notifyListeners(AUDIO_DEVICE_STATE_MIC_MUTE, STREAM_TYPE_VOICE_RECORD, SOUND_MANAGER_VOLUME_MUTE, gRecorderVolumeListenerList);

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
	notifyListeners(AUDIO_DEVICE_STATE_MIC_MUTE, STREAM_TYPE_VOICE_RECORD, SOUND_MANAGER_VOLUME_UNMUTE, gRecorderVolumeListenerList);

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
	sq_queue_t list;
	int16_t state;
	if (stream_policy == STREAM_TYPE_VOICE_RECORD) {
		state = AUDIO_DEVICE_STATE_MIC_MUTE;
		list = gRecorderVolumeListenerList;
	} else {
		state = AUDIO_DEVICE_STATE_SPEAKER_MUTE;
		list = gPlayerVolumeListenerList;
	}
	notifyListeners(state, stream_policy, SOUND_MANAGER_VOLUME_UNMUTE - (mute ? 1 : 0), list);

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

bool enableDMIC(bool enable)
{
	medvdbg("SoundManager : enableDMIC. enable : %d\n", enable);
	audio_manager_result_t res = set_dmic(enable);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("senableDMIC failed enable : %d, ret : %d\n", enable, res);
		return false;
	}
	return true;
}

