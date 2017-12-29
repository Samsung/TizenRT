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

#if 0
#define UNROLL_LOOPS			/* Enable loops unrolling */
#endif

#include <string.h>

#include "ss_sha2.h"

#define SS_SHFR(x, n)    (x >> n)
#define SS_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SS_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SS_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SS_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SS_SHA256_F1(x) (SS_ROTR(x,  2) ^ SS_ROTR(x, 13) ^ SS_ROTR(x, 22))
#define SS_SHA256_F2(x) (SS_ROTR(x,  6) ^ SS_ROTR(x, 11) ^ SS_ROTR(x, 25))
#define SS_SHA256_F3(x) (SS_ROTR(x,  7) ^ SS_ROTR(x, 18) ^ SS_SHFR(x,  3))
#define SS_SHA256_F4(x) (SS_ROTR(x, 17) ^ SS_ROTR(x, 19) ^ SS_SHFR(x, 10))

#define SS_SHA512_F1(x) (SS_ROTR(x, 28) ^ SS_ROTR(x, 34) ^ SS_ROTR(x, 39))
#define SS_SHA512_F2(x) (SS_ROTR(x, 14) ^ SS_ROTR(x, 18) ^ SS_ROTR(x, 41))
#define SS_SHA512_F3(x) (SS_ROTR(x,  1) ^ SS_ROTR(x,  8) ^ SS_SHFR(x,  7))
#define SS_SHA512_F4(x) (SS_ROTR(x, 19) ^ SS_ROTR(x, 61) ^ SS_SHFR(x,  6))

#define SS_UNPACK32(x, str)                      \
{                                             \
	*((str) + 3) = (uint8) ((x));       \
	*((str) + 2) = (uint8) ((x) >>  8);       \
	*((str) + 1) = (uint8) ((x) >> 16);       \
	*((str) + 0) = (uint8) ((x) >> 24);       \
}

#define SS_PACK32(str, x)                        \
{                                             \
	*(x) =   ((uint32) *((str) + 3))    \
			| ((uint32) *((str) + 2) <<  8)    \
			| ((uint32) *((str) + 1) << 16)    \
			| ((uint32) *((str) + 0) << 24);   \
}

#define SS_UNPACK64(x, str)                      \
{                                             \
	*((str) + 7) = (uint8) ((x));       \
	*((str) + 6) = (uint8) ((x) >>  8);       \
	*((str) + 5) = (uint8) ((x) >> 16);       \
	*((str) + 4) = (uint8) ((x) >> 24);       \
	*((str) + 3) = (uint8) ((x) >> 32);       \
	*((str) + 2) = (uint8) ((x) >> 40);       \
	*((str) + 1) = (uint8) ((x) >> 48);       \
	*((str) + 0) = (uint8) ((x) >> 56);       \
}

#define SS_PACK64(str, x)                        \
{                                             \
	*(x) =   ((uint64) *((str) + 7))    \
			| ((uint64) *((str) + 6) <<  8)    \
			| ((uint64) *((str) + 5) << 16)    \
			| ((uint64) *((str) + 4) << 24)    \
			| ((uint64) *((str) + 3) << 32)    \
			| ((uint64) *((str) + 2) << 40)    \
			| ((uint64) *((str) + 1) << 48)    \
			| ((uint64) *((str) + 0) << 56);   \
}

/* Macros used for loops unrolling */

#define SS_SHA256_SCR(i)                         \
{                                             \
	w[i] =  SS_SHA256_F4(w[i -  2]) + w[i -  7]  \
			+ SS_SHA256_F3(w[i - 15]) + w[i - 16]; \
}

#define SS_SHA512_SCR(i)                         \
{                                             \
	w[i] =  SS_SHA512_F4(w[i -  2]) + w[i -  7]  \
			+ SS_SHA512_F3(w[i - 15]) + w[i - 16]; \
}

#define SS_SHA256_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
	t1 = wv[h] + SS_SHA256_F2(wv[e]) + SS_CH(wv[e], wv[f], wv[g]) \
		+ ss_sha256_k[j] + w[j];                              \
	t2 = SS_SHA256_F1(wv[a]) + SS_MAJ(wv[a], wv[b], wv[c]);       \
	wv[d] += t1;                                            \
	wv[h] = t1 + t2;                                        \
}

#define SS_SHA512_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
	t1 = wv[h] + SS_SHA512_F2(wv[e]) + SS_CH(wv[e], wv[f], wv[g]) \
		+ ss_sha512_k[j] + w[j];                              \
	t2 = SS_SHA512_F1(wv[a]) + SS_MAJ(wv[a], wv[b], wv[c]);       \
	wv[d] += t1;                                            \
	wv[h] = t1 + t2;                                        \
}

uint32 ss_sha224_h0[8] = { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
						   0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
						 };

uint32 ss_sha256_h0[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
						   0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
						 };

uint64 ss_sha384_h0[8] = { 0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
						   0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
						   0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
						   0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
						 };

uint64 ss_sha512_h0[8] = { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
						   0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
						   0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
						   0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
						 };

uint32 ss_sha256_k[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
						   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
						   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
						   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
						   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
						   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
						   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
						   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
						   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
						   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
						   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
						   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
						   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
						   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
						   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
						   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
						 };

uint64 ss_sha512_k[80] = { 0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
						   0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
						   0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
						   0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
						   0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
						   0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
						   0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
						   0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
						   0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
						   0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
						   0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
						   0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
						   0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
						   0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
						   0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
						   0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
						   0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
						   0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
						   0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
						   0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
						   0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
						   0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
						   0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
						   0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
						   0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
						   0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
						   0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
						   0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
						   0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
						   0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
						   0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
						   0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
						   0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
						   0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
						   0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
						   0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
						   0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
						   0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
						   0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
						   0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
						 };

/* SHA-256 functions */

void ss_sha256_transf(ss_sha256_ctx *ctx, const unsigned char *message, unsigned int block_nb)
{
	uint32 w[64];
	uint32 wv[8];
	uint32 t1, t2;
	const unsigned char *sub_block;
	int i;

#ifndef UNROLL_LOOPS
	int j;
#endif

	for (i = 0; i < (int)block_nb; i++) {
		sub_block = message + (i << 6);

#ifndef UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			SS_PACK32(&sub_block[j << 2], &w[j]);
		}

		for (j = 16; j < 64; j++) {
			SS_SHA256_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = ctx->h[j];
		}

		for (j = 0; j < 64; j++) {
			t1 = wv[7] + SS_SHA256_F2(wv[4]) + SS_CH(wv[4], wv[5], wv[6])
				 + ss_sha256_k[j] + w[j];
			t2 = SS_SHA256_F1(wv[0]) + SS_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			ctx->h[j] += wv[j];
		}
#else
		SS_PACK32(&sub_block[0], &w[0]);
		SS_PACK32(&sub_block[4], &w[1]);
		SS_PACK32(&sub_block[8], &w[2]);
		SS_PACK32(&sub_block[12], &w[3]);
		SS_PACK32(&sub_block[16], &w[4]);
		SS_PACK32(&sub_block[20], &w[5]);
		SS_PACK32(&sub_block[24], &w[6]);
		SS_PACK32(&sub_block[28], &w[7]);
		SS_PACK32(&sub_block[32], &w[8]);
		SS_PACK32(&sub_block[36], &w[9]);
		SS_PACK32(&sub_block[40], &w[10]);
		SS_PACK32(&sub_block[44], &w[11]);
		SS_PACK32(&sub_block[48], &w[12]);
		SS_PACK32(&sub_block[52], &w[13]);
		SS_PACK32(&sub_block[56], &w[14]);
		SS_PACK32(&sub_block[60], &w[15]);

		SS_SHA256_SCR(16);
		SS_SHA256_SCR(17);
		SS_SHA256_SCR(18);
		SS_SHA256_SCR(19);
		SS_SHA256_SCR(20);
		SS_SHA256_SCR(21);
		SS_SHA256_SCR(22);
		SS_SHA256_SCR(23);
		SS_SHA256_SCR(24);
		SS_SHA256_SCR(25);
		SS_SHA256_SCR(26);
		SS_SHA256_SCR(27);
		SS_SHA256_SCR(28);
		SS_SHA256_SCR(29);
		SS_SHA256_SCR(30);
		SS_SHA256_SCR(31);
		SS_SHA256_SCR(32);
		SS_SHA256_SCR(33);
		SS_SHA256_SCR(34);
		SS_SHA256_SCR(35);
		SS_SHA256_SCR(36);
		SS_SHA256_SCR(37);
		SS_SHA256_SCR(38);
		SS_SHA256_SCR(39);
		SS_SHA256_SCR(40);
		SS_SHA256_SCR(41);
		SS_SHA256_SCR(42);
		SS_SHA256_SCR(43);
		SS_SHA256_SCR(44);
		SS_SHA256_SCR(45);
		SS_SHA256_SCR(46);
		SS_SHA256_SCR(47);
		SS_SHA256_SCR(48);
		SS_SHA256_SCR(49);
		SS_SHA256_SCR(50);
		SS_SHA256_SCR(51);
		SS_SHA256_SCR(52);
		SS_SHA256_SCR(53);
		SS_SHA256_SCR(54);
		SS_SHA256_SCR(55);
		SS_SHA256_SCR(56);
		SS_SHA256_SCR(57);
		SS_SHA256_SCR(58);
		SS_SHA256_SCR(59);
		SS_SHA256_SCR(60);
		SS_SHA256_SCR(61);
		SS_SHA256_SCR(62);
		SS_SHA256_SCR(63);

		wv[0] = ctx->h[0];
		wv[1] = ctx->h[1];
		wv[2] = ctx->h[2];
		wv[3] = ctx->h[3];
		wv[4] = ctx->h[4];
		wv[5] = ctx->h[5];
		wv[6] = ctx->h[6];
		wv[7] = ctx->h[7];

		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 0);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 1);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 2);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 3);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 4);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 5);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 6);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 7);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 8);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 9);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 10);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 11);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 12);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 13);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 14);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 15);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 16);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 17);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 18);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 19);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 20);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 21);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 22);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 23);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 24);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 25);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 26);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 27);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 28);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 29);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 30);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 31);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 32);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 33);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 34);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 35);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 36);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 37);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 38);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 39);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 40);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 41);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 42);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 43);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 44);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 45);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 46);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 47);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 48);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 49);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 50);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 51);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 52);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 53);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 54);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 55);
		SS_SHA256_EXP(0, 1, 2, 3, 4, 5, 6, 7, 56);
		SS_SHA256_EXP(7, 0, 1, 2, 3, 4, 5, 6, 57);
		SS_SHA256_EXP(6, 7, 0, 1, 2, 3, 4, 5, 58);
		SS_SHA256_EXP(5, 6, 7, 0, 1, 2, 3, 4, 59);
		SS_SHA256_EXP(4, 5, 6, 7, 0, 1, 2, 3, 60);
		SS_SHA256_EXP(3, 4, 5, 6, 7, 0, 1, 2, 61);
		SS_SHA256_EXP(2, 3, 4, 5, 6, 7, 0, 1, 62);
		SS_SHA256_EXP(1, 2, 3, 4, 5, 6, 7, 0, 63);

		ctx->h[0] += wv[0];
		ctx->h[1] += wv[1];
		ctx->h[2] += wv[2];
		ctx->h[3] += wv[3];
		ctx->h[4] += wv[4];
		ctx->h[5] += wv[5];
		ctx->h[6] += wv[6];
		ctx->h[7] += wv[7];
#endif							/* !UNROLL_LOOPS */
	}
}

void ss_sha256(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ss_sha256_ctx ctx;

	ss_sha256_init(&ctx);
	ss_sha256_update(&ctx, message, len);
	ss_sha256_final(&ctx, digest);
}

void ss_sha256_init(ss_sha256_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = ss_sha256_h0[i];
	}
#else
	ctx->h[0] = ss_sha256_h0[0];
	ctx->h[1] = ss_sha256_h0[1];
	ctx->h[2] = ss_sha256_h0[2];
	ctx->h[3] = ss_sha256_h0[3];
	ctx->h[4] = ss_sha256_h0[4];
	ctx->h[5] = ss_sha256_h0[5];
	ctx->h[6] = ss_sha256_h0[6];
	ctx->h[7] = ss_sha256_h0[7];
#endif							/* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ss_sha256_update(ss_sha256_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = SS_SHA256_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < SS_SHA256_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / SS_SHA256_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ss_sha256_transf(ctx, ctx->block, 1);
	ss_sha256_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % SS_SHA256_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 6], rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 6;
}

void ss_sha256_final(ss_sha256_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((SS_SHA256_BLOCK_SIZE - 9)
					 < (ctx->len % SS_SHA256_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	SS_UNPACK32(len_b, ctx->block + pm_len - 4);

	ss_sha256_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0; i < 8; i++) {
		SS_UNPACK32(ctx->h[i], &digest[i << 2]);
	}
#else
	SS_UNPACK32(ctx->h[0], &digest[0]);
	SS_UNPACK32(ctx->h[1], &digest[4]);
	SS_UNPACK32(ctx->h[2], &digest[8]);
	SS_UNPACK32(ctx->h[3], &digest[12]);
	SS_UNPACK32(ctx->h[4], &digest[16]);
	SS_UNPACK32(ctx->h[5], &digest[20]);
	SS_UNPACK32(ctx->h[6], &digest[24]);
	SS_UNPACK32(ctx->h[7], &digest[28]);
#endif							/* !UNROLL_LOOPS */
}

/* SHA-512 functions */

void ss_sha512_transf(ss_sha512_ctx *ctx, const unsigned char *message, unsigned int block_nb)
{
	uint64 w[80];
	uint64 wv[8];
	uint64 t1, t2;
	const unsigned char *sub_block;
	int i, j;

	for (i = 0; i < (int)block_nb; i++) {
		sub_block = message + (i << 7);

#ifndef UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			SS_PACK64(&sub_block[j << 3], &w[j]);
		}

		for (j = 16; j < 80; j++) {
			SS_SHA512_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = ctx->h[j];
		}

		for (j = 0; j < 80; j++) {
			t1 = wv[7] + SS_SHA512_F2(wv[4]) + SS_CH(wv[4], wv[5], wv[6])
				 + ss_sha512_k[j] + w[j];
			t2 = SS_SHA512_F1(wv[0]) + SS_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			ctx->h[j] += wv[j];
		}
#else
		SS_PACK64(&sub_block[0], &w[0]);
		SS_PACK64(&sub_block[8], &w[1]);
		SS_PACK64(&sub_block[16], &w[2]);
		SS_PACK64(&sub_block[24], &w[3]);
		SS_PACK64(&sub_block[32], &w[4]);
		SS_PACK64(&sub_block[40], &w[5]);
		SS_PACK64(&sub_block[48], &w[6]);
		SS_PACK64(&sub_block[56], &w[7]);
		SS_PACK64(&sub_block[64], &w[8]);
		SS_PACK64(&sub_block[72], &w[9]);
		SS_PACK64(&sub_block[80], &w[10]);
		SS_PACK64(&sub_block[88], &w[11]);
		SS_PACK64(&sub_block[96], &w[12]);
		SS_PACK64(&sub_block[104], &w[13]);
		SS_PACK64(&sub_block[112], &w[14]);
		SS_PACK64(&sub_block[120], &w[15]);

		SS_SHA512_SCR(16);
		SS_SHA512_SCR(17);
		SS_SHA512_SCR(18);
		SS_SHA512_SCR(19);
		SS_SHA512_SCR(20);
		SS_SHA512_SCR(21);
		SS_SHA512_SCR(22);
		SS_SHA512_SCR(23);
		SS_SHA512_SCR(24);
		SS_SHA512_SCR(25);
		SS_SHA512_SCR(26);
		SS_SHA512_SCR(27);
		SS_SHA512_SCR(28);
		SS_SHA512_SCR(29);
		SS_SHA512_SCR(30);
		SS_SHA512_SCR(31);
		SS_SHA512_SCR(32);
		SS_SHA512_SCR(33);
		SS_SHA512_SCR(34);
		SS_SHA512_SCR(35);
		SS_SHA512_SCR(36);
		SS_SHA512_SCR(37);
		SS_SHA512_SCR(38);
		SS_SHA512_SCR(39);
		SS_SHA512_SCR(40);
		SS_SHA512_SCR(41);
		SS_SHA512_SCR(42);
		SS_SHA512_SCR(43);
		SS_SHA512_SCR(44);
		SS_SHA512_SCR(45);
		SS_SHA512_SCR(46);
		SS_SHA512_SCR(47);
		SS_SHA512_SCR(48);
		SS_SHA512_SCR(49);
		SS_SHA512_SCR(50);
		SS_SHA512_SCR(51);
		SS_SHA512_SCR(52);
		SS_SHA512_SCR(53);
		SS_SHA512_SCR(54);
		SS_SHA512_SCR(55);
		SS_SHA512_SCR(56);
		SS_SHA512_SCR(57);
		SS_SHA512_SCR(58);
		SS_SHA512_SCR(59);
		SS_SHA512_SCR(60);
		SS_SHA512_SCR(61);
		SS_SHA512_SCR(62);
		SS_SHA512_SCR(63);
		SS_SHA512_SCR(64);
		SS_SHA512_SCR(65);
		SS_SHA512_SCR(66);
		SS_SHA512_SCR(67);
		SS_SHA512_SCR(68);
		SS_SHA512_SCR(69);
		SS_SHA512_SCR(70);
		SS_SHA512_SCR(71);
		SS_SHA512_SCR(72);
		SS_SHA512_SCR(73);
		SS_SHA512_SCR(74);
		SS_SHA512_SCR(75);
		SS_SHA512_SCR(76);
		SS_SHA512_SCR(77);
		SS_SHA512_SCR(78);
		SS_SHA512_SCR(79);

		wv[0] = ctx->h[0];
		wv[1] = ctx->h[1];
		wv[2] = ctx->h[2];
		wv[3] = ctx->h[3];
		wv[4] = ctx->h[4];
		wv[5] = ctx->h[5];
		wv[6] = ctx->h[6];
		wv[7] = ctx->h[7];

		j = 0;

		do {
			SS_SHA512_EXP(0, 1, 2, 3, 4, 5, 6, 7, j);
			j++;
			SS_SHA512_EXP(7, 0, 1, 2, 3, 4, 5, 6, j);
			j++;
			SS_SHA512_EXP(6, 7, 0, 1, 2, 3, 4, 5, j);
			j++;
			SS_SHA512_EXP(5, 6, 7, 0, 1, 2, 3, 4, j);
			j++;
			SS_SHA512_EXP(4, 5, 6, 7, 0, 1, 2, 3, j);
			j++;
			SS_SHA512_EXP(3, 4, 5, 6, 7, 0, 1, 2, j);
			j++;
			SS_SHA512_EXP(2, 3, 4, 5, 6, 7, 0, 1, j);
			j++;
			SS_SHA512_EXP(1, 2, 3, 4, 5, 6, 7, 0, j);
			j++;
		} while (j < 80);

		ctx->h[0] += wv[0];
		ctx->h[1] += wv[1];
		ctx->h[2] += wv[2];
		ctx->h[3] += wv[3];
		ctx->h[4] += wv[4];
		ctx->h[5] += wv[5];
		ctx->h[6] += wv[6];
		ctx->h[7] += wv[7];
#endif							/* !UNROLL_LOOPS */
	}
}

void ss_sha512(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ss_sha512_ctx ctx;

	ss_sha512_init(&ctx);
	ss_sha512_update(&ctx, message, len);
	ss_sha512_final(&ctx, digest);
}

void ss_sha512_init(ss_sha512_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = ss_sha512_h0[i];
	}
#else
	ctx->h[0] = ss_sha512_h0[0];
	ctx->h[1] = ss_sha512_h0[1];
	ctx->h[2] = ss_sha512_h0[2];
	ctx->h[3] = ss_sha512_h0[3];
	ctx->h[4] = ss_sha512_h0[4];
	ctx->h[5] = ss_sha512_h0[5];
	ctx->h[6] = ss_sha512_h0[6];
	ctx->h[7] = ss_sha512_h0[7];
#endif							/* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ss_sha512_update(ss_sha512_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = SS_SHA512_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < SS_SHA512_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / SS_SHA512_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ss_sha512_transf(ctx, ctx->block, 1);
	ss_sha512_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % SS_SHA512_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 7], rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 7;
}

void ss_sha512_final(ss_sha512_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = 1 + ((SS_SHA512_BLOCK_SIZE - 17)
					< (ctx->len % SS_SHA512_BLOCK_SIZE));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	SS_UNPACK32(len_b, ctx->block + pm_len - 4);

	ss_sha512_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0; i < 8; i++) {
		SS_UNPACK64(ctx->h[i], &digest[i << 3]);
	}
#else
	SS_UNPACK64(ctx->h[0], &digest[0]);
	SS_UNPACK64(ctx->h[1], &digest[8]);
	SS_UNPACK64(ctx->h[2], &digest[16]);
	SS_UNPACK64(ctx->h[3], &digest[24]);
	SS_UNPACK64(ctx->h[4], &digest[32]);
	SS_UNPACK64(ctx->h[5], &digest[40]);
	SS_UNPACK64(ctx->h[6], &digest[48]);
	SS_UNPACK64(ctx->h[7], &digest[56]);
#endif							/* !UNROLL_LOOPS */
}

/* SHA-384 functions */

void ss_sha384(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ss_sha384_ctx ctx;

	ss_sha384_init(&ctx);
	ss_sha384_update(&ctx, message, len);
	ss_sha384_final(&ctx, digest);
}

void ss_sha384_init(ss_sha384_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = ss_sha384_h0[i];
	}
#else
	ctx->h[0] = ss_sha384_h0[0];
	ctx->h[1] = ss_sha384_h0[1];
	ctx->h[2] = ss_sha384_h0[2];
	ctx->h[3] = ss_sha384_h0[3];
	ctx->h[4] = ss_sha384_h0[4];
	ctx->h[5] = ss_sha384_h0[5];
	ctx->h[6] = ss_sha384_h0[6];
	ctx->h[7] = ss_sha384_h0[7];
#endif							/* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ss_sha384_update(ss_sha384_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = SS_SHA384_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < SS_SHA384_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / SS_SHA384_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ss_sha512_transf(ctx, ctx->block, 1);
	ss_sha512_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % SS_SHA384_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 7], rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 7;
}

void ss_sha384_final(ss_sha384_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((SS_SHA384_BLOCK_SIZE - 17)
					 < (ctx->len % SS_SHA384_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	SS_UNPACK32(len_b, ctx->block + pm_len - 4);

	ss_sha512_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0; i < 6; i++) {
		SS_UNPACK64(ctx->h[i], &digest[i << 3]);
	}
#else
	SS_UNPACK64(ctx->h[0], &digest[0]);
	SS_UNPACK64(ctx->h[1], &digest[8]);
	SS_UNPACK64(ctx->h[2], &digest[16]);
	SS_UNPACK64(ctx->h[3], &digest[24]);
	SS_UNPACK64(ctx->h[4], &digest[32]);
	SS_UNPACK64(ctx->h[5], &digest[40]);
#endif							/* !UNROLL_LOOPS */
}

/* SHA-224 functions */

void ss_sha224(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ss_sha224_ctx ctx;

	ss_sha224_init(&ctx);
	ss_sha224_update(&ctx, message, len);
	ss_sha224_final(&ctx, digest);
}

void ss_sha224_init(ss_sha224_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = ss_sha224_h0[i];
	}
#else
	ctx->h[0] = ss_sha224_h0[0];
	ctx->h[1] = ss_sha224_h0[1];
	ctx->h[2] = ss_sha224_h0[2];
	ctx->h[3] = ss_sha224_h0[3];
	ctx->h[4] = ss_sha224_h0[4];
	ctx->h[5] = ss_sha224_h0[5];
	ctx->h[6] = ss_sha224_h0[6];
	ctx->h[7] = ss_sha224_h0[7];
#endif							/* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ss_sha224_update(ss_sha224_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = SS_SHA224_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < SS_SHA224_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / SS_SHA224_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ss_sha256_transf(ctx, ctx->block, 1);
	ss_sha256_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % SS_SHA224_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 6], rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 6;
}

void ss_sha224_final(ss_sha224_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((SS_SHA224_BLOCK_SIZE - 9)
					 < (ctx->len % SS_SHA224_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	SS_UNPACK32(len_b, ctx->block + pm_len - 4);

	ss_sha256_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0; i < 7; i++) {
		SS_UNPACK32(ctx->h[i], &digest[i << 2]);
	}
#else
	SS_UNPACK32(ctx->h[0], &digest[0]);
	SS_UNPACK32(ctx->h[1], &digest[4]);
	SS_UNPACK32(ctx->h[2], &digest[8]);
	SS_UNPACK32(ctx->h[3], &digest[12]);
	SS_UNPACK32(ctx->h[4], &digest[16]);
	SS_UNPACK32(ctx->h[5], &digest[20]);
	SS_UNPACK32(ctx->h[6], &digest[24]);
#endif							/* !UNROLL_LOOPS */
}
