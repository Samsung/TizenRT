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
// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <map>

// namespace std::pmr {
// template <class K, class V, class Compare = less<Key> >
// using map =
//     ::std::map<K, V, Compare, polymorphic_allocator<pair<const K, V>>>
//
// template <class K, class V, class Compare = less<Key> >
// using multimap =
//     ::std::multimap<K, V, Compare, polymorphic_allocator<pair<const K, V>>>
//
// } // namespace std::pmr

#include <map>
#include <memory_resource>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_aliases_header_map_synop(void) {
  using K  = int;
  using V  = char;
  using DC = std::less<int>;
  using OC = std::greater<int>;
  using P  = std::pair<const K, V>;
  {
    using StdMap = std::map<K, V, DC, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::map<K, V>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::map<K, V, OC, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::map<K, V, OC>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    std::pmr::map<int, int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }
  {
    using StdMap = std::multimap<K, V, DC, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::multimap<K, V>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::multimap<K, V, OC, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::multimap<K, V, OC>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    std::pmr::multimap<int, int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }

  return 0;
}
