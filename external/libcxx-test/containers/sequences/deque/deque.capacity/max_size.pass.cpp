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

// size_type max_size() const;

#include "asan_testing.h"
#include <cassert>
#include <deque>
#include <limits>
#include <type_traits>

#include "test_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_deque_deque_capacity_max_size(void) {
  {
    typedef limited_allocator<int, 10> A;
    typedef std::deque<int, A> C;
    C c;
    TC_ASSERT_EXPR(c.max_size() <= 10);
    LIBCPP_ASSERT(c.max_size() == 10);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
  }
  {
    typedef limited_allocator<int, (std::size_t)-1> A;
    typedef std::deque<int, A> C;
    const C::size_type max_dist =
        static_cast<C::size_type>(std::numeric_limits<C::difference_type>::max());
    C c;
    TC_ASSERT_EXPR(c.max_size() <= max_dist);
    LIBCPP_ASSERT(c.max_size() == max_dist);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
  }
  {
    typedef std::deque<char> C;
    const C::size_type max_dist =
        static_cast<C::size_type>(std::numeric_limits<C::difference_type>::max());
    C c;
    TC_ASSERT_EXPR(c.max_size() <= max_dist);
    TC_ASSERT_EXPR(c.max_size() <= alloc_max_size(c.get_allocator()));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c));
  }

  return 0;
}
