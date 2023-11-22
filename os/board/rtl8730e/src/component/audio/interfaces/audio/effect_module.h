/*
**
** Copyright 2012, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef AMEBA_FWK_MEDIA_AUDIO_AUDIO_LITE_AUDIO_EFFECT_MODULE_H
#define AMEBA_FWK_MEDIA_AUDIO_AUDIO_LITE_AUDIO_EFFECT_MODULE_H

#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RTEffectModule;
struct RTEffectAttributesBuilder;

int32_t RTEffectAttributesBuilder_Create(struct RTEffectAttributesBuilder **builder);
void    RTEffectAttributesBuilder_Destroy(struct RTEffectAttributesBuilder *builder);
int32_t RTEffectAttributesBuilder_SetInRate(struct RTEffectAttributesBuilder *builder, uint32_t rate);
int32_t RTEffectAttributesBuilder_SetOutRate(struct RTEffectAttributesBuilder *builder, uint32_t rate);
int32_t RTEffectAttributesBuilder_SetInFormat(struct RTEffectAttributesBuilder *builder, uint32_t format);
int32_t RTEffectAttributesBuilder_SetOutFormat(struct RTEffectAttributesBuilder *builder, uint32_t format);
int32_t RTEffectAttributesBuilder_SetInChannels(struct RTEffectAttributesBuilder *builder, uint32_t channels);
int32_t RTEffectAttributesBuilder_SetOutChannels(struct RTEffectAttributesBuilder *builder, uint32_t channels);
int32_t RTEffectAttributesBuilder_SetFrameCount(struct RTEffectAttributesBuilder *builder, uint32_t frame_count);

int32_t RTEffectModule_Create(struct RTEffectAttributesBuilder *builder, struct RTEffectModule **stream);
void    RTEffectModule_Destroy(struct RTEffectModule *stream);
int32_t RTEffectModule_SetConfig(struct RTEffectModule *stream);
int32_t RTEffectModule_Start(struct RTEffectModule *stream);
int32_t RTEffectModule_Stop(struct RTEffectModule *stream);
void    RTEffectModule_SetInBuffer(struct RTEffectModule *stream, int16_t *buf);
void    RTEffectModule_SetOutBuffer(struct RTEffectModule *stream, int16_t *buf);
void    RTEffectModule_SetFrameCount(struct RTEffectModule *module, int32_t frame_count);
void    RTEffectModule_process(struct RTEffectModule *stream);
int32_t RTEffectModule_command(struct RTEffectModule *stream, uint32_t cmd_code, uint32_t cmd_size, void *cmd_data, uint32_t *reply_size, void *reply_data);

#ifdef __cplusplus
}
#endif

#endif