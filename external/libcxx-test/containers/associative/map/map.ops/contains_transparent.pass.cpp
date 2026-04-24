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

#include <cassert>
#include <map>
#include "libcxx_tc_common.h"

// <map>

// template<class K> bool contains(const K& x) const; // C++20

struct Comp {
  using is_transparent = void;

  bool operator()(const std::pair<int, int>& lhs,
                  const std::pair<int, int>& rhs) const {
    return lhs < rhs;
  }

  bool operator()(const std::pair<int, int>& lhs, int rhs) const {
    return lhs.first < rhs;
  }

  bool operator()(int lhs, const std::pair<int, int>& rhs) const {
    return lhs < rhs.first;
  }
};

template <typename Container>
void test() {
  Container s{{{2, 1}, 1}, {{1, 2}, 2}, {{1, 3}, 3}, {{1, 4}, 4}, {{2, 2}, 5}};

  TC_ASSERT_EXPR(s.contains(1));
  TC_ASSERT_EXPR(!s.contains(-1));
}

int tc_containers_associative_map_map_ops_contains_transparent(void) {
  test<std::map<std::pair<int, int>, int, Comp> >();
  test<std::multimap<std::pair<int, int>, int, Comp> >();

  return 0;
}
