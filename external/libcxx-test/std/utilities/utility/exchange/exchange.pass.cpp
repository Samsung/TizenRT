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
// UNSUPPORTED: c++03, c++11
// <utility>

// exchange

// template<class T, class U=T>
//    constexpr T            // constexpr after C++17
//    exchange(T& obj, U&& new_value)
//      noexcept(is_nothrow_move_constructible<T>::value && is_nothrow_assignable<T&, U>::value);

#include <utility>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int v = 12;

    if (12 != std::exchange(v,23) || v != 23)
        return false;

    if (23 != std::exchange(v,static_cast<short>(67)) || v != 67)
        return false;

    if (67 != std::exchange<int, short>(v, {}) || v != 0)
        return false;
    return true;
    }
#endif

template<bool Move, bool Assign>
struct TestNoexcept {
    TestNoexcept() = default;
    TestNoexcept(const TestNoexcept&);
    TestNoexcept(TestNoexcept&&) noexcept(Move);
    TestNoexcept& operator=(const TestNoexcept&);
    TestNoexcept& operator=(TestNoexcept&&) noexcept(Assign);
};

constexpr bool test_noexcept() {
  {
    int x = 42;
    ASSERT_NOEXCEPT(std::exchange(x, 42));
  }
  {
    TestNoexcept<true, true> x;
    ASSERT_NOEXCEPT(std::exchange(x, std::move(x)));
    ASSERT_NOT_NOEXCEPT(std::exchange(x, x)); // copy-assignment is not noexcept
  }
  {
    TestNoexcept<true, false> x;
    ASSERT_NOT_NOEXCEPT(std::exchange(x, std::move(x)));
  }
  {
    TestNoexcept<false, true> x;
    ASSERT_NOT_NOEXCEPT(std::exchange(x, std::move(x)));
  }

  return true;
}

int tc_utilities_utility_exchange_exchange(void) {
    {
    int v = 12;
    TC_ASSERT_EXPR( std::exchange ( v, 23 ) == 12 );
    TC_ASSERT_EXPR( v == 23 );
    TC_ASSERT_EXPR( std::exchange ( v, static_cast<short>(67) ) == 23 );
    TC_ASSERT_EXPR( v == 67 );

    TC_ASSERT_EXPR((std::exchange<int, short> ( v, {} )) == 67 );
    TC_ASSERT_EXPR( v == 0 );

    }

    {
    bool b = false;
    TC_ASSERT_EXPR( !std::exchange ( b, true ));
    TC_ASSERT_EXPR( b );
    }

    {
    const std::string s1 ( "Hi Mom!" );
    const std::string s2 ( "Yo Dad!" );
    std::string s3 = s1; // Mom
    TC_ASSERT_EXPR( std::exchange ( s3, s2 ) == s1 );
    TC_ASSERT_EXPR( s3 == s2 );
    TC_ASSERT_EXPR( std::exchange ( s3, "Hi Mom!" ) == s2 );
    TC_ASSERT_EXPR( s3 == s1 );

    s3 = s2; // Dad
    TC_ASSERT_EXPR( std::exchange ( s3, {} ) == s2 );
    TC_ASSERT_EXPR( s3.size () == 0 );

    s3 = s2; // Dad
    TC_ASSERT_EXPR( std::exchange ( s3, "" ) == s2 );
    TC_ASSERT_EXPR( s3.size () == 0 );
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

    static_assert(test_noexcept(), "");

  return 0;
}
