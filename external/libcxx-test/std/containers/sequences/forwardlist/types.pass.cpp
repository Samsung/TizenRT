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

// <forward_list>

// template <class T, class Allocator = allocator<T>>
// class forward_list
// {
// public:
//   typedef T         value_type;
//   typedef Allocator allocator_type;
//
//   typedef value_type&                                                reference;
//   typedef const value_type&                                          const_reference;
//   typedef typename allocator_traits<allocator_type>::pointer         pointer;
//   typedef typename allocator_traits<allocator_type>::const_pointer   const_pointer;
//   typedef typename allocator_traits<allocator_type>::size_type       size_type;
//   typedef typename allocator_traits<allocator_type>::difference_type difference_type;
//   ...
// };

#include <forward_list>
#include <type_traits>
#include "libcxx_tc_common.h"

struct A { std::forward_list<A> v; }; // incomplete type support

int tc_libcxx_containers_forwardlist_types(void)
{
    {
    typedef std::forward_list<char> C;
    static_assert((std::is_same<C::value_type, char>::value), "");
    static_assert((std::is_same<C::allocator_type, std::allocator<char> >::value), "");
    static_assert((std::is_same<C::reference, char&>::value), "");
    static_assert((std::is_same<C::const_reference, const char&>::value), "");
    static_assert((std::is_same<C::pointer, char*>::value), "");
    static_assert((std::is_same<C::const_pointer, const char*>::value), "");
    static_assert((std::is_same<C::size_type, std::size_t>::value), "");
    static_assert((std::is_same<C::difference_type, std::ptrdiff_t>::value), "");

    static_assert((std::is_signed<typename C::difference_type>::value), "");
    static_assert((std::is_unsigned<typename C::size_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::iterator>::difference_type>::value), "");
    static_assert((std::is_same<typename C::difference_type,
        typename std::iterator_traits<typename C::const_iterator>::difference_type>::value), "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
