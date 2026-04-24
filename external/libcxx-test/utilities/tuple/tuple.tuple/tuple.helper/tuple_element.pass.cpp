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
// <tuple>

// template <class... Types> class tuple;

// template <size_t I, class... Types>
// struct tuple_element<I, tuple<Types...> >
// {
//     typedef Ti type;
// };

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, std::size_t N, class U>
void test()
{
    static_assert((std::is_same<typename std::tuple_element<N, T>::type, U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<N, const T>::type, const U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<N, volatile T>::type, volatile U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<N, const volatile T>::type, const volatile U>::value), "");
#if TEST_STD_VER > 11
    static_assert((std::is_same<typename std::tuple_element_t<N, T>, U>::value), "");
    static_assert((std::is_same<typename std::tuple_element_t<N, const T>, const U>::value), "");
    static_assert((std::is_same<typename std::tuple_element_t<N, volatile T>, volatile U>::value), "");
    static_assert((std::is_same<typename std::tuple_element_t<N, const volatile T>, const volatile U>::value), "");
#endif
}

int tc_utilities_tuple_tuple_tuple_tuple_helper_tuple_element(void) {
    test<std::tuple<int>, 0, int>();
    test<std::tuple<char, int>, 0, char>();
    test<std::tuple<char, int>, 1, int>();
    test<std::tuple<int*, char, int>, 0, int*>();
    test<std::tuple<int*, char, int>, 1, char>();
    test<std::tuple<int*, char, int>, 2, int>();

  return 0;
}
