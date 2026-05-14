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

// <deque>

// deque(initializer_list<value_type> il);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_cons_initializer_list(void) {
    {
    std::deque<int> d = {3, 4, 5, 6};
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(d[0] == 3);
    TC_ASSERT_EXPR(d[1] == 4);
    TC_ASSERT_EXPR(d[2] == 5);
    TC_ASSERT_EXPR(d[3] == 6);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }
    {
    std::deque<int, min_allocator<int>> d = {3, 4, 5, 6};
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(d[0] == 3);
    TC_ASSERT_EXPR(d[1] == 4);
    TC_ASSERT_EXPR(d[2] == 5);
    TC_ASSERT_EXPR(d[3] == 6);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(d));
    }

  return 0;
}
