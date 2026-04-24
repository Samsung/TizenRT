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

constexpr int
test()
{
    optional<Y> opt{Y{}};
    return (*std::move(opt)).test();
}

int tc_utilities_optional_optional_object_optional_object_observe_dereference_rvalue(void) {
    {
        optional<X> opt; ((void)opt);
        ASSERT_SAME_TYPE(decltype(*std::move(opt)), X&&);
        LIBCPP_STATIC_ASSERT(noexcept(*opt));
        // ASSERT_NOT_NOEXCEPT(*std::move(opt));
        // FIXME: This assertion fails with GCC because it can see that
        // (A) operator*() is constexpr, and
        // (B) there is no path through the function that throws.
        // It's arguable if this is the correct behavior for the noexcept
        // operator.
        // Regardless this function should still be noexcept(false) because
        // it has a narrow contract.
    }
    {
        optional<X> opt(X{});
        TC_ASSERT_EXPR((*std::move(opt)).test() == 6);
    }
    static_assert(test() == 7, "");

    return 0;
}
