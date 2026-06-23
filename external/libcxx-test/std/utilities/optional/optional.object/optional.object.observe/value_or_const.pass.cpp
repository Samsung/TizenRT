//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class U> constexpr T optional<T>::value_or(U&& v) const&;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct Y
{
    int i_;

    constexpr Y(int i) : i_(i) {}
};

struct X
{
    int i_;

    constexpr X(int i) : i_(i) {}
    constexpr X(const Y& y) : i_(y.i_) {}
    constexpr X(Y&& y) : i_(y.i_+1) {}
    friend constexpr bool operator==(const X& x, const X& y)
        {return x.i_ == y.i_;}
};

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
    {
        constexpr optional<X> opt(2);
        constexpr Y y(3);
        static_assert(opt.value_or(y) == 2, "");
    }
    {
        constexpr optional<X> opt(2);
        static_assert(opt.value_or(Y(3)) == 2, "");
    }
    {
        constexpr optional<X> opt;
        constexpr Y y(3);
        static_assert(opt.value_or(y) == 3, "");
    }
    {
        constexpr optional<X> opt;
        static_assert(opt.value_or(Y(3)) == 4, "");
    }
    {
        const optional<X> opt(2);
        const Y y(3);
        TC_ASSERT_EXPR(opt.value_or(y) == 2);
    }
    {
        const optional<X> opt(2);
        TC_ASSERT_EXPR(opt.value_or(Y(3)) == 2);
    }
    {
        const optional<X> opt;
        const Y y(3);
        TC_ASSERT_EXPR(opt.value_or(y) == 3);
    }
    {
        const optional<X> opt;
        TC_ASSERT_EXPR(opt.value_or(Y(3)) == 4);
    }
    return true;
}
} // namespace

int tc_utilities_optional_optional_object_optional_object_observe_value_or_const_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
