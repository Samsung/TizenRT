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

// template<RandomAccessIterator Iter, Callable<auto, Iter::difference_type> Rand>
//   requires ShuffleIterator<Iter>
//         && Convertible<Rand::result_type, Iter::difference_type>
//   void
//   random_shuffle(Iter first, Iter last, Rand&& rand);

// REQUIRES: c++03 || c++11 || c++14
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <algorithm>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"


struct gen
{
    std::ptrdiff_t operator()(std::ptrdiff_t n)
    {
        return n-1;
    }
};


template <class Iter>
void
test_with_iterator()
{

    int ia[] = {1, 2, 3, 4};
    int ia1[] = {4, 1, 2, 3};
    const unsigned sa = sizeof(ia)/sizeof(ia[0]);
    gen r;

    std::random_shuffle(ia, ia+sa, r);
    LIBCPP_ASSERT(std::equal(ia, ia+sa, ia1));
    TC_ASSERT_EXPR(std::is_permutation(ia, ia+sa, ia1));

    std::random_shuffle(ia, ia+sa, r);
    TC_ASSERT_EXPR(std::is_permutation(ia, ia+sa, ia1));
}


int tc_algorithms_alg_modifying_operations_alg_random_shuffle_random_shuffle_rand(void) {
    test_with_iterator<random_access_iterator<int*> >();
    test_with_iterator<int*>();
    return 0;
}
