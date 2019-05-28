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

/* Alloc.h -- Memory allocation functions
2018-02-19 : Igor Pavlov : Public domain */

#ifndef __COMMON_ALLOC_H
#define __COMMON_ALLOC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "7zTypes.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
EXTERN_C_BEGIN
#ifndef _WIN32
#define MidAlloc(size) MyAlloc(size)
#define MidFree(address) MyFree(address)
#define BigAlloc(size) MyAlloc(size)
#define BigFree(address) MyFree(address)
#endif
/****************************************************************************
 * Public types
 ****************************************************************************/
typedef struct {
	ISzAlloc vt;
	ISzAllocPtr baseAlloc;
	unsigned numAlignBits;		/* ((1 << numAlignBits) >= sizeof(void *)) */
	size_t offset;				/* (offset == (k * sizeof(void *)) && offset < (1 << numAlignBits) */
} CAlignOffsetAlloc;

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern const ISzAlloc g_Alloc;
extern const ISzAlloc g_BigAlloc;
extern const ISzAlloc g_MidAlloc;
extern const ISzAlloc g_AlignedAlloc;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void *MyAlloc(size_t size);
void MyFree(void *address);

#ifdef _WIN32
void SetLargePageSize();
void *MidAlloc(size_t size);
void MidFree(void *address);
void *BigAlloc(size_t size);
void BigFree(void *address);
#endif

void AlignOffsetAlloc_CreateVTable(CAlignOffsetAlloc *p);

EXTERN_C_END
#endif
