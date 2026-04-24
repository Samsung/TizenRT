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
//   requires OutputIterator<Iter, Iter::reference>
//         && OutputIterator<Iter, const T&>
//         && HasEqualTo<Iter::value_type, T>
//   constexpr void      // constexpr after C++17
//   replace(Iter first, Iter last, const T& old_value, const T& new_value);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"


#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
          int ia[]       = {0, 1, 2, 3, 4};
    const int expected[] = {0, 1, 5, 3, 4};

    std::replace(std::begin(ia), std::end(ia), 2, 5);
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
    std::replace(Iter(ia), Iter(ia+sa), 2, 5);
    TC_ASSERT_EXPR(ia[0] == 0);
    TC_ASSERT_EXPR(ia[1] == 1);
    TC_ASSERT_EXPR(ia[2] == 5);
    TC_ASSERT_EXPR(ia[3] == 3);
    TC_ASSERT_EXPR(ia[4] == 4);
}

int tc_algorithms_alg_modifying_operations_alg_replace_replace(void) {
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
