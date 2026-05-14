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
// <utility>

// template<class T, T N>
//   using make_integer_sequence = integer_sequence<T, 0, 1, ..., N-1>;

// UNSUPPORTED: c++03, c++11

#include <utility>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_intseq_intseq_make_make_integer_seq(void) {
    static_assert(std::is_same<std::make_integer_sequence<int, 0>, std::integer_sequence<int>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<int, 1>, std::integer_sequence<int, 0>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<int, 2>, std::integer_sequence<int, 0, 1>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<int, 3>, std::integer_sequence<int, 0, 1, 2>>::value, "");

    static_assert(std::is_same<std::make_integer_sequence<unsigned long long, 0>, std::integer_sequence<unsigned long long>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<unsigned long long, 1>, std::integer_sequence<unsigned long long, 0>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<unsigned long long, 2>, std::integer_sequence<unsigned long long, 0, 1>>::value, "");
    static_assert(std::is_same<std::make_integer_sequence<unsigned long long, 3>, std::integer_sequence<unsigned long long, 0, 1, 2>>::value, "");

  return 0;
}
