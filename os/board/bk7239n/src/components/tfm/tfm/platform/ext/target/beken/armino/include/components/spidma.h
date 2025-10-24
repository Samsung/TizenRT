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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Init the spidma
 *
 * This API init spidma, open spidma
.*
 * @param data configure for camera and jpeg
 *
 * @attention 1. work for imaging transfer by spidma transfer way
 *
 * @return
 *    - kNoErr: success
 *    - other: failed
 */
bk_err_t spidma_intfer_init(void *data);

/**
 * @brief     Denit the spidma
 *
 * This API deinit spidma, reset spidma config, close spidma function
.*
 * @param data configure for camera and jpeg
 *
 * @attention 1. work for imaging transfer by spidma transfer way
 *
 * @return
 *    - kNoErr: success
 *    - other: failed
 */
bk_err_t spidma_intfer_deinit(void);

#ifdef __cplusplus
}
#endif


