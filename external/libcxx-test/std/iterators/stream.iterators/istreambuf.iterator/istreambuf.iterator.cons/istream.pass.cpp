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

// <iterator>

// istreambuf_iterator

// istreambuf_iterator(basic_istream<charT,traits>& s) throw();

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istreambuf_iterator_cons_istream(void)
{
    {
        std::istringstream inf;
        std::istreambuf_iterator<char> i(inf);
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<char>());
    }
    {
        std::istringstream inf("a");
        std::istreambuf_iterator<char> i(inf);
        TC_ASSERT_EXPR(i != std::istreambuf_iterator<char>());
    }
    {
        std::wistringstream inf;
        std::istreambuf_iterator<wchar_t> i(inf);
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<wchar_t>());
    }
    {
        std::wistringstream inf(L"a");
        std::istreambuf_iterator<wchar_t> i(inf);
        TC_ASSERT_EXPR(i != std::istreambuf_iterator<wchar_t>());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
