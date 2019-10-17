/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/*****************************************************************************
 *
 * LZMA SDK is written and placed in the public domain by Igor Pavlov.
 *
 * Some code in LZMA SDK is based on public domain code from another developers:
 *   1) PPMd var.H (2001): Dmitry Shkarin
 *   2) SHA-256: Wei Dai (Crypto++ library)
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute the
 * original LZMA SDK code, either in source code form or as a compiled binary, for
 * any purpose, commercial or non-commercial, and by any means.
 *
 * LZMA SDK code is compatible with open source licenses, for example, you can
 * include it to GNU GPL or GNU LGPL code.
 *
 *****************************************************************************/

/* LzmaLib.c -- LZMA library wrapper
2015-06-13 : Igor Pavlov : Public domain */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "Alloc.h"
#include "LzmaDec.h"
#include "LzmaEnc.h"
#include "LzmaLib.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

MY_STDAPI LzmaCompress(unsigned char *dest, size_t *destLen, const unsigned char *src, size_t srcLen, unsigned char *outProps, size_t *outPropsSize, int level,	/* 0 <= level <= 9, default = 5 */
					   unsigned dictSize,	/* use (1 << N) or (3 << N). 4 KB < dictSize <= 128 MB */
					   int lc,	/* 0 <= lc <= 8, default = 3  */
					   int lp,	/* 0 <= lp <= 4, default = 0  */
					   int pb,	/* 0 <= pb <= 4, default = 2  */
					   int fb,	/* 5 <= fb <= 273, default = 32 */
					   int numThreads	/* 1 or 2, default = 2 */
					  )
{
	CLzmaEncProps props;
	LzmaEncProps_Init(&props);
	props.level = level;
	props.dictSize = dictSize;
	props.lc = lc;
	props.lp = lp;
	props.pb = pb;
	props.fb = fb;
	props.numThreads = numThreads;
	props.reduceSize = srcLen;

	return LzmaEncode(dest, destLen, src, srcLen, &props, outProps, outPropsSize, 1, NULL, &g_Alloc, &g_Alloc);
}

MY_STDAPI LzmaUncompress(unsigned char *dest, size_t *destLen, const unsigned char *src, size_t *srcLen, const unsigned char *props, size_t propsSize)
{
	ELzmaStatus status;
	return LzmaDecode(dest, destLen, src, srcLen, props, (unsigned)propsSize, LZMA_FINISH_ANY, &status, &g_Alloc);
}
