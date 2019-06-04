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

/* 7zFile.h -- File IO
2017-04-03 : Igor Pavlov : Public domain */

#ifndef __7Z_FILE_H
#define __7Z_FILE_H

#ifdef _WIN32
#define USE_WINDOWS_FILE
#endif

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifdef USE_WINDOWS_FILE
#include <windows.h>
#else
#include <stdio.h>
#endif

#include "7zTypes.h"

EXTERN_C_BEGIN
/****************************************************************************
 * Private Types
 ****************************************************************************/
/* ---------- File ---------- */
typedef struct {
#ifdef USE_WINDOWS_FILE
	HANDLE handle;
#else
	FILE *file;
#endif
} CSzFile;

/* ---------- FileInStream ---------- */

typedef struct {
	ISeqInStream vt;
	CSzFile file;
} CFileSeqInStream;

typedef struct {
	ISeekInStream vt;
	CSzFile file;
} CFileInStream;

typedef struct {
	ISeqOutStream vt;
	CSzFile file;
} CFileOutStream;

/****************************************************************************
 * Private Function
 ****************************************************************************/

void File_Construct(CSzFile *p);
#if !defined(UNDER_CE) || !defined(USE_WINDOWS_FILE)
WRes InFile_Open(CSzFile *p, const char *name);
WRes OutFile_Open(CSzFile *p, const char *name);
#endif
#ifdef USE_WINDOWS_FILE
WRes InFile_OpenW(CSzFile *p, const WCHAR *name);
WRes OutFile_OpenW(CSzFile *p, const WCHAR *name);
#endif
WRes File_Close(CSzFile *p);

/* reads max(*size, remain file's size) bytes */
WRes File_Read(CSzFile *p, void *data, size_t *size);

/* writes *size bytes */
WRes File_Write(CSzFile *p, const void *data, size_t *size);

WRes File_Seek(CSzFile *p, Int64 *pos, ESzSeek origin);
WRes File_GetLength(CSzFile *p, UInt64 *length);
void FileSeqInStream_CreateVTable(CFileSeqInStream *p);
void FileInStream_CreateVTable(CFileInStream *p);
void FileOutStream_CreateVTable(CFileOutStream *p);

EXTERN_C_END
#endif
