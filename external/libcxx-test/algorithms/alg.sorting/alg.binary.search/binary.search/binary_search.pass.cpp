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

// template<ForwardIterator Iter, class T>
//   constexpr bool      // constexpr after C++17
//   binary_search(Iter first, Iter last, const T& value);

#include <algorithm>
#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool lt(int a, int b) { return a < b; }

TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 3, 6, 7};

    return  std::binary_search(std::begin(ia), std::end(ia), 1)
        &&  std::binary_search(std::begin(ia), std::end(ia), 3)
        && !std::binary_search(std::begin(ia), std::end(ia), 9)
        ;
    }
#endif

template <class Iter, class T>
void
test(Iter first, Iter last, const T& value, bool x)
{
    TC_ASSERT_EXPR(std::binary_search(first, last, value) == x);
}

template <class Iter>
void
test()
{
    const unsigned N = 1000;
    const int M = 10;
    std::vector<int> v(N);
    int x = 0;
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = x;
        if (++x == M)
            x = 0;
    }
    std::sort(v.begin(), v.end());
    for (x = 0; x < M; ++x)
        test(Iter(v.data()), Iter(v.data()+v.size()), x, true);
    test(Iter(v.data()), Iter(v.data()+v.size()), -1, false);
    test(Iter(v.data()), Iter(v.data()+v.size()), M, false);
}

int tc_algorithms_alg_sorting_alg_binary_search_binary_search_binary_search(void) {
    int d[] = {0, 2, 4, 6};
    for (int* e = d; e <= d+4; ++e)
        for (int x = -1; x <= 7; ++x)
            test(d, e, x, (x % 2 == 0) && ((e-d)*2 > x));

    test<forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*> >();
    test<const int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
