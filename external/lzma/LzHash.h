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

/* LzHash.h -- HASH functions for LZ algorithms
2015-04-12 : Igor Pavlov : Public domain */

#ifndef __LZ_HASH_H
#define __LZ_HASH_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define kHash2Size (1 << 10)
#define kHash3Size (1 << 16)
#define kHash4Size (1 << 20)

#define kFix3HashSize (kHash2Size)
#define kFix4HashSize (kHash2Size + kHash3Size)
#define kFix5HashSize (kHash2Size + kHash3Size + kHash4Size)

#define HASH2_CALC hv = cur[0] | ((UInt32)cur[1] << 8);

#define HASH3_CALC \
{ \
	UInt32 temp = p->crc[cur[0]] ^ cur[1]; \
	h2 = temp & (kHash2Size - 1); \
	hv = (temp ^ ((UInt32)cur[2] << 8)) & p->hashMask; \
}

#define HASH4_CALC { \
	UInt32 temp = p->crc[cur[0]] ^ cur[1]; \
	h2 = temp & (kHash2Size - 1); \
	temp ^= ((UInt32)cur[2] << 8); \
	h3 = temp & (kHash3Size - 1); \
	hv = (temp ^ (p->crc[cur[3]] << 5)) & p->hashMask; \
}

#define HASH5_CALC { \
	UInt32 temp = p->crc[cur[0]] ^ cur[1]; \
	h2 = temp & (kHash2Size - 1); \
	temp ^= ((UInt32)cur[2] << 8); \
	h3 = temp & (kHash3Size - 1); \
	temp ^= (p->crc[cur[3]] << 5); \
	h4 = temp & (kHash4Size - 1); \
	hv = (temp ^ (p->crc[cur[4]] << 3)) & p->hashMask; \
}

/* #define HASH_ZIP_CALC hv = ((cur[0] | ((UInt32)cur[1] << 8)) ^ p->crc[cur[2]]) & 0xFFFF; */
#define HASH_ZIP_CALC hv = ((cur[2] | ((UInt32)cur[0] << 8)) ^ p->crc[cur[1]]) & 0xFFFF;

#define MT_HASH2_CALC h2 = (p->crc[cur[0]] ^ cur[1]) & (kHash2Size - 1);

#define MT_HASH3_CALC { \
	UInt32 temp = p->crc[cur[0]] ^ cur[1]; \
	h2 = temp & (kHash2Size - 1); \
	h3 = (temp ^ ((UInt32)cur[2] << 8)) & (kHash3Size - 1); \
}

#define MT_HASH4_CALC { \
	UInt32 temp = p->crc[cur[0]] ^ cur[1]; \
	h2 = temp & (kHash2Size - 1); \
	temp ^= ((UInt32)cur[2] << 8); \
	h3 = temp & (kHash3Size - 1); \
	h4 = (temp ^ (p->crc[cur[3]] << 5)) & (kHash4Size - 1); \
}

#endif
