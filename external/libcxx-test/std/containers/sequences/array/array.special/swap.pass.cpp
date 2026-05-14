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

// template <class T, size_t N> void swap(array<T,N>& x, array<T,N>& y);

#include <array>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NonSwappable {
    TEST_CONSTEXPR NonSwappable() { }
private:
    NonSwappable(NonSwappable const&);
    NonSwappable& operator=(NonSwappable const&);
};

template <class Tp>
decltype(swap(std::declval<Tp>(), std::declval<Tp>()))
can_swap_imp(int);

template <class Tp>
std::false_type can_swap_imp(...);

template <class Tp>
struct can_swap : std::is_same<decltype(can_swap_imp<Tp>(0)), void> { };

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c1 = {1, 2, 3.5};
        C c2 = {4, 5, 6.5};
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.size() == 3);
        TC_ASSERT_EXPR(c1[0] == 4);
        TC_ASSERT_EXPR(c1[1] == 5);
        TC_ASSERT_EXPR(c1[2] == 6.5);
        TC_ASSERT_EXPR(c2.size() == 3);
        TC_ASSERT_EXPR(c2[0] == 1);
        TC_ASSERT_EXPR(c2[1] == 2);
        TC_ASSERT_EXPR(c2[2] == 3.5);
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c1 = {};
        C c2 = {};
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
    }
    {
        typedef NonSwappable T;
        typedef std::array<T, 0> C0;
        static_assert(can_swap<C0&>::value, "");
        C0 l = {};
        C0 r = {};
        swap(l, r);
#if TEST_STD_VER >= 11
        static_assert(noexcept(swap(l, r)), "");
#endif
    }
#if TEST_STD_VER >= 11
    {
        // NonSwappable is still considered swappable in C++03 because there
        // is no access control SFINAE.
        typedef NonSwappable T;
        typedef std::array<T, 42> C1;
        static_assert(!can_swap<C1&>::value, "");
    }
#endif

    return true;
}

int tc_containers_sequences_array_array_special_swap(void) {
    tests();
#if TEST_STD_VER >= 20
    static_assert(tests(), "");
#endif
    return 0;
}
