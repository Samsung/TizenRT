/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
//===----------------------- config_elast.h -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_CONFIG_ELAST
#define _LIBCPP_CONFIG_ELAST

#if defined(_WIN32)
#include <stdlib.h>
#else
#include <errno.h>
#endif

#if defined(ELAST)
#define _LIBCPP_ELAST ELAST
#elif defined(__TINYARA__)
// No _LIBCPP_ELAST needed on NuttX
#elif defined(_NEWLIB_VERSION)
#define _LIBCPP_ELAST __ELASTERROR
#elif defined(__Fuchsia__)
// No _LIBCPP_ELAST needed on Fuchsia
#elif defined(__linux__)
#define _LIBCPP_ELAST 4095
#elif defined(__APPLE__)
// No _LIBCPP_ELAST needed on Apple
#elif defined(__sun__)
#define _LIBCPP_ELAST ESTALE
#elif defined(_WIN32)
#define _LIBCPP_ELAST _sys_nerr
#else
// Warn here so that the person doing the libcxx port has an easier time:
#warning ELAST for this platform not yet implemented
#endif

#endif // _LIBCPP_CONFIG_ELAST
