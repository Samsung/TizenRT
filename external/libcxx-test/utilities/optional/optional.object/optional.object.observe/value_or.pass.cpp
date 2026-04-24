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

// template <class U> constexpr T optional<T>::value_or(U&& v) &&;

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;
using std::in_place_t;
using std::in_place;

struct Y
{
    int i_;

    constexpr Y(int i) : i_(i) {}
};

struct X
{
    int i_;

    constexpr X(int i) : i_(i) {}
    constexpr X(X&& x) : i_(x.i_) {x.i_ = 0;}
    constexpr X(const Y& y) : i_(y.i_) {}
    constexpr X(Y&& y) : i_(y.i_+1) {}
    friend constexpr bool operator==(const X& x, const X& y)
        {return x.i_ == y.i_;}
};

constexpr int test()
{
    {
        optional<X> opt(in_place, 2);
        Y y(3);
        TC_ASSERT_EXPR(std::move(opt).value_or(y) == 2);
        TC_ASSERT_EXPR(*opt == 0);
    }
    {
        optional<X> opt(in_place, 2);
        TC_ASSERT_EXPR(std::move(opt).value_or(Y(3)) == 2);
        TC_ASSERT_EXPR(*opt == 0);
    }
    {
        optional<X> opt;
        Y y(3);
        TC_ASSERT_EXPR(std::move(opt).value_or(y) == 3);
        TC_ASSERT_EXPR(!opt);
    }
    {
        optional<X> opt;
        TC_ASSERT_EXPR(std::move(opt).value_or(Y(3)) == 4);
        TC_ASSERT_EXPR(!opt);
    }
    return 0;
}

int tc_utilities_optional_optional_object_optional_object_observe_value_or(void) {
    static_assert(test() == 0);

  return 0;
}
