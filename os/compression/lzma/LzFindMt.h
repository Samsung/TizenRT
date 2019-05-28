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

/* LzFindMt.h -- multithreaded Match finder for LZ algorithms
2018-07-04 : Igor Pavlov : Public domain */

#ifndef __LZ_FIND_MT_H
#define __LZ_FIND_MT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "LzFind.h"
#include "Threads.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

EXTERN_C_BEGIN
#define kMtHashBlockSize (1 << 13)
#define kMtHashNumBlocks (1 << 3)
#define kMtHashNumBlocksMask (kMtHashNumBlocks - 1)
#define kMtBtBlockSize (1 << 14)
#define kMtBtNumBlocks (1 << 6)
#define kMtBtNumBlocksMask (kMtBtNumBlocks - 1)
/* kMtCacheLineDummy must be >= size_of_CPU_cache_line */
#define kMtCacheLineDummy 128
/****************************************************************************
 * Private Types
 ****************************************************************************/
typedef struct _CMtSync {
	BoolInt wasCreated;
	BoolInt needStart;
	BoolInt exit;
	BoolInt stopWriting;

	CThread thread;
	CAutoResetEvent canStart;
	CAutoResetEvent wasStarted;
	CAutoResetEvent wasStopped;
	CSemaphore freeSemaphore;
	CSemaphore filledSemaphore;
	BoolInt csWasInitialized;
	BoolInt csWasEntered;
	CCriticalSection cs;
	UInt32 numProcessedBlocks;
} CMtSync;

typedef UInt32 *(*Mf_Mix_Matches)(void *p, UInt32 matchMinPos, UInt32 *distances);
typedef void (*Mf_GetHeads)(const Byte *buffer, UInt32 pos, UInt32 *hash, UInt32 hashMask, UInt32 *heads, UInt32 numHeads, const UInt32 *crc);
typedef struct _CMatchFinderMt {
	/* LZ */
	const Byte *pointerToCurPos;
	UInt32 *btBuf;
	UInt32 btBufPos;
	UInt32 btBufPosLimit;
	UInt32 lzPos;
	UInt32 btNumAvailBytes;

	UInt32 *hash;
	UInt32 fixedHashSize;
	UInt32 historySize;
	const UInt32 *crc;

	Mf_Mix_Matches MixMatchesFunc;

	/* LZ + BT */
	CMtSync btSync;
	Byte btDummy[kMtCacheLineDummy];

	/* BT */
	UInt32 *hashBuf;
	UInt32 hashBufPos;
	UInt32 hashBufPosLimit;
	UInt32 hashNumAvail;

	CLzRef *son;
	UInt32 matchMaxLen;
	UInt32 numHashBytes;
	UInt32 pos;
	const Byte *buffer;
	UInt32 cyclicBufferPos;
	UInt32 cyclicBufferSize;	/* it must be historySize + 1 */
	UInt32 cutValue;

	/* BT + Hash */
	CMtSync hashSync;
	/* Byte hashDummy[kMtCacheLineDummy]; */

	/* Hash */
	Mf_GetHeads GetHeadsFunc;
	CMatchFinder *MatchFinder;
} CMatchFinderMt;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

void MatchFinderMt_Construct(CMatchFinderMt *p);
void MatchFinderMt_Destruct(CMatchFinderMt *p, ISzAllocPtr alloc);
SRes MatchFinderMt_Create(CMatchFinderMt *p, UInt32 historySize, UInt32 keepAddBufferBefore, UInt32 matchMaxLen, UInt32 keepAddBufferAfter, ISzAllocPtr alloc);
void MatchFinderMt_CreateVTable(CMatchFinderMt *p, IMatchFinder *vTable);
void MatchFinderMt_ReleaseStream(CMatchFinderMt *p);

EXTERN_C_END
#endif
