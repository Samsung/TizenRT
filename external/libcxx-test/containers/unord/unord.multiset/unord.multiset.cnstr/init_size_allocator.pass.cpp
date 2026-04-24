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
// UNSUPPORTED: c++03, c++11

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// unordered_multiset(initializer_list<value_type> init, size_type n, const allocator_type& alloc);

#include <unordered_set>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstddef>
#include <iterator>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class Allocator>
void test(const Allocator& alloc)
{
    typedef std::unordered_multiset<int,
                                    test_hash<int>,
                                    test_equal_to<int>,
                                    Allocator> C;
    C c({
            1,
            2,
            3,
            4,
            1,
            2
        },
        7,
        alloc);

    LIBCPP_ASSERT(c.bucket_count() == 7);
    TC_ASSERT_EXPR(c.size() == 6);
    TC_ASSERT_EXPR(c.count(1) == 2);
    TC_ASSERT_EXPR(c.count(2) == 2);
    TC_ASSERT_EXPR(c.count(3) == 1);
    TC_ASSERT_EXPR(c.count(4) == 1);
    TC_ASSERT_EXPR(c.hash_function() == test_hash<int>());
    TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>());
    TC_ASSERT_EXPR(c.get_allocator() == alloc);
    TC_ASSERT_EXPR(!c.empty());
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
    TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
    TC_ASSERT_EXPR(c.max_load_factor() == 1);
}

int tc_containers_unord_unord_multiset_unord_multiset_cnstr_init_size_allocator(void) {
    test(test_allocator<int>(10));
    test(min_allocator<int>());
    test(explicit_allocator<int>());

    return 0;
}
