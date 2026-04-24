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
 ****************************************************************************///
//
// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr void  // constexpr in C++20
//   nth_element(Iter first, Iter nth, Iter last);

#include <algorithm>
#include <cassert>

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
            std::nth_element(Iter(work), Iter(work+m), Iter(work+n));
            TC_ASSERT_EXPR(std::is_permutation(work, work+n, orig));
            // No element to m's left is greater than m.
            for (int i = 0; i < m; ++i) {
                TC_ASSERT_EXPR(!(work[i] > work[m]));
            }
            // No element to m's right is less than m.
            for (int i = m; i < n; ++i) {
                TC_ASSERT_EXPR(!(work[i] < work[m]));
            }
            std::copy(orig, orig+15, work);
        }
    }

    {
        T input[] = {3,1,4,1,5,9,2};
        std::nth_element(Iter(input), Iter(input+4), Iter(input+7));
        TC_ASSERT_EXPR(input[4] == 4);
        TC_ASSERT_EXPR(input[5] + input[6] == 5 + 9);
    }

    {
        T input[] = {0, 1, 2, 3, 4, 5, 7, 6};
        std::nth_element(Iter(input), Iter(input + 6), Iter(input + 8));
        TC_ASSERT_EXPR(input[6] == 6);
        TC_ASSERT_EXPR(input[7] == 7);
    }

    {
        T input[] = {1, 0, 2, 3, 4, 5, 6, 7};
        std::nth_element(Iter(input), Iter(input + 1), Iter(input + 8));
        TC_ASSERT_EXPR(input[0] == 0);
        TC_ASSERT_EXPR(input[1] == 1);
    }

    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_nth_element_nth_element(void) {
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
