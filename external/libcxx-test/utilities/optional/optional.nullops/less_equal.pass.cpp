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

// template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept;
// template <class T> constexpr bool operator<=(nullopt_t, const optional<T>& x) noexcept;

#include <optional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_optional_optional_nullops_less_equal(void) {
    using std::optional;
    using std::nullopt_t;
    using std::nullopt;

    {
    typedef int T;
    typedef optional<T> O;

    constexpr O o1;     // disengaged
    constexpr O o2{1};  // engaged

    static_assert (  (nullopt <= o1), "" );
    static_assert (  (nullopt <= o2), "" );
    static_assert (  (o1 <= nullopt), "" );
    static_assert ( !(o2 <= nullopt), "" );

    static_assert (noexcept(nullopt <= o1), "");
    static_assert (noexcept(o1 <= nullopt), "");
    }

  return 0;
}
