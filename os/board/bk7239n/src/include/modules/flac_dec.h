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


#ifndef _FLAC_DECODER_H
#define _FLAC_DECODER_H

#include "flac_dec_types.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief FLAC decoder API
 * @defgroup Audio flac group
 * @{
 */

/**
 * @brief     Init audio Flac decoder
 *
 * @param read_cb this api will be call to read decoded data before decode one frame data
 * @param write_cb this api will be call to write decoded data after decode one frame data complete
 * @param error_cb this api will be call when error
 * @param metadata_cb this api will be call when decode audio information (channel, bits, sample rate and so on) complete
 * @param data the parameter of all callback api
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_dec_init(flac_dec_setup_t *setup);


/**
 * @brief     Deinit audio Flac decoder
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_dec_deinit(void);

/**
 * @brief     Decoder one frame data
 *
 * @param size save the size of data after processed
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors.
 */
bk_err_t bk_aud_flac_dec_process(void);

#ifdef __cplusplus
}
#endif

#endif	/* _FLAC_DECODER_H */
