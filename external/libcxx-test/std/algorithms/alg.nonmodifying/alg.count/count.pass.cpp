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
 ****************************************************************************///
//
// <algorithm>

// template<InputIterator Iter, class T>
//   requires HasEqualTo<Iter::value_type, T>
//   constexpr Iter::difference_type   // constexpr after C++17
//   count(Iter first, Iter last, const T& value);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    int ib[] = {1, 2, 3, 4, 5, 6};
    return    (std::count(std::begin(ia), std::end(ia), 2) == 3)
           && (std::count(std::begin(ib), std::end(ib), 9) == 0)
           ;
    }
#endif

} // namespace

int tc_libcxx_algorithms_alg_nonmodifying_alg_count_count(void) {
    int ia[] = {0, 1, 2, 2, 0, 1, 2, 3};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    TC_ASSERT_EXPR(std::count(cpp17_input_iterator<const int*>(ia),
                      cpp17_input_iterator<const int*>(ia + sa), 2) == 3);
    TC_ASSERT_EXPR(std::count(cpp17_input_iterator<const int*>(ia),
                      cpp17_input_iterator<const int*>(ia + sa), 7) == 0);
    TC_ASSERT_EXPR(std::count(cpp17_input_iterator<const int*>(ia),
                      cpp17_input_iterator<const int*>(ia), 2) == 0);

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
