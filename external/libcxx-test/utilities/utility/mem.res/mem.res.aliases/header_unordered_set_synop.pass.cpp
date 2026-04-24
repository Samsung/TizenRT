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

// <unordered_set>

// namespace std::pmr {
// template <class V, class H = hash<V>, class P = equal_to<V> >
// using unordered_set =
//     ::std::unordered_set<V, H, P, polymorphic_allocator<V>>
//
// template <class V,  class H = hash<V>, class P = equal_to<V> >
// using unordered_multiset =
//     ::std::unordered_multiset<V, H, P, polymorphic_allocator<V>>
//
// } // namespace std::pmr

#include <unordered_set>
#include <memory_resource>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

template <class T>
struct MyHash : std::hash<T> {};

template <class T>
struct MyPred : std::equal_to<T> {};

int tc_utilities_utility_mem_res_mem_res_aliases_header_unordered_set_synop(void) {
  using V  = char;
  using DH = std::hash<V>;
  using MH = MyHash<V>;
  using DP = std::equal_to<V>;
  using MP = MyPred<V>;
  {
    using StdSet = std::unordered_set<V, DH, DP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_set<V>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::unordered_set<V, MH, DP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_set<V, MH>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::unordered_set<V, MH, MP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_set<V, MH, MP>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    std::pmr::unordered_set<int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }
  {
    using StdSet = std::unordered_multiset<V, DH, DP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_multiset<V>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::unordered_multiset<V, MH, DP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_multiset<V, MH>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    using StdSet = std::unordered_multiset<V, MH, MP, std::pmr::polymorphic_allocator<V>>;
    using PmrSet = std::pmr::unordered_multiset<V, MH, MP>;
    static_assert(std::is_same<StdSet, PmrSet>::value, "");
  }
  {
    std::pmr::unordered_multiset<int> m;
    TC_ASSERT_EXPR(m.get_allocator().resource() == std::pmr::get_default_resource());
  }

  return 0;
}
