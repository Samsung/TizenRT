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
// class unordered_set

// unordered_set(size_type n, const allocator_type& alloc);

#include <unordered_set>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "../../../NotConstructible.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class Allocator>
void test(const Allocator& alloc)
{
    typedef std::unordered_set<NotConstructible,
                               test_hash<NotConstructible>,
                               test_equal_to<NotConstructible>,
                               Allocator> C;
    C c(7, alloc);
    LIBCPP_ASSERT(c.bucket_count() == 7);
    TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>());
    TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>());
    TC_ASSERT_EXPR(c.get_allocator() == alloc);
    TC_ASSERT_EXPR(c.size() == 0);
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
    TC_ASSERT_EXPR(c.load_factor() == 0);
    TC_ASSERT_EXPR(c.max_load_factor() == 1);
}

int tc_containers_unord_unord_set_unord_set_cnstr_size_allocator(void) {
    test(test_allocator<NotConstructible>(10));
    test(min_allocator<NotConstructible>());
    test(explicit_allocator<NotConstructible>());

    return 0;
}
