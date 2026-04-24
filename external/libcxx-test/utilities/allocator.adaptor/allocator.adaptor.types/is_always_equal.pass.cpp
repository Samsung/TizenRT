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
// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// typedef see below is_always_equal;

#include <scoped_allocator>
#include <type_traits>

#include "test_macros.h"
#include "allocators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_types_is_always_equal(void) {
    // sanity checks
    static_assert( (std::is_same<
            std::allocator_traits<A1<int>>::is_always_equal, std::false_type>::value
            ), "" );

    static_assert( (std::is_same<
            std::allocator_traits<min_allocator<int>>::is_always_equal, std::true_type>::value
            ), "" );

    // wrapping one allocator
    static_assert(
        (std::is_same<
            std::scoped_allocator_adaptor<A1<int>>::is_always_equal,
            std::allocator_traits<A1<int>>::is_always_equal
        >::value), "");

    // wrapping one allocator
    static_assert(
        (std::is_same<
            std::scoped_allocator_adaptor<min_allocator<int>>::is_always_equal,
            std::allocator_traits<min_allocator<int>>::is_always_equal
        >::value), "");

    // wrapping two allocators (check the values instead of the types)
    static_assert((
            std::scoped_allocator_adaptor<A1<int>, A2<int>>::is_always_equal::value ==
            ( std::allocator_traits<A1<int>>::is_always_equal::value &&
              std::allocator_traits<A2<int>>::is_always_equal::value)
        ), "");

    // wrapping two allocators (check the values instead of the types)
    static_assert((
            std::scoped_allocator_adaptor<A1<int>, min_allocator<int>>::is_always_equal::value ==
            ( std::allocator_traits<A1<int>>::is_always_equal::value &&
              std::allocator_traits<min_allocator<int>>::is_always_equal::value)
        ), "");


    // wrapping three allocators (check the values instead of the types)
    static_assert((
            std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>>::is_always_equal::value ==
            ( std::allocator_traits<A1<int>>::is_always_equal::value &&
              std::allocator_traits<A2<int>>::is_always_equal::value &&
              std::allocator_traits<A3<int>>::is_always_equal::value)
        ), "");

  return 0;
}
