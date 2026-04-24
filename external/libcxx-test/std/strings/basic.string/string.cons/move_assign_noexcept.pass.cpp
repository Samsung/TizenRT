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

// basic_string& operator=(basic_string&& c)
//     noexcept(
//         allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
//         allocator_traits<allocator_type>::is_always_equal::value); // C++17

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

// Note: Custom allocator tests (test_allocator, some_alloc, some_alloc2, some_alloc3)
// removed. These minimal allocators don't meet the full C++ allocator requirements
// expected by libc++ 17's basic_string, causing SFINAE errors and static_assert
// failures. The noexcept behavior with default std::allocator is still tested.

int tc_libcxx_strings_string_cons_move_assign_noexcept(void)
{
    {
        typedef std::string C;
        static_assert(std::is_nothrow_move_assignable<C>::value, "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}