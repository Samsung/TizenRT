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

// template<InputIterator InIter, RandomAccessIterator RAIter>
//   requires ShuffleIterator<RAIter>
//         && OutputIterator<RAIter, InIter::reference>
//         && HasLess<InIter::value_type, RAIter::value_type>
//         && LessThanComparable<RAIter::value_type>
//   constexpr RAIter  // constexpr in C++20
//   partial_sort_copy(InIter first, InIter last, RAIter result_first, RAIter result_last);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter, class OutIter>
TEST_CONSTEXPR_CXX20 bool test()
{
    int orig[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    T work[15] = {};
    for (int n = 0; n < 15; ++n) {
        for (int m = 0; m < 15; ++m) {
            OutIter it = std::partial_sort_copy(Iter(orig), Iter(orig+n), OutIter(work), OutIter(work+m));
            if (n <= m) {
                TC_ASSERT_EXPR(it == OutIter(work+n));
                TC_ASSERT_EXPR(std::is_permutation(OutIter(work), it, orig));
            } else {
                TC_ASSERT_EXPR(it == OutIter(work+m));
            }
            TC_ASSERT_EXPR(std::is_sorted(OutIter(work), it));
            if (it != OutIter(work)) {
                // At most m-1 elements in the input are less than the biggest element in the result.
                int count = 0;
                for (int i = m; i < n; ++i) {
                    count += (T(orig[i]) < *(it - 1));
                }
                TC_ASSERT_EXPR(count < m);
            }
        }
    }

    {
        int input[] = {3, 4, 2, 5, 1};
        T output[] = {0, 0, 0};
        std::partial_sort_copy(Iter(input), Iter(input + 5), OutIter(output), OutIter(output + 3));
        TC_ASSERT_EXPR(output[0] == 1);
        TC_ASSERT_EXPR(output[1] == 2);
        TC_ASSERT_EXPR(output[2] == 3);
    }
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_sort_partial_sort_copy_partial_sort_copy(void) {
    int i = 42;
    int j = 75;
    std::partial_sort_copy(&i, &i, &j, &j);  // no-op
    TC_ASSERT_EXPR(i == 42);
    TC_ASSERT_EXPR(j == 75);

    test<int, random_access_iterator<int*>, random_access_iterator<int*> >();
    test<int, random_access_iterator<int*>, int*>();
    test<int, int*, random_access_iterator<int*> >();
    test<int, int*, int*>();

#if TEST_STD_VER >= 11
    test<MoveOnly, random_access_iterator<int*>, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, random_access_iterator<int*>, MoveOnly*>();
    test<MoveOnly, int*, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, int*, MoveOnly*>();
#endif

#if TEST_STD_VER >= 20
    static_assert(test<int, random_access_iterator<int*>, random_access_iterator<int*>>());
    static_assert(test<int, int*, random_access_iterator<int*>>());
    static_assert(test<int, random_access_iterator<int*>, int*>());
    static_assert(test<int, int*, int*>());
    static_assert(test<MoveOnly, random_access_iterator<int*>, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, random_access_iterator<int*>, MoveOnly*>());
    static_assert(test<MoveOnly, int*, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, int*, MoveOnly*>());
#endif

    TC_SUCCESS_RESULT();


    return 0;
}
