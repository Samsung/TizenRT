//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <deque>

// deque& operator=(initializer_list<value_type> il);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_cons_op_equal_initializer_list(void) {
    {
    std::deque<int> d;
    d = {3, 4, 5, 6};
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(d[0] == 3);
    TC_ASSERT_EXPR(d[1] == 4);
    TC_ASSERT_EXPR(d[2] == 5);
    TC_ASSERT_EXPR(d[3] == 6);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }
    {
    std::deque<int, min_allocator<int>> d;
    d = {3, 4, 5, 6};
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(d[0] == 3);
    TC_ASSERT_EXPR(d[1] == 4);
    TC_ASSERT_EXPR(d[2] == 5);
    TC_ASSERT_EXPR(d[3] == 6);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }

  return 0;
}
