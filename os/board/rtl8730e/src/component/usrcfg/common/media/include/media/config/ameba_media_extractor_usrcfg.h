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

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_EXTRACTOR_CONFIG_H
#define AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_EXTRACTOR_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------
//MediaExtractorConfig
typedef struct MediaExtractorConfig {
	const char *name_;
	void *extractor_type_;
} MediaExtractorConfig;

// ---------------------------
//Media
#ifdef MEDIA_PLAYER
extern MediaExtractorConfig kMediaExtractorConfigs[];
extern size_t kNumMediaExtractorConfigs;
#endif

// ---------------------------
//MediaLite
#ifdef MEDIA_LITE_PLAYER
extern MediaExtractorConfig kMediaLiteExtractorConfigs[];
extern size_t kNumMediaLiteExtractorConfigs;
#endif

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_FWK_MEDIA_PLAYBACK_CONFIG_MEDIA_CONFIG_MEDIA_EXTRACTOR_CONFIG_H