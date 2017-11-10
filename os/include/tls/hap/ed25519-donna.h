/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
	Public domain by Andrew M. <liquidsun@gmail.com>
	Modified from the amd64-51-30k implementation by
 Daniel J. Bernstein
 Niels Duif
 Tanja Lange
 Peter Schwabe
 Bo-Yin Yang
 */

#include <tls/hap/ed25519-donna-portable.h>

#define ED25519_32BIT
#include <tls/hap/curve25519-donna-32bit.h>

#include <tls/hap/curve25519-donna-helpers.h>

/* separate uint128 check for 64 bit sse2 */
#include <tls/hap/modm-donna-32bit.h>

typedef unsigned char hash_512bits[64];

/*
	Timing safe memory compare
 */
static int ed25519_verify(const unsigned char *x, const unsigned char *y, uint32_t len)
{
	uint32_t differentbits = 0;
	while (len--) {
		differentbits |= (*x++ ^ *y++);
	}
	return (int)(1 & ((differentbits - 1) >> 8));
}

/*
 * Arithmetic on the twisted Edwards curve -x^2 + y^2 = 1 + dx^2y^2
 * with d = -(121665/121666) = 37095705934669439343138083508754565189542113879843219016388785533085940283555
 * Base point: (15112221349535400772501151409588531511454012693041857206046113283949847762202,46316835694926478169428394003475163141307993866256225615783033603165251855960);
 */

typedef struct ge25519_t {
	bignum25519 x, y, z, t;
} ge25519;

typedef struct ge25519_p1p1_t {
	bignum25519 x, y, z, t;
} ge25519_p1p1;

typedef struct ge25519_niels_t {
	bignum25519 ysubx, xaddy, t2d;
} ge25519_niels;

typedef struct ge25519_pniels_t {
	bignum25519 ysubx, xaddy, z, t2d;
} ge25519_pniels;

#include <tls/hap/ed25519-donna-basepoint-table.h>

#include <tls/hap/ed25519-donna-32bit-tables.h>

#include <tls/hap/ed25519-donna-impl-base.h>
