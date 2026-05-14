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
// UNSUPPORTED: c++03

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set(initializer_list<value_type> il);

#include <unordered_set>
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

int tc_containers_unord_unord_set_unord_set_cnstr_init(void) {
  {
        typedef std::unordered_set<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   test_allocator<int>
                                   > C;
        typedef int P;
        C c = {
                P(1),
                P(2),
                P(3),
                P(4),
                P(1),
                P(2)
            };
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>());
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>());
        TC_ASSERT_EXPR(c.get_allocator() == test_allocator<int>());
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::unordered_set<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   min_allocator<int>
                                   > C;
        typedef int P;
        C c = {
                P(1),
                P(2),
                P(3),
                P(4),
                P(1),
                P(2)
            };
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>());
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>());
        TC_ASSERT_EXPR(c.get_allocator() == min_allocator<int>());
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER > 11
    {
        typedef int T;
        typedef test_hash<T> HF;
        typedef test_equal_to<T> Comp;
        typedef test_allocator<T> A;
        typedef std::unordered_set<T, HF, Comp, A> C;

        A a(42);
        C c({
                T(1),
                T(2),
                T(3),
                T(4),
                T(1),
                T(2)
            }, 12, a);

        TC_ASSERT_EXPR(c.bucket_count() >= 12);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == HF());
        TC_ASSERT_EXPR(c.key_eq() == Comp());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!(c.get_allocator() == A()));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef int T;
        typedef test_hash<T> HF;
        typedef test_equal_to<T> Comp;
        typedef test_allocator<T> A;
        typedef std::unordered_set<T, HF, Comp, A> C;

        A a(42);
        HF hf(43);
        C c({
                T(1),
                T(2),
                T(3),
                T(4),
                T(1),
                T(2)
            }, 12, hf, a);

        TC_ASSERT_EXPR(c.bucket_count() >= 12);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == hf);
        TC_ASSERT_EXPR(!(c.hash_function() == HF()));
        TC_ASSERT_EXPR(c.key_eq() == Comp());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!(c.get_allocator() == A()));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif

  return 0;
}
