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
//     is_partitioned(InputIterator first, InputIterator last, Predicate pred);

#include <algorithm>
#include <functional>
#include <cstddef>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "counting_predicates.h"
#include "libcxx_tc_common.h"

struct is_odd {
  TEST_CONSTEXPR bool operator()(const int &i) const { return i & 1; }
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 4, 6};
    int ib[] = {1, 2, 3, 4, 5, 6};
    return     std::is_partitioned(std::begin(ia), std::end(ia), is_odd())
           && !std::is_partitioned(std::begin(ib), std::end(ib), is_odd());
    }
#endif


int tc_algorithms_alg_modifying_operations_alg_partitions_is_partitioned(void) {
  {
    const int ia[] = {1, 2, 3, 4, 5, 6};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::end(ia)),
                               std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {2, 4, 6, 1, 3, 5};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6, 7};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(!std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                                cpp17_input_iterator<const int *>(std::end(ia)),
                                std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 2, 4, 6, 7};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::begin(ia)),
                               std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::begin(ia)));
  }
  {
    const int ia[] = {1, 3, 5, 7, 9, 11, 2};
    unary_counting_predicate<is_odd, int> pred((is_odd()));
    TC_ASSERT_EXPR(std::is_partitioned(cpp17_input_iterator<const int *>(std::begin(ia)),
                               cpp17_input_iterator<const int *>(std::end(ia)),
                               std::ref(pred)));
    TC_ASSERT_EXPR(static_cast<std::ptrdiff_t>(pred.count()) <=
           std::distance(std::begin(ia), std::end(ia)));
  }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
