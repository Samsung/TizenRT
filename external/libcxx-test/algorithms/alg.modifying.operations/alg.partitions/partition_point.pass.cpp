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

// template<class ForwardIterator, class Predicate>
//     constexpr ForwardIterator       // constexpr after C++17
//     partition_point(ForwardIterator first, ForwardIterator last, Predicate pred);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

struct is_odd
{
    TEST_CONSTEXPR bool operator()(const int& i) const {return i & 1;}
};


#if TEST_STD_VER > 17
TEST_CONSTEXPR bool test_constexpr() {
    int ia[] = {1, 3, 5, 2, 4, 6};
    int ib[] = {1, 2, 3, 4, 5, 6};
    return    (std::partition_point(std::begin(ia), std::end(ia), is_odd()) == ia+3)
           && (std::partition_point(std::begin(ib), std::end(ib), is_odd()) == ib+1)
           ;
    }
#endif


int tc_algorithms_alg_modifying_operations_alg_partitions_partition_point(void) {
    {
        const int ia[] = {2, 4, 6, 8, 10};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia));
    }
    {
        const int ia[] = {1, 2, 4, 6, 8};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 1));
    }
    {
        const int ia[] = {1, 3, 2, 4, 6};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 2));
    }
    {
        const int ia[] = {1, 3, 5, 2, 4, 6};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 3));
    }
    {
        const int ia[] = {1, 3, 5, 7, 2, 4};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 4));
    }
    {
        const int ia[] = {1, 3, 5, 7, 9, 2};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 5));
    }
    {
        const int ia[] = {1, 3, 5, 7, 9, 11};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::end(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia + 6));
    }
    {
        const int ia[] = {1, 3, 5, 2, 4, 6, 7};
        TC_ASSERT_EXPR(std::partition_point(forward_iterator<const int*>(std::begin(ia)),
                                    forward_iterator<const int*>(std::begin(ia)),
                                    is_odd()) == forward_iterator<const int*>(ia));
    }

#if TEST_STD_VER > 17
    static_assert(test_constexpr());
#endif

  return 0;
}
