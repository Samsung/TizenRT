/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup Media
 * @{
 *
 * @brief Declares APIs for media framework.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file rteffect_player.h
 *
 * @brief Provides the <b>EffectPlayer</b> class to implement operations related to effect player.
 *
 *
 * @since 1.0
 * @version 1.0
 */


#ifndef AMEBA_FWK_MEDIA_PLAYBACK_INTERFACES_MEDIA_EFFECT_PLAYER_H
#define AMEBA_FWK_MEDIA_PLAYBACK_INTERFACES_MEDIA_EFFECT_PLAYER_H

#include "osal_c/osal_types.h"

__BEGIN_DECLS

struct RTEffectPlayer;

enum {
	/** effect error happens */
	RTERROR_HAPPENS    = 0,
	/** effect successfully loaded */
	RTEFFECT_LOADED    = 1,
	/** effect successfully played done */
	RTPLAY_DONE        = 2,
};

/**
 * @brief Callback of RTEffectPlayer to notify the state of the current playing effect sound.
 * @since 1.0
 * @version 1.0
 */
struct RTEffectPlayerCallback {
	/**
	* @brief Player state listener.
	 *
	 * @param effect_id is the index set for the current effect sound.
	 * @param state is the state of current effect sound.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTEffectPlayerStateChange)(const struct RTEffectPlayerCallback *listener, const struct RTEffectPlayer *effect_player, int effect_id, uint32_t state);
};

/**
 * @brief Create RTEffectPlayer.
 *
 * @return a new RTEffectPlayer object pointer.
 * @since 1.0
 * @version 1.0
 */
struct RTEffectPlayer *RTEffectPlayer_Create();

/**
 * @brief Destroy RTEffectPlayer.
 *
 * @param player The RTEffectPlayer object pointer.
 * @since 1.0
 * @version 1.0
 */
void RTEffectPlayer_Destroy(struct RTEffectPlayer *player);

/**
 * @brief load the path of sound to effectplayer.
 *
 * @param player The RTEffectPlayer object pointer.
 * @param path is the absolute path of sound to load.
 * @return effect_id of the sound, this id is alway bigger than 0, if load fail, effect_id is 0.
 * @since 1.0
 * @version 1.0
 */
int RTEffectPlayer_Load(struct RTEffectPlayer *player, const char *path);

/**
 * @brief unload the effect_id loaded to effectplayer
 *
 * @param player The RTEffectPlayer object pointer.
 * @param effect_id is the sample id to unload.
 * @since 1.0
 * @version 1.0
 */
void RTEffectPlayer_UnLoad(struct RTEffectPlayer *player, int effect_id);

/**
 * @brief play the effect of effect_id
 *
 * @param player The RTEffectPlayer object pointer.
 * @param effect_id is the sample id to play.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the config value is not proper.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTEffectPlayer_Play(struct RTEffectPlayer *player, int effect_id);

/**
 * @brief stop the effect of effect_id
 *
 * @param player The RTEffectPlayer object pointer.
 * @param effect_id is the channel id to stop.
 * @since 1.0
 * @version 1.0
 */
void RTEffectPlayer_Stop(struct RTEffectPlayer *player, int effect_id);

/**
 * @brief SetCallback to effectplayer to get load complete signal
 *
 * @param player The RTEffectPlayer object pointer.
 * @param callback {@link RTEffectPlayerCallback} provides the information to user from fwk .
 * @since 1.0
 * @version 1.0
 */
void RTEffectPlayer_SetCallback(struct RTEffectPlayer *player, struct RTEffectPlayerCallback *callback);

/**
 * @brief Sets effect player volume when play started.
 *
 * @param player The RTEffectPlayer object pointer.
 * @param left The volume of left channel.
 * @param right The volume of right channel.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the config value is not proper.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTEffectPlayer_SetVolume(struct RTEffectPlayer *player, int effect_id, float left, float right);

__END_DECLS

#endif /*AMEBA_FWK_MEDIA_PLAYBACK_INTERFACES_MEDIA_EFFECT_PLAYER_H*/
/** @} */
