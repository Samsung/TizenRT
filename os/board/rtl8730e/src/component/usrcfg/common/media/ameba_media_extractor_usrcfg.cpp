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

#include <cstddef>

#include "media/config/ameba_media_extractor_usrcfg.h"

// ----------------------------------------------------------------------
//MediaExtractorConfig
extern void* GetWAVExtractorType();
extern void* GetMP3ExtractorType();
extern void* GetAACExtractorType();
extern void* GetMPEG4ExtractorType();
extern void* GetFLACExtractorType();
extern void* GetOGGExtractorType();

// ---------------------------
//Media
#ifdef MEDIA_PLAYER
MediaExtractorConfig kMediaExtractorConfigs[] = {
#if defined(MEDIA_DEMUX_WAV)
    { "wav", GetWAVExtractorType() },
#endif
#if defined(MEDIA_DEMUX_AAC)
    { "aac", GetAACExtractorType() },
#endif
#if defined(MEDIA_DEMUX_MP4)
    { "mp4", GetMPEG4ExtractorType() },
#endif
#if defined(MEDIA_DEMUX_MP3)
    { "mp3", GetMP3ExtractorType() },
#endif
#if defined(MEDIA_DEMUX_FLAC)
    { "flac", GetFLACExtractorType() },
#endif
#if defined(MEDIA_DEMUX_OGG)
    { "ogg-vorbis", GetOGGExtractorType() },
#endif
#if defined(MEDIA_DEMUX_OGG)
    { "ogg-opus", GetOGGExtractorType() },
#endif
};

size_t kNumMediaExtractorConfigs =
    sizeof(kMediaExtractorConfigs) / sizeof(kMediaExtractorConfigs[0]);
#endif

// ---------------------------
//MediaLite
#ifdef MEDIA_LITE_PLAYER
MediaExtractorConfig kMediaLiteExtractorConfigs[] = {
#if defined(MEDIA_LITE_DEMUX_WAV)
    { "wav", GetWAVExtractorType() },
#endif
#if defined(MEDIA_LITE_DEMUX_AAC)
    { "aac", GetAACExtractorType() },
#endif
#if defined(MEDIA_LITE_DEMUX_MP4)
    { "mp4", GetMPEG4ExtractorType() },
#endif
#if defined(MEDIA_LITE_DEMUX_MP3)
    { "mp3", GetMP3ExtractorType() },
#endif
};

size_t kNumMediaLiteExtractorConfigs =
    sizeof(kMediaLiteExtractorConfigs) / sizeof(kMediaLiteExtractorConfigs[0]);
#endif
