/*
 * Copyright (c) 2023 Realtek, LLC.
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

#include "audio/audio_type.h"
#include "audio/config/ameba_audio_mixer_usrcfg.h"

PrimaryAudioConfig kPrimaryAudioConfig = {44100, 2, RTAUDIO_FORMAT_PCM_16_BIT, 1024, 4, RTAUDIO_OUT_MIN_FRAMES_STAGE1};
