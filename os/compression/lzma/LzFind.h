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

/* LzFind.h -- Match finder for LZ algorithms
2017-06-10 : Igor Pavlov : Public domain */

#ifndef __LZ_FIND_H
#define __LZ_FIND_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "7zTypes.h"

EXTERN_C_BEGIN
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define Inline_MatchFinder_GetPointerToCurrentPos(p) ((p)->buffer)
#define Inline_MatchFinder_GetNumAvailableBytes(p) ((p)->streamPos - (p)->pos)
#define Inline_MatchFinder_IsFinishedOK(p) \
	((p)->streamEndWasReached \
	&& (p)->streamPos == (p)->pos \
	&& (!(p)->directInput || (p)->directInputRem == 0))
/****************************************************************************
 * Public Type
 ****************************************************************************/
typedef UInt32 CLzRef;

typedef struct _CMatchFinder {
	Byte *buffer;
	UInt32 pos;
	UInt32 posLimit;
	UInt32 streamPos;
	UInt32 lenLimit;

	UInt32 cyclicBufferPos;
	UInt32 cyclicBufferSize;	/* it must be = (historySize + 1) */

	Byte streamEndWasReached;
	Byte btMode;
	Byte bigHash;
	Byte directInput;

	UInt32 matchMaxLen;
	CLzRef *hash;
	CLzRef *son;
	UInt32 hashMask;
	UInt32 cutValue;

	Byte *bufferBase;
	ISeqInStream *stream;

	UInt32 blockSize;
	UInt32 keepSizeBefore;
	UInt32 keepSizeAfter;

	UInt32 numHashBytes;
	size_t directInputRem;
	UInt32 historySize;
	UInt32 fixedHashSize;
	UInt32 hashSizeSum;
	SRes result;
	UInt32 crc[256];
	size_t numRefs;

	UInt64 expectedDataSize;
} CMatchFinder;

typedef void (*Mf_Init_Func)(void *object);
typedef UInt32(*Mf_GetNumAvailableBytes_Func)(void *object);
typedef const Byte *(*Mf_GetPointerToCurrentPos_Func)(void *object);
typedef UInt32(*Mf_GetMatches_Func)(void *object, UInt32 *distances);
typedef void (*Mf_Skip_Func)(void *object, UInt32);

typedef struct _IMatchFinder {
	Mf_Init_Func Init;
	Mf_GetNumAvailableBytes_Func GetNumAvailableBytes;
	Mf_GetPointerToCurrentPos_Func GetPointerToCurrentPos;
	Mf_GetMatches_Func GetMatches;
	Mf_Skip_Func Skip;
} IMatchFinder;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int MatchFinder_NeedMove(CMatchFinder *p);
Byte *MatchFinder_GetPointerToCurrentPos(CMatchFinder *p);
void MatchFinder_MoveBlock(CMatchFinder *p);
void MatchFinder_ReadIfRequired(CMatchFinder *p);
void MatchFinder_Construct(CMatchFinder *p);

/* Conditions:
     historySize <= 3 GB
     keepAddBufferBefore + matchMaxLen + keepAddBufferAfter < 511MB
*/
int MatchFinder_Create(CMatchFinder *p, UInt32 historySize, UInt32 keepAddBufferBefore, UInt32 matchMaxLen, UInt32 keepAddBufferAfter, ISzAllocPtr alloc);
void MatchFinder_Free(CMatchFinder *p, ISzAllocPtr alloc);
void MatchFinder_Normalize3(UInt32 subValue, CLzRef *items, size_t numItems);
void MatchFinder_ReduceOffsets(CMatchFinder *p, UInt32 subValue);
UInt32 *GetMatchesSpec1(UInt32 lenLimit, UInt32 curMatch, UInt32 pos, const Byte *buffer, CLzRef *son, UInt32 _cyclicBufferPos, UInt32 _cyclicBufferSize, UInt32 _cutValue, UInt32 *distances, UInt32 maxLen);

/*
Conditions:
  Mf_GetNumAvailableBytes_Func must be called before each Mf_GetMatchLen_Func.
  Mf_GetPointerToCurrentPos_Func's result must be used only before any other function
*/

void MatchFinder_CreateVTable(CMatchFinder *p, IMatchFinder *vTable);
void MatchFinder_Init_LowHash(CMatchFinder *p);
void MatchFinder_Init_HighHash(CMatchFinder *p);
void MatchFinder_Init_3(CMatchFinder *p, int readData);
void MatchFinder_Init(CMatchFinder *p);
UInt32 Bt3Zip_MatchFinder_GetMatches(CMatchFinder *p, UInt32 *distances);
UInt32 Hc3Zip_MatchFinder_GetMatches(CMatchFinder *p, UInt32 *distances);
void Bt3Zip_MatchFinder_Skip(CMatchFinder *p, UInt32 num);
void Hc3Zip_MatchFinder_Skip(CMatchFinder *p, UInt32 num);

EXTERN_C_END
#endif
