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

#if defined(__cplusplus)
extern "C" {
#endif

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

#if defined(__cplusplus)
}
#endif

#endif

