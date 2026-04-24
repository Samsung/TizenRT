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

// template<class RandomAccessIterator, class UniformRandomNumberGenerator>
//     void shuffle(RandomAccessIterator first, RandomAccessIterator last,
//                  UniformRandomNumberGenerator& g);

#include <algorithm>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_algorithms_alg_modifying_operations_alg_random_shuffle_random_shuffle_urng(void) {
    int ia[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int ia1[] = {2, 7, 1, 4, 3, 6, 5, 10, 9, 8};
    int ia2[] = {1, 8, 3, 4, 6, 9, 5, 7, 2, 10};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    std::minstd_rand g;
    std::shuffle(ia, ia+sa, g);
    LIBCPP_ASSERT(std::equal(ia, ia+sa, ia1));
    TC_ASSERT_EXPR(std::is_permutation(ia, ia+sa, ia1));
    std::shuffle(ia, ia+sa, std::move(g));
    LIBCPP_ASSERT(std::equal(ia, ia+sa, ia2));
    TC_ASSERT_EXPR(std::is_permutation(ia, ia+sa, ia2));

  return 0;
}
