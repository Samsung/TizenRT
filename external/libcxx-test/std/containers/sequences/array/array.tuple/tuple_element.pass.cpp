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
// <array>

// tuple_element<I, array<T, N> >::type

#include <array>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test()
{
    {
    typedef T Exp;
    typedef std::array<T, 3> C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T const Exp;
    typedef std::array<T, 3> const C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T volatile Exp;
    typedef std::array<T, 3> volatile C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T const volatile Exp;
    typedef std::array<T, 3> const volatile C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
}

int tc_containers_sequences_array_array_tuple_tuple_element(void) {
    test<double>();
    test<int>();

  return 0;
}
