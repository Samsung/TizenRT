//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// unordered_multiset(unordered_multiset&& u);

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

int tc_containers_unord_unord_multiset_unord_multiset_cnstr_move(void) {
    {
        typedef std::unordered_multiset<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   test_allocator<int>
                                   > C;
        C c0(7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            test_allocator<int>(10)
           );
        C c = std::move(c0);
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == test_allocator<int>(10));
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);

        TC_ASSERT_EXPR(c0.empty());
    }
    {
        typedef std::unordered_multiset<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   test_allocator<int>
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
            test_allocator<int>(10)
           );
        C::iterator it0 = c0.begin();
        C c = std::move(c0);
        TC_ASSERT_EXPR(it0 == c.begin()); // Iterators remain valid
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(c.count(1) == 2);
        TC_ASSERT_EXPR(c.count(2) == 2);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == test_allocator<int>(10));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);

        TC_ASSERT_EXPR(c0.empty());
    }
    {
        typedef std::unordered_multiset<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   min_allocator<int>
                                   > C;
        C c0(7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            min_allocator<int>()
           );
        C c = std::move(c0);
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == min_allocator<int>());
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);

        TC_ASSERT_EXPR(c0.empty());
    }
    {
        typedef std::unordered_multiset<int,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   min_allocator<int>
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
            min_allocator<int>()
           );
        C::iterator it0 = c0.begin();
        C c = std::move(c0);
        TC_ASSERT_EXPR(it0 == c.begin()); // Iterators remain valid
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(c.count(1) == 2);
        TC_ASSERT_EXPR(c.count(2) == 2);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == min_allocator<int>());
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);

        TC_ASSERT_EXPR(c0.empty());
    }

    return 0;
}
