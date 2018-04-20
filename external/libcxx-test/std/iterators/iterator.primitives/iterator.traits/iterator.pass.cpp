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

// template<class Iter>
// struct iterator_traits
// {
//   typedef typename Iter::difference_type difference_type;
//   typedef typename Iter::value_type value_type;
//   typedef typename Iter::pointer pointer;
//   typedef typename Iter::reference reference;
//   typedef typename Iter::iterator_category iterator_category;
// };

#include <iterator>
#include <type_traits>
#include "libcxx_tc_common.h"

struct A {};

struct test_iterator
{
    typedef int                       difference_type;
    typedef A                         value_type;
    typedef A*                        pointer;
    typedef A&                        reference;
    typedef std::forward_iterator_tag iterator_category;
};

int tc_libcxx_iterators_iterator_traits_iterator(void)
{
    typedef std::iterator_traits<test_iterator> It;
    static_assert((std::is_same<It::difference_type, int>::value), "");
    static_assert((std::is_same<It::value_type, A>::value), "");
    static_assert((std::is_same<It::pointer, A*>::value), "");
    static_assert((std::is_same<It::iterator_category, std::forward_iterator_tag>::value), "");
    TC_SUCCESS_RESULT();
    return 0;
}
