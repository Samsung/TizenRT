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

#include "media/config/ameba_media_decoder_usrcfg.h"

// ----------------------------------------------------------------------
//MediaDecoderConfig
#if defined MEDIA_PLAYER || defined WIFI_AUDIO
void *CreatePCMDec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateMP3Dec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateHAACDec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateVorbisDec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateOpusDec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateFLACDec(
        const char *name, const void *callbacks,
        void *appData, void **component);

void *CreateFLACEnc(
        const char *name, const void *callbacks,
        void *appData, void **component);

MediaDecoderConfig kMediaDecoderConfigs[] = {
#if defined(MEDIA_CODEC_PCM)
    { "wav", CreatePCMDec },
#endif
#if defined(MEDIA_CODEC_MP3)
    { "mp3", CreateMP3Dec },
#endif
#if defined(MEDIA_CODEC_HAAC)
    { "aac", CreateHAACDec },
#endif
#if defined(MEDIA_CODEC_HAAC)
    { "mp4", CreateHAACDec },
#endif
#if defined(MEDIA_CODEC_PCM)
    { "flac", CreatePCMDec },
#endif
#if defined(MEDIA_CODEC_VORBIS)
    { "ogg-vorbis", CreateVorbisDec },
#endif
#if defined(MEDIA_CODEC_OPUS)
    { "ogg-opus", CreateOpusDec },
#endif
#if defined(MEDIA_DEC_FLAC)
    { "flac-decoder", CreateFLACDec },
#endif
#if defined(MEDIA_ENC_FLAC)
    {"flac-encoder", CreateFLACEnc},
#endif
};

size_t kNumMediaDecoderConfigs =
    sizeof(kMediaDecoderConfigs) / sizeof(kMediaDecoderConfigs[0]);
#endif


// ----------------------------------------------------------------------
//MediaLiteDecoderConfig
#ifdef MEDIA_LITE_PLAYER
extern "C" {
void *CreatePCMLiteDec(const char *name);
void *CreateMP3LiteDec(const char *name);
void *CreateHAACLiteDec(const char *name);
}

MediaLiteDecoderConfig kMediaLiteDecoderConfigs[] = {
#if defined(MEDIA_LITE_CODEC_PCM)
    { "wav", CreatePCMLiteDec },
#endif
#if defined(MEDIA_LITE_CODEC_MP3)
    { "mp3", CreateMP3LiteDec },
#endif
#if defined(MEDIA_LITE_CODEC_HAAC)
    { "aac", CreateHAACLiteDec },
#endif
#if defined(MEDIA_LITE_CODEC_HAAC)
    { "mp4", CreateHAACLiteDec },
#endif
#if defined(MEDIA_LITE_CODEC_FLAC)
    { "flac", NULL },
#endif
#if defined(MEDIA_LITE_CODEC_VORBIS)
    { "ogg-vorbis", NULL },
#endif
#if defined(MEDIA_LITE_CODEC_OPUS)
    { "ogg-opus", NULL },
#endif
};

size_t kNumMediaLiteDecoderConfigs =
    sizeof(kMediaLiteDecoderConfigs) / sizeof(kMediaLiteDecoderConfigs[0]);
#endif
