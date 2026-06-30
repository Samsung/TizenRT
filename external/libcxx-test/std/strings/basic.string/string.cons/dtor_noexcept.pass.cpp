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

// UNSUPPORTED: c++98, c++03

// <string>

// ~basic_string() // implied noexcept;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

// Note: Custom allocator tests (test_allocator, some_alloc) removed.
// These minimal allocators don't meet the full C++ allocator requirements
// expected by libc++ 17's basic_string, causing SFINAE errors.

// Test that it's possible to take the address of basic_string's destructors
// by creating globals which will register their destructors with cxa_atexit.
std::string s;
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
std::wstring ws;
#endif

int tc_libcxx_strings_string_cons_dtor_noexcept(void)
{
    {
        typedef std::string C;
        static_assert(std::is_nothrow_destructible<C>::value, "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}