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

// template<InputIterator InIter1, InputIterator InIter2, typename OutIter,
//          CopyConstructible Compare>
//   requires OutputIterator<OutIter, InIter1::reference>
//         && OutputIterator<OutIter, InIter2::reference>
//         && Predicate<Compare, InIter1::value_type, InIter2::value_type>
//         && Predicate<Compare, InIter2::value_type, InIter1::value_type>
//   OutIter
//   set_union(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2,
//             OutIter result, Compare comp);

// UNSUPPORTED: c++03

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"


int tc_algorithms_alg_sorting_alg_set_operations_set_union_set_union_move(void) {
    std::vector<MoveOnly> lhs, rhs;
    lhs.push_back(MoveOnly(2));
    rhs.push_back(MoveOnly(2));

    std::vector<MoveOnly> res;
    std::set_union(std::make_move_iterator(lhs.begin()),
                   std::make_move_iterator(lhs.end()),
                   std::make_move_iterator(rhs.begin()),
                   std::make_move_iterator(rhs.end()), std::back_inserter(res));

    TC_ASSERT_EXPR(res.size() == 1);
    TC_ASSERT_EXPR(res[0].get() == 2);

  return 0;
}
