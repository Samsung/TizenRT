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

// <vector>

// static int swap(vector& c)
//     noexcept(!allocator_type::propagate_on_container_swap::value ||
//              __is_nothrow_swappable<allocator_type>::value);
//
//  In C++17, the standard says that swap shall have:
//     noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
//              allocator_traits<Allocator>::is_always_equal::value);

// This tests a conforming extension

#include <vector>
#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class T>
struct some_alloc
{
    typedef T value_type;

    some_alloc() {}
    some_alloc(const some_alloc&);
    void deallocate(void*, unsigned) {}

    typedef std::true_type propagate_on_container_swap;
};

template <class T>
struct some_alloc2
{
    typedef T value_type;

    some_alloc2() {}
    some_alloc2(const some_alloc2&);
    void deallocate(void*, unsigned) {}

    typedef std::false_type propagate_on_container_swap;
    typedef std::true_type is_always_equal;
};

int tc_libcxx_containers_vector_bool_swap_noexcept(void)
{
#if defined(_LIBCPP_VERSION)
    {
        typedef std::vector<bool> C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }
    {
        typedef std::vector<bool, test_allocator<bool>> C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }
    {
        typedef std::vector<bool, other_allocator<bool>> C;
        static_assert(noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }
#endif // _LIBCPP_VERSION
    {
#if TEST_STD_VER >= 14
#if defined(_LIBCPP_VERSION)
    //  In C++14, if POCS is set, swapping the allocator is required not to throw
        typedef std::vector<bool, some_alloc<bool>> C;
        static_assert( noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
#endif // _LIBCPP_VERSION
#else
        typedef std::vector<bool, some_alloc<bool>> C;
        static_assert(!noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
#endif
    }
#if TEST_STD_VER >= 14
#if defined(_LIBCPP_VERSION)
    {
        typedef std::vector<bool, some_alloc2<bool>> C;
    //  if the allocators are always equal, then the swap can be noexcept
        static_assert( noexcept(swap(std::declval<C&>(), std::declval<C&>())), "");
    }
#endif // _LIBCPP_VERSION
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
