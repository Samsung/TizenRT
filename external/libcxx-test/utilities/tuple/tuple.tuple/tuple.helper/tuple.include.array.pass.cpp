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
//
//  LWG #2212 says that tuple_size and tuple_element must be
//     available after including <utility>

#include <array>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, std::size_t N, class U, std::size_t idx>
void test()
{
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<volatile T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const volatile T> >::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, T>::type, U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, const T>::type, const U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, volatile T>::type, volatile U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, const volatile T>::type, const volatile U>::value), "");
}

int tc_utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array(void) {
    test<std::array<int, 5>, 5, int, 0>();
    test<std::array<int, 5>, 5, int, 1>();
    test<std::array<const char *, 4>, 4, const char *, 3>();
    test<std::array<volatile int, 4>, 4, volatile int, 3>();
    test<std::array<char *, 3>, 3, char *, 1>();
    test<std::array<char *, 3>, 3, char *, 2>();

  return 0;
}
