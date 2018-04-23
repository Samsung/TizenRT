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

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>
//   operator+(charT lhs, const basic_string<charT,traits,Allocator>& rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(charT lhs, basic_string<charT,traits,Allocator>&& rhs);

#include <string>
#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int test0(typename S::value_type lhs, const S& rhs, const S& x) {
  TC_ASSERT_EXPR(lhs + rhs == x);
    return 0;
}


int tc_libcxx_strings_string_op__char_string(void)
{
    typedef std::string S;
    TC_ASSERT_FUNC((test0('a', S(""), S("a"))));
    TC_ASSERT_FUNC((test0('a', S("12345"), S("a12345"))));
    TC_ASSERT_FUNC((test0('a', S("1234567890"), S("a1234567890"))));
    TC_ASSERT_FUNC((test0('a', S("12345678901234567890"), S("a12345678901234567890"))));
    TC_SUCCESS_RESULT();
    return 0;
}
