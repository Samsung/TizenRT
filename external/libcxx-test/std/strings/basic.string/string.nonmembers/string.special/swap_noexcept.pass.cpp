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

// UNSUPPORTED: c++98, c++03

// <string>

// static int swap(basic_string& c)
//     noexcept(!allocator_type::propagate_on_container_swap::value ||
//              __is_nothrow_swappable<allocator_type>::value);
//
//  In C++17, the standard says that swap shall have:
//     noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
//              allocator_traits<Allocator>::is_always_equal::value);

// This tests a conforming extension

#include <string>
#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

// Note: Custom allocator tests (some_alloc, some_alloc2, test_allocator) removed.
// These minimal allocators don't meet the full C++ allocator requirements expected
// by libc++ 17's basic_string (missing allocate(), proper deallocate() signature,
// rebind, etc.), causing enable_if<false> SFINAE errors in string constructors.

int tc_libcxx_strings_string_special_swap_noexcept(void)
{
    {
        typedef std::string C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
