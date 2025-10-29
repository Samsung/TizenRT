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
#include "tfm_hkdf_nsc.h"
#include "mbedtls/error.h"
#include "mbedtls/hkdf.h"

#define HKDF_TAG "HKDF"

#define HKDF_LOGI(...) BK_LOGI(HKDF_TAG, ##__VA_ARGS__)
#define HKDF_LOGW(...) BK_LOGW(HKDF_TAG, ##__VA_ARGS__)
#define HKDF_LOGE(...) BK_LOGE(HKDF_TAG, ##__VA_ARGS__)
#define HKDF_LOGD(...) BK_LOGD(HKDF_TAG, ##__VA_ARGS__)

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_hkdf_nsc_stub(void)
{
	return;
}

__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_mbedtls_hkdf_nsc( uint8_t type,
                                                                                   hkdf_input_t *in, hkdf_output_t *out )
{
	int ret = 0;
	mbedtls_md_type_t id;

	switch( type )
	{
		case 0x05:
			id = MBEDTLS_MD_SHA1;   break; //sha1
		case 0x08:
			id = MBEDTLS_MD_SHA224; break; //sha224
		case 0x09:
			id = MBEDTLS_MD_SHA256; break; //sha256
		default:
			HKDF_LOGE("unsupport type %d\r\n", type);
			return MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
	}

 	const mbedtls_md_info_t *info = mbedtls_md_info_from_type( id );

	return mbedtls_hkdf( info, in->salt, in->salt_len, in->ikm, in->ikm_len, in->info, in->info_len,
                        out->okm, out->okm_len);

}

