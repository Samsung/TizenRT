//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// explicit unordered_set(const allocator_type& __a);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "../../../NotConstructible.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_unord_set_cnstr_allocator(void) {
    {
        typedef std::unordered_set<NotConstructible,
                                   test_hash<NotConstructible>,
                                   test_equal_to<NotConstructible>,
                                   test_allocator<NotConstructible>
                                   > C;
        C c(test_allocator<NotConstructible>(10));
        LIBCPP_ASSERT(c.bucket_count() == 0);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>());
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>());
        TC_ASSERT_EXPR(c.get_allocator() == test_allocator<NotConstructible>(10));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<NotConstructible,
                                   test_hash<NotConstructible>,
                                   test_equal_to<NotConstructible>,
                                   min_allocator<NotConstructible>
                                   > C;
        C c(min_allocator<NotConstructible>{});
        LIBCPP_ASSERT(c.bucket_count() == 0);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>());
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>());
        TC_ASSERT_EXPR(c.get_allocator() == min_allocator<NotConstructible>());
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER > 11
    {
        typedef NotConstructible T;
        typedef test_hash<T> HF;
        typedef test_equal_to<T> Comp;
        typedef test_allocator<T> A;
        typedef std::unordered_set<T, HF, Comp, A> C;

        A a(43);
        C c(3, a);
        LIBCPP_ASSERT(c.bucket_count() == 3);
        TC_ASSERT_EXPR(c.hash_function() == HF());
        TC_ASSERT_EXPR(c.key_eq() == Comp ());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!(c.get_allocator() == A()));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef NotConstructible T;
        typedef test_hash<T> HF;
        typedef test_equal_to<T> Comp;
        typedef test_allocator<T> A;
        typedef std::unordered_set<T, HF, Comp, A> C;

        HF hf(42);
        A a(43);
        C c(4, hf, a);
        LIBCPP_ASSERT(c.bucket_count() == 4);
        TC_ASSERT_EXPR(c.hash_function() == hf);
        TC_ASSERT_EXPR(!(c.hash_function() == HF()));
        TC_ASSERT_EXPR(c.key_eq() == Comp ());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!(c.get_allocator() == A()));
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.empty());
        TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
        TC_ASSERT_EXPR(c.load_factor() == 0);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif
#endif

  return 0;
}
