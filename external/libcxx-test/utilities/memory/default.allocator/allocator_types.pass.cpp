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
// <memory>

// Check that the nested types of std::allocator are provided:

// template <class T>
// class allocator
// {
// public:
//     typedef size_t    size_type;
//     typedef ptrdiff_t difference_type;
//     typedef T         value_type;
//
//     typedef T*        pointer;           // deprecated in C++17, removed in C++20
//     typedef T const*  const_pointer;     // deprecated in C++17, removed in C++20
//     typedef T&        reference;         // deprecated in C++17, removed in C++20
//     typedef T const&  const_reference;   // deprecated in C++17, removed in C++20
//     template< class U > struct rebind { typedef allocator<U> other; }; // deprecated in C++17, removed in C++20
//
//     typedef true_type propagate_on_container_move_assignment;
//     typedef true_type is_always_equal;
// ...
// };

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <memory>
#include <type_traits>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct U;

template <typename T>
void test() {
    typedef std::allocator<T> Alloc;
    static_assert((std::is_same<typename Alloc::size_type, std::size_t>::value), "");
    static_assert((std::is_same<typename Alloc::difference_type, std::ptrdiff_t>::value), "");
    static_assert((std::is_same<typename Alloc::value_type, T>::value), "");
    static_assert((std::is_same<typename Alloc::propagate_on_container_move_assignment, std::true_type>::value), "");
    static_assert((std::is_same<typename Alloc::is_always_equal, std::true_type>::value), "");

#if TEST_STD_VER <= 17
    static_assert((std::is_same<typename Alloc::pointer, T*>::value), "");
    static_assert((std::is_same<typename Alloc::const_pointer, T const*>::value), "");
    static_assert((std::is_same<typename Alloc::reference, T&>::value), "");
    static_assert((std::is_same<typename Alloc::const_reference, T const&>::value), "");
    static_assert((std::is_same<typename Alloc::template rebind<U>::other, std::allocator<U> >::value), "");
#endif
}

int tc_utilities_memory_default_allocator_allocator_types(void) {
    test<char>();
#ifdef _LIBCPP_VERSION
    test<char const>(); // extension
#endif
    return 0;
}
