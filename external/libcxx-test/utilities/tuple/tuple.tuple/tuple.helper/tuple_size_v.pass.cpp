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
// UNSUPPORTED: c++03, c++11, c++14

// <tuple>

// template <class T> constexpr size_t tuple_size_v = tuple_size<T>::value;

#include <tuple>
#include <utility>
#include <array>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Tuple, int Expect>
void test()
{
    static_assert(std::tuple_size_v<Tuple> == Expect, "");
    static_assert(std::tuple_size_v<Tuple> == std::tuple_size<Tuple>::value, "");
    static_assert(std::tuple_size_v<Tuple const> == std::tuple_size<Tuple>::value, "");
    static_assert(std::tuple_size_v<Tuple volatile> == std::tuple_size<Tuple>::value, "");
    static_assert(std::tuple_size_v<Tuple const volatile> == std::tuple_size<Tuple>::value, "");
}

int tc_utilities_tuple_tuple_tuple_tuple_helper_tuple_size_v(void) {
    test<std::tuple<>, 0>();

    test<std::tuple<int>, 1>();
    test<std::array<int, 1>, 1>();

    test<std::tuple<int, int>, 2>();
    test<std::pair<int, int>, 2>();
    test<std::array<int, 2>, 2>();

    test<std::tuple<int, int, int>, 3>();
    test<std::array<int, 3>, 3>();

  return 0;
}
