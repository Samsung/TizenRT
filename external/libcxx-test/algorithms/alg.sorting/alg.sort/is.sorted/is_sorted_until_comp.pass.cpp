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

// template<ForwardIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires CopyConstructible<Compare>
//   Iter
//   is_sorted_until(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 0, 1};
    int ib[] = {1, 1, 0};
    return    (std::is_sorted_until(std::begin(ia), std::end(ia), std::greater<int>()) == ia+2)
           && (std::is_sorted_until(std::begin(ib), std::end(ib), std::greater<int>()) == ib+3);
    }
#endif

template <class Iter>
void
test()
{
    {
    int a[] = {0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a), std::greater<int>()) == Iter(a));
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }

    {
    int a[] = {0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }

    {
    int a[] = {0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }

    {
    int a[] = {0, 0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {0, 0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+3));
    }
    {
    int a[] = {0, 0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {0, 0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {0, 1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {0, 1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {0, 1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {0, 1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+1));
    }
    {
    int a[] = {1, 0, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 0, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+3));
    }
    {
    int a[] = {1, 0, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {1, 0, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+2));
    }
    {
    int a[] = {1, 1, 0, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 1, 0, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+3));
    }
    {
    int a[] = {1, 1, 1, 0};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
    {
    int a[] = {1, 1, 1, 1};
    unsigned sa = sizeof(a) / sizeof(a[0]);
    TC_ASSERT_EXPR(std::is_sorted_until(Iter(a), Iter(a+sa), std::greater<int>()) == Iter(a+sa));
    }
}

int tc_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_until_comp(void) {
    test<forward_iterator<const int*> >();
    test<bidirectional_iterator<const int*> >();
    test<random_access_iterator<const int*> >();
    test<const int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
