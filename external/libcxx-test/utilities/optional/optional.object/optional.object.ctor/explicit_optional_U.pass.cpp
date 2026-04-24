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

// template <class U>
//   explicit optional(optional<U>&& rhs);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

template <class T, class U>
TEST_CONSTEXPR_CXX20 void test(optional<U>&& rhs, bool is_going_to_throw = false)
{
    static_assert(!(std::is_convertible<optional<U>&&, optional<T>>::value), "");
    bool rhs_engaged = static_cast<bool>(rhs);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        optional<T> lhs(std::move(rhs));
        TC_ASSERT_EXPR(is_going_to_throw == false);
        TC_ASSERT_EXPR(static_cast<bool>(lhs) == rhs_engaged);
    }
    catch (int i)
    {
        TC_ASSERT_EXPR(i == 6);
    }
#else
    if (is_going_to_throw) return;
    optional<T> lhs(std::move(rhs));
    TC_ASSERT_EXPR(static_cast<bool>(lhs) == rhs_engaged);
#endif
}

class X
{
    int i_;
public:
    constexpr explicit X(int i) : i_(i) {}
    constexpr X(X&& x) : i_(x.i_) { x.i_ = 0; }
    TEST_CONSTEXPR_CXX20 ~X() {i_ = 0;}
    friend constexpr bool operator==(const X& x, const X& y) {return x.i_ == y.i_;}
};

int count = 0;

class Z
{
public:
    explicit Z(int) { TEST_THROW(6); }
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        optional<int> rhs;
        test<X>(std::move(rhs));
    }
    {
        optional<int> rhs(3);
        test<X>(std::move(rhs));
    }

    return true;
}

int tc_utilities_optional_optional_object_optional_object_ctor_explicit_optional_U(void) {
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    test();
    {
        optional<int> rhs;
        test<Z>(std::move(rhs));
    }
    {
        optional<int> rhs(3);
        test<Z>(std::move(rhs), true);
    }

  return 0;
}
