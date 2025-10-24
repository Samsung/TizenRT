// Copyright 2022-2023 Beken
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


#ifndef AUDIO_AGC_H_
#define AUDIO_AGC_H_

#include <modules/audio_agc_types.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/*
 * @brief This function is called to process input mic data.
 *
 * This function processes a 10/20ms frame and adjusts (normalizes) the gain
 * both analog and digitally. The gain adjustments are done only during
 * active periods of speech. The input speech length can be either 10ms or
 * 20ms and the output is of the same length. The length of the speech
 * vectors must be given in samples (80/160 when FS=8000, and 160/320 when
 * FS=16000 or FS=32000).
 *
 * This function should be called after processing the near-end microphone
 * signal.
 *
 * @param[in]       agcInst: AGC instance
 * @param[in]       inNear : Near-end input speech vector (10 or 20 ms) for L band
 * @param[in]       samples: Number of samples in input/output vector
 *
 * @param[out]      out: Gain-adjusted near-end speech vector (L band)
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_process(void* agcInst,
                      const int16_t* inNear,
                      int16_t samples,
                      int16_t* out);

/*
 * @brief This function sets the config parameters.
 *
 * @param[in]       agcInst: AGC instance
 * @param[in]       config : config struct
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_set_config(void* agcInst, bk_agc_config_t config);

/*
 * @brief This function returns the config parameters.
 *
 * @param[in]       agcInst: AGC instance
 *
 * @param[out]      config : config struct
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_get_config(void* agcInst, bk_agc_config_t* config);

/*
 * @brief This function creates an AGC instance.
 *
 * @param[in]       agcInst: AGC instance
 * @param[out]      agcInst: AGC instance
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_create(void **agcInst);

/*
 * @brief This function frees the AGC instance.
 *
 * @param[in]       agcInst: AGC instance
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_free(void *agcInst);

/*
 * @brief This function initializes an AGC instance.
 *
 * Input:
 * @param[in]       agcInst : AGC instance.
 * @param[in]       minLevel: Minimum possible mic level
 * @param[in]       maxLevel: Maximum possible mic level
 * @param[in]       fs      : Sampling frequency
 *
 * @return
 *                  - BK_OK: success
 *                  - other: failed
 */
bk_err_t bk_aud_agc_init(void *agcInst,
                   int32_t minLevel,
                   int32_t maxLevel,
                   uint32_t fs);

#if defined(__cplusplus)
}
#endif

#endif  // AUDIO_AGC_H_
