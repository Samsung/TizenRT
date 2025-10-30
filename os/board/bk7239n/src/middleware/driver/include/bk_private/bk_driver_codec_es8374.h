// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void es8374_codec_init(void);
void es8374_codec_configure(unsigned int fs, unsigned char datawidth);
void es8374_codec_close(void);
void es8374_codec_volume_control(unsigned char volume);
void es8374_codec_mute_control(BOOL enable);

#ifdef __cplusplus
}
#endif
