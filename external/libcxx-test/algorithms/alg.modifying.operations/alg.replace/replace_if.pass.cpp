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

// template<ForwardIterator Iter, Predicate<auto, Iter::value_type> Pred, class T>
//   requires OutputIterator<Iter, Iter::reference>
//         && OutputIterator<Iter, const T&>
//         && CopyConstructible<Pred>
//   constexpr void      // constexpr after C++17
//   replace_if(Iter first, Iter last, Pred pred, const T& new_value);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR bool equalToTwo(int v) { return v == 2; }

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
          int ia[]       = {0, 1, 2, 3, 4};
    const int expected[] = {0, 1, 5, 3, 4};

    std::replace_if(std::begin(ia), std::end(ia), equalToTwo, 5);
    return std::equal(std::begin(ia), std::end(ia), std::begin(expected), std::end(expected))
        ;
    }
#endif


template <class Iter>
void
test()
{
    int ia[] = {0, 1, 2, 3, 4};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    std::replace_if(Iter(ia), Iter(ia+sa), equalToTwo, 5);
    TC_ASSERT_EXPR(ia[0] == 0);
    TC_ASSERT_EXPR(ia[1] == 1);
    TC_ASSERT_EXPR(ia[2] == 5);
    TC_ASSERT_EXPR(ia[3] == 3);
    TC_ASSERT_EXPR(ia[4] == 4);
}

int tc_algorithms_alg_modifying_operations_alg_replace_replace_if(void) {
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
