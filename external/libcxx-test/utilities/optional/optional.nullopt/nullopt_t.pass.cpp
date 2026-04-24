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
// <optional>

// struct nullopt_t{see below};
// inline constexpr nullopt_t nullopt(unspecified);

// [optional.nullopt]/2:
//   Type nullopt_t shall not have a default constructor or an initializer-list
//   constructor, and shall not be an aggregate.

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::nullopt_t;
using std::nullopt;

constexpr bool test()
{
    nullopt_t foo{nullopt};
    (void)foo;
    return true;
}

int tc_utilities_optional_optional_nullopt_nullopt_t(void) {
    static_assert(std::is_empty_v<nullopt_t>);
    static_assert(!std::is_default_constructible_v<nullopt_t>);

    static_assert(std::is_same_v<const nullopt_t, decltype(nullopt)>);
    static_assert(test());

  return 0;
}
