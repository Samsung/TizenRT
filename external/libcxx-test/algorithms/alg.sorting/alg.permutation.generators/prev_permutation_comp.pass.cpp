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
// <algorithm>

// template<BidirectionalIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Compare>
//   constexpr bool  // constexpr in C++20
//   prev_permutation(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"

#include <cstdio>
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 int factorial(int x)
{
    int r = 1;
    for (; x; --x)
        r *= x;
    return r;
}

template <class Iter>
TEST_CONSTEXPR_CXX20 bool
test()
{
    typedef std::greater<int> C;
    int ia[] = {1, 2, 3, 4, 5, 6};
    const int sa = sizeof(ia)/sizeof(ia[0]);
    int prev[sa];
    for (int e = 0; e <= sa; ++e)
    {
        int count = 0;
        bool x;
        do
        {
            std::copy(ia, ia+e, prev);
            x = std::prev_permutation(Iter(ia), Iter(ia+e), C());
            if (e > 1)
            {
                if (x)
                    TC_ASSERT_EXPR(std::lexicographical_compare(ia, ia+e, prev, prev+e, C()));
                else
                    TC_ASSERT_EXPR(std::lexicographical_compare(prev, prev+e, ia, ia+e, C()));
            }
            ++count;
        } while (x);
        TC_ASSERT_EXPR(count == factorial(e));
    }
    return true;
}

int tc_algorithms_alg_sorting_alg_permutation_generators_prev_permutation_comp(void) {
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 20
    static_assert(test<bidirectional_iterator<int*>>());
    static_assert(test<random_access_iterator<int*>>());
    static_assert(test<int*>());
#endif

    return 0;
}
