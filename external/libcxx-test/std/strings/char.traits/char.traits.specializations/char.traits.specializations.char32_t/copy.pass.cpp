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

// <string>

// template<> struct char_traits<char32_t>

// static char_type* copy(char_type* s1, const char_type* s2, size_t n);

#include <string>
#include <cassert>

int main()
{
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    char32_t s1[] = {1, 2, 3};
    char32_t s2[3] = {0};
    assert(std::char_traits<char32_t>::copy(s2, s1, 3) == s2);
    assert(s2[0] == char32_t(1));
    assert(s2[1] == char32_t(2));
    assert(s2[2] == char32_t(3));
    assert(std::char_traits<char32_t>::copy(NULL, s1, 0) == NULL);
    assert(std::char_traits<char32_t>::copy(s1, NULL, 0) == s1);
#endif  // _LIBCPP_HAS_NO_UNICODE_CHARS
}
