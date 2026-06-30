//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// template <class U>
//   optional(const optional<U>& rhs);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

namespace {

template <class T, class U>
TEST_CONSTEXPR_CXX20 void
test(const optional<U>& rhs, bool is_going_to_throw = false)
{
    bool rhs_engaged = static_cast<bool>(rhs);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        optional<T> lhs = rhs;
        TC_ASSERT_EXPR(is_going_to_throw == false);
        TC_ASSERT_EXPR(static_cast<bool>(lhs) == rhs_engaged);
        if (rhs_engaged)
            TC_ASSERT_EXPR(*lhs == *rhs);
    }
    catch (int i)
    {
        TC_ASSERT_EXPR(i == 6);
    }
#else
    if (is_going_to_throw) return;
    optional<T> lhs = rhs;
    TC_ASSERT_EXPR(static_cast<bool>(lhs) == rhs_engaged);
    if (rhs_engaged)
        TC_ASSERT_EXPR(*lhs == *rhs);
#endif
}

class X
{
    int i_;
public:
    constexpr X(int i) : i_(i) {}
    constexpr X(const X& x) : i_(x.i_) {}
    TEST_CONSTEXPR_CXX20 ~X() {i_ = 0;}
    friend constexpr bool operator==(const X& x, const X& y) {return x.i_ == y.i_;}
};

class Y
{
    int i_;
public:
    constexpr Y(int i) : i_(i) {}

    friend constexpr bool operator==(const Y& x, const Y& y) {return x.i_ == y.i_;}
};

int count = 0;

class Z
{
    int i_;
public:
    Z(int i) : i_(i) {TEST_THROW(6);}

    friend bool operator==(const Z& x, const Z& y) {return x.i_ == y.i_;}
};

template<class T, class U>
constexpr bool test_all()
{
  {
    optional<U> rhs;
    test<T>(rhs);
  }
  {
    optional<U> rhs(U{3});
    test<T>(rhs);
  }
  return true;
}

} // namespace

int tc_utilities_optional_optional_object_optional_object_ctor_const_optional_U(void) {
    test_all<int, short>();
    test_all<X, int>();
    test_all<Y, int>();
#if TEST_STD_VER > 17
    static_assert(test_all<int, short>());
    static_assert(test_all<X, int>());
    static_assert(test_all<Y, int>());
#endif
    {
        typedef Z T;
        typedef int U;
        optional<U> rhs;
        test<T>(rhs);
    }
    {
        typedef Z T;
        typedef int U;
        optional<U> rhs(U{3});
        test<T>(rhs, true);
    }

    static_assert(!(std::is_constructible<optional<X>, const optional<Y>&>::value), "");

    TC_SUCCESS_RESULT();
    return 0;
}
