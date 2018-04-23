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

// <vector>

// vector<bool>()
//        noexcept(is_nothrow_default_constructible<allocator_type>::value);

// This tests a conforming extension

// UNSUPPORTED: c++98, c++03

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class T>
struct some_alloc
{
    typedef T value_type;
    some_alloc(const some_alloc&);
};

int tc_libcxx_containers_vector_bool_default_noexcept(void)
{
#if defined(_LIBCPP_VERSION)
    {
        typedef std::vector<bool> C;
        static_assert(std::is_nothrow_default_constructible<C>::value, "");
    }
    {
        typedef std::vector<bool, test_allocator<bool>> C;
        static_assert(std::is_nothrow_default_constructible<C>::value, "");
    }
#endif // _LIBCPP_VERSION
    {
        typedef std::vector<bool, other_allocator<bool>> C;
        static_assert(!std::is_nothrow_default_constructible<C>::value, "");
    }
    {
        typedef std::vector<bool, some_alloc<bool>> C;
        static_assert(!std::is_nothrow_default_constructible<C>::value, "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
