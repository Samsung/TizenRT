//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <deque>

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_modifiers_insert_iter_initializer_list(void) {
    {
    std::deque<int> d(10, 1);
    std::deque<int>::iterator i = d.insert(d.cbegin() + 2, {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == 1);
    TC_ASSERT_EXPR(d[1] == 1);
    TC_ASSERT_EXPR(d[2] == 3);
    TC_ASSERT_EXPR(d[3] == 4);
    TC_ASSERT_EXPR(d[4] == 5);
    TC_ASSERT_EXPR(d[5] == 6);
    TC_ASSERT_EXPR(d[6] == 1);
    TC_ASSERT_EXPR(d[7] == 1);
    TC_ASSERT_EXPR(d[8] == 1);
    TC_ASSERT_EXPR(d[9] == 1);
    TC_ASSERT_EXPR(d[10] == 1);
    TC_ASSERT_EXPR(d[11] == 1);
    TC_ASSERT_EXPR(d[12] == 1);
    TC_ASSERT_EXPR(d[13] == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }
    {
    std::deque<int, min_allocator<int>> d(10, 1);
    std::deque<int, min_allocator<int>>::iterator i = d.insert(d.cbegin() + 2, {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == 1);
    TC_ASSERT_EXPR(d[1] == 1);
    TC_ASSERT_EXPR(d[2] == 3);
    TC_ASSERT_EXPR(d[3] == 4);
    TC_ASSERT_EXPR(d[4] == 5);
    TC_ASSERT_EXPR(d[5] == 6);
    TC_ASSERT_EXPR(d[6] == 1);
    TC_ASSERT_EXPR(d[7] == 1);
    TC_ASSERT_EXPR(d[8] == 1);
    TC_ASSERT_EXPR(d[9] == 1);
    TC_ASSERT_EXPR(d[10] == 1);
    TC_ASSERT_EXPR(d[11] == 1);
    TC_ASSERT_EXPR(d[12] == 1);
    TC_ASSERT_EXPR(d[13] == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }

  return 0;
}
