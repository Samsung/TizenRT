// Copyright 2022-2025 Beken
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


#include <stdio.h>
#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "tfm_secure_api.h"
#include "tfm_drbg_nsc.h"

#define DRBG_TAG "DRBG"

#define DRBG_LOGI(...) BK_LOGI(DRBG_TAG, ##__VA_ARGS__)
#define DRBG_LOGW(...) BK_LOGW(DRBG_TAG, ##__VA_ARGS__)
#define DRBG_LOGE(...) BK_LOGE(DRBG_TAG, ##__VA_ARGS__)
#define DRBG_LOGD(...) BK_LOGD(DRBG_TAG, ##__VA_ARGS__)

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_drbg_nsc_stub( void )
{
	return;
}

extern int block_cipher_df_ex( unsigned char *output, const unsigned char *data, size_t data_len );
__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_block_cipher_df_nsc( unsigned char *output,
                                                                          const unsigned char *data, size_t data_len )
{
	return block_cipher_df_ex( output, data, data_len );
}

