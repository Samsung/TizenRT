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
// <list>

// template <class T, class Alloc = allocator<T> >
// class list
// {
// public:
//
//     // types:
//     typedef T value_type;
//     typedef Alloc allocator_type;
//     typedef typename allocator_type::reference reference;
//     typedef typename allocator_type::const_reference const_reference;
//     typedef typename allocator_type::pointer pointer;
//     typedef typename allocator_type::const_pointer const_pointer;

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_ENABLE_CXX20_REMOVED_ALLOCATOR_MEMBERS
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <list>
#include <type_traits>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

struct A { std::list<A> v; }; // incomplete type support

int tc_containers_sequences_list_types(void) {
    {
    typedef std::list<int> C;
    static_assert((std::is_same<C::value_type, int>::value), "");
    static_assert((std::is_same<C::allocator_type, std::allocator<int> >::value), "");
    static_assert((std::is_same<C::reference, std::allocator<int>::reference>::value), "");
    static_assert((std::is_same<C::const_reference, std::allocator<int>::const_reference>::value), "");
    static_assert((std::is_same<C::pointer, std::allocator<int>::pointer>::value), "");
    static_assert((std::is_same<C::const_pointer, std::allocator<int>::const_pointer>::value), "");

    static_assert((std::is_signed<typename C::difference_type>::value), "");
    static_assert((std::is_unsigned<typename C::size_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::iterator>::difference_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::const_iterator>::difference_type>::value), "");
    }

#if TEST_STD_VER >= 11
    {
    typedef std::list<int, min_allocator<int>> C;
    static_assert((std::is_same<C::value_type, int>::value), "");
    static_assert((std::is_same<C::allocator_type, min_allocator<int> >::value), "");
    static_assert((std::is_same<C::reference, int&>::value), "");
    static_assert((std::is_same<C::const_reference, const int&>::value), "");
    static_assert((std::is_same<C::pointer, min_pointer<int>>::value), "");
    static_assert((std::is_same<C::const_pointer, min_pointer<const int>>::value), "");

    static_assert((std::is_signed<typename C::difference_type>::value), "");
    static_assert((std::is_unsigned<typename C::size_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::iterator>::difference_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::const_iterator>::difference_type>::value), "");
    }
#endif

  return 0;
}
