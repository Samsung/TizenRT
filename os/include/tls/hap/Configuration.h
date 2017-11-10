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
#pragma once
//
//  Configuration.h
//  Workbench
//
//  Created by Wai Man Chan on 10/27/14.
//
//

#define MBEDTLS 1

#ifdef MBEDTLS
#include <tls/sha512.h>
#else
#include <openssl/sha.h>
#endif

#include <stdint.h>
#include <unistd.h>

#ifdef MBEDTLS
typedef mbedtls_sha512_context SHACTX;
#define SHAInit(fmt, ...) \
   do { mbedtls_sha512_init(fmt); mbedtls_sha512_starts(fmt, 0 ); } while(0)
#define SHAUpdate mbedtls_sha512_update
#define SHAFinal(a, b) mbedtls_sha512_finish(b, a)
#define SHA512(a, b, c) mbedtls_sha512(a, b, c, 0); // 0=SHA512, 1=SHA384

#else
typedef SHA512_CTX SHACTX;
#define SHAInit SHA512_Init
#define SHAUpdate SHA512_Update
#define SHAFinal SHA512_Final

#endif

#define SHA_DIGESTSIZE 64
#define SHA_BlockSize 128

