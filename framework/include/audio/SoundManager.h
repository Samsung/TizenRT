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

/**
 * @file SoundManager.h
 * @brief A wrapper over audio_manager to provide audio functionalities like volume, equalizer, & mute.
 */

#ifndef __AUDIO_SOUNDMANAGER_H
#define __AUDIO_SOUNDMANAGER_H

#include <media/stream_info.h>
#include <stdbool.h>
#include <stdlib.h>
#include <queue.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define AUDIO_DEVICE_STATE_MIC_MUTE		0x0001	/* for mute, unmute events of recorder device */
#define AUDIO_DEVICE_STATE_SPEAKER_MUTE		0x0010	/* for mute, unmute events of player device */
#define AUDIO_DEVICE_STATE_MIC_GAIN_LEVEL	0x0100	/* for volume change events of recorder device */
#define AUDIO_DEVICE_STATE_SPEAKER_GAIN_LEVEL	0x1000	/* for volume change events of player device */
#define AUDIO_DEVICE_STATE_ALL			0x1111	/* for all events of both devices */

#define SOUND_MANAGER_VOLUME_MUTE -2
#define SOUND_MANAGER_VOLUME_UNMUTE -1
 

/**
 * @brief: Sound Manager calls this function to notify all registered applications.
 * This callback is called when mic status and speaker status changed.
 * It is expected that callee will not hold this thread.
 * @param[in] state State for which change has occured.
 * @param[in] stream_type Stream type for which the state change has occurred.
 * @param[in] value Value of the state change.
 * For example, if state is AUDIO_DEVICE_STATE_MIC_MUTE or AUDIO_DEVICE_STATE_SPEAKER_MUTE, then value can be SOUND_MANAGER_VOLUME_MUTE or SOUND_MANAGER_VOLUME_UNMUTE. 
 * If state is AUDIO_DEVICE_STATE_MIC_GAIN_LEVEL or AUDIO_DEVICE_STATE_SPEAKER_GAIN_LEVEL, then value can be between 0 to 15.
*/
typedef void (*VolumeStateChangedListener)(uint16_t state, stream_policy_t stream_type, int8_t value);

struct VolumeStateChangedListenerListNode {
    FAR struct VolumeStateChangedListenerListNode *flink;
    stream_policy_t stream_type;
    uint16_t state;
    VolumeStateChangedListener listener;
};

/**
 * @brief Retrieves the current volume level for a given stream.
 * @param[out] volume A pointer to a uint8_t where the current volume level will be stored.
 * @param[in] stream_info A pointer to a stream_info_t structure containing information about the stream.
 * @return true if the operation was successful, false otherwise.
 */
bool getVolume(uint8_t *volume, stream_info_t *stream_info);

/**
 * @brief Sets the volume level for a given stream.
 * @param[in] volume The new volume level to be set.
 * @param[in] stream_info A pointer to a stream_info_t structure containing information about the stream.
 * @return true if the operation was successful, false otherwise.
 */
bool setVolume(uint8_t volume, stream_info_t *stream_info);

/**
 * @brief Registers a listener for volume state changes.
 * @param[in] state State change of the device that application wants notification.
 * For example, if app wants to listen to MIC_MUTE state change, then it should pass AUDIO_DEVICE_STATE_MIC_MUTE as state. 
 * For listening to multiple events, it is required to pass bitwise OR of the states for which notification is required.
 * @param[in] stream_type Stream type for which the state change has occurred.
 * @param[in] listener Callback function to be called when the state changes.
*/
bool addVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, VolumeStateChangedListener listener);

/**
 * @brief UnRegisters a listener for volume state changes.
 * @param[in] state State change of the device that application wants notification.
 * @param[in] stream_type Stream type for which the state change has occurred.
 * @param[in] listener Callback function to be unregistered.
*/
bool removeVolumeStateChangedListener(uint16_t state, stream_policy_t stream_type, VolumeStateChangedListener listener);

/**
 * @brief Applies a predefined equalizer preset.
 * @param[in] preset The index of the equalizer preset to be applied.
 * @return true if the operation was successful, false otherwise.
 */
bool setEqualizer(uint32_t preset);

/**
 * @brief Mutes the mic.
 * @return true if the operation was successful, false otherwise.
 */
bool setMicMute(void);

/**
 * @brief Unmutes the mic.
 * @return true if the operation was successful, false otherwise.
 */
bool setMicUnmute(void);

/**
 * @brief Sets mute state for the given stream.
 * @param[in] stream_policy Policy of the stream.
 * @param[in] mute The mute state to be set. true for mute, false for unmute.
 * @return true if the operation was successful, false otherwise.
 */
bool setStreamMute(stream_policy_t stream_policy, bool mute);

/**
 * @brief Gets mute state for the given stream.
 * @param[in] stream_policy Policy of the stream.
 * @param[in] mute Pointer to store the mute state. true for mute, false for unmute.
 * @return true if the operation was successful, false otherwise.
 */
bool getStreamMuteState(stream_policy_t stream_policy, bool *mute);

/**
 * @brief Enable dmic or keep internal mic
 * @param[in] enable dmic or not
 * @return true if the operation was successful, false otherwise.
 */
bool enableDMIC(bool enable);

#if defined(__cplusplus)
}
#endif

#endif

