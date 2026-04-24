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

// template<InputIterator Iter, Predicate<auto, Iter::value_type> Pred>
//   requires CopyConstructible<Pred>
//   constexpr Iter   // constexpr after C++17
//   find_if(Iter first, Iter last, Pred pred);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct eq {
    TEST_CONSTEXPR eq (int val) : v(val) {}
    TEST_CONSTEXPR bool operator () (int v2) const { return v == v2; }
    int v;
    };

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 4, 6};
    int ib[] = {1, 2, 3, 7, 5, 6};
    eq c(4);
    return    (std::find_if(std::begin(ia), std::end(ia), c) == ia+4)
           && (std::find_if(std::begin(ib), std::end(ib), c) == ib+6)
           ;
    }
#endif

int tc_algorithms_alg_nonmodifying_alg_find_find_if(void) {
    int ia[] = {0, 1, 2, 3, 4, 5};
    const unsigned s = sizeof(ia)/sizeof(ia[0]);
    cpp17_input_iterator<const int*> r = std::find_if(cpp17_input_iterator<const int*>(ia),
                                                cpp17_input_iterator<const int*>(ia+s),
                                                eq(3));
    TC_ASSERT_EXPR(*r == 3);
    r = std::find_if(cpp17_input_iterator<const int*>(ia),
                     cpp17_input_iterator<const int*>(ia+s),
                     eq(10));
    TC_ASSERT_EXPR(r == cpp17_input_iterator<const int*>(ia+s));

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
