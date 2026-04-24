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

// template <size_t I, class T, size_t N> const T&& get(const array<T, N>&& a);

// UNSUPPORTED: c++03

#include <array>
#include <memory>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_array_array_tuple_get_const_rv(void) {

    {
        typedef std::unique_ptr<double> T;
        typedef std::array<T, 1> C;
        const C c = {std::unique_ptr<double>(new double(3.5))};
        static_assert(std::is_same<const T&&, decltype(std::get<0>(std::move(c)))>::value, "");
        static_assert(noexcept(std::get<0>(std::move(c))), "");
        const T&& t = std::get<0>(std::move(c));
        TC_ASSERT_EXPR(*t == 3.5);
    }

#if TEST_STD_VER >= 14
    {
        typedef double T;
        typedef std::array<T, 3> C;
        constexpr const C c = {1, 2, 3.5};
        static_assert(std::get<0>(std::move(c)) == 1, "");
        static_assert(std::get<1>(std::move(c)) == 2, "");
        static_assert(std::get<2>(std::move(c)) == 3.5, "");
    }
#endif

    return 0;
}
