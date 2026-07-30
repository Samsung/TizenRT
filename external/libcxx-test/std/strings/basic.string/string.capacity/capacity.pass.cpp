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

// <string>

// size_type capacity() const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

// Note: Original test used test_allocator with throw_after mechanism for
// exception safety testing. Removed due to incompatibility with libc++ 17
// (test_allocator doesn't have throw_after, and custom allocators trigger
// deprecation warnings with libc++ 17's char_traits).

int tc_libcxx_strings_string_capacity_capacity(void)
{
    {
    typedef std::string S;
    S s;
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    s.assign(10, 'a');
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    s.erase(5);
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    s.assign(100, 'a');
    TC_ASSERT_EXPR(s.capacity() >= s.size());
    s.erase(50);
    TC_ASSERT_EXPR(s.capacity() >= s.size());

    // Test that capacity grows as we push back
    while (s.size() < s.capacity())
        s.push_back('b');
    TC_ASSERT_EXPR(s.size() == s.capacity());
    }
    TC_SUCCESS_RESULT();
    return 0;
}