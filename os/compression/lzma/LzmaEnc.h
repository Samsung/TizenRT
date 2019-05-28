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

/*  LzmaEnc.h -- LZMA Encoder
2017-07-27 : Igor Pavlov : Public domain */

#ifndef __LZMA_ENC_H
#define __LZMA_ENC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "7zTypes.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

EXTERN_C_BEGIN
#define LZMA_PROPS_SIZE 5
/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct _CLzmaEncProps {
	int level;					/* 0 <= level <= 9 */
	UInt32 dictSize;			/* (1 << 12) <= dictSize <= (1 << 27) for 32-bit version
								   (1 << 12) <= dictSize <= (3 << 29) for 64-bit version
								   default = (1 << 24) */
	int lc;						/* 0 <= lc <= 8, default = 3 */
	int lp;						/* 0 <= lp <= 4, default = 0 */
	int pb;						/* 0 <= pb <= 4, default = 2 */
	int algo;					/* 0 - fast, 1 - normal, default = 1 */
	int fb;						/* 5 <= fb <= 273, default = 32 */
	int btMode;					/* 0 - hashChain Mode, 1 - binTree mode - normal, default = 1 */
	int numHashBytes;			/* 2, 3 or 4, default = 4 */
	UInt32 mc;					/* 1 <= mc <= (1 << 30), default = 32 */
	unsigned writeEndMark;		/* 0 - do not write EOPM, 1 - write EOPM, default = 0 */
	int numThreads;				/* 1 or 2, default = 2 */

	UInt64 reduceSize;			/* estimated size of data that will be compressed. default = (UInt64)(Int64)-1.
								   Encoder uses this value to reduce dictionary size */
} CLzmaEncProps;

typedef void *CLzmaEncHandle;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void LzmaEncProps_Init(CLzmaEncProps *p);
void LzmaEncProps_Normalize(CLzmaEncProps *p);
UInt32 LzmaEncProps_GetDictSize(const CLzmaEncProps *props2);

/* ---------- CLzmaEncHandle Interface ---------- */

/* LzmaEnc* functions can return the following exit codes:
SRes:
  SZ_OK           - OK
  SZ_ERROR_MEM    - Memory allocation error
  SZ_ERROR_PARAM  - Incorrect paramater in props
  SZ_ERROR_WRITE  - ISeqOutStream write callback error
  SZ_ERROR_OUTPUT_EOF - output buffer overflow - version with (Byte *) output
  SZ_ERROR_PROGRESS - some break from progress callback
  SZ_ERROR_THREAD - error in multithreading functions (only for Mt version)
*/

CLzmaEncHandle LzmaEnc_Create(ISzAllocPtr alloc);
void LzmaEnc_Destroy(CLzmaEncHandle p, ISzAllocPtr alloc, ISzAllocPtr allocBig);
SRes LzmaEnc_SetProps(CLzmaEncHandle p, const CLzmaEncProps *props);
void LzmaEnc_SetDataSize(CLzmaEncHandle p, UInt64 expectedDataSiize);
SRes LzmaEnc_WriteProperties(CLzmaEncHandle p, Byte *properties, SizeT *size);
unsigned LzmaEnc_IsWriteEndMark(CLzmaEncHandle p);
SRes LzmaEnc_Encode(CLzmaEncHandle p, ISeqOutStream *outStream, ISeqInStream *inStream, ICompressProgress *progress, ISzAllocPtr alloc, ISzAllocPtr allocBig);
SRes LzmaEnc_MemEncode(CLzmaEncHandle p, Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen, int writeEndMark, ICompressProgress *progress, ISzAllocPtr alloc, ISzAllocPtr allocBig);

/* ---------- One Call Interface ---------- */
SRes LzmaEncode(Byte *dest, SizeT *destLen, const Byte *src, SizeT srcLen, const CLzmaEncProps *props, Byte *propsEncoded, SizeT *propsSize, int writeEndMark, ICompressProgress *progress, ISzAllocPtr alloc, ISzAllocPtr allocBig);

EXTERN_C_END
#endif
