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


#ifndef _FLAC_ENCODER_H
#define _FLAC_ENCODER_H

#include "flac_enc_types.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief FLAC encoder API
 * @defgroup Audio flac group
 * @{
 */

/**
 * @brief     Init audio Flac encoder
 *
 * @param setup this api will be call to init flac encoder
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_enc_init(flac_enc_setup_t *setup);


/**
 * @brief     Deinit audio Flac encoder
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_enc_deinit(void);

/**
 * @brief     Encoder one frame data
 *
 * @param pcm_buf pcm data to encoder
 * @param samples the number of samples in per channel
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_enc_process(int32_t *pcm_buf, uint32_t samples);

#ifdef __cplusplus
}
#endif

#endif	/* _FLAC_ENCODER_H */
