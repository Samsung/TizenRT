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

// template <class InputIterator, class Predicate>
//     constexpr bool       // constexpr after C++17
//   any_of(InputIterator first, InputIterator last, Predicate pred);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct test1
{
    TEST_CONSTEXPR bool operator()(const int& i) const
    {
        return i % 2 == 0;
    }
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {2, 4, 6, 8};
    int ib[] = {1, 3, 5, 7};
    return  std::any_of(std::begin(ia), std::end(ia), test1())
        && !std::any_of(std::begin(ib), std::end(ib), test1())
        ;
    }
#endif

int tc_algorithms_alg_nonmodifying_alg_any_of_any_of(void) {
    {
        int ia[] = {2, 4, 6, 8};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == true);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }
    {
        const int ia[] = {2, 4, 5, 8};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == true);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }
    {
        const int ia[] = {1, 3, 5, 7};
        const unsigned sa = sizeof(ia)/sizeof(ia[0]);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia + sa), test1()) == false);
        TC_ASSERT_EXPR(std::any_of(cpp17_input_iterator<const int*>(ia),
                           cpp17_input_iterator<const int*>(ia), test1()) == false);
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
