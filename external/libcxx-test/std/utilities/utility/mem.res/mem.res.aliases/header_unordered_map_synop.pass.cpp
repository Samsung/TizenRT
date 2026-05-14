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

// <unordered_map>

// namespace std::pmr {
// template <class K, class V, class H = hash<K>, class P = equal_to<K> >
// using unordered_map =
//     ::std::unordered_map<K, V, H, P, polymorphic_allocator<pair<const K, V>>>
//
// template <class K, class V,  class H = hash<K>, class P = equal_to<K> >
// using unordered_multimap =
//     ::std::unordered_multimap<K, V, H, P, polymorphic_allocator<pair<const K, V>>>
//
// } // namespace std::pmr

#include <unordered_map>
#include <memory_resource>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

template <class T>
struct MyHash : std::hash<T> {};

template <class T>
struct MyPred : std::equal_to<T> {};

int tc_utilities_utility_mem_res_mem_res_aliases_header_unordered_map_synop(void) {
  using K  = int;
  using V  = char;
  using DH = std::hash<K>;
  using MH = MyHash<K>;
  using DP = std::equal_to<K>;
  using MP = MyPred<K>;
  using P  = std::pair<const K, V>;
  {
    using StdMap = std::unordered_map<K, V, DH, DP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_map<K, V>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::unordered_map<K, V, MH, DP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_map<K, V, MH>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::unordered_map<K, V, MH, MP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_map<K, V, MH, MP>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    std::pmr::unordered_map<int, int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }
  {
    using StdMap = std::unordered_multimap<K, V, DH, DP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_multimap<K, V>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::unordered_multimap<K, V, MH, DP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_multimap<K, V, MH>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    using StdMap = std::unordered_multimap<K, V, MH, MP, std::pmr::polymorphic_allocator<P>>;
    using PmrMap = std::pmr::unordered_multimap<K, V, MH, MP>;
    static_assert(std::is_same<StdMap, PmrMap>::value, "");
  }
  {
    std::pmr::unordered_multimap<int, int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }

  return 0;
}
