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
// <utility>

// template<class T>
//   requires MoveAssignable<T> && MoveConstructible<T>
//   void
//   swap(T& a, T& b);

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 11
struct CopyOnly {
    CopyOnly() {}
    CopyOnly(CopyOnly const&) noexcept {}
    CopyOnly& operator=(CopyOnly const&) { return *this; }
};

struct MoveOnly {
    MoveOnly() {}
    MoveOnly(MoveOnly&&) {}
    MoveOnly& operator=(MoveOnly&&) noexcept { return *this; }
};

struct NoexceptMoveOnly {
    NoexceptMoveOnly() {}
    NoexceptMoveOnly(NoexceptMoveOnly&&) noexcept {}
    NoexceptMoveOnly& operator=(NoexceptMoveOnly&&) noexcept { return *this; }
};

struct NotMoveConstructible {
    NotMoveConstructible& operator=(NotMoveConstructible&&) { return *this; }
private:
    NotMoveConstructible(NotMoveConstructible&&);
};

struct NotMoveAssignable {
    NotMoveAssignable(NotMoveAssignable&&);
private:
    NotMoveAssignable& operator=(NotMoveAssignable&&);
};

template <class Tp>
auto can_swap_test(int) -> decltype(std::swap(std::declval<Tp>(), std::declval<Tp>()));

template <class Tp>
auto can_swap_test(...) -> std::false_type;

template <class Tp>
constexpr bool can_swap() {
    return std::is_same<decltype(can_swap_test<Tp>(0)), void>::value;
}
#endif

#if TEST_STD_VER > 17
constexpr bool test_swap_constexpr()
{
    int i = 1;
    int j = 2;
    std::swap(i, j);
    return i == 2 && j == 1;
}
#endif // TEST_STD_VER > 17

int tc_utilities_utility_utility_swap_swap(void) {

    {
        int i = 1;
        int j = 2;
        std::swap(i, j);
        TC_ASSERT_EXPR(i == 2);
        TC_ASSERT_EXPR(j == 1);
    }
#if TEST_STD_VER >= 11
    {

        std::unique_ptr<int> i(new int(1));
        std::unique_ptr<int> j(new int(2));
        std::swap(i, j);
        TC_ASSERT_EXPR(*i == 2);
        TC_ASSERT_EXPR(*j == 1);

    }
    {
        // test that the swap
        static_assert(can_swap<CopyOnly&>(), "");
        static_assert(can_swap<MoveOnly&>(), "");
        static_assert(can_swap<NoexceptMoveOnly&>(), "");

        static_assert(!can_swap<NotMoveConstructible&>(), "");
        static_assert(!can_swap<NotMoveAssignable&>(), "");

        CopyOnly c;
        MoveOnly m;
        NoexceptMoveOnly nm;
        static_assert(!noexcept(std::swap(c, c)), "");
        static_assert(!noexcept(std::swap(m, m)), "");
        static_assert(noexcept(std::swap(nm, nm)), "");
    }
#endif

#if TEST_STD_VER > 17
    static_assert(test_swap_constexpr());
#endif // TEST_STD_VER > 17

  return 0;
}
