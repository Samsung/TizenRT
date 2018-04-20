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

// <utility>

// template <class T1, class T2> struct pair

// tuple_element<I, pair<T1, T2> >::type

#include <utility>
 #include "libcxx_tc_common.h"

template <class T1, class T2>
static int test()
{
    {
    typedef T1 Exp1;
    typedef T2 Exp2;
    typedef std::pair<T1, T2> P;
    static_assert((std::is_same<typename std::tuple_element<0, P>::type, Exp1>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, P>::type, Exp2>::value), "");
    }
    {
    typedef T1 const Exp1;
    typedef T2 const Exp2;
    typedef std::pair<T1, T2> const P;
    static_assert((std::is_same<typename std::tuple_element<0, P>::type, Exp1>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, P>::type, Exp2>::value), "");
    }
    {
    typedef T1 volatile Exp1;
    typedef T2 volatile Exp2;
    typedef std::pair<T1, T2> volatile P;
    static_assert((std::is_same<typename std::tuple_element<0, P>::type, Exp1>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, P>::type, Exp2>::value), "");
    }
    {
    typedef T1 const volatile Exp1;
    typedef T2 const volatile Exp2;
    typedef std::pair<T1, T2> const volatile P;
    static_assert((std::is_same<typename std::tuple_element<0, P>::type, Exp1>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, P>::type, Exp2>::value), "");
    }
    return 0;
}

int tc_libcxx_utilities_pair_astuple_tuple_element(void)
{
    TC_ASSERT_FUNC((test<int, short>()));
    TC_ASSERT_FUNC((test<int*, char>()));
    TC_SUCCESS_RESULT();
    return 0;
}
