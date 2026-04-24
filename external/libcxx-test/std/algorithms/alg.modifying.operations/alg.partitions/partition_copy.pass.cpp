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

// template <class InputIterator, class OutputIterator1,
//           class OutputIterator2, class Predicate>
//     constexpr pair<OutputIterator1, OutputIterator2>     // constexpr after C++17
//     partition_copy(InputIterator first, InputIterator last,
//                    OutputIterator1 out_true, OutputIterator2 out_false,
//                    Predicate pred);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
struct is_odd
{
    TEST_CONSTEXPR bool operator()(const int& i) const {return i & 1;}
};

#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 4, 6};
    int r1[10] = {0};
    int r2[10] = {0};

    auto p = std::partition_copy(std::begin(ia), std::end(ia),
                    std::begin(r1), std::begin(r2), is_odd());

    return std::all_of(std::begin(r1), p.first, is_odd())
        && std::all_of(p.first, std::end(r1), [](int a){return a == 0;})
        && std::none_of(std::begin(r2), p.second, is_odd())
        && std::all_of(p.second, std::end(r2), [](int a){return a == 0;})
           ;
    }
#endif

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_partition_copy(void) {
    {
        const int ia[] = {1, 2, 3, 4, 6, 8, 5, 7};
        int r1[10] = {0};
        int r2[10] = {0};
        typedef std::pair<cpp17_output_iterator<int*>,  int*> P;
        P p = std::partition_copy(cpp17_input_iterator<const int*>(std::begin(ia)),
                                  cpp17_input_iterator<const int*>(std::end(ia)),
                                  cpp17_output_iterator<int*>(r1), r2, is_odd());
        TC_ASSERT_EXPR(base(p.first) == r1 + 4);
        TC_ASSERT_EXPR(r1[0] == 1);
        TC_ASSERT_EXPR(r1[1] == 3);
        TC_ASSERT_EXPR(r1[2] == 5);
        TC_ASSERT_EXPR(r1[3] == 7);
        TC_ASSERT_EXPR(p.second == r2 + 4);
        TC_ASSERT_EXPR(r2[0] == 2);
        TC_ASSERT_EXPR(r2[1] == 4);
        TC_ASSERT_EXPR(r2[2] == 6);
        TC_ASSERT_EXPR(r2[3] == 8);
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
