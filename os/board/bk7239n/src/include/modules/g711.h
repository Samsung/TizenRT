// Copyright 2020-2021 Beken
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

#ifndef __G711_H__
#define __G711_H__
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif//__cplusplus

/**
 * @brief AEC API
 * @defgroup AEC API group
 * @{
 */

/**
 * @brief     G711A encodec pcm data to a-law data
 *
 * @param pcm_val 16 bit pcm data
 *
 * @return
 *    - unsigned char: a-law data after pcm data has been encoded by G711A
 */
unsigned char linear2alaw(int pcm_val);

 /**
 * @brief     G711A decodec a-law data to pcm data
 *
 * @param a_val 8 bit a-law data
 *
 * @return
 *    - int: pcm data after a-law data has been decoded by G711A
 */
int alaw2linear(unsigned char a_val);

/**
 * @brief     G711U encodec pcm data to u-law data
 *
 * @param pcm_val 16 bit pcm data
 *
 * @return
 *    - unsigned char: u-law data after pcm data has been encoded by G711U
 */
unsigned char linear2ulaw(int pcm_val);

 /**
 * @brief     G711U decodec u-law data to pcm data
 *
 * @param u_val 8 bit u-law data
 *
 * @return
 *    - int: pcm data after u-law data has been decoded by G711U
 */
int ulaw2linear(unsigned char u_val);

/**
 * @}
 */


#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//__AEC_H__

