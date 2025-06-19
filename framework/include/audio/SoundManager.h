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

#define AUDIO_DEVICE_MUTE_VALUE -1
#define AUDIO_DEVICE_UNMUTE_VALUE -2

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief: This listener is to notify about the mic and speaker state changes.
 * Applications should register this listener with the SoundManager, which maintains a list of such listeners.
 * Whenever any state change occurs, soundmanager calls each registered listener in the list.
 * It is expected that callee will not hold this listener.
 * @param[in] stream_type Stream type for which the state change has occurred.
 * STREAM_TYPE_VOICE_RECORD indicates mic, while other stream types correspond to the speaker.
 * @param[in] volume Volume level after the state change occurs
 * volume = AUDIO_DEVICE_MUTE_VALUE(-1) means mute for all stream types.
 * volume = AUDIO_DEVICE_UNMUTE_VALUE(-2) means umute for STREAM_TYPE_VOICE_RECORD, if input gain is not supported. 
 * If gain is supported, volume >= 0 will be shared. Currently, input gain is  not supported, so AUDIO_DEVICE_UNMUTE_VALUE will be given.
 * volume >= 0 means unmute with value representing the current volume/gain set for that stream type.
*/
typedef void (*VolumeStateChangedListener)(stream_policy_t stream_type, int8_t volume);

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
 * @brief Adds a listener in the listener list.
 * @param[in] listener Listener to be added in the listener list.
 * @return true if the listener was successfully added, false otherwise.
*/
bool addVolumeStateChangedListener(VolumeStateChangedListener listener);

/**
 * @brief Removes a listener from the listener list.
 * @param[in] listener Listener to be removed.
 * @return true if the listener was successfully removed, false otherwise.
*/
bool removeVolumeStateChangedListener(VolumeStateChangedListener listener);

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

