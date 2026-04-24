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
// UNSUPPORTED: no-exceptions
// <memory>

// allocator:
// constexpr T* allocate(size_t n);

#include <memory>
#include <cassert>
#include <new>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename T>
void test_max(std::size_t count)
{
    std::allocator<T> a;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
        TEST_IGNORE_NODISCARD a.allocate(count);
        TC_ASSERT_EXPR(false);
    } catch (const std::bad_array_new_length &) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
}

template <typename T>
void test()
{
    // Bug 26812 -- allocating too large
    typedef std::allocator<T> A;
    typedef std::allocator_traits<A> AT;
    A a;
    test_max<T> (AT::max_size(a) + 1);             // just barely too large
    test_max<T> (AT::max_size(a) * 2);             // significantly too large
    test_max<T> (((std::size_t) -1) / sizeof(T) + 1);   // multiply will overflow
    test_max<T> ((std::size_t) -1);                     // way too large
}

int tc_utilities_memory_default_allocator_allocator_members_allocate_size(void) {
    test<double>();
    LIBCPP_ONLY(test<const double>());

  return 0;
}
