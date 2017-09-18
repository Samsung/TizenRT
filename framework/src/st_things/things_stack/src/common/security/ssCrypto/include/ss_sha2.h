/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Chul Lee(chuls.lee@samsung.com)'s posting:
 *   Modification for naming confilct.
 *   Attach prefix 'SS_' for all function and constants.
 *   Change name of data context to 'SS_SHAxxxContext' (xxx is bit length of digest)
 */

#ifndef SHA2_H
#define SHA2_H

#define SS_SHA224_DIGEST_SIZE (224 / 8)
#define SS_SHA256_DIGEST_SIZE (256 / 8)
#define SS_SHA384_DIGEST_SIZE (384 / 8)
#define SS_SHA512_DIGEST_SIZE (512 / 8)

#define SS_SHA256_BLOCK_SIZE  (512 / 8)
#define SS_SHA512_BLOCK_SIZE  (1024 / 8)
#define SS_SHA384_BLOCK_SIZE  SS_SHA512_BLOCK_SIZE
#define SS_SHA224_BLOCK_SIZE  SS_SHA256_BLOCK_SIZE

#ifndef SHA2_TYPES
#define SHA2_TYPES
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * SS_SHA256_BLOCK_SIZE];
	uint32 h[8];
} ss_sha256_ctx;

typedef struct {
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * SS_SHA512_BLOCK_SIZE];
	uint64 h[8];
} ss_sha512_ctx;

typedef ss_sha512_ctx ss_sha384_ctx;
typedef ss_sha256_ctx ss_sha224_ctx;

void ss_sha224_init(ss_sha224_ctx *ctx);
void ss_sha224_update(ss_sha224_ctx *ctx, const unsigned char *message, unsigned int len);
void ss_sha224_final(ss_sha224_ctx *ctx, unsigned char *digest);
void ss_sha224(const unsigned char *message, unsigned int len, unsigned char *digest);

void ss_sha256_init(ss_sha256_ctx *ctx);
void ss_sha256_update(ss_sha256_ctx *ctx, const unsigned char *message, unsigned int len);
void ss_sha256_final(ss_sha256_ctx *ctx, unsigned char *digest);
void ss_sha256(const unsigned char *message, unsigned int len, unsigned char *digest);

void ss_sha384_init(ss_sha384_ctx *ctx);
void ss_sha384_update(ss_sha384_ctx *ctx, const unsigned char *message, unsigned int len);
void ss_sha384_final(ss_sha384_ctx *ctx, unsigned char *digest);
void ss_sha384(const unsigned char *message, unsigned int len, unsigned char *digest);

void ss_sha512_init(ss_sha512_ctx *ctx);
void ss_sha512_update(ss_sha512_ctx *ctx, const unsigned char *message, unsigned int len);
void ss_sha512_final(ss_sha512_ctx *ctx, unsigned char *digest);
void ss_sha512(const unsigned char *message, unsigned int len, unsigned char *digest);

#ifdef __cplusplus
}
#endif
#endif							/* !SHA2_H */
