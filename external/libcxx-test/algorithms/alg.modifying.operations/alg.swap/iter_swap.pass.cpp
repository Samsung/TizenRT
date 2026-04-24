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

// template<Iterator Iter1, Iterator Iter2>
//   requires HasSwap<Iter1::reference, Iter2::reference>
//   void
//   iter_swap(Iter1 a, Iter2 b);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 17
constexpr bool test_swap_constexpr()
{
    int i = 1;
    int j = 2;
    std::iter_swap(&i, &j);
    return i == 2 && j == 1;
}
#endif // TEST_STD_VER > 17

int tc_algorithms_alg_modifying_operations_alg_swap_iter_swap(void) {
    int i = 1;
    int j = 2;
    std::iter_swap(&i, &j);
    TC_ASSERT_EXPR(i == 2);
    TC_ASSERT_EXPR(j == 1);

#if TEST_STD_VER > 17
    static_assert(test_swap_constexpr());
#endif // TEST_STD_VER > 17

  return 0;
}
