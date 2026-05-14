//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <optional>

// void reset() noexcept;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X
{
    static bool dtor_called;
    X() = default;
    X(const X&) = default;
    X& operator=(const X&) = default;
    ~X() {dtor_called = true;}
};

bool X::dtor_called = false;

constexpr bool check_reset()
{
    {
        optional<int> opt;
        static_assert(noexcept(opt.reset()) == true, "");
        opt.reset();
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    {
        optional<int> opt(3);
        opt.reset();
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    return true;
}

int tc_utilities_optional_optional_object_optional_object_mod_reset(void) {
    check_reset();
#if TEST_STD_VER >= 20
    static_assert(check_reset());
#endif
    {
        optional<X> opt;
        static_assert(noexcept(opt.reset()) == true, "");
        TC_ASSERT_EXPR(X::dtor_called == false);
        opt.reset();
        TC_ASSERT_EXPR(X::dtor_called == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    {
        optional<X> opt(X{});
        X::dtor_called = false;
        opt.reset();
        TC_ASSERT_EXPR(X::dtor_called == true);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
        X::dtor_called = false;
    }

  return 0;
}
