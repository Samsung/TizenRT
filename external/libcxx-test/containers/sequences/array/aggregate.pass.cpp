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
// Make sure std::array is an aggregate type.
// We can only check this in C++17 and above, because we don't have the
// trait before that.
// UNSUPPORTED: c++03, c++11, c++14

#include <array>
#include <type_traits>
#include "libcxx_tc_common.h"

template <typename T>
void check_aggregate()
{
    static_assert(std::is_aggregate<std::array<T, 0> >::value, "");
    static_assert(std::is_aggregate<std::array<T, 1> >::value, "");
    static_assert(std::is_aggregate<std::array<T, 2> >::value, "");
    static_assert(std::is_aggregate<std::array<T, 3> >::value, "");
    static_assert(std::is_aggregate<std::array<T, 4> >::value, "");
}

struct Empty { };
struct Trivial { int i; int j; };
struct NonTrivial {
    int i; int j;
    NonTrivial(NonTrivial const&) { }
};

int tc_containers_sequences_array_aggregate(void) {
    check_aggregate<char>();
    check_aggregate<int>();
    check_aggregate<long>();
    check_aggregate<float>();
    check_aggregate<double>();
    check_aggregate<long double>();
    check_aggregate<Empty>();
    check_aggregate<Trivial>();
    check_aggregate<NonTrivial>();

    return 0;
}
