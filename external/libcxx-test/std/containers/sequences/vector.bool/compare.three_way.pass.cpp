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

// <vector>

// class vector<bool, Allocator>

// template<class T, class Allocator>
//   constexpr synth-three-way-result<T> operator<=>(const vector<T, Allocator>& x,
//                                                   const vector<T, Allocator>& y);

#include <cassert>
#include <vector>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

constexpr bool test_sequence_container_spaceship_vectorbool() {
  // Empty containers
  {
    std::vector<bool> l1;
    std::vector<bool> l2;
    TC_ASSERT_EXPR(testOrder(l1, l2, std::strong_ordering::equivalent));
  }
  // Identical contents
  {
    std::vector<bool> t1{true, true};
    std::vector<bool> t2{true, true};
    TC_ASSERT_EXPR(testOrder(t1, t2, std::strong_ordering::equivalent));

    std::vector<bool> f1{false, false};
    std::vector<bool> f2{false, false};
    TC_ASSERT_EXPR(testOrder(f1, f2, std::strong_ordering::equivalent));
  }
  // Less, due to contained values
  {
    std::vector<bool> l1{true, false};
    std::vector<bool> l2{true, true};
    TC_ASSERT_EXPR(testOrder(l1, l2, std::strong_ordering::less));
  }
  // Greater, due to contained values
  {
    std::vector<bool> l1{true, true};
    std::vector<bool> l2{true, false};
    TC_ASSERT_EXPR(testOrder(l1, l2, std::strong_ordering::greater));
  }
  // Shorter list
  {
    std::vector<bool> l1{true};
    std::vector<bool> l2{true, false};
    TC_ASSERT_EXPR(testOrder(l1, l2, std::strong_ordering::less));

    std::vector<bool> t1{true};
    std::vector<bool> t2{true, true};
    TC_ASSERT_EXPR(testOrder(t1, t2, std::strong_ordering::less));

    std::vector<bool> f1{false};
    std::vector<bool> f2{false, false};
    TC_ASSERT_EXPR(testOrder(f1, f2, std::strong_ordering::less));

    std::vector<bool> e;
    TC_ASSERT_EXPR(testOrder(e, t1, std::strong_ordering::less));
    TC_ASSERT_EXPR(testOrder(e, f1, std::strong_ordering::less));
  }
  // Longer list
  {
    std::vector<bool> l1{true, false};
    std::vector<bool> l2{true};
    TC_ASSERT_EXPR(testOrder(l1, l2, std::strong_ordering::greater));

    std::vector<bool> t1{true, true};
    std::vector<bool> t2{true};
    TC_ASSERT_EXPR(testOrder(t1, t2, std::strong_ordering::greater));

    std::vector<bool> f1{false, false};
    std::vector<bool> f2{false};
    TC_ASSERT_EXPR(testOrder(f1, f2, std::strong_ordering::greater));

    std::vector<bool> e;
    TC_ASSERT_EXPR(testOrder(t2, e, std::strong_ordering::greater));
    TC_ASSERT_EXPR(testOrder(f2, e, std::strong_ordering::greater));
  }

  return true;
}

int tc_containers_sequences_vector_bool_compare_three_way(void) {
  TC_ASSERT_EXPR(test_sequence_container_spaceship_vectorbool());
  static_assert(test_sequence_container_spaceship_vectorbool());
  return 0;
}
