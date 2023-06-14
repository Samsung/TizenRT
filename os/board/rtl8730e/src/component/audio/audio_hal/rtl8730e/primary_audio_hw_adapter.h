/*
 * Copyright (c) 2022 Realtek, LLC.
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

#ifndef AMEBA_HARDWARE_AUDIO_PRIMARY_AUDIO_ADAPTER_H
#define AMEBA_HARDWARE_AUDIO_PRIMARY_AUDIO_ADAPTER_H

#include <stdio.h>

#include "hardware/audio/audio_hw_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

// PrimaryAudioHwAdapter
struct PrimaryAudioHwAdapter {
	struct AudioHwAdapter adapter;

	_mutex lock;
	struct PrimaryAudioHwRender *output;
	struct PrimaryAudioHwCapture *input;
};

#ifdef __cplusplus
}
#endif
#endif //AMEBA_HARDWARE_AUDIO_PRIMARY_AUDIO_ADAPTER_H