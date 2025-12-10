/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
#include <audio/audio_manager.h>
#include <debug.h>
#include <PlayerObserverWorker.h>
#include <RecorderObserverWorker.h>
#include <list>

using namespace media;

static list<VolumeStateChangedListener> gVolumeListenerList;
static std::mutex gVolumeListenerListAccessLock;
static int listenerCount = 0;

static void notifyListeners(stream_policy_t stream_type, int8_t volume)
{
	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	list<VolumeStateChangedListener>::iterator itr;
	for (itr = gVolumeListenerList.begin(); itr != gVolumeListenerList.end(); itr++) {
		(*itr)(stream_type, volume);
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

	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	if (listenerCount == 0) {
		return true;
	}

	if (stream_info->policy != STREAM_TYPE_VOICE_RECORD) {
		PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
		pow.enQueue(&notifyListeners, stream_info->policy, volume);
	} else {
		RecorderObserverWorker &row = RecorderObserverWorker::getWorker();
		row.enQueue(&notifyListeners, stream_info->policy, volume);
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

bool addVolumeStateChangedListener(VolumeStateChangedListener listener)
{
	if (listener == nullptr) {
		meddbg("addVolumeStateChangedListener : invalid argument.\n");
		return false;
	}

	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	list<VolumeStateChangedListener>::iterator itr;
	for (itr = gVolumeListenerList.begin(); itr != gVolumeListenerList.end(); itr++) {
		if (*itr == listener) {
			medvdbg("Listener already registered, ignore operation!!\n");
			return true;
		}
	}
	gVolumeListenerList.push_back(listener);
	listenerCount += 1;
	medvdbg("added new listener %x\n", listener);
	return true;
}

bool removeVolumeStateChangedListener(VolumeStateChangedListener listener)
{
	if (listener == nullptr) {
		meddbg("removeVolumeStateChangedListener : invalid argument.\n");
		return false;
	}

	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	list<VolumeStateChangedListener>::iterator itr;
	for (itr = gVolumeListenerList.begin(); itr != gVolumeListenerList.end(); itr++) {
		if (*itr == listener) {
			gVolumeListenerList.erase(itr);
			listenerCount -= 1;
			medvdbg("found the listener to remove %x\n", listener);
			return true;
		}
	}
	meddbg("Listener not found\n");
	return false;
}

bool setMicMute(void)
{
	medvdbg("SoundManager : setMicMute\n");
	audio_manager_result_t res = set_audio_stream_mute(STREAM_TYPE_VOICE_RECORD, true);
	if (res != AUDIO_MANAGER_SUCCESS) {
		meddbg("set_audio_stream_mute failed stream_policy : %d, mute : %d, ret : %d\n", STREAM_TYPE_VOICE_RECORD, true, res);
		return false;
	}

	RecorderObserverWorker &row = RecorderObserverWorker::getWorker();
	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	if (listenerCount == 0) {
		return true;
	}

	row.enQueue(&notifyListeners, STREAM_TYPE_VOICE_RECORD, AUDIO_DEVICE_MUTE_VALUE);
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

	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	if (listenerCount == 0) {
		return true;
	}

	int8_t volume = AUDIO_DEVICE_UNMUTE_VALUE;
	res = get_input_audio_gain(reinterpret_cast<uint8_t*>(&volume));
	if (res != AUDIO_MANAGER_SUCCESS && res != AUDIO_MANAGER_DEVICE_NOT_SUPPORT) {
		return false;
	}
	RecorderObserverWorker &row = RecorderObserverWorker::getWorker();
	row.enQueue(&notifyListeners, STREAM_TYPE_VOICE_RECORD, volume);
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

	int8_t volume = 0;
	if (mute) {
		volume = AUDIO_DEVICE_MUTE_VALUE;
	} else if (stream_policy == STREAM_TYPE_VOICE_RECORD) {
		volume = AUDIO_DEVICE_UNMUTE_VALUE;
		res = get_input_audio_gain(reinterpret_cast<uint8_t*>(&volume));
		if (res != AUDIO_MANAGER_SUCCESS && res != AUDIO_MANAGER_DEVICE_NOT_SUPPORT) {
			return false;
		}
	} else {
		res = get_output_stream_volume(reinterpret_cast<uint8_t*>(&volume), stream_policy);
		if (res != AUDIO_MANAGER_SUCCESS) {
			meddbg("get_output_stream_volume failed stream_policy : %d, ret : %d\n", stream_policy, res);
			return false;
		}
	}
	lock_guard<mutex> lock(gVolumeListenerListAccessLock);
	if (listenerCount == 0) {
		return true;
	}

	if (stream_policy != STREAM_TYPE_VOICE_RECORD) {
		PlayerObserverWorker &pow = PlayerObserverWorker::getWorker();
		pow.enQueue(&notifyListeners, stream_policy, volume);
	} else {
		RecorderObserverWorker &row = RecorderObserverWorker::getWorker();
		row.enQueue(&notifyListeners, stream_policy, volume);
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
