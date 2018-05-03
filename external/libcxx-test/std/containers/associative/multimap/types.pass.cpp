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

// <map>

// template <class Key, class T, class Compare = less<Key>,
//           class Allocator = allocator<pair<const Key, T>>>
// class multimap
// {
// public:
//     // types:
//     typedef Key                                      key_type;
//     typedef T                                        mapped_type;
//     typedef pair<const key_type, mapped_type>        value_type;
//     typedef Compare                                  key_compare;
//     typedef Allocator                                allocator_type;
//     typedef typename allocator_type::reference       reference;
//     typedef typename allocator_type::const_reference const_reference;
//     typedef typename allocator_type::pointer         pointer;
//     typedef typename allocator_type::const_pointer   const_pointer;
//     typedef typename allocator_type::size_type       size_type;
//     typedef typename allocator_type::difference_type difference_type;
//     ...
// };

#include <map>
#include <type_traits>
#include "libcxx_tc_common.h"

int tc_libcxx_containers_multimap_types(void)
{
    {
    typedef std::multimap<int, double> C;
    static_assert((std::is_same<C::key_type, int>::value), "");
    static_assert((std::is_same<C::mapped_type, double>::value), "");
    static_assert((std::is_same<C::value_type, std::pair<const int, double> >::value), "");
    static_assert((std::is_same<C::key_compare, std::less<int> >::value), "");
    static_assert((std::is_same<C::allocator_type, std::allocator<std::pair<const int, double> > >::value), "");
    static_assert((std::is_same<C::reference, std::pair<const int, double>&>::value), "");
    static_assert((std::is_same<C::const_reference, const std::pair<const int, double>&>::value), "");
    static_assert((std::is_same<C::pointer, std::pair<const int, double>*>::value), "");
    static_assert((std::is_same<C::const_pointer, const std::pair<const int, double>*>::value), "");
    static_assert((std::is_same<C::size_type, std::size_t>::value), "");
    static_assert((std::is_same<C::difference_type, std::ptrdiff_t>::value), "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
