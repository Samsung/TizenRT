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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set& operator=(const unordered_set& u);

#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstddef>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_unord_set_cnstr_assign_copy(void) {
    {
        typedef test_allocator<int> A;
        typedef std::unordered_set<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   A
                                   > C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            A(10)
           );
        C c(a, a + 2,
            7,
            test_hash<int>(2),
            test_equal_to<int>(3),
            A(4)
           );
        c = c0;
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == A(4));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        C *p = &c;
        c = *p;
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(std::is_permutation(c.begin(), c.end(), a));
    }
    {
        typedef other_allocator<int> A;
        typedef std::unordered_set<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   A
                                   > C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            A(10)
           );
        C c(a, a + 2,
            7,
            test_hash<int>(2),
            test_equal_to<int>(3),
            A(4)
           );
        c = c0;
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == A(10));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef min_allocator<int> A;
        typedef std::unordered_set<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   A
                                   > C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
        };
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            A()
           );
        C c(a, a + 2,
            7,
            test_hash<int>(2),
            test_equal_to<int>(3),
            A()
           );
        c = c0;
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == A());
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif

  return 0;
}
