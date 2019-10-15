/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef ROM_MD5_H
#define ROM_MD5_H

#if PSK_SUPPORT_TKIP

#ifdef CONFIG_USE_MBEDTLS_ROM_ALG
#include "mbedtls/md5.h"
#define md5_ctx mbedtls_md5_context
#else
/* MD5 context. */
typedef struct {
	u32 state[4];  /* state (ABCD) */
	u32 count[2];  /* number of bits, modulo 2^64 (lsb first) */
	u8 buffer[64]; /* input buffer */
} md5_ctx;
#endif

void rt_md5_init(md5_ctx *context);
void rt_md5_append(md5_ctx *context, u8 *input, u32 inputLen);
void rt_md5_final(u8 digest[16], md5_ctx *context);
void rt_md5_hmac(unsigned char *text, int text_len, unsigned char *key,
				 int key_len, void *digest);

#endif //#if PSK_SUPPORT_TKIP
#endif
