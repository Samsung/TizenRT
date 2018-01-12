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
 * Copyright (c) 1997-2007  The Stanford SRP Authentication Project
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Redistributions in source or binary form must retain an intact copy
 * of this copyright notice.
 */

#include <tls/hap/Configuration.h>

#include <tls/hap/t_defines.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif							/* HAVE_UNISTD_H */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

#ifdef WIN32
#include <process.h>
#include <io.h>
#endif

#include <tls/hap/t_sha.h>

#ifndef NULL
#define NULL 0
#endif

#ifdef OPENSSL
#include <openssl/opensslv.h>
#include <openssl/rand.h>
#elif defined(TOMCRYPT)
#include "tomcrypt.h"
static prng_state g_rng;
static unsigned char entropy[32];
#elif defined(CRYPTOLIB)
#include "libcrypt.h"
static unsigned char crpool[64];
#elif defined(MBEDTLS)
#include <tls/ctr_drbg.h>
#else
static unsigned char randpool[SHA_DIGESTSIZE], randout[SHA_DIGESTSIZE];
static unsigned long randcnt = 0;
static unsigned int outpos = 0;
SHACTX randctxt;
#endif							/* OPENSSL */

/*
 * t_envhash - Generate a 160-bit SHA hash of the environment
 *
 * This routine performs an SHA hash of all the "name=value" pairs
 * in the environment concatenated together and dumps them in the
 * output.  While it is true that anyone on the system can see
 * your environment, someone not on the system will have a very
 * difficult time guessing it, especially since some systems play
 * tricks with variable ordering and sometimes define quirky
 * environment variables like $WINDOWID or $_.
 */
//extern "C" char** environ;
const char *environ_[2] = { "123", "123" };

static void
t_envhash(unsigned char *out)
{
	char **ptr;
	char ebuf[256];
	SHACTX ctxt;

	SHAInit(&ctxt);
	for (ptr = (char **)environ_; *ptr; ++ptr) {
		strncpy(ebuf, *ptr, 255);
		ebuf[255] = '\0';
		SHAUpdate(&ctxt, (const unsigned char *)ebuf, strlen(ebuf));
	}
	SHAFinal(out, &ctxt);
}


/*
 * Generate a high-entropy seed for the strong random number generator.
 * This uses a wide variety of quickly gathered and somewhat unpredictable
 * system information.  The 'preseed' structure is assembled from:
 *
 *   The system time in seconds
 *   The system time in microseconds
 *   The current process ID
 *   The parent process ID
 *   A hash of the user's environment
 *   A hash gathered from the file system
 *   Input from a random device, if available
 *   Timings of system interrupts
 *
 * The entire structure (60 bytes on most systems) is fed to SHA to produce
 * a 160-bit seed for the strong random number generator.  It is believed
 * that in the worst case (on a quiet system with no random device versus
 * an attacker who has access to the system already), the seed contains at
 * least about 80 bits of entropy.  Versus an attacker who does not have
 * access to the system, the entropy should be slightly over 128 bits.
 */
static char initialized = 0;

static void t_initrand(void)
{
	if (initialized) {
		return;
	}

	initialized = 1;

#if defined(MBEDTLS)
	return;
#endif

}

#define NUM_RANDOMS 12

_TYPE(void) t_stronginitrand(void)
{
#if defined(MBEDTLS)							/* t_initrand() has been improved enough to make this unnecessary */
#else
	SHACTX ctxt;
	unsigned int rawrand[NUM_RANDOMS];
	int i;

	if (!initialized) {
		t_initrand();
	}
	for (i = 0; i < NUM_RANDOMS; ++i) {
		rawrand[i] = raw_truerand();
	}
	SHAInit(&ctxt);
	SHAUpdate(&ctxt, (unsigned char *)rawrand, sizeof(rawrand));
	SHAFinal(randkey2, &ctxt);
	memset(rawrand, 0, sizeof(rawrand));
#endif
}

/*
 * The strong random number generator.  This uses a 160-bit seed
 * and uses SHA-1 in a feedback configuration to generate successive
 * outputs.  If S[0] is set to the initial seed, then:
 *
 *         S[i+1] = SHA-1(i || S[i])
 *         A[i] = SHA-1(S[i])
 *
 * where the A[i] are the output blocks starting with i=0.
 * Each cycle generates 20 bytes of new output.
 */
_TYPE(void)
t_random(unsigned char *data, unsigned size)
{
	if (!initialized) {
		t_initrand();
	}

	if (size <= 0) {			/* t_random(NULL, 0) forces seed initialization */
		return;
	}

#ifdef OPENSSL
	RAND_bytes(data, size);
#elif defined(TOMCRYPT)
	yarrow_read(data, size, &g_rng);
#elif defined(GCRYPT)
	gcry_randomize(data, size, GCRY_STRONG_RANDOM);
#elif defined(CRYPTOLIB)
	randomBytes(data, size, PSEUDO);
#elif defined(MBEDTLS)
	see_generate_random((unsigned int *) data, (unsigned int) size);
#else
	while (size > outpos) {
		if (outpos > 0) {
			memcpy(data, randout + (sizeof(randout) - outpos), outpos);
			data += outpos;
			size -= outpos;
		}

		/* Recycle */
		SHAInit(&randctxt);
		SHAUpdate(&randctxt, randpool, sizeof(randpool));
		SHAFinal(randout, &randctxt);
		SHAInit(&randctxt);
		SHAUpdate(&randctxt, (unsigned char *)&randcnt, sizeof(randcnt));
		SHAUpdate(&randctxt, randpool, sizeof(randpool));
		SHAFinal(randpool, &randctxt);
		++randcnt;
		outpos = sizeof(randout);
	}

	if (size > 0) {
		memcpy(data, randout + (sizeof(randout) - outpos), size);
		outpos -= size;
	}
#endif
}

/*
 * The interleaved session-key hash.  This separates the even and the odd
 * bytes of the input (ignoring the first byte if the input length is odd),
 * hashes them separately, and re-interleaves the two outputs to form a
 * single 320-bit value.
 */
_TYPE(unsigned char *)
t_sessionkey(unsigned char *key, unsigned char *sk, unsigned sklen)
{
	unsigned i, klen;
	unsigned char *hbuf;
	unsigned char hout[SHA_DIGESTSIZE];
	SHACTX ctxt;

	while (sklen > 0 && *sk == 0) {	/* Skip leading 0's */
		--sklen;
		++sk;
	}

	klen = sklen / 2;
	if ((hbuf = malloc(klen * sizeof(char))) == 0) {
		return 0;
	}

	for (i = 0; i < klen; ++i) {
		hbuf[i] = sk[sklen - 2 * i - 1];
	}
	SHAInit(&ctxt);
	SHAUpdate(&ctxt, hbuf, klen);
	SHAFinal(hout, &ctxt);
	for (i = 0; i < sizeof(hout); ++i) {
		key[2 * i] = hout[i];
	}

	for (i = 0; i < klen; ++i) {
		hbuf[i] = sk[sklen - 2 * i - 2];
	}
	SHAInit(&ctxt);
	SHAUpdate(&ctxt, hbuf, klen);
	SHAFinal(hout, &ctxt);
	for (i = 0; i < sizeof(hout); ++i) {
		key[2 * i + 1] = hout[i];
	}

	memset(hout, 0, sizeof(hout));
	memset(hbuf, 0, klen);
	free(hbuf);
	return key;
}

_TYPE(void)
t_mgf1(unsigned char *mask, unsigned masklen, const unsigned char *seed, unsigned seedlen)
{
	SHACTX ctxt;
	unsigned i = 0;
	unsigned pos = 0;
	unsigned char cnt[4];
	unsigned char hout[SHA_DIGESTSIZE];

	while (pos < masklen) {
		cnt[0] = (i >> 24) & 0xFF;
		cnt[1] = (i >> 16) & 0xFF;
		cnt[2] = (i >> 8) & 0xFF;
		cnt[3] = i & 0xFF;
		SHAInit(&ctxt);
		SHAUpdate(&ctxt, seed, seedlen);
		SHAUpdate(&ctxt, cnt, 4);

		if (pos + SHA_DIGESTSIZE > masklen) {
			SHAFinal(hout, &ctxt);
			memcpy(mask + pos, hout, masklen - pos);
			pos = masklen;
		} else {
			SHAFinal(mask + pos, &ctxt);
			pos += SHA_DIGESTSIZE;
		}

		++i;
	}

	memset(hout, 0, sizeof(hout));
	memset((unsigned char *)&ctxt, 0, sizeof(ctxt));
}
