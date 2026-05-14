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
