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
// UNSUPPORTED: c++03, c++11

// <map>

// class map

// template<typename K>
//         pair<iterator,iterator>             equal_range(const K& x); // C++14
// template<typename K>
//         pair<const_iterator,const_iterator> equal_range(const K& x) const;
//         // C++14

#include <cassert>
#include <map>
#include <utility>
#include "libcxx_tc_common.h"

struct Comp {
  using is_transparent = void;

  bool operator()(const std::pair<int, int> &lhs,
                  const std::pair<int, int> &rhs) const {
    return lhs < rhs;
  }

  bool operator()(const std::pair<int, int> &lhs, int rhs) const {
    return lhs.first < rhs;
  }

  bool operator()(int lhs, const std::pair<int, int> &rhs) const {
    return lhs < rhs.first;
  }
};

int tc_containers_associative_map_map_ops_equal_range_transparent(void) {
  std::map<std::pair<int, int>, int, Comp> s{
      {{2, 1}, 1}, {{1, 2}, 2}, {{1, 3}, 3}, {{1, 4}, 4}, {{2, 2}, 5}};

  auto er = s.equal_range(1);
  long nels = 0;

  for (auto it = er.first; it != er.second; it++) {
    TC_ASSERT_EXPR(it->first.first == 1);
    nels++;
  }

  TC_ASSERT_EXPR(nels == 3);

  return 0;
}
