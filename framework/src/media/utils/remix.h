/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef REMIX_H
#define REMIX_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief   Get number of channels of the given channle layout
 * @param   layout: channel layout
 * @return  number of channels, return 0 if the given layout is unsupported
 */
uint32_t layout2ch(uint32_t layout);

/**
 * @brief   Get default layout which has the given number of channels
 * @param   nb_chs: number of channels
 * @return  channel layout, return 0 if the given number channels is unsupported
 */
uint32_t ch2layout(uint32_t nb_chs);

/**
 * @brief   Remix audio channel
 * @remarks Support multi-channel to stereo, stereo to mono, and mono to stereo.
 * @param   in_layout: channel layout of the input audio
 * @param   out_layout: channel layout desired for the output, supports Mono/Stereo only.
 * @param   input: pointer to the input buffer
 * @param   in_frames: number of frames in input buffer
 * @param   output: pointer to the output buffer, it can be same with input buffer
 * @param   max_frames: maximum of frames in specified layout can be stored in output buffer
 * @return  number of frames remixed and stored in output buffer, return negative value on failure.
 */
int32_t rechannel(uint32_t in_layout, uint32_t out_layout, const int16_t *input, uint32_t in_frames, int16_t *output, uint32_t max_frames);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* REMIX_H */
