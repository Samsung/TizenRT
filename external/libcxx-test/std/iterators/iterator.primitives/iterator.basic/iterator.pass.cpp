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

// <iterator>

// template<class Category, class T, class Distance = ptrdiff_t,
//          class Pointer = T*, class Reference = T&>
// struct iterator
// {
//   typedef T         value_type;
//   typedef Distance  difference_type;
//   typedef Pointer   pointer;
//   typedef Reference reference;
//   typedef Category  iterator_category;
// };

#include <iterator>
#include <type_traits>
#include "libcxx_tc_common.h"

struct A {};

template <class T>
static int
test2()
{
    typedef std::iterator<std::forward_iterator_tag, T> It;
    static_assert((std::is_same<typename It::value_type, T>::value), "");
    static_assert((std::is_same<typename It::difference_type, std::ptrdiff_t>::value), "");
    static_assert((std::is_same<typename It::pointer, T*>::value), "");
    static_assert((std::is_same<typename It::reference, T&>::value), "");
    static_assert((std::is_same<typename It::iterator_category, std::forward_iterator_tag>::value), "");
    return 0;
}

template <class T>
static int
test3()
{
    typedef std::iterator<std::bidirectional_iterator_tag, T, short> It;
    static_assert((std::is_same<typename It::value_type, T>::value), "");
    static_assert((std::is_same<typename It::difference_type, short>::value), "");
    static_assert((std::is_same<typename It::pointer, T*>::value), "");
    static_assert((std::is_same<typename It::reference, T&>::value), "");
    static_assert((std::is_same<typename It::iterator_category, std::bidirectional_iterator_tag>::value), "");
    return 0;
}

template <class T>
static int
test4()
{
    typedef std::iterator<std::random_access_iterator_tag, T, int, const T*> It;
    static_assert((std::is_same<typename It::value_type, T>::value), "");
    static_assert((std::is_same<typename It::difference_type, int>::value), "");
    static_assert((std::is_same<typename It::pointer, const T*>::value), "");
    static_assert((std::is_same<typename It::reference, T&>::value), "");
    static_assert((std::is_same<typename It::iterator_category, std::random_access_iterator_tag>::value), "");
    return 0;
}

template <class T>
static int
test5()
{
    typedef std::iterator<std::input_iterator_tag, T, long, const T*, const T&> It;
    static_assert((std::is_same<typename It::value_type, T>::value), "");
    static_assert((std::is_same<typename It::difference_type, long>::value), "");
    static_assert((std::is_same<typename It::pointer, const T*>::value), "");
    static_assert((std::is_same<typename It::reference, const T&>::value), "");
    static_assert((std::is_same<typename It::iterator_category, std::input_iterator_tag>::value), "");
    return 0;
}

int tc_libcxx_iterators_iterator_basic_iterator(void)
{
    TC_ASSERT_FUNC((test2<A>()));
    TC_ASSERT_FUNC((test3<A>()));
    TC_ASSERT_FUNC((test4<A>()));
    TC_ASSERT_FUNC((test5<A>()));
    TC_SUCCESS_RESULT();
    return 0;
}
