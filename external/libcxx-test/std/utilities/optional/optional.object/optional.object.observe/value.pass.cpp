//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_optional_access-missing && !no-exceptions

// <optional>

// constexpr T& optional<T>::value() &;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;
using std::bad_optional_access;

struct X
{
    X() = default;
    X(const X&) = delete;
    constexpr int test() const & {return 3;}
    int test() & {return 4;}
    constexpr int test() const && {return 5;}
    int test() && {return 6;}
};

struct Y
{
    constexpr int test() & {return 7;}
};

constexpr int
test()
{
    optional<Y> opt{Y{}};
    return opt.value().test();
}


int tc_utilities_optional_optional_object_optional_object_observe_value(void) {
    {
        optional<X> opt; ((void)opt);
        ASSERT_NOT_NOEXCEPT(opt.value());
        ASSERT_SAME_TYPE(decltype(opt.value()), X&);
    }
    {
        optional<X> opt;
        opt.emplace();
        TC_ASSERT_EXPR(opt.value().test() == 4);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        optional<X> opt;
        try
        {
            (void)opt.value();
            TC_ASSERT_EXPR(false);
        }
        catch (const bad_optional_access&)
        {
        }
    }
#endif
    static_assert(test() == 7, "");

  return 0;
}
