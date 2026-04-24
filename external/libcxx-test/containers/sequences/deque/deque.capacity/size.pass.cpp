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
// <deque>

// class deque

// size_type size() const noexcept;

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_capacity_size(void) {
    {
    typedef std::deque<int> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(2));
    TC_ASSERT_EXPR(c.size() == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(c.size() == 2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(3));
    TC_ASSERT_EXPR(c.size() == 3);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
#if TEST_STD_VER >= 11
    {
    typedef std::deque<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(2));
    TC_ASSERT_EXPR(c.size() == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(c.size() == 2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.push_back(C::value_type(3));
    TC_ASSERT_EXPR(c.size() == 3);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
    }
#endif

  return 0;
}
