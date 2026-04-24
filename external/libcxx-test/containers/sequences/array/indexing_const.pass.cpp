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
// <array>

// const_reference operator[](size_type) const; // constexpr in C++14
// Libc++ marks it as noexcept

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C const c = {1, 2, 3.5};
        LIBCPP_ASSERT_NOEXCEPT(c[0]);
        ASSERT_SAME_TYPE(C::const_reference, decltype(c[0]));
        C::const_reference r1 = c[0];
        TC_ASSERT_EXPR(r1 == 1);
        C::const_reference r2 = c[2];
        TC_ASSERT_EXPR(r2 == 3.5);
    }
    // Test operator[] "works" on zero sized arrays
    {
        {
            typedef double T;
            typedef std::array<T, 0> C;
            C const c = {};
            LIBCPP_ASSERT_NOEXCEPT(c[0]);
            ASSERT_SAME_TYPE(C::const_reference, decltype(c[0]));
            if (c.size() > (0)) { // always false
                C::const_reference r = c[0];
                (void)r;
            }
        }
        {
            typedef double T;
            typedef std::array<T const, 0> C;
            C const c = {};
            LIBCPP_ASSERT_NOEXCEPT(c[0]);
            ASSERT_SAME_TYPE(C::const_reference, decltype(c[0]));
            if (c.size() > (0)) { // always false
              C::const_reference r = c[0];
              (void)r;
            }
        }
    }

    return true;
}

int tc_containers_sequences_array_indexing_const(void) {
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif
  return 0;
}
