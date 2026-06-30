//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<RandomAccessIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter> && CopyConstructible<Compare>
//   constexpr void  // constexpr in C++20
//   nth_element(Iter first, Iter nth, Iter last, Compare comp);

#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter>
TEST_CONSTEXPR_CXX20 bool test()
{
    int orig[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    T work[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    for (int n = 0; n < 15; ++n) {
        for (int m = 0; m < n; ++m) {
            std::nth_element(Iter(work), Iter(work+m), Iter(work+n), std::greater<T>());
            TC_ASSERT_EXPR(std::is_permutation(work, work+n, orig));
            // No element to m's left is less than m.
            for (int i = 0; i < m; ++i) {
                TC_ASSERT_EXPR(!(work[i] < work[m]));
            }
            // No element to m's right is greater than m.
            for (int i = m; i < n; ++i) {
                TC_ASSERT_EXPR(!(work[i] > work[m]));
            }
            std::copy(orig, orig+15, work);
        }
    }

    {
        T input[] = {3,1,4,1,5,9,2};
        std::nth_element(Iter(input), Iter(input+4), Iter(input+7), std::greater<T>());
        TC_ASSERT_EXPR(input[4] == 2);
        TC_ASSERT_EXPR(input[5] + input[6] == 1 + 1);
    }

    {
        T input[] = {0, 1, 2, 3, 4, 5, 7, 6};
        std::nth_element(Iter(input), Iter(input + 6), Iter(input + 8), std::greater<T>());
        TC_ASSERT_EXPR(input[6] == 1);
        TC_ASSERT_EXPR(input[7] == 0);
    }

    {
        T input[] = {1, 0, 2, 3, 4, 5, 6, 7};
        std::nth_element(Iter(input), Iter(input + 1), Iter(input + 8), std::greater<T>());
        TC_ASSERT_EXPR(input[0] == 7);
        TC_ASSERT_EXPR(input[1] == 6);
    }

    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_nth_element_nth_element_comp(void) {
    test<int, random_access_iterator<int*> >();
    test<int, int*>();

#if TEST_STD_VER >= 11
    test<MoveOnly, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, MoveOnly*>();
#endif

#if TEST_STD_VER >= 20
    static_assert(test<int, random_access_iterator<int*>>());
    static_assert(test<int, int*>());
    static_assert(test<MoveOnly, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, MoveOnly*>());
#endif

    TC_SUCCESS_RESULT();


    return 0;
}
