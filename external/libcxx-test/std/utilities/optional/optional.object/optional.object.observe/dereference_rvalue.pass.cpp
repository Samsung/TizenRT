//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr T&& optional<T>::operator*() &&;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X
{
    constexpr int test() const& {return 3;}
    int test() & {return 4;}
    constexpr int test() const&& {return 5;}
    int test() && {return 6;}
};

struct Y
{
    constexpr int test() && {return 7;}
};

constexpr int test_inner()
{
    optional<Y> opt{Y{}};
    return (*std::move(opt)).test();
}

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
    {
        optional<X> opt; ((void)opt);
        ASSERT_SAME_TYPE(decltype(*std::move(opt)), X&&);
        LIBCPP_STATIC_ASSERT(noexcept(*opt));
    }
    {
        optional<X> opt(X{});
        TC_ASSERT_EXPR((*std::move(opt)).test() == 6);
    }
    static_assert(test_inner() == 7, "");
    return true;
}
} // namespace

int tc_utilities_optional_optional_object_optional_object_observe_dereference_rvalue_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
