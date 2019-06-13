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

/* Compiler.h
2017-04-03 : Igor Pavlov : Public domain */

#ifndef __7Z_COMPILER_H
#define __7Z_COMPILER_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef _MSC_VER

#ifdef UNDER_CE
#define RPC_NO_WINDOWS_H
/* #pragma warning(disable : 4115) // '_RPC_ASYNC_STATE' : named type definition in parentheses */
#pragma warning(disable : 4201)	// nonstandard extension used : nameless struct/union
#pragma warning(disable : 4214)	// nonstandard extension used : bit field types other than int
#endif

#if _MSC_VER >= 1300
#pragma warning(disable : 4996)	// This function or variable may be unsafe
#else
#pragma warning(disable : 4511)	// copy constructor could not be generated
#pragma warning(disable : 4512)	// assignment operator could not be generated
#pragma warning(disable : 4514)	// unreferenced inline function has been removed
#pragma warning(disable : 4702)	// unreachable code
#pragma warning(disable : 4710)	// not inlined
#pragma warning(disable : 4714)	// function marked as __forceinline not inlined
#pragma warning(disable : 4786)	// identifier was truncated to '255' characters in the debug information
#endif

#endif

#define UNUSED_VAR(x) (void)x;

#endif							/* __7Z_COMPILER_H */
