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
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// <queue>

// template<class T, three_way_comparable Container>
//   compare_three_way_result_t<Container>
//     operator<=>(const queue<T, Container>& x, const queue<T, Container>& y);

#include <cassert>
#include <deque>
#include <queue>
#include <list>

#include "nasty_containers.h"
#include "test_container_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_ops_compare_three_way(void) {
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::queue, std::deque>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::queue, std::list>()));
  TC_ASSERT_EXPR((test_sequence_container_adaptor_spaceship<std::queue, nasty_list>()));
  // `std::queue` is not constexpr, so no `static_assert` test here.
  return 0;
}
