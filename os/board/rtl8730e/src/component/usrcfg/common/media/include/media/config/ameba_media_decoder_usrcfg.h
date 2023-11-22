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

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_DECODER_CONFIG_H
#define AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_DECODER_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------
//MediaDecoderConfig
#if defined MEDIA_PLAYER || defined WIFI_AUDIO
typedef void *(*CreateOMXComponentFunc)(
	const char *, const void *,
	void *, void **);

typedef struct MediaDecoderConfig {
	const char *name_;
	CreateOMXComponentFunc codec_func_;
} MediaDecoderConfig;

extern MediaDecoderConfig kMediaDecoderConfigs[];
extern size_t kNumMediaDecoderConfigs;
#endif


// ----------------------------------------------------------------------
//MediaLiteDecoderConfig
#ifdef MEDIA_LITE_PLAYER
typedef void *(*CreateCodecLiteFunc)(const char *);

typedef struct MediaLiteDecoderConfig {
	const char *name_;
	CreateCodecLiteFunc codec_func_;
} MediaLiteDecoderConfig;

extern MediaLiteDecoderConfig kMediaLiteDecoderConfigs[];
extern size_t kNumMediaLiteDecoderConfigs;
#endif

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_DECODER_CONFIG_H
