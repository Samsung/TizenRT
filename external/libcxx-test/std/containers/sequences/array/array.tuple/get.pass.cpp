//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// template <size_t I, class T, size_t N> T& get(array<T, N>& a);

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename ...T>
TEST_CONSTEXPR std::array<int, sizeof...(T)> tempArray(T ...args)
{
    return {args...};
}

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        std::array<double, 1> array = {3.3};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        std::get<0>(array) = 99.1;
        TC_ASSERT_EXPR(std::get<0>(array) == 99.1);
    }
    {
        std::array<double, 2> array = {3.3, 4.4};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        TC_ASSERT_EXPR(std::get<1>(array) == 4.4);
        std::get<0>(array) = 99.1;
        std::get<1>(array) = 99.2;
        TC_ASSERT_EXPR(std::get<0>(array) == 99.1);
        TC_ASSERT_EXPR(std::get<1>(array) == 99.2);
    }
    {
        std::array<double, 3> array = {3.3, 4.4, 5.5};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        TC_ASSERT_EXPR(std::get<1>(array) == 4.4);
        TC_ASSERT_EXPR(std::get<2>(array) == 5.5);
        std::get<1>(array) = 99.2;
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        TC_ASSERT_EXPR(std::get<1>(array) == 99.2);
        TC_ASSERT_EXPR(std::get<2>(array) == 5.5);
    }
    {
        std::array<double, 1> array = {3.3};
        static_assert(std::is_same<double&, decltype(std::get<0>(array))>::value, "");
    }
    {
        TC_ASSERT_EXPR(std::get<0>(tempArray(1, 2, 3)) == 1);
        TC_ASSERT_EXPR(std::get<1>(tempArray(1, 2, 3)) == 2);
        TC_ASSERT_EXPR(std::get<2>(tempArray(1, 2, 3)) == 3);
    }

    return true;
}

int tc_containers_sequences_array_array_tuple_get(void) {
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif
    return 0;
}
