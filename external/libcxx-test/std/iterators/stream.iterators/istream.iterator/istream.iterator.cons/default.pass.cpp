//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "libcxx_tc_common.h"
#include <string>

#include "test_macros.h"

struct S { S(); }; // not constexpr

#if TEST_STD_VER > 14
template <typename T, bool isTrivial = std::is_trivially_default_constructible_v<T>>
struct test_trivial {
int operator ()() const {
    constexpr std::istream_iterator<T> it;
    (void)it;
    return 0;
    }
};

template <typename T>
struct test_trivial<T, false> {
int operator ()() const { return 0; }
};
#endif


int tc_libcxx_iterators_istream_iterator_cons_default(void)
{
    {
    typedef std::istream_iterator<int> T;
    T it;
    TC_ASSERT_EXPR(it == T());
#if TEST_STD_VER >= 11
    constexpr T it2;
    (void)it2;
#endif
    }

#if TEST_STD_VER > 14
    TC_ASSERT_FUNC((test_trivial<int>()()));
    TC_ASSERT_FUNC((test_trivial<char>()()));
    TC_ASSERT_FUNC((test_trivial<double>()()));
    TC_ASSERT_FUNC((test_trivial<S>()()));
    TC_ASSERT_FUNC((test_trivial<std::string>()()));
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
