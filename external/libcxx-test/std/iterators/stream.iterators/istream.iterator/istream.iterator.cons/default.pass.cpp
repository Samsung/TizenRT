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

// Usage of is_trivially_constructible is broken with these compilers.
// See https://bugs.llvm.org/show_bug.cgi?id=31016
// XFAIL: clang-3.7, apple-clang-7 && c++17

// <iterator>

// class istream_iterator

// constexpr istream_iterator();
// C++17 says: If is_trivially_default_constructible_v<T> is true, then this
//    constructor is a constexpr constructor.

#include <iterator>
#include <cassert>
#include <string>

#include "test_macros.h"

struct S { S(); }; // not constexpr

#if TEST_STD_VER > 14
template <typename T, bool isTrivial = std::is_trivially_default_constructible_v<T>>
struct test_trivial {
void operator ()() const {
    constexpr std::istream_iterator<T> it;
    (void)it;
    }
};

template <typename T>
struct test_trivial<T, false> {
void operator ()() const {}
};
#endif


int main()
{
    {
    typedef std::istream_iterator<int> T;
    T it;
    assert(it == T());
#if TEST_STD_VER >= 11
    constexpr T it2;
    (void)it2;
#endif
    }

#if TEST_STD_VER > 14
    test_trivial<int>()();
    test_trivial<char>()();
    test_trivial<double>()();
    test_trivial<S>()();
    test_trivial<std::string>()();
#endif
}
