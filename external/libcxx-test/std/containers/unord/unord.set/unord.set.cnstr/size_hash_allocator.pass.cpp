//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set(size_type n, const hasher& hash, const allocator_type& alloc);

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
    C c(7, test_hash<NotConstructible>(5), alloc);
    LIBCPP_ASSERT(c.bucket_count() == 7);
    TC_ASSERT_EXPR(c.hash_function() == test_hash<NotConstructible>(5));
    TC_ASSERT_EXPR(c.key_eq() == test_equal_to<NotConstructible>());
    TC_ASSERT_EXPR(c.get_allocator() == alloc);
    TC_ASSERT_EXPR(c.size() == 0);
    TC_ASSERT_EXPR(c.empty());
    TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == 0);
    TC_ASSERT_EXPR(c.load_factor() == 0);
    TC_ASSERT_EXPR(c.max_load_factor() == 1);
}

int tc_containers_unord_unord_set_unord_set_cnstr_size_hash_allocator(void) {
    test(test_allocator<NotConstructible>(10));
    test(min_allocator<NotConstructible>());
    test(explicit_allocator<NotConstructible>());

    return 0;
}
