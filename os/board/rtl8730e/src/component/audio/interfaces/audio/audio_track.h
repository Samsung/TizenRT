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
#ifndef AMEBA_FWK_MEDIA_AUDIO_AUDIOLITE_INTERFACES_AUDIO_AUDIO_TRACK_H
#define AMEBA_FWK_MEDIA_AUDIO_AUDIOLITE_INTERFACES_AUDIO_AUDIO_TRACK_H

//#include "osal_c/osal_mutex.h"

#include "audio/audio_time.h"
#include "audio/audio_type.h"
#include "osal_c/osal_types.h"
#include "osal_c/osal_compat.h"

#ifdef __cplusplus
extern "C" {
#endif


struct RTAudioTrack;

/**
 * @brief Defines all the audio track configs.
 *
 * @since 1.0
 * @version 1.0
 */
typedef struct {
	/** category type of track */
	uint32_t category_type;
	/** sample_rate of track, supports 8000,16000,32000,44100,48000 */
	uint32_t sample_rate;
	/** channel num of track, supports 1, 2 */
	uint32_t channel_count;
	/** format of track, supports RTAUDIO_FORMAT_PCM_8_BIT, RTAUDIO_FORMAT_PCM_16_BIT,
	 * RTAUDIO_FORMAT_PCM_24_BIT_PACKEDM, RTAUDIO_FORMAT_PCM_32_BIT, RTAUDIO_FORMAT_PCM_FLOAT
	 */
	uint32_t format;
	/** bufsize of track */
	uint32_t buffer_bytes;
} RTAudioTrackConfig;

typedef struct {
	/** speed of track, support 0.5 ~ 6.0 */
	float speed;
	/** pitch of track, only support 1.0 now */
	float pitch;
} RTAudioPlaybackRate;

/**
 * @brief Create RTAudioTrack.
 */
struct RTAudioTrack *RTAudioTrack_Create(void);

/**
 * @brief Release RTAudioTrack.
 */
void RTAudioTrack_Destroy(struct RTAudioTrack *track);

/**
 * @brief Init audio track.
 * See the {@link RTAudioTrackConfig} for information about the options available to configure
 * your track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param config a {@link RTAudioTrackConfig} instance used to configure track information.
 * @param flags is the output flags for current audio track.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_NO_INIT | the init is not done.
 * OSAL_ERR_INVALID_OPERATION | the config value is not proper.
 * OSAL_ERR_NO_MEMORY | the memory alloc for track is fail.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_Init(struct RTAudioTrack *track, const RTAudioTrackConfig *config, uint32_t flags);

/**
 * @brief Set audio track play water level.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param bytes is the start playing water level bytes.
 * @return Returns the water level bytes sets to audio framework.
 * @since 1.0
 * @version 1.0
 */
ssize_t RTAudioTrack_SetStartThresholdBytes(struct RTAudioTrack *track, ssize_t bytes);

/**
 * @brief Get audio track play water level.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return Returns the playing water level bytes of current audio track.
 * @since 1.0
 * @version 1.0
 */
ssize_t RTAudioTrack_GetStartThresholdBytes(struct RTAudioTrack *track);

/**
 * @brief Start audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | should not start now.
 * OSAL_ERR_DEAD_OBJECT | the ipc is dead.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_Start(struct RTAudioTrack *track);

/**
 * @brief Stop audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @since 1.0
 * @version 1.0
 */
void RTAudioTrack_Stop(struct RTAudioTrack *track);

/**
 * @brief Write audio data.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param buffer is the src buffer of application.
 * @param size is the src buffer data bytes.
 * @param should_block choice whether to block when write stuck, suggest:true.
 * @return size of data written
 * @since 1.0
 * @version 1.0
 */
ssize_t RTAudioTrack_Write(struct RTAudioTrack *track, const void *buffer, size_t size, bool should_block);

/**
 * @brief Get minimun buffer bytes for track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param category_type can be a value listed below: \n
 * type | Description
 * ----------------------| -----------------------
 * RTAUDIO_CATEGORY_MEDIA | the data stream is music.
 * RTAUDIO_CATEGORY_COMMUNICATION | the data stream is call.
 * RTAUDIO_CATEGORY_SPEECH | the data stream is voice recognition.
 * RTAUDIO_CATEGORY_BEEP | the data stream is beep.
 * @param sample_rate is the samplerate of RTAudioTrack.
 * @param format can be a value listed below: \n
 * format | Description
 * ----------------------| -----------------------
 * RTAUDIO_FORMAT_PCM_8_BIT | 8bit data format.
 * RTAUDIO_FORMAT_PCM_16_BIT | 16bit data format.
 * RTAUDIO_FORMAT_PCM_32_BIT | 32bit data format.
 * RTAUDIO_FORMAT_PCM_FLOAT | float data format.
 * RTAUDIO_FORMAT_PCM_24_BIT_PACKED | 24bit data format.
 * @param channel_count is the channel count of RTAudioTrack.
 * @return {@link RTAudioTrackConfig#buffer_bytes} size of mininum buffer bytes
 * @since 1.0
 * @version 1.0
 */
size_t RTAudioTrack_GetMinBufferBytes(struct RTAudioTrack *track, uint32_t category_type, uint32_t sample_rate, uint32_t format, uint32_t channel_count);

/**
 * @brief Set samplerate of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param sample_rate the samplerate of track.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetSampleRate(struct RTAudioTrack *track, uint32_t sample_rate);

/**
 * @brief Get samplerate of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return samplerate
 * @since 1.0
 * @version 1.0
 */
uint32_t RTAudioTrack_GetSampleRate(struct RTAudioTrack *track);

/**
 * @brief Set format of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param format can be a value listed below: \n
 * format | Description
 * ----------------------| -----------------------
 * RTAUDIO_FORMAT_PCM_8_BIT | 8bit data format.
 * RTAUDIO_FORMAT_PCM_16_BIT | 16bit data format.
 * RTAUDIO_FORMAT_PCM_32_BIT | 32bit data format.
 * RTAUDIO_FORMAT_PCM_FLOAT | float data format.
 * RTAUDIO_FORMAT_PCM_24_BIT_PACKED | 24bit data format.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetFormat(struct RTAudioTrack *track, uint32_t format);

/**
 * @brief Get format of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return format can be a value listed below: \n
 * format | Description
 * ----------------------| -----------------------
 * RTAUDIO_FORMAT_INVALID | invalid data format.
 * RTAUDIO_FORMAT_PCM_8_BIT | 8bit data format.
 * RTAUDIO_FORMAT_PCM_16_BIT | 16bit data format.
 * RTAUDIO_FORMAT_PCM_32_BIT | 32bit data format.
 * RTAUDIO_FORMAT_PCM_FLOAT | float data format.
 * RTAUDIO_FORMAT_PCM_24_BIT_PACKED | 24bit data format.
 * @since 1.0
 * @version 1.0
 */
uint32_t RTAudioTrack_GetFormat(struct RTAudioTrack *track);

/**
 * @brief Set channel count of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param channel the channel count of track.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetChannelCount(struct RTAudioTrack *track, uint32_t channel);

/**
 * @brief Get channel count of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return channel count of track.
 * @since 1.0
 * @version 1.0
 */
uint32_t RTAudioTrack_GetChannelCount(struct RTAudioTrack *track);

/**
 * @brief Pause audio track streaming.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @since 1.0
 * @version 1.0
 */
void RTAudioTrack_Pause(struct RTAudioTrack *track);

/**
 * @brief Flush audio track streaming.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @since 1.0
 * @version 1.0
 */
void RTAudioTrack_Flush(struct RTAudioTrack *track);

/**
 * @brief Set volume of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param left the left channel volume of track, the value ranges from 0.0 to 1.0.
 * @param right the right channel volume of track, the value ranges from 0.0 to 1.0.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetVolume(struct RTAudioTrack *track, float left, float right);

/**
 * @brief Set speed of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param rate See the {@link RTAudioPlaybackRate} for information.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_PARAM | the speed value is not supported.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetPlaybackRate(struct RTAudioTrack *track, RTAudioPlaybackRate rate);

/**
 * @brief Get speed of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param rate See the {@link RTAudioPlaybackRate} for information.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_GetPlaybackRate(struct RTAudioTrack *track, RTAudioPlaybackRate *rate);

/**
 * @brief Get timestamp of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param stamp the timestamp get from audio framework, to be notice, this api can only
 * be used when there's no mixing of two tracks, for the mixing situation, to be supported.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_GetTimestamp(struct RTAudioTrack *track, RTAudioTimestamp *tstamp);

/**
 * @brief Get present playing PTS of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param now_ns the system time, or tsf time.
 * @param audio_ns the audio playing time.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_GetPresentTime(struct RTAudioTrack *track, int64_t *now_ns, int64_t *audio_ns);

/**
 * @brief Set params of track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param strs can be used to set the private data of track, for example, the amplifier pin.
 * to set the amplier pin, for example _PB_7, you can get your pin value from:
 * fwlib/include/ameba_pinmux.h, it's value is "#define _PB_7 (0x27)"
 * which is 39 of uint32_t.So the amp_pin here should set 39.The strs should be "amp_pin=39".
 * The amplifier pin setting with this method should be only used in "passthrough" mode(no audio mixer version).
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_SetParameters(struct RTAudioTrack *track, const char *strs);

/**
 * @brief Get DMA buffer status of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @return DMA buffer available size to write
 * @since 1.0
 * @version 1.0
 */
uint32_t RTAudioTrack_GetBufferStatus(struct RTAudioTrack *track);

/**
 * @brief Get msec latency of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param latency is the pointer of the latency user wants.
 * @return  Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_NO_INIT | track error.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_GetLatency(struct RTAudioTrack *track, uint32_t *latency);

/**
 * @brief Get position of audio track.
 *
 * @param track is the pointer of struct RTAudioTrack.
 * @param position is the pointer of the total number of frames played since track start.
 *      This value will overflow periodically according to track's rate.
 *      This value is reset to zero by RTAudioTrack_Stop() and RTAudioTrack_Flush().
 * @return  Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_NO_INIT | track error.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioTrack_GetPosition(struct RTAudioTrack *track, uint64_t *position);

#ifdef __cplusplus
}
#endif


#endif  // AMEBA_FWK_MEDIA_AUDIO_AUDIOLITE_INTERFACES_AUDIO_AUDIO_TRACK_H
