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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#define iswalpha sun_iswalpha
#define iswupper sun_iswupper
#define iswlower sun_iswlower
#define iswdigit sun_iswdigit
#define iswxdigit sun_iswxdigit
#define iswalnum sun_iswalnum
#define iswspace sun_iswspace
#define iswpunct sun_iswpunct
#define iswprint sun_iswprint
#define iswgraph sun_iswgraph
#define iswcntrl sun_iswcntrl
#define iswctype sun_iswctype
#define towlower sun_towlower
#define towupper sun_towupper
#define wcswcs sun_wcswcs
#define wcswidth sun_wcswidth
#define wcwidth sun_wcwidth
#define wctype sun_wctype
#define _WCHAR_T 1
#include_next "wchar.h"
#undef iswalpha
#undef iswupper
#undef iswlower
#undef iswdigit
#undef iswxdigit
#undef iswalnum
#undef iswspace
#undef iswpunct
#undef iswprint
#undef iswgraph
#undef iswcntrl
#undef iswctype
#undef towlower
#undef towupper
#undef wcswcs
#undef wcswidth
#undef wcwidth
#undef wctype
