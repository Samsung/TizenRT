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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <algorithm>

// template<class ForwardIterator>
// constexpr ForwardIterator
//   shift_right(ForwardIterator first, ForwardIterator last,
//               typename iterator_traits<ForwardIterator>::difference_type n);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

template<class T, class Iter>
constexpr bool test()
{
    int orig[] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    T work[] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};

    for (int n = 0; n <= 15; ++n) {
        for (int k = 0; k <= n+2; ++k) {
            std::copy(orig, orig+n, work);
            Iter it = std::shift_right(Iter(work), Iter(work+n), k);
            if (k < n) {
                TC_ASSERT_EXPR(it == Iter(work+k));
                TC_ASSERT_EXPR(std::equal(orig, orig+n-k, work+k, work+n));
            } else {
                TC_ASSERT_EXPR(it == Iter(work+n));
                TC_ASSERT_EXPR(std::equal(orig, orig+n, work, work+n));
            }
        }
    }

    // n == 0
    {
        T input[]          = { 0, 1, 2 };
        const T expected[] = { 0, 1, 2 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, 0);
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), it, e));
    }

    // n > 0 && n < len
    {
        T input[]          = { 0, 1, 2 };
        const T expected[] = { 0, 1 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, 1);
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), it, e));
    }
    {
        T input[]          = { 1, 2, 3, 4, 5, 6, 7, 8 };
        const T expected[] = { 1, 2, 3, 4, 5, 6 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, 2);
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), it, e));
    }
    {
        T input[]          = { 1, 2, 3, 4, 5, 6, 7, 8 };
        const T expected[] = { 1, 2 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, 6);
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), it, e));
    }

    // n == len
    {
        T input[]          = { 0, 1, 2 };
        const T expected[] = { 0, 1, 2 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, std::size(input));
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), b, e));
        TC_ASSERT_EXPR(it == e);
    }

    // n > len
    {
        T input[]          = { 0, 1, 2 };
        const T expected[] = { 0, 1, 2 };
        Iter b = Iter(std::begin(input));
        Iter e = Iter(std::end(input));
        Iter it = std::shift_right(b, e, std::size(input) + 1);
        TC_ASSERT_EXPR(std::equal(std::begin(expected), std::end(expected), b, e));
        TC_ASSERT_EXPR(it == e);
    }

    return true;
}

int tc_algorithms_alg_modifying_operations_alg_shift_shift_right(void) {
    test<int, forward_iterator<int*>>();
    test<int, bidirectional_iterator<int*>>();
    test<int, random_access_iterator<int*>>();
    test<int, int*>();
    test<MoveOnly, forward_iterator<MoveOnly*>>();
    test<MoveOnly, bidirectional_iterator<MoveOnly*>>();
    test<MoveOnly, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, MoveOnly*>();

    static_assert(test<int, forward_iterator<int*>>());
    static_assert(test<int, bidirectional_iterator<int*>>());
    static_assert(test<int, random_access_iterator<int*>>());
    static_assert(test<int, int*>());
    static_assert(test<MoveOnly, forward_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, bidirectional_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, MoveOnly*>());

    return 0;
}
