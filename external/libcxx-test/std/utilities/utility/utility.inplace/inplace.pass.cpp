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

// <utility>

// struct in_place_t {
//   explicit in_place_t() = default;
// };
// inline constexpr in_place_t in_place{};

// template <class T>
//   struct in_place_type_t {
//     explicit in_place_type_t() = default;
//   };
// template <class T>
//   inline constexpr in_place_type_t<T> in_place_type{};

// template <size_t I>
//   struct in_place_index_t {
//     explicit in_place_index_t() = default;
//   };
// template <size_t I>
//   inline constexpr in_place_index_t<I> in_place_index{};

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

template <class Tp, class Up>
constexpr bool check_tag(Up) {
    return std::is_same<Tp, std::decay_t<Tp>>::value
        && std::is_same<Tp, Up>::value;
}

int tc_utilities_utility_utility_inplace_inplace(void) {
    // test in_place_t
    {
        using T = std::in_place_t;
        static_assert(check_tag<T>(std::in_place));
    }
    // test in_place_type_t
    {
        using T1 = std::in_place_type_t<void>;
        using T2 = std::in_place_type_t<int>;
        using T3 = std::in_place_type_t<const int>;
        static_assert(!std::is_same<T1, T2>::value && !std::is_same<T1, T3>::value);
        static_assert(!std::is_same<T2, T3>::value);
        static_assert(check_tag<T1>(std::in_place_type<void>));
        static_assert(check_tag<T2>(std::in_place_type<int>));
        static_assert(check_tag<T3>(std::in_place_type<const int>));
    }
    // test in_place_index_t
    {
        using T1 = std::in_place_index_t<0>;
        using T2 = std::in_place_index_t<1>;
        using T3 = std::in_place_index_t<static_cast<std::size_t>(-1)>;
        static_assert(!std::is_same<T1, T2>::value && !std::is_same<T1, T3>::value);
        static_assert(!std::is_same<T2, T3>::value);
        static_assert(check_tag<T1>(std::in_place_index<0>));
        static_assert(check_tag<T2>(std::in_place_index<1>));
        static_assert(check_tag<T3>(std::in_place_index<static_cast<std::size_t>(-1)>));
    }

  return 0;
}
